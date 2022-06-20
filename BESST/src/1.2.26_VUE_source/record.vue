<template>
    <div class="record-list">
        <div class="header">
            <el-form :inline="true" class="list-form-inline">
                <el-form-item label="SN">
                    <el-input v-model="sn"></el-input>
                </el-form-item>
                <el-form-item label="Account">
                    <el-input v-model="mail"></el-input>
                </el-form-item>
                <el-form-item label="Data">
                    <el-input v-model="data"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="getSaveList()" title="search"></div>
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
                    <tbody v-if="!loading">
                        <tr class="large" v-for="item in recordList">
                            <td>{{item.sn}}</td>
                            <td>{{item.vehicle_sn}}</td>
                            <td>{{item.org_type == 3?"ASSEMBLER": item.org_type ==2?"DEALER":item.org_type ==4?"COMPONENT":"BRAND"}}</td>
                            <td>{{item.user_email}}</td>
                            <td>{{item.time | timeFormat}}</td>
                            <td>{{item.type | operationType}}</td>
                            <td>
                                <span v-if="item.type === 1 || item.type === 3">
                                    Old:{{ item.data.old}} <br>
                                    New: {{ item.data.new }}
                                </span>
                                <span v-else>
                                    {{ item.data}}
                                </span>
                            </td>
                        </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="recordList.length <= 0 && !loading">
                    {{ $t('noResult') }}
                </div>
                <div class="loading" v-if="loading">
                    <img src="../../assets/images/icon/loading-bars.svg" alt="Loading icon"/>
                </div>
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="handleCurrentChange"
                    :current-page.sync="currentPage"
                    :page-size="10"
                    layout="prev, pager, next"
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
        name: 'recordList',
        data(){
            return {
                tableHead: ['SN','Vehicle','Operator','Account','Time', 'Type', 'Data'],
                recordList: [],
                currentPage: 1,
                total: 0,
                date:Core.Util.time(),
                org: Core.Data.getOrg(),
                sn: '',
                data: '',
                mail: '',
                loading: false,
            }
        },
        filters: {
            operationType: function (type) {
               switch (type) {
                   case 1:
                       return 'Edit';
                   case 2:
                       return 'Update';
                   case 3:
                       return 'Replace';
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
            this.initData();
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });
            this.org = Core.Data.getOrg();
            // 获取SN记录列表
            this.getRecordList();
        },
        methods: {
            initData() {
                this.tableHead = [
                    this.$t('tableHead.sn'),
                    this.$t('tableHead.vehicle'),
                    this.$t('tableHead.operator'),
                    this.$t('tableHead.account'),
                    this.$t('tableHead.time'),
                    this.$t('tableHead.type'),
                    this.$t('tableHead.data'),
                ];
            },
            getRecordList() {
                this.loading = true;

                if (this.org.type === Core.Const.ORG_TYPE.BRAND) {
                    Core.Api.ObjectRecord.getListForBrand( this.currentPage ).then(res => {
                        this.loading = false;
                        let list = res.record_list ? res.record_list : [];
                        list.forEach(item => {
                            if (item.data && item.type == 1) {  // type = 1，是edit
                                if(item.data.match("{")){
                                    item.data = JSON.parse(item.data);
                                }else{
                                    item.data = {
                                        "old": item.data,
                                        "new": 'wrong type'
                                    }
                                }
                            }
                        });
                        this.recordList = list;
                        this.total = res.count;
                    })
                } else {
                    Core.Api.ObjectRecord.getListForBF( this.currentPage ).then(res => {
                        this.loading = false;
                        let list = res.record_list ? res.record_list : [];
                        console.log(list);
                        list.forEach(item => {
                            if (item.data && item.type == 1) {
                                if(item.data.match("{")){
                                    item.data = JSON.parse(item.data);
                                }else{
                                    item.data = {
                                        "old": item.data,
                                        "new": 'wrong type'
                                    }
                                }
                            }
                        });
                        this.recordList = list;
                        this.total = res.count;
                    })
                }
            },
            getSaveList() {
                var type = 1; // Brand
                if(this.org.type === Core.Const.ORG_TYPE.BRAND){
                    type = 2; // BF
                }
                if(this.sn.trim() || this.data.trim() || this.mail.trim()){
                    this.loading = true;
                    console.log("type",type);

                    Core.Api.ObjectRecord.searchList(this.sn.trim(), this.data.trim(), this.mail.trim(), this.currentPage, '', type).then(res => {
                        this.loading = false;
                        if(res){
                            let list = res.record_list ? res.record_list : [];
                            list.forEach(item => {
                                if (item.data && item.type == 1) {
                                    if(item.data.match("{")){
                                        item.data = JSON.parse(item.data);
                                    }else{
                                        item.data = {
                                            "old": item.data,
                                            "new": 'wrong type'
                                        }
                                    }
                                }
                            });
                            this.recordList = list;
                            this.total = res.count;
                        }else{
                            this.recordList = [];
                            this.total = 0;
                        }
                    })
                }else{
                    this.getRecordList();
                }
            },
            handleCurrentChange(val) {
                this.currentPage = val;
                if(this.sn.trim() || this.data.trim() || this.mail.trim()){
                    this.getSaveList();
                }else{
                    this.getRecordList();
                }
            },
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .record-list {
        width: 100%;
        height: 100%;
        display: flex;
        flex-direction: column;
        .table-wrap {
            /*flex: 1;*/
        }
        .header {
            height: auto;
            .el-form-item {
                margin-bottom: 0;
            }
        }
        /*.loading {*/
            /*width: 100%;*/
            /*display: flex;*/
            /*align-items: center;*/
            /*justify-content: center;*/
        /*}*/
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
// record.vue?182c0fc0