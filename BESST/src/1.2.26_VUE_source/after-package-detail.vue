<template>
    <div class="after-pa-detail">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goPackageList" title="back"></div>
                </div>
            </div>
            <label class="title">orderDetail</label>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item edit" @click="goPackageEdit" v-if="true"
                         title="edit"></div>
                    <div class="icon-item delete" @click="dialogDeleteVisible = true"
                         v-if="true" title="delete"></div>
                </div>
            </div>
        </div>
        <div class="loading" v-if="loading">
            <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon"/>
        </div>
        <div class="content-wrap" v-else>
            <div class="content">
                <div class="order-info">
                    <div class="left">
                        <div class="info-list">
                            <div class="col-left">
                                <ul class="list-group">
                                    <li class="list-group-item">
                                        <span>paCode</span>
                                        <span>{{packageItem.paCode || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span>paFrom</span>
                                        <span>{{packageItem.paFrom || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span>paUsage</span>
                                        <span>{{packageItem.paUsage || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span>receiveDate</span>
                                        <span>{{packageItem.receiveDate || '/'}}</span>
                                    </li>
                                </ul>
                            </div>
                            <div class="col-right">
                                <ul class="list-group">
                                    <li class="list-group-item">
                                        <span>paCode_2</span>
                                        <span>{{packageItem.paCode_2 || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span>paContact</span>
                                        <span>{{packageItem.paContact || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span>paTel</span>
                                        <span>{{packageItem.paTel || '/'}}</span>
                                    </li>
                                    <li class="list-group-item">
                                        <span></span>
                                        <span></span>
                                    </li>
                                </ul>
                            </div>
                            <div class="note">
                                <div class="item">
                                    <label class="label">address</label>
                                    <span>{{packageItem.address}}</span>
                                </div>
                                <div class="item">
                                    <label class="label">note</label>
                                    <span>{{packageItem.note}}</span>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="parts-list">
                        <label class="label1">itemsList</label>
                    </div>
                    <div class="table-wrap">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                            <td v-for="item in tableHead">{{item}}</td>
                            </thead>
                            <tbody class="height2">
                            <tr v-for="(item, index) in packageCompItems">

                                <td>{{item.a}}</td>
                                <td>{{item.b}}</td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                    <div class="footer">
                        <span>111</span>
                    </div>
                </div>
            </div>
        </div>


        <el-dialog
            title="title"
            :visible.sync="dialogDeleteVisible"
            size="tiny">
            <span>content</span>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteDealerOrder">yes</el-button>
                 <el-button type="warning" @click="dialogDeleteVisible = false">no</el-button>
            </span>
        </el-dialog>


    </div>
</template>

<script>
    import Core from 'core/core';

    export default {
        data(){
            return {
                Core: Core,
                packageDetail: {},
                loading: false,
                dialogDeleteVisible: false,
                packageId: '',
                authList: {},

                tableHead:['SN','Status'],
                packageItem:{},
                packageCompItems:[],


            }
        },
        created(){

        },
        mounted(){

            this.packageId = this.$route.query.packageId;
            this.getPackageDetail();
        },
        methods: {
            getPackageDetail( packageId ){
                this.packageItem = {
                    paId:'NO000001',
                    paCode:"1",
                    paFrom:"2",
                    paUsage:"3",
                    receiveDate:"2018-8-21",
                    paCode_2:"11",
                    paContact:"4",
                    paTel:"+86 12 1447272",
                    address:"address address address address address address address",
                    note:"7",
                };
                this.packageCompItems = [
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                    {'a':1, 'b':2},
                ];
            },
            goPackageList(){
                this.$router.push({name:'afterPackage'});
            },
            goPackageEdit(){
                this.$router.push({name:'afterPaEdit',query: {"packageItem": this.packageItem,"from":"detail"}});
            },
            deleteDealerOrder(){

            },
        },
        computed: {},
        components: {}
    }
</script>

<style scoped lang="scss" rel="stylesheet/scss">
    .after-pa-detail{
        .content {
            /*height: 100%;*/
            /*overflow: auto;*/
            border: 1px solid #2B2F3B;
            .order-info {
                /*height: 272px;*/
                .title {
                    height: 50px;
                    line-height: 50px;
                    padding-left: 40px;
                    /*font-size: 12px;*/
                }
                .left {
                    float: left;
                    width: 100%;
                    background: #353945;
                    box-shadow: 0 -1px #2B2F3B;
                    .item {
                        margin-left: 40px;
                        line-height: 60px;
                        overflow: hidden;
                        height: 50px;
                        .label {
                            display: inline-block;
                            width: 156px;
                        }
                    }
                    .info-list {
                        font-size: 0;
                        .col-left {
                            display: inline-block;
                            width: 50%;
                        }
                        .col-right {
                            display: inline-block;
                            width: 50%;
                        }
                        .node{
                            width: 100%;
                            .label{
                                width:40%;
                            }
                        }
                        .list-group {
                            padding: 0 40px;
                            .list-group-item {
                                height: 50px;
                                line-height: 50px;
                                font-size: 0;
                                span {
                                    display: inline-block;
                                }
                                span:first-child {
                                    /*font-size: 12px;*/
                                    color: #FFFFFF;
                                    width: 40%;
                                }
                                span:last-child {
                                    /*font-size: 12px;*/
                                    color: #A1A5B1;
                                    width: 60%;
                                }
                            }
                        }
                    }
                }
                .parts-list {
                    float: left;
                    padding: 27px 40px;
                    /*font-size: 12px;*/
                    overflow: hidden;
                    height: 50px;
                    width: 100%;
                    box-shadow: 0 -1px #2B2F3B;
                    .label1 {
                        display: inline-block;
                        /*font-size: 12px;*/
                    }
                }
                .table-wrap {
                    box-shadow: 0 -1px #2B2F3B;
                    overflow-x: hidden;
                    .long-td{
                        max-width: 200px;
                        text-overflow: ellipsis;
                        overflow: hidden;
                    }
                    .table-content{
                        padding-bottom: 0;
                    }
                }
                .footer {
                    width: 100%;
                    height: 50px;
                    padding: 10px 38px;
                    clear: both;
                    .footer-pagination {
                        float: right;
                    }
                    .footer-delete {
                        margin-top: 3px;
                        background-color: #858890;
                    }
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// after-package-detail.vue?cb05fa4e