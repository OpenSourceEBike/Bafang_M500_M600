import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'

const MODE_HMI_PASS_THROUGH = 'aa010101010d0b';

const CMD_GET_VERSION = '1210'; // 获取版本号
const CMD_GET_FRAME_NUM = '1211'; // 获取车架号
const CMD_GET_BLUE_PRINT_NUM = '1212'; // 获取图纸号
const CMD_GET_SN = '1214'; // 获取序列号
const CMD_GET_ERROR_CODE = '1215'; // 获取历史错误代码
const CMD_GET_SERVICE_MILEAGE = '1216'; // 里程
const CMD_GET_HARDWARE_VERSION = '1217'; // 硬件版本号
const CMD_GET_TOTAL_GEAR = '1218'; // 总档位
const CMD_GET_TOTAL_MILEAGE = '1219'; // 总里程
const CMD_GET_MODEL = '110A1B'; // 获取仪表型号
const CMD_GET_CLIENT_CODE = '110B1C'; // 获取客户号
const CMD_GET_PARAMS = '110D1E'; // 获取轮经速度参数

const CMD_CLEAR_SERVICE_MILEAGE = '18110200002B'; // 清除维护公里数

const CMD_GET_KEY_CARD_START = '1812'; // 开始配卡
const CMD_GET_KEY_CARD_STOP = '1813'; // 结束配卡
const CMD_GET_KEY_CARD = '1220'; // 读取卡号
const CMD_GET_CIRCUMFERENCE = '1221'; // 读取仪表周长


//  Serial Port
let connBus;
let bus;
let ackBus;
let pendingProcessData = '';
var isCan = false;
var n = 0;
var count = true;
var key = false;
let hdData = [];
let veData = [];
let moData = [];
let snData = [];
let mnData = [];
let step = 0;

let snWriteLength = 0;
let snWriteCount = 0;

// USB Hid
let usbDevice = false;

