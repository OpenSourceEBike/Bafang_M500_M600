<template>
    <div class="inspector-record">
        <div class="icons go-back" @click="back"><div title="Back" class="icon-item back"></div></div>
        <div class="test-plan-header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item label="SN:">
                     <el-input v-model="SN" placeholder="please input SN"></el-input>
                </el-form-item>
                <el-form-item>
                    <div class="btn-group-inline-icon">
                        <div class="icons">
                            <div class="icon-item search" @click="searchInspectorRecord"  title="search"></div>
                        </div>
                    </div>
                </el-form-item>
                <el-form-item>
                    <el-button @click="showInspectorRecordDetail">show detail</el-button>
                </el-form-item>
            </el-form>
        </div>
        <div class="test-plan-content">
             <el-table
                :data="inspectorRecordList"
                style="width: 100%"
                max-height="480">
                <el-table-column align="center" width="300"  v-for="(item, index) in inspectorRecordHeadData" :key="index" :prop="item.name"  :label="item.label"></el-table-column>
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
                currentPage4: 1,
                totalSizes:0,
                pageSize: 10,
                SN: '',
                inspectorRecordList: [],
                testPlanToken: '',
                inspectorRecordHeadData: [],
                userNameList: []
            }
        },
        methods:{
            showInspectorRecordDetail(){

            },
            searchInspectorRecord(){
                let startIndex = this.currentPage4;
                this.handleCurrentChange(startIndex);
            },
             handleSizeChange(val) {
                console.log(`每页 ${val} 条`);
            },
            handleCurrentChange(val) {
               let data = {}
               if(this.SN){
                   data.sn = this.SN;
               }
               let sort = 'asc';
               this.$http.post(
                    `https://www.bafang-electric.com:7777/api/inspector/record/filter?startIndex=${val}&itemsPerPage=${this.pageSize}&sort=${sort}`,data,
                    {
                        headers: {
                            jwt: this.$route.query.testPlanToken
                        }
                    }
                ).then(res=>{
                    if(res.status === 200){
                        if(res.data.data&&res.data.data.length>0){
                            this.inspectorRecordList = [];
                            this.inspectorRecordHeadData = [];
                            this.totalSizes = res.data.total;
                            this.inspectorRecordList = res.data.data;
                            this.inspectorRecordList.forEach((item)=>{
                                item['createdOn'] = new Intl.DateTimeFormat('zh', {
                                        year: 'numeric',
                                        month: '2-digit',
                                        day: '2-digit',
                                        hour: '2-digit',
                                        minute: '2-digit',
                                        second: '2-digit',
                                        hour12: false
                                }).format(new Date(item['createdOn'])).replace(/\//g,'-');
                                if(item['createdBy']){
                                    this.userNameList.forEach((userNameItem)=>{
                                        if(userNameItem['_id'] ==item['createdBy']){
                                            item['createdBy'] = userNameItem.username;
                                        }
                                    })
                                }
                             })
                            let keys = Object.keys(res.data.data[0]).filter((item)=>{
                                return item !='__v'&& item !='_id';
                            })
                            keys.forEach((item)=>{
                                let headItem = {};
                                headItem.name = item;
                                switch(item){
                                    case 'createdOn':
                                        headItem.label="创建时间";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'createdBy':
                                        headItem.label="创建人";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'itemId':
                                        headItem.label="itemId";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'sn':
                                        headItem.label="SN";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                }

                            })
                        }
                    }
                }).catch(err=>{
                    console.log(err);
                    this.$message({
                        type: 'warning',
                        message: '查询失败'
                    })
                })
            },
            back() {
                this.$router.go(-1);
            },
            showDeleteDialog(){

            },
            showInspectorRecord(){

            },
            showDetail(){

            },
            getUserNameByUserId(userId){
                 this.$http.get(
                    'https://www.bafang-electric.com:7777/api/users',
                    {
                        headers: {
                            jwt: this.$route.query.testPlanToken
                        }
                    }
                 ).then(res=>{
                     if(res.data && res.data.data&&res.data.data.length>0){
                        this.userNameList = res.data.data;
                        this.getInspectorRecord();
                     }
                 }).catch(err=>{
                     console.log(err);
                 })
            },
            getInspectorRecord(){
                this.$http.get(
                    'https://www.bafang-electric.com:7777/api/inspector/record',
                    {
                        headers: {
                            jwt: this.$route.query.testPlanToken
                        }
                    }
                ).then(res=>{
                    if(res.status === 200){
                        if(res.data.data&&res.data.data.length>0){
                            this.totalSizes = res.data.total;
                            this.inspectorRecordList = res.data.data;
                            this.inspectorRecordList.forEach((item)=>{
                                item['createdOn'] = new Intl.DateTimeFormat('zh', {
                                        year: 'numeric',
                                        month: '2-digit',
                                        day: '2-digit',
                                        hour: '2-digit',
                                        minute: '2-digit',
                                        second: '2-digit',
                                        hour12: false
                                }).format(new Date(item['createdOn'])).replace(/\//g,'-');
                                if(item['createdBy']){
                                    this.userNameList.forEach((userNameItem)=>{
                                        if(userNameItem['_id'] ==item['createdBy']){
                                            item['createdBy'] = userNameItem.username;
                                        }
                                    })
                                }
                             })
                            let keys = Object.keys(res.data.data[0]).filter((item)=>{
                                return item !='__v'&& item !='_id';
                            })
                            keys.forEach((item)=>{
                                let headItem = {};
                                headItem.name = item;
                                switch(item){
                                    case 'createdOn':
                                        headItem.label="创建时间";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'createdBy':
                                        headItem.label="创建人";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'itemId':
                                        headItem.label="itemId";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                    case 'sn':
                                        headItem.label="SN";
                                        this.inspectorRecordHeadData.push(headItem);
                                        break;
                                }

                            })
                        }
                    }
                }).catch(err=>{
                    console.log(err);
                    this.$message({
                        type: 'warning',
                        message: '查询失败'
                    })
                })
            }
        },
        mounted(){
            this.getUserNameByUserId();
        }
    }
</script>
<style lang="scss">
    .inspector-record{
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
// inspectorRecord.vue?cfe2905e