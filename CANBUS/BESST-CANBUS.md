# CANBUS information from the BESST software sourcecode


## Summary

##### The following Canbus frameid's have been identified:

|Function|GET/SET|second-last-digit|last-digit|
|---|---|---|---|
|pulse|GET|30|00|
|sensorSignal|GET|31|00|
|controllerInformation|GET|32|00|
|controllerInformation_1|GET|32|01|
|controllerInformation_2|GET|32|02|
|controllerInformation_3|GET/SET|32|03|
|controllerInformation_4|GET|32|04|
|controllerInformation_5|GET|32|05|
|batteryInformation|GET|34|00|
|batteryInformation_1|GET|34|01|
|batteryInformation_2|GET|34|02|
|besst|GET|35|00|
|transmissionInformation|GET|36|00|
|transmissionInformation_1|GET|36|01|
|transmissionInformation_2|GET|36|02|
|transmissionInformation_3|GET|36|03|
|eletronicLockInformation|GET|37|00|
|hardwareVersionNumber|GET/SET|60|00|
|softwareVersionNumber|GET|60|01|
|modelNumber|GET/SET|60|02|
|serialNumber|GET/SET|60|03|
|customerNumber|GET/SET|60|04|
|conParams|GET/SET|60|11|
|conParams_2|GET/SET|60|12|
|conParams_3|GET/SET|60|13|
|sensorCalibration|GET|61|00|
|positionSensorCalibration|SET|62|00|
|controllerFeatures|GET|62|15|
|dataOfHmi|GET|63|00|
|dataOfHmi_1|GET|63|01|
|dataOfHmi_2|GET/SET|63|02|
|dataOfHmi_3|GET|63|03|
|designInformation|GET|64|?|
|chargingInformation|GET|64|01|
|batteryCellsInformation|GET|64|02|
|batteryCellsInformation_1|GET|64|03|
|batteryCellsInformation_2|GET|64|04|
|batteryCellsInformation_3|GET|64|05|
|batteryCellsInformation_4|GET|64|06|
|batteryCellsInformation_5|GET|64|07|
|batteryCellsInformation_6|GET|64|08|
|batteryCellsInformation_7|GET|64|09|
|batteryCharacteristics|GET|64|15|


## Global Enums in BESST code

##### source and target devices on the canbus.

BESST normally sends from BST (0x05) to a target, most often HMI (0x03)
It's not 100% sure how these translate to the CANBUS frameid and data packages

```
/**
 * @enum {hex}
 * @readonly
 */
module.exports.DEVICE = {
    SEN: 0x01,
    CTL: 0x02,
    HMI: 0x03,
    BAT: 0x04,
    BST: 0x05,
    ADV: 0x1f,
}
```


##### References for the BESST USB tool used to send certain CANBUS commands

```
/**
 * @enum {hex}
 * @readonly
 */
module.exports.CAN = {
    WRITE_CMD: 0x00,
    READ_CMD: 0x01,
    NORMAL_ACK: 0x02,
    ERROR_ACK: 0x03,
    LONG_START_CMD: 0x04,
    LONG_TRANG_CMD: 0x05,
    LONG_END_CMD: 0x06,
    LONG_WARNING_CMD: 0x07,
}
```

## Main functions

##### GET Function
```
/**
 * Adds a get request to the queue to fetch back a single field
 * @function getDeviceType
 * @param {string} usbCommand - defaults to common.USB_CMD.USB_CMD_PC_TO_CAN
 * @param {string} source - defaults to common.DEVICE.BST
 * @param {string} target - defaults to common.DEVICE.HMI
 * @param {string} can - defaults to common.CAN.READ_CMD
 * @param {string} code - operation main code
 * @param {string} subCode - operation sub code
 * @param {string} payload - payload to send through
 * @param {string} deferred - promise for when the request has been completed
 * @param {string} piroitise - add the request to the top of the queue stack
 * @param {string} doNotWait - do not wait for a reponse from USB device, as soon as request is sent resolve the deferred promise
 * @param {string} response - 
 * @return {string} promise
 */
module.exports.get = (usbCommand, source, target, can, code, subCode, payload, deferred, piroitise = false, doNotWait = false, response={dataPacks:[]}) => {
    return handler.request({
        usbCommand: usbCommand || common.USB_CMD.USB_CMD_PC_TO_CAN,
        source: source || common.DEVICE.BST,
        target: target || common.DEVICE.HMI,
        can: can || common.CAN.READ_CMD,
        code: code,
        subCode: subCode,
        payload: payload,
        deferred: deferred,
        doNotWait: doNotWait,
        response: response
    }, piroitise)
}
```


