<template>
    <div class="battery">
        <div class="battery-content">
            <label class="title">
                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                <div class="connect-tip">
                    <a href="javascript:;" @click="dialogTipVisible = true" class="btn-tip btn-underline">
                        <img src="../../assets/images/icon/icon-help.png" class="tip-img" />
                    </a>
                </div>
                <!--<el-switch-->
                <!--v-model="isCan"-->
                <!--:on-color="'#98B42C'"-->
                <!--:off-color="'#22252E'"-->
                <!--@change="blurUsbHID"-->
                <!--:on-text="'Can'"-->
                <!--:off-text="'Uart'"-->
                <!--class="switch">-->
                <!--</el-switch>-->
            </label>
            <div class="info">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.mainData') }}</label>
                </div>
                <div class="part">
                    <!--<div class="sn">-->
                    <!--<label class="label">SN</label>-->
                    <!--<el-input class="scan-input" v-model="sn" disabled></el-input> -->
                    <!--</div>-->
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">SN</label>
                        <el-input class="input input-sn" v-model="sn" ></el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label">SN</label>
                        <el-input class="input " v-model="sn" ></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="preview" class="btn-read" @click="readSn">{{ $t('cNam.read') }}</el-button>
                            <el-button type="primary" class="btn-write"
                                       @click="writeSn"
                                       v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.model') }}</label>
                        <el-input class="input" v-model="model" disabled></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                        <el-input class="input" v-model="version" disabled></el-input>
                    </div>
                </div>
                <div class="part right">
                    <div class="item" v-if = "!usbHID">
                        <label class="label">{{ $t('cNam.com') }}</label>
                        <el-select v-model="com" @change="changeCom" @click.native="getComList" class="com-select" :placeholder="$t('cNam.select')">
                            <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                            </el-option>
                        </el-select>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button class="btn" @click="connect" type="primary">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="item" v-else>
                        <label class="label">USB HID</label>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button @click="connect" type="primary" :disabled = "loading || isReset">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <!--btn-row-->
                    <div class="btn-row">
                        <div class="btn-group">
                            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>
                            <div class="btns">
                                <el-button class="btn" type="preview" @click="readBase" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.read') }}</el-button>
                                <el-button class="btn" type="primary" @click="reset" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.reset') }}</el-button>
                            </div>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                        <el-input class="input" v-model="hardVersion" disabled></el-input>
                    </div>
                </div>
            </div>
            <div class="info data">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.parameter') }}</label>
                </div>
                <div class="part">
                    <div class="item">
                        <label class="label">{{ $t('cNam.ASOC') }}</label>
                        <el-input class="input" v-model="ASOC" disabled>
                            <template slot="append">%</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.packVoltage') }}</label>
                        <el-input class="input" v-model="packVoltage" disabled>
                            <template slot="append">mV</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.fullChargeCapacity') }}</label>
                        <el-input class="input" v-model="fullChargeCapacity" disabled>
                            <template slot="append">mAh</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.current') }}</label>
                        <el-input class="input" v-model="current" disabled>
                            <template slot="append">mA</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.cell8_14') }}</label>
                        <el-input class="input" v-model="cell8_14" disabled>
                            <template slot="append">mV</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.currentChargeInterval') }}</label>
                        <el-input class="input" v-model="currentChargeInterval" disabled>
                            <template slot="append">day</template>
                        </el-input>
                    </div>
                </div>
                <div class="part right">
                    <div class="item">
                        <label class="label">{{ $t('cNam.RSOC') }}</label>
                        <el-input class="input" v-model="RSOC" disabled>
                            <template slot="append">%</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.capacityLeft') }}</label>
                        <el-input class="input" v-model="capacityLeft" disabled>
                            <template slot="append">mAh</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.temperature') }}</label>
                        <el-input class="input" v-model="temperature" disabled>
                            <template slot="append">℃</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.cell1_7') }}</label>
                        <el-input class="input" v-model="cell1_7" disabled>
                            <template slot="append">mV</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.cycleLife') }}</label>
                        <el-input class="input" v-model="cycleLife" disabled>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.maxChargeInterval') }}</label>
                        <el-input class="input" v-model="maxChargeInterval" disabled>
                            <template slot="append">day</template>
                        </el-input>
                    </div>
                </div>
            </div>
            <div class="info data" v-if = "isCan && org.type === Core.Const.ORG_TYPE.COMPONENT">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.setting') }}</label>
                </div>
                <div class="part" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                    <div class="item" >
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCode" placeholder="6004"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="writeParamWithAuth">{{ $t('cNam.write') }}</el-button>
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
                    <div class="item" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.manName') }}</label>
                        <el-input class="input" v-model="manufacturer" placeholder="6005"></el-input>
                    </div>
                    <div class="btn-row" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="preview" @click="readManufacturer">{{ $t('cNam.read') }}</el-button>
                            <el-button type="primary" @click="writeManufacturer">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                </div>
                <div class="part right">
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

            <div class="info after" v-show="bafangBT && (org.type === Core.Const.ORG_TYPE.COMPONENT || org.type === Core.Const.ORG_TYPE.BRAND)">
                <div class="message" >
                    <span>{{$t('cNam.afterSalesTitle')}}</span>
                    <span></span>
                </div>
                <div class="part">
                    <div class="item">
                        <label class="label">{{ $t('cNam.produceDate') }}</label>
                        <el-input class="input" v-model="produceDate" ></el-input>
                    </div>
                    <div class="item" v-if = "isCan">
                        <label class="label">{{ $t('cNam.SOH') }}</label>
                        <el-input class="input" v-model = "sohCan" disabled>
                            <template slot="append">%</template>
                        </el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label">{{ $t('cNam.SOH') }}</label>
                        <el-input class="input" v-model = "SOH" disabled>
                            <template slot="append">%</template>
                        </el-input>
                    </div>
                </div>
                <div class="part right">
                    <div class="item">
                        <label class="label">{{ $t('cNam.useStage') }}</label>
                        <el-input class="input" v-model = "afterSales.stage" disabled>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.result') }}</label>
                        <el-input class="input" v-model = "afterSalesResult" disabled>
                        </el-input>
                    </div>

                </div>
                <div class="message">
                    <span v-show="inWarranty">{{afterSalesNote}}</span>
                    <span v-show="!inWarranty" class="bg" >{{afterSalesNote}}</span>
                </div>
            </div>
        </div>

        <!--说明弹框-->
        <el-dialog :title="$t('cNam.operationTip')" custom-class="tip-dialog"
                   :visible.sync="dialogTipVisible">
            <div>
                <h1> {{ $t('cNam.txtTitle1') }} </h1>
                <div class="txt"><span class="itemName">COM</span>   <span>{{ $t('cNam.txtTitleCom') }}</span></div>
                <div class="txt"><span class="itemName">USB HID</span>   <span>{{ $t('cNam.txtTitleHid') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.connect') }}</span> <span>{{ $t('cNam.txtTitleCon') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.read') }}</span>    <span>{{ $t('cNam.txtTitleRead') }}</span></div>
                <div class="txt" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT"><span class="itemName" >{{ $t('cNam.write') }}</span>   <span>{{ $t('cNam.txtTitleWrt') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.reset') }}</span>   <span>{{ $t('cNam.txtTitleReset') }}</span></div>
                <div class="clear-20"></div>
                <h1> {{ $t('cNam.txtTitle2') }} </h1>
                <div class="txt">{{ $t('cNam.txtNote1') }}</div>

            </div>
        </el-dialog>

    </div>
