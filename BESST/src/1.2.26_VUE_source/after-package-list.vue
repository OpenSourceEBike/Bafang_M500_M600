<template>
    <div class="after-package">
        <div class="header">
            <el-form :inline="true" class="list-form-inline">
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item back" @click="goAfterIndex" title="back"></div>
                    </div>
                </div>
                <el-form-item label="Client Code">
                    <el-input v-model="clientCode"></el-input>
                </el-form-item>
                <el-form-item label="Package Code">
                    <el-input v-model="packageCode"></el-input>
                </el-form-item>
                <el-date-picker
                    v-model="startDate"
                    type="date"
                    placeholder="">
                </el-date-picker>
                <el-date-picker
                    v-model="endDate"
                    type="date"
                    placeholder="">
                </el-date-picker>
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
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <!--<td v-if="total"><el-checkbox v-model="allSelect" @change="selectAll"></el-checkbox></td>-->
                    <td v-for="(item, index) in headData" :class="{'al-right': index=== 0 || index=== 1 }">{{item}}</td>
                    </thead>
                    <tbody v-if="!loading">
                    <tr v-for="item in packageList">
                        <!--<td><el-checkbox v-model="item.select"></el-checkbox></td>-->
                        <td class="al-right">{{ item.a  || '/' }}</td>
                        <td class="al-right">{{ item.b || '/' }}</td>
                        <td>{{ item.c || '/' }}</td>
                        <td>{{ item.d  }}</td>
                        <td>{{ item.e || '/' }}</td>
                        <td>{{ item.f || '/' }}</td>
                        <td>
                            <a class="btn-underline" @click.stop="goPackageEditView(item.a)">edit</a>
                            <a class="btn-underline" @click.stop="goPackageDetailView(item.a)">detail</a>
                            <a class="btn-underline" @click.stop="showDeleteDialog(item)" v-if="true">delete</a>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-if="packageList.length <= 0 && !loading">
                    noResult
                </div>
                <div class="loading" v-if="loading">
                    <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
                </div>
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

        <!--增加包裹-->
        <el-dialog
            title="Add"
            :visible.sync="dialogAddVisible"
            size="small">
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
            <span></span>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="addPackage(item)">Save</el-button>
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
                <el-button type="primary" @click="deletePackage(item)">yes</el-button>
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
                packageList: [],
                loading: false,
                headData: [],
                currentPage: Core.Data.get(Core.Const.DATA.KEY_AFTER_SALES_PAGE) ? Core.Data.get(Core.Const.DATA.KEY_AFTER_SALES_PAGE) : 1,
                total: 0,
                dialogAddVisible: false,
                dialogDeleteVisible: false,
                selectId: '',
                authList: {},

                // 搜索
                clientCode: '',
                packageCode: '',
                startDate: Core.Util.getCurrentMonthFirst(),
                endDate: Core.Util.transDay(),


                //增加
                paCode: '',
                paFrom: '',
                paUsageType:[
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
                receiveDate: Core.Util.transDay(),

            }

        },
        created(){
        },
        mounted(){
            this.headData = ['package1','package2','package3','package4','package5','package6','Operation'];



            this.getPackageList();


        },
        methods:{
            getPackageList(){

                let res = [
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':1, 'f':1,},
                ];
                this.packageList = res.slice(0,10);

                this.total= res.length;
            },
            goAfterIndex(){
                this.$router.push({name:'afterIndex'});
            },
            goPackageEditView(packageId){
                this.$router.push({name:'afterPaEdit'  ,query: {"packageId": packageId,"from":"list"}});
            },
            goPackageDetailView(packageId){
                this.$router.push({name:'afterPaDetail',query: {"packageId": packageId,"from":"list"}});
            },
            showDeleteDialog(item){
                this.dialogDeleteVisible = true;
            },
            handleCurrentChange(val) {
                this.currentPage = val;
                Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
//                this.getOrderList();
            },
            addPackage(item){

            },
            deletePackage(item){

            },

        },

    }
</script>

<style lang="scss" rel="stylesheet/scss" >
    .after-package {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        .list-form-inline {
            height:40px;
            line-height: 40px;
            display: inline-block;
            .el-form-item {
                margin-right: 2px !important;
                margin-bottom: 0;
            }
            .el-form-item__content {
                width: 180px !important;
            }
        }
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
            padding-left: 2px;
        }
    }
</style>




// WEBPACK FOOTER //
// after-package-list.vue?7f74e766