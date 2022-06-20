<template>
    <div class="sensor">
        <div class="sensor-content">
            <label class="title">
                <img class="icon" src="../../assets/images/icon/icon-sensor.png" />{{ $t('cNam.sensor') }}<span class="can">Can</span>
                <div class="connect-tip">
                    <a href="javascript:;" @click="dialogTipVisible = true" class="btn-tip btn-underline">
                        <img src="../../assets/images/icon/icon-help.png" class="tip-img" />
                    </a>
                </div>
            </label>
            <div class="info">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.mainData') }}</label>
                </div>
                <div class="info-left">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">SN</label>
                        <el-input class="input input-sn" v-model="sn" ></el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label">SN</label>
                        <el-input class="input" v-model="sn" ></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <!-- <el-button>Read</el-button> -->
                            <el-button type="primary" @click="writeSn"
                                       :disabled="isDealerOrAssembler && offLineFlag"
                                       v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">{{ $t('cNam.write') }}
                            </el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.model') }}</label>
                        <el-input class="input" v-model="model" disabled></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                        <el-input class="input" v-model="hardVersion" disabled></el-input>
                    </div>
                </div>
                <div class="info-right">
                    <div class="item">
                        <label class="label">USB HID</label>
                        <!--<el-select v-model="com" @change="changeCom" @click.native="getComList"-->
                        <!--:placeholder="$t('cNam.select')">-->
                        <!--<el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">-->
                        <!--</el-option>-->
                        <!--</el-select>-->
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button @click="connect" type="primary" :disabled = " !usbHID || loading || isReset">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>
                            <div class="btns">
                                <el-button type="preview" @click="readBase" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.read') }}</el-button>
                                <el-button type="primary" @click="reset" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.reset') }}</el-button>
                            </div>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                        <el-input class="input" v-model="version" disabled></el-input>
                    </div>
                </div>
            </div>
            <div class="info">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.parameter') }}</label>
                </div>
                <div class="info-left">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCodeRead" disabled></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.torque') }}</label>
                        <el-input class="input" v-model="torqueValue" disabled>
                            <template slot="append">mV</template>
                        </el-input>
                    </div>
                </div>
                <div class="info-right">
                    <div class="item">
                        <label class="label">{{ $t('cNam.cadence') }}</label>
                        <el-input class="input" v-model="cadence" disabled>
                            <template slot="append">RPM</template>
                        </el-input>
                    </div>
                </div>
            </div>
            <div class="info" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.setting') }}</label>
                </div>
                <div class="info-left">
                    <div class="item" >
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCode" placeholder="6004"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="writeParamWithAuth">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.manName') }}</label>
                        <el-input class="input" v-model="manufacturer" placeholder="6005"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="preview" @click="readManufacturer">{{ $t('cNam.read') }}</el-button>
                            <el-button type="primary" @click="writeManufacturer">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <!--<div class="item" >-->
                    <!--<label class="label">{{ $t('cNam.model') }}</label>-->
                    <!--<el-input class="input" v-model="modelWrite"></el-input>-->
                    <!--</div>-->
                    <!--<div class="item" >-->
                    <!--<label class="label">{{ $t('cNam.hardwareVersion') }}</label>-->
                    <!--<el-input class="input" v-model="hardwareWrite"></el-input>-->
                    <!--</div>-->
                    <!--<div class="btn-row">-->
                    <!--<div class="btn-group">-->
                    <!--<el-button type="info " @click="clickCopy">复制数据</el-button>-->
                    <!--<el-button type="preview" @click="clickWriteReset">{{ $t('cNam.write') }}</el-button>-->
                    <!--</div>-->
                    <!--</div>-->
                </div>
                <div class="info-right">
                    <div class="item">
                        <label class="label">{{ $t('cNam.errorCode') }}</label>
                        <el-input class="input" v-model="errorCode" placeholder="6007"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="preview" @click="readErrorCode">{{ $t('cNam.read') }}</el-button>
                            <!--<el-button type="primary" @click="writeErrorCode">{{ $t('cNam.write') }}</el-button>-->
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.bootLoad') }}</label>
                        <el-input class="input" v-model="bootLoad" placeholder="6008"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="preview" @click="readBootLoad">{{ $t('cNam.read') }}</el-button>
                            <!--<el-button type="primary" @click="writeBootLoad">{{ $t('cNam.write') }}</el-button>-->
                        </div>
                    </div>
                </div>
            </div>
            <!--
            <div class="info record" v-if="offLineFlag">
                <div class="wrap">
                    <p class="title-record">Offline Operation Record</p>
                    <p><span>Remain Operation Amount:</span> <span class="number">{{ 100 - offlineRecordLength}}</span>
                    </p>
                    <p v-if = "offlineRecordLength > 97" >
                       <span class="number"> {{ $t('cNam.offlineCon') }} </span>
                    </p>
                </div>
            </div>
            -->
        </div>

        <!--说明弹框-->
        <el-dialog :title="$t('cNam.operationTip')" custom-class="tip-dialog"
                   :visible.sync="dialogTipVisible">
            <div>
                <h1> {{ $t('cNam.txtTitle1') }} </h1>
                <div class="txt"><span class="itemName">USB HID</span>   <span>{{ $t('cNam.txtTitleHid') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.connect') }}</span> <span>{{ $t('cNam.txtTitleCon') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.read') }}</span>    <span>{{ $t('cNam.txtTitleRead') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.write') }}</span>   <span>{{ $t('cNam.txtTitleWrt') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.reset') }}</span>   <span>{{ $t('cNam.txtTitleReset') }}</span></div>
            </div>
            <div class="clear-20"></div>
            <div class="">
                <h1>
                </h1>
                <div class="txt">
                </div>
                <div class="txt">
                </div>
                <div class="txt">
                </div>
            </div>

        </el-dialog>


    </div>
