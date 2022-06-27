<template>
    <div class="controller">
        <div class="controller-content">
            <label class="title">
                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                <div class="connect-tip">
                    <a href="javascript:;" @click="dialogTipVisible = true" class="btn-tip btn-underline">
                        <img src="../../assets/images/icon/icon-help.png" class="tip-img"/>
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
                    <span @click="jumpToUpdate" v-show="showUpdateTag">New version ready for Update</span>
                </div>
                <div class="part">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">SN</label>
                        <el-input class="input input-sn " v-model="sn"></el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label">SN</label>
                        <el-input class="input " v-model="sn"></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" class="btn-write" @click="writeSn" :disabled = "loading"
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
                <div class="part right">
                    <div class="com" v-if="!usbHID">
                        <label class="label">{{ $t('cNam.com') }}</label>
                        <el-select v-model="com" @change="changeCom" class="com-select" @click.native="getComList"
                                   :placeholder="$t('cNam.select')">
                            <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                            </el-option>
                        </el-select>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button class="btn-connect" @click="connect" type="primary">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="com" v-else>
                        <label class="label">USB HID</label>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button class="btn-connect" @click="connect" :disabled = "loading || isReset">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>
                            <el-button class="btn-write" type="preview" @click="readBase" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.read') }}
                            </el-button>
                            <!--<el-button class="btn" type="primary" @click="reset" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.reset') }}</el-button>-->
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
                <div class="part">
                    <div class="item" v-show = "!isCan">
                        <label class="label">{{ $t('cNam.spdMeterType') }}</label>
                        <el-input class="input" v-model="spdMeterType" disabled></el-input>
                    </div>
                    <div class="item" v-show = "!isCan">
                        <label class="label">{{ $t('cNam.nmlVolt') }}</label>
                        <el-input class="input" v-model="nmlVolt" disabled>
                            <template slot="append">V</template>
                        </el-input>
                    </div>
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT && isCan">
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCodeRead" disabled></el-input>
                    </div>
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT && !isCan">
                        <label class="label">{{ $t('cNam.manName') }}</label>
                        <el-input class="input" v-model="manNameRead" disabled></el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.speedLimit') }}</label>
                        <el-input class="input" v-model="speedLimitRead" disabled>
                            <template slot="append">Km/h</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                        <el-input class="input" v-model="wheelDiameterRead" disabled></el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.circumference') }}</label>
                        <el-input class="input" v-model="circumferenceRead" disabled>
                            <template slot="append">mm</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conSpeed') }}</label>
                        <el-input class="input" v-model="conSpeed" disabled>
                            <template slot="append">Km/h</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conCurrent') }}</label>
                        <el-input class="input" v-model="conCurrent" disabled>
                            <template slot="append">A</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conVoltage') }}</label>
                        <el-input class="input" v-model="conVoltage" disabled>
                            <template slot="append">V</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.motorTemperature') }}</label>
                        <el-input class="input" v-model="motorTemperature" disabled>
                            <template slot="append">℃</template>
                        </el-input>
                    </div>
                </div>
                <div class="part right">
                    <div class="item" v-show = "!isCan">
                        <label class="label">{{ $t('cNam.spdMeterSignalNum') }}</label>
                        <el-input class="input" v-model="spdMeterSignalNum" disabled></el-input>
                    </div>
                    <div class="item" v-show = "!isCan">
                        <label class="label">{{ $t('cNam.maxCur') }}</label>
                        <el-input class="input" v-model="maxCur" disabled>
                            <template slot="append">A</template>
                        </el-input>
                    </div>

                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.capacityLeft') }}</label>
                        <el-input class="input" v-model="conCapacityLeft" disabled>
                            <template slot="append">%</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conSingleTrip') }}</label>
                        <el-input class="input" v-model="conSingleTrip" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conSingleTripLeft') }}</label>
                        <el-input class="input" v-model="conSingleTripLeft" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conCadence') }}</label>
                        <el-input class="input" v-model="conCadence" disabled>
                            <template slot="append">RPM</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conTorque') }}</label>
                        <el-input class="input" v-model="conTorque" disabled>
                            <template slot="append">mV</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.calorie') }}</label>
                        <el-input class="input" v-model="calorie" disabled>
                            <template slot="append">Kcal</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.assistStatus') }}</label>
                        <el-input class="input" v-model="assistance" disabled></el-input>
                    </div>
                    <div class="item" v-show = "isCan">
                        <label class="label">{{ $t('cNam.conTemperature') }}</label>
                        <el-input class="input" v-model="conTemperature" disabled>
                            <template slot="append">℃</template>
                        </el-input>
                    </div>
                </div>
            </div>
            <!-- Uart 页面逻辑  -->
            <div class="data" v-if="!isCan">
                <div class="part">
                    <div class="head">
                        <label class="label">{{ $t('cNam.limitVolt') }}</label>
                        <el-input class="input" v-model="limitVolt" disabled></el-input>
                    </div>
                    <div class="text">
                        <label class="label">{{ $t('cNam.limitCurrent') }}(%)</label>
                        <label class="label">{{ $t('cNam.limitSpd') }}(%)</label>
                    </div>
                    <div class="content">
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}0</label>
                            <el-input class="input-left" v-model="curList[0]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[0]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}1</label>
                            <el-input class="input-left" v-model="curList[1]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[1]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}2</label>
                            <el-input class="input-left" v-model="curList[2]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[2]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}3</label>
                            <el-input class="input-left" v-model="curList[3]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[3]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}4</label>
                            <el-input class="input-left" v-model="curList[4]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[4]" disabled></el-input>
                        </div>
                    </div>
                </div>
                <div class="part right">
                    <div class="head">
                        <label class="label">{{ $t('cNam.limitCurrent') }}</label>
                        <el-input class="input space" v-model="limitCur" disabled></el-input>
                    </div>
                    <div class="text">
                        <label class="label">{{ $t('cNam.limitCurrent') }}(%)</label>
                        <label class="label">{{ $t('cNam.limitSpd') }}(%)</label>
                    </div>
                    <div class="content">
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}5</label>
                            <el-input class="input-left" v-model="curList[5]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[5]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}6</label>
                            <el-input class="input-left" v-model="curList[6]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[6]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}7</label>
                            <el-input class="input-left" v-model="curList[7]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[7]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}8</label>
                            <el-input class="input-left" v-model="curList[8]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[8]" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('cNam.assit') }}9</label>
                            <el-input class="input-left" v-model="curList[9]" disabled></el-input>
                            <el-input class="input-right" v-model="spdList[9]" disabled></el-input>
                        </div>
                    </div>
                </div>
                <div class="btn-group">
                    <el-button type="preview" @click="writeParam"  v-if="userName.indexOf('fqc') > -1">{{ $t('cNam.write') }}</el-button>
                </div>
            </div>

            <!-- Can 页面逻辑  -->
            <div class="info" v-if="isCan">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.setting') }}</label>
                </div>
                <div class="part">
                    <div class="item">
                        <label class="label" >{{ $t('cNam.speedLimit') }}</label>
                        <el-select v-model="speedLimit"
                                   @change=""
                                   class="com-select"
                                   @click.native=""
                                   filterable
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
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" class="btn-write" @click="clickWriteParam()"
                            >{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                </div>
                <div class="part right">
                    <div class="item" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label" >{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCode"  placeholder="6004"></el-input>
                    </div>
                    <div class="btn-row" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="primary" class="btn-write" @click="writeClientCode()">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <div class="item" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.manName') }}</label>
                        <el-input class="input" v-model="manName" placeholder="6005"></el-input>
                    </div>
                    <div class="btn-row" v-show="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="preview" @click="readManufacturer">{{ $t('cNam.read') }}</el-button>
                            <el-button type="primary" @click="writeManufacturer">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
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
                    <!--<div class="item">-->
                    <!--<label class="label" >{{ $t('cNam.model') }}</label>-->
                    <!--<el-input class="input" v-model="modelWrite"></el-input>-->
                    <!--</div>-->
                    <!--<div class="item">-->
                    <!--<label class="label" >{{ $t('cNam.hardwareVersion') }}</label>-->
                    <!--<el-input class="input" v-model="hardwareWrite"></el-input>-->
                    <!--</div>-->
                    <!--<div class="btn-row">-->
                    <!--<div class="btn-group">-->
                    <!--<el-button type="info " @click="clickCopy">复制数据</el-button>-->
                    <!--<el-button type="preview" class="btn-write" @click="clickWriteReset()">{{ $t('cNam.write') }}</el-button>-->
                    <!--</div>-->
                    <!--</div>-->
                    <div class="item">
                        <label class="label">{{ $t('cNam.positionSensor') }}</label>
                        <el-button class="btn-connect" @click="sensorSetStart">{{ $t('cNam.start') }}</el-button>
                        <span style="padding:0 20px 0 20px" v-if="sensorStart">{{ $t('cNam.positionSensor') }}+{{ $t('cNam.start') }}</span>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.torqueSenCali') }}</label>
                        <el-button class="btn-connect" @click="sensorZeroSetStart">{{ $t('cNam.start') }}</el-button>
                        <span style="padding:0 20px 0 20px" v-if="sensorZeroStart">{{ $t('cNam.torqueSenCali') }}+{{ $t('cNam.start') }}</span>
                    </div>
                </div>
            </div>
            <!-- 控制器参数设置  1340-1341-1212  SRAM-->
            <div class="info " v-show = "isCan && (org.type === Core.Const.ORG_TYPE.COMPONENT || org.id === 1340 || org.id === 1341 || org.id === 1212 )">
            <!--<div class="info">-->
                <div class="con-Params">
                    <label class="label">{{ $t('cNam.paramsName') }}</label>
                    <el-input class="input" v-model="ctrlParamsVersion"></el-input>
                    <el-button class="btn-connect" @click="conParamsGetOnline">{{ $t('cNam.paramsCloud') }}</el-button>
                </div>
                <div class="con-Params">
                    <el-button class="btn-connect" @click="conParamsReadAll">Read All</el-button>
                    <el-button class="btn-connect" @click="conParamsWriteAll">Write All</el-button>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div>
                        <div class="con-Params">
                            Params  Read:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6011"
                                v-model="conParamsRead_11"></el-input>
                            <el-button class="btn-connect" type="preview" @click="conParamsRead_6011">{{ $t('cNam.read') }}6011</el-button>
                        </div>
                        <div class="con-Params">
                            Params Write:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6011"
                                v-model="conParamsWrite_11"></el-input>
                            <el-button class="btn-connect" @click="conParamsCombine_6011">{{ $t('cNam.paramsCombine') }}</el-button>
                            <el-button class="btn-connect" type="preview" @click="conParamsUpdate_6011">{{ $t('cNam.write') }}6011</el-button>
                        </div>
                    </div>
                    <div>
                        <div class="con-Params">
                            Params  Read:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6012"
                                v-model="conParamsRead_12"></el-input>
                            <el-button class="btn-connect" type="preview" @click="conParamsRead_6012">{{ $t('cNam.read') }}6012</el-button>
                        </div>
                        <div class="con-Params">
                            Params Write:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6012"
                                v-model="conParamsWrite_12"></el-input>
                            <el-button class="btn-connect" @click="conParamsCombine_6012">{{ $t('cNam.paramsCombine') }}</el-button>
                            <el-button class="btn-connect" type="preview" @click="conParamsUpdate_6012">{{ $t('cNam.write') }}6012</el-button>
                        </div>
                    </div>

                    <template>
                        <el-collapse v-model="openCollapse">
                            <el-collapse-item  v-for="(data, index) in dataObject" :name= index  :title= Object.keys(data)[0] >
                                <div>
                                    <div class="table-wrap">
                                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                                            <thead>
                                            <td v-for="item in tableHead">{{item}}</td>
                                            </thead>
                                            <tbody class="height2">
                                                <tr v-for="(item, index) in data[Object.keys(data)[0]]">
                                                <td class="long-td"><span>{{item.title}}</span>
                                                    <el-tooltip class="item" effect="dark" :content="item.title_en" placement="right-start">
                                                        <i>{{item.unit}}</i>
                                                    </el-tooltip>
                                                </td>
                                                <td>{{item.category}}</td>
                                                <td>{{item.data}}</td>
                                                <td>{{item.length}}</td>
                                                <td class="short-td">
                                                    <el-input v-model="item.standard_value"></el-input>
                                                </td>
                                                <td class="short-td">
                                                    <el-input v-model="item.read_value"></el-input>
                                                </td>
                                                <td class="long-td" v-if = "Array.isArray(item.range) && typeof(item.range[0]) == 'number' ">
                                                    <el-select v-model="item.set_value" :placeholder="$t('cNam.select')" class="input">
                                                        <el-option
                                                            v-for="(i,index) in item.range"
                                                            :key="index"
                                                            :label="i"
                                                            :value="i">
                                                        </el-option>
                                                    </el-select>
                                                </td>
                                                <td class="long-td" v-else-if = "Array.isArray(item.range) ">
                                                    <el-select v-model="item.set_value" :placeholder="$t('cNam.select')" class="input">
                                                        <el-option
                                                            v-for="ii in item.range"
                                                            :key="ii.id"
                                                            :label="ii.value"
                                                            :value="ii.id">
                                                        </el-option>
                                                    </el-select>
                                                </td>
                                                <td v-else><el-input v-model="item.set_value"></el-input></td>
                                            </tr>
                                            </tbody>
                                        </table>
                                    </div>
                                </div>
                            </el-collapse-item>
                        </el-collapse>
                    </template>

                    <div>
                        <div class="con-Params">
                            Params  Read:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6017"
                                v-model="conParamsRead_17"></el-input>
                            <el-button class="btn-connect" type="preview" @click="conParamsRead_6017">{{ $t('cNam.read') }}6017</el-button>
                        </div>
                        <div class="con-Params">
                            Params Write:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6017"
                                v-model="conParamsWrite_17"></el-input>
                            <el-button class="btn-connect" @click="conParamsCombine_6017">{{ $t('cNam.paramsCombine') }}</el-button>
                            <el-button class="btn-connect" type="preview" @click="conParamsUpdate_6017">{{ $t('cNam.write') }}6017</el-button>
                        </div>
                    </div>
                    <div>
                        <div class="con-Params">
                            Params  Read:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6018"
                                v-model="conParamsRead_18"></el-input>
                            <el-button class="btn-connect" type="preview" @click="conParamsRead_6018">{{ $t('cNam.read') }}6018</el-button>
                        </div>
                        <div class="con-Params">
                            Params Write:
                            <el-input
                                type="textarea"
                                :rows="2"
                                resize="none"
                                placeholder="6018"
                                v-model="conParamsWrite_18"></el-input>
                            <el-button class="btn-connect" @click="conParamsCombine_6018">{{ $t('cNam.paramsCombine') }}</el-button>
                            <el-button class="btn-connect" type="preview" @click="conParamsUpdate_6018">{{ $t('cNam.write') }}6018</el-button>
                        </div>
                    </div>
                </div>
            </div>

            <!--<el-button class="btn-connect" @click="conParamsLocalSave_13">存储到本地</el-button>-->
            <!--<td>-->
            <!--<el-tooltip class="item" effect="dark" :content="item.description" placement="right-end">-->
            <!--<el-button>{{item.read_value}}</el-button>-->
            <!--</el-tooltip>-->
            <!--</td>-->

        </div>

        <!--说明弹框-->
        <el-dialog :title="$t('cNam.operationTip')" custom-class="tip-dialog"
                   :visible.sync="dialogTipVisible">
            <div>
                <h1> {{ $t('cNam.txtTitle1') }} </h1>
                <div class="txt"><span class="itemName">COM</span> <span>{{ $t('cNam.txtTitleCom') }}</span></div>
                <div class="txt"><span class="itemName">USB HID</span>   <span>{{ $t('cNam.txtTitleHid') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.connect') }}</span> <span>{{ $t('cNam.txtTitleCon')
                    }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.read') }}</span> <span>{{ $t('cNam.txtTitleRead')
                    }}</span></div>
                <div class="txt" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT"><span
                    class="itemName">{{ $t('cNam.write') }}</span> <span>{{ $t('cNam.txtTitleWrt') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.reset') }}</span> <span>{{ $t('cNam.txtTitleReset')
                    }}</span></div>
                <div class="clear-20"></div>
                <h1> {{ $t('cNam.txtTitle2') }} </h1>
                <div class="txt">{{ $t('cNam.txtNote1') }}</div>
            </div>
        </el-dialog>

        <el-dialog
            :title="$t('auth')"
            :visible.sync="dialogWriteAuthVisible">
            <div>
                <label>
                    {{ $t('noControllerAuthTip') }}
                </label>
                <div class="clear-20"></div>
                <el-select v-model="brandId" slot="append" class="input">
                    <el-option
                        v-for="item in brandList"
                        :key="item.id"
                        :label="item.name"
                        :value="item.id">
                    </el-option>
                </el-select>
            </div>
            <span slot="footer" class="dialog-footer">
                 <el-button type="primary" @click="applyAuth(0)">{{ $t('applyControllerAuth') }}</el-button>
                 <el-button type="primary" @click="applyAuth(1)">{{ $t('applyAllAuth') }}</el-button>
                 <el-button type="primary" @click="dialogWriteAuthVisible = false">{{ $t('cNam.cancel') }}</el-button>
             </span>
        </el-dialog>
    </div>
