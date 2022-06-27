<template>
    <div class="my-diagnosis-container">
        <div class="switch">
            <div class="select">
                <div class="select-area" :class="{active: isActive}" @click="selectItemDefect()">{{ $t('diagnosisElectronic')}}</div>
                <div class="select-area" :class="{active: !isActive}" @click="cancelSelectDefect()">{{ $t('diagnosisMechanic')}}</div>
            </div>
        </div>
        <div v-if="!dialogDefectVisible" class="switch-body">
            <div class="switch-container">
                <!--头部 begin-->
                <div class="header top-box">
                    <div class="select-label" v-if = "!usbHID">
                        <label class="title">COM</label>
                        <el-select v-model="com" placeholder="Select" @change="change" @click.native="getComList">
                            <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                            </el-option>
                        </el-select>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button class="btn-connect" type="primary" @click="connect()">{{ $t('cNam.connect')}}</el-button>
                    </div>
                    <div class="select-label" v-else>
                        <label class="title">USB HID</label>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button class="btn-connect" type="primary" @click="connect()">{{ $t('cNam.connect')}}</el-button>
                    </div>
                    <el-button class="btn-reset" type="primary" @click="reset" id="reset" >{{ $t('cNam.restart')}}</el-button>
                    <!--<el-button class="btn-reset" type="primary" @click="current()" >dianliu</el-button>-->
                    <!--<el-button class="btn-reset" type="primary" @click="test()" >測試</el-button>-->

                    <div class="search-item">
                        <label class="title">{{ $t('eBikeID')}}</label>
                        <el-input class="input-se input-sn" placeholder="SN" v-model="vehicleSn"></el-input>
                    </div>
                </div>
                <!--头部 end-->
                <!--内容 begin-->

                <div class="content-container">
                    <div class="content">
                        <!--内容左侧 begin-->
                        <div class="content-left">
                            <!--连接项 begin-->
                            <div class="content-select" :class="{connect: isConnected}">
                                <div class="select-item connect" @click="showItemSelectDialog(1)">{{ $t('cNam.select')}}</div>
                                <!--<span class="connection" @click = "readInfo">Read</span>-->
                                <span class="connection" style="cursor: pointer">
                                    <el-button class="report" type="success" @click="beginConnection" :disabled="!isConnected">{{ $t('cNam.read')}}</el-button>
                                </span>
                                <!--<span class="uart" v-if="isConnected  && !isCan">UART</span>-->
                                <!--<span class="uart" v-if="isConnected  && isCan">CAN</span>-->
                            </div>
                            <!-- <span>{{detailVisible}}{{slideDisplay}}{{canTest}}{{startRunning}}{{startTest}}</span> -->
                            <div class="connection-item">
                                <template v-for="(item, index) in connectionSelectDisplay">
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
                                            <!-- 还未加载完成 -->
                                            <template v-if="!item.loaded">
                                                <el-progress
                                                    v-if="item.percentage > 0 && item.percentage < 100"
                                                    :text-inside="true" :stroke-width="30"
                                                    :percentage="item.percentage" status="success"
                                                    class="progress"></el-progress>
                                            </template>
                                            <!-- 加载完成 -->
                                            <template v-if="item.loaded" @click.stop="">
                                                <!-- 加载结果 成功 -->
                                                <template v-if="item.status === 1">
                                                    <!-- 有新版本 -->
                                                    <span v-if="item.update" class="result new-version">{{ $t('newVer') }}</span>
                                                    <span class="result" v-if="!item.update">{{ $t('con') }}</span>
                                                </template>
                                                <!-- 加载结果 失败 -->
                                                <span v-if="item.status === -1" class="result fail">{{ $t('disCon') }}</span>
                                                <span v-if="item.status === 2" class="result new-version">{{ $t('cNam.note') }}</span>
                                            </template>
                                        </div>
                                        <div class="detail" v-if="item.name != 'Pedal Sensor'">
                                            <span v-if="item.status === 1">{{ $t('readOk') }}</span>
                                            <span v-if="item.status === -1">{{ $t('readFail') }}</span>
                                        </div>
                                        <div class="detail" v-else>
                                            <span class="txt" v-if="item.status === 1">{{ $t('readOk') }}</span>
                                            <span class="txt" v-if="isSenCan && item.status === -1">{{ $t('readFail') }}</span>
                                            <span class="txt" v-if="!isSenCan && item.status === 2">{{ $t('cNam.notice') }}</span>
                                        </div>
                                    </div>
                                </template>
                            </div>
                            <div class="clear-20"></div>
                            <!--测试项 begin-->
                            <div class="content-select" :class="{connect: canTest}">
                                <div class="select-item connect" @click="showItemSelectDialog(2)">{{ $t('cNam.select') }}</div>
                                <span class="connection"  style="cursor: pointer">
                                     <el-button class="report" type="success" @click="beginTestSystem" :disabled="!canTest">{{ $t('testSys') }}</el-button>
                                </span>
                                <!--<el-switch v-model="switchTest" on-color="#98b42c" off-color="#606161" on-text="All"-->
                                <!--off-text="S" :disabled="!canTest"></el-switch>-->
                            </div>
                            <div class="connection-item">
                                <template v-for="(item, index) in testSelectDisplay">
                                    <div class="info-item" @click="showTestDiv(item, index)" :class="{select: item.No == testSelect.No}">
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
                                              @click.stop="changeOption(item, 1)">{{ $t('ok') }}</span>
                                                <span class="option" :class="{fail: item.okOrFail == 2 }"
                                                      @click.stop="changeOption(item, 2)">{{ $t('fail') }}</span>
                                            </template>
                                        </div>
                                        <div class="detail" v-if="item.detail" @click.stop="showDetail(item)">
                                            <span class="txt">{{ $t('detail') }}</span>
                                            <span class="arrow"></span>
                                        </div>
                                        <div class="replace-detail" v-if="!item.detail">

                                        </div>
                                    </div>
                                </template>
                            </div>
                            <div style="text-align:center">
                                <el-button class="report" type="success" v-show="true" @click="goReport">
                                    {{ $t('createReport') }}
                                </el-button>
                            </div>
                            <div class="connection-item" v-show="false">
                                <div class="info-item">
                                    <el-button class="" type="primary" @click="readInfo_50()" >循环测试</el-button>{{countReadInfo}}
                                    <el-input class="" placeholder="7000" v-model="timeOut"></el-input>
                                </div>
                                <div class="info-item">
                                    <el-button class="" type="primary" @click="read_50()" >重复50测试</el-button>{{countRead_50Info}}
                                    <el-button class="" type="primary" @click="read_50_record()" >记录</el-button>
                                    <el-button class="" type="primary" @click="clear_50_record()" >清除</el-button>
                                    <el-input class="" placeholder="7000" v-model="timeOutRead"></el-input>
                                </div>
                            </div>
                        </div>
                        <!--内容左侧 end-->
                        <!--内容右侧 begin-->
                        <div class="content-right">

                            <!-- 滑动上面的内容 -->

                            <!-- 开始加载模块后就会消失 -->
                            <template v-if="!canTest">
                                <div class="slide-content" v-if="slideDisplay && !detailVisible">
                                    <!-- 滑块头部>> -->
                                    <div class="slide-content-top width" @click="showSlide(1)">
                                        <span class="slide-to-right"></span>
                                        <span class="slide-to-right"></span>
                                        <span style="margin-left: 40%">  {{ $t('instruction') }}</span>
                                    </div>
                                    <!-- 不同情况展现不同滑块内容 1.before  2.after 3.help 4.finish(finish是加载模块之后)-->
                                    <!-- 1. 在连接前的页面 -->
                                    <div class="slide-content-bottom before-connect" v-if="!isConnected">
                                        <img src="../../assets/images/diagnosis/icon-before-connect.png">
                                        <div class="system-txt">
                                            <span class="title step">{{ $t('step') }} 1 :
                                                <span>{{ $t('step11') }} </span><br>
                                                <span class="next-line" v-if="!isCan">{{ $t('step12') }}<br></span>
                                                <span class="next-line" >{{ $t('click') }}
                                                    <span class="highlight">{{ $t('cNam.connect') }}</span>". {{ $t('step13') }} </span>
                                                <div class="green"></div>
                                            </span>
                                        </div>
                                    </div>
                                    <!-- 2. 连接后的页面 -->
                                    <div class="slide-content-bottom after-connect" v-if="isConnected && !isHelp">
                                        <img src="../../assets/images/diagnosis/icon-after-connect.png">
                                        <div class="system-txt">
                                            <span class="title step">{{ $t('step') }} 1 :
                                                <span> {{ $t('step21') }}</span>
                                            </span>
                                            <span class="title step">{{ $t('step') }} 2 :
                                                <span> {{ $t('step22') }} </span>
                                            </span>
                                            <span class="title step" v-if="isCan">{{ $t('step') }} 3 :
                                                <span> {{ $t('step232') }}</span><br>
                                            </span>
                                            <span class="title step" v-else>{{ $t('step') }} 3 :
                                                <span> {{ $t('step23') }}</span><br>
                                            </span>
                                            <!--<span class="title step" v-else-if="data">{{ $t('step') }} 3 :-->
                                            <!--<span> {{ $t('step23') }}</span><br>-->
                                            <!--<span class="next-line"> {{ $t('step24') }}</span>-->
                                            <!--</span>-->
                                        </div>
                                        <div class="operate-button">
                                            <span class="help" @click="isHelp = true; connect()">{{ $t('help') }}</span>
                                            <span class="continue" @click="beginConnection">{{ $t('cNam.read') }}</span>
                                        </div>
                                    </div>
                                    <!-- 3. 点击help 显示的页面 -->
                                    <div class="slide-content-bottom help" v-if="isHelp">
                                        <div class="slide-content-warn">
                                            <div class="warn-list" v-if = "isCan">
                                                <span class="dot">{{ $t('case') }} 1: {{ $t('case111') }}</span>
                                                <span >{{ $t('case121') }}</span>
                                                <span class="">{{ $t('case17') }}<img src="../../assets/images/diagnosis/icon-meun.png">{{ $t('case181') }}</span>
                                            </div>
                                            <div class="warn-list" v-else>
                                                <span class="dot">{{ $t('case') }} 1: {{ $t('case11') }}</span>
                                                <span >{{ $t('case12') }}</span>
                                            </div>
                                            <!--<div class="warn-list" v-show = "!isCan">-->
                                            <!--<span class="dot">{{ $t('case') }} 2: {{ $t('case132') }}</span>-->
                                            <!--<span class="">{{ $t('case17') }}<img src="../../assets/images/diagnosis/icon-meun.png">{{ $t('case181') }}</span>-->
                                            <!--<span>{{ $t('case191') }}</span>-->
                                            <!--<span>{{ $t('case201') }}</span>-->
                                            <!--</div>-->
                                            <div class="warn-list" v-show = "!isCan">
                                                <span class="dot">{{ $t('case') }} 2: {{ $t('case13') }}<sup @click="pcMode = true"> <i  class="el-icon-view"></i> </sup> :</span>
                                                <span></span>
                                                <span class="">{{ $t('case17') }}<img src="../../assets/images/diagnosis/icon-meun.png">{{ $t('case18') }}</span>
                                                <!--<span>{{ $t('case19') }}</span>-->
                                                <span>{{ $t('case20') }}</span>
                                            </div>
                                            <div class="warn-list">
                                                <span>{{ $t('case21') }}</span>
                                            </div>
                                        </div>
                                        <div class="operate-button">
                                            <span class="cancel" @click="isHelp = false">{{ $t('cNam.cancel') }}</span>
                                            <span class="cancel" @click="beginConnection">{{ $t('connection') }}</span>
                                        </div>
                                    </div>
                                </div>
                                <!-- 滑动内容隐藏 -->
                                <div class="slide-content slide-hidden" v-if="!slideDisplay">
                                    <div class="slide-content-top slide-hidden height"  @click="showSlide(2)">
                                        <span class="slide-to-right slide-hidden"></span>
                                        <span class="slide-to-right slide-hidden"></span>
                                    </div>
                                </div>
                            </template>

                            <!-- 可以进入test 模式 test的滑块-->
                            <template v-if="canTest">
                                <div class="slide-content" v-if="slideDisplay">
                                    <div class="slide-content-top width" @click="showSlide(1)">
                                        <span class="slide-to-right"></span>
                                        <span class="slide-to-right"></span>
                                    </div>
                                    <div class="slide-content-bottom">
                                        <div class="slide-content-warn">
                                            <img src="../../assets/images/diagnosis/icon-warn.png">
                                            <div class="warn-list">
                                                <span class="dot">{{ $t('test11') }}</span>
                                                <span class="dot">{{ $t('test12') }}</span>
                                            </div>
                                        </div>
                                        <div class="show-video">
                                            <img src="../../assets/images/diagnosis/icon-under-pic.png">
                                            <img class="play-video" src="../../assets/images/diagnosis/icon-start-mp4.png">
                                        </div>
                                        <div class="system-txt">
                                            <span class="title ">{{ $t('testSys') }}</span>
                                            <span class="title step">{{ $t('step') }} 1:
                                                <span>{{ $t('test13') }}</span>
                                            </span>
                                            <!--<span class="title step">{{ $t('step') }} 2 :-->
                                            <!--<span>{{ $t('test14') }}</span>-->
                                            <!--</span>-->
                                            <span class="title step">{{ $t('step') }} 2:
                                                <span>{{ $t('test15') }}</span>
                                            </span>
                                            <!-- 点击开始测试 滑动栏消失 -->
                                            <span class="test-button" @click="beginTestSystem()">
                                                {{ $t('test') }}
                                            </span>
                                        </div>
                                    </div>
                                </div>
                                <div class="slide-content slide-hidden" v-if="!slideDisplay">
                                    <div class="slide-content-top slide-hidden height" @click="showSlide(2)">
                                        <span class="slide-to-right slide-hidden"></span>
                                        <span class="slide-to-right slide-hidden"></span>
                                    </div>
                                </div>
                            </template>

                            <!-- 小车图标  永远存在 并且当所有完成的时候 出现 create report按钮  -->
                            <div class="slide-bottom" v-if="!slideDisplay">
                                <img src="../../assets/images/diagnosis/icon-error.png" class="error-img"
                                     @click="dialogSolutionVisible = true"
                                     v-show="error.target">
                                <img src="../../assets/images/diagnosis/icon-new-bike.png" v-show="error.target">
                            </div>

                            <!-- 显示detail中的内容 默认0 不展现 -->
                            <template v-if="testSelect.No >= 7 && testSelect.No <= 11 && detailVisible">
                                <item-detail
                                    :vehicle-Sn="vehicleSn"
                                    :test-select="testSelect"
                                    :hmi-info="hmiInfo"
                                    :controller-info="controllerInfo"
                                    :battery-info="batteryInfo"
                                    :pedal-sensor-info="pedalSensorInfo"
                                    :motor-info="motorInfo"
                                    :com="com"
                                    :isCan="isCan"
                                    @close="closeDetail"
                                    @reconnect="reconnect"
                                    class="item-detail">
                                </item-detail>
                            </template>
                            <div class="clear-20"></div>
                            <!-- 选中 Test System后出现右边栏 -->
                            <template v-if=" testSelect.No< 8 && testSelect.No >= 0  && !detailVisible">
                                <div class="test-system-item">
                                    <div class="throttle" v-if="testSelect.No ==0">
                                        <img class="img" src="../../assets/images/diagnosis/throttle.png">
                                        <img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"
                                             v-if="lightFlag.throttle">
                                        <img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"
                                             v-else>
                                        <div class="clear-20"></div>
                                        {{ $t('start11') }}
                                        <div class="clear-40"></div>
                                        {{ $t('start13') }}
                                        <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}
                                        </el-button>
                                        {{ $t('start14') }}
                                        <div class="clear-20"></div>
                                    </div>
                                    <div class="brake" v-if="testSelect.No==1">
                                        <div>
                                            <div class="data mark">
                                                <p>{{ $t('analogLight') }}</p>
                                                <p>
                                                    <img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"
                                                         v-if="lightFlag.brake">
                                                    <img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"
                                                         v-else>
                                                </p>
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/brake.png">
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="gray-button"
                                                           @click="showTestDiv(testSelectDisplay[currentIndex + 1], currentIndex + 1)">{{ $t('start37') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>
                                                    {{ $t('brake01') }}
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <div>
                                            <div class="data mark">

                                                <el-button class="info" @click="changeOption(testSelect, 1)">{{ $t('brake07') }}</el-button>
                                                <br>
                                                <el-button type="info" style="" @click="breakReleaseFail = !breakReleaseFail; breakHoldOk = false " :class="{active:breakReleaseFail}">{{ $t('brake08') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>
                                                    <span>{{ $t('brake02') }}</span> <br>
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>
                                                    <span>{{ $t('brake06') }}</span><br/>
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                    </div>
                                    <div class="light" v-if="testSelect.No ==2">
                                        <div>
                                            <div class="data mark">
                                                <p>{{ $t('analogLight')}}</p>
                                                <p>
                                                    <img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"
                                                         v-if="lightFlag.light">
                                                    <img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"
                                                         v-else>
                                                </p>
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/light.png">
                                                <span>
                                                    {{ $t('light01')}}&nbsp<img src="../../assets/images/icon/head-light-inactive.png" class="light-img" /> / "+"
                                                </span>
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button type="info" @click="lightOnOk = !lightOnOk; lightOnFail = false"  :class="{active:lightOnOk}">{{ $t('light07') }}</el-button>
                                                <br>
                                                <el-button type="info" @click="lightOnFail = !lightOnFail; lightOnOk = false" :class="{active:lightOnFail}">{{ $t('light08') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p class="bold">
                                                    {{ $t('light04') }}
                                                </p>
                                                <p>
                                                    <span>{{ $t('light02') }}</span>
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="gray-button"
                                                           @click="showTestDiv(testSelectDisplay[currentIndex + 1], currentIndex + 1)">{{ $t('start37') }}</el-button>
                                                <br>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>
                                                    <span>{{ $t('light03') }}</span>
                                                    <br>
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button type="info" @click="lightOFFOk = !lightOFFOk; lightOFFFail = false" :class="{active:lightOFFOk}">{{ $t('light09') }}</el-button>
                                                <br>
                                                <el-button type="info" @click="lightOFFFail = !lightOFFFail; lightOFFOk = false" :class="{active:lightOFFFail}">{{ $t('light10') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p class="bold">
                                                    {{ $t('light05') }}
                                                </p>
                                                <p>
                                                    <span>{{ $t('light022') }}</span> <br>
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <span>{{ $t('light08') }}</span>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                    </div>
                                    <div class="support" v-if="testSelect.No === 3">
                                        <div>
                                            <div class="data mark">
                                                <p v-if="levelIsChecked">
                                                    <label>{{ $t('cNam.hmiLevel') }}:</label>
                                                    <el-input v-model="testLevel" style="width: 110px"></el-input>
                                                </p>
                                                <p v-else>
                                                    &nbsp
                                                </p>
                                                <!--<p>-->
                                                <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                                <!--v-if="lightFlag.support">-->
                                                <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"-->
                                                <!--v-else>-->
                                                <!--</p>-->
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/hmi.png">
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <!--{{ $t('start21') }}-->
                                        <!--<div>-->
                                        <!--<div class="data mark">-->
                                        <!--<span>&nbsp</span>-->
                                        <!--</div>-->
                                        <!--<div class="data img-wrap">-->
                                        <!--<span>{{ $t('support01') }}</span><br>-->
                                        <!--</div>-->
                                        <!--</div>-->
                                        <!--<div class="clear-10"></div>-->
                                        <!--<div>-->
                                        <!--<div class="data mark">-->
                                        <!--<el-button type="info" @click="levelUpOk = !levelUpOk; levelUpFail = false" :class="{active:levelUpOk}">加档正常</el-button>-->
                                        <!--<br>-->
                                        <!--<el-button type="info" @click="levelUpFail = !levelUpFail; levelUpOk = false" :class="{active:levelUpFail}">无法加档</el-button>-->
                                        <!--</div>-->
                                        <!--<div class="data img-wrap">-->
                                        <!--<span>连续按“+”，观测仪表直至最高档位； 连续按“-”， 观测仪表直至最低档位，系统自动判定OK，否则请在左侧点击“Fail”</span>-->
                                        <!--</div>-->
                                        <!--</div>-->
                                        <!--<div class="clear-20"></div>-->
                                        <!--<div>-->
                                        <!--<div class="data mark">-->
                                        <!--<el-button type="info" @click="levelDownOk = !levelDownOk; levelDownFail = false" :class="{active:levelDownOk}">减档正常</el-button>-->
                                        <!--<br>-->
                                        <!--<el-button type="info" @click="levelDownFail = !levelDownFail; levelDownOk = false" :class="{active:levelDownFail}">无法减档</el-button>-->
                                        <!--</div>-->
                                        <!--<div class="data img-wrap">-->
                                        <!--<span>连续按“-”，观测仪表直至最低档位，如果仪表档位和页面显示档位一致，则减档正常，请在左侧选择“减档正常“；否则为无法减档故障，请在左侧选择“无法减档“</span>-->
                                        <!--</div>-->
                                        <!--</div>-->
                                        <!--<div class="clear-20"></div>-->
                                        <div>
                                            <div class="data mark">
                                                <el-button class="success" icon="close" @click="changeOption(testSelect, 1)">{{ $t('support03') }}</el-button>
                                                <br>
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('support04') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                {{ $t('support02') }}
                                            </div>
                                        </div>
                                    </div>
                                    <div class="speed" v-if="testSelect.No == 4">
                                        <div>
                                            <div class="data mark">
                                                <label>{{ $t('cNam.conSpeed') }}:</label>
                                                <el-input v-model="testData.wheelSpeed" style="width: 110px">
                                                    <template slot="append">km/h</template>
                                                </el-input>
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/speed-middle.png">
                                                <img class="img" src="../../assets/images/diagnosis/speed-hub.png">
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="success" @click="changeOption(testSelect, 1)">{{ $t('speed04') }}</el-button>
                                                <br>
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('speed05') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <span>{{ $t('speed01') }}</span><br>
                                            </div>
                                        </div>
                                        <div class="clear-20"></div>
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                        <!--v-if="lightFlag.speed">-->
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"-->
                                        <!--v-else>-->
                                    </div>
                                    <div class="pedal" v-if="testSelect.No == 5">
                                        <el-switch v-model="cadenceOrTorque" on-text="S" off-text="T" class="fixSwitch"
                                                   @change="pedalChange">
                                        </el-switch>
                                        <div v-if="cadenceOrTorque">
                                            <div>
                                                <div class="data mark">
                                                    <label>{{ $t('cNam.conCadence') }}:</label>
                                                    <el-input v-model="testData.cadence" style="width:110px">
                                                        <template slot="append">RPM</template>
                                                    </el-input>
                                                </div>
                                                <div class="data img-wrap">
                                                    <img class="img" src="../../assets/images/diagnosis/speed.png">
                                                    <!--<div class="img inline">-->
                                                    <!--<div class="cadence-shade"></div>-->
                                                    <!--<IEcharts :option="cadenceOption"></IEcharts>-->
                                                    <!--</div>-->
                                                </div>
                                            </div>
                                            <div class="clear-10"></div>
                                            <div>
                                                <div class="data mark">
                                                    <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('cadence03') }}</el-button>
                                                </div>
                                                <div class="data img-wrap">
                                                    <span>{{ $t('cadence01') }}</span> <br>
                                                </div>
                                            </div>
                                            <!--<img class="display-ok fr pos" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                            <!--v-if="lightFlag.cadence">-->
                                            <!--<img class="display-ok fr pos"-->
                                            <!--src="../../assets/images/diagnosis/icon-test-unlight.png" v-else>-->
                                            <!--<div class="warning-row">-->
                                            <!--<img class="warning-img" src="../../assets/images/diagnosis/icon-waring.png">-->
                                            <!--<div class="inline ml-26">-->
                                            <!--<span class="dot"></span> {{ $t('start29') }} <br>-->
                                            <!--<span class="dot"></span> {{ $t('start30') }}-->
                                            <!--</div>-->
                                            <!--</div>-->
                                            <!-- <span class="mark-span" :class="{ active: markSpan.cadence }"></span> <br/> If no green light, please click -->
                                            <div class="clear-40"></div>
                                            {{ $t('torque02') }}
                                            <div>
                                                <span v-if="isSupport">This signal interval is long than normal. It would take more time or fail to make judgement </span>
                                            </div>
                                        </div>
                                        <div v-else>
                                            <div>
                                                <div class="data mark">
                                                    <label>{{ $t('cNam.conTorque') }}:</label>
                                                    <el-input v-model="testData.sensorVoltage" style="width:110px">
                                                        <template slot="append">V</template>
                                                    </el-input>
                                                </div>
                                                <div class="data img-wrap">
                                                    <img class="img" src="../../assets/images/diagnosis/speed.png">
                                                    <!--<img class="old-icon" src="../../assets/images/diagnosis/torque.png">-->
                                                    <!--<ve-line :data="sensorData" :width="'217px'" :height="'220px'"-->
                                                    <!--:yAxis="sensorData.yAxis" :colors="sensorData.colors"></ve-line>-->
                                                    <!--<IEcharts :option="torqueOption"></IEcharts>-->
                                                    <!--<el-button class="start-btn" type="primary" @click="sensorStart()">-->
                                                    <!--{{ $t('start33') }}-->
                                                    <!--</el-button>-->
                                                </div>
                                            </div>
                                            <div class="clear-10"></div>
                                            <div>
                                                <div class="data mark">
                                                    <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}</el-button>
                                                </div>
                                                <div class="data img-wrap">
                                                    <span>{{ $t('torque01') }}</span>
                                                </div>
                                            </div>
                                            <div class="clear-20"></div>
                                            <!--<img class="display-ok fr pos" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                            <!--v-if="lightFlag.torque">-->
                                            <!--<img class="display-ok fr pos"-->
                                            <!--src="../../assets/images/diagnosis/icon-test-unlight.png" v-else>-->
                                        </div>
                                    </div>
                                    <div class="walk-assistance " v-if="testSelect.No === 6">
                                        <div>
                                            <div class="data mark">
                                                <p v-if="levelIsChecked">
                                                    <label>{{ $t('cNam.hmiLevel') }}:</label>
                                                    <el-input v-model="testLevel">
                                                    </el-input>
                                                </p>
                                                <p>
                                                    <label>{{ $t('cNam.conSpeed') }}:</label>
                                                    <el-input v-model="testData.wheelSpeed" style="width: 110px">
                                                        <template slot="append">km/h</template>
                                                    </el-input>
                                                </p>
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/walk.png">
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="gray-button"
                                                >Note</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>
                                                    {{ $t('walk06') }}
                                                </p>
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('walk05') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>{{ $t('walk03') }}<img src="../../assets/images/diagnosis/icon-new-walk-assistance.png" class="light-img">
                                                    {{ $t('walk04') }}</p>
                                                <p>{{ $t('walk01') }}<img src="../../assets/images/diagnosis/icon-new-walk-assistance.png" class="light-img">
                                                    {{ $t('walk02') }}</p>
                                            </div>
                                        </div>
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                        <!--v-if="lightFlag.walk">-->
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"-->
                                        <!--v-else>-->
                                        <div class="clear-20"></div>
                                        <!--{{ $t('start24') }}-->
                                        <!--<img src="../../assets/images/diagnosis/icon-new-walk-assistance.png" class="light-img">-->
                                        <!--{{ $t('start25') }}-->
                                        <!--<div class="clear-20"></div>-->
                                        <!--{{ $t('start22') }}-->
                                        <!--<el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}-->
                                        <!--</el-button>-->
                                        <!--{{ $t('start23') }}<br />-->
                                        <!--<div class="clear-20"></div>-->
                                    </div>
                                    <div class="motor " v-if="testSelect.No == 7">
                                        <el-switch v-model="cadenceOrTorque" on-text="S" off-text="T" class="fixSwitch"
                                                   @change="pedalChange">
                                        </el-switch>
                                        <!--<div>-->
                                        <!--<div class="data mark">-->
                                        <!--<p class="txt">Result</p>-->
                                        <!--</div>-->
                                        <!--<div class="data img-wrap">-->
                                        <!--<p class="txt">Operation</p>-->
                                        <!--</div>-->
                                        <!--</div>-->
                                        <!--<div class="clear-20"></div>-->
                                        <div>
                                            <div class="data mark">
                                                <p>
                                                    <label>{{ $t('cNam.conSpeed') }}:</label>
                                                    <el-input v-model="testData.wheelSpeed" style="width: 110px">
                                                        <template slot="append">km/h</template>
                                                    </el-input>
                                                </p>
                                                <p>
                                                    <label>{{ $t('cNam.conCurrent') }}:</label>
                                                    <el-input v-model="testData.current" style="width: 110px">
                                                        <template slot="append">A</template>
                                                    </el-input>
                                                </p>
                                                <p v-if="cadenceOrTorque">
                                                    <label>{{ $t('cNam.conCadence') }}:</label>
                                                    <el-input v-model="testData.cadence" style="width: 110px">
                                                        <template slot="append">RPM</template>
                                                    </el-input>
                                                </p>
                                                <p v-else>
                                                    <label>{{ $t('cNam.conTorque') }}:</label>
                                                    <el-input v-model="testData.sensorVoltage" style="width: 110px">
                                                        <template slot="append">V</template>
                                                    </el-input>
                                                </p>
                                            </div>
                                            <div class="data img-wrap">
                                                <img class="img" src="../../assets/images/diagnosis/speed.png">
                                            </div>
                                        </div>
                                        <div class="clear-10"></div>
                                        <div>
                                            <div class="data mark">
                                                <!--<el-button type="success" @click="changeOption(testSelect, 1)">OK</el-button>-->
                                                <!--<br>-->
                                                <el-button class="fail" icon="close" @click="changeOption(testSelect, 2)">{{ $t('fail') }}</el-button>
                                            </div>
                                            <div class="data img-wrap">
                                                <p>{{ $t('motor01') }}</p>
                                                <p>{{ $t('motor02') }}</p>
                                                <p>{{ $t('motor03') }}</p>
                                            </div>
                                        </div>
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-light.png"-->
                                        <!--v-if="lightFlag.motor">-->
                                        <!--<img class="display-ok" src="../../assets/images/diagnosis/icon-test-unlight.png"-->
                                        <!--v-else>-->
                                        <!--<div class="warning-row">-->
                                        <!--<img class="warning-img" src="../../assets/images/diagnosis/icon-waring.png">-->
                                        <!--<div class="inline ml-26">-->
                                        <!--<span class="dot"></span> {{ $t('start29') }} <br>-->
                                        <!--<span class="dot"></span> {{ $t('start30') }}-->
                                        <!--</div>-->
                                        <!--</div>-->
                                        <div class="clear-20"></div>
                                    </div>
                                    <!-- test system 手动测试的下方按钮 -->
                                    <div class="btn-row" v-if="!switchTest && testSelectDisplay.length > 0">
                                        <span>
                                            <el-button class="gray-button"
                                                       @click="showTestDiv(testSelectDisplay[currentIndex - 1], currentIndex - 1)"
                                                       v-if="testSelect.No != testSelectDisplay[0].No">{{ $t('start36') }}</el-button>
                                            <input style="opacity: 0; width: 1px; height: 1px" ref="empty-input" />
                                        </span>
                                        <span>
                                            <template v-if="testSelect.No !== 7">
                                                <el-button class="gray-button"
                                                           @click="showTestDiv(testSelectDisplay[currentIndex + 1], currentIndex + 1)">{{ $t('start37') }}</el-button>
                                                <el-button class="gray-button"
                                                           @click="showTestDiv('', -1)">{{ $t('cNam.cancel') }}</el-button>
                                            </template>
                                            <template v-else>
                                                <el-button class="gray-button" @click="showTestDiv(testSelectDisplay[currentIndex + 1], -1)">{{ $t('finish') }}</el-button>
                                            </template>
                                        </span>
                                    </div>
                                </div>
                            </template>
                        </div>
                        <!--内容右侧 end-->
                    </div>
                </div>
                <!--内容 end-->

                <!--测试项选择-->
                <el-dialog :title="$t('cNam.select')" :visible.sync="testSelectDialogVisible"
                           custom-class="auth-dialog"
                           :show-close="false"
                           :close-on-click-modal="false">
                    <p class="auth-title">{{ $t('testSys') }}</p>
                    <el-checkbox-group v-model="testSelectDisplay">
                        <el-checkbox v-for="item in systemList" :key="item.name" :label="item" @change="changeCheckBox">
                            {{item.name}}
                        </el-checkbox>
                    </el-checkbox-group>
                    <div class="clear-20 "></div>
                    <div class="fr">
                        <el-button type="primary" @click="testSelectDialogClose(1)">{{ $t('cNam.cancel') }}</el-button>
                        <el-button type="preview" @click="testSelectDialogClose(2)">{{ $t('cNam.confirm') }}</el-button>
                    </div>
                    <div class="clear-20 "></div>
                </el-dialog>

                <!--连接项选择-->
                <el-dialog :title="$t('cNam.select')" :visible.sync="connectionSelectDialogVisible"
                           custom-class="auth-dialog"
                           :show-close="false"
                           :close-on-click-modal="false">
                    <p class="auth-title">{{ $t('cNam.component') }} </p>
                    <el-checkbox-group v-model="connectionSelectDisplay">
                        <el-checkbox v-for="item in itemList"  :key="item.name" :label="item" @change="changeConnectionBox" :disabled="item.required">
                            {{item.name}}
                        </el-checkbox>
                    </el-checkbox-group>
                    <div class="clear-20 "></div>
                    <div class="fr">
                        <el-button type="primary" @click="testSelectDialogClose(3)">{{ $t('cNam.cancel') }}</el-button>
                        <el-button type="preview" @click="testSelectDialogClose(2)">{{ $t('cNam.confirm') }}</el-button>
                    </div>
                    <div class="clear-20 "></div>
                </el-dialog>

                <!--error弹窗-->
                <el-dialog :title="$t('errTitle') " custom-class="error-dialog"
                           :visible.sync="dialogSolutionVisible">
                    <div class="error-shooting">
                        <div class="content-item">
                            <label class="title">{{ $t('name') }}</label>
                            <div class="desc">
                                <span>{{error.name}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('remark') }}</label>
                            <div class="desc">
                                <span>{{error.remark}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('desc') }}</label>
                            <div class="desc">
                                <span>{{error.desc}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('target') }}</label>
                            <div class="desc">
                                <span>{{error.target ? error.target.join(',') : ''}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('solution') }}</label>
                            <div class="desc">
                                <p v-for="(item, index) in error.solution">{{index + 1}}. {{item}}</p>
                            </div>
                        </div>
                    </div>
                    <span slot="footer" class="dialog-footer">
                        <el-button type="primary" @click="dialogSolutionVisible = false">{{ $t('cNam.cancel') }}</el-button>
                    </span>
                </el-dialog>

                <!-- Besst Tool 2-->
                <el-dialog :title="$t('toolTitle1')" custom-class="tip-dialog"
                           :visible.sync="besstTool">
                    <div class="">
                        <div class="">
                            <img src="../../assets/images/diagnosis/tool_2.png" height="300" width="300">
                        </div>
                        <div class="txt">
                        </div>
                        <h1>
                            {{ $t('tool11') }}
                        </h1>
                    </div>
                    <span slot="footer" class="dialog-footer">
                        <el-button type="primary" @click="besstTool = false">{{ $t('cNam.cancel') }}</el-button>
                    </span>
                </el-dialog>

                <!-- PC mode -->
                <el-dialog :title="$t('toolTitle2')" custom-class="tip-dialog"
                           :visible.sync="pcMode">
                    <div class="">
                        <h1>
                            {{ $t('tool12') }}
                        </h1>
                        <div class="txt">
                        </div>
                        <h1>{{ $t('tool13') }}</h1>
                        <div class="txt">
                            {{ $t('tool14') }}
                        </div>
                        <h1>{{ $t('tool15') }}</h1>
                        <div class="txt">
                            {{ $t('tool16') }}
                        </div>
                    </div>
                    <span slot="footer" class="dialog-footer">
                        <el-button type="primary" @click="pcMode = false">{{ $t('cNam.cancel') }}</el-button>
                    </span>
                </el-dialog>

                <!-- Open Can Controller -->
                <el-dialog :title="$t('toolTitle2')" custom-class="tip-dialog"
                           :visible.sync="usbCanControllerOpen">
                    <div class="">
                        <h1>
                            {{ usbCanControllerOpenMsg }}
                        </h1>
                    </div>
                    <span slot="footer" class="dialog-footer">
                        <el-button type="primary" @click="usbCanControllerOpen = false">{{ $t('cNam.cancel') }}</el-button>
                    </span>
                </el-dialog>

                <!-- Update Note -->
                <el-dialog title="Diagnosis Update Note" custom-class="tip-dialog"
                           :visible.sync="diagnosisUpdateNoteOpen">
                    <div class="notice19">
                        <h1>
                            {{ $t('notice19_01') }}
                        </h1>
                        <p>
                            {{ $t('notice19_02') }}
                        </p >
                        <p>
                            {{ $t('notice19_03') }}
                            <br>
                            {{ $t('notice19_04') }}
                        </p >
                        <p>
                            {{ $t('notice19_05') }}
                        </p >
                    </div>
                    <span slot="footer" class="dialog-footer">
                        <el-checkbox v-model="remindCheck" style="padding-right: 20px">{{ $t('notice19_00') }}</el-checkbox>
                        <el-button type="primary" @click="remindSevenDays">{{ $t('cNam.confirm') }}</el-button>
                    </span>
                </el-dialog>
            </div>

        </div>
        <div class="switch-body" v-else>
            <!-- 瑕疵选择弹窗-->
            <mechanical-diagnosis
                @skipShow="BeforeReport"
                :defectList ="defectList"
                :errorList ="errorList"
                :fileAll ="fileAll"
                :reportInfo="reportInfo"
                :problemText="problemText"
                ref="mechanical">
            </mechanical-diagnosis>
        </div>
        <el-dialog :visible.sync="showBeforeReport">
            <div> {{$t('goElectronic')}} </div>
            <br>
            <el-button type="primary" @click="selectItemDefect">{{ $t('diagnosisElectronic')}}</el-button>
            <el-button type="primary" @click="goBeforeReport">{{ $t('createReport')}}</el-button>
        </el-dialog>
        <el-dialog :visible.sync="showAfterReport">
            <div> {{$t('goMechanic')}} </div>
            <br>
            <el-button type="primary" @click="cancelSelectDefect">{{ $t('diagnosisMechanic')}}</el-button>
            <el-button type="primary" @click="goAfterReport">{{ $t('createReport')}}</el-button>
        </el-dialog>
    </div>
