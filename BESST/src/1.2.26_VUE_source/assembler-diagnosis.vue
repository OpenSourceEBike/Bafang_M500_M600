<template>
    <div class="assembler-diagnosis-test">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goBack" title="Back"></div>
                </div>
            </div>
            <label>Drive System Test</label>
        </div>
        <div class="content-wrap">
            <div class="content">
                <!--内容左侧 begin-->
                <div class="content-left">
                    <!--连接项 begin-->
                    <div class="content-select connect">
                        <div class="select-item" v-show="false">Select</div>
                        <span class="connection">Connection</span>
                        <span class="uart" v-if="isConnected">UART</span>
                    </div>
                    <div class="connection-item">
                        <template v-for="item in connectionSelectDisplay">
                            <div class="info-item" @click="showDetail(item)"
                                 :class="{select: item.No === testSelect.No}">
                                <div class="item-info">
                                    <div class="item-icon">
                                        <img :src="item.img">
                                    </div>
                                    <span class="item-name">{{item.name}}</span>
                                </div>
                                <div class="result-wrap">
                                    <!-- (先判断是否加载完全,再判断加载成功还是失败,显示成功前还要判断是否要更新) -->
                                    <!--<span class="result" v-if="item.status === 1">OK</span>-->
                                    <!--<span v-if="item.status === -1" class="result fail">Fail</span>-->
                                </div>
                                <div class="detail">
                                    <span class="txt">Detail</span>
                                    <span class="arrow"></span>
                                </div>
                            </div>
                        </template>
                    </div>

                    <!--测试项 begin-->
                    <div class="content-select" :class="{connect: startTest}">
                        <div class="select-item" @click="showItemSelectDialog()">Select</div>
                        <span class="connection" @click="testSelect = ''" style="cursor: pointer">Test System</span>
                        <el-switch v-model="switchTest" on-color="#98b42c" off-color="#606161" on-text="A"
                                   off-text="M"></el-switch>
                    </div>
                    <div class="connection-item">
                        <template v-for="(item,index) in testSelectDisplay">
                            <div class="info-item" @click="showTestDiv(item, index)"
                                 :class="{select: item.No == testSelect.No}">
                                <div class="item-info">
                                    <div class="item-icon">
                                        <img :src="item.img">
                                    </div>
                                    <span class="item-name">{{item.name}}</span>
                                </div>
                                <div class="result-wrap">
                                    <!-- 显示 OK 还是 Fail -->
                                    <template v-if="startTest">
                                        <span class="option" :class="{check: item.okOrFail == 1 }"
                                              @click.stop="changeOption(item,1)">OK</span>
                                        <span class="option" :class="{fail: item.okOrFail == 2 }"
                                              @click.stop="changeOption(item,2)">Fail</span>
                                    </template>

                                </div>
                                <div class="detail" v-if="item.detail" @click.stop="showDetail(item)">
                                    <span class="txt">Detail</span>
                                    <span class="arrow"></span>
                                </div>
                                <div class="replace-detail" v-if="!item.detail">

                                </div>
                            </div>
                        </template>
                    </div>

                </div>
                <!--内容左侧 end-->
                <!--内容右侧 begin-->
                <div class="content-right">
                    <!--<el-button class="btn-reset" type="primary" @click="clearData">Reset</el-button>-->
                    <!-- 滑动上面的内容 -->

                    <!-- 可以进入test 模式 test的滑块-->
                    <template>
                        <div class="slide-content" v-if="slideDisplay">
                            <div class="slide-content-top">
                                <span class="slide-to-right" @click="showSlide(1)"></span>
                                <span class="slide-to-right" @click="showSlide(1)"></span>
                            </div>
                            <div class="slide-content-bottom">
                                <div class="slide-content-warn">
                                    <img src="../../../assets/images/diagnosis/icon-warn.png">
                                    <div class="warn-list">
                                        <span class="dot">Suspend the bicycle to prevent injury or damage.</span>
                                        <span class="dot">Make sure the wheel is free to rotate before pressing the button!</span>
                                    </div>
                                </div>
                                <div class="show-video">
                                    <img src="../../../assets/images/diagnosis/icon-under-pic.png">
                                    <img class="play-video" src="../../../assets/images/diagnosis/icon-start-mp4.png">
                                </div>
                                <div class="system-txt">
                                    <span class="title ">Test System</span>
                                    <span class="title step">Step 1 :
                                        <span>Select the functions</span>
                                    </span>
                                    <span class="title step">Step 2 :
                                        <span>Select "A" or "M" , A for Auto test, M for manual test;</span>
                                    </span>
                                    <span class="title step">Step 3 :
                                        <span>Start your test process</span>
                                    </span>
                                    <!-- 点击开始测试 滑动栏消失 -->
                                    <span class="test-button" @click="beginTestSystem()">
                                        Test
                                    </span>

                                </div>
                            </div>
                        </div>
                        <div class="slide-content slide-hidden" v-if="!slideDisplay">
                            <div class="slide-content-top slide-hidden">
                                <span class="slide-to-right slide-hidden" @click="showSlide(2)"></span>
                                <span class="slide-to-right slide-hidden" @click="showSlide(2)"></span>
                            </div>
                        </div>
                    </template>

                    <!-- 小车图标  永远存在 并且当所有完成的时候 出现 create report按钮 -->
                    <div class="slide-bottom" v-if="!slideDisplay && !detailVisible ">
                        <img src="../../../assets/images/diagnosis/icon-error.png" class="error-img"
                                 @click="dialogSolutionVisible = true"
                                 v-if="error.target">
                        <img src="../../../assets/images/diagnosis/icon-new-bike.png">
                        <span class="test-button" v-if="allFinish" @click="goToReport">
                            Create Report
                        </span>

                    </div>

                    <!-- 显示detail中的内容 默认0 不展现 -->
                    <template v-if="testSelect.No>=7 && testSelect.No<=11 && detailVisible">
                        <item-detail
                                :vehicle-id="vehicleId"
                                :test-select="testSelect"
                                :hmi-info="hmiInfo"
                                :controller-info="controllerInfo"
                                :battery-info="batteryInfo"
                                :pedal-info="pedalSensorInfo"
                                :motor-info="motorInfo"
                                :com="com"
                                @close="closeDetail"
                                @reconnect="reconnect"
                                class="item-detail">
                        </item-detail>
                    </template>

                    <!-- 选中 Test System后出现右边栏 -->
                    <template v-if=" testSelect.No< 7 && testSelect.No >= 0 && startTest && testInfoVisible">
                        <div class="test-system-item" ref="test-item">
                            <div class="brake" v-if="testSelect.No==0">
                                <img class="img" src="../../../assets/images/diagnosis/brake.png">
                                <div class="clear-20"></div>
                                Please brake and release the Brake Lever at least two times, then you should <br /> see
                                green light.
                                <span class="mark-span"></span>
                                <div class="clear-20"></div>
                                If no green light, please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and then replace it.<br />
                                <div class="clear-20"></div>

                            </div>
                            <div class="throttle" v-if="testSelect.No ==1">
                                <img class="img" src="../../../assets/images/diagnosis/throttle.png">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-light.png"
                                     v-if="lightFlag.throttle">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-unlight.png"
                                     v-else>
                                <div class="clear-20"></div>
                                Please rotate the throttle,then motor should work,and also you should
                                <br />see green light.
                                <div class="clear-40"></div>
                                If no green light ,please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and then replace it.
                                <div class="clear-20"></div>
                            </div>
                            <div class="light" v-if="testSelect.No ==2">
                                <img class="img" src="../../../assets/images/diagnosis/light.png">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-light.png"
                                     v-if="lightFlag.light">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-unlight.png"
                                     v-else>
                                <div class="clear-20"></div>
                                Please press amd hold the button "
                                <img src="../../../assets/images/icon/head-light-inactive.png" class="light-img" />" on
                                the
                                control unit, then you
                                <br />should see the light on or off,and also you should see green light.
                                <div class="clear-40"></div>
                                If light doesn't work, but you still see the green light,then replace the light.<br />
                                If neither light works nor you see green light,please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and<br /> then replace control unit or controller.
                                <div class="clear-20"></div>
                            </div>
                            <div class="support " v-if="testSelect.No === 3">
                                <img class="img" src="../../../assets/images/diagnosis/hmi.png">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-light.png"
                                     v-if="lightFlag.support">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-unlight.png"
                                     v-else>
                                <div class="clear-20"></div>
                                Please press "+" "-" alternately, then you should see green light.
                                <br />
                                <div class="clear-20"></div>
                                If no green light, please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and then replace it.<br />
                                <div class="clear-20"></div>
                            </div>
                            <div class="walk-assistance " v-if="testSelect.No === 4">
                                <img class="img" src="../../../assets/images/diagnosis/walk.png">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-light.png"
                                     v-if="lightFlag.walk">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-unlight.png"
                                     v-else>
                                <div class="clear-20"></div>
                                Please press and hold the button "
                                <img src="../../../assets/images/diagnosis/icon-new-walk-assistance.png"
                                     class="light-img">
                                "or"--",then the motor should<br />
                                work at slow speed ,and also you should see green light.
                                <div class="clear-20"></div>
                                If no green light, please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and then replace it.<br />
                                <div class="clear-20"></div>
                            </div>
                            <div class="speed " v-if="testSelect.No ==5">
                                <img class="img" src="../../../assets/images/diagnosis/speed-middle.png">
                                <img class="img" src="../../../assets/images/diagnosis/speed-hub.png">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-light.png"
                                     v-if="lightFlag.speed">
                                <img class="display-ok" src="../../../assets/images/diagnosis/icon-test-unlight.png"
                                     v-else>
                                <div style="width: 160px">
                                    <el-input v-model="testData.wheelSpeed">
                                        <template slot="append">km/h</template>
                                    </el-input>
                                </div>
                                <div class="clear-20"></div>
                                Way 1: Please turn the wheel forward by hand, then you will see the speed <br />above
                                and get green light.
                                <div class="clear-10"></div>
                                Way 2: Please click
                                <span class="start">Start</span>,then you will see the speed above and get green <br />light.
                                <div class="clear-20"></div>
                                If no green light, please keep the magnet in the right induction position, <br /> if
                                still does not word, please click
                                <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                </el-button>
                                and then replace it.
                                <div class="clear-10"></div>
                            </div>
                            <div class="pedal" v-if="testSelect.No ==6">
                                <el-switch v-model="cadenceOrTorque" on-text="S" off-text="T" class="fr"
                                           @change="pedalChange">
                                </el-switch>
                                <div v-if="cadenceOrTorque">
                                    <img class="img" src="../../../assets/images/diagnosis/speed.png">
                                    <div class="img inline">
                                        <div class="cadence-shade"></div>
                                        <IEcharts :option="cadenceOption"></IEcharts>
                                    </div>
                                    <div class="clear-10"></div>
                                    <img class="display-ok fr"
                                         src="../../../assets/images/diagnosis/icon-test-light.png"
                                         v-if="lightFlag.cadence">
                                    <img class="display-ok fr"
                                         src="../../../assets/images/diagnosis/icon-test-unlight.png" v-else>
                                    <div class="clear-0"></div>
                                    <div class="warning-row">
                                        <img class="warning-img" src="../../../assets/images/diagnosis/icon-waring.png">
                                        <div class="inline ml-26">
                                            <span class="dot"></span> Suspend the bicycle to prevent injury or
                                                                      damage.<br />
                                            <span class="dot"></span> Make sure the wheel is free to rotate!
                                        </div>
                                    </div>
                                    <div class="clear-20"></div>
                                    Please turn the pedal forward by hand, then you should see green light.
                                    <!-- <span class="mark-span" :class="{ active: markSpan.cadence }"></span> <br/> If no green light, please click -->
                                    If no green light,please click
                                    <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                    </el-button>
                                    and then replace it.<br />
                                </div>
                                <div v-else>
                                    <img class="old-icon" src="../../../assets/images/diagnosis/torque.png">
                                    <div class="img inline">
                                        <ve-line :data="sensorData" :width="'220px'" :height="'220px'"
                                                 :yAxis="sensorData.yAxis" :colors="sensorData.colors"></ve-line>
                                        <el-button class="start-btn" type="primary" @click="sensorStart()">
                                            Start Torque
                                        </el-button>
                                    </div>
                                    <div class="clear-10"></div>
                                    <img class="display-ok fr"
                                         src="../../../assets/images/diagnosis/icon-test-light.png"
                                         v-if="lightFlag.torque">
                                    <img class="display-ok fr"
                                         src="../../../assets/images/diagnosis/icon-test-unlight.png" v-else>
                                    <div class="clear-0"></div>
                                    Please squeeze the brake level and place a foot on the pedal with around
                                    <div class="clear-10"></div>
                                    10kg force, then you should see green light.
                                    <div class="clear-20"></div>
                                    If no green light, please click
                                    <el-button class="fail" icon="close" @click="changeOption(testSelect,2)">Failed
                                    </el-button>
                                    and then replace it.<br />
                                </div>
                                <div class="clear-0"></div>
                            </div>
                            <!-- test system 手动测试的下方按钮 -->
                            <div class="btn-row" v-if="!switchTest && testSelectDisplay.length > 0">
                                <span>
                                    <el-button class="gray-button"
                                               @click="showTestDiv(testSelectDisplay[currentIndex - 1], currentIndex -1)"
                                               v-if="testSelect.No !== testSelectDisplay[0].No">&lt; Back</el-button>
                                    <input style="opacity: 0; width: 1px; height: 1px" ref="empty-input" />
                                </span>
                                <span>
                                    <template
                                            v-if="testSelect.No !== lastShowTestNo">
                                            <el-button class="gray-button"
                                                       @click="showTestDiv(testSelectDisplay[currentIndex + 1], currentIndex + 1)">Next &gt;</el-button>
                                             <el-button class="gray-button"
                                                        @click="showTestDiv('', -1)">Cancel</el-button>
                                        </template>
                                        <template v-else>
                                            <el-button class="gray-button" @click="showTestDiv('', -1)">Finish</el-button>
                                    </template>
                                </span>
                            </div>
                        </div>

                    </template>

                </div>
                <!--内容右侧 end-->
            </div>
        </div>
        <!--测试项选择-->
        <el-dialog title="Select" :visible.sync="testSelectDialogVisible"
                   custom-class="auth-dialog"
                   :show-close="false"
                   :close-on-click-modal="false">
            <p class="auth-title">Test System </p>
            <el-checkbox-group v-model="testSelectDisplay">
                <el-checkbox v-for="item in systemList" :key="item.name" :label="item" @change="changeCheckBox">
                    {{item.name}}
                </el-checkbox>
            </el-checkbox-group>
            <div class="clear-20 "></div>
            <div class="fr">
                <el-button @click="testSelectDialogClose()">Cancel</el-button>
                <el-button @click="testSelectDialogVisible = false">Confirm</el-button>
            </div>
            <div class="clear-20 "></div>

        </el-dialog>

        <!--error弹窗-->
        <el-dialog title="Error Solution" custom-class="error-dialog"
                   :visible.sync="dialogSolutionVisible">
            <div class="error-shooting">
                <div class="content-item">
                    <label class="title">Name</label>
                    <div class="desc">
                        <span>{{error.name}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">Remark</label>
                    <div class="desc">
                        <span>{{error.remark}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">Description</label>
                    <div class="desc">
                        <span>{{error.desc}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">Target</label>
                    <div class="desc">
                        <span>{{error.target ? error.target.join(',') : ''}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">Solution</label>
                    <div class="desc">
                        <p v-for="(item, index) in error.solution">{{index + 1}}. {{item}}</p>
                    </div>
                </div>
            </div>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="dialogSolutionVisible = false">Cancel</el-button>
            </span>
        </el-dialog>
    </div>

</template>

<script>
    import Core from 'core/core'
    import Port from  'device/conn'
    import Diagnose from 'device/diagnose'
    import IEcharts from 'vue-echarts-v3/src/full';
    import itemDetail from "src/components/3rd-diagnosis/item-detail";

    export default {
        components: {
            IEcharts,
            itemDetail: itemDetail
        },
        data() {
            return {
                isConnected: false,
                com: '',
                orderInfo: '',
                isBindBat: false,


                itemList: [
                    {
                        No: 8,
                        name: "HMI",
                        img: require("assets/images/diagnosis/icon-new-hmi.png"),
                        loaded: true,
                        percentage: 100,
                        status: 0,
                        update: false,
                    },
                    {
                        No: 9,
                        name: "Controller",
                        img: require("assets/images/diagnosis/icon-new-controller.png"),
                        loaded: true,
                        percentage: 100,
                        status: 0,
                        update: false
                    },
                    {
                        No: 10,
                        name: "Battery",
                        img: require("assets/images/diagnosis/icon-new-battery.png"),
                        loaded: true,
                        percentage: 100,
                        status: 0,
                        update: false
                    },
                    {
                        No: 11,
                        name: "Pedal Sensor",
                        img: require("assets/images/diagnosis/icon-new-pedal-sensor.png"),
                        loaded: true,
                        percentage: 100,
                        status: 0,
                        update: false,
                    }
                ],
                systemList: [
                    {
                        No: 0,
                        name: "Throttle",
                        img: require("assets/images/diagnosis/icon-new-throttle.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 1,
                        name: "Brake",
                        img: require("assets/images/diagnosis/icon-new-brake.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 2,
                        name: "Light",
                        img: require("assets/images/diagnosis/icon-new-light.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 3,
                        name: "Support",
                        img: require("assets/images/diagnosis/icon-new-support.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 4,
                        name: "Walk Assistance",
                        img: require("assets/images/diagnosis/icon-new-walk-assistance.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 5,
                        name: "Speed Signal",
                        img: require("assets/images/diagnosis/icon-new-speed-signal.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 6,
                        name: "Cadence Pedal",
                        img: require("assets/images/diagnosis/icon-new-pedal-sensor.png"),
                        detail: false,
                        okOrFail: 0,
                    },
                    {
                        No: 7,
                        name: "Motor",
                        img: require("assets/images/diagnosis/icon-new-motor.png"),
                        detail: true,
                        status: 0,
                        okOrFail: 0
                    }
                ],

                testSelectDisplay: [],
                connectionSelectDisplay: [],
                testSelectDialogVisible: false,
                forClose: '',

                //点击视频下方的test按钮开始test system
                startTest: false,
                switchTest: true,
                testSelect: "",
                slideDisplay: true,

                // 两个表格
                cadenceData: [],
                cadenceOption: {
                    colors: ["#E26829"],
                    grid: {
                        left: "-10%",
                        right: "4%",
                        bottom: "13%",
                        containLabel: true
                    },
                    xAxis: {
                        type: "category",
                        data: ["", "", "", "", "", "", ""]
                    },
                    yAxis: {
                        type: "value",
                        min: 0,
                        max: 0.02
                    },
                    series: [
                        {
                            name: "Cadence",
                            type: "line",
                            step: "end",
                            data: [0, 0, 0, 0, 0, 0, 0, 0]
                        }
                    ]
                },
                sensorData: {
                    colors: ["#98B42C"],
                    columns: ["time", "voltage"],
                    rows: [],
                    yAxis: {
                        type: "value",
                        splitLine: {
                            show: true
                        },
                        min: 0,
                        max: 5
                    }
                },
                sensorStartTime: 0,

                //switch选的是true还是false true是cadence
                cadenceOrTorque: true,

                //添加一个数据  用来区别是点击item还是detail触发
                detailVisible: false,
                // 控制显示test 详情
                testInfoVisible: false,

                //诊断部分数据
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
                    brake: '',
                    wheelSpeed: '',
                },

                //上一页面传过来的 5个部件的数据
                hmiInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    speedLimit: '',
                    wheelDiameter: '',
                    totalMileage: '',
                    totalGear: '',
                    clientCodeRead: ''
                },
                controllerInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    manufacturer: '',
                    nmlVolt: '',
                    maxCurrent: '',
                },
                batteryInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    soc: '',
                    soh: '',
                    fullChargerCapacity: '',
                    leftCapacity: '',
                    temperature: '',
                    current: '',
                    loopTime: '',
                    cell1to7: '',
                    cell8to14: '',
                    lastCharge: '',
                    maxChargeInterval: '',
                    packVoltage: '',
                },
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
                rpmSensorInfo: {},

                vehicleSn: '',
                vehicleDetail: '',
                componentList: '',

                // report 数据
                // 1.vehicle {id, sn, itemName, secretKey}     // 上个页面给
                // 2.hmiInfo,ctrlInfo,batteryInfo, motorInfo,pedalSensorInfo  //前3个上个页面给
                //3 testData, result, bomList,   bomList 自己访问接口拿， 剩下2个 这个个页面给
                // connectList, testList, allConnectList, allTestList  // allConnectList 上个页面给

                //结果数据
                result: {
                    hmiOK: true,
                    controllerOK: true,
                    batteryOK: true,
                    motorOK: true,
                    pedalSensorOK: true,
                    rpmSensorOK: true,
                    brakeOK: true,
                    lightOK: true,
                    walkOK: true,
                    throttleOK: true,
                    supportOK: true,

                    hmi: '',
                    controller: '',
                    battery: '',
                    motor: '',
                    pedalSensor: '',
                    rpmSensor: '',
                    brake: '',
                    light: '',
                    walk: '',
                    throttle: '',
                    support: '',
                },

                error: '',
                reportInfo: {},
                dialogComponentErrorVisible: false,

                // 控制灯亮的对象
                lightFlag: {
                    throttle: false,
                    brake: false,
                    light: false,
                    support: false,
                    walk: false,
                    speed: false,
                    cadence: false,
                    torque: false
                },

                vehicleId: '',
                currentIndex: -1,
                lightArr: [],

                dialogSolutionVisible: false,

            }
        },

        created: function () {
            let self = this;
            document.onkeydown = function (e) {
                let key = window.event.keyCode;
                if (key === 13) {
                    console.log('key enter');

                    //test 页面
                    if (!self.startTest && self.slideDisplay) {
                        self.beginTestSystem();
                        return;
                    }

                    // 进入了诊断
                    if (self.startTest && self.testInfoVisible && !self.switchTest) {
                        if (self.testSelect.No !== self.lastShowTestNo) {
                            self.showTestDiv(self.testSelectDisplay[self.currentIndex + 1], self.currentIndex + 1);
                        } else {
                            self.showTestDiv('',  -1);
                        }
                    }
                }

            }
        },

        mounted: function () {
            let params = this.$route.params || '';
            this.com = params.com;
            if(!this.com) {
                this.$message.error('params wrong');
                return;
            }
            this.orderInfo = params.orderInfo;
            this.isBindBat = params.isBindBat || false;

            this.testSelectDisplay = this.systemList;
            if (this.isBindBat == false) {
                this.connectionSelectDisplay = this.itemList.filter(item => {
                    return item.No !== 10;
                });
            } else {
                this.connectionSelectDisplay = this.itemList;

            }

            let reportInfo = params.report;
            if (reportInfo) {
                // 获取到 vehicle sn, hmiInfo, controllerInfo, batteryInfo result
                this.processReportData(reportInfo);
                // 获取到 bomList, vehicleDetail motorInfo, pedalSensorInfo
                this.getVehicleBySn(this.vehicleSn);
            }

        },

        destroyed: function() {
            document.onkeydown = function() {};
        },

        watch: {
            slideDisplay: function (val) {
                if (val) {
                    this.testInfoVisible = false;
                    this.detailVisible = false;
                    this.testSelect = '';
                }
            },
            testInfoVisible: function (val) {
                if (val) {
                    this.slideDisplay = false;
                    this.detailVisible = false;
                }
            },
            detailVisible: function (val) {
                if (val) {
                    this.testInfoVisible = false;
                    this.slideDisplay = false;
                }
            }
        },

        computed: {
            lastShowTestNo() {
                let length = this.testSelectDisplay.length;
                console.log(length, 'test length');
                if (length === 0) {
                    return -1;
                }
                if (length >= 2) {
                    return this.testSelectDisplay[length - 1].No === 7 ? this.testSelectDisplay[length - 2].No : this.testSelectDisplay[length - 1].No
                } else {
                    // 只有一个的情况下
                    return this.testSelectDisplay[0].No === 7 ? -1 : this.testSelectDisplay[0].No
                }
            },

            allFinish() {
                return this.testSelectDisplay.every(item => item.okOrFail !== 0)
            }
        },

        methods: {
            //根据No获取list里的对象
            getTargetItem(no, list) {
                let index = list.findIndex(item => {
                    return item.No === no
                });
                if (index > -1) {
                    return list[index]
                }
                return false
            },
            //计算速度
            calculateWheelSpeed(speed) {
                let wheelDiameter = this.hmiInfo.wheelDiameter ? this.hmiInfo.wheelDiameter : 0;
                // wheelDiameter = 26时 系数约  0.12442;
                return Math.round((Math.PI * wheelDiameter * 2.54 * 0.01) * speed * 0.06);
            },

            changeCheckBox() {
                this.testSelectDisplay.sort((a, b) => a.No - b.No);
            },

            clearData() {
            },

            //弹窗界面 1: connection 和  2:test system
            showItemSelectDialog() {
                if (this.startTest) {
                    return;
                }
                this.testSelectDialogVisible = true;
                this.forClose = this.testSelectDisplay;
            },

            //关闭弹窗界面  2:确认  1:关闭test system  3:关闭connection
            testSelectDialogClose() {
                this.testSelectDisplay = this.forClose;
                this.testSelectDialogVisible = false;
            },

            //点击栏目改变栏目背景的颜色
            showTestDiv(item, index) {
                if (!this.startTest) {return};
                if (!item) {
                    this.testInfoVisible = false;
                    this.currentIndex = -1;
                    this.testSelect = '';
                    return;
                }
                if (item !== this.testSelect) {
                    if (item.No < 8 && item.No >= 0) {
                        this.testInfoVisible = true;
                        this.testSelect = item;
                        this.currentIndex = index;
                    }
                } else {
                    if (this.detailVisible) {
                        this.testInfoVisible = true;
                        if (item.No < 8 && item.No >= 0) {
                            this.currentIndex = index;
                        }
                    } else {
                        this.testInfoVisible = false;
                        this.currentIndex = -1;
                        this.testSelect = '';
                    }
                }
                this.$nextTick(() => {
                    if (this.$refs['empty-input']) {
                        console.log( this.$refs['empty-input'])
                        this.$refs['empty-input'].focus();
                    }
                });

                console.log(this.testSelect, "item被点击");
            },

            //显示detail页面
            showDetail(item) {
                if (item !== this.testSelect) {
                    this.detailVisible = true;
                    this.testSelect = item;
                } else {
                    if (this.testInfoVisible) {
                        this.detailVisible = true;
                    } else {
                        this.detailVisible = false;
                        this.testSelect = '';
                    }
                }
                console.log(this.testSelect, "detail被点击");
            },
            closeDetail() {
                this.testSelect = "";
                this.detailVisible = false;
            },

            //是否显示滑动框中的内容
            showSlide(index) {
                this.slideDisplay = index !== 1;
            },

            //点击test开始test system测试
            beginTestSystem() {
                this.startTest = true;
                this.slideDisplay = false;
                if (this.testSelectDisplay.length <= 0) {
                    return;
                }
                if (!this.switchTest) {
                    this.showTestDiv(this.testSelectDisplay[0], 0);
                }
                this.readTestData();
            },

            //改变pedal类别
            pedalChange() {
                if (this.cadenceOrTorque == true) {
                    this.$set(this.testSelect, "name", "Cadence Pedal");
                } else {
                    this.$set(this.testSelect, "name", "Torque Pedal");
                }
            },

            reconnect() {
                // 理论触发不到
                console.log('reconnect');
            },

            //改变ok or fail
            changeOption(item, index) {
                if (!item) {
                    return;
                }
                this.$set(item, "okOrFail", index);
                let key = '';
                switch (item.No) {
                    case 0:
                        key = 'throttle';
                        break;
                    case 1:
                        key = 'brake';
                        break;
                    case 2:
                        key = 'light';
                        break;
                    case 3:
                        key = 'support';
                        break;
                    case 4:
                        key = 'walk';
                        break;
                    case 5:
                        key = 'speed';
                        break;
                    case 6:
                        key = 'pedal';
                        break;
                    case 7:
                        key = 'motor';
                        break;
                    case 8:
                        key = 'hmi';
                        break;
                    case 9:
                        key = 'ctrl';
                        break;
                    case 10:
                        key = 'bat';
                        break;

                }
                if (index === 1) {
                    this.setOK(key)
                }
                if (index === 2) {
                    this.setFail(key)
                }
            },
            setFail(str) {
                switch (str) {
                    case 'hmi':
                        this.result.hmiOK = false;
                        this.result.hmi = "Fail";
                        break;
                    case 'ctrl':
                        this.result.controllerOK = false;
                        this.result.controller = "Fail";
                        break;
                    case 'bat':
                        this.result.batteryOK = false;
                        this.result.battery = "Fail";
                        break;
                    case 'brake':
                        this.result.brakeOK = false;
                        this.result.brake = "Fail";
                        break;
                    case 'throttle':
                        this.result.throttleOK = false;
                        this.result.throttle = "Fail";
                        break;
                    case 'light':
                        this.result.lightOK = false;
                        this.result.light = "Fail";
                        break;
                    case 'support':
                        this.result.supportOK = false;
                        this.result.support = "Fail";
                        break;
                    case 'walk':
                        this.result.walkOK = false;
                        this.result.walk = "Fail";
                        break;
                    case 'speed':
                        this.result.rpmSensorOK = false;
                        this.result.rpmSensor = "Fail";
                        break;
                    case 'pedal':
                        this.result.pedalSensorOK = false;
                        this.result.pedalSensor = "Fail";
                        break;
                    case 'motor':
                        this.result.motorOK = false;
                        this.result.motor = "Fail";
                        break;
                }
            },
            setOK(str) {
                switch (str) {
                    case 'hmi':
                        this.result.hmiOK = true;
                        this.result.hmi = "OK";
                        break;
                    case 'ctrl':
                        this.result.controllerOK = true;
                        this.result.controller = "OK";
                        break;
                    case 'bat':
                        this.result.batteryOK = true;
                        this.result.battery = "OK";
                        break;
                    case 'brake':
                        this.result.brakeOK = true;
                        this.result.brake = "OK";
                        break;
                    case 'throttle':
                        this.result.throttleOK = true;
                        this.result.throttle = "OK";
                        break;
                    case 'light':
                        this.result.lightOK = true;
                        this.result.light = "OK";
                        break;
                    case 'support':
                        this.result.supportOK = true;
                        this.result.support = "OK";
                        break;
                    case 'walk':
                        this.result.walkOK = true;
                        this.result.walk = "OK";
                        break;
                    case 'speed':
                        this.result.rpmSensorOK = true;
                        this.result.rpmSensor = "OK";
                        break;
                    case 'pedal':
                        this.result.pedalSensorOK = true;
                        this.result.pedalSensor = "OK";
                        break;
                    case 'motor':
                        this.result.motorOK = true;
                        this.result.motor = "OK";
                        break;

                }
            },

            sensorStart() {
                this.sensorData.rows = [];
                this.sensorStartTime = new Date().getTime();
            },

            cadenceStart() {
                this.cadenceData.rows = [];
            },

            processReportData(reportInfo) {
                if (reportInfo) {
                    if (reportInfo.device) {
                        console.log(reportInfo.device, 'report device');
                        this.hmiInfo =  reportInfo.device.hmi;
                        this.controllerInfo = reportInfo.device.controller;
                        this.vehicleSn = reportInfo.device.vehicleSn;
                    }

                    if (reportInfo.result) {
                        let result = reportInfo.result;
                        this.result['hmiOK'] = result['hmiOK'];
                        this.result['controllerOK'] = result['controllerOK'];
                        this.result['batteryOK'] = result['batteryOK'];
                        this.result['hmi'] = result['hmi'];
                        this.result['controller'] = result['controller'];
                        this.result['battery'] = result['battery'];
                    }
                }
            },

            getVehicleBySn(sn) {
                sn = sn.trim();
                console.log("get vehicle", sn);
                Core.Api.Vehicle.vehicleDetailBySn(sn).then(
                    res => {
                        this.vehicleDetail = res.vehicle;
                        this.vehicleId = res.vehicle.id;
                        console.log('id', this.vehicleId)
                    }
                );
                Core.Api.Vehicle.getVehicleBomBySn(sn).then(res => {
                    this.componentList = res.bom;
                    this.getBomInfo(res.bom);
                });
            },

            //处理获取的bomList
            getBomInfo(bomList) {
                let $this = this;
                bomList.forEach(function (bom, index) {
                    console.log(bom);
                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.MOTOR) {
                        console.log($this.motorInfo, 'motor info');
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
                                $this.pedalSensorInfo.hardVersion = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "soft_ver") {
                                $this.pedalSensorInfo.version = attrList[index].value;
                            }
                        }
                    }
                });
            },


            //诊断
            readTestData() {
                let targetItem = {};
                let bus;
                bus = Diagnose.connect(this.com);
                bus.subscribe(res => {
                    if (res.type === 'connect') {
                        this.$message({
                            message: 'go into test mode successfully',
                            type: 'success'
                        });
                    }
                    if (!res.data || !res.data.test) {
                        return;
                    }
                    if ("error" in res.data.test) {
                        this.testData.error = Core.Util.get(res, 'data.test.error', this.testData.error);
                        console.log('获取到的error 是什么', this.testData.error);
                        if (this.testData.error) {
                            this.error = Core.Const.TEST_ERROR_CODE[parseInt(this.testData.error)] || '';
                            if (this.error && this.error.target) {
                                // 有错误，并且错误有指向。
                            } else {
                                this.dialogSolutionVisible = false;
                            }
                        }
                    }
                    if ("brake" in res.data.test) {
                        this.testData.brake = Core.Util.get(res, 'data.test.brake', this.testData.brake);
                        console.log('获取到的error brake是什么', this.testData.brake);
                        if (this.testSelect.No === 0 || this.switchTest) {
                            this.lightFlag.brake = this.testData.brake > 0;
                            if (this.testData.brake > 0) {
                                targetItem = this.getTargetItem(1, this.testSelectDisplay);
                                this.changeOption(targetItem, 1);
                            } else {
                                // this.changeOption(targetItem, 2);
                            }
                        }
                    }
                    if ("throttle" in res.data.test) {
                        this.testData.throttle = Core.Util.get(res, 'data.test.throttle', this.testData.throttle);
                        console.log('获取到的error throttle是什么', this.testData.throttle);
                        if (this.testSelect.No === 0 || this.switchTest) {
                            this.lightFlag.throttle = this.testData.throttle > 0;
                            if (this.testData.throttle > 0) {
                                targetItem = this.getTargetItem(0, this.testSelectDisplay);
                                this.changeOption(targetItem, 1);
                            } else {
                                // this.changeOption(targetItem, 2);
                            }
                        }
                    }
                    if ("light" in res.data.test) {
                        this.testData.light = Core.Util.get(res, 'data.test.light', this.testData.light);
                        if(this.lightArr.indexOf(this.testData.light) < 0) {
                            this.lightArr.push(this.testData.light);
                        }
                        if (this.testSelect.No === 2 || this.switchTest) {
                            this.lightFlag.light = this.testData.light > 0;
                            console.log(this.lightFlag.light);

                            if (this.lightArr.indexOf(0) > -1 && this.lightArr.indexOf(1) > -1) {
                                targetItem = this.getTargetItem(2, this.testSelectDisplay);
                                this.changeOption(targetItem, 1);
                            } else {
                                // this.changeOption(targetItem, 2);
                            }
                        }
                    }
                    if ("gear" in res.data.test) {
                        // 第二次进入
                        if (this.currentGear) {
                            console.log('gear 1');
                            this.testData.gear = Core.Util.get(res, 'data.test.gear', this.testData.gear);
                            if (this.testSelect.No === 3 || this.switchTest) {
                                targetItem = this.getTargetItem(3, this.testSelectDisplay);
                                if (this.testData.gear !== this.currentGear) {
                                    this.lightFlag.support = true;
                                    this.changeOption(targetItem, 1)
                                }
                            }
                            this.currentGear = this.testData.gear;
                        }
                        this.currentGear = Core.Util.get(res, 'data.test.gear', this.testData.gear);
                        if (this.testSelect.No === 4 || this.switchTest) {
                            if (this.currentGear === 'walk') {
                                this.lightFlag.walk = true;
                                targetItem = this.getTargetItem(4, this.testSelectDisplay);
                                this.changeOption(targetItem, 1);
                                let cadenceItem = this.getTargetItem(6, this.testSelectDisplay);
                                if (cadenceItem.okOrFail === 1) {
                                    let motorItem = this.getTargetItem(7, this.testSelectDisplay);
                                    this.changeOption(motorItem, 1);
                                }
                            }
                        }
                    }
                    if ("speed" in res.data.test) {
                        this.testData.speed = Core.Util.get(res, 'data.test.speed', this.testData.speed);
                        if (this.testSelect.No === 5 || this.switchTest) {
                            targetItem = this.getTargetItem(5, this.testSelectDisplay);
                            if (this.testData.speed > 0) {
                                this.lightFlag.speed = true;
                                this.changeOption(targetItem, 1);
                            }
                        }
                        this.$set(this.testData, "wheelSpeed", this.calculateWheelSpeed(this.testData.speed));
                        console.log(this.testData.wheelSpeed, '赋值的速度值');
                    }

                    if ("sensor_torque" in res.data.test) {
                        this.testData.sensorTorque = Core.Util.get(res, 'data.test.sensor_torque', this.testData.sensorTorque);

                        if (this.testSelect.No === 6 || this.switchTest) {
                            targetItem = this.getTargetItem(6, this.testSelectDisplay);
                            if (!this.cadenceOrTorque && this.testData.sensorTorque > 0) {
                                this.lightFlag.torque = true;
                                this.changeOption(targetItem, 1)
                            }
                        }
                    }
                    if ("cadence" in res.data.test) {
                        this.testData.cadence = Core.Util.get(res, 'data.test.cadence', this.testData.cadence);
                        if (this.testSelect.No === 6 || this.switchTest) {
                            targetItem = this.getTargetItem(6, this.testSelectDisplay);
                            if (this.cadenceOrTorque && this.testData.cadence > 0) {
                                console.log(this.testData.cadence, 'read cadence');
                                this.lightFlag.cadence = true;
                                this.changeOption(targetItem, 1);
                                let walkItem = this.getTargetItem(4, this.testSelectDisplay);
                                if (walkItem.okOrFail === 1) {
                                    let motorItem = this.getTargetItem(7, this.testSelectDisplay);
                                    this.changeOption(motorItem, 1);
                                }
                            }
                        }
                    }
                    // 2个表格
                    if ('sensor_volt' in res.data.test) {
                        this.testData.sensorVoltage = Core.Util.get(res, 'data.test.sensor_volt', this.testData.sensorVoltage);
                        this.testData.sensorVoltage = this.testData.sensorVoltage.toFixed(2);
                        if (this.sensorStartTime !== 0) {
                            if ((new Date().getTime() - this.sensorStartTime) / 1000 < 60) {
                                this.sensorData.rows.push({time: '', voltage: this.testData.sensorVoltage});
                            }
                        }
                    }
                    // this.testData.cadence = Math.floor(Math.random() * 130)
                    if ('cadence' in res.data.test) {
                        // 添加的数值肯定大于0（后面求倒数） 小于等于1
                        if (this.cadenceData.push(this.testData.cadence) > 3) {
                            this.cadenceData.shift();
                        }
                        let cadenceX = [0];
                        this.cadenceData.forEach(item => {
                            let v = parseFloat(Core.Util.sprintf("%.3f", 1 / item));
                            cadenceX.push(v, 0);
                        });
                        cadenceX.push(0);
                        let maxCadence = cadenceX.max();
                        this.cadenceOption.yAxis.max = maxCadence > 0.02 ? Math.ceil(maxCadence * 100) / 100 : 0.02;
                        let i = 0;
                        this.cadenceOption.xAxis.data.length = 0;
                        this.cadenceOption.series[0].data.length = 0;
                        while (i < cadenceX.length) {
                            this.cadenceOption.xAxis.data.push('');
                            this.cadenceOption.series[0].data.push(cadenceX[i]);
                            i++;
                        }

                        console.log('diagnosis cadence', this.cadenceOption.xAxis.data, this.cadenceOption.series[0].data);
                    }

                    // 未用到的2个数据
                    this.testData.current = Core.Util.get(res, 'data.test.current', this.testData.current);
                    this.testData.capacity = Core.Util.get(res, 'data.test.capacity', this.testData.capacity);
                    if ('sensor_torque' in res.data.test) {
//                        receiveTorque = true;
                    }

                });
            },



            goBack() {
                Port.reset(100);
                let orderId = this.orderInfo.orderId;
                let itemId = this.orderInfo.itemId;
                let from = 'test';
                this.$router.push({
                    name: 'assemblerOrderProduce',
                    query: {"orderId": orderId, "itemId": itemId, "from": from, "isBindBat": this.isBindBat}
                });
            },

            goToReport() {
                this.reportInfo = {
                    device: {
                        vehicle: {
                            id: this.vehicleId,
                            sn: this.vehicleSn,
                            itemName: this.vehicleDetail && this.vehicleDetail.item.name,
                            secretKey: this.vehicleDetail && this.vehicleDetail.secret_key
                        },
                        hmi: this.hmiInfo,
                        controller: this.controllerInfo,
                        battery: this.batteryInfo,
                        motor: this.motorInfo,
                        pedalSensor: this.pedalSensorInfo,
                        rpmSensor: this.rpmSensorInfo
                    },
                    test: this.testData,
                    result: this.result,
                    bomList: this.componentList,
                    connectList: this.connectionSelectDisplay,
                    testList: this.testSelectDisplay,
                    allConnectList: this.itemList,
                    allTestList: this.systemList
                };
                console.log(this.reportInfo);
                //生产页面需要用到
                let orderId = this.orderInfo.orderId;
                let itemId = this.orderInfo.itemId;
                let from = 'produce';

                this.$router.push({
                    name: 'assemblerDiagnosisResult',
                    query: {report: this.reportInfo, "orderId": orderId, "itemId": itemId, "from": from, "isBindBat": this.isBindBat}
                })
            },

            //用不到
            saveReport() {
                this.reportInfo.device.motor = this.motorInfo;
                this.reportInfo.device.pedalSensor = this.pedalSensorInfo;
                this.reportInfo.device.rpmSensor = this.rpmSensorInfo; //这个暂时没使用
                this.reportInfo = {
                    device: {
                        vehicle: {
                            id: this.vehicleId,
                            sn: this.orderInfo.vehicleInfo.vehicleSn,
                        },
                        hmi: this.reportInfo.device.hmi,
                        controller: this.reportInfo.device.controller,
                        battery: this.reportInfo.device.battery,
                        motor: this.motorInfo,
                        pedalSensor: this.pedalSensorInfo,
                        rpmSensor: this.rpmSensorInfo
                    },
                    test: this.testData,
                    result: this.result,
                    bomList: this.reportInfo.bomList
                };
                Core.Api.Vehicle.saveVehicleReport(0, this.vehicleId, JSON.stringify(this.reportInfo), '', '', '')
                    .then(res => {
                            this.$message({
                                message: 'Save successfully',
                                type: 'success'
                            });
                            this.goBack();
                        }
                    );
            },
        },

    }
</script>

<style lang="scss">
    .assembler-diagnosis-test {
        font-size: 16px;
        .header.top-box {
            min-width: 920px;
            display: flex;
            flex-direction: row;
            justify-content: space-between;
            align-items: center;
            .select-label {
                display: flex;
                align-items: center;
                .btn-connect {
                    margin: 0 12px;
                    span {
                        font-size: 15px;
                        color: #ffffff;
                        padding: 7px 56px 7px 66px;
                    }
                }
                .tip {
                    display: inline-block;
                    border-radius: 50%;
                    width: 15px;
                    height: 15px;
                    margin-left: 5px;
                    background: #e26829;
                    &.connect {
                        background: #98b42c;
                    }
                }
                .el-select {
                    width: 200px;
                }
            }
            .search-item {
                white-space: nowrap;
                .input-sn {
                    width: 200px;
                    .el-input__inner {
                        text-align: center;
                        padding-right: 40px;
                    }
                }
            }
        }
        .content-wrap {
            padding: 10px 0;
            & > .content {
                padding-bottom: 20px;
                position: relative;
                width: auto;
                display: flex;
                margin-top: 3px;
                padding-left: 40px;
                .content-left {
                    display: flex;
                    flex-direction: column;
                    flex: 4;
                    align-items: flex-end;

                    .content-select {
                        background: #a1a5b1;
                        border-radius: 25px;
                        height: 34px;
                        display: flex;
                        align-items: center;
                        padding: 0 10px;
                        position: relative;
                        width: 384px;
                        justify-content: space-between;
                        &.connect {
                            background: #e26829;
                        }
                        div.select-item {
                            color: #fff;
                            width: 94px;
                            height: 17px;
                            background-color: transparent;
                            text-align: center;
                            border: 1px solid #fff;
                            margin-left: 9px;
                            border-radius: 25px;
                            line-height: 15px;
                            cursor: pointer;
                        }
                        .connection {
                            position: absolute;
                            left: 144px;
                            font-weight: 600;
                            font-size: 20px;
                        }
                        .uart {
                            border-radius: 25px;
                            font-weight: 400;
                            background-color: #98b42c;
                            padding: 0px 10px;
                            font-size: 14px;
                            color: #ffffff;
                            margin-right: 4px;
                        }
                    }
                    .connection-item {
                        margin-top: 14px;
                        margin-bottom: 7px;
                        .info-item {
                            width: 384px;
                            height: 34px;
                            background-color: #343844;
                            display: flex;
                            justify-content: space-between;
                            align-items: center;
                            margin-bottom: 11px;
                            color: #ffffff;
                            padding: 0 17px;
                            cursor: pointer;
                            &.select {
                                background: rgb(101, 111, 140);
                            }
                            .item-info {
                                display: flex;
                                align-items: center;
                                width: 144px;

                                .item-icon {
                                    width: 33px;
                                    height: 30px;
                                    display: flex;
                                    align-items: center;
                                    justify-content: center;
                                    img {
                                        max-width: 23px;
                                        max-height: 20px;
                                    }
                                }
                                .item-name {
                                    margin-left: 10px;
                                    font-size: 13px;
                                    color: #ffffff;
                                }
                            }
                            div.result-wrap {
                                width: 120px;
                                display: flex;
                                justify-content: center;
                                span {
                                    background-color: #98b42c;
                                    font-size: 16px;
                                    padding: 0px 20px;
                                    border-radius: 25px;
                                    &.fail {
                                        background-color: #fe1e1e;
                                    }
                                    &.new-version {
                                        padding: 0px 8px;
                                        background-color: #dea60b;
                                    }
                                    &.option {
                                        padding: 0 13px;
                                        background-color: #949494;
                                        margin-right: 2px;
                                        cursor: pointer;
                                        &.check {
                                            background-color: #98b42c;
                                        }
                                        &.fail {
                                            background-color: #fe1e1e;
                                        }
                                    }
                                }
                                .progress {
                                    border: none !important;
                                    .el-progress-bar__outer {
                                        border-radius: 25px;
                                        height: 11px !important;
                                        width: 115px !important;
                                    }
                                    .el-progress-bar__inner {
                                        background-color: #98b42c;
                                        // border-radius: 0;
                                    }
                                    .el-progress-bar__innerText {
                                        line-height: 30px;
                                        display: none;
                                    }
                                }
                            }
                            div.detail {
                                font-size: 14px;
                                display: flex;
                                margin-left: 27px;
                                align-items: center;
                                width: 57px;
                                height: 100%;
                                cursor: pointer;
                                span.txt {
                                    font-size: 14px;
                                }
                                span.arrow {
                                    margin-left: 11px;
                                    border-right: 1px solid;
                                    border-bottom: 1px solid;
                                    width: 8px;
                                    height: 8px;
                                    transform: rotate(-45deg);
                                }
                            }
                            div.replace-detail {
                                font-size: 14px;
                                display: flex;
                                margin-left: 27px;
                                width: 57px;
                                height: 100%;
                            }
                        }
                    }
                }
                .content-right {
                    display: flex;
                    flex-direction: column;
                    flex: 6;
                    position: relative;
                    align-items: flex-end;
                    min-width: 700px;
                    .btn-reset {
                        width: 128px;
                        align-self: center;
                    }
                    div.slide-content {
                        width: 600px;
                        min-height: 575px;
                        margin-top: 40px;
                        background-color: #383d49;
                        border: solid 1px #e26829;
                        border-right: solid 1px transparent;
                        position: absolute;
                        z-index: 4;
                        //后加的
                        display: flex;
                        flex-direction: column;
                        &.slide-hidden {
                            z-index: 0;
                            // right: 10px;
                            // width: 20px;
                            width: 35px;
                            & .slide-content-top {
                                width: 20px;
                            }
                        }
                        div.slide-content-top {
                            // margin-left: 8px;
                            padding-left: 8px;
                            height: 30px;
                            // width: 524px;
                            width: 100%;
                            display: flex;
                            align-items: center;
                            border-bottom: solid 1px #e26829;
                            &.slide-hidden {
                                border-bottom: solid 1px transparent;
                            }
                            span.slide-to-right {
                                cursor: pointer;
                                border-right: 2px solid #e26829;
                                border-bottom: 2px solid #e26829;
                                width: 12px;
                                height: 12px;
                                margin-right: -2px;
                                transform: rotate(-45deg);
                                &.slide-hidden {
                                    transform: rotate(135deg);
                                    position: absolute;
                                    // left: -14px;
                                    left: 8px;
                                    &:last-child {
                                        // left: -24px;
                                        left: 18px;
                                    }
                                }
                            }
                        }
                        div.slide-content-bottom {
                            padding: 13px 48px 13px 42px;
                            display: flex;
                            flex-direction: column;
                            flex: 1;
                            .slide-content-warn {
                                display: flex;
                                img {
                                    width: 31px;
                                    height: 31px;
                                    margin-top: 6px;
                                }
                                .warn-list {
                                    display: flex;
                                    flex-direction: column;
                                    margin-left: 20px;
                                    line-height: 27px;
                                    align-items: flex-start;
                                    .dot {
                                        font-size: 16px;
                                        position: relative;
                                        margin-left: 20px;
                                        &::before {
                                            content: "";
                                            position: absolute;
                                            display: inline-block;
                                            width: 5px;
                                            height: 5px;
                                            left: -15px;
                                            top: 11px;
                                            border-radius: 50%;
                                            background: white;
                                        }
                                    }
                                }
                                .fail-btn-holder {
                                    margin-top: 20px;
                                    margin-left: 20px;
                                    .fail-btn {
                                        width: 80px;
                                        text-align: center;
                                    }
                                }
                            }

                            .show-video {
                                margin-top: 14px;
                                margin-bottom: 12px;
                                position: relative;
                                display: flex;
                                align-items: center;
                                justify-content: center;
                                img {
                                    width: 475px;
                                    height: 266px;
                                    &.play-video {
                                        position: absolute;
                                        width: 208px;
                                        height: 208px;
                                    }
                                }
                            }
                            .system-txt {
                                display: flex;
                                flex-direction: column;
                                span.title {
                                    font-size: 29px;
                                    color: #e26829;
                                    font-weight: 700;
                                    vertical-align: middle;
                                    &.step {
                                        font-weight: 400;
                                        font-size: 16px;
                                        & span {
                                            color: #fff;
                                            font-size: 16px;
                                            &.next-line {
                                                margin-left: 62px;
                                                & .highlight {
                                                    font-size: 16px;
                                                    color: #e26829;
                                                }
                                            }
                                        }
                                    }
                                }
                                span.test-button {
                                    font-size: 26px;
                                    padding: 0px 40px;
                                    margin-top: 6px;
                                    background-color: #e26829;
                                    border-radius: 25px;
                                    align-self: flex-end;
                                    cursor: pointer;
                                }
                            }
                            .operate-button {
                                display: flex;
                                justify-content: space-between;
                                span {
                                    cursor: pointer;
                                    background-color: #e26829;
                                    padding: 6px 18px;
                                    font-size: 13px;
                                    color: #fff;
                                    border-radius: 25px;
                                    font-weight: 500;
                                }
                            }
                        }
                        & > .slide-content-bottom.help {
                            padding: 50px 60px 13px 60px;
                            justify-content: space-between;
                            .slide-content-warn {
                                flex-direction: column;
                                .warn-list:first-child {
                                    margin-top: 0px;
                                    .dot:first-child {
                                        margin-left: 0px;
                                        display: flex;
                                        align-items: center;
                                        &::before {
                                            top: 17px;
                                        }
                                        img {
                                            width: 31px;
                                            height: 31px;
                                            margin-left: 7px;
                                            margin-right: 7px;
                                        }
                                    }
                                }
                                .warn-list {
                                    margin-top: 33px;
                                    line-height: 26px;
                                    .dot {
                                        margin-left: 0;
                                    }
                                }
                            }
                        }
                        & > .slide-content-bottom.before-connect {
                            .system-txt {
                                span.title {
                                    margin-top: 20px;
                                    &.step {
                                        line-height: 24px;
                                        font-weight: 600;
                                    }
                                    .green {
                                        width: 21px;
                                        height: 21px;
                                        border-radius: 50%;
                                        position: relative;
                                        display: inline-block;
                                        background-color: #98b42c;
                                        top: 4px;
                                    }
                                }
                            }
                        }
                        & > .slide-content-bottom.after-connect {
                            padding: 13px 48px 13px 42px;
                            img {
                                width: 522px;
                                height: 261px;
                            }
                            .system-txt {
                                span.title {
                                    margin-top: 20px;
                                    &.step {
                                        line-height: 24px;
                                        font-weight: 600;
                                    }
                                }
                            }
                            .operate-button {
                                margin-top: 52px;
                                padding-right: 10px;
                                // justify-content: flex-end;
                                span {
                                    margin-left: 24px;

                                    &.help {
                                        padding: 6px 31px;
                                    }
                                }
                            }
                        }
                    }
                    div.slide-bottom {
                        margin-top: 26px;
                        padding-top: 50px;
                        width: 554px;
                        display: flex;
                        height: 550px;
                        flex-direction: column;
                        justify-content: space-between;
                        align-items: center;
                        align-self: center;
                        img {
                            width: 463px;
                            height: 323px;
                            opacity: 0.85;
                        }
                        .error-img {
                            width: 30px;
                            height: 30px;
                            cursor: pointer;
                            position: absolute;
                            right: calc(50% - 200px);
                            z-index: 3;
                        }
                        span.test-button {
                            font-size: 26px;
                            padding: 8px 50px;
                            background-color: #e26829;
                            border-bottom-left-radius: 25px;
                            align-self: flex-end;
                            border-top-left-radius: 25px;
                            cursor: pointer;
                        }
                    }
                    .item-detail {
                        position: relative;
                        z-index: 5;
                        top: 40px;
                        align-self: center;
                        // align-self: flex-end;
                    }
                    .test-system-item {
                        position: absolute;
                        z-index: 8;
                        top: 39px;
                        border: 2px solid #626262;
                        background: #2a2e3a;
                        width: 605px;
                        min-height: 579px;
                        padding: 34px;
                        display: flex;
                        flex-direction: column;
                        justify-content: space-between;
                        align-self: center;
                        .el-button.fail {
                            background: #949494;
                            padding: 3px 10px;
                            .el-icon-close {
                                color: red;
                                &::before {
                                    font-size: 12px;
                                }
                            }
                            &.size {
                                padding: 6px 18px;
                            }
                        }
                        .light-img {
                            width: 16px;
                            height: 16px;
                        }
                        img.display-ok {
                            width: 70px;
                            height: 66px;
                            margin-left: 20px;
                        }
                        .el-input {
                            .el-input__inner {
                                background: rgb(57, 61, 73);
                            }
                            .el-input-group__append {
                                background: rgb(57, 61, 73);
                            }
                        }
                        .speed {
                            img.display-ok {
                                margin-left: 0;
                            }
                        }
                        span {
                            &.start {
                                background: red;
                                border-radius: 25px;
                                padding: 0 7px;
                                font-size: 12px;
                            }
                            &.dot {
                                display: inline-block;
                                width: 5px;
                                height: 5px;
                                border-radius: 50%;
                                background: white;
                            }
                        }
                        .inline {
                            display: inline-block;
                        }
                        .pedal {
                            .old-icon {
                                width: 220px;
                                height: 220px;
                                background: white;
                                border-radius: 30px;
                                vertical-align: top;
                            }
                            .img {
                                width: 220px;
                                height: 220px;
                                background: white;
                                border-radius: 30px;
                                vertical-align: top;
                                .start-btn {
                                    position: absolute;
                                    margin-top: -30px;
                                    margin-left: 50px;
                                }
                                .cadence-shade {
                                    position: absolute;
                                    width: 10px;
                                    height: 160px;
                                    background: white;
                                    z-index: 100;
                                    margin-top: 30px;
                                }
                            }
                        }
                        .ml-66 {
                            margin-left: 66px;
                        }
                        .warning-row {
                            .warning-img {
                                vertical-align: top;
                                margin-top: 5px;
                            }
                        }

                        .btn-row {
                            display: flex;
                            justify-content: space-between;
                            bottom: 20px;
                        }
                    }
                }
            }
        }
        .el-dialog__wrapper {
            .auth-dialog {
                width: 500px;
                .el-dialog__body {
                    padding: 6px 40px 10px 40px;
                    .auth-title {
                        color: #fff;
                        padding: 10px 0;
                        font-weight: bold;
                    }
                }
            }
        }
        .error-dialog {
            // left: 45%;
            // transform: scale(0.75, 0.75);
            // -ms-transform: scale(0.75, 0.75); /* IE 9 */
            // -webkit-transform: scale(0.75, 0.75); /* Safari 和 Chrome */
            // -o-transform: scale(0.75, 0.75); /* Opera */
            // -moz-transform: scale(0.75, 0.75);
        }

    }
</style>



// WEBPACK FOOTER //
// assembler-diagnosis.vue?0f2ac791