<template>
    <div>
        <div class="search-header head-list-header">
            <el-form :inline="true" :model="formSearch" class="list-form-inline save-head-list-form">
                 <el-form-item>
                    <div class="icons" @click="back">
                        <div title="Back" class="icon-item back"></div>
                    </div>
                </el-form-item>
                <el-form-item label="head Name:">
                    <el-input v-model="formSearch.headNameInput" @keyup.enter.native="onSearchHeadList" placeholder="head Name"></el-input>
                </el-form-item>
                <el-form-item label="status:">
                    <el-select v-model="formSearch.status" placeholder="status" @change="changeStatus(formSearch.status)">
                        <el-option
                            @change="changeStatusItem(item)"
                            v-for="item in statusData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                        ></el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="parent Type:">
                    <el-select v-model="formSearch.parentType" placeholder="parent type" @change="changeParentType(formSearch.parentType)">
                        <el-option
                            v-for="item in categoryTypeData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                        ></el-option>
                    </el-select>
                </el-form-item>
                <!--<el-form-item label="id:">
                     <el-input v-model="formSearch.headId" @keyup.enter.native="onSearchHeadList" placeholder="head id"></el-input>
                </el-form-item>-->
                <div class="btn-group-inline-icon">
                    <div class="icons">
                        <div class="icon-item search" @click="onSearchHeadList"  title="search"></div>
                    </div>
                </div>
                <div class="save-head-list">
                    <el-button type="primary" :disabled="saveHeadListVisible" @click="saveHeadList">save</el-button>
                </div>
            </el-form>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add-v2" @click="addHead" v-if="true" title="add"></div>
                </div>
            </div>
        </div>
        <!--<div v-if="headListTable.length<=0">no data</div>-->
        <!--<el-table ref="multipleTable" @selection-change="handleSelectionChange" :data="headListTable" border max-height="500" cellspacing="0" cellpadding="0" v-else @scroll.native="headScroll">
            <el-table-column
                type="selection"
                width="55">
            </el-table-column>
            <el-table-column v-for="(item, index) in headData" :key="index" :prop="item.name" :width="item.width" :label="item.name"></el-table-column>
            <el-table-column
                fixed="right"
                label="operation"
                >
                <template slot-scope="scope">
                    <el-button
                        type="text"
                        size="small"
                        @click="editHead(scope.row,scope.$index)">
                        edit
                    </el-button>
                    <el-button
                        type="text"
                        size="small"
                        @click="deleteHead(scope.row,scope.$index)">
                        delete
                    </el-button>
                </template>
                </el-table-column>
        </el-table>-->
       <!--<div class="table-footer" v-if="isPagination">
            <el-pagination @current-change="handleCurrentChange" :current-page.sync="currentPage" :page-size="10" layout="prev, pager, next, jumper" :total="headListTotal">
            </el-pagination>
        </div>-->
        <el-dialog title="edit head data" :visible.sync="dialogEditVisible">
            <el-form class="edit-head-data">
                <el-form-item label="head name:">
                    <el-input v-model="editHeadName" placeholder="please edit head name"></el-input>
                </el-form-item>
                <el-form-item label="status:">
                    <el-select v-model="editStatus" placeholder="status">
                        <el-option
                            v-for="item in statusData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                            v-if="item.label != '+' && item.label != '-'"
                        ></el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="parent type:">
                    <el-select v-model="editParentType" placeholder="parent type">
                        <el-option
                            v-for="item in categoryTypeData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                            v-if="item.label != '+' || item.label != '-'"
                        ></el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="editHeadData">{{ $t('op.confirm') }}</el-button>
                <el-button type="primary" @click="dialogEditVisible = false">{{ $t('op.cancel') }}</el-button>
            </div>
        </el-dialog>
        <el-dialog title="delete head data" :visible.sync="deleteHeadVisible">
            <span>Are you sure delete the head?</span>
           <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="confirmDelete">{{ $t('op.confirm') }}</el-button>
                <el-button type="primary" @click="cancelDelete">{{ $t('op.cancel') }}</el-button>
            </div>
        </el-dialog>
        <el-dialog title="delete selected head data" :visible.sync="deleteSelectedHeadVisible">
            <span>Are you sure delete selected head list?</span>
           <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="confirmDeleteSelected">{{ $t('op.confirm') }}</el-button>
                <el-button type="primary" @click="deleteSelectedHeadVisible = false">{{ $t('op.cancel') }}</el-button>
            </div>
        </el-dialog>
        <el-dialog title="add head data" :visible.sync="addHeadVisible" class="add-head-data">
            <el-form class="head-form">
                <el-form-item label="head name">
                    <el-input v-model="addHeadName" placeholder="input the head name"></el-input>
                </el-form-item>
                <el-form-item label="status:">
                    <el-select v-model="addStatus" placeholder="status">
                        <el-option
                            v-for="item in statusData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                            v-if="item.label != '+' && item.label != '-'"
                        ></el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="parent type:">
                    <el-select v-model="addParentType" placeholder="parent type">
                        <el-option
                             v-for="item in categoryTypeData"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value"
                            v-if="item.label != '+' && item.label != '-'"
                        ></el-option>
                    </el-select>
                </el-form-item>
            </el-form>
           <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="confirmAdd">{{ $t('op.confirm') }}</el-button>
                <el-button type="primary" @click="cancelAdd">{{ $t('op.cancel') }}</el-button>
            </div>
        </el-dialog>
        <el-dialog title="edit head status" :visible.sync="editStatusVisible">
            <div v-if="addStatusVisible">
                <el-form>
                    <el-form-item label="status label:">
                        <el-input v-model="addStatusName"></el-input>
                    </el-form-item>
                    <!--<el-form-item label="status value:">
                        <el-input v-model="addStatusNumber"></el-input>
                    </el-form-item>-->
                    <el-button @click="addStatusData">save</el-button>
                </el-form>
            </div>
            <div v-if="deleteStatusVisible">
                <ul class="status-list">
                    <li class="status-item" v-for="(item,index) in statusData" :key="index">
                        <span v-if="item.label != '+'&& item.label != '-'">{{item.label}}</span>
                        <el-button v-if="item.label != '+'&& item.label != '-'" type="primary" icon="delete" @click="deleteStatusItem(item)"></el-button>
                    </li>
                </ul>
            </div>
        </el-dialog>
        <el-dialog title="edit head parent type" :visible.sync="editParentTypeVisible">
            <div v-if="addParentTypeVisible">
                <el-form>
                    <el-form-item label="parent type label:">
                        <el-input v-model="addParentTypeName"></el-input>
                    </el-form-item>
                    <!--<el-form-item label="parent type value:">
                        <el-input v-model="addParentTypeNumber"></el-input>
                    </el-form-item>-->
                    <el-button @click="addcategoryTypeData">save</el-button>
                </el-form>
            </div>
            <div v-if="deleteParentTypeVisible">
                <ul class="status-list">
                    <li class="status-item" v-for="(item,index) in categoryTypeData" :key="index">
                        <span v-if="item.label != '+'&& item.label != '-'">{{item.label}}</span>
                        <el-button v-if="item.label != '+'&& item.label != '-'" type="primary" icon="delete" @click="deleteParentType(item)"></el-button>
                    </li>
                </ul>
            </div>
        </el-dialog>
        <div class="select-head-list">
             <el-transfer
                @change="selectTransferData"
                v-model="selectedHeadList"
                filterable
                filter-placeholder="please input head name"
                :titles="['all head list','selected head list']"
                :data="allHeadList"
                :render-content="renderTransferData"
                :left-default-checked = "leftTransferData"
                :right-default-checked = "rightTransferData"
                class="select-transfer"
                >
                <!--<el-button class="transfer-footer" slot="left-footer" size="small" @click="deleteAllHeadList">all delete</el-button>-->
            </el-transfer>
        </div>
    </div>
