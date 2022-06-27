import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'

const CMD_SET_MODE_CTRL_PASS_THROUGH = 'AA010201010D0B'; // 控制器透传
const CMD_SET_MODE_IN_BAT = 'aa0917092603490d0b'; // 进入读电池
const CMD_SET_MODE_OUT_BAT = 'aa0917102703510d0b'; // 退出读电池

const CMD_GET_INNER_TEMP = '08'; // 温度
const CMD_GET_VOLTAGE = '09'; // 电压
const CMD_GET_CURRENT = '0A'; // 实时电流
const CMD_GET_RELATIVE_CAPACITY = '0D'; // 相对容量
const CMD_GET_ABSOLUTE_CAPACITY = '0E'; // 绝对容量
const CMD_GET_LEFT_CAPACITY = '0F'; // 剩余容量
const CMD_GET_FULL_CAPACITY = '10'; // 满电容量
const CMD_GET_LOOP_TIME = '17'; // 循环次数
const CMD_GET_BATTERY_1_TO_7 = '24'; // 1~7节电池电压
const CMD_GET_BATTERY_8_TO_14 = '25'; // 8~14节电池电压
const CMD_GET_CHARGE_INTERVAL = '47'; // 当前充电间隔时间
const CMD_GET_MAX_CHARGE_INTERVAL = '48'; // 最大充电间隔时间
const CMD_GET_MODEL = '50'; // 型号
const CMD_GET_SOFTWARE_VERSION = '51'; // 软件版本号
const CMD_GET_HARDWARE_VERSION = '53'; // 硬件版本号
const CMD_GET_SN = '52'; // 序列号

//  Serial Port
let connBus;
let bus;
let ackBus;
let pendingProcessData = '';
let isCan = false;
let n = 0;
let count = true;
let canTimer = null;

let step = 0;

let snWriteLength = 0;
let snWriteCount = 0;

let Arr1to7 = [0, 0, 0, 0, 0, 0, 0];
let Arr8to14 = [0, 0, 0, 0, 0, 0, 0];

// USB Hid
let usbDevice = false;

