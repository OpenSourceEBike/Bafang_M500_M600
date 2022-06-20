import Util from '../core/js/util'
import Log from '../core/js/log'
import Rx from 'rxjs/Rx';
import Port from './conn'

const MODE_IN_TEST = 'aa010301010d0b';
const MODE_OUT_TEST = 'aa010401010d0b';

let connBus;
let bus = null;
let subscription;
let pendingProcessData = '';
let testStartInterval = null;

// let GEAR_MODE = [];

let GEAR_MODE_3 = 3;
let GEAR_MODE_4 = 4;
let GEAR_MODE_5 = 5;
let GEAR_MODE_9 = 9;
var currentGearData = {
    gear: 0,
    cadence: 0,
    speed: 0,
    current: 0,
    capacity: 0,
    sensorTorque: 0,
    sensorVoltage: 0

};
var gearMode = 5; // 挡位模式 默认5档仪表

let gearFilter = [];
// let gearCode = [];

gearFilter[GEAR_MODE_3] = [];
gearFilter[GEAR_MODE_3][0] = 0;
gearFilter[GEAR_MODE_3][12] = 1;
gearFilter[GEAR_MODE_3][2] = 2;
gearFilter[GEAR_MODE_3][3] = 3;
gearFilter[GEAR_MODE_3][6] = 'walk';

gearFilter[GEAR_MODE_4] = [];
gearFilter[GEAR_MODE_4][0] = 0;
gearFilter[GEAR_MODE_4][1] = 1;
gearFilter[GEAR_MODE_4][12] = 2;
gearFilter[GEAR_MODE_4][21] = 3;
gearFilter[GEAR_MODE_4][3] = 4;
gearFilter[GEAR_MODE_4][6] = 'walk';

gearFilter[GEAR_MODE_5] = [];
gearFilter[GEAR_MODE_5][0] = 0;
gearFilter[GEAR_MODE_5][11] = 1;
gearFilter[GEAR_MODE_5][13] = 2;
gearFilter[GEAR_MODE_5][21] = 3;
gearFilter[GEAR_MODE_5][23] = 4;
gearFilter[GEAR_MODE_5][3] = 5;
gearFilter[GEAR_MODE_5][6] = 'walk';

gearFilter[GEAR_MODE_9] = [];
gearFilter[GEAR_MODE_9][0] = 0;
gearFilter[GEAR_MODE_9][1] = 1;
gearFilter[GEAR_MODE_9][11] = 2;
gearFilter[GEAR_MODE_9][12] = 3;
gearFilter[GEAR_MODE_9][13] = 4;
gearFilter[GEAR_MODE_9][2] = 5;
gearFilter[GEAR_MODE_9][21] = 6;
gearFilter[GEAR_MODE_9][22] = 7;
gearFilter[GEAR_MODE_9][23] = 8;
gearFilter[GEAR_MODE_9][3] = 9;
gearFilter[GEAR_MODE_9][6] = 'walk';

