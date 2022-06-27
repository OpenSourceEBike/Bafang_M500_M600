<template>
    <div class="componentSave-list">
        <div class="header">
            <el-form :inline="true" class="list-form-inline">
                <span class="headTitle">{{$t('recordTitle')}}</span>
                <el-form-item label="SN">
                    <el-input v-model="sn"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="searchRecordList()" title="search"></div>
                    </div>
                </div>
            </el-form>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <td v-for="item in tableHead">{{item}}</td>
                    </thead>
                    <tbody>
                    <tr class="large" v-for="item in componentSaveList">
                        <td>{{item.org_id}}</td>
                        <td>{{item.user_id}}</td>
                        <td>{{item.sn}}</td>
                        <td>{{item.status | statusType}}</td>
                        <td>{{item.sale_customer_id}}</td>
                        <td>{{item.sale_time | dateFormat}}</td>
                        <td>{{item.create_time | dateFormat}}</td>
                        <td>{{item.update_time | dateFormat}}</td>
                        <td>{{item.type}}</td>
                        <td>
                            <a class="btn-underline">User</a>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </div>

            <div class="table-footer">
                <el-pagination
                    @current-change="handleCurrentChange"
                    :current-page.sync="currentPage"
                    layout="prev, pager, next"
                    :page-size="20"
                    :total="total">
                </el-pagination>
                <p class="total">{{ $t('total') }}: {{total}}</p>
            </div>
        </div>
    </div>

</template>

<script>
    import Core from 'core/core'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default{
        name: 'componentSaveList',
        data(){
            return {
                tableHead: ['Org','User','SN','Status','Customer','Sale Time','Create Time','Update Time','Type','Operation'],
                componentSaveList: [],
                currentPage: 1,
                total: 0,
                date: Core.Util.time(),
                org: Core.Data.getOrg(),
                sn: '',
                data: {
                    page: 1,
                    pagesize: 20
                }
            }
        },
        filters: {
            statusType: function (type) {
               switch (type) {
                   case 1:
                       return '生产';
                   case 2:
                       return '被使用';
                   case 3:
                       return '废弃的';
                   default:
                       return '--';
               }
            }
        },
        i18n: {
            messages: {
                en: EN.Brand.RecordList,
                zh: ZH.Brand.RecordList,
                de: DE.Brand.RecordList,
                nl: NL.Brand.RecordList
            }
        },
        mounted() {
            // 获取SN记录列表

            this.getRecordList();
            this.getTotalSize();
        },
        methods: {
            searchRecordList(){
                this.getRecordList();
                this.getTotalSize();
            },
            getRecordList() {
                if (this.org.type === Core.Const.ORG_TYPE.COMPONENT) {
                    Core.Api.Object.objectGetAssemblySavedComponent(this.sn.trim(), this.data.page, this.data.pagesize).then(res => {
                        this.componentSaveList = res.all_component.object || [];
                        this.$message({
                            type: 'success',
                            message: 'search successfully'
                        })
                    })

//                    brand_id: 0
//                    brand_org_id: 0
//                    category_id: 4
//                    category_root_id: 4
//                    create_time: 1516860466
//                    id: 404652
//                    insurance_id: null
//                    insurance_name: null
//                    insurance_tracker_code: null
//                    item_id: 0
//                    order_id: 0
//                    org_id: 25
//                    produce_time: 1516860466
//                    sale_customer_id: 0
//                    sale_time: 0
//                    sn: "DPC171.U1.0605F8R9125499"
//                    status: 2
//                    type: 0  // 没有使用过
//                    update_time: 1516860545
//                    user_id: 0
//                    weight: 0

                } else {
                    Core.Api.Object.objectAssemblySavedComponent(this.sn.trim(), this.data.page, this.data.pagesize).then(res => {
                        let list = res.object || [];
                        console.log(res);
//                        list.forEach(item => {
//                            if (item.data && item.type == 1) {
//                                if(item.data.match("{")){
//                                    item.data = JSON.parse(item.data);
//                                }else{
//                                    item.data = {
//                                        "old": item.data,
//                                        "new": 'wrong type'
//                                    }
//                                }
//                            }
//                        });
                        this.componentSaveList = list;
                    })
                }
            },

            handleCurrentChange(val) {
                this.currentPage = val;
                this.data.page = val;
                this.getRecordList();
            },

            getTotalSize(){
                try{
                    Core.Api.request({
                        url: '/object/component-all-list-count',
                        data: {
                            sn: this.sn
                        },
                        method: 'GET'
                    }).then(res=>{
                        if(res.code === 0){
                            this.total = res.data.count;
                        }
                    }).catch(error=>{
                        console.log(error);
                    })
                }catch(err){
                    console.log(err);
                }
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .componentSave-list {
        width: 100%;
        height: 100%;
        display: flex;
        flex-direction: column;
        .td{
            overflow: hidden;
            text-overflow: ellipsis;
        }
        .name{
            max-width: 120px;
        }
        .address{
            max-width: 280px;
        }
    }
</style>



// WEBPACK FOOTER //
// componentSave.vue?6b8c279d