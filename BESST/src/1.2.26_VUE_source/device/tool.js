import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'

let connBus;
let bus;
let pendingProcessData = '';

const CMD_GET_SN = 'AA060302010D0B';
const CMD_GET_HARDWARE_VERSION = 'AA040101010D0B';
const CMD_GET_SOFTWARE_VERSION = 'AA050101010D0B';

function connect(comName) {
    connBus = Port.connect(comName, 57600);
    connBus.subscribe(item => {
        switch (item.type) {
            case 'data':
                {
                    pendingProcessData += item.data;
                    parse();
                }
                break;

            default:
                {
                    bus.next(item);
                }
        }
    });

    bus = new Rx.Subject();
    return bus;
}

function parse() {
    let data = pendingProcessData;

    if (data.indexOf('AA') < 0) {
        return;
    }

    while (data.length > 0) {
        let length = parseInt(data.substring(4, 6), 16);

        let calculatedChecksum = Util.calculateChecksumFF(data.substring(4, 2 * length + 6));
        let checkSumPart = parseInt(data.substring(2 * length + 6, 2 * length + 8), 16);
        if (calculatedChecksum === checkSumPart) {
            let frame = data.substring(0, 2 * length + 12);
            Log.d(`parse out frame: ${frame}`);
            process(frame);
            pendingProcessData = pendingProcessData.substring(2 * length + 12);
            data = data.substring(2 * length + 12);
            continue;
        }

        data = data.substring(2);
    }
}

function process(frame) {
    Port.ack();

    let wrapper = {};

    let functionCode = parseInt(frame.substring(2, 4), 16);
    let length = parseInt(frame.substring(4, 6), 16);
    let data = Util.hexToString(frame.substring(6, 6 + length * 2));

    let tool = {};

    switch (functionCode) {
        case 0x04:
            {
                tool['hardware_version'] = data;
            }
            break;

        case 0x05:
            {
                tool['version'] = data;
            }
            break;

        case 0x06:
        case 0x07:
            {
                tool['sn'] = data;
            }
    }

    wrapper['tool'] = tool;
    bus.next({ type: 'data', data: wrapper });
}

function getBus() {
    if (bus == null) {
        bus = new Rx.Subject();
    }
    return bus;
}

function makeFrame(rawdata) {
    var hex = Util.stringToHex(rawdata);
    var buf = Buffer(hex, 'hex');
    var checkSum = 0;
    for (var j = 0; j < buf.length; j++) {
        checkSum = checkSum + buf[j];
    }
    checkSum += buf.length;
    checkSum = checkSum & 0xff;
    return 'AA07' + hex + Util.sprintf('%02X', hex.length / 2) + Util.sprintf('%02X', checkSum) + '0D0B';
}

function sendCmdGetFullData() {
    let cmdList = [
        CMD_GET_SN,
        CMD_GET_HARDWARE_VERSION,
        CMD_GET_SOFTWARE_VERSION
    ];

    cmdList.forEach((item) => {
        Port.writeByQueueAndWait(item, 200);
    });

    return getBus();
}


function sendCmdGetTestData() {
    let cmdList = [
        CMD_GET_SN,
        CMD_GET_HARDWARE_VERSION,
        CMD_GET_SOFTWARE_VERSION
    ];

    cmdList.forEach((item) => {
        Port.writeByQueueAndWait(item, 200);
    });

    return getBus();
}

function sendCmdGetBaseData() {
    let cmdList = [
        CMD_GET_SN,
        CMD_GET_HARDWARE_VERSION,
        CMD_GET_SOFTWARE_VERSION
    ];

    cmdList.forEach((item) => {
        Port.writeByQueueAndWait(item, 200);
    });

    return getBus();
}

function writeSn(sn) {
    Port.writeByQueue(makeFrame(sn));
}

const Tool = {
    port: Port,
    connect: connect,
    getBus: getBus,

    sendCmdGetFullData: sendCmdGetFullData,
    sendCmdGetTestData: sendCmdGetTestData,
    sendCmdGetBaseData: sendCmdGetBaseData,
    writeSn: writeSn
};

export default Tool;


//////////////////
// WEBPACK FOOTER
// ./device/tool.js
// module id = 160
// module chunks = 73