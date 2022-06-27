<template>
    <div class="result-container">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()"></div>
                </div>
            </div>
            <span class="title">{{ $t('resultTitle') }}</span>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item print" @click="onClickPrint" :title=" $t('print')"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                    <div class="bicycle-type mb-10">
                        <label class="mr-15">
                            {{ $t('resultBike') }}
                        </label>
                        <el-input v-model="vehicleModel" disabled
                                  class="b-type-input inline-block w-200"></el-input>
                    </div>
                    <table class="component-list" border="0" cellspacing="0" cellpadding="0">
                        <thead class="t-head">
                            <tr>
                                <td v-for="item in listHead"> {{item}}</td>
                            </tr>
                        </thead>
                        <tbody class="t-body">
                            <tr  v-for="item in tableData">
                                <td>{{item.name}}</td>
                                <td>{{item.model}}</td>
                                <td>{{item.sn}}</td>
                                <td>{{item.version}}</td>
                                <td>{{item.parameterVersion}}</td>
                                <td class="btn-underline" @click="showDetail(item.key)" v-if = "item.show">{{ $t('detail') }}</td>
                                <td v-else>-</td>
                            </tr>
                        </tbody>
                    </table>
                    <div class="table-bottom mb-10">
                        <div>
                            <label class="mr-15">
                                {{ $t('cNam.motor') }}
                            </label>
                            <el-input class="scan-input inline-block" placeholder="SN" v-model="motorSn"></el-input>
                        </div>
                        <div>
                            <label class="mr-15">
                                {{ $t('bikeId') }}
                            </label>
                            <el-input class="scan-input inline-block" placeholder="SN" v-model="vehicleSn"></el-input>
                        </div>
                        <div>
                            <label class="mr-15">
                                {{ $t('key') }}
                            </label>
                            <el-input class="scan-input inline-block" placeholder="SN" v-model="vehicleKey"></el-input>
                        </div>

                    </div>
                    <div class="result-holder">
                        <div class="result-top">
                            {{ $t('result') }}
                        </div>
                        <div class="result-content">
                            <template v-for="item in listStatus">
                                <div>
                                    <span>{{item.name}}</span>
                                    <span class="result-span ok" v-if="item.okOrFail==1">{{ $t('ok') }}</span>
                                    <span class="result-span fail" v-if="item.okOrFail==2">{{ $t('fail') }}</span>
                                    <span class="result-span none" v-if="item.okOrFail==0">--</span>
                                </div>
                            </template>
                        </div>
                        <div class="result-desc">
                            <span>{{ $t('defects') }}</span>
                            <div v-for="d in defect">
                                <span class="defect-span">{{d}}</span>
                            </div>
                        </div>
                        <div class="result-desc">
                            <div class="title-wrap">
                                <span>{{ $t('defectDes') }}</span>
                            </div>
                            <div class="title-wrap">
                                <p>{{desc}}</p>
                            </div>
                        </div>
                        <div class="result-desc">
                            <div class="title-wrap">
                                <span>{{ $t('image') }}</span>
                            </div>
                            <div v-for="(item, index) in fileList.img" class="img-holder">
                                <img :src="linkPre + item" class="img-content"/>
                            </div>
                        </div>
                        <div class="result-desc">
                            <div class="title-wrap">
                                <span>{{ $t('video') }}</span>
                            </div>
                            <div v-for="(item, index) in fileList.video" class="video-holder">
                                <video :src="linkPre + item" controls="controls" class="videoItem"></video>
                            </div>
                        </div>
                    </div>
                    <div class="clear-20"></div>
                    <span class="save-report fr" v-if="!offLineFlag" @click="saveReport">{{ $t('saveReport') }}</span>
                    <span class="fr" v-if="offLineFlag">{{ $t('offLineTip') }}</span>
                </div>
        </div>

        <el-dialog
                title="Detail"
                :visible.sync="dialogVisible"
                class="diagnosis-detail-warp"
                custom-class="diagnosis-detail">
            <div calss="detail-content">
                <div class="info">
                    <template v-if="selectItem === 'ctrl'">
                        <div class="info-left">
                            <div class="item">
                                <label class="label">SN</label>
                                <el-input class="input" v-model="controllerInfo.sn"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.model') }}</label>
                                <el-input class="input" v-model="controllerInfo.model"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                <el-input class="input" v-model="controllerInfo.version"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                <el-input class="input" v-model="controllerInfo.hardVersion"></el-input>
                            </div>
                        </div>
                        <div class="info-right">
                            <div class="item" v-show= "!isCan">
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
                            <div class="item" v-show= "isCan">
                                <label class="label">{{ $t('cNam.speedLimit') }}</label>
                                <el-input class="input" v-model="controllerInfo.speedLimit">
                                    <template slot="append">km/h</template>
                                </el-input>
                            </div>
                            <div class="item" v-show= "isCan">
                                <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                                <el-input class="input" v-model="controllerInfo.wheelDiameter"></el-input>
                            </div>
                            <div class="item" v-show= "isCan">
                                <label class="label">{{ $t('cNam.circumference') }}</label>
                                <el-input class="input" v-model="controllerInfo.circumference">
                                    <template slot="append">mm</template>
                                </el-input>
                            </div>
                        </div>
                    </template>
                    <template v-if="selectItem === 'hmi'">
                        <div class="info-left">
                            <div class="item">
                                <label class="label">SN</label>
                                <el-input class="input" v-model="hmiInfo.sn"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.model') }}</label>
                                <el-input class="input" v-model="hmiInfo.model"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                <el-input class="input" v-model="hmiInfo.version"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                <el-input class="input" v-model="hmiInfo.hardVersion"></el-input>
                            </div>
                        </div>
                        <div class="info-right">
                            <div class="item">
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
                            </div>
                            <!--<div class="item">-->
                                <!--<label class="label">{{ $t('cNam.totalGear') }}</label>-->
                                <!--<el-input class="input" v-model="hmiInfo.totalGear"></el-input>-->
                            <!--</div>-->
                        </div>
                    </template>
                    <template v-if="selectItem === 'pedal'">
                        <div class="info-left">
                            <div class="item">
                                <label class="label">SN</label>
                                <el-input class="input" v-model="pedalSensorInfo.sn"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.model') }}</label>
                                <el-input class="input" v-model="pedalSensorInfo.name"></el-input>
                            </div>
                        </div>
                        <div class="info-right">
                            <div class="item">
                                <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                <el-input class="input" v-model="pedalSensorInfo.version"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                <el-input class="input" v-model="pedalSensorInfo.hardVersion"></el-input>
                            </div>
                        </div>
                    </template>
                    <template v-if="selectItem === 'motor'">
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
                    <template v-if="selectItem === 'bat'">
                        <div class="info-left">
                            <div class="item">
                                <label class="label">SN</label>
                                <el-input class="input" v-model="batteryInfo.sn"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                                <el-input class="input" v-model="batteryInfo.version"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.ASOC') }}</label>
                                <el-input class="input" v-model="batteryInfo.soh">
                                    <template slot="append">%</template>
                                </el-input>
                            </div>
                            <div class="item">
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
                            </div>
                        </div>
                        <div class="info-right">
                            <div class="item">
                                <label class="label">{{ $t('cNam.model') }}</label>
                                <el-input class="input" v-model="batteryInfo.model"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">{{ $t('cNam.hardwareVersion') }}</label>
                                <el-input class="input" v-model="batteryInfo.hardVersion"></el-input>
                            </div>
                            <div class="item">
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
                            </div>
                        </div>
                    </template>
                </div>
            </div>
        </el-dialog>

        <!--打印报告-->
        <div class="print-content-wrap">
            <div class="content">
                <h4>E-BIKE DIAGNOSIS REPORT</h4>
                <el-collapse v-model="printActiveNames">
                    <el-collapse-item title="Store Info" name="1">
                        <ul class="list-group-customer">
                            <li class="list-group-item-50">
                                <div class="label-title">Store Name</div>
                                <div class="content">{{ org.name || '-' }}</div>
                            </li>
                            <li class="list-group-item-50">
                                <div class="label-title">Store Email</div>
                                <div class="content">{{ org.email || '-' }}</div>
                            </li>
                        </ul>
                        <ul class="list-group-customer">
                            <li class="list-group-item-50">
                                <div class="label-title">Store Tel.</div>
                                <div class="content">{{ org.phone || '-' }}</div>
                            </li>
                        </ul>
                        <ul class="list-group-customer">
                            <li class="list-group-item-100">
                                <div class="label-title">Store Address</div>
                                <div class="content">{{ org.address + org.country|| '-' }}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Bike Info" name="2">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">Bike ID</div>
                                <div class="content">{{ vehicleSn || '-' }}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis HMI" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">{{hmiInfo ? hmiInfo.sn + hmiInfo.tip : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{hmiInfo ? hmiInfo.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{hmiInfo ? hmiInfo.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{hmiInfo ? hmiInfo.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.totalMileage') }}(km)</div>
                                <div class="content">{{hmiInfo ? hmiInfo.totalMileage : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show = '!isCan'>
                                <div class="label-title">{{ $t('cNam.wheelDiameter') }}</div>
                                <div class="content">{{hmiInfo ? hmiInfo.wheelDiameter : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show = '!isCan'>
                                <div class="label-title">{{ $t('cNam.speedLimit') }}(km/h)</div>
                                <div class="content">{{hmiInfo ? hmiInfo.speedLimit : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis Controller" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">{{controllerInfo ? controllerInfo.sn + controllerInfo.tip : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{controllerInfo ? controllerInfo.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{controllerInfo ? controllerInfo.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{controllerInfo ? controllerInfo.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show= "!isCan">
                                <div class="label-title">{{ $t('cNam.nmlVolt') }}(V)</div>
                                <div class="content">{{controllerInfo ? controllerInfo.nmlVolt : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show= "!isCan">
                                <div class="label-title">{{ $t('cNam.maxCur') }}(A)</div>
                                <div class="content">{{controllerInfo ? controllerInfo.maxCurrent : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show= "isCan">
                                <div class="label-title">{{ $t('cNam.speedLimit') }}(km/h)</div>
                                <div class="content">{{controllerInfo ? controllerInfo.speedLimit : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show= "isCan">
                                <div class="label-title">{{ $t('cNam.wheelDiameter') }}</div>
                                <div class="content">{{controllerInfo ? controllerInfo.wheelDiameter : "-"}}</div>
                            </li>
                            <li class="list-group-item" v-show= "isCan">
                                <div class="label-title">{{ $t('cNam.circumference') }}(mm)</div>
                                <div class="content">{{controllerInfo ? controllerInfo.circumference : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis Battery" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">{{batteryInfo ? batteryInfo.sn + batteryInfo.tip : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{batteryInfo ? batteryInfo.model : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{batteryInfo ? batteryInfo.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{batteryInfo ? batteryInfo.hardVersion : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.ASOC') }}(%)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.soh : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.RSOC') }}(%)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.soc : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.packVoltage') }}(mV)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.packVoltage : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.capacityLeft') }}(mAh)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.leftCapacity : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.fullChargeCapacity') }}(mAh)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.fullChargerCapacity : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.temperature') }}(℃)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.temperature : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.current') }}(mA)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.current : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cell1_7') }}(mV)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.cell1to7 : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cell8_14') }}(mV)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.cell8to14 : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.cycleLife') }}</div>
                                <div class="content">{{batteryInfo ? batteryInfo.loopTime : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.currentChargeInterval') }}(day)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.lastCharge : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.maxChargeInterval') }}(day)</div>
                                <div class="content">{{batteryInfo ? batteryInfo.maxChargeInterval : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis Motor" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">{{motorInfo ? motorInfo.sn : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{motorInfo ? motorInfo.name : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.power') }}(W)</div>
                                <div class="content">{{motorInfo? motorInfo.power : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.wheelSize') }}</div>
                                <div class="content">{{motorInfo ? motorInfo.wheelSize : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.voltage') }}(V)</div>
                                <div class="content">{{motorInfo ? motorInfo.voltage : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.rpm') }}</div>
                                <div class="content">{{motorInfo ? motorInfo.rpm : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis Pedal Sensor" name="3">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">SN</div>
                                <div class="content">{{pedalSensorInfo ? pedalSensorInfo.sn : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.model') }}</div>
                                <div class="content">{{pedalSensorInfo ? pedalSensorInfo.name : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.softwareVersion') }}</div>
                                <div class="content">{{pedalSensorInfo ? pedalSensorInfo.version : "-"}}</div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hardwareVersion') }}</div>
                                <div class="content">{{pedalSensorInfo ? pedalSensorInfo.hardVersion : "-"}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                    <el-collapse-item title="Diagnosis Result" name="4">
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.hmi') }}</div>
                                <div class="content">
                                    <span>{{ result.hmi }}</span>
                                    <span class="split" v-if="hmiInfo.error">|</span>
                                    <span class="error">{{ hmiInfo.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.controller') }}</div>
                                <div class="content">
                                    <span>{{ result.controller }}</span>
                                    <span class="split" v-if="controllerInfo.error">|</span>
                                    <span class="error">{{ controllerInfo.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.battery') }}</div>
                                <div class="content">
                                    <span>{{ result.battery }}</span>
                                    <span class="split" v-if="batteryInfo.error">|</span>
                                    <span class="error">{{ batteryInfo.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.motor') }}</div>
                                <div class="content">
                                    <span>{{ result.motor }}</span>
                                    <span class="split" v-if="motorInfo.error">|</span>
                                    <span class="error">{{ motorInfo.error }}</span>
                                </div>
                            </li>
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('cNam.sensor') }}</div>
                                <div class="content">
                                    <span>{{ result.pedalSensor}}</span>
                                    <span class="split" v-if="pedalSensorInfo.error">|</span>
                                    <span class="error">{{ pedalSensorInfo.error }}</span>
                                </div>
                            </li>
                        </ul>
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('defects') }}</div>
                            </li>
                            <li v-for="d in defect" class="list-group-item">
                                <span class="defect-span">{{d}}</span>
                            </li>
                        </ul>
                        <ul class="list-group">
                            <li class="list-group-item">
                                <div class="label-title">{{ $t('defectDes') }}</div>
                                <div class="content">{{desc}}</div>
                            </li>
                        </ul>
                    </el-collapse-item>
                </el-collapse>
            </div>
        </div>

        <!--&lt;!&ndash;额外sn弹窗&ndash;&gt;-->
        <!--<el-dialog :title="$t('tip')" :visible.sync="extDialogVisible">-->
            <!--The following component SN is not exist in BESST. Please check more. <br />-->
            <!--If everything is all right, you can still bind the SN to bike by clicking "Save".-->
            <!--<div class="clear-10"></div>-->
            <!--<div v-for="item in extSnList">-->
                <!--<span v-if="item.type === Const.CATEGORY_ROOT_TYPE.HMI" class="width-80">HMI</span>-->
                <!--<span v-if="item.type === Const.CATEGORY_ROOT_TYPE.MOTOR" class="width-80">Motor</span>-->
                <!--<span v-if="item.type === Const.CATEGORY_ROOT_TYPE.CONTROLLER" class="width-80">Controller</span>-->

                <!--<span class="width-280">SN: {{item.sn}}</span>-->
                <!--&lt;!&ndash;<el-button type="primary" @click="saveSimpleItem(item)">Save</el-button>&ndash;&gt;-->
                <!--<div class="clear-20"></div>-->
            <!--</div>-->
            <!--<span slot="footer" class="dialog-footer">-->
                <!--<el-button type="preview" @click="extDialogVisible = false">{{ $t('confirm') }}</el-button>-->
            <!--</span>-->
        <!--</el-dialog>-->
    </div>

</template>

<script >
    import Core from "core/core";
    import printArea from "printArea";
    import NL from "src/assets/lang/nl";
    import EN from "src/assets/lang/en";
    import DE from "src/assets/lang/de";
    import ZH from "src/assets/lang/zh";
    import itemDetail from "./item-detail";

    export default {
        components: {
            itemDetail: itemDetail
        },
        data() {
            return {
                dialogVisible: false,
                vehicle: '',
                vehicleSn: '',
                vehicleModel: "",
                vehicleKey: "",
                listHead: [
                    "Component",
                    "Model",
                    "Series Number",
                    "Software Version",
                    "Parameter Version",
                    "Operation "
                ],
                tableData: [
                    {
                        name: 'HMI',
                        key: 'hmi',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: true,
                    },
                    {
                        name: 'Controller',
                        key: 'ctrl',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: true,
                    },
                    {
                        name: 'Battery',
                        key: 'bat',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: true,
                    },
                    {
                        name: 'Brake',
                        key: 'brake',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: false,
                    },
                    {
                        name: 'Throttle',
                        key: 'throttle',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: false,
                    },
                    {
                        name: 'Light',
                        key: 'light',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: false,
                    },
                    {
                        name: 'Speed Sensor',
                        key: 'speed',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: false,
                    },
                    {
                        name: 'Pedal Sensor',
                        key: 'pedal',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: true,
                    },
                    {
                        name: 'Motor',
                        key: 'motor',
                        model: '',
                        sn: '',
                        version: '',
                        parameterVersion: '',
                        show: true,
                    }
                ],
                listStatus: [],
                selectItem: '', //  ctrl, hmi, bat, motor pedal
                //读取到的
                hmiInfo: {},
                controllerInfo: {},
                batteryInfo: {},
                //数据库
                motorInfo: {},
                pedalSensorInfo: {},
                rpmSensorInfo: {},

                //表格展示  数据库的车辆数据
                hmiData: {},
                batteryData: {},
                controllerData: {},

                //机械诊断
                defect:[],
                selectedDefectList: {},
                file: [],

                existArr: [],

                report: '',
                reportJson: '',
                linkPre: "http://static.besst.bafang-service.com/besst/file/",

                motorSn: '', //扫描枪输入sn

                // 打印报告
                printActiveNames: ['1', '2', '3', '4'],
                org: '',
                result: '',

                //生产页面过来带的数据
                from: '',
                orderId: '',
                itemId: '',
                isBindBat: false,

                //是否离线
                offLineFlag: false,

                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',
                isCan: false,

                // 不存在于数据库的sn
                extSnList: [],
                extDialogVisible: false,

                //  图片视频
                fileList: {
                    video:[],
                    img:[],
                },
                //  问题描述
                desc:'',
                defectOptionList: Core.Const.OptionList,

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
        mounted: function () {

            this.offLineFlag = Core.Data.getOffline();
            // 国际化
            this.org = Core.Data.getOrg();

            this.initDefectOptionList();

            this.report = this.$route.query['report'];
            if(this.$route.query['defect']){
//                console.log("query",this.$route.query['defect']);
                let receiveDefect = this.$route.query['defect'];
                let keys = Object.keys(receiveDefect);

                this.selectedDefectList = {
                    'hmi':[],
                    'battery':[],
                    'bike':[],
                    'charger':[],
                    'motor':[],
                    'other':[],
                    'sensor':[],
                };

                for(let i=0; i<keys.length; i++){
                    for(let j=0; j<receiveDefect[keys[i]].length; j++){
                        this.defect.push(keys[i]+':'+ receiveDefect[keys[i]][j]);
                        this.defectOptionList.forEach((item)=>{
                            if(item.type == keys[i]){
                                item.data.forEach((data)=>{
                                        if(data.label == receiveDefect[keys[i]][j] ){
                                            this.selectedDefectList[keys[i]].push( data.name );
                                        }
                                    }
                                )
                            }
                        });
                    }
                }
//                console.log("selectedDefectList", this.selectedDefectList, JSON.stringify(this.selectedDefectList));
            }
            if(this.$route.query['file']){
//                console.log("query",this.$route.query['file'], !(this.$route.query['file']) );
                this.fileList = this.$route.query['file'];
            }
            if(this.$route.query['remark']){
                this.desc = this.$route.query['remark'];
            }

//            console.log(this.fileList);
//            this.fileList = {
//                video:['6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4'],
//                img:['dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7.png']
//            };
            this.reportJson = JSON.stringify(this.report);
//            console.log(this.report, 'result page');
            this.processReport(this.report);

            this.from = this.$route.query['from'];
            if (this.from === 'produce') {
                this.orderId = this.$route.query['orderId'];
                this.itemId = this.$route.query['itemId'];
                this.isBindBat = this.$route.query['isBindBat'];
            }

            this.listHead = [
                this.$t('cNam.component'),
                this.$t('cNam.model'),
                this.$t('cNam.sn'),
                this.$t('cNam.softwareVersion'),
                this.$t('cNam.paramsName'),
                this.$t('cNam.txtTitle1')
            ];
            this.tableData[0].name = this.$t('cNam.hmi');
            this.tableData[1].name = this.$t('cNam.controller');
            this.tableData[2].name = this.$t('cNam.battery');
            this.tableData[3].name = this.$t('brake');
            this.tableData[4].name = this.$t('throttle');
            this.tableData[5].name = this.$t('light');
            this.tableData[6].name = this.$t('speed');
            this.tableData[7].name = this.$t('pedalSensor');
            this.tableData[8].name = this.$t('motor');

        },
        watch: {
            motorSn: function (newValue, oldValue) {
                let vm = this;

                if(vm.offLineFlag){
                    vm.motorSn = newValue.replace(/\s+/g, "");
                    return;
                }else{
                    if(newValue === oldValue){return};
                    if(typeof oldValue !== 'undefined'){
                        let time = new Date().getTime();
                        vm.isScanner =  time - vm.lastInputTime <= 100;//
                        vm.lastInputTime = time;
                    }else{
                        vm.lastInputTime = new Date().getTime();
                    }
                    if(!vm.scannerTimeout){
                        vm.scannerTimeout = setTimeout(function(){
                            if(vm.isScanner){
                                vm.motorSn = vm.motorSn.replace(/\s+/g, "");
                                if(vm.motorInfo.sn ){
                                    if (vm.motorSn && (vm.motorSn !== vm.motorInfo.sn) ) {
                                        vm.$message.warning( vm.$t('motorSNTip'));
                                    }
                                    console.log("1");
                                }
                                // else {
                                //     vm.getComponentDetail(vm.motorSn);
                                //     console.log("2");
                                // }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        },1000)
                    }
                }
            },
        },
        methods: {
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
            getByKey(key, list) {
                let index = list.findIndex(item => item.key === key);
                if (index > -1) {
                    return list[index];
                } else {
                    return '';
                }
            },

            processReport(report) {
                if (!report) {
                    this.$router.back();
                    return
                }
                if( this.report.device && this.report.device.hasOwnProperty('vehicle') ){
                    this.vehicle =this.report.device.vehicle || {};
                    console.log(this.vehicle);
                }
                this.isCan = this.report.can;
                this.result = this.report.result || '';

                this.extSnList = this.report.existComList;
                this.existArr = this.report.existList;
                this.vehicleSn = this.vehicle.sn;
                this.vehicleId = this.vehicle.id;
                this.vehicleModel =  this.vehicle.itemName;
                this.vehicleKey = this.vehicle.secretKey;
                this.hmiInfo = (report.device && report.device.hmi) || {};
                this.controllerInfo = (report.device && report.device.controller) || {};
                this.batteryInfo = (report.device && report.device.battery) || {};
                this.motorInfo = (report.device && report.device.motor) || {};
                this.pedalSensorInfo = (report.device && report.device.pedalSensor) || {};

                let componentList =  report.bomList || [];
                this.getBomInfo(componentList);
                //  读取和数据获取
                this.tableData.map(item => {
                    switch (item.key) {
                        case 'ctrl':
                            item.sn = this.controllerInfo.sn;
                            item.model = this.controllerInfo.model;
                            item.version = this.controllerInfo.version;
                            break;
                        case 'hmi':
                            item.sn = this.hmiInfo.sn;
                            item.model = this.hmiInfo.model;
                            item.version = this.hmiInfo.version;
                            break;
                        case 'bat':
                            item.sn = this.batteryInfo.sn;
                            item.model = this.batteryInfo.model;
                            item.version = this.batteryInfo.version;
                            break;
                        case 'motor':
                            item.sn = this.motorInfo.sn;
                            item.model = this.motorInfo.model;
                            break;
                        case 'pedal':
                            item.sn = this.pedalSensorInfo.sn;
                            item.model = this.pedalSensorInfo.name;
                            break;
                    }
                });
                this.listStatus = report.testList;
            },

            getBomInfo(bomList) {
                let $this = this;
                bomList.forEach(function (bom, index) {
                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.HMI) {
                        $this.hmiData.sn = bom.sn;
                        $this.hmiData.model = bom.item.name;
                        let attrList = bom.item.attr_list;
                        if (attrList === null || attrList.length <= 0) {
                            return;
                        }
                        for (let index in attrList) {
                            if (attrList[index].attr_def_key === "hard_ver") {
                                $this.hmiData.hardVersion = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "soft_ver") {
                                $this.hmiData.version = attrList[index].value;
                            }
                        }
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER) {
                        $this.controllerData.sn = bom.sn;
                        $this.controllerData.model = bom.item.name;
                        let attrList = bom.item.attr_list;
                        if (attrList === null || attrList.length <= 0) {
                            return;
                        }
                        for (let index in attrList) {
                            if (attrList[index].attr_def_key === "hard_ver") {
                                $this.controllerData.hardVersion = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "soft_ver") {
                                $this.controllerData.version = attrList[index].value;
                            }
                        }
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.BATTERY) {
                        $this.batteryData.sn = bom.sn;
                        $this.batteryData.model = bom.item.name;
                        let attrList = bom.item.attr_list;
                        if (attrList === null || attrList.length <= 0) {
                            return;
                        }
                        for (let index in attrList) {
                            if (attrList[index].attr_def_key === "hard_ver") {
                                $this.batteryData.hardVersion = attrList[index].value;
                                continue;
                            }
                            if (attrList[index].attr_def_key === "soft_ver") {
                                $this.batteryData.version = attrList[index].value;
                            }
                        }
                    }

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
                });
            },

            saveReport() {
//                if (this.extSnList.length > 0) {
//                    this.extDialogVisible = true
//                    return;
//                }

                if (!this.vehicleId) {
                    this.$message.warning( "No bike traced. You could only print the report and save to local.");
                    return;
                }

                if (this.vehicle.sn){
                    if(this.vehicleSn !== this.vehicle.sn  ) {
                        this.$message.warning( this.$t('bikeSNTip2'));
                        return;
                    }
                }

                let defectRes = JSON.stringify(this.selectedDefectList);

                Core.Api.Vehicle.saveVehicleReport(0, this.vehicleId, this.reportJson, defectRes, this.desc, JSON.stringify(this.fileList) ).then(
                    res => {
                        let id = res.report_id;
                        Core.Data.set(Core.Const.DATA.KEY_REPORT, "");
                        if (this.from === 'produce') {
                            let orderId = this.orderId;
                            let itemId = this.itemId;
                            let from = 'test';
                            this.$router.push({
                                name: 'assemblerOrderProduce',
                                query: {"orderId": orderId, "itemId": itemId, "from": from, "isBindBat": this.isBindBat}
                            });
                            return
                        }
                        if (Core.Data.getOrg().type == 1) {
                            this.$router.push({name: "brandDiagnosisReport", query: {id: id, from: 'diagnosis3rdResult'}});
                        }
                        if (Core.Data.getOrg().type == 2) {
                            this.$router.push({name: "dealerDiagnosisReport", query: {id: id, from: 'diagnosis3rdResult'}});
                        }
                        if (Core.Data.getOrg().type == 3) {
                            this.$router.push({name: "assemblerDiagnosisReport", query: {id: id, from: 'diagnosis3rdResult'}});
                        }
                        if (Core.Data.getOrg().type == 4) {
                            this.$router.push({name: "componentDiagnosisReport", query: {id: id, from: 'diagnosis3rdResult'}});
                        }
                    }
                );
            },

            onClickPrint() {
                //                window.print();
                let print = "div.print-content-wrap";

                let extraCss = "";
                let mode = "iframe";
                let close = false;
                let retainAttr = ["class", "id", "style", "on"];
                let headElements =
                    '<meta charset="utf-8" />,<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';

                let options = {
                    mode: mode,
                    popClose: close,
                    extraCss: extraCss,
                    retainAttr: retainAttr,
                    extraHead: headElements
                };
                $(print).printArea(options);
            },

            showDetail(key) {
                let configList = ['hmi', 'ctrl', 'bat', 'pedal', 'motor'];
                let index = configList.indexOf(key);
                if (index > -1) {
                    this.selectItem = key;
                    this.dialogVisible = true;
                }
            },

            getComponentDetail(sn) {
                if(!sn) {return};
                Core.Api.Component.componentDetailBySn(sn).then(res => {
                    let bomlist = [res.component];
                    this.getBomInfo(bomlist);
                });
            },

//            saveSimpleItem(item) {
//                Core.Api.Component.saveSimpleComponent(item.type, item.sn).then(res => {
//                    this.blurCommonSnFunc(item.type, item.sn, 'save extra sn failed');
//                    this.extSnList.forEach((i, index) => {
//                        if (i.sn === item.sn) {
//                            this.extSnList.splice(index, 1)
//                        }
//                    });
//                    if (this.extSnList.length === 0) {
//                        this.extDialogVisible = false;
//                    }
//                })
//            },

        }
    };
</script>

<style lang="scss">
    .result-container {
        .header {
            .right {
                float: right;
                .print-btn {
                    background: #98b42c;
                    border-radius: 100px;
                }
            }
        }

        .content-wrap {
            padding: 10px 40px;
            overflow: auto;
            .content {
                width: 950px;
                display: block;
            }
            .bicycle-type {
                > label {
                    font-weight: 500;
                    vertical-align: center;
                    font-size: 16px;
                }
                .b-type-input {
                    vertical-align: center;
                }
            }
            .component-list {
                width: 950px;
                border: 2px solid #2b2f3b;
                display: table;
                .t-head {
                    width: 100%;
                    background: #2b2f3b;
                    tr {
                        td {
                            font-size: 16px;
                            padding: 8px 10px;
                            &:last-child {
                            }
                        }
                    }
                }
                .t-body {
                    tr:nth-child(2n) {
                        background: #323642;
                    }
                    tr {
                        background: #353945;
                        td {
                            font-size: 13px;
                            padding: 8px 10px;
                        }
                    }
                }
            }
            .table-bottom {
                padding: 6px 14px;
                display: flex;
                justify-content: space-between;
                > div {
                    display: flex;
                    align-items: center;
                    .scan-input {
                        flex: 1;
                        & input {
                            padding-right: 40px;
                            text-align: right;
                        }
                    }
                }
            }
            .result-holder {
                .result-top {
                    padding: 8px 10px;
                    background: #2b2f3b;
                    font-size: 16px;
                }
                .result-content {
                    display: flex;
                    flex-direction: row;
                    justify-content: space-between;
                    background: #353945;
                    padding: 6px 16px;
                    > div {
                        /*display: flex;*/
                        /*flex-direction: row;*/
                        /*align-items: flex-start;*/
                        .result-span {
                            padding: 1px 14px;
                            border-radius: 100px;
                            margin-top: 9px;
                            &.ok {
                                background: #98b42c;
                            }
                            &.fail {
                                background: #ee3c3c;
                            }
                            &.none {
                                background: #7d7d7d;
                            }
                        }
                    }
                    .img-content {
                        width: 100px;
                    }
                    .videoItem{
                        display: inline-block;
                        width:160px;
                    }
                }
                .result-desc{
                    padding: 8px 10px;
                    clear: both;
                    .title-wrap{
                        width:100%;
                        line-height: 30px;
                        font-size:16px;
                    }
                    .defect-span {
                        display: inline-block;
                        width: 400px;
                        float:left;
                        color: white;
                        margin-right: 10px;
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
                }
            }
            .save-report {
                padding: 4px 20px;
                color: #fff;
                background: #e26829;
                border-radius: 25px;
                margin-right: 8px;
                cursor: pointer;
            }
        }

        .print-content-wrap {
            display: none;
        }
        /*.content-wrap {*/
        /*display: none;*/
        /*}*/
        .mb-10 {
            margin-bottom: 10px;
        }

        .w-100 {
            width: 100px;
        }
        .w-150 {
            width: 150px;
        }
        .w-200 {
            width: 200px;
        }
        .w-260 {
            width: 260px;
        }

        .inline-block {
            display: inline-block;
        }
        .color-red {
            color: #e26829;
        }

        .diagnosis-detail-warp {

            .diagnosis-detail {
                width: 620px;
                .el-dialog__header {
                    display: none;
                }
                .el-dialog__body {
                    padding: 0;
                }

                .info {
                    overflow: hidden;
                    background: #353945;
                    width: 100%;
                    padding: 20px 20px 0;
                    // box-shadow: 0 1px 0 #2B2F3B;
                    min-height: 260px;
                    margin-bottom: 20px;
                    display: flex;
                    .info-left {
                        border-right: 1px dotted #2b2f3b;
                        flex: 1;
                        padding-left: 13px;
                        .item {
                            height: 60px;
                            line-height: 60px;
                            .label {
                                display: inline-block;
                                line-height: 20px;
                                width: 100px;
                                vertical-align: middle;
                            }
                            .input {
                                width: 165px;
                                // margin-left: 10px;
                                & > input {
                                    height: 29px;
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
                            height: 60px;
                            line-height: 60px;
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
                }
            }

        }
    }
</style>



// WEBPACK FOOTER //
// result.vue?a916cd16