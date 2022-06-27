<template>
    <div class="hmi">
        <div class="hmi-content">
            <label class="title">
                <img class="icon" src="../../assets/images/icon/icon-hmi.png" />{{ $t('hmi') }}
                <div class="connect-tip">
                    <a href="javascript:;" @click="dialogTipVisible = true" class="btn-tip btn-underline">
                        <img src="../../assets/images/icon/icon-help.png" class="tip-img" />
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
            <!--基本数据-->
            <div class="info option">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.mainData') }}</label>
                    <span @click="jumpToUpdate" v-show="showUpdateTag" style="color:#E26829">New version ready for Update</span>
                </div>
                <div class="info-left">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">SN</label>
                        <el-input class="input input-sn" v-model="sn" ></el-input>
                    </div>
                    <div class="item" v-else>
                        <label class="label">SN</label>
                        <el-input class="input" v-model="sn" ></el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <!-- <el-button>Read</el-button> -->
                            <el-button type="primary" @click="writeSn"
                                       :disabled="isDealerOrAssembler && offLineFlag"
                                       v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">{{ $t('cNam.write') }}
                            </el-button>
                            <!--<el-button @click="test()" type="primary">test</el-button>-->
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

                <div class="info-right">
                    <div class="item" v-if = "!usbHID">
                        <label class="label">{{ $t('cNam.com') }}</label>
                        <el-select v-model="com" @change="changeCom" @click.native="getComList"
                                   :placeholder="$t('cNam.select')">
                            <el-option v-for="(com, index) in comList" :key=index :label="com" :value="com">
                            </el-option>
                        </el-select>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button @click="connect" type="primary">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="item" v-else>
                        <label class="label">USB HID</label>
                        <label class="tip" :class="{connect: isConnected}"></label>
                        <el-button @click="connect" type="primary"  :disabled = "loading || isReset">{{ $t('cNam.connect') }}</el-button>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>
                            <div class="btns">
                                <el-button type="preview" @click="readBase" :disabled = "loading || isReset || !isConnected">{{ $t('cNam.read') }}</el-button>
                                <el-button type="primary" @click="reset" :disabled = "loading || isReset || !isConnected" v-show="isCan" >{{ $t('cNam.reset') }}</el-button>
                                <!--<el-button type="preview" @click="readTestData" :disabled = "loading || isReset || !isConnected" v-show="!isCan">{{ $t('cNam.test') }}</el-button>-->
                            </div>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.softwareVersion') }}</label>
                        <el-input class="input" v-model="version" disabled></el-input>
                    </div>
                </div>
            </div>
            <!--参数内容-->
            <div class="info">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.parameter') }}</label>
                </div>
                <div class="info-left">
                    <div class="item" v-show = '!isCan'>
                        <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                        <el-input class="input" v-model="wheelDiameterRead" disabled></el-input>
                    </div>
                    <div class="item" v-show = '!isCan'>
                        <label class="label">{{ $t('cNam.speedLimit') }}</label>
                        <el-input class="input" v-model="speedLimitRead" disabled>
                            <template slot="append">Km/h</template>
                        </el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.totalMileage') }}</label>
                        <el-input class="input" v-model="totalMileage" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.singleMileage') }}</label>
                        <el-input class="input" v-model="singleMileage" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiSingleMileageClear') }}</label>
                        <el-input class="input" v-model="hmiSingleTrip" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.speedMax') }}</label>
                        <el-input class="input" v-model="speedMax" disabled>
                            <template slot="append">Km/h</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.speedAvg') }}</label>
                        <el-input class="input" v-model="speedAverage" disabled>
                            <template slot="append">Km/h</template>
                        </el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiTurnOffTime') }}</label>
                        <el-input class="input" v-model="timeClose" disabled></el-input>
                    </div>
                </div>
                <div class="info-right">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCodeRead" disabled></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.totalGear') }}</label>
                        <el-input class="input" v-model="totalGear" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiLevel') }}</label>
                        <el-input class="input" v-model="hmiLevel" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiMode') }}</label>
                        <el-input class="input" v-model="hmiMode" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiBoost') }}</label>
                        <el-input class="input" v-model="boost" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiLight') }}</label>
                        <el-input class="input" v-model="hmiLight" disabled></el-input>
                    </div>
                    <div class="item" v-show = 'isCan'>
                        <label class="label">{{ $t('cNam.hmiButton') }}</label>
                        <el-input class="input" v-model="hmiButton" disabled></el-input>
                    </div>
                </div>
            </div>
            <!-- Uart 页面逻辑  -->
            <div class="info" v-if = "!isCan">
                <div class="note-title">
                    <label class="title">{{ $t('cNam.setting') }}</label>
                </div>
                <div class="info-left">
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCode"></el-input>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.wheelDiameter') }}</label>
                        <!--<el-input class="input" v-model="wheelDiameter" ></el-input>-->
                        <el-select v-model="wheelDiameter">
                            <el-option v-for="(item, index) in wheelDiameters" :key=index :label="item"
                                       :value="item">
                            </el-option>
                        </el-select>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.speedLimit') }}</label>
                        <!--<el-input class="input" v-model="speedLimit" ></el-input>-->
                        <el-select v-model="speedLimit">
                            <el-option v-for="(item, index) in speedLimits" :key=index :label="item" :value="item">
                            </el-option>
                        </el-select>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="preview" @click="clickWriteParam">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.serviceMileage') }}</label>
                        <el-input class="input" v-model="serviceMileage" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="clearServiceMileage">{{ $t('cNam.clear') }}
                            </el-button>
                        </div>
                    </div>
                    <div class="clear-10"></div>
                    <div v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                            <label class="label">{{ $t('cNam.setTimeZone') }}</label>
                            <el-select v-model="utcTime" :placeholder="$t('cNam.select')" class="input">
                                <el-option
                                    v-for="item in timezone"
                                    :key="item.value"
                                    :label="item.label"
                                    :value="item.value">
                                </el-option>
                            </el-select>
                        </div>
                        <div class="btn-row">
                            <div class="btn-group">
                                <el-button type="primary" @click="">{{ $t('cNam.write') }}</el-button>
                            </div>
                        </div>
                        <div class="clear-10"></div>
                    </div>
                </div>
                <div class="info-right">
                    <!--<div class="bar" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">-->
                        <!--<label class="title">-->
                            <!--<img class="icon" src="../../assets/images/icon/icon-key.png" /><span>{{ $t('cNam.keyCard') }}</span>-->
                        <!--</label>-->
                    <!--</div>-->
                    <!--<div class="info-left" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">-->
                        <!--<div class="item">-->
                            <!--<label class="label">Card #1 </label>-->
                            <!--<el-input class="input" v-model="keyCards[0]" disabled></el-input>-->
                        <!--</div>-->
                        <!--<div class="item">-->
                            <!--<label class="label">Card #2 </label>-->
                            <!--<el-input class="input" v-model="keyCards[1]" disabled></el-input>-->
                        <!--</div>-->
                        <!--<div class="item">-->
                            <!--<label class="label">Card #3 </label>-->
                            <!--<el-input class="input" v-model="keyCards[2]" disabled></el-input>-->
                        <!--</div>-->
                        <!--<div class="item">-->
                            <!--<label class="label">Card #4 </label>-->
                            <!--<el-input class="input" v-model="keyCards[3]" disabled></el-input>-->
                        <!--</div>-->
                        <!--<div class="item">-->
                            <!--<label class="label">Card #5 </label>-->
                            <!--<el-input class="input" v-model="keyCards[4]" disabled></el-input>-->
                        <!--</div>-->
                        <!--<div class="btn-row">-->
                            <!--<div class="btn-group">-->
                                <!--<el-button type="preview" @click="keyCardStart">{{ $t('cNam.start') }}</el-button>-->
                                <!--<el-button type="primary" @click="keyCardStop">{{ $t('cNam.stop') }}</el-button>-->
                                <!--<el-button type="preview" @click="readKeyCard">{{ $t('cNam.read') }}</el-button>-->
                            <!--</div>-->
                            <!--<br/>-->
                            <!--&lt;!&ndash;<div>&ndash;&gt;-->
                            <!--&lt;!&ndash;KeyData:{{keyData}}&ndash;&gt;-->
                            <!--&lt;!&ndash;</div>&ndash;&gt;-->
                        <!--</div>-->
                    <!--</div>-->
                    <div class="item">
                        <label class="label">{{ $t('cNam.setTotalMileage') }}</label>
                        <el-input class="input" v-model="setTotalMileage">
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="clearMileageUartOperation('total')">{{ $t('cNam.set') }}
                            </el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.setSingleMileage') }}</label>
                        <el-input class="input" v-model="setSingleMileage">
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="clearMileageUartOperation('single')">{{ $t('cNam.set') }}
                            </el-button>
                        </div>
                    </div>
                </div>
            </div>
            <!-- Can 页面逻辑  -->
            <div class="info" v-else>
                <div class="note-title">
                    <label class="title">{{ $t('cNam.setting') }}</label>
                </div>
                <div class="info-left">
                    <div class="item"  v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.clientCode') }}</label>
                        <el-input class="input" v-model="clientCode" placeholder="6004"></el-input>
                    </div>
                    <div class="btn-row" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="primary" @click="clickWriteParam">{{ $t('cNam.write') }}</el-button>
                        </div>
                    </div>
                    <!--<div class="item" >-->
                    <!--<label class="label">{{ $t('cNam.model') }}</label>-->
                    <!--<el-input class="input" v-model="modelWrite"></el-input>-->
                    <!--</div>-->
                    <!--<div class="item" >-->
                    <!--<label class="label">{{ $t('cNam.hardwareVersion') }}</label>-->
                    <!--<el-input class="input" v-model="hardwareWrite"></el-input>-->
                    <!--</div>-->
                    <div class="item" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <label class="label">{{ $t('cNam.manName') }}</label>
                        <el-input class="input" v-model="manufacturer" placeholder="6005"></el-input>
                    </div>
                    <div class="btn-row" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
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
                    <!--<div class="btn-row">-->
                    <!--<div class="btn-group">-->
                    <!--<el-button type="info " @click="clickCopy">复制数据</el-button>-->
                    <!--<el-button type="preview" @click="clickWriteReset">{{ $t('cNam.write') }}</el-button>-->
                    <!--</div>-->
                    <!--</div>-->
                    <div class="item">
                        <div class="btn-row">
                            <label class="label">{{ $t('cNam.keyCard') }}</label>
                            <el-button type="primary" @click="keyCardStart">{{ $t('cNam.start') }}</el-button>
                        </div>
                        <!--<div>-->
                        <!--KeyData:{{keyData}}-->
                        <!--</div>-->
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.time') }}</label>
                        <el-time-picker
                            v-model="setTime"
                            value-format = 'HH:mm:ss'
                            @change="handleChange()"
                            >
                        </el-time-picker>
                    </div>
                    <div>
                        <div class="btn-row">
                            <div class="btn-group">
                                <el-button type="primary" @click="setTimeCAN">{{ $t('cNam.set') }}</el-button>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="info-right">
                    <div class="item">
                        <label class="label">{{ $t('cNam.serviceMileage') }}</label>
                        <el-input class="input" v-model="serviceMileage" disabled>
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="clearServiceMileageCan">{{ $t('cNam.clear') }}
                            </el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.setTotalMileage') }}</label>
                        <el-input class="input" v-model="setTotalMileage">
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="setTotalMileageCan">{{ $t('cNam.set') }}
                            </el-button>
                        </div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('cNam.setSingleMileage') }}</label>
                        <el-input class="input" v-model="setSingleMileage">
                            <template slot="append">Km</template>
                        </el-input>
                    </div>
                    <div class="btn-row">
                        <div class="btn-group">
                            <el-button type="primary" @click="setSingleMileageCan">{{ $t('cNam.set') }}
                            </el-button>
                        </div>
                    </div>
                </div>
            </div>
            <!-- Technician 页面逻辑  -->
            <div class="info" v-if="isTechnician">
                <div class="note-title">
                    <label class="title">Technician</label>
                </div>
                <div class="info-left">
                    <div class="item"  v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        CAN仪表功能测试
                        <el-button type="primary" @click="goToDiagnosis()">进入正常状态</el-button>
                        <el-button type="primary" @click="goOutDiagnosis()">退出正常状态</el-button>
                    </div>
                    <div class="btn-row" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="primary" @click="test('0101')">开仪表开控制器</el-button>
                            <el-button type="primary" @click="test('0100')">开仪表不开控制器</el-button>
                        </div>
                    </div>
                    <div class="btn-row" v-if="org.type === Core.Const.ORG_TYPE.COMPONENT">
                        <div class="btn-group">
                            <el-button type="primary" @click="test('0001')">不开仪表开控制器</el-button>
                            <el-button type="primary" @click="test('0000')">不开仪表不开控制器</el-button>
                        </div>
                    </div>
                </div>
                <div class="info-right">
                </div>
            </div>

            <!--
            <div class="info record" v-if="offLineFlag">
                <div class="wrap">
                    <p class="title-record">Offline Operation Record</p>
                    <p><span>Remain Operation Amount:</span> <span class="number">{{ 100 - offlineRecordLength}}</span>
                    </p>
                    <p v-if = "offlineRecordLength > 97" >
                       <span class="number"> {{ $t('cNam.offlineCon') }} </span>
                    </p>
                </div>
            </div>
            -->
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
                <div class="txt"><span class="itemName">{{ $t('cNam.write') }}</span>   <span>{{ $t('cNam.txtTitleWrt') }}</span></div>
                <div class="txt"><span class="itemName">{{ $t('cNam.reset') }}</span>   <span>{{ $t('cNam.txtTitleReset') }}</span></div>
                <div class="txt"><span class="itemName" v-show = "!isCan">{{ $t('cNam.test') }}</span>   <span>{{ $t('cNam.txtTitleTest') }}</span></div>
                <div class="clear-20"></div>
                <h1> {{ $t('cNam.txtTitle2') }} </h1>
                <div class="txt">{{ $t('cNam.txtNote1') }}</div>
                <div class="txt">{{ $t('cNam.txtNote2') }}</div>
            </div>
            <div class="clear-20"></div>
            <div class="">
                <h1>
                    {{ $t('cNam.pcMode') }} - UART
                </h1>
                <div class="txt">
                    {{ $t('cNam.pcModeTxt') }}
                </div>
                <div class="txt">
                    {{ $t('cNam.pcModeTxtLCD') }}
                </div>
                <div class="txt">
                    {{ $t('cNam.pcModeTxtLED') }}
                </div>
            </div>
        </el-dialog>

        <el-dialog
            :title="$t('auth')"
            :visible.sync="dialogWriteAuthVisible">
            <div>
                <label>
                    {{ $t('noHmiAuthTip') }}
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
                 <el-button type="primary" @click="applyAuth(0)">{{ $t('applyHmiAuth') }}</el-button>
                  <el-button type="primary" @click="applyAuth(1)">{{ $t('applyAllAuth') }}</el-button>
                <el-button type="primary" @click="dialogWriteAuthVisible = false">{{ $t('cNam.cancel') }}</el-button>
            </span>
        </el-dialog>

        <el-dialog title= "Confirm PC mode" :visible.sync="pcDialogVisible" class="tip-dialog">
            <div>
                <h1><label>{{ $t('cNam.note') }}:</label>{{ $t('cNam.hmiUART') }}</h1>
                <p></p>
                <div class="">
                    <div class="txt">
                        {{ $t('cNam.hmiUARTText1') }}
                    </div>
                    <div class="txt">
                        {{ $t('cNam.hmiUARTText2') }}
                    </div>
                    <div class="txt">
                        {{ $t('cNam.hmiUARTText3') }}
                    </div>
                    <div class="txt">
                        {{ $t('cNam.hmiUARTText4') }}
                    </div>
                </div>
                <div class="clear-20"></div>
                <div slot="footer" class="dialog-footer">
                    <el-button type="primary" @click="pcDialogVisible = false">{{ $t('cNam.cancel') }}</el-button>
                    <el-button type="success" @click="continueRead()">{{ $t('cNam.read') }}</el-button>
                </div>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import HMI from "../../../device/hmi"
    import Diagnosis from "../../../device/diagnose"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import vueLoading from 'vue-loading-template'
    import Core from 'core/core'

    export default {
        name: 'ToolHmi',
        components: {
            vueLoading
        },
        data() {
            return {
                isConnected: HMI.port.isConnected(),
                com: '',
                comList: [],
                speedLimits: [],
                wheelDiameters: [],

                sn: '',
                model: '',
                version: '',
                hardVersion: '',
                clientCode: '',
                wheelDiameter: '',
                speedLimit: '',
                clientCodeRead: '',
                wheelDiameterRead: '',
                speedLimitRead: '',
                serviceMileage: '',
                totalMileage: '',
                totalGear: '',
                speedAverage: '',
                singleMileage: '',
                speedMax: '',
                timeClose: '',
                hmiMode: '',
                hmiLevel: '',
                boost: '',
                hmiLight: '',
                hmiButton: '',
                hmiSingleTrip: '',

                setTotalMileage: '',
                setSingleMileage: '',
                setTime: new Date(),

                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',

                writeStatus: false,
                loading: false,

                isDealerOrAssembler: false,
                dialogWriteAuthVisible: false,
                brandId: '',
                brandList: [],
                auth: "",

                offLineFlag: Core.Data.getOffline(),

                keyCards: ['','','','',''],
                keyData: '',

                org: Core.Data.getOrg() || '',
                Core: Core,
                userName: Core.Data.get("userName") || "",

                userId: Core.Data.getUser().id,
                recordType: Core.Const.OBJECT_RECORD_TYPE.EDIT,

                timezone: [
                    {
                        value: '0700',
                        label: 'UTC +7'
                    },
                    {
                        value: '0800',
                        label: 'UTC +8'
                    }
                ],
                utcTime: '',

                readSn: '', // 读取到的sn
                offlineRecordList: [],
                offlineRecordLength: 0,

                dialogTipVisible: false,

                isCan: false,
                usbHID: false,
                readInterval: null,
                isReset: false,
                lanTrans: ['Not in BOOST','In BOOST','Light turn off','Light turn on','Not pressed','5','Pressed','Clean','No clean','Never turn off'],

                modelWrite: '',
                hardwareWrite: '',
                errorCode: '',
                bootLoad: '',
                manufacturer: '',

                pcDialogVisible: false,
                reconnectInterval: null,
                connectionIntervalHmi: null,

                isTechnician: false,

                showUpdateTag: false,

            }
        },
        i18n: {
            messages: {
                en: EN.Components.Tool.Hmi,
                zh: ZH.Components.Tool.Hmi,
                de: DE.Components.Tool.Hmi,
                nl: NL.Components.Tool.Hmi
            }
        },

        mounted() {
            this.initTrans();

            this.isDealerOrAssembler = Core.Data.getOrg().type === Core.Const.ORG_TYPE.DEALER || Core.Data.getOrg().type === Core.Const.ORG_TYPE.OEM || Core.Data.getOrg().type === Core.Const.ORG_TYPE.SPECIAL_DEAlER;
            this.offLineFlag = Core.Data.getOffline();
            this.userId = Core.Data.getUser().id;
            this.recordType = Core.Const.OBJECT_RECORD_TYPE.EDIT;

            for (let i = 6; i < 35; i++) {
                this.wheelDiameters.push(i);
            }
            this.wheelDiameter = 26;

            for (let i = 10; i <= 60; i++) {
                this.speedLimits.push(i);
            }
            // 经销商离线10-25
            if (this.isDealerOrAssembler && this.offLineFlag) {
                this.speedLimits = [];
                for (let i = 10; i <= 25; i++) {
                    this.speedLimits.push(i);
                }
            }
            this.speedLimit = 25;
            this.getOfflineRecordList();
            this.offlineRecordLength = this.offlineRecordList.length;

            HMI.port.timeLog('HMI');

//            BESSTSDK.client.connect();
//                .then(res => {
//                    this.usbHID = true;
//                    console.log(res);
//                }).catch(err =>{
//                    debugger;
//                    console.log(err);
//                }).finally(()=>{
//                    console.log('finally');
//                });
            HMI.port.setBoxOperation();
            this.checkConnection();
//            this.checkUSBUartOrCan();
            this.isTechnician = Core.Data.get('accountType').length>0 && Core.Data.get('accountType').indexOf(7)>-1;

        },
        methods: {
            jumpToUpdate() {
                this.$router.push(
                    {
                        name: "brandToolUpdate",
                        query: { from: 'Hmi', sn: this.sn.trim(), soft: this.version, isUpdate: true}
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
            checkConnection(){
                let vm = this;
                if(vm.connectionIntervalHmi){
                    clearInterval(vm.connectionIntervalHmi);
                    vm.connectionIntervalHmi = '';
                }
                vm.connectionIntervalHmi = setInterval(()=>{
//                    console.log('checkConnection',new Date().getTime());
                    let connection = HMI.port.getConnection();
                    if(connection.usb){
//                        this.checkUSBUartOrCan();
                        vm.usbHID = true;
                    }
                    if(connection.com){
                        vm.usbHID = false;
                    }
                    if(connection.usb || connection.com){
                        vm.init();
                        clearInterval(vm.connectionIntervalHmi);
                    }
                },300)
            },
//            checkUSBUartOrCan(){
//                let count = 0;
//                let checkUSBUartOrCanIntervalHmi = setInterval(()=>{
//                    let USBUartOrCan = HMI.port.checkUsbUart();
//                    if(USBUartOrCan.usb){
//
//                        if(USBUartOrCan.can || USBUartOrCan.uart){
//                            clearInterval(checkUSBUartOrCanIntervalHmi);
//                        }
//                    }
//                },800)
//            },
            init(){
                console.log('init',new Date().getTime());
                HMI.port.startConnection();
                if(HMI.port.getUSBDevice() || this.usbHID){
                    this.usbHID = true;
                    console.log(" get USB Device HMI");
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
                this.isConnected = HMI.port.isConnected();
                console.log('init',new Date().getTime());

            },
            initTrans(){
//                console.log(this.lanTrans);
                this.lanTrans[0] = this.$t('cNam.hmiBoostOut');
                this.lanTrans[1] = this.$t('cNam.hmiBoostIn');
                this.lanTrans[2] = this.$t('cNam.hmiLightOff');
                this.lanTrans[3] = this.$t('cNam.hmiLightOn');
                this.lanTrans[4] = this.$t('cNam.hmiBtnPressNot');
                this.lanTrans[6] = this.$t('cNam.hmiBtnPress');
                this.lanTrans[7] = this.$t('cNam.hmiClean');
                this.lanTrans[8] = this.$t('cNam.hmiNotClean');
                this.lanTrans[9] = this.$t('cNam.hmiNoTurnOff');
//                console.log(this.lanTrans);
            },
            getComList() {
//                if (HMI.port.getPort() != '' && HMI.port.isConnected()) {
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
                            console.log('getComList',new Date().getTime());
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
//                    HMI.port.setCan(true);
//                }else{
//                    HMI.port.setCan(false);
//                }
//                this.isConnected = HMI.port.isConnected();
//                this.init();
//            },
            changeCom() {
//                this.isConnected = false;
            },

            connect(type) {
                console.log('connect',new Date().getTime());
                if( this.usbHID ){
                    if(type === 1){
                        let vm = this;
                        setTimeout(()=>{
                            vm.test('0101');
                        },1000)
                    }else{
                        this.test('0101');
                    }
                    this.isConnected = HMI.port.isConnected();
                    this.isCan = !HMI.port.checkUsbUart();
                    if(this.isCan){
                        this.readInterval = 1200;
                    }else{
                        this.readInterval = 4000;
                    }
                    console.log("usbHID connect");
                    HMI.port.getListenData().subscribe((res) => {
                        if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb') {
                            this.isConnected = false;
                            this.clear();
                        }
                    });
                }else{
                    this.readInterval = 6500;
                    if (!this.com ) {
                        this.$message({
                            message: this.$t('cNam.selectComTip'),
                            type: 'warning'
                        });
                        return;
                    } else {
                        HMI.connect(this.com);
                        HMI.getBus().subscribe(res => {
                            if (res.type === 'open' || res.type === 'connect') {
                                this.isConnected = true;
                                console.log('connect',new Date().getTime());
                            }
                            if (res.type === 'close' || res.type === 'error') {
                                this.com = '';
                                this.isConnected = false;
                                this.loading = false;
//                            this.$message({
//                                message: this.$t('cNam.comDisconnect'),
//                                type: 'warning'
//                            });
                            }
                        });
                    }
                }
            },

            readBase(){
                if(this.isCan){
                    this.readBaseInfo();
                }else{
                    // 判断是否为产线使用
                    if(this.userName.indexOf("fqc") < 0){
                        this.pcDialogVisible = true;
                    }else{
                        this.readBaseInfo();
                    }
                }
            },
            continueRead(){
                this.readBaseInfo();
                this.pcDialogVisible = false;
            },

            test(cmd){
                HMI.port.checkUsbConnection(cmd, 1);
            },
            goToDiagnosis(){
                HMI.port.usbSDK.client.switchModel('01').then(res=>{
                    console.log(res);
                }).catch(err =>{

                    console.log(err,'goToDiagnosis');
                }).finally(()=>{
                    console.log('goToDiagnosis response');
                });
            },
            goOutDiagnosis(){
                HMI.port.usbSDK.client.switchModel('00').then(res=>{
                    console.log(res);
                }).catch(err =>{

                    console.log(err,'goOutDiagnosis');
                }).finally(()=>{
                    console.log('goOutDiagnosis response');
                });
            },
            readBaseInfo() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.loading = true;
                console.log("[readInterval]",this.readInterval);
                setTimeout(() => {
                    if (this.loading) {
                        if (!this.checkData()) {
                            if(this.isCan){
                                this.$message({
                                    message: this.$t('hmiTipCan'),
                                    type: "warning"
                                });
                            }else{
                                this.$message({
                                    message: this.$t('hmiTip'),
                                    type: "warning"
                                });
                            }
                        }
                    }
                    this.loading = false;
                }, this.readInterval);
                this.clear();
                this.connect(1);
                this.showUpdateTag = false;
                if (!this.isCan || HMI.port.checkUsbUart()) {
                    HMI.sendCmdGetFullData();
                    HMI.getBus().subscribe(res => {
//                        console.log('hmi-vm2:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.hmi) {
                            return;
                        }
                        if (res.data.hmi.hasOwnProperty("sn")) {
                            this.sn = res.data.hmi.sn;
                            this.readSn = this.sn;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.hmi.hasOwnProperty("model")) {
                            this.model = res.data.hmi.model;
                        }
                        if (res.data.hmi.hasOwnProperty("version")) {
                            this.version = res.data.hmi.version;
                            this.checkSnVersionUpdate();
                        }
                        if (res.data.hmi.hasOwnProperty("hardware_version")) {
                            this.hardVersion = res.data.hmi.hardware_version;
                        }
                        if (res.data.hmi.hasOwnProperty("client_code")) {
                            this.clientCodeRead = res.data.hmi.client_code
                        }
                        if (res.data.hmi.hasOwnProperty("wheel_diameter")) {
                            this.wheelDiameterRead = res.data.hmi.wheel_diameter
                        }
                        if (res.data.hmi.hasOwnProperty("speed_limit")) {
                            this.speedLimitRead = res.data.hmi.speed_limit
                        }
                        if (res.data.hmi.hasOwnProperty("service_mileage")) {
                            this.serviceMileage = res.data.hmi.service_mileage
                        }
                        if (res.data.hmi.hasOwnProperty("single_mileage")) {
                            this.singleMileage = res.data.hmi.single_mileage
                        }
                        if (res.data.hmi.hasOwnProperty("total_mileage")) {
                            this.totalMileage = res.data.hmi.total_mileage
                        }
                        if (res.data.hmi.hasOwnProperty("total_gear")) {
                            this.totalGear = res.data.hmi.total_gear
                        }
                        if (res.data.hmi.hasOwnProperty("key_card")) {
                            let data = res.data.hmi.key_card;
                            let start = 0;
                            let end = 0;
                            let count = 0;
                            for(let i=0; i< data.length;){
                                if( data.substr(i,2) == '5C' ){
                                    end = i;
                                    this.keyCards[count] = data.substring(start,end) ;
                                    start = i+2;
                                    count++;
                                }
                                i = i + 2;
                            }
                            // let arr = data.split("|");
                            // for(let len = 0; len<5; len ++){
                            //     this.keyCards[len] = arr[len];
                            // }
                        }
                    });
                }else{

                    HMI.port.getUsbBasic(0x03).subscribe(res => {
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

                    HMI.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05,0x03)
                        .then(res => {
//                            console.log(res,'dataOfHmi_1 6301');

                            let result_1 = Buffer.from(res.data, 'hex');
//                            console.log(result_1,'dataOfHmi_1 6301');
                            if(result_1.length === 8){
                                this.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2]*256*256;
                                this.singleMileage = (result_1[3] + result_1[4] * 256 + result_1[5]*256*256)/10;
                                this.speedMax = (result_1[6] + result_1[7] * 256)/10;
                            }
                            return   HMI.port.usbSDK.machineInformation.get.dataOfHmi_2(0x05,0x03)
                        }).then(res => {
//                        console.log(res,'dataOfHmi_2 6302');

                        let result_3 = Buffer.from(res.data, 'hex');
//                        console.log(result_3,'dataOfHmi_2 6302');
//                        console.log(result_3[0],'dataOfHmi_2 6302');
//                        console.log(result_3[1],'dataOfHmi_2 6302');
                        if(result_3.length >= 5){
                            this.speedAverage = (result_3[0] + result_3[1] * 256) / 10;
                            this.serviceMileage = (result_3[2] + result_3[3] * 256 + result_3[4] * 256 * 256) / 10;
                        }
                    }).catch(err =>{
                        console.log(err);
                    }).finally(()=>{
                    });

                    HMI.port.getListenData().subscribe((res)=> {
//                            console.log(res);
                        if(!res.data){
                            return;
                        }
                        if( res.type == 'error_usb'){
                            this.isConnected = false;
                            this.clear();
                        }
                        if (res.data.code === '63' && res.data.subCode === '00') {
                            let temTotal = Buffer.from(res.data.data, 'hex');
                            if(temTotal.length >= 4){
                                let temString_0 = HMI.port.addZero(temTotal[0].toString(2), 8);
                                this.totalGear = parseInt(temString_0.substr(4, 4), 2);
                                this.hmiMode = temString_0.substr(3, 1) === '0' ? 'ECO' : 'SPORT';
                                this.boost = temString_0.substr(2, 1) === '0' ?  this.lanTrans[0]:  this.lanTrans[1];
                                this.hmiLevel = Diagnosis.getGearNumber(temTotal[1], this.totalGear );
                                let temString_2 = HMI.port.addZero(temTotal[2].toString(2), 8);
                                this.hmiLight = temString_2.substr(7, 1) === '0' ? this.lanTrans[2] : this.lanTrans[3];
                                this.hmiButton = temString_2.substr(6, 1) === '0' ? this.lanTrans[4] : this.lanTrans[6];
                                let temString_3 = HMI.port.addZero(temTotal[3].toString(2), 8);
                                this.hmiSingleTrip = temString_3.substr(7, 1) === '0' ? this.lanTrans[7] : this.lanTrans[8];
                            }
                        }
                        if(res.data.code === '63' && res.data.subCode === '03' ){
                            let result_4 = Buffer.from(res.data.data, 'hex');
                            if(result_4.length >= 1){
                                let temTime = result_4[0];
                                this.timeClose = temTime === 255? this.lanTrans[9] : temTime + 'Min';
                            }
                        }
                    });
                }
            },
            usbErrorCheck(err){
                HMI.port.checkUsbError(err);
            },
            writeSn() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
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
//                        if (res.auth == null || !res.auth.auth_hmi) {
//                            Core.Api.Org.getAllPartnerBranList().then(res => {
//                                this.brandList = res.org_list;
//                                if (this.brandList.length > 0) {
//                                    this.brandId = this.brandList[0].id;
//                                }
//                                this.dialogWriteAuthVisible = true;
//
//                            });
//                        } else {
//                            this.writeSnWithAuth();
//                        }
//                    });
//                } else {
                this.writeSnWithAuth();
//                }
            },

            writeSnWithAuth() {
                this.writeStatus = true;
                this.connect();
                if(!this.isCan || HMI.port.checkUsbUart()){
                    HMI.writeSn(this.sn);
                    HMI.getBus().subscribe(res => {
//                    console.log('hmi-vm3:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!this.writeStatus) {
                            return;
                        }
                        if (res.data.hmi && 'sn' in res.data.hmi) {
                            let sn = res.data.hmi.sn;
                            console.log(sn, this.sn);
                            if (sn === this.sn || sn == ' ') {
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
                            this.writeStatus = false;
                        }
                    });
                } else {
                    HMI.port.usbSDK.machineInformation.update.serialNumber(0x05, 0x03, HMI.port.Core.Util.stringToHex(this.sn) )
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.sn = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                HMI.port.usbSDK.machineInformation.get.serialNumber(0x05, 0x03)
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
                        this.writeStatus = false;
                    });
                }
            },

            clickWriteParam() {
                if(!this.checkBeforeWrite()) return false;
                if (this.userName.indexOf("fqc") < 0 && parseInt(this.speedLimit) > 25) {
                    this.$message({
                        message: this.$t('cNam.speedLimitNote'),
                        type: "warning",
                        duration: 0,
                        showClose: true
                    });
                }
                let data = {
                    old: {
                        "Wheel Diameter": this.wheelDiameterRead,
                        "Speed Limit": this.speedLimitRead
                    },
                    new: {
                        "Wheel Diameter": this.wheelDiameter,
                        "Speed Limit": this.speedLimit
                    },
                };

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

                if (this.offLineFlag || !navigator.onLine) {
                    if(!this.checkOfflineDataLength()) return;
                    this.writeParamWithAuth(data);
                } else {
                    this.writeParamWithAuth(data);
                }
            },

            writeParamWithAuth(data) {
                let isShowMessage = false;
                this.clientCode = this.clientCode.replace(/\s/g, "");
                if ( this.clientCode ) {
                    if( this.clientCode.length < 25){
                        if(!this.isCan || HMI.port.checkUsbUart()){
                            HMI.writeClientCode(this.clientCode);
                            HMI.getBus().subscribe(res => {
                                if (res.type === 'close' || res.type === 'error') {
                                    this.isConnected = false;
                                    this.com = '';
                                }
                                if (!res.data || !res.data.hmi) {
                                    return;
                                }
                                if ('client_code' in res.data.hmi) {
                                    let code = res.data.hmi.client_code;
                                    this.clientCodeRead = code;
                                    if ((this.client_code === code || code === ' ') && !isShowMessage) {
                                        this.$message({
                                            message: this.$t('cNam.writeSuccess'),
                                            type: 'success'
                                        });

                                        isShowMessage = true;
                                    }
                                }
                            });
                        }else{
                            HMI.port.usbSDK.machineInformation.update.customerNumber(0x05, 0x03, HMI.port.Core.Util.stringToHex(this.clientCode))
                                .then(res => {
//                                    console.log(res);
                                    if (res.can === 'NORMAL_ACK') {
                                        this.clientCode = '';
                                        this.$message({
                                            message: this.$t('cNam.writeSuccess'),
                                            type: 'success'
                                        });
                                        HMI.port.usbSDK.machineInformation.get.customerNumber(0x05, 0x03)
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
                                this.writeStatus = false;
                            });
                        }
                    }else{
                        this.$message(this.$t('cNam.customerMaxLength'));
                    }
                }
                if (!this.isCan) {
                    this.wheelDiameterRead = '';
                    this.speedLimitRead = '';
                    HMI.writeParam(this.wheelDiameter, this.speedLimit);
                    HMI.getBus().subscribe(res => {
//                    console.log('hmi-vm5:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.hmi) {
                            return;
                        }
                        if ('wheel_diameter' in res.data.hmi) {
                            let diameter = res.data.hmi.wheel_diameter;
                            this.wheelDiameterRead = diameter;
                            if (diameter === this.wheelDiameter && !isShowMessage) {
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                isShowMessage = true;
                            }
                        }
                        if ('speed_limit' in res.data.hmi) {
                            let limit = res.data.hmi.speed_limit;
                            this.speedLimitRead = limit;
                            if (limit === this.speedLimit && !isShowMessage) {
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                isShowMessage = true;
                            }
                        }

                        //提示过message了 记录内容
                        if (isShowMessage && Core.Data.getOrg().type != Core.Const.ORG_TYPE.COMPONENT) {
                            console.log('save record');
                            this.saveRecord(data);
                        }
                    });
                }
            },
            clickCopy(){
                this.modelWrite = this.model;
                this.hardwareWrite = this.hardVersion;
            },
            clickWriteReset(){
                if(this.modelWrite && this.modelWrite.indexOf('DP')>-1){
                    HMI.port.usbSDK.machineInformation.update.modelNumber(0x05, 0x03, HMI.port.Core.Util.stringToHex(this.modelWrite.trim()))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.modelWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                HMI.port.usbSDK.machineInformation.get.modelNumber(0x05, 0x03)
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
                            this.$message({
                                message: this.$t('cNam.operationFail'),
                                type: 'error'
                            });
                        console.log(err);
                        this.usbErrorCheck(err);
                    }).finally(()=>{
                        this.writeStatus = false;
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeModelTip'),
                        type: 'warning'
                    });
                }
                if(this.hardwareWrite && this.hardwareWrite.indexOf('DP')>-1){
                    HMI.port.usbSDK.machineInformation.update.hardwareVersionNumber(0x05, 0x03, HMI.port.Core.Util.stringToHex(this.hardwareWrite.trim()))
                        .then(res => {
//                            console.log(res);
                            if (res.can === 'NORMAL_ACK') {
                                this.hardwareWrite = '';
                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                                HMI.port.usbSDK.machineInformation.get.hardwareVersionNumber(0x05, 0x03)
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
                        this.writeStatus = false;
                    });
                } else {
                    this.$message({
                        message: this.$t('cNam.writeHardwareTip'),
                        type: 'warning'
                    });
                }
            },
            clearServiceMileage() {
                HMI.clearServiceMileage();
            },
            clearServiceMileageCan() {
//                if( this.serviceMileage > 5000){
                HMI.port.usbSDK.machineInformation.update.dataOfHmi_2(0x05,0x03,'0000000000')
                    .then(res=>{
//                        console.log(res);
                        this.$message({
                            message:  this.$t('cNam.success'),
                            type: 'success'
                        });
                    }).catch(err =>{
                        this.$message({
                            message: this.$t('cNam.operationFail'),
                            type: 'error'
                        });
                    console.log(err);
                }).finally(()=>{
                });
            },
            // CAN 设置总里程
            setTotalMileageCan() {
                if(this.singleMileage === '' || this.totalMileage === '') {
                    this.$message({
                        message: this.$t('cNam.readDateTip'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                if(!this.checkTotalMileageLimit()) return;

                if( parseInt(this.setTotalMileage) < parseInt(this.singleMileage) ){
                    this.$message({
                        message: this.$t('mileageTotalLarger'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }

                if (this.offLineFlag || !navigator.onLine) {
                    if(!this.checkOfflineDataLength()) return;
                    this.setTotalMileageCanAuth()
                } else {
                    this.setTotalMileageCanAuth()
                }
            },

            setTotalMileageCanAuth() {
                let buf = Buffer.from(('000000'+parseInt(this.setTotalMileage).toString(16)).slice(-6),'Hex').reverse().toString('Hex');
                HMI.port.usbSDK.machineInformation.update.clear_Hmi_Total_Mileage(0x05,0x03,buf)
                    .then(res=>{
//                        console.log(res);
                        if(res.can === "NORMAL_ACK"){
                            let data = {
                                old: {
                                    "Total Mileage": this.totalMileage
                                },
                                new: {
                                    "Total Mileage": this.setTotalMileage
                                },
                            };
                            this.saveRecord(data);

                            HMI.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05,0x03)
                                .then(res => {
                                    let result_1 = Buffer.from(res.data, 'hex');
                                    if(result_1.length === 8){
                                        this.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2]*256*256;
                                        this.singleMileage = (result_1[3] + result_1[4] * 256 + result_1[5]*256*256)/10;
                                        this.speedMax = (result_1[6] + result_1[7] * 256)/10;
                                    }
                            }).catch(err =>{
                                console.log(err);
                            }).finally(()=>{
                            });

                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                        }else if(res.can === "ERROR_ACK"){
                            this.$message({
                                message: this.$t('cNam.writeFail'),
                                type: 'error'
                            });
                        }
                    }).catch(err =>{
                    this.$message({
                        message: this.$t('cNam.operationFail'),
                        type: 'error'
                    });
                    console.log(err);
                }).finally(()=>{
                });
            },
            // CAN 设置单里程
            setSingleMileageCan() {
                if(this.singleMileage === '' || this.totalMileage === '') {
                    this.$message({
                        message: this.$t('cNam.readDateTip'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                let single = this.checkSingleMileageLimit();
                console.log("single", typeof single);

                if(typeof single === 'number'){
                    let buf = Buffer.from(('000000'+single.toString(16)).slice(-6),'Hex').reverse().toString('Hex');
                    HMI.port.usbSDK.machineInformation.update.clear_Hmi_Single_Mileage(0x05,0x03,buf)
                        .then(res=>{
                        console.log(res);
                            if(res.can === "NORMAL_ACK"){
                                HMI.port.usbSDK.machineInformation.get.dataOfHmi_1(0x05,0x03)
                                    .then(res => {
                                        let result_1 = Buffer.from(res.data, 'hex');
                                        if(result_1.length === 8){
                                            this.totalMileage = result_1[0] + result_1[1] * 256 + result_1[2]*256*256;
                                            this.singleMileage = (result_1[3] + result_1[4] * 256 + result_1[5]*256*256)/10;
                                            this.speedMax = (result_1[6] + result_1[7] * 256)/10;
                                        }
                                    }).catch(err =>{
                                    console.log(err);
                                }).finally(()=>{
                                });

                                this.$message({
                                    message: this.$t('cNam.writeSuccess'),
                                    type: 'success'
                                });
                            }else if(res.can === "ERROR_ACK"){
                                this.$message({
                                    message: this.$t('cNam.writeFail'),
                                    type: 'error'
                                });
                            }
                        }).catch(err =>{
                        this.$message({
                            message: this.$t('cNam.operationFail'),
                            type: 'error'
                        });
                        console.log(err);
                    }).finally(()=>{
                    });
                }
            },

            clearMileageUartOperation(type) {
                if(!this.checkBeforeWrite()) return false;
                if(type === 'total'){
                    let total = this.checkTotalMileageLimit();
                    if(total){
                        if (this.offLineFlag || !navigator.onLine) {
                            if(!this.checkOfflineDataLength()) return;
                            HMI.clearMileageUart(this.setTotalMileage, type);
                        } else {
                            HMI.clearMileageUart(this.setTotalMileage, type);
                        }
                    }else{
                        return;
                    }
                }else if(type === 'single'){
                    let single = this.checkSingleMileageLimit();
                    if(single){
                        HMI.clearMileageUart(single, type);
                    }else{
                        return;
                    }
                }

                HMI.getBus().subscribe(res => {
                    if (res.type === 'close' || res.type === 'error') {
                        this.isConnected = false;
                        this.com = '';
                    }
                    if (!res.data || !res.data.hmi) {
                        return;
                    }
                    if ('set_total_mileage' in res.data.hmi) {
                        if (res.data.hmi.set_total_mileage === parseInt(this.setTotalMileage)) {

                            let data = {
                                old: {
                                    "Total Mileage": this.totalMileage
                                },
                                new: {
                                    "Total Mileage": this.setTotalMileage
                                },
                            };
                            this.saveRecord(data);

                            this.setTotalMileage = '';
                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                        }
                    }
                    if ('set_single_mileage' in res.data.hmi) {
                        let single_mileage = res.data.hmi.set_single_mileage;
                        if (single_mileage === parseInt(this.setSingleMileage)*10) {
                            this.setSingleMileage = '';
                            this.$message({
                                message: this.$t('cNam.writeSuccess'),
                                type: 'success'
                            });
                        }
                    }
                });
            },
            checkTotalMileageLimit() {
                if(this.setTotalMileage === '') {
                    this.$message({
                        message: this.$t('cNam.inputValueTip'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                if(!this.checkBeforeWrite()) return false;
                if(this.setTotalMileage.indexOf('.')> -1){
                    this.$message({
                        message: this.$t('mileageTotalSupport'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                if(parseInt(this.setTotalMileage) > 99999){
                    this.$message({
                        message: this.$t('mileageTotalMax'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }

                return true;
            },
            checkSingleMileageLimit() {
                if(this.setSingleMileage === '') {
                    this.$message({
                        message: this.$t('cNam.inputValueTip'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                if(!this.checkBeforeWrite()) return false;
                if(parseInt(this.setSingleMileage) > parseInt(this.totalMileage)){
                    this.$message({
                        message: this.$t('mileageSingleLess'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                let singleArr = this.setSingleMileage.split(".");
                if(this.setSingleMileage.indexOf('.')> -1 && singleArr[1].length>2 ){
                    this.$message({
                        message: this.$t('mileageSingleSupport'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                let num;
                if(singleArr.length === 2 && parseInt(singleArr[1]) < 10){
                    num = parseInt(singleArr[0])*10 + parseInt(singleArr[1]);
                }else{
                    num = parseInt(singleArr[0])*10;
                }
                if(num > 99999){
                    this.$message({
                        message: this.$t('mileageSingleMax'),
                        type: 'info',
                        duration: 5000,
                        showClose: true
                    });
                    return false;
                }
                return num
            },
            keyCardStart() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                if(!this.isCan || HMI.port.checkUsbUart()){
                    HMI.connect(this.com);

                    HMI.writeKeyCardStart();
                    HMI.getBus().subscribe(res => {
//                        console.log('hmi-vm9:', res);
                        if (res.type === 'close' || res.type === 'error') {
                            this.isConnected = false;
                            this.com = '';
                        }
                        if (!res.data || !res.data.hmi) {
                            return;
                        }
                        if (res.data.hmi.hasOwnProperty("key_card_start")) {
                            this.$message({
                                message: this.$t('cNam.keyCardStart'),
                                type: "success"
                            });
                        }

                    });
                }else{
                    HMI.port.usbSDK.machineInformation.update.set_Hmi_Card(0x05,0x03,'01')
                        .then(res=>{
//                            console.log(res);

//                            can: "NORMAL_ACK"
//                            canCode: 2
//                            code: "65"
//                            data: "00"
//                            dataLen: 0
//                            dataPacks: []
//                            dataParsed: ""
//                            source: "HMI"
//                            sourceCode: 3
//                            subCode: "00"
//                            target: "BST"
//                            targetCode: 5
                            if (res.can === 'NORMAL_ACK') {
                                this.$message({
                                    message: this.$t("pairCardIn"),
                                    type: 'success'
                                });
                            }
                        }).catch(err =>{
                        console.log(err);
                    }).finally(()=>{
                    });
                    HMI.port.getListenData().subscribe((res) => {
//                        console.log(res,"res");
                        // { can:"NORMAL_ACK",canCode:2,code:"35",data:"",dataLen:0,source:"CTL",sourceCode:2,subCode:"01",target:"BST",targetCode:5 }
                        /*
                        can: "NORMAL_ACK"
                        canCode: 2
                        code: "65"
                        data: "00"
                        dataLen: 1
                        source: "HMI"
                        sourceCode: 3
                        subCode: "00"
                        target: "BST"
                        targetCode: 5


                        can: "NORMAL_ACK"
                        canCode: 2
                        code: "65"
                        data: "01"
                        dataLen: 1
                        source: "HMI"
                        sourceCode: 3
                        subCode: "00"
                        target: "BST"
                        targetCode: 5


                        can: "NORMAL_ACK"
                        canCode: 2
                        code: "65"
                        data: "02"
                        dataLen: 1
                        source: "HMI"
                        sourceCode: 3
                        subCode: "00"
                        target: "BST"
                        targetCode:

                        */
                            if (!res.data) {
                            return;
                        }
                        if (res.type == 'error_usb' ) {
                            this.isConnected = false;
                            this.clear();
                        }
                        if (res.data.code === '65' && res.data.subCode === '00' && res.data.targetCode === 5 && res.data.can === "NORMAL_ACK") {
                            this.$message({
                                message: this.$t("pairCardSuccess") +' '+ parseInt(res.data.data.substr(0,2),16),
                                type: 'success'
                            });
                        }
                    });
                }

            },
            keyCardStop() {
                HMI.connect(this.com);

                HMI.writeKeyCardStop();
                HMI.getBus().subscribe(res => {
//                    console.log('hmi-vm8:', res);
                    if (res.type === 'close' || res.type === 'error') {
                        this.isConnected = false;
                        this.com = '';
                    }
                    if (!res.data || !res.data.hmi) {
                        return;
                    }
                    if (res.data.hmi.hasOwnProperty("key_card_stop")) {
                        this.$message({
                            message:  this.$t('cNam.keyCardStop'),
                            type: "success"
                        });
                    }

                });
            },
            readKeyCard() {
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return;
                }
                this.loading = true;
                HMI.connect(this.com);
                HMI.sendCmdGetKeyCard();
                HMI.getBus().subscribe(res => {
//                    console.log('hmi-vm2:', res);
                    if (res.type === 'close' || res.type === 'error') {
                        this.isConnected = false;
                        this.com = '';
                    }
                    if (!res.data || !res.data.hmi) {
                        return;
                    }
                    if (res.data.hmi.hasOwnProperty("key_card")) {
                        this.keyData = res.data.hmi.key_card;
                        let start = 0;
                        let end = 0;
                        let count = 0;
                        for(let i=0; i< this.keyData.length;){
                            if( this.keyData.substr(i,2) == '5C' ){
                                end = i;
                                this.keyCards[count] = this.keyData.substring(start,end);
                                start = i+2;
                                count++;
                            }
                            i = i + 2;
                        }

                    }
                });
                setTimeout(() => {
                    if (this.loading) {
                        if (this.keyCards[0]=='') {
                            this.$message({
                                message: this.$t('hmiTip'),
                                type: "warning"
                            });
                        }
                    }
                    this.loading = false;
                }, 2000);

            },
            handleChange() {
                console.log(this.setTime);
//                console.log( this.setTime.split(' '));
            },

            setTimeCAN() {
                let dateSet;
                if(this.setTime){
                    dateSet = new Date(this.setTime);
                }else{
                    dateSet = new Date();
                }
                let hour = dateSet.getHours();
                let minute = dateSet.getMinutes();
                let second = dateSet.getSeconds();
//                console.log(hour,minute,second);
//                console.log( Buffer.from([hour,minute,second]).toString('Hex') )
                HMI.port.usbSDK.machineInformation.update.set_Hmi_Time(0x05,0x03,Buffer.from([hour,minute,second]).toString('Hex'))
                    .then(res=>{
//                        console.log(res);
//                        can: "NORMAL_ACK"
//                        canCode: 2
//                        code: "62"
//                        data: ""
//                        dataLen: NaN
//                        dataPacks: []
//                        dataParsed: ""
//                        source: "HMI"
//                        sourceCode: 3
//                        subCode: "02"
//                        target: "BST"
//                        targetCode: 5
                        if (res.can === 'NORMAL_ACK') {
                            this.$message({
                                message: "success",
                                type: 'success'
                            });
                        }
                    }).catch(err =>{

                    console.log(err);
                }).finally(()=>{
                });
            },
            readTestData() {
                if (this.usbHID) {
                    Diagnosis.setupUsb();
                    Diagnosis.parseTestUart()
                }else{
                    Diagnosis.connect(this.com);
                }
                Diagnosis.getBus().subscribe(res => {

                });
            },

            checkBeforeWrite() {
                // 判断sn //
                if( Core.Data.getOrg().type !== Core.Const.ORG_TYPE.COMPONENT ){
                    if (!this.sn || this.sn !== this.readSn) {
                        this.$message.warning(this.$t('cNam.needReadSn'));
                        return false;
                    }
                }
                if (!this.isConnected) {
                    this.$message({
                        message: this.$t('cNam.comDisconnect'),
                        type: "warning"
                    });
                    return false;
                }
                if (!this.usbHID) {
                    HMI.connect(this.com);
                }
                return true;
            },

            checkOfflineDataLength() {
                // 如果离线
                // 如果上一次的数据在被处理，需要等待
                let isHandle = Core.Data.getIsBatchRecordIng();
                if (isHandle) {
                    this.$message.warning(this.$t('cNam.offlineHandle'));
                    return false;
                }
                this.getOfflineRecordList();
                // 离线的到了 50 条 需要联网
                if (this.offlineRecordList.length >= 50) {
                    this.$message.warning(this.$t('cNam.offlineCon'));
                    return false;
                }
                return true;
            },

            saveRecord(data) {
                if(!this.sn || this.sn !== this.readSn) {
                    return false;
                }
                let userId = this.userId;
//                console.log(userId);
                let sn = this.sn;
                let dataJson = JSON.stringify(data);
                let time = Core.Util.time();
                let type = this.recordType;
                this.sn = "";
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
            checkData() {
                if (this.sn && this.model && this.version && this.hardVersion) {
                    return true;
                }else{
                    return false;
                }
            },
            readManufacturer() {
                this.manufacturer = '';
                HMI.port.usbSDK.machineInformation.get.manufacturer(0x05,0x03)
                    .then(res=>{
                        this.manufacturer = res.dataParsed.replace(/\s+/g, "");
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
                HMI.port.usbSDK.machineInformation.update.manufacturer(0x05,0x03,HMI.port.Core.Util.stringToHex(this.manufacturer.trim()))
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
                HMI.port.usbSDK.machineInformation.get.errorCode(0x05,0x03)
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
            writeErrorCode() {
                HMI.port.usbSDK.machineInformation.update.errorCode(0x05,0x03,HMI.port.Core.Util.stringToHex(this.errorCode.trim()))
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
                HMI.port.usbSDK.machineInformation.get.bootLoad(0x05,0x03)
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
            writeBootLoad() {
                HMI.port.usbSDK.machineInformation.update.bootLoad(0x05,0x03,HMI.port.Core.Util.stringToHex(this.bootLoad.trim()))
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
            clear() {
                this.sn = '';
                this.model = '';
                this.version = '';
                this.hardVersion = '';
                this.serviceMileage = '';
                this.clientCodeRead = '';
                this.wheelDiameterRead = '';
                this.speedLimitRead = '';
                this.totalGear = '';
                this.totalMileage = '';
                this.keyCards = ['', '', '', '', ''];
                this.speedAverage = '';
                this.singleMileage = '';
                this.speedMax = '';
                this.timeClose = '';
                this.hmiMode = '';
                this.hmiLevel = '';
                this.boost = '';
                this.hmiLight = '';
                this.hmiButton = '';
                this.hmiSingleTrip = '';
            },
            reset() {
                this.isReset = true;
                setTimeout(()=>{
                    this.isReset = false;
                },1000);
                this.clear();
                HMI.port.reset(100);
                this.isConnected = false;
//                setTimeout(()=>{
//                    this.isConnected = HMI.port.isConnected();
//                },400);
                if(!this.isCan || HMI.port.checkUsbUart()){
                    this.$message({
                        message: this.$t('cNam.resetTip'),
                        type: 'info'
                    });
                }

            },

            //申请权限
            applyAuth(type) {
                if (!this.brandId) {
                    this.$message({
                        message: this.$t('brandEmptyTip'),
                        type: 'warning'
                    });
                    return
                }
                Core.Api.Tool.applyAuth(this.brandId, 1, type).then(res => {
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
            if(!this.isCan){
                Diagnosis.outTest();
            }
            if(this.reconnectInterval){
                clearInterval(this.reconnectInterval);
            }
            if(this.connectionIntervalHmi){
                clearInterval(this.connectionIntervalHmi);
            }
            HMI.port.setBoxOperation();
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
                    HMI.port.setConnection();
                    HMI.port.setBoxOperation();
                    if(false){
                        vm.reconnectInterval = setInterval(()=>{
                            count++;
                            console.log("Count",count);
                            if(count>1 && HMI.port.isConnected()){
                                vm.connect();
                            }
                            if(count>3 && HMI.port.isConnected()){
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
                                if(count>3 && HMI.port.isConnected()){
                                    console.log("Count stop",count);
                                    clearInterval(vm.reconnectInterval);
                                }
                            }
                        },500);
                    }

                }
//                if(!vm.connectChange){
//
//                }
//                if(oldValue == true){
//                    let time = new Date().getTime();
//                    vm.lastConectChangeTime = time;
//                    if(lastTime-time>1000){
//                    }
//                }
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .hmi {
        width: 100%;
        height: 100%;
        overflow: auto;
        padding: 44px 0;
        .btn-back {
            margin-left: 40px;
            top: -26px;
            position: relative;
        }
        .hmi-content {
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
            .info {
                overflow: hidden;
                background: #353945;
                width: 950px;
                /*min-height: 380px;*/
                margin-top: 22px;
                -moz-box-shadow: 0 1px 0 #2B2F3B;
                /* 老的 Firefox */
                box-shadow: 0 1px 0 #2B2F3B;
                .btn-row {
                    height: 30px;
                    width: 100%;
                    position: relative;
                    line-height: 30px;
                }

                .border-bottom {
                    border-bottom: 1px solid #2B2F3B;
                }
                .info-left {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    border-right: 1px dotted #2B2F3B;
                    .item {
                        margin-left: 33px;
                        height: 50px;
                        line-height: 50px;
                        .label {
                            width: 110px;
                            display: inline-block;
                            line-height: 20px;
                            vertical-align: middle;
                            word-wrap:break-word;
                        }
                        .input {
                            width: 295px;
                            // margin-left: 10px;
                            & > input {
                                height: 29px;
                            }
                        }
                        .input-sn:after {
                            top: 15px;
                        }
                        .el-select {
                            width: 290px;
                        }
                    }
                    .btn-group {
                        position: absolute;
                        right: 40px;
                    }
                    /*.sn {*/
                    /*height: 60px;*/
                    /*margin-left: 33px;*/
                    /*line-height: 60px;*/
                    /*width: 410px;*/
                    /*.label {*/
                    /*width: 90px;*/
                    /*display: inline-block;*/
                    /*}*/
                    /*.sn-input {*/
                    /*width: 150px;*/
                    /*margin-left: 10px;*/
                    /*}*/
                    /*.btn-read {*/
                    /*width: 70px;*/
                    /*margin-left: 7px;*/
                    /*}*/
                    /*.btn-write {*/
                    /*margin-left: 1px;*/
                    /*height: 29px;*/
                    /*width: 70px;*/
                    /*}*/
                    /*}*/
                }
                .info-right {
                    float: left;
                    margin-top: 9px;
                    width: 474px;
                    .item {
                        margin-left: 33px;
                        height: 50px;
                        line-height: 50px;
                        display: flex;
                        align-items: center;
                        .el-select {
                            width: 187px;
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
                        .label {
                            width: 110px;
                            display: inline-block;
                            line-height: 20px;
                            vertical-align: middle;
                            word-wrap:break-word;
                        }
                        .input {
                            width: 295px;
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
                .note-title {
                    margin-top: 10px;
                }
            }
            /*.data {*/
            /*height: 100%;*/
            /*margin-top: 1px;*/
            /*}*/
            .record{
                height:80px;
                line-height: 20px;
                .wrap{
                    .title-record{
                        font-size:16px;
                    }
                    padding:20px 33px;
                    .number{
                        color:#E26829;
                    }
                }
            }
            .tip-dialog {
                font-size: 18px;
                line-height: 28px;
                .el-dialog__body {
                    padding: 0;
                    .txt{
                        font-size: 16px;
                        line-height: 20px;
                    }
                }

            }
        }
    }
</style>



// WEBPACK FOOTER //
// hmi.vue?5ec0dbfa