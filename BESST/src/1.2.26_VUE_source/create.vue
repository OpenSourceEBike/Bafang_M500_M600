<template>
    <div class="create-project">
        <h1 style="text-align: center;padding-top: 10px;">客户项目建档</h1>
        <div class="icons go-back" @click="back"><div title="Back" class="icon-item back"></div></div>
        <el-form :inline="true" class="demo-form-inline">
            <el-form-item label="客户名称:" >
                 <el-input v-model="customerNameInput" placeholder="Customer Name" clearable></el-input>
             </el-form-item>
             <el-form-item label="客户代号:" >
                 <el-input v-model="customerNumberInput" placeholder="Customer No." clearable></el-input>
             </el-form-item>
             <el-form-item label="项目:">
                <el-select v-model="selectedSystem" placeholder="请选择" @change="getSystem">
                    <el-option
                    v-for="(item,index) in systemList"
                    :key="index"
                    :label="item.name"
                    :value="item.value"></el-option>
                </el-select>
             </el-form-item>
            <el-form-item label="创建日期:" >
                 <el-input v-model="customerCreateDate" placeholder="Create Date" clearable></el-input>
             </el-form-item>
             <el-form-item label="客户订单号:" >
                 <el-input v-model="customerOrderNumber" placeholder="Create Order No." clearable></el-input>
             </el-form-item>
            <el-form-item>
                <div class="btn-group-inline-icon">
                    <div class="icons">
                        <div class="icon-item search" @click="onSearchClientInfo"  title="search"></div>
                    </div>
                </div>
            </el-form-item>
            <el-form-item>
                <el-button @click="saveClientOrder" style="margin-left: 20px;">save</el-button>
            </el-form-item>
        </el-form>
      <el-table
        :data="tableData"
        style="width: 100%"
        max-height="250">
        <el-table-column v-for="(item, index) in headData" :key="index" :prop="item.name"  :label="item.label"></el-table-column>
        <el-table-column
        fixed="right"
        label="操作"
        >
        <template slot-scope="scope">
            <el-button
            @click.native.prevent="showDeleteDialog(scope.$index, tableData)"
            type="text"
            size="small">
            移除
            </el-button>
            <el-button
            @click.native.prevent="editRow(scope.$index, tableData)"
            type="text"
            size="small">
             编辑
            </el-button>
            <el-button
            @click.native.prevent="toProduct(scope.$index, tableData)"
            type="text"
            size="small">
             生产
            </el-button>
        </template>
    </el-table-column>
</el-table>
<el-dialog
    :title="$t('Delete Project Item')"
    :visible.sync="dialogDeleteVisible"
    size="tiny">
    <span>{{ $t('Are you sure delete current item?') }}</span>
    <span slot="footer" class="dialog-footer">
        <el-button type="preview" @click="deleteProjectItem">{{ $t('confirm') }}</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">{{ $t('cancel') }}</el-button>
    </span>
</el-dialog>
</div>