function connect(comName) {
    if(comName){
        usbDevice = false;
        Port.setUSBDevice(false);
        connBus = Port.connect(comName, 57600);
    }else{
        // usbDevice = true;
        // isCan = Port.getCan();
        // connBus = Port.connectHID();
        // setTimeout(function(){
        //     Port.openHIDConHMI('0101');
        // },50);
    }

    hdData = [];
    veData = [];
    moData = [];
    snData = [];
    mnData = [];
    count = true;

    pendingProcessData = '';
    connBus.subscribe(item => {
        switch (item.type) {
            case 'open':
            case 'connect':
                {
                    console.log('hmi connect');
                    setup();
                    step = 0;
                    bus.next(item);
                }
                break;

            case 'data':
                {

                    if (count && item.data.substr( 0, 6 ) == '120000' ){
                        n++;
                        if (n > 2) {
                            isCan = true;
                            Port.setCan(true);
                            count = false;
                            console.log("Can2 hmi");
                        } else {
                            isCan = false;
                        }
                    }
                    if (isCan) {
                         if(usbDevice && item.data.substr( 0, 6 ) == '120000'){
                            let dataTem = item.data;
                            for( let a = 0; a < dataTem.length; ){
                                // pendingProcessData = dataTem(6+a,32);
                                // console.log(dataTem);
                                parseCan(dataTem.substr(6+a,26));
                                a += 26;
                            }
                        // }else {
                        //     pendingProcessData = item.data;
                        //     parseCan();
                        }

                    } else {
                        if(usbDevice && item.data.substr( 0, 2 ) == '10'){
                            console.log(item.data);
                            let dataLen = parseInt( item.data.substr( 6, 2 ),16 );
                            // console.log( item.data.substr( 8, dataLen*2 ),'data');
                            pendingProcessData += item.data.substr( 8, dataLen*2 );
                        }else {
                            pendingProcessData += item.data;
                        }
                        parseKey(item.data);
                        parse();
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

function parse() {
    if(!usbDevice) {
        let mode = Port.getMode();

        if (mode !== MODE_HMI_PASS_THROUGH) {
            setup();
            return;
        }
    }

    if (pendingProcessData.length <= 4) {
        return;
    }

    // console.log('hmi parse', pendingProcessData);
    parseModePassThrough();
}

function parseModePassThrough() {
    function parseXLenCheck(stepLength, data) {
        let length = parseInt(data.substring(stepLength, stepLength + 2), 16);
        let calculatedChecksum = Util.calculateChecksumFF(data.substring(0, stepLength + 2 + 2 * length));
        let checkSum = parseInt(data.substring(stepLength + 2 + 2 * length, stepLength + 2 + 2 * length + 2), 16);
        let needMoreData = data.length < stepLength + 2 + 2 * length + 2;
        if (length > 60) {
            needMoreData = false;
        }
        if (checkSum === calculatedChecksum) {
            let frame = data.substring(0, stepLength + 2 + 2 * length + 2);
            // Log.d(`parse out frame: ${frame}`);
            // Port.ack();
            processPassThrough(stepLength, frame);
            pendingProcessData = data.substring(stepLength + 2 + 2 * length + 2);
            return [true, false];
        }
        return [false, needMoreData];
    }

    while (pendingProcessData.length > 0) {
        // 0000Len...Check 和 00Len...Check
        // console.log('parse', pendingProcessData)

        let [parseOKXXXX, needMoreDataXXXX] = parseXLenCheck(4, pendingProcessData);
        // console.log('xxxx', parseOKXXXX, needMoreDataXXXX)
        if (parseOKXXXX) {
            continue;
        }

        let [parseOKXX, needMoreDataXX] = parseXLenCheck(2, pendingProcessData);
        // console.log('xx', parseOKXX, needMoreDataXX)
        if (parseOKXX) {
            continue;
        }

        if (needMoreDataXXXX || needMoreDataXX) {
            return;
        }

        pendingProcessData = pendingProcessData.substring(2);
    }

}

function processPassThrough(stepLength, frame) {

    let wrapper = {};

    let hmi = {};
    let parseXXXXOk = false;
    let functionPart = '';
    let data = '';
    let tempData = ''; // 去掉末尾00

    // 0000Len...Check
    if (stepLength === 4) {
        functionPart = parseInt(frame.substring(0, 4), 16);
        data = frame.substring(6, frame.length - 2);
        switch (functionPart) {
            case 0x110A:
                for (let i = data.length / 2 - 1; i > 0; i--) {
                    if (data.substring(2 * i, 2 * i + 2) === "00") {
                        tempData = data.substring(0, 2 * i);
                    }
                }
                data = tempData === '' ? data : tempData;
                hmi['model'] = Util.hexToString(data).trim();
                parseXXXXOk = true;
                break;
            case 0x110B:
                hmi['client_code'] = Util.hexToString(data).trim();
                parseXXXXOk = true;
                break;
            case 0x110D:
                hmi['wheel_diameter'] = parseInt(data.substring(0, 4), 16);
                hmi['speed_limit'] = parseInt(data.substring(4, 6), 16);
                parseXXXXOk = true;
                break;
            case 0x160B:
                hmi['client_code'] = Util.hexToString(data).trim();
                parseXXXXOk = true;
                break;
            case 0x160D:
                hmi['wheel_diameter'] = parseInt(data.substring(2, 4), 16);
                parseXXXXOk = true;
                break;
            case 0x1801:
                hmi['sn'] = Util.hexToString(data).trim();
                console.log('sn', hmi['sn']);
                parseXXXXOk = true;
                break;
            case 0x1811:
                // 设置0表示清除成功
                hmi['service_mileage'] = 0;
                parseXXXXOk = true;
                break;
            case 0x1814:
                hmi['wheel_circumference'] = Util.hexToString(data);
                parseXXXXOk = true;
                break;
            case 0x1815:
                hmi['time_zone'] = Util.hexToString(data);
                parseXXXXOk = true;
                break;
            case 0x1816:
                hmi['set_total_mileage'] = parseInt(data, 16);
                parseXXXXOk = true;
                break;
            case 0x1817:
                hmi['set_single_mileage'] = parseInt(data, 16);
                parseXXXXOk = true;
                break;
        }

        // if (parseXXXXOk === true) {
        //     wrapper['hmi'] = hmi;
        //     bus.next({ type: 'data', data: wrapper });
        //     if (ackBus) {
        //         console.log('ack bus', hmi);
        //         ackBus.next({ type: 'data', data: wrapper });
        //     }
        //     return;
        // }
    } else {
        // 00Len...Check
        functionPart = parseInt(frame.substring(0, 2), 16);
        data = frame.substring(4, frame.length - 2);
        switch (functionPart) {
            case 0x10:
                // 去掉末尾00
                for (let i = data.length / 2 - 1; i > 0; i--) {
                    if (data.substring(2 * i, 2 * i + 2) === "00") {
                        tempData = data.substring(0, 2 * i);
                    }
                }
                data = tempData === '' ? data : tempData;
                hmi['version'] = Util.hexToString(data).trim();
                break;
            case 0x11:
                hmi['frame_num'] = Util.hexToString(data).trim();
                break;
            case 0x12:
                hmi['blue_print_num'] = Util.hexToString(data).trim();
                break;
            case 0x13:
                hmi['id'] = Util.hexToString(data).trim();
                break;
            case 0x14:
                hmi['sn'] = Util.hexToString(data).trim();
                break;
            case 0x15:
                let code = 0;
                let errorCode = [];
                for (let i = 0; i < data.length / 2; i++) {
                    code = Util.hexToString(data.substring(i * 2, i * 2 + 4)).trim();
                    Log.d(`error code: ${code}`);
                    errorCode.push(code);
                }
                hmi['error_code'] = Util.uniqueArray(errorCode);
                break;
            case 0x16:
                Log.d(`16: ${data}`);
                hmi['service_mileage'] = data !== "" ? parseInt(data, 16) : 0;
                break;
            case 0x17:
                Log.d(`17: ${data}`);
                for (let i = data.length / 2 - 1; i > 0; i--) {
                    if (data.substring(2 * i, 2 * i + 2) === "00") {
                        tempData = data.substring(0, 2 * i);
                    }
                }
                data = tempData === '' ? data : tempData;
                hmi['hardware_version'] = Util.hexToString(data).trim();
                break;
            case 0x18:
                Log.d(`total gear 18: ${data}`);
                let totalGear = Util.hexToString(data);
                // let regx = /[0-9]/;
                // totalGear = regx.test(totalGear) ? parseInt(regx) : regx;
                hmi['total_gear'] = totalGear;
                break;
            case 0x19:
                Log.d(`19: ${data}`);
                hmi['total_mileage'] = data !== "" ? parseInt(data, 16) : 0;
                break;
            case 0x20:
                Log.d(`20: ${data}`);
                // let tem = Buffer.from(data, 'hex');
                // let a = [];
                // for (let t = 0; t < tem.length; t++) {
                //     if (tem[t] == 92) {
                //         a.push(124);
                //     } else {
                //         a.push(tem[t]);
                //     }
                // }
                // let b = Buffer.from(a);
                hmi['key_card'] = data;
                break;
            case 0x21:
                Log.d(`21: ${data}`);
                hmi['wheel_circumference'] = data !== "" ? parseInt(data, 16) : 0;
                break;

        }
    }

    wrapper['hmi'] = hmi;
    // console.log('bus', hmi);
    bus.next({ type: 'data', data: wrapper });

    if (ackBus) {
        // console.log('ack bus', hmi);
        ackBus.next({ type: 'data', data: wrapper });
    }
}

function parseKey(data) {

    let wrapper = {};
    let hmi = {};

    for (let i = 0; i < data.length;) {
        let functionPart = data.substr(i, 4);

        switch (functionPart) {
            case '1812':
                hmi['key_card_start'] = 1;
                break;
            case '1813':
                hmi['key_card_stop'] = 1;
                break;
        }
        i = i + 2;
    }

    wrapper['hmi'] = hmi;
    // console.log('bus', hmi);
    bus.next({ type: 'data', data: wrapper });

    if (ackBus) {
        // console.log('ack bus', hmi);
        ackBus.next({ type: 'data', data: wrapper });
    }
}

function setup() {

    if(!usbDevice) {
        if (Port.getMode() == MODE_HMI_PASS_THROUGH) {
            return;
        }
        console.log('setup hmi');
        Port.setMode(MODE_HMI_PASS_THROUGH);
        Port.writeByQueueAndWait(MODE_HMI_PASS_THROUGH, 200);
        console.log('setup hmi finish');
    }

}

function makeFrame(cmd) {

    let checkBuf = Buffer(cmd, 'hex');
    let checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum += checkBuf.length;
    checkSum = checkSum & 0xff;

    let frame = 'AA08' + cmd + Util.sprintf('%02X', checkBuf.length) + Util.sprintf('%02X', checkSum) + '0D0B';

    return frame;

}

// rawData 原始数据
function makeFrameForWrite(headString, rawData) {
    let hex = Util.stringToHex(rawData);
    return makeFrameForWriteHex(headString, hex);

    // let dataBuf = Buffer(data, 'hex');
    // let length = dataBuf.length;
    // data = headString + Util.sprintf('%02X', length) + data;

    // var checkBuf = Buffer(data, 'hex');
    // var checkSum = 0;
    // for (var j = 0; j < checkBuf.length; j++) {
    //     checkSum = checkSum + checkBuf[j];
    // }
    // checkSum = checkSum & 0xff;

    // let frame = data + Util.sprintf('%02X', checkSum);
    // return makeFrame(frame);
}

function makeFrameForWriteHex(headString, hex) {
    let dataBuf = Buffer(hex, 'hex');
    let length = dataBuf.length;
    hex = headString + Util.sprintf('%02X', length) + hex;

    let checkBuf = Buffer(hex, 'hex');
    let checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum = checkSum & 0xff;

    let frame = hex + Util.sprintf('%02X', checkSum);
    console.log(frame);
    if(Port.getUSBDevice()){
        return frame;
    }else {
        return makeFrame(frame);
    }

}

function sendCmdGetFullData() {
    if (Port.getUSBDevice()) {
        // Port.writeByQueueAndWait(Buffer.concat([0x34,0x00,0x00,0x01],64), 100);
        // Port.writeByQueueAndWait(Buffer.concat( [ Buffer.from([0x30,0x00,0x00]) ],64), 100);
        sendCmdGetFullDataUsb();
    } else {
        setup();

        let keyData = {
            'sn': makeFrame(CMD_GET_SN),
            'client_code': makeFrame(CMD_GET_CLIENT_CODE),
            'wheel_diameter': makeFrame(CMD_GET_PARAMS),
            'model': makeFrame(CMD_GET_MODEL),
            'version': makeFrame(CMD_GET_VERSION),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
            // 'wheel_circumference': makeFrame(CMD_GET_CIRCUMFERENCE),
            'service_mileage': makeFrame(CMD_GET_SERVICE_MILEAGE),
            'total_mileage': makeFrame(CMD_GET_TOTAL_MILEAGE),
            'total_gear': makeFrame(CMD_GET_TOTAL_GEAR),
            // 'key_card': makeFrame(CMD_GET_KEY_CARD)

        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.hmi) {
                let hmi = res.data.hmi;
                for (let key in hmi) {
                    ack(key);
                }
            }
        });
    }

}

function sendCmdGetBaseData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb('b');
    } else {
        setup();

        let keyData = {
            'sn': makeFrame(CMD_GET_SN),
            'model': makeFrame(CMD_GET_MODEL),
            'version': makeFrame(CMD_GET_VERSION),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.hmi) {
                let hmi = res.data.hmi;
                for (let key in hmi) {
                    ack(key);
                }
            }
        });
    }
}

