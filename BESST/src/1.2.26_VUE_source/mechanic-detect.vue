<template>
    <div class="mechanic-detect">
        <div class="header top-box">
            <!--<el-button type="primary" icon="arrow-left" @click="$router.back()">Back</el-button>-->
            <!--<el-button @click="clearData" class="btn-connect" type="preview">clear</el-button>-->
            This is new function that we would promote. It is still not finished yet.
        </div>

        <div class="content-wrap">
            <div class="content">
                <div class="info read">
                    <el-collapse >
                        <el-collapse-item name="1">
                            <template slot="title" >
                                <div class="search-item">
                                    <label class="label">{{ $t('bike') }}</label>
                                    <el-input class=" input input-sn" v-model="vehicleSn" :placeholder="$t('sn')"></el-input>
                                    <el-button type="preview" class="search-btn" v-if="!offLineFlag" @click.stop="getVehicle">{{ $t('confirm') }}
                                    </el-button>
                                </div>
                            </template>
                            <div class="component-list">
                                <ul>
                                    <li>
                                        <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>
                                        <label class="title-text">HMI</label>
                                        <el-input class="input-sn" v-model="hmiInfo.sn" :placeholder="$t('sn')"
                                                  @click.native.stop="stopPrevent" @blur="onBlurSn(hmiInfo.sn)"></el-input>
                                        <el-button type="delete" class="title-select"
                                                   @click.native.stop="showErrorSelectDialog('hmi')">{{ $t('selectError')}}
                                        </el-button>
                                    </li>
                                    <li>
                                        <img class="icon" src="../../assets/images/icon/icon-controller.png"/>
                                        <label class="title-text">Controller</label>
                                        <el-input class="input-sn" v-model="controllerInfo.sn" :placeholder="$t('sn')"
                                                  @click.native.stop="stopPrevent"
                                                  @blur="onBlurSn(controllerInfo.sn)"></el-input>
                                        <el-button type="delete" class="title-select"
                                                   @click.native.stop="showErrorSelectDialog('controller')">{{ $t('selectError') }}
                                        </el-button>
                                    </li>
                                    <li>
                                        <img class="icon" src="../../assets/images/icon/icon-battery.png"/>
                                        <label class="title-text">Battery</label>
                                        <el-input class="input-sn" v-model="batteryInfo.sn" :placeholder="$t('sn')"
                                                  @click.native.stop="stopPrevent"
                                                  @blur="onBlurSn(batteryInfo.sn)"></el-input>
                                        <el-button type="delete" class="title-select"
                                                   @click.native.stop="showErrorSelectDialog('battery')">
                                            {{ $t('selectError') }}
                                        </el-button>
                                    </li>
                                    <li>
                                        <img class="icon" src="../../assets/images/icon/icon-motor.png"/>
                                        <label class="title-text">Motor</label>
                                        <el-input class="input-sn" v-model="motorInfo.sn" :placeholder="$t('sn')"
                                                  @click.native.stop="stopPrevent"
                                                  @blur="onBlurSn(motorInfo.sn)"></el-input>
                                        <el-button type="delete" class="title-select"
                                                   @click.native.stop="showErrorSelectDialog('motor')">{{ $t('selectError') }}
                                        </el-button>
                                    </li>
                                    <li>
                                        <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>
                                        <label class="title-text">Pedal Sensor</label>
                                        <el-input class="input-sn" v-model="pedalSensorInfo.sn" :placeholder="$t('sn')"
                                                  @click.native.stop="stopPrevent"
                                                  @blur="onBlurSn(pedalSensorInfo.sn)"></el-input>
                                        <el-button type="delete" class="title-select"
                                                   @click.native.stop="showErrorSelectDialog('pedalSensor')">
                                            {{ $t('selectError') }}
                                        </el-button>
                                    </li>
                                    <li>
                                        <el-input
                                            type="textarea"
                                            :rows="1"
                                            resize="none"
                                            placeholder="Type here"
                                            v-model="other">
                                        </el-input>
                                        <el-button type="primary" @click="saveBikeDetect">Save</el-button>

                                    </li>
                                </ul>
                            </div>
                        </el-collapse-item>
                    </el-collapse>
                    <div class="clear-20"></div>
                    <div class="parts-list">
                        <span class="title">Mechanic Detect List</span>
                        <div class="search">
                            <el-input class="input-sn" v-model="componentSN" :placeholder="$t('sn')"
                                      @click.native.stop="stopPrevent"></el-input>
                            <el-button @click="" type="primary">Search</el-button>
                        </div>
                    </div>
                    <div class="table-wrap">
                        <div class="table-content-holder">
                            <!--<div class="no-data-content" v-if="detectList.length <= 0 && !loading">-->
                                <!--noResult-->
                            <!--</div>-->
                            <table class="table-content" border="0" cellspacing="0" cellpadding="0" >
                                <thead>
                                <td v-for="(item, index) in headData" :class="{'al-right': index=== 0 || index=== 1 }">{{item}}</td>
                                </thead>
                                <tbody>
                                <tr v-for="item in detectList">
                                    <td class="al-right">{{ item.a  || '/' }}</td>
                                    <td class="al-right">{{ item.b || '/' }}</td>
                                    <td>{{ item.c || '/' }}</td>
                                    <td>{{ item.d  }}</td>
                                    <td>{{ item.e || '/' }}</td>
                                    <td>{{ item.f || '/' }}</td>
                                    <td>
                                        <a class="btn-underline" @click.stop="goDetectEditView(item)">Edit</a>
                                        <a class="btn-underline" @click.stop="showDeleteDialog(item)" v-if="true">delete</a>
                                    </td>
                                </tr>
                                </tbody>
                            </table>
                        </div>
                        <div class="loading" v-if="loading">
                            <img src="../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
                        </div>
                        <div class="table-footer">
                            <el-pagination
                                @current-change="handleCurrentChange"
                                :current-page.sync="currentPage"
                                :page-size="8"
                                layout="prev, pager, next, jumper"
                                :total="total">
                            </el-pagination>
                            <p class="total">total: {{total}}</p>

                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- 错误选择弹窗-->
        <el-dialog :title="$t('selectError')" :visible.sync="dialogErrorSelectVisible"
                   custom-class="error-select-dialog">
            <div class="content">
                <el-checkbox-group v-model="errorList">
                    <el-checkbox v-for="error in selectErrorList" :key="error.label"
                                 :label="error.label"></el-checkbox>
                </el-checkbox-group>
            </div>
            <span slot="footer" class="dialog-footer">
                    <el-button type="preview" @click="addItemError">Add</el-button>
                    <el-button type="primary" @click="dialogErrorSelectVisible = false">{{ $t('cancel') }}</el-button>
                </span>
        </el-dialog>
    </div>
