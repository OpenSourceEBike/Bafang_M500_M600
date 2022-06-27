import Util from '../core/js/util'
import Core from '../core/core.js'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx'
import usbSDK from 'besst-usb-sdk'

const os = require('os');
//
// // CPU 的字节序
// console.log('endianness : ' + os.endianness());
//
// // 操作系统名
// console.log('type : ' + os.type());
//
// // 操作系统名
// console.log('platform : ' + os.platform());
//
// // 系统内存总量
// console.log('host name : ' + os.hostname() ) ;
//
// // 操作系统空闲内存量
// console.log('free memory : ' + os.freemem() + " bytes.");

// // 获取
function getNetMac() {
    try {
        let netIn = os.networkInterfaces();

        let keys = Object.keys(netIn);

        for (let i = 0; i < keys.length; i++) {
            let data = netIn[keys[i]];
            console.log(keys[i]);
            console.log(data);
        }
    } catch (err) {
        console.log('Error Mac', err);
    }
}
// getNetMac();
//
//
// try{
const SerialPort = require('serialport');
// const HID = require('node-hid');
// }catch(err){
//     console.log(err);
// }



const connection = {
    'com': false,
    'usb': false,
    'can': false,
    'uart': false,
    'hmi': false,
    'controller': false,
    'sensor': false,
    'update': false,
};


//  -- SerialPort
let portDevice = false;

const CMD_SET_MODE_RESET = 'AAFE9861DD9BF0';

let portInstance = null;
let portName = null;
let portBaudRate = null;
let bus = null;
let status = null;
let lastWriteTime = null;
let mode = null;
let isGettingPortList = false;
let can = false;

// USB Hid
let usbDevice = false;


function getPortList() {
    let portBus = new Rx.Subject();

    // if (isGettingPortList) {
    //     return portBus;
    // }
    // isGettingPortList = true;
    let list = [];
    // let isFoundBeestPort = false;
    try {
        SerialPort.list(function(err, ports) {
            // let index = 0;
            ports.forEach(function(port) {
                if (port.manufacturer == 'Silicon Labs') {
                    // console.log("SerialPort",port);
                    list.push(port.comName);
                    portDevice = true;
                    connection.com = true;
                    connection.usb = false;
                }
            });
            portBus.next({ type: 'port', data: list });
            // let connectInterval = setInterval(function() {
            //     if (isFoundBeestPort || index > list.length - 1) {
            //         return;
            //     }
            //     let port = list[index];
            //     disConnect();
            //     setTimeout(function() {
            //         let connBus = connect(port, 57600);
            //         connBus.subscribe(item => {
            //             if (isFoundBeestPort) {
            //                 return;
            //             }
            //             switch (item.type) {
            //                 case 'open':
            //                 case 'connect':
            //                     {
            //                         writeByQueueAndWait('AA040101010D0B', 0);
            //                     }
            //                     break;

            //                 case 'data':
            //                     {
            //                         if (item.data.indexOf('AA') > -1) {
            //                             console.log('find com')
            //                             isFoundBeestPort = true;
            //                             portBus.next({ type: 'port', data: [port] });
            //                             clearInterval(connectInterval);
            //                             isGettingPortList = false;
            //                         }
            //                     }
            //                     break;

            //                 default:
            //                     {
            //                         portBus.next(item);
            //                     }
            //             }
            //         });
            //     }, 700);

            //     index++;
            //     if (index > list.length - 1 && !isFoundBeestPort) {
            //         portBus.next({ type: 'port', data: list });
            //         clearInterval(connectInterval);
            //         isGettingPortList = false;
            //     }
            // }, 1400);
        });
    } catch (err) {
        console.log(err);
    }

    return portBus;
}

let portEventHandler = {
    onOpen: function(err) {
        Log.d(`port event open`);
        bus.next({ type: 'open' });
        // bus.next({ type: 'port', data: {name: portName, status: true} });
        status = 'open';
    },
    onData: function(data) {
        // Log.d(`port event data: ${data.toString('hex').toUpperCase()}`);
        let dataTem = data.toString('hex').toUpperCase();
        bus.next({ type: 'data', data: dataTem });
        // if (dataTem.match('AA0C')) {
        //     can = true;
        // } else if (dataTem.match('AA08')) {
        //     can = false;
        // }
        // console.log(can);
        status = 'data';

    },

    onClose: function() {
        Log.d(`port event close`);
        bus.next({ type: 'close' });
        // bus.next({ type: 'port', data: {name: portName, status: false} });
        status = 'close';
        portInstance = null;
        portDevice = false;
        if (dataQueueWriter.timer) {
            clearInterval(dataQueueWriter.timer)
        }
        startConnection();
    },

    onError: function(error) {
        Log.d(`port event error`, error);
        bus.next({ type: 'error', error: error });
        // bus.next({ type: 'port', data: {name: portName, status: false} });
        status = 'error';
        portDevice = false;
        if (dataQueueWriter.timer) {
            clearInterval(dataQueueWriter.timer)
        }
        startConnection();
    }
};