function sendCmdGetTestData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb('t');
    } else {
        setup();

        let keyData = {
            'sn': makeFrame(CMD_GET_SN),
            'model': makeFrame(CMD_GET_MODEL),
            'version': makeFrame(CMD_GET_VERSION),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
            'wheel_diameter': makeFrame(CMD_GET_PARAMS),
            'client_code': makeFrame(CMD_GET_TOTAL_MILEAGE)
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.hmi) {
                let hmi = res.data.hmi;
                for (let key in hmi) {
                    ack(key);
                }
            }
        });
    }
}

function sendCmdGetKeyCard() {
    setup();

    let keyCard = { 'key_card': makeFrame(CMD_GET_KEY_CARD) };

    let ack = Port.writeAndEnsure(keyCard, 200);

    ackBus = new Rx.Subject();
    ackBus.subscribe(res => {
        if (res.data && res.data.hmi) {
            let hmi = res.data.hmi;
            for (let key in hmi) {
                ack(key);
            }
        }
    });
}

function clearServiceMileage() {
    setup();
    Port.writeByQueueAndWait(makeFrame(CMD_CLEAR_SERVICE_MILEAGE), 300);
}

function writeData(headString, data) {
    let cmd = makeFrameForWrite(headString, data);
    Port.writeByQueueAndWait(cmd, 300);
}

