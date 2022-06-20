<template>
    <div class="update">
        <div class="update-content">
            <!--<button @click="getFirmwareBySn('CRS10F.350.SN.U1.1K04F8S4190414')">test</button>-->
            <label class="title">
                <img class="icon" src="../../assets/images/icon/icon-update.png" />{{ $t('update') }}
                <div class="connect-tip">
                    <a href="javascript:;" @click="dialogTipVisible = true" class="btn-tip btn-underline">
                        <img src="../../assets/images/icon/icon-help.png" class="tip-img" v-if="!showHelp"/>
                        <img src="../../assets/images/icon/icon-help.gif" class="tip-img" v-else/>
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
            <div class="content">
                <div class="left">
                    <div class="item">
                        <label class="label">{{ $t('cNam.component') }}</label>
                        <el-select v-model="componentType" @change="changeComp" class="input" :placeholder="$t('cNam.select')">
                            <el-option
                                v-for="item in optionsCom"
                                :key="item.type"
                                :label="item.label"
                                :value="item.type">
                            </el-option>
                        </el-select>
                    </div>
                    <div class="item" v-if="disableInput">
                        <label class="label" >SN</label>
                        <el-input class="input" v-model="sn" disabled></el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label" >SN</label>
                        <el-input class="input input-sn" v-model="sn"></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                        <el-input class="input" v-model="version" disabled></el-input>
                    </div>
                    <div class="item btn">
                        <div class="pull-right operation">
                            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>

                            <el-button type="danger" @click="clear">{{ $t('cNam.clear') }}</el-button>
                            <el-button type="preview" @click="readBase">{{ $t('cNam.read') }}</el-button>
                        </div>
                    </div>
                    <div class="space">
                    </div>
                    <div class="item" v-if="!offLineFlag && org.type != Core.Const.ORG_TYPE.SPECIAL_DEAlER">
                        <el-button class="cloud" :style="a" @click="getFirmwareBySn()" type="primary">{{ $t('fileCloudToUpdate') }}</el-button>
                        <!--<label class="label">{{ $t('itemName') }}</label>-->
                        <!--<el-input-->
                        <!--:placeholder="$t('select')"-->
                        <!--class="input"-->
                        <!--:disabled="true"-->
                        <!--v-model="selectObject.name"-->
                        <!--@click.native="dialogSelectCategoryVisible = true">-->
                        <!--</el-input>  -->
                    </div>

                    <div class="file">
                        <label class="label" :style="a" for="uploadFile" ref ="fileToU">{{ $t('fileToUpdate') }} </label> <br>
                        <span class="text-span">{{fileName}}</span>
                        <input class="file-input" id="uploadFile" ref="fileUpdate" type="file"
                               @change = 'onChangeUpdateFile()'/>
                    </div>
                    <div class="file">
                        <el-progress v-show="percentage > 0" :percentage="percentage"></el-progress>
                        <div class="">
                            <span v-show="startTip" class="text-span"> {{ $t('cNam.updateStarting') }}</span>
                        </div>
                    </div>
                    <div class="btns pull-right">
                        <el-button @click="startUpdate" type="preview">{{ $t('update') }}</el-button>
                    </div>
                </div>
                <div class="right">
                    <div class="item" v-if = "!usbHID">
                        <label class="label">{{ $t('cNam.com') }}</label>
                        <el-select v-model="com" @change='changeCom' @click.native="getComList" :placeholder="$t('cNam.select')">
                            <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                            </el-option>
                        </el-select>
                        <label class="tip" :class="{connect: isConnected}"></label>
                    </div>
                    <div class="item" v-else>
                        <label class="label">USB HID</label>
                        <label class="tip" :class="{connect: isConnected}"></label>
                    </div>

                    <!--
                    <div class="item">
                        <label class="label">{{ $t('cNam.component') }}</label>
                        <el-select v-model="BaudRateType" @change='changeBaudRate'  :placeholder="$t('cNam.select')">
                            <el-option
                                v-for="item in optionsBaudRate"
                                :key="item.type"
                                :label="item.label"
                                :value="item.label">
                            </el-option>
                        </el-select>
                    </div>
                    -->
                    <div class="clear-float">
                        <div class="btns pull-right">
                            <el-button class="btn" @click="connect" type="primary" :disabled = "loading || isReset">{{ $t('cNam.connect') }}</el-button>
                            <el-button class="btn" type="primary" @click="reset" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.reset') }}</el-button>
                        </div>
                    </div>
                    <div class="tip item" v-if="!usbHID">
                        <label>{{ $t('tip') }}:</label>
                        <label>{{ $t('updateTip') }}</label>
                    </div>
                    <div v-else>
                        <div class="clear-10"></div>
                        <div class="note tip">
                            <div class="msg-title">{{ $t('cNam.updateMsg') }}</div>
                        </div>
                        <div class="note">
                            <div class="msg">{{ updateMsg }}</div>
                        </div>
                        <div class="clear-30"></div>
                        <div class="file">
                            <el-progress v-show="boxPercentage > 0" :percentage="boxPercentage"></el-progress>
                        </div>
                    </div>
                </div>
            </div>
            <div class="content">
                <div class="step">{{ $t('cNam.updateStep0') }} </div>
                <div class="step">
                    <div><span class="no">1:</span><span class="stepText"> {{ $t('cNam.updateStep1') }} </span></div>
                    <div><span class="no">2:</span><span class="stepText"> {{ $t('cNam.updateStep2') }} </span></div>
                    <div><span class="no">3:</span><span class="stepText"> {{ $t('cNam.updateStep3') }} </span></div>
                    <div><span class="no">4:</span><span class="stepText"> {{ $t('cNam.updateStep4') }} </span></div>
                    <div><span class="no">5:</span><span class="stepText"> {{ $t('cNam.updateStep5') }} </span></div>
                    <div><span class="no">6:</span><span class="stepText"> {{ $t('cNam.updateStep6') }} </span></div>
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
                <div class="txt"><span class="itemName">{{ $t('cNam.reset') }}</span>   <span>{{ $t('cNam.txtTitleReset') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('update') }}</span>   <span>{{ $t('cNam.txtTitleUpt') }}</span></div>
                <div class="clear-20"></div>
                <h1> {{ $t('cNam.txtTitle2') }} </h1>
                <div class="txt">{{ $t('cNam.txtNote1') }}</div>
                <div class="txt">{{ $t('cNam.txtNote3') }}</div>
                <div class="clear-20"></div>
                <template>
                    <updateFirmware></updateFirmware>
                </template>
            </div>
        </el-dialog>

        <!--固件的历史记录弹窗-->
        <el-dialog :title="queryItemCondition" :visible.sync="dialogRecordListVisible">
            <table border="0" cellspacing="0" cellpadding="0" class="history-table">
                <thead>
                <tr>
                    <td class="history-td head">
                        {{ $t('version') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('updateTime') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('remark') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('operation') }}
                    </td>
                </tr>
                </thead>
                <tr v-for="item in HWRecordList">
                    <td class="history-td">{{item.version}}</td>
                    <td class="history-td">{{item.create_time | dateFormat}}</td>
                    <td class="history-td">{{item.remark}}</td>
                    <td class="history-td">
                        <el-button type="text" @click="updateObject(item)">
                            {{ $t('update') }}
                        </el-button>
                    </td>
                </tr>
            </table>
        </el-dialog>

    </div>