</template>

<script>
    import Battery from "../../../device/battery"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import vueLoading from 'vue-loading-template'
    import Core from 'core/core'

    export default{
        name: 'battery',
        components: {
            vueLoading
        },
        data(){
            return {
                isConnected: Battery.port.isConnected(),
                haveBack: false,
                com: '',
                comList: [],
                sn: '',
                model: '',
                version: '',
                hardVersion: '',
                ASOC: '',
                RSOC: '',
                packVoltage: '',
                capacityLeft: '',
                fullChargeCapacity: '',
                temperature: '',
                current: '',
                cell1_7: '',
                cell8_14: '',
                cycleLife: '',
                loading: false,
                writeStatus: false,
                maxChargeInterval: '',
                currentChargeInterval: '',
                sohCan: '',
                Battery: Battery,

                org: Core.Data.getOrg(),
                Core: Core,

                dialogTipVisible: false,
                //Maintenance Content
                produceDate:'',
                warranty:[
                    { stage: 'V1', month: 'within one Months', soh: '95', cycle: 0, content: 'Under Maintenance' },
                    { stage: 'V2', month: '0~6 Months', soh: '90', cycle: 50, content: 'Under Maintenance' },
                    { stage: 'V3', month: '6~18 Months', soh: '80', cycle: 250, content: 'Under Maintenance' },
                    { stage: 'V4', month: '18~30 Months', soh: '60', cycle: 500, content: 'Under Maintenance' },
                    { stage: 'V5', month: '> 30 Months', soh: '', cycle: '', content: 'Out of Maintenance' }
                ],
                afterSales:{},
                afterSalesNote:'',
                inWarranty: '',
                bafangBT:false,

                isCan: false,
                usbHID: false,
                readInterval: null,
                isReset: false,

                clientCode: '',
                modelWrite: '',
                hardwareWrite: '',
                errorCode: '',
                bootLoad: '',
                manufacturer: '',

                reconnectInterval: null,
                connectionIntervalBat: null,
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Tool.Battery,
                zh: ZH.Components.Tool.Battery,
                de: DE.Components.Tool.Battery,
                nl: NL.Components.Tool.Battery
            }
        },
        mounted() {
            this.haveBack = this.$route.params.haveBack;
            Battery.port.timeLog('Battery');
            Battery.port.setBoxOperation();
            this.checkConnection();
//            this.checkUSBUartOrCan();
            this.warranty[0].content = this.$t('cNam.batUnderMt');
            this.warranty[1].content = this.$t('cNam.batUnderMt');
            this.warranty[2].content = this.$t('cNam.batUnderMt');
            this.warranty[3].content = this.$t('cNam.batUnderMt');
            this.warranty[4].content = this.$t('cNam.batOutMt');
        },
        methods: {
            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalBat){
                    clearInterval(vm.connectionIntervalBat);
                    vm.connectionIntervalBat = '';
                }
                vm.connectionIntervalBat = setInterval(()=>{
                    let connection = Battery.port.getConnection();
                    if(connection.usb){
//                        this.checkUSBUartOrCan();
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalBat);
                    }
                },300)
            },