</template>
<script>
    import Core from 'core/core'
    import itemDetail from "./item-detail";
    import IEcharts from "vue-echarts-v3/src/full";
    import VCharts from "v-charts";
    import Battery from 'device/battery'
    import Sensor from 'device/sensor'
    import Controller from 'device/controller'
    import HMI from 'device/hmi'
    import Diagnosis from 'device/diagnose'
    import EN from "src/assets/lang/en";
    import ZH from "src/assets/lang/zh";
    import DE from "src/assets/lang/de";
    import NL from "src/assets/lang/nl";
    import mechanicalDiagnosis from "./mechanicalDiagnosis"

    export default {
        data() {
            return {
                //是否离线
                offLineFlag: false,

                //默认的数据栏  未经select筛选的所有数据
                //电池和传感器是没有升级版本的
                //loaded:是否加载完成; status: 1 成功, -1 失败, 0 init; update:是否要升级( 升级完才显示success为成功 )
                itemList: [
                    {
                        No: 8,
                        name: "HMI",
                        img: require("../../assets/images/diagnosis/icon-new-hmi.png"),
                        loaded: false,
                        percentage: 0,
                        status: 0,
                        update: false,
                        required: true
                    },
                    {
                        No: 9,
                        name: "Controller",
                        img: require("../../assets/images/diagnosis/icon-new-controller.png"),
                        loaded: false,
                        percentage: 0,
                        status: 0,
                        update: false,
                        required: true
                    },
                    {
                        No: 10,
                        name: "Battery",
                        img: require("../../assets/images/diagnosis/icon-new-battery.png"),
                        loaded: false,
                        percentage: 0,
                        status: 0,
                        update: false,
                        required: false
                    },
                    {
                        No: 11,
                        name: "Pedal Sensor",
                        img: require("../../assets/images/diagnosis/icon-new-pedal-sensor.png"),
                        loaded: false,
                        percentage: 0,
                        status: 0,
                        update: false,
                        required: false
                    }
                ],
                itemListSelect:false,
                // okOrFail  1:ok 2:fail 0:空白
                // 通过数据中的detail来控制页面是否显示detail按钮
                //由于Motor是有detail显示的  所以要将detail显示的要素 1.加载完成 2.加载成功 给他赋值
//                    {
//                        No: 0,
//                        name: "Throttle",
//                        img: require("../../assets/images/diagnosis/icon-new-throttle.png"),
//                        detail: false,
//                        okOrFail: 0,
//                        required: false
//                    },

                systemList: [
                    {
                        No: 1,
                        name: "Brake",
                        img: require("../../assets/images/diagnosis/icon-new-brake.png"),
                        detail: false,
                        okOrFail: 0,
                        required: false
                    },
                    {
                        No: 2,
                        name: "Light",
                        img: require("../../assets/images/diagnosis/icon-new-light.png"),
                        detail: false,
                        okOrFail: 0,
                        required: false
                    },
                    {
                        No: 3,
                        name: "Assist mode",
                        img: require("../../assets/images/diagnosis/icon-new-support.png"),
                        detail: false,
                        okOrFail: 0,
                        required: true
                    },
                    {
                        No: 4,
                        name: "Speed Signal",
                        img: require("../../assets/images/diagnosis/icon-new-speed-signal.png"),
                        detail: false,
                        okOrFail: 0,
                        required: true
                    },
                    {
                        No: 5,
                        name: "Cadence",
                        img: require("../../assets/images/diagnosis/icon-new-pedal-sensor.png"),
                        detail: false,
                        okOrFail: 0,
                        required: true
                    },
                    {
                        No: 6,
                        name: "Walk Assistance",
                        img: require("../../assets/images/diagnosis/icon-new-walk-assistance.png"),
                        detail: false,
                        okOrFail: 0,
                        required: true
                    },
                    {
                        No: 7,
                        name: "Motor",
                        img: require("../../assets/images/diagnosis/icon-new-motor.png"),
                        detail: true,
                        status: 0,
                        okOrFail: 0,
                        required: true
                    }
                ],
                systemListSelect:false,
                //test system经过select后显示的项目
                testSelectDisplay: [],
                connectionSelectDisplay: [],
                testSelectDialogVisible: false,
                connectionSelectDialogVisible: false,
                //手动还是自动测试
                switchTest: false,  // true 自动， false 手动

                // 栏目点击变颜色 0-7是test system 项;8-11是connection项
                testSelect: "",
                //滑动栏的内容是否显示
                slideDisplay: true,
                // 是否显示help
                isHelp: false,

                // 两个表格
                cadenceData: [],
                cadenceTime: [],
                cadenceOption: {
                    // title: {
                    //     text: 'Cadence Strength'
                    // },
                    colors: ["#E26829"],
                    // legend: {
                    //     data: ['Cadence']
                    // },
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
                            data: [0, 0, 0, 0, 0, 0, 0]
                        }
                    ]
                },
                torqueData: [],
                torqueOption: {
//                    title: {
//                        text: 'ECharts Hello World'
//                    },
                    colors: ["#E26829"],
                    tooltip: {},
                    grid: {
                        left: "-10%",
                        right: "4%",
                        bottom: "13%",
                        containLabel: true
                    },
                    xAxis: {
                        data: []
                    },
                    yAxis: {
                        type: "value",
                        min: 0,
                        max: 5
                    },
                    series: [
                        {
                            name: "Torque",
                            type: "line",
                            smooth: true,
                            itemStyle : {
                                normal : {
                                    color:'#98B42C',
                                    lineStyle:{
                                        color:'#98B42C'
                                    }
                                }
                            },
                            data: []
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
                // Can Sensor
                sensorVoltCount: 0,
                testBrakeCount: 0,
                testLevel: '',

                //点击connection开始连接
                startRunning: false,
                //点击视频下方的test按钮开始test system
                startTest: false,

                //switch选的是true还是false true是cadence
                cadenceOrTorque: true,

                //添加一个数据  用来区别是点击item还是detail触发
                detailVisible: false,
                // 控制显示test 详情
                testInfoVisible: false,

                // // 全部结束出现create report按钮
                // allFinish: false,

                // 页面控制---end

                //硬件配置
                config: {
                    hmi: {device: HMI, time: 6000, key: 'hmi'},
                    ctrl: {device: Controller, time: 7800, key: 'ctrl'},
                    bat: {device: Battery, time: 12000, key: 'bat'},
                    sen: {device: Sensor, time: 3000, key: 'sen'}
                },

                //硬件部分
                vehicleSn: '',
                vehicleId: '',
                vehicleDetail: '',
                componentList: [],

                device: HMI,
                isConnected: false,
                comList: [],
                com: "",

                //数据库数据
                motorInfo: {
                    sn: '',
                    name: '',
                    power: '',
                    voltage: '',
                    wheelSize: '',
                    rpm: '',
                    error: '',
                    tip: '',
                    note: ''
                },
                pedalSensorInfo: {
                    sn: '',
                    name: '',
                    version: '',
                    hardVersion: '',
                    error: '',
                    tip: '',
                    note: ''
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
                    clientCodeRead: '',
                    error: '',
                    tip: '',
                    note: ''
                },
                controllerInfo: {
                    sn: '',
                    model: '',
                    version: '',
                    hardVersion: '',
                    manufacturer: '',
                    nmlVolt: '',
                    maxCurrent: '',
                    error: '',
                    tip: '',
                    speedLimit: '',
                    wheelDiameter: '',
                    circumference: '',
                    note: ''
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
                    error: '',
                    packVoltage: '',
                    tip: '',
                    note: ''
                },

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
                    wheelSpeed: ''
                },

                //结果数据
                result: {
                    hmiOK: true,
                    controllerOK: true,
                    batteryOK: true,
                    pedalSensorOK: true,
                    rpmSensorOK: true,
                    motorOK: true,
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
                    support: '',
                    walk: '',
                    throttle: '',
                },

                //诊断错误弹窗
                dialogSolutionVisible: false,
                error: {},
                errorList: [],

                // 上传文件
                fileAll: {
                    video:[],
                    img:[],
                },
                // 问题描述

                //用于判断gear
                currentGear: '',
                // Can ctl walk status
                walkStatus: '',

                reportInfo: {},

                //读取的timeout
                timeoutList: [],

                // 控制灯亮的对象
                lightFlag: {
                    throttle: false,
                    brake: false,
                    light: false,
                    support: false,
                    walk: false,
                    speed: false,
                    cadence: false,
                    torque: false,
                    motor: false
                },
                hasLight: false,
                lightArr: [],
                supportArr: [],
                levelLowChecked: null,
                levelHighChecked: null,

                initLightStatus: null,
                breakHoldOk: false,
                breakHoldFail: false,
                breakReleaseOk: false,
                breakReleaseFail: false,
                lightOnOk: false,
                lightOnFail: false,
                lightOFFOk: false,
                lightOFFFail: false,
                levelUpOk: false,
                levelUpFail: false,
                levelDownOk: false,
                levelDownFail: false,

                levelIsChecked: false,

                currentIndex: -1,
                besstTool: false,
                pcMode: false,

                isCan: false,
                usbHID: false,
                readInterval: null,
                isReset: false,
                isRead: false,
                wheelDiameters: [],
                usbCanControllerOpen: false,
                usbCanControllerOpenMsg: '',
                usbCanControllerOpenBG: false,

                diagnosisUpdateNoteOpen: false,
                remindCheck: false,

                // Defect
                dialogDefectVisible: false,
                defectList: {},
                problemText: '',

                existList: [],

                isSenCan: true,
                subscription: null,
                step: {
                    canStep1: '',
                    canStep4: '',
                    canStep5: '',
                },
                showBeforeReport: false,
                showAfterReport: false,
                isActive: true,
                reconnectInterval: null,
                connectionIntervalDig: null,

                toolHardware: '',
                toolHardwareCount: 0,
                readToolHardInterval: null,
                listenInterval: null,
                listenIntervalOFF: null,

                countReadInfo: 0,
                countRead_50Info: 0,
                timeOut: 8000,
                timeOutRead: 7000,
                countInterval: null,
                countResetInterval: null,

                test50_record: [{a:2}],

                CTCount: [],
                isSupport: false,
            }
        },

        components: {
            itemDetail: itemDetail,
            IEcharts,
            mechanicalDiagnosis
        },

        i18n: {
            messages: {
                en: EN.Components.Diagnosis.diagnosis3rd,
                zh: ZH.Components.Diagnosis.diagnosis3rd,
                nl: NL.Components.Diagnosis.diagnosis3rd,
                de: DE.Components.Diagnosis.diagnosis3rd
            }
        },

        created: function () {
            let self = this;
            document.onkeydown = function (e) {
                let key = window.event.keyCode;
//                let id = window.event.target.id;
                if (key === 13) {

                    console.log('key enter');
//                    console.log('nodeName'+id);
                    // 串口未连接 return
                    if (!self.isConnected) {
                        self.connect();
                    }
                    // console.log(self.startRunning, self.slideDisplay, self.startTest, 'star ruing, slide visible, star test');
                    // console.log(self.testInfoVisible, 'test info visible');
                    // self.isConnected = this.isConnected;
                    // 未开始读取数据 侧边栏激活 兼容读取数据后回车事件 判断不在test模式下
                    if (!self.startRunning && self.slideDisplay && !self.startTest ) {
                        // 无论哪种情况，清空数据再次读取
                        var timer = setTimeout(()=>{
                            self.beginConnection();
                            clearTimeout(timer);
                        }, 200);
                        console.log("run 1");
                        return;
                    }

                    if ((self.startRunning && self.detailVisible && !self.startTest) && (self.testSelect && (self.testSelect.status < 0 || self.testSelect.update))) {
                        self.reconnect();
                        console.log("run 2");
                        return;
                    }

                    //test 页面
                    if (self.canTest && self.startRunning && self.slideDisplay) {
                        self.beginTestSystem();
                        console.log("run 3");
                        return;
                    }

                    // 进入了诊断
                    if (self.startTest && self.testInfoVisible && !self.switchTest) {
                        console.log(self.testSelect);
                        console.log("=======>");
                        if (self.testSelect.No !== self.lastShowTestNo) {
                            self.showTestDiv(self.testSelectDisplay[self.currentIndex + 1], self.currentIndex + 1);
                        } else {
                            self.showTestDiv('', -1);
                        }
                        console.log("run 5");
                    }

                    //  保存报告
                    if (self.allFinish){
                        self.goToReport();
                    }
                    console.log("enter 990");
                }
            }
        },

        mounted() {
            this.wheelDiameters = Core.Const.wheelDiameters;

            this.offLineFlag = Core.Data.getOffline();
            this.initTrans();
            this.testSelectDisplay = this.systemList;
            if(this.isCan){
                this.connectionSelectDisplay = this.itemList;
            }else{
                this.connectionSelectDisplay = this.itemList.slice(0,3);
            }

            this.checkConnection();

            this.step = {
                canStep1: this.$t('canStep1'),
                canStep4: this.$t('canStep4'),
                canStep5: this.$t('canStep5'),
            };

            this.toolHardware = Diagnosis.port.getBESST_Pro().hard_ver;
//            if( !this.toolHardware.trim() ){
//                this.checkToolHardware();
//            }
            this.toolVersionMsg();
            this.remindSevenDaysCheck();
        },

        beforeDestroy:function () {
            if(!this.isCan){
                Diagnosis.outTest();
            }
            if(this.subscription){
                this.subscription.unsubscribe();
            }
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalDig){
                clearInterval(this.connectionIntervalDig);
                clearInterval(this.connectionIntervalDig);
            }
            if(this.readToolHardInterval){
                clearInterval(this.readToolHardInterval);
                clearInterval(this.readToolHardInterval);
            }
            if(this.listenInterval){
                clearInterval(this.listenInterval);
                clearInterval(this.listenInterval);
            }
            if(this.listenIntervalOFF){
                clearInterval(this.listenIntervalOFF);
                clearInterval(this.listenIntervalOFF);
            }
            if(this.countInterval){
                clearInterval(this.countInterval);
                clearInterval(this.countInterval);
            }
            if(this.countResetInterval){
                clearInterval(this.countResetInterval);
                clearInterval(this.countResetInterval);
            }
            this.goOutDiagnosis();
            Diagnosis.port.setBoxOperation();
        },

        destroyed: function() {
            document.onkeydown = function() {};
            Diagnosis.busComplete();
        },

        watch: {
            vehicleSn: function (newValue, oldValue) {
                this.vehicleSn = newValue.replace(/\s+/g, "");
            },
            slideDisplay: function (val) {
                if (val) {
                    this.testInfoVisible = false;
                    this.detailVisible = false;
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
            },
            isConnected: function(newValue, oldValue) {
                let vm = this;
                if(newValue != oldValue && oldValue == true){
                    let count = 0;
                    Diagnosis.port.setConnection();
                    Diagnosis.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count>1 && Diagnosis.port.isConnected()){
                                vm.connect();
                            }
                            if(count>3 && Diagnosis.port.isConnected()){
                                console.log("Count stop",count);
                                clearInterval(vm.reconnectInterval);
                            }
                        },500);
                    }else{
                        if(vm.listenInterval){
                            clearInterval(vm.listenInterval);
                            vm.listenInterval = null;
                        }
                        if(vm.listenIntervalOFF){
                            clearInterval(vm.listenIntervalOFF);
                            vm.listenIntervalOFF = null;
                        }
                        if(vm.reconnectInterval){
                            clearInterval(vm.reconnectInterval);
                            vm.reconnectInterval = null;
                        }
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count === 1){
                                vm.toolHardware = '';
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
                                if(count>3 && Diagnosis.port.isConnected()){
                                    console.log("Count stop",count);
                                    clearInterval(vm.reconnectInterval);
                                    vm.reconnectInterval = null;
                                }
                            }
                        },500);
                    }
                }
            }
        },

        computed: {

            canTest(){
                // 能否进入test   仪表和控制ok 就可以

                let canStartTest = this.connectionSelectDisplay.every(item => {
                    if (item.No === 8 || item.No === 9) {
                        return item.status > 0  // 一定要成功的项
                    }
                    // pedal sensor 不考虑
                    if (!this.isCan && item.No === 11) {
                        return true  // 测不测都不影响的项
                    }
                    return item.status !== 0; // 要测试完成的项 无论失败与否
                });
                if (canStartTest) {
                    this.slideDisplay = true;
                }
                return canStartTest;
            },

            lastShowTestNo(){
                let length = this.testSelectDisplay.length;
                if (length === 0) {
                    return -1;
                }
                if (length >= 1) {
                    return this.testSelectDisplay[length - 1].No === 7 ? true : this.testSelectDisplay[length - 1].No
//                } else {
                    // 只有一个的情况下
//                    return this.testSelectDisplay[0].No === 7 ? -1 : this.testSelectDisplay[0].No
                }
            },

//            connectionStatus() {
//                return this.connectionSelectDisplay.every(item => {
//                    if (item.No !== 11) {
//                        return item.status > 0
//                    } else {
//                        return true
//                    }
//                });
//            },

            allFinish() {
                return this.testSelectDisplay.every(item => item.okOrFail !== 0)
            }
        },
        methods: {
            checkCTCount(){
                if(this.isSupport){
                    return true;
                }
                let time = new Date().getTime();
                this.CTCount.push(time);
                let temCT = this.CTCount;
                let len = this.CTCount.length;
                let sum = 0;
                if( len > 2) {
                    for (let l = 0; l < len - 1; l++) {
                        sum += temCT[l + 1] - temCT[l];
                    }
//                    console.log(sum / (len - 1));
                    if (sum / (len - 1) > 1000) {
                        this.isSupport = true;
                    }
                }
            },
            read_50_record(){
                if(this.test50_record.length > 0){
                    this.test50_record = this.test50_record.slice(1)
                    console.log(this.test50_record);
                    var len = this.test50_record.length;
                    var count = 0;
                    this.test50_record.forEach((item)=>{
                        console.log(JSON.stringify(item));
                        let tem = Object.values(item);
                        let check = false;
                        tem.forEach(t=>{
                            if(t=== -1) check = true;
                        })
                        if(check){
                            count++;
                        }
                    })
                    console.log("Read Fail Percent", Math.floor(100*(count/len)))
                }
            },
            clear_50_record(){
                this.test50_record = [];
            },
            read_50(){
                this.countRead_50Info = 0;
                let a = parseInt(this.timeOutRead);
                console.log(this.countResetInterval,"countResetInterval", typeof this.countResetInterval)
                if(this.countResetInterval){
                    console.log("true","countResetInterval");
                    clearInterval(this.countResetInterval);
                    this.countResetInterval = null;
                    return;
                }
                let vm = this;
                this.countResetInterval = setInterval(()=>{
                    console.log(vm.countRead_50Info,new Date(), new Date().getTime());
                    vm.countRead_50Info++;
//                    vm.reset();
//                    setTimeout(()=>{
                        vm.test50_record.push(
                            {"hmi": vm.getTargetItem(8, vm.connectionSelectDisplay).status,
                             "ctrl": vm.getTargetItem(9, vm.connectionSelectDisplay).status,
                             "bat": vm.getTargetItem(10, vm.connectionSelectDisplay).status,
                             "sen": vm.getTargetItem(11, vm.connectionSelectDisplay).status,
                        });

                        vm.clearReadData();
                        vm.readInfo();
//                    },2500);
                    if(vm.countRead_50Info>50){
                        clearInterval(vm.countResetInterval);
                    }
                },a)
            },
            readInfo_50(){
                this.countReadInfo = 0;
                let a = parseInt(this.timeOut);
                if(7000<a && a>10000){
                    this.message.error("循环时间比较短,7300-10000 合适");
                    return;
                }
                console.log(this.countInterval,"countInterval", typeof this.countInterval)
                if(this.countInterval){
                    console.log("true","countInterval");
                    clearInterval(this.countInterval);
                    this.countInterval = null;
                    return;
                }
                let vm = this;
                this.countInterval = setInterval(()=>{
                    console.log(vm.countReadInfo,new Date(), new Date().getTime());
                    vm.countReadInfo++;
                    vm.clearReadData();
                    vm.readInfo();
                    setTimeout(()=>{
                        vm.logRead();
                    },a-600);
                    if(vm.countReadInfo>1000){
                        clearInterval(vm.countInterval);
                    }
                },a)
            },
            toolVersionMsg(){
                if(this.toolHardware){

                }
            },
            BeforeReport(){
                console.log("showBeforeReport");
                this.showBeforeReport = true;
            },
            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalDig){
                    return;
                }
                vm.connectionIntervalDig = setInterval(()=>{
//                    console.log('checkConnection',new Date().getTime());

                    let connection = Diagnosis.port.getConnection();
                    if(connection.usb){
                        vm.usbHID = true;
                        this.isCan = !Diagnosis.port.checkUsbUart();
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalDig);
                        vm.connectionIntervalDig = null;
                    }
                },300)
            },

            init(){
                Diagnosis.port.startConnection();
                if(Diagnosis.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
                    console.log(" get USB Device Diagnosis");
                    if(this.isReset){
                        this.connect(1);
                    }else{
                        this.connect();
                    }
                }else{
                    console.log("COM HMI");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = Diagnosis.port.isConnected();
            },
            initTrans() {
                this.itemList[0].name = this.$t('cNam.hmi');
                this.itemList[1].name = this.$t('cNam.controller');
                this.itemList[2].name = this.$t('cNam.battery');
                this.itemList[3].name = this.$t('pedalSensor');

//                this.systemList[0].name = this.$t('throttle');
                this.systemList[0].name = this.$t('brake');
                this.systemList[1].name = this.$t('light');
                this.systemList[2].name = this.$t('support');
                this.systemList[3].name = this.$t('speed');
                this.systemList[4].name = this.$t('cadence');
                this.systemList[5].name = this.$t('walk');
                this.systemList[6].name = this.$t('motor');
            },
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

            //获取串口列表
            getComList() {
                if (this.device.port.getPort() != '' && this.device.port.isConnected()) {
                    this.com = this.device.port.getPort();
                    this.comList = [this.device.port.getPort()];
                    this.isConnected = this.device.port.isConnected();
                    return
                }
                this.device.port.getPortList().subscribe(res => {
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

            test(cmd,type){
                let vm = this;
                if(type===1){
                    setTimeout(()=>{
                        vm.device.port.checkUsbConnection(cmd,2);
                    },1000)
                }else{
                    this.device.port.checkUsbConnection(cmd,2);
                }
                console.log(cmd, "test");
            },

            current(){
                Diagnosis.port.usbSDK.client.checkComponents().then(res=>{
                    console.log(res);
                    if(res.message){
                        let str = res.message.substr(0,20);
                        console.log("dianliu",str);
                    }
                }).catch(err =>{
                    console.log(err);
                }).finally(()=>{
                });
            },

            checkToolHardware(){
                if(!this.isConnected){
                    return;
                }
                let vm = this;
                if( vm.toolHardwareCount === 0) {
                    if (vm.readToolHardInterval) {
                        return;
                    }
                    vm.readToolHardInterval = setInterval(() => {
                        console.log(this.toolHardware, "toolHardware");
                        vm.toolHardwareCount++;
                        Diagnosis.port.getUsbBoxBasic().subscribe(res => {
                            if (!res.data) {
                                return;
                            }
                            if (res.data.hasOwnProperty("hardware_version")) {
                                vm.toolHardware = res.data.hardware_version;
                                Diagnosis.port.setBESST_Pro('', '', vm.toolHardware);
                            }
                            if (res.data.hasOwnProperty("version")) {
                                Diagnosis.port.setBESST_Pro('', res.data.version, '');
                            }
                            if (res.data.hasOwnProperty("sn")) {
                                Diagnosis.port.setBESST_Pro(res.data.sn, '', '');
                            }
                        });
                        if (vm.toolHardware || vm.toolHardwareCount > 100) {
                            vm.toolHardwareCount = 0;
                            clearInterval(vm.readToolHardInterval);
                            vm.readToolHardInterval = null;
                        }
                    }, 500);
                }
            },

            listenCurrentOff(){
                if(!this.isConnected){
                    return;
                }
                let vm = this;
                if (vm.listenIntervalOFF) {
                    return false;
                }else{
                    vm.listenIntervalOFF = setInterval(() => {
                        Diagnosis.port.usbSDK.client.checkComponents().then(res=>{
                            console.log("listenIntervalOFF",Core.Util.time());
                            if(res.message){
                                // 330000 04 00000000 000000000...00
                                let str = res.message.substr(8,8);
                                let buf = Buffer.from(str, "hex");
                                let volt = buf[0]*256+buf[1];
                                if(volt < 200){
                                    Diagnosis.port.setBoxOperation();
                                    vm.test('0000');
                                    setTimeout(()=>{
                                        vm.listenCurrent();
                                    },300);
                                    clearInterval(vm.listenIntervalOFF);
                                    vm.listenIntervalOFF = null;
                                }
                                if(vm.isRead){
                                    console.log("listenIntervalOFF",vm.isRead);
                                    clearInterval(vm.listenIntervalOFF);
                                    vm.listenIntervalOFF = null;
                                }
                            }
                        }).catch(err =>{
                            console.log(err);
                        }).finally(()=>{
                        });
                    },800)
                }
            },

            listenCurrent(){
                if(!this.isConnected){
                    return;
                }
                let vm = this;
                if (vm.listenInterval) {
                    return false;
                }else{
                    vm.listenInterval = setInterval(() => {
                        Diagnosis.port.usbSDK.client.checkComponents().then(res=>{
//                            console.log("listenCurrent");
                            if(res.message){
                                // 330000 04 00000000 000000000...00
                                let str = res.message.substr(8,8);
                                let buf = Buffer.from(str, "hex");
                                let volt = buf[0]*256+buf[1];
                                if(volt > 200){
                                    Diagnosis.port.setBoxOperation();
                                    vm.test('0100');
                                    setTimeout(()=>{
                                        vm.listenCurrentOff();
                                    },300);
                                    clearInterval(vm.listenInterval);
                                    vm.listenInterval = null;
                                }
                                if(vm.isRead){
                                    console.log("listenCurrent",vm.isRead);
                                    clearInterval(vm.listenInterval);
                                    vm.listenInterval = null;
                                }
                            }
                        }).catch(err =>{
                            console.log(err);
                        }).finally(()=>{
                        });
                    },800)
                }
            },

            //连接com
            connect(type){

                if( this.usbHID ){
                    //  进入诊断

                    this.isCan = !Diagnosis.port.checkUsbUart();
                    this.isConnected = Diagnosis.port.isConnected();

                    if(!this.toolHardware.trim()){
                        this.checkToolHardware();
                        let vm = this;
                        setTimeout(()=>{
                            vm.connect();
                        },500);
                        return;
                    }else{
//                        if(this.toolHardware.indexOf('3.0.2') > -1 || this.toolHardware.indexOf('3.0.3') > -1 || this.toolHardware.indexOf('3.0.5') > -1 ){
//                            this.test('',type);
//                        }else{
//                            this.test('0100',type);
//                        }
                        this.test('',type);
                        console.log("toolHardware",this.toolHardware);
                        let toolHardwareTem = this.toolHardware.replace(/\s/g, "")
                        if( (toolHardwareTem === "BESST.UC3.0" || toolHardwareTem === "BESST.UC3.0.1") && !this.isReset){
                            this.listenCurrent();
                        }
                    }


                    if(this.isCan){
                        this.goToDiagnosis();

                        this.config.hmi.time = 2000;
                        this.config.ctrl.time = 2000;
                        this.config.bat.time = 2000;
                        this.config.sen.time = 1200;
                        this.isSenCan = false;
                        let time = new Date().getTime();
                        let isSenCanTime = Diagnosis.port.checkSensor().last_en_time;
                        if( time - isSenCanTime < 200 ){
                            this.isSenCan = true;
                        }
                    }else{
                        this.test('0101',type);
                        this.config.hmi.time = 6500;
                        this.config.ctrl.time = 7800;
                        this.config.bat.time = 10000;
                        this.config.sen.time = 1100;
//
//                        this.connectionSelectDisplay = this.itemList.filter((item)=>{
//                            return item.No != '11';
//                        });
                    }
                    console.log("usbHID connect", this.isCan);
                    Diagnosis.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type === 'error_usb') {
                            this.isConnected = false;
                        }
                    });
                }else{
//                    this.connectionSelectDisplay = this.itemList.filter((item)=>{
//                        return item.No != '11';
//                    });
                    if (!this.com) {
                        this.$message({
                            message: this.$t('cNam.selectComTip'),
                            type: 'warning'
                        });
                        return;
                    } else {
                        let bus = this.device.connect(this.com);
                        if(bus){
                            bus.subscribe(res => {
//                                console.log('con type', res.type);
                                if (res.type === 'open' || res.type === 'connect') {
                                    this.isConnected = true;
                                    Core.Data.setPort(this.com);
                                }
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                            });
                        }
                    }
                }
            },

            connectCheck(){

            },

            //连接com口变化  无逻辑
            change() {
                // console.log(this.value, "value");
            },

            //点击connection开启载入模块
            beginConnection() {
                this.startRunning = true;
                this.slideDisplay = false;
                this.readInfo();
            },

            //点击test开始test system测试
            beginTestSystem() {

//                this.testSelect = '';
                this.startTest = true;
                this.slideDisplay = false;
                if (this.testSelectDisplay.length <= 0) {
                    return;
                }
                if (!this.switchTest) {
                    this.showTestDiv(this.testSelectDisplay[0], 0);
                }

//                if(this.isCan){
//                    this.usbCanControllerOpen = true;
//                    this.goToDiagnosis();
//                    setTimeout(()=>{
//                        this.test('0000','beginTestSystem');
//                    },100);
//                    let vm = this;
//                    setTimeout(()=>{
//                        let time = new Date().getTime();
//                        let hasCon = Diagnosis.port.checkCtl();
////                        console.log('[Test]',time);
////                        console.log('[Test]',hasCon.last_en_time);
////                        console.log('[Test]',time - hasCon.last_en_time);
////                        console.log('[Test]',this.isCan);
//
//                        if(time - hasCon.last_en_time < 400){
//                            vm.usbCanControllerOpenMsg = vm.step.canStep1;
//
//                            let status = 1;
//                            let checkInterval = setInterval(()=>{
//                                let checkCtlRes = Diagnosis.port.checkCtl();
//                                let now = new Date().getTime();
////                                console.log('[Test]',now);
////                                console.log('[Test]',checkCtlRes.last_en_time);
////                                console.log('[Test]',now - hasCon.last_en_time);
//                                if(status === 1 || now - checkCtlRes.last_en_time > 300){
//                                    vm.usbCanControllerOpenMsg = vm.step.canStep4;
//                                    vm.usbCanControllerOpenBG = true;
//                                    status = 2;
//                                }else if( status === 2 && now - checkCtlRes.last_en_time < 180){
//                                    vm.usbCanControllerOpenMsg = vm.step.canStep5;
//                                    vm.usbCanControllerOpenBG = false;
//                                    vm.usbCanControllerOpen = false;
//                                    status = 3;
////                                    this.test('0100','beginTestSystem');
//                                }
//                                if(status === 3){
//                                    vm.usbCanControllerOpenBG = false;
//                                    clearInterval(checkInterval);
//                                }
////                                console.log('[Test]',status);
//
//                            },100)
//                        }else{
//                            vm.usbCanControllerOpenMsg = vm.step.canStep4;
//                        }
//                    },400)
//                }
                this.readTestData();
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
                        key = 'speed';
                        break;
                    case 5:
                        key = 'pedal';
                        break;
                    case 6:
                        key = 'walk';
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
                    case 11:
                        key = 'sen';
                        break;

                }
                if (index === 1) {
                    this.setOK(key)
                }
                if (index === 2) {
                    this.setFail(key)
                }
            },
            //点击栏目改变栏目背景的颜色
            showTestDiv(item, index) {
//                if (!this.startRunning && !this.startTest) {
//                    return;
//                }
                console.log(item, "item被点击");

                if (!item) {
                    this.testInfoVisible = false;
                    this.currentIndex = -1;
                    this.testSelect = '';
                    console.log(this.testInfoVisible);
                    return;
                }
                if (item.No == 1) {
                    this.testBrakeCount = 0;
                } else if (item.No == 2){
                    this.lightArr = [];
                    console.log("lightArr");
                } else if (item.No == 3){
                    this.supportArr = [];
                    console.log("supportArr");
                    let targetItem = this.getTargetItem(2, this.testSelectDisplay);
                    if(this.lightOnOk){
                        this.changeOption(targetItem, 1);
                    }
//                    console.log("hasLight",this.lightOnOk,targetItem);
                } else if (item.No == 4){
                    this.testData.wheelSpeed = '';
                    console.log("speed");
                } else if (item.No == 5){
                    this.cadenceData = [];
                    console.log("cadenceData");
                }
                if (item !== this.testSelect) {
                    if (item.No < 8 && item.No >= 0) {
                        this.testInfoVisible = true;
                        this.testSelect = item;
                        this.currentIndex = index;
                    } else {
                        this.currentIndex = -1;
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
//                        console.log( this.$refs['empty-input'])
                        this.$refs['empty-input'].focus();
                    }
                });
                console.log(this.testSelect, "item被点击");
            },
            //显示detail页面
            showDetail(item) {
//                if (!this.startRunning) {
//                    return
//                }
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
//                console.log(this.testSelect, "detail被点击");
            },
            //关闭detail页面
            closeDetail(index) {
                this.testSelect = "";
                this.detailVisible = false;
            },
            //是否显示滑动框中的内容
            showSlide(index) {
                if (index == 1) {
                    this.slideDisplay = false;
                } else {
//                    console.log('显示滑块', this.testSelect);
                    this.testSelect = '';
                    this.slideDisplay = true;
                    // if (this.testSelect.No !== undefined || this.testSelect.No !== '') {
                    //     this.testSelect = '';
                    //     this.detailVisible = false;
                    // }
                }
            },


            //弹窗界面 1: connection 和  2:test system
            showItemSelectDialog(index) {
                if (index == 1) {
//                    if (this.startRunning) {
//                        return;
//                    }
                    this.connectionSelectDialogVisible = true;
                    this.forClose = this.connectionSelectDisplay;
                } else {
//                    if (this.startTest || !this.canTest) {
//                        return;
//                    }
                    this.testSelectDialogVisible = true;
                    this.forClose = this.testSelectDisplay;
                }
            },
            //关闭弹窗界面  2:确认  1:关闭test system  3:关闭connection
            testSelectDialogClose(index) {
                if (index == 1) {
                    this.testSelectDisplay = this.forClose;
                }
                if (index == 3) {
                    this.connectionSelectDisplay = this.forClose;
                }
                // test select
                // if(this.testSelectDialogVisible ){
                //     Core.Data.setDiagnosisTest();
                // }
                this.testSelectDialogVisible = false;

                // connection select
                // if(this.connectionSelectDialogVisible ){
                //     Core.Data.setDiagnosisConnection();
                // }
                this.connectionSelectDialogVisible = false;
            },
            //下面两个改变checkbox的值
            changeCheckBox() {
                this.testSelectDisplay.sort((a, b) => a.No - b.No);
                // console.log(this.testSelectDisplay);
            },
            changeConnectionBox() {
                this.connectionSelectDisplay.sort((a, b) => a.No - b.No);
            },

            //改变pedal类别
            pedalChange() {
                if (this.cadenceOrTorque == true) {
                    this.$set(this.testSelect, "name", this.$t('cadence'));
                } else {
                    this.$set(this.testSelect, "name", this.$t('torque'));
                }
            },

            //根据bom sn获取车辆的sn
            getVehicleByBomSn(bomSn, type) {
                if (this.offLineFlag) {
                    return;
                }
                let sn = bomSn.trim();
                if (this.vehicleSn || !sn) {
                    return;
                }
                Core.Api.Component.componentDetailBySn(sn)
                    .then(res => {
                            switch (type) {
                                case 1:
                                    this.hmiInfo.tip = "";
                                    break;
                                case 2:
                                    this.controllerInfo.tip = "";
                                    break;
                                case 3:
                                    this.batteryInfo.tip = "";
                                    break;
                                case 4:
                                    if(this.isSenCan){
                                        this.pedalSensorInfo.tip = "";
                                    }
                                    break;
                            }
                            // 检查Component 是否存在数据库
                            return Core.Api.Vehicle.getVehicleByBomSn(sn)
                        }
                    ).then(res => {
                    if (res.vehicle !== null) {
                        this.vehicleSn = res.vehicle.sn;
                        this.getVehicle();
                    }
                }).catch(err => {
                    console.log("[componentDetailBySn]",err);
                    switch (type) {
                        case 1:
                            this.hmiInfo.tip = err.message;
                            this.existList.push(1);
                            break;
                        case 2:
                            this.controllerInfo.tip = err.message;
                            this.existList.push(2);
                            break;
                        case 3:
                            this.batteryInfo.tip = err.message;
                            this.existList.push(3);
                            break;
                        case 4:
                            if(this.isSenCan){
                                this.pedalSensorInfo.tip = err.message;
                                this.existList.push(4);
                            }
                            break;
                    }
                });
            },
            //根据车辆sn获取车辆详情
            getVehicle() {
                if (this.offLineFlag) {
                    return;
                }
                this.vehicleSn = this.vehicleSn.trim();
//                console.log("get vehicle", this.vehicleSn);
                Core.Api.Vehicle.vehicleDetailBySn(this.vehicleSn).then(
                    res => {
                        this.vehicleDetail = res.vehicle;
                        this.vehicleId = res.vehicle.id;
                        console.log('id', this.vehicleId)
                    }
                );
                Core.Api.Vehicle.getVehicleBomBySn(this.vehicleSn).then(res => {
//                    this.componentList = res.bom;
                    this.componentList = res.bom;
                    this.getBomInfo(res.bom);
                });
            },
            //处理获取的bomList
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
                    if(!$this.usbHID && !$this.isSenCan){
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
                    }
                });
            },

            //计算速度
            calculateWheelSpeed(speed) {
                let wheelDiameter = this.hmiInfo.wheelDiameter ? this.hmiInfo.wheelDiameter : 26;
                // wheelDiameter = 26时 系数约  0.12442;
                return Math.round((Math.PI * wheelDiameter * 2.54 * 0.01) * speed * 0.06);
            },
            goToDiagnosis() {
                Diagnosis.port.usbSDK.client.switchModel('01').then(res=>{
                    console.log(res);
                }).catch(err =>{

                    console.log(err,'goToDiagnosis');
                }).finally(()=>{
                    console.log('goToDiagnosis response');
                });
            },
            goOutDiagnosis() {
                Diagnosis.port.usbSDK.client.switchModel('00').then(res=>{
                    console.log(res);
                }).catch(err =>{

                    console.log(err,'goOutDiagnosis');
                }).finally(()=>{
                    console.log('goOutDiagnosis response');
                });
            },
            //读取硬件数据的方法
            readBase(device) {
                if (!this.isConnected) {
                    this.$message({
                        type: 'warning',
                        message: this.$t('cNam.comDisconnect')
                    });
                    return;
                }
                let vm = this;
                let target = '';
                switch (device) {
                    case HMI:
                        target = vm.getTargetItem(8, vm.connectionSelectDisplay);
                        setTimeout(function () {
                            console.log('complete hmi');
                            vm.result.hmiOK = !!vm.hmiInfo.sn && !!vm.hmiInfo.version && !!vm.hmiInfo.hardVersion && !!vm.hmiInfo.model;
                            vm.result.hmi = vm.result.hmiOK ? 'OK' : 'Fail';
                            if (target) {
                                target.loaded = true;
                                if (vm.result.hmiOK) {
                                    vm.getVehicleByBomSn(vm.hmiInfo.sn, 1);
                                    target.status = 1;
                                    if (vm.hmiInfo.version && vm.hmiInfo.sn && !vm.offLineFlag) {
                                        Core.Api.Object.objectVersionCompare(vm.hmiInfo.sn, vm.hmiInfo.version).then(res => {
                                            target.update = res.flag === "4";
                                            if (!vm.detailVisible && target.update) {
                                                vm.showDetail(vm.itemList[0]);
                                            }
                                        });
                                    }
                                } else {
                                    if(!vm.hmiInfo.sn && !vm.hmiInfo.version && !vm.hmiInfo.hardVersion && !vm.hmiInfo.model){
                                        target.status = -1;
                                    }else{
                                        target.status = 2;
                                    }
                                    if( !vm.detailVisible && target.status == -1 ){
                                        vm.showDetail(vm.itemList[0]);
                                    }
                                }
                            }
//                            for(let key in vm.hmiInfo){
//                                if(vm.hmiInfo[key] == '')  vm.hmiInfo[key] = '-';
//                            }
                        }, vm.config.hmi.time);
//                        HMI.connect();
                        if (!this.isCan || HMI.port.checkUsbUart()) {
                            if(!this.usbHID){
                                HMI.connect(this.com);
                            }
                            HMI.sendCmdGetTestData();
                            HMI.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                                if (!res.data || !res.data.hmi) {
                                    return;
                                }
                                if (res.data.hmi.hasOwnProperty("sn")) {
                                    this.hmiInfo.sn = res.data.hmi.sn;
                                }
                                this.hmiInfo.model = Core.Util.get(res, 'data.hmi.model', this.hmiInfo.model);
                                this.hmiInfo.version = Core.Util.get(res, 'data.hmi.version', this.hmiInfo.version);
                                this.hmiInfo.hardVersion = Core.Util.get(res, 'data.hmi.hardware_version', this.hmiInfo.hardVersion);
                                this.hmiInfo.wheelDiameter = Core.Util.get(res, 'data.hmi.wheel_diameter', this.hmiInfo.wheelDiameter);
                                this.hmiInfo.speedLimit = Core.Util.get(res, 'data.hmi.speed_limit', this.hmiInfo.speedLimit);
//                                this.hmiInfo.totalMileage = Core.Util.get(res, 'data.hmi.total_mileage', this.hmiInfo.totalMileage);
                                this.hmiInfo.totalGear = Core.Util.get(res, 'data.hmi.total_gear', this.hmiInfo.totalGear);
                                if( this.hmiInfo.totalGear !=''){
                                    this.levelIsChecked = true;
                                    Diagnosis.setGearMode(this.hmiInfo.totalGear);
                                }

                                if (res.data.hmi.hasOwnProperty("client_code")) {
                                    this.hmiInfo.clientCodeRead = res.data.hmi.client_code
                                }
                            });
                        }else{
                            HMI.port.getUsbBasic(0x03).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    this.hmiInfo.sn = res.data.sn;
                                }
                                if (res.data.hasOwnProperty("model")) {
                                    this.hmiInfo.model = res.data.model;
                                }
                                if (res.data.hasOwnProperty("version")) {
                                    this.hmiInfo.version = res.data.version;
                                }
                                if (res.data.hasOwnProperty("hardware_version")) {
                                    this.hmiInfo.hardVersion = res.data.hardware_version;
                                }
                                if (res.data.hasOwnProperty("client_code")) {
                                    this.hmiInfo.clientCodeRead = res.data.client_code;
                                }
                            });

                            HMI.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05,0x03)
                                .then(res => {
//                                    console.log(res,'dataOfHmi_1 6301');
                                    let result_1 = Buffer.from(res.data, 'hex');
                                    this.hmiInfo.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2]*256*256;
                                }).catch(err =>{
                                console.log(err);
                            }).finally(()=>{
                            });
                            HMI.port.getListenData().subscribe((res)=> {
//                            console.log(res);
//                            //  code: "63", subCode: "00", dataLen: 4, data: "050b0001", can: "WRITE_CMD", …}
                                if (!res.data){
                                    return;
                                }
                                if (res.type === 'error_usb'){
                                    this.isConnected = false;
                                }
                                if (res.data.code === '63' && res.data.subCode === '00') {
                                    let temTotal = Buffer.from(res.data.data, 'hex');
                                    let temString_0 = HMI.port.addZero(temTotal[0].toString(2), 8);
                                    this.hmiInfo.totalGear = parseInt(temString_0.substr(4, 4), 2);
                                    if(this.hmiInfo.totalGear != ''){
                                        this.levelIsChecked = true;
                                        Diagnosis.setGearMode(this.hmiInfo.totalGear);
                                    }
                                }
                            });
                        }

                        break;
                    case Controller:
                        target = vm.getTargetItem(9, vm.connectionSelectDisplay);
                        setTimeout(function () {
                            console.log('complete ctrl');
                            vm.result.controllerOK = !!vm.controllerInfo.sn && !!vm.controllerInfo.version && !!vm.controllerInfo.hardVersion && !!vm.controllerInfo.model;
                            vm.result.controller = vm.result.controllerOK ? 'OK' : 'Fail';
                            if (target) {
                                target.loaded = true;
                                if (vm.result.controllerOK) {
                                    vm.getVehicleByBomSn(vm.controllerInfo.sn, 2);
                                    target.status = 1;
                                    if (vm.controllerInfo.version && vm.controllerInfo.sn && !vm.offLineFlag) {
                                        Core.Api.Object.objectVersionCompare(vm.controllerInfo.sn, vm.controllerInfo.version).then(res => {
                                            target.update = res.flag === "4";
                                            if( !vm.detailVisible && target.update ){
                                                vm.showDetail(vm.itemList[1]);
                                            }
                                        });
                                    }
                                } else {
                                    if(!vm.controllerInfo.sn && !vm.controllerInfo.version && !vm.hmiInfo.controllerInfo && !vm.controllerInfo.model){
                                        target.status = -1;
                                    }else{
                                        target.status = 2;
                                    }
                                    if( !vm.detailVisible && target.status == -1 ){
                                        vm.showDetail(vm.itemList[1]);
                                    }
                                }
                            }
//                            for(let key in vm.controllerInfo){
//                                if(vm.controllerInfo[key] == '')  vm.controllerInfo[key] = '-';
//                            }
                        }, vm.config.ctrl.time);

                        if (!this.isCan || Controller.port.checkUsbUart()) {
                            if(!this.usbHID){
                                Controller.connect(this.com);
                            }
                            Controller.sendCmdGetTestData();
                            Controller.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                                if (!res.data || !res.data.ctrl) {
                                    return;
                                }
//                                console.log('ctrl', res.data.ctrl)
                                this.controllerInfo.sn = Core.Util.get(res, 'data.ctrl.sn', this.controllerInfo.sn);
                                this.controllerInfo.model = Core.Util.get(res, 'data.ctrl.model', this.controllerInfo.model);
                                this.controllerInfo.version = Core.Util.get(res, 'data.ctrl.version', this.controllerInfo.version);
                                this.controllerInfo.manufacturer = Core.Util.get(res, 'data.ctrl.man_name', this.controllerInfo.manufacturer);
//                                this.controllerInfo.nmlVolt = Core.Util.get(res, 'data.ctrl.nml_volt', this.controllerInfo.nmlVolt);
//                                this.controllerInfo.maxCurrent = Core.Util.get(res, 'data.ctrl.max_cur', this.controllerInfo.maxCurrent);
                                this.controllerInfo.hardVersion = Core.Util.get(res, 'data.ctrl.hardware_version', this.controllerInfo.hardVersion);
//                                this.controllerInfo.spdMeterType = Core.Util.get(res, 'data.ctrl.spd_meter_type', this.controllerInfo.spdMeterType);
//                                this.controllerInfo.spdMeterSignal = Core.Util.get(res, 'data.ctrl.spd_meter_signal_num', this.controllerInfo.spdMeterSignal);
//                                if (res.data.ctrl.hasOwnProperty("wheel_dia")) {
//                                    this.controllerInfo.wheelDiameter = res.data.ctrl.wheel_dia / 2;
//                                }
                            });
                        }else{
                            Controller.port.getUsbBasic(0x02).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    this.controllerInfo.sn = res.data.sn;
                                }
                                if (res.data.hasOwnProperty("model")) {
                                    this.controllerInfo.model = res.data.model;
                                }
                                if (res.data.hasOwnProperty("version")) {
                                    this.controllerInfo.version = res.data.version;
                                }
                                if (res.data.hasOwnProperty("hardware_version")) {
                                    this.controllerInfo.hardVersion = res.data.hardware_version;
                                }
                                if (res.data.hasOwnProperty("client_code")) {
                                    this.controllerInfo.manufacturer = res.data.client_code;
                                }
                            });
                            Controller.port.getListenData().subscribe((res) => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.type === 'error_usb'){
                                    this.isConnected = false;
                                }
                                if (res.data.code === '32' && res.data.subCode === '03' && res.data.targetCode == 15) {
                                    let result_3 = Buffer.from(res.data.data, 'hex');
                                    this.controllerInfo.speedLimit = (result_3[0] + result_3[1] * 256)/100;
                                    this.wheelDiameters.find((item) => {
                                        if (item.value == res.data.data.substr(4,4).toUpperCase()) {
                                            this.controllerInfo.wheelDiameter = item.label;
                                        }
                                    });
                                    this.controllerInfo.circumference = result_3[4] + result_3[5] * 256;
                                }
                            });
                        }
                        break;
                    case Battery:
                        target = vm.getTargetItem(10, vm.connectionSelectDisplay);
                        setTimeout(function () {
                            vm.result.batteryOK = !!vm.batteryInfo.sn && !!vm.batteryInfo.version && !!vm.batteryInfo.hardVersion && !!vm.batteryInfo.model;
                            vm.result.battery = vm.result.batteryOK ? 'OK' : 'Fail';
                            if (target) {
                                target.loaded = true;
//                                target.status = vm.result.batteryOK ? 1 : -1;
                                if (vm.result.batteryOK) {
                                    vm.getVehicleByBomSn(vm.batteryInfo.sn, 3);
                                    target.status = 1;
                                    if (vm.batteryInfo.version && vm.batteryInfo.sn && !vm.offLineFlag && vm.isCan) {
                                        Core.Api.Object.objectVersionCompare(vm.batteryInfo.sn, vm.batteryInfo.version).then(res => {
                                            target.update = res.flag === "4";
                                            if( !vm.detailVisible && target.update ){
                                                vm.showDetail(vm.itemList[2]);
                                            }
                                        });
                                    }
                                } else {
                                    if(!vm.batteryInfo.sn && !vm.batteryInfo.version && !vm.batteryInfo.hardVersion && !vm.batteryInfo.model){
                                        target.status = -1;
                                    }else{
                                        target.status = 2;
                                    }
                                    if( !vm.detailVisible && target.status == -1 ){
                                        vm.showDetail(vm.itemList[2]);
                                    }
                                }
                            }
//                            for(let key in vm.batteryInfo){
//                                if(vm.batteryInfo[key] == '')  vm.batteryInfo[key] = '-';
//                            }
                        }, vm.config.bat.time);
                        if (!this.isCan || Battery.port.checkUsbUart()){
                            if(!this.usbHID){
                                Battery.connect(this.com);
                            }
                            Battery.sendCmdGetFullData();
                            Battery.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                                if (!res.data || !res.data.battery) {
                                    return;
                                }
//                                console.log('battery', res.data.battery);
                                // this.batteryInfo.sn = Core.Util.get(res, 'data.battery.sn', this.batteryInfo.sn);
                                this.batteryInfo.sn = Core.Util.get(res, 'data.battery.sn', this.batteryInfo.sn);
                                this.batteryInfo.model = Core.Util.get(res, 'data.battery.model', this.batteryInfo.model);
                                this.batteryInfo.version = Core.Util.get(res, 'data.battery.software_version', this.batteryInfo.version);
                                this.batteryInfo.hardVersion = Core.Util.get(res, 'data.battery.hardware_version', this.batteryInfo.hardVersion);
                                this.batteryInfo.soc = Core.Util.get(res, 'data.battery.relative_capacity', this.batteryInfo.soc);
                                this.batteryInfo.soh = Core.Util.get(res, 'data.battery.absolute_capacity', this.batteryInfo.soh);
                                this.batteryInfo.fullChargerCapacity = Core.Util.get(res, 'data.battery.full_capacity', this.batteryInfo.fullChargerCapacity);
                                this.batteryInfo.leftCapacity = Core.Util.get(res, 'data.battery.left_capacity', this.batteryInfo.leftCapacity);
                                this.batteryInfo.temperature = Core.Util.get(res, 'data.battery.inner_temperature', this.batteryInfo.temperature);
                                this.batteryInfo.current = Core.Util.get(res, 'data.battery.real_current', this.batteryInfo.current);
                                this.batteryInfo.loopTime = Core.Util.get(res, 'data.battery.loop_time', this.batteryInfo.loopTime);
                                this.batteryInfo.lastCharge = Core.Util.get(res, 'data.battery.cur_charge_interval', this.batteryInfo.lastCharge);
                                this.batteryInfo.maxChargeInterval = Core.Util.get(res, 'data.battery.max_charge_interval', this.batteryInfo.maxChargeInterval);
                                this.batteryInfo.packVoltage = Core.Util.get(res, 'data.battery.total_volt', this.batteryInfo.packVoltage);
                                if ("1to7" in res.data.battery) {
                                    let cell = res.data.battery["1to7"];
                                    this.batteryInfo.cell1to7 = cell.join();
                                }
                                if ("8to14" in res.data.battery) {
                                    let cell = res.data.battery["8to14"];
                                    this.batteryInfo.cell8to14 = cell.join();
                                }
                            });
                        }else{
                            Battery.port.getUsbBasic(0x04).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    this.batteryInfo.sn = res.data.sn;
                                }
                                if (res.data.hasOwnProperty("model")) {
                                    this.batteryInfo.model = res.data.model;
                                }
                                if (res.data.hasOwnProperty("version")) {
                                    this.batteryInfo.version = res.data.version;
                                }
                                if (res.data.hasOwnProperty("hardware_version")) {
                                    this.batteryInfo.hardVersion = res.data.hardware_version;
                                }
                            });

                            Battery.port.getBatteryOfData().subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("cycleLife")) {
                                    this.batteryInfo.loopTime= res.data.cycleLife;
                                }
                                if (res.data.hasOwnProperty("maxChargeInterval")) {
                                    this.batteryInfo.maxChargeInterval = res.data.maxChargeInterval;
                                }
                                if (res.data.hasOwnProperty("currentChargeInterval")) {
                                    this.batteryInfo.lastCharge = res.data.currentChargeInterval;
                                }
                                if (res.data.hasOwnProperty("cell1_7")) {
                                    this.batteryInfo.cell1to7 = res.data.cell1_7;
                                }
                                if (res.data.hasOwnProperty("cell8_14")) {
                                    this.batteryInfo.cell8to14 = res.data.cell8_14;
                                }
                            });
                            Battery.port.getListenData().subscribe((res) => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.type === 'error_usb'){
                                    this.isConnected = false;
                                }
                                if (res.data.code === '34' && res.data.subCode === '00') {
                                    let result = Buffer.from(res.data.data, 'hex');
                                    this.batteryInfo.fullChargerCapacity = result[0] + result[1] * 256;
                                    this.batteryInfo.leftCapacity = result[2] + result[3] * 256;
                                    this.batteryInfo.soc = result[4];
                                    this.batteryInfo.soh = result[5];
                                    //                            this.asoh = result[6];
                                }
                                if (res.data.code === '34' && res.data.subCode === '01') {
                                    let tem = res.data.data.substring(2, 4) + res.data.data.substring(0, 2);
                                    //  tem & 0x8000, 如果还是8000,就是负的
                                    let currentBinaryLen = parseInt(tem, 16).toString(2).length;
                                    // tem 取反加1，或者 0xFFFF-tem+1
                                    this.batteryInfo.current = currentBinaryLen < 16 ? parseInt(tem, 16) : -(~parseInt(tem, 16) + 1 + Math.pow(2, 16)) * 10;
                                    let result_2 = Buffer.from(res.data.data, 'hex');
                                    this.batteryInfo.packVoltage = (result_2[2] + result_2[3] * 256) * 10;
                                    this.batteryInfo.temperature = result_2[4] - 40;
                                }
                            });
                        }

                        break;
                    case Sensor:
                        target = vm.getTargetItem(11, vm.connectionSelectDisplay);
                        setTimeout(function () {
                            console.log('complete sensor');
                            vm.result.pedalSensorOK = !!vm.pedalSensorInfo.sn && !!vm.pedalSensorInfo.version && !!vm.pedalSensorInfo.hardVersion && !!vm.pedalSensorInfo.name;
                            vm.result.pedalSensor = vm.result.pedalSensorOK ? 'OK' : 'Fail';
                            if (target) {
                                target.loaded = true;
                                if (vm.result.pedalSensorOK) {
                                    vm.getVehicleByBomSn(vm.pedalSensorInfo.sn, 4);
                                    target.status = 1;
                                    if (vm.pedalSensorInfo.version && vm.pedalSensorInfo.sn && !vm.offLineFlag && vm.isCan) {
                                        Core.Api.Object.objectVersionCompare(vm.pedalSensorInfo.sn, vm.pedalSensorInfo.version).then(res => {
                                            target.update = res.flag === "4";
                                            if( !vm.detailVisible && target.update ){
                                                vm.showDetail(vm.itemList[3]);
                                            }
                                        });
                                    }
                                } else {
                                    if(!vm.pedalSensorInfo.sn && !vm.pedalSensorInfo.version && !vm.pedalSensorInfo.hardVersion && !vm.pedalSensorInfo.name){
                                        target.status = -1;
                                    }else{
                                        target.status = 2;
                                    }
                                    if( !vm.detailVisible && target.status == -1 ){
                                        vm.showDetail(vm.itemList[3]);
                                    }
                                }
                            }
//                            for(let key in vm.pedalSensorInfo){
//                                if(vm.pedalSensorInfo[key] == '')  vm.pedalSensorInfo[key] = '-';
//                            }
                        }, vm.config.sen.time);
                        Sensor.port.getUsbBasic(0x01).subscribe(res => {
                            if (!res.data) {
                                return;
                            }
                            if (res.data.hasOwnProperty("sn")) {
                                this.pedalSensorInfo.sn = res.data.sn;
                            }
                            if (res.data.hasOwnProperty("model")) {
                                this.pedalSensorInfo.name = res.data.model;
                            }
                            if (res.data.hasOwnProperty("version")) {
                                this.pedalSensorInfo.version = res.data.version;
                            }
                            if (res.data.hasOwnProperty("hardware_version")) {
                                this.pedalSensorInfo.hardVersion = res.data.hardware_version;
                            }
                        });
                        break;
                }
            },

