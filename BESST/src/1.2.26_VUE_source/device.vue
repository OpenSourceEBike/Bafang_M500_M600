<template>
	<div >
    <el-button @click="listPort()">list port</el-button>
    <el-button @click="connect()">connect</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="toolWriteSn()">tool write-sn</el-button>
    <el-button @click="toolGetFullData()">tool cmd-get-full-data</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="batteryConnect()">battery connect</el-button>
    <el-button @click="batteryGetInnerTemp()">battery cmd-get-inner-temp</el-button>
    <el-button @click="batteryGetFullData()">battery cmd-get-full-data</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="ctrlConnect()">ctrl connect</el-button>
    <el-button @click="ctrlGetFullData()">ctrl cmd-get-full-data</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="hmiConnect()">hmi connect</el-button>
    <el-button @click="hmiGetFullData()">hmi cmd-get-full-data</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="diagnose()">diagnose</el-button>
    <br />
    <hr />
    <br />
    <el-button @click="testBatchRead()">testBatchRead</el-button>
  </div>


</template>

 <script type="text/ecmascript-6">

  import Tool from "../../../device/tool"
  import Battery from "../../../device/battery"
  import Controller from "../../../device/controller"
  import HMI from "../../../device/hmi"
  import Diagnose from "../../../device/diagnose"


	export default {
    name: 'device',
    data() {
      return {

      }
    },
    methods: {
      listPort: function(){
        Tool.port.getPortList().subscribe(portList => {
          console.log(portList);
        })
      },

      connect: function(){
        let bus = Tool.connect('COM3');
        bus.subscribe(item => {
          console.log(item);
        });
      },

      toolWriteSn: function() {
        Tool.writeSn('123');
      },

      toolGetFullData: function(){
        Tool.sendCmdGetFullData();
      },





      batteryConnect: function(){
        let bus = Battery.connect('COM3');
        bus.subscribe(item => {
          console.log(item);
        });
      },

      batteryGetInnerTemp: function(){
        Battery.sendCmdGetInnrTemp();
      },

      batteryGetFullData: function(){
        Battery.sendCmdGetFullData();
      },




      ctrlConnect: function(){
        let bus = Controller.connect('COM3');
        bus.subscribe(item => {
          console.log(item);
        });
      },

      ctrlGetFullData: function(){
        Controller.sendCmdGetFullData();
      },

      hmiConnect() {
        let bus = HMI.connect('COM3');
        bus.subscribe(item => {
          console.log(item);
        });
      },

      hmiGetFullData() {
          HMI.sendCmdGetFullData();
      },


      diagnose() {
        let bus = Diagnose.connect('COM3');
        bus.subscribe(res => {
          console.log('view', res);
          if (res.type == 'port') {
            let portName = res.data.name
            let status = res.data.status
            console.log('connect', portName, status);
          }
        });
      },

      testBatchRead() {
          if (!HMI.port.isConneted()) {
            console.log('disconnect');
            return;
          }

      }

    }

  }
</script>

<style  lang="scss" rel="stylesheet/scss" >


</style>



// WEBPACK FOOTER //
// device.vue?7d738662