</template>
<script>
import Core from 'core/core';
import LAN from "../../../src/assets/lang/lan";

export default {
    // inject: ['reload'],
    name: 'headList',
    data(){
        return{
            title: 'headList',
            headList: [],
            categoryType: '',
            headListTable: [],
            formSearch:{
                parentType: '',
                headNameInput: '',
                status: '',
                headId: ''
            },
            //headData: [{name:'head_name'},{name:'is_show'},{name:'level'},{name:'sort_no'}],
            headData: [{name: 'id'},{name:'headName'},{name:'parentType'},{name: 'status'}],
            categoryTypeData: [
                // {
                //     value: 3,
                //     label: '电机'
                // },
                // {
                //    value: 4,
                //     label: '仪表'
                // },
                // {
                //    value: 7,
                //     label: '控制器'
                // },
                // {
                //    value: 5,
                //     label: '电池'
                // },
                // {
                //    value: 12,
                //     label: '充电器'
                // },
                // {
                //    value: 6,
                //     label: '传感器'
                // },
                // {
                //     value: 8,
                //     label: '连接器'
                // },
                // {
                //     value: 11,
                //     label: '附件'
                // },
                // {
                //     value: 14,
                //     label: '其他'
                // },
                {
                    value: 9,
                    label: '+'
                },
                {
                    value: 10,
                    label: '-'
                },
            ],
            statusData:[
                //  {
                //     value: 1,
                //     label: '硬件'
                // },
                // {
                //     value: 2,
                //     label: '固件'
                // },
                // {
                //    value: 3,
                //     label: '参数'
                // },
                // {
                //     value: 4,
                //     label: '+'
                // },
                // {
                //    value: 5,
                //     label: '-'
                // },
            ],
            dialogEditVisible: false,
            headNameInput: '',
            isShow: '',
            deleteHeadVisible: false,
            addHeadVisible: false,
            parentTypeList: [],
            parentType: '',
            addHeadName: '',
            addParentType: '',
            editHeadName: '',
            editIndex: '',
            editParentType: '',
            deleteId: '',
            multipleSelection: [],
            editId: '',
            otherAddParentType: '',
            otherEditParentType: '',
            addStatus: '',
            editStatus: '',
            editStatusVisible: false,
            addStatusVisible: false,
            deleteStatusVisible: false,
            addStatusName: '',
            addStatusNumber: '',
            editParentTypeVisible: false,
            addParentTypeVisible: false,
            deleteParentTypeVisible: false,
            addParentTypeName: '',
            addParentTypeNumber: '',
            headListTotal: 0,
            currentPage: 1,
            deleteSelectedHeadVisible: false,
            isPagination: false,
            allHeadList: [],
            selectedHeadList: [],
            selectedShowList: '',
            leftTransferData: [],
            rightTransferData: [],
            saveHeadListVisible: false
        }
    },
    i18n: {
        messages: {
            en: LAN.EN.Component.Order.List,
            zh: LAN.ZH.Component.Order.List,
            de: LAN.DE.Component.Order.List,
            nl: LAN.NL.Component.Order.List
        }
    },
    methods: {
        back(){
            this.$router.push({
                name: 'createParam'
            })
        },
        //headlist 分页
        handleCurrentChange(val){
            Core.Api.request({
                url: '/param/head/list/filter?page='+val+'&size='+10,
                method: 'POST',
                data: {
                    "parent_type": this.formSearch.parentType,
                    "head_name": this.formSearch.headNameInput,
                    "status": this.formSearch.status
                },
            }).then(res=>{
                if(res.content && res.content.length>0){
                    this.headListTotal = res.totalElements;
                    this.$message({
                        type: 'success',
                        message: 'Search successfully!'
                    });
                    var headList = res.content.filter((item)=>{
                        return item.head_name !='';
                    })
                    this.headListTable = [];
                    if(headList.length<=0){
                        this.isPagination = false;
                        this.$message({
                            type: 'warning',
                            message: '暂无数据，即将搜索全部数据'
                        })
                        var that = this;
                        setTimeout(function(){
                            that.getParamHead();
                        },1000);
                        return;
                    }else{
                        this.isPagination = true;
                        for(var item of headList){
                            if(item.head_name !=null){
                                var headItem = {
                                    headName: item.head_name,
                                    parentType: item.parent_type,
                                    id: item.id,
                                    status: item.status
                                };
                                this.headListTable.push(headItem);
                            }
                        }
                        this.headListTable.forEach((item)=>{
                            this.categoryTypeData.forEach((categoryItem)=>{
                                if(categoryItem){
                                    if(item.parentType == categoryItem.value){
                                        item.parentType = categoryItem.label;
                                    }
                                }
                            });
                            this.statusData.forEach((statusItem)=>{
                                if(statusItem){
                                    if(item.status == statusItem.value){
                                        item.status = statusItem.label;
                                    }
                                }
                            })
                            if(this.$route.query && this.$route.query.showList &&  this.$route.query.showList!=''){
                                var showList = this.$route.query.showList.substring(this.$route.query.showList.indexOf(',')+1).split(',');
                                showList.forEach((showListItem)=>{
                                    if(item.id == showListItem){
                                        this.multipleSelection.push(item);
                                    }
                                })

                                this.toggleSelection(this.multipleSelection);
                            }
                        })
                    }
                }else{
                    this.$message({
                        type: 'warning',
                        message: 'No data,return to the main page in two seconds'
                    });
                    this.headListTable = [];
                    var that = this;
                    // setTimeout(function(){
                    //     console.log(1111);
                    //     that.reload();
                    //     that.getParamHead();
                    // },2000);

                }
            }).catch(err=>{
                console.log(err);
            })
        },
        getHeadList(){
            Core.Api.request({
                method: 'GET',
                url: '/param/head/all',
            }).then(res=>{
                this.headListTable = res.filter((item)=>{return item.head_name!=null});
            }).catch(err=>{
                console.log(err);
            })
        },
        onSearchHeadList(){
            // if(this.formSearch.parentType == ''){
            //     this.$message({
            //         type: 'warning',
            //         message: 'please select parent type'
            //     });
            //     return;
            // }
            // if(this.formSearch.status == ''){
            //     this.$message({
            //         type: 'warning',
            //         message: 'please select status'
            //     });
            //     return;
            // }
            if(isNaN(this.formSearch.parentType)){
                this.categoryTypeData.forEach((item)=>{
                    if(item.label == this.formSearch.parentType){
                        this.formSearch.parentType = item.value;
                    }
                })
            }
            if(isNaN(this.formSearch.status)){
                this.statusData.forEach((item)=>{
                    if(item.label == this.formSearch.status){
                        this.formSearch.status = item.value;
                    }
                })
            }
            var that = this;
            var data = {
                "parent_type": that.formSearch.parentType,
                "status": that.formSearch.status
            }
            if(that.formSearch.headNameInput!=''){
                data.head_name = that.formSearch.headNameInput;
            }
            if(that.formSearch.headId!=''){
                data.id = that.formSearch.headId;
            }
            Core.Api.request({
                url: '/param/head/list/filterAll',
                method: 'POST',
                data: data
            }).then(res=>{
                if(res && res.length>0){
                // if(res.content && res.content.length>0){
                    var headList = res.filter((item)=>{
                        return item.head_name!='';
                    })
                    if(headList.length<=0){
                        that.isPagination = false;
                        that.headListTable = [];
                        that.formSearch.parentType = '';
                        that.formSearch.headNameInput = '';
                        that.formSearch.status = '';
                        that.$message({
                            type: 'warning',
                            message: '暂无数据，即将搜索全部数据'
                        })
                        return false;
                    }
                    that.isPagination = true;
                    that.headListTotal = res.totalElements;
                    that.allHeadList = [];
                    that.$message({
                        type: 'success',
                        message: 'Search successfully!'
                    });

                    that.headListTable = [];
                    that.multipleSelection = [];
                    headList.forEach((item,index)=>{
                        if(item.head_name !=null){
                            var headItem = {
                                headName: item.head_name,
                                parentType: item.parent_type,
                                id: item.id,
                                status: item.status
                            };
                            that.headListTable.push(headItem);
                            var allHeadListItem = {
                                key: index,
                                label: item.head_name,
                                parentTypeValue: item.parent_type,
                                id: item.id,
                                statusValue: item.status,
                                title: item.head_name
                            }
                            that.categoryTypeData.forEach((categoryItem)=>{
                                if(categoryItem){
                                    if(item.parent_type == categoryItem.value){
                                        allHeadListItem.parentTypeName = categoryItem.label;
                                    }
                                }
                            });
                            that.statusData.forEach((statusItem)=>{
                                if(statusItem){
                                    if(item.status == statusItem.value){
                                        allHeadListItem.statusName = statusItem.label;
                                    }
                                }
                            })
                            that.allHeadList.push(allHeadListItem);
                        }
                    })
                    // that.headListTable.forEach((item)=>{
                    //     that.categoryTypeData.forEach((categoryItem)=>{
                    //         if(categoryItem){
                    //             if(item.parentType == categoryItem.value){
                    //                 item.parentType = categoryItem.label;
                    //             }
                    //         }
                    //     });
                    //     that.statusData.forEach((statusItem)=>{
                    //         if(statusItem){
                    //             if(item.status == statusItem.value){
                    //                 item.status = statusItem.label;
                    //             }
                    //         }
                    //     })
                    //     if(that.$route.query && that.$route.query.showList &&  that.$route.query.showList!=''){
                    //         var showList = that.$route.query.showList.substring(that.$route.query.showList.indexOf(',')+1).split(',');
                    //         showList.forEach((showListItem)=>{
                    //             if(item.id == showListItem){
                    //                 that.multipleSelection.push(item);
                    //             }
                    //         })
                    //         that.toggleSelection(that.multipleSelection);
                    //     }
                    // })
                    if(that.$route.query && that.$route.query.showList &&  that.$route.query.showList!=''){
                        var showList = that.$route.query.showList.substring(that.$route.query.showList.indexOf(',')+1).split(',');
                        that.allHeadList.forEach((item,index)=>{
                            showList.forEach((showListItem,showIndex)=>{
                                if(item.id == showListItem){
                                    if(that.selectedHeadList.indexOf(index)<=-1){
                                        that.selectedHeadList.push(index);
                                        showList.splice(showIndex,1)
                                    }
                                }
                            })
                        })
                        //that.rightTransferData = that.selectedHeadList.concat();
                    }
                }else{
                    that.$message({
                        type: 'warning',
                        message: 'No data,return to the main page in two seconds'
                    });
                    that.headListTable = [];
                    that.formSearch.parentType = '';
                    that.formSearch.headNameInput = '';
                    that.formSearch.status = '';
                     setTimeout(function(){
                        that.getParamHead();
                    },1000);

                }
                // if(res.content && res.content.length>0){
                //     this.$message({
                //         type: 'success',
                //         message: 'Search successfully!'
                //     });
                //     this.headListTable = [];
                //     for(var item of res.content){
                //         if(item.head_name !=null){
                //             var headItem = {
                //                 headName: item.head_name,
                //                 parentType: item.parent_type,
                //                 id: item.id
                //             };
                //             this.headListTable.push(headItem);
                //         }
                //     }
                //     this.headListTable.forEach((item)=>{
                //         this.categoryTypeData.forEach((categoryItem)=>{
                //             if(categoryItem){
                //                 if(item.parentType == categoryItem.value){
                //                     item.parentType = categoryItem.label;
                //                 }
                //             }
                //         })
                //     })
                // }else{
                //     this.$message({
                //         type: 'warning',
                //         message: 'No data'
                //     });
                //     this.headListTable = [];
                // }
                // console.log(this.headListTable);
            }).catch(err=>{
                console.log(err);
            })

        },
        onClean(){

        },
        addHead(){
            this.addHeadVisible = true;
        },
        showHeadDetail(item,index){
            // console.log(item,index);
            this.$router.push({
                path: '/component/headList/detail',
                query:{
                    parentType: item.parentType
                }
            });
        },
        editHead(item,index){
            // console.log(item,index);
            if(this.multipleSelection.length>0){
                this.$message({
                    type: 'warning',
                    message: 'sorry,multiple edit is not allowed'
                });
                return;
            }
            this.editId = item.id;
            this.dialogEditVisible = true;
            this.editHeadName = item.headName;
            this.editIndex = index;
            for(var option of this.categoryTypeData){
                if(option.label == item.parentType){
                    this.editParentType = option.value;
                }
            }
            for(var statusItem of this.statusData){
                if(statusItem.label == item.status){
                    this.editStatus = statusItem.value;
                }
            }
            // console.log(this.editParentType);
        },
        deleteHead(item,index){
            this.deleteHeadVisible = true;
            this.deleteId = item.id;
            // console.log(item,index)
        },
        deleteSelectHeadList(){
            this.deleteSelectedHeadVisible = true;
        },
        confirmDeleteSelected(){
            var multipleChoiceDelete = [];
            this.allHeadList.forEach((item)=>{
                multipleChoiceDelete.push(item.id);
            })
            Core.Api.request({
                url: '/param/head/list/bulkRemove',
                method: 'DELETE',
                body:JSON.stringify(multipleChoiceDelete)
            }).then(res=>{
                // this.headListTable.forEach((item,index)=>{
                //     this.multipleSelection.forEach((multipleItem)=>{
                //         if(multipleItem){
                //             if(multipleItem.id == item.id){
                //                 this.headListTable.splice(index,1);
                //             }
                //         }
                //     })
                // })
                // this.multipleSelection = [];
                this.allHeadList = [];
                this.deleteSelectedHeadVisible = false;
                // this.reload();
            }).catch(err=>{
                console.log(err);
            })
        },
        editHeadData(){
            var data = {
                "head_name": this.editHeadName,
                "id": this.editId?this.editId:0,
                "parent_type": this.editParentType,
                "status": this.editStatus,
            }
            var that = this;
            Core.Api.request({
                url: '/param/head/list',
                method: 'POST',
                data: data
            }).then(res=>{
                that.dialogEditVisible = false;
                for(var option of that.categoryTypeData){
                    if(option.value == res.parent_type){
                        that.headListTable[that.editIndex].parentType = option.label;
                        that.allHeadList[that.editIndex].parentTypeName = option.label;
                        that.allHeadList[that.editIndex].parentTypeValue = option.value
                    }
                }
                for(var statusItem of that.statusData){
                    if(statusItem.value == res.status){
                        that.headListTable[that.editIndex].status = statusItem.label;
                        that.allHeadList[that.editIndex].statusName = statusItem.label;
                        that.allHeadList[that.editIndex].statusValue = statusItem.value
                    }
                }
                that.headListTable[that.editIndex].headName = res.head_name;
                that.allHeadList[that.editIndex].headName = res.head_name;
                that.allHeadList[that.editIndex].label = res.head_name;
                // this.reload();
            }).catch(err=>{
                console.log(err);
            });
        },
        confirmDelete(){
            var that = this;
            Core.Api.request({
                url: '/param/head/list/'+this.deleteId,
                method: 'DELETE',
            }).then(res=>{
                if(res.code == 202 && res.message == 'updated'){
                    that.$message({
                        type: 'success',
                        message: 'delete successfully'
                    })
                    that.headListTable.forEach((item,index)=>{
                        if(item.id == that.deleteId){
                            that.headListTable.splice(index,1);
                        }
                    })
                    that.allHeadList = that.allHeadList.filter((item)=>{
                        return item.id != that.deleteId;
                    })
                    that.deleteId = '';
                    that.deleteHeadVisible = false;
                }

                // this.reload();
            }).catch(err=>{
                console.log(err);
            })
        },
        cancelDelete(){
            this.deleteId = 0;
            this.deleteHeadVisible = false;
        },
        confirmAdd(){
            var data = {
                "head_name": this.addHeadName,
                "id": 0,
                "parent_type": this.addParentType,
                "status": this.addStatus
            }
            Core.Api.request({
                url: '/param/head/list',
                method: 'POST',
                data: data
            }).then(res=>{
               if(res.id){
                this.$message({
                    type: 'success',
                    message: 'add successfully'
                })
                var status,parentType;
                this.statusData.forEach((item)=>{
                    if(item.value == res.status){
                        status = item.label;
                    }
                })
                 this.categoryTypeData.forEach((categoryItem)=>{
                    if(res.parent_type == categoryItem.value){
                        parentType = categoryItem.label;
                    }
                })
                var item = {
                    headName: res.head_name,
                    id: res.id,
                    parentType: parentType,
                    status: status
                }
                this.headListTable.push(item);
                this.addHeadName = '';
                this.addParentType = '';
                this.addStatus = '';
                this.addHeadVisible = false;
               }
               //this.reload();
            }).catch(err=>{
               console.log(err);
            });
        },
        cancelAdd(){
            this.addHeadVisible = false;
        },
        getParamHead(){
            this.headListTable = [];
            Core.Api.request({
                url: '/param/head/list/all',
                method:'GET',
            }).then(res=>{
                this.isPagination = false;
                this.currentPage = 1;
                if(res == null || res == undefined || res.length<=0){
                    return;
                }
                this.headListTable = [];
                this.allHeadList = [];
                var headListTable = res.filter((item)=>{return item.head_name !=''});
                if(headListTable.length<=0){
                    this.$message({
                        type: 'warning',
                        message: 'no data'
                    })
                    return;
                }
                headListTable.forEach((item,index)=>{
                     if(item.head_name !=null&&item.head_name !=''){
                        var transferHeadItem = {
                                key: index,
                                id: item.id,
                                label: item.head_name,
                                title: item.head_name,
                                parentTypeValue: item.parent_type,
                                statusValue: item.status
                            }
                        this.categoryTypeData.forEach((categoryItem)=>{
                            if(categoryItem){
                                if(item.parent_type == categoryItem.value){
                                    transferHeadItem.parentTypeName = categoryItem.label;
                                }
                            }
                        });
                        this.statusData.forEach((statusItem)=>{
                            if(statusItem){
                                if(item.status == statusItem.value){
                                    transferHeadItem.statusName = statusItem.label;
                                }
                            }
                        })
                        this.categoryTypeData.forEach((categoryItem)=>{
                            if(categoryItem){
                                if(item.parent_type == categoryItem.value){
                                    item.parent_type = categoryItem.label;
                                }
                            }
                        })
                        this.statusData.forEach((statusItem)=>{
                            if(item.status == statusItem.value){
                                item.status = statusItem.label;
                            }
                        })
                        this.allHeadList.push(transferHeadItem);
                        var headItem = {
                            headName: item.head_name,
                            parentType: item.parent_type,
                            id: item.id,
                            status: item.status?item.status:'-'
                        };

                        this.headListTable.push(headItem);
                     }
                })
                // for(var item of headListTable){
                //     if(item.head_name !=null&&item.head_name !=''){
                //         var headItem = {
                //             headName: item.head_name,
                //             parentType: item.parent_type,
                //             id: item.id,
                //             status: item.status?item.status:'-'
                //         };
                //         this.headListTable.push(headItem);
                //     }
                // }
                if(this.$route.query && this.$route.query.showList &&  this.$route.query.showList!=''){
                    var showList = this.$route.query.showList.substring(this.$route.query.showList.indexOf(',')+1).split(',');
                    this.allHeadList.forEach((item,index)=>{
                        showList.forEach((showListItem,showIndex)=>{
                            if(item.id == showListItem){
                                if(this.selectedHeadList.indexOf(index)<=-1){
                                    this.selectedHeadList.push(index);
                                    showList.splice(showIndex,1)
                                }
                            }
                        })
                    })
                }
            }).catch(err=>{
                console.log(err);
            })
        },
        toggleSelection(rows) {
            if (rows) {
                this.$nextTick(()=>{
                    rows.forEach(row => {
                        this.$refs.multipleTable.toggleRowSelection(row,true);
                    });
                })
            } else {
                this.$refs.multipleTable.clearSelection();
            }
        },
        changeStatusItem(data){
            console.log(data);
        },
        changeStatus(val){
            if(val == 999){
                this.editStatusVisible = true;
                this.addStatusVisible = true;
                this.deleteStatusVisible = false;
            }else if(val == 1000){
                this.editStatusVisible = true;
                this.addStatusVisible = false;
                this.deleteStatusVisible = true;
            }
        },
        changeParentType(val){
            if(val == 1001){
                this.editParentTypeVisible = true;
                this.addParentTypeVisible = true;
                this.deleteParentTypeVisible = false;
            }else if(val == 1002){
                this.editParentTypeVisible = true;
                this.addParentTypeVisible = false;
                this.deleteParentTypeVisible = true;
            }
        },
        handleSelectionChange(val){
            this.multipleSelection = val;
        },
        headScroll(e){
            console.log(e);
        },
        saveHeadList(){
            // if(this.selectedHeadList.length<=0){
            //     this.$message({
            //         type: 'warning',
            //         message: 'please select head list at first'
            //     });
            //     return false;
            // };
            this.saveHeadListVisible = true;
            var showList = '',childList = '';
            var multipleSelection = Array.from(new Set(this.selectedHeadList));
            multipleSelection.forEach((item)=>{
                showList+=(this.allHeadList[item].id+',')
            });
            var checkedHeadItem = {
                "category_id":  this.$route.query.categoryId,
                "child_list": this.$route.query.list,
                "show_list": showList.lastIndexOf(',') ==showList.length-1?showList.substring(0,showList.lastIndexOf(',')):showList,
                "parent_type": this.formSearch.parentType,
                "parent_id": this.$route.query.parentId,
                "root_id": this.$route.query.rootId,
                "id": this.$route.query.headListId?this.$route.query.headListId:0
            };
            Core.Api.request({
                url: '/param/head/binding/save',
                method:'POST',
                data: JSON.stringify(checkedHeadItem)
            }).then(res=>{
                if(res.id){
                    this.saveHeadListVisible = false;
                    this.$message({
                        type: 'success',
                        message: 'save successfully!'
                    });
                    this.$router.push({
                        name: 'createParam'
                    });
                }else{
                    this.saveHeadListVisible = false;
                }
            }).catch(err=>{
                console.log(err);
            })
        },
        saveSingleHeadList(item,index){
            var showList = '',childList = '';
            showList+=item.id;
            var checkedHeadItem = {
                "category_id":  this.$route.query.categoryId,
                "child_list": this.$route.query.list,
                "show_list": showList.lastIndexOf(',') ==showList.length-1?showList.substring(0,showList.lastIndexOf(',')):showList,
                "parent_type": this.$route.query.parentType,
                "parent_id": this.$route.query.parentId,
                "root_id": this.$route.query.rootId,
                "id": this.$route.query.headListId?this.$route.query.headListId:0
            };
            Core.Api.request({
                url: '/param/head/binding/save',
                method:'POST',
                data: JSON.stringify(checkedHeadItem)
            }).then(res=>{
                if(res.id){
                    this.$message({
                        type: 'success',
                        message: 'save successfully!'
                    });
                    this.$router.push({
                        name: 'createParam'
                    });
                }
            }).catch(err=>{
                console.log(err);
            })
        },
        addStatusData(){
             var statusList = [];
            this.statusData.forEach((item)=>{
                if(item.label != '+' && item.label !='-'){
                    statusList.push(item.value);
                }
            })
            var maxStatus = Math.max(...statusList)
            if(maxStatus == 998){
                maxStatus = 1000;
            }
            var data = {
                "id": 0,
                "key": maxStatus+1,
                "value": this.addStatusName,
                "type": 1,
                "type_name": 'headStatus'
            };
            Core.Api.request({
                url: '/dictionary',
                method: 'POST',
                data:data
            }).then(res=>{
                console.log(res);
                if(res.id){
                    this.editStatusVisible = false;
                    this.addStatusNumber = '';
                    this.addStatusName = '';
                    this.getStatusOrParentType();
                }
            }).catch(err=>{
                console.log(err);
            })
        },
        addcategoryTypeData(){
            var parentTypeList = [];
            this.categoryTypeData.forEach((item)=>{
                if(item.label != '+' && item.label !='-'){
                    parentTypeList.push(item.value);
                }
            })
            var maxParentType = Math.max(...parentTypeList)
            if(maxParentType == 1000){
                maxParentType = 1002;
            }
            var data = {
                "id": 0,
                "key": maxParentType+1,
                "value": this.addParentTypeName,
                "type": 0,
                "type_name": 'headParentType'
            };
            Core.Api.request({
                url: '/dictionary',
                method: 'POST',
                data:data
            }).then(res=>{
                console.log(res);
                if(res.id){
                    this.editParentTypeVisible = false;
                    this.addParentTypeNumber = '';
                    this.addParentTypeName = '';
                    this.getStatusOrParentType();
                }
            }).catch(err=>{
                console.log(err);
            })
        },
        getStatusOrParentType(){
            Core.Api.request({
                url: '/dictionary/filterAll',
                method:'POST'
            }).then(res=>{
                if(res&&res.length>0){
                    this.statusData = [];
                    this.categoryTypeData = [];
                    var statusData = [],
                        categoryTypeData = [];
                    res.forEach((item)=>{
                        if(item.type_name == 'headStatus'){
                            var statusItem = {
                                label: item.value,
                                value: item.key,
                                id: item.id
                            }
                            statusData.push(statusItem);
                        }else if(item.type_name == 'headParentType'){
                             var parentTypeItem = {
                                label: item.value,
                                value: item.key,
                                id: item.id
                            }
                            categoryTypeData.push(parentTypeItem);
                        }
                    })
                    statusData.sort(function(a,b){
                        return a.value-b.value;
                    });
                    categoryTypeData.sort(function(a,b){
                        return a.value-b.value;
                    })
                    this.statusData = Array.from(new Set(statusData));
                    this.categoryTypeData = Array.from(new Set(categoryTypeData));
                    this.statusData.forEach((item)=>{
                        if(item.value == this.$route.query.rootId){
                            this.formSearch.status = item.label;
                        }
                    });
                    this.categoryTypeData.forEach((item)=>{
                        if(item.value == this.$route.query.parentType){
                            this.formSearch.parentType = item.label;
                        }
                    });
                    //this.getParamHead();
                    var that = this;
                    that.onSearchHeadList();
                    // setTimeout(function(){
                    //     that.onSearchHeadList();
                    // },2000);
                }
            }).catch(err=>{
                console.log(err);
            })
        },

        deleteStatusItem(item){
            Core.Api.request({
                    url: '/dictionary/' + item.id,
                    method: 'DELETE'
                }).then((res) => {
                    console.log(res);
                    this.$message({
                        type: 'success',
                        message: 'delete successfully'
                    })
                    //this.editStatusVisible = false;
                    this.getStatusOrParentType();
                }).catch(err=>{
                    console.log(err);
                })
        },
        deleteParentType(item){
            Core.Api.request({
                url: '/dictionary/' + item.id,
                method: 'DELETE'
            }).then((res) => {
                console.log(res);
                this.$message({
                    type: 'success',
                    message: 'delete successfully'
                })
                this.editStatusVisible = false;
                this.getStatusOrParentType();
            }).catch(err=>{
                console.log(err);
            })
        },
        getFilterHeadList(){
            Core.Api.request({
                url: '/param/head/list/filter?page=1&size=10',
                method: 'POST',
            }).then(res=>{
                console.log(res);
                this.headListTotal = res.totalElements;
            }).catch(err=>{
                console.log(err);
            })
        },
        changeHeadList(e){
            console.log(e)
        },
        selectTransferData(currentValue,direction,data){
            // if(direction == 'left'){
            //     this.rightTransferData = this.rightTransferData.filter((item)=>{
            //         return !data.includes(item);
            //     });
            // }else{
            //     this.rightTransferData = this.selectedHeadList;
            // }
        },
        renderTransferData(h,option){
            return (
                <span class="transfer-data">
                    <span  class="transfer-item" on-click={ (e) => {e.stopPropagation();this.changeHeadList(e)}}>{option.label}</span>
                    <span class="parent-type-name">{option.parentTypeName}</span>
                    <span class="status-name">{option.statusName}</span>
                    <span class="edit-transfer-data" on-click={ (e) => {e.stopPropagation();e.preventDefault(); this.editTransferData(option)}}>edit</span>
                    <span class="delete-transfer-data" on-click={ (e) => {e.stopPropagation();e.preventDefault(); this.deleteTransferData(option)}}>delete</span>
                </span>
            )

        },
        editTransferData(option){
            this.editId = option.id;
            this.dialogEditVisible = true;
            this.editHeadName = option.label;
            this.editIndex = this.allHeadList.indexOf(option);
            this.editParentType = option.parentTypeValue;
            for(var statusItem of this.statusData){
                if(statusItem.label == option.status){
                    this.editStatus = statusItem.value;
                }
            }
            this.editStatus = option.statusValue;
        },
        deleteTransferData(option){
            this.deleteHeadVisible = true;
            this.deleteId = option.id;
        },
        deleteAllHeadList(){
            this.deleteSelectedHeadVisible = true;
        }
    },
    mounted() {
        if(this.$route.query.parentType){
            this.formSearch.parentType = this.$route.query.parentType;
        }
        if(this.$route.query.componentType){
            this.formSearch.status = this.$route.query.status;
        }
        this.getStatusOrParentType();

        this.$store.state.getParamHead = this.getParamHead;
    },
}
</script>
<style lang="scss">
    .list-form-inline{
        .el-form-item__content{
            width: 120px!important;
        }
    }
    .status-item{
        display: flex;
        justify-content: space-between;
        margin-bottom: 10px;
    }
    .save-head-list-form .el-form-item{
        margin-bottom: 0;
    }
    .save-head-list{
        display: flex;
        justify-content: flex-end;
        margin-bottom: 20px;
    }
    .head-list-header{
        padding: 20px 40px;
        border-bottom: 0;
    }
    .select-transfer{
        margin-left: 20px;
        // margin-top: 20px;
        .el-transfer-panel__body{
            height: 500px;
            .el-input__inner{
                background-color:#2B2F3B!important;
                border: none;
            }
        }
        .el-transfer-panel__list.is-filterable{
            height: 450px;
        }
        .el-transfer-panel{
            // width: 600px;
            width: 43%;
            .transfer-data{
                width: 100%;
                display: flex;
                text-align: center;
                .transfer-item{
                    flex: 0 0 30%;
                    overflow: hidden;
                    white-space: nowrap;
                    text-overflow: ellipsis;
                }
                .parent-type-name{
                   flex: 0 0 25%;
                    overflow: hidden;
                    white-space: nowrap;
                    text-overflow: ellipsis;
                }
                .status-name{
                    flex: 0 0 25%;
                    overflow: hidden;
                    white-space: nowrap;
                    text-overflow: ellipsis;
                }
                .edit-transfer-data{
                    flex: 0 0 10%;
                }
                .delete-transfer-data{
                    flex: 0 0 10%;
                }
            }
            .el-transfer-panel__filter{
                text-align: right;
                input{
                    width: 60%;
                }
            }
        }
    }
    .add-head-data{
        .head-form{
            .el-form-item{
                display: flex;
                .el-form-item__label{
                    width: 120px;
                    text-align: center;
                }
                .el-form-item__content{
                    .el-input{
                        width: 280px;
                    }
                    .el-select{
                        width: 280px;
                    }
                }
            }
        }
    }
    .edit-head-data{
        .el-form-item{
            display: flex;
            .el-form-item__label{
                width: 120px;
                text-align: center;
            }
            .el-form-item__content{
                .el-input{
                    width: 240px;
                }
                .el-select{
                    width: 240px;
                }
            }
        }
    }

</style>




// WEBPACK FOOTER //
// list.vue?7dbd6a4e