</template>

<script>
    import Sensor from "../../../device/sensor"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import vueLoading from 'vue-loading-template'
    import Core from 'core/core'

    export default {
        name: 'ToolSensor',
        components: {
            vueLoading
        },
        data() {
            return {
                isConnected: Sensor.port.isConnected(),
                com: '',
                comList: [],

                sn: '',
                model: '',
                version: '',
                hardVersion: '',
                clientCodeRead: '',
                torqueValue: '',
                cadence: '',

                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',

                writeStatus: false,
                loading: false,

                isDealerOrAssembler: false,
                dialogWriteAuthVisible: false,
                brandId: '',
                brandList: [],
                auth: "",

                offLineFlag: Core.Data.getOffline(),

                org: Core.Data.getOrg(),
                Core: Core,

                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.EDIT,

                readSn: '', // 读取到的sn
                offlineRecordList:[],
                offlineRecordLength:0,

                dialogTipVisible:false,
                isReset: false,
                isCan: false,
                usbHID: false,
                connectChange: false,

                clientCode: '',
                modelWrite: '',
                hardwareWrite: '',
                errorCode: '',
                bootLoad: '',
                manufacturer: '',

                reconnectInterval: null,
                connectionIntervalSen: null,
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Tool.Hmi,
                zh: ZH.Components.Tool.Hmi,
                de: DE.Components.Tool.Hmi,
                nl: NL.Components.Tool.Hmi
            }
        },

        mounted() {
            this.isDealerOrAssembler = Core.Data.getOrg().type === Core.Const.ORG_TYPE.DEALER || Core.Data.getOrg().type === Core.Const.ORG_TYPE.OEM || Core.Data.getOrg().type === Core.Const.ORG_TYPE.SPECIAL_DEAlER;
            this.offLineFlag = Core.Data.getOffline();
            this.userId = Core.Data.getUser().id;

            this.org = Core.Data.getOrg();

            this.getOfflineRecordList();
            Sensor.port.timeLog('Sensor');
            Sensor.port.setBoxOperation();
            this.checkConnection();
        },
        methods: {
            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalSen){
                    clearInterval(vm.connectionIntervalSen);
                    vm.connectionIntervalSen = '';
                }
                vm.connectionIntervalSen = setInterval(()=>{
                    let connection = Sensor.port.getConnection();
                    if(connection.usb){
//                        this.checkUSBUartOrCan();
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalSen);
                    }
                },300)
            },