//            showDetailCheck(no) {
//                for(let i=0; i<no; i++ ){
//                    if(this.itemList[i].update || this.itemList[i].status == -1){
//                        this.showDetail(vm.itemList[i]);
//                    }
//                }
//            },

            //获取选中的硬件
            getReadDeviceList() {
                let selectDevices = this.connectionSelectDisplay;
                if (selectDevices.length <= 0) {
                    this.$message.warning("selectDevice1");
                    return;
                }
                let config = this.config;
                let deviceList = [];

                selectDevices.forEach(item => {
                    switch (item.No) {
                        case 8:
                            deviceList.push(config.hmi);
                            break;
                        case 9:
                            deviceList.push(config.ctrl);
                            break;
                        case 10:
                            deviceList.push(config.bat);
                            break;
                        case 11:
                            deviceList.push(config.sen);
                            break;
                    }
                });
                // 需要获取 HMI  Controller Battery 中的一个
//                if (deviceList.length <= 0) {
//                    this.$message.warning("");
//                    return;
//                }
                console.log(deviceList);
                return deviceList;
            },

            //计算进度条
            startShowPercentage(target, time, start = 0) {
                target.percentage = start;
                let perChange = (100 / (time / 100)); // 0.01秒的变化值
                let bomPercentage = setInterval(function () {
                    target.loaded = false;
                    target.percentage = (target.percentage + perChange) > 100 ?
                        100 : Math.round((target.percentage + perChange) * 100) / 100;
                    if (target.percentage >= 100) {
                        target.loaded = true;
                        clearInterval(bomPercentage)
                    }
                }, 100);
            },

            switchElectronic(){
                if( this.$refs.mechanical ){
                    this.defectList = this.$refs.mechanical.selectedList;
                    this.errorList = this.$refs.mechanical.errorCodeList;
                    this.fileAll = this.$refs.mechanical.fileList;
                    this.problemText = this.$refs.mechanical.problemDescription;
                    let SnList = this.$refs.mechanical.SNList;
                    this.hmiInfo.sn = SnList.HMI;
                    this.controllerInfo.sn = SnList.controller;
                    this.batteryInfo.sn = SnList.battery;
                    this.pedalSensorInfo.sn = SnList.sensor;
                    this.motorInfo.sn = SnList.motor;
                    this.vehicleSn = SnList.bike;
//                    console.log(this.$refs.mechanical.selectedList);
//                    console.log(this.$refs.mechanical.errorCodeList);
//                    console.log(this.$refs.mechanical.fileList);
//                    console.log(this.$refs.mechanical.problemDescription);
                }
            },

            //  电子诊断
            selectItemDefect(){
                this.isActive = true;
                this.dialogDefectVisible = false;
                this.showBeforeReport = false;
                this.switchElectronic();
            },
            // 机械诊断
            cancelSelectDefect(){
                this.isActive = false;
                this.dialogDefectVisible = true;
                this.showAfterReport = false;
                this.createReportInfo();
            },
            //初始化数据
            reset(){
                this.isSupport = false;
                this.CTCount = [];
                Diagnosis.busComplete();
                if(this.subscription){
//                    console.log(this.subscription);
                    this.subscription.unsubscribe();
//                    console.log(this.subscription);
                    this.subscription = null;
//                    console.log(this.subscription);
                }
                console.log('clear 1660');
                this.isReset = true;
                this.isRead = false;
                setTimeout(()=>{
                    this.isReset = false;
                },1500);
                // this.isConnected = false;
                this.clearReadData();
                this.toolHardware = '';
                this.testData = {
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
                    wheelSpeed: ''
                };
                this.dialogSolutionVisible = false;
                this.error = {};

                if(this.isCan){
                    this.connectionSelectDisplay = this.itemList;
                }else{
                    this.connectionSelectDisplay = this.itemList.slice(0,3);
                }
                this.testSelectDisplay = this.systemList;

                this.testSelectDialogVisible = false;
                this.connectionSelectDialogVisible = false;
                this.switchTest = false;
                this.testSelect = "";
                this.slideDisplay = true;
                this.startTest = false;
                this.cadenceOrTorque = true;
                this.testInfoVisible = false;
                this.levelLowChecked = false;
                this.levelHighChecked = false;

                this.lightFlag = {
                    throttle: false,
                    brake: false,
                    light: false,
                    support: false,
                    walk: false,
                    speed: false,
                    cadence: false,
                    torque: false,
                    motor: false
                };

                this.initLightStatus = null;
                this.breakHoldOk =  false;
                this.breakHoldFail =  false;
                this.breakReleaseOk =  false;
                this.breakReleaseFail = false;
                this.lightOnOk = false;
                this.lightOnFail =  false;
                this.lightOFFOk =  false;
                this.lightOFFFail =  false;
                this.levelUpOk =  false;
                this.levelUpFail = false;
                this.levelDownOk =  false;
                this.levelDownFail =  false;
                this.levelIsChecked = false;
                this.hasLight = false;

                this.lightArr = [];
                this.supportArr = [];
                this.cadenceData = [];
                this.cadenceTime = [];
                this.cadenceOption = {
                    // title: {
                    //     text: 'Cadence Strength'
                    // },
                    colors: ["#E26829"],
                    // legend: {
                    //     data: ['Cadence']
                    // },
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
                };
                this.torqueData = [];
                this.torqueOption = {
                    colors: ["#E26829"],
                    tooltip: {},
                    grid: {
                        left: "-10%",
                        right: "4%",
                        bottom: "13%",
                        containLabel: true
                    },
                    xAxis: {
                        data: []
                    },
                    yAxis: {
                        type: "value",
                        min: 0,
                        max: 5
                    },
                    series: [
                        {
                            name: "Torque",
                            type: "line",
                            smooth: true,
                            itemStyle : {
                                normal : {
                                    color:'#98B42C',
                                    lineStyle:{
                                        color:'#98B42C'
                                    }
                                }
                            },
                            data: []
                        }
                    ]
                };
                this.sensorData = {
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
                };
                this.sensorStartTime = 0;
                this.lightArr = [];
                this.supportArr = [];
                this.currentIndex = -1;
                Diagnosis.port.reset(100);
                this.isConnected = false;
//                if(!this.isCan){
//                    this.$message({
//                        message: this.$t('cNam.resetTip'),
//                        type: 'info'
//                    });
//                }

                let resetTem = document.getElementById("reset");
                resetTem.blur();

                this.defectList = {};
                this.existList = [];
                this.testBrakeCount = 0;
                this.testLevel = '';
                this.walkStatus = '';

                if(this.listenInterval){
                    clearInterval(this.listenInterval);
                    clearInterval(this.listenInterval);
                }
                if(this.listenIntervalOFF){
                    clearInterval(this.listenIntervalOFF);
                    clearInterval(this.listenIntervalOFF);
                }

                Diagnosis.port.setBoxOperation();
            },

            clear(No){
                switch(No){
                    case 8:
                        this.hmiInfo = {
                            sn: '',
                            model: '',
                            version: '',
                            hardVersion: '',
                            speedLimit: '',
                            wheelDiameter: '',
                            totalMileage: '',
                            totalGear: '',
                            clientCodeRead: '',
                            error: '',
                            tip: '',
                            note: ''
                        };
                        break;
                    case 9:
                        this.controllerInfo = {
                            sn: '',
                            model: '',
                            version: '',
                            hardVersion: '',
                            manufacturer: '',
                            nmlVolt: '',
                            maxCurrent: '',
                            error: '',
                            tip: '',
                            speedLimit: '',
                            wheelDiameter: '',
                            circumference: '',
                            note: ''
                        };
                        break;
                    case 10:
                        this.batteryInfo = {
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
                            error: '',
                            packVoltage: '',
                            tip: '',
                            note: ''
                        };
                        break;
                    case 11:
                        this.pedalSensorInfo = {
                            sn: '',
                            name: '',
                            version: '',
                            hardVersion: '',
                            error: '',
                            tip: '',
                            note: ''
                        };
                        break;
                    case 7:
                        this.motorInfo = {
                            sn: '',
                            name: '',
                            power: '',
                            voltage: '',
                            wheelSize: '',
                            rpm: '',
                            error: '',
                            tip: '',
                            note: ''
                        };
                        break;
                }
            },

            clearReadData() {
                this.vehicleSn = '';
                this.vehicleId = '';
                this.vehicleDetail = {};
                this.componentList = [];

                this.clear(8);
                this.clear(9);
                this.clear(10);
                this.clear(11);
                this.clear(7);

                this.itemList.forEach(item=>{
                    item.loaded = false;
                    item.percentage = 0;
                    item.status =  0;
                    item.update = false;
                });
                this.systemList.forEach(item=>{
                    item.detail = false;
                    item.okOrFail = 0;
                });

                this.connectionSelectDisplay.map(item => {
                    item.status = 0;
                    item.percentag = 0;
                    item.update = false
                });
                this.testSelectDisplay = this.systemList;

                this.result = {
                    hmiOK: true,
                    controllerOK: true,
                    batteryOK: true,
                    motorOK: true,
                    pedalSensorOK: true,
                    rpmSensorOK: true,
                    brakeOK: true,
                    lightOK: true,
                    walkOK: '',
                    throttleOK: '',
                    supportOK: '',

                    hmi: '',
                    battery: '',
                    controller: '',
                    motor: '',
                    pedalSensor: '',
                    rpmSensor: '',
                    brake: '',
                    light: '',
                    walk: '',
                    throttle: '',
                    support: '',
                };
                this.testSelect = "";
                this.isHelp = false;
                this.startRunning = false;
                this.detailVisible = false;

                this.timeoutList.forEach(item => {
                    clearTimeout(item);
                })
            },


            //读取
            readInfo(no) {
                if (!this.isConnected) {
                    this.$message({
                        type: 'warning',
                        message: this.$t('cNam.comDisconnect')
                    });
                    return;
                }
                this.isRead = true;
//                this.goToDiagnosis();
                let $this = this;
                let deviceList = [];
                if(no){
                    switch (no) {
                        case 8:
                            deviceList.push(this.config.hmi);
                            break;
                        case 9:
                            deviceList.push(this.config.ctrl);
                            break;
                        case 10:
                            deviceList.push(this.config.bat);
                            break;
                        case 11:
                            deviceList.push(this.config.sen);
                            break;
                    }
                }else{
                    deviceList = this.getReadDeviceList();
                }
                if(deviceList[0]){
//                    console.log( deviceList[0].key );
                    if(!this.usbHID){
                        switch(deviceList[0].key){
                            case 'hmi':
                                HMI.connect(this.com);
                                break;
                            case 'ctrl':
                                Controller.connect(this.com);
                                break;
                            case 'bat':
                                Battery.connect(this.com);
                                break;
                        }
                    }else{
                        this.connect();
                    }

                    console.log("connect success 1994");
                }
                let beginTime = 0;
                deviceList.forEach((item, index) => {
                    if (item.device) {
                        this.timeoutList[index] = setTimeout(() => {
                            let dex = -1;
                            switch (item.key) {
                                case 'hmi':
                                    dex = $this.connectionSelectDisplay.findIndex(item => {
                                        return item.No === 8
                                    });
                                    break;
                                case 'ctrl':
                                    dex = $this.connectionSelectDisplay.findIndex(item => {
                                        return item.No === 9
                                    });
                                    break;
                                case 'bat':
                                    dex = $this.connectionSelectDisplay.findIndex(item => {
                                        return item.No === 10
                                    });
                                    break;
                                case 'sen':
                                    if($this.isCan && $this.isSenCan){
                                        dex = $this.connectionSelectDisplay.findIndex(item => {
                                            return item.No === 11
                                        });
                                    }
                                    break;
                            }
                            if (dex > -1) {
                                $this.startShowPercentage($this.connectionSelectDisplay[dex], item.time);
                            }
                            $this.readBase(item.device);
                        }, beginTime);
                        beginTime = beginTime + item.time;
                    }
                })
                setTimeout(()=>{
                    $this.isRead = false;
                    console.log("listenIntervalOFF",Core.Util.time());
                    let toolHardwareTem = $this.toolHardware.replace(/\s/g, "")
                    if($this.isCan &&  (toolHardwareTem === "BESST.UC3.0" || toolHardwareTem === "BESST.UC3.0.1")){
                        $this.listenCurrentOff();
                    }
                },beginTime + 1400)
            },

            reconnect() {
                console.log('re read');
                this.clear( this.testSelect.No );
                this.readInfo(this.testSelect.No);
//                this.clearReadData();
//                this.beginConnection();
                console.log(this.slideDisplay, 's d', this.connectionSelectDisplay);
            },

            //诊断
            readTestData() {
                let targetItem = {};
                this.testBrakeCount = 0;
                this.testLevel = '';
                this.cadenceTime = [];
                this.walkStatus = '';

                if (this.usbHID) {
                    if(!this.isCan || HMI.port.checkUsbUart()){
                        Diagnosis.setupUsb();
                        Diagnosis.parseTestUart();
                    }else{
                        Diagnosis.parseTestCan();
                    }
                }else {
                    Diagnosis.connect(this.com);
                }
                this.subscription = Diagnosis.getBus().subscribe(res => {
                    if (res.type === 'connect') {
                        this.$message({
                            message: this.$t('goTest'),
                            type: 'success'
                        });
                    }
                    if (!res.data || !res.data.test) {
                        return;
                    }
//                    console.log(res.data.test);
                    if ("error" in res.data.test) {
                        let errorTem = Core.Util.get(res, 'data.test.error', this.testData.error);
//                        console.log('获取到的error 是什么', this.testData.error, errorTem);
                        if (errorTem && errorTem > 7) {
                            let tem = [];
                            try{
                                this.error = Core.Const.TEST_ERROR_CODE[parseInt(errorTem)] || '';
                                tem.push( parseInt(errorTem) );
                                this.errorList = Core.Util.uniqueArray(tem);  // [9,30 ,10];
                            }catch(err){
                                console.log(err);
                            }
                            if (this.error && this.error.target) {
                                // 有错误，并且错误有指向。
                            } else {
                                this.dialogSolutionVisible = false;
                            }
                        }
                    }
                    if ("brake" in res.data.test) {
                        this.testData.brake = Core.Util.get(res, 'data.test.brake', this.testData.brake);
//                        console.log('获取到的error brake是什么', this.testData.brake);
                        if (this.testSelect.No === 1 || this.switchTest) {
                            this.lightFlag.brake = this.testData.brake > 0;
                            if (this.breakReleaseFail) {
                                targetItem = this.getTargetItem(1, this.testSelectDisplay);
                                this.changeOption(targetItem, 2);
                            }
                        }
                    }
                    if ("throttle" in res.data.test) {
                        this.testData.throttle = Core.Util.get(res, 'data.test.throttle', this.testData.throttle);
//                        console.log('获取到的error throttle是什么', this.testData.throttle);
                        if (this.testSelect.No === 0 || this.switchTest) {
                            this.lightFlag.throttle = this.testData.throttle > 0;
                            if (this.testData.throttle > 0) {
                                targetItem = this.getTargetItem(0, this.testSelectDisplay);
                                this.changeOption(targetItem, 1);
                            }
                        }
                    }
                    if ("light" in res.data.test) {
                        this.testData.light = Core.Util.get(res, 'data.test.light', this.testData.light);
//                        console.log('获取到的light是什么', this.testData.light, this.lightArr);
//                        if (this.testSelect.No === 2 || this.switchTest) {
//                            if(this.lightOnOk && this.lightOFFOk){
//                                targetItem = this.getTargetItem(2, this.testSelectDisplay);
//                                this.changeOption(targetItem, 1);
//                            }
//                            if (!this.isCan || HMI.port.checkUsbUart()) {
//                                if( (this.lightArr.join('').indexOf('01') > -1) && (this.lightArr.join('').indexOf('10') > -1) ){
//                                    this.lightFlag.light = this.testData.light > 0;
//                                    targetItem = this.getTargetItem(2, this.testSelectDisplay);
//                                    this.changeOption(targetItem, 1);
//                                }
//                            }else{
//                                if( (this.lightArr.join('').indexOf('011') > -1) && (this.lightArr.join('').indexOf('110') > -1) ){
//                                    this.lightFlag.light = this.testData.light > 0;
//                                    targetItem = this.getTargetItem(2, this.testSelectDisplay);
//                                    this.changeOption(targetItem, 1);
//                                }
//                            }

//                        }
                        if (this.testSelect.No === 2 || this.switchTest) {
                            this.lightArr.push(this.testData.light);
                            if(!this.isCan || HMI.port.checkUsbUart()){
                                if(this.lightArr.length > 80){
                                    this.lightArr = this.lightArr.slice(50);
                                }
                            }else{
                                if(this.lightArr.length > 40){
                                    this.lightArr = this.lightArr.slice(20);
                                }
                            }
                            this.lightFlag.light = this.testData.light > 0;
                            if(this.lightOFFFail && this.lightOnFail){
                                targetItem = this.getTargetItem(2, this.testSelectDisplay);
                                this.changeOption(targetItem, 2);
                            }
                        }
                    }
                    if ("gear" in res.data.test) {
                        this.currentGear = Core.Util.get(res, 'data.test.gear', this.testData.gear);
//                        console.log('获取到的currentGear是什么', this.currentGear, this.supportArr);
                        if(this.levelIsChecked){
                            try{
                                if(this.currentGear && this.hmiInfo.totalGear){
                                    this.testLevel = Diagnosis.getGearNumber(this.currentGear, this.hmiInfo.totalGear);
                                }
                            }catch(err){
                                console.log("getGearNumber",err);
                            }
                        }else{
                            try{
                                if(this.currentGear){
                                    this.testLevel = Diagnosis.getGearNumber(this.currentGear,5);
                                }
                            }catch(err){
                                console.log("getGearNumber",err);
                            }
                        }
//                        for(let i=0;i<arrCheck.length;i++){
//                            // 0 档 向上加档测试
//                            if(arrCheck[i] == 0){
//                                if( arrCheck.length && arrCheck[i] !== arrCheck[i+2] && arrCheck[i] !== arrCheck[i+1] && arrCheck[i+1] !== arrCheck[i+2]){
//                                    this.levelLowChecked = true;
//                                }
//                            }
//                            // 最高档档 向下减档测试
//                            if(arrCheck[i] == 3){
//                                if( arrCheck.length && arrCheck[i] !== arrCheck[i+2] && arrCheck[i] !== arrCheck[i+1] && arrCheck[i+1] !== arrCheck[i+2]){
//                                    this.levelHighChecked = true;
//                                }
//                            }
//                        }
                        if (this.testSelect.No === 3 || this.switchTest) {
                            this.supportArr.push(this.currentGear);
                            if(!this.isCan || HMI.port.checkUsbUart()){
                                if (this.supportArr.length > 80){
                                    this.supportArr = this.supportArr.slice(50);
                                }
                            }else{
                                if (this.supportArr.length > 40){
                                    this.supportArr = this.supportArr.slice(20);
                                }
                            }
                            let arrCheck = this.simplify(this.supportArr);
                            if( arrCheck.length>3 ){
                                this.lightFlag.support = true;
                                targetItem = this.getTargetItem(3, this.testSelectDisplay);
                                this.changeOption(targetItem, 1)
                            }
                        }

//                        if ( (this.testSelect.No === 6 || this.switchTest) && this.currentGear === 6 ) {
//                            console.log('获取到的currentGear是什么', this.currentGear,  this.testData.speed);
//
//                            if(!this.isCan || HMI.port.checkUsbUart()){
//
//
//                                if (this.testData.speed > 15 && this.testData.speed < 47) {
//                                    this.lightFlag.walk = true;
//                                    targetItem = this.getTargetItem(6, this.testSelectDisplay);
//                                    this.changeOption(targetItem, 1);
////                                    let cadenceItem = this.getTargetItem(6, this.testSelectDisplay);
////                                    if (cadenceItem.okOrFail === 1) {
////                                        let motorItem = this.getTargetItem(7, this.testSelectDisplay);
////                                        this.changeOption(motorItem, 1);
////                                    }
//                                }
//                            }else{
//                                if (this.walkStatus === 1 && this.testData.speed > 1 && this.testData.speed < 6) {
//                                    this.lightFlag.walk = true;
//                                    targetItem = this.getTargetItem(6, this.testSelectDisplay);
//                                    this.changeOption(targetItem, 1);
////                                    let cadenceItem = this.getTargetItem(6, this.testSelectDisplay);
////                                    if (cadenceItem.okOrFail === 1) {
////                                        let motorItem = this.getTargetItem(7, this.testSelectDisplay);
////                                        this.changeOption(motorItem, 1);
////                                    }
//                                }
//                            }
//                        }
                    }
                    if ("assistance" in res.data.test) {
                        this.walkStatus = Core.Util.get(res, 'data.test.assistance', '');
                    }
                    if ("speed" in res.data.test) {
                        this.testData.speed = Core.Util.get(res, 'data.test.speed', this.testData.speed);
                        if(!this.isCan || HMI.port.checkUsbUart()){
                            // UART 速度是计算出来的
                            this.$set(this.testData, "wheelSpeed", this.calculateWheelSpeed(this.testData.speed));
                        }else{
                            this.testData.wheelSpeed = this.testData.speed;
                        }
                        if (this.testSelect.No === 4 || this.switchTest) {
                            targetItem = this.getTargetItem(4, this.testSelectDisplay);
                            if( this.testData.speed > 6 ){
                                this.lightFlag.speed = true;
                                this.changeOption(targetItem, 1);
                            }
                        }

                        if ( (this.testSelect.No === 6 || this.switchTest) && this.currentGear === 6 ) {
//                            console.log('获取到的currentGear是什么', this.currentGear,  this.testData.speed);

                            if(!this.isCan || HMI.port.checkUsbUart()){


                                if (this.testData.speed > 15 && this.testData.speed < 47) {
                                    this.lightFlag.walk = true;
                                    targetItem = this.getTargetItem(6, this.testSelectDisplay);
                                    this.changeOption(targetItem, 1);
//                                    let cadenceItem = this.getTargetItem(6, this.testSelectDisplay);
//                                    if (cadenceItem.okOrFail === 1) {
//                                        let motorItem = this.getTargetItem(7, this.testSelectDisplay);
//                                        this.changeOption(motorItem, 1);
//                                    }
                                }
                            }else{
                                if (this.walkStatus === 1 && this.testData.speed > 1 && this.testData.speed < 6) {
                                    this.lightFlag.walk = true;
                                    targetItem = this.getTargetItem(6, this.testSelectDisplay);
                                    this.changeOption(targetItem, 1);
//                                    let cadenceItem = this.getTargetItem(6, this.testSelectDisplay);
//                                    if (cadenceItem.okOrFail === 1) {
//                                        let motorItem = this.getTargetItem(7, this.testSelectDisplay);
//                                        this.changeOption(motorItem, 1);
//                                    }
                                }
                            }
                        }
//                        console.log(this.testData.wheelSpeed, '赋值的速度值',  this.testData.speed);
                    }
                    if ("sensor_torque" in res.data.test) {
                        this.testData.sensorTorque = Core.Util.get(res, 'data.test.sensor_torque', this.testData.sensorTorque);
                        if (this.testSelect.No === 5 || this.switchTest) {
                            targetItem = this.getTargetItem(5, this.testSelectDisplay);
                            if (!this.cadenceOrTorque && this.testData.sensorTorque > 0.75) {
                                this.lightFlag.torque = true;
                                this.changeOption(targetItem, 1)
                            }
                        }
                    }
                    if ("cadence" in res.data.test) {
                        this.testData.cadence = Core.Util.get(res, 'data.test.cadence', this.testData.cadence);
                        this.checkCTCount()
                        if(this.testSelect.No === 5 || this.switchTest){
                            if( this.testData.cadence > 20){
                                // 获取稳定的Cadence 值
                                this.cadenceData.push(this.testData.cadence);
//                            this.cadenceTime.push(new Date().getTime());
                            }
                            let num = 0;
                            let efficient = 0;
                            if(!this.isCan || HMI.port.checkUsbUart()){ // Uart
                                num = 2;
                                efficient = 66;
                                if(this.cadenceData.length > 40){
                                    this.cadenceData = this.cadenceData.slice(20);
                                }
                            }else{ // Can
                                num = 25;
                                efficient = 90;
                                if(this.cadenceData.length > 80){
                                    this.cadenceData = this.cadenceData.slice(50);
                                }
                            }
                            if( this.cadenceData.length > num){
                                let count = 0;
                                let len = this.cadenceData.length;
                                for(let l=0; l<len-1; l++){
                                    if( Math.abs(this.cadenceData[l] - this.cadenceData[l+1]) < 10){
                                        count++;
                                    }
                                }
                                if(Math.round(count*100/len) > efficient){
                                    targetItem = this.getTargetItem(5, this.testSelectDisplay);
                                    this.lightFlag.cadence = true;
                                    this.changeOption(targetItem, 1);
                                }
                            }
                        }

//                        console.log('diagnosis cadenceTime',this.testData.cadence, this.cadenceData);
                        /**
                         //  判断两个信号的时间差
                         if (this.testSelect.No === 5 || this.switchTest) {
                            targetItem = this.getTargetItem(5, this.testSelectDisplay);
                            if (this.cadenceOrTorque && (this.cadenceTime[this.cadenceTime.length-1] - this.cadenceTime[0]>3000)) {
                                let check = true;
                                for(let i = 1; i< this.cadenceTime.length-2; i++ ){
                                    let f = this.cadenceTime[i+1]-this.cadenceTime[i];
                                    let b = this.cadenceTime[i]-this.cadenceTime[i-1];
                                    console.log("ii",this.cadenceTime[i+1]-this.cadenceTime[i], f/b);
                                    console.log("i",this.cadenceTime[i]-this.cadenceTime[i-1], b/f);
                                    if((f/b > 3) ||(b/f > 3) ){
                                        check = false;
                                    }
                                }
                                if(check){
                                    this.lightFlag.cadence = true;
                                    this.changeOption(targetItem, 1);
                                }
//                                let walkItem = this.getTargetItem(5, this.testSelectDisplay);
//                                if (walkItem.okOrFail === 1) {
//                                    let motorItem = this.getTargetItem(7, this.testSelectDisplay);
//                                    this.changeOption(motorItem, 1);
//                                }
                            }
                        }
                         **/
                        /**
                         if ('sensor_type' in res.data.test && res.data.test.sensor_type) {
                            if (this.sensorVoltCount % 250 === 0 && this.sensorVoltCount < 40000) {
                                if (this.cadenceData.push(this.testData.cadence) > 4) {
                                    this.cadenceData.shift();
                                }
                            }
                        } else {
                            if (this.cadenceData.push(this.testData.cadence) > 4) {
                                this.cadenceData.shift();
                            }
                        }
                         **/


//                        console.log('diagnosis cadence', this.cadenceOption.xAxis.data, this.cadenceOption.series[0].data);
                    }
                    // 2个表格

//                    if ('sensor_volt' in res.data.test) {
//                        this.testData.sensorVoltage = Core.Util.get(res, 'data.test.sensor_volt', this.testData.sensorVoltage);
//                        this.testData.sensorVoltage = this.testData.sensorVoltage.toFixed(2);
//                        if (this.sensorStartTime !== 0) {
//                            if ((new Date().getTime() - this.sensorStartTime) / 1000 < 60) {
//                                this.sensorData.rows.push({time: '', voltage: this.testData.sensorVoltage});
//                            }
//                        }
//                    }

                    if ('sensor_volt' in res.data.test) {
                        this.testData.sensorVoltage = Core.Util.get(res, 'data.test.sensor_volt', this.testData.sensorVoltage);
                        this.testData.sensorVoltage = this.testData.sensorVoltage.toFixed(2);
                        // 获取稳定的Cadence 值
//                        this.torqueData.push(this.testData.sensorVoltage);
//                        if( this.torqueData.length > 10){
//                            let len = 0,count =0;
//                            len = this.cadenceData.length;
//                            for(let l=0; l<len-1; l++){
//                                if( Math.abs(this.cadenceData[i] - this.cadenceData[i]) < 10){
//                                    count++;
//                                }
//                            }
//                            if(Math.round(count/len)>0.9){
//                                targetItem = this.getTargetItem(5, this.testSelectDisplay);
//                                this.lightFlag.cadence = true;
//                                this.changeOption(targetItem, 1);
//                            }
//                        }
                        if (this.testSelect.No === 5 || this.switchTest) {
                            targetItem = this.getTargetItem(5, this.testSelectDisplay);
                            if (!this.cadenceOrTorque && this.testData.sensorVoltage > 0.75 && this.testData.sensorVoltage < 5) {
                                this.lightFlag.torque = true;
                                this.changeOption(targetItem, 1)
                            }
                        }
                        /**
                         * torque 画图
                         *
                         let time = new Date().getTime();
                         if (this.sensorStartTime !== 0 && ( (time - this.sensorStartTime) / 1000 < 40) ) {
                            this.sensorVoltCount++;
//                            console.log("[Count]", this.sensorVoltCount);
                            if (this.isCan){
                                if('sensor_type' in res.data.test && res.data.test.sensor_type ){
                                    if( this.sensorVoltCount % 250 === 0 && this.sensorVoltCount < 40000) {
//                                        if (this.torqueData.push(this.testData.sensorVoltage) > 8) {
//                                            this.torqueData.shift();
                                        this.torqueOption.xAxis.data.push('');
                                        this.torqueOption.series[0].data.push(this.testData.sensorVoltage);
//                                        }
                                    }
                                }else if ('sensor_type' in res.data.test && !res.data.test.sensor_type ){
//                                    if (this.torqueData.push(this.testData.sensorVoltage) > 8) {
//                                        this.torqueData.shift();
                                    this.torqueOption.xAxis.data.push('');
                                    this.torqueOption.series[0].data.push(this.testData.sensorVoltage);
//                                    }
                                }
                            } else{
//                                 if( this.sensorVoltCount % 5 == 0 && this.sensorVoltCount < 400) {

//                                    if (this.torqueData.push(this.testData.sensorVoltage) > 8) {
//                                        this.torqueData.shift();
//                                    }
                                this.torqueOption.xAxis.data.push('');
                                this.torqueOption.series[0].data.push(this.testData.sensorVoltage);
//                                }
                            }
                        }
                         **/
                    }
                    // this.testData.cadence = Math.floor(Math.random() * 130)
                    if ('cadence' in res.data.test) {
                        // 添加的数值肯定大于0（后面求倒数） 小于等于1
                        let cadenceX = [0];
                        this.cadenceData.forEach(item => {
                            let v = parseFloat(Core.Util.sprintf("%.3f", 1 / item));
                            cadenceX.push(v, 0);
                        });
                        cadenceX.push(0);
                        let maxCadence = Math.max.apply(null,cadenceX);
                        this.cadenceOption.yAxis.max = maxCadence > 0.02 ? Math.ceil(maxCadence * 100) / 100 : 0.02;
                        let i = 0;
                        this.cadenceOption.xAxis.data.length = 0;
                        this.cadenceOption.series[0].data.length = 0;
                        while (i < cadenceX.length) {
                            this.cadenceOption.xAxis.data.push('');
                            this.cadenceOption.series[0].data.push(cadenceX[i]);
                            i++;
                        }
//                        console.log('diagnosis cadence', this.cadenceOption.xAxis.data, this.cadenceOption.series[0].data);
                    }

                    if( 'current' in res.data.test) {
                        this.testData.current = Core.Util.get(res, 'data.test.current', this.testData.current);
//                        console.log( this.testData.current );
//                        console.log('获取到的current是什么', this.testData.current);

                        if ((this.testSelect.No === 6 || this.testSelect.No === 5 || this.testSelect.No === 7) || this.switchTest) {
                            targetItem = this.getTargetItem(7, this.testSelectDisplay);
                            if(this.testSelect.No === 6 && this.lightFlag.walk && this.testData.current < 3) {
                                this.changeOption(targetItem, 1);
                            }else if(this.testSelect.No === 5 && this.lightFlag.cadence && this.testData.current < 3){
                                this.changeOption(targetItem, 1);
                            }else{
                                if( (this.testData.cadence >20 && this.testData.speed >6 && this.testData.current> 0.1) || ( this.testData.sensorVoltage >0.75 && this.testData.speed >6 && this.testData.current >0.1) ){
                                    this.lightFlag.motor = true;
                                    this.changeOption(targetItem, 1);
                                }
                            }
                        }
                    }
                    // 未用到的1个数据

//                    this.testData.capacity = Core.Util.get(res, 'data.test.capacity', this.testData.capacity);

                });
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
                        this.result.light = "Fail" + this.result.light;
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
                this.torqueOption.xAxis.data = [];
                this.torqueOption.series[0].data = [];
                this.sensorStartTime = new Date().getTime();
            },

            cadenceStart() {
                this.cadenceData.rows = [];
            },

            createReportInfo() {

                let connectListTem = this.connectionSelectDisplay.map( ({No,status,update}) => { return {No,status,update} });
                let testListTem = this.testSelectDisplay.map( ({No,name,detail,okOrFail}) => { return {No,name,detail,okOrFail} });
                let allConnectListTem =this.itemList.map( (item) => { return item.No; });
                let allTestListTem = this.systemList.map( (item) => { return item.No; });
                let comList= this.componentList;
                this.testData.error = this.errorList.join(',');
                if(this.vehicleId){
                    this.reportInfo = {
                        device: {
                            vehicle: {
                                id: this.vehicleId,
                                sn: this.vehicleSn,
                                itemName: this.vehicleDetail.item.name,
                                secretKey: this.vehicleDetail.secret_key
                            },
                            hmi: this.hmiInfo,
                            controller: this.controllerInfo,
                            battery: this.batteryInfo,
                            motor: this.motorInfo,
                            pedalSensor: this.pedalSensorInfo,
                        },
                        can: this.isCan,
                        test: this.testData,
                        result: this.result,
                        existComList: this.existList,
                        bomList: this.componentList,
                        connectList: connectListTem,
                        testList: testListTem,
                        allConnectList: allConnectListTem,
                        allTestList: allTestListTem
                    };
                }else{
                    this.reportInfo = {
                        device: {
                            vehicle: {
                                id: this.vehicleId,
                                sn: this.vehicleSn,
                                itemName: '',
                                secretKey: ''
                            },
                            hmi: this.hmiInfo,
                            controller: this.controllerInfo,
                            battery: this.batteryInfo,
                            motor: this.motorInfo,
                            pedalSensor: this.pedalSensorInfo,
                        },
                        can: this.isCan,
                        test: this.testData,
                        result: this.result,
                        existComList: this.existList,
                        bomList: this.componentList,
                        connectList: connectListTem,
                        testList: testListTem,
                        allConnectList: allConnectListTem,
                        allTestList: allTestListTem

                    }
                }
//                console.log("reportInfo",this.reportInfo);
            },
            goBeforeReport() {
                this.switchElectronic();
                this.showBeforeReport = false;
                this.goToReport();
            },
            goAfterReport() {
                this.showAfterReport = false;
                this.goToReport();
            },
            goReport(){
                this.showAfterReport = true;
            },
            goToReport() {
                console.log('click create report');
//                console.log(this.vehicleDetail);
                this.createReportInfo();
                // saveReport() {
                //     if (this.offLineFlag) {
                //         // this.onClickPrint();
                //         return;
                //     }
                //     if (!this.vehicleId) {
                //         this.$message.warning('Please type vehicle id');
                //         return;
                //     }
                //     this.reportInfo = {
                //         device: {
                //             vehicle: {
                //                 id: this.vehicleId,
                //                 sn: this.vehicleSn
                //             },
                //             vehicleDetail: this.vehicleDetail,
                //             hmi: this.hmiInfo,
                //             controller: this.controllerInfo,
                //             battery: this.batteryInfo,
                //             motor: this.motorInfo,
                //             pedalSensor: this.pedalSensorInfo
                //         },
                //         test: this.testData,
                //         result: this.result,
                //         bomList: this.componentList,
                //         connectList: this.connectionSelectDisplay,
                //         testList: this.testSelectDialogVisible,
                //     };

                //     Core.Api.Vehicle.saveVehicleReport(0, this.vehicleId, JSON.stringify(this.reportInfo), '', '', '').then(
                //         res => {
                //             let id = res.report_id;
                //             Core.Data.set(Core.Const.DATA.KEY_REPORT, "");
                //             if (Core.Data.getOrg().type == 1) {
                //                 this.$router.push({name: "brandDiagnosisReport", query: {id: id}});
                //             }
                //             if (Core.Data.getOrg().type == 2) {
                //                 this.$router.push({name: "dealerDiagnosisReport", query: {id: id}});
                //             }
                //             if (Core.Data.getOrg().type == 3) {
                //                 this.$router.push({name: "assemblerDiagnosisReport", query: {id: id}});
                //             }
                //             if (Core.Data.getOrg().type == 4) {
                //                 this.$router.push({name: "componentDiagnosisReport", query: {id: id}});
                //             }
                //         }
                //     );
                // },

//                let keys = Object.keys( this.defectList );
//                let defectList = [];
//                for(let i=0; i<keys.length; i++){
//                    for(let j=0; j<this.defectList[keys[i]].length; j++){
//                        defectList.push(keys[i]+':'+ this.defectList[keys[i]][j]);
//                    }
//                }
//                console.log("defectList",defectList);
//                console.log("fileAll",this.fileAll);

                this.$router.push({
                    path: 'result',
                    query: {report: this.reportInfo, defect: this.defectList, file: this.fileAll, remark: this.problemText},
                })
            },

            // 去除相邻两个重复
            simplify(arr){
                let temArr = [];
                for(var i=0; i<arr.length; i++){
                    if(i < arr.length && arr[i]!==arr[i+1]){
                        temArr.push(arr[i]);
                    }
                }
//                console.log("temArr",temArr);
                return temArr;
            },

            // 新版诊断提醒检查
            remindSevenDaysCheck(){
                let re = Core.Data.get(Core.Const.DATA.KEY_DIG_REMIND);

                let now = Core.Util.time();

                if (re && now - re.time < 7*24*3600){
                    this.remindCheck = false;
                    this.diagnosisUpdateNoteOpen = false;
                }else{
                    this.remindCheck = true;
                    this.diagnosisUpdateNoteOpen = true;
                }
            },

            // 新版诊断提醒
            remindSevenDays(){
                console.log("remind check",this.remindCheck);

                this.diagnosisUpdateNoteOpen = false;
                if(this.remindCheck){
                    let check = {
                        check: this.remindCheck,
                        time: Core.Util.time()
                    }
                    Core.Data.set(Core.Const.DATA.KEY_DIG_REMIND, check);

                }
            }



        },
    };