function connect(name, baudRate) {
    Log.d(`connect port: ${name}, baudRate: ${baudRate}`);

    if (portName === name) {
        console.log('1');
        // console.log(portInstance, bus);
        if (portInstance && portInstance.isOpen && bus) {

            Log.d(`already connected`);
            bus = new Rx.Subject();

            setTimeout(() => {
                // console.log('con connect')
                bus.next({ type: 'connect', data: { mode: mode } }); // 连接其他类型设备，重新connect
            }, 10);

            return bus;
        }
    } else {
        console.log('2');
        disConnect();
    }

    let port;
    try {
        port = new SerialPort(name, { baudRate: baudRate });
    } catch (err) {
        console.log(err);
    }

    if (port) {
        port.on('open', portEventHandler.onOpen);
        port.on('data', portEventHandler.onData);
        port.on('close', portEventHandler.onClose);
        port.on('error', portEventHandler.onError);
        // port.on('readable', function () {
        //     console.log('Data:', port.read());
        // });
    }

    portInstance = port;
    portName = name;
    portBaudRate = baudRate;

    dataQueueWriter.init();

    console.log('connect', name, 'done');
    // console.log(portInstance);

    if (!bus) {
        bus = new Rx.Subject();
    }

    return bus;
}

function disConnect() {
    if (portInstance && portInstance.isOpen) {
        Log.d(`closing port ${portName}`);

        portInstance.close(function() {
            Log.d(`real close port ${portName}`);
        });

        portInstance = null;
        portName = null;
    }
}

let writeInterval = 0;

function writeBySingleByte(hexData, callback) {
    // Log.d(`write`);
    if (isReadyForWrite()) {
        // Log.d(`real write: ${hexData}`);

        let i = 0;
        writeInterval = setInterval(() => {
            if (i < Math.floor(hexData.length / 2)) {
                let byte = hexData.substring(i * 2, i * 2 + 2);
                let buf = Buffer.from(byte, 'hex');
                // console.log(`write byte: ${byte}`, buf);
                // portInstance.write(byte, 'hex');
                portInstance.write(buf);
                i++;
            } else {
                clearInterval(writeInterval);
                writeInterval = 0;
                if (Util.isFunction(callback)) {
                    callback();
                }
            }
        }, 10);
    }
}

function write(data, callback) {
    // Log.d(`write`);
    if (usbDevice) {
        // arrData
    } else {
        if (isReadyForWrite()) {
            // hexData
            // Log.d(`real write: ${data}`);
            let buf = Buffer.from(data, 'hex');
            // console.log(`write hexData: ${hexData}`, buf);
            // portInstance.write(hexData, 'hex');
            portInstance.write(buf, callback);
        }
    }
}

function writeCan(data) {
    // Log.d(`write`);
    if (usbDevice) {
        // arrData
    }
}

function getBus() {
    return bus;
}

function isReadyForWrite() {
    return portInstance && status === 'open' || status === 'data';
}

function setMode(m) {
    mode = m;
}

function getMode() {
    return mode;
}

let resetStatus = false;

function reset(waitInterval) {
    mode = '';
    resetStatus = true;
    setTimeout(() => {
        resetStatus = false;
        // checkUsbConnection();
    }, 2000);
    if (waitInterval) {
        if (usbDevice) {
            usbSDK.client.reset()
                .then(res => {}).catch(err => {
                    console.log(err);
                }).finally(() => {});

        } else {
            writeByQueueAndWait(CMD_SET_MODE_RESET, waitInterval);
        }
    } else {
        if (usbDevice) {
            usbSDK.client.reset()
                .then(res => {}).catch(err => {
                    console.log(err);
                }).finally(() => {});
        } else {
            writeByQueue(CMD_SET_MODE_RESET);
        }
    }
}

function time() {
    return new Date().getTime();
}


