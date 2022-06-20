import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn';
import Crc from 'crc';

const HID = require('node-hid');
let usbDevice = null;

let bus = null;

//  USB Hid
let devices = null;

// let device = null;
let connectBus = null;
let status = null;
let besst = true;



//  -- SerialPort

const fs = require('graceful-fs');
const Y_MODEM = {
    SOH: 1, // 128 byte blocks
    // STX: 0x02, // 1k blocks
    ETX: 3, // 正文结束
    EOT: 4, // 传输结束
    ACK: 6, // 确认
    NAK: 0x15, //
    CAN: 0x18, // 取消

    // CRC16: 0x43,
    C: 0x43,
    ABORT1: 0x41,
    ABORT2: 0x61,
    PACKET_LENGTH: 128,

    PACKET_MARK: 0x01,
    EXPECTED_PACKET_LENGTH: 128 + 5,

};

let connBus;
let pendingProcessData = '';

let globalFile = null;
let fileBuffer;
let fileName = '';
let headFrame = '';
let isInYmodemMode = false;
let step = 1;
let seq = 0;
let offset = 0;
let lastFrameOffset = 0;
let lastFrameState = 0;
let eotReceiveCount = 0;
let frameNumber = 0;
let cNumber = 0;
let connBusEvent;

function connect(comName, baudRate) {

    if(comName && baudRate){
        connBus = Port.connect(comName, baudRate);
    }else{
        usbDevice = true;
        connBus = Port.connectHID();
        // device = Port.getDevice();
    }
    // if(connBusEvent !== null && connBusEvent !== undefined){
    //     connBusEvent.unsubscribe();
    // }
    console.log(connBus.subscribe());
    connBusEvent = connBus.subscribe(item => {
        switch (item.type) {
            case 'open':
            case 'connect':
            {
                bus.next(item);
                cNumber = 0;
                step = 1;
            }
                break;
            case 'data':
            {
                pendingProcessData = item.data;

                if( usbDevice ){
                    parseHID();
                }else {
                    parse();
                }

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
    let wrapper = {};
    let update = {};

    if (isInYmodemMode) {
        process();
    }
    switch (pendingProcessData) {
        case 'AA020101010D0B':
            //进入ymodem模式
            isInYmodemMode = true;
            break;
        case 'AA020102030D0B':
            //进入update模式
            if (headFrame == null) {
                console.log('无厂家代号等信息');
                return;
            }
            Port.writeByQueue(headFrame, 200);
            break;
        case 'AA030101010D0B':
            //工具有升级包,不用升级
            break;
        case 'OKPRGCTL':
            update['status'] = 'ok';
            wrapper['update'] = update;
            bus.next({ type: 'data', data: wrapper });
            break;
        case 'ERRPRGCTL':
            update['status'] = 'error';
            wrapper['update'] = update;
            bus.next({ type: 'data', data: wrapper });
            break;
        case 'TIMEOUTCTL':
            update['status'] = 'timeout';
            wrapper['update'] = update;
            bus.next({ type: 'data', data: wrapper });
            break;
    }
}

function process() {
    let wrapper = {};
    let update = {};
    // console.log('process', pendingProcessData);
    for (let i = 0; i < Math.floor(pendingProcessData.length / 2); i++) {
        // console.log('char', pendingProcessData);
        let charString = pendingProcessData.substr(i * 2, 2);
        switch (parseInt(charString, 16)) {
            case Y_MODEM.C:
                if (step === 1) {
                    console.log('head');
                    step = 2;
                    sendFileHeadFrame(fileName, fileBuffer.length);
                    lastFrameState = 0;
                }
                if (step === 2) {
                    if (lastFrameState) {
                        //下位机接收到第一帧，发送数据包
                        console.log('数据包0');
                        offset = 0;
                        sliceFile(fileBuffer, offset);
                        step = 3;
                    }
                    // else
                    // {
                    // 	//重新发第一帧
                    // 	console.log('第一帧');
                    // 	step = 2;
                    // 	send_filename_header(fileName, file.length, function()
                    // 	{
                    // 		console.log('done header');
                    // 	});
                    // 	lastFrameState = 0;
                    // }
                }
                if (step === 3) {
                    console.log('c1：' + step);
                    cNumber++;
                    // 收到三次
                    if (cNumber > 4) {
                        console.log('reset besst and resend 3C');
                        Port.reset(500);
                        setTimeout(function() {
                            step = 1;
                            sendPackage(globalFile);
                            update['message'] = "Update process need start again.";
                            wrapper['update'] = update;
                            bus.next({ type: 'data', data: wrapper });
                        }, 1000);

                    }
                }
                if (step === 5) {
                    console.log('c2:' + step);
                    seq = 0;
                    sendYmodemFrame(Buffer([0x00]));
                    step = 6;
                }
                break;
            case Y_MODEM.ACK:
                console.log('ack1:' + step);
                lastFrameState = 1;
                if (step === 3) {
                    offset = offset + 1;
                    // console.log('数据包' + offset);
                    sliceFile(fileBuffer, offset);
                    lastFrameState = 0;
                    update['percentage'] = offset / frameNumber;
                    wrapper['update'] = update;
                    bus.next({ type: 'data', data: wrapper });
                }
                if (step === 4 && eotReceiveCount === 2) {
                    step = 5;
                    console.log('ack2:' + step);
                }
                if (step === 6) {
                    console.log('done');
                }
                break;
            case Y_MODEM.NAK:
                console.log('nak:' + step);
                if (step === 3) {
                    // 重传这一帧
                    // lastFrameState = 1;
                    // sliceFile(fileBuffer, lastFrameOffset);
                    // lastFrameState = 0;
                    // 重传整个文件

                    console.log('reset besst and resend nak');
                    Port.reset(500);
                    setTimeout(function() {
                        step = 1;
                        sendPackage(globalFile);
                        update['message'] = "Update process need start again.";
                        wrapper['update'] = update;
                        bus.next({ type: 'data', data: wrapper });
                    }, 1000);
                }
                if (step === 4) {
                    eotReceiveCount = 2;
                    Port.writeByQueue(Buffer([Y_MODEM.EOT]).toString('hex'));
                }
                break;
        }
    }
}

function getBus() {
    return bus;
}

function sendPackage(data) {
    if (data == null) {
        return
    }
    setup();


    globalFile = data;
    fileName = data.name.toUpperCase();
    if (data instanceof File) {

        fileBuffer = fs.readFileSync(data.path);
    } else {
        // 如果是直接 传网上的数据
        fileBuffer = data.fileBuffer;
    }

    fileBuffer = Buffer.from(fileBuffer);

    let baseInfoBuffer = fileBuffer.slice(0, 16);

    fileBuffer = fileBuffer.slice(16, fileBuffer.length);
    frameNumber = Math.ceil(fileBuffer.length / Y_MODEM.PACKET_LENGTH);
    // console.log('frame count', frameNumber);

    let hardwareId = baseInfoBuffer.slice(0, 1).toString('hex');
    let factoryCode = baseInfoBuffer.slice(1, 2).toString('hex');
    let softwareType = baseInfoBuffer.slice(2, 3).toString('hex');
    headFrame = 'AA02' + factoryCode + hardwareId + softwareType + '0D0B';
    // console.log(headFrame)

    console.log("reset and resend");

    Port.reset(100);
    setTimeout(function(){
        Port.writeByQueue(headFrame);
    },2000);


    return bus;

}

function sendFileHeadFrame(name, size) {
    console.log('head frame', name, size);
    seq = 0;
    offset = 0;
    // 固定？
    name = 'STM32.Bin';
    let nameBuffer = new Buffer(name);
    nameBuffer = Buffer.concat([Buffer(12 - nameBuffer.length), nameBuffer], 12);
    //KB保留小数?
    //if (size <= 1024) {
    //    size = size + 'B';
    //} else {
    //    size = Math.ceil(size/1024) + 'K';
    //}
    let sizeBuffer = new Buffer(size + '');
    sizeBuffer = Buffer.concat([Buffer(8 - sizeBuffer.length), sizeBuffer], 8);

    let offsetBuffer = new Buffer([offset]);
    offsetBuffer = Buffer.concat([Buffer(8 - offsetBuffer.length), offsetBuffer], 8);

    let fileHeadBufffer = new Buffer(nameBuffer + sizeBuffer + offsetBuffer);
    sendYmodemFrame(fileHeadBufffer);
}

function sendYmodemFrame(dataBuffer) {
    if (Y_MODEM.PACKET_LENGTH - dataBuffer.length > 0) {
        dataBuffer = Buffer.concat([dataBuffer, Buffer(Y_MODEM.PACKET_LENGTH - dataBuffer.length)], Y_MODEM.PACKET_LENGTH);
    }

    let seqchr = Buffer.from([seq & 0xFF]);
    let seqchrNeg = Buffer.from([(-seq - 1) & 0xFF]);

    let crc16 = Crc.crc16xmodem(dataBuffer).toString(16); //两位
    if (crc16.length == 1) {
        crc16 = '000' + crc16;
    } else if (crc16.length == 2) {
        crc16 = '00' + crc16;
    } else if (crc16.length == 3) {
        crc16 = '0' + crc16;
    }

    let packetBuf = Buffer.concat([Buffer.from([Y_MODEM.PACKET_MARK]), seqchr, seqchrNeg, dataBuffer, Buffer(crc16, 'hex')]);
    if (packetBuf.length != Y_MODEM.EXPECTED_PACKET_LENGTH) {
        throw ('packet length is wrong!');
    }

    // currentSerialPortInstance.once('data', function(data) {
    // 	var response = data[0];
    // 	ymodem_seq += 1;
    // 	cb(response);
    // });
    // console.log(packetBuf);
    Port.writeByQueueAndWait(packetBuf.toString('hex'), 10);

    seq += 1;
}

function checkCrc(arrBuf) {
    let crc16 = '0000'+ Crc.crc16xmodem(arrBuf).toString(16); //两位
    console.log(crc16);
    return crc16.substr(-4);
}

function sliceFile() {
    lastFrameOffset = offset;
    let lower = offset * Y_MODEM.PACKET_LENGTH;
    let higher = (offset + 1) * Y_MODEM.PACKET_LENGTH;
    let end = false;
    if (higher >= fileBuffer.length) {
        higher = fileBuffer.length;
    }
    if (lower >= fileBuffer.length) {
        step = 4;
        // console.log('eot' + step);
        eotReceiveCount = 1;
        Port.writeByQueue(Buffer([Y_MODEM.EOT]).toString('hex'));
    } else {
        let buf = fileBuffer.slice(lower, higher);
        sendYmodemFrame(buf);
    }
}

function setup() {
    fileName = '';
    headFrame = '';
    isInYmodemMode = false;
    step = 1;
    seq = 0;
    offset = 0;
    lastFrameOffset = 0;
    lastFrameState = 0;
    eotReceiveCount = 0;
    globalFile = null;
    fileOriginBuffer = [];
    fileBuffer = [];
    fileInfo = [];
    fileBufferLen = 0;
    fileArr = [];
    fileArrLen = 0;
    fileArrLastLen = 0;
    fileCrc = '';
}



//  更新操作

// let connBus;
let updateBus;
// let pendingProcessData = '';

// let globalFile = null;
let fileOriginBuffer;
// let fileBuffer;
let fileInfo = [];
let fileBufferLen = 0;
let fileArr = [];
let fileArrLen = 0;
let fileArrLastLen = 0;
// let fileName = '';
let fileCrc = '';
// let step = 1;
// let seq = 0;
// let lastFrameOffset = 0;
// let offset = 0;
let illegal = false;
let startWriteHID = false;
let errorCount = 0;

//
function parseHID(){
    let wrapper = {};
    let update = {};
    let processData = pendingProcessData;
    // pendingProcessData = '22000001020000';
    // console.log('process', pendingProcessData);

    // for (var i = 0; i < Math.floor(pendingProcessData.length / 2); i++) {

    let command = processData.substr(0,2);
    // console.log(command,"command");
    if(command === '22'){

        let charString = processData.substr( 8, 2 );
        console.log(charString,"charString");

        // i = i+6;
        switch ( charString ) {
            case '01':   // true
                if (step === 2) {
                    sendFrame(offset);

                    if(startWriteHID){
                        if( offset > fileArrLen){
                            update['fail'] = '升级失败，请按BESST盒子的重置按钮一次，然后再连接';
                            wrapper['update'] = update;
                            bus.next({ type: 'data', data: wrapper });
                            break;
                        }
                        update['percentage'] = offset / fileArrLen;
                        wrapper['update'] = update;
                        bus.next({ type: 'data', data: wrapper });

                    }
                    offset++;
                }
                break;
            case '00':
                let len = parseInt(processData.substr(2,4), 16);
                console.log(len,'len');

                console.log("response error code 00",processData);
                errorCount++;
                if(errorCount>20){
                    errorCount = 0;
                    update['error'] = '出现错误过多，升级失败，请按重置按钮。再连接，然后重新开始';
                    wrapper['update'] = update;
                    bus.next({ type: 'data', data: wrapper });
                    break;
                }
                if(startWriteHID && len <= fileBufferLen){
                    sendFrame(len);
                }
                break;
        }
    }
    if(command === '26'){
        let charString = processData.substr( 8, 2 );
        console.log(charString,"charString");
        // i = i+6;
        switch ( charString ) {
            case '01':   // true
                let data  = processData.substr( 2, 4 );
                if(data == fileCrc.toUpperCase() ){
                    update['result'] = 'update successfully';
                    wrapper['update'] = update;
                    bus.next({ type: 'data', data: wrapper });
                }else{
                    update['result'] = 'file check failed,please retry';
                    wrapper['update'] = update;
                    bus.next({ type: 'data', data: wrapper });
                }
                break;
        }
    }
}



// function resetHID(){
//
//     lastFrameOffset = 0;
//
//     globalFile = null;
//     fileOriginBuffer = [];
//     fileBuffer = [];
//     fileInfo = [];
//     fileBufferLen = 0;
//     fileArr = [];
//     fileArrLen = 0;
//     fileArrLastLen = 0;
//     fileName = '';
//     fileCrc = '';
//     step = 1;
//     seq = 0;
//     offset = 0;
//     startWriteHID = false;
//     besst =  false;
//
//     Port.writeHIDUSB( Port.bufferToArray(Buffer.concat([ Buffer.from('39','hex') ],64)) );
//
//     // setTimeout(()=>{
//     //     getDevice();
//     //     Port.connect();
//     // },200);
// }
//
function readFileData(data) {

    offset = 0;
    step = 1;
    startWriteHID = false;
    fileArr = [];
    fileOriginBuffer = [];
    errorCount = 0;

    if (data instanceof File) {
        console.log('11-File');
        fileOriginBuffer = fs.readFileSync(data.path);
    } else {
        // 如果是直接 传网上的数据
        console.log('22-Data');
        fileOriginBuffer = data.fileBuffer;
    }

    fileOriginBuffer = Buffer.from(fileOriginBuffer);

    fileInfo = fileOriginBuffer.slice(0,16);
    console.log(fileInfo);
    fileBuffer = fileOriginBuffer.slice(16);

    let count = 0;
    for(let f=4; f<16 ; f++){
        if( fileInfo[f] == 0){
            count++;
        }
    }
    if(count < 12){
        console.log("Illegal File");
        illegal = true;
    }

    let temArr = Port.bufferToArray( fileBuffer );
    fileBufferLen = temArr.length;
    console.log(fileBufferLen,"fileBufferLen");

    for(let i=0; i < fileBufferLen; ){
        fileArr.push( temArr.slice(i, i+60) );
        i=i+60;
    }
    // console.log(fileInfo.length,"fileInfo");

    fileArrLen = fileArr.length;
    console.log(fileArrLen , 'fileArrLen');
    fileArrLastLen = fileArr[fileArrLen-1].length;
    console.log(fileArrLastLen , 'fileArrLastLen');


    // if(fileArrLastLen<61){
    //     let tempArr = fileArr[fileArrLen-1];
    //     while(tempArr.length<60){
    //         tempArr.push(0);
    //     // }
    //     // console.log(tempArr);
    //     fileArr[fileArrLen-1] = tempArr;
    // }
    if(illegal){
        // 生成文件源文件
        fileCrc =  Crc.crc16xmodem(fileOriginBuffer).toString(16); //两位
    }else{
        // 更新时bin 文件
        fileCrc =  Crc.crc16xmodem(fileBuffer).toString(16); //两位
    }

    fileCrc = ('0000'+fileCrc).substr(-4);
    console.log(fileCrc,'fileCrc');


    return {'illegal':illegal,'crc':fileCrc};

}

// function writeCrcFile(hard,manu,type){
//     console.log("准备写入文件");
//     if(illegal){
//         let head = Buffer.concat( [Buffer.from(hard,'hex'),Buffer.from(manu,'hex'),Buffer.from(type,'hex')], 16);
//         console.log(head);
//         // let crcBuffer = Buffer.concat( [Buffer.from(fileCrc,'hex')] , 16);
//         let newBuffer = Buffer.concat( [ head, fileOriginBuffer] );
//
//         fs.writeFile('input.bin', newBuffer,  function(err) {
//             if (err) {
//                 return console.error(err);
//             }
//             console.log("数据写入成功！");
//         });
//     }
// }

//
function sendFrameErase() {

    let dataBuf = Buffer.concat([ Buffer.from([0x25,0x00,0x00,0x00]) ],64);

    let frameArr = Port.bufferToArray( dataBuf );

    Port.writeHIDUSB(frameArr);
}
//
function sendFileHeadFrameHID() {

    let lenHex = fileBufferLen.toString(16);

    if(lenHex.length%2 !=0){
        lenHex = '0'+lenHex;
    }
    console.log(lenHex,"lenHex");

    let lenBuf = Buffer.from(lenHex,'hex');
    console.log(lenBuf,"lenBuf");

    let arr = [0,0,0,0];

    for(let i=0; i<lenBuf.length; i++){
        arr[arr.length-1-i] = lenBuf[lenBuf.length-1-i];
    }

    // sendFrame(lenBuf,255);
    // console.log( Buffer.concat([buffer,lenBuf]) );

    let data = "24000009" + Buffer.from(arr).toString('hex') + fileCrc;
    let dataBuf = Buffer.concat([Buffer.from(data,'hex'),fileInfo.slice(0,3)],64);
    let frameArr = Port.bufferToArray( dataBuf );


    if( fileInfo.toString('hex').indexOf('fc') < 0 ){
        console.log("BESST");

        step = 2;
        Port.writeHIDUSB(frameArr);
    } else {
        if(Port.getBesst()){
            sendFrameErase();
            console.log("DFU");
            // setTimeout(function (){
            //     besst = true;
            //     getDevice();
            //     setTimeout(()=>{
            //         getDevice();
            //         connect();
            //         step = 2;
            //     },1000);
            //     setTimeout(function(){
            //         writeHIDUSB(frameArr);
            //     },1000);
            //
            // },3000);
        }else{
            step = 2;
            Port.writeHIDUSB(frameArr);
        }

    }

    return bus;
}


function sendFrame(offset){
    startWriteHID = true;
    let dataLen = 60;
    if(offset === (fileArrLen-1)){
        dataLen = fileArrLastLen;
    }
    if(offset >= fileArrLen){
        return;
    }
    // let dataLen = arr.length;
    // if (60- dataLen > 0) {
    //     let arrBuffer = Buffer.concat([Buffer.from(arr), Buffer(60 - arr.length)], 60);
    //     arr = Port.bufferToArrayay(arrBuffer);
    // }

    console.log(offset);
    // console.log(dataBuffer.length);
    // console.log(dataBuffer.toString('hex'),'sendFrame');

    let seqchrL = Buffer.from([offset & 0xFF]);
    let seqchrH = Buffer.from([(offset>>8) & 0xFF ]);
    // console.log(seqchrL);
    // console.log(seqchrH);

    let packetBuf = Buffer.concat([ Buffer.from([0x23]), seqchrH, seqchrL, Buffer.from([dataLen]), Buffer.from( fileArr[offset] ) ],64);

    let packetBufArr = Port.bufferToArray(packetBuf);

    // console.log( packetBuf , '329');

    Port.writeHIDUSB( packetBufArr );
}


const Update = {
    port: Port,
    connect: connect,
    getBus: getBus,
    sendPackage: sendPackage,
    // writeHIDUSB: writeHIDUSB,

    // resetHID: resetHID,
    checkCrc: checkCrc,
    readFileData: readFileData,
    sendFileHeadFrameHID: sendFileHeadFrameHID,

};

export default Update;



//////////////////
// WEBPACK FOOTER
// ./device/update.js
// module id = 111
// module chunks = 73