<template>
    <div class="after-pi">
        <div class="header">
            <!--<div class="btn-group-inline">-->
                <!--<div class="icons">-->
                    <!--<div class="icon-item back" @click="goAfterIndex" title="back"></div>-->
                <!--</div>-->
            <!--</div>-->
            <el-form :inline="true" class="list-form-inline">
                <!--<el-form-item label="">-->
                    <div class="btn-group-inline">
                        <div class="icons">
                            <span class="icon-item back" @click="goAfterIndex" title="back"></span>
                        </div>
                    </div>
                <!--</el-form-item>-->
                <el-form-item label="PO No.">
                    <el-input v-model="poNo"></el-input>
                </el-form-item>
                <el-form-item label="Customer">
                    <el-input v-model="customer"></el-input>
                </el-form-item>
                <el-form-item label="PI No.">
                    <el-input v-model="piNo"></el-input>
                </el-form-item>
                <el-form-item label="Creator">
                    <el-input v-model="creator"></el-input>
                </el-form-item>
                <!--<el-form-item label="Start">-->
                    <el-date-picker
                        v-model="startDate"
                        type="date"
                        placeholder="">
                    </el-date-picker>
                <!--</el-form-item>-->
                <!--<el-form-item label="End">-->
                    <el-date-picker
                        v-model="endDate"
                        type="date"
                        placeholder="">
                    </el-date-picker>
                <!--</el-form-item>-->
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="" title="search"></div>
                    </div>
                </div>
            </el-form>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add" @click="dialogAddVisible = true"
                         v-if="true" title="add"></div>
                </div>
            </div>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <div class="no-data-content" v-if="piList.length <= 0 && !loading">
                    noResult
                </div>
                <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
                    <thead>
                    <!--<td v-if="total"><el-checkbox v-model="allSelect" @change="selectAll"></el-checkbox></td>-->
                    <td v-for="(item, index) in headData" :class="{'al-right': index=== 0 || index=== 1 }">{{item}}</td>
                    </thead>
                    <tbody v-if="!loading">
                    <tr v-for="item in piList">
                        <!--<td><el-checkbox v-model="item.select"></el-checkbox></td>-->
                        <td class="al-right">{{ item.a  || '/' }}</td>
                        <td class="al-right">{{ item.b || '/' }}</td>
                        <td>{{ item.c || '/' }}</td>
                        <td>{{ item.d  }}</td>
                        <td>{{ item.e || '/' }}</td>
                        <td>{{ item.f || '/' }}</td>
                        <td>{{ item.g || '/' }}</td>
                        <td>
                            <a class="btn-underline" @click.stop="goPIEditView(item.a)">edit</a>
                            <!--<a class="btn-underline" @click.stop="goPISplitView(item.a)">split</a>-->
                            <a class="btn-underline" @click.stop="goPIDetailView(item.a)">detail</a>
                            <a class="btn-underline" @click.stop="showDeleteDialog(item)" v-if="true">delete</a>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </div>
            <div class="loading" v-if="loading">
                <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="handleCurrentChange"
                    :current-page.sync="currentPage"
                    :page-size="10"
                    layout="prev, pager, next, jumper"
                    :total="total">
                </el-pagination>
                <p class="total">total: {{total}}</p>
            </div>
        </div>

        <!--增加形式发票-->
        <el-dialog
            title="Add"
            :visible.sync="dialogAddVisible"
            size="small">
            <!--号码自动生成，根据列表或者数据库最新数据-->
            <div class="item">
                <label class="label">Proforma Invoice<span class="required">*</span></label>
                <el-input class="input " v-model="piNumber"></el-input>
            </div>
            <div class="item">
                <label class="label">Type<span class="required">*</span></label>
                <el-select v-model="piType" class="input">
                    <el-option
                        v-for="item in piTypeType"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                    </el-option>
                </el-select>
            </div>
            <span></span>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="addPI(item)">Save</el-button>
                <el-button type="warning" @click="dialogAddVisible = false">Cancel</el-button>
            </span>
        </el-dialog>

        <!--删除弹框-->
        <el-dialog
            title="Delete"
            :visible.sync="dialogDeleteVisible"
            size="tiny">
            <span></span>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deletePI(item)">yes</el-button>
                 <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
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
                piList: [],
                loading: false,
                headData: [],
                currentPage: Core.Data.get(Core.Const.DATA.KEY_AFTER_SALES_PAGE) ? Core.Data.get(Core.Const.DATA.KEY_AFTER_SALES_PAGE) : 1,
                total: 0,
                dialogAddVisible: false,
                dialogDeleteVisible: false,
                selectId: '',
                authList: {},

                // 搜索
                poNo: '',  // 采购订单
                customer: '',
                piNo: '',  // 形式发票
                creator: '',
                startDate: Core.Util.getCurrentMonthFirst(),
                endDate: Core.Util.transDay(),


                //增加 - 领号
                piNumber: '',
                piType: '',
                piTypeType:[
                    {
                        value: '1',
                        label: 'Sales'
                    },
                    {
                        value: '2',
                        label: 'After Sales'
                    },
                ]
            }

        },
        created(){
        },
        mounted(){
            this.headData = ['Date','PI No.','PO No.','Customer','Creator','Type','Status','Operation'];



            this.getPIList();


        },
        methods:{
            getPIList(){

                let res = [
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'after sales','g':1},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':'sales','g':1},

                ];
                this.piList = res.slice(0,10);

                this.total= res.length;
            },
            goAfterIndex(){
                this.$router.push({name:'afterIndex'});
            },
            goPIEditView(piId){
                this.$router.push({name:'afterPiEdit'  ,query: {"piId": piId,"from":"list"}});
            },
            goPISplitView(piId){
                this.$router.push({name:'afterPiSplit'  ,query: {"piId": piId,"from":"list"}});
            },
            goPIDetailView(piId){
                this.$router.push({name:'afterPiDetail',query: {"piId": piId,"from":"list"}});
            },
            showDeleteDialog(item){
                this.dialogDeleteVisible = true;
            },
            handleCurrentChange(val) {
                this.currentPage = val;
                Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
//                this.getOrderList();
            },
            addPI(item){

            },
            deletePI(item){

            },
        },

    }
</script>

<style lang="scss" rel="stylesheet/scss" >
    .after-pi {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        .list-form-inline {
            /*position: relative;*/
            /*float:left;*/
            height: 100%;
            line-height: 50px;
            .el-form-item {
                margin-right: 2px !important;
                margin-bottom: 0;
            }
            .el-form-item__content {
                width: 180px !important;
            }
        }
        /*.table-wrap ::before{*/
            /*display: table;*/
            /*content: '';*/
            /*clear: both;*/
        /*}*/
        .el-dialog--small {
            .item {
                line-height: 60px;
                height: 60px;
                .label {
                    display: inline-block;
                    width: 200px;
                }
                .el-input {
                    width: 300px;
                    margin-left: 10px;
                }
            }
        }
        .el-dialog {
            span.required {
                color: #E26829;
                padding-left: 2px;
                display: inline-block;
            }
        }
    }
</style>




// WEBPACK FOOTER //
// after-pi.vue?6baad3d3