import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'

const MODE_CTRL_PASS_THOUGH = 'aa010201010d0b'; // 控制器透传

const CMD_GET_VERSION = '1150'; // 版本号
const CMD_GET_FRAME_NUM = '1422'; // 车架号
const CMD_GET_BLUE_PRINT_NUM = '1412'; // 图纸号
const CMD_GET_ID = '1413'; // ID
const CMD_GET_PRODUCT_SN = '1414'; // 序列号
const CMD_GET_ERROR_CODE = '1415'; // 错误代码
const CMD_GET_MODEL = '1416'; // 型号
const CMD_GET_HARDWARE_VERSION = '1417'; // 硬件版本

const CMD_BASIC_INFO = '11512580f6'; //基础信息
const CMD_PARAM = '1152'; //参数


//  Serial Port
let connBus;
let bus;
let ackBus;
let pendingProcessData = '';
var isCan = false;
var n = 0;
var count = true;
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
                    setup();
                    step = 0;
                    bus.next(item);
                }
                break;

            case 'data':
                {
                    if (count && item.data.substr( 0, 6 ) == '120000') {
                        n++;
                        if (n > 2) {
                            isCan = true;
                            Port.setCan(true);
                            count = false;
                            // console.log("Can2 controller");
                        } else {
                            isCan = false;
                        }
                    }
                    if (isCan) {
                        if(usbDevice && item.data.substr( 0, 6 ) == '120000'){
                            let dataTem = item.data;
                            for( let a = 0; a < dataTem.length; ){
                                // pendingProcessData = dataTem(6+a,32);
                                parseCan(dataTem.substr(6+a,26));
                                a += 26;
                            }
                            // }else {
                            //     pendingProcessData = item.data;
                            //     parseCan();
                        }
                    } else {
                        if(usbDevice && item.data.substr( 0, 2 ) == '11'){
                            let dataLen = parseInt( item.data.substr( 6, 2 ),16 );
                            // console.log( item.data.substr( 8, dataLen*2 ),'data');
                            pendingProcessData += item.data.substr( 8, dataLen*2 );
                        }else {
                            pendingProcessData += item.data;
                        }
                        parse();
                    }
                    // console.log(item.data,'contro');
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
    if(!usbDevice){
        let mode = Port.getMode();

        if (mode !== MODE_CTRL_PASS_THOUGH) {
            setup();
            return;
        }
    }

    if (pendingProcessData.indexOf('AA010201010D0B') > -1) {
        pendingProcessData = pendingProcessData.substring(pendingProcessData.indexOf('AA010201010D0B') + 14);
    }

    if (pendingProcessData.length <= 4) {
        return;
    }

    // console.log('ctrl parse', pendingProcessData);
    function parseXLenCheck(stepLength, data) {
        let length = parseInt(data.substring(stepLength, stepLength + 2), 16);
        let calculatedChecksum = Util.calculateChecksumFF(data.substring(0, stepLength + 2 + 2 * length));
        let checkSum = parseInt(data.substring(stepLength + 2 + 2 * length, stepLength + 2 + 2 * length + 2), 16);
        let needMoreData = data.length < stepLength + 2 + 2 * length + 2;
        // 避免等待数据非常长
        if (length > 60) {
            needMoreData = false;
        }
        if (checkSum === calculatedChecksum) {
            let frame = data.substring(0, stepLength + 2 + 2 * length + 2);
            // Log.d(`parse out frame: ${frame}`);
            process(frame);
            pendingProcessData = data.substring(stepLength + 2 + 2 * length + 2);
            return [true, false];
        }
        return [false, needMoreData];
    }

    while (pendingProcessData.length > 0) {
        // 0000Len...Check 和 00Len...Check

        let [parseOKXXXX, needMoreDataXXXX] = parseXLenCheck(4, pendingProcessData);
        // console.log(1, parseOKXXXX, needMoreDataXXXX);
        if (parseOKXXXX) {
            continue;
        }

        let [parseOKXX, needMoreDataXX] = parseXLenCheck(2, pendingProcessData);
        // console.log(2, parseOKXX, needMoreDataXX);
        if (parseOKXX) {
            continue;
        }

        if (needMoreDataXX || needMoreDataXXXX) {
            return;
        }

        pendingProcessData = pendingProcessData.substring(2);
    }
}