function connect(comName) {
    subscription = Port.connect(comName, 57600).subscribe(item => {
        switch (item.type) {
            case 'open':
            case 'connect':
            {
                // console.log('diagnosis connect');
                setup();
                bus.next(item);
            }
                break;

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

    // if(!bus){
        bus = new Rx.Subject();
    // }

    return bus;
}

function parse() {
    // let data = pendingProcessData;

    let mode = Port.getMode();

    switch (mode) {
        case MODE_IN_TEST:
        {
            // if (data.length >= 4) {
            //     let start = data.indexOf('160B');
            //     let end = data.indexOf('160B', start + 1);
            //     if (start >= 0 && end >= 0) {
            //         processNormal(data.substring(start, end));
            //         pendingProcessData = pendingProcessData.substring(end);
            //     }
            // }
            if (pendingProcessData.length > 80) {
                process();
            }
            if (pendingProcessData.length >= 512 * 512) {
                // console.log('数据重置为空');
                pendingProcessData = '';
            }
        }
            break;
    }
}

let gearArr = [];

function process() {
    let wrapper = {};
    let test = {};
    let temp = '';
    let buffer;
    let checkSum = 0;
    let type = '';

    // console.log('process', pendingProcessData)
    while (pendingProcessData.length > 0) {
        if (pendingProcessData.length < 16) {
            return;
        }
        let subIndex = 0;
        type = '';
        test = {};
        type = pendingProcessData.substring(0, 4);
        switch (type) {
            case "160B":
                temp = pendingProcessData.substring(0, 8);
                buffer = Buffer.from(temp, 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2]) & 0xff;
                if (checkSum === buffer[3]) {
                    let gear = parseInt(temp.substring(4, 6), 16);
                    gearArr.push(gear);
                    // if(checkMode()){
                        test['gear'] = gear;
                        subIndex = 8;
                    // }

                    // console.log('gear', gear, test['gear']);
                }
                break;
            case "1121":
                temp = pendingProcessData.substring(0, 12);
                buffer = Buffer.from(temp.substring(6), 'hex');
                checkSum = (buffer[0] + buffer[1]) & 0xff;
                if (checkSum === buffer[2]) {
                    test['cadence'] = parseInt(temp.substring(6, 10), 16);
                    subIndex = 12;
                    // console.log('cadence', test['cadence']);
                }
                break;
            case "1120": // 轮毂电机 外置， 中置电机 内置
                temp = pendingProcessData.substring(0, 10);
                buffer = Buffer.from(temp.substring(2), 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2]) & 0xff;
                if (checkSum === buffer[3]) {
                    test['speed'] = parseInt(temp.substring(4, 8), 16);
                    subIndex = 10;
                    // console.log('speed', test['speed']);
                }
                break;
            case "110A": // 电池实时电流
                temp = pendingProcessData.substring(0, 8);
                if (temp.substring(4, 6) === temp.substring(6, 8)) {
                    test['current'] = parseInt(temp.substring(4, 6), 16) * 0.5;
                    subIndex = 8;
                    // console.log('current', test['current']);
                }
                break;
            case "1111":
                temp = pendingProcessData.substring(0, 8);
                if (temp.substring(4, 6) === temp.substring(6, 8)) {
                    test['capacity'] = parseInt(temp.substring(4, 6), 16);
                    subIndex = 8;
                    // console.log('capacity', test['capacity']);
                }
                break;
            case "1125":
                temp = pendingProcessData.substring(0, 16);
                buffer = Buffer.from(temp.substring(6), 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xff;
                if (checkSum === buffer[4]) {
                   // 力矩传感器电压 V
                    test['sensor_volt'] = parseInt(temp.substring(6, 10), 16) * 0.001;
                    // 扭力信号 N/m
                    test['sensor_torque'] = parseInt(temp.substring(10, 12), 16) + parseInt(temp.substring(12, 14), 16) * 0.01;
                    subIndex = 16;
                    // console.log('sensor_volt', test['sensor_volt']);
                }
                break;
            case "161A":
                temp = pendingProcessData.substring(0, 6);
                test['light'] = temp.substring(4, 6) === 'F0' ? 0 : 1;
                subIndex = 6;
                // console.log('light', test['light']);
                break;
            case "1108":
                temp = pendingProcessData.substring(0, 6);
                if (temp.substring(4, 6) === '03') {
                    test['brake'] = 1;
                } else if (temp.substring(4, 6) === '04') {
                    test['throttle'] = 1;
                } else {
                    test['throttle'] = 0;
                    test['brake'] = 0;
                    test['error'] = temp.substring(4, 6);
                    // console.log('error', test['error']);
                }
                // console.log('Error', temp.substring(4, 6));
                subIndex = 6;
                break;
        }
        wrapper['test'] = test;
        bus.next({ type: 'data', data: wrapper });


        subIndex = test.length > 0 ? subIndex : 2;
        pendingProcessData = pendingProcessData.substring(subIndex);
    }
}

function parseTestCan(){
    let wrapper = {};
    let test = {};
    let sensor_Count = 0;
    subscription = Port.getListenData().subscribe((res) => {
        if (!res.data) {
            return;
        }
        wrapper = {};
        test = {};

        if (res.data.targetCode === 15 && res.data.canCode === 7 ) {
            // 021f0712000108  1200 控制器一般故障
            // {"code":"12","subCode":"00","source":"CTL","sourceCode":2,"target":null,"targetCode":15,"can":"LONG_WARNING_CMD","canCode":7,"dataLen":1,"data":"08"}
            // console.log(res.data);
            let result = parseInt( res.data.data.substr(0,2) );
            // console.log('Error', result);

            if(result === 5){
                test['throttle'] = 1;
            }else if(result === 3){
                test['brake'] = 1;
            }else{
                test['brake'] = 0;
                test['throttle'] = 0;
                test['error'] = result;
            }
        }
        if (res.data.code === '63' && res.data.subCode === '00') {
            let temTotal = Buffer.from(res.data.data, 'hex');
            if(temTotal.length >= 4){
                let temString_0 = Port.addZero(temTotal[0].toString(2), 8);
                // this.totalGear = parseInt(temString_0.substr(4, 4), 2);
                // this.speedModel = temString_0.substr(3, 1) === '0' ? 'ECO' : 'SPORT';
                // this.boost = temString_0.substr(2, 1) === '0' ? '不进入BOOST' : '进入BOOST';
                // this.hmiLevel = temTotal[1];
                test['gearLevel'] = getGearNumber(temTotal[1], parseInt(temString_0.substr(4, 4), 2));
                test['gear'] = temTotal[1];

                let temString_2 = Port.addZero(temTotal[2].toString(2), 8);
                test['light']  = temString_2.substr(7, 1) === '0' ? 0 : 1;

                // this.hmiButton = temString_2.substr(6, 1) === '0' ? '+键没按下' : '+键按下';
                // let temString_3 = Port.addZero(temTotal[3].toString(2), 8);
                // this.hmiSingleTrip = temString_3.substr(7, 1) === '0' ? '单次里程清零' : '单次里程不清零';
            }
        }
        if (res.data.code === '32' && res.data.subCode === '00') {
            // 2s 一次
            let result = Buffer.from(res.data.data, 'hex');
            // console.log(result);
            if(result.length >= 6){
                test['cadence'] = result[3];
                test['sensor_volt'] =(result[4] + result[5] * 256) * 0.001;
                test['sensor_type'] = false;
            }
        }
        if (res.data.code === '32' && res.data.subCode === '01') {
            let temInfo_1 = Buffer.from(res.data.data, 'hex');
            if(temInfo_1.length >= 4){
                test['speed'] = (temInfo_1[0] + temInfo_1[1] * 256) / 100;
                test['current'] = (temInfo_1[2] + temInfo_1[3] * 256) / 100;
                // this.conVoltage = (temInfo_1[4] + temInfo_1[5] * 256) / 100;
                // this.conTemperature = temInfo_1[6] - 40;
                // this.motorTemperature = temInfo_1[7] - 40;
            }
        }
        if (res.data.code === '32' && res.data.subCode === '02') {
            let temInfo_2 = Buffer.from(res.data.data, 'hex');
            if(temInfo_2.length >= 1){
                let temInfo_2_value = Port.addZero(temInfo_2[0].toString(2), 8);
                test['assistance'] = temInfo_2_value.substr(7, 1) === '0' ? 0 : 1;
            }
        }
        if (res.data.code === '31' && res.data.subCode === '00') {
            // 100ms 一次
            let result = Buffer.from(res.data.data, 'hex');
            // console.log(result);
            if(result.length >= 3){
                test['sensor_volt'] =(result[0] + result[1] * 256) * 0.001;
                test['cadence'] = result[2];
                test['sensor_type'] = true;
            }
        }
        wrapper['test'] = test;
        bus.next({ type: 'data', data: wrapper });

    });

    // if(bus){
        bus = new Rx.Subject();
    // }else{
    //     return bus;
    // }
}

function processNormal(frame) {
    // console.log('test frame', frame);
    let wrapper = {};
    let test = {};

    let temp = '';
    let buffer;
    let checkSum = 0;
    for (let i = 0; i < frame.length - 4; i = i + 2) {
        switch (frame.substring(i, i + 4)) {
            case "160B":
                temp = frame.substring(i, i + 8);
                buffer = Buffer.from(temp, 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2]) & 0xff;
                if (checkSum == buffer[3]) {
                    let gear = parseInt(temp.substring(4, 6), 16)
                    test['gear'] = getGearNumber(gear)
                    // console.log('gear', gear, test['gear']);
                }
                break;
            case "1121":
                temp = frame.substring(i, i + 12);
                buffer = Buffer.from(temp.substring(6), 'hex');
                checkSum = (buffer[0] + buffer[1]) & 0xff;
                if (checkSum == buffer[2]) {
                    test['cadence'] = parseInt(temp.substring(6, 10), 16)
                    // console.log('cadence', test['cadence']);
                }
                break;
            case "1120":
                temp = frame.substring(i, i + 10);
                buffer = Buffer.from(temp.substring(2), 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2]) & 0xff;
                if (checkSum == buffer[3]) {
                    test['speed'] = parseInt(temp.substring(4, 8), 16)
                    // console.log('speed', test['speed']);
                }
                break;
            case "110A":
                temp = frame.substring(i, i + 8);
                // Log.d('code 110a', temp);
                if (temp.substring(4, 6) == temp.substring(6, 8)) {
                    test['current'] = parseInt(temp.substring(4, 6), 16) * 0.5
                    // console.log('current', test['current']);
                }
                break;
            case "1111":
                temp = frame.substring(i, i + 8);
                // Log.d('code 1111', temp);
                if (temp.substring(4, 6) == temp.substring(6, 8)) {
                    test['capacity'] = parseInt(temp.substring(4, 6), 16)
                    // console.log('capacity', test['capacity']);
                }
                break;
            case "1125":
                temp = frame.substring(i, i + 16);
                buffer = Buffer.from(temp.substring(6), 'hex');
                checkSum = (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xff;
                // Log.d('code 1125', temp, buffer, checkSum);
                if (checkSum == buffer[4]) {
                    test['sensor_volt'] = parseInt(temp.substring(6, 10), 16) * 0.001
                    test['sensor_torque'] = parseInt(temp.substring(10, 12), 16) + parseInt(temp.substring(12, 14), 16) * 0.01
                    // console.log('sensor_volt', test['sensor_volt']);
                }
                break;
            case "161A":
                temp = frame.substring(i, i + 6);
                // Log.d('code 161a', temp);
                if (temp.substring(4, 6) == 'F0') {
                    test['light'] = 0
                } else if (temp.substring(4, 6) == 'F1') {
                    test['light'] = 1
                }
                // console.log('light', test['light']);
                break;
            case "1108":
                temp = frame.substring(i, i + 6);
                test['error'] = temp.substring(4, 6)
                break;
        }
    }

    wrapper['test'] = test;
    bus.next({ type: 'data', data: wrapper });
}

function setup() {
    Port.setMode(MODE_IN_TEST);

    let cmdList = [
        MODE_IN_TEST,
        MODE_IN_TEST,
        MODE_IN_TEST,
        MODE_IN_TEST
    ];

    cmdList.forEach((item) => {
        // console.log('write test', item);
        Port.writeByQueueAndWait(item, 300);
    });
}

function setupUsb(){
    Port.setMode(MODE_IN_TEST);

    pendingProcessData = '';
    let time = '';

    Port.usbSDK.machineInformation.uart.hmi.update('181028')
        .then(res=>{
            // console.log(res);
        }).catch(err =>{

        console.log(err);
    }).finally(()=>{
    });
    setTimeout(()=>{
        Port.usbSDK.machineInformation.uart.ctl.update('171128')
            .then(res=>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
        Port.usbSDK.client.switchModel('01')
            .then(res=>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
            console.log('response');
        });
    },100);
    setTimeout(()=>{
        Port.usbSDK.client.config('04B004B0')
            .then(res=>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
            console.log('response');
        });

    },200);
}

