# CANBUS information from the BESST software sourcecode


## Summary

##### The following Canbus frameid's have been identified:

|Function|GET/SET|second-last-digit|last-digit|
|---|---|---|---|
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


#####


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



#####


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



#####


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



#####


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




#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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



#####


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