</template>

<script>
    import Controller from "../../../device/controller"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import vueLoading from 'vue-loading-template'
    import Core from 'core/core'
    import ctrlParams from 'core/ctrlParams'
    import circumferenceObject from 'core/circumference'

    export default {
        name: 'controller',
        components: {
            vueLoading
        },
        data() {
            return {
                com: '',
                comList: [],
                isConnected: Controller.port.isConnected(),

                sn: '',
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

                loading: false,
                writeStatus: false,

                isDealerOrAssembler: false,
                dialogWriteAuthVisible: false,
                brandId: '',
                brandList: [],
                auth: "",

                offLineFlag: false,

                org: Core.Data.getOrg(),
                Core: Core,
                userName: Core.Data.get("userName") || "",

                Controller: Controller,
                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.EDIT,

                dialogTipVisible: false,

                isCan: false,
                usbHID: false,
                readInterval: null,
                isReset: false,
                lanTrans: ['Turn off','Turn on'],

                speedLimit: '',
                speedLimits: [],
                wheelDiameter: '',
                wheelDiameters: [],
                circumference: '',
                circumferences: [],
                manName: '',
                clientCode: '',
                clientCodeRead: '',

                readSn: '', // 读取到的sn
                offlineRecordList: [],
                offlineRecordLength: 0,

                tableHead: ['Item Name','Type','Bit','Length','Standard','Read','Set Value'],
                piCompItems: [],
                conParamsRead_11: '',
                conParamsWrite_11: '',
                conParamsRead_12: '',
                conParamsWrite_12: '',
                conParamsRead_13: '',
                conParamsWrite_13: '',
                conParamsRead_14: '',
                conParamsWrite_14: '',
                conParamsRead_15: '',
                conParamsWrite_15: '',
                conParamsRead_16: '',
                conParamsWrite_16: '',
                conParamsRead_17: '',
                conParamsWrite_17: '',
                conParamsRead_18: '',
                conParamsWrite_18: '',

                ctrlParamsObject:{},
                ctrlParams6011: [],
                ctrlParams6012: [],
                ctrlParams6013: [],
                ctrlParams6014: [],
                ctrlParams6015: [],
                ctrlParams6016: [],
                ctrlParams6017: [],
                ctrlParams6018: [],
                ctrlParamsVersion: '',

                dataObject: [],
                categoryGroup: [],

                modelWrite: '',
                hardwareWrite: '',
                errorCode: '',
                bootLoad: '',
                manufacturer: '',

                reconnectInterval: null,
                connectionIntervalCon: null,

                sensorStart: false,
                sensorZeroStart: false,

                activeName: 'first',
                openCollapse: 0,

                showUpdateTag: false,

            }
        },

        i18n: {
            messages: {
                en: EN.Components.Tool.Controller,
                zh: ZH.Components.Tool.Controller,
                de: DE.Components.Tool.Controller,
                nl: NL.Components.Tool.Controller
            }
        },

        mounted() {
            this.initTrans();

            this.wheelDiameters = Core.Const.wheelDiameters;
            if (Core.Data.getOrg()) {
                this.isDealerOrAssembler = Core.Data.getOrg().type === Core.Const.ORG_TYPE.DEALER || Core.Data.getOrg().type === Core.Const.ORG_TYPE.OEM || Core.Data.getOrg().type === Core.Const.ORG_TYPE.SPECIAL_DEAlER;
            }
            this.offLineFlag = Core.Data.getOffline();
            this.userId = Core.Data.getUser().id;
            this.recordType = Core.Const.OBJECT_RECORD_TYPE.EDIT;

            for (let i = 10; i <= 60; i++) {
                this.speedLimits.push(i);
                if(i<60){
                    this.speedLimits.push(i+0.5);
                }
            }
            // 经销商离线10-25
            if (this.isDealerOrAssembler && this.offLineFlag) {
                this.speedLimits = [];
                for (let i = 10; i <= 25; i++) {
                    this.speedLimits.push(i);
                    if(i<25){
                        this.speedLimits.push(i+0.5);
                    }
                }
//                this.wheelDiameters.pop();
            }
            this.speedLimit = 25;
            this.getOfflineRecordList();
            this.offlineRecordLength = this.offlineRecordList.length;

            Controller.port.timeLog('Controller');
            Controller.port.setBoxOperation();
            this.checkConnection();

            this.ctrlParamsObject = ctrlParams;
            this.ctrlParams6011 = this.ctrlParamsObject['6011'];
            this.ctrlParams6012 = this.ctrlParamsObject['6012'];
            this.ctrlParams6013 = this.ctrlParamsObject['6013'];
            this.ctrlParams6014 = this.ctrlParamsObject['6014'];
            this.ctrlParams6015 = this.ctrlParamsObject['6015'];
            this.ctrlParams6016 = this.ctrlParamsObject['6016'];
            this.ctrlParams6017 = this.ctrlParamsObject['6017'];
            this.ctrlParams6018 = this.ctrlParamsObject['6018'];

            this.parseParams();

        },
        methods: {
            jumpToUpdate() {
                this.$router.push(
                    {
                        name: "brandToolUpdate",
                        query: { from: 'Ctrl', sn: this.sn.trim(), soft: this.version, isUpdate: true}
                    },);
            },
            compare() {
                Core.Api.Object.objectVersionCompare(this.sn.trim(), this.version.trim()).then(res => {
                    if(res && res.flag === "4"){
                        this.showUpdateTag = true;
                    }else{
                        this.showUpdateTag = false;
                    }
                });
            },
            checkSnVersionUpdate() {
                if(this.sn && this.version && !this.offLineFlag){
                    this.compare();
                }
            },
            addCategoryItem(item, category){
                let dataItem = item;
                this.dataObject.forEach((i,index)=>{
                    if(Object.keys(i)[0] === category){
                        this.dataObject[index][category].push(dataItem);
                    }
                })
            },

            filterCategory(item){

                if(!item) return;
                switch(item.category){
                    case this.categoryGroup[0]:
                        this.addCategoryItem(item, this.categoryGroup[0]);
                        break;
                    case this.categoryGroup[1]:
                        this.addCategoryItem(item, this.categoryGroup[1]);
                        break;
                    case this.categoryGroup[2]:
                        this.addCategoryItem(item, this.categoryGroup[2]);
                        break;
                    case this.categoryGroup[3]:
                        this.addCategoryItem(item, this.categoryGroup[3]);
                        break;
                    case this.categoryGroup[4]:
                        this.addCategoryItem(item, this.categoryGroup[4]);
                        break;
                    case this.categoryGroup[5]:
                        this.addCategoryItem(item, this.categoryGroup[5]);
                        break;
                    case this.categoryGroup[6]:
                        this.addCategoryItem(item, this.categoryGroup[6]);
                        break;
                    case this.categoryGroup[7]:
                        this.addCategoryItem(item, this.categoryGroup[7]);
                        break;
                    case this.categoryGroup[8]:
                        this.addCategoryItem(item, this.categoryGroup[8]);
                        break;
                    case this.categoryGroup[9]:
                        this.addCategoryItem(item, this.categoryGroup[9]);
                        break;
                    case this.categoryGroup[10]:
                        this.addCategoryItem(item, this.categoryGroup[10]);
                        break;
                    case this.categoryGroup[11]:
                        this.addCategoryItem(item, this.categoryGroup[11]);
                        break;
                    case this.categoryGroup[12]:
                        this.addCategoryItem(item, this.categoryGroup[12]);
                        break;
                    case this.categoryGroup[13]:
                        this.addCategoryItem(item, this.categoryGroup[13]);
                        break;
                    case this.categoryGroup[14]:
                        this.addCategoryItem(item, this.categoryGroup[14]);
                        break;
                    case this.categoryGroup[15]:
                        this.addCategoryItem(item, this.categoryGroup[15]);
                        break;
                    case this.categoryGroup[16]:
                        this.addCategoryItem(item, this.categoryGroup[16]);
                        break;
                    default:
                        break;
                }
            },

            parseParams(){
                let keys = Object.keys(this.ctrlParamsObject);
                let category = new Set;
                keys.forEach((index) => {
                    this.ctrlParamsObject[index].forEach((i)=>{
                        category.add(i.category);
                    });
                });

                this.categoryGroup =  Array.from(category);

                this.categoryGroup.forEach((i)=>{
                     this.dataObject.push({[i]:[]});
                });

                keys.forEach((index) => {
                    this.ctrlParamsObject[index].forEach((i)=>{
                        this.filterCategory(i,index);
                    });
                });
            },

            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalCon){
                    clearInterval(vm.connectionIntervalCon);
                    vm.connectionIntervalCon = '';
                }
                vm.connectionIntervalCon = setInterval(()=>{
                    let connection = Controller.port.getConnection();
                    if(connection.usb){
//                        this.checkUSBUartOrCan();
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalCon);
                    }
                },300)
            },