function parseTestUart(){

    subscription = Port.getListenData().subscribe((res) => {
        if (!res.data) {
            return;
        }
        if (res.data) {
            // console.log(res.data);
            let data = res.data.toString("hex").toUpperCase();
            if(data.substr(0, 2) === '0F'){
                // console.log("Diagnosis Bit Length",parseInt(data.substr(6,2),16));
                // console.log("Diagnosis",data.substr(8, parseInt(data.substr(6,2),16)*2));
                pendingProcessData += data.substr(8, parseInt(data.substr(6,2),16)*2);
                parse();
            }
        }
        //
        // wrapper['test'] = test;
        // bus.next({ type: 'data', data: wrapper });

    });

    if(!bus){
        bus = new Rx.Subject();
    }

    return bus;
}

function busComplete(){
    // console.log("[bus]",bus);
    bus = null;
    if(subscription){
        subscription.unsubscribe();
        subscription = null;
    }
}

function getBus() {
    return bus;
}

function setGearMode(mode) {
    gearMode = parseInt(mode);
    // console.log('gear', gearMode);
}

function outTest() {
    if(Port.getUSBDevice()){
        Port.setMode(MODE_OUT_TEST);
        Port.usbSDK.machineInformation.uart.hmi.update('FF')
            .then(res=>{
                // console.log(res);
            }).catch(err =>{

            console.log(err);
        }).finally(()=>{
        });
        Port.usbSDK.client.switchModel('00').then(res=>{
            // console.log(res);
        }).catch(err =>{

            console.log(err);
        }).finally(()=>{
            console.log('response');
        });
    }else{
        Port.setMode(MODE_OUT_TEST);
        Port.writeByQueueAndWait(MODE_OUT_TEST, 300);
    }
}

