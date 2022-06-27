<template>
  <div class="report-charts">
    <div class="icons go-back">
        <div title="Back" class="icon-item back" @click="back"></div>
    </div>
    <el-form>
        <el-form-item label="开始日期:" class="start-date">
            <el-date-picker
              v-model="startDate"
              type="datetime"
              placeholder="选择日期时间"
              default-time="00:00:00">
            </el-date-picker>
        </el-form-item>
        <el-form-item label="结束日期:">
          <el-date-picker
              v-model="endDate"
              type="datetime"
              placeholder="选择日期时间"
              default-time="00:00:00">
            </el-date-picker>
        </el-form-item>
        <el-form-item label="规格:">
            <el-input v-model="specification"></el-input>
        </el-form-item>
        <el-form-item>
            <el-button @click="searchReportCharts" icon="search">search</el-button>
        </el-form-item>
    </el-form>
    <div class="echarts">
        <IEcharts
          :option="bar"
          :loading="loading"
          @ready="onReady"
          @click="onClick"
        >
      </IEcharts>
    <!--<button @click="doRandom">Random</button>-->
    </div>
  </div>
</template>

<script type="text/babel">
  import IEcharts from 'vue-echarts-v3/src/full.js';
  export default {
    components: {
      IEcharts
    },
    props: {
    },
    data(){
      return{
        startDate: '',
        endDate: '',
        specification: '',
        xAxisData:[],
        seriesData: [],
        loading: true,
        bar: {
          color:['#7fb80e','#aa2116'],
          title: {
            text: 'report charts',
            textStyle:{
                color: '#fff'
            }
          },
          tooltip : {
            trigger: 'axis',
            axisPointer : {            // 坐标轴指示器，坐标轴触发有效
                type : 'shadow'        // 默认为直线，可选为：'line' | 'shadow'
            }
          },
          legend: {
              data: [],
              textStyle:{
                color: '#fff'
              }
          },
          grid: {
            left: '3%',
            right: '4%',
            bottom: '3%',
            containLabel: true
          },
          xAxis:  {
            type: 'category',
            data: [],
            axisLabel: {
              show:true,
              rotate: 30,
              inside: false,
              interval: 0,
              textStyle: {
                  color: '#fff',
                  fontSize: 12
              },
              width: 100,
              triggerEvent: true,
            },
            axisTick: {
              show: true,
              lineStyle:{
                  width: 1
              },
            },
            axisLine: {
              show: true
            },
            z: 20,
          },
          yAxis: {
              type: 'value',
              axisLabel: {
                show:true,
                inside: false,
                interval: 0,
                textStyle: {
                    color: '#fff',
                    fontSize: 12
                },
              width: 100
            },
          },
          series: []
        }
      }
    },
    methods: {
      
      back() {
          this.$router.go(-1);
      },
      doRandom(){
        const that = this;
        let data = [];
        for (let i = 0, min = 5, max = 99; i < 6; i++) {
          data.push(Math.floor(Math.random() * (max + 1 - min) + min));
        }
        that.loading = !that.loading;
        that.bar.series[0].data = data;
      },
      onReady(instance, ECharts) {
        console.log(instance, ECharts);
      },
      onClick(event, instance, ECharts) {
        console.log(arguments);
      },
      searchReportCharts(){
        this.getReportChartsData();
      },
      getReportChartsData(){
        this.bar.legend.data = [];
        this.bar.xAxis.data = [];
        this.bar.series = [];
        this.loading = true;
        const inspectorRecordId =  '',
            sn = this.specification?this.specification:this.$route.query.SN,
            testPlanId = this.$route.query.testPlanReportId?this.$route.query.testPlanReportId:'',
            uuid = '',
            startOf = this.startDate?new Date(this.startDate).getTime():0,
            endOf = this.endDate?new Date(this.endDate).getTime():0,
            startIndex = 0,
            itemsPerPage = this.$route.query.totalSizes,
            decisionResult = true;
        this.$http.get(
            `https://www.bafang-electric.com:7777/api/diagnostics/report/export/json?inspectorRecordId=${inspectorRecordId}&testPlanId=${testPlanId}&uuid=${uuid}&startOf=${startOf}&endOf=${endOf}&startIndex=${startIndex}&itemsPerPage=${itemsPerPage}&decisionResult=${decisionResult}&sn=${sn}`,
            {headers:{
            jwt:this.$route.query.token
        }}).then((res)=>{
                if(res&&res.data&&res.data.data&&res.data.data.length>0){
                  let testUnitInfo = {};
                  res.data.data.forEach((item)=>{
                    if(Object.keys(testUnitInfo).length>0){
                      let differenceList = 0;
                      Object.keys(testUnitInfo).forEach((testUnitItem)=>{
                         if(testUnitItem !== item['工况名称']){
                            differenceList++;
                          }
                      })
                      if(differenceList==0){
                        if(item['判定结果']===true || item['判定结果']===false){
                          testUnitInfo[item['工况名称']] = item['判定结果'].toString();
                        } 
                      }else{
                        if(testUnitInfo[item['工况名称']]){
                          if(item['判定结果']===true || item['判定结果']===false){
                            testUnitInfo[item['工况名称']] +=(','+item['判定结果'].toString());
                          }
                        }else{
                          if(item['判定结果']===true || item['判定结果']===false){
                          testUnitInfo[item['工况名称']] = item['判定结果'].toString();
                          }
                        }
                      }
                    }else{
                      if(item['判定结果']===true || item['判定结果']===false){
                        testUnitInfo[item['工况名称']] = item['判定结果'].toString();
                      }
                    }
                  })
                  this.bar.legend.data = ['合格','不合格'];
                  let successItem = {
                      name: '合格',
                      type: 'bar',
                      stack: '总量',
                      barWidth: 50,
                      label: {
                          normal: {
                              show: true,
                              position: 'insideRight'
                          }
                      },
                      data: []
                    },
                    failItem = {
                      name: '不合格',
                      stack: '总量',
                      type: 'bar',
                      barWidth: 50,
                      label: {
                          normal: {
                              show: true,
                              position: 'insideRight'
                          }
                      },
                      data: []
                    };
                    Object.keys(testUnitInfo).forEach((item)=>{
                      let isSuccess = 0,
                      isFail = 0;
                      testUnitInfo[item].split(',').forEach((testItem)=>{
                        if(testItem=='true'){
                          isSuccess++;
                        }else{
                          isFail++;
                        }
                      })
                      successItem.data.push(isSuccess);
                      failItem.data.push(isFail);
                    })
                    this.bar.series.push(successItem);
                    this.bar.series.push(failItem);
                    Object.keys(testUnitInfo).forEach(item=>{
                      this.bar.xAxis.data.push(item);
                    })
                    this.loading = false;
                }
            })
        }
    },
    mounted() {
        this.getReportChartsData();
    },
  };
</script>

<style scoped lang="scss">
  .report-charts{
    .echarts {
      width: 95%;
      height: 520px;
      margin-left: 5%;
      over-flow: auto;
    }
    .el-form{
        display: flex;
        flex-wrap: wrap;
        .el-form-item{
            flex: 1;
            margin-left: 2%;
            .el-form-item__content{
                display: flex;
                .el-form-item__label{
                    width: 120px;
                }
                .el-input{
                    margin-right: 20px;
                    flex:1;
                    width:200px;
                }
            }
        }
        .start-date{
          margin-left: 60px;
        }
    }
    .icons.go-back{
      display: flex;
      justify-content: flex-start;
      margin-left: 40px;
    }
    .x-axis-tip {
      display: none;
      position: absolute;
      padding: 5px 5px;
      font-size: 12px;
      line-height: 18px;
      color: #575757;
      background: #FFFFFF;
      box-shadow: 0 1px 4px 0 rgba(0,0,0,0.20);
      border-radius: 4px;
      max-width: 280px;
      white-space: normal;
      word-break: break-all;
    }
  }
  
</style>


// WEBPACK FOOTER //
// reportCharts.vue?983e0744