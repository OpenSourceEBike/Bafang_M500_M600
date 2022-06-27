<template>
    <div class="report-container">
        <div class="header" >
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goBack" :title="$t('back')"></div>
                </div>
            </div>
            <span class="title">{{ $t('diagnosisReportTitle') }}</span>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item print" @click="onClickPrint" :title="$t('print')"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                <el-collapse v-model="activeNames">
                    <el-collapse-item :title=titleText name="1">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('name') }}</div>
                                <div class="content">{{ orgInfo.name || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('address') }}</div>
                                <div class="content">{{ orgInfo.address + orgInfo.country || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('email') }}</div>
                                <div class="content">{{ orgInfo.email || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('tel') }}</div>
                                <div class="content">{{ orgInfo.phone || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('reportDate') }}</div>
                                <div class="content">{{ reportInfo.create_time | timeFormat }}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('bike.title')" name="2">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">Bike ID</div>
                                <div class="content">{{ reportInfo.vehicle_sn || '-' }}</div>
                            </li>
                            <li class="list-group-item" v-for="item in componentList">
                                <div class="label-title">{{item.category_root_id | categoryRoot}} SN</div>
                                <div class="content">{{ item.sn || '-' }}</div>
                            </li>
                            <!--<li class="list-group-item">-->
                            <!--<div class="label-title">{{item.category_root_id | categoryRoot}} SN</div>-->
                            <!--<div class="content">{{ reportInfo.device.controller.sn || '-' }}</div>-->
                            <!--</li>-->
                            <!--<li class="list-group-item">-->
                            <!--<div class="label-title">Battery sn</div>-->
                            <!--<div class="content">{{ reportInfo.device.battery.sn || '-' }}</div>-->
                            <!--</li>-->
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.data')" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-content">
                                    <!--<el-tooltip placement="right">-->
                                    <div slot="content">
                                        <div class="item-group">
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.hmi') }} SN</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.sn : "-") : "-"}}
                                                </div>
                                                <div class="inline-block">
                                                    <span class="color-red" v-if="device && device.hmi && device.hmi.tip">({{device.hmi.tip}})</span>
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.model') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.model : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.softwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.version : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.hardwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.hardVersion : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.totalMileage') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.totalMileage : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.wheelDiameter') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.wheelDiameter : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.speedLimit') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.hmi ? device.hmi.speedLimit : "-") : "-"}}
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <!--<span>HMI</span>-->
                                    <!--</el-tooltip>-->
                                </div>
                                <!--<div class="content">-->
                                    <!--<span class="color-red" v-if="device && device.hmi && device.hmi.tip" >({{device.hmi.tip}})</span>-->
                                <!--</div>-->
                            </li>
                            <li class="list-group-item">
                                <div class="label-content">
                                    <!--<el-tooltip placement="right">-->
                                    <div slot="content">
                                        <div class="item-group">
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.controller') }} SN</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.sn : "-") : "-"}}
                                                </div>
                                                <div class="inline-block">
                                                    <span class="color-red" v-if="device && device.controller && device.controller.tip" >({{device.controller.tip}})</span>
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.model') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.model : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.softwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.version : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.hardwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.hardVersion :  "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.nmlVolt') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.nmlVolt : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.maxCur') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.maxCurrent : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.speedLimit') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.speedLimit : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.wheelDiameter') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.wheelDiameter : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item" v-if="!isCan">
                                                <div class="w-100 inline-block">{{ $t('cNam.circumference') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.controller ? device.controller.circumference : "-") : "-"}}
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <!--<span>Controller</span>-->
                                    <!--</el-tooltip>-->
                                </div>
                                <!--<div class="content">{{device ? (device.controller ? device.controller.sn : "-") : "-"}}-->
                                <!--</div>-->
                            </li>
                            <li class="list-group-item">
                                <div class="label-content">
                                    <!--<el-tooltip placement="right">-->
                                        <div slot="content">
                                            <div class="item-group">
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.battery') }} SN</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.sn: "-") : "-"}}
                                                    </div>
                                                    <div class="inline-block">
                                                        <span class="color-red" v-if="device && device.battery && device.battery.tip" >({{device.battery.tip}})</span>
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.model') }}</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.model : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.softwareVersion') }}</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.version : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.hardwareVersion') }}</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.hardVersion : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.ASOC') }}(%)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.soh : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.RSOC') }}(%)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.soc : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.packVoltage') }}(mV)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.packVoltage : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.capacityLeft') }}(mAh)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.leftCapacity : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.fullChargeCapacity') }}(mAh)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.fullChargerCapacity : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.temperature') }}(℃)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.temperature : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.current') }}(mA)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.current : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.cell1_7') }}(mV)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.cell1to7 : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.cell8_14') }}(mV)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.cell8to14 : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.cycleLife') }}</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.loopTime : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.currentChargeInterval') }}(day)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.lastCharge : "-") : "-"}}
                                                    </div>
                                                </div>
                                                <div class="item">
                                                    <div class="w-100 inline-block">{{ $t('cNam.maxChargeInterval') }}(day)</div>
                                                    <div class="inline-block">
                                                        {{device ? (device.battery ? device.battery.maxChargeInterval : "-") : "-"}}
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                        <!--<span>Battery</span>-->
                                    <!--</el-tooltip>-->
                                </div>
                                <!--<div class="content">{{device ? (device.battery ? device.battery.sn: "-") : "-"}}-->
                                    <!--<span class="color-red" v-if="device && device.battery && device.battery.tip" >({{device.battery.tip}})</span>-->
                                <!--</div>-->
                            </li>
                            <li class="list-group-item" v-if="isCan" >
                                <div class="label-content">
                                    <!--<el-tooltip placement="right">-->
                                    <div slot="content">
                                        <div class="item-group">
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.sensor') }} SN</div>
                                                <div class="inline-block">
                                                    {{device ? (device.pedalSensor ? device.pedalSensor.sn: "-") : "-"}}
                                                </div>
                                                <div class="inline-block">
                                                    <span class="color-red" v-if="device && device.pedalSensor && device.pedalSensor.tip" >({{device.pedalSensor.tip}})</span>
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.model') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.pedalSensor ? device.pedalSensor.name : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.softwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.pedalSensor ? device.pedalSensor.version : "-") : "-"}}
                                                </div>
                                            </div>
                                            <div class="item">
                                                <div class="w-100 inline-block">{{ $t('cNam.hardwareVersion') }}</div>
                                                <div class="inline-block">
                                                    {{device ? (device.pedalSensor ? device.pedalSensor.hardVersion : "-") : "-"}}
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <!--<span>Battery</span>-->
                                    <!--</el-tooltip>-->
                                </div>
                                <!--<div class="content">{{device ? (device.battery ? device.battery.sn: "-") : "-"}}-->
                                <!--<span class="color-red" v-if="device && device.battery && device.battery.tip" >({{device.battery.tip}})</span>-->
                                <!--</div>-->
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.result')" name="4">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hmi') }}</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.hmi }}</span>
                                    <span class="split" v-if="device.hmi.error">|</span>
                                    <span class="error">{{ device.hmi.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.controller') }}</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.controller }}</span>
                                    <span class="split" v-if="device.controller.error">|</span>
                                    <span class="error">{{ device.controller.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.motor') }}</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.motor}}</span>
                                    <span class="split" v-if="device.motor.error">|</span>
                                    <span class="error">{{ device.motor.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.sensor') }}</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.pedalSensor}}</span>
                                    <span class="split" v-if="device.pedalSensor.error">|</span>
                                    <span class="error">{{ device.pedalSensor.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.battery') }}</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.battery }}</span>
                                    <span class="split" v-if="device.battery.error">|</span>
                                    <span class="error">{{ device.battery.error }}</span>
                                </div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('cNam.remark')" name="5">
                        <div class="remark-row">
                            <el-row class="error-row" v-for="code in error" v-bind:key="code.name">
                                <p class="error-text"><span class="label-title"> Error Code: </span> {{code.name}}</p>
                                <p class="error-text"> Description: {{code.desc}} </p>
                                <p class="error-text"> Target: {{code.target ? code.target.join(',') : ''}} </p>
                                <div class="error-solution">
                                    <p> <span class="label-title"> Solution: </span></p>
                                    <p class="error-text pl-30" v-for="(item, index) in code.solution">{{item}}</p>
                                </div>
                            </el-row>
                        </div>
                        <div class="remark-row">
                            <span>{{ $t('cNam.defect') }}</span>
                            <div class="defect-list">
                                <span v-for="error in selectErrorList" class="error-span">{{error}}</span>
                            </div>
                        </div>
                        <div class="clear-10"></div>
                        <div class="remark-row">
                            <span>{{ $t('cNam.description') }}</span>
                            <p>{{remark}}</p>
                        </div>
                        <div class="clear-10"></div>
                        <div v-if="fileString">
                            <div class="remark-row">
                                <div>
                                    <span>{{ $t('cNam.image') }}</span>
                                </div>
                                <div v-for="(item, index) in picList" class="img-holder">
                                    <img :src="picPre + item" class="img-content" @click="handlePreview(picPre + item)"/>
                                    <div class="close-btn" @click="removeImg(index)"></div>
                                </div>
                            </div>
                        </div>
                        <div v-else>
                            <div class="remark-row">
                                <div class="title-wrap">
                                    <span>{{ $t('cNam.image') }}</span>
                                </div>
                                <div v-for="(item, index) in fileList.img" class="img-holder">
                                    <img :src="linkPre + item" class="img-content"/>
                                </div>
                            </div>
                            <div class="clear-10"></div>
                            <div class="remark-row">
                                <div class="title-wrap">
                                    <span>{{ $t('cNam.video') }}</span>
                                </div>
                                <div v-for="(item, index) in fileList.video" class="video-holder">
                                    <video :src="linkPre + item" controls="controls" class="videoItem"></video>
                                </div>
                            </div>
                        </div>

                        <!--<div class="pull-right remark-row">-->
                            <!--<el-button type="primary" @click="saveReport">Save</el-button>-->
                        <!--</div>-->
                    </el-collapse-item>
                </el-collapse>
                <div class="clear-20"></div>
                <div v-if="from === 'diagnosis3rdResult' ">
                    <!--<div v-if="true ">-->
                    <el-button @click="goBackDiagnosis">Diagnosis Bike</el-button>
                </div>
            </div>

        </div>

        <div class="print-content-wrap">
            <div class="content">
                <h4>E-BIKE DIAGNOSIS REPORT {{ reportInfo.vehicle_sn || ' ' }}</h4>
                <el-collapse v-model="printActiveNames">
                    <el-collapse-item :title=titleText name="1">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('name') }}</div>
                                <div class="content">{{ orgInfo.name || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('address') }}</div>
                                <div class="content">{{ orgInfo.address + orgInfo.country || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('email') }}</div>
                                <div class="content">{{ orgInfo.email || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{labelText}}{{ $t('tel') }}</div>
                                <div class="content">{{ orgInfo.phone || '-' }}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('reportDate') }}</div>
                                <div class="content">{{ reportInfo.create_time | timeFormat }}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('bike.title')" name="2">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">Bike ID</div>
                                <div class="content">{{ reportInfo.vehicle_sn || '-' }}</div>
                            </li>
                            <li class="list-group-item" v-for="item in componentList">
                                <div class="label-title">{{item.category_root_id | categoryRoot}} SN</div>
                                <div class="content">{{ item.sn || '-' }}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.hmi')" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">
                                    <div class="sn-main">
                                        <span>{{device ? (device.hmi ? device.hmi.sn : "-") : "-"}} </span>
                                    </div>
                                    <div class="sn-main">
                                        <span v-if="device && device.hmi && device.hmi.tip">({{device.hmi.tip}})</span>
                                    </div>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{device ? device.hmi.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{device ? device.hmi.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{device ? device.hmi.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.totalMileage') }}</div>
                                <div class="content">{{device ? device.hmi.totalMileage : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-if="!isCan">
                                <div class="label-title">{{ $t('cNam.wheelDiameter') }}</div>
                                <div class="content">{{device ? device.hmi.wheelDiameter : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-if="!isCan">
                                <div class="label-title">{{ $t('cNam.speedLimit') }}</div>
                                <div class="content">{{device ? device.hmi.speedLimit : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.controller')" name="3">
                        <ul class="list-group">
                            <li class="list-group-item ">
                                <div class="label-title">SN</div>
                                <div class="content">
                                    <div class="sn-main">
                                        <span>{{device ? (device.controller ? device.controller.sn : "-") : "-"}} </span>
                                    </div>
                                    <div class="sn-main">
                                        <span v-if="device && device.controller && device.controller.tip" >({{device.controller.tip}}) 1</span>
                                    </div>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{device ? device.controller.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{device ? device.controller.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{device ? device.controller.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-if="!isCan">
                                <div class="label-title">{{ $t('cNam.nmlVolt') }}</div>
                                <div class="content">{{device ? device.controller.nmlVolt : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-if="!isCan">
                                <div class="label-title">{{ $t('cNam.maxCur') }}</div>
                                <div class="content">{{device ? device.controller.maxCurrent : "-"}}</div>
                            </li>
                            <div class="list-group-item" v-if="isCan">
                                <div class="label-title">{{ $t('cNam.speedLimit') }}</div>
                                <div class="content">{{device ? device.controller.speedLimit : "-"}}</div>
                            </div>
                            <div class="list-group-item" v-if="isCan">
                                <div class="label-title">{{ $t('cNam.wheelDiameter') }}</div>
                                <div class="content">{{device ? device.controller.wheelDiameter : "-"}}</div>
                            </div>
                            <div class="list-group-item" v-if="isCan">
                                <div class="label-title">{{ $t('cNam.circumference') }}</div>
                                <div class="content">{{device ? device.controller.circumference : "-"}}</div>
                            </div>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.battery')" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">
                                    <div class="sn-main">
                                        <span>{{device ? (device.battery ? device.battery.sn : "-") : "-"}}</span>
                                    </div>
                                    <div class="sn-main">
                                        <span v-if="device && device.battery && device.battery.tip" >({{device.battery.tip}})</span>
                                    </div>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{device ? device.battery.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{device ? device.battery.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{device ? device.battery.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.ASOC') }}(%)</div>
                                <div class="content">{{device ? device.battery.soh : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.RSOC') }}(%)</div>
                                <div class="content">{{device ? device.battery.soc : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.packVoltage') }}(mV)</div>
                                <div class="content">{{device ? device.battery.packVoltage : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.capacityLeft') }}(mAh)</div>
                                <div class="content">{{device ? device.battery.leftCapacity : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.fullChargeCapacity') }}(mAh)</div>
                                <div class="content">{{device ? device.battery.fullChargerCapacity : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.temperature') }}(℃)</div>
                                <div class="content">{{device ? device.battery.temperature : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.current') }}(mA)</div>
                                <div class="content">{{device ? device.battery.current : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cell1_7') }}(mV)</div>
                                <div class="content">{{device ? device.battery.cell1to7 : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cell8_14') }}(mV)</div>
                                <div class="content">{{device ? device.battery.cell8to14 : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cycleLife') }}</div>
                                <div class="content">{{device ? device.battery.loopTime : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.currentChargeInterval') }}(day)</div>
                                <div class="content">{{device ? device.battery.lastCharge : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.maxChargeInterval') }}(day)</div>
                                <div class="content">{{device ? device.battery.maxChargeInterval : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.sensor')" name="3" v-if="isCan">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">
                                    <div class="sn-main">
                                        <span>{{device ? (device.pedalSensor ? device.pedalSensor.sn : "-") : "-"}}</span>
                                    </div>
                                    <div class="sn-main">
                                        <span v-if="device && device.pedalSensor && device.pedalSensor.tip" >({{device.pedalSensor.tip}})</span>
                                    </div>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{device ? device.pedalSensor.name : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{device ? device.pedalSensor.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{device ? device.pedalSensor.hardVersion : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.result')" name="4">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">HMI</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.hmiOK ? 'OK' : 'Fail' }}</span>
                                    <span class="split" v-if="device.hmi.error">|</span>
                                    <span class="error">{{ device.hmi.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">Controller</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.controllerOK ? 'OK' : 'Fail' }}</span>
                                    <span class="split" v-if="device.controller.error">|</span>
                                    <span class="error">{{ device.controller.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">Motor</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.motorOK ? 'OK' : 'Fail' }}</span>
                                    <span class="split" v-if="device.motor.error">|</span>
                                    <span class="error">{{ device.motor.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">PedalSensor</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.pedalSensorOK ? 'OK' : 'Fail' }}</span>
                                    <span class="split" v-if="device.pedalSensor.error">|</span>
                                    <span class="error">{{ device.pedalSensor.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">Battery</div>
                                <div class="content">
                                    <span>{{ reportInfo.processData.result.batteryOK ? 'OK' : 'Fail' }}</span>
                                    <span class="split" v-if="device.battery.error">|</span>
                                    <span class="error">{{ device.battery.error }}</span>
                                </div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item :title="$t('diagnosis.result')" name="5">
                        <el-row class="error-row" v-for="code in error" v-bind:key="code.name">
                            <p class="error-text"><span class="label-title"> Error Code: </span> {{code.name}}</p>
                            <p class="error-text"> Description: {{code.desc}} </p>
                            <p class="error-text"> Target: {{code.target ? code.target.join(',') : ''}} </p>
                            <div class="error-solution">
                                <p> <span class="label-title"> Solution: </span></p>
                                <p class="error-text pl-30" v-for="(item, index) in code.solution">{{item}}</p>
                            </div>
                        </el-row>
                        <el-row class="error-row">
                            <p class="error-text"><span class="label-title">Defects: </span></p>
                            <p class="error-text pl-30" v-for="error in selectErrorList" >{{error}}</p>
                        </el-row>
                        <el-row class="error-row">
                            <p class="error-text"><span class="label-title"> Remark: </span></p>
                            <p class="error-text pl-30">{{remark}}</p>
                        </el-row>
                    </el-collapse-item>
                </el-collapse>
            </div>
        </div>

        <!--预览图-->
        <el-dialog v-model="preDialogVisible"  :title="preDialogText" custom-class="pre-dialog" top="20vh">
            <div style="text-align: center; background: #FFFFFF">
                <img class="pre-img" :src="dialogImageUrl" alt="">
            </div>
        </el-dialog>
    </div>

</template>

<script type="text/ecmascript-6">
    import Core from "core/core";
    import printArea from 'printArea';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        data() {
            return {
                activeNames: ['1'],
                printActiveNames: ['1', '2', '3', '4', '5'],
                reportInfo: {
                    processData: {
                        device: {
                            vehicle: {
                                sn: ''
                            }
                        },
                        result: {}
                    }
                },
                orgInfo: {},

                componentList: [],
                labelText: 'Org',
                titleText: 'Org Info',

                device: {
                    hmi: {sn: ''},
                    controller: {sn: ''},
                    battery: {sn: ''},
                    motor: {sn: ''},
                    pedalSensor: {sn: ''},
                    rpmSensor: {}
                },

                dialogErrorSelectVisible: false,
                errorList: [],
                selectErrorList: [],
                defectOptionList: Core.Const.OptionList,

                remark: '',

                //图片上传
                upload: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    photographList: [],
                    data: {
                        token: Core.Data.getToken()
                    }
                },

                preDialogVisible: false,
                dialogImageUrl: '',
                preDialogText: '',

                errorCode: [],
                error: [],
                reportId: 0,

                picPre: Core.Const.NET.IMG_URL_PREFIX,
                linkPre: Core.Const.NET.FILE_URL_PREFIX,
                picList: [],

                from: '',
                isCan: false,
                fileString: false,
                fileList: {
                    video: [],
                    img: [],
                },
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Diagnosis.report,
                zh: ZH.Components.Diagnosis.report,
                de: DE.Components.Diagnosis.report,
                nl: NL.Components.Diagnosis.report
            }
        },
        mounted: function () {
            // 国际化
            // this.orgInfo = Core.Data.getOrg();
            this.initData();
            this.reportId = this.$route.query.id;
            this.from = this.$route.query.from;

            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });

            this.initDefectOptionList();

            this.getDetail();

        },
        methods: {
            initData() {
                this.labelText = this.$t('orgLabel');
                this.titleText = this.$t('orgTitle');

                if (this.orgInfo.type === 2) {
                    this.labelText = this.$t('dealerLabel');
                    this.titleText = this.$t('dealerTitle');
                }
                if (this.orgInfo.type === 3) {
                    this.labelText = this.$t('assemblerLabel');
                    this.titleText = this.$t('assemblerTitle');
                }
            },
            initDefectOptionList() {

                this.defectOptionList[0]['data'][0].label = this.$t("cNam.bikeLabel_1");
                this.defectOptionList[0]['data'][1].label = this.$t("cNam.bikeLabel_2");
                this.defectOptionList[0]['data'][2].label = this.$t("cNam.bikeLabel_3");
                this.defectOptionList[0]['data'][3].label = this.$t("cNam.bikeLabel_4");
                this.defectOptionList[0]['data'][4].label = this.$t("cNam.bikeLabel_5");
                this.defectOptionList[0]['data'][5].label = this.$t("cNam.bikeLabel_6");
                this.defectOptionList[0]['data'][6].label = this.$t("cNam.bikeLabel_7");
                this.defectOptionList[0]['data'][7].label = this.$t("cNam.bikeLabel_8");
                this.defectOptionList[0]['data'][8].label = this.$t("cNam.bikeLabel_9");
                this.defectOptionList[0]['data'][9].label = this.$t("cNam.bikeLabel_10");
                this.defectOptionList[0]['data'][10].label = this.$t("cNam.bikeLabel_11");
                this.defectOptionList[0]['data'][11].label = this.$t("cNam.bikeLabel_12");
                this.defectOptionList[1]['data'][0].label = this.$t("cNam.motorLabel_1");
                this.defectOptionList[1]['data'][1].label = this.$t("cNam.motorLabel_2");
                this.defectOptionList[1]['data'][2].label = this.$t("cNam.motorLabel_3");
                this.defectOptionList[1]['data'][3].label = this.$t("cNam.motorLabel_4");
                this.defectOptionList[1]['data'][4].label = this.$t("cNam.motorLabel_5");
                this.defectOptionList[1]['data'][5].label = this.$t("cNam.motorLabel_6");
                this.defectOptionList[1]['data'][6].label = this.$t("cNam.motorLabel_7");
                this.defectOptionList[1]['data'][7].label = this.$t("cNam.motorLabel_8");
                this.defectOptionList[1]['data'][8].label = this.$t("cNam.motorLabel_9");
                this.defectOptionList[1]['data'][9].label = this.$t("cNam.motorLabel_10");
                this.defectOptionList[2]['data'][0].label = this.$t("cNam.hmiLabel_1");
                this.defectOptionList[2]['data'][1].label = this.$t("cNam.hmiLabel_2");
                this.defectOptionList[2]['data'][2].label = this.$t("cNam.hmiLabel_3");
                this.defectOptionList[2]['data'][3].label = this.$t("cNam.hmiLabel_4");
                this.defectOptionList[2]['data'][4].label = this.$t("cNam.hmiLabel_5");
                this.defectOptionList[2]['data'][5].label = this.$t("cNam.hmiLabel_6");
                this.defectOptionList[2]['data'][6].label = this.$t("cNam.hmiLabel_7");
                this.defectOptionList[2]['data'][7].label = this.$t("cNam.hmiLabel_8");
                this.defectOptionList[2]['data'][8].label = this.$t("cNam.hmiLabel_9");
                this.defectOptionList[3]['data'][0].label = this.$t("cNam.ctlLabel_1");
                this.defectOptionList[3]['data'][1].label = this.$t("cNam.ctlLabel_2");
                this.defectOptionList[3]['data'][2].label = this.$t("cNam.ctlLabel_3");
                this.defectOptionList[3]['data'][3].label = this.$t("cNam.ctlLabel_4");
                this.defectOptionList[4]['data'][0].label = this.$t("cNam.batLabel_1");
                this.defectOptionList[4]['data'][1].label = this.$t("cNam.batLabel_2");
                this.defectOptionList[4]['data'][2].label = this.$t("cNam.batLabel_3");
                this.defectOptionList[4]['data'][3].label = this.$t("cNam.batLabel_4");
                this.defectOptionList[4]['data'][4].label = this.$t("cNam.batLabel_5");
                this.defectOptionList[4]['data'][5].label = this.$t("cNam.batLabel_6");
                this.defectOptionList[4]['data'][6].label = this.$t("cNam.batLabel_7");
                this.defectOptionList[4]['data'][7].label = this.$t("cNam.batLabel_8");
                this.defectOptionList[4]['data'][8].label = this.$t("cNam.batLabel_9");
                this.defectOptionList[4]['data'][9].label = this.$t("cNam.batLabel_10");
                this.defectOptionList[4]['data'][10].label = this.$t("cNam.batLabel_11");
                this.defectOptionList[5]['data'][0].label = this.$t("cNam.sensor_1");
                this.defectOptionList[5]['data'][1].label = this.$t("cNam.sensor_2");
                this.defectOptionList[5]['data'][2].label = this.$t("cNam.sensor_3");
                this.defectOptionList[5]['data'][3].label = this.$t("cNam.sensor_4");
                this.defectOptionList[5]['data'][4].label = this.$t("cNam.sensor_5");
                this.defectOptionList[5]['data'][5].label = this.$t("cNam.sensor_6");
                this.defectOptionList[5]['data'][6].label = this.$t("cNam.sensor_7");
            },
            getDetail(){
                Core.Api.Vehicle.getVehicleReportDetail(this.reportId).then(res => {
                    this.reportInfo = res.report;
                    this.reportInfo.processData = JSON.parse(this.reportInfo.report_data);
                    console.log(JSON.parse(this.reportInfo.report_data));
                    if( typeof this.reportInfo.processData.test == 'number') {
                        this.errorCode.push(typeof this.reportInfo.processData.test);
                    }else{
                        this.errorCode = this.reportInfo.processData.test.error === '' ? [] : this.reportInfo.processData.test.error.split(',');  //  ["9", "30", "10"]
                    }
                    console.log("errorCode",this.errorCode);
                    if (this.errorCode.length > 0 ){
                        for(let i=0; i< this.errorCode.length; i++ ){
                            if(parseInt(this.errorCode[i]) > 1){
                                this.error.push( Core.Const.TEST_ERROR_CODE[parseInt(this.errorCode[i])] );
                            }
                        }
                    }

                    this.remark = res.report.comment || '-';

                    if (res.report.defect) {
                        if(res.report.defect.indexOf('other') > -1 && res.report.defect.indexOf('charger') > -1 && res.report.defect.indexOf('sensor') > -1 ){
                            let receiveDefect = JSON.parse(res.report.defect);
                            let keys = Object.keys(receiveDefect);
                            for(let i=0; i<keys.length; i++){
                                for(let j=0; j<receiveDefect[keys[i]].length; j++){
                                    this.defectOptionList.forEach((item)=>{
                                        if(item.type == keys[i]){
                                            item.data.forEach((data)=>{
                                                    if(data.name == receiveDefect[keys[i]][j] ){
                                                        this.selectErrorList.push( keys[i] + ":" + data.label );
                                                    }
                                                }
                                            )
                                        }
                                    });
                                }
                            }
                        }else{
                            this.selectErrorList = res.report.defect.split(',')
                        }
                    }
//                this.selectErrorList = ["bikeList:无助力功能", "bikeList:不能停止电机运转", "bikeList:无法开机", "bikeList:转把可电机运行，助力功能不正常", "motorList:主轴松动", "motorList:外壳损坏"]

                    if (res.report.file) {
                        if(res.report.file.indexOf('video')>-1){
                            this.fileList = JSON.parse(res.report.file);
                            this.fileString = false;
                        }else{
                            this.fileString = true;
                            this.picList = res.report.file.split(',');
                        }
                    }
                    if (this.reportInfo.processData.hasOwnProperty('can')) {
                        this.isCan = this.reportInfo.processData.can
                    }

                    console.log( this.isCan , "isCan");

//                this.fileList = {
//                    video:['6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4','6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4'],
//                    img:['dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7.png','dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7.png']
//                }
                    this.device = this.reportInfo.processData.device;
                    this.orgInfo = res.report.org;
                    if (this.reportInfo.processData.bomList && this.reportInfo.processData.bomList.length > 0) {
                        this.componentList = this.reportInfo.processData.bomList
                    } else {
                        if (this.reportInfo.processData.device) {
                            let sn = this.reportInfo.processData.device.vehicle ? this.reportInfo.processData.device.vehicle.sn : "";
                            Core.Api.Vehicle.getVehicleBomBySn(sn).then(res => {
                                this.componentList = res.bom;
                            });
                        }
                    }
                    console.log( res );
                });

            },

            showAllInfo() {
                this.activeNames = ['1', '2', '3', '4', '5']
            },

            onClickPrint() {
//                window.print();
                let print = "div.print-content-wrap";

                let extraCss = "";
                let mode = "iframe";
                let close = false;
                let retainAttr = ['class', 'id', 'style', 'on'];
                let headElements = '<meta charset="utf-8" /><title>'+ this.reportInfo.vehicle_sn +'</title>';

                let options = {
                    mode: mode,
                    popClose: close,
                    extraCss: extraCss,
                    retainAttr: retainAttr,
                    extraHead: headElements
                };
                $(print).printArea(options);
            },

            //图片上传
            handleRemove(file, fileList) {
                console.log(file, fileList);
                // this.upload.photographList = fileList
            },
            handlePreview(url) {
                this.dialogImageUrl = url;
                this.preDialogVisible = true;
            },

            handleSuccess(file, fileList) {
                if (this.upload.photographList.length >= 5) {
                    this.$message.warning('Up to 5 pictures')
                    return;
                }
                console.log(file, "file")
                console.log(fileList, 'fileList')
                let targetFile = file.data.file.name
                console.log(targetFile)
                this.picList.push(targetFile)
            },

            removeImg(index) {
                console.log(index)
                this.picList.splice(index, 1);
            },


            saveReport() {
                let defffect = this.selectErrorList.join();
                let comment = this.remark;
                let file = this.picList.join();
                console.log(file, 111)
                Core.Api.Vehicle.saveVehicleReport(this.reportId, this.reportInfo.vehicle_id, this.reportInfo.report_data,
                    defffect, comment, file).then(res => {
                    this.$message.success('Save success');
                })
            },

            goBack() {
                if (Core.Data.getOrg().type == 1) {
                    this.$router.push({name: "brandDiagnosisReportList"});
                }
                if (Core.Data.getOrg().type == 2) {
                    this.$router.push({name: "dealerDiagnosisReportList"});
                }
                if (Core.Data.getOrg().type == 3) {
                    this.$router.push({name: "assemblerDiagnosisReportList"});
                }
                if (Core.Data.getOrg().type == 4) {
                    this.$router.push({name: "componentDiagnosisReportList"});
                }
            },
            goBackDiagnosis(){
                if (Core.Data.getOrg().type == 1) {
                    this.$router.push({name: "brandDiagnosisNormal"});
                }
                if (Core.Data.getOrg().type == 2) {
                    this.$router.push({name: "dealerDiagnosisNormal"});
                }
                if (Core.Data.getOrg().type == 3) {
                    this.$router.push({name: "assemblerDiagnosisNormal"});
                }
                if (Core.Data.getOrg().type == 4) {
                    this.$router.push({name: "componentDiagnosisNormal"});
                }
            }
        }
    }
</script>


<style lang="scss">
    .report-container {
        .header {
            .right {
                float: right;
                .print-btn {
                    background: #98B42C;
                    border-radius: 100px;
                }
            }
        }
        .content {
            .list-group {
                .list-group-item {
                    display: flex;
                    align-items: center;
                    line-height: 20px;
                    padding: 6px 30px;
                    min-height: 50px;
                    .label-title {
                        width: 300px;
                        font-size: 14px;
                        color: #A1A5B1;
                    }
                    .label-content {
                        width: 100%;
                        font-size: 14px;
                        color: #A1A5B1;
                        /*.item-group{*/
                            /*padding: 10px*/
                        /*}*/
                    }
                    .content {
                        flex: 1;
                        display: flex;
                        align-items: flex-start;
                        flex-direction: column;
                        font-size: 14px;
                        color: #FFFFFF;
                        .split {
                            padding: 0 20px;
                        }
                        .error {
                            flex: 1
                        }
                    }
                    &:nth-child(2n) {
                        background: rgba(39, 43, 55, 0.2);
                    }
                }
            }
            .remark-row {
                padding: 6px 30px;
                .el-upload--picture-card {

                }
            }
            .defect-list {
                padding-bottom: 6px;
                .error-span {
                    color: white;
                    margin-right: 10px;
                    display: inline-block;
                    width: 300px;
                    vertical-align: top;
                }
            }
        }
        .print-content-wrap {
            display: none;
        }
        .content-wrap {
            /*display: none;*/
        }
        .w-100 {
            width: 290px;
        }
        .inline-block {
            display: inline-block;
        }
        .pre-dialog {
            width: auto;
            .el-dialog__header {
                display: none;
            }
            .el-dialog__body {
                padding: 0;
                >div{
                    font-size: 0;
                }
            }
            .pre-img {
                min-width: 100px;
                max-width: 500px;
            }
        }
        .img-holder {
            position: relative;
            width: 160px;
            display: inline-block;
            background-color: transparent;
            margin-right: 10px;
            .img-content {
                width: 160px;
            }
        }
        .video-holder{
            display:inline-block;
            margin-right:10px;
            .videoItem{
                width:160px;
            }
        }
        .color-red {
            color: #E26829;
        }
    }

</style>



// WEBPACK FOOTER //
// report.vue?dde3d5a6