</template>

<script>
    import HMI from '../../../device/hmi'
    import Controller from '../../../device/controller'
    import Tool from '../../../device/tool'
    import Battery from '../../../device/battery'
    import Sensor from '../../../device/sensor'
    import Update from '../../../device/update'
    import vueLoading from 'vue-loading-template'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import Core from 'core/core'
    import updateFirmware from "src/components/instruction/updateFirmware";

    export default {
        name: 'toolUpdate',
        components: {
            vueLoading,
            updateFirmware
        },
        data() {
            return {
                isConnected: Update.port.isConnected(),
                com: '',
                comList: [],

                percentage: 0,
                boxPercentage: 0,
                queryItemCondition: '',
                itemList: [],
                itemTableHead: ['Picture', 'Name', 'Part No.', 'Operation'],
                dialogListVisible: false,
                dialogRecordListVisible: false,
                HWRecordList: [],
                isDealerOrAssembler: false,

                dialogSelectCategoryVisible: false,
                categoryList: [],
                defaultProps: {
                    id: 'id',
                    children: 'child',
                    label: 'name',
                },
                selectObject: {},
                selectCategory: {
                    id: '',
                    label: ''
                },
                dialogObjectListVisible: false,

                currentPage: 1,
                total: 0,
                offLineFlag: Core.Data.getOffline(),
                fileName: '',
                file: null,
                fileBufferLength: 0,
                fileBufferCrc: "",

                org: Core.Data.getOrg(),
                Core: Core,
                userName: Core.Data.get("userName") || "",

                dialogTipVisible: false,

                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.UPDATE,

                sn: '',
                version: '',
                hardware_version: '',
                deviceList: {'HMI': HMI,'controller': Controller, 'tool': Tool, 'bat': Battery, 'sensor': Sensor},
                device: '',

                optionsCom: [
                    {
                        type: 'hmi',
                        label: 'HMI'
                    },
                    {
                        type: 'ctrl',
                        label: 'Controller'
                    },
                    {
                        type: 'bat',
                        label: 'Battery'
                    },
                    {
                        type: 'sensor',
                        label: 'Sensor'
                    },
                    {
                        type: 'tool',
                        label: 'Service Tool'
                    }

                ],
                componentType: '',
                disableInput: true,

                optionsBaudRate: [{
                    label: '57600'
                }, {
                    label: '56000'
                },  {
                    label: '43000'
                },  {
                    label: '38400'
                }, {
                    label: '19200'
                },{
                    label: '9600'
                }],
                BaudRateType:'57600',
                lastBaudType:'',
                offlineRecordList:[],

                lang:'',
                startTip:false,

                beforeUpdate:false,
                a:{
                    width:'280px',
                },
                isCan: false,
                isCanSoft: false,
                usbHID: false,
                isReset: false,
                target: 0x03,
                loading: false,

                updateMsg: '',
                updateMsgObject: {},
                updateOnline: false,

                subscription: null,
                showHelp: false,
                reconnectInterval: null,
                connectionInterval: null,

                isTemAccount: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Tool.Update,
                zh: ZH.Components.Tool.Update,
                de: DE.Components.Tool.Update,
                nl: NL.Components.Tool.Update
            }
        },
        mounted() {
            this.checkRouterFrom();
            this.isDealerOrAssembler = Core.Data.getOrg().type === Core.Const.ORG_TYPE.DEALER || Core.Data.getOrg().type === Core.Const.ORG_TYPE.OEM;

            this.offLineFlag = Core.Data.getOffline();
            this.userId = Core.Data.getUser().id;

            if (this.isDealerOrAssembler) {
                this.disableInput = true;
            }else{
                this.disableInput = false;
            }

            this.lastBaudType = this.BaudRateType;
            this.lang = Core.Data.getLang();

            if( this.lang =='zh' || this.lang =='en'){
                this.a =  {width:'220px'};
            }
//            Update.port.setConnection('update',true);
//            this.init();
            this.updateMsgObject = {
                msg0: this.$t('cNam.updateMsg0'),
                msg1: this.$t('cNam.updateMsg1'),
                msg2: this.$t('cNam.updateMsg2'),
                msg3: this.$t('cNam.updateMsg3'),
                msg4: this.$t('cNam.updateMsg4'),
                msg5: this.$t('cNam.updateMsg5'),
                msg6: this.$t('cNam.updateMsg6'),
                msg7: this.$t('cNam.updateMsg7'),
                msg8: this.$t('cNam.updateMsg8'),
                msg10: this.$t('cNam.updateMsg10'),
                msg11: this.$t('cNam.updateMsg11'),
                msg20: this.$t('cNam.updateMsg20'),
                msg30: this.$t('cNam.updateMsg30'),
                successTip: this.$t('cNam.updateSuccessTip'),
                failTip: this.$t('cNam.updateFailTip'),
                readFailTip: this.$t('cNam.updateReadFail'),
                toolReconnectTip: this.$t('cNam.updateToolReconnectTip')
            };
            this.updateMsg = this.updateMsgObject.msg0;
            Update.port.setBoxOperation();
            this.checkConnection();

            // this.isTemAccount = Core.Data.getSpecialAccount('tem');
            this.isTemAccount = Core.Data.get('accountType').length>0 && Core.Data.get('accountType').indexOf(5)>-1;

        },
        computed:{
//           width(){
//               if( this.lang =='zh' || this.lang =='en'){
//                   return {width:'220px'}
//               }else{
//                   return {width:'280px'}
//               }
//           }
        },
        watch:{
            lang:function (newLang, oldLang) {
                if( newLang =='zh' || newLang =='en'){
                    this.a = {width:'180px'};
                }else{
                    this.a = {width:'280px'};
                }
            },
            isConnected: function(newValue, oldValue) {
                let vm = this;
                if(newValue != oldValue && oldValue == true){
                    let count = 0;
                    Update.port.setConnection();
                    Update.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            if(count>1 && Update.port.isConnected()){
                                console.log("Count",count);
                                vm.connect();
                            }
                            if(count>3 && Update.port.isConnected()){
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
                                if(count>3 && Update.port.isConnected()){
                                    console.log("Count stop",count);
                                    clearInterval(vm.reconnectInterval);
                                }
                            }
                        },500);
                    }
                }
            }
        },
        methods: {
            checkRouterFrom(){
                let query = this.$route.query;
                if(query.isUpdate){
                    if(query.from === "Ctrl"){
                        this.sn = query.sn;
                        this.version = query.soft;
                        this.componentType = 'ctrl';
                    }else if(query.from === "Hmi"){
                        this.sn = query.sn;
                        this.version = query.soft;
                        this.componentType = 'hmi';
                    }
                    this.getFirmwareBySn();
                }
            },
            checkConnection(){
                let vm = this;
                if(vm.connectionInterval){
                    clearInterval(vm.connectionInterval);
                    vm.connectionInterval = '';
                }
                vm.connectionInterval = setInterval(()=>{
//                    console.log('checkConnection',new Date().getTime());
                    let connection = Update.port.getConnection();
                    if(connection.usb){
//                        this.checkUSBUartOrCan();
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionInterval);
                    }
                },300)
            },
