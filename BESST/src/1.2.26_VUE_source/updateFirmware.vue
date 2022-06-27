<template>
    <div class="updateInstruction">
        <div>
            <!--<h1> Update Steps </h1>-->
            <h1>{{$t('cNam.updateSteps')}}</h1>
        </div>
        <div >
            <p>{{$t('cNam.connectorType')}}</p>
             <!--<p>If the connector has the same type of figure 1, please click "Uart" to check detail of instruction. If the connector has the same type of figure 2, please click "CAN Bus" to check detail of instruction.</p>-->
            <div class="clear-10"></div>
            <div style="border:1px solid #2B2F3B;display:inline-block">
                <img src="../../assets/images/uart.png" width="260"/>
                <i>{{$t('cNam.figureFirst')}}</i>
            </div>
            <div style="margin-left:20px;border:1px solid #2B2F3B;display:inline-block">
                <img src="../../assets/images/can.png" width="260"/>
                <i>{{$t('cNam.figureSecond')}}</i>
            </div>
        </div>
        <div class="clear-10"></div>
        <!--<div>
            How to update software from service tool to component?
        </div>-->
        <div>{{$t('cNam.updateSoftwareMethod')}}</div>
        <el-tabs v-model="activeName" type="card" @tab-click="handleClick">
            <el-tab-pane label="Uart" name="first">
                <div>
                    <div class="title">{{$t('cNam.hmi')}}</div>
                    <div>
                        <p>{{$t('cNam.HMIFirst')}}</p>
                        <!--<p>1. Plug the HMI to service tool.</p>-->
                        <p>{{$t('cNam.HMISecond')}}</p>
                        <!--<p>2. Press the power button of HMI to init update process. If update starting, service tool "Uart" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.HMIThird')}}</p>
                        <!--<p>3. If update is successful, the "Test" LED of service tool turn on.</p>-->
                        <p>{{$t('cNam.HMIForth')}}</p>
                        <!--<p>4. Reset the box to normal status. Otherwise, you would get step 2 repeatedly.</p>-->
                    </div>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div class="title">{{$t('cNam.controller')}}</div>
                    <div>
                        <p>{{$t('cNam.ControllerFirst')}}</p>
                        <!--<p>1. Plug the controller to service tool to init update process. If update starting, service tool "Uart" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.ControllerSecond')}}</p>
                        <!--<p>2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller.</p>-->
                        <p>{{$t('cNam.ControllerThird')}}</p>
                        <!--<p>3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.</p>-->
                    </div>
                </div>
            </el-tab-pane>
            <el-tab-pane label="CAN Bus" name="second">
                <div>
                    <div class="title">{{$t('cNam.hmi')}}</div>
                    <div>
                        <p>{{$t('cNam.HMIFifth')}}</p>
                        <!--<p>1. Plug the HMI to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.HMISixth')}}</p>
                        <!--<p>2. If update is successful, the "Test" LED of service tool turn on. And then unplug the HMI.</p>-->
                        <p>{{$t('cNam.HMISeventh')}}</p>
                        <!--<p>3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.</p>-->
                    </div>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div class="title">{{$t('cNam.controller')}}</div>
                    <div>
                        <p>{{$t('cNam.ControllerForth')}}</p>
                        <!--<p>1. Plug the controller to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.ControllerFifth')}}</p>
                        <!--<p>2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller and detach the battery.</p>-->
                        <p>{{$t('cNam.ControllerSixth')}}</p>
                        <!--<p>3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.</p>-->
                    </div>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div class="title">{{$t('cNam.battery')}}</div>
                    <div>
                        <p>{{$t('cNam.ControllerForth')}}</p>
                        <!--<p>1. Plug the controller to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.ControllerFifth')}}</p>
                        <!--<p>2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller.</p>-->
                        <p>{{$t('cNam.ControllerSixth')}}</p>
                        <!--<p>3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.</p>-->
                    </div>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div class="title">{{$t('cNam.sensor')}}</div>
                    <div>
                        <p>{{$t('cNam.ControllerForth')}}</p>
                        <!--<p>1. Plug the controller to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. </p>-->
                        <p>{{$t('cNam.ControllerFifth')}}</p>
                        <!--<p>2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller.</p>-->
                        <p>{{$t('cNam.ControllerSixth')}}</p>
                        <!--<p>3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.</p>-->
                    </div>
                </div>
            </el-tab-pane>
            <el-tab-pane label="Service Tool" name="third">
                <div class="title">{{$t('cNam.secondVersionTitle')}}</div>
                <!--<div class="title">Besides first version of service tool, other service tool support update software to meet user's new requirements</div>-->
                <div>
                    <div class="title">{{$t('cNam.secondVersion')}}</div>
                    <div>
                        <p>{{$t('cNam.secondVersionFirst')}}</p>
                        <!--<p>1. Probably, you would get the update from online</p>-->
                        <p>{{$t('cNam.secondVersionSecond')}}</p>
                        <!--<p>2. Double click "Update" to download software to service tool and update itself then.
                            During this process,you will see service tool "PC" LED blinking. If "PC" LED keeps on, you need reset service tool to normal. If "Test" LED already turn on, you just need click "Update" once again. </p>-->
                        <p>{{$t('cNam.secondVersionThird')}}</p>
                        <!--<p>3. If update is successful, service tool change to normal state.</p>-->
                    </div>
                </div>
                <div class="clear-10"></div>
                <div>
                    <div class="title">{{$t('cNam.thirdVersion')}}</div>
                    <div>
                        <p>{{$t('cNam.secondVersionFirst')}}</p>
                        <!--<p>1. Probably, you would get the update from online</p>-->
                        <p>{{$t('cNam.thirdVersionFirst')}}</p>
                        <!--<p>2. Click "Update" once to download software to service tool and update itself then.
                        During this process,you will see service tool "PC" LED blinking. If "PC" LED keeps on, you need reset service tool to normal. If "Test" LED already turn on, you need click "Update" again. </p>-->
                        <p>{{$t('cNam.secondVersionThird')}}</p>
                        <!--<p>3. If update is successful, service tool change to normal state.</p>-->
                    </div>
                </div>
            </el-tab-pane>
        </el-tabs>
    </div>
</template>

<script>
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        name: "updateInstruction",
        data(){
            return {
                activeName: ''
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Tool.Update,
                zh: ZH.Components.Tool.Update,
                de: DE.Components.Tool.Update,
                nl: NL.Components.Tool.Update
            }
        },
        created(){
        },
        methods: {
            // 点击标签页
            handleClick(tab, event) {
//                console.log(tab, event);
            }
        },
        computed: {},
        components: {}
    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
    .updateInstruction{
        .title{
            color: #E26829;
        }
    }

</style>



// WEBPACK FOOTER //
// updateFirmware.vue?2be55115