import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'


let connBus;
let bus;
let ackBus;
let pendingProcessData = '';
var isCan = true;
var n = 0;
let hdData = [];
let veData = [];
let moData = [];
let snData = [];
let mnData = [];
let step = 0;

let snWriteLength = 0;
let snWriteCount = 0;

// USB Hid
let usbDevice = true;

function connect() {
    connBus = Port.connectHID();
    // setTimeout(function(){
    //     Port.openHIDConHMI('0101');
    // },50);

    hdData = [];
    veData = [];
    moData = [];
    snData = [];
    mnData = [];

    pendingProcessData = '';
    connBus.subscribe(item => {
        switch (item.type) {
            case 'open':
            case 'connect':
                {
                    console.log('sensor connect');
                    step = 0;
                    bus.next(item);
                }
                break;

            case 'data':
                {

                    if(usbDevice && item.data.substr( 0, 6 ) == '120000'){
                        let dataTem = item.data;
                        for( let a = 0; a < dataTem.length; ){
                            // pendingProcessData = dataTem(6+a,32);
                            parseCan(dataTem.substr(6+a,26));
                            a += 26;
                        }

                    }
                    // console.log(item.data);
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



function sendCmdGetFullData() {
    if (isCan) {
        sendCmdGetFullDataCan();
    }
}






// ----》 Can数据
function parseCan(data) {

    let wrapper = {};
    let sensorCan = {};

    let len = parseInt(data.substr(8, 2), 16);

    let dataHex = data.substr(10, len*2);

    if (data.substr(6,2) != '00') {
        // console.log( data );
        let functionPart = Port.processIndex(data.substr(0,8));

        switch (functionPart) {

            //  硬件版本号
            case '0105046000':
                step = 1;
                hdData[0] = parseInt(data.substr(10, 2), 16);
                Port.writeCan(makeFrameCan("02", "6000"));
                break;
                //  软件版本号
            case '0105046001':
                step = 2;
                veData[0] = parseInt(data.substr(10, 2), 16);
                Port.writeCan(makeFrameCan("02", "6001"));
                break;
                //  型号名称
            case '0105046002':
                step = 3;
                moData[0] = parseInt(data.substr(10, 2), 16);
                Port.writeCan(makeFrameCan("02", "6002"));
                break;
                //  SN
            case '0105046003':
                step = 4;
                snData[0] = parseInt(data.substr(10, 2), 16);
                Port.writeCan(makeFrameCan("02", "6003"));
                break;
                //  客户号
            case '0105046004':
                step = 5;
                mnData[0] = parseInt(data.substr(10, 2), 16);
                Port.writeCan(makeFrameCan("02", "6004"));
                break;
                //  写SN
            case '0105026003':
                snWriteCount++;
                if (snWriteCount == snWriteLength) {
                    console.log("start write");
                    sensorCan['sn'] = ' ';
                }
                break;
                //  写客户代号
            case '0105026004':
                snWriteCount++;
                if (snWriteCount == snWriteLength) {
                    console.log("start write");
                    sensorCan['man_name'] = ' ';
                }
                break;
            case '011F003100':
                let result = Buffer.from(dataHex, 'hex');
                // console.log(result);
                sensorCan['torque_value'] = result[0] + result[1] * 256;
                sensorCan['cadence'] = result[2];
                break;
        }

        let long = parseInt(functionPart.substr(8, 2), 16);
        /** **/
        if (step === 1) {
            switch (functionPart.substr(0, 8)) {

                case "01050500":
                    hdData[long + 1] = Util.hexToString(dataHex);
                    if (typeof(checkData(hdData)) === 'string') {
                        sensorCan['hardware_version'] = checkData(hdData);
                        console.log("11");
                        step = 0;
                    }
                    answerCan('6000');
                    break;
                case "01050600":
                    hdData[long + 1] = Util.hexToString(dataHex);
                    console.log('hdData',hdData);
                    if (typeof(checkData(hdData)) === 'string') {
                        sensorCan['hardware_version'] = checkData(hdData);
                        console.log("12");
                        step = 0;
                    }
                    step = 0;
                    answerCan('6000');
                    break;
            }
        }

        if (step === 2) {
            switch (functionPart.substr(0, 8)) {
                case "01050500":
                    veData[long + 1] = Util.hexToString(dataHex);
                    if (typeof(checkData(veData)) === 'string') {
                        sensorCan['version'] = checkData(veData);
                        console.log("21");
                        step = 0;
                    }
                    answerCan('6001');
                    break;
                case "01050600":
                    veData[long + 1] = Util.hexToString(dataHex);
                    console.log('veData',veData);
                    if (typeof(checkData(veData)) === 'string') {
                        sensorCan['version'] = checkData(veData);
                        console.log("22");
                        step = 0;
                    }
                    answerCan('6001');
                    break;
            }
        }

        if (step === 3) {
            switch (functionPart.substr(0, 8)) {
                case "01050500":
                    moData[long + 1] = Util.hexToString(dataHex);
                    answerCan('6002');
                    if (typeof(checkData(moData)) === 'string') {
                        sensorCan['model'] = checkData(moData);
                        console.log("31");
                        step = 0;
                    }
                    break;
                case "01050600":
                    moData[long + 1] = Util.hexToString(dataHex);
                    console.log('moData',moData);
                    if (typeof(checkData(moData)) === 'string') {
                        sensorCan['model'] = checkData(moData);
                        console.log("32");
                        step = 0;
                    }
                    answerCan('6002');
                    break;
            }
        }

        if (step === 4) {
            switch (functionPart.substr(0, 8)) {
                case "01050500":
                    snData[long + 1] = Util.hexToString(dataHex);
                    if (typeof(checkData(snData)) === 'string') {
                        sensorCan['sn'] = checkData(snData);
                        console.log("41");
                        step = 0;
                    }
                    answerCan('6003');
                    break;
                case "01050600":
                    snData[long + 1] = Util.hexToString(dataHex);
                    console.log('snData',snData);
                    if (typeof(checkData(snData)) === 'string') {
                        sensorCan['sn'] = checkData(snData);
                        console.log("42");
                        step = 0;
                    }
                    answerCan('6003');
                    break;
            }
        }

        if (step === 5) {
            switch (functionPart.substr(0, 8)) {
                case "01050500":
                    mnData[long + 1] = Util.hexToString(dataHex);
                    if (typeof(checkData(mnData)) === 'string') {
                        sensorCan['man_name'] = checkData(mnData);
                        console.log("51");
                        step = 0;
                    }
                    answerCan('6004');
                    break;
                case "01050600":
                    mnData[long + 1] = Util.hexToString(dataHex);
                    console.log('mnData',mnData);
                    if (typeof(checkData(mnData)) === 'string') {
                        sensorCan['man_name'] = checkData(mnData);
                        console.log("52");
                        step = 0;
                    }
                    answerCan('6004');
                    break;
            }
        }
    }

    wrapper['sensor'] = sensorCan;
    // console.log('bus', sensor);
    bus.next({ type: 'data', data: wrapper });

    if (ackBus) {
        // console.log('ack bus', sensor);
        ackBus.next({ type: 'data', data: wrapper });
    }

}

function checkData(arr) {
    console.log(arr);
    let len = arr[0];
    let str = '';

    for (let i = 1; i < arr.length; i++) {
        str += arr[i];
    }

    if (len === str.length) {
        return str;
    } else {
        return false;
    }
}

function makeFrameCan(as, cmd) {
    let cmdTemp = Port.makeIndex( '0501' + as + cmd ) + '00';
    return Port.makeFrameHIDCan('15',cmdTemp,'sensor');
}

function answerCan(cmd) {
    Port.writeCan(makeFrameCan("02", cmd));
}

function sendCmdGetFullDataCan() {
    setTimeout(()=>{
        let keyData = {
            'sn': makeFrameCan('01', '6003'),
            'model': makeFrameCan('01', '6002'),
            'version': makeFrameCan('01', '6001'),
            'hardware_version': makeFrameCan('01', '6000'),
            'man_name': makeFrameCan('01', '6004'),
        };

        let ack = Port.writeAndEnsureCan(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.sensor) {
                let sensor = res.data.sensor;
                for (let key in sensor) {
                    ack(key);
                }
            }
        });
    },500)

}

function makeFrameCanWrite(as, cmd, data) {
    let cmdTemp = Port.makeIndex( '0501' + as + cmd );
    if (data) {
        let length = data.length / 2;
        cmdTemp = cmdTemp + '0' + length + data;
    } else {
        cmdTemp = cmdTemp + '00';
    }

    return Port.makeFrameHIDCan('15',cmdTemp);
}

function writeSensor(data, cmd) {

    let dataHex = Util.stringToHex(data);
    let len = data.length;

    snWriteCount = 0;
    snWriteLength = Math.ceil(len / 8) + 1;

    Port.writeByQueueAndWait(makeFrameCanWrite('00', cmd, Buffer.from([len]).toString('Hex')), 250);

    if (len < 9) {
        Port.writeByQueueAndWait(makeFrameCanWrite('04', cmd, dataHex), 500);
        Port.writeByQueueAndWait(makeFrameCanWrite('06', '0000'), 300);
    } else {

        let cmd2 = '';

        for (let l = 0; l < Math.ceil(len/8);) {
            // console.log(dataHex.substr(l * 16, 16));

            if (l < Math.ceil(len/8)-1 && l > 0) {
                cmd2 = makeFrameCanWrite('05', '00' + Buffer.from([l - 1]).toString('Hex'), dataHex.substr(l * 16, 16));
            } else if (l === 0) {

                cmd2 = makeFrameCanWrite('04', cmd, dataHex.substr(l * 16, 16));
            } else {
                cmd2 = makeFrameCanWrite('06', '00' + Buffer.from([l - 1]).toString('Hex'), dataHex.substr(l * 16, 16));
            }
            // console.log(cmd2);
            Port.writeByQueueAndWait(cmd2, 400);
            l++;
        }
    }


}

function writeSnCan(data) {
    writeSensor(data, '6003');
}

function writeManCan(data) {
    writeSensor(data, '6004');
}


function getBus() {
    return bus;
}


const Sensor = {
    port: Port,
    connect: connect,
    getBus: getBus,

    sendCmdGetFullData: sendCmdGetFullData,
    writeSnCan: writeSnCan,
    writeManCan: writeManCan,
};

export default Sensor;



//////////////////
// WEBPACK FOOTER
// ./device/sensor.js
// module id = 161
// module chunks = 73