//            checkUSBUartOrCan(){
//                let checkUSBUartOrCanIntervalUpt = setInterval(()=>{
//                    let USBUartOrCan = Update.port.getConnection();
//                    if(USBUartOrCan.usb){
//                        if(USBUartOrCan.can){
//                            this.isCan = true;
////                            this.init();
//                        }
//                        if(USBUartOrCan.can || USBUartOrCan.uart){
//                            clearInterval(checkUSBUartOrCanIntervalUpt);
//                        }
//                    }
//                },800)
//            },
            init(){
                Update.port.startConnection();
                if(Update.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
//                    Update.port.setUSBDevice(true);
                    console.log(" getUSBDevice  update");
                    if(this.isReset){
                        this.connect(1);
                    }else{
                        this.connect();
                    }
                }else{
                    console.log("COM Update");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = Update.port.isConnected();
            },
            getComList() {
                // if (Update.port.getPort() != '' && Update.port.isConnected()) {
                //     this.com = Update.port.getPort();
                //     this.comList = [Update.port.getPort()];
                //     this.isConnected = Update.port.isConnected();
                // }
                Update.port.getPortList().subscribe(res => {
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
//                    Update.port.setCan(true);
//                }else{
//                    Update.port.setCan(false);
//                }
//                this.isConnected = Update.port.isConnected();
//                this.init();
//            },
            changeCom() {
//                this.isConnected = false;
            },

            changeComp(){
                if(this.isDealerOrAssembler){
                    if(this.componentType === 'tool'){
                        console.log(this.componentType);
                        this.disableInput = false;
                    }else{
                        this.disableInput = true;
                    }
                }
            },
            // 改变波特率
            changeBaudRate(){
                console.log("current Bound"+this.BaudRateType);
                this.writeBaud( this.BaudRateType );
                this.lastBaudType = this.BaudRateType;
            },
            writeBaud(data){
                let baudRate = parseInt( data ).toString(16).toUpperCase();
                let checkBuf = Buffer( baudRate , 'hex');
                let checkSum = 0;
                for (let c = 0; c < checkBuf.length; c++) {
                    checkSum = checkSum + checkBuf[c];
                }
                checkSum = (checkSum & 0xff).toString(16).toUpperCase();
                // console.log(checkSum);
                let frame = 'AAF0' + baudRate + checkSum + '0D0B';

                Update.connect( this.com,parseInt(this.lastBaudType) );
                Update.port.writeCan(frame);
            },

//            connectHID() {
//                let connectStatus;
//                try{
//                    connectStatus = Update.port.usbSDK.client.connect();
//                }catch(err){
//                    console.log("Connect HID Error",err);
//                }
//                return connectStatus;
//            },
            splitOption(){
                this.optionsCom = this.optionsCom.filter((item)=>{
                    return item.type != 'bat' && item.type != 'sensor'
                });
                console.log(this.optionsCom);
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
                    this.isConnected = Update.port.isConnected();
                    this.isCan = !Update.port.checkUsbUart();

                    console.log("usbHID connect");
                    Update.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb') {
                            this.isConnected = false;
                        }
                    });
                    if(this.isConnected){
                        this.updateMsg = this.updateMsgObject.msg30;
                    }
                }else{
                    if (!this.com) {
                        this.$message({
                            message: this.$t('cNam.selectComTip'),
                            type: 'warning'
                        });
                        return;
                    } else {
                        Update.connect(this.com,parseInt(this.BaudRateType));
                        Update.getBus().subscribe(res => {
//                    console.log('update-vm1:', res);
                            if (res.type === 'open' || res.type === 'connect') {
                                this.isConnected = true;
                            }
                            if (res.type === 'close' || res.type === 'error') {
                                this.com = '';
                                this.isConnected = false;
                                this.loading = false;
//                        this.$message({
//                            message: this.$t('comDisconnect'),
//                            type: 'warning'
//                        });
                            }
                        });
                    }
                }
                if(!this.isCan){
                    this.splitOption();
                }
            },

            test(){
                Update.port.checkUsbConnection('0101',1);
            },

            readBase() {
                if(!this.isConnected){
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: 'warning'
                    });
                }
                if (!this.componentType) {
                    this.$message({
                        message: this.$t('componentTypeTip'),
                        type: 'warning'
                    });
                    return;
                }else{
                    switch(this.componentType){
                        case 'hmi':
                            this.device = this.deviceList['HMI'];
                            this.target = 0x03;
                            break;
                        case 'ctrl':
                            this.device = this.deviceList['controller'];
                            this.target = 0x02;
                            break;
                        case 'bat':
                            this.device = this.deviceList['bat'];
                            this.target = 0x04;
                            break;
                        case 'sensor':
                            this.device = this.deviceList['sensor'];
                            this.target = 0x01;
                            break;
                        case 'tool':
                            this.device = this.deviceList['tool'];
                            this.target = 0x05;
                            break;
                    }
                }
                this.loading = true;
                setTimeout(() => {
                    if (!this.sn) {
                        this.$message({
                            message: this.$t('cNam.needReadSn'),
                            type: "warning"
                        });
                        this.showHelp = true;
                    }else{
                        this.showHelp = false;
                    }
                    this.loading = false;
                }, 2000);
                this.connect(1);
                if( this.target < 0x05){
                    if (!this.isCan || HMI.port.checkUsbUart()) {
                        if(!this.usbHID){
                            this.device.connect(this.com);
                        }
                        this.device.sendCmdGetBaseData();
                        this.device.getBus().subscribe(res => {
                            if (res.type === 'close' || res.type === 'error') {
                                this.isConnected = false;
                                this.com  = "";
                            }
                            if (res.data) {
                                switch (this.componentType) {
                                    case 'hmi':
                                        this.sn = Core.Util.get(res, 'data.hmi.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.hmi.version', this.version);
                                        break;
                                    case 'ctrl':
                                        this.sn = Core.Util.get(res, 'data.ctrl.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.ctrl.version', this.version);
                                        break;
                                    case 'bat':
                                        this.sn = Core.Util.get(res, 'data.bat.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.bat.version', this.version);
                                        break;
                                    case 'sensor':
                                        this.sn = Core.Util.get(res, 'data.sensor.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.sensor.version', this.version);
                                        break;
                                    case 'tool':
                                        this.sn = Core.Util.get(res, 'data.tool.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.tool.version', this.version);
                                        break;
                                }
                                if (this.sn && this.version) {
                                    this.updateMsg = this.$t('cNam.updateMsgLO');
                                } else if (this.sn) {
                                    this.updateMsg = this.$t('cNam.updateMsgL');
                                }
                            }
                        });
                    }else{
                        this.device.port.getUsbBasic(this.target, true).subscribe(res => {
//                            console.log(res);
                            if (!res.data) {
                                return;
                            }
                            if (res.data.hasOwnProperty("sn")) {
                                this.sn = res.data.sn;
                            }
                            if (res.data.hasOwnProperty("version")) {
                                this.version = res.data.version;
                            }
                            if (this.sn && this.version) {
                                this.updateMsg = this.$t('cNam.updateMsgLO');
                            } else if (this.sn){
                                this.updateMsg = this.$t('cNam.updateMsgL');
                            }
                        });
                    }
                }else if(this.target == 0x05){
                    if(!this.usbHID){
                        this.device.connect(this.com);
                        this.device.sendCmdGetBaseData();
                        this.device.getBus().subscribe(res => {
                            if (res.type === 'close' || res.type === 'error') {
                                this.isConnected = false;
                                this.com  = "";
                            }
                            if (res.data) {
                                switch (this.componentType) {
                                    case 'tool':
                                        this.sn = Core.Util.get(res, 'data.tool.sn', this.sn);
                                        this.version = Core.Util.get(res, 'data.tool.version', this.version);
                                        this.hardware_version = Core.Util.get(res, 'data.tool.hardware_version', this.hardware_version);
                                        break;
                                }
                                if(this.sn && this.version){
                                    this.updateMsg = this.$t('cNam.updateMsgLO');
                                }else if(this.sn){
                                    this.updateMsg = this.$t('cNam.updateMsgL');
                                }
                            }
                        });
                    }else{
                        Tool.port.getUsbBoxBasic().subscribe(res => {
//                            console.log(res);
                            if (!res.data) {
                                return;
                            }
                            if (res.data.hasOwnProperty("sn")) {
                                this.sn = res.data.sn;
                            }
                            if (res.data.hasOwnProperty("version")) {
                                this.version = res.data.version;
                            }
                            if (res.data.hasOwnProperty("hardware_version")) {
                                this.hardware_version = res.data.hardware_version;
                            }
                        });
                    }
                }
            },

            startUpdate() {
                if(this.isTemAccount){
                    this.$message({
                        message: this.$t('cNam.tryAccountsTip'),
                        type: 'warning'
                    });
                    return;
                }
//                console.log(this.fileName, 'before');
                if(this.file === null){
                    this.$message({
                        message: this.$t('cNam.updateSelectFile'),
                        type: 'warning'
                    });
                    return;
                }
                this.beforeUpdate = true;
                let vm = this;
                setTimeout(()=>{
                    vm.beforeUpdate = false;
                },2000);
//                if(Core.Util){
//
//                }

                if( (Core.Data.getOrg().type !== Core.Const.ORG_TYPE.COMPONENT)) {
                    if( !this.componentType){
                        this.$message({
                            message: this.$t('componentTypeTip'),
                            type: 'warning'
                        });
                        return;
                    }
                    if(!this.sn){
                        this.$message({
                            message: this.$t('cNam.snEmptyTip'),
                            type: 'warning'
                        });
                        return;
                    }
                    if( !Core.Util.checkSNByType(this.sn.trim(), this.componentType.trim()) ){
                        let msg = ''
                        switch(this.componentType){
                            case 'hmi':
                                msg = this.$t('cNam.checkSnHmi')
                                break;
                            case 'ctrl':
                                msg = this.$t('cNam.checkSnCtrl')
                                break;
                            case 'sensor':
                                msg = this.$t('cNam.checkSnSensor')
                                break;
                            case 'tool':
                                msg = this.$t('cNam.checkSnTool')
                                break;
                        }
                        this.$message({
                            message: msg,
                            type: 'warning'
                        });
                        return;
                    }
                }

                if(this.offLineFlag || !navigator.onLine){
                    // 如果上一次的数据在被处理，需要等待
                    let isHandle = Core.Data.getIsBatchRecordIng();
                    if (isHandle) {
                        this.$message.warning(this.$t('offlineHandle'));
                        return;
                    }
                    this.getOfflineRecordList();
                    // 离线的到了 50 条 需要联网
                    if (this.offlineRecordList.length >= 50) {
                        this.$message.warning(this.$t('cNam.offlineCon'));
                        return;
                    }
                    this.doUpdate(this.file);
                }else{
                    this.doUpdate(this.file);
                }
            },

            doUpdate(data) {
                let count = 0;
                let vm = this;
                vm.startTip = true;
                if( vm.usbHID ){
//                    Update.port.getUSBList();
//                    Update.connect();
//                    Update.readFileData( data );
//                    Update.sendFileHeadFrameHID();

                    Update.port.usbSDK.client.events.on('firmware-update-progress',(e)=>{
                        vm.startTip = false;
                        vm.percentage = Math.ceil(e.upload / e.total * 100);
//                        console.log(e);
                        if( e.upload === e.total || vm.percentage === 100){
                            if(count === 0){

                                vm.updateOnline = false;

                                if (vm.sn && vm.org.id !== 25) {
                                    vm.saveUpdateRecord();
                                }
                                if(vm.componentType){
                                    switch(vm.componentType){
                                        case 'hmi':
                                            vm.updateMsg = vm.updateMsgObject.msg1;
                                            break;
                                        case 'ctrl':
                                            vm.updateMsg = vm.updateMsgObject.msg2;
                                            break;
                                        case 'tool':
                                            vm.updateMsg = vm.updateMsgObject.msg3;
                                            break;
                                    }
                                }else{
                                    vm.updateMsg = '';
                                }
                                vm.checkBoxUpdate();
                            }
                            count++;
                        }
                    });
                    Update.port.usbSDK.client.events.on('firmware-update-completed', (e) => {
                        vm.percentage = 0;
//                        this.$message({
//                            message: this.$t('cNam.updateSuccessTip'),
//                            type: 'success'
//                        });
                    });
                    Update.port.usbSDK.client.events.on('firmware-update-check', (e) => {
                        vm.percentage = 0;
//                        console.log(e);
                        if(e.d === 1){
                            vm.$message({
                                message: vm.updateMsgObject.successTip,
                                type: 'success',
                                duration: 3000,
                                showClose: true
                            });
                        }else{
                            vm.$message({
                                message: vm.updateMsgObject.failTip,
                                type: 'error',
                                duration: 5000,
                                showClose: true
                            });
                        }
//                        this.$message({
//                            message: this.$t('cNam.updateSuccessTip'),
//                            type: 'success'
//                        });
                    });
                    if(vm.updateOnline) {
                        if(vm.firmCheck(data.fileBuffer)){
                            Update.port.usbSDK.firmware.update.firmware(data.fileBuffer);
                        }
                    }else{
                        let reader = new FileReader();
                        reader.readAsArrayBuffer(vm.file);
                        reader.onload  = function(e){
//                            console.log(e.target.result);
                            if(vm.firmCheck(e.target.result)){
                                Update.port.usbSDK.firmware.update.firmware(e.target.result);
                            }
//                            console.log("[Arr]",arr[2]);
//                            console.log("[Arr]",vm.isCanSoft);
                        };
                        reader.onerror = function(e){
                            reader.abort();
                            vm.$message({
                                message: vm.updateMsgObject.readFailTip,
                                type: 'error',
                                duration: 5000,
                                showClose: true
                            });
                        }
                    }

                    Update.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if( res.type === 'error_usb'){
                            vm.isConnected = false;
                        }
                    });