//            checkUSBUartOrCan(){
//                let checkUSBUartOrCanIntervalSen = setInterval(()=>{
//                    let USBUartOrCan = Sensor.port.getConnection();
//                    if(USBUartOrCan.usb){
//                        if(USBUartOrCan.can){
//                            this.isCan = true;
//                        }
//                        if(USBUartOrCan.uart){
//                            this.isCan = false;
//                        }
//                        if(USBUartOrCan.can || USBUartOrCan.uart){
//                            clearInterval(checkUSBUartOrCanIntervalSen);
//                        }
//                    }
//                },800)
//            },
            init(){
                Sensor.port.startConnection();
                if(Sensor.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
                    console.log(" get USB Device Sensor");
                    if(this.isReset){
                        this.connect();
                    }else{
                        this.connect(1);
                    }
                }
                this.isConnected = Sensor.port.isConnected();
            },

            connect(type) {
                if (!this.usbHID) {
                    this.$message({
                        message: this.$t('cNam.noCanConnect'),
                        type: 'warning'
                    });
                    return;
                }
                if ( this.usbHID ) {
                    if(type === 1){
                        this.test();
                    }else{
                        setTimeout(()=>{
                            this.test();
                        },1000)
                    }
                    this.isConnected = true;
                    console.log("usbHID connect");
                    Sensor.port.getListenData().subscribe((res)=> {
                        if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb') {
                            this.isConnected = false;
                            this.clear();
                        }
                    });
                }
            },
            test(){
                Sensor.port.checkUsbConnection('0101',1);
            },
            readBase() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.clear();
                this.loading = true;
                this.connect(1);
                setTimeout(() => {
                    if (this.loading) {
                        if (!this.checkData()) {
                            this.$message({
                                message: this.$t('cNam.sensorTip'),
                                type: "warning"
                            });
                        }
                    }
                    this.loading = false;
                }, 1200);