// write queue
let pendingWriteItemQueue = [];
let writeCount = 0;
const WRITE_MODE_NON_BLOCK = 0;
const WRITE_MODE_BLOCK = 1;
const dataQueueWriter = {
    timer: 0,

    init: function() {
        if (dataQueueWriter.timer) {
            clearInterval(dataQueueWriter.timer);
        }
        dataQueueWriter.timer = setInterval(dataQueueWriter.run, 45);
    },

    // mode, 0: non-block; 1: block;
    push: function(data, mode, timeout) {
        let now = time();
        let id = now + '-' + (++writeCount);
        timeout = timeout > 0 ? timeout : 3000;
        let item = {
            id: id,
            time: now,
            mode: mode,
            timeout: timeout,
            status: 0,
            data: data
        };
        pendingWriteItemQueue.push(item);
        return id;
    },

    pop: function() {
        if (writeInterval) {
            return;
        }
        while (true) {
            if (pendingWriteItemQueue.length === 0) {
                return;
            }

            let topItem = pendingWriteItemQueue[0];

            let now = time();

            if (topItem.mode === WRITE_MODE_BLOCK) {
                // console.log(topItem);

                let writeBeginTime = topItem['write_begin_time'];
                let timeout = topItem['timeout'];

                // Log.d(`${now - writeBeginTime} ${timeout}`);

                if (writeBeginTime && now - writeBeginTime > timeout) {
                    pendingWriteItemQueue.shift();
                    continue;
                }

                if (topItem.status !== 0) {
                    return;
                }

                topItem.status = 1;
            }

            if (topItem.status === 2) {
                pendingWriteItemQueue.shift();
                continue;
            }

            return topItem;
        }
    },

    ack: function(frame) {
        Log.d(`ack`);
        let arr = [];
        for (let a = 0; a < pendingWriteItemQueue.length; a++) {
            if (frame != pendingWriteItemQueue[a].data) {
                arr.push(pendingWriteItemQueue[a]);
            }
        }
        pendingWriteItemQueue = arr;
        // pendingWriteItemQueue.shift();
        dataQueueWriter.run();

    },

    run: function() {
        // Log.d(`run`);

        let item = dataQueueWriter.pop();
        if (!item) {
            return;
        }

        item['write_begin_time'] = time();

        write(item.data);
        item.status = 2;
        item['write_end_time'] = time();
    }
};

function writeByQueue(hexData) {
    return dataQueueWriter.push(hexData, WRITE_MODE_NON_BLOCK, -1);
}

function writeByQueueAndWait(hexData, timeout) {
    return dataQueueWriter.push(hexData, WRITE_MODE_BLOCK, timeout);
}

function emptyQueue() {
    pendingWriteItemQueue = [];
}

function ack() {
    dataQueueWriter.ack();
}

function isConnected() {
    if (!usbDevice) {
        if (status === 'open' || status === 'data') {
            return true;
        } else if (status === 'close' || status === 'error') {
            return false;
        }
    } else {
        return usbSDK.client.isConnected();
    }
}

function getPort() {
    return portName;
}

function getCan() {
    console.log("Can:", can);
    return can;
}

function setCan(val) {
    console.log("Can:", val);
    can = val;
}

function writeAndEnsure(keyData, waitInterval) {
    let check = {};
    let keys = Object.keys(keyData);

    let ack = function(key) {
        // console.log('ack', key);
        check[key] = true;
    };

    keys.forEach(key => {
        check[key] = false;

        let data = keyData[key];
        writeByQueueAndWait(data, waitInterval);
    });

    let writeCompleteTime = new Date().getTime();

    let lastRewriteTime = 0;
    let lastRewriteCount = 0;
    let totalRetryCount = 0;
    let checkInterval = setInterval(function() {
        let now = new Date().getTime();
        if (now - writeCompleteTime < waitInterval * keys.length) {
            return;
        }

        if (lastRewriteTime > 0 && now - lastRewriteTime < lastRewriteCount * waitInterval) {
            return;
        }

        totalRetryCount++;
        lastRewriteCount = 0;
        for (let key in check) {
            if (!check[key]) {
                let data = keyData[key];
                console.log('retry', key);
                writeByQueueAndWait(data, waitInterval);
                lastRewriteTime = new Date().getTime();
                lastRewriteCount++;
            }
        }

        if (lastRewriteCount === 0 || totalRetryCount > 2) {
            clearInterval(checkInterval);
        }
    }, 2000);

    return ack;
}

//once
function writeAndEnsureCan(keyData, waitInterval) {
    let check = {};
    let keys = Object.keys(keyData);

    let ack = function(key) {
        check[key] = true;
    };

    keys.forEach(key => {
        check[key] = false;

        let data = keyData[key];
        writeByQueueAndWait(data, waitInterval);
    });

    return ack;
}



//  -- USB HID
var devices = null;
var device = null;
var besst = true;


function getUSBDevice() {
    return usbSDK.client.isConnected();
}

function setUSBDevice(value) {
    usbDevice = value;
}

function timeLog(n) {
    if (n) {
        console.log(new Date().getTime(), n);
    } else {
        console.log(new Date().getTime(), 'timeLog');
    }
}


// HID写入拼接
function makeIndex(data) {

    // let cmd2 = '' + parseInt(data.substr(2, 2), 16).toString(2);
    // let cmd3 = '' + parseInt(data.substr(4, 2), 16).toString(2);
    // let cmd = addZero(cmd2, 5) + addZero(cmd3, 3);
    // let buf = Buffer.concat([Buffer.from(data.substr(0, 2), 'hex'), Buffer.from([parseInt(cmd, 2)]), Buffer.from(data.substr(6, 2), 'hex'), Buffer.from(data.substr(8, 2), 'hex')]);
    //
    // let result = buf.reverse().toString('hex').toUpperCase();
    //
    // // console.log(result, 'make index - write');
    //
    // return result;
}

function addZero(cmd, len) {
    let l = cmd.length;
    for (l; l < len; l++) {
        cmd = '0' + cmd;
    }
    return cmd;
}