//                    reader.onload  = function(e){
//                        console.log(e.target.result);
//                    };

                    console.log("start update",);
                    setTimeout(()=>{
                        if(vm.percentage === 0 && vm.componentType === 'tool' && !vm.isConnected){
                            vm.$message({
                                message: vm.updateMsgObject.toolReconnectTip,
                                type: 'info',
                                duration: 5000,
                                showClose: true
                            })
                        }
                    },2300)
                }else {
                    if (vm.device) {
                        vm.device.port.reset(100);
                    }
                    setTimeout( ()=>{
//                        console.log("update bound:" + this.BaudRateType);
                        Update.connect(vm.com, parseInt(vm.BaudRateType));

                        Update.sendPackage(data);
                        Update.getBus().subscribe(res => {
                            if (res.type === 'close' || res.type === 'error') {
                                vm.isConnected = false;
                                vm.com = "";
                            }
                            if (res.type === 'data' && res.data.update) {
                                vm.startTip = false;
                                let update = res.data.update;
                                if (update.percentage) {
                                    vm.percentage = Math.ceil(update.percentage * 100);
                                }
                                if (update.percentage === 1) {
                                    vm.$message({
                                        message: vm.updateMsgObject.successTip,
                                        type: 'success',
                                        duration: 3000,
                                        showClose: true
                                    });
                                    vm.percentage = 0;
                                    if (vm.sn && vm.org.id !== 25) {
                                        vm.saveUpdateRecord();
                                    }
                                }
                                if (update.fail) {
                                    vm.$message({
                                        message: update.fail,
                                        type: 'warning'
                                    });
                                }
                                if (update.error) {
                                    vm.$message({
                                        message: update.error,
                                        type: 'warning'
                                    });
                                }
                            }

                        });
                    },800)
                }
            },

            saveUpdateRecord(){
                if(!this.sn) {
                    this.$message.warning(this.$t('cNam.needReadSn'));
                    return false;
                }
                let sn = this.sn;
                let time = Core.Util.time();
                let filename = this.file.name || this.fileName;
                let type = this.recordType;
                this.sn = "";
                if(this.userName.indexOf("fqc") < 0){
                    if (!this.offLineFlag && navigator.onLine) {
                        Core.Api.ObjectRecord.save(sn, filename +"--"+ this.fileBufferCrc, time, type);
                    } else {
                        let userId = this.userId;
                        let recordData = {
                            userId: userId,
                            sn: sn,
                            data: filename +"--"+ this.fileBufferCrc,
                            time: time,
                            type: type
                        };
                        this.offlineRecordList.push(recordData);

                        Core.Data.setRecordList(this.offlineRecordList);
                    }
                }
            },

            getOfflineRecordList(){
                if(this.offLineFlag || !navigator.onLine){
                    this.offlineRecordList = Core.Data.getRecordList() || [];
                }
            },

            checkBoxUpdate(){
                let count = 1;

//                this.dialogUpdateResVisible = true;
//                Update.port.getListenData().subscribe((res) => {
//                    if (!res.data) {
//                        return;
//                    }
//                    if (res.type == 'error_usb') {
//                        this.isConnected = false;
//                        this.clear();
//                    }
//                    if(res.data.toString('hex').toUpperCase().substr(0,2) == '28'){
//                        console.log(res.data.toString('hex'));
////                        let len = res.data.toString('hex').toUpperCase().substr(2,4);
////                        this.boxPercentage = Math.ceil( parseInt( Buffer.from(len).toString('hex'), 16)/this.fileBufferLength*100 );
//                    }
//                });
                let vm = this;
                let res = Update.port.usbSDK.client.events.on('firmware-update-completed-progress',(e)=>{
//                    console.log(e); // {progress: 0, status: "01"}
//                    console.log(e.progress,vm.fileBufferLength);
                    if(vm.componentType){
                        switch (vm.componentType) {
                            case 'hmi':
                                vm.updateMsg = vm.updateMsgObject.msg4;
                                break;
                            case 'ctrl':
                                vm.updateMsg = vm.updateMsgObject.msg5;
                                break;
                            case 'tool':
                                vm.updateMsg = vm.updateMsgObject.msg6;
                                break;
                        }
                    }else{
                        vm.updateMsg = vm.updateMsgObject.msg10;
                    }
//                    console.log("[true]",vm.isCanSoft, vm.isCan);
//                    console.log("[true]",vm.isCanSoft || vm.isCan);
                    if(vm.isCanSoft){
                        vm.boxPercentage = Math.ceil(e.progress / vm.fileBufferLength * 100);
                    }else{
                        if( e.progress*128 > vm.fileBufferLength &&  e.progress*128 - vm.fileBufferLength < 1024){
                            vm.boxPercentage = 100;
                        }else{
                            vm.boxPercentage = Math.ceil((e.progress*128) / vm.fileBufferLength * 100);
                        }
                    }
                    if( vm.boxPercentage > 100){
                        vm.updateMsg = vm.updateMsgObject.msg11;
                    }
                    if( vm.boxPercentage === 0){
                        if(count === 1){
                            vm.updateMsg = vm.updateMsgObject.msg20;
                        }
                        count++;
                    }else {
                        count = 1;
                    }
                });
//                console.log(res,'[ud]');
            },