function connect(comName) {
    if(comName){
        usbDevice = false;
        Port.setUSBDevice(false);
        connBus = Port.connect(comName, 57600);
    }else{
        // usbDevice = true;

    }

    count = true;

    connBus.subscribe(item => {
        switch (item.type) {
            case 'open':
            case 'connect':
                {
                    console.log('bat connect');
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
                            console.log("Can2 battery");
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
                            console.log( item.data.substr( 8, dataLen*2 ),'data');
                            pendingProcessData += item.data.substr( 8, dataLen*2 );
                        }else {
                            pendingProcessData += item.data;
                        }
                        parse();
                    }

                    //console.log(item.data);
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
    let mode = Port.getMode();

    // console.log('battery', mode , pendingProcessData);

    if (mode !== CMD_SET_MODE_IN_BAT) {
        return;
    }

    let data = pendingProcessData;

    while (data.length > 0) {
        let length = parseInt(data.substring(6, 8), 16);
        if (data.length < 2 * length + 16) {
            return;
        }

        let calculatedChecksum = Util.calculateChecksumFFFF(data.substring(2, 2 * length + 8)); // 从第2个字节到数据字节
        let checkSumPart = parseInt(data.substring(2 * length + 8, 2 * length + 12), 16);

        calculatedChecksum = ((calculatedChecksum & 0xFF) << 8) | ((calculatedChecksum & 0xFF00) >> 8);
        if (calculatedChecksum === checkSumPart) {
            let frame = data.substring(0, 2 * length + 16);
            // Log.d(`parse out frame: ${frame}`);
            process(frame);
            data = data.substring(2 * length + 16);
            pendingProcessData = data;
            continue;
        }

        data = data.substring(2);
    }
}

function process(frame) {
    // Port.ack();
    // if(Port.getUSBDevice()){
    //     frame = '0000' + frame;
    // }
    let wrapper = {};

    let functionPart = parseInt(frame.substring(4, 6), 16);
    // console.log('function', functionPart)
    let length = parseInt(frame.substring(6, 8), 16);
    let data = frame.substring(8, length * 2 + 8);

    let battery = {};

    switch (functionPart) {
        case 0x08:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                battery['inner_temperature'] = Util.sprintf("%.1f", (parseInt(data, 16) - 2731) * 0.1);

            }
            break;

        case 0x09:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                battery['total_volt'] = parseInt(data, 16);
            }
            break;

        case 0x0a:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                let currentBinaryLen = parseInt(data, 16).toString(2).length;
                battery['real_current'] = currentBinaryLen < 16 ? parseInt(data, 16) : -(~parseInt(data, 16) + 1 + Math.pow(2, 16));
            }
            break;

        case 0x0d:
            {
                battery['relative_capacity'] = parseInt(data.substring(0, 2), 16);
            }
            break;

        case 0x0e:
            {
                battery['absolute_capacity'] = parseInt(data.substring(0, 2), 16);
            }
            break;

        case 0x0f:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                battery['left_capacity'] = parseInt(data, 16);
            }
            break;

        case 0x10:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                battery['full_capacity'] = parseInt(data, 16);
            }
            break;

        case 0x17:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                battery['loop_time'] = parseInt(data, 16);
            }
            break;

        case 0x24:
            {
                battery['1to7'] = [];
                for (let i = 0; i < 7; i++) {
                    let str = data.substring(i * 4 + 2, i * 4 + 4) + data.substring(i * 4, i * 4 + 2);
                    battery['1to7'][i] = parseInt(str, 16);
                }
            }
            break;

        case 0x25:
            {
                battery['8to14'] = [];
                for (let i = 0; i < 7; i++) {
                    let str = data.substring(i * 4 + 2, i * 4 + 4) + data.substring(i * 4, i * 4 + 2);
                    battery['8to14'][i] = parseInt(str, 16);
                }
            }
            break;

        case 0x47:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                let temp = parseInt(data.substring(0, 4), 16);
                battery['cur_charge_interval'] = Math.floor(temp / 24);
            }
            break;

        case 0x48:
            {
                data = data.substring(2, 4) + data.substring(0, 2);
                let temp = parseInt(data.substring(0, 4), 16);
                battery['max_charge_interval'] = Math.floor(temp / 24);
            }
            break;

        case 0x50:
            {
                battery['model'] = Util.hexToString(data).trim();
            }
            break;

        case 0x51:
            {
                battery['software_version'] = Util.hexToString(data).trim();
            }
            break;

        case 0x52:
            {
                console.log('battery sn raw', data);
                // data.replace(/0+$/g,"");
                let tempData = '';
                for (let i = data.length / 2 - 1; i > 0; i--) {
                    if (data.substring(2 * i, 2 * i + 2) === "00") {
                        tempData = data.substring(0, 2 * i);
                    }
                }
                data = tempData === '' ? data : tempData;
                console.log('battery sn data', data);
                battery['sn'] = Util.hexToString(data).trim();
                console.log('battery sn', battery['sn']== ' ', battery['sn'].length);
            }
            break;

        case 0x53:
            {
                battery['hardware_version'] = Util.hexToString(data).trim();
            }
            break;

        default:
            {
                battery['raw'] = data;
            }
    }

    wrapper['battery'] = battery;

    bus.next({ type: 'data', data: wrapper });
    if (ackBus) {
        ackBus.next({ type: 'data', data: wrapper });
    }
}


function setup() {
    if (Port.getMode() == CMD_SET_MODE_IN_BAT) {
        return;
    }
    Port.setMode(CMD_SET_MODE_IN_BAT);
    let modeList = [CMD_SET_MODE_CTRL_PASS_THROUGH, CMD_SET_MODE_IN_BAT];
    modeList.forEach((item) => {
        Port.writeByQueueAndWait(item, 300);
    });
}