function getGearNumber(gearCode, mode) {
    // console.log(gearCode, mode, "gear");
    let res;
    if(mode === 3 || mode === 4 || mode === 5 || mode === 9){
        try{
            res = gearFilter[mode][gearCode];
        }catch(err){
            console.log(err);
        }
    } else {
        try{
            res = gearFilter[5][gearCode];
        }catch(err){
            console.log(err);
        }
    }

    return res;
}


// function checkMode(){
//     let index;
//     if( gearArr.indexOf(0) >-1) {
//         index = gearArr.indexOf(0);
//         if(gearArr.length > index+1){
//             switch(gearArr[index]){
//                 case 12: {
//                     gearMode = 3;
//                     break;
//                 }
//                 case 1: {
//                     if(gearArr.length > index+2){
//
//                     }
//                     break;
//                 }
//                     gearMode = 4;
//                 case 11: gearMode = 5;
//             }
//         }
//     }else if(gearArr.indexOf(3) >-1){
//         index = gearArr.indexOf(0);
//
//     }
//
//     if(gearArr.indexOf(0) > -1){
//         return true;
//
//     }else{
//         return false;
//     }
// }

const Diagnose = {
    port: Port,
    connect: connect,
    getBus: getBus,

    setGearMode: setGearMode,
    getGearNumber: getGearNumber,
    outTest: outTest,
    parseTestCan: parseTestCan,
    parseTestUart: parseTestUart,
    busComplete: busComplete,
    setupUsb: setupUsb,
};

export default Diagnose;



//////////////////
// WEBPACK FOOTER
// ./device/diagnose.js
// module id = 129
// module chunks = 73