//            getFile(){
//                console.log(this.$refs.fileUpdate.files);
//                if (this.$refs.fileUpdate.files.length>0) {
//                    this.file = this.$refs.fileUpdate.files[0];
//                    console.log(this.file);
//                    this.fileName = this.file.name;
//                    if( this.file.path.match("OneDrive") ){
//                        this.$message({
//                            message: "Your file is in OneDrive folder, it is not supported now. Please move out.",
//                            type: 'warning'
//                        });
//                    }
//                }else{
//                    this.file = null;
//                    this.fileName = '';
//                }
//            },

            onChangeUpdateFile() {
                if (this.$refs.fileUpdate.files.length>0) {
                    this.file = this.$refs.fileUpdate.files[0];
//                    console.log(this.file);
                    this.fileBufferLength = this.file.size;
//                    console.log(this.fileBufferLength);
                    this.fileName = this.file.name;
                    if( this.file.path && this.file.path.match("OneDrive") ){
                        this.$message({
                            message: this.$t('cNam.updateOnedriveNote'),
                            type: 'warning'
                        });
                    }
                    if( this.file.name.substr(-3) !== 'bin' && this.file.type !== '"application/octet-stream"'){
                        this.$message({
                            message: this.$t('cNam.updateMsgL'),
                            type: 'warning'
                        });
                        this.file = null;
                        return;
                    }
                    let reader = new FileReader();
                    reader.readAsArrayBuffer(this.file);
                    let vm = this;
                    reader.onload  = function(e){
                        vm.firmCheck(e.target.result);
                    };
                }else{
                    this.file = null;
                    this.fileName = '';
                }
            },

            reset() {
                this.percentage = 0;
                this.isReset = true;
                this.showHelp = false;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                if( this.usbHID ){
                    this.startTip = false;
                    this.updateMsg = '';
                    this.boxPercentage = 0;
                    this.updateOnline = false;
//                    if(!Update.getBesst()){
//                        UpdateHID.getDevice();
//                        UpdateHID.connect();
//                        UpdateHID.resetHID(300);
//                    }else{
//                        UpdateHID.resetHID(300);
//                    }
//                    Update.connect();
                    Update.port.reset(300);
                    this.isConnected = false;
//                    Update.getBus().subscribe(res => {
//                        if (res.type === 'close' || res.type === 'error') {
//                            this.isConnected = false;
//                            this.com = '';
//                            this.$message({
//                                message: 'Reset Success',
//                                type: "success"
//                            });
//                        }
//                    });
                }else{
                    if( this.com ){
                        Update.connect(this.com,parseInt(this.BaudRateType));
                        Update.port.reset(300);
                        setTimeout(()=>{
                            this.isConnected = Update.port.isConnected();
                        },400);
                    }
                }
            },
            clear(){
                this.sn = '';
                this.version = '';
            },
            selectItem(item) {
                Core.Api.Firmware.getHistory(item.id).then(res => {
                    this.HWRecordList = res.record;
                    if (res.record.length <= 0) {
                        this.$message({
                            message: this.$t('noHardwareFileTip'),
                            type: "warning"
                        });
                    } else {
                        this.dialogListVisible = false;
                        this.queryItemCondition = item.name;
                        this.dialogRecordListVisible = true;
                    }
                });
            },


            getAllCategory() {
                Core.Api.Category.queryUpdateComponent().then(res => {
                    this.categoryList = res.category_list;
                });

            },

            getItemListByCategoryId() {
                Core.Api.Item.getListForUpdate(this.selectCategory.id, this.currentPage).then(res => {
                    this.itemList = res.item_list;
                    this.total = res.count;
                    if (this.itemList.length > 0) {
                        this.dialogObjectListVisible = true;
                    } else {
                        this.$message({
                            message: this.$t('objectEmptyTip'),
                            type: 'warning'
                        });
                    }
                });
            },

            showFirmware(item) {
                this.selectObject = item;
                this.dialogObjectListVisible = false;
                this.selectItem(item);
            },

            handleItemListPageChange(page) {
                this.currentPage = page;
                this.getItemListByCategoryId();
            },

            // 获取固件 by sn
            getFirmwareBySn() {
                let sn = this.sn.trim();
                if (!sn) {
                    this.$message({
                        message: this.$t('cNam.snEmptyTip'),
                        type: 'warning'
                    });
                    return
                }

                if (sn.length > 40) {
                    this.$message({
                        message: "Please check SN. Maximum length is 40.",
                        type: 'warning'
                    });
                    return
                }

                Core.Api.Firmware.getHistoryBySn(sn)
                    .then(res => {
                        let record = res.record;
                        console.log(this.version, this.hardware_version,"BSF");
                        this.HWRecordList = record;
                        if(this.HWRecordList.length > 0){
                            this.dialogRecordListVisible = true;
                        }else{
                            this.$message({
                                message: "There is no history software in BESST.",
                                type: 'warning'
                            });
                        }

                    })
            },

            // 在线更新
            updateObject(item) {
                if(this.isTemAccount){
                    this.$message({
                        message: this.$t('cNam.tryAccountsTip'),
                        type: 'warning'
                    });
                    return;
                }
                let url = Core.Const.NET.FILE_URL_PREFIX + item.file;
                let $this = this;
                $this.fileName = item.version;
                $this.$http.get(url, {responseType: 'blob'})
                    .then((response) => {
//                        console.log(response, 'get complete');
                        $this.fileBufferLength = response.body.size;
//                        console.log(this.fileBufferLength, 'get complete');

                        /*
                         body:Blob(39899) {size: 39899, type: "application/octet-stream"}
                         bodyBlob:Blob(39899) {size: 39899, type: "application/octet-stream"}
                         headers:Headers {map: {…}}
                         ok:true
                         status:200
                         statusText:"OK"
                         url:"http://static.besst.bafang-service.com/besst/file/0f0c813df4f10736234a7019002f3bbe18e16f9ae8b9785c5befa030b79aee55.bin"
                         data:(...)
                         */
//                        console.log(typeof response.body);
                        // console.log(response.body.length)
//                        let fileBody = response.body;
//                        console.log(fileBody, 'getFile');
                        $this.$message({
                            message: $this.$t('cNam.updateGetFileSucc'),
                            type: 'warning'
                        });
                        var fileReader = new FileReader();
                        fileReader.onload = function () {
                            let data = {
                                fileBuffer: this.result,
                                name: item.version,
                                type: 'data'
                            };
                            if($this.firmCheck(this.result)){
                                $this.doUpdate(data);
                                $this.dialogRecordListVisible = false;
                            }
//                            console.log("[Arr]",arr[2]);
//                            console.log("[Arr]",$this.isCanSoft);

                        };
                        fileReader.readAsArrayBuffer(response.body);
                        fileReader.onerror = function(e){
                            fileReader.abort();
                            $this.$message({
                                message: $this.updateMsgObject.readFailTip,
                                type: 'error',
                                duration: 5000,
                                showClose: true
                            });
                        }
                    });
                $this.updateOnline = true;
            },

            // 软件程序前16个字符检查
            firmCheck( buffer ) {

                let arr = Buffer.from(buffer);
                let BESST_Pro = Update.port.getBESST_Pro();
                let BESST_update = false;
                if( BESST_Pro.soft_ver && BESST_Pro.soft_ver.indexOf('BSF33.0')>-1 && BESST_Pro.soft_ver > "BSF33.03" ){
                    BESST_update = true
                }
                this.fileBufferCrc = Update.checkCrc(arr);

                let show = false;
                if(arr[2]>128 && arr[2]<150){
                    this.isCanSoft = true;
                }else if(arr[2]>0 && arr[2]<8){
                    this.isCanSoft = false;
                }else if(arr[2] !== 0xFC){
                    show = true;
                }
                let res = false;
                if(this.isCanSoft){
                    let tem = arr.slice(18,32);
                    if(Buffer.from(tem).toString('hex') === '0000000000000000000000000000'){
                        let fileContentBuffer_32 = arr.slice(32);
                        let fileCrc_32 = Update.checkCrc(fileContentBuffer_32);
                        let bufferCrc = Buffer.from(arr.slice(16, 18)).toString('hex');
                        if( fileCrc_32 !== bufferCrc && BESST_update){
                            this.$message({
                                message: this.$t("cNam.updateCrcError"),
                                type: "warning",
                                duration: 5000,
                                showClose: true,
                            });
                        }
                    }else{
                        for(let i=5; i<14; i++ ){
                            if(arr[i]!== 0){
                                res = true;
                            }
                        }
                    }
                }else{
                    for(let i=4; i<14; i++ ){
                        if(arr[i]!== 0){
                            res = true;
                        }
                    }
                }
                if(res || show){
                    this.$message({
                        message: this.$t("cNam.updateDataError"),
                        type: "warning",
                        duration: 5000,
                        showClose: true,
                    });
                    return false;
                }else{
                    return true;
                }
            },
        },
        beforeDestroy: function () {
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionInterval){
                clearInterval(this.connectionInterval);
            }
            Update.port.setBoxOperation();
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .update {
        padding-top: 44px;
        width: 100%;
        height: 100%;
        overflow: auto;
        .update-content {
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
            .content {
                overflow: hidden;
                background: #353945;
                width: 950px;
                margin-top: 22px;
                padding-bottom: 30px;
                .left {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    border-right: 1px dotted #2B2F3B;
                    overflow: hidden;
                    .btns {
                        margin-top: 30px;
                        margin-right: 26px;
                        .btn {
                            margin-left: 10px;
                        }
                    }

                    .online {
                        .label {
                            display: block;
                            margin-top: 20px;
                            margin-left: 23px;
                            padding: 10px;
                            font-size: 14px;
                        }
                        .btn {
                            margin-left: 33px;
                        }
                        .input {
                            margin-left: 6px;
                            width: 300px;
                        }
                    }
                    .item {
                        margin-left: 33px;
                        height: 60px;
                        line-height: 60px;
                        .cloud{
                            text-align: left;
                            span{
                                padding-left: 10px;
                            }
                        }
                        .label {
                            width: 110px;
                            display: inline-block;
                        }
                        .input {
                            width: 300px;
                        }
                        .tip {
                            display: inline-block;
                            border-radius: 50%;
                            width: 10px;
                            height: 10px;
                            background: #E26829;
                            margin-left: 5px;
                        }
                        .connect {
                            background: #76EE00;
                        }
                    }
                    .btn {
                        height: 30px;
                        line-height: 30px;
                        .operation {
                            margin-right: 26px;
                            .loading {
                                display: inline-block;
                                vertical-align: middle;
                                margin-top: -13px;
                            }
                        }

                    }
                    .space{
                        width:100%;
                        height:10px;
                    }
                }
                .right {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    .item {
                        margin-left: 33px;
                        height: 60px;
                        line-height: 60px;
                        .el-select {
                            width: 300px;
                        }
                        .label {
                            width: 90px;
                            display: inline-block;
                        }
                        .input {
                            margin-left: 10px;
                            width: 310px;
                        }
                        .tip {
                            display: inline-block;
                            border-radius: 50%;
                            width: 10px;
                            height: 10px;
                            background: #E26829;
                            margin-left: 10px;
                        }
                        .connect {
                            background: #76EE00;
                        }
                    }
                    .note {
                        margin-left: 33px;
                        padding: 5px 33px 0 0;
                        height: 42px;
                        line-height: 42px;
                        .msg-title {
                            width: 100%;
                            border-top: 1px solid #2B2F3B;
                            color: #E26829;
                        }
                        .msg {
                            width: 100%;
                        }
                    }
                    .btns {
                        margin-right: 30px;
                        .btn {
                            margin-left: 10px;
                        }
                    }
                    .tip {
                        margin-top: 20px;
                        color: #E26829;
                        line-height: 30px;
                    }
                }
                .file {
                    position:relative;
                    .label {
                        display:inline-block;
                        height: 26px;
                        line-height: 26px;
                        margin-left: 33px;
                        background-color: #E26829;
                        border-radius: 26px;
                        padding-left: 25px;
                        /*padding-right: 30px;*/
                    }
                    .text-span{
                        display:block;
                        width:400px;
                        height:20px;
                        margin: 15px 0 0 33px;
                        /*overflow: hidden;*/
                        /*text-overflow: ellipsis;*/
                        /*-o-text-overflow: ellipsis;*/
                        /*white-space:nowrap;*/
                    }
                    .btn {
                        margin-left: 33px;
                    }
                    .file-input {
                        display: none;
                        margin: 10px 30px;
                        width: 300px;
                    }
                    .textFixed{
                        position:absolute;
                        top:0;
                        left:0;
                    }
                    .el-progress {
                        padding: 5px 0  0 33px;
                    }

                }
                .step{
                    margin-left: 33px;
                    margin-top: 9px;
                    span{
                        display:inline-block;
                    }
                    .no{
                        font-weight:bold;
                        width: 2%;
                        vertical-align: top;
                    }
                    .stepText{
                        width: 92%;
                        line-height: 20px;
                    }
                }
            }
        }
        .dialogSelectItem {
            .el-dialog__header {
                display: none;
            }
        }
        .el-dialog--small {
            width: 770px;
        }
        .history-table {
            width: 100%;
            border-top: 1px solid rgba(255, 255, 255, 0.1);
            tr {
                line-height: 30px;
            }
            .head {
                color: white;
            }
            .history-td {
                padding: 0 6px;
                width: 300px;
                border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                font-size: 14px;
                text-align: center;
            }
            .img {
                width: auto;
                height: 30px;
                vertical-align: middle;
                background-color: white;
            }
        }

        .pagination {
            padding: 10px;
        }

        .update-res-content {
            /*width: 510px;*/
            /*display: flex;*/
            /*flex-direction: column;*/
            /*background: #353945;*/
            /*margin-top: 52px;*/
            /*.update-res-top {*/
            /*height: 40px;*/
            /*padding: 0 8px;*/
            /*display: flex;*/
            /*justify-content: flex-end;*/
            /*align-items: center;*/
            /*background: rgb(42, 46, 58);*/
            /*& .el-icon-close {*/
            /*font-size: 12px;*/
            /*padding: 12px;*/
            /*cursor: pointer;*/
            /*}*/
            /*}*/
            .update-res-center {
                display: flex;
                flex-direction: column;
                padding: 5px 20px;
                span {
                    font-size: 13px;
                }
                .item {
                    padding: 5px 0;
                    display: flex;
                    flex-direction: row;
                    align-items: flex-start;
                    .left {
                        width: 100px;
                    }
                    .right {
                        flex: 1;
                    }
                }
                .type {
                    align-items: center;
                    img {
                        margin-right: 10px;
                    }
                }
                .note {
                    min-height: 100px;
                }
                .file {

                }
            }
            .update-res-bottom {
                padding: 10px 24px;
                display: flex;
                justify-content: space-between;
            }
        }
    }
</style>



// WEBPACK FOOTER //
// update.vue?09db235a