</template>

<script>
    import HMI from '../../../device/hmi'
    import printArea from 'printArea'
    import Battery from '../../../device/battery'
    import Controller from '../../../device/controller'
    import Diagnose from '../../../device/diagnose'
    import Core from 'core/core'
    import vueLoading from 'vue-loading-template'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        components: {
            vueLoading
        },
        data() {
            return {
                org: Core.Data.getOrg() || "",
                HMI: HMI,
                Battery: Battery,
                Controller: Controller,

                vehicleId: '',
                vehicleSn: '',
                componentList: [],
                device: HMI,
                deviceList: {hmi: HMI, battery: Battery, controller: Controller},
                comList: '',
                com: Core.Data.getPort() ? Core.Data.getPort() : '',
                isConnected: HMI.port.isConnected(),
                isTestBtnActive: false,

                //数据库数据
                motorInfo: {
                    sn: '',
                    name: '',
                    power: '',
                    voltage: '',
                    wheelSize: '',
                    rpm: '',
                    error: ''
                },
                pedalSensorInfo: {
                    sn: '',
                    name: '',
                    version: '',
                    hardVersion: '',
                    error: ''
                },
                rpmSensorInfo: {
                    sn: '',
                    name: '',
                    version: '',
                    hardVersion: '',
                    error: ''
                },
                //读取数据
                hmiInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    speedLimit: '',
                    wheelDiameter: '',
                    totalMileage: '',
                    totalGear: '',
                    error: ''
                },
                controllerInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    manufacturer: '',
                    nmlVolt: '',
                    maxCurrent: '',
                    error: ''
                },
                batteryInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    soc: '',
                    soh: '',
                    packChargerCapacity: '',
                    fullChargerCapacity: '',
                    leftCapacity: '',
                    temperature: '',
                    current: '',
                    loopTime: '',
                    cell1to7: '',
                    cell8to14: '',
                    lastCharge: '',
                    maxChargeInterval: '',
                    error: '',
                    packVoltage: '',
                },

                testData: {
                    light: '',
                    error: '',
                    gear: '',
                    cadence: '',
                    speed: '',
                    current: '',
                    capacity: '',
                    sensorTorque: '',
                    sensorVoltage: '',
                },

                ERROR: {
                    HMI: [8, 9, 21, 22, 24, 25, 26, 30],
                    CONTROLLER: [7, 8, 9, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 30],
                    BATTERY: [7, 13, 22],
                    MOTOR: [8, 9, 10, 14],
                    RPMSENSOR: [21],
                    PEDALSENSOR: [25, 26],
                },

                percentage: {
                    hmi: 0,
                    controller: 0,
                    battery: 0
                },

                showPercentage: {
                    hmi: false,
                    controller: false,
                    battery: false
                },
                showLoading: {
                    hmi: false,
                    controller: false,
                    battery: false
                },

                defectOptions: [
                    {value: 1, label: "Motor doesn't work"},
                    {value: 2, label: "Motor's noisy"},
                    {value: 3, label: "Motor got damage"},
                    {value: 4, label: "HMI failed"},
                    {value: 5, label: "HMI got error code"},
                    {value: 6, label: "HMI got damage"},
                    {value: 7, label: "Disordered indication"},
                    {value: 8, label: "Out of control"},
                    {value: 9, label: "E-brake failed"},
                    {value: 10, label: "E-brake got damage"},
                    {value: 11, label: "Light failed"},
                    {value: 12, label: "Walk assistance failed"},
                    {value: 13, label: "Throttle failed"},
                    {value: 14, label: "Throttle got damage"},
                    {value: 15, label: "Short range"},
                    {value: 16, label: "Cable got damage"},
                    {value: 17, label: "Battery problems"},
                    {value: 18, label: "Battery got damage"},
                ],
                motorDefectOptions: [
                    {value: 1, label: "Motor doesn't work,but the display work normally"},
                    {value: 2, label: "Motor Shakes"},
                    {value: 3, label: "Motor Noisy"},
                    {value: 4, label: "Motor Less Power"},
                    {value: 5, label: "Motor runs intermittently"},
                    {value: 6, label: "No 6KM/H function"},
                ],
                hmiDefectOptions: [
                    {value: 1, label: "Display doesn't work"},
                    {value: 2, label: "Display Water inside"},
                    {value: 3, label: "Display Button damaged"},
                    {value: 4, label: "Display Cable damaged"},
                    {value: 5, label: "Display Screen damaged"},
                    {value: 6, label: "Display Bracket damaged"},
                    {value: 7, label: "Display Screw loosened"},
                    {value: 8, label: "Display black screen"},
                    {value: 9, label: "Display Messy code"},
                ],
                batteryDefectOptions: [
                    {value: 1, label: "Battery damaged"},
                    {value: 2, label: "Charger doesn't work"},
                    {value: 3, label: "Battery can't be charged"}
                ],
                controllerDefectOptions: [
                    {value: 1, label: "No assistance"},
                    {value: 2, label: "No Lighting function"},
                    {value: 3, label: "Brake can't cut off the power"},
                    {value: 4, label: "Trip distance too short"}
                ],


                dialogSolutionVisible: false,
                componentError: {},
                // collapseActives: ['1', '2', '3'],

                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',


                dialogConfirmVisible: false,

                finished: false,

                // 错误选择弹框
                dialogErrorSelectVisible: false,
                selectErrorList: [],
                selectType: '',
                errorList: [],


                offLineFlag: false,
                dialogEmptySNVisible: false,

                // APP
                other: '',
                // 列表
                headData: [],
                detectList: [],
                total: 0,
                detectSelectedItem: {},
                currentPage: Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) ? Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) : 1,
                componentSN: '',
                loading: false,

            }
        },
        i18n: {
            messages: {
                en: EN.Components.Diagnosis.diagnosisNormal,
                zh: ZH.Components.Diagnosis.diagnosisNormal,
                de: DE.Components.Diagnosis.diagnosisNormal,
                nl: NL.Components.Diagnosis.diagnosisNormal
            }
        },
        mounted: function () {
            // 国际化
            let $this = this;
            this.initData();
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });
            this.isTestBtnActive = true;
            this.offLineFlag = Core.Data.getOffline();

            this.headData = ['Date','Bike SN','Component SN','Pr.Dt','Status','Defects Option','Operation'];
            this.getPackageList();
        },

        methods: {
            initData() {
                this.defectOptions = [
                    {value: 1, label: this.$t('defectOptions.motorNotWork')},
                    {value: 2, label: this.$t('defectOptions.motorNoisy')},
                    {value: 3, label: this.$t('defectOptions.motorDamage')},
                    {value: 4, label: this.$t('defectOptions.hmiFailed')},
                    {value: 5, label: this.$t('defectOptions.hmiErrorCode')},
                    {value: 6, label: this.$t('defectOptions.hmiDamage')},
                    {value: 7, label: this.$t('defectOptions.disorderedIndication')},
                    {value: 8, label: this.$t('defectOptions.outOfControl')},
                    {value: 9, label: this.$t('defectOptions.eBrakeFailed')},
                    {value: 10, label: this.$t('defectOptions.eBrakeDamage')},
                    {value: 11, label: this.$t('defectOptions.lightFailed')},
                    {value: 12, label: this.$t('defectOptions.walkAssistanceFailed')},
                    {value: 13, label: this.$t('defectOptions.throttleFailed')},
                    {value: 14, label: this.$t('defectOptions.throttleDamage')},
                    {value: 15, label: this.$t('defectOptions.shortRange')},
                    {value: 16, label: this.$t('defectOptions.cableDamage')},
                    {value: 17, label: this.$t('defectOptions.batteryProblem')},
                    {value: 18, label: this.$t('defectOptions.batteryDamage')},
                ];
                this.motorDefectOptions = [
                    {value: 1, label: this.$t('motorDefectOptions.motorNotWork')},
                    {value: 2, label: this.$t('motorDefectOptions.motorShake')},
                    {value: 3, label: this.$t('motorDefectOptions.motorNoisy')},
                    {value: 4, label: this.$t('motorDefectOptions.motorLessPower')},
                    {value: 5, label: this.$t('motorDefectOptions.motorRunIntermittently')},
                    {value: 6, label: this.$t('motorDefectOptions.noFunction')},
                ];
                this.hmiDefectOptions = [
                    {value: 1, label: this.$t('hmiDefectOptions.displayNotWork')},
                    {value: 2, label: this.$t('hmiDefectOptions.displayWaterInside')},
                    {value: 3, label: this.$t('hmiDefectOptions.displayBtnDamage')},
                    {value: 4, label: this.$t('hmiDefectOptions.displayCableDamage')},
                    {value: 5, label: this.$t('hmiDefectOptions.displayScreenDamage')},
                    {value: 6, label: this.$t('hmiDefectOptions.displayBracketDamage')},
                    {value: 7, label: this.$t('hmiDefectOptions.displayScrewLoosen')},
                    {value: 8, label: this.$t('hmiDefectOptions.displayBlackScreen')},
                    {value: 9, label: this.$t('hmiDefectOptions.displayMessyCode')},
                ];
                this.batteryDefectOptions = [
                    {value: 1, label: this.$t('batteryDefectOptions.batteryDamage')},
                    {value: 2, label: this.$t('batteryDefectOptions.chargerNotWork')},
                    {value: 3, label: this.$t('batteryDefectOptions.BatteryCannotCharge')}
                ];
                this.controllerDefectOptions = [
                    {value: 1, label: this.$t('controllerDefectOptions.noAssistance')},
                    {value: 2, label: this.$t('controllerDefectOptions.noLightFunc')},
                    {value: 3, label: this.$t('controllerDefectOptions.brakeCannotCutOffPower')},
                    {value: 4, label: this.$t('controllerDefectOptions.tripDistanceShort')}
                ];
            },

            clearData() {
                this.vehicleId = '';
                this.vehicleSn = '';
                Object.keys(this.hmiInfo).forEach(key => {
                    this.hmiInfo[key] = '';
                });
                Object.keys(this.controllerInfo).forEach(key => {
                    this.controllerInfo[key] = '';
                });
                Object.keys(this.batteryInfo).forEach(key => {
                    this.batteryInfo[key] = '';
                });
                Object.keys(this.motorInfo).forEach(key => {
                    this.motorInfo[key] = '';
                });
                Object.keys(this.pedalSensorInfo).forEach(key => {
                    this.pedalSensorInfo[key] = '';
                });
                Object.keys(this.rpmSensorInfo).forEach(key => {
                    this.rpmSensorInfo[key] = '';
                });

                this.result = {
                    hmiOK: true,
                    controllerOK: true,
                    batteryOK: true,
                    pedalSensorOK: true,
                    rpmSensorOK: true,
                    motorOK: true,
                    hmi: '',
                    motor: '',
                    battery: '',
                    controller: '',
                    pedalSensor: '',
                    rpmSensor: ''
                };


                this.percentage = {
                    hmi: 0,
                    controller: 0,
                    battery: 0
                };

                HMI.port.reset(100);
            },

            onBlurSn(sn) {
                if (!sn) {
                    return;
                }
                if(this.offLineFlag){
                    return;
                }
                Core.Api.Component.componentDetailBySn(sn).then(
                    res => {
                        // 检查Component 是否存在数据库
                    }
                );
            },

            showErrorSelectDialog(type) {
                this.selectType = type;
                switch (type) {
                    case 'hmi':
                        this.selectErrorList = this.hmiDefectOptions;
                        this.errorList = this.hmiInfo.error ? this.hmiInfo.error.split(', ') : [];
                        break;
                    case 'controller':
                        this.selectErrorList = this.controllerDefectOptions;
                        this.errorList = this.controllerInfo.error ? this.controllerInfo.error.split(', ') : [];
                        break;
                    case 'battery':
                        this.selectErrorList = this.batteryDefectOptions;
                        this.errorList = this.batteryInfo.error ? this.batteryInfo.error.split(', ') : [];
                        break;
                    case 'motor':
                        this.selectErrorList = this.motorDefectOptions;
                        this.errorList = this.motorInfo.error ? this.motorInfo.error.split(', ') : [];
                        break;
                    case 'pedalSensor':
                        this.selectErrorList = this.defectOptions;
                        this.errorList = this.pedalSensorInfo.error ? this.pedalSensorInfo.error.split(', ') : [];
                        break;
                    case 'rpmSensor':
                        this.selectErrorList = this.defectOptions;
                        this.errorList = this.rpmSensorInfo.error ? this.rpmSensorInfo.error.split(', ') : [];
                        break;
                    default:
                        break;
                }
                this.dialogErrorSelectVisible = true;
            },

            addItemError() {
                switch (this.selectType) {
                    case 'hmi':
                        this.hmiInfo.error = this.errorList.join(", ");
                        this.activeNames = ['1'];
                        if(this.hmiInfo.sn ==''){
                            this.$message('Please input ' + this.selectType + ' SN');
                        }else{
                            this.add
                        }
                        break;
                    case 'controller':
                        this.controllerInfo.error = this.errorList.join(", ");
                        this.activeNames = ['2'];
                        break;
                    case 'battery':
                        this.batteryInfo.error = this.errorList.join(", ");
                        this.activeNames = ['3'];
                        break;
                    case 'motor':
                        this.motorInfo.error = this.errorList.join(", ");
                        this.activeNames = ['4'];
                        break;
                    case 'pedalSensor':
                        this.pedalSensorInfo.error = this.errorList.join(", ");
                        this.activeNames = ['5'];
                        break;
                    case 'rpmSensor':
                        this.rpmSensorInfo.error = this.errorList.join(", ");
                        this.activeNames = ['6'];
                        break;
                    default:
                        break;
                }
                this.dialogErrorSelectVisible = false;
            },

            getVehicle() {
                if(this.offLineFlag){
                    return;
                }
                if(!this.vehicleSn){
                    this.$message("Please input vehicle frame number");
                    return;
                }
                this.vehicleSn = this.vehicleSn.trim();
                Core.Api.Vehicle.vehicleDetailBySn(this.vehicleSn, this.org.id).then(
                    res => {
                        this.vehicleId = res.vehicle.id;
                        console.log('id', this.vehicleId)
                    }
                );
                Core.Api.Vehicle.getVehicleBomBySn(this.vehicleSn).then(res => {
//                    this.componentList = res.bom;
                    this.getBomInfo(res.bom);
                });
            },
            getBomInfo(bomList) {
                let $this = this;
                bomList.forEach(function (bom, index) {

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.MOTOR) {
                        $this.motorInfo.sn = bom.sn;
                        $this.motorInfo.name = bom.item.name;
                        let attrList = bom.item.attr_list;
                        if (attrList === null || attrList.length <= 0) {
                            return;
                        }
                        for (let index in attrList) {
                            if (attrList[index].attr_def_key === "power") {
                                $this.motorInfo.power = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "voltage") {
                                $this.motorInfo.voltage = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "wheel_size") {
                                $this.motorInfo.wheelSize = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "rpm") {
                                $this.motorInfo.rpm = attrList[index].value;
                            }
                        }
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.SENSOR) {
                        $this.pedalSensorInfo.sn = bom.sn;
                        $this.pedalSensorInfo.name = bom.item.name;
                        let attrList = bom.item.attr_list;
                        if (attrList === null || attrList.length <= 0) {
                            return;
                        }
                        for (let index in attrList) {
                            if (attrList[index].attr_def_key === "hard_ver") {
                                $this.pedalSensorInfo.version = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "soft_ver") {
                                $this.pedalSensorInfo.hardVersion = attrList[index].value;
                            }
                        }
                    }
                });
            },

            showSolutionDialog() {
                this.dialogSolutionVisible = true;
                // this.componentErrorDetail(this.testData.error);
            },

            componentErrorDetail(code) {
                // console.log('code', code);
                if(!code){
                    return;
                }
                this.componentError = Core.Const.TEST_ERROR_CODE[code];
//                Core.Api.Error.componentErrorDetail(code).then(res => {
//                    this.componentError = res.error;
                if (this.ERROR.HMI.indexOf(code) > -1) {
                    this.result.hmi = this.componentError.name;
                }
                if (this.ERROR.CONTROLLER.indexOf(code) > -1) {
                    this.result.controller = this.componentError.name;
                }
                if (this.ERROR.BATTERY.indexOf(code) > -1) {
                    this.result.battery = this.componentError.name;
                }
                if (this.ERROR.MOTOR.indexOf(code) > -1) {
                    this.result.motor = this.componentError.name;
                }
                if (this.ERROR.RPMSENSOR.indexOf(code) > -1) {
                    this.result.rpmSensor = this.componentError.name;
                }
                if (this.ERROR.PEDALSENSOR.indexOf(code) > -1) {
                    this.result.pedalSensor = this.componentError.name;
                }
//                });
            },

            stopPrevent() {
                console.log("stop prevent");
            },

            getErrorCode(code) {
                return Core.Const.TEST_ERROR_CODE[code].error;
            },

            getVehicleByBomSn(bomSn) {
                if(this.offLineFlag){
                    return;
                }
                let sn = bomSn.trim();
                if (this.vehicleSn) {
                    return;
                }

                Core.Api.Component.componentDetailBySn(sn)
                    .then(res => {
                            // 检查Component 是否存在数据库
                            return Core.Api.Vehicle.getVehicleByBomSn(sn)
                        }
                    ).then(res => {
                    if (res.vehicle !== null) {
                        this.vehicleSn = res.vehicle.sn;
                        this.getVehicle();
                    }
                });
            },

            onClickPrint() {
                let print = "div.print-content-wrap";

                let extraCss = "";
                let mode = "iframe";
                let close = false;
                let retainAttr = ['class', 'id', 'style', 'on'];
                let headElements = '<meta charset="utf-8" />,<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';

                let options = { mode : mode, popClose : close, extraCss : extraCss, retainAttr : retainAttr, extraHead : headElements };
                $(print).printArea(options);
            },

            goLogin(){
                this.$router.push({name: 'login'});
            },

            getPackageList(){

                let res = [];
                this.detectList = res.slice(0,8);

                this.total= res.length;
            },
            handleCurrentChange(){

            },
            goDetectEditView(item){

            },
            showDeleteDialog(item){

            },

            saveBikeDetect(){

            }

        },

        watch: {
//            vehicleSn: function (newValue, oldValue) {
//                this.vehicleInfo.vehicleSn = newValue.replace(/\s+/g, "");
//            },
            'hmiInfo.sn': {
                handler: function (newValue, oldValue) {
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
                                let value = vm.hmiInfo.sn.replace(/\s+/g, "");
                                if (value.indexOf('DP') > -1) {
                                    let array = value.split('DP');
                                    if (array.length > 1) {
                                        value = 'DP' + array[array.length - 1];
                                    }
                                    vm.hmiInfo.sn = value;
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 1000)
                    }
                },
                deep: true
            },
            'controllerInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.controllerInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'batteryInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.batteryInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'motorInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.motorInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'pedalSensorInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.pedalSensorInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'rpmSensorInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.rpmSensorInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
        }
    }