function writeSn(data) {
    let cmd = makeFrameForWrite('1801', data);

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        Port.usbSDK.client.config('00002580').then(res =>{
        }).then(res =>{
            console.log(res);
        }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.hmi.update(cmd)
                .then(res=>{
                    console.log(res);
                    // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                    processPassThrough(4,res.data);
                }).catch(err =>{

                console.log(err);
                if (err == 'Timeout'){
                    Port.usbSDK.machineInformation.uart.hmi.update(cmd)
                        .then(res=>{
                            console.log(res);
                            processPassThrough(4,res.data);
                        }).catch(err =>{

                    }).finally(()=>{
                    });
                }
            }).finally(()=>{
            });
        },100)
    } else {
        setup();
        Port.writeByQueueAndWait(cmd, 300);
    }
}

function writeClientCode(data) {
    let cmd = makeFrameForWrite('160B', data);

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        Port.usbSDK.client.config('00002580').then(res =>{
            }).then(res =>{
                console.log(res);
            }).catch(err =>{

                console.log(err);
            }).finally(()=>{
        });
        Port.usbSDK.machineInformation.uart.hmi.update(cmd)
            .then(res=>{
                console.log(res);
                // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                processPassThrough(4,res.data);
            }).catch(err =>{

            console.log(err);

        }).finally(()=>{
        });
    } else {
        setup();
        Port.writeByQueueAndWait(cmd, 300);
    }
}