function process(frame) {

    let wrapper = {};

    let ctrl = {};

    let functionPart = parseInt(frame.substring(0, 2), 16);
    let data = frame.substring(4, frame.length - 2);
    switch (functionPart) {
        case 0x01:
            ctrl['sn'] = Util.hexToString(data).trim();
            Log.d(`${ctrl['sn']}`);
            break;
            //case 0x10:
            //    controller.data.versionNum = Core.Util.hex2a(data);
            //    break;
        case 0x11:
            ctrl['frame_num'] = Util.hexToString(data).trim();
            break;
        case 0x12:
            ctrl['blue_print_num'] = Util.hexToString(data).trim();
            break;
        case 0x13:
            ctrl['id'] = Util.hexToString(data).trim();
            break;
        case 0x14:
            ctrl['sn'] = Util.hexToString(data).trim();
            Log.d(`${ctrl['sn']}`);
            break;
        case 0x15:
            let length = parseInt(frame.substring(2, 4), 16);
            Log.d(`${length}`);
            let errorCodes = [];
            for (i = 0; i < length; i++) {
                errorCodes.push(parseInt(data.substring(i * 2, i * 2 + 2), 16));
            }
            ctrl['error_code'] = Util.uniqueArray(errorCodes);
            break;
        case 0x16:
            ctrl['model'] = Util.hexToString(data).trim();
            break;
        case 0x17:
            ctrl['hardware_version'] = Util.hexToString(data).trim();
            break;
        case 0x50:
            ctrl['version'] = Util.hexToString(data).trim();
            break;
        case 0x51:
            ctrl['man_name'] = Util.hexToString(data.substring(0, 8)).trim();
            //controller.data.model = Core.Util.hex2a(data.substring(8, 16));
            //controller.data.hardVersion = Core.Util.hex2a(data.substring(16, 20));
            //controller.data.softVersion = Core.Util.hex2a(data.substring(20, 28));
            switch (parseInt(data.substring(28, 30), 16)) {
                case 0:
                    ctrl['nml_volt'] = 24;
                    break;
                case 1:
                    ctrl['nml_volt'] = 36;
                    break;
                case 2:
                    ctrl['nml_volt'] = 48;
                    break;
                case 3:
                    ctrl['nml_volt'] = 43;
                    break;
            }
            ctrl['max_cur'] = parseInt(data.substring(30, 32), 16);
            break;
        case 0x52:

            ctrl['limit_volt'] = parseInt(data.substring(0, 2), 16);
            ctrl['limit_cur'] = parseInt(data.substring(2, 4), 16);
            ctrl['cur'] = [];
            ctrl['spd'] = [];
            for (let i = 0; i < 10; i++) {
                ctrl['cur'][i] = parseInt(data.substring(4 + i * 2, 6 + i * 2), 16);
                ctrl['spd'][i] = parseInt(data.substring(24 + i * 2, 26 + i * 2), 16);
            }
            ctrl['wheel_dia'] = parseInt(data.substring(44, 46), 16);
            let spd = parseInt(data.substring(46, 48), 16);
            spd = Util.sprintf('%08b', spd);
                // console.log('spd', spd)
            ctrl['spd_meter_type'] = spd.substring(5, 7);
            ctrl['spd_meter_signal_num'] = parseInt(spd.substring(0, 5), 2);
            break;
    }

    wrapper['ctrl'] = ctrl;
    bus.next({ type: 'data', data: wrapper });

    if (ackBus) {
        ackBus.next({ type: 'data', data: wrapper });
    }
}


function setup() {

    if(!usbDevice){
        if (Port.getMode() === MODE_CTRL_PASS_THOUGH) {
            return;
        }
        console.log('set ctrl');
        Port.setMode(MODE_CTRL_PASS_THOUGH);
        Port.writeByQueueAndWait(MODE_CTRL_PASS_THOUGH, 200);
        console.log('set ctrl finish');
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

    let frame = 'AA09' + cmd + Util.sprintf('%02X', checkBuf.length) + Util.sprintf('%02X', checkSum) + '0D0B';
    // console.log('frame',frame);

    return frame;

}
// rawData 原始数据
function makeFrameForWrite(headString, rawData) {
    let dataHex = Util.stringToHex(rawData);
    return makeFrameForWriteHex(headString, dataHex);
}

function makeFrameForWriteHex(headString, dataHex) {
    let length = Buffer(dataHex, 'hex').length;
    let checkHex = headString.substring(2) + Util.sprintf('%02X', length) + dataHex;

    let checkBuf = Buffer(checkHex, 'hex');
    let checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum = checkSum & 0xff;

    let frame = headString + Util.sprintf('%02X', length) + dataHex + Util.sprintf('%02X', checkSum);
    if(Port.getUSBDevice()){
        return frame;
    }else {
        return makeFrame(frame);
    }

}


function sendCmdGetFullData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb();
    } else {
        setup();

        let keyData = {
            'version': makeFrame(CMD_GET_VERSION),
            'sn': makeFrame(CMD_GET_PRODUCT_SN),
            'model': makeFrame(CMD_GET_MODEL),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
            'limit_volt': makeFrame(CMD_PARAM),
            'man_name': makeFrame(CMD_BASIC_INFO),
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.ctrl) {
                let ctrl = res.data.ctrl;
                for (let key in ctrl) {
                    ack(key);
                }
            }
        });
    }
}

