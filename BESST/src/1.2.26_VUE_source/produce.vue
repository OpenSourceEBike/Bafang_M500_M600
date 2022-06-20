<template>
    <div class="assembler-produce">
        <div class="header">
            <!--<button @click='compareVersion( "CRS10F.350.FC1.0K10F8T8201598","CRS10FC4313h101007.0","ctrl")'>test</button>-->
            <!--<button @click='getFirmwareBySn( "CRS10F.350.SN.U1.1K04F8TB251255")'>test</button>-->
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goListView" :title="$t('back')"></div>
                </div>
            </div>
            <div class="btn-group-inline">
                <div class=""  v-if = "!usbHID">
                    <label class="header-label">{{ $t('cNam.com') }}</label>
                    <el-select v-model="com" @change="changeCom" :placeholder="$t('cNam.select')"
                               @click.native='getComList'>
                        <el-option
                            v-for="(com, index) in comList"
                            :key="index"
                            :label="com"
                            :value="com">
                        </el-option>
                    </el-select>
                    <label class="tip" :class="{connect: isConnected}"></label>
                    <el-button type="primary" @click="connect">{{ $t('cNam.connect') }}</el-button>
                </div>
                <div class="" v-else>
                    <label class="header-label">USB HID</label>
                    <label class="tip" :class="{connect: isConnected}"></label>
                    <el-button class="btn-connect" type="primary" @click="connect()">{{ $t('cNam.connect')}}</el-button>
                </div>
            </div>
            <div class="connect-tip">
                <a href="javascript:;" @click="dialogVideoVisible = true" class="btn-tip btn-underline">
                    <img src="../../../assets/images/icon/icon-help.png" class="tip-img" />
                </a>
            </div>
        </div>

        <div class="content-wrap">
            <div class="content">
                <!--订单部分-->
                <div class="info order">
                    <!--<el-collapse v-model="activeNames">-->
                    <!--<el-collapse-item name="1">-->
                    <!--<template slot="title">-->
                    <!--<label class="title-text">{{ $t('orderInfo') }}</label>-->
                    <!--</template>-->

                    <div class="info-list-box">
                        <div class="info-list">
                            <div class="col-left">
                                <ul class="list-group mr-tpo-0">
                                    <li class="list-group-item no-border-top">
                                        <span>{{ $t('brand') }}</span>
                                        <span>{{itemInfo.brand ? itemInfo.brand.name : ""}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span class="height">{{ $t('model') }}</span>
                                        <span>{{itemInfo.name}}({{itemInfo.ext_part_no}})</span>
                                    </li>
                                </ul>
                            </div>
                            <div class="col-right">
                                <ul class="list-group mr-tpo-0">
                                    <li class="list-group-item no-border-top">
                                        <span>{{ $t('poNo') }}</span>
                                        <span>{{orderInfo.source_id}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span class="height">{{ $t('produced_qty') }}</span>
                                        <span>{{itemInfo.item ? itemInfo.item.produced : '0'}}/{{itemInfo.item ? itemInfo.item.amount : '0'}}</span>
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                    <!--</el-collapse-item>-->
                    <!--</el-collapse>-->
                </div>

                <!--读取本地数据-->
                <div class="info read">
                    <!--读取信息部分-->
                    <div class="search-item">
                        <label class="label">{{ $t('vehicleSn') }}</label>
                        <el-input class="scan-input" v-model="vehicleInfo.vehicleSn" :placeholder="$t('sn')"
                                  @blur="blurVehicleSN" :maxlength="45"></el-input>
                    </div>
                    <div class="search-item">
                        <label class="label">{{ $t('componentSn') }}</label>
                        <el-input class="scan-input" v-model="componentSn" :placeholder="$t('sn')"></el-input>
                    </div>

                    <el-collapse v-model="activeNames">
                        <!--hmi-->
                        <el-collapse-item name="2">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-hmi.png" />
                                <label class="title-text">{{ $t('cNam.hmi') }}</label>
                                <el-input class="input-sn" v-model="hmiInfo.sn" :placeholder="$t('sn')"
                                          @click.native.stop="stopPrevent" @blur="blurHMISN"></el-input>
                                <div class="title-solution"
                                     v-if="result.hmi !== '' && result.hmi !== 'OK' && result.hmi !== ''">
                                    <el-button type="text" @click.native.stop="dialogSolutionVisible = true">
                                        {{ $t('solution') }}
                                    </el-button>
                                </div>
                                <vue-loading class="title-loading" type="spin" color="#E26829"
                                             v-if="showLoading.hmi || blur.hmi"></vue-loading>
                                <el-progress v-show="showPercentage.hmi" :text-inside="true" :stroke-width="18"
                                             :percentage="percentage.hmi" status="success"
                                             class="title-progress"></el-progress>
                                <el-button type="primary"
                                           v-if="updateFlag['hmi']"
                                           @click.stop="showHwDialog('hmi')">{{ $t('update') }}</el-button>

                            </template>
                            <div class="info-list-box">
                                <div class="info-list">
                                    <div class="col-left">
                                        <el-button @click="readBase(HMI)" class="btn-connect" type="primary"
                                                   :disabled="isRead">
                                            {{ $t('cNam.read') }}
                                        </el-button>
                                        <span style="color: white;" v-if="!result.hmiOK">Please try to click"Read"button once to catch full data.</span>
                                        <ul class="list-group">
                                            <li class="list-group-item">
                                                <span>SN</span>
                                                <span>{{hmiInfo.sn || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.model') }}</span>
                                                <span>{{hmiInfo.model || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.softwareVersion') }}</span>
                                                <span>{{hmiInfo.version || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.hardwareVersion') }}</span>
                                                <span>{{hmiInfo.hardVersion || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <div class="col-right">
                                        <ul class="list-group">
                                            <li class="list-group-item" >
                                                <span>{{ $t('cNam.totalMileage') }}</span>
                                                <span>{{hmiInfo.totalMileage || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = '!isCan'>
                                                <span>{{ $t('cNam.speedLimit') }}</span>
                                                <span>{{hmiInfo.speedLimit || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = '!isCan'>
                                                <span>{{ $t('cNam.wheelDiameter') }}</span>
                                                <span>{{hmiInfo.wheelDiameter || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = 'isCan'>
                                                <span>&nbsp;</span>
                                                <span>&nbsp;</span>
                                            </li>
                                            <li class="list-group-item" v-show = 'isCan'>
                                                <span>&nbsp;</span>
                                                <span>&nbsp;</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>&nbsp;</span>
                                                <span>&nbsp;</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <ul class="list-group list-all" v-if="hmiInfo.error">
                                        <li class="list-group-item">
                                            <span>{{ $t('error') }}</span>
                                            <span>{{hmiInfo.error}}</span>
                                        </li>
                                    </ul>
                                </div>
                            </div>

                        </el-collapse-item>
                        <!--controller-->
                        <el-collapse-item name="3">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-controller.png" />
                                <label class="title-text">{{ $t('cNam.controller') }}</label>
                                <el-input class="input-sn" v-model="controllerInfo.sn" :placeholder="$t('sn')"
                                          @click.native.stop="stopPrevent"
                                          @blur="blurControllerSN"></el-input>
                                <div class="title-solution"
                                     v-if="result.controller !== '' && result.controller !== 'OK' && result.controller !== ''">
                                    <el-button type="text" @click.native.stop="showSolutionDialog">
                                        {{ $t('solution') }}
                                    </el-button>
                                </div>
                                <vue-loading class="title-loading" type="spin" color="#E26829"
                                             v-if="showLoading.controller || blur.ctrl "></vue-loading>
                                <el-progress v-show="showPercentage.controller" :text-inside="true"
                                             :stroke-width="18"
                                             :percentage="percentage.controller" status="success"
                                             class="title-progress"></el-progress>

                                <el-button type="primary"
                                           v-if="updateFlag['ctrl']"
                                           @click.stop="showHwDialog('ctrl')">{{ $t('update') }}
                                </el-button>
                            </template>
                            <div class="info-list-box">
                                <div class="info-list">
                                    <div class="col-left">
                                        <el-button @click="readBase(Controller)" class="btn-connect" type="primary"
                                                   :disabled="isRead">
                                            {{ $t('cNam.read') }}
                                        </el-button>
                                        <span style="color: white;" v-if="!result.controllerOK">Please try to click"Read"button once to catch full data.</span>
                                        <ul class="list-group">
                                            <li class="list-group-item">
                                                <span>SN</span>
                                                <span>{{controllerInfo.sn || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.model') }}</span>
                                                <span>{{controllerInfo.model || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.softwareVersion') }}</span>
                                                <span>{{controllerInfo.version || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.hardwareVersion') }}</span>
                                                <span>{{controllerInfo.hardVersion || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <div class="col-right">
                                        <ul class="list-group">
                                            <li class="list-group-item" v-show = '!isCan'>
                                                <span>{{ $t('cNam.nmlVolt') }}</span>
                                                <span>{{controllerInfo.nmlVolt || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = '!isCan'>
                                                <span>{{ $t('cNam.maxCur') }}</span>
                                                <span>{{controllerInfo.maxCurrent || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = 'isCan'>
                                                <span>{{ $t('cNam.speedLimit') }}</span>
                                                <span>{{controllerInfo.speedLimit || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = 'isCan'>
                                                <span>{{ $t('cNam.wheelDiameter') }}</span>
                                                <span>{{controllerInfo.wheelDiameter || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = 'isCan'>
                                                <span>{{ $t('cNam.circumference') }}</span>
                                                <span>{{controllerInfo.circumference || "-"}}</span>
                                            </li>
                                            <li class="list-group-item" v-show = '!isCan'>
                                                <span>&nbsp;</span>
                                                <span>&nbsp;</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>&nbsp;</span>
                                                <span>&nbsp;</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <ul class="list-group list-all" v-if="controllerInfo.error">
                                        <li class="list-group-item">
                                            <span>{{ $t('error') }}</span>
                                            <span>{{controllerInfo.error}}</span>
                                        </li>
                                    </ul>
                                </div>
                            </div>

                        </el-collapse-item>
                        <!--battery-->
                        <el-collapse-item name="4">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-battery.png" />
                                <label class="title-text">{{ $t('cNam.battery') }}</label>
                                <el-input class="input-sn" v-model="batteryInfo.sn" :placeholder="$t('sn')"
                                          @click.native.stop="stopPrevent"
                                          @blur="blurBatterySN"></el-input>
                                <div class="title-solution"
                                     v-if="result.battery !== '' && result.battery !== 'OK' && result.battery !== ''">
                                    <el-button type="text" @click.native.stop="showSolutionDialog">
                                        {{ $t('solution') }}
                                    </el-button>
                                </div>
                                <vue-loading class="title-loading" type="spin" color="#E26829"
                                             v-if="showLoading.battery || blur.bat"></vue-loading>
                                <el-progress v-show="showPercentage.battery" :text-inside="true" :stroke-width="18"
                                             :percentage="percentage.battery" status="success"
                                             class="title-progress"></el-progress>
                                <div class="bind-battery">
                                    <label class="switch-label">{{ $t('bindBattery') }}</label>
                                    <el-switch
                                        v-model="isBindBattery"
                                        :on-color="'#98B42C'"
                                        :off-color="'#22252E'"
                                        @change="blurBatterySN"
                                        :on-text="'Yes'"
                                        :off-text="'No'"
                                        class="switch">
                                    </el-switch>
                                </div>

                            </template>
                            <div class="info-list-box">
                                <div class="info-list">
                                    <div class="col-left">
                                        <el-button @click="readBase(Battery) " class="btn-connect" :disabled="isRead"
                                                   type="primary">
                                            {{ $t('cNam.read') }}
                                        </el-button>
                                        <span style="color: white;" v-if="!result.batteryOK">Please try to click"Read"button once to catch full data.</span>
                                        <ul class="list-group">
                                            <li class="list-group-item">
                                                <span>SN</span>
                                                <span>{{batteryInfo.sn || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.softwareVersion') }}</span>
                                                <span>{{batteryInfo.version || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.ASOC') }}(%)</span>
                                                <span>{{batteryInfo.soc || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.packVoltage') }}(mV)</span>
                                                <span>{{batteryInfo.packVoltage || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.fullChargeCapacity') }}(mAh)</span>
                                                <span>{{batteryInfo.fullChargerCapacity || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.current') }}(mA)</span>
                                                <span>{{batteryInfo.current || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.cell8_14') }}(mV)</span>
                                                <span>{{batteryInfo.cell8to14 || "-"}}</span>
                                            </li>
                                            <li class="list-group-item ">
                                                <span class="height">{{ $t('cNam.currentChargeInterval') }}(h)</span>
                                                <span>{{batteryInfo.lastCharge || "-"}}</span>
                                            </li>

                                        </ul>
                                    </div>
                                    <div class="col-right">
                                        <ul class="list-group">
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.model') }}</span>
                                                <span>{{batteryInfo.model || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.hardwareVersion') }}</span>
                                                <span>{{batteryInfo.hardVersion || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.RSOC') }}(%)</span>
                                                <span>{{batteryInfo.soh || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.capacityLeft') }}(mAh)</span>
                                                <span>{{batteryInfo.leftCapacity || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.temperature') }}(℃)</span>
                                                <span>{{batteryInfo.temperature || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.cell1_7') }}(mV)</span>
                                                <span>{{batteryInfo.cell1to7 || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.cycleLife') }}</span>
                                                <span>{{batteryInfo.loopTime || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span class="height">{{ $t('cNam.maxChargeInterval') }}(h)</span>
                                                <span>{{batteryInfo.maxChargeInterval || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <ul class="list-group list-all" v-if="batteryInfo.error">
                                        <li class="list-group-item">
                                            <span>{{ $t('error') }}</span>
                                            <span>{{batteryInfo.error}}</span>
                                        </li>
                                    </ul>
                                </div>
                            </div>

                        </el-collapse-item>
                    </el-collapse>
                </div>
                <!--读取数据库数据-->
                <div class="info online">
                    <el-collapse v-model="activeNames">
                        <!--motor-->
                        <el-collapse-item name="5">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-motor.png" />
                                <label class="title-text">{{$t('cNam.motor')}}</label>
                                <el-input class="input-sn" v-model="motorInfo.sn" :placeholder="$t('sn')"
                                          @click.native.stop="stopPrevent"
                                          @blur="blurMotorSN"></el-input>
                                <div class="title-solution"
                                     v-if="result.motor !== '' && result.motor !== 'OK' && result.motor !== ''">
                                    <el-button type="text" @click.native.stop="showSolutionDialog">
                                        {{ $t('solution') }}
                                    </el-button>
                                </div>
                                <vue-loading class="title-loading" type="spin" color="#E26829"
                                             v-if="blur.motor"></vue-loading>
                            </template>
                            <div class="info-list-box">
                                <div class="info-list">
                                    <div class="col-left">
                                        <ul class="list-group mr-tpo-0 ">
                                            <li class="list-group-item no-border-top">
                                                <span>SN</span>
                                                <span>{{motorInfo.sn || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.power') }}</span>
                                                <span>{{motorInfo.power || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.voltage') }}</span>
                                                <span>{{motorInfo.voltage || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <div class="col-right">
                                        <ul class="list-group mr-tpo-0">
                                            <li class="list-group-item no-border-top">
                                                <span>{{ $t('cNam.model') }}</span>
                                                <span>{{motorInfo.name || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.wheelSize') }}</span>
                                                <span>{{motorInfo.wheelSize || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.rpm') }}</span>
                                                <span>{{motorInfo.rpm || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <ul class="list-group list-all" v-if="motorInfo.error">
                                        <li class="list-group-item">
                                            <span>{{ $t('error') }}</span>
                                            <span>{{motorInfo.error}}</span>
                                        </li>
                                    </ul>
                                </div>
                            </div>

                        </el-collapse-item>
                        <!--Pedal Sensor-->
                        <el-collapse-item name="6">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-sensor.png" />
                                <label class="title-text">{{ $t('cNam.sensor') }}</label>
                                <el-input class="input-sn" v-model="pedalSensorInfo.sn" :placeholder="$t('sn')"
                                          @click.native.stop="stopPrevent"
                                          @blur="blurSensorSN"></el-input>
                                <div class="title-solution"
                                     v-if="result.pedalSensor !== '' && result.pedalSensor !== 'OK' && result.pedalSensor !== ''">
                                    <el-button type="text" @click.native.stop="dialogSolutionVisible = true">
                                        {{ $t('solution') }}
                                    </el-button>
                                </div>
                                <vue-loading class="title-loading" type="spin" color="#E26829"
                                             v-if="(showLoading.sensor && isCan) || blur.sensor"></vue-loading>
                                <el-progress v-show="showPercentage.sensor && isCan" :text-inside="true" :stroke-width="18"
                                             :percentage="percentage.sensor" status="success"
                                             class="title-progress"></el-progress>
                                <el-button type="primary"
                                           v-if="updateFlag['sensor'] && isCan"
                                           @click.stop="showHwDialog('sensor')">{{ $t('update') }}</el-button>

                            </template>
                            <div class="info-list-box">
                                <div class="info-list">
                                    <div class="col-left">
                                        <ul class="list-group mr-tpo-0">
                                            <li class="list-group-item no-border-top">
                                                <span>SN</span>
                                                <span>{{pedalSensorInfo.sn || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.softwareVersion') }}</span>
                                                <span>{{pedalSensorInfo.version || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <div class="col-right">
                                        <ul class="list-group mr-tpo-0">
                                            <li class="list-group-item no-border-top">
                                                <span>{{ $t('cNam.model') }}</span>
                                                <span>{{pedalSensorInfo.name || "-"}}</span>
                                            </li>
                                            <li class="list-group-item">
                                                <span>{{ $t('cNam.hardwareVersion') }}</span>
                                                <span>{{pedalSensorInfo.hardVersion || "-"}}</span>
                                            </li>
                                        </ul>
                                    </div>
                                    <ul class="list-group list-all" v-if="pedalSensorInfo.error">
                                        <li class="list-group-item">
                                            <span>{{ $t('error') }}</span>
                                            <span>{{pedalSensorInfo.error}}</span>
                                        </li>
                                    </ul>
                                </div>
                            </div>
                        </el-collapse-item>
                        <!--Key-->
                        <el-collapse-item name="7">
                            <template slot="title">
                                <img class="icon" src="../../../assets/images/icon/icon-key.png" />
                                <label class="title-text">{{ $t('cNam.key') }}</label>
                                <el-input class="input-sn" v-model="vehicleInfo.key"
                                          :placeholder="$t('cNam.keyCard')" :maxlength="45"></el-input>
                            </template>
                        </el-collapse-item>
                        <!--Tracking code-->
                        <!--<el-collapse-item name="8">-->
                        <!--<template slot="title">-->
                        <!--&lt;!&ndash;<img class="icon" src="../../../assets/images/icon/icon-key.png" />&ndash;&gt;-->
                        <!--<label class="title-text left-20">Tracking Code</label>-->
                        <!--<el-input class="input-sn" v-model="vehicleInfo.trackingCode"-->
                        <!--placeholder="Tracking Code" :maxlength="45"></el-input>-->
                        <!--</template>-->
                        <!--</el-collapse-item>-->
                    </el-collapse>
                </div>
                <div class="clear-20"></div>

                <div class="btn-group">
                    <el-button type="preview" @click="clear()">{{ $t('cNam.clear') }}</el-button>
                    <el-button type="preview" @click="readInfo" :disabled="isRead">{{ $t('cNam.read') }}</el-button>
                    <el-button type="primary" @click="saveVehicle" :disabled="!canSave || saveStatus">{{ $t('cNam.save') }}</el-button>
                </div>
                <div class="test">
                    <!--<div class="test-btn" @click="goToTestView" :class="{ active: isTestBtnActive }">-->
                    <!--<div class="test-btn" @click="goToTestView">-->
                        <!--{{ $t('cNam.test') }}-->
                    <!--</div>-->
                </div>
            </div>
        </div>

        <!--视频弹框-->
        <el-dialog :title="$t('connectTip')" custom-class="video-dialog"
                   :visible.sync="dialogVideoVisible">
            <div class="dialog-container">
                <video controls="controls" autoplay="autoplay">
                    <source src="/static/connect-tip-zh.mp4" type="video/mp4" />
                    {{ $t('videoTip') }}
                </video>
            </div>
        </el-dialog>
        <!--图片弹窗-->
        <el-dialog :title="$t('connectTipVideo')" :visible.sync="dialogTipVisible" class="tip-dialog">
            <div class="dialog-container-pic">
                <video class="img" autoplay="autoplay" controls="controls">
                    <source src="/static/connect-tip-zh.mp4" type="video/mp4" />
                    {{ $t('videoTip') }}
                </video>
            </div>
            <span slot="footer" class="dialog-footer">
                <el-button type="preview" @click="readInfo">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogTipVisible = false">{{ $t('cNam.cancel') }}</el-button>
            </span>
        </el-dialog>

        <!--额外sn弹窗-->
        <el-dialog :title="$t('tip')" :visible.sync="extDialogVisible">
            <div>
                {{$t('saveSnText')}}
            </div>
            <div class="clear-10"></div>
            <div v-for="item in extSnList">
                <span v-if="item.type === Const.CATEGORY_ROOT_TYPE.HMI" class="width-80">{{ $t('cNam.hmi') }}</span>
                <span v-if="item.type === Const.CATEGORY_ROOT_TYPE.MOTOR" class="width-80">{{ $t('cNam.motor') }}</span>
                <span v-if="item.type === Const.CATEGORY_ROOT_TYPE.CONTROLLER" class="width-80">{{ $t('cNam.controller') }}</span>

                <span class="width-460">SN: {{item.sn}}</span>
                <el-button type="primary" @click="saveSimpleItem(item)">{{ $t('cNam.save') }}</el-button>
                <div class="clear-20"></div>
            </div>
            <span slot="footer" class="dialog-footer">
                <el-button type="preview" @click="extDialogVisible = false">{{ $t('confirm') }}</el-button>
            </span>
        </el-dialog>

        <!--固件的历史记录弹窗  :before-close="handleCloseFirmwareListDialog" -->
        <el-dialog :title="$t('historyDialog.title')" :visible.sync="historyDialogVisible" class="history-dialog">
            <table border="0" cellspacing="0" cellpadding="0" class="history-table" v-if="HWRecord.length > 0">
                <thead>
                <tr>
                    <td class="history-td head">
                        {{ $t('historyDialog.version') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('historyDialog.updateTime') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('historyDialog.remark') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('historyDialog.operation') }}
                    </td>
                </tr>
                </thead>
                <tr v-for="item in HWRecord">
                    <td class="history-td">{{item.version}}</td>
                    <td class="history-td">{{item.create_time | dateFormat}}</td>
                    <td class="history-td">{{item.remark}}</td>
                    <td class="history-td">
                        <el-button class="btn-underline" type="text" @click="updateObject(item)">{{ $t('update') }}</el-button>
                    </td>
                </tr>
            </table>
            <div class="clear-10"></div>
            <div class="progress-wrap">
                <el-progress v-show="updatePercentage > 0" :percentage="updatePercentage"></el-progress>
            </div>
            <div class="clear-10"></div>
            <div class="clear-float">
                <span> {{$t('cNam.updateMsgTip')}}</span><el-button @click="resetUpdate" class="pull-right">{{ $t('reset') }}</el-button>
            </div>
        </el-dialog>
    </div>

</template>

<script>
    import HMI from 'device/hmi'
    import Battery from 'device/battery'
    import Controller from 'device/controller'
    import Sensor from 'device/sensor'
    import Diagnosis from 'device/diagnose'
    import Update from "device/update"
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
                Const: Core.Const,

                HMI: HMI,
                Battery: Battery,
                Controller: Controller,
                Sensor: Sensor,

                activeNames: ['1'],
                device: HMI,
                deviceList: {hmi: HMI, battery: Battery, controller: Controller, sensor: Sensor},
                comList: '',
                com: '',
                isConnected: false,
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
                //
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
                    clientCodeRead: '',
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
                    error: '',
                    speedLimit: '',
                    wheelDiameter: '',
                    circumference: ''
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
                    error: ''
                },

                //硬件配置
                config: {
                    hmi: {device: HMI, time: 6000, key: 'hmi'},
                    ctrl: {device: Controller, time: 7800, key: 'ctrl'},
                    bat: {device: Battery, time: 12000, key: 'bat'},
                    sen: {device: Sensor, time: 3000, key: 'sen'}
                },

                stateTime: {
                    time1:0,
                    time2:6,
                    time3:14,
                    time4:26,
                    time5:29
                },

                percentage: {
                    hmi: 0,
                    controller: 0,
                    battery: 0,
                    sensor: 0,
                },

                showPercentage: {
                    hmi: false,
                    controller: false,
                    battery: false,
                    sensor: false
                },
                showLoading: {
                    hmi: false,
                    controller: false,
                    battery: false,
                    sensor: false
                },

                result: {
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
                },

                dialogVideoVisible: false,
                dialogTipVisible: false,
                dialogSolutionVisible: false,
                componentError: {},

                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',

                reportInfo: {},

                dialogConfirmVisible: false,

                finished: false,

                // 错误选择弹框
                dialogErrorSelectVisible: false,
                selectErrorList: [],
                selectItem: '',
                errorList: [],

                //oem 需要加的的东西
                itemInfo: {item: {amount: '', produced: '',}},
                orderInfo: {},
                orderId: '',
                itemId: '',
                isBindBattery: false,
                vehicleInfo: {
                    vehicleSn: '',
                    objectIdList: [],
                    key: '',
                    trackingCode: ''
                },
                componentSn: '',

                //判断是否包含了部件
                has: {
                    bat: false,
                    ctrl: false,
                    sensor: false,
                    motor: false,
                    hmi: false
                },
                // 鼠标失去焦点
                blur: {
                    bat: false,
                    ctrl: false,
                    sensor: false,
                    motor: false,
                    hmi: false
                },

                canUseSn: true,

                bomList: [],

                isRead: false,
                canSave: true,
                saveStatus: false,

                // 不存在于数据库的sn
                extSnList: [],
                extDialogVisible: false,


                //  固件弹窗
                HWRecord: [],
                file: {},
                remark: '',
                HWVersion: '',
                fileList: [],
                historyDialogVisible: false,
                updatePercentage: 0,
                updateComponentType: '',

                //是否版本差异判断 flag
                updateFlag: {
                    'hmi': false,
                    'ctrl': false,
                    'sensor': false,
                    'bat': false
                },
                recordType: Core.Const.OBJECT_RECORD_TYPE.UPDATE,

                isCan: false,
                usbHID: false,
                isReset: false,
                wheelDiameters: [],

                reconnectInterval: null,
                connectionIntervalProduce: null,
                updateMsgObject: {},

            }
        },
        i18n: {
            messages: {
                en: EN.Assembler.AssemblerOrderProduce,
                zh: ZH.Assembler.AssemblerOrderProduce,
                de: DE.Assembler.AssemblerOrderProduce,
                nl: NL.Assembler.AssemblerOrderProduce
            }
        },
        mounted: function () {
            this.orderId = this.$route.query.orderId;
            this.itemId = this.$route.query.itemId;
            let from = this.$route.query.from;
            if (from === 'test') {
                this.isConnected = true;
                if (this.$route.query.isBindBat == true) {
                    this.isBindBattery = true;
                }
            }
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
//                    this.initData();
                }, 100);
            });

            // 获取item详情
            this.getObjectDetail();
            this.checkConnection();

            if (this.isTestBtnActive) {
                this.$message({
                    type: 'warning',
                    message: this.$t('cNam.comDisconnect')
                });
            }

            this.wheelDiameters = Core.Const.wheelDiameters;
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
                readFailTip: this.$t('cNam.updateReadFail')
            };
        },

        methods: {
            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalProduce){
                    clearInterval(vm.connectionIntervalProduce);
                    vm.connectionIntervalProduce = '';
                }
                vm.connectionIntervalProduce = setInterval(()=>{
                    let connection = HMI.port.getConnection();
                    if(connection.usb){
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalProduce);
                    }
                },300)
            },
            init(){
                HMI.port.startConnection();
                if(HMI.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
//                    console.log(" get USB Device Produce");
                    if(this.isReset){
                        this.connect(1);
                    }else{
                        this.connect();
                    }
                }else{
//                    console.log("COM HMI");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = HMI.port.isConnected();
            },

            setupData() {
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
            },

            showVideoDialog() {
                this.dialogVideoVisible = true;
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
                    this.isConnected = HMI.port.isConnected();
                    this.isCan = !HMI.port.checkUsbUart();
                    if(this.isCan){
                        this.config.hmi.time = 2000;
                        this.config.ctrl.time = 2000;
                        this.config.bat.time = 2000;
                        this.config.sen.time = 1200;
                        this.stateTime = {
                            time1:0,
                            time2:2,
                            time3:4,
                            time4:6,
                            time5:8
                        };
                    }else {
                        this.config.hmi.time = 6500;
                        this.config.ctrl.time = 7800;
                        this.config.bat.time = 10000;
                        this.config.sen.time = 1100;
                        this.stateTime = {
                            time1:0,
                            time2:7,
                            time3:15,
                            time4:25,
                            time5:27
                        };
                    }
//                    console.log("usbHID connect");
                    HMI.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type === 'error_usb') {
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
                    }
                    this.dialogTipVisible = true;
                    let bus = this.device.connect(this.com);
                    if(bus){
                        bus.subscribe(res => {
                            if (res.type === 'open' || res.type === 'connect') {
                                this.isConnected = true;
                                Core.Data.setPort(this.com);
                            }
                            if (res.type === 'close' || res.type === 'error') {
//                            this.com = '';
                                this.isConnected = false;
                                this.setupData();
//                            this.$message({
//                                message: 'Com is disconnect',
//                                type: 'warning'
//                            });
                            }
                        });
                    }
                }
            },

            test(){
                HMI.port.checkUsbConnection('0101',1);
            },

            // 以下2个，是硬件读取的
            showHwDialog(type) {
                this.HWRecord.length = 0;
                this.HWRecord = [];
                if( type === 'ctrl'){
                    this.updateComponentType = 'ctrl';
                    this.getFirmwareBySn(this.controllerInfo.sn);
                }else if(type === 'hmi'){
                    this.updateComponentType = 'hmi';
                    this.getFirmwareBySn(this.hmiInfo.sn);
                }else if(type === 'sensor'){
                    this.updateComponentType = 'sensor';
                    this.getFirmwareBySn(this.pedalSensorInfo.sn);
                }else if(type === 'bat'){
                    this.updateComponentType = 'bat';
                    this.getFirmwareBySn(this.batteryInfo.sn);
                }
                this.historyDialogVisible = true;
            },


            //真正的读取方法
            readBase(device) {
                if (!this.isConnected) {
                    this.$message({
                        type: 'warning',
                        message: this.$t('cNam.comDisconnect')
                    });
                    return;
                }
                let vm = this;
                switch (device) {
                    case HMI:
                        vm.isRead = true;
//                        console.log(vm.isRead, 'hmi1');
                        vm.showPercentage.hmi = true;
                        vm.showLoading.hmi = true;
                        setTimeout(function () {
                            vm.result.hmiOK = vm.hmiInfo.sn && vm.hmiInfo.sn !== '-';
                            if (!vm.result.hmi) {
                                vm.result.hmi = vm.result.hmiOK ? 'OK' : '';
                            }
                            vm.showLoading.hmi = false;
                            vm.showPercentage.hmi = false;
                            vm.isRead = false;
//                            console.log(vm.isRead, 'hmi2');
                        }, vm.config.hmi.time);

                        if (!vm.isCan || HMI.port.checkUsbUart()) {
                            if (!vm.usbHID) {
                                HMI.connect(vm.com);
                            }
                            HMI.sendCmdGetFullData();
                            HMI.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    vm.isConnected = false;
                                    vm.com = '';
                                }
                                if (!res.data || !res.data.hmi) {
                                    return;
                                }
                                if (res.data.hmi.hasOwnProperty("sn")) {
                                    vm.hmiInfo.sn = res.data.hmi.sn;
                                    vm.blurHMISN();
                                }
                                vm.hmiInfo.model = Core.Util.get(res, 'data.hmi.model', vm.hmiInfo.model);
                                vm.hmiInfo.version = Core.Util.get(res, 'data.hmi.version', vm.hmiInfo.version);
                                vm.hmiInfo.hardVersion = Core.Util.get(res, 'data.hmi.hardware_version', vm.hmiInfo.hardVersion);
                                vm.hmiInfo.wheelDiameter = Core.Util.get(res, 'data.hmi.wheel_diameter', vm.hmiInfo.wheelDiameter);
                                vm.hmiInfo.speedLimit = Core.Util.get(res, 'data.hmi.speed_limit', vm.hmiInfo.speedLimit);
                                vm.hmiInfo.totalMileage = Core.Util.get(res, 'data.hmi.total_mileage', vm.hmiInfo.totalMileage);
                                vm.hmiInfo.totalGear = Core.Util.get(res, 'data.hmi.total_gear', vm.hmiInfo.totalGear);
                                if (res.data.hmi.hasOwnProperty("client_code")) {
                                    vm.hmiInfo.clientCodeRead = res.data.hmi.client_code
                                }
                                vm.percentage.hmi = vm.getPercentage([vm.hmiInfo.sn, vm.hmiInfo.model,
                                    vm.hmiInfo.version, vm.hmiInfo.hardVersion, vm.hmiInfo.wheelDiameter,
                                    vm.hmiInfo.speedLimit]);
                                if (vm.percentage.hmi == 100) {
                                    vm.showPercentage.hmi = false;
                                } else {
                                    vm.showPercentage.hmi = true;
                                }
                                vm.result.hmiOK = vm.hmiInfo.sn !== '-';
                            });
                        }else {
                            HMI.port.getUsbBasic(0x03).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    vm.hmiInfo.sn = res.data.sn;
                                    vm.blurHMISN();
                                }
                                if (res.data.hasOwnProperty("model")) {
                                    vm.hmiInfo.model = res.data.model;
                                }
                                if (res.data.hasOwnProperty("version")) {
                                    vm.hmiInfo.version = res.data.version;
                                }
                                if (res.data.hasOwnProperty("hardware_version")) {
                                    vm.hmiInfo.hardVersion = res.data.hardware_version;
                                }
                                if (res.data.hasOwnProperty("client_code")) {
                                    vm.hmiInfo.clientCodeRead = res.data.client_code;
                                }
                                vm.percentage.hmi = vm.getPercentage([vm.hmiInfo.sn, vm.hmiInfo.model,
                                    vm.hmiInfo.version, vm.hmiInfo.hardVersion, vm.hmiInfo.clientCodeRead]);
                                if (vm.percentage.hmi == 100) {
                                    vm.showPercentage.hmi = false;
                                } else {
                                    vm.showPercentage.hmi = true;
                                }
                                vm.result.hmiOK = vm.hmiInfo.sn !== '-';
                            });
                            HMI.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05,0x03)
                                .then(res => {
                                    let result_1 = Buffer.from(res.data, 'hex');
                                    vm.hmiInfo.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2]*256*256;
                                });
                            HMI.port.getListenData().subscribe((res)=> {
                                if (!res.data){
                                    return;
                                }
                                if (res.type === 'error_usb'){
                                    vm.isConnected = false;
                                }
                                if (res.data.code === '63' && res.data.subCode === '00') {
                                    let temTotal = Buffer.from(res.data.data, 'hex');
                                    let temString_0 = HMI.port.addZero(temTotal[0].toString(2), 8);
                                    vm.hmiInfo.totalGear = parseInt(temString_0.substr(4, 4), 2);
//                                    Diagnosis.setGearMode(this.hmiInfo.totalGear);
                                }
                            });
                        }

                        break;
                    case Controller:
                        this.isRead = true;
//                        console.log(this.isRead, "Ctr1");
                        this.showPercentage.controller = true;
                        this.showLoading.controller = true;
                        setTimeout(function () {
                            vm.result.controllerOK = vm.controllerInfo.sn && vm.controllerInfo.sn !== '-';
                            if (!vm.result.controller) {
                                vm.result.controller = vm.result.controllerOK ? 'OK' : '';
                            }
                            vm.isRead = false;
                            vm.showLoading.controller = false;
                            vm.showPercentage.controller = false;
                            vm.isRead = false;
                            if (!vm.isBindBattery) {
                                vm.finished = true;
                                vm.isTestBtnActive = true;
                                if( vm.hmiInfo.sn && vm.hmiInfo.version){
                                    vm.compareVersion(vm.hmiInfo.sn, vm.hmiInfo.version, 'hmi');
                                }
                                if( vm.controllerInfo.sn && vm.controllerInfo.version) {
                                    vm.compareVersion(vm.controllerInfo.sn, vm.controllerInfo.version, 'ctrl');
                                }
                            }
                        }, vm.config.ctrl.time);
                        if (!vm.isCan || Controller.port.checkUsbUart()) {
                            if (!vm.usbHID) {
                                Controller.connect(vm.com);
                            }
                            Controller.sendCmdGetFullData();
                            Controller.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    vm.isConnected = false;
                                    vm.com = '';
                                }
                                if (!res.data || !res.data.ctrl) {
                                    return;
                                }
                                if (res.data.ctrl.hasOwnProperty("sn")) {
                                    vm.controllerInfo.sn = res.data.ctrl.sn;
                                    vm.blurControllerSN();
                                }
                                this.controllerInfo.model = Core.Util.get(res, 'data.ctrl.model', this.controllerInfo.model);
                                this.controllerInfo.version = Core.Util.get(res, 'data.ctrl.version', this.controllerInfo.version);
                                this.controllerInfo.manufacturer = Core.Util.get(res, 'data.ctrl.man_name', this.controllerInfo.manufacturer);
                                this.controllerInfo.nmlVolt = Core.Util.get(res, 'data.ctrl.nml_volt', this.controllerInfo.nmlVolt);
                                this.controllerInfo.maxCurrent = Core.Util.get(res, 'data.ctrl.max_cur', this.controllerInfo.maxCurrent);
                                this.controllerInfo.hardVersion = Core.Util.get(res, 'data.ctrl.hardware_version', this.controllerInfo.hardVersion);

                                this.controllerInfo.spdMeterType = Core.Util.get(res, 'data.ctrl.spd_meter_type', this.controllerInfo.spdMeterType);
                                this.controllerInfo.spdMeterSignal = Core.Util.get(res, 'data.ctrl.spd_meter_signal_num', this.controllerInfo.spdMeterSignal);
                                if (res.data.ctrl.hasOwnProperty("wheel_dia")) {
                                    this.controllerInfo.wheelDiameter = res.data.ctrl.wheel_dia / 2;
                                }

                                this.percentage.controller = this.getPercentage([this.controllerInfo.sn, this.controllerInfo.model,
                                    this.controllerInfo.version, this.controllerInfo.manufacturer, this.controllerInfo.nmlVolt,
                                    this.controllerInfo.maxCurrent, this.controllerInfo.hardVersion]);
                                if (this.percentage.controller == 100) {
                                    this.showPercentage.controller = false;
                                } else {
                                    this.showPercentage.controller = true;
                                }
                                this.result.controllerOK = this.controllerInfo.sn !== '-';
                            });
                        }else {
                            Controller.port.getUsbBasic(0x02).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    this.controllerInfo.sn = res.data.sn;
                                    this.blurControllerSN();
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
                                this.percentage.controller = this.getPercentage([this.controllerInfo.sn, this.controllerInfo.model,
                                    this.controllerInfo.version,  this.controllerInfo.hardVersion]);
                                if (this.percentage.controller == 100) {
                                    this.showPercentage.controller = false;
                                } else {
                                    this.showPercentage.controller = true;
                                }
                                this.result.controllerOK = this.controllerInfo.sn !== '-';
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
                        this.isRead = true;
//                        console.log(this.isRead, "bat111");
                        this.showPercentage.battery = true;
                        this.showLoading.battery = true;
                        setTimeout(function () {
                            vm.isTestBtnActive = true;
                            vm.isRead = false;
//                            console.log(vm.isRead, "bat222");
                            vm.result.batteryOK = vm.batteryInfo.sn && vm.batteryInfo.sn !== '-';
                            if (!vm.result.battery) {
                                vm.result.battery = vm.result.batteryOK ? 'OK' : '';
                            }
                            vm.showLoading.battery = false;
                            vm.showPercentage.battery = false;
                            vm.finished = true;
                            if( vm.hmiInfo.sn && vm.hmiInfo.version){
                                vm.compareVersion(vm.hmiInfo.sn, vm.hmiInfo.version, 'hmi');
                            }
                            if( vm.controllerInfo.sn && vm.controllerInfo.version) {
                                vm.compareVersion(vm.controllerInfo.sn, vm.controllerInfo.version, 'ctrl');
                            }
                        }, vm.config.bat.time);
                        if (!vm.isCan || Battery.port.checkUsbUart()) {
                            if (!vm.usbHID) {
                                Battery.connect(vm.com);
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
                                if (res.data.battery.hasOwnProperty('sn')) {
                                    this.batteryInfo.sn = res.data.battery.sn;
                                    this.blurBatterySN();
                                }
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
                                this.percentage.battery = this.getPercentage([this.batteryInfo.sn, this.batteryInfo.model,
                                    this.batteryInfo.version, this.batteryInfo.hardVersion, this.batteryInfo.soc,
                                    this.batteryInfo.soh, this.batteryInfo.fullChargerCapacity,
                                    this.batteryInfo.leftCapacity, this.batteryInfo.temperature,
                                    this.batteryInfo.current, this.batteryInfo.loopTime, this.batteryInfo.maxChargeInterval]);
                                if (this.percentage.battery == 100) {
                                    this.showPercentage.battery = false;
                                } else {
                                    this.showPercentage.battery = true;
                                }
                                this.result.batteryOK = this.batteryInfo.sn !== '-';
                            });
                        }else{
                            Battery.port.getUsbBasic(0x04).subscribe(res => {
                                if (!res.data) {
                                    return;
                                }
                                if (res.data.hasOwnProperty("sn")) {
                                    this.batteryInfo.sn = res.data.sn;
                                    this.blurBatterySN();
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
                                this.percentage.battery = this.getPercentage([this.batteryInfo.sn, this.batteryInfo.model,
                                    this.batteryInfo.version, this.batteryInfo.hardVersion, this.batteryInfo.soc,
                                    this.batteryInfo.soh, this.batteryInfo.fullChargerCapacity,
                                    this.batteryInfo.leftCapacity, this.batteryInfo.temperature,
                                    this.batteryInfo.current, this.batteryInfo.loopTime, this.batteryInfo.maxChargeInterval]);
                                if (this.percentage.battery == 100) {
                                    this.showPercentage.battery = false;
                                } else {
                                    this.showPercentage.battery = true;
                                }
                                this.result.batteryOK = this.batteryInfo.sn !== '-';
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
                        this.isRead = true;
//                        console.log(this.isRead, "Sensor");
                        this.showPercentage.sensor = true;
                        this.showLoading.sensor = true;
                        setTimeout(function () {
                            vm.result.pedalSensorOK = vm.pedalSensorInfo.sn && vm.pedalSensorInfo.sn !== '-';
                            if (!vm.result.pedalSensor) {
                                vm.result.pedalSensor = vm.result.pedalSensorOK ? 'OK' : '';
                            }
                            vm.isRead = false;
                            vm.showLoading.sensor = false;
                            vm.showPercentage.sensor = false;
                            vm.isRead = false;
                            if (!vm.isBindBattery) {
                                vm.finished = true;
                                vm.isTestBtnActive = true;
                                if( vm.hmiInfo.sn && vm.hmiInfo.version){
                                    vm.compareVersion(vm.hmiInfo.sn, vm.hmiInfo.version, 'hmi');
                                }
                                if( vm.controllerInfo.sn && vm.controllerInfo.version) {
                                    vm.compareVersion(vm.controllerInfo.sn, vm.controllerInfo.version, 'ctrl');
                                }
                            }
                        }, vm.config.sen.time);
                        Sensor.port.getUsbBasic(0x01).subscribe(res => {
                            if (!res.data) {
                                return;
                            }
                            if (res.data.hasOwnProperty("sn")) {
                                this.pedalSensorInfo.sn = res.data.sn;
                                this.blurSensorSN();
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
                            this.percentage.sensor = this.getPercentage([this.pedalSensorInfo.sn, this.pedalSensorInfo.model,
                                this.batteryInfo.version, this.pedalSensorInfo.hardVersion]);
                            if (this.percentage.sensor == 100) {
                                this.showPercentage.sensor = false;
                            } else {
                                this.showPercentage.sensor = true;
                            }
                            this.result.pedalSensorOK = this.pedalSensorInfo.sn !== '-';
                        });
                        break;
                }
            },

            getPercentage(keys) {
                let okAmount = 0;
                // for (let index in keys) {
                //     if (keys[index] !== '' && keys[index] !== '-') {
                //         okAmount++;
                //     }
                // }
                keys.forEach(item => {
                    if (item !== '' && item !== '-') {
                        okAmount++;
                    }
                });

                // console.log(okAmount, keys.length);
                let percent = Math.ceil(okAmount / (keys.length) * 100);
                // console.log('percent', percent);
                return percent;
            },

            changeCom() {
//                this.comTip = 'not connected';
            },

            //读取3个的全部数据
            readInfo() {
                if (!this.isConnected) {
                    this.$message({
                        type: 'warning',
                        message: this.$t('cNam.comDisconnect')
                    });
                    return;
                }

                let vm = this;
                this.dialogTipVisible = false;
                this.isTestBtnActive = false;

                let hmiOk = false;
                let controllerOk = false;
                let batterOk = false;

                let stage = 0;
                let nextStage = 1;
                let currentTime = Core.Util.time();
                let timePassed = 0;

                let checkInterval = setInterval(function () {

                    timePassed = Core.Util.time() - currentTime;
                    if (vm.stateTime.time1 <= timePassed && timePassed <= vm.stateTime.time2) {
                        nextStage = 1;
                        if (hmiOk) {
                            nextStage = 2;
                        }
                    } else if (vm.stateTime.time2 < timePassed && timePassed <= vm.stateTime.time3) {
                        nextStage = 2;
                        if (controllerOk) {
                            nextStage = 3;
                        }
                    } else if (vm.stateTime.time3 <= timePassed && timePassed <= vm.stateTime.time4) {
                        nextStage = 3;
                        if (batterOk) {
                            nextStage = 4;
                        }
//                    } else if (vm.stateTime.time4 <= timePassed && timePassed <= vm.stateTime.time5){
//                        if(vm.isCan){
//                            nextStage = 4;
//                        }
                    } else if (vm.stateTime.time5 < timePassed) {
                        clearInterval(checkInterval);
                    }

                    if (stage != nextStage) {
                        stage = nextStage;
                        switch (stage) {
                            case 1:
//                                console.log('hmi');
                                vm.readBase(vm.deviceList['hmi']);
                                break;
                            case 2:
//                                console.log('controller');
                                vm.readBase(vm.deviceList['controller']);
                                break;
                            case 3:
//                                console.log('battery');
                                if (vm.isBindBattery) {
                                    vm.readBase(vm.deviceList['battery']);
                                }
                                break;
                            case 4:
//                                console.log('sensor');
                                if (vm.isCan) {
//                                    vm.readBase(vm.deviceList['sensor']);
                                }
                                break;
                        }
                    }
                }, 50);

                // let bus = this.device.connect(this.com);

                // this.readBase(HMI);

                // setTimeout(function () {
                //     vm.readBase(Controller)
                // }, 8000);
                // if(this.isBindBattery){
                //     setTimeout(function () {
                //         vm.readBase(Battery)
                //     }, 16000);
                // }
            },

            stopPrevent() {
                console.log("stop prevent");
            },

            //去诊断的第二页面  // 先保存车
//            goToTestView() {
//                this.$message({
//                    message: 'We are developing 3rd version of test. This is too old . Sorry for any inconvenience',
//                    type: 'warning'
//                });
                /***
                 if (!this.isTestBtnActive) {
                     return;
                 }
                 if (this.extSnList.length > 0) {
                     this.extDialogVisible = true;
                     return;
                 }
                 this.getIdList();
                 if (!this.checkParams()) {
                    return;
                }


                 let orderId = this.orderId;
                 let itemId = this.itemId;
                 let sn = this.vehicleInfo.vehicleSn;
                 let objectList = this.vehicleInfo.objectIdList.join();
                 let secretKey = this.vehicleInfo.key;
                 let produceTime = Core.Util.time();
                 let changePage =  () => {
                    this.reportInfo = {
                        device: {
                            vehicleSn:this.vehicleInfo.vehicleSn,
                            hmi: this.hmiInfo,
                            controller: this.controllerInfo,
                            battery: this.batteryInfo,
                        },
                        result: this.result,
                    };

                    // todo
                    // let morkParams = {
                    //     com: 'com3',
                    //     orderInfo: {orderId: 1, itemId: 1},
                    //     isBindBat: this.isBindBattery,
                    //     report: {
                    //         device: {
                    //             vehicleSn:11111,
                    //             hmi: {},
                    //             controller: {},
                    //             battery: {},
                    //         },
                    //         result:{},
                    //     }
                    // };

                    this.$router.push({
                        name: "assemblerDiagnosisTest",
                        params: {
                            com: this.com,
                            orderInfo: {orderId: this.orderId, itemId: this.itemId},
                            isBindBat: this.isBindBattery,
                            report: this.reportInfo,
                        }
                        // params: morkParams,
                    });
                };

                 Core.Api.Vehicle.saveVehicle(orderId, itemId, sn, objectList, secretKey, produceTime).then(res => {
                     //去页面
                    changePage();
                 });
                 console.log(changePage);

                 */

//            },

            //获取这个车型的信息
            getObjectDetail() {
//                console.log('detail', this.itemId);
                Core.Api.Item.getItemDetail(this.itemId).then(res => {
//                    console.log(res);
                    this.itemInfo = res.item;
                    this.category_root = res.item.category_root_id;
                    this.getProductionOrderDetail();
                });
            },

            //获取生产单信息
            getProductionOrderDetail() {
                let $this = this;
                Core.Api.Order.getProductionOrderDetail(this.orderId).then(res => {
                    $this.orderInfo = res.order;
                    $this.orderInfo.item_list.forEach(function (e) {
                        if (e.item_id === $this.itemInfo.id) {
                            $this.itemInfo.item = e;
                        }
                    });

                    if ($this.itemInfo.item.produced >= $this.itemInfo.item.amount) {
                        this.$message({
                            message: this.$t('itemFinishTip'),
                            type: 'warning'
                        });
                        this.goListView();
                    }

                })
            },

            //获取端口号
            getComList() {
//                if (HMI.port.getPort() != '' && HMI.port.isConnected()) {
//                    console.log('rem');
//                    this.com = HMI.port.getPort();
//                    this.comList = [HMI.port.getPort()];
//                    this.isConnected = HMI.port.isConnected();
//                    return
//                }
                HMI.port.getPortList().subscribe(res => {
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

            //生产一辆车
            saveVehicle() {
                if (this.extSnList.length > 0) {
                    this.extDialogVisible = true
                    return;
                }

                let orderId = this.orderId;
                let itemId = this.itemId;
                let sn = this.vehicleInfo.vehicleSn;
                this.getIdList();
                let objectList = this.vehicleInfo.objectIdList.join();
                let secretKey = this.vehicleInfo.key;
                let produceTime = Core.Util.time();
                // 判断是不是还在获取数据
                let  blurStatus = false;
                for(let key in this.blur){
                    if(this.blur[key]){
                        blurStatus = true;
                    }
                }
                if(blurStatus) {
                    this.$message({
                        message: this.$t('cNam.getDataTip'),
                        type: 'warning',
                        duration: 0,
                        showClose: true
                    });
                    return;
                }

                if (!this.checkParams()) {
                    return;
                }
                this.saveStatus = true;
                Core.Api.Vehicle.saveVehicle(orderId, itemId, sn, objectList, secretKey, produceTime).then(res => {
                    this.saveStatus = false;
                    this.$message({
                        message: this.$t('cNam.saveSuccess'),
                        type: 'success'
                    });
                    this.clear();
                    this.getProductionOrderDetail();

                })
            },

            blurCheck(type){
                switch (type) {
                    case Core.Const.CATEGORY_ROOT_TYPE.MOTOR:
                        this.blur.motor = false;

                        break;
                    case Core.Const.CATEGORY_ROOT_TYPE.SENSOR:
                        this.blur.sensor = false;

                        break;
                    case Core.Const.CATEGORY_ROOT_TYPE.BATTERY:
                        this.blur.bat = false;

                        break;
                    case Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER:
                        this.blur.ctrl = false;

                        break;
                    case Core.Const.CATEGORY_ROOT_TYPE.HMI:
                        this.blur.hmi = false;

                        break;
                    default:
                        break;
                }
            },

            //得到SN后触发的方法 - Input 失去焦点
            blurCommonSnFunc(type, sn, msg) {
                let vm = this;
                let status = false;
                let tip = '';
                if (!sn) {
                    return;
                }
                vm.canSave = false;
                Core.Api.Component.componentDetailBySn(sn).then(res => {
                    vm.canSave = true;
                    if (res.component.category_root_id !== type) {
                        vm.$message({
                            message: msg,
                            type: 'error',
                            duration: 0,
                            showClose: true
                        });
                        return;
                    }
                    // 如果获取到了对应的sn需要extSnlist中对应type 删除
                    if (vm.extSnList.length > 0) {
                        for (let index in vm.extSnList) {
                            if (this.extSnList[index].type === type) {
                                this.extSnList.splice(index, 1);
                            }
                        }
                    }
                    vm.blurCheck(type);

                    //2 是已使用 3 是损害
                    if (res.component.status === 2) {
                        tip = sn + " " + vm.$t('isUsed');
                        vm.$message({
                            message: tip,
                            type: 'warning',
                            duration: 0,
                            showClose: true
                        });
                        return;
                    }else if( res.component.status === 3 ){
                        tip = sn + " " + vm.$t('canNotUsed');
                        vm.$message({
                            message: tip,
                            type: 'warning',
                            duration: 0,
                            showClose: true
                        });
                        return;
                    }
                    // get device hard/soft-version
                    switch (type) {
                        case Core.Const.CATEGORY_ROOT_TYPE.MOTOR:
//                            console.log('motor')
                            vm.has['motor'] = true;

                            if (res.component.item && res.component.item.attr_list) {
                                vm.motorInfo.name = res.component.item.name;
                                res.component.item.attr_list.forEach(function (e) {
                                    if (e.attr_def_key === "power") {
                                        vm.motorInfo.power = e.value;
                                    }
                                    if (e.attr_def_key === "voltage") {
                                        vm.motorInfo.voltage = e.value;
                                    }
                                    if (e.attr_def_key === "wheel_size") {
                                        vm.motorInfo.wheelSize = e.value;
                                    }
                                    if (e.attr_def_key === "rpm") {
                                        vm.motorInfo.rpm = e.value;
                                    }
                                });
                            }
                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.SENSOR:
//                            console.log('sensor');
                            vm.has['sensor'] = true;

//                            if(!this.isCan){
                            if (res.component.item && res.component.item.attr_list) {
                                vm.pedalSensorInfo.name = res.component.item.name;
                                res.component.item.attr_list.forEach(function (e) {
                                    if (e.attr_def_key === "hard_ver") {
                                        vm.pedalSensorInfo.hardVersion = e.value;
                                    }
                                    if (e.attr_def_key === "soft_ver") {
                                        vm.pedalSensorInfo.version = e.value;
                                    }
                                });
                            }
//                            }

                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.BATTERY:
//                            console.log('bat');
                            vm.has['bat'] = true;

                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER:
//                            console.log('ctrl');
                            vm.has['ctrl'] = true;

                            break;
                        case Core.Const.CATEGORY_ROOT_TYPE.HMI:
//                            console.log('hmi');
                            vm.has['hmi'] = true;

                            break;
                        default:
                            break;
                    }


                    if (type === Core.Const.CATEGORY_ROOT_TYPE.BATTERY && !vm.isBindBattery) {
                        return;
                    }
                    let idList = [];
                    let categoryLis = [];

                    // 获取已有的bomList
                    vm.bomList.forEach((item, index) => {
                        idList.push(item.id);
                        categoryLis.push(item.root_id);
                    });

                    if (idList.indexOf(res.component.id) > -1) {
                        return;
                    }
                    // 判断这个类型是否已存在 有就讲之前的去除掉
                    let index = categoryLis.indexOf(res.component.category_root_id);

                    if (index > -1) {
                        vm.bomList.splice(index, 1);
                    }

                    vm.bomList.push(
                        {
                            id: res.component.id,
                            root_id: res.component.category_root_id
                        }
                    );
                }).catch(err => {
                    console.log(err);

                    vm.canSave = true;
                    vm.blurCheck(type);

                    if (type === Core.Const.CATEGORY_ROOT_TYPE.SENSOR || type === Core.Const.CATEGORY_ROOT_TYPE.BATTERY) {
                        return
                    }
                    sn = sn.replace(/\s/g, "");
                    let extSn = {sn: sn.trim(), type: type};
                    let list = vm.extSnList;
                    let needPush = true;
                    for (let index in list) {
                        if (list[index].sn === sn) {
                            needPush = false;
                            break;
                        }
                        if (list[index].type === type) {
                            needPush = false;
                            list[index] = extSn;
                            break;
                        }
                    }
                    if (needPush) {
                        vm.extSnList.push(extSn);
                    }
                });
//                console.log(vm.extSnList);

            },

            blurMotorSN() {
                if( this.motorInfo.sn.length > 0 && this.motorInfo.sn.length < 41 ){
                    this.blur.motor = true;
                    this.blurCommonSnFunc(Core.Const.CATEGORY_ROOT_TYPE.MOTOR, this.motorInfo.sn.trim(), this.$t('cNam.hasMotorSn'));
                }else{
                    this.$message.warning(this.$t('cNam.hasMotorSn'));
                }
            },
            blurSensorSN() {
                if( this.pedalSensorInfo.sn.length > 0 && this.pedalSensorInfo.sn.length < 41 ){
                    this.blur.sensor = true;
                    this.blurCommonSnFunc(Core.Const.CATEGORY_ROOT_TYPE.SENSOR, this.pedalSensorInfo.sn.trim(), this.$t('cNam.hasSensorSn'));
                }else{
                    this.$message.warning(this.$t('cNam.hasSensorSn'));
                }
            },
            blurBatterySN() {
                if(!this.isBindBattery){
                    return;
                }
                if( this.batteryInfo.sn.length > 0 && this.batteryInfo.sn.length < 41 ){
                    this.blur.bat = true;
                    this.blurCommonSnFunc(Core.Const.CATEGORY_ROOT_TYPE.BATTERY, this.batteryInfo.sn.trim(), this.$t('cNam.hasBatSn'));
                }else{
                    this.$message.warning(this.$t('cNam.hasBatSn'));
                }
            },
            blurHMISN() {
                if( this.hmiInfo.sn.length > 0 && this.hmiInfo.sn.length < 41 ){
                    this.blur.hmi = true;
                    this.blurCommonSnFunc(Core.Const.CATEGORY_ROOT_TYPE.HMI, this.hmiInfo.sn.trim(), this.$t('cNam.hasHmiSn'));
                }else{
                    this.$message.warning(this.$t('cNam.hasHmiSn'));
                }
            },
            blurControllerSN() {
                if( this.controllerInfo.sn.length > 0 && this.controllerInfo.sn.length < 41 ){
                    this.blur.ctrl = true;
                    this.blurCommonSnFunc(Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER, this.controllerInfo.sn.trim(), this.$t('cNam.hasCtlSn'));
                }else{
                    this.$message.warning(this.$t('cNam.hasCtlSn'));
                }
            },
            //清除数据
            clear() {
                this.setupData();
                this.extSnList = [];
                this.has = {
                    bat: false,
                    ctrl: false,
                    sensor: false,
                    motor: false,
                    hmi: false,
                };
                this.vehicleInfo = {
                    vehicleSn: '',
                    objectIdList: [],
                    key: '',
                };
                this.bomList = [];
                this.percentage = {
                    hmi: 0,
                    controller: 0,
                    battery: 0,
                    sensor: 0
                };

                this.updateFlag = {
                    'hmi': false,
                    'ctrl': false,
                    'sensor': false,
                    'bat': false
                };
                this.showPercentage = {
                    hmi: false,
                    controller: false,
                    battery: false,
                    sensor: false
                };
                this.showLoading = {
                    hmi: false,
                    controller: false,
                    battery: false,
                    sensor: false
                };
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

                if(!this.usbHID || !this.isCan){
                    this.reset()
                }
            },

            reset() {
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                HMI.port.reset(100);
                this.isConnected = false;
                if(!this.isCan || HMI.port.checkUsbUart()){
                    this.$message({
                        message: this.$t('cNam.resetTip'),
                        type: 'info'
                    });
                }
            },

            checkParams() {
                if (!this.vehicleInfo.vehicleSn) {
                    this.$message({
                        message: this.$t('inputBikeSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if(Core.Util.verifyStringNaming(this.vehicleInfo.vehicleSn)){
                    this.$message({
                        message:  this.$t('vehicleSn') + this.$t('cNam.strNaming'),
                        type: 'warning',
                        duration: 0,
                        showClose: true
                    });
                    return false;
                }
                if (!this.canUseSn) {
                    this.$message({
                        message: this.$t('bikeSnExt'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.motorInfo.sn) {
                    this.$message({
                        message: this.$t('inputMotorSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.hmiInfo.sn) {
                    this.$message({
                        message: this.$t('inputHmiSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.controllerInfo.sn) {
                    this.$message({
                        message: this.$t('inputCtlSn'),
                        type: 'warning'
                    });
                    return false;
                }
//                if (!this.pedalSensorInfo.sn) {
//                    this.$message({
//                        message: this.$t('inputSensorSn'),
//                        type: 'warning'
//                    });
//                    return false;
//                }
                if (!this.has['hmi']) {
                    this.$message({
                        message: this.$t('cNam.hasHmiSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.has['ctrl']) {
                    this.$message({
                        message: this.$t('cNam.hasCtlSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if (this.isBindBattery && !this.has['bat']) {
                    this.$message({
                        message: this.$t('cNam.hasBatSn'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.has['motor']) {
                    this.$message({
                        message: this.$t('cNam.hasMotorSn'),
                        type: 'warning'
                    });
                    return false;
                }
                //  没有输入就不检查
                if (this.pedalSensorInfo.sn && !this.has['sensor']) {
                    this.$message({
                        message: this.$t('cNam.hasSensorSn'),
                        type: 'warning'
                    });
                    return false;
                }
                return true;
            },

            blurVehicleSN() {
                Core.Api.Vehicle.snIsExist(this.vehicleInfo.vehicleSn).then(res => {
                    console.log("[Bike IsExist]", res);
                    this.canUseSn = !res.result;
                    if (!this.canUseSn) {
                        this.$message({
                            message: this.$t('bikeSnExt'),
                            type: "warning"
                        });
                    }
                });
            },

            getIdList() {
                this.vehicleInfo.objectIdList = [];
                if (this.isBindBattery) {
                    this.bomList.forEach((item, index) => {
                        this.vehicleInfo.objectIdList.push(item.id);
                    });
                } else {
                    let noBatList = this.bomList.filter((item, index) => {
                        return item.root_id !== 5;
                    });
                    noBatList.forEach((item, index) => {
                        this.vehicleInfo.objectIdList.push(item.id);
                    });

                }
            },

            goListView() {
                this.$router.push({name: 'assemblerOrderDetail', query: {"orderId": this.orderId}});
            },

            saveSimpleItem(item) {

                let snSplit = [];

                if(item.sn.length < 20 || item.sn.length >40){
                    this.$message({
                        message: 'SN长度范围是 20-40，请检查。 The length of SN should be 20~40, please check.',
                        type: 'warning',
                        duration: 0,
                        showClose: true
                    });
                    return;
                }
                let result = true;
                switch(item.type){
                    case 4:
                        snSplit = item.sn.split('.');
                        if(snSplit.length>=3 && item.sn.substr(0,2) === 'DP'){
                            result = false;
                        }
                        break;
                    case 7:
                        snSplit = item.sn.split('.');
                        if(snSplit.length>=3 && item.sn.substr(0,2) === 'CR'){
                            result = false;
                        }
                        break;
                    case 3:
                        snSplit = item.sn.split('.');
                        if(snSplit.length>=2 && item.sn.substr(1,1) === 'M'){
                            result = false;
                        }
                        break;
                }
                if(result){
                    this.$message({
                        message: 'SN 不符合规范。SN does not meet specifications.',
                        type: 'error',
                        duration: 3000
                    });
                    return;
                }
                Core.Api.Component.saveSimpleComponent(item.type, item.sn).then(res => {
                    this.blurCommonSnFunc(item.type, item.sn, 'save extra sn failed. 保存SN失败');
                    this.extSnList.forEach((i, index) => {
                        if (i.sn === item.sn) {
                            this.extSnList.splice(index, 1)
                        }
                    });
                    if (this.extSnList.length === 0) {
                        this.extDialogVisible = false;
                    }
                })
            },

            //获取版本号是否和item 一致
            compareVersion(sn, version, type) {
                if (sn && version) {

//                    this.$http({url:url, method: 'GET'}).then(function (response) {
//                        console.log(response);
//                       if( response.body.data.flag === "5" ){
//                           this.updateFlag[type] = 'true';
//                           console.log(this.updateFlag[type]);
//                            this.historyDialogVisible=true;
//                       }
                    Core.Api.Object.objectVersionCompare(sn, version).then(res => {
                        if( res.flag === "4" ) {
                            this.updateFlag[type] = 'true';
                        }
                    });
                }
            },

            //获取固件列表by sn
            getFirmwareBySn(sn) {
                this.updatePercentage = 0;
                Core.Api.Firmware.getHistoryBySn(sn).then(res => {
                    this.HWRecord = res.record;
                    this.HWRecord.length ?
                        this.historyDialogVisible = true : this.$message.warning('no firmware');
                })
            },

            updateObject(item) {
                let url = Core.Const.NET.FILE_URL_PREFIX + item.file;
//                console.log(url, 'url');
                let $this = this;
                this.$http.get(url, {responseType: 'blob'})
                    .then((response) => {
                        let fileBody = response.body;
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
                            $this.doUpdate(data);
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
            },

            doUpdate(data) {
//                console.log('start update');
                let vm = this;
                let count = 0;
                let crc = "--"+ Update.checkCrc(data.fileBuffer);

                if(  vm.usbHID ){
                    Update.port.usbSDK.client.events.on('firmware-update-progress',(e)=>{
                        vm.startTip = false;
                        vm.updatePercentage = Math.ceil(e.upload / e.total * 100);
//                        console.log(e);
                        if( e.upload == e.total || vm.percentage == 100){
                            if(count == 0){
                                vm.$message({
                                    message: vm.updateMsgObject.successTip,
                                    type: 'success'
                                });
                            }
                            count++;
                        }
                    });
                    Update.port.usbSDK.client.events.on('firmware-update-completed', (e) => {
                        vm.updatePercentage = 0;
                        let time = Core.Util.time();
                        let filename = data.name;
                        if(vm.updateComponentType === "ctrl"){
                            Core.Api.ObjectRecord.save(vm.controllerInfo.sn, filename + crc, time, vm.recordType);
                        }else if(vm.updateComponentType === "hmi"){
                            Core.Api.ObjectRecord.save(vm.hmiInfo.sn, filename + crc, time, vm.recordType);
                        }else if(vm.updateComponentType === "sensor"){
                            Core.Api.ObjectRecord.save(vm.pedalSensorInfo.sn, filename + crc, time, vm.recordType);
                        }else if(vm.updateComponentType === "bat"){
                            Core.Api.ObjectRecord.save(vm.batteryInfo.sn, filename + crc, time, vm.recordType);
                        }
                    })

                    Update.port.usbSDK.client.events.on('firmware-update-check', (e) => {
                        vm.updatePercentage = 0;
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
                    Update.connect(this.com,57600);
                    Update.port.reset(100);
                    setTimeout( ()=>{
//                        console.log(bus, 'bus');
                        Update.sendPackage(data).subscribe(
                            res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                                if (res.type === 'data' && res.data.update) {
                                    let update = res.data.update;
                                    if (update.percentage) {
                                        this.updatePercentage = Math.ceil(update.percentage * 100);
                                    }
                                    if (update.percentage === 1) {
                                        this.$message({
                                            message: this.$t('cNam.updateSuccessTip'),
                                            type: 'success'
                                        });
                                        this.updatePercentage = 0;
                                        let time = Core.Util.time();
                                        let filename = data.name;
                                        if(this.updateComponentType === "ctrl"){
                                            Core.Api.ObjectRecord.save(this.controllerInfo.sn, filename + crc, time, this.recordType);
                                        }else if(this.updateComponentType === "hmi"){
                                            Core.Api.ObjectRecord.save(this.hmiInfo.sn, filename + crc, time, this.recordType);
                                        }else if(this.updateComponentType === "sensor"){
                                            Core.Api.ObjectRecord.save(this.pedalSensorInfo.sn, filename + crc, time, this.recordType);
                                        }else if(this.updateComponentType === "bat"){
                                            Core.Api.ObjectRecord.save(this.batteryInfo.sn, filename + crc, time, this.recordType);
                                        }
                                    }
                                }
                            }
                        );
                    },2000)
                }
            },
            resetUpdate() {
                this.updatePercentage = 0;
                Update.port.reset(100);
            },

        },

        beforeDestroy:function () {
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalProduce){
                clearInterval(this.connectionIntervalProduce);
            }
            HMI.port.setBoxOperation();
        },

        watch: {
            'componentSn': {
                handler: function (newValue, oldValue) {
                    let vm = this;
                    if (newValue === oldValue || newValue === '') {
                        return;
                    }
                    if (typeof oldValue !== 'undefined' && oldValue.length > 15) {
                        let time = new Date().getTime();
                        vm.isScanner = time - vm.lastInputTime <= 50; // 间隔小于50ms的就认为是扫描枪输入
                        vm.lastInputTime = time;
                    } else {
                        vm.lastInputTime = new Date().getTime();
                    }
                    if (!vm.scannerTimeout) {
                        vm.scannerTimeout = setTimeout(function () {
                            if (vm.isScanner) {
                                let value = vm.componentSn.replace(/\s+/g, ""); // 去空格
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                                if (value.indexOf('DP') > -1) {
                                    // 仪表要去掉DP之前的字符
                                    let array = value.split('DP');
                                    if (array.length > 1) {
                                        value = 'DP' + array[array.length - 1];
                                    }
                                    vm.hmiInfo.sn = value;
                                    vm.componentSn = '';
                                } else if (value.indexOf('CR') > -1) {
                                    vm.controllerInfo.sn = value;
                                    vm.componentSn = '';
                                } else if (value.indexOf('BT') > -1) {
                                    vm.batteryInfo.sn = value;
                                    vm.componentSn = '';
                                } else if (value.indexOf('FM') > -1 || value.indexOf('MM') > -1 || value.indexOf('RM') > -1) {
                                    vm.motorInfo.sn = value;
                                    vm.componentSn = '';
                                } else if (value.indexOf('SR') > -1) {
                                    let array = value.split('SR');
                                    if (array.length > 1) {
                                        value = 'SR' + array[array.length - 1];
                                    }
                                    vm.pedalSensorInfo.sn = value;
                                    vm.componentSn = '';
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 800)
                    }
                }
            },

            'vehicleInfo.vehicleSn': {
                handler: function (newValue, oldValue) {
                    this.vehicleInfo.vehicleSn = newValue.replace(/\s+/g, "");

                },
                deep: true
            },

            'vehicleInfo.key': {
                handler: function (newValue, oldValue) {
                    this.vehicleInfo.key = newValue.replace(/\s+/g, "");
                },
                deep: true
            },

            'hmiInfo.sn': {
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
                                let value = vm.hmiInfo.sn.replace(/\s+/g, "");
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
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
                                let value = vm.pedalSensorInfo.sn.replace(/\s+/g, "");
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                                if (value.indexOf('SR') > -1) {
                                    let array = value.split('SR');
                                    if (array.length > 1) {
                                        value = 'SR' + array[array.length - 1];
                                    }
                                    vm.pedalSensorInfo.sn = value;
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 1000)
                    }
                },
                deep: true
            },
            'rpmSensorInfo.sn': {
                handler: function (newValue, oldValue) {
                    this.rpmSensorInfo.sn = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            isConnected: function(newValue, oldValue) {
                let vm = this;
                if(newValue != oldValue && oldValue == true){
                    let count = 0;
                    HMI.port.setConnection();
                    HMI.port.setBoxOperation();
                    if(false){
                        let reconnectInterval = setInterval(()=>{
                            count++;
                            if(count>1 && Controller.port.isConnected()){
//                                console.log("Count",count);
                                vm.connect();
                            }
                            if(count>5 && Controller.port.isConnected()){
//                                console.log("Count stop",count);
                                clearInterval(reconnectInterval);
                            }
                            if(count>100){
                                clearInterval(reconnectInterval);
                            }
                        },200);
                    }else{
                        if(vm.reconnectInterval){
                            clearInterval(vm.reconnectInterval);
                            vm.reconnectInterval = '';
                        }
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
//                            console.log("Count",count);
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
                                if(count>3 && HMI.port.isConnected()){
//                                    console.log("Count stop",count);
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
    .assembler-produce {
        font-size: 16px;
        .header-label {
            margin: 0 6px 0 30px;
        }
        .content-wrap {
            padding: 0;
            .content {
                padding-left: 0;
                .el-collapse-item__header {
                    font-size: 0;
                }
                .btn-group {
                    text-align: right;
                    /*padding-bottom: 10px;*/
                }
                .info {
                    font-size: 0;
                    .search-item {
                        display: inline-block;
                        width: 50%;
                        font-size: 0;
                        padding: 15px 0;
                        .tip-img {
                            width: 18px;
                            position: relative;
                            top: 4px;
                            margin-left: 14px;
                        }

                        .label {
                            display: inline-block;
                            width: 150px;
                            padding-left: 30px;
                        }
                        .scan-input {
                            width: 240px;
                            .el-input__inner {
                                padding-right: 40px;
                                text-align: right;
                            }
                        }
                        .search-btn {
                            border-radius: 100px;
                            position: relative;
                            margin-left: 15px;
                        }
                    }
                    .title-select {
                        position: absolute;
                        right: 120px;
                        top: 16px;
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
                        padding-left: 10px;
                        width: 100px;
                        line-height: 20px;
                        vertical-align: middle;
                        word-wrap: break-word;
                    }
                    .left-20{
                        margin-left: 20px;
                    }
                    .key {
                        width: 120px;
                    }
                    .title-solution {
                        display: inline-block;
                        margin-left: 5px;
                    }
                    .title-sn {
                        display: inline-block;
                        width: 260px;
                    }
                    .title-flag {
                        display: inline-block;
                        /*width: 60px;*/
                    }
                    .title-progress {
                        display: inline-block;
                        width: 120px;
                        margin-left: 50px;
                    }
                    > .title {
                        position: relative;
                        padding-left: 50px;
                        li {
                            display: inline-block;
                            font-size: 14px;
                            line-height: 60px;
                            &:nth-child(1) {
                                width: 100px;
                            }
                            &:nth-child(2) {
                                width: 250px;
                            }
                            &:nth-child(3) {
                                width: 296px;
                            }
                            &:nth-child(4) {
                                width: 160px;
                            }
                        }
                        .service {
                            position: absolute;
                            right: 0;
                            top: 50%;
                            margin-top: -13px;
                        }
                    }
                    .info-list-box {
                        border-top: 1px solid #22252E;
                        border-bottom: 1px solid #22252E;
                        padding: 0 30px;
                        .header-title {
                            font-size: 14px;
                            color: #858890;
                            margin-bottom: 6px;
                        }
                        .info-list {
                            font-size: 0;
                            .col-left {
                                display: inline-block;
                                width: 50%;
                                text-align: left;
                            }
                            .col-right {
                                display: inline-block;
                                width: 50%;
                                text-align: right;
                            }

                            .list-group {
                                text-align: left;
                                margin-top: 20px;

                                .list-group-item {
                                    height: 60px;
                                    font-size: 0;
                                    display: flex;
                                    align-items: center;
                                    line-height: 20px;
                                    border-top: 1px solid rgba(255, 255, 255, 0.1);
                                    border-bottom: none;

                                    span {
                                        display: inline-block;
                                        font-size: 14px;
                                    }
                                    span:first-child {
                                        color: #FFFFFF;
                                        width: 157px;
                                        vertical-align: middle;
                                        word-wrap:break-word;
                                    }
                                    span:last-child {
                                        padding-left: 5px;
                                        color: #A1A5B1;
                                        flex: 1;
                                    }
                                }

                                .clear-border-top {
                                    border-top: none;
                                }
                                &.list-all {
                                    span:first-child {
                                        width: 157px;
                                    }
                                }
                                .height {
                                    display:inline-block;
                                    vertical-align: middle;
                                    height: 40px;
                                }

                            }
                        }

                    }
                    .input-sn {
                        width: 240px;
                        height: 30px;
                        margin-right: 10px;
                        .el-input__inner {
                            padding-right: 40px;
                            text-align: right;
                        }
                    }
                }

            }
        }

        .history-table {
            width: 100%;
            /*min-width:700px;*/
            border-top: 1px solid rgba(255, 255, 255, 0.1);
            tr {
                line-height: 30px;
            }
            .head {
                color: white;
            }
            .history-td {
                padding: 0 6px;
                width: 160px;
                border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                font-size: 14px;
                text-align: center;
            }
        }
        .progress-wrap{
            display:block;
            height:30px;
            margin-right:-30px;
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
        .input {
            margin-left: 20px;
            width: 260px;
        }
        .connect-tip {
            display: inline-block;
            margin-left: 20px;
            a {
                color: #E26829;
            }
        }
        .tip-img {
            width: 18px;
            position: relative;
            top: 4px;
        }
        .select-holder {
            margin: 20px 10px;
            display: inline-block;
            .label {
                display: inline-block;
                width: 100px;
                color: white;
                margin-left: 40px;
            }
        }

        .bind-battery {
            display: inline-block;
            position: absolute;
            width: 50%;
            right: 0;
            text-align: right;
            padding-right: 90px;
            .switch-label {
                margin-right: 24px;
            }
        }
        .order {
            border-left: 1px solid #22252E;
            border-right: 1px solid #22252E;
            .info-list-box {
                border-top: none !important;
                padding: 0 30px;
            }
        }
        .btn-connect {
            margin-top: 17px;
        }
        .tip {
            display: inline-block;
            border-radius: 50%;
            width: 10px;
            height: 10px;
            background: #E26829;
            margin-left: 6px;
        }
        .connect {
            background: #76EE00;
        }
        .test {
            overflow: hidden;
            text-align: center;
            margin-top: -38px;
            .test-btn {
                /*margin-bottom: 10px;*/
                display: inline-block;
                font-size: 24px;
                height: 50px;
                width: 250px;
                border-radius: 25px;
                background-color: #A1A5B1;
                cursor: pointer;
                line-height: 50px;
                font-weight: bold;
                /*&:hover {*/
                /*background-color: #E26829;*/
                /*}*/
            }
            .test-btn.active {
                background-color: #E26829;
            }
        }
        .tip-dialog {
            .el-dialog__body {
                padding: 0;
            }
        }
        .history-dialog{
            .el-dialog__body {
                padding-bottom: 10px;
            }
        }
        .el-dialog--small {
            width: 770px;
        }
        .mr-tpo-0 {
            margin-top: 0 !important;
        }
        .no-border-top {
            border-top: none !important;
        }

        .width-80 {
            display: inline-block;
            width: 80px;
        }

        .width-460 {
            display: inline-block;
            width: 460px;
        }
    }
</style>




// WEBPACK FOOTER //
// produce.vue?78224b16