function writeKeyCardStart() {
    setup();

    let keyData = {
        'key_card_start': makeFrame(CMD_GET_KEY_CARD_START)
    }

    let ack = Port.writeAndEnsure(keyData, 200);

    ackBus = new Rx.Subject();
    ackBus.subscribe(res => {
        if (res.data && res.data.hmi) {
            let hmi = res.data.hmi;
            for (let key in hmi) {
                ack(key);
            }
        }
    });
}

function writeKeyCardStop() {
    setup();

    let keyData = {
        'key_card_stop': makeFrame(CMD_GET_KEY_CARD_STOP)
    }

    let ack = Port.writeAndEnsure(keyData, 200);

    ackBus = new Rx.Subject();
    ackBus.subscribe(res => {
        if (res.data && res.data.hmi) {
            let hmi = res.data.hmi;
            for (let key in hmi) {
                ack(key);
            }
        }
    });
}

function writeCircumference() {
    setup();
}

function writeTimeZone(data) {
    setup();
    let cmd = makeFrameForWrite('1814', data);
    Port.writeByQueueAndWait(cmd, 300);
}

function writeParam(wheelDiameter, speedLimit) {
    let paramStr = '00';
    paramStr += wheelDiameter ? Util.sprintf('%02X', wheelDiameter) : '00';
    paramStr += speedLimit ? Util.sprintf('%02X', speedLimit) : '00';
    console.log(paramStr);

    let cmd = makeFrameForWriteHex('160D', paramStr);

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        Port.usbSDK.client.config('00002580').then(res =>{
        }).then(res =>{
            console.log(res);
        }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.hmi.update(cmd)
                .then(res=>{
                    console.log(res);
                    // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                    processPassThrough(4,res.data);
                }).catch(err =>{

                console.log(err);
                if (err == 'Timeout'){
                    Port.usbSDK.machineInformation.uart.hmi.update(cmd)
                        .then(res=>{
                            console.log(res);
                            processPassThrough(4,res.data);
                        }).catch(err =>{

                    }).finally(()=>{
                    });
                }
            }).finally(()=>{
            });
        },100)
    }else{
        setup();
        Port.writeByQueueAndWait(cmd, 300);
    }

}