//                Sensor.sendCmdGetFullData();
//                Sensor.getBus().subscribe(res => {
////                    console.log('sensor-vm2:', res);
//                    if (res.type === 'close' || res.type === 'error') {
//                        this.isConnected = false;
//                    }
//                    if (!res.data || !res.data.sensor) {
//                        return;
//                    }
//                    if (res.data.sensor.hasOwnProperty("sn")) {
//                        this.sn = res.data.sensor.sn;
//                        this.readSn = this.sn;
//                    }
//                    if (res.data.sensor.hasOwnProperty("model")) {
//                        this.model = res.data.sensor.model;
//                    }
//                    if (res.data.sensor.hasOwnProperty("version")) {
//                        this.version = res.data.sensor.version;
//                    }
//                    if (res.data.sensor.hasOwnProperty("hardware_version")) {
//                        this.hardVersion = res.data.sensor.hardware_version;
//                    }
//                    if (res.data.sensor.hasOwnProperty("man_name")) {
//                        this.manName = res.data.sensor.man_name;
//                    }
//                    if (res.data.sensor.hasOwnProperty("torque_value")) {
//                        this.torqueValue = res.data.sensor.torque_value;
//                    }
//                    if (res.data.sensor.hasOwnProperty("cadence")) {
//                        this.cadence = res.data.sensor.cadence;
//                    }
//                });
                Sensor.port.getUsbBasic(0x01).subscribe(res => {
                    if (!res.data) {
                        return;
                    }
                    if (res.data.hasOwnProperty("sn")) {
                        this.sn = res.data.sn;
                        this.readSn = this.sn;
                    }
                    if (res.data.hasOwnProperty("model")) {
                        this.model = res.data.model;
                    }
                    if (res.data.hasOwnProperty("version")) {
                        this.version = res.data.version;
                    }
                    if (res.data.hasOwnProperty("hardware_version")) {
                        this.hardVersion = res.data.hardware_version;
                    }
                    if (res.data.hasOwnProperty("client_code")) {
                        this.clientCodeRead = res.data.client_code;
                    }
                });

                Sensor.port.getListenData().subscribe((res) => {
                    if (!res.data) {
                        return;
                    }
                    if (res.type == 'error_usb') {
                        this.isConnected = false;
                        this.clear();
                    }
                    if (res.data.code === '31' && res.data.subCode === '00') {
                        let result = Buffer.from(res.data.data, 'hex');
                        // console.log(result);
                        if(result.length >=3 ){
                            this.torqueValue = result[0] + result[1] * 256;
                            this.cadence = result[2];
                        }
                    }
                });
            },
            usbErrorCheck(err){
                Sensor.port.checkUsbError(err);
            },
            writeSn() {
                if (!this.sn) {
                    this.$message({
                            message: this.$t('cNam.snEmptyTip'),
                            type: "warning"
                        }
                    );
                    return;
                }
                this.sn = this.sn.replace(/\s/g, "");
                Sensor.port.usbSDK.machineInformation.update.serialNumber(0x05, 0x01, Sensor.port.Core.Util.stringToHex(this.sn))
                    .then(res => {
//                        console.log(res);
                        if (res.can === 'NORMAL_ACK') {
                            this.sn = '';
                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                            Sensor.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x01)
                                .then(res =>{
                                    this.sn = res.dataParsed;
                                    this.readSn = this.sn;
                                }).catch(err =>{
                                console.log(err);
                                this.usbErrorCheck(err);
                            }).finally(()=>{
                            });
                        }else{
                            this.$message({
                                message: this.$t('cNam.writeFail'),
                                type: 'error'
                            });
                        }

                    }).catch(err =>{
                    console.log(err);
                    this.usbErrorCheck(err);
                }).finally(()=>{
                    this.writeStatus = false;
                });
//                this.writeSnWithAuth();

            },