##### UPDATE/SET Function
```
//
/**
 * Adds an update request to the queue to modify a single value
 * Requests are sent with the following CAN codes
 * common.CAN.WRITE_CMD
 * common.CAN.LONG_START_CMD
 * common.CAN.LONG_TRANG_CMD
 * common.CAN.LONG_END_CMD
 * @function getDeviceType
 * @param {string} usbCommand - usb command
 * @param {string} source - signal source
 * @param {string} target - signal target
 * @param {string} code - operation main code
 * @param {string} subCode - operation sub code
 * @param {string} payload - payload to send through 
 * @return {string} promise
 */
module.exports.update = (usbCommand, source, target, code, subCode, payload) => {
    if (payload === undefined || payload === null) {
        return q.reject(new nestedError('Null or undefined payload'));
    } else {
        let hexData = payload;
        if (hexData.length % 2 !== 0) {
            hexData = '0' + hexData;
        }
        const dataLen = hexData.length / 2;
        // if data is not empty will split with a 8 bit pacak to send
        const dataPacks = [];
        let _hexData = hexData;
        for (let i = 0; i < dataLen; i += 8) {
            dataPacks.push(_hexData.substr(0, 16));
            _hexData = _hexData.substr(16);
        }
        const startPack = dataPacks.shift();
        const endPack = dataPacks.pop() || '';
        return handler.request({
                usbCommand: usbCommand,
                source: source,
                target: target,
                can: common.CAN.WRITE_CMD,
                code: code,
                subCode: subCode,
                payload: common.numberToHexStr(dataLen)
            })
            .then(res => {
                if (res.canCode === common.CAN.NORMAL_ACK) {
                    return handler.request({
                        usbCommand: usbCommand,
                        source: source,
                        target: target,
                        can: common.CAN.LONG_START_CMD,
                        code: code,
                        subCode: subCode,
                        payload: startPack
                    }, true);
                } else {
                    throw new nestedError('Error Response ', res);
                }
            }).then(res => {
                if (res.canCode === common.CAN.NORMAL_ACK) {

                    if (dataPacks.length === 0) {
                        return res;
                    } else {
                        let deferred = q.defer();
                        async.eachOfLimit(dataPacks, 1, (pack, index, callback) => {
                            let codeIndex = common.hexAllocToStr(index, 4);
                            handler.request({
                                usbCommand: usbCommand,
                                source: source,
                                target: target,
                                can: common.CAN.LONG_TRANG_CMD,
                                code: codeIndex.substr(0, 2),
                                subCode: codeIndex.substr(2, 2),
                                payload: pack
                            }, true).then(res => {
                                if (res.canCode === common.CAN.NORMAL_ACK) {
                                    callback();
                                } else {
                                    //retry
                                    handler.request({
                                        usbCommand: usbCommand,
                                        source: source,
                                        target: target,
                                        can: common.CAN.LONG_TRANG_CMD,
                                        code: codeIndex.substr(0, 2),
                                        subCode: codeIndex.substr(2, 2),
                                        payload: pack
                                    }, true).then(res => {
                                        if (res.canCode === common.CAN.NORMAL_ACK) {
                                            callback();
                                        } else {
                                            callback(res);
                                        }
                                    });
                                }
                            }).then(res => {
                                if (res.canCode === common.CAN.NORMAL_ACK) {
                                    callback();
                                } else {
                                    callback(res);
                                }
                            });
                        }, (err) => {
                            if (err) {
                                deferred.reject(err);
                            } else {
                                deferred.resolve(res);
                            }
                        });

                        return deferred.promise;
                    }
                } else {
                    throw new nestedError("Timeout");
                }
            }).then(res => {
                if (res.canCode === common.CAN.NORMAL_ACK) {
                    let codeIndex = common.hexAllocToStr(dataPacks.length, 4);
                    return handler.request({
                        usbCommand: usbCommand,
                        source: source,
                        target: target,
                        can: common.CAN.LONG_END_CMD,
                        code: codeIndex.substr(0, 2),
                        subCode: codeIndex.substr(2, 2),
                        payload: endPack
                    }, true).then(res => {
                        if (res.canCode === common.CAN.NORMAL_ACK) {
                            return res;
                        } else {
                            throw new nestedError("Timeout");
                        }
                    });
                } else {
                    throw new nestedError("Timeout");
                }
            });
    }
}
```

##### Simplified UPDATE/SET Function
```
//
/**
 * Adds a simple update request to the queue to modify a single value
 * common.CAN.WRITE_CMD
 * @function getDeviceType
 * @param {string} usbCommand - usb command
 * @param {string} source - signal source
 * @param {string} target - signal target
 * @param {string} code - operation main code
 * @param {string} subCode - operation sub code
 * @param {string} payload - payload to send through 
 * @return {string} promise
 */
module.exports.updateSimple = (usbCommand, source, target, code, subCode, payload) => {
    if (payload === undefined || payload === null) {
        return q.reject(new nestedError('Null or undefined payload'));
    } else {
        return handler.request({
            usbCommand: usbCommand || common.USB_CMD.USB_CMD_PC_TO_CAN,
            source: source || common.DEVICE.BST,
            target: target || common.DEVICE.CTL,
            can: common.CAN.WRITE_CMD,
            code: code,
            subCode: subCode,
            payload: payload
        })
    }
}
```

## "Public" CANBUS GET Commands

```
/**
 * Public information get
 * @module publicInformation/get
 */
 ```


##### pulse

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.pulse = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '30', '00');
};
```


##### sensorSignal


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.sensorSignal = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '31', '00');
};
```



##### controllerInformation

example code:
```
if (res.data.code === '32' && res.data.subCode === '00') {
    var temInfo_0 = Buffer.from(res.data.data, 'hex');
    _this13.conCapacityLeft = temInfo_0[0];
    _this13.conSingleTrip = (temInfo_0[1] + temInfo_0[2] * 256) / 100;
    _this13.conCadence = temInfo_0[3];
    _this13.conTorque = temInfo_0[4] + temInfo_0[5] * 256;
    var tem = temInfo_0[6] + temInfo_0[7] * 256;
    if (tem < 65535) {
        _this13.conSingleTripLeft = tem / 100;
    } else {
        _this13.conSingleTripLeft = "-";
    }
}
```

Function code:

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '00');
};
```



##### controllerInformation_1

example code:
```
if (res.data.code === '32' && res.data.subCode === '01') {
    var temInfo_1 = Buffer.from(res.data.data, 'hex');
    _this13.conSpeed = (temInfo_1[0] + temInfo_1[1] * 256) / 100;
    _this13.conCurrent = (temInfo_1[2] + temInfo_1[3] * 256) / 100;
    _this13.conVoltage = (temInfo_1[4] + temInfo_1[5] * 256) / 100;
    _this13.conTemperature = temInfo_1[6] - 40;
    _this13.motorTemperature = temInfo_1[7] - 40;
}
```

Function code:

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation_1 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '01');
};
```



##### controllerInformation_2

example code:
```
if (res.data.code === '32' && res.data.subCode === '02') {
    var temInfo_2 = Buffer.from(res.data.data, 'hex');
    var temInfo_2_value = _controller2.default.port.addZero(temInfo_2[0].toString(2), 8);
    _this13.assistance = temInfo_2_value.substr(7, 1) === '0' ? _this13.lanTrans[0] : _this13.lanTrans[1];
}
```

Function code:



```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '02');
};
```




##### controllerInformation_3


example code:
```
if (res.data.code === '32' && res.data.subCode === '03' && res.data.targetCode == 15) {
    var result_3 = Buffer.from(res.data.data, 'hex');
    _this13.speedLimitRead = (result_3[0] + result_3[1] * 256) / 100;
    _this13.wheelDiameters.find(function (item) {
        if (item.value == res.data.data.substr(4, 4).toUpperCase()) {
            _this13.wheelDiameterRead = item.label;
        }
    });
    _this13.circumferenceRead = result_3[4] + result_3[5] * 256;
}
```

Function code:


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation_3 = (source, target) => {
    // return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '03');
    const deferred = q.defer();
    //Timeout requests after 1 seconds
    let timer = setTimeout(() => {
        deferred.reject('Timeout');
    }, 10000);

    client.events.on('data', (res) => {
        if (res.code === '32' && res.subCode == '03') {
            clearTimeout(timer);
            deferred.resolve(res);
        }
    });
    return deferred.promise;
};
```



##### controllerInformation_4


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation_4 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '04');
};
```



##### controllerInformation_5

example code:
```
if (res.data.code === '32' && res.data.subCode === '05') {
    var temInfo_5 = Buffer.from(res.data.data, 'hex');
    _this13.calorie = temInfo_5[0] + temInfo_5[1] * 256;
}
```