// HID读取解析
function processIndex(data) {
    // let tem = Buffer.from(data, 'hex').reverse();
    // let cmd = parseInt(tem.toString('hex').substr(2, 2), 16).toString(2);
    // let cmd1 = cmd.substr(0, cmd.length - 3);
    // let cmd3 = cmd.slice(-3);
    //
    // let str = Buffer.from([tem[0], parseInt(cmd1, 2), parseInt(cmd3, 2), tem[2], tem[3]]).toString('hex').toUpperCase();
    //
    // // console.log(str, 'process index - read' );
    //
    // return str;
}

//  USB HID----》
function makeFrameHID(index, cmd, data) {
    // //  Uart HID 读写
    // let str = index + '0000';
    // let dataArr = Buffer.from(str,'hex');
    // let dataBuf = '';
    // if(data){
    //     let strBuf = Util.stringToHex(data);
    //     let str= cmd + strBuf;
    //     // console.log( str );
    //     dataBuf = Buffer.concat([ dataArr, Buffer.from([str.length/2]), Buffer.from(cmd,'hex'), Buffer.from(strBuf,'hex')],64);
    // }else{
    //     dataBuf = Buffer.concat([ dataArr, Buffer.from([cmd.length/2]), Buffer.from(cmd,'hex') ],64);
    // }
    //
    // // console.log("dataBuf",bufferToArray(dataBuf));
    //
    // return dataBuf;
}

function makeFrameHIDCan(cmd, data) {
    // //  Can HID 读写
    // let cmdStr = cmd +'0000'+data;
    // let dataBuf = Buffer.concat([ Buffer.from(cmdStr,'hex') ],64);
    // // console.log("dataBuf",bufferToArray(dataBuf));
    // return dataBuf;
}

//   Can 打开控制器和仪表开关
let openController = true;
let countOpen = 0;
let HMIInterval = null;

const BESST_Pro = {
    sn: '',
    soft_ver: '',
    hard_ver: ''
};

function getBESST_Pro() {
    return BESST_Pro;
}

function setBESST_Pro(sn, soft, hard) {
    if (sn) {
        BESST_Pro.sn = sn;
    }
    if (soft) {
        BESST_Pro.soft_ver = soft;
    }
    if (hard) {
        BESST_Pro.hard_ver = hard;
    }
}

//  查看检查的USB是什么
function startConnection() {
    // let checkTimeout;
    let count = 0;
    console.log('connection', connection);
    let checkUSBInterval = setInterval(function() {
        getPortList();
        try {
            usbDevice = usbSDK.client.connect() || usbSDK.client.isConnected();
            usbSDK.common.timeout = {
                CAN: 100,
                UART: 360,
                FIRMWARE_UPGRADE: 20000,
                TOOL: 90
            };
            usbSDK.common.logger = {
                trace: false,
                debug: false,
                info: false
            };
        } catch (err) {
            console.log("Connect HID Error", err);
        }
        if (usbDevice) {
            // listenData();
            connection.usb = true;
            connection.com = false;
            timeLog('init');
            // getUsbBoxBasic();
        }
        if (portDevice || usbDevice) {
            clearInterval(checkUSBInterval);
        }
    }, 359);
}

// 开启检测硬件设备连接
startConnection();


// function checkUsbProtocol(){
//     let str = '';
//     let checkUSBInterval = setInterval( function(){
//         try{
//             if( usbDevice ){
//                 usbSDK.client.checkComponents().then(res=>{
//                     console.log(res);
//                     if(res.message){
//                         // 3300000400000000000000000...00
//                         str = res.message.substr(8,8);
//                         console.log(str);
//                     }
//                 }).catch(err =>{
//                     console.log(err);
//                     checkUsbError(err);
//                 }).finally(()=>{
//                 });
//             }
//         }catch(err){
//             console.log("Connect HID Error",err);
//         }
//         if(  false ){
//             clearInterval(checkUSBInterval);
//         }
//     },500);
// }
// checkUsbProtocol();

function checkUsbError(err) {
    // Connection not yet established
    if (err && err.indexOf('not') > -1) {
        try {
            usbSDK.client.connect();
        } catch (err) {
            console.log(err);
        }
    }
}
const boxOperation = {
    read: false,
    diagnosis: false
};

function setBoxOperation() {
    boxOperation.read = false;
    boxOperation.diagnosis = false;
    console.log(boxOperation, "boxOperation");
}

function wakeUpBox(cmdTem) {
    if (!resetStatus) {
        setTimeout(() => {
            usbSDK.client.wakeUp(cmdTem + '0000').then(res => {
                console.log(res, cmdTem);
            }).catch(err => {
                console.log(err);
                checkUsbError(err);
            }).finally(() => {
                console.log('response', cmdTem);
            });
        }, 300)
    } else {
        setTimeout(() => {
            usbSDK.client.wakeUp(cmdTem + '0000').then(res => {
                console.log(res, cmdTem);
            }).catch(err => {
                console.log(err);
                checkUsbError(err);
            }).finally(() => {
                console.log('response', cmdTem);
            });
        }, 1300)
    }
}

