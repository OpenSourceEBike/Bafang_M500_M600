<template>
    <div class="test-plan">
        <!--<el-date-picker
        v-model="value1"
        type="datetime"
        placeholder="选择日期时间">
        </el-date-picker>-->
        <div class="test-plan-header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item label="测试计划:">
                     <el-input></el-input>
                </el-form-item>
                <el-form-item label="类别:">
                    <el-input></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button @click="searchTestPlan" icon="search">search</el-button>
                </el-form-item>
                <el-form-item>
                    <el-button @click="showInspectorRecord">返工记录</el-button>
                </el-form-item>
            </el-form>
        </div>
        <div class="test-plan-content">
             <el-table
                :data="testPlanList"
                style="width: 100%">
                <el-table-column  v-for="(item, index) in headerData" :key="index" :prop="item.name"  :label="item.label"></el-table-column>
                <el-table-column
                    fixed="right"
                    label="操作"
                    >
                    <template slot-scope="scope">
                        <el-button
                        @click.native.prevent="showTestReport(scope.$index, testPlanList)"
                        type="text"
                        size="small">
                        测试报告
                        </el-button>
                        <el-button
                        @click.native.prevent="showDetail(scope.$index, testPlanList)"
                        type="text"
                        size="small">
                        详情
                        </el-button>
                    </template>
                </el-table-column>
            </el-table>
        </div>
    </div>
</template>
<script>

    export default{
        data(){
            return{
                testPlanList: [],
                testPlanToken: '',
                headerData: [
                    {
                        name: 'name',
                        label: '名称'
                    },
                    {
                        name: 'category',
                        label: '类别'
                    },
                    {
                        name: 'sub_category',
                        label: '子目录'
                    },
                ],
                value1: ''
            }
        },
        methods:{
            searchTestPlan(){

            },
            showDeleteDialog(){

            },
            showInspectorRecord(){
                this.$router.push({
                    name: 'inspectorRecord',
                    query:{
                        testPlanToken: this.testPlanToken
                    }
                })
            },
             getTestPlanToken(){
                this.$http.get(
                    'https://www.bafang-electric.com:7777/api/login?username=admin&password=Jimmy007'
                ).then(res=>{
                    console.log(res.data);
                    if(res.data&&res.data.jwt){
                        this.testPlanToken = res.data.jwt;
                        this.getTestPlanData();
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },
            showTestReport(index,data){
                this.$router.push({
                    name: 'testPlanReport',
                    query: {
                        testPlanReportId: data[index].testPlanId,
                        token: this.testPlanToken
                    }
                })
            },
            showDetail(){

            },
            getTestPlanData(){
                this.$http.get(
                    'https://www.bafang-electric.com:7777/api/testplans',
                    {
                        headers:{
                            jwt: this.testPlanToken
                        }
                    }
                ).then(res=>{
                    console.log(res.data);
                    if(res.data&&res.data.data&&res.data.data.length>0){
                        this.testPlanList = res.data.data;
                    }
                }).catch(err=>{
                    console.log(err);
                })
            }
        },
        mounted(){
            console.log(this.$route.query);
            this. getTestPlanToken();

        }
    }
</script>
<style lang="scss">
.test-plan{
    .demo-form-inline .el-form-item__label {
        float: left;
    }

    .el-form--inline .el-form-item {
        /*width: 30%;
        margin-left: 20px;*/
    }

    .el-form {
        padding-top: 20px;
    }

    .project-btn {
        display: flex;
        justify-content: flex-end;
        // padding: 20px 0;
    }

    .save-project {
        margin-right: 15%;
    }

    .el-form--inline .el-form-item__label {
        width: 100px;
    }

    .icons {
        display: block;
    }
    .select-param{
        width: 800px;
    }
    .show-el-collapse{
        border-bottom: 1px solid black;
        .category-sn{
            display: flex;
            margin-top: 40px;
            label{
                margin-left: 15%;
                color: #A1A5B1;
                flex: 1;
            }
            .el-input{
                margin-left: 10px;
                width: 400px;
            }
        }
        .el-collapse-item{
            border-bottom: 1/px solid black;
        }
        .el-collapse-item__header{
            display: flex;
            align-items: center;
            padding-top: 20px;
            padding-bottom: 20px;
            // span{
            //     width: 60px;
            // }
            img{
                width: 20px;
                height: 20px;
                margin-right:12px;
            }
        }
        .el-collapse-item__content{
            height: 240px;
            // overflow: auto;
            .add-new-param{
                display: flex;
                justify-content: flex-end;
                padding: 10px 0;
                .add-param-btn{
                    margin-right: 15%;
                }
            }
            .el-form{
                // padding-top: 0;
                display: flex;
                flex-wrap: wrap;
                .el-form-item{
                    flex: 0 0 30%;
                    margin-left: 2%;
                    .el-form-item__content{
                        display: flex;
                        .el-form-item__label{
                            width: 120px;
                        }
                        .el-input{
                            margin-right: 20px;
                            flex:1;
                        }
                    }
                }
            }
        }
    }
    .el-radio__label{
        color: cyan;
    }
    .select-phrase{
        display: flex;
        flex-wrap: wrap;
        .el-radio{
            flex: 0 0 30%;
            margin-left:6px!important;
            margin-bottom: 10px;
        }
    }
    .add-param-dialog{
        .el-input{
            width: 240px!important;
        }
        .save-param{
            display: flex;
            justify-content: flex-end;
            .el-button{
                margin-right: 60px;
            }
        }
    }
    .icons.go-back{
        display: flex;
        justify-content: flex-start;
        margin-left: 40px;
    }
}

</style>



// WEBPACK FOOTER //
// testPlan.vue?d28ffb7e