function makeReadFrame(cmd) {
    let frame =  null;

    let check = 0x16 + parseInt(cmd, 16) + 0x01 + 0x00;
    check = ((check & 0xFF) << 8) | ((check & 0xFF00) >> 8);
    check = Util.sprintf('%04X', check);
    let inner = '3A16' + cmd + '0100' + check + '0D0A';

    let innerLength = inner.length / 2;

    let checkBuf = Buffer(inner, 'hex');
    let checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum += checkBuf.length;
    checkSum = checkSum & 0xff;

    if(usbDevice){
        frame = Port.makeFrameHID('14',inner);
    }else {
        frame = 'AA09' + inner + Util.sprintf('%02X', innerLength) + Util.sprintf('%02X', checkSum) + '0D0B';
    }

    return frame;
}

function makeWriteFrame(cmd, dataHex) {
    let frame =  null;

    let length = Buffer(dataHex, 'hex').length;
    let checkHex = '17' + cmd + Util.sprintf('%02X', length) + dataHex;

    let checkBuf = Buffer(checkHex, 'hex');
    let checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum = ((checkSum & 0xFF) << 8) | ((checkSum & 0xFF00) >> 8);

    let inner = '3A17' + cmd + Util.sprintf('%02X', length) + dataHex + Util.sprintf('%04X', checkSum) + '0D0A';
    console.log('frame', inner);

    let innerLength = inner.length / 2;
    checkBuf = Buffer(inner, 'hex');
    checkSum = 0;
    for (let j = 0; j < checkBuf.length; j++) {
        checkSum = checkSum + checkBuf[j];
    }
    checkSum += checkBuf.length;
    checkSum = checkSum & 0xff;
    if(Port.getUSBDevice()) {
        frame = inner;
    }else{
        frame = 'AA09' + inner + Util.sprintf('%02X', innerLength) + Util.sprintf('%02X', checkSum) + '0D0B';
    }
    return frame;
}

function sendCmdGetFullData() {

    if (Port.getUSBDevice()) {
        sendCmdGetFullDataUsb();
    } else {

        setup();

        let keyData = {
            'sn': makeReadFrame(CMD_GET_SN),
            'model': makeReadFrame(CMD_GET_MODEL),
            'software_version': makeReadFrame(CMD_GET_SOFTWARE_VERSION),
            'hardware_version': makeReadFrame(CMD_GET_HARDWARE_VERSION),
            'real_current': makeReadFrame(CMD_GET_CURRENT),
            'total_volt': makeReadFrame(CMD_GET_VOLTAGE),
            'inner_temperature': makeReadFrame(CMD_GET_INNER_TEMP),
            'left_capacity': makeReadFrame(CMD_GET_LEFT_CAPACITY),
            'full_capacity': makeReadFrame(CMD_GET_FULL_CAPACITY),
            'cur_charge_interval': makeReadFrame(CMD_GET_CHARGE_INTERVAL),
            'max_charge_interval': makeReadFrame(CMD_GET_MAX_CHARGE_INTERVAL),
            'loop_time': makeReadFrame(CMD_GET_LOOP_TIME),
            '1to7': makeReadFrame(CMD_GET_BATTERY_1_TO_7),
            '8to14': makeReadFrame(CMD_GET_BATTERY_8_TO_14),
            'relative_capacity': makeReadFrame(CMD_GET_RELATIVE_CAPACITY),
            'absolute_capacity': makeReadFrame(CMD_GET_ABSOLUTE_CAPACITY),
        };

        let ack = Port.writeAndEnsure(keyData, 300);

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.battery) {
                let battery = res.data.battery;
                for (let key in battery) {
                    ack(key);
                }
            }
        });

        // 最后发退出电池的指令
        setTimeout(function() {
            Port.writeByQueueAndWait(CMD_SET_MODE_OUT_BAT, 300);
            Port.writeByQueueAndWait(CMD_SET_MODE_OUT_BAT, 300);
        }, 14000);
    }
}

