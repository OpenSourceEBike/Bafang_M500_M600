<template>
    <div class="detail">
        <!-- 加载成功显示 -->
        <div class="detail-content">
            <label class="title">
                <div class="title-left" v-if="testSelect.No == 8">
                    <img class="icon" src="../../assets/images/icon/icon-hmi.png" />
                    <!-- <img  v-if="testSelect.No == 11" class="icon" src="../../assets/images/icon/icon-motor.png" /> -->
                    <span>{{ $t('cNam.hmi')}}</span>
                    <span v-if="tabToUpdate">{{ $t('cNam.update') }}</span>
                </div>
                <div class="title-left" v-if="testSelect.No == 9">
                    <img class="icon" src="../../assets/images/icon/icon-controller.png" />
                    <span>{{ $t('cNam.controller')}}</span>
                    <span v-if="tabToUpdate">{{ $t('cNam.update') }}</span>
                </div>
                <div class="title-left" v-if="testSelect.No == 10">
                    <img class="icon" src="../../assets/images/icon/icon-battery.png" />
                    <span>{{ $t('cNam.battery')}}</span>
                </div>
                <div class="title-left" v-if="testSelect.No == 11">
                    <img class="icon" src="../../assets/images/icon/icon-pedal.png" />
                    <span>{{ $t('pedalSensor')}}</span>
                    <span v-if="tabToUpdate">{{ $t('cNam.update') }}</span>
                </div>
                <div class="title-left" v-if="testSelect.No == 7">
                    <img class="icon" src="../../assets/images/icon/icon-motor.png" />
                    <span>{{ $t('cNam.motor')}}</span>
                    <span v-if="tabToUpdate">{{ $t('cNam.update') }}</span>
                </div>
                <div class="title-right" @click="closeItemDetail">
                    <i class="el-icon-close"></i>
                </div>
            </label>
            <div class="content">

                <!-- 数据 -->
                <template v-if="!tabToUpdate">
                    <div class="info">
                        <!-- hmi -->
                        <template v-if="testSelect.No == 8">
                            <div class="info-left">
                                <div class="item">
                                    <label class="label">SN</label>
                                    <el-input class="input" v-model="hmiInfo.sn"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.model') }}</label>
                                    <el-input class="input" v-model="hmiInfo.model"></el-input>
                                </div>
                            </div>
                            <div class="info-right">
                                <div class="item">
                                    <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                    <el-input class="input" v-model="hmiInfo.version" :class="{active: testSelect.update}"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                    <el-input class="input" v-model="hmiInfo.hardVersion"></el-input>
                                </div>
                                <!--<div class="item mt-48">-->
                                    <!--<label class="label">{{ $t('cNam.clientCode') }}</label>-->
                                    <!--<el-input class="input" v-model="hmiInfo.clientCodeRead"></el-input>-->
                                <!--</div>-->
                                <!-- <div class="item">
                                    <label class="label">{{ $t('cNam.totalMileage') }}</label>
                                    <el-input class="input" v-model="hmiInfo.totalMileage">
                                        <template slot="append">km</template>
                                    </el-input>
                                </div>
                                <div class="item" v-show = '!isCan'>
                                    <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                                    <el-input class="input" v-model="hmiInfo.wheelDiameter"></el-input>
                                </div>
                                <div class="item" v-show = '!isCan'>
                                    <label class="label">{{ $t('cNam.speedLimit') }}</label>
                                    <el-input class="input" v-model="hmiInfo.speedLimit">
                                        <template slot="append">km/h</template>
                                    </el-input>
                                </div> -->
                                <!--<div class="item">-->
                                    <!--<label class="label">{{ $t('cNam.totalGear') }}</label>-->
                                    <!--<el-input class="input" v-model="hmiInfo.totalGear"></el-input>-->
                                <!--</div>-->
                            </div>
                        </template>
                        <!-- controller -->
                        <template v-if="testSelect.No == 9">
                            <div class="info-left">
                                <div class="item">
                                    <label class="label">SN</label>
                                    <el-input class="input" v-model="controllerInfo.sn"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.model') }}</label>
                                    <el-input class="input" v-model="controllerInfo.model"></el-input>
                                </div>
                            </div>
                            <div class="info-right">
                                <div class="item">
                                    <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                    <el-input class="input" v-model="controllerInfo.version" :class="{active: testSelect.update}"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                    <el-input class="input" v-model="controllerInfo.hardVersion"></el-input>
                                </div>
                                <!--<div class="item">-->
                                    <!--<label class="label">{{ $t('cNam.manName') }}</label>-->
                                    <!--<el-input class="input" v-model="controllerInfo.manufacturer"></el-input>-->
                                <!--</div>-->
                                <!-- <div class="item" v-show= "!isCan">
                                    <label class="label">{{ $t('cNam.nmlVolt') }}</label>
                                    <el-input class="input" v-model="controllerInfo.nmlVolt">
                                        <template slot="append">V</template>
                                    </el-input>
                                </div>
                                <div class="item" v-show= "!isCan">
                                    <label class="label">{{ $t('cNam.maxCur') }}</label>
                                    <el-input class="input" v-model="controllerInfo.maxCurrent">
                                        <template slot="append">A</template>
                                    </el-input>
                                </div>
                                <div class="item" v-show = "isCan">
                                    <label class="label">{{ $t('cNam.speedLimit') }}</label>
                                    <el-input class="input" v-model="controllerInfo.speedLimit" disabled>
                                        <template slot="append">Km/h</template>
                                    </el-input>
                                </div>
                                <div class="item" v-show = "isCan">
                                    <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                                    <el-input class="input" v-model="controllerInfo.wheelDiameter" disabled></el-input>
                                </div>
                                <div class="item" v-show = "isCan">
                                    <label class="label">{{ $t('cNam.circumference') }}</label>
                                    <el-input class="input" v-model="controllerInfo.circumference" disabled>
                                        <template slot="append">mm</template>
                                    </el-input>
                                </div> -->
                            </div>
                        </template>
                        <!-- battery -->
                        <template v-if="testSelect.No == 10">
                            <div class="info-left">
                                <div class="item">
                                    <label class="label">SN</label>
                                    <el-input class="input" v-model="batteryInfo.sn"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.model') }}</label>
                                    <el-input class="input" v-model="batteryInfo.model"></el-input>
                                </div>
                                <!-- <div class="item">
                                    <label class="label">{{ $t('cNam.ASOC') }}</label>
                                    <el-input class="input" v-model="batteryInfo.soh">
                                        <template slot="append">%</template>
                                    </el-input>
                                </div> -->
                                <!-- <div class="item">
                                    <label class="label">{{ $t('cNam.packVoltage') }}</label>
                                    <el-input class="input" v-model="batteryInfo.packVoltage">
                                        <template slot="append">mV</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.fullChargeCapacity') }}</label>
                                    <el-input class="input" v-model="batteryInfo.fullChargerCapacity">
                                        <template slot="append">mAh</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.current') }}</label>
                                    <el-input class="input" v-model="batteryInfo.current">
                                        <template slot="append">mA</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.cell8_14') }}</label>
                                    <el-input class="input" v-model="batteryInfo.cell8to14">
                                        <template slot="append">mV</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.currentChargeInterval') }}</label>
                                    <el-input class="input" v-model="batteryInfo.lastCharge">
                                        <template slot="append">day</template>
                                    </el-input>
                                </div> -->
                            </div>
                            <div class="info-right">
                                <div class="item">
                                    <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                    <el-input class="input" v-model="batteryInfo.version" :class="{active: testSelect.update}"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                    <el-input class="input" v-model="batteryInfo.hardVersion"></el-input>
                                </div>
                                <!-- <div class="item">
                                    <label class="label">{{ $t('cNam.RSOC') }}</label>
                                    <el-input class="input" v-model="batteryInfo.soc">
                                        <template slot="append">%</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.capacityLeft') }}</label>
                                    <el-input class="input" v-model="batteryInfo.leftCapacity">
                                        <template slot="append">mAh</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.temperature') }}</label>
                                    <el-input class="input" v-model="batteryInfo.temperature">
                                        <template slot="append"> ℃</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.cell1_7') }}</label>
                                    <el-input class="input" v-model="batteryInfo.cell1to7">
                                        <template slot="append">mV</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.cycleLife') }}</label>
                                    <el-input class="input" v-model="batteryInfo.loopTime"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.maxChargeInterval') }}</label>
                                    <el-input class="input" v-model="batteryInfo.maxChargeInterval">
                                        <template slot="append">day</template>
                                    </el-input>
                                </div> -->
                            </div>
                        </template>
                        <!-- pedal -->
                        <template v-if="testSelect.No == 11">
                            <div class="info-left">
                                <div class="item">
                                    <label class="label">SN</label>
                                    <el-input class="input" v-model="pedalSensorInfo.sn"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                    <el-input class="input" v-model="pedalSensorInfo.version" :class="{active: testSelect.update}"></el-input>
                                </div>
                            </div>
                            <div class="info-right">
                                <div class="item">
                                    <label class="label">{{ $t('cNam.model') }}</label>
                                    <el-input class="input" v-model="pedalSensorInfo.name"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                    <el-input class="input" v-model="pedalSensorInfo.hardVersion"></el-input>
                                </div>
                            </div>
                        </template>
                        <!-- motor -->
                        <template v-if="testSelect.No == 7">
                            <div class="info-left">
                                <div class="item">
                                    <label class="label">SN</label>
                                    <el-input class="input" v-model="motorInfo.sn"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.power') }}</label>
                                    <el-input class="input" v-model="motorInfo.power">
                                        <template slot="append">W</template>
                                    </el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.voltage') }}</label>
                                    <el-input class="input" v-model="motorInfo.voltage">
                                        <template slot="append">V</template>
                                    </el-input>
                                </div>
                            </div>
                            <div class="info-right">
                                <div class="item">
                                    <label class="label">{{ $t('cNam.model') }}</label>
                                    <el-input class="input" v-model="motorInfo.model"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.wheelSize') }}</label>
                                    <el-input class="input" v-model="motorInfo.wheelSize"></el-input>
                                </div>
                                <div class="item">
                                    <label class="label">{{ $t('cNam.rpm') }}</label>
                                    <el-input class="input" v-model="motorInfo.rpm"></el-input>
                                </div>
                            </div>
                        </template>
                    </div>
                    <div class="message">
                        <div v-if = "testSelect.No == 8 && hmiInfo.tip">
                            {{ $t('cNam.message') }}: <span>{{hmiInfo.tip}}</span>
                        </div>
                        <div v-else-if = "testSelect.No == 9 && controllerInfo.tip">
                            {{ $t('cNam.message') }}: <span>{{controllerInfo.tip}}</span>
                        </div>
                        <div v-else-if = "testSelect.No == 10 && batteryInfo.tip">
                            {{ $t('cNam.message') }}: <span>{{batteryInfo.tip}}</span>
                        </div>
                        <div v-else-if = "testSelect.No == 11 && pedalSensorInfo.tip">
                            {{ $t('cNam.message') }}: <span>{{pedalSensorInfo.tip}}</span>
                        </div>
                        <div v-else = "motorInfo.tip">
                            {{ $t('cNam.message') }}: <span>{{motorInfo.tip}}</span>
                        </div>
                    </div>
                    <!-- 没有update要内容填充 battery内容已够长不需要 -->
                    <template v-if="!testSelect.update">
                        <!--<div class="clear-100" v-if="!(testSelect.No == 10)"></div>-->
                    </template>

                    <!-- 有new version点击显示 -->
                    <template v-if="testSelect.update">
                        <div class="bottom-title">
                            <span>{{ $t('updateHistory') }}</span>
                        </div>
                        <div class="bottom-content">
                            <el-table :data="tableData" style="width:100%" empty-text="No Data">
                                <el-table-column prop="version" align="center" label="Version">
                                </el-table-column>
                                <el-table-column align="center" label="Update Time">
                                    <template slot-scope="scope">
                                        <!-- 1 未更新版本 2 已更新版本 -->
                                        {{scope.row.create_time | dateFormat}}
                                    </template>
                                </el-table-column>
                                <el-table-column prop="remark" align="center" label="Remark">

                                </el-table-column>
                                <el-table-column align="center" label="Operation">
                                    <template slot-scope="scope">
                                        <!-- 1 未更新版本 2 已更新版本 -->
                                        <div class="operation" @click="goToUpdate(scope.row)" :class = "{active: versionCompare < scope.row.version }">{{ $t('cNam.update') }}</div>
                                    </template>
                                </el-table-column>
                            </el-table>
                            <div class="operation cancel" @click="closeItemDetail">{{ $t('cNam.cancel') }}</div>
                        </div>
                    </template>
                </template>

                <!--升级时-->
                <template v-if="tabToUpdate && testSelect.status > 0">
                    <div class="info update">
                        <div class="update-top">
                            <div class="update-head">
                                <span>{{ $t('updateTitle') }}: {{selectHWFile.version}} </span>
                                <el-progress :percentage="percentage" class="progress" v-show="percentage > 0"></el-progress>
                                <span class="finish" v-if="percentage == 100">{{ $t('finish') }}</span>
                            </div>
                            <template v-if="testSelect.No == 8">
                                <div class="text" v-show="isCan" >{{ $t('step') }} 1: {{ $t('update1111') }}</div>
                                <div class="text" v-show="!isCan">{{ $t('step') }} 1: {{ $t('update111') }}</div>
                                <div class="text">{{ $t('step') }} 2: {{ $t('update11') }}</div>
                                <div class="text next-line" v-show="isCan">{{ $t('update122') }}</div>
                                <div class="text next-line" v-show="!isCan">{{ $t('update12') }}</div>
                                <div class="text next-line">{{ $t('update13') }}</div>
                                <div class="text">{{ $t('step') }} 3: {{ $t('update14') }}</div>
                                <div class="text">{{ $t('step') }} 4: {{ $t('update15') }}</div>
                            </template>
                            <template v-if="testSelect.No == 9">
                                <div class="text">{{ $t('step') }} 1: {{ $t('update112') }}</div>
                                <div class="text">{{ $t('step') }} 2: {{ $t('update11') }}</div>
                                <div class="text next-line">{{ $t('update16') }}</div>
                                <div class="text next-line">{{ $t('update17') }}</div>
                                <div class="text">{{ $t('step') }} 3: {{ $t('update14') }}</div>
                                <div class="text">{{ $t('step') }} 4: {{ $t('update19') }}</div>
                            </template>
                            <template v-if="testSelect.No == 11">
                                <div class="text">{{ $t('step') }} 1: {{ $t('update11') }}</div>
                                <div class="text next-line">{{ $t('update20') }}</div>
                                <div class="text">{{ $t('step') }} 2: {{ $t('update14') }}</div>
                            </template>
                        </div>
                        <div class="update-bottom">
                            <span :class="{active: updateReady}" @click="cancelUpdateModel">{{ $t('cNam.cancel') }}</span>
                            <span :class="{active: updateReady}" @click="handleReconnect">{{ $t('connection') }}</span>
                        </div>
                    </div>
                </template>

                <!-- 加载失败显示 -->
                <template v-if="testSelect.status !=1">
                    <div class="load-fail-content" >
                        <!--<div class="fail-content-top">-->
                            <!--<i class="el-icon-close " @click="closeItemDetail()"></i>-->
                        <!--</div>-->
                        <div class="fail-content-center">
                            <div class="item type">
                                <span class="left"><img src="../../assets/images/diagnosis/icon-fail-warn.png"></span>
                                <span class="right">{{failDetail.type}}</span>
                            </div>
                            <div class="item description">
                                <span class="left">{{ $t('desc') }}</span>
                                <span class="right">{{failDetail.description}}</span>
                            </div>
                            <div class="item note" v-if="testSelect.No == 7">
                                <span class="left">{{ $t('cNam.note') }}</span>
                                <span class="right">{{failDetail.noteMotor}}</span>
                            </div>
                            <div class="item note" v-else>
                                <span class="left">{{ $t('cNam.note') }}</span>
                                <span class="right">{{failDetail.note}}</span>
                            </div>
                        </div>
                        <div class="fail-content-bottom">
                            <el-button type="primary" @click="closeItemDetail()">{{ $t('cNam.cancel') }}</el-button>
                            <el-button type="primary" @click="handleReconnect">{{ $t('cNam.read') }}</el-button>
                        </div>
                    </div>
                </template>
            </div>
        </div>
    </div>
