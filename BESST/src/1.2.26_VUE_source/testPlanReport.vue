<template>
    <div class="test-plan-report">
        <div>
            <div class="icons go-back">
                <div title="Back" class="icon-item back" @click="back"></div>
            </div>
            <el-form class="search-report">
                <el-form-item label="SN:" class="sn-item">
                    <el-input v-model="SN" placeholder="please input SN"></el-input>
                </el-form-item>
                <el-form-item class="search-item">
                    <el-button @click="searchTestPlanReport">search</el-button>
                </el-form-item>
                <el-form-item class="search-item">
                    <el-button @click="exportTestPlanReport">导出测试报告</el-button> 
                </el-form-item>
                <el-form-item class="search-item">
                    <el-button @click="showReportCharts">report charts</el-button>
                </el-form-item>
            </el-form>
        </div>
       <div class="test-plan-report">
            <el-table :data="reportData" 
                style="width: 100%;margin-top: 20px;"
                max-height="520"
                v-if="reportData.length>0">
                <el-table-column
                    label="ItemId"
                    width="100">
                    <template slot-scope="scope">
                        <span>{{scope.row['ItemId']}}</span>
                    </template>
                </el-table-column>
                <el-table-column
                    label="时间"
                    width="110">
                    <template slot-scope="scope">
                        <span>{{scope.row['时间']}}</span>
                    </template>
                </el-table-column>
                <el-table-column
                    label="SN"
                    width="280">
                    <template slot-scope="scope">
                        <span>{{scope.row['SN']}}</span>
                    </template>
                </el-table-column>
                <el-table-column width="180"  v-for="(item, index) in reportHeaderData" :key="index" :prop="item.name"  :label="item.label"></el-table-column>
                <el-table-column
                label="判定结果"
                width="100">
                    <template slot-scope="scope">
                        <span :style="{'color': (scope.row['判定结果']=='true' ? '#00ff00':'#ff0000')}">{{scope.row['判定结果']}}</span>
                    </template>
                </el-table-column>
            </el-table>
            <el-pagination
                @current-change="handleCurrentChange"
                :current-page="currentPage4"
                :page-size="pageSize"
                layout="prev, pager, next,total"
                hide-on-single-page=true
                :total="totalSizes">
            </el-pagination>
       </div> 
    </div>