//            checkUSBUartOrCan(){
//                let checkUSBUartOrCanIntervalCon = setInterval(()=>{
//                    let USBUartOrCan = Controller.port.getConnection();
//                    if(USBUartOrCan.usb){
//                        if(USBUartOrCan.can){
//                            this.isCan = true;
//                        }
//                        if(USBUartOrCan.uart){
//                            this.isCan = false;
//                        }
//                        if(USBUartOrCan.can || USBUartOrCan.uart){
//                            clearInterval(checkUSBUartOrCanIntervalCon);
//                        }
//                    }
//                },800)
//            },
            init(){
                Controller.port.startConnection();
                if (Controller.port.getUSBDevice() || this.usbHID) {
                    this.usbHID = true;
                    console.log(" get USB Device Controller");
                    if(this.isReset){
                        this.connect(1);
                    }else{
                        this.connect();
                    }
                } else {
                    console.log("COM Controller");
                    this.usbHID = false;
                    this.getComList();
                }
                this.isConnected = Controller.port.isConnected();

            },
            initTrans(){
                this.lanTrans[0] = this.$t('cNam.assistOff');
                this.lanTrans[1] = this.$t('cNam.assistOn');
            },
            getComList(){
//                if (Controller.port.getPort() != '' && Controller.port.isConnected()) {
//                    this.com =Controller.port.getPort();
//                    this.comList = [Controller.port.getPort()];
//                    this.isConnected = Controller.port.isConnected();
//                    return
//                }
                Controller.port.getPortList().subscribe(res => {
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
//                    Controller.port.setCan(true);
//                }else{
//                    Controller.port.setCan(false);
//                }
//                this.isConnected = Controller.port.isConnected();
//                this.init();
//            },
            changeCom() {
//                this.isConnected = false;
            },

            connect(type) {
                if ( this.usbHID ) {
                    if(type === 1){
                        setTimeout(()=>{
                            this.test();
                        },1000)
                    }else{
                        this.test();
                    }
                    this.isConnected = Controller.port.isConnected();
                    this.isCan = !Controller.port.checkUsbUart();
                    if(this.isCan){
                        this.readInterval = 1200;
                    }else{
                        this.readInterval = 4500;
                    }
                    console.log("usbHID connect");
                    Controller.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb') {
                            this.isConnected = false;
                            this.clear();
                        }
                    });
                } else {
                    this.readInterval = 7800;
                    if (!this.com) {
                        this.$message({
                            message: this.$t('cNam.selectComTip'),
                            type: 'warning'
                        });
                        return;
                    } else {
                        Controller.connect(this.com);
                        Controller.getBus().subscribe(res => {
                            if (res.type === 'open' || res.type === 'connect') {
                                this.isConnected = true;
                            }
                            if (res.type === 'close' || res.type === 'error') {
                                this.com = '';
                                this.isConnected = false;
                                this.loading = false;
//                        this.$message({
//                            message: this.$t('cNam.comDisconnect'),
//                            type: 'warning'
//                        });
                            }
                        });
                    }
                }
            },
            handleClick(tab, event) {
//                console.log(tab, event);
//                VueComponent {_uid: 97, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: VueComponent, …}$attrs: Object__proto__: Objectconstructor: ƒ Object()hasOwnProperty: ƒ hasOwnProperty()isPrototypeOf: ƒ isPrototypeOf()propertyIsEnumerable: ƒ propertyIsEnumerable()toLocaleString: ƒ toLocaleString()toString: ƒ toString()valueOf: ƒ valueOf()__defineGetter__: ƒ __defineGetter__()__defineSetter__: ƒ __defineSetter__()__lookupGetter__: ƒ __lookupGetter__()__lookupSetter__: ƒ __lookupSetter__()get __proto__: ƒ __proto__()set __proto__: ƒ __proto__()$children: (233) [VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, VueComponent, …]$createElement: ƒ (a, b, c, d)$el: div.el-tab-pane$listeners: (...)$options: {parent: VueComponent, _parentVnode: VNode, propsData: {…}, _parentListeners: {…}, _renderChildren: Array(5), …}$parent: VueComponent {_uid: 89, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: VueComponent, …}$refs: {}$root: Vue {_uid: 4, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: Vue, …}$scopedSlots: {default: ƒ, $stable: false, $key: undefined, $hasNormal: true}$slots: {default: Array(5)}$store: Store {_committing: false, _actions: {…}, _actionSubscribers: Array(0), _mutations: {…}, _wrappedGetters: {…}, …}$vnode: VNode {tag: "vue-component-44-ElTabPane", data: {…}, children: undefined, text: undefined, elm: div.el-tab-pane, …}active: trueclosable: falsedisabled: falseindex: "1"isClosable: falselabel: "6012"labelContent: undefinedname: "second"_c: ƒ (a, b, c, d)_computedWatchers: {isClosable: Watcher, active: Watcher}_data: {__ob__: Observer}_directInactive: false_events: {}_hasHookEvent: false_i18n: VueI18n {_vm: Vue, _formatter: BaseFormatter, _missing: null, _root: null, _sync: true, …}_inactive: null_isBeingDestroyed: false_isDestroyed: false_isMounted: true_isVue: true_props: {}_renderProxy: Proxy {_uid: 97, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: VueComponent, …}_routerRoot: Vue {_uid: 4, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: Vue, …}_self: VueComponent {_uid: 97, _isVue: true, $options: {…}, _renderProxy: Proxy, _self: VueComponent, …}_staticTrees: null_subscribing: true_uid: 97_vnode: VNode {tag: "div", data: {…}, children: Array(5), text: undefined, elm: div.el-tab-pane, …}_watcher: Watcher {vm: VueComponent, deep: false, user: false, lazy: false, sync: false, …}_watchers: (4) [Watcher, Watcher, Watcher, Watcher]$d: (...)$data: (...)$http: (...)$i18n: (...)$isServer: (...)$n: (...)$promise: (...)$props: (...)$resource: (...)$route: (...)$router: (...)$ssrContext: (...)$t: (...)$tc: (...)$te: (...)$url: (...)get $attrs: ƒ reactiveGetter()set $attrs: ƒ reactiveSetter(newVal)get $listeners: ƒ reactiveGetter()set $listeners: ƒ reactiveSetter(newVal)get index: ƒ proxyGetter()set index: ƒ proxySetter(val)__proto__: Vue MouseEvent {isTrusted: true, screenX: 368, screenY: 342, clientX: 368, clientY: 239, …}
            },

            conParamsGetOnline(){
                // 从云端获取
                this.$message('Not support yet');
            },
            conParamsRead_6011(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_1(0x05, 0x02)
                    .then(res=>{
//                        getUsbBasicBus.next({type: 'data', data: {'model':res.dataParsed}});
                        this.conParamsRead_11 = res.data;
//                        this.conParamsRead_11 = "24010203000400050006000708091011121314001516000117180119002021012200230024002500260027004000002942000043444500014647484950005174";
                        this.conParamsParse(this.conParamsRead_11, this.ctrlParams6011);
//                        console.log(res.dataParsed);
                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 11');
                });
            },
            conParamsRead_6012(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_2(0x05, 0x02)
                    .then(res=>{
//                        getUsbBasicBus.next({type: 'data', data: {'model':res.dataParsed}});

                        this.conParamsRead_12 = res.data;
//                        this.conParamsRead_12 = "0f0c090807071e1e201c190c060605050404646464646432231e19140f0a06060505040403030302020232281e14100c50785046463c8296ffffffffffffff68";
                        this.conParamsParse(this.conParamsRead_12, this.ctrlParams6012);
//                        console.log(res.dataParsed);
                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 12');
                });
            },
            conParamsRead_6013(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_3(0x05, 0x02)
                    .then(res=>{
                        this.conParamsRead_13 = res.data;
                        this.conParamsParse(this.conParamsRead_13, this.ctrlParams6013);

                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 13');
                });
            },
            conParamsRead_6014(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_4(0x05, 0x02)
                    .then(res=>{
                        this.conParamsRead_14 = res.data;
                        this.conParamsParse(this.conParamsRead_14, this.ctrlParams6014);

                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 14');
                });
            },
            conParamsRead_6017(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_7(0x05, 0x02)
                    .then(res=>{
                        this.conParamsRead_17 = res.data;
                        this.conParamsParse(this.conParamsRead_17, this.ctrlParams6017);

                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 13');
                });
            },
            conParamsRead_6018(){
                // 从控制器读取
                Controller.port.usbSDK.machineInformation.get.conParams_8(0x05, 0x02)
                    .then(res=>{
                        this.conParamsRead_18 = res.data;
                        this.conParamsParse(this.conParamsRead_18, this.ctrlParams6018);

                    }).catch(err =>{
                    console.log(err);

                }).finally(()=>{
                    console.log('conParams 18');
                });
            },
            conParamsParse(value, ctrlParams){
                if(value){
                    let checkSum = Core.Util.calculateChecksumFF(value.substr(0,126));
                    console.log(checkSum);
                    if( checkSum == parseInt(value.substr(126,2),16) ){
                        this.$message("数据校验通过");
                        ctrlParams.forEach((item)=>{
                            let ind = parseInt(item.data.substr(0,2));
                            let len = item.length;
                            let data = value.substr(ind*2, len*2);
                            let arr = Buffer.from(data,'Hex').reverse();
                            item.read_value = parseInt( arr.toString('Hex'), 16);
                            item.set_value = parseInt( arr.toString('Hex'), 16);
                        });
                    }else{
                        this.$message("数据校验不通过");
                    }
                }
            },
            conParamsCombine_6011(){
                this.conParamsCombine("6011", this.ctrlParams6011);
            },
            conParamsCombine_6012(){
                this.conParamsCombine("6012", this.ctrlParams6012);
            },
            conParamsCombine_6013(){
                this.conParamsCombine("6013", this.ctrlParams6013);
            },
            conParamsCombine_6014(){
                this.conParamsCombine("6014", this.ctrlParams6014);
            },
            conParamsCombine_6017(){
                this.conParamsCombine("6017", this.ctrlParams6017);
            },
            conParamsCombine_6018(){
                this.conParamsCombine("6018", this.ctrlParams6018);
            },

            conParamsCombine(type, ctrlParams){
                let data = '';

                let dataIndexLast = 0;
                let firstIndex = 0;

                ctrlParams.forEach((item,index)=>{
                    let len = item.length;
                    let dataArr = item.data.split("-");
                    let dateTem,hexTemp;
                    let dataIndex = parseInt(dataArr[0]);

                    if(index === 0 && dataIndex !== 0){
                        firstIndex = dataIndex;
                        dataIndexLast = dataIndex;
                        data = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff".substr(0, firstIndex * 2) + data ;
                    }
                    let dataSupply = "";
                    if(dataIndex - dataIndexLast > 1) {
                        dataSupply = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF".substr(0, 2*(dataIndex - dataIndexLast - 1));
                    }

                    if(len === 1){
                        dataIndexLast = dataIndex;
                    }else{
                        dataIndexLast = parseInt(dataArr[len-1])
                    }

                    if(item.set_value){
                        dateTem = parseInt(item.set_value);
                    }else{
                        dateTem = 0x00;
                    }

                    hexTemp = this.addLength(dateTem.toString(16),len);
                    data += dataSupply + Buffer.from(hexTemp,'Hex').reverse().toString('Hex');

                });

                data = (data + 'ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff').substr(0, 126);
                let checkSum = Core.Util.calculateChecksumFF(data);
                console.log(checkSum);
                let value = data + Buffer.from([checkSum]).toString('Hex');
                switch(type){
                    case "6011":
                        this.conParamsWrite_11 = value;
                        break;
                    case "6012":
                        this.conParamsWrite_12 = value;
                        break;
                    case "6013":
                        this.conParamsWrite_13 = value;
                        break;
                    case "6014":
                        this.conParamsWrite_14 = value;
                        break;
                    case "6017":
                        this.conParamsWrite_17 = value;
                        break;
                    case "6018":
                        this.conParamsWrite_18 = value;
                        break;
                }

            },

            addLength(hexData,len){
                let hex = hexData || '';
                let step = len*2;
                let hexLength = hex.length/2;
                if(len === hexLength){
                    return hex
                }else if( len > hexLength ){
                    return ('000000'+hex).substr(-step);
                }else{
                    return ('------').substr(-step)
                }
            },
            conParamsUpdate_6011(){
                if(this.conParamsWrite_11 == ''){
                    this.conParamsCombine_6011();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_1(0x05, 0x02, this.conParamsWrite_11)
                    .then(res => {
//                        console.log(res);
                        if (res.can === 'NORMAL_ACK') {
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
                });
            },
            conParamsUpdate_6012(){
                if(this.conParamsWrite_12 == ''){
                    this.conParamsCombine_6012();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_2(0x05, 0x02, this.conParamsWrite_12)
                    .then(res => {
//                        console.log(res);
                        if (res.can === 'NORMAL_ACK') {
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
                });
            },
            conParamsUpdate_6013(){
                if(this.conParamsWrite_13 == ''){
                    this.conParamsCombine_6013();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_3(0x05, 0x02, this.conParamsWrite_13)
                    .then(res => {
//                        console.log(res);
                        if (res.can === 'NORMAL_ACK') {
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
                });
            },
            conParamsUpdate_6014(){
                if(this.conParamsWrite_14 == ''){
                    this.conParamsCombine_6014();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_4(0x05, 0x02, this.conParamsWrite_14)
                    .then(res => {
                        console.log(res);
                        if (res.can == 'NORMAL_ACK') {
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
                });
            },
            conParamsUpdate_6017(){
                if(this.conParamsWrite_17 == ''){
                    this.conParamsCombine_6017();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_7(0x05, 0x02, this.conParamsWrite_17)
                    .then(res => {
//                        console.log(res);
                        if (res.can === 'NORMAL_ACK') {
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
                });
            },
            conParamsUpdate_6018(){
                if(this.conParamsWrite_18 == ''){
                    this.conParamsCombine_6018();
                }
                Controller.port.usbSDK.machineInformation.update.conParams_8(0x05, 0x02, this.conParamsWrite_18)
                    .then(res => {
                        console.log(res);
                        if (res.can == 'NORMAL_ACK') {
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
                });
            },
            conParamsReadAll(){
                this.conParamsRead_6011();
                this.conParamsRead_6012();
                this.conParamsRead_6017();
                this.conParamsRead_6018();
            },
            conParamsWriteAll(){
                this.conParamsUpdate_6011();
                this.conParamsUpdate_6012();
                this.conParamsUpdate_6017();
                this.conParamsUpdate_6018();
            },
//            readBase(){
//                let vm = this;
//                let n = 0;
//                let readCount = setInterval(()=>{
//                    n++;
//                    console.log('[count]',n);
//                    vm.readBaseBasic();
//                    if(n>50){
//                        clearInterval(readCount);
//                    }
//                },2000);
//            },

            test(){
                Controller.port.checkUsbConnection('0101',1);
            },

            readBase() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.loading = true;
                this.writeStatus = false;
                console.log(this.readInterval);
                setTimeout(() => {
                    if (this.loading) {
                        if (!this.checkData()) {
                            this.$message({
                                message: this.$t('controllerTip'),
                                type: "warning"
                            });
                        }
                    }
                    this.loading = false;
                }, this.readInterval);
                this.clear();
                this.connect(1);
                this.showUpdateTag = false;
                if(!this.isCan || Controller.port.checkUsbUart()) {
                    Controller.sendCmdGetFullData();
                    Controller.getBus().subscribe(res => {
                        //                    console.log('Controller-vm2:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.ctrl) {
                            return;
                        }
                        if (res.data.ctrl.hasOwnProperty("sn")) {
                            this.sn = res.data.ctrl.sn;
                            this.readSn = this.sn;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.ctrl.hasOwnProperty("model")) {
                            this.model = res.data.ctrl.model;
                        }
                        if (res.data.ctrl.hasOwnProperty("version")) {
                            this.version = res.data.ctrl.version;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.ctrl.hasOwnProperty("cur")) {
                            this.curList = res.data.ctrl.cur;
                        }
                        if (res.data.ctrl.hasOwnProperty("spd")) {
                            this.spdList = res.data.ctrl.spd;
                        }
                        if (res.data.ctrl.hasOwnProperty("man_name")) {
                            this.manNameRead = res.data.ctrl.man_name;
                        }
                        if (res.data.ctrl.hasOwnProperty("nml_volt")) {
                            this.nmlVolt = res.data.ctrl.nml_volt;
                        }
                        if (res.data.ctrl.hasOwnProperty("max_cur")) {
                            this.maxCur = res.data.ctrl.max_cur;
                        }
                        if (res.data.ctrl.hasOwnProperty("limit_cur")) {
                            this.limitCur = res.data.ctrl.limit_cur;
                        }
                        if (res.data.ctrl.hasOwnProperty("limit_volt")) {
                            this.limitVolt = res.data.ctrl.limit_volt;
                        }
                        if (res.data.ctrl.hasOwnProperty("wheel_dia")) {
                            if (this.isCan) {
                                this.wheelDiameters.find((item) => {
                                    if (item.value == res.data.ctrl.wheel_dia) {
                                        this.wheelDiameterRead = item.label;
                                    }
                                });
                            } else {
                                this.wheelDiameterRead = res.data.ctrl.wheel_dia / 2;
                            }
                        }
                        if (res.data.ctrl.hasOwnProperty("speed_limit")) {
                            this.speedLimitRead = res.data.ctrl.speed_limit;
                        }
                        if (res.data.ctrl.hasOwnProperty("circumference")) {
                            this.circumferenceRead = res.data.ctrl.circumference;
                        }
                        if (res.data.ctrl.hasOwnProperty("spd_meter_type")) {
                            this.spdMeterType = res.data.ctrl.spd_meter_type;
                        }
                        if (res.data.ctrl.hasOwnProperty("spd_meter_signal_num")) {
                            this.spdMeterSignalNum = res.data.ctrl.spd_meter_signal_num;
                        }
                        if (res.data.ctrl.hasOwnProperty("hardware_version")) {
                            this.hardVersion = res.data.ctrl.hardware_version;
                        }
//                        if (res.data.ctrl.hasOwnProperty("info_0")) {
//                            let temInfo_0 = Buffer.from(res.data.ctrl.info_0, 'hex');
//                            this.conCapacityLeft = temInfo_0[0];
//                            this.conSingleTrip = (temInfo_0[1] + temInfo_0[2] * 256) / 100;
//                            this.conCadence = temInfo_0[3];
//                            this.conTorque = temInfo_0[4] + temInfo_0[5] * 256;
//                            this.conSingleTripLeft = (temInfo_0[6] + temInfo_0[7] * 256) / 100;
//                        }
//                        if (res.data.ctrl.hasOwnProperty("info_1")) {
//                            let temInfo_1 = Buffer.from(res.data.ctrl.info_1, 'hex');
//                            this.conSpeed = (temInfo_1[0] + temInfo_1[1] * 256) / 100;
//                            this.conCurrent = (temInfo_1[2] + temInfo_1[3] * 256) / 100;
//                            this.conVoltage = (temInfo_1[4] + temInfo_1[5] * 256) / 100;
//                            this.conTemperature = temInfo_1[6] - 40;
//                            this.motorTemperature = temInfo_1[7] - 40;
//                        }
//                        if (res.data.ctrl.hasOwnProperty("info_2")) {
//                            let temInfo_2 = Buffer.from(res.data.ctrl.info_2, 'hex');
//                            let temInfo_2_value = Controller.port.addZero(temInfo_2[0].toString(2), 8);
//                            this.assistance = temInfo_2_value.substr(7, 1) == '0' ? '无助推' : '助推';
//                        }
//                        if (res.data.ctrl.hasOwnProperty("info_5")) {
//                            let temInfo_5 = Buffer.from(res.data.ctrl.info_5, 'hex');
//                            this.calorie = temInfo_5[0] + temInfo_5[1] * 256;
//                        }
                    });
                }else {

                    Controller.port.getUsbBasic(0x02).subscribe(res => {
                        if (!res.data) {
                            return;
                        }
                        if (res.data.hasOwnProperty("sn")) {
                            this.sn = res.data.sn;
                            this.readSn = this.sn;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.hasOwnProperty("model")) {
                            this.model = res.data.model;
                        }
                        if (res.data.hasOwnProperty("version")) {
                            this.version = res.data.version;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.hasOwnProperty("hardware_version")) {
                            this.hardVersion = res.data.hardware_version;
                        }
                        if (res.data.hasOwnProperty("client_code")) {
                            this.clientCodeRead = res.data.client_code;
                        }
                    });

                    Controller.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if( res.type == 'error_usb' ) {
                            this.isConnected = false;
                            this.clear();
                        }
                        if (res.data.code === '32' && res.data.subCode === '00') {
                            // 2s 一次
                            let temInfo_0 = Buffer.from(res.data.data, 'hex');
                            if(temInfo_0.length === 8){
                                this.conCapacityLeft = temInfo_0[0];
                                this.conSingleTrip = (temInfo_0[1] + temInfo_0[2] * 256) / 100;
                                this.conCadence = temInfo_0[3];
                                this.conTorque = temInfo_0[4] + temInfo_0[5] * 256;
                                let tem = (temInfo_0[6] + temInfo_0[7] * 256);
                                if(tem < 65535){
                                    this.conSingleTripLeft = tem / 100;
                                }else{
                                    this.conSingleTripLeft = "-";
                                }
                            }
                        }
                        if (res.data.code === '32' && res.data.subCode === '01') {
                            let temInfo_1 = Buffer.from(res.data.data, 'hex');
                            if(temInfo_1.length === 8){
                                this.conSpeed = (temInfo_1[0] + temInfo_1[1] * 256) / 100;
                                this.conCurrent = (temInfo_1[2] + temInfo_1[3] * 256) / 100;
                                this.conVoltage = (temInfo_1[4] + temInfo_1[5] * 256) / 100;
                                this.conTemperature = temInfo_1[6] - 40;
                                if(temInfo_1[7] === 255){
                                    this.motorTemperature = "--";
                                }else{
                                    this.motorTemperature = temInfo_1[7] - 40;
                                }
                            }
                        }
                        if (res.data.code === '32' && res.data.subCode === '02') {
                            let temInfo_2 = Buffer.from(res.data.data, 'hex');
                            if(temInfo_2.length >= 2){
                                let temInfo_2_value = Controller.port.addZero(temInfo_2[0].toString(2), 8);
                                this.assistance = temInfo_2_value.substr(7, 1) === '0' ? this.lanTrans[0] : this.lanTrans[1];
                            }
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
                        if (res.cpd && !this.writeStatus){
                            this.speedLimitRead = res.cpd._3203_ct_sl;
                            this.wheelDiameters.find((item) => {
                                if (item.value == res.cpd._3203_ct_wd) {
                                    this.wheelDiameterRead = item.label;
                                }
                            });
                            this.circumferenceRead = res.cpd._3203_ct_cf;
                        }
                        if (res.data.code === '32' && res.data.subCode === '05') {
                            let temInfo_5 = Buffer.from(res.data.data, 'hex');
                            if(temInfo_5.length >= 2){
                                this.calorie = temInfo_5[0] + temInfo_5[1] * 256;
                            }
                        }
                    });
                }
            },
            usbErrorCheck(err){
                Controller.port.checkUsbError(err);
            },
            writeSn() {
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
                if (!this.sn) {
                    this.$message({
                            message: this.$t('cNam.snEmptyTip'),
                            type: "warning"
                        }
                    );
                    return;
                }
                this.sn = this.sn.replace(/\s/g, "");
                if(this.sn.length > 40){
                    this.$message({
                            message: this.$t('cNam.snMaxLength'),
                            type: "warning"
                        }
                    );
                }
//                if (this.isDealerOrAssembler) {
//                    Core.Api.Tool.authDetail().then(res => {
//                        this.auth = res.auth;
//                        if (res.auth == null || !res.auth.auth_controller) {
//                            Core.Api.Org.getAllPartnerBranList().then(res => {
//                                this.brandList = res.org_list;
//                                if (this.brandList.length > 0) {
//                                    this.brandId = this.brandList[0].id;
//                                }
//                                this.dialogWriteAuthVisible = true;
//                            });
//                        } else {
//                            this.writeSnWithAuth();
//                        }
//                    });
//                } else {
                this.writeSnWithAuth();
//                }
            },

            writeSnWithAuth(){
//                this.writeStatus = true;
                this.connect();
                if(!this.isCan || Controller.port.checkUsbUart()){
                    Controller.writeSn(this.sn);
                    Controller.getBus().subscribe(res => {
//                    console.log('Controller-vm3:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (res.data.ctrl && 'sn' in res.data.ctrl) {
                            let sn = res.data.ctrl.sn;
                            if (sn === this.sn || sn === ' ') {
                                this.sn = '';
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
                    Controller.port.usbSDK.machineInformation.update.serialNumber(0x05, 0x02, Controller.port.Core.Util.stringToHex(this.sn))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.sn = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Controller.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x02)
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
//                            this.writeStatus = false;
                    });
                }

            },

//            writeParamWithAuth() {
////                if (this.isDealerOrAssembler) {
////                    Core.Api.Tool.authDetail().then(res => {
////                        this.auth = res.auth;
////                        if (res.auth == null || !res.auth.auth_controller) {
////                            Core.Api.Org.getAllPartnerBranList().then(res => {
////                                this.brandList = res.org_list;
////                                if (this.brandList.length > 0) {
////                                    this.brandId = this.brandList[0].id;
////                                }
////                                this.dialogWriteAuthVisible = true;
////                            });
////                        } else {
////                            this.writeParam();
////                        }
////                    });
////                } else {
//                    this.writeParam();
////                }
//            },
//
            writeParam() {
//                this.$message({
//                    message: 'not support yet',
//                    type: "warning"
//                });
                Controller.writeParam(this.limitVolt, this.limitCur, this.curList, this.spdList, this.wheelDiameterRead, this.spdMeterType, this.spdMeterSignalNum);
            },
            clickCopy(){
                this.modelWrite = this.model;
                this.hardwareWrite = this.hardVersion;
            },
            clickWriteReset(){
                if (this.modelWrite && this.modelWrite.indexOf('CR')>-1) {
                    Controller.port.usbSDK.machineInformation.update.modelNumber(0x05, 0x02, Controller.port.Core.Util.stringToHex(this.modelWrite.trim()))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.modelWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Controller.port.usbSDK.machineInformation.get.modelNumber(0x05, 0x02)
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
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeModelTip'),
                        type: 'warning'
                    });
                }
                if (this.hardwareWrite && this.hardwareWrite.indexOf('CR') > -1) {
                    Controller.port.usbSDK.machineInformation.update.hardwareVersionNumber(0x05, 0x02, Controller.port.Core.Util.stringToHex(this.hardwareWrite.trim()))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.hardwareWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                Controller.port.usbSDK.machineInformation.get.hardwareVersionNumber(0x05, 0x02)
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
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeHardwareTip'),
                        type: 'warning'
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
                if(parseInt(this.speedLimit) > 60 && parseInt(this.speedLimit) < 10 ){
                    this.$message({
                        message: "SpeedLimit should between 10-60 Km/h",
                        type: "warning",
                        duration: 0,
                        showClose: true
                    });
                    return;
                }

                if(this.userName.indexOf("fqc") < 0 && parseInt(this.speedLimit) > 25) {
                    this.$message({
                        message: this.$t('cNam.speedLimitNote'),
                        type: "warning",
                        duration: 0,
                        showClose: true
                    });
                }

                // 判断sn //
                if( Core.Data.getOrg().type != Core.Const.ORG_TYPE.COMPONENT ){
                    if (!this.sn || this.sn !== this.readSn) {
                        this.$message.warning(this.$t('cNam.needReadSn'));
                        return;
                    }
                }

                // 经销商和组装厂不能写比当前高的数据
                if ( (this.org.type === Core.Const.ORG_TYPE.DEALER || this.org.type === Core.Const.ORG_TYPE.OEM || this.org.type === Core.Const.ORG_TYPE.SPECIAL_DEAlER) ){
                    if( this.speedLimitRead == ''){
                        this.$message.warning( this.$t('cNam.speedLimitEmpty') );
                        return;
                    }else if( this.speedLimitRead <= 25 ){
                        if(this.speedLimit > 25){
                            this.$message.warning(this.$t('cNam.offlineNo'));
                            return;
                        }
                    }else if( this.speedLimitRead > 25){
                        if( this.speedLimitRead < this.speedLimit){
                            this.$message.warning(this.$t('cNam.offlineNo'));
                            return;
                        }
                    }
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

                let data = {
                    old: {
                        "Wheel Diameter": this.wheelDiameterRead,
                        "Speed Limit": this.speedLimitRead,
                        "Circumference": this.circumferenceRead
                    },
                    new: {
                        "Wheel Diameter": wheelDiameterParse,
                        "Speed Limit": this.speedLimit,
                        "Circumference": this.circumference
                    },
                };
                if (this.offLineFlag || !navigator.onLine) {
                    // 如果离线


                    // 如果上一次的数据在被处理，需要等待
                    let isHandle = Core.Data.getIsBatchRecordIng();
                    if (isHandle) {
                        this.$message.warning(this.$t('cNam.offlineHandle'));
                        return;
                    }
                    this.getOfflineRecordList();
                    // 离线的到了 30 条 需要联网
                    if (this.offlineRecordList.length >= 50) {
                        this.$message.warning(this.$t('cNam.offlineCon'));
                        return;
                    }
                    this.writeSpeed(data);
                } else {
                    this.writeSpeed(data);
                }

            },

            writeSpeed(data) {

                if(this.wheelDiameter === "" && this.wheelDiameterRead !== "" ){
                    this.wheelDiameter = this.wheelDiameterRead;
                }else if(!this.wheelDiameter){
                    this.$message.warning('Please select wheel diameter');
                    return ;
                }
                if(this.circumference === "" && this.circumferenceRead !== "" ){
                    this.circumference = this.circumferenceRead;
                }else if(!this.circumference){
                    this.$message.warning('Please select circumference');
                    return ;
                }

                let spd = this.speedLimit*100;
                let cir = parseInt(this.circumference);
                let spdStr =  Buffer.from( [(spd>>8)&0xff, spd&0xff]).reverse().toString('hex');
                let cirBuf =  Buffer.from( [(cir>>8)&0xff, cir&0xff]).reverse().toString('hex');
//                Port.writeByQueueAndWait(makeFrameCanWrite('00','3203', spdStr + wheel +cirBuf), 100);
//                console.log(spdStr + this.wheelDiameter + cirBuf);
                Controller.port.usbSDK.publicInformation.update.controllerInformation_3(0x05, 0x02, spdStr + this.wheelDiameter + cirBuf)
                    .then(res => {
//                        console.log(res);
//                        can:"NORMAL_ACK"canCode:2code:"32"data:""dataPacks:{}dataParsed:""source:"CTL"sourceCode:2subCode:"03"target:"BST"targetCode:5
                        if (res.can == "NORMAL_ACK" && res.targetCode == 5){
                            this.speedLimitRead = '';
                            this.wheelDiameterRead = '';
                            this.circumferenceRead = '';
                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                            if( Core.Data.getOrg().type != Core.Const.ORG_TYPE.COMPONENT ){
                                this.saveRecord(data);
                            }
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
            readErrorCode() {
                this.errorCode = '';
                Controller.port.usbSDK.machineInformation.get.errorCode(0x05,0x02)
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

            saveRecord(data) {
                if(!this.sn) {
                    this.$message.warning(this.$t('cNam.needReadSn'));
                    return false;
                }
                let userId = this.userId;
//                console.log(userId);
                let sn = this.sn;
                let dataJson = JSON.stringify(data);
                let time = Core.Util.time();
                let type = this.recordType;
                this.sn = '';
                if(this.userName.indexOf("fqc") < 0){
                    if (!this.offLineFlag && navigator.onLine) {
                        Core.Api.ObjectRecord.save(sn, dataJson, time, type);
                    } else {
                        let recordData = {
                            userId: userId,
                            sn: sn,
                            data: dataJson,
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

            //判断是否一个数据都没有
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
                this.spdMeterType = '';
                this.circumferenceRead = '';
                this.wheelDiameterRead = '';
                this.speedLimitRead = '';
                this.manNameRead = '';
                this.nmlVolt = '';
                this.maxCur = '';
                this.spdMeterSignalNum = '';
                this.limitVolt = '';
                this.curList = '';
                this.spdList = '';
                this.limitCur = '';

                this.conCapacityLeft = '';
                this.conSingleTrip = '';
                this.conCadence = '';
                this.conTorque = '';
                this.conSingleTripLeft = '';
                this.conSpeed = '';
                this.conCurrent = '';
                this.conVoltage = '';
                this.conTemperature = '';
                this.motorTemperature = '';
                this.assistance = '';
                this.calorie = '';

            },
            reset() {
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                this.clear();
                Controller.port.reset(100);
                this.isConnected = false;
//                setTimeout(()=>{
//                    this.isConnected = Controller.port.isConnected();
//                },500);
                if(!this.usbHID){
                    Controller.getBus().subscribe(res => {
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
            },

            //申请权限
            applyAuth(type){
                if (!this.brandId) {
                    this.$message({
                        message: this.$t('brandEmptyTip'),
                        type: 'warning'
                    });
                    return
                }
                Core.Api.Tool.applyAuth(this.brandId, type, 1).then(res => {
                    if (this.auth == null) {
                        this.$message({
                            message: this.$t('applySuccess'),
                            type: 'success'
                        });
                    } else {
                        this.$message({
                            message: this.$t('getApplySuccess'),
                            type: 'success'
                        });
                    }
                    this.dialogWriteAuthVisible = false;
                })
            }
        },
        beforeDestroy:function () {
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalCon){
                clearInterval(this.connectionIntervalCon);
            }
            Controller.port.setBoxOperation();
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
                            let array = vm.sn.split('DP');
                            if (array.length > 1) {
                                vm.sn = ('DP' + array[array.length - 1]).replace(/\s+/g, "");
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
                    Controller.port.setConnection();
                    Controller.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count>1 && Controller.port.isConnected()){
                                vm.connect();
                            }
                            if(count>3 && Controller.port.isConnected()){
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
                                if(count>3 && Controller.port.isConnected()){
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
    .controller {
        width: 100%;
        height: 100%;
        overflow: auto;
        padding: 44px 0;
        .btn-back {
            margin-left: 40px;
            top: -26px;
            position: relative;
        }
        .controller-content {
            width: 950px;
            margin: 0 auto;
            .connect-tip {
                display: inline-block;
                float: right;
                a {
                    color: #E26829;
                }
                .tip-img {
                    top: 0;
                    right: 20px;
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
                /*min-height: 350px;*/
                margin-top: 22px;
                -moz-box-shadow: 0 1px 0 #2B2F3B; /* 老的 Firefox */
                box-shadow: 0 1px 0 #2B2F3B;
                .part {
                    float: left;
                    margin-top: 9px;
                    width: 473px;
                    border-right: 1px dotted #2B2F3B;
                    .item {
                        overflow: hidden;
                        margin-left: 33px;
                        height: 50px;
                        line-height: 50px;
                        .label {
                            width: 110px;
                            display: inline-block;
                            line-height: 20px;
                            word-wrap: break-word;
                            vertical-align: middle;
                        }
                        .input {
                            width: 295px;
                        }
                        .input-sn:after {
                            top: 15px;
                        }
                    }
                    .com {
                        height: 50px;
                        margin-left: 33px;
                        line-height: 50px;
                        display: flex;
                        align-items: center;
                        padding-right: 28px;
                        .label {
                            width: 110px;
                            display: inline-block;
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
                        .com-select {
                            flex: 1;
                            margin-left: 10px;
                        }
                    }
                    /*.sn {*/
                        /*width: 440px;*/
                        /*margin-left: 33px;*/
                        /*height: 60px;*/
                        /*line-height: 60px;*/
                        /*.label {*/
                            /*width: 110px;*/
                            /*display: inline-block;*/
                        /*}*/
                        /*.sn-input {*/
                            /*width: 295px;*/
                        /*}*/
                        /*.btn-write {*/
                            /*margin-left: 4px;*/
                        /*}*/
                    /*}*/
                }
                .btn-row {
                    width: 100%;
                    position: relative;
                    height: 30px;
                    line-height: 30px;
                    .btn-group {
                        position: absolute;
                        right: 0;
                    }
                    .loading {
                        display: inline-block;
                        vertical-align: middle;
                        margin-top: -13px;
                    }
                }
                .right {
                    border-right: none;
                }
                .con-Params{
                    margin:10px 20px;
                    .input {
                        width: 295px;
                    }
                }
                .table-wrap{
                    .long-td{
                        max-width: 200px;
                        text-overflow: ellipsis;
                        overflow: hidden;
                        text-align: left;
                    }
                    .short-td{
                        max-width: 100px;
                        text-overflow: ellipsis;
                        overflow: hidden;
                        text-align: left;
                    }
                }
                .note-title {
                    margin-top: 10px;
                }

            }
            .can-data {
                margin-top: 1px;
            }
            .data {
                width: 950px;
                height: 100%;
                background: #353945;
                margin-top: 1px;
                .part {
                    display: inline-block;
                    margin-top: 9px;
                    width: 440px;
                    margin-left: 32px;
                    border-right: 1px dotted #2B2F3B;
                    .head {
                        height: 60px;
                        line-height: 60px;
                        .label {
                            display: inline-block;
                            width: 110px;
                            line-height: 20px;
                            vertical-align: middle;
                        }
                        .input {
                            margin-left: 5px;
                            width: 296px;
                            position: relative;
                        }
                    }
                    .text {
                        margin-left: 112px;
                        height: 40px;
                        line-height: 40px;
                        .label {
                            color: #A1A5B1;
                            width: 148px;
                            line-height: 20px;
                            word-wrap: break-word;
                            display: inline-block;
                            vertical-align: middle;
                            text-align: center;
                        }
                    }
                    .content {
                        .item {
                            height: 50px;
                            line-height: 50px;
                            .label {
                                display: inline-block;
                                width: 110px;
                                word-wrap: break-word;
                                line-height: 20px;
                                vertical-align: middle;
                            }
                            .input-left {
                                margin-left: 5px;
                                width: 148px;
                                & > input {
                                    border-radius: 20px 0 0 20px;
                                }
                            }
                            .input-right {
                                width: 148px;
                                & > input {
                                    border-radius: 0 20px 20px 0;
                                }
                            }
                        }
                    }
                }
                .right {
                    border-right: none;
                }
            }

            .btn-group {
                float: right;
                height: 30px;
                line-height: 30px;
                margin-right: 30px;
            }
            .el-collapse-item{
                border-bottom: 1px solid #353945;
            }
        }
    }
</style>



// WEBPACK FOOTER //
// controller.vue?479936f5