</template>

<script>

    import Update from '../../../device/update'
    import EN from "src/assets/lang/en";
    import ZH from "src/assets/lang/zh";
    import DE from "src/assets/lang/de";
    import NL from "src/assets/lang/nl";
    import vueLoading from "vue-loading-template";
    import Core from "core/core";

    export default {
        name: "diagnosisItemDetailFor3rd",
        components: {
            vueLoading
        },
        props: {
            vehicleSn: String,
            testSelect: Object,
            hmiInfo: Object,
            controllerInfo: Object,
            batteryInfo: Object,
            pedalSensorInfo: Object,
            motorInfo: Object,
            com: '',
            isCan: false,
        },
        data() {
            return {
                //新添加的测试项
                // 1 未更新版本 2 已更新版本
                tableData: [],

                tabToUpdate: false,
                //错误信息  连接错误信息固定
                failDetail: {
                    type: "Error",
                    description: "No data collected. It might be connection failure.",
                    // target: "Controller",
                    // solution: {
                    //     txt: "Replace controller",
                    //     doc: "Error 30 Solution.pdf",
                    //     video: "Error 30 Solution.mp4"
                    // },
                    note: 'Please click "Reconnect" to connect again.',
                    noteMotor: 'The motor information is empty because no bike traced.'
                },
                org: Core.Data.getOrg(),
                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.UPDATE,

                percentage: 0,
                selectHWFile: {},
                fileName: '',

                startTip: false,
                updateMsg: '',
                updateSn: '',
                versionCompare: '',
                updateMsgObject: {},
                updateReady: false,
                fileBufferCrc: ''

            };
        },
        i18n: {
            messages: {
                en: EN.Components.Diagnosis.diagnosis3rd,
                zh: ZH.Components.Diagnosis.diagnosis3rd,
                nl: NL.Components.Diagnosis.diagnosis3rd,
                de: DE.Components.Diagnosis.diagnosis3rd
            }
        },

        mounted() {
            this.failDetail.type = this.$t('error');
            this.failDetail.description = this.$t('failDesc');
            this.failDetail.note = this.$t('failNote');
            this.failDetail.noteMotor = this.$t('failNoteMotor');
            if (this.testSelect.update) {
                this.getFileList();
            }
            if (this.testSelect.status < 0){
                switch( this.testSelect.No ){
                    case 8:  // 仪表
                        if(this.hmiInfo.sn=='' && this.hmiInfo.model !='' && this.hmiInfo.version !=''){
                            this.failDetail['description'] = this.$t("errorHmi") ;
                        }
                        break;
                    case 9: // 控制器
                        if(this.controllerInfo.sn=='' && this.controllerInfo.model !='' && this.controllerInfo.version !=''){
                            this.failDetail['description'] = this.$t("errorController") ;
                        }
                        break;
                    case 10: // 电池
                        if(this.batteryInfo.sn=='' && this.batteryInfo.model !=''&& this.batteryInfo.version !=''){
                            this.failDetail['description'] = this.$t("errorBattery")  ;
                        }
                        break;
                    case 11: // 传感器
                        if(this.isCan && this.pedalSensorInfo.sn=='' && this.pedalSensorInfo.name !=''&& this.pedalSensorInfo.version !=''){
                            this.failDetail['description'] = this.$t("errorSensor")  ;
                        }
                        break;
                }
            } else {

            }

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
        },
        watch: {
            "testSelect.update": {
                handler(val) {
                    console.log(val, 'update ???');
                    if (val) {
                        this.getFileList();
                    }
                },
                deep: true
            }
        },

        methods: {
            closeItemDetail() {
//                console.log(this.testSelect.No, "id");
                this.$emit("close", 0);
            },
            goToUpdate(item) {
//                console.log("update1");
                this.selectHWFile = item;
                this.tabToUpdate = true;
                this.updateObject(item);
                this.goToUpdate = false;
            },

            // 获取版本文件
            getFileList() {
//                console.log('get hw list');
                switch( this.testSelect.No ){
                    case 8:  // 仪表
                        this.updateSn = this.hmiInfo.sn;
                        this.versionCompare = this.hmiInfo.version;
                        break;
                    case 9:  // 控制器
                        this.updateSn = this.controllerInfo.sn;
                        this.versionCompare = this.controllerInfo.version;
                        break;
                    case 10: // 电池
                        this.updateSn = this.batteryInfo.sn;
                        this.versionCompare = this.batteryInfo.version;
                        break;
                    case 11: // 传感器
                        this.updateSn = this.pedalSensorInfo.sn;
                        this.versionCompare = this.pedalSensorInfo.version;
                        break;
                }
                if (!this.updateSn) {
                    return;
                }
                Core.Api.Firmware.getHistoryBySn( this.updateSn )
                    .then(res => {
                        let list = res.record;
                        if (list.length > 3) {
                            list = list.splice(0, 3);
                        }
                        this.tableData = list;
//                        console.log(this.tableData);
                    })
            },

            // 在线更新
            updateObject(item) {
                let url = Core.Const.NET.FILE_URL_PREFIX + item.file;
                let $this = this;
                $this.fileName = item.version;

                this.$http.get(url, {responseType: 'blob'})
                    .then((response) => {
                        var fileReader = new FileReader();
                        fileReader.onload = function () {
                            let data = {
                                fileBuffer: this.result,
                                name: item.version,
                                type: 'data'
                            };
                            $this.doUpdate(data);
                        };
                        fileReader.readAsArrayBuffer(response.body);
                    });
            },

            doUpdate(data) {
                this.fileBufferCrc = Update.checkCrc(data.fileBuffer)
                let usbHID = Update.port.getUSBDevice();
                let vm = this;
                if(usbHID){
                    let count = 0;
                    Update.port.usbSDK.client.events.on('firmware-update-progress',(e)=>{
//                        this.startTip = false;
                        this.percentage = Math.ceil(e.upload / e.total * 100);
//                        console.log(e);
                        if( e.upload === e.total || vm.percentage === 100){
                            if(count === 0){
                                vm.$message({
                                    message: vm.updateMsgObject.successTip,
                                    type: 'success'
                                });
                                this.updateReady = true;
                                if (vm.sn) {
                                    vm.saveUpdateRecord();
                                }
//                                this.checkBoxUpdate();
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
                    Update.port.usbSDK.firmware.update.firmware(data.fileBuffer);

                    Update.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if( res.type === 'error_usb'){
                            vm.isConnected = false;
                        }
                    });
                }else{
                    console.log("update12");
                    Update.port.reset(100);
                    setTimeout( ()=>{
                        vm.percentage = 0;
                        Update.connect(vm.com, 57600);
                        let bus = Update.sendPackage(data);
                        bus.subscribe(res => {
                            if (res.type === 'close' || res.type === 'error') {
                                vm.isConnected = false;
                                vm.com = "";
                            }
                            if (res.type === 'data' && res.data.update) {
//                                this.startTip = false;

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
                                    if (vm.sn) {
                                        vm.saveUpdateRecord();
                                    }
                                }
                            }
                        });
                    },800)
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


                Update.port.usbSDK.client.events.on('firmware-update-completed-progress',(e)=>{
//                    console.log(e);
//                    console.log(e.progress,this.fileBufferLength);

                    if(this.isCan){
                        this.boxPercentage = Math.ceil(e.progress / this.fileBufferLength * 100);
                    }else{
                        if( e.progress*128 > this.fileBufferLength &&  e.progress*128 - this.fileBufferLength < 1024){
                            this.boxPercentage = 100;
                        }else{
                            this.boxPercentage = Math.ceil((e.progress*128) / this.fileBufferLength * 100);
                        }
                    }
                    if( this.boxPercentage > 100){
                        this.updateMsg = this.updateMsgObject.msg11;
                    }
                    if( this.boxPercentage === 0){
                        if(count === 1){
                            this.updateMsg = this.updateMsgObject.msg20;
                        }
                        count++;
                    }else {
                        count = 1;
                    }
                });
            },

            saveUpdateRecord(){
                let time = Core.Util.time();
                let filename = this.fileName;
                let type = this.recordType;
                Core.Api.ObjectRecord.save(this.updateSn, filename +"--"+ this.fileBufferCrc, time, type);
            },
            handleCancel(){
                this.$emit('closeDetail');
            },
            handleReconnect() {
                this.$emit('reconnect');
            },

            cancelUpdateModel() {
                if (this.percentage > 0 && this.percentage < 100) {
                    return;
                }
                this.selectHWFile = {};
                this.tabToUpdate = false;
            },
        },
        // created: function () {
        //     let self = this;
        //     document.onkeydown = function (e) {
        //         let key = window.event.keyCode;
        //         if (key === 13) {
        //             console.log('key enter');

        //             if (self.testSelect.status <0 ) {
        //                 self.handleReconnect();
        //             }

        //         }

        //     }
        // },
    };
</script>

<style lang="scss" rel="stylesheet/scss">
    .detail {
        .detail-content {
            width: 618px;
            margin: 0 auto;
            border: 1px solid rgb(86, 87, 90);
            background: #353945;
            display: flex;
            flex-direction: column;
            .title {
                padding-left: 30px;
                padding-right: 30px;
                font-size: 16px;
                height: 60px;
                display: flex;
                align-items: center;
                border-bottom: 1px solid rgb(86, 87, 90);
                justify-content: space-between;
                .title-left {
                    display: flex;
                    align-items: center;
                    img {
                        width: 34px;
                        height: 34px;
                        margin-right: 16px;
                    }
                    span {
                        font-size: 20px;
                        margin-right: 24px;
                        font-weight: 500;
                    }
                }
                .title-right {
                    cursor: pointer;
                }
            }
            .content {
                display: flex;
                flex-direction: column;
                width: 100%;
                .info {
                    overflow: hidden;
                    background: #353945;
                    width: 100%;
                    padding: 20px 20px 0;
                    // box-shadow: 0 1px 0 #2B2F3B;
                    min-height: 96px;
                    margin-bottom: 20px;
                    display: flex;
                    .info-left {
                        border-right: 1px dotted #2b2f3b;
                        flex: 1;
                        padding-left: 13px;
                        .item {
                            height: 48px;
                            line-height: 48px;
                            .label {
                                display: inline-block;
                                line-height: 20px;
                                width: 100px;
                                vertical-align: middle;
                                font-size: 12px;
                            }
                            .input {
                                width: 165px;
                                // margin-left: 10px;
                                & > input {
                                    height: 29px;
                                    font-size:12px;
                                }
                            }
                            .el-select {
                                width: 165px;
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
                                width: 170px;
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
                        flex: 1;
                        .item {
                            margin-left: 10px;
                            height: 48px;
                            line-height: 48px;
                            display: flex;
                            align-items: center;
                            &.mt-48 {
                                margin-top: 48px;
                            }
                            .label {
                                width: 100px;
                                display: inline-block;
                                line-height: 20px;
                                vertical-align: middle;
                                font-size: 12px;
                            }
                            .input {
                                width: 170px;
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
                    .active {
                        .el-input__inner{
                            border: 1px solid #e26829
                        }
                    }
                }
                .message{
                    padding: 0 30px 20px 32px;
                }
                .bottom-title {
                    height: 38px;
                    background: #2b2f3b;
                    line-height: 38px;
                    margin: 0 20px 20px;
                    span {
                        color: #fff;
                        margin-left: 16px;
                    }
                }
                .bottom-content {
                    margin: 0 20px 4px;
                    .el-table {
                        & th {
                            height: auto;
                        }
                        .cell {
                            font-size: 12px;
                            padding: 0;
                            word-break: normal;
                            .active{
                                background-color: #e26829;
                            }
                        }
                    }
                    .operation {
                        margin: 0 38px;
                        height: 25px;
                        background: #a1a5b1;
                        border-radius: 25px;
                        cursor: pointer;
                        &.finish {
                            background-color: #98b42c;
                        }
                        &.cancel {
                            background: #e26829;
                            width: 100px;
                            text-align: center;
                            margin-top: 10px;
                            line-height: 25px;
                            height: 25px;
                            position: relative;
                            left: 433px;
                            margin-bottom: 10px;
                        }
                    }
                }
                & > .info.update {
                    flex-direction: column;
                    justify-content: space-between;
                    height: 527px;
                    padding: 20px;
                    .update-top {
                        position: relative;
                        .update-head{
                            height: 60px;
                            span {
                                &.finish {
                                    position: absolute;
                                    top: 17px;
                                    left: 295px;
                                    background: #353945;
                                }
                            }
                        }
                        div.text {
                            margin-bottom: 10px;
                            &.next-line {
                                margin-left: 49px;
                            }
                        }
                        .progress {
                            margin-top: 15px;
                            margin-bottom: 30px;
                            width: 340px !important;
                            border: none !important;
                            .el-progress-bar__outer {
                                border-radius: 25px;
                                height: 12px !important;
                                width: 290px !important;
                            }
                            .el-progress-bar__inner {
                                background-color: #98b42c;
                                // border-radius: 0;
                            }
                            .el-progress__text {
                                font-size: 14px !important;
                                color: #fff;
                            }
                        }
                    }
                    .update-bottom {
                        display: flex;
                        justify-content: space-between;
                        span {
                            cursor: pointer;
                            &.active {
                                background-color: #e26829;
                            }
                            background-color: #949494;

                            padding: 4px 18px;
                            font-size: 13px;
                            color: #fff;
                            border-radius: 25px;
                            font-weight: 500;
                        }
                    }
                }
                .load-fail-content {
                    display: flex;
                    flex-direction: column;
                    background: #353945;
                    /*margin-top: 52px;*/
                    padding: 20px 20px 0;
                    .fail-content-top {
                        height: 40px;
                        padding: 0 8px;
                        display: flex;
                        justify-content: flex-end;
                        align-items: center;
                        background: rgb(42, 46, 58);
                        & .el-icon-close {
                            font-size: 12px;
                            padding: 12px;
                            cursor: pointer;
                        }
                    }
                    .fail-content-center {
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
                    }
                    .fail-content-bottom {
                        padding: 10px 24px;
                        display: flex;
                        justify-content: space-between;
                    }
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// item-detail.vue?49eb5f14