//            writeSnWithAuth() {
//                this.writeStatus = true;
//                Sensor.writeSnCan(this.sn);
//                Sensor.getBus().subscribe(res => {
//                    console.log('sensor-vm3:', res);
//                    if (res.type === 'close' || res.type === 'error') {
//                        this.isConnected = false;
//                    }
//                    if (!this.writeStatus) {
//                        return;
//                    }
//                    if (res.data.sensor && 'sn' in res.data.sensor) {
//                        let sn = res.data.sensor.sn;
//                        console.log(sn, this.sn);
//                        if (sn === this.sn || sn == ' ') {
//                            this.sn = '';
//                            this.$message({
//                                message: this.$t('cNam.writeSuccess'),
//                                type: 'success'
//                            });
//
//                        } else {
//                            this.$message({
//                                message: this.$t('cNam.writeFail'),
//                                type: 'error'
//                            });
//                        }
//                        this.writeStatus = false;
//                    }
//                });
//            },


            clickWriteParam() {
                let data = {
                    old: {
                        "manName": this.clientCode,
                    },
                    new: {
                        "manName": this.clientCode,
                    },
                };
                // 判断sn //
                if( Core.Data.getOrg().type != Core.Const.ORG_TYPE.COMPONENT ){
                    if (!this.sn || this.sn !== this.readSn) {
                        this.$message.warning(this.$t('needReadSn'));
                        return;
                    }
                }

                if (this.offLineFlag) {
                    // 如果离线

                    // 如果上一次的数据在被处理，需要等待
                    let isHandle = Core.Data.getIsBatchRecordIng();
                    if (isHandle) {
                        this.$message.warning(this.$t('offlineHandle'));
                        return;
                    }
                    this.getOfflineRecordList();
                    // 离线的到了 100 条 需要联网
                    if (this.offlineRecordList.length >= 30) {
                        this.$message.warning(this.$t('offlineCon'));
                        return;
                    }
                    this.writeParamWithAuth(data);
                } else {
                    this.writeParamWithAuth(data);
                }
            },

            writeParamWithAuth() {
                this.clientCode = this.clientCode.replace(/\s/g, "");
                if ( this.clientCode ) {
                    if( this.clientCode.length < 25){
                        Sensor.port.usbSDK.machineInformation.update.customerNumber(0x05, 0x01, Sensor.port.Core.Util.stringToHex(this.clientCode))
                            .then(res => {
//                                    console.log(res);
                                if (res.can === 'NORMAL_ACK') {
                                    this.clientCode = '';
                                    this.$message({
                                        message: this.$t('cNam.writeSuccess'),
                                        type: 'success'
                                    });
                                }else{
                                    this.$message({
                                        message: this.$t('cNam.writeFail'),
                                        type: 'error'
                                    });
                                }
                            }).catch(err =>{
                            console.log(err);
                            this.usbErrorCheck(err);
                        }).finally(()=>{
                            this.writeStatus = false;
                        });
                    }
                }
            },

            getOfflineRecordList(){
                if(this.offLineFlag){
                    this.offlineRecordList = Core.Data.getRecordList() || [];
                }
            },
            clickCopy(){
                this.modelWrite = this.model;
                this.hardwareWrite = this.hardVersion;
            },
            clickWriteReset(){
                if(this.modelWrite && this.modelWrite.indexOf('SR')>-1){
                    Sensor.port.usbSDK.machineInformation.update.modelNumber(0x05, 0x01, Sensor.port.Core.Util.stringToHex(this.modelWrite.trim()))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.modelWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Sensor.port.usbSDK.machineInformation.get.modelNumber(0x05, 0x01)
                                    .then(res =>{
                                        this.modelWrite = res.dataParsed;
                                    }).catch(err =>{
                                    console.log(err);
                                    this.usbErrorCheck(err);
                                }).finally(()=>{
                                });
                            }else{
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }
                        }).catch(err =>{
                        console.log(err);
                        this.usbErrorCheck(err);
                    }).finally(()=>{
                        this.writeStatus = false;
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeModelTip'),
                        type: 'warning'
                    });
                }
                if(this.hardwareWrite && this.hardwareWrite.indexOf('SR')>-1){
                    Sensor.port.usbSDK.machineInformation.update.hardwareVersionNumber(0x05, 0x01, Sensor.port.Core.Util.stringToHex(this.hardwareWrite.trim()))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.hardwareWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Sensor.port.usbSDK.machineInformation.get.hardwareVersionNumber(0x05, 0x01)
                                    .then(res =>{
                                        this.hardwareWrite = res.dataParsed;
                                    }).catch(err =>{
                                    console.log(err);
                                    this.usbErrorCheck(err);
                                }).finally(()=>{
                                });
                            }else{
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }
                        }).catch(err =>{
                        console.log(err);
                        this.usbErrorCheck(err);
                    }).finally(()=>{
                        this.writeStatus = false;
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeHardwareTip'),
                        type: 'warning'
                    });
                }
            },
            readManufacturer() {
                this.manufacturer = '';
                Sensor.port.usbSDK.machineInformation.get.manufacturer(0x05,0x01)
                    .then(res=>{
                        this.manufacturer = res.dataParsed.replace(/\s+/g, "");
                        this.$message({
                            message: this.$t('readSuccess'),
                            type: 'success'
                        });
                    }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                });
            },
            writeManufacturer() {
                Sensor.port.usbSDK.machineInformation.update.manufacturer(0x05,0x01,Sensor.port.Core.Util.stringToHex(this.manufacturer.trim()))
                    .then(res=>{
                        if (res.can === 'NORMAL_ACK') {
                            this.$message({
                                message: this.$t("writeSuccess"),
                                type: 'success'
                            });
                        }
                    }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                });
            },
            readErrorCode() {
                this.errorCode = '';
                Sensor.port.usbSDK.machineInformation.get.errorCode(0x05,0x01)
                    .then(res=>{
                        this.errorCode = res.dataParsed.replace(/\s+/g, "");
                        this.$message({
                            message: this.$t('readSuccess'),
                            type: 'success'
                        });
                    }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                });
            },
            readBootLoad() {
                this.bootLoad = '';
                Sensor.port.usbSDK.machineInformation.get.bootLoad(0x05,0x01)
                    .then(res=>{
                        this.bootLoad = res.dataParsed.replace(/\s+/g, "");
                        this.$message({
                            message: this.$t('readSuccess'),
                            type: 'success'
                        });
                    }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                });
            },
            clear() {
                this.sn = '';
                this.model = '';
                this.version = '';
                this.hardVersion = '';
                this.manName = '';
                this.torque_value = '';
                this.cadence = '';
            },

            reset() {
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                this.clear();
                Sensor.port.reset(100);
                console.log("reset HID");
                this.isConnected = false;
//                setTimeout(()=>{
//                    this.isConnected = Sensor.port.isConnected();
//                },400);
//                Sensor.getBus().subscribe(res => {
//                    if (res.type === 'close' || res.type === 'error') {
//                        this.isConnected = false;
//                        this.com = '';
//                        this.$message({
//                            message: 'Reset Success',
//                            type: "success"
//                        });
//                    }
//                });
            },

            //判断是否一个数据都没有
            checkData() {
                if (this.sn && this.model && this.version && this.hardVersion) {
                    return true;
                }else{
                    return false;
                }
            },

        },
        beforeDestroy:function () {
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalSen){
                clearInterval(this.connectionIntervalSen);
            }
            Sensor.port.setBoxOperation();
        },
        watch: {
            sn: function (newValue, oldValue) {
                let vm = this;
                if (newValue === oldValue || newValue === '') {
                    return;
                }
                if (typeof oldValue !== 'undefined') {
                    let time = new Date().getTime();
                    vm.isScanner = time - vm.lastInputTime <= 100; // 间隔小于100ms的就认为是扫描枪输入
                    vm.lastInputTime = time;
                } else {
                    vm.lastInputTime = new Date().getTime();
                }
                if (!vm.scannerTimeout) {
                    vm.scannerTimeout = setTimeout(function () {
                        if (vm.isScanner) {
                            vm.sn = vm.sn.replace(/\s+/g, "");
                            let array = vm.sn.split('SR');
                            if (array.length > 1) {
                                vm.sn = ('SR' + array[array.length - 1]).replace(/\s+/g, "");
                            }
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }
            },
            isConnected: function(newValue, oldValue) {
                let vm = this;
                if(newValue != oldValue && oldValue == true){
                    let count = 0;
                    Sensor.port.setConnection();
                    Sensor.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count>1 && Sensor.port.isConnected()){
                                vm.connect();
                            }
                            if(count>3 && Sensor.port.isConnected()){
                                console.log("Count stop",count);
                                clearInterval(vm.reconnectInterval);
                            }
                        },500);
                    }else{
                        if(vm.reconnectInterval){
                            clearInterval(vm.reconnectInterval);
                            vm.reconnectInterval = '';
                        }
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count === 1){
                                vm.checkConnection();
                            }else if(count > 1){
                                if(vm.usbHID){
                                    clearInterval(vm.reconnectInterval);
                                    vm.init();
                                }
                                if(count>3 && Sensor.port.isConnected()){
                                    console.log("Count stop",count);
                                    clearInterval(vm.reconnectInterval);
                                }
                            }
                        },500);
                    }
                }
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .sensor {
        width: 100%;
        height: 100%;
        overflow: auto;
        padding: 44px 0;
        .btn-back {
            margin-left: 40px;
            top: -26px;
            position: relative;
        }
        .sensor-content {
            width: 950px;
            margin: 0 auto;
            .connect-tip{
                display: inline-block;
                float:right;
                a {
                    color: #E26829;
                }
                .tip-img {
                    top: 0;
                    right:20px;
                }
            }
            .title {
                margin-left: 33px;
                font-size: 16px;
                /*display: flex;*/
                align-items: center;
                img {
                    width: 24px;
                    height: 24px;
                    margin-right: 16px;
                }
                .can{
                    display: inline-block;
                    width: 58px;
                    position: relative;
                    font-size: 14px;
                    line-height: 22px;
                    height: 22px;
                    padding-left: 15px;
                    margin-left:10px;
                    vertical-align: middle;
                    border-color:rgb(34, 37, 46);
                    background-color:rgb(34, 37, 46);
                    outline: none;
                    border-radius: 12px;
                    box-sizing: border-box;
                    cursor: pointer;
                    transition: border-color .3s, background-color .3s;
                }
            }
            .info {
                overflow: hidden;
                background: #353945;
                width: 950px;
                /*min-height: 120px;*/
                margin-top: 22px;
                -moz-box-shadow: 0 1px 0 #2B2F3B;
                /* 老的 Firefox */
                box-shadow: 0 1px 0 #2B2F3B;
                .btn-row {
                    height: 30px;
                    width: 100%;
                    position: relative;
                    line-height: 30px;
                }
                .info-left {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    border-right: 1px dotted #2B2F3B;
                    .item {
                        margin-left: 33px;
                        height: 50px;
                        line-height: 50px;
                        .label {
                            width: 110px;
                            display: inline-block;
                            line-height: 20px;
                            vertical-align: middle;
                            word-wrap:break-word;
                        }
                        .input {
                            width: 295px;
                            // margin-left: 10px;
                            & > input {
                                height: 29px;
                            }
                        }
                        .el-select {
                            width: 290px;
                        }
                        .input-sn:after {
                            top: 15px;
                        }
                    }
                    .btn-group {
                        position: absolute;
                        right: 40px;
                    }
                    .sn {
                        height: 60px;
                        margin-left: 33px;
                        line-height: 60px;
                        width: 410px;
                        .label {
                            width: 90px;
                            display: inline-block;
                        }
                        .sn-input {
                            width: 150px;
                            margin-left: 10px;
                        }
                        .btn-read {
                            width: 70px;
                            margin-left: 7px;
                        }
                        .btn-write {
                            margin-left: 1px;
                            height: 29px;
                            width: 70px;
                        }
                    }
                }
                .info-right {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    .item {
                        margin-left: 33px;
                        height: 50px;
                        line-height: 50px;
                        display: flex;
                        align-items: center;
                        .el-select {
                            width: 187px;
                        }
                        .tip {
                            display: inline-block;
                            border-radius: 50%;
                            width: 10px;
                            height: 10px;
                            background: #E26829;
                            margin: 0 6px;
                        }
                        .connect {
                            background: #76EE00;
                        }
                        .label {
                            width: 110px;
                            display: inline-block;
                            line-height: 20px;
                            vertical-align: middle;
                            word-wrap:break-word;
                        }
                        .input {
                            width: 295px;
                        }
                        > button {
                            margin-right: 12px;
                        }

                    }
                    .btn-group {
                        position: absolute;
                        right: 26px;
                    }
                    .btns {
                        display: inline-block;
                    }
                    .loading {
                        display: inline-block;
                        vertical-align: middle;
                    }
                }
                .note-title {
                    margin-top: 10px;
                }
            }
            .record{
                height:80px;
                line-height: 20px;
                .wrap{
                    .title-record{
                        font-size:16px;
                    }
                    padding:20px 33px;
                    .number{
                        color:#E26829;
                    }
                }
            }
            .tip-dialog {
                .el-dialog__body {
                    padding: 0;
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// sensor.vue?55dd58f3