function sendCmdGetBaseData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb("b");
    } else {
        setup();

        let keyData = {
            'sn': makeFrame(CMD_GET_PRODUCT_SN),
            'model': makeFrame(CMD_GET_MODEL),
            'version': makeFrame(CMD_GET_VERSION),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.ctrl) {
                let ctrl = res.data.ctrl;
                for (let key in ctrl) {
                    ack(key);
                }
            }
        });
    }
}

function sendCmdGetTestData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb();
    } else {
        setup();

        let keyData = {
            'sn': makeFrame(CMD_GET_PRODUCT_SN),
            'model': makeFrame(CMD_GET_MODEL),
            'version': makeFrame(CMD_GET_VERSION),
            'hardware_version': makeFrame(CMD_GET_HARDWARE_VERSION),
            'limit_volt': makeFrame(CMD_PARAM),
            'man_name': makeFrame(CMD_BASIC_INFO),
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.ctrl) {
                let ctrl = res.data.ctrl;
                for (let key in ctrl) {
                    // console.log("key",key);
                    ack(key);
                }
            }
        });
    }
}

function initCanController(){
    //  USB - Uart切换波特率
    // console.log("init start");

    Port.writeByQueueAndWait( Buffer.concat( [Buffer.from([0x21,0x00,0x00,0x04,0xB0 ]) ],64), 90);
    // Port.writeHID( Buffer.concat( [ Buffer.from([0x21,0x00,0x00,0x04,0xB0 ]) ],64) );
    // setTimeout(function () {
    Port.writeByQueueAndWait( Port.makeFrameHID('14',CMD_BASIC_INFO ),90 );
    Port.writeByQueueAndWait( Buffer.concat( [Buffer.from([0x21,0x00,0x00,0x25,0x80 ]) ],64), 90);
    // Port.writeHID( Port.makeFrameHID(CMD_BASIC_INFO));
    // Port.writeHID( Buffer.concat( [ Buffer.from([0x21,0x00,0x00,0x25,0x80 ]) ],64) );

    // },500)
    // Port.writeByQueueAndWait( Port.makeFrameHIDCan('11512580f6'),1 );
    // Port.writeByQueueAndWait(Buffer.concat( [ Buffer.from([0x21,0x00,0x00,0x25,0x80 ]) ],64), 500);
}

function writeSn(data) {
    let cmd = makeFrameForWrite('1701', data);
    // console.log("writeSn",cmd);  // 17011F4352583130422e3235302e534e2e55332e30413031463854373232393533333B
    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        setTimeout(()=>{
            Port.usbSDK.client.config('04B0').then(res =>{
            }).then(res =>{
                // console.log(res);
            }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },10);
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.ctl.basicInfo()
                .then(res =>{
                    // console.log(res);
                }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },140);
        setTimeout(()=>{
            Port.usbSDK.client.config('2580').then(res =>{
            }).then(res =>{
                // console.log(res);
                setTimeout(()=>{
                    Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                        .then(res=>{
                            // console.log(res);
                            /**
                            buffer: Uint8Array(64) [17, 0, 0, 34, 1, 31, 67, 82, 88, 49, 48, 66, 46, 50, 53, 48, 46, 83, 78, 46, 85, 51, 46, 48, 65, 48, 49, 70, 56, 84, 55, 50, 50, 57, 53, 51, 51, 59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                            data: "011f4352583130422e3235302e534e2e55332e30413031463854373232393533333b"
                            dataPacks: {}
                            hex: "11000022011f4352583130422e3235302e534e2e55332e30413031463854373232393533333b0000000000000000000000000000000000000000000000000000"
                             **/

                            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                            // if()
                            if(res.data.toUpperCase() === cmd.substring(2).toUpperCase()){
                                process(res.data);
                            }else{
                                Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                                    .then(res=>{
                                        if(res.data.toUpperCase() === cmd.substring(2).toUpperCase()){
                                            process(res.data);
                                        }
                                    }).catch(err =>{

                                }).finally(()=>{
                                });
                            }
                        }).catch(err =>{

                        console.log(err);
                        if (err == 'Timeout'){
                            Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                                .then(res=>{
                                    // console.log(res);
                                    process(res.data);
                                }).catch(err =>{

                            }).finally(()=>{
                            });
                        }
                    }).finally(()=>{
                    });
                },50);
            }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },240);


    } else {
        setup();
        Port.writeByQueueAndWait(cmd, 300);
    }
}