</template>
<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        data() {
            return {
                systemList: [],
                projectNumberInput: '',
                projectNameInput: '',
                customerNameInput: '',
                customerCreateDate:'',
                customerNumberInput: '',
                customerOrderNumber: '',
                headData: [
                    {
                        name: 'code',
                        label: '客户代号'
                    },
                    {
                        name: 'client_order_no',
                        label: '客户订单号'
                    },
                    {
                        name: 'name',
                        label: '客户名'
                    },
                    {
                        name: 'project_code',
                        label: '系统'
                    }
                    // {
                    //     name: 'charger',
                    //     label: '充电器'
                    // },
                    // {
                    //     name: 'serviceTool',
                    //     label: '服务工具'
                    // },
                ],
                options: [{
                    value: '0',
                    name: '驱动系统',
                }, {
                    value: '1',
                    name: '零部件'
                }],
                projectType: '',
                motor: '',
                controller: '',
                HMI: '',
                battery: '',
                sensor: '',
                charger: '',
                serviceTool: '',
                attachment: '',
                tableData: [],
                dialogDeleteVisible: false,
                // deleteProjectItem: false,
                selectItemID: '',
                selectedSystem: ''
            }
        },
        methods: {
            toProduct(){

            },
            back() {
                this.$router.go(-1);
            },
            saveClientOrder(){
                this.customerNameInput = this.customerNameInput?this.customerNameInput: this.$route.query.customerName;
                this.customerNumberInput = this.customerNumberInput?this.customerNumberInput: this.$route.query.customerNumber;
                let data = {
                    "client": {
                        "address": this.$route.query.address,
                        "code": this.customerNumberInput,
                        "create_time": 0,
                        "create_user_id": 0,
                        "id": 0,
                        "name": this.customerNameInput,
                        "phone": 0,
                        "update_time": 0,
                        "update_user_id": 0,
                        "weight": 0
                    },
                    "clientOrder": [
                        {
                        "client_id": 0,
                        "client_order_no": 224,
                        "create_time": 0,
                        "create_user_id": 0,
                        "id": 0,
                        "production_order_id": 3,
                        "project_id": this.selectedSystem,
                        "update_time": 0,
                        "update_user_id": 0,
                        "weight": 0
                        }
                    ]
                }
                Core.Api.request({
                    url: '/clientOrder/save',
                    method: 'POST',
                    headers: {
                        'content-type': 'x-www-form-url-encoded'
                    },
                    data: JSON.stringify(data)
                }).then(res=>{
                    console.log(res);
                }).catch(err=>{
                    console.log(err);
                })
            },
            getSystem(val){

            },
            onSearchClientInfo(){
                console.log(1);
            },
             showDeleteDialog(store, data) {
                this.selectItemID = data[store].id;
                this.dialogDeleteVisible = true;
            },
            createProject() {
                this.$router.push({
                    name: 'clientCreate'
                })
            },
            // deleteRow(index, rows) {
            //     rows.splice(index, 1);
            // },
            editRow(index, rows) {
                this.$router.push({
                    name: 'clientCreate',
                    query: {
                        id: rows[index].id
                    }
                })
            },
            showDetail(index,rows){
                this.$router.push({
                    name: 'systemDetail',
                    query: {
                        id: rows[index].id
                    }
                })
            },
            searchProject(val) {

            },
            getProjectType() {
                console.log(this.projectType)
            },
            deleteProjectItem(){
                let token = Core.Data.getToken(),
                    id = this.selectItemID,
                    that = this;
                Vue.http.post(
                  Core.Const.NET.END_POINT+ '/besst/project/systemDelete?token='+Core.Data.getToken()+'&id='+id
                ).then((res)=>{
                    if(res.data.code == 0){
                        that.$message({
                            type: 'success',
                            message: 'delete successfully'
                        });
                        that.dialogDeleteVisible = false;
                        that.tableData.forEach((item,index)=>{
                            if(item.id == id){
                                that.tableData.splice(index,1)
                            }
                        })
                    }
                }).catch((err)=>{
                    console.log(err);
                })
            },
            getClientOrderData(){
                Core.Api.request({
                    url: '/clientOrder/clientOrderById?id='+this.$route.query.id,
                    method: 'GET'
                }).then((res)=>{
                    if(res&&res.data&&res.data.length>0){
                        this.tableData = res.data;
                        this.customerNameInput = res.data[0].name;
                        this.customerNumberInput = res.data[0].code;
                        this.customerOrderNumber = res.data[0].client_order_no;
                        this.customerCreateDate = res.data[0].create_time?this.getFormatterDate(res.data[0].create_time*1000):'';
                        // this.systemList.forEach((item)=>{
                        //     if(item.name == res.data[0].project_code){
                        //         this.selectedSystem = item.value;
                        //     }
                        // })
                    }else{
                        this.customerNameInput = this.$route.query.customerName;
                        this.customerNumberInput = this.$route.query.customerNumber;
                    }
                }).catch((err)=>{
                     console.log(err);
                })
            },
            getFormatterDate(time){
                return new Intl.DateTimeFormat('zh', {
                    year: 'numeric',
                    month: '2-digit',
                    day: '2-digit',
                    hour: '2-digit',
                    minute: '2-digit',
                    second: '2-digit',
                    hour12: false
                }).format(new Date(time)).replace(/\//g,'-');
            },
             getSystemData(){
                Core.Api.request({
                    url: '/besst/project/allBesst',
                    method: 'GET'
                }).then((res)=>{
                    if(res.data&&res.data.length>0){
                        res.data.forEach((item)=>{
                            let systemItem = {
                                name: item.name,
                                id: item.id,
                                value: item.id
                            };
                            this.systemList.push(systemItem);
                        })
                    }
                }).catch((err)=>{
                     console.log(err);
                })
            }
        },

        mounted() {
            this.getClientOrderData();
            this.getSystemData();

        },
    }
</script>
<style>
    .create-project .el-form--inline .el-form-item__label {
        float: left;
    }

    .create-project .el-form--inline .el-form-item {
        width: 30%;
        margin-left: 20px;
    }

    .create-project .el-form {
        padding-top: 20px;
    }

    .project-btn {
        display: flex;
        justify-content: flex-end;
    }

    .save-project {
        margin-right: 30%;
    }

    .create-project .el-form--inline .el-form-item__label {
        width: 100px;
    }
    .create-project .el-dialog {
        width: 300px;
    }
    .icons.go-back{
        display: flex;
        justify-content: flex-start;
        margin-left: 40px;
    }
</style>



// WEBPACK FOOTER //
// create.vue?0913fd29