Function code:


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerInformation_5 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '32', '05');
};
```



##### batteryInformation


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '34', '00');
};
```



##### batteryInformation_1


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryInformation_1 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '34', '01');
};
```



##### batteryInformation_2


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryInformation_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '34', '02');
};
```



##### besst


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.besst = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '35', '00');
};
```



##### transmissionInformation


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.transmissionInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '36', '00');
};
```



##### transmissionInformation_1


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.transmissionInformation_1 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '36', '01');
};
```



##### transmissionInformation_2


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.transmissionInformation_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '36', '02');
};
```



##### transmissionInformation_3


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.transmissionInformation_3 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '36', '03');
};
```



##### eletronicLockInformation


```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.eletronicLockInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '37', '00');
};
```








## CANBUS GET commands

These commands request information from components on the CANBUS, for available components see above list.

Each function also includes an example from the BESST code how the function is called.

```
/**
 * Machine information CAN get
 * @module machineInformation/can/get
 */
```

##### hardwareVersionNumber

get.hardwareVersionNumber(0x05, target)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.hardwareVersionNumber = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '00');
};
```

##### softwareVersionNumber

get.softwareVersionNumber(0x05, target)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.softwareVersionNumber = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '01');
};
```

##### modelNumber

get.modelNumber(0x05, target)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.modelNumber = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '02');
};
```

##### serialNumber

get.serialNumber(0x05, 0x04)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.serialNumber = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '03');
};
```

##### customerNumber

get.customerNumber(0x05, 0x02)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.customerNumber = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '04');
};
```

##### conParams

get.conParams(0x05, 0x02)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.conParams = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '11');
};
```

##### conParams_2

get.conParams_2(0x05, 0x02)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.conParams_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '12');
};
```

##### conParams_3

get.conParams_3(0x05, 0x02)
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.conParams_3 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '60', '13');
};
```

##### sensorCalibration

Never Called in code
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.sensorCalibration = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '61', '00');
};
```

##### controllerFeatures

Never Called in code
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.controllerFeatures = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '62', '15');
};
```

##### dataOfHmi

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.dataOfHmi = () => {
    const deferred = q.defer();
    //Timeout requests after 1 seconds
    let timer = setTimeout(() => {
        deferred.reject('Timeout');
    }, 10000);

    client.events.on('data', (res) => {
        if (res.code === '63' && res.subCode == '00') {
            clearTimeout(timer);
            deferred.resolve(res);
        }
    });
    return deferred.promise;
};
```

##### dataOfHmi_1


get.dataOfHmi_1(0x05, 0x03)

Full example:
```
_hmi2.default.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05, 0x03).then(function (res) {
    var result_1 = Buffer.from(res.data, 'hex');
    _this4.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2] * 256 * 256;
    _this4.singleMileage = (result_1[3] + result_1[4] * 256 + result_1[5] * 256 * 256) / 10;
    _this4.speedMax = (result_1[6] + result_1[7] * 256) / 10;
});
```

actual code:

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.dataOfHmi_1 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '63', '01');
};
```

##### dataOfHmi_2

get.dataOfHmi_2(0x05, 0x03)

Full example:
```
# var result_3 = Buffer.from(res.data, 'hex');
# _this4.speedAverage = (result_3[0] + result_3[1] * 256) / 10;
# _this4.serviceMileage = (result_3[2] + result_3[3] * 256 + result_3[4] * 256 * 256) / 10;
```

actual code:
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.dataOfHmi_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '63', '02');
};
```

##### dataOfHmi_3

Not Called in code
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.dataOfHmi_3 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '63', '03');
};
```

##### designInformation

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.designInformation = () => {
    const deferred = q.defer();
    //Timeout requests after 1 seconds
    let timer = setTimeout(() => {
        deferred.reject('Timeout');
    }, common.timeout);

    client.events.on('data', (res) => {
        if (res.code === '64') {
            clearTimeout(timer);
            deferred.resolve(res);
        }
    });
    return deferred.promise;
};
```

##### chargingInformation

get.chargingInformation
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.chargingInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '01');
};
```

##### batteryCellsInformation

get.batteryCellsInformation
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '02');
};
```

##### batteryCellsInformation_1

get.batteryCellsInformation_1
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_1 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '03');
};
```

##### batteryCellsInformation_2

get.batteryCellsInformation_2
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_2 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '04');
};
```

##### batteryCellsInformation_3

get.batteryCellsInformation_3
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_3 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '05');
};
```

##### batteryCellsInformation_4

get.batteryCellsInformation_4
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_4 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '06');
};
```

##### batteryCellsInformation_5

get.batteryCellsInformation_5
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_5 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '07');
};
```

##### batteryCellsInformation_6

get.batteryCellsInformation_6
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_6 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '08');
};
```

##### batteryCellsInformation_7

get.batteryCellsInformation_7
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCellsInformation_7 = (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '09');
};
```

##### batteryCharacteristics

get.batteryCharacteristics
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @returns {Promise}
 */
module.exports.batteryCharacteristics= (source, target) => {
    return client.request_get(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, common.CAN.READ_CMD, '64', '15');
};
```

## "Public" CANBUS SET/UPDATE functions

Example code:
```
writeSpeed: function writeSpeed(data) {
    var _this17 = this;

    var spd = this.speedLimit * 100;
    var cir = parseInt(this.circumference);
    var spdStr = Buffer.from([spd >> 8 & 0xff, spd & 0xff]).reverse().toString('hex');
    var cirBuf = Buffer.from([cir >> 8 & 0xff, cir & 0xff]).reverse().toString('hex');

    _controller2.default.port.usbSDK.publicInformation.update.controllerInformation_3(0x05, 0x02, spdStr + this.wheelDiameter + cirBuf).then(function (res) {
        console.log(res);

        if (res.can == "NORMAL_ACK" && res.targetCode == 5) {
            _this17.speedLimitRead = '';
            _this17.wheelDiameterRead = '';
            _this17.circumferenceRead = '';
            _this17.$message({
                message: _this17.$t('cNam.writeSuccess'),
                type: 'success'
            });
            _this17.saveRecord(data);
        } else if (res.can == "ERROR_ACK" && res.targetCode == 5) {
            _this17.$message({
                message: _this17.$t('cNam.writeFail'),
                type: 'error'
            });
        }
    }).catch(function (err) {
        console.log(err);
        _this17.usbErrorCheck(err);
    }).finally(function () {
        console.log("write speed finally.");
    });
},
```

Actual function:

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.controllerInformation_3 = (source, target, newValue) => {
    return client.request_updateSimple(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '32', '03', newValue);
};
```

## CANBUS SET/UPDATE functions

##### hardwareVersionNumber

```
/**
 * Machine information CAN get
 * @module machineInformation/can/update
 */