function checkUsbConnection(cmd, type) {

    // 默认
    let cmdTem = '0000';
    if (cmd) {
        cmdTem = cmd;
    }

    if (!usbSDK.client.isConnected()) {
        try {
            usbSDK.client.connect();
        } catch (err) {
            console.log(err);
        }
    }

    // wakeUpBox(cmdTem);

    if (type === 1) {
        // read
        if (!boxOperation.read && connection.usb) {
            boxOperation.read = true;
            wakeUpBox(cmdTem);
        }
    } else if (type === 2) {
        //diagnosis
        if (!boxOperation.diagnosis && connection.usb) {
            boxOperation.diagnosis = true;
            wakeUpBox(cmdTem);
        }
    }

    console.log('checkUsbConnection', boxOperation, cmdTem);

    // let result;
    // try{
    //     result = usbSDK.client.connect();
    // }catch(err){
    //     console.log(err);
    // }
    // if(result == false){
    //     usbSDK.client.disconnect();
    //     try{
    //         usbSDK.client.connect();
    //     }catch(err){
    //         console.log(err);
    //     }
    // }
    // return usbSDK.client.isConnected();
}

function getUsbBasic(target, base) {
    let getUsbBasicBus = new Rx.Subject();
    //                    BESSTSDK.client.connect();
    //                    HMI.port.usbSDK.machineInformation.get.hardwareVersionNumber(0x05, this.target)
    //                        .then(res => {
    //                            this.hardVersion = res.dataParsed;
    //                        }).then(res =>{
    //                            this.hardVersion = res.dataParsed;
    //                            return BESSTSDK.machineInformation.get.softwareVersionNumber(BESSTSDK.common.DEVICE.BST, BESSTSDK.common.DEVICE.CTL);
    //                        }).then(res =>{
    //                            this.version = res.dataParsed;
    //                            return BESSTSDK.machineInformation.get.customerNumber(BESSTSDK.common.DEVICE.BST, BESSTSDK.common.DEVICE.CTL);
    //                        }).then(res =>{
    //                            this.clientCodeRead =  res.dataParsed;
    //                            return BESSTSDK.machineInformation.get.modelNumber(BESSTSDK.common.DEVICE.BST, BESSTSDK.common.DEVICE.CTL);
    //                        }).then(res =>{
    //                            this.model =  res.dataParsed;
    //                        }).catch(err =>{
    ////                            debugger;
    //                            console.log(err);
    //                            this.usbErrorCheck(err);
    //                        }).finally(()=>{
    //                        });


    usbSDK.machineInformation.get.serialNumber(0x05, target)
        .then(res => {
            // console.log(res,"serialNumber");
            getUsbBasicBus.next({ type: 'data', data: { 'sn': res.dataParsed.replace(/\s+/g, "") } });
            // this.sn = res.dataParsed;
            // this.readSn = this.sn;
        }).catch(err => {

            console.log(err);
            if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                usbSDK.machineInformation.get.serialNumber(0x05, target)
                    .then(res => {
                        getUsbBasicBus.next({ type: 'data', data: { 'sn': res.dataParsed.replace(/\s+/g, "") } });
                    }).catch(err => {
                        console.log(err);
                    }).finally(() => {
                        console.log('serialNumber 2');
                    });
            }
        }).finally(() => {
            console.log('serialNumber 1');
        });
    usbSDK.machineInformation.get.softwareVersionNumber(0x05, target)
        .then(res => {
            // console.log(res,"softwareVersionNumber");
            getUsbBasicBus.next({ type: 'data', data: { 'version': res.dataParsed } });

            // this.version = res.dataParsed;
        }).catch(err => {

            console.log(err);
            if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                usbSDK.machineInformation.get.softwareVersionNumber(0x05, target)
                    .then(res => {
                        getUsbBasicBus.next({ type: 'data', data: { 'version': res.dataParsed } });
                    }).catch(err => {
                        console.log(err);
                    }).finally(() => {
                        console.log('softwareVersionNumber 2');

                    });
            }
        }).finally(() => {
            console.log('softwareVersionNumber 1');
        });

    if (!base) {
        usbSDK.machineInformation.get.hardwareVersionNumber(0x05, target)
            .then(res => {
                // console.log(res,"hardwareVersionNumber");
                getUsbBasicBus.next({ type: 'data', data: { 'hardware_version': res.dataParsed } });
                // this.hardVersion = res.dataParsed;
            }).catch(err => {
                console.log(err);
                if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                    usbSDK.machineInformation.get.hardwareVersionNumber(0x05, target)
                        .then(res => {
                            getUsbBasicBus.next({ type: 'data', data: { 'hardware_version': res.dataParsed } });
                        }).catch(err => {
                            console.log(err);
                        }).finally(() => {
                            console.log('hardwareVersionNumber 2');
                        });
                }
            }).finally(() => {
                console.log('hardwareVersionNumber 1');
            });
        usbSDK.machineInformation.get.customerNumber(0x05, target)
            .then(res => {
                getUsbBasicBus.next({ type: 'data', data: { 'client_code': res.dataParsed } });

                // this.clientCodeRead = res.dataParsed;
            }).catch(err => {

                console.log(err);
                if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                    usbSDK.machineInformation.get.customerNumber(0x05, target)
                        .then(res => {
                            getUsbBasicBus.next({ type: 'data', data: { 'client_code': res.dataParsed } });
                        }).catch(err => {
                            console.log(err);
                        }).finally(() => {
                            console.log('customerNumber 2');
                        });
                }
            }).finally(() => {
                console.log('customerNumber 1');
            });
        usbSDK.machineInformation.get.modelNumber(0x05, target)
            .then(res => {
                // console.log(res,"modelNumber");
                getUsbBasicBus.next({ type: 'data', data: { 'model': res.dataParsed } });

                // this.model = res.dataParsed;
            }).catch(err => {

                console.log(err);
                if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                    usbSDK.machineInformation.get.modelNumber(0x05, target)
                        .then(res => {
                            getUsbBasicBus.next({ type: 'data', data: { 'model': res.dataParsed } });
                        }).catch(err => {
                            console.log(err);
                        }).finally(() => {
                            console.log('modelNumber 2');
                        });
                }
            }).finally(() => {
                console.log('modelNumber 1');
            });
    }


    return getUsbBasicBus;
}