function sendCmdGetBaseData() {

    if (Port.getUSBDevice()) {
        sendCmdGetBaseDataUsb();
    } else {
        setup();

        let keyData = {
            'sn': makeReadFrame(CMD_GET_SN),
            'model': makeReadFrame(CMD_GET_MODEL),
            'software_version': makeReadFrame(CMD_GET_SOFTWARE_VERSION),
            'hardware_version': makeReadFrame(CMD_GET_HARDWARE_VERSION),
        };

        let ack = null;

        if (usbDevice) {
            Port.writeByQueueAndWait( Buffer.concat( [Buffer.from([0x21,0x00,0x00,0x25,0x80 ]) ],64), 250);
            ack = Port.writeAndEnsure(keyData, 250);
        } else {
            ack = Port.writeAndEnsure(keyData, 300);
        }

        ackBus = new Rx.Subject();
        ackBus.subscribe(res => {
            if (res.data && res.data.battery) {
                let battery = res.data.battery;
                for (let key in battery) {
                    ack(key);
                }
            }
        });

        // 最后发退出电池的指令
        setTimeout(function() {
            Port.writeByQueueAndWait(CMD_SET_MODE_OUT_BAT, 300);
        }, 10000);
    }
}

function writeSn(data) {
    let dataHex = Util.stringToHex(data);
    let frame = makeWriteFrame(CMD_GET_SN, dataHex);

    if (Port.getUSBDevice()) {
        bus = new Rx.Subject();
        if(setModeStatus){
            setTimeout(()=>{
                setModeOut();
            },100);
        }
        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.battery.setModeIn()
                .then(res => {
                    console.log(res);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
                console.log('response setModeIn');
            });
        },300);

        setTimeout(()=>{
            Port.usbSDK.client.config('2580')
                .then(res => {
                    console.log(res);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
                console.log('response config');
            });
        },600);

        setTimeout(()=>{
            Port.usbSDK.machineInformation.uart.battery.update(frame)
                .then(res=>{
                    console.log(res);
                    process(res.data);
                }).catch(err =>{

                console.log(err);
                if (err == 'Timeout'){
                    Port.usbSDK.machineInformation.uart.battery.update(frame)
                        .then(res=>{
                            console.log(res);
                            process(res.data);
                        }).catch(err =>{

                    }).finally(()=>{
                    });
                }
            }).finally(()=>{
            });
        },900);

        setTimeout(()=>{
            setModeStatus = true;
            setModeOut();
        },1200);

    } else {
        setup();


        Port.writeByQueueAndWait(frame, 200);

        // 最后发退出电池的指令
        setTimeout(function() {
            Port.writeByQueueAndWait(CMD_SET_MODE_OUT_BAT, 300);
        }, 2000);
    }
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
let setModeStatus = false;

function sendCmdGetBaseDataUsb() {
    console.log("status",setModeStatus);
    bus = new Rx.Subject();
    if(setModeStatus){
        setTimeout(()=>{
            setModeOut();
        },100);
    }
    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.battery.setModeIn()
            .then(res => {
                Port.setMode(CMD_SET_MODE_IN_BAT);
                console.log(res);
            }).catch(err => {

            console.log(err);
        }).finally(() => {
            console.log('response setModeIn');
        });
    },300);

    setTimeout(()=>{
        Port.usbSDK.client.config('2580')
            .then(res => {
                console.log(res);
            }).catch(err => {

            console.log(err);
        }).finally(() => {
            console.log('response config');
        });
    },600);

    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.battery.serialNumber()
            .then(res => {
                console.log(res);
                // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
                process(res.data);
            }).catch(err => {

            console.log(err);
            if(err == 'Timeout'){
                Port.usbSDK.machineInformation.uart.battery.serialNumber()
                    .then(res => {
                        console.log(res);
                        process(res.data);
                    }).catch(err => {

                    console.log(err);
                }).finally(() => {
                });
            }
        }).finally(() => {
        });
    },900);

    setTimeout(()=>{
        setModeStatus = true;
        setModeOut();
    },1200);
}