</template>
<script>
    export default{
        data(){
            return{
                testPlanReportURL: '',
                downloadName: '',
                reportHeaderData: [],
                reportData: [],
                currentPage4: 1,
                totalSizes:0,
                pageSize: 10,
                SN: ''
            }
        },
        methods:{
            searchTestPlanReport(){
                const inspectorRecordId =  '',
                    sn = this.SN,
                    testPlanId = this.$route.query.testPlanReportId?this.$route.query.testPlanReportId:'',
                    uuid = '',
                    startOf = 0,
                    endOf = 0,
                    startIndex = (this.currentPage4-1)*10,
                    itemsPerPage = this.pageSize,
                    decisionResult = true;
                this.getReportData(inspectorRecordId,testPlanId,uuid,startOf,endOf,startIndex,itemsPerPage,decisionResult,sn);
                // this.currentPage4 = 1;
            },
            handleCurrentChange(val) {
                const inspectorRecordId =  '',
                    sn = this.SN,
                    testPlanId = this.$route.query.testPlanReportId?this.$route.query.testPlanReportId:'',
                    uuid = '',
                    startOf = 0,
                    endOf = 0,
                    startIndex = (val-1)*10,
                    itemsPerPage = this.pageSize,
                    decisionResult = true;
                this.getReportData(inspectorRecordId,testPlanId,uuid,startOf,endOf,startIndex,itemsPerPage,decisionResult,sn);
            },
            showReportCharts(){
                if(this.SN){
                    this.$router.push({
                        name: 'reportCharts',
                        query: {
                            token: this.$route.query.token,
                            totalSizes: this.totalSizes,
                            testPlanReportId: this.$route.query.testPlanReportId,
                            SN: this.SN
                        }
                    })
                }else{
                    this.$message({
                        type: 'warning',
                        message: 'please input SN at first'
                    });
                    return false;
                }
                
            },
            back() {
                this.$router.go(-1);
            },
            formatJson(filterVal, jsonData) {
                return jsonData.map(v => filterVal.map(j => v[j]))
            },
            getReportData(inspectorRecordId,testPlanId,uuid,startOf,endOf,startIndex,itemsPerPage,decisionResult,sn){
                this.$http.get(
                    `https://www.bafang-electric.com:7777/api/diagnostics/report/export/json?inspectorRecordId=${inspectorRecordId}&testPlanId=${testPlanId}&uuid=${uuid}&startOf=${startOf}&endOf=${endOf}&startIndex=${startIndex}&itemsPerPage=${itemsPerPage}&decisionResult=${decisionResult}&sn=${sn.replace(/\s/g,'')}`,
                    {headers:{
                    jwt:this.$route.query.token
                }}).then((res)=>{
                    if(res&&res.data&&res.data.data&&res.data.data.length>0){
                         this.reportHeaderData = [];
                        this.reportData = [];
                        this.totalSizes = 0;
                        for(var i in res.data.data[0]){
                            if(i!=='判定结果' && i!='时间' && i!='ItemId'&&i!='UUID'&&i!='SN'&&i!=='判定结果2'){
                                let reportHeadItem = {
                                    name: i,
                                    label: i
                                }
                                this.reportHeaderData.push(reportHeadItem);
                            }
                           
                        }
                        res.data.data.forEach((item)=>{
                            for(var i in item){
                                if(i === '判定结果'){
                                    item[i] = item[i].toString();
                                }
                            }
                        })
                        this.reportData = res.data.data;
                        this.totalSizes = res.data.total;
                    }
                })
            },
            getTestPlanReport(){
                const inspectorRecordId =  '',
                    sn = this.SN,
                    testPlanId = this.$route.query.testPlanReportId?this.$route.query.testPlanReportId:'',
                    uuid = '',
                    startOf = 0,
                    endOf = 0,
                    startIndex = this.currentPage4,
                    itemsPerPage = this.pageSize,
                    decisionResult = true;
                this.getReportData(inspectorRecordId,testPlanId,uuid,startOf,endOf,startIndex,itemsPerPage,decisionResult,sn);
            },
            exportTestPlanReport(){
                const inspectorRecordId =  '',
                    sn = this.SN,
                    testPlanId = this.$route.query.testPlanReportId?this.$route.query.testPlanReportId:'',
                    uuid = '',
                    startOf = 0,
                    endOf = 0,
                    startIndex = 0,
                    itemsPerPage = this.totalSizes,
                    decisionResult = true;
                this.$http.get(
                    `https://www.bafang-electric.com:7777/api/diagnostics/report/export/json?inspectorRecordId=${inspectorRecordId}&testPlanId=${testPlanId}&uuid=${uuid}&startOf=${startOf}&endOf=${endOf}&startIndex=${startIndex}&itemsPerPage=${itemsPerPage}&decisionResult=${decisionResult}&sn=${sn}`,
                    {headers:{
                    jwt:this.$route.query.token
                }}).then((res)=>{
                    if(res&&res.data&&res.data.data&&res.data.data.length>0){
                         res.data.data.forEach((item)=>{
                            for(var i in item){
                                if(i.indexOf('判定结果')>-1){
                                    item[i] = item[i].toString();
                                }
                            }
                        })
                        var tableHtml='<html><head><meta charset="UTF-8"></head><body>';
                        var lHtml = "<table border='1' style='width: 750px;height: 242px; border:1px solid ;border-collapse: collapse;border-color:#000;background-color:white;' align='center'><thead><tr>";
                        let keys = Object.keys(res.data.data[0]);
                        keys.forEach((item,index)=>{
                            if(index<keys.length-1){
                                lHtml += `<th>${item}</th>`;
                            }else{
                                lHtml += `<th>${item}</th></tr><thead><tr>`;
                            }
                        })
                        res.data.data.forEach((item,index)=>{
                            for(var i in item){
                                if(index<res.data.data.length-1){
                                     if(i == Object.keys(item)[Object.keys(item).length-1]){
                                        lHtml += `<td>${item[i]}</td></tr><tr>`;
                                     }else{
                                        lHtml += `<td>${item[i]}</td>`;
                                     }
                                }else{
                                     if(i == Object.keys(item)[Object.keys(item).length-1]){
                                         lHtml += `<td>${item[i]}</td></tr>测试报告</table>`;
                                     }else{
                                        lHtml += `<td>${item[i]}</td>`;
                                     }
                                }
                            }
                        })
                        tableHtml += lHtml;
                        tableHtml += '</body></html>';
                        var excelBlob = new Blob([tableHtml], {type: 'application/vnd.ms-excel'});
                        var fileName = "测试报告"+".xls";
                        var oa = document.createElement('a');
                        oa.href = URL.createObjectURL(excelBlob);
                        oa.download = fileName;
                        document.body.appendChild(oa);
                        oa.click();
                    }
                })
            }
        },
        mounted(){
            if(this.$route.query&&this.$route.query.testPlanReportId){
                this.getTestPlanReport();
            }
        }
    }
</script>
<style lang="scss">
    .test-plan-report{
        .icons.go-back{
            display: flex;
            justify-content: flex-start;
            margin-left: 40px;
        }
        .el-table .fail-row {
            background: oldlace;
        }

        .el-table .success-row {
            background: #f0f9eb;
        }
        .search-report{
            // margin-top: -30px;
            display: flex;
            .el-form-item{
                display: flex;
            }
            .sn-item{
                margin-left: 100px;
            }
            .search-item{
                margin-left: 60px;
            }
        }  
    }
   
</style>


// WEBPACK FOOTER //
// testPlanReport.vue?34b2a74b