function removeZero(hexStr) {
    let tem = '';
    if (hexStr) {
        for (let i = hexStr.length / 2 - 1; i > 0; i--) {
            if (hexStr.substring(2 * i, 2 * i + 2) === "00") {
                tem = hexStr.substring(0, 2 * i);
            } else {
                return hexStr;
            }
        }
    } else {
        return tem;
    }
}
// removeZero( '1616546573742048656c6c6f20576f726c64000000' );

function getUsbBoxBasic() {
    let getUsbBoxBasicBus = new Rx.Subject();

    usbSDK.client.hardwareVersion()
        .then(res => {
            // hardware_version
            // console.log(res, "hardware_version");

            // {message: "3000000c42455353542e554320332e32000000000000000000…0000000000000000000000000000000000000000000000000"}
            // 3000000c42455353542e554320332e320000000000000000000000000000000000000000000000000"
            let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
            getUsbBoxBasicBus.next({ type: 'data', data: { 'hardware_version': Util.hexToString(result) } });
            return usbSDK.client.softwareVersion();
        }).then(res => {
            // version
            // console.log(res, "version");

            // 3200000f42455353545231303130332e312e32000000000000…0000000000000000000000000000000000000000000000000"
            let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
            getUsbBoxBasicBus.next({ type: 'data', data: { 'version': Util.hexToString(result) } });
            return usbSDK.client.serialNumber();
        }).then(res => {
            // sn
            // console.log(res, "sn");

            // 3100001616546573742048656c6c6f20576f726c640000000000000000000000000000000000000000000000"
            let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
            getUsbBoxBasicBus.next({ type: 'data', data: { 'sn': Util.hexToString(result) } });
        }).catch(err => {
            console.log(err);
            usbSDK.client.hardwareVersion()
                .then(res => {
                    let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
                    getUsbBoxBasicBus.next({ type: 'data', data: { 'hardware_version': Util.hexToString(result) } });
                    return usbSDK.client.softwareVersion();
                }).then(res => {
                    let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
                    getUsbBoxBasicBus.next({ type: 'data', data: { 'version': Util.hexToString(result) } });
                    return usbSDK.client.serialNumber();
                }).then(res => {
                    let result = removeZero(res.message.substr(8, parseInt(res.message.substr(6, 2), 16) * 2));
                    getUsbBoxBasicBus.next({ type: 'data', data: { 'sn': Util.hexToString(result) } });
                }).catch(err => {
                    console.log(err);
                }).finally(() => {});
        }).finally(() => {});

    return getUsbBoxBasicBus;
}

function checkUartSum(stepLength, data) {
    let length = parseInt(data.substring(stepLength, stepLength + 2), 16);
    let calculatedChecksum = Util.calculateChecksumFF(data.substring(0, stepLength + 2 + 2 * length));
    let checkSum = parseInt(data.substring(stepLength + 2 + 2 * length, stepLength + 2 + 2 * length + 2), 16);
    if (checkSum === calculatedChecksum) {
        // console.log('true');
        return true;
    } else {
        // console.log('false');
        return false;
    }
}

// checkUartSum(2,'5218270d002d323834494a4c5064002330303c46484c546438068b');

let listenDataBus;

const SensorCheck = {
    'enable': false,
    'last_en_time': 0
};
const ControllerCheck = {
    'enable': false,
    'last_en_time': 0
};
const HmiCheck = {
    'enable': false,
    'last_en_time': 0
};