</script>

<style lang="scss" >
    .mechanic-detect {
        font-size: 16px;
        .header.top-box {
            .label-com {
                margin-left: 20px;
                margin-right: 10px;
            }
            .btn-connect {
                margin: 0 8px;
            }
            .tip {
                display: inline-block;
                border-radius: 50%;
                width: 10px;
                height: 10px;
                background: #E26829;
            }
            .connect {
                background: #76EE00;
            }
            .tip-img {
                width: 18px;
                position: relative;
                top: 4px;
            }
        }

        .content-wrap {
            padding: 10px 0;
            .content {
                padding-left: 0;
                .el-collapse-item__header {
                    font-size: 0;
                }
                .info {
                    .search-item {
                        width: 100%;
                        /*font-size: 0;*/
                        .tip-img {
                            width: 18px;
                            position: relative;
                            top: 4px;
                            margin-left: 14px;
                        }
                        .label {
                            display: inline-block;
                            width: 100px;
                            margin-left:22px;
                        }
                        .search-btn {
                            border-radius: 100px;
                            position: relative;
                            margin-left: 15px;
                        }
                    }
                    .title-select {
                        /*position: absolute;*/
                        /*right: 120px;*/
                        /*top: 16px;*/
                    }
                    .title-loading {
                        position: absolute;
                        display: inline-block;
                        width: 30px;
                        height: 30px;
                        margin-top: 15px;
                    }
                    .icon {
                        display: inline-block;
                        width: 20px;
                        height: 20px;
                        /*line-height: 20px;*/
                        vertical-align: middle;
                        position: relative;
                        top: -4px;
                    }
                    .title-text {
                        display: inline-block;
                        margin-left: 10px;
                        width: 100px;
                    }
                    .title-flag {
                        display: inline-block;
                        /*width: 60px;*/
                    }
                    .input-sn {
                        width: 240px;
                        height: 30px;
                        margin-left:30px;
                        .el-input__inner {
                            padding-right: 40px;
                            text-align: right;
                        }
                    }
                    .component-list{
                        ul{
                            li{
                                position:relative;
                                height: 60px;
                                line-height: 60px;
                                padding-left:20px;
                                border-top: 1px solid #22252E;
                                .title-select {
                                    position: absolute;
                                    right: 120px;
                                    top: 16px;
                                }
                                .el-textarea{
                                    width:810px;
                                    vertical-align: middle;
                                }
                            }
                        }
                    }
                }
                .parts-list{
                    position:relative;
                    height:50px;
                    line-height: 50px;
                    .title{
                        padding-left:55px;
                        font-size:16px;
                    }
                    .search{
                        float:right;
                        padding-right:30px;
                        .input-sn:after{
                            top:15px;
                        }
                    }
                }
                .table-wrap{
                    border:1px solid #2A2E3A;
                    .table-content-holder{

                    }
                    .table-footer{
                        position:static;
                    }
                }
                .el-collapse-item__header{
                    height: 60px;
                    line-height: 60px;
                    .el-collapse-item__header__arrow{
                        top: 24px;
                    }
                }
            }
        }

        .dialog-container-pic {
            text-align: center;
            .img {
                width: 100%;
                height: auto;
            }
        }

        .error-shooting {
            .content-item {
                width: 100%;
                padding: 16px 0;
                border-bottom: 1px solid rgba(255, 255, 255, 0.10);
                font-size: 0;
                .title {
                    display: inline-block;
                    vertical-align: top;
                    width: 140px;
                    font-size: 14px;
                    color: rgba(255, 255, 255, 0.6);
                    padding: 0 5px;
                }
                .desc {
                    display: inline-block;
                    vertical-align: top;
                    width: calc(100% - 140px);
                    span {
                        font-size: 14px;
                        color: #FFFFFF;
                        &.strong {
                            font-weight: bold;
                        }
                    }
                    p {
                        font-size: 14px;
                        color: #FFFFFF;
                    }
                }
            }
        }

        .print-content-wrap {
            display: none;
        }

        .tip-dialog{
            .el-dialog__body{
                padding: 0;
            }
        }

    }
</style>



// WEBPACK FOOTER //
// mechanic-detect.vue?cc0cd930