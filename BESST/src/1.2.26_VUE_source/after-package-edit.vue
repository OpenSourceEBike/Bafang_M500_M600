<template>
    <div class="after-pa-edit">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goPackageList" title=" back "></div>
                </div>
            </div>
            <label class="title">save package</label>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item save" @click="savePackage" v-if="true"
                         title="save"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                <div class="order-info">
                    <div class="left">
                        <div class="col-left">
                            <div class="item">
                                <label class="label">Package Code<span class="required">*</span></label>
                                <el-input class="input input-sn" v-model="paCode"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Package From<span class="required">*</span></label>
                                <el-input class="input " v-model="paFrom"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Package Usage<span class="required">*</span></label>
                                <el-select v-model="paUsage" class="input">
                                    <el-option
                                        v-for="item in paUsageType"
                                        :key="item.value"
                                        :label="item.label"
                                        :value="item.value">
                                    </el-option>
                                </el-select>
                            </div>
                            <div class="item">
                                <label class="label">Receive Data<span class="required">*</span></label>
                                <el-date-picker
                                    v-model="receiveDate"
                                    type="date"
                                    placeholder="receiveDate">
                                </el-date-picker>
                            </div>
                        </div>

                        <div class="col-right">
                            <div class="item">
                                <label class="label">Package Code 2</label>
                                <el-input class="input input-sn" v-model="paCode"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Contact</label>
                                <el-input class="input " v-model="paContact"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Tel</label>
                                <el-input class="input " v-model="paFrom"></el-input>
                            </div>
                        </div>
                        <div class="note">
                            <div class="item">
                                <label class="label">address</label>
                                <el-input class="input-note" v-model="address"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">note</label>
                                <el-input class="input-note" v-model="note"></el-input>
                            </div>

                        </div>
                    </div>

                </div>
                <div class="order-info border">
                    <label class="title">title</label>
                    <div class="parts-list">
                        <label class="labelList">itemList</label>
                        <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
                        <label class="labelAdd">addItem</label>
                    </div>
                    <div class="table-wrap">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                            <td>
                                <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
                            </td>
                            <td v-for="item in tableHead">{{item}}</td>
                            </thead>
                            <tbody class="height2">
                            <tr v-for="(item, index) in packageCompItems">
                                <td>
                                    <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                                </td>
                                <td>{{item.a}}</td>
                                <td>{{item.b}}</td>
                                <td>
                                    <a class="btn-underline" v-on:click="deleteItem(index)">delete</a>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                    <div class="footer">
                        <div class="btn-left">
                            <el-button type="delete" @click="deletePackageItem">delete</el-button>
                        </div>
                        <div class="btn-right">
                            <el-button type="primary" @click="createPackage">savePackage</el-button>
                        </div>
                    </div>
                </div>

            </div>
        </div>

        <!-- 生成这个表，即生成售后记录，处理结果。-->
        <!--<el-dialog-->
        <!--class="dialog"-->
        <!--title="addItem"-->
        <!--:visible.sync="dialogCreateVisible"-->
        <!--size="large"-->
        <!--top="10%">-->
        <!--<div class="content">-->
        <!--<div class="header">-->
        <!--<el-form :inline="true" class="list-form-inline">-->
        <!--<el-form-item label="Client Code">-->
        <!--<label class="label">SN</label>-->
        <!--<el-input class="input input-sn" v-model="paCode"></el-input>-->
        <!--</el-form-item>-->
        <!--</el-form>-->
        <!--&lt;!&ndash;<el-input&ndash;&gt;-->
        <!--&lt;!&ndash;:placeholder="category"&ndash;&gt;-->
        <!--&lt;!&ndash;class="search-content"&ndash;&gt;-->
        <!--&lt;!&ndash;:disabled="true"&ndash;&gt;-->
        <!--&lt;!&ndash;v-model="selectCategoryItem.name"&ndash;&gt;-->
        <!--&lt;!&ndash;@click.native="selectCategory">&ndash;&gt;-->
        <!--&lt;!&ndash;</el-input>&ndash;&gt;-->
        <!--&lt;!&ndash;<el-input&ndash;&gt;-->
        <!--&lt;!&ndash;:placeholder="modelNamePN"&ndash;&gt;-->
        <!--&lt;!&ndash;class="search-content"&ndash;&gt;-->
        <!--&lt;!&ndash;v-model="queryItemCondition">&ndash;&gt;-->
        <!--&lt;!&ndash;</el-input>&ndash;&gt;-->
        <!--<div class="btn-group-inline">-->
        <!--<div class="icons">-->
        <!--<div class="icon-item search" @click="queryPackageItem" title=" search "></div>-->
        <!--<div class="icon-item clear" @click="clearCondition" title=" clear "></div>-->
        <!--</div>-->
        <!--</div>-->
        <!--</div>-->
        <!--<div class="table-wrap">-->

        <!--<table class="table-content" border="0" cellspacing="0" cellpadding="0">-->
        <!--<thead>-->
        <!--<td>-->
        <!--<el-checkbox v-model="allSelectedForAdd"></el-checkbox>-->
        <!--</td>-->
        <!--<td v-for="item in itemHeadData">{{item}}</td>-->
        <!--</thead>-->

        <!--<tbody>-->
        <!--<tr v-for="(item, index) in selectItems">-->
        <!--<td>-->
        <!--<el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>-->
        <!--</td>-->
        <!--<td><img :src="item.img | getImgUrl"></td>-->
        <!--<td>{{item.name}}</td>-->
        <!--<td>{{item.ext_part_no}}</td>-->
        <!--</tr>-->
        <!--</tbody>-->
        <!--</table>-->

        <!--<el-pagination-->
        <!--class="pagination"-->
        <!--@current-change="handleCurrentPageChange"-->
        <!--:current-page.sync="currentPage"-->
        <!--:page-size="10"-->
        <!--layout="prev, pager, next"-->
        <!--:total="total">-->
        <!--</el-pagination>-->

        <!--</div>-->
        <!--<div class="footer">-->
        <!--<el-button type="primary" @click="addItems">addItems</el-button>-->
        <!--</div>-->
        <!--</div>-->
        <!--</el-dialog>-->


        <!--<el-dialog-->
            <!--class="dialog"-->
            <!--title="Add Component"-->
            <!--:visible.sync="dialogAddComponentVisible"-->
            <!--size="large">-->
            <!--<div class="content" style="max-height: 500px; overflow: auto">-->
                <!--<div class="category-tree">-->
                    <!--<el-tree :data="categoryList" :props="defaultProps" @node-click="handleNodeClick"></el-tree>-->
                <!--</div>-->
            <!--</div>-->
        <!--</el-dialog>-->

    </div>