const CPD = {
    _3000_ht: "",
    _3100_s_to: "",
    _3100_s_ca: "",
    _3100_s_fa: "",
    _3200_ct_lc: "",
    _3200_ct_sg: "",
    _3200_ct_ca: "",
    _3200_ct_to: "",
    _3200_ct_rm: "",
    _3201_ct_sp: "",
    _3201_ct_cu: "",
    _3201_ct_vo: "",
    _3201_ct_ct: "",
    _3201_ct_mt: "",
    _3202_ct_as: "",
    _3203_ct_sl: "",
    _3203_ct_wd: "",
    _3203_ct_cf: "",
    _3204_ct_cc: "",
    _3205_ct_cl: "",
    _3206_ct_cl: "",
    _3400_bt_fc: "",
    _3400_bt_rc: "",
    _3400_bt_asoc: "",
    _3400_bt_rsoc: "",
    _3400_bt_soh: "",
    _3401_bt_cu: "",
    _3401_bt_vo: "",
    _3401_bt_tp: "",
    _3402_bt_em: "",
    _3600_ts_le: "",
    _3601_ts_st: "",
    _3602_ts_ed: "",
    _3603_ts_rt: "",
    _3700_el_em: "",
};


function listenData() {
    console.log('start listen');
    listenDataBus = new Rx.Subject();
    try {
        usbSDK.client.events.on('public-data', (res) => {
            // console.log(res);
            // { can:"NORMAL_ACK",canCode:2,code:"35",data:"",dataLen:0,source:"CTL",sourceCode:2,subCode:"01",target:"BST",targetCode:5 }
            if (res.code === '31') {
                SensorCheck.enable = true;
                SensorCheck.last_en_time = time();
            } else if (res.code === '32') {
                ControllerCheck.enable = true;
                ControllerCheck.last_en_time = time();
            } else if (res.code === '63') {
                HmiCheck.enable = true;
                HmiCheck.last_en_time = time();
            }
            if (res.code === '35') {
                console.log(res);
            }
            if (res.code === '32' && res.subCode === '03' && res.targetCode === 15) {
                let result_3 = Buffer.from(res.data, 'hex');
                if (result_3.length >= 6) {
                    CPD._3203_ct_sl = (result_3[0] + result_3[1] * 256) / 100;
                    CPD._3203_ct_wd = res.data.substr(4, 4).toUpperCase();
                    CPD._3203_ct_cf = result_3[4] + result_3[5] * 256;
                }
            }
            try {
                listenDataBus.next({ type: 'data', data: res, cpd: CPD });
            } catch (error) {
                console.log(error);
            }

        });
        usbSDK.client.events.on('usb_error', (res) => {
            try {
                listenDataBus.next({ type: 'error_usb', data: res });
            } catch (error) {
                console.log(error);
            }
            // usbDevice = false;
            // console.log("error_usb", new Date().getTime());

        });

        // usbSDK.client.events.on('firmware-update-progress', (res) => {
        //     listenDataBus.next({ type: 'update_fp', data: res });
        // });
        //
        // usbSDK.client.events.on('firmware-update-completed', (res) => {
        //     // console.log('firmware-update-completed',res);
        //     listenDataBus.next({ type: 'update_fc', data: res });
        // });
        //
        // usbSDK.client.events.on('firmware-update-completed-progress', (res) => {
        //     listenDataBus.next({ type: 'update_fcp', data: res });
        // });
    } catch (err) {
        console.log(err);
    }

    return listenDataBus;
}

function getListenData() {
    // listenData();
    if (!listenDataBus) {
        listenData();
    }
    return listenDataBus;
}