</script>
<style lang="scss">
    .my-diagnosis-container {
        font-size: 16px;
        width: 100%;
        height: 100%;
        display: -webkit-box;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-orient: vertical;
        -webkit-box-direction: normal;
        -ms-flex-direction: column;
        flex-direction: column;
        .switch {
            border-bottom: 1px solid #343844;
            height:40px;
            background: #383d49;
            .select {
                text-align: center;
                position: relative;
                .select-area {
                    float: left;
                    width: 49%;
                    line-height: 40px;
                }
                .active {
                    background: #343844;
                    color: #fff;
                    font-size: 20px;
                }
            }
        }
        .switch-body{
            -webkit-box-flex: 1;
            -ms-flex: 1;
            flex: 1;
            width: 100%;
            height: calc(100% - 40px);
            .switch-container{
                overflow: auto;
                width: 100%;
                height: 100%;
            }
        }
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
                    }
                }
                .tip {
                    display: inline-block;
                    border-radius: 50%;
                    width: 16px;
                    height: 16px;
                    margin-left: 10px;
                    background: #e26829;
                    &.connect {
                        background: #98b42c;
                    }
                }
                .el-select {
                    width: 100px;
                }
            }
            .search-item {
                white-space: nowrap;
                .input-se {
                    width: 200px;
                    .el-input__inner {
                        text-align: center;
                        padding-right: 40px;
                    }
                }
            }
        }
        .content-container {
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
                            &.connect {
                                background: #e26829;
                            }
                        }
                        // .el-select {
                        //     color: #fff;
                        //     .el-input {
                        //         padding-left: 5px;
                        //         .el-input__inner {
                        //             width: 101px;
                        //             height: 17px !important;
                        //             color: #fff;
                        //             background-color: transparent;
                        //             text-align: center;
                        //             border: 1px solid #fff;
                        //             padding-left: 9px;
                        //             &::placeholder {
                        //                 color: #fff;
                        //             }
                        //         }
                        //         .el-input__icon {
                        //             color: #fff;
                        //         }
                        //     }
                        // }
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
                    // .content-select:last-of-type{
                    // }
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
                                    font-size: 14px;
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
                                width: 63px;
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
                                display: inline-block;
                                font-size: 14px;
                                display: flex;
                                margin-left: 27px;
                                width: 57px;
                                height: 100%;
                            }
                        }
                    }
                    .content-detect {
                        width: 384px;
                        height: 34px;
                        line-height: 34px;
                        /*margin-top: 14px;*/
                        margin-bottom: 7px;
                        align-items: center;
                        .detect-title{
                            display:inline-block;
                            float:left;
                            margin-left: 25px;
                        }
                        .detect-btn{
                            display:inline-block;
                            float:right;
                            line-height:1;
                            align-self: center;
                            white-space: nowrap;
                            cursor: pointer;
                            border: 0;
                            background: #a1a5b1;
                            color: #fff;
                            -webkit-appearance: none;
                            text-align: center;
                            box-sizing: border-box;
                            outline: none;
                            margin-top: 4px;
                            margin-right: 20px;
                            -moz-user-select: none;
                            -webkit-user-select: none;
                            -ms-user-select: none;
                            padding: 6px 15px;
                            font-size: 14px;
                            border-radius: 15px;
                        }
                        .brown{
                            background: #E26829;
                            border-color: #E26829;
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
                    .report{
                        padding: 5px 26px;
                        align-self: center;
                        span{
                            font-size: 20px;
                        }
                    }
                    /*.btn-reset {*/
                    /*margin-left:80px;*/
                    /*margin-right:180px;*/
                    /*align-self: center;*/
                    /*padding: 5px 26px;*/
                    /*font-size: 20px;*/
                    /*span{*/
                    /*font-size: 20px;*/
                    /*}*/
                    /*}*/
                    div.slide-content {
                        width: 600px;
                        min-height: 575px;
                        background-color: #383d49;
                        border: solid 1px #e26829;
                        border-right: solid 1px transparent;
                        position: absolute;
                        right: 0;
                        z-index: 4;
                        //后加的
                        display: flex;
                        flex-direction: column;
                        &.slide-hidden {
                            z-index: 0;
                            // right: 10px;
                            // width: 20px;
                            width: 35px;
                            &.slide-content-top {
                                width: 32px;
                                cursor: pointer;
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
                            &.width{
                                min-width:590px;
                                cursor: pointer;
                            }
                            &.height{
                                min-height:560px;
                                cursor: pointer;
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
                                        font-size: 14px;
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
                        // & >.slide-content-bottom:nth-of-type(2) {
                        & > .slide-content-bottom.help {
                            padding: 13px 48px 13px 42px;
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
                                        // font-weight: 600;
                                    }
                                    .green {
                                        width: 16px;
                                        height: 16px;
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
                            overflow-x: hidden;
                            img {
                                width: 522px;
                                height: 260px;
                            }
                            .system-txt {
                                span.title {
                                    margin-top: 20px;
                                    &.step {
                                        line-height: 24px;
                                        // font-weight: 600;
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
                        /*height: 550px;*/
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
                        top: 14px;
                        align-self: center;
                        // align-self: flex-end;
                    }
                    .test-system-item {
                        position: absolute;
                        z-index: 8;
                        border: 2px solid #626262;
                        background: #2a2e3a;
                        width: 605px;
                        min-height: 579px;
                        padding: 34px;
                        display: flex;
                        flex-direction: column;
                        justify-content: space-between;
                        align-self: center;
                        .data{
                            display: inline-block;
                            float:left;
                            line-height:20px;
                            .txt{
                                font-size:16px;
                                color: #e26829;
                            }
                            .bold{
                                font-weight: bold;
                                color: #e26829;
                            }
                        }
                        .mark{
                            width:120px;
                            overflow: hidden ;
                            .el-button{
                                margin-top: 5px;
                                margin-right: 5px;
                                max-width: 115px;
                                span{
                                    display:block;
                                    max-width: 115px;
                                    white-space: pre-wrap;
                                    /*word-break: break-all;*/
                                    word-wrap: break-word;
                                    overflow: hidden;
                                }
                            }
                            .active {
                                background: #343844;
                                color: #E26829;
                                font-size: 20px;
                            }
                        }
                        .img-wrap{
                            width:410px;
                            padding-left:5px;
                            border-left: 1px solid #626262;
                            box-shadow: 0 1px 0 #2B2F3B;
                            span{
                                vertical-align:middle;
                            }
                        }
                        .img-wrap:after{

                        }
                        .fixSwitch{
                            position:absolute;
                            top: 20px;
                            right: 10px;
                        }
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
                        .img{
                            width:160px;
                            height:160px;
                        }
                        img.display-ok {
                            width: 70px;
                            height: 66px;
                        }
                        img.pos{
                            position:absolute;
                            right: 22px;
                            top: 190px;
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
                                padding: 0px 7px;
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
                                width: 160px;
                                height: 160px;
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
                                float:left;
                            }
                            .ml-26{
                                float:left;
                                width:450px;
                                margin-top:5px;
                            }
                        }
                        .btn-row {
                            display: flex;
                            justify-content: space-between;
                            bottom: 20px;
                            padding-top: 5px;
                            /*border-top: 1px solid #626262;*/
                            /*box-shadow: 0 1px 0 #2B2F3B;*/
                            /*border: 0;*/
                            /*border-color: #E26829;*/
                            /*border-radius: 15px;*/
                        }
                        .ig{
                            width:180px;
                        }
                        .txt{
                            width:59%;
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
            left: 45%;
            /*transform: scale(0.75, 0.75);*/
            /*-ms-transform: scale(0.75, 0.75); !* IE 9 *!*/
            /*-webkit-transform: scale(0.75, 0.75); !* Safari 和 Chrome *!*/
            /*-o-transform: scale(0.75, 0.75); !* Opera *!*/
            /*-moz-transform: scale(0.75, 0.75);*/
        }
        .notice19 {
            font-size:14px;
            line-height: 20px;
        }
    }
</style>




// WEBPACK FOOTER //
// diagnosis.vue?7c659338