function writeParam(limitVolt, limitCur, curArray, spdArray, wheelDia, spdMeterType, spdMeterSignalNum) {
    let paramStr = '';
    paramStr += limitVolt ? Util.sprintf('%02X', limitVolt) : '00';
    paramStr += limitCur ? Util.sprintf('%02X', limitCur) : '00';
    for (let i = 0; i < 10; i++) {
        let cur = curArray[i];
        paramStr += cur ? Util.sprintf('%02X', parseInt(cur)) : '00';
    }
    for (let i = 0; i < 10; i++) {
        let spd = spdArray[i];
        paramStr += spd ? Util.sprintf('%02X', parseInt(spd)) : '00';
    }
    paramStr += wheelDia ? Util.sprintf('%02X', wheelDia) : '00';
    let spdMeter = Util.sprintf('%05b', spdMeterSignalNum) + spdMeterType + '0';
    paramStr += spdMeter ? Util.sprintf('%02X', parseInt(spdMeter, 2)) : '00';


    let cmd = makeFrameForWriteHex('1652', paramStr);

    // console.log('param', cmd);
    // Port.writeByQueueAndWait(cmd, 200);

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        setTimeout(()=>{
            Port.usbSDK.client.config('04B0').then(res =>{
            }).then(res =>{
                // console.log(res);
            }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },10);
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.ctl.basicInfo()
                .then(res =>{
                    // console.log(res);
                }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },110);
        setTimeout(()=>{
            Port.usbSDK.client.config('2580').then(res =>{
            }).then(res =>{
                // console.log(res);
                setTimeout(()=>{
                    Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                        .then(res=>{
                            // console.log(res);
                            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                            // if()
                            if(res.data.toUpperCase() === cmd.substring(2)){
                                process(res.data);
                            }else{
                                Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                                    .then(res=>{
                                        if(res.data.toUpperCase() === cmd.substring(2)){
                                            process(res.data);
                                        }
                                    }).catch(err =>{

                                }).finally(()=>{
                                });
                            }
                        }).catch(err =>{

                        console.log(err);
                        if (err == 'Timeout'){
                            Port.usbSDK.machineInformation.uart.ctl.update(cmd)
                                .then(res=>{
                                    // console.log(res);
                                    process(res.data);
                                }).catch(err =>{

                            }).finally(()=>{
                            });
                        }
                    }).finally(()=>{
                    });
                },50);
            }).catch(err =>{

                console.log(err);
            }).finally(()=>{
            });
        },210);


    } else {
        setup();
        Port.writeByQueueAndWait(cmd, 300);
    }

}


// function processCan( arr, type ,cmd) {
//     switch (tem.substr(0, 8)) {
//         case "02050500":
//             arr[long] = Util.hexToString(data);
//             Port.writeCan(makeFrameCan("02",cmd));
//             break;
//         case "02050600":
//             arr[long] = Util.hexToString(data);
//             if( typeof(checkData(arr))==='string'  ){
//                 ctrlCan[type] = checkData(arr);
//             }
//             step = 0;
//             Port.writeCan(makeFrameCan("02", cmd));
//             break;
//     }
// }


function makeFrameCan(as, cmd) {
    let cmdTemp = Port.makeIndex( '0502' + as + cmd ) + '00';
    return Port.makeFrameHIDCan('15',cmdTemp,'ctrl');
}

function answerCan(cmd) {
    Port.writeCan(makeFrameCan("02", cmd));
}

// 位置传感器标定
function sensorSet() {
    Port.writeCan(makeFrameCan("02", 6200));
}