function getBatteryOfData() {
    let arr1to7 = [0, 0, 0, 0, 0, 0, 0],
        arr8to14 = [0, 0, 0, 0, 0, 0, 0];
    usbSDK.machineInformation.get.chargingInformation(0x05, 0x04)
        .then(res => {
            let result_3 = Buffer.from(res.data, 'hex');
            if (result_3.length >= 6) {
                let cycle_Life = result_3[0] + result_3[1] * 256;
                getUsbBatteryBus.next({ type: 'data', data: { 'cycleLife': cycle_Life } });

                let max_ChargeInterval = Math.floor((result_3[2] + result_3[3] * 256) / 24);
                getUsbBatteryBus.next({ type: 'data', data: { 'maxChargeInterval': max_ChargeInterval } });

                let current_ChargeInterval = Math.floor((result_3[4] + result_3[5] * 256) / 24);
                getUsbBatteryBus.next({ type: 'data', data: { 'currentChargeInterval': current_ChargeInterval } });
            }

        }).catch(err => {
            console.log(err);
        }).finally(() => {});
    usbSDK.machineInformation.get.batteryCellsInformation(0x05, 0x04)
        .then(res => {
            let result_4 = Buffer.from(res.data, 'hex');
            if (result_4.length === 8) {
                arr1to7[0] = result_4[0] + result_4[1] * 256;
                arr1to7[1] = result_4[2] + result_4[3] * 256;
                arr1to7[2] = result_4[4] + result_4[5] * 256;
                arr1to7[3] = result_4[6] + result_4[7] * 256;
            }
        }).catch(err => {
            console.log(err);
        }).finally(() => {});
    usbSDK.machineInformation.get.batteryCellsInformation_1(0x05, 0x04)
        .then(res => {
            let result_5 = Buffer.from(res.data, 'hex');
            if (result_5.length === 8) {
                arr1to7[4] = result_5[0] + result_5[1] * 256;
                arr1to7[5] = result_5[2] + result_5[3] * 256;
                arr1to7[6] = result_5[4] + result_5[5] * 256;
                arr8to14[0] = result_5[6] + result_5[7] * 256;
                let cell1_7 = arr1to7.join();
                getUsbBatteryBus.next({ type: 'data', data: { 'cell1_7': cell1_7 } });
            }
        }).catch(err => {
            console.log(err);
        }).finally(() => {});
    usbSDK.machineInformation.get.batteryCellsInformation_2(0x05, 0x04)
        .then(res => {
            let result_6 = Buffer.from(res.data, 'hex');
            if (result_6.length >= 4) {
                arr8to14[1] = result_6[0] + result_6[1] * 256;
                arr8to14[2] = result_6[2] + result_6[3] * 256;
                if(result_6.length >= 6){
                    arr8to14[3] = result_6[4] + result_6[5] * 256;
                    arr8to14[4] = result_6[6] + result_6[7] * 256;
                }
                let cell8_14 = arr8to14.join();
                getUsbBatteryBus.next({ type: 'data', data: { 'cell8_14': cell8_14 } });
            }
        }).catch(err => {
            console.log(err);
        }).finally(() => {});
    usbSDK.machineInformation.get.batteryCellsInformation_3(0x05, 0x04)
        .then(res => {
            let result_7 = Buffer.from(res.data, 'hex');
            if (result_7.length >= 4) {
                arr8to14[5] = result_7[0] + result_7[1] * 256;
                arr8to14[6] = result_7[2] + result_7[3] * 256;
                let cell8_14 = arr8to14.join();
                getUsbBatteryBus.next({ type: 'data', data: { 'cell8_14': cell8_14 } });
            }
        }).catch(err => {
            console.log(err);
        }).finally(() => {});

    let getUsbBatteryBus = new Rx.Subject();
    return getUsbBatteryBus
}

function checkUsbUart() {
    if (usbDevice) {
        let type;
        try {
            type = usbSDK.client.getDeviceType();
        } catch (err) {
            console.log(err, "checkUsbUart");
        }
        if (type == 'CAN') {
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}


//  数组去重
function rep(arr) {
    let ret = [];
    for (let i = 0; i < arr.length; i++) {
        if (arr.indexOf(arr[i]) == i) {
            ret.push(arr[i]);
        }
    }
    return ret;
}

let updateStatus = false;

//  返回连接状态
function getConnection() {
    return connection;
}

function setConnection() {
    connection.usb = false;
    connection.com = false;
}


// 传感器数据
function checkSensor() {
    return SensorCheck;
}
// 控制器数据
function checkCtl() {
    return ControllerCheck;
}
// 仪表数据
function checkHmi() {
    return HmiCheck;
}


const Port = {
    Core: Core,
    getPortList: getPortList,
    connect: connect,
    write: write,
    writeCan: writeCan,
    writeByQueue: writeByQueue,
    writeByQueueAndWait: writeByQueueAndWait,
    emptyQueue: emptyQueue,
    ack: ack,
    getBus: getBus,
    setMode: setMode,
    getMode: getMode,
    reset: reset,
    isConnected: isConnected,
    writeAndEnsure: writeAndEnsure,
    writeAndEnsureCan: writeAndEnsureCan,
    getPort: getPort,
    getCan: getCan,
    setCan: setCan,

    getUSBDevice: getUSBDevice,
    setUSBDevice: setUSBDevice,
    getBESST_Pro: getBESST_Pro,
    setBESST_Pro: setBESST_Pro,
    processIndex: processIndex,
    makeIndex: makeIndex,
    makeFrameHID: makeFrameHID,
    makeFrameHIDCan: makeFrameHIDCan,

    timeLog: timeLog,
    getConnection: getConnection,
    setConnection: setConnection,
    setBoxOperation: setBoxOperation,
    addZero: addZero,

    usbSDK: usbSDK,
    checkUsbError: checkUsbError,
    startConnection: startConnection,
    checkUsbConnection: checkUsbConnection,
    getUsbBasic: getUsbBasic,
    getUsbBoxBasic: getUsbBoxBasic,
    getListenData: getListenData,
    getBatteryOfData: getBatteryOfData,
    checkUsbUart: checkUsbUart,
    checkCtl: checkCtl,
    checkSensor: checkSensor,
    checkHmi: checkHmi,
    checkUartSum: checkUartSum,

    getNetMac: getNetMac
};

export default Port;



//////////////////
// WEBPACK FOOTER
// ./device/conn.js
// module id = 77
// module chunks = 73