module.exports.hardwareVersionNumber = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '00', newValue);
};
```

##### modelNumber

```
module.exports.modelNumber = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '02', newValue);
};
```

##### serialNumber

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.serialNumber = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '03', newValue);
};
```

##### customerNumber

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.customerNumber = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '04', newValue);
};
```

##### dataOfHmi_2

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.dataOfHmi_2 = (source, target, newValue) => {
    return client.request_updateSimple(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '63', '02', newValue);
};
```

##### conParams

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.conParams = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '11', newValue);
};
```

##### conParams_2

```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.conParams_2 = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '12', newValue);
};
```

##### conParams_3

conParams_3(0x05, 0x02, this.conParamsWrite_13

```
/**
 *  @function
 *  @param {string} source - defaults to BST
 *  @param {string} target - defaults to HMI 
 *  @param {string} newValue - new value
 *  @returns {Promise}
 */
module.exports.conParams_3 = (source, target, newValue) => {
    return client.request_update(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '60', '13', newValue);
};
```

##### positionSensorCalibration

positionSensorCalibration(0x05, 0x02, '')
```
/**
 * @function
 * @param {string} source - defaults to BST
 * @param {string} target - defaults to HMI 
 * @param {string} newValue - new value
 * @returns {Promise}
 */
module.exports.positionSensorCalibration = (source, target,newValue) => {
    return client.request_updateSimple(common.USB_CMD.USB_CMD_PC_TO_CAN, source, target, '62', '00',newValue);
};
```


## Setting the controller parameters

Inside BESST using SET by CANBUS towards the controller is actually possible, which should allow setting things like Max current.
However it needs more reverse enginering, because BESST obfusticates how to get actual indiviual values out of the GET/SET CANBUS data.

##### CANBUS 6011 6012 6013 Data

Following is a list of all data available for the controller under CANBUS frames 6011 6012 6013

```
function init(){
    return [
        {
            "6011":[
                {
                    "title":"额定电压",
                    "title_en":"Rated Voltage",
                    "category":"Battery",
                    "range":[36,43,48],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"36，43，48",
                    "unit":"DCV",
                    "split":0,
                    "data":"0",
                    "length":1
                },
                {
                    "title":"电量满时最大限流值",
                    "title_en":"Max Limited Current",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"最大限流， 0-100A",
                    "unit":"A",
                    "split":0,
                    "data":"1",
                    "length":1
                },
                {
                    "title":"过电压",
                    "title_en":"Over-voltage",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"DCmV",
                    "split":0,
                    "data":"2",
                    "length":1
                },
                {
                    "title":"欠压",
                    "title_en":"Under-voltage",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"电机不工作或空转时欠压值，0-100V",
                    "unit":"DCmV",
                    "split":0,
                    "data":"3-4",
                    "length":2
                },
                {
                    "title":"骑行动态欠压",
                    "title_en":"Riding under-voltage",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"DCV",
                    "split":0,
                    "data":"5-6",
                    "length":2
                },
                {
                    "title":"欠压恢复电压",
                    "title_en":"Recovery voltage",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"DCmV",
                    "split":0,
                    "data":"7-8",
                    "length":2
                },
                {
                    "title":"电池容量",
                    "title_en":"Battery capacity",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"mAh",
                    "split":0,
                    "data":"9-10",
                    "length":2
                },
                {
                    "title":"电量不足时最大限流",
                    "title_en":"Ordinary Limited Current",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"11",
                    "length":1
                },
                {
                    "title":"电流开始衰减时电量值阈值",
                    "title_en":"Capacity percentage for current begins to decay",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"12",
                    "length":1
                },
                {
                    "title":"电流开始衰减时电量百分比系数",
                    "title_en":"Capacity percentage for current begins to decay coefficient",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"13",
                    "length":1
                },
                {
                    "title":"电量不足的电量值/电压值阈值",
                    "title_en":"Low power value/voltage threshold",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"14",
                    "length":1
                },
                {
                    "title":"电量不足时最大电流",
                    "title_en":"Ordinary max. Current",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"15",
                    "length":1
                },
                {
                    "title":"满电里程数显示",
                    "title_en":"Full capacity mileage display",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Km",
                    "split":0,
                    "data":"16",
                    "length":1
                },
                {
                    "title":"理论满电里程数",
                    "title_en":"Theoretical full mileage",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Km",
                    "split":0,
                    "data":"17",
                    "length":1
                },
                {
                    "title":"里程计算采样时间*平均电流值",
                    "title_en":"Mileage calculation sampling time * average current value",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"s*A",
                    "split":0,
                    "data":"18-19",
                    "length":2
                },
                {
                    "title":"满负载占空比例系数",
                    "title_en":"Full load duty ratio",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"20",
                    "length":1
                },
                {
                    "title":"电池温度，健康状态，容量衰减等评估系数",
                    "title_en":"Battery temperature, health status, capacity attenuation, etc",
                    "category":"Battery",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"21",
                    "length":1
                },
                {
                    "title":"传感器类型",
                    "title_en":"Sensor Type",
                    "category":"Pedal_sensor",
                    "range":[0,1,2],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0--力矩传感器， 1--速度传感器， 2--仅转把",
                    "unit":"",
                    "split":0,
                    "data":"22",
                    "length":1
                },
                {
                    "title":"倒刹功能",
                    "title_en":"Coaster brake function",
                    "category":"Pedal_sensor",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0--不支持倒刹， 1--支持倒刹",
                    "unit":"Y/N",
                    "split":0,
                    "data":"23",
                    "length":1
                },
                {
                    "title":"速度码盘齿数",
                    "title_en":"Speed dial number of teeth",
                    "category":"Pedal_sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"24",
                    "length":1
                },
                {
                    "title":"制动时间参数",
                    "title_en":"Braking time parameter",
                    "category":"Pedal_sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"s",
                    "split":0,
                    "data":"25",
                    "length":1
                },
                {
                    "title":"速度信号通道数",
                    "title_en":"Speed signal channel number",
                    "category":"Pedal_sensor",
                    "range":[1,2],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"1-单通道，2-正交双通道",
                    "unit":"",
                    "split":0,
                    "data":"26",
                    "length":1
                },
                {
                    "title":"防跟转力矩检查齿数",
                    "title_en":"Check teeth for heel torque",
                    "category":"Pedal_sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"27",
                    "length":1
                },
                {
                    "title":"电机类型",
                    "title_en":"Motor type",
                    "category":"Motor",
                    "range":[0,1,2],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0-轮毂电机，1-中置电机，2-直驱电机（踏板车）",
                    "unit":"",
                    "split":0,
                    "data":"28",
                    "length":1
                },
                {
                    "title":"电机极对数",
                    "title_en":"Motor pole logarithm",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"29",
                    "length":1
                },
                {
                    "title":"测速磁钢数",
                    "title_en":"Magnetic steel number for velocity measurement",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"30",
                    "length":1
                },
                {
                    "title":"温度传感器",
                    "title_en":"Temperature sensor",
                    "category":"Motor",
                    "range":[0,1,2],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0-无，1-10K 3435，2-PT1000",
                    "unit":"",
                    "split":0,
                    "data":"31",
                    "length":1
                },
                {
                    "title":"减速比",
                    "title_en":"Deceleration ratio (multiplied by 0.01)",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"乘以0.01",
                    "unit":"",
                    "split":0,
                    "data":"32-33",
                    "length":2
                },
                {
                    "title":"电机机芯最高转速",
                    "title_en":"Max. speed of rotor",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"不明确或不需要填0",
                    "unit":"RPM",
                    "split":0,
                    "data":"34-35",
                    "length":2
                },
                {
                    "title":"D轴电感",
                    "title_en":"D-axis  inductance",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"不明确或不需要填0",
                    "unit":"uH",
                    "split":0,
                    "data":"36-37",
                    "length":2
                },
                {
                    "title":"Q轴电感",
                    "title_en":"Q-axis  inductance",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"不明确或不需要填0",
                    "unit":"uH",
                    "split":0,
                    "data":"38-39",
                    "length":2
                },
                {
                    "title":"相限电阻",
                    "title_en":"Phase resistance",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"不明确或不需要填0",
                    "unit":"mΩ",
                    "split":0,
                    "data":"40-41",
                    "length":2
                },
                {
                    "title":"反电势系数",
                    "title_en":"Reverse potential coefficient",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"不明确或不需要填0",
                    "unit":"（0.001V/RPM）",
                    "split":0,
                    "data":"42-43",
                    "length":2
                },
                {
                    "title":"电机冲片直径",
                    "title_en":"Diameter of motor blanking",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"mm",
                    "split":0,
                    "data":"44-46",
                    "length":3
                },
                {
                    "title":"电机冲片槽数",
                    "title_en":"Number of motor chip slots",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"47",
                    "length":1
                },
                {
                    "title":"电机冲片叠厚",
                    "title_en":"Motor lamination thickness",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"mm",
                    "split":0,
                    "data":"48-50",
                    "length":3
                },
                {
                    "title":"电机绕组参数",
                    "title_en":"Motor winding parameters",
                    "category":"Motor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"匝数",
                    "split":0,
                    "data":"51",
                    "length":1
                },
                {
                    "title":"转把启动电压",
                    "title_en":"Throttle start voltage",
                    "category":"Throttle",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"0.1DCV",
                    "split":0,
                    "data":"52",
                    "length":1
                },
                {
                    "title":"转把满度电压",
                    "title_en":"Throttle max. output voltage",
                    "category":"Throttle",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"0.1DCV",
                    "split":0,
                    "data":"53",
                    "length":1
                },
                {
                    "title":"限速开关功能",
                    "title_en":"Speed limit switch function",
                    "category":"Throttle",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0-不限速，1-限速",
                    "unit":"",
                    "split":0,
                    "data":"54",
                    "length":1
                },
                {
                    "title":"限速速度设置",
                    "title_en":"Speed limit setting",
                    "category":"Throttle",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"55",
                    "length":1
                },
                {
                    "title":"转把信号变化速度和电流输出系数",
                    "title_en":"Turn signal change speed and current output coefficient",
                    "category":"Throttle",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"56",
                    "length":1
                },
                {
                    "title":"启动电流",
                    "title_en":"Starting current",
                    "category":"Speed_Sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"57",
                    "length":1
                },
                {
                    "title":"电流加载时间",
                    "title_en":"Current loading time",
                    "category":"Speed_Sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"0.1s",
                    "split":0,
                    "data":"58",
                    "length":1
                },
                {
                    "title":"电流减载时间",
                    "title_en":"Current load shedding time",
                    "category":"Speed_Sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"0.1s",
                    "split":0,
                    "data":"59",
                    "length":1
                },
                {
                    "title":"速度助力信号有效时间",
                    "title_en":"Speed boost signal effective time",
                    "category":"Speed_Sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"s",
                    "split":0,
                    "data":"60-61",
                    "length":2
                },
                {
                    "title":"踏频速度和电流输出系数",
                    "title_en":"Tread speed and current output coefficient",
                    "category":"Speed_Sensor",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"62",
                    "length":1
                },
            ],
            "6012":[
                {
                    "title":"档位1-限流百分比",
                    "title_en":"Current limit percentage for level 1",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"0",
                    "length":1
                },
                {
                    "title":"档位2-限流百分比",
                    "title_en":"Current limit percentage for level 2",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"1",
                    "length":1
                },
                {
                    "title":"档位3-限流百分比",
                    "title_en":"Current limit percentage for level 3",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"2",
                    "length":1
                },
                {
                    "title":"档位4-限流百分比",
                    "title_en":"Current limit percentage for level 4",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"3",
                    "length":1
                },
                {
                    "title":"档位5-限流百分比",
                    "title_en":"Current limit percentage for level 5",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"4",
                    "length":1
                },
                {
                    "title":"档位6-限流百分比",
                    "title_en":"Current limit percentage for level 6",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"5",
                    "length":1
                },
                {
                    "title":"档位7-限流百分比",
                    "title_en":"Current limit percentage for level 7",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"6",
                    "length":1
                },
                {
                    "title":"档位8-限流百分比",
                    "title_en":"Current limit percentage for level 8",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"7",
                    "length":1
                },
                {
                    "title":"档位9-限流百分比",
                    "title_en":"Current limit percentage for level 9",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"8",
                    "length":1
                },
                {
                    "title":"档位1-限速百分比",
                    "title_en":"Speed limit percentage for level 1",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"9",
                    "length":1
                },
                {
                    "title":"档位2-限速百分比",
                    "title_en":"Speed limit percentage for level 2",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"10",
                    "length":1
                },
                {
                    "title":"档位3-限速百分比",
                    "title_en":"Speed limit percentage for level 3",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"11",
                    "length":1
                },
                {
                    "title":"档位4-限速百分比",
                    "title_en":"Speed limit percentage for level 4",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"12",
                    "length":1
                },
                {
                    "title":"档位5-限速百分比",
                    "title_en":"Speed limit percentage for level 5",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"13",
                    "length":1
                },
                {
                    "title":"档位6-限速百分比",
                    "title_en":"Speed limit percentage for level 6",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"14",
                    "length":1
                },
                {
                    "title":"档位7-限速百分比",
                    "title_en":"Speed limit percentage for level 7",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"15",
                    "length":1
                },
                {
                    "title":"档位8-限速百分比",
                    "title_en":"Speed limit percentage for level 8",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"16",
                    "length":1
                },
                {
                    "title":"档位9-限速百分比",
                    "title_en":"Speed limit percentage for level 9",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"17",
                    "length":1
                },
                {
                    "title":"助推最大电流",
                    "title_en":"Boost max. current",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"18",
                    "length":1
                },
                {
                    "title":"助推档限速百分比",
                    "title_en":"Percent boost speed limit",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"19",
                    "length":1
                },
                {
                    "title":"助推速度设置",
                    "title_en":"Presence of booster",
                    "category":"Level",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0有助推  1无助推",
                    "unit":"",
                    "split":0,
                    "data":"20",
                    "length":1
                },
                {
                    "title":"助推电流变化斜率系数",
                    "title_en":"Boost current change slope coefficient",
                    "category":"Level",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"21",
                    "length":1
                },
                {
                    "title":"有无仪表",
                    "title_en":"Have or not display",
                    "category":"System",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0 有仪表， 1 无仪表",
                    "unit":"",
                    "split":0,
                    "data":"22",
                    "length":1
                },
                {
                    "title":"大灯常亮",
                    "title_en":"Lamps are normally on",
                    "category":"System",
                    "range":[0,1],
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"0 - 随仪表，1 - 上电常亮",
                    "unit":"",
                    "split":0,
                    "data":"23",
                    "length":1
                },
                {
                    "title":"启动力矩值-0",
                    "title_en":"Starting torque value-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"24",
                    "length":1
                },
                {
                    "title":"启动力矩值-1",
                    "title_en":"Starting torque value-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"25",
                    "length":1
                },
                {
                    "title":"启动力矩值-2",
                    "title_en":"Starting torque value-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"26",
                    "length":1
                },
                {
                    "title":"启动力矩值-3",
                    "title_en":"Starting torque value-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"27",
                    "length":1
                },
                {
                    "title":"启动力矩值-4",
                    "title_en":"Starting torque value-4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"28",
                    "length":1
                },
                {
                    "title":"满载力矩值-0",
                    "title_en":"Full load torque value-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"29",
                    "length":1
                },
                {
                    "title":"满载力矩值-1",
                    "title_en":"Full load torque value-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"30",
                    "length":1
                },
                {
                    "title":"满载力矩值-2",
                    "title_en":"Full load torque value-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"31",
                    "length":1
                },
                {
                    "title":"满载力矩值-3",
                    "title_en":"Full load torque value-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"32",
                    "length":1
                },
                {
                    "title":"满载力矩值-4",
                    "title_en":"Full load torque value-4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"33",
                    "length":1
                },
                {
                    "title":"返回力矩值-0",
                    "title_en":"Return torque value-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"34",
                    "length":1
                },
                {
                    "title":"返回力矩值-1",
                    "title_en":"Return torque value-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"35",
                    "length":1
                },
                {
                    "title":"返回力矩值-2",
                    "title_en":"Return torque value-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"36",
                    "length":1
                },
                {
                    "title":"返回力矩值-3",
                    "title_en":"Return torque value-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"37",
                    "length":1
                },
                {
                    "title":"返回力矩值-4",
                    "title_en":"",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"Kg",
                    "split":0,
                    "data":"38",
                    "length":1
                },
                {
                    "title":"最大电流-0",
                    "title_en":"Maximum current-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"39",
                    "length":1
                },
                {
                    "title":"最大电流-1",
                    "title_en":"Maximum current-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"40",
                    "length":1
                },
                {
                    "title":"最大电流-2",
                    "title_en":"Maximum current-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"41",
                    "length":1
                },
                {
                    "title":"最大电流-3",
                    "title_en":"Maximum current-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"42",
                    "length":1
                },
                {
                    "title":"最大电流-4",
                    "title_en":"Maximum current-4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"43",
                    "length":1
                },
                {
                    "title":"最小电流-0",
                    "title_en":"Minimum current-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"44",
                    "length":1
                },
                {
                    "title":"最小电流-1",
                    "title_en":"Minimum current-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"45",
                    "length":1
                },
                {
                    "title":"最小电流-2",
                    "title_en":"Minimum current-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"46",
                    "length":1
                },
                {
                    "title":"最小电流-3",
                    "title_en":"Minimum current-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"47",
                    "length":1
                },
                {
                    "title":"最小电流-4",
                    "title_en":"Minimum current-4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"48",
                    "length":1
                },
                {
                    "title":"力矩衰减-0",
                    "title_en":"Torque attenuation-0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"49",
                    "length":1
                },
                {
                    "title":"力矩衰减-1",
                    "title_en":"Torque attenuation-1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"50",
                    "length":1
                },
                {
                    "title":"力矩衰减-2",
                    "title_en":"Torque attenuation-2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"51",
                    "length":1
                },
                {
                    "title":"力矩衰减-3",
                    "title_en":"Torque attenuation-3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"52",
                    "length":1
                },
                {
                    "title":"力矩衰减-4",
                    "title_en":"Torque attenuation-4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"53",
                    "length":1
                },
                {
                    "title":"启动脉冲数-0",
                    "title_en":"Start pulse number 0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"54",
                    "length":1
                },
                {
                    "title":"启动脉冲数-1",
                    "title_en":"Start pulse number 1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"55",
                    "length":1
                },
                {
                    "title":"启动脉冲数-2",
                    "title_en":"Start pulse number 2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"56",
                    "length":1
                },
                {
                    "title":"启动脉冲数-3",
                    "title_en":"Start pulse number 3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"57",
                    "length":1
                },
                {
                    "title":"启动脉冲数-4",
                    "title_en":"Start pulse number 4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"",
                    "split":0,
                    "data":"58",
                    "length":1
                }
            ],
            "6013":[
                {
                    "title":"电流衰减时间-0",
                    "title_en":"Current decay time 0",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"5ms",
                    "unit":"",
                    "split":0,
                    "data":"0",
                    "length":1
                },
                {
                    "title":"电流衰减时间-1",
                    "title_en":"Current decay time 1",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"5ms",
                    "unit":"",
                    "split":0,
                    "data":"1",
                    "length":1
                },
                {
                    "title":"电流衰减时间-2",
                    "title_en":"Current decay time 2",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"5ms",
                    "unit":"",
                    "split":0,
                    "data":"2",
                    "length":1
                },
                {
                    "title":"电流衰减时间-3",
                    "title_en":"Current decay time 3",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"5ms",
                    "unit":"",
                    "split":0,
                    "data":"3",
                    "length":1
                },
                {
                    "title":"电流衰减时间-4",
                    "title_en":"Current decay time 4",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"5ms",
                    "unit":"",
                    "split":0,
                    "data":"4",
                    "length":1
                },
                {
                    "title":"力矩信号加载速度系数",
                    "title_en":"Torque signal loading speed coefficient",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"5",
                    "length":1
                },
                {
                    "title":"助力速度传感器加载系数",
                    "title_en":"Load factor of booster speed sensor",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"6",
                    "length":1
                },
                {
                    "title":"助力速度传感器速度参考值",
                    "title_en":"Power speed sensor speed reference value",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"RPM",
                    "split":0,
                    "data":"7",
                    "length":1
                },
                {
                    "title":"速度助力传感器跟随空载电流值",
                    "title_en":"The speed boost sensor follows the no-load current value",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"8",
                    "length":1
                },
                {
                    "title":"输出功率随力矩信号衰减系数",
                    "title_en":"Attenuation coefficient of output power with torque signal",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"9",
                    "length":1
                },
                {
                    "title":"输出功率随速度助力信号衰减系数",
                    "title_en":"The attenuation coefficient of the output power along with the speed boost signal",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"10",
                    "length":1
                },
                {
                    "title":"力矩信号优先时间",
                    "title_en":"Torque signal priority time",
                    "category":"Torque",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"ms",
                    "split":0,
                    "data":"11-12",
                    "length":2
                },
                {
                    "title":"电机温度安全阈值",
                    "title_en":"Motor temperature safety threshold",
                    "category":"Motor_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"140",
                    "unit":"℃",
                    "split":0,
                    "data":"13",
                    "length":1
                },
                {
                    "title":"输出功率随温升变化衰减系数",
                    "title_en":"The attenuation coefficient of output power varies with temperature rise",
                    "category":"Motor_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"14",
                    "length":1
                },
                {
                    "title":"温度衰减启动值",
                    "title_en":"Temperature attenuation start value",
                    "category":"Motor_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"120",
                    "unit":"℃",
                    "split":0,
                    "data":"15",
                    "length":1
                },
                {
                    "title":"电机过热温度",
                    "title_en":"Motor overheat temperature",
                    "category":"Motor_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"150",
                    "unit":"℃",
                    "split":0,
                    "data":"16",
                    "length":1
                },
                {
                    "title":"过热温度固定输出电流",
                    "title_en":"Overheat temperature fixed output current",
                    "category":"Motor_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"17",
                    "length":1
                },
                {
                    "title":"控制器温度安全阈值",
                    "title_en":"Controller temperature safety threshold",
                    "category":"Controller_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"90",
                    "unit":"℃",
                    "split":0,
                    "data":"18",
                    "length":1
                },
                {
                    "title":"输出功率随温升变化衰减系数",
                    "title_en":"The attenuation coefficient of output power varies with temperature rise",
                    "category":"Controller_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"%",
                    "split":0,
                    "data":"19",
                    "length":1
                },
                {
                    "title":"温度衰减启动值",
                    "title_en":"Temperature attenuation start value",
                    "category":"Controller_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"80",
                    "unit":"℃",
                    "split":0,
                    "data":"20",
                    "length":1
                },
                {
                    "title":"控制器过热温度",
                    "title_en":"Overheat temperature of controller",
                    "category":"Controller_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"95",
                    "unit":"℃",
                    "split":0,
                    "data":"21",
                    "length":1
                },
                {
                    "title":"过热温度固定输出电流",
                    "title_en":"Overheat temperature fixed output current",
                    "category":"Controller_Temperature",
                    "range":0,
                    "read_value":"",
                    "standard_value":"",
                    "set_value":"",
                    "description":"",
                    "unit":"A",
                    "split":0,
                    "data":"22",
                    "length":1
                }
            ]
        }
    ]
}
```


##### How BESST processes CANBUS data for 6011 6012 and 6013

```
        conParamsRead_6011: function conParamsRead_6011() {
            var _this4 = this;

            _controller2.default.port.usbSDK.machineInformation.get.conParams(0x05, 0x02).then(function (res) {
                _this4.conParamsRead_11 = res.data;

                if (_this4.conParamsRead_11) {

                    var checkSum = _core2.default.Util.calculateChecksumFF(_this4.conParamsRead_11.substr(0, 126));
                    console.log(checkSum);
                    if (checkSum == parseInt(_this4.conParamsRead_11.substr(126, 2), 16)) {
                        _this4.$message("数据校验通过");
                        _this4.ctrlParams6011.forEach(function (item) {
                            var ind = parseInt(item.data.substr(0, 2));
                            var len = item.length;
                            if (len == 1) {
                                item.read_value = parseInt(_this4.conParamsRead_11.substr(ind * 2, len * 2), 16);
                                item.set_value = parseInt(_this4.conParamsRead_11.substr(ind * 2, len * 2), 16);
                            } else {
                                var data = _this4.conParamsRead_11.substr(ind * 2, len * 2);
                                var arr = Buffer.from(data, 'Hex').reverse();
                                item.read_value = parseInt(arr.toString('Hex'), 16);
                                item.set_value = parseInt(arr.toString('Hex'), 16);
                            }
                        });
                    } else {
                        _this4.$message("数据校验不通过");
                    }
                }
            }).catch(function (err) {
                console.log(err);
            }).finally(function () {
                console.log('conParams 11');
            });
        },
        conParamsRead_6012: function conParamsRead_6012() {
            var _this5 = this;

            _controller2.default.port.usbSDK.machineInformation.get.conParams_2(0x05, 0x02).then(function (res) {

                _this5.conParamsRead_12 = res.data;

                if (_this5.conParamsRead_12) {

                    var checkSum = _core2.default.Util.calculateChecksumFF(_this5.conParamsRead_12.substr(0, 126));
                    console.log(checkSum);
                    if (checkSum == parseInt(_this5.conParamsRead_12.substr(126, 2), 16)) {
                        _this5.$message("数据校验通过");
                        _this5.ctrlParams6012.forEach(function (item) {
                            var ind = parseInt(item.data.substr(0, 2));
                            var len = item.length;
                            if (len == 1) {
                                item.read_value = parseInt(_this5.conParamsRead_12.substr(ind * 2, len * 2), 16);
                                item.set_value = parseInt(_this5.conParamsRead_12.substr(ind * 2, len * 2), 16);
                            } else {
                                var data = _this5.conParamsRead_12.substr(ind * 2, len * 2);
                                var arr = Buffer.from(data, 'Hex').reverse();
                                item.read_value = parseInt(arr.toString('Hex'), 16);
                                item.set_value = parseInt(arr.toString('Hex'), 16);
                            }
                        });
                    } else {
                        _this5.$message("数据校验不通过");
                    }
                }
            }).catch(function (err) {
                console.log(err);
            }).finally(function () {
                console.log('conParams 12');
            });
        },
        conParamsRead_6013: function conParamsRead_6013() {
            var _this6 = this;

            _controller2.default.port.usbSDK.machineInformation.get.conParams_3(0x05, 0x02).then(function (res) {

                _this6.conParamsRead_13 = res.data;

                if (_this6.conParamsRead_13) {

                    var checkSum = _core2.default.Util.calculateChecksumFF(_this6.conParamsRead_13.substr(0, 126));
                    console.log(checkSum);
                    if (checkSum == parseInt(_this6.conParamsRead_13.substr(126, 2), 16)) {
                        _this6.$message("数据校验通过");
                        _this6.ctrlParams6013.forEach(function (item) {
                            var ind = parseInt(item.data.substr(0, 2));
                            var len = item.length;
                            if (len == 1) {
                                item.read_value = parseInt(_this6.conParamsRead_13.substr(ind * 2, len * 2), 16);
                                item.set_value = parseInt(_this6.conParamsRead_13.substr(ind * 2, len * 2), 16);
                            } else {
                                var data = _this6.conParamsRead_13.substr(ind * 2, len * 2);
                                var arr = Buffer.from(data, 'Hex').reverse();
                                item.read_value = parseInt(arr.toString('Hex'), 16);
                                item.set_value = parseInt(arr.toString('Hex'), 16);
                            }
                        });
                    } else {
                        _this6.$message("数据校验不通过");
                    }
                }
            }).catch(function (err) {
                console.log(err);
            }).finally(function () {
                console.log('conParams 12');
            });
        },
        conParamsCombine_6011: function conParamsCombine_6011() {
            var _this7 = this;

            var data = '';

            this.ctrlParams6011.forEach(function (item) {
                var len = item.length;

                var dateTem = parseInt(item.set_value);
                var hexTemp = dateTem.toString(16);

                hexTemp = _this7.addLength(hexTemp, len);
                var arr = Buffer.from(hexTemp, 'Hex');
                data += arr.reverse().toString('Hex');
            });
            data = (data + 'ff').substr(0, 126);
            var checkSum = _core2.default.Util.calculateChecksumFF(data);
            console.log(checkSum);
            this.conParamsWrite_11 = data + Buffer.from([checkSum]).toString('Hex');
        },
        conParamsCombine_6012: function conParamsCombine_6012() {
            var _this8 = this;

            var data = '';

            this.ctrlParams6012.forEach(function (item) {
                var len = item.length;

                var dateTem = parseInt(item.set_value);
                var hexTemp = dateTem.toString(16);

                hexTemp = _this8.addLength(hexTemp, len);
                var arr = Buffer.from(hexTemp, 'Hex');
                data += arr.reverse().toString('Hex');
            });
            data = (data + 'ffffffffffffffffffffffffffffffffffffffffff').substr(0, 126);
            var checkSum = _core2.default.Util.calculateChecksumFF(data);
            console.log(checkSum);
            this.conParamsWrite_12 = data + Buffer.from([checkSum]).toString('Hex');
        },
        conParamsCombine_6013: function conParamsCombine_6013() {
            var _this9 = this;

            var data = '';

            this.ctrlParams6013.forEach(function (item) {
                var len = item.length;

                var dateTem = parseInt(item.set_value);
                var hexTemp = dateTem.toString(16);

                hexTemp = _this9.addLength(hexTemp, len);
                var arr = Buffer.from(hexTemp, 'Hex');
                data += arr.reverse().toString('Hex');
            });
            data = (data + 'ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff').substr(0, 126);
            var checkSum = _core2.default.Util.calculateChecksumFF(data);
            console.log(checkSum);
            this.conParamsWrite_13 = data + Buffer.from([checkSum]).toString('Hex');
        },
        addLength: function addLength(hexData, len) {
            var hex = hexData || '';
            var step = len * 2;
            var hexLength = hex.length / 2;
            if (len === hexLength) {
                return hex;
            } else if (len > hexLength) {
                return ('000000' + hex).substr(-step);
            } else {
                return '------'.substr(-step);
            }
        },
        conParamsUpdate_6011: function conParamsUpdate_6011() {
            var _this10 = this;

            if (this.conParamsWrite_11 == '') {
                this.conParamsCombine_6011();
            }
            _controller2.default.port.usbSDK.machineInformation.update.conParams(0x05, 0x02, this.conParamsWrite_11).then(function (res) {
                console.log(res);
                if (res.can === 'NORMAL_ACK') {
                    _this10.$message({
                        message: _this10.$t('cNam.writeSuccess'),
                        type: 'success'
                    });
                } else {
                    _this10.$message({
                        message: _this10.$t('cNam.writeFail'),
                        type: 'error'
                    });
                }
            }).catch(function (err) {
                console.log(err);
                _this10.usbErrorCheck(err);
            }).finally(function () {});
        },
        conParamsUpdate_6012: function conParamsUpdate_6012() {
            var _this11 = this;

            if (this.conParamsWrite_12 == '') {
                this.conParamsCombine_6012();
            }
            _controller2.default.port.usbSDK.machineInformation.update.conParams_2(0x05, 0x02, this.conParamsWrite_12).then(function (res) {
                console.log(res);
                if (res.can === 'NORMAL_ACK') {
                    _this11.$message({
                        message: _this11.$t('cNam.writeSuccess'),
                        type: 'success'
                    });
                } else {
                    _this11.$message({
                        message: _this11.$t('cNam.writeFail'),
                        type: 'error'
                    });
                }
            }).catch(function (err) {
                console.log(err);
                _this11.usbErrorCheck(err);
            }).finally(function () {});
        },
        conParamsUpdate_6013: function conParamsUpdate_6013() {
            var _this12 = this;

            if (this.conParamsWrite_13 == '') {
                this.conParamsCombine_6013();
            }
            _controller2.default.port.usbSDK.machineInformation.update.conParams_3(0x05, 0x02, this.conParamsWrite_13).then(function (res) {
                console.log(res);
                if (res.can === 'NORMAL_ACK') {
                    _this12.$message({
                        message: _this12.$t('cNam.writeSuccess'),
                        type: 'success'
                    });
                } else {
                    _this12.$message({
                        message: _this12.$t('cNam.writeFail'),
                        type: 'error'
                    });
                }
            }).catch(function (err) {
                console.log(err);
                _this12.usbErrorCheck(err);
            }).finally(function () {});
        },
```