function getBus() {
    return bus;
}

let clearServiceCan = false;

function clearServiceMileageCan(){
    // 写5000，数据是50000 000051C300
    clearServiceCan = true;
    // Port.writeByQueueAndWait(makeFrameCanWrite('00','6302', '000000000'), 100);
    Port.usbSDK.machineInformation.update.dataOfHmi_2(0x05,0x03,'0000000000')
        .then(res=>{
            console.log(res);
        }).catch(err =>{

        console.log(err);
    }).finally(()=>{
    });
}

function clearMileageUart(mileage,type){
    if(!mileage){
        return;
    }
    let cmdMileage;
    let mileHex =('00000000'+parseInt(mileage).toString(16)).substr(-8) ;

    // if(mileHex.length%2 !== 0){
    //     mileHex = '0'+mileHex;
    // }
    if(type === 'total'){
        cmdMileage = makeFrameForWriteHex('1816', mileHex);
    }else if(type === 'single'){
        cmdMileage = makeFrameForWriteHex('1817', mileHex);
    }

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        Port.usbSDK.client.config('00002580').then(res =>{
        }).then(res =>{
            console.log(res);
        }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.hmi.update(cmdMileage)
                .then(res=>{
                    console.log(res);
                    // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                    processPassThrough(4,res.data);
                }).catch(err =>{

                console.log(err);
                if (err === 'Timeout'){
                    Port.usbSDK.machineInformation.uart.hmi.update(cmdMileage)
                        .then(res=>{
                            console.log(res);
                            processPassThrough(4,res.data);
                        }).catch(err =>{

                    }).finally(()=>{
                    });
                }
            }).finally(()=>{
            });
        },100)
    }else{
        setup();
        Port.writeByQueueAndWait(cmdMileage, 300);
    }
}





//  USB 内容


function sendCmdGetFullDataUsb(param){
    /*
    buffer:Uint8Array(64) [16, 0, 0, 17, 16, 14, 68, 80, 67, 49, 48, 75, 49, 48, 51, 48, 48, 54, 46, 48, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    data:"100e44504331304b3130333030362e3029"
    dataPacks:{}
    hex:"10000011100e44504331304b3130333030362e302900000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    */
    bus = new Rx.Subject();

    Port.usbSDK.client.config('00002580').then(res=>{
        console.log(res);
    }).catch(err =>{

        console.log(err);
    }).finally(()=>{
        console.log('response');
    });
    setTimeout(()=>{
        readBasic(param);
    },300);

    // bus = new Rx.Subject();
}

let uartHmiBus;