//            checkUSBUartOrCan(){
//                let checkUSBUartOrCanIntervalBat = setInterval(()=>{
//                    let USBUartOrCan = Battery.port.getConnection();
//                    if(USBUartOrCan.usb){
//                        if(USBUartOrCan.can){
//                            this.isCan = true;
//                        }
//                        if(USBUartOrCan.uart){
//                            this.isCan = false;
//                        }
//                        if(USBUartOrCan.can || USBUartOrCan.uart){
//                            clearInterval(checkUSBUartOrCanIntervalBat);
//                        }
//                    }
//                },800)
//            },
            init(){
                Battery.port.startConnection();
                if(Battery.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
                    console.log(" get USB Device Battery");
                    if(this.isReset){
                        this.connect(1);
                    }else{
                        this.connect();
                    }
                }else{
                    console.log("COM Battery");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = Battery.port.isConnected();
            },

            getComList(){
//                if (Battery.port.getPort() != '' && Battery.port.isConnected()) {
//                    this.com = Battery.port.getPort();
//                    this.comList = [Battery.port.getPort()];
//                    this.isConnected = Battery.port.isConnected();
//                    return
//                }
                Battery.port.getPortList().subscribe(res => {
                    if (res.type === 'port') {
                        if (res.data) {
                            this.comList = res.data;
                            this.com = this.comList[0];
                            this.connect();
                        } else {
                            this.$message({
                                message: this.$t('cNam.noComConnect'),
                                type: 'warning'
                            });
                        }
                    }
                });
            },

//            blurUsbHID(){
//                if(this.isCan){
//                    Battery.port.setCan(true);
//                }else{
//                    Battery.port.setCan(false);
//                }
//                this.isConnected = Battery.port.isConnected();
//                this.init();
//            },

            changeCom() {
//                this.isConnected = false;
            },

            connect(type) {
                if( this.usbHID ){
                    if(type === 1){
                        setTimeout(()=>{
                            this.test();
                        },1000)
                    }else{
                        this.test();
                    }
                    this.isConnected = Battery.port.isConnected();
                    this.isCan = !Battery.port.checkUsbUart();
                    if(this.isCan){
                        this.readInterval = 1200;
                    }else{
                        this.readInterval = 7500;
                    }
                    console.log("usbHID connect");
                    Battery.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb') {
                            this.isConnected = false;
                            this.clear();
                        }
                    });
                }else{
                    this.readInterval = 10000;
                    if(!this.com) {
                        this.$message({
                            message: this.$t('cNam.selectComTip'),
                            type: 'warning'
                        });
                        return;
                    } else {
                        Battery.connect(this.com);
                        Battery.getBus().subscribe(res => {
                            if (res.type === 'open' || res.type === 'connect') {
                                this.isConnected = true;
                            }
                            if (res.type === 'close' || res.type === 'error') {
                                this.com = '';
                                this.isConnected = false;
                                this.loading = false;
//                                this.$message({
//                                    message: this.$t('cNam.comDisconnect'),
//                                    type: 'warning'
//                                });
                            }
                        });
                    }
                }
            },

            test(){
                Battery.port.checkUsbConnection('0101',1);
            },

            readBase() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.bafangBT = false;
                this.loading = true;
                setTimeout(() => {
                    if(this.loading){
                        if(!this.checkData()){
                            this.$message({
                                message:  this.$t('batteryTip'),
                                type: "warning"
                            });
                        }
                    }
                    this.loading  = false;
                }, this.readInterval);
                this.clear();
                this.connect(1);
                if(!this.isCan || Battery.port.checkUsbUart()) {
                    Battery.sendCmdGetFullData();
                    Battery.getBus().subscribe(res => {
                        //                    console.log('battery-vm2:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.battery) {
                            return;
                        }
                        if (res.data.battery.hasOwnProperty("sn")) {
                            this.sn = res.data.battery.sn;
                            console.log('sn', this.sn.length);
                            if(this.sn!='' && this.sn.indexOf("BT") >-1 ){
                                this.bafangBT = true;
                                this.produceDate = Core.Util.produceDateParse(this.sn);
                            }
                        }
                        if (res.data.battery.hasOwnProperty("model")) {
                            this.model = res.data.battery.model;
                        }
                        if (res.data.battery.hasOwnProperty("software_version")) {
                            this.version = res.data.battery.software_version;
                        }
                        if (res.data.battery.hasOwnProperty("hardware_version")) {
                            this.hardVersion = res.data.battery.hardware_version;
                        }
                        if (res.data.battery.hasOwnProperty("relative_capacity")) {
                            this.RSOC = res.data.battery.relative_capacity;
                        }
                        if (res.data.battery.hasOwnProperty("absolute_capacity")) {
                            this.ASOC = res.data.battery.absolute_capacity;
                        }
                        if (res.data.battery.hasOwnProperty("inner_temperature")) {
                            this.temperature = res.data.battery.inner_temperature;
                        }
                        if (res.data.battery.hasOwnProperty("real_current")) {
                            this.current = res.data.battery.real_current;
                        }
                        if (res.data.battery.hasOwnProperty("loop_time")) {
                            this.cycleLife = res.data.battery.loop_time;
                        }
                        if (res.data.battery.hasOwnProperty("1to7")) {
                            let cell = res.data.battery["1to7"];
                            this.cell1_7 = cell.join();
                        }
                        if (res.data.battery.hasOwnProperty("8to14")) {
                            let cell = res.data.battery["8to14"];
                            this.cell8_14 = cell.join();
                        }
                        if (res.data.battery.hasOwnProperty("left_capacity")) {
                            this.capacityLeft = res.data.battery.left_capacity;
                        }
                        if (res.data.battery.hasOwnProperty("full_capacity")) {
                            this.fullChargeCapacity = res.data.battery.full_capacity;
                        }
                        if (res.data.battery.hasOwnProperty("total_volt")) {
                            this.packVoltage = res.data.battery.total_volt;
                        }
                        if (res.data.battery.hasOwnProperty("max_charge_interval")) {
                            this.maxChargeInterval = res.data.battery.max_charge_interval;
                        }
                        if (res.data.battery.hasOwnProperty("cur_charge_interval")) {
                            this.currentChargeInterval = res.data.battery.cur_charge_interval;
                        }
                    });
                }else{

                    Battery.port.getUsbBasic(0x04).subscribe(res => {
                        if (!res.data) {
                            return;
                        }
                        if (res.data.hasOwnProperty("sn")) {
                            this.sn = res.data.sn;
                            if(this.sn!='' && this.sn.indexOf("BT") >-1 ){
                                this.bafangBT = true;
                                this.produceDate = Core.Util.produceDateParse(this.sn);
                            }
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
                            this.clientCode = res.data.client_code;
                        }

                    });

                    Battery.port.getBatteryOfData().subscribe(res => {
                        if (!res.data) {
                            return;
                        }
                        if (res.data.hasOwnProperty("cycleLife")) {
                            this.cycleLife = res.data.cycleLife;
                        }
                        if (res.data.hasOwnProperty("maxChargeInterval")) {
                            this.maxChargeInterval = res.data.maxChargeInterval;
                        }
                        if (res.data.hasOwnProperty("currentChargeInterval")) {
                            this.currentChargeInterval = res.data.currentChargeInterval;
                        }
                        if (res.data.hasOwnProperty("cell1_7")) {
                            this.cell1_7 = res.data.cell1_7;
                        }
                        if (res.data.hasOwnProperty("cell8_14")) {
                            this.cell8_14 = res.data.cell8_14;
                        }
                    });
                    Battery.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if( res.type == 'error_usb') {
                            this.isConnected = false;
                            this.clear();
                        }
                        if (res.data.code === '34' && res.data.subCode === '00') {
                            let result = Buffer.from(res.data.data, 'hex');
                            this.fullChargeCapacity = result[0] + result[1] * 256;
                            this.capacityLeft = result[2] + result[3] * 256;
                            this.RSOC = result[4];
                            this.ASOC = result[5];
                            this.sohCan = result[6];
                        }
                        if (res.data.code === '34' && res.data.subCode === '01') {
                            let tem = res.data.data.substring(2, 4) + res.data.data.substring(0, 2);
                            //  tem & 0x8000, 如果还是8000,就是负的
                            let currentBinaryLen = parseInt(tem, 16).toString(2).length;
                            // tem 取反加1，或者 0xFFFF-tem+1
                            this.current = currentBinaryLen < 16 ? parseInt(tem, 16) * 10 : -(~parseInt(tem, 16) + 1 + Math.pow(2, 16)) * 10;
                            let result_2 = Buffer.from(res.data.data, 'hex');
                            this.packVoltage = (result_2[2] + result_2[3] * 256) * 10;
                            this.temperature = result_2[4] - 40;
                        }
                    });
                }
            },

            readSn(){
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.connect(1);
                if(!this.isCan || Battery.port.checkUsbUart()) {
                    Battery.sendCmdGetBaseData();
                    Battery.getBus().subscribe(res => {
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.battery) {
                            return;
                        }
                        if (res.data.battery.hasOwnProperty("sn")) {
                            this.sn = res.data.battery.sn;
                            if(this.sn!=''){
                                this.produceDate = Core.Util.produceDateParse(this.sn);
                            }
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
                    });
                }else{
                    Battery.port.getUsbBasic(0x04).subscribe(res => {
                        if (!res.data) {
                            return;
                        }
                        if (res.data.hasOwnProperty("sn")) {
                            this.sn = res.data.sn;
                            if(this.sn!='' && this.sn.indexOf("BT") >-1 ){
                                this.bafangBT = true;
                                this.produceDate = Core.Util.produceDateParse(this.sn);
                            }
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

                    });
                }

            },
            usbErrorCheck(err){
                Battery.port.checkUsbError(err);
            },
            writeSn() {
                if (!this.sn) {
                    return;
                }
                this.sn = this.sn.replace(/\s/g,"").trim();
                if(this.sn.length>40){
                    this.$message({
                            message: this.$t('cNam.snMaxLength'),
                            type: "warning"
                        }
                    );
                }
                if(!this.usbHID){
                    Battery.connect(this.com);
                }
                this.writeStatus = true;
                if(!this.isCan || Battery.port.checkUsbUart()) {
                    Battery.writeSn(this.sn);
                    Battery.getBus().subscribe(res => {
//                    console.log('battery-vm4:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!this.writeStatus) {
                            return;
                        }
                        if (res.data.battery && 'sn' in res.data.battery) {
                            let sn = res.data.battery.sn;
                            if (sn == this.sn || sn == ' ') {
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                this.sn = '';
                            } else {
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }
                            this.writeStatus = false;
                        }
                    });
                } else {
                    Battery.port.usbSDK.machineInformation.update.serialNumber(0x05, 0x04, Battery.port.Core.Util.stringToHex(this.sn))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.sn = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Battery.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x04)
                                    .then(res =>{
                                        this.sn = res.dataParsed;
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
                }
            },
            writeParamWithAuth() {
                this.clientCode = this.clientCode.replace(/\s/g, "");
                if ( this.clientCode && this.clientCode.length < 25 ) {
                    Battery.port.usbSDK.machineInformation.update.customerNumber(0x05, 0x04, Battery.port.Core.Util.stringToHex(this.clientCode))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.clientCode = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Battery.port.usbSDK.machineInformation.get.customerNumber(0x05, 0x04)
                                    .then(res =>{
                                        this.clientCode = res.dataParsed;
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
                }else{
                    this.$message(this.$t('cNam.customerMaxLength'));
                }
            },
            clickCopy(){
                this.modelWrite = this.model;
                this.hardwareWrite = this.hardVersion;
            },
            clickWriteReset(){
                if(this.modelWrite && this.modelWrite.indexOf('BT')>-1){
                    Battery.port.usbSDK.machineInformation.update.modelNumber(0x05, 0x04, Battery.port.Core.Util.stringToHex(this.modelWrite.trim()))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.modelWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Battery.port.usbSDK.machineInformation.get.modelNumber(0x05, 0x04)
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
                if(this.hardwareWrite){
                    Battery.port.usbSDK.machineInformation.update.hardwareVersionNumber(0x05, 0x04, Battery.port.Core.Util.stringToHex(this.hardwareWrite.trim()))
                        .then(res => {
                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.hardwareWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Battery.port.usbSDK.machineInformation.get.hardwareVersionNumber(0x05, 0x04)
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
                Battery.port.usbSDK.machineInformation.get.manufacturer(0x05,0x04)
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
                Battery.port.usbSDK.machineInformation.update.manufacturer(0x05,0x04,Battery.port.Core.Util.stringToHex(this.manufacturer.trim()))
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
                Battery.port.usbSDK.machineInformation.get.errorCode(0x05,0x04)
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
                Battery.port.usbSDK.machineInformation.get.bootLoad(0x05,0x04)
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
            checkData(){
                if (this.sn && this.model && this.version && this.hardVersion) {
                    return true;
                }else{
                    return false;
                }
            },
            clear() {
                this.sn = '';
                this.model = '';
                this.version = '';
                this.hardVersion = '';
                this.ASOC = '';
                this.packVoltage = '';
                this.fullChargeCapacity = '';
                this.current = '';
                this.cell8_14 = '';
                this.RSOC = '';
                this.capacityLeft = '';
                this.temperature = '';
                this.cell1_7 = '';
                this.cycleLife = '';
                this.maxChargeInterval = '';
                this.currentChargeInterval = '';

                this.produceDate = '';
                this.afterSales = {};
                this.afterSalesNote = '';
                this.inWarranty = '';
                this.bafangBT = false;
            },
            reset() {
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                this.clear();
                Battery.port.reset(100);
                this.isConnected = false;
//                setTimeout(()=>{
//                    this.isConnected = Battery.port.isConnected();
//                },500);
                if(!this.usbHID){
                    Battery.getBus().subscribe(res => {
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
//                            this.$message({
//                                message: 'Reset Success',
//                                type: "success"
//                            });
                        }
                    });
                }
            }
        },
        computed: {
            // 计算属性的
            SOH: function(){
                // `this` 指向 vm 实例
                if(this.RSOC && this.ASOC && (this.RSOC >= this.ASOC)){
                    return Core.Util.sprintf("%.1f", (this.ASOC/this.RSOC)*100)
                }else{
                    return ' '
                }
            },
            afterSalesResult:function(){
                let vm = this;
                let result = '';
                if( parseInt(vm.SOH)> 0 && vm.produceDate.indexOf("-")>0){
                    if( vm.afterSales.stage =="V5"){
                        vm.afterSalesNote = vm.$t('cNam.batUseTime');
                        result = vm.$t('cNam.batOutMt');
                        vm.inWarranty = false;
                    }else{
                        if( parseInt(vm.SOH) >= parseInt(vm.afterSales.soh)  && vm.cycleLife <= vm.afterSales.cycle){
                            result = vm.afterSales.content;
                            vm.afterSalesNote = 'On Stage '+vm.afterSales.stage+': If Cycle Count is less than ' +vm.afterSales.cycle+' and the SOH  bigger than ' + vm.afterSales.soh+'%，the battery is '+result+'.';
                            vm.inWarranty = true;
                        }else{
                            result = vm.$t('cNam.batOutMt');
                            vm.afterSalesNote = 'On Stage '+vm.afterSales.stage+': If Cycle Count is less than ' +vm.afterSales.cycle+' and the SOH  bigger than ' + vm.afterSales.soh+'%，the battery is '+result+'.';
                            vm.inWarranty = false;
                            console.log(' inWarranty 2');
                        }
                    }
                }
                return result
            },
        },
        beforeDestroy:function () {
            Battery.setModeOut();
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalBat){
                clearInterval(this.connectionIntervalBat);
            }
            Battery.port.setBoxOperation();
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
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }
            },
            produceDate: function(newVal,oldVal){
                if(newVal =='' || oldVal.indexOf("-")>0){
                    return;
                }
                console.log("date?");
                let vm = this;
                let timePro = Core.Util.parseDate(newVal) || '';
                let timeNow = Core.Util.time();
//                console.log("time1: "+timePro);
//                console.log("time1: "+timeNow);

                let day = Math.floor( ((timeNow - timePro )/3600)/24);
//                console.log("day: "+day);
//                console.log("day",typeof(day));
                if(day < 30){
                    vm.afterSales = vm.warranty[0];
                }else if(day >=30 && day < 180){
                    vm.afterSales = vm.warranty[1];
                }else if(day >=180 && day < 540){
                    vm.afterSales = vm.warranty[2];
                }else if(day >=540 && day < 900){
                    vm.afterSales = vm.warranty[3];
                }else{
                    vm.afterSales = vm.warranty[4];
                }
            },
            isConnected: function(newValue, oldValue) {
                let vm = this;
                if(newValue != oldValue && oldValue == true){
                    let count = 0;
                    Battery.port.setConnection();
                    Battery.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count>1 && Battery.port.isConnected()){
                                vm.connect();
                            }
                            if(count>3 && Battery.port.isConnected()){
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
                                if(!vm.usbHID && vm.com){
                                    clearInterval(vm.reconnectInterval);
                                    vm.init();
                                }
                                if(vm.usbHID){
                                    clearInterval(vm.reconnectInterval);
                                    vm.init();
                                }
                                if(count>3 && Battery.port.isConnected()){
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
    .battery {
        padding: 44px 0;
        width: 100%;
        height: 100%;
        overflow: auto;
        .btn-back {
            margin-left: 40px;
            top: -26px;
            position: relative;
        }
        .battery-content {
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
            }
            .info {
                overflow: hidden;
                background: #353945;
                width: 950px;
                height: 245px;
                margin-top: 22px;
                -moz-box-shadow: 0 1px 0 #2B2F3B; /* 老的 Firefox */
                box-shadow: 0 1px 0 #2B2F3B;
                .part {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    border-right: 1px dotted #2B2F3B;
                    .item {
                        position: relative;
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
                        }
                        .input-sn:after {
                            top: 15px;
                        }
                        &:last-child{
                            margin-bottom: 10px;
                        }
                    }
                    .btn-row {
                        height: 30px;
                        width: 100%;
                        position: relative;
                        line-height: 30px;
                        .btn-group {
                            position: absolute;
                            right: 30px;
                        }
                    }
                    .long {
                        .long-label {
                            position: relative;
                            top: 0;
                            width: 110px;
                            display:inline-block;
                        }
                        .input {
                            top: -10px
                        }
                    }
                    .sn {
                        height: 50px;
                        margin-left: 33px;
                        line-height: 50px;
                        width: 440px;
                        .label {
                            width: 90px;
                            display: inline-block;
                        }
                        .sn-input {
                            width: 295px;
                        }
                        .btn-read {
                            width: 70px;
                            margin-left: 7px;
                        }
                        .btn-write {
                            margin-left: 1px;
                            width: 70px;

                        }

                    }
                }
                .right {
                    border-right: none;
                    font-size: 0;
                    .com-select {
                        width: 186px;
                    }
                    .tip {
                        display: inline-block;
                        border-radius: 50%;
                        width: 10px;
                        height: 10px;
                        background: #E26829;
                        margin: 0 10px 0 6px;
                    }
                    .connect {
                        background: #76EE00;
                    }
                    .btn-row {
                        height: 30px;
                        width: 100%;
                        position: relative;
                        line-height: 30px;
                        .btn-group {
                            position: absolute;
                            right: 30px;
                            .btns {
                                display: inline-block;
                            }
                            .loading {
                                display: inline-block;
                                vertical-align: middle;
                            }
                        }
                    }

                }
                .note-title {
                    margin-top: 10px;
                }
            }
            .data {
                height: auto;
            }
            .after{
                height: auto;
                margin-top: 1px;
                .message{
                    float: left;
                    width: 885px;
                    /*position: relative;*/
                    margin: 2px 0 0 33px ;
                    height: 40px;
                    line-height:40px;
                }
                .part{
                    margin-top:10px;
                }
                .bg{
                    background-color:rgba(255,0,0,0.6);
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// battery.vue?06f6e517