</template>

<script>
    import Core from 'core/core';

    export default {
        data(){
            return {
                Core: Core,
                dialogAddComponentVisible: false,
                dialogCreateVisible: false,
                componentSn: '',
                allSelectedForDelete: false,
                tableHead: ['SN','Status','Operation'],
                packageItem: {},
                packageCompItems: [],

                //包裹信息
                paCode: '',
                paFrom: '',
                paUsageType: [
                    {
                        value: '1',
                        label: 'Client Repair'
                    },
                    {
                        value: '2',
                        label: 'Bafang Component'
                    },
                    {
                        value: '3',
                        label: 'Other Material'
                    }],
                paUsage: '',
                receiveDate: '',
                paCode_2: '',
                paContact: '',
                paTel: '',
                address: '',
                note: '',


                //增加
                allSelectedForAdd: false,
                itemHeadData: ['SN','Status','Operation'],
                selectItems: [],



            }
        },
        created(){
        },
        mounted(){
//            console.log(this.$route);
            if( this.$route.query.from =='list'){
                this.queryPackageItem();
            }else if(this.$route.query.from =='detail'){
                let params = this.$route.query.packageItem;
                    this.paCode = params.paCode;
                    this.paFrom = params.paFrom;
                    this.paUsage = params.paUsage;
                    this.paContact = params.paContact;
                    this.receiveDate = params.receiveDate;
                    this.address = params.address;
                    this.note = params.note;
            }

        },
        methods: {
            createPackage(){

            },
            queryPackageItem(){

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
            savePackage(){
                this.$router.push({name:'afterPaDetail',query: {"packageItem": this.packageCompItems}});
            },
            deletePackageItem(){

            },
            clearCondition(){
//                this.selectItems = [];
//                this.queryItemList = [];
//                this.queryItemCondition = '';
//                this.selectCategoryItem = '';
//                this.total = 0;
//                this.currentPage  = 1;
            },
            addItems(){
//                this.selectItems.forEach((item, index) => {
//                    if (item.selected === true) {
//                        let isAdded = false;
//                        this.orderItems.forEach((orderItem) => {
//                            if (orderItem.item_id === item.id) {
//                                isAdded = true;
//                            }
//                        });
//                        if (!isAdded) {
//                            let addItem = this.selectItems[index];
//                            this.orderItems.push({
//                                item_id: addItem.id,
//                                item_name: addItem.name,
//                                part_no: '',
//                                item_part_no: addItem.ext_part_no,
//                                amount: 1,
//                                selected: false
//                            });
//                        }
//                    }
//                });
                this.closeChooseItem();

            },
            deleteItem(index){

            },
            handleCurrentPageChange(page) {
//                this.currentPage = page;
//                this.queryOrderItem();
            }

        },
        computed: {},
        components: {}
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .after-pa-edit{
        .content {
            /*height: 100%;*/
            border: 1px solid #2B2F3B;
            margin-bottom: 10px;
            .order-info {
                width: 100%;
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
                    .col-left{
                        float: left;
                        margin-top: 9px;
                        width: 474px;
                        .el-date-editor {
                            width: 256px;
                            margin-left: 10px;
                        }
                    }
                    .col-right{
                        float: left;
                        margin-top: 9px;
                        width: 474px;
                    }
                    .note{
                        margin-bottom: 5px;
                    }
                    .item {
                        margin: 2px 0 0 40px;
                        line-height: 60px;
                        overflow: hidden;
                        height: 50px;
                        clear:both;
                        .label {
                            display: inline-block;
                            width: 110px;
                        }
                        .input {
                            margin-left: 10px;
                            width: 256px;
                            border-radius: 0 0 10px 10px;
                        }
                        .input-note {
                            margin-left: 10px;
                            width: 726px;
                        }
                    }
                }
                .parts-list {
                    float: left;
                    padding: 0 40px;
                    font-size: 12px;
                    overflow: hidden;
                    height: 50px;
                    line-height: 50px;
                    width: 100%;
                    box-shadow: 0 -1px #2B2F3B;
                    .labelList {
                        /*font-size: 12px;*/
                    }
                    .att-btn {
                        margin-left: 46px;
                        padding: 6px 9px;
                        min-width: 0;
                    }
                    .labelAdd {
                        margin-left: 14px;
                        /*font-size: 12px;*/
                    }
                }
                .table-wrap{
                    /*height:500px;*/
                }
                .footer {
                    width: 100%;
                    height: 60px;
                    line-height: 60px;
                    box-shadow: 0 -1px #2B2F3B;
                    .footer-pagination {
                        /*float: right;*/
                    }
                    .btn-left{
                        display:inline-block;
                        width:100px;
                        float:left;
                        padding-left: 40px;
                    }
                    .btn-right{
                        display:inline-block;
                        float:right;
                        width:100px;
                        margin-right:60px;
                    }
                }
            }

        }
        .height1{
            height:272px;
        }
        .height2{
            height:372px;
        }
        .dialog {
            .el-dialog--large {
                width: 1100px;
            }
            .header {
                .search-content {
                    width: 200px;
                    margin-right: 10px;
                }
                .search-btn {
                    position: absolute;
                    top: 15px;
                    left: 250px;
                    width: 30px;
                    height: 30px;
                }
            }
        }
    }

</style>



// WEBPACK FOOTER //
// after-package-edit.vue?35e16588