function sendCmdGetFullDataUsb() {
    /*
     buffer:Uint8Array(64) [16, 0, 0, 17, 16, 14, 68, 80, 67, 49, 48, 75, 49, 48, 51, 48, 48, 54, 46, 48, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
     data:"100e44504331304b3130333030362e3029"
     dataPacks:{}
     hex:"10000011100e44504331304b3130333030362e302900000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
     */
    console.log("status",setModeStatus);
    bus = new Rx.Subject();
    if(setModeStatus){
        setTimeout(()=>{
            setModeOut();
        },100);
    }
    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.battery.setModeIn()
            .then(res => {
                Port.setMode(CMD_SET_MODE_IN_BAT);
                console.log(res);
            }).catch(err => {

            console.log(err);
        }).finally(() => {
            console.log('response setModeIn');
        });
    },300);

    setTimeout(()=>{
        Port.usbSDK.client.config('2580')
            .then(res => {
                console.log(res);
            }).catch(err => {

            console.log(err);
        }).finally(() => {
            console.log('response config');
        });
    },600);

    setTimeout(()=>{
        readBasic();
    },900);

}

function readBasic(){
    setTimeout(()=>{
        console.log("Count",readCount);
        if(readCount > 10){
            setModeStatus = true;
            setModeOut();
        }else{
            setTimeout(()=>{
                setModeStatus = true;
                setModeOut();
            },6000)
        }
    },6000);
    let readCount = 0;
    Port.usbSDK.machineInformation.uart.battery.softwareVersionNumber()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.softwareVersionNumber()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.hardwareVersionNumber()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.hardwareVersionNumber()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.modelNumber()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.modelNumber()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.serialNumber()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.serialNumber()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.tempreature()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.tempreature()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.voltage()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.voltage()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.current()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.current()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.relativeCapacity()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.relativeCapacity()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.absoluteCapacity()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.absoluteCapacity()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.capacityLeft()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.capacityLeft()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.fullCapacity()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.fullCapacity()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.cycleCount()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.cycleCount()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.cells1to7()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.cells1to7()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.cells8to14()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.cells8to14()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.currentChargeInterval()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.currentChargeInterval()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
    Port.usbSDK.machineInformation.uart.battery.maxChargeInterval()
        .then(res => {
            // console.log(res);
            readCount++;
            // uartHmiBus.next({type: 'data', data: {'model':res.dataParsed}});
            process(res.data);
        }).catch(err => {

        console.log(err);
        if(err == 'Timeout'){
            Port.usbSDK.machineInformation.uart.battery.maxChargeInterval()
                .then(res => {
                    // console.log(res);
                    process(res.data);
                }).catch(err => {

                console.log(err);
            }).finally(() => {
            });
        }
    }).finally(() => {
    });
}

function setModeOut(){
    Port.usbSDK.client.config('2580')
        .then(res => {
            console.log(res);
        }).catch(err => {

        console.log(err);
    }).finally(() => {
        console.log('response config');
    });
    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.battery.setModeOut()
            .then(res => {
                console.log(res);
                setModeStatus = false;
            }).catch(err => {

            console.log(err);
        }).finally(() => {
            console.log('response setModeOut');
        });
    },300)
}


const Battery = {
    port: Port,
    connect: connect,
    getBus: getBus,

    setup: setup,
    sendCmdGetFullData: sendCmdGetFullData,
    sendCmdGetBaseData: sendCmdGetBaseData,
    writeSn: writeSn,
    setModeOut: setModeOut,

    getCan: getCan,
    setCan: setCan,

};

export default Battery;



//////////////////
// WEBPACK FOOTER
// ./device/battery.js
// module id = 76
// module chunks = 73