function writeSpeedWheel(spd, wheel, cir) {

    // setTimeout(()=>{
        let spdStr =  Buffer.from( [(spd>>8)&0xff, spd&0xff]).reverse().toString('hex');
        let cirBuf =  Buffer.from( [(cir>>8)&0xff, cir&0xff]).reverse().toString('hex');
        Port.writeByQueueAndWait(makeFrameCanWrite('00','3203', spdStr + wheel +cirBuf), 100);

    // },500);

}

function getBus() {
    return bus;
}

function getCan() {
    return isCan;
}

function setCan(val){
    isCan = val;
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
    setTimeout(()=>{
        Port.usbSDK.client.config('04B0').then(res =>{
            }).then(res =>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
    },10);
    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.ctl.basicInfo()
            .then(res =>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
    },140);
    setTimeout(()=>{
        Port.usbSDK.client.config('2580').then(res =>{
            }).then(res =>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
    },500);
    setTimeout(()=>{
        readBasic(param);
    },600);

}

let uartHmiBus;

function readBasic(param){
    let readKey = [];
    let retryTime ;
    let basicTime = 380;
    // Port.usbSDK.common.timeout.UART = 380;
    if(param && param === 'b'){
        readKey = [
            "model",
            "sn",
            "version",
            "hardware_version",
        ];
        retryTime = 5 * basicTime + 50 ;
    }else{
        readKey = [
            "model",
            "sn",
            "version",
            "hardware_version",
            "limit_volt",
            "man_name"
        ];
        retryTime = 7 * basicTime + 50 ;
    }
    let check = {};

    let ack = function (key) {
        // console.log('ack', check, key);
        check[key] = true;
    };

    readCtl.init();

    readKey.forEach(key => {
        check[key] = false;
        readCtl[key]();
    });

    uartHmiBus.subscribe(res => {
        if (!res.data) {
            return;
        }
        if (res.type ==='data' && res.data.hasOwnProperty("version")) {
            process(res.data["version"]);
            ack('version');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("hardware_version")) {
            process(res.data["hardware_version"]);
            ack('hardware_version');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("model")) {
            process(res.data["model"]);
            ack('model');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("sn")) {
            process(res.data["sn"]);
            ack('sn');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("man_name")) {
            process(res.data["man_name"]);
            ack('man_name');
        }
        if (res.type ==='data' && res.data.hasOwnProperty("limit_volt")) {
            process(res.data["limit_volt"]);
            ack('limit_volt');
        }
    });

    let count = 0;
    let checkInterval = setInterval(()=>{
            for (let key in check) {
                if (!check[key]) {
                    readCtl[key]();
                    // console.log('retry', key);
                }
            }
            count++;
            if (count > 2) {
                clearInterval(checkInterval);
            }
        }, retryTime);
}

let readCtl = {

    "init":function(){
        // console.log("init");
        uartHmiBus = new Rx.Subject();
    },
    "version":function(){
        Port.usbSDK.machineInformation.uart.ctl.softwareVersionNumber()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'version':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    },
    "hardware_version":function(){
        Port.usbSDK.machineInformation.uart.ctl.hardwareVersionNumber()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'hardware_version':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    },
    "model":function(){
        Port.usbSDK.machineInformation.uart.ctl.modelNumber()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'model':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    },
    "sn":function(){
        Port.usbSDK.machineInformation.uart.ctl.serialNumber()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'sn':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    },
    "limit_volt":function(){
        // 1152
        Port.usbSDK.machineInformation.uart.ctl.parameters()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'limit_volt':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    },
    "man_name":function(){
        // 11512580f6
        Port.usbSDK.machineInformation.uart.ctl.basicInfo()
            .then(res=>{
                if(Port.checkUartSum(2,res.data)){
                    uartHmiBus.next({type: 'data', data: {'man_name':res.data}});
                }
            }).catch(err =>{
            console.log(err);
        }).finally(()=> {
        });
    }

};



const Ctrl = {
    port: Port,
    connect: connect,
    getBus: getBus,

    sendCmdGetFullData: sendCmdGetFullData,
    sendCmdGetBaseData: sendCmdGetBaseData,
    sendCmdGetTestData: sendCmdGetTestData,
    writeSn: writeSn,
    writeParam: writeParam,
    writeSpeedWheel: writeSpeedWheel,

    getCan: getCan,
    setCan: setCan,

};

export default Ctrl;



//////////////////
// WEBPACK FOOTER
// ./device/controller.js
// module id = 79
// module chunks = 73