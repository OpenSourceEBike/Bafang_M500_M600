<template>
    <div class="component-order-connect">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" title="返回"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                <div class="content-card-item">
                    <label class="content-header">{{ $t('orderInfo.title') }}</label>
                    <div class="content-body">
                        <div class="item">
                            <label class="label">{{ $t('orderInfo.model') }}</label>
                            <span>{{itemInfo.name || '/'}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('orderInfo.customerName') }}</label>
                            <span>{{orderInfo.customer_id || '/'}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('orderInfo.customerPoNo') }}</label>
                            <span>{{orderInfo.customer_po_no || '/'}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('orderInfo.customerPartNo') }}</label>
                            <span>{{orderItemInfo.part_no || '/'}}</span>
                        </div>
                        <div class="item">
                            <label class="label">Produced/Amount</label>
                            <span>{{orderItemInfo ? orderItemInfo.produced : 0}}/{{orderItemInfo ? orderItemInfo.amount : 0}}</span>
                        </div>
                    </div>
                </div>

                <!--查询-->
                <div class="content-card-item">
                    <label class="content-header">搜索/扫描</label>
                    <div class="content-body">
                        <div class="hardware-item">
                            <div class="input-holder">
                                <label class="label">SN</label>
                                <el-input class="sn input-sn" v-model="mainSn" ></el-input>
                                <el-button type="preview" @click="readBase"> 读取控制器和传感器 </el-button>
                            </div>
                        </div>
                    </div>
                </div>

                <!--结果集-->
                <div class="content-card-item">
                    <label class="content-header">状态列表</label>
                    <div class="content-body">
                        <div class="hardware-item">
                            <div class="select-holder">
                                <label class="label">{{ $t('cNam.controller') }}</label>
                                <el-input v-model="controllerSn" placeholder="-"></el-input>
                                <label class="tip" :class="{connect: isControllerConnected}"></label>
                                <el-button type="preview" @click="readController">{{ $t('cNam.read') }}</el-button>
                            </div>
                            <div class="select-holder">
                                <label class="label">{{ $t('cNam.sensor') }}</label>
                                <el-input v-model="sensorSn" placeholder="-">{{sensorSn || '-'}}</el-input>
                                <label class="tip" :class="{connect: isSensorConnected}"></label>
                                <el-button type="preview" @click="readSensor">{{ $t('cNam.read') }}</el-button>
                            </div>
                            <div class="select-holder">
                                <label class="label">{{ $t('cNam.motor') }}</label>
                                <el-input v-model="motorSn" placeholder="-">{{motorSn || '-'}}</el-input>
                                <label class="tip" :class="{connect: isMotorConnected}"></label>
                            </div>
                        </div>
                        <div class="operation-item">
                            <div class="">
                                <label class="label">位置传感器标定</label>
                                <el-button class="btn-connect" @click="sensorSetStart">开始</el-button>
                                <span style="padding:0 20px 0 20px" v-if="sensorStart">位置传感器标定开始</span>
                            </div>
                            <div class="">
                                <label class="label">传感器0点标定</label>
                                <el-button class="btn-connect" @click="sensorZeroSetStart">开始</el-button>
                                <span style="padding:0 20px 0 20px" v-if="sensorZeroStart">传感器0点标定开始</span>
                            </div>
                        </div>
                    </div>
                    <div class="content-body">
                        <div class="btn-item">
                            <div class="operation-holder left">
                                <el-button type="warning" @click="clearUp">{{ $t('cNam.clear') }}</el-button>
                                <el-button type="danger" @click="removeBindingConnect">解除绑定</el-button>
                                <el-button type="danger" @click="findBindingRecordList">查看绑定记录</el-button>
                            </div>
                            <div class="operation-holder right">
                                <span  style="padding:0 20px 0 20px">{{num}}</span>
                                <el-button type="primary" @click="saveBinding">绑定</el-button>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="content-card-item">
                    <label class="content-header mark">刷程序
                        <el-switch
                        v-model="updateOperation"
                        :on-text=" '隐藏' "
                        :off-text=" '显示' ">
                        </el-switch></label>
                    <div class="content-body" v-show="updateOperation">
                        <div class="hardware-item">
                            <div >
                                <el-button class="cloud" :style="a" @click="getFirmware()" type="primary">{{ $t('up.fileCloudToUpdate') }}</el-button>
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
                                <label class="label" :style="a" for="uploadFile">{{ $t('up.fileToUpdate') }} </label> <br>
                                <span>{{fileName}}</span>
                                <input class="file-input" id="uploadFile" ref="fileUpdate" type="file"
                                       @change = 'onChangeUpdateFile()'/>
                            </div>
                            <div class="file">
                                <el-progress v-show="percentage > 0" :percentage="percentage"></el-progress>
                                <span v-show="startTip"> {{ $t('cNam.updateStarting') }}</span>
                            </div>
                            <div class="pull-right">
                                <el-button @click="startUpdate" type="preview">{{ $t('up.update') }}</el-button>
                            </div>
                        </div>
                        <div class="hardware-item">
                            <div class="select-holder" v-if = "!usbHID">
                                <label class="label">{{ $t('programingInfo.com') }}</label>
                                <el-select v-model="com" @click.native="getComList" :placeholder="$t('up.select')">
                                    <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                                    </el-option>
                                </el-select>
                                <label class="tip" :class="{connect: isConnected}"></label>
                                <el-button type="primary" @click="connect">{{ $t('connect') }}</el-button>
                                <el-button type="danger" @click="reset">{{ $t('reset') }}</el-button>
                            </div>
                            <div class="select-holder" v-else>
                                <label class="label">USB HID</label>
                                <label class="tip" :class="{connect: isConnected}"></label>
                                <el-button type="primary" @click="connect">{{ $t('connect') }}</el-button>
                                <el-button type="danger" @click="reset">{{ $t('reset') }}</el-button>
                            </div>
                            <div class="clear-10"></div>
                            <!--<div class="file-upload-holder">-->
                            <!--<label class="label">{{ $t('programingInfo.file') }}</label>-->
                            <!--<input class="file-input" ref="fileUpdate" type="file"/>-->
                            <!--<el-progress v-show="percentage > 0 && percentage < 100" :percentage="percentage"></el-progress>-->
                            <!--</div>-->
                            <!--<div class="operation-holder">-->
                            <!--<div class="icons">-->
                            <!--<div class="icon-item update" @click="startUpdate" :title="$t('update')"></div>-->
                            <!--</div>-->
                            <!--&lt;!&ndash;<el-button type="primary" @click="startUpdate">{{ $t('update') }}</el-button>&ndash;&gt;-->
                            <!--</div>-->
                            <div class="input-holder" v-show="true">
                                <div class="note">
                                    <div class="msg-title">{{ $t('cNam.updateMsg') }}</div>
                                </div>
                                <div class="note">
                                    <div class="msg">{{ updateMsg }}</div>
                                </div>
                                <div class="clear-30"></div>
                            </div>
                            <div class="file">
                                <el-progress v-show="boxPercentage > 0" :percentage="boxPercentage"></el-progress>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="clear-10"></div>
                <div class="content-card-item">
                    <label class="content-header mark">控制器设置
                        <el-switch
                            v-model="setOperation"
                            :on-text=" '隐藏' "
                            :off-text=" '显示' ">
                        </el-switch></label>
                    <div class="content-body" v-show="setOperation">
                        <div class="item">
                            <label class="label" >{{ $t('cNam.speedLimit') }}</label>
                            <el-select v-model="speedLimit"
                                       @change=""
                                       class="com-select"
                                       @click.native=""
                                       :placeholder="$t('cNam.select')">
                                <el-option v-for="(item, index) in speedLimits" :key=index :label="item" :value="item" >
                                </el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label" >{{ $t('cNam.wheelDiameter') }}</label>
                            <el-select v-model="wheelDiameter"
                                       @change="updateWheelCircumference"
                                       class="com-select"
                                       @click.native=""
                                       filterable
                                       :placeholder="$t('cNam.select')">
                                <el-option v-for="(item, index) in wheelDiameters" :key=index :label="item.label" :value="item.value" >
                                </el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label" >{{ $t('cNam.circumference') }}</label>
                            <el-select v-model="circumference"
                                       @change=""
                                       class="com-select"
                                       @click.native=""
                                       filterable
                                       allow-create
                                       :placeholder="$t('cNam.select')">
                                <el-option v-for="(item, index) in circumferences" :key=index :label="item" :value="item" >
                                </el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <div class="btn-group">
                                <el-button type="primary" class="btn-write" @click="clickWriteParam()">{{ $t('cNam.write') }}</el-button>
                                <el-button type="success" class="btn-write" @click="clickReadParam()">{{ $t('cNam.read') }}</el-button>
                            </div>
                        </div>
                        <!--<div class="item">-->
                            <!--限速：{{speedLimitRead}}, 轮径：{{wheelDiameterRead}}, 周长：{{circumferenceRead}}-->
                        <!--</div>-->
                    </div>
                    <div class="content-body" v-show="setOperation && hasCtl">
                        <div class="item" v-for="item in needAttrList">
                            <label class="label">{{item.attr_def_name}}</label>
                            <span>{{item.value}}</span>
                        </div>
                        <div class="hardware-item">
                            <div  class="select-holder">
                                <label class="label">SN长度</label>
                                <el-select v-model="snLength" placeholder="Please select" @change="changeSnLength()">
                                    <el-option v-for="item in snLengthList" :key=item :label="item" :value="item">
                                    </el-option>
                                </el-select>
                            </div>
                        </div>
                        <div class="hardware-item">
                            <div  class="input-holder">
                                <label class="label">SN</label>
                                <el-input v-model="controllerSn" class="sn input-sn" ref="content" id="scan"></el-input>
                                <el-button type="primary" @click="writeSnController">{{ $t('cNam.write') }}</el-button>
                                <el-button type="primary" @click="next" :disabled="!canNext || isSaving">下一个</el-button>
                            </div>
                        </div>
                        <div class="clear-10"></div>
                        <div class="item" v-if = "!isCan">
                            <label class="label">{{$t('cNam.nmlVolt')}}</label>
                            <span :class="{errorTip: errorList.indexOf('voltage') > -1}">{{nmlVolt}}</span>
                        </div>
                        <div class="item" v-if = "!isCan">
                            <label class="label">{{$t('cNam.limitCurrent')}}</label>
                            <span :class="{errorTip: errorList.indexOf('current_limit') > -1}">{{limitCurrent}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{$t('cNam.hardwareVersion')}}</label>
                            <span :class="{errorTip: errorList.indexOf('hard_ver') > -1}">{{hardVersion}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{$t('cNam.softwareVersion')}}</label>
                            <span :class="{errorTip: errorList.indexOf('soft_ver') > -1}">{{version}}</span>
                        </div>
                        <div class="item" v-if = "!isCan">
                            <label class="label">{{$t('cNam.spdMeterSignalNum')}}</label>
                            <span :class="{errorTip: errorList.indexOf('pas_signals') > -1}">{{pasSignals}}</span>
                        </div>
                        <div class="item">
                            <label class="label">{{$t('cNam.model')}}</label>
                            <span :class="{errorTip: errorList.indexOf('model') > -1}">{{model}}</span>
                        </div>
                        <div class="item" v-if = "isCan">
                            <label class="label">{{$t('cNam.speedLimit')}}</label>
                            <span :class="{errorTip: errorList.indexOf('speedLimit') > -1}">{{speedLimitRead}}</span>
                        </div>
                        <div class="item" v-if = "isCan">
                            <label class="label">{{$t('cNam.wheelDiameter')}}</label>
                            <span :class="{errorTip: errorList.indexOf('wheelDiameter') > -1}">{{wheelDiameterRead}}</span>
                        </div>
                        <div class="item" v-if = "isCan">
                            <label class="label">{{$t('cNam.circumference')}}</label>
                            <span :class="{errorTip: errorList.indexOf('circumference') > -1}">{{circumferenceRead}}</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!--固件的历史记录弹窗-->
        <el-dialog :title="queryItemCondition" :visible.sync="dialogRecordListVisible"  size="large">
            <table border="0" cellspacing="0" cellpadding="0" class="history-table">
                <thead>
                <tr>
                    <td class="history-td head">
                        {{ $t('up.updateTime') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('up.version') }}
                    </td>
                    <td class="history-td head">
                        文件名称
                    </td>
                    <td class="history-td head">
                        {{ $t('up.remark') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('up.operation') }}
                    </td>
                </tr>
                </thead>
                <tr v-for="item in HWRecordList">
                    <td class="history-td">{{item.create_time | dateFormat}}</td>
                    <td class="history-td">{{item.version}}</td>
                    <td class="history-td">{{item.firmware_name}}</td>
                    <td class="history-td">{{item.remark}}</td>
                    <td class="history-td">
                        <el-button type="text" @click="updateObject(item)">
                            {{ $t('up.update') }}
                        </el-button>
                    </td>
                </tr>
            </table>
        </el-dialog>

        <!--提示弹窗-->
        <el-dialog title="Tip" :visible.sync="dialogTipVisible" size="small">
            <p>请检查 {{tip}}</p>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="saveComponent">{{$t('cNam.save')}}</el-button>
                <el-button type="primary" @click="clear()">{{$t("cNam.cancel")}}</el-button>
            </div>
        </el-dialog>

        <!--查询绑定记录-->
        <el-dialog title="绑定记录" :visible.sync="dialogBindingRecordVisible" size="large">
            <table border="0" cellspacing="0" cellpadding="0" class="history-table">
                <thead>
                <tr>
                    <td class="history-td head">
                        电机
                    </td>
                    <td class="history-td head">
                        控制器
                    </td>
                    <td class="history-td head">
                        传感器
                    </td>
                    <td class="history-td head">
                        创建时间
                    </td>
                    <td class="history-td head">
                        跟新时间
                    </td>
                </tr>
                </thead>
                <tr v-for="item in bindingRecordList">
                    <td class="history-td">{{item.motor_sn}}</td>
                    <td class="history-td">{{item.controller_sn}}</td>
                    <td class="history-td">{{item.sensor_sn}}</td>
                    <td class="history-td">{{item.create_time | dateFormat}}</td>
                    <td class="history-td">{{item.update_time | dateFormat}}</td>
                </tr>
            </table>
            <div class="table-footer">
                <el-pagination
                    @current-change="handleCurrentChange"
                    :current-page.sync="currentPage"
                    :page-size="10"
                    layout="prev, pager, next, jumper"
                    :total="total">
                </el-pagination>
                <p class="total">Total: {{total}}</p>
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="dialogBindingRecordVisible = false">{{$t("cNam.cancel")}}</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import Core from 'core/core'
    import HMI from '../../../../device/hmi'
    import Battery from '../../../../device/battery'
    import Controller from '../../../../device/controller'
    import Tool from '../../../../device/tool'
    import Update from '../../../../device/update'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import circumferenceObject from 'core/circumference'

    export default {
        name: "componentOrderDataConnect",
        data() {
            return {
                orderId: '',
                item: '',
                itemPass: '',
                orderInfo: '',
                orderItemInfo: '',
                itemInfo: '',
                category_root: '',
                quantity: '-',

                bindId: 0,

                isControllerConnected: false,
                isSensorConnected: false,
                isMotorConnected: false,
                motorSn: '',
                sensorSn: '',
                controllerSn: '',
                mainSn: '',
                num: 0,


                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',
                sensorStart: false,
                sensorZeroStart: false,

                startTip: false,

                beforeUpdate: false,
                a: {
                    width: '280px',
                },
                isCan: false,
                isCanSoft: false,
                usbHID: true,
                isReset: false,
                target: 0x03,
                loading: false,

                updateMsg: '',
                updateMsgObject: {},
                updateOnline: false,
                fileName: '',
                file: null,
                fileBufferLength: 0,

                percentage: 0,
                boxPercentage: 0,
                queryItemCondition: '',
                dialogListVisible: false,
                dialogRecordListVisible: false,
                HWRecordList: [],
                bindingRecordList: [],
                total: 0,
                currentPage: 1,

                com: '',
                comList: [],
                isConnected: HMI.port.isConnected(),
                sn: '',
                isWrite: true,

                //  刷程序计数
                orderCount: 0,
                orderSnCount: 0,
                snUpdateRecord: [],
                snUpdateRecordDialogVisible: false,
                firmwareVersion: '',
                reconnectInterval: null,
                connectionInterval: null,

                // 控制器
                readInterval: null,
                lanTrans: ['Turn off', 'Turn on'],
                limitCurrent: '',
                pasSignals: '',

                speedLimit: '',
                speedLimits: [],
                wheelDiameter: '',
                wheelDiameters: [],
                circumference: '',
                circumferences: [],
                manName: '',
                clientCode: '',
                clientCodeRead: '',

                model: '',
                version: '',
                hardVersion: '',
                curList: [],
                spdList: [],
                manNameRead: '',
                nmlVolt: '',
                maxCur: '',
                limitCur: '',
                limitVolt: '',
                speedLimitRead: '',
                wheelDiameterRead: '',
                circumferenceRead: '',
                spdMeterType: '',
                spdMeterSignalNum: '',
                conCapacityLeft: '',
                conSingleTrip: '',
                conCadence: '',
                conTorque: '',
                conSingleTripLeft: '',
                conSpeed: '',
                conCurrent: '',
                conVoltage: '',
                conTemperature: '',
                motorTemperature: '',
                assistance: '',
                calorie: '',

                modelWrite: '',
                hardwareWrite: '',
                errorCode: '',
                bootLoad: '',
                manufacturer: '',


                // 传感器


                // 组织
                org: Core.Data.getOrg(),
                Core: Core,
                userName: Core.Data.get("userName") || "",

                Controller: Controller,
                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.EDIT,

                setOperation: true,
                updateOperation: false,
                saveCtlOperation: false,
                needAttrList: [],
                ctrlAttrValues: {},
                tip: '',
                hasCtl: false,
                hasCtlAmount: 0,
                snLengthList: [20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40],
                snLength: '',
                snLengthCheck: false,
                canNext: false,
                itemRoot: '',
                errorList: [],
                dialogTipVisible: false,
                dialogBindingRecordVisible: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Component.Order.Produce,
                zh: ZH.Component.Order.Produce,
                de: DE.Component.Order.Produce,
                nl: NL.Component.Order.Produce
            }
        },
        mounted: function () {
            this.orderId = this.$route.query.orderId;
            this.itemId = this.$route.query.itemId;
            this.quantity = this.$route.query.account;
            // 获取订单详情
            this.getProductionOrderDetail(this.orderId);
            // 获取item详情
            this.getItemDetail(this.itemId);

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
                successTip: this.$t('cNam.updateSuccessTip')
            };

            this.updateMsg = this.updateMsgObject.msg0;
            Update.port.setBoxOperation();
            this.checkConnection();
            this.wheelDiameters = Core.Const.wheelDiameters;

            this.initTrans();

        },
        methods: {
            getProductionOrderDetail(orderId) {
                Core.Api.Order.getProductionOrderDetail(orderId).then(res => {
                    this.orderInfo = res.order;
                    this.getOrderItemInfo(res.order.item_list);
                    Core.Api.Binding.findByOrderIdCount(this.orderInfo.source_id).then(res=>{
                        this.num= res.count;
                    });
                })
            },

            getOrderItemInfo(list) {
                let $this = this;

                list.forEach((item)=>{
                    if(item.item_id === $this.itemId){
                        $this.orderItemInfo = item
                    }else if(item.category_root_id === 7){
                        $this.hasCtl = true;
                        $this.itemRoot = 7;
                        $this.itemPass = item;
                        if( $this.itemPass.item_name && $this.itemPass.item_name.indexOf('CR') > -1 && $this.itemPass.item_name.indexOf(".FC") > -1 ){
                            $this.isCan = true;
                        }
                        Core.Api.Item.getItemDetail(item.item_id).then(res => {
//                            console.log(res);
                            $this.getControllerAttr(res.item.attr_list);
                            $this.getDeviceList();
                        })
                    }
                })
            },

            getControllerAttr(attrList) {
                if (attrList.length <= 0) {
                    return;
                }
                let needNameList,attrTem;
                needNameList = ['voltage', 'current_limit', 'soft_ver', 'hard_ver', 'pas_signals'];
                if( this.usbHID && this.isCan ){
                    needNameList = ['circuference', 'wheel_diameter', 'speed_limit',  'soft_ver', 'hard_ver'];
                }
                attrTem = attrList.filter((item) => {
                    return needNameList.indexOf(item.attr_def_key) > -1;
                });
                attrTem.forEach((item) => {
                    if(item.attr_def_key === "circuference"){
                        this.circumference = item.value;
                    }else if(item.attr_def_key === "wheel_diameter"){
                        this.wheelDiameter = item.value;
                    }else if(item.attr_def_key === "speed_limit"){
                        this.speedLimit = item.value;
                    }
                    this.ctrlAttrValues[item.attr_def_key] = item.value + '';
                });
                this.needAttrList = attrList.filter((item) => {
                    return ['soft_ver', 'hard_ver'].indexOf(item.attr_def_key) > -1;
                });
            },

            checkController() {
                this.tip = '';
//                if (this.model !== this.itemInfo.name.substr(0, this.model.length)) {
//                    this.tip = "Model ";
//                }
                if (this.version.replace(/\s/g,"") != this.ctrlAttrValues['soft_ver'].replace(/\s/g,"")) {
                    this.tip = this.tip + ", Software Version";
                }
                if (this.hardVersion.replace(/\s/g,"") != this.ctrlAttrValues['hard_ver'].replace(/\s/g,"")) {
                    this.tip = this.tip + ", Hardware Version";
                }
                if( !this.isCan ){
                    if (this.nmlVolt != this.ctrlAttrValues['voltage']) {
                        this.tip = this.tip + ", Voltage";
                    }
                    if (this.limitCurrent != this.ctrlAttrValues['current_limit']) {
                        this.tip = this.tip + ", Limit Current";
                    }
                    if (this.pasSignals != this.ctrlAttrValues['pas_signals']) {
                        this.tip = this.tip + ", Pas Signals";
                    }
                }else{
                    if (this.circumference != this.ctrlAttrValues['circuference']) {
                        this.tip = this.tip + ", Circumference";
                    }
                    if (this.wheelDiameter != this.ctrlAttrValues['wheel_diameter']) {
                        this.tip = this.tip + ", Wheel diameter";
                    }
                    if (this.speedLimit != this.ctrlAttrValues['speed_limit']) {
                        this.tip = this.tip + ", Speed limit";
                    }
                }
                if (this.tip !== '') {
                    return false
                }
                return true;
            },

            getItemDetail(id) {
                let vm = this;
                Core.Api.Item.getItemDetail(id).then(res => {
                    vm.itemInfo = res.item;
                    vm.category_root = res.item.category_root_id;
//                    console.log(vm.category_root, '....root...');
                    // this.getPortList();
                })
            },

            clearUp() {
                this.controllerSn = '';
                this.sensorSn = '';
                this.motorSn = '';
                this.isControllerConnected = false;
                this.isMotorConnected = false;
                this.isSensorConnected = false;
            },

            saveBinding() {
                if (!this.motorSn) {
                    this.$message({
                        message: '请输入电机序列号',
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.controllerSn || !this.sensorSn){
                    this.$message({
                        message: '请输入控制器或者传感器序列号',
                        type: 'warning'
                    });
                    return false;
                }

                Core.Api.Binding.findByOrderIdCount(this.orderInfo.source_id).then(res=>{
                    this.num= res.count;
//                    console.log(this.num);
//                    console.log(this.orderInfo.amount);
//                    console.log(this.num ==this.orderInfo.amount);
//                    console.log("+++++++++++++++++++++++++++++++++++++++++");
                    if (this.num==this.orderInfo.amount||this.num>this.orderInfo.amount) {
                        this.$message({
                            message: 'The number of bindings is the same as the number of orders, 绑定数量满足订单数量了',
                            type: 'warning'
                        });
                        return false;
                    }else{
                        this.doSaveBinding();
                    }
                }) ;
            },
            doSaveBinding() {
                if (this.isControllerConnected&&this.isSensorConnected&&this.isMotorConnected){
                    Core.Api.Binding.saveBinding(this.controllerSn, this.sensorSn, this.motorSn,this.orderInfo.source_id).then(res => {
                        this.controllerSn = '';
                        this.sensorSn = '';
                        this.motorSn = '';
                        this.isControllerConnected = false;
                        this.isMotorConnected = false;
                        this.isSensorConnected = false;
                        this.$message({
                            message: "Save Successfully",
                            type: 'success'
                        });

                    });
                } else if (!this.isControllerConnected&&!this.isSensorConnected&&this.isMotorConnected){
                    Core.Api.Binding.updateBinding(this.controllerSn, this.sensorSn, this.motorSn,this.bindId).then(res => {
                        this.controllerSn = '';
                        this.sensorSn = '';
                        this.motorSn = '';
                        this.isControllerConnected = false;
                        this.isMotorConnected = false;
                        this.isSensorConnected = false;
                        this.$message({
                            message: "Save Successfully",
                            type: 'success'
                        });

                    });
                }else if (this.isControllerConnected&&this.isSensorConnected){
                    Core.Api.Binding.saveBinding(this.controllerSn, this.sensorSn, this.motorSn,this.orderInfo.source_id).then(res => {
                        this.controllerSn = '';
                        this.sensorSn = '';
                        this.motorSn = '';
                        this.isControllerConnected = false;
                        this.isMotorConnected = false;
                        this.isSensorConnected = false;
                        this.$message({
                            message: "Save Successfully",
                            type: 'success'
                        });

                    });
                }else{
                    this.$message({
                        message: "Do not save,check values",
                        type: 'warning'
                    });
                }
            },
            removeBindingConnect() {
                Core.Api.Binding.deleteBinding(this.bindId).then(res => {
                    this.$message({
                        message: "Save Successfully",
                        type: 'success'
                    });
                    this.controllerSn = '';
                    this.sensorSn = '';
                    this.motorSn = '';
                    this.isMotorConnected = false;
                    this.isControllerConnected=false;
                    this.isSensorConnected = false;
                })
            },
            findBindingRecordList(){
                this.dialogBindingRecordVisible = true;
                Core.Api.Binding.findRecordList(this.orderInfo.source_id,"","","",this.currentPage,10).then(res => {
                    this.bindingRecordList = res.list;
                    this.total = res.count;
                })
            },
            handleCurrentChange(){
                this.findBindingRecordList();
            },
            readBase() {
                this.readController();
                this.readSensor();
            },
            readController() {
                this.clearParams();
                Update.port.getUsbBasic(0x02).subscribe(res => {
                    if (!res.data) {
                        return;
                    }
                    if (res.data.hasOwnProperty("sn")) {
                        let sn = res.data.sn;
                        let snLength = sn.length;
                        this.controllerSn = sn;
                        this.searchSpecialSn(this.controllerSn, "ctl")
                        if(snLength !== this.snLength) {
                            this.checkSnLengthMsg();
                            this.canNext = false;
                        }else{
                            this.canNext = true;
                        }
                    }
                    if (res.data.hasOwnProperty("model")) {
                        this.model = res.data.model;
                        if (this.model != this.itemPass.item_name) {
                            this.errorList.push('model');
                        }
                    }
                    if (res.data.hasOwnProperty("version")) {
                        this.version = res.data.version;
                        if (this.version != this.ctrlAttrValues['soft_ver']) {
                            this.errorList.push('soft_ver');
                        }
                    }
                    if (res.data.hasOwnProperty("hardware_version")) {
                        this.hardVersion = res.data.hardware_version;
                        if (this.version != this.ctrlAttrValues['hard_ver']) {
                            this.errorList.push('hard_ver');
                        }
                    }
                    if (res.data.hasOwnProperty("client_code")) {
                        this.clientCode = res.data.client_code;
                    }
                });
                this.clickReadParam();
            },
            readSensor() {
                Update.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x01)
                    .then(res => {
//                        console.log(res,"serialNumber");
                        this.sensorSn = res.dataParsed;
                        this.searchSpecialSn(this.sensorSn, "sen")
                    }).catch(err => {
                    console.log(err);
                    if (err == 'Timeout' || err == 'INVALID_LENGTH') {
                        Update.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x01)
                            .then(res => {
                                this.sensorSn = res.dataParsed;
                                this.searchSpecialSn(this.sensorSn, "sen")

                            }).catch(err => {
                            console.log(err);
                        }).finally(() => {
//                            console.log('serialNumber 2');
                        });
                    }
                }).finally(() => {
//                    console.log('serialNumber 1');
                });
            },
            changeSnLength() {
                this.snLengthCheck = true;
                this.getDeviceList();
            },
            getDeviceList() {
                let lengthCheck = this.checkModelName(this.itemPass.item_name);
                let itemNameLength = this.itemPass.item_name.replace(/\s+/g, "").length;
                let temLength;

                if (this.itemRoot === Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER) {
                    if (this.isCan) {
                        if (lengthCheck.dot === 4 && lengthCheck.space === 4) {
                            temLength = itemNameLength + 10;
                        } else {
                            temLength = 28;
                        }
                    } else {
                        if (lengthCheck.dot === 5 && lengthCheck.space === 4) {
                            temLength = itemNameLength + 10;
                        } else {
                            temLength = 31;
                        }
                    }
                    if (this.snLengthCheck && this.snLength !== temLength) {
                        this.checkModelNameMsg();
                    } else {
                        this.snLength = temLength;
                    }
                }
            },

            checkModelName(modelName) {
                if(!modelName) return;
                let dotLength = modelName.split(".").length;
                let spaceLength = modelName.split(" ").length;
                return {"dot":dotLength,"space":spaceLength}
            },

            checkSnLengthMsg() {
                this.$message({
                    message: 'please check sn length read，请检查读取的长度',
                    type: 'warning',
                    duration: 2000,
                    showClose: true
                });
            },

            next() {
                let sn = this.sn.replace(/\s/g, "");
                if(sn.length !== this.snLength) {
                    this.checkSnLengthMsg();
                    return;
                }
                if (this.itemRoot === Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER) {
                    if (this.checkController()) {
                        this.saveComponent();
                    } else {
                        this.dialogTipVisible = true;
                    }
                }
            },

            clearParams() {
                this.sn = '';
                this.model = '';
                this.version = '';
                this.nmlVolt = '';
                this.limitCurrent = '';
                this.pasSignals = '';
                this.hardVersion = '';
                this.serviceMileage = '';
                this.clientCode = '';
                this.testJson = [];
            },
            getTestJson() {
                let params = {};
                params['soft_ver'] = this.version.trim();
                params['model'] = this.model.trim();
                if(this.usbHID){
                    params['hard_ver'] = this.hardVersion.trim();
                    switch (this.itemRoot) {
                        case Core.Const.CATEGORY_ROOT_TYPE.HMI:
                            params['client_code'] = this.clientCode.trim();
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.SENSOR:
                            params['client_code'] = this.clientCode.trim();
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.TOOL:
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER:
                            params['wheel_size'] = this.wheelDiameter;
                            params['speed_limit'] = this.speedLimit;
                            params['circumference'] = this.circumference;
//                            params['voltage'] = this.nmlVolt;
//                            params['current_limit'] = this.limitCurrent;
                            params['client_code'] = this.clientCode.trim();
                            // console.log(params);
                            break;
                    }
                }else{
                    switch (this.itemRoot) {
                        case Core.Const.CATEGORY_ROOT_TYPE.HMI:
                            params['wheel_size'] = this.wheelDiameter;
                            params['speed_limit'] = this.speedLimit;
                            params['hard_ver'] = this.hardVersion.trim();
                            params['client_code'] = this.clientCode.trim();
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.SENSOR:
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.TOOL:
                            params['hard_ver'] = this.hardVersion.trim();
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER:
                            params['voltage'] = this.nmlVolt;
                            params['current_limit'] = this.limitCurrent;
                            params['hard_ver'] = this.hardVersion.trim();
                            // console.log(params);
                            break;
                    }
                }

                if (params && params != '') {
                    return JSON.stringify(params);
                }

                return '';
            },

            saveComponent() {
                if (this.isSaving) {
                    return;
                }
                let sn = this.sn.replace(/\s/g, "");

                if (!this.sn) {
                    this.$message({
                        message: "Please input sn, 请输入SN",
                        type: 'warning',
                        duration: 2000,
                        showClose: true
                    });
                    return;
                }else{
                    let name = this.itemPass.item_name.replace(/\s/g, "");
                    if(name.substr(0,6).toUpperCase() !== sn.substr(0,6).toUpperCase()){
                        this.$message({
                            message: "SN 内容和型号不匹配，请确认",
                            type: 'warning',
                            duration: 2000,
                            showClose: true
                        });
                        return;
                    }
                }

                this.isSaving = true;
                let id = '';
                let orderId = this.orderId;
                let qc = '';
                let testData = this.getTestJson();
                let productTime = Core.Util.time();
                let itemId = this.itemPass.item_id;
                Core.Api.Component.saveComponent(id, orderId, sn, qc, testData, productTime, itemId).then(res => {
                    if (this.itemRoot === Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER) {
                        this.canNext = false;
                    }
                    this.clearParams();
                    this.$message({
                        message: this.$t('cNam.saveSuccess'),
                        type: 'success',
                        duration: 2000,
                        showClose: true
                    });
                    this.dialogTipVisible = false;
                    this.isSaving = false;
                }).catch(res => {
                    this.isSaving = false;
                });
            },


            sensorSetStart() {
                this.sensorStart = false;
                Controller.port.usbSDK.machineInformation.update.positionSensorCalibration(0x05,0x02,'')
                    .then(res =>{
                    }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                    console.log('positionSensorCalibration 1');
                });
                Controller.port.getListenData().subscribe((res) => {
                    if (!res.data) {
                        return;
                    }
                    if (res.type == 'error_usb' ) {
                        this.isConnected = false;
                    }
                    if (res.data.code == '12' && res.data.subCode == '00') {
//                        console.log("res.data",res.data);
//                        can: "LONG_WARNING_CMD"
//                        canCode: 7
//                        code: "12"
//                        data: "00"
//                        dataLen: 1
//                        source: "CTL"
//                        sourceCode: 2
//                        subCode: "00"
//                        target: null
//                        targetCode: 15
                        if( res.data.data == '08' ){
                            this.sensorStart = true;
//                            this.$message({
//                                message: "正在标定",
//                                type: 'info'
//                            });
                        }else if( res.data.data == '00' ){
                            if(this.sensorStart){
                                this.$message({
                                    message: this.$t('cNam.success'),
                                    type: 'success'
                                });
                                this.sensorStart = false;
                            }
                        }
                    }
                });
            },
            sensorZeroSetStart() {
                this.sensorZeroStart = false;
                Controller.port.usbSDK.machineInformation.update.zeroPointSensorCalibration(0x05,0x02,'')
                    .then(res =>{
                        this.sensorZeroStart = true;
                        if (res.code == '61' && res.subCode == '01' && res.targetCode == 5 ) {
                            this.sensorZeroStart = false;

                            if( res.can === "ERROR_ACK" ){
                                this.$message({
                                    message: this.$t('cNam.operationFail'),
                                    type: 'error'
                                });
                            }else if( res.can === "NORMAL_ACK" ){
                                this.$message({
                                    message: this.$t('cNam.success'),
                                    type: 'success'
                                });
                            }
                        }

                    }).catch(err =>{
                    console.log(err);

                    if (err == 'Timeout'){
//                        this.$message({
//                            message: "回复超时，请检查",
//                            type: 'info'
//                        });
                        Controller.port.usbSDK.machineInformation.update.zeroPointSensorCalibration(0x05,0x02,'')
                            .then(res =>{
                                this.sensorZeroStart = true;
                                if (res.code == '61' && res.subCode == '01' && res.targetCode == 5 ) {
                                    this.sensorZeroStart = false;

                                    if( res.can === "ERROR_ACK" ){
                                        this.$message({
                                            message: this.$t('cNam.operationFail'),
                                            type: 'error'
                                        });
                                    }else if( res.can === "NORMAL_ACK" ){
                                        this.$message({
                                            message: this.$t('cNam.success'),
                                            type: 'success'
                                        });
                                    }
                                }
                            }).catch(err => {
                            console.log(err);
                        }).finally(()=>{
                            console.log('zeroPointSensorCalibration 2');
                        });
                    }

                }).finally(()=>{
                    console.log('zeroPointSensorCalibration 1');
                });

            },

            checkConnection() {
                let vm = this;
                if(vm.connectionInterval){
                    clearInterval(vm.connectionInterval);
                    vm.connectionInterval = '';
                }
                vm.connectionInterval = setInterval(()=>{
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
                        vm.connect();
                        clearInterval(vm.connectionInterval);
                    }
                },300)
            },

            init(){
                Update.port.startConnection();
                if(Update.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
//                    Update.port.setUSBDevice(true);
                    console.log(" getUSBDevice  update");
                    if(this.isReset){
                        this.connect();
                    }else{
                        this.connect(1);
                    }
                }else{
                    console.log("COM Update");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = Update.port.isConnected();
            },
            getComList() {

                this.device.port.getPortList().subscribe(res => {
                    if (res.type === 'port') {
                        if (res.data) {
                            this.comList = res.data;
                            this.com = this.comList[0];
                            this.connect();
                        } else {
                            this.$message({
                                message: 'no com connect',
                                type: 'warning'
                            });
                        }
                    }
                });
            },

            connect(type) {
                this.checkBoxUpdate();

                if( this.usbHID ){
                    if(type === 1){
                        this.test();
                    }else{
                        setTimeout(()=>{
                            this.test();
                        },1000)
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
            },

            test(){
                Update.port.checkUsbConnection('0101',1);
            },

            onChangeUpdateFile() {
                console.log(this.$refs.fileUpdate);
                if (this.$refs.fileUpdate.files.length>0) {
                    this.file = this.$refs.fileUpdate.files[0];
                    console.log(this.file);
                    this.fileBufferLength = this.file.size;
//                    console.log(this.fileBufferLength);
                    this.fileName = this.file.name;
                } else {
                    this.file = null;
                    this.fileName = '';
                }
            },

            reset() {
                this.percentage = 0;
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                if( this.usbHID ){
                    this.startTip = false;
                    this.updateMsg = '';
                    this.boxPercentage = 0;
                    this.updateOnline = false;
                    Update.port.reset(300);
                    this.isConnected = false;
                }else{
                    if( this.com ){
                        Update.connect(this.com, 57600);
                        Update.port.reset(300);
                        setTimeout(()=>{
                            this.isConnected = Update.port.isConnected();
                        },400);
                    }
                }
            },

            // 获取固件 by sn
            getFirmware() {
//                Core.Api.Firmware.getHistory(this.itemId)
//                    .then(res => {
//                        this.HWRecordList = res.record;
//                        this.dialogRecordListVisible = true;
//                    })
                Core.Api.Firmware.getHistory(this.itemInfo.id).then(res => {
                    /*create_time: 1524291381
                     file: "0f0c813df4f10736234a7019002f3bbe18e16f9ae8b9785c5befa030b79aee55.bin"
                     firmware_name: ""
                     id: 6
                     item_id: 1005
                     operator_id: 24
                     operator_type: 4
                     remark: ""
                     update_time: 1524291381
                     version: "CRS10F4313E101013.0"
                     weight: 0
                     * */
                    this.HWRecordList = res.record;
                    this.dialogRecordListVisible = true;
                });
            },
            checkBoxUpdate(){
                let count = 1;
                let vm = this;
                let res = Update.port.usbSDK.client.events.on('firmware-update-completed-progress',(e)=>{
//                    console.log(e); // {progress: 0, status: "01"}
//                    console.log(e.progress,vm.fileBufferLength);
                    if(vm.category_root){
                        switch (vm.category_root) {
                            case 4:
                                vm.updateMsg = vm.updateMsgObject.msg4;
                                break;
                            case 7:
                                vm.updateMsg = vm.updateMsgObject.msg5;
                                break;
                            case 10:
                                vm.updateMsg = vm.updateMsgObject.msg6;
                                break;
                        }
                    }else{
                        vm.updateMsg = vm.updateMsgObject.msg10;
                    }
//                    console.log("[true]",vm.isCanSoft, vm.isCan);
//                    console.log("[true]",vm.isCanSoft || vm.isCan);
                    if(vm.isCanSoft || vm.isCan){
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
            // 在线更新
            updateObject(item) {
                let url = Core.Const.NET.FILE_URL_PREFIX + item.file;
                let $this = this;
                $this.firmwareVersion = item.version;
                $this.fileName = item.firmware_name;
                console.log($this.fileName, 'get fileName');

                this.$http.get(url, {responseType: 'blob'})
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
                            message: this.$t('cNam.updateGetFileSucc'),
                            type: 'warning'
                        });
                        var fileReader = new FileReader();
                        fileReader.onload = function () {
                            let data = {
                                fileBuffer: this.result,
                                name: item.version,
                                type: 'data'
                            };

                            let arr = Buffer.from(this.result);
                            if($this.firmCheck(arr)){
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
                                duration: 0,
                                showClose: true
                            });
                        }
                    });
                this.updateOnline = true;
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
                        if( e.upload == e.total || vm.percentage == 100){
                            if(count == 0){
                                vm.$message({
                                    message: vm.updateMsgObject.successTip,
                                    type: 'success'
                                });
                                vm.updateOnline = false;
                                if(vm.category_root){
                                    switch(vm.category_root){
                                        case 4:
                                            vm.updateMsg = vm.updateMsgObject.msg1;
                                            break;
                                        case 7:
                                            vm.updateMsg = vm.updateMsgObject.msg2;
                                            break;
                                        case 10:
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
                    });
                    if(vm.updateOnline) {
                        Update.port.usbSDK.firmware.update.firmware(data.fileBuffer);
                    }else{
                        let reader = new FileReader();
                        reader.readAsArrayBuffer(vm.file);
                        reader.onload  = function(e){
//                            console.log(e.target.result);
                            let arr = Buffer.from(e.target.result);
                            if(vm.firmCheck(arr)){
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
                                duration: 0,
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
                                        type: 'success'
                                    });
                                    vm.percentage = 0;
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

            startUpdate() {
                this.beforeUpdate = true;
                setTimeout(()=>{
                    this.beforeUpdate = false;
                },2000);
                console.log(this.fileName, 'before');
                if( this.fileName == ''){
                    this.$message({
                        message: this.$t('cNam.updateSelectFile'),
                        type: 'warning'
                    });
                    return;
                }
                this.doUpdate(this.file);
            },

            // 控制器
            initTrans(){
                for (let i = 10; i <= 60; i++) {
                    this.speedLimits.push(i);
                }
                // 经销商离线10-25
                if (this.isDealerOrAssembler && this.offLineFlag) {
                    this.speedLimits = [];
                    for (let i = 10; i <= 25; i++) {
                        this.speedLimits.push(i);
                    }
//                this.wheelDiameters.pop();
                }
                this.speedLimit = 25;

                this.lanTrans[0] = this.$t('cNam.assistOff');
                this.lanTrans[1] = this.$t('cNam.assistOn');
            },
            usbErrorCheck(err){
                Controller.port.checkUsbError(err);
            },
            writeSnController() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                if(!this.usbHID){
                    Controller.connect(this.com);
                }
                if (!this.controllerSn) {
                    this.$message({
                            message: this.$t('cNam.snEmptyTip'),
                            type: "warning"
                        }
                    );
                    return;
                }
                this.controllerSn = this.controllerSn.replace(/\s/g, "");
                if(this.controllerSn.length > 40){
                    this.$message({
                            message: this.$t('cNam.snMaxLength'),
                            type: "warning"
                        }
                    );
                    return;
                }

                this.writeSnWithAuth();
            },

            writeSnWithAuth(){
//                this.writeStatus = true;
                this.connect();
                if(!this.isCan || Controller.port.checkUsbUart()){
                    Controller.writeSn(this.controllerSn);
                    Controller.getBus().subscribe(res => {
//                    console.log('Controller-vm3:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (res.data.ctrl && 'sn' in res.data.ctrl) {
                            let sn = res.data.ctrl.sn;
                            if (sn === this.controllerSn || sn === ' ') {
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                            } else {
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }
//                            this.writeStatus = false;
                        }
                    });
                } else {
                    Controller.port.usbSDK.machineInformation.update.serialNumber(0x05, 0x02, Controller.port.Core.Util.stringToHex(this.controllerSn))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.controllerSn = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                this.readController();
                            }else{
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }

                        }).catch(err =>{
                        console.log(err);
                    }).finally(()=>{
//                            this.writeStatus = false;
                    });
                }

            },

            updateWheelCircumference(){

                let wheelSize;

                this.wheelDiameters.find((item) => {
                    if (item.value == this.wheelDiameter) {
                        wheelSize = item.label;
                    }
                });

                circumferenceObject.init().forEach((item)=>{
                    if( item.inch === wheelSize){
                        this.circumferences = item.circumference;
//                    console.log("[circumferences]", this.circumferences);
                    }
                })
//                console.log(this.wheelDiameter);
            },
            clickReadParam() {
                this.speedLimitRead = '';
                this.wheelDiameterRead = '';
                this.circumferenceRead = '';

                Controller.port.getListenData().subscribe((res) => {
                    if (!res.data) {
                        return;
                    }
                    if( res.type == 'error_usb' ) {
                        this.isConnected = false;
                    }
                    if (res.data.code === '32' && res.data.subCode === '03' && res.data.targetCode == 15) {
                        //  console.log(res.data);
                        //  can:"WRITE_CMD"canCode:0code:"32"data:"a406d0002c090000"dataLen:6source:"CTL"sourceCode:2subCode:"03"target:nulltargetCode:15
                        let result_3 = Buffer.from(res.data.data, 'hex');
                        if(result_3.length >= 6){
                            this.speedLimitRead = (result_3[0] + result_3[1] * 256)/100;
                            this.wheelDiameters.find((item) => {
                                if (item.value == res.data.data.substr(4,4).toUpperCase()) {
                                    this.wheelDiameterRead = item.label;
                                }
                            });
                            this.circumferenceRead = result_3[4] + result_3[5] * 256;
                        }
                    }
                });
            },
            clickWriteParam() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.writeStatus = true;
                if(!this.usbHID){
                    Controller.connect(this.com);
                }


                let wheelDiameterParse;

                if( this.wheelDiameter != 'FFFF' ){

                    let circumferenceMin;
                    let circumferenceMax;

                    this.wheelDiameters.find((item) => {
                        if (item.value == this.wheelDiameter) {
                            circumferenceMin = parseInt(item.min);
                            circumferenceMax = parseInt(item.max);
                            wheelDiameterParse = item.label;
                        }
                    });

                    if( parseInt(this.circumference) < circumferenceMin || parseInt(this.circumference) > circumferenceMax){
                        this.$message({
                            message: this.$t('cNam.whileSizeRange') + wheelDiameterParse + ' '+ circumferenceMin + '-' + circumferenceMax,
                            type: "warning"
                        });
                        return;
                    }
                }
                this.writeSpeed();
            },

            writeSpeed() {

                let spd = parseInt(this.speedLimit)*100;
                let cir = parseInt(this.circumference);
                let spdStr =  Buffer.from( [(spd>>8)&0xff, spd&0xff]).reverse().toString('hex');
                let cirBuf =  Buffer.from( [(cir>>8)&0xff, cir&0xff]).reverse().toString('hex');
//                Port.writeByQueueAndWait(makeFrameCanWrite('00','3203', spdStr + wheel +cirBuf), 100);

                let tem = this.wheelDiameter;

                if( typeof tem == "string" && tem.length !=4){
                    this.wheelDiameters.find((item) => {
                        if (item.label == tem) {
                            this.wheelDiameter = item.value;
                        }
                    });
                }

                console.log(spdStr, this.wheelDiameter, cirBuf);


                Controller.port.usbSDK.publicInformation.update.controllerInformation_3(0x05, 0x02, spdStr + this.wheelDiameter + cirBuf)
                    .then(res => {
//                        console.log(res);
//                        can:"NORMAL_ACK"canCode:2code:"32"data:""dataPacks:{}dataParsed:""source:"CTL"sourceCode:2subCode:"03"target:"BST"targetCode:5
                        if (res.can == "NORMAL_ACK" && res.targetCode == 5){
                            this.speedLimitRead = '';
                            this.wheelDiameterRead = '';
                            this.circumferenceRead = '';
                            this.clickReadParam();
                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                        }else if(res.can == "ERROR_ACK" && res.targetCode == 5){
                            this.$message({
                                message: this.$t('cNam.writeFail'),
                                type: 'error'
                            });
                        }
                    }).catch(err =>{
                    console.log(err);
                    this.usbErrorCheck(err);
                }).finally(()=>{
                    console.log("write speed finally.");
//                    setTimeout( () =>{
//                        this.writeStatus = false;
//                    },3000);
                });
//                console.log( this.speedLimit, this.wheelDiameter, this.circumference );
//                Controller.getBus().subscribe(res => {
//                    if (res.type === 'close' || res.type === 'error') {
//                        this.isConnected = false;
//                    }
//                    if (!res.data || !res.data.ctrl) {
//                        return;
//                    }
//                    if (res.data.ctrl.hasOwnProperty("write_speed")) {
//                        this.$message('receive response , write success');
//                    }
//                });
            },

            writeClientCode(){
                if(!this.usbHID ) return;
                if(!this.clientCode ) return;
                this.clientCode = this.clientCode.replace(/\s/g, "");
                if (this.clientCode.length < 25) {
                    Controller.port.usbSDK.machineInformation.update.customerNumber(0x05, 0x02, Controller.port.Core.Util.stringToHex(this.clientCode))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.clientCode = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Controller.port.usbSDK.machineInformation.get.customerNumber(0x05, 0x02)
                                    .then(res =>{
                                        this.clientCode = res.dataParsed;
                                        this.clientCodeRead = this.clientCode;
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
                    });
                } else {
                    this.$message(this.$t('cNam.customerMaxLength'));
                }
            },
            readManufacturer() {
                this.manName = '';
                Controller.port.usbSDK.machineInformation.get.manufacturer(0x05,0x02)
                    .then(res=>{
                        this.manName = res.dataParsed.replace(/\s+/g, "");
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
                Controller.port.usbSDK.machineInformation.update.manufacturer(0x05,0x02,Controller.port.Core.Util.stringToHex(this.manName.trim()))
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
            readBootLoad() {
                this.bootLoad = '';
                Controller.port.usbSDK.machineInformation.get.bootLoad(0x05,0x02)
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

            // 软件程序前16个字符检查
            firmCheck( arr ) {
//                console.log(arr);

                let BESST_Pro = Update.port.getBESST_Pro();
                let BESST_update = false;
                if( BESST_Pro.soft_ver && BESST_Pro.soft_ver.indexOf('BSF33.0')>-1 && BESST_Pro.soft_ver > "BSF33.03" ){
                    BESST_update = true
                }

                let show = false;
                if(arr[2]>128 && arr[2]<136){
                    this.isCanSoft = true;
                }else if(arr[2]>0 && arr[2]<8){
                    this.isCanSoft = false;
                }else if(arr[2] !== 0xFC){
                    show = true;
                }
                let res = false;
                if(this.isCanSoft){
                    let sec = false;
                    for(let i=18; i<32; i++ ){
                        if(arr[i]!== 0){
                            sec = true;
                        }else{
                            sec = false;
                        }
                    }
                    if(!sec){
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
                        for(let i=4; i<16; i++ ){
                            if(arr[i]!== 0){
                                res = true;
                            }
                        }
                    }
                }else{
                    for(let i=4; i<16; i++ ){
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

            searchSpecialSn(val, type) {
                if(!val){
                    this.$message({
                        message: type +" " + "SN没有数据。请检查",
                        type: 'warning'
                    });
                    return;
                }

                let vm = this;
                if(type == "motor"){
                    console.log("这里是motorSn 查询口："+val);
                    if (vm.isMotorConnected==false) {
                        Core.Api.Binding.findBySpecialSn("","",val).then(res=>{
                            let models = res.object_binding;
                            if (models.controller_sn==undefined&&models.sensor_sn==undefined&&models.motor_sn==undefined){
                                vm.isMotorConnected=true;//定义为1时可以进行绑定
                            }else{
                                vm.controllerSn=models.controller_sn;
                                //vm.motorSn=models.motorSn;
                                vm.sensorSn =models.sensor_sn;
                                vm.isMotorConnected=false;//定义为2时不可进行绑定
                                vm.bindId = models.id;
                            }
                        })
                    }else{
                        vm.motorSn=val;
                    }
                }else if(type == "ctl"){
                    console.log("这里是controllerSn 查询口："+val);
                    if (vm.isControllerConnected==false) {
                        Core.Api.Binding.findBySpecialSn(val, "", "").then(res => {
                            let models = res.object_binding;
                            if (models.controller_sn === undefined && models.sensor_sn === undefined && models.motor_sn === undefined) {
                                vm.isControllerConnected =true;//定义为1时可以进行绑定
                            } else {
                                //vm.controllerSn = models.controllerSn;
                                vm.motorSn = models.motor_sn;
                                vm.sensorSn = models.sensor_sn;
                                vm.isControllerConnected = false;//定义为2时不可进行绑定
                                vm.bindId = models.id;
                            }
                        })
                    }else{
                        vm.controllerSn=val;
                    }
                }else if(type == "sen"){
                    console.log("这里是sensorSn 查询口："+val);
                    if (vm.isSensorConnected==false) {
                        Core.Api.Binding.findBySpecialSn("", val, "").then(res => {
                            let models = res.object_binding;
                            if (models.controller_sn == undefined && models.sensor_sn == undefined && models.motor_sn == undefined) {
                                vm.isSensorConnected = true;//定义为1时可以进行绑定
                            } else {
                                vm.controllerSn = models.controller_sn;
                                vm.motorSn = models.motor_sn;
                                // vm.sensorSn = models.sensorSn;
                                vm.isSensorConnected = false;//定义为2时不可进行绑定
                                vm.bindId = models.id;
                            }
                        })
                    }else{
                        vm.sensorSn=val;
                    }
                }
            }

        },
        watch: {
            'mainSn':{
                handler: function (newValue, oldValue) {
                    let vm = this;
                    if (newValue === oldValue || newValue === '') {
                        return;
                    }
                    if (typeof oldValue !== 'undefined') {
                        let time = new Date().getTime();
                        vm.isScanner = time - vm.lastInputTime <= 100; // 间隔小于50ms的就认为是扫描枪输入
                        vm.lastInputTime = time;
                    } else {
                        vm.lastInputTime = new Date().getTime();
                    }
                    if (!vm.scannerTimeout) {
                        vm.scannerTimeout = setTimeout(function () {
                            if (vm.isScanner) {
                                let value = vm.mainSn.replace(/\s+/g, ""); // 去空格
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                                if (value.indexOf('MM') > -1) {
                                    console.log("22222222222222222222222"+vm.isMotorConnected);
                                    vm.motorSn = value;
                                    vm.mainSn = '';
                                } else if (value.indexOf('CR') > -1) {
                                    console.log("111111111111111111111111"+vm.isControllerConnected);
                                    vm.controllerSn = value;
                                    vm.mainSn = '';
                                }  else if (value.indexOf('SR') > -1) {
                                    console.log("33333333333333333333333"+vm.isSensorConnected);
                                    vm.sensorSn = value;
                                    vm.mainSn = '';
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 1000)
                    }
                }
            },


            motorSn: function (newValue, oldValue) {
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
                            vm.motorSn = vm.motorSn.replace(/\s+/g, "");
                            let array = vm.motorSn.split('MM');
                            if (array.length > 1) {
                                vm.motorSn = ('MM' + array[array.length - 1]).replace(/\s+/g, "");
                            }
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }
                if (newValue!==oldValue){
                    vm.isMotorConnected=false;
                }
                vm.searchSpecialSn(vm.motorSn, "motor")
            },
            controllerSn:function(newValue, oldValue){
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
                            vm.controllerSn = vm.controllerSn.replace(/\s+/g, "");
                            let array = vm.controllerSn.split('CR');
                            if (array.length > 1) {
                                vm.controllerSn = ('CR' + array[array.length - 1]).replace(/\s+/g, "");
                            }
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }

                if (newValue!==oldValue){
                    vm.isControllerConnected=false;
                }
                vm.searchSpecialSn(vm.controllerSn, "ctl")
            },
            sensorSn:function(newValue, oldValue){
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
                            vm.sensorSn = vm.sensorSn.replace(/\s+/g, "");
                            let array = vm.sensorSn.split('SR');
                            if (array.length > 1) {
                                vm.sensorSn = ('SR' + array[array.length - 1]).replace(/\s+/g, "");
                            }
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }

                if (newValue!==oldValue){
                    vm.isSensorConnected=false;
                }
                vm.searchSpecialSn(vm.sensorSn, "sen")
            },
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .component-order-connect {
        width: 100%;
        height: 100%;
        .header {
            line-height: 60px;
            height: 60px;
            padding: 0 38px;
            border-bottom: 1px solid #2B2F3B;
            .label {
                margin-left: 25px;
                font-size: 16px;
            }
            .add-btn {
                margin: 20px 0;
                float: right;
                background-color: #858890;
            }
        }
        .content-card-item {
            .content-header {
                display: block;
                padding: 10px 43px;
                font-size: 14px;
                /*height: 50px;*/
                /*line-height: 30px;*/
                font-weight: bold;
                box-sizing: border-box;
            }
            .mark {
                cursor: pointer;
            }
            .content-body {
                font-size: 0;
                background: #353945;
                overflow: hidden;
                .item {
                    width: 50%;
                    display: inline-block;
                    padding-left: 60px;
                    line-height: 40px;
                    label {
                        display: inline-block;
                        width: 120px;
                        font-size: 14px;
                        color: #A1A5B1;
                    }
                    span {
                        font-size: 14px;
                    }
                }
                .hardware-item {
                    float: left;
                    padding: 10px 60px;
                    line-height: 40px;
                    overflow: hidden;
                    .select-holder {
                        width: 100%;
                        & > label {
                            display: inline-block;
                            width: 120px;
                            /*float: left;*/
                        }
                        .el-input{
                            /*float: left;*/
                            width: 295px;
                        }
                        .tip {
                            display: inline-block;
                            border-radius: 50%;
                            width: 15px;
                            height: 15px;
                            background: #E26829;
                            margin-left: 6px;
                        }
                        .connect {
                            background: #76EE00;
                        }
                    }
                    .file-upload-holder {
                        overflow: hidden;
                        & > label {
                            float: left;
                            display: inline-block;
                            width: 120px;
                        }
                        .upload-demo {
                            float: left;
                            overflow: hidden;
                            .el-upload {
                                float: left;
                            }
                            .el-upload-list {
                                float: left;
                                .el-upload-list__item {
                                    padding-left: 20px;
                                }
                            }
                            .el-upload-list__item:first-child {
                                margin-top: 13px;
                            }
                        }
                        .file-input {
                            width: calc(100% - 120px);
                        }
                    }
                    .input-holder {
                        overflow: hidden;
                        & > label {
                            float: left;
                            display: inline-block;
                            width: 120px;
                        }
                        .el-input {
                            float: left;
                            width: 295px;
                        }
                        .sn:after {
                            top: 10px;
                        }
                    }
                    .file {
                        position:relative;
                        .label {
                            display:inline-block;
                            height: 20px;
                            line-height: 20px;
                            text-align: center;
                            background-color: #E26829;
                            border-radius: 26px;
                        }
                        span{
                            display:inline-block;
                            margin: 15px 0 0 33px;
                            overflow: hidden;
                            text-overflow: ellipsis;
                            -o-text-overflow: ellipsis;
                            white-space:nowrap;
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
                    }
                }
                .operation-item {
                    float: right;
                    /*width: 300px;*/
                    padding: 10px 10px;
                    line-height: 40px;
                }
                .btn-item{
                    margin: 5px 60px;
                    height: 30px;
                    line-height: 30px;
                    .operation-holder {
                        margin-right: 4px;
                    }
                    .left{
                        float: left;
                    }
                    .right{
                        float: right;
                    }
                }
                .errorTip {
                    color: #E26829;
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// data-connect.vue?2834c4ea