function readBasic(param){
    let readKey = [];
    let retryTime;
    let basicTime = 350;
    if(param && param === 'b'){
        readKey = [
            "model",
            "model",
            "sn",
            "version",
            "hardware_version"
        ];
        retryTime = 5 * basicTime + 50 ;
    }else if(param && param === 't'){
        readKey = [
            "model",
            "sn",
            "version",
            "hardware_version",
            "wheel_diameter",
            "total_gear",
            "client_code"
        ];
        retryTime = 6 * basicTime + 50 ;
    }else{
        readKey = [
            "model",
            "model",
            "sn",
            "version",
            "hardware_version",
            "wheel_diameter",
            "client_code",
            "service_mileage",
            "total_mileage",
            "total_gear"
        ];
        retryTime = 10 * basicTime + 50 ;
    }
    let check = {};

    let ack = function (key) {
        // console.log('ack', check, key);
        check[key] = true;
    };

    readHmi.init();

    readKey.forEach(key => {
        check[key] = false;
        readHmi[key]();
    });

    uartHmiBus.subscribe(res => {
        if (!res.data) {
            return;
        }
        if (res.type ==='data' && res.data.hasOwnProperty("version")) {
            processPassThrough(2,res.data["version"]);
            ack('version');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("hardware_version")) {
            processPassThrough(2,res.data["hardware_version"]);
            ack('hardware_version');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("model")) {
            processPassThrough(4,res.data["model"]);
            ack('model');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("sn")) {
            processPassThrough(2,res.data["sn"]);
            ack('sn');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("wheel_diameter")) {
            processPassThrough(4,res.data["wheel_diameter"]);
            ack('wheel_diameter');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("service_mileage")) {
            processPassThrough(2,res.data["service_mileage"]);
            ack('service_mileage');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("total_gear")) {
            processPassThrough(2,res.data["total_gear"]);
            ack('total_gear');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("total_mileage")) {
            processPassThrough(2,res.data["total_mileage"]);
            ack('total_mileage');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("client_code")) {
            processPassThrough(4,res.data["client_code"]);
            ack('client_code');
        }
    });

    let count = 0;
    let checkInterval = setInterval(()=>{
            for (let key in check) {
                if (!check[key]) {
                    readHmi[key]();
                    // console.log('retry', key);
                }
            }
            count++;
            if (count > 2) {
                clearInterval(checkInterval);
            }
        },retryTime);
}

let readHmi = {

    "init": function () {
        console.log("init");
        uartHmiBus = new Rx.Subject();
    },
    'sn': function () {
        Port.usbSDK.machineInformation.uart.hmi.serialNumber()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'sn': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'client_code': function () {
        Port.usbSDK.machineInformation.uart.hmi.clientCode()
            .then(res=>{
                if (Port.checkUartSum(4, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'client_code': res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=>{
        });
    },
    'wheel_diameter': function () {
        Port.usbSDK.machineInformation.uart.hmi.speedAndWheel()
            .then(res => {
                if (Port.checkUartSum(4, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'wheel_diameter': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'model': function () {
        Port.usbSDK.machineInformation.uart.hmi.modelNumber()
            .then(res => {
                if (Port.checkUartSum(4, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'model': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'version': function () {
        Port.usbSDK.machineInformation.uart.hmi.softwareVersionNumber()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'version': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'hardware_version': function () {
        Port.usbSDK.machineInformation.uart.hmi.hardwareVersionNumber()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'hardware_version': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'service_mileage': function () {
        Port.usbSDK.machineInformation.uart.hmi.serviceMileage()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'service_mileage': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'total_mileage': function () {
        Port.usbSDK.machineInformation.uart.hmi.totalMileage()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'total_mileage': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },
    'total_gear': function () {
        Port.usbSDK.machineInformation.uart.hmi.totalGear()
            .then(res => {
                if (Port.checkUartSum(2, res.data)) {
                    uartHmiBus.next({type: 'data', data: {'total_gear': res.data}});
                }
            }).catch(err => {
            console.log(err);
        }).finally(() => {
        });
    },

};


const HMI = {
    port: Port,
    connect: connect,
    getBus: getBus,

    sendCmdGetFullData: sendCmdGetFullData,
    sendCmdGetBaseData: sendCmdGetBaseData,
    sendCmdGetTestData: sendCmdGetTestData,
    sendCmdGetKeyCard: sendCmdGetKeyCard,
    clearServiceMileage: clearServiceMileage,
    writeKeyCardStart: writeKeyCardStart,
    writeKeyCardStop: writeKeyCardStop,
    writeSn: writeSn,
    writeClientCode: writeClientCode,
    writeTimeZone: writeTimeZone,
    writeCircumference: writeCircumference,
    writeParam: writeParam,
    clearServiceMileageCan: clearServiceMileageCan,
    clearMileageUart: clearMileageUart,

};

export default HMI;



//////////////////
// WEBPACK FOOTER
// ./device/hmi.js
// module id = 80
// module chunks = 73