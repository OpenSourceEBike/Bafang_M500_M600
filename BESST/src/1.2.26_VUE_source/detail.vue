<template>
    <div class="dictionary-page">
        <div class="header">
            <el-form :inline="true" class="dictionary-base-info">
                <el-form-item :label="$t('字典/词条名')">
                    <el-input  v-model="dictionaryName"></el-input>
                </el-form-item>
                <el-form-item :label="$t('code')">
                    <el-input  v-model="dictionaryCode"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search"   @click="getDicList()" :title="$t('dic.search')"></div>
                    </div>
                </div>
                <div class="btn-group-top">
                    <div class="icons">
                        <div class="icon-item add" @click="addDictionaryList()" :title="$t('add')"></div>
                    </div>
                </div>
            </el-form>
            <div class="flex">
                <div class="width-33">
                    <div class="title">字典/词条名</div>
                    <div class="dic-content">
                        <div v-for="(item,index) in dicList" :key="index" @click="getDicDetail(item)" class="info" :class='{"active" : active == item.name}'>
                            <span >{{item.name}}</span>
                            <span class="icon">                    
                                <i class="el-icon-edit" v-on:click.stop='showEditDialog(item.id)'></i>
                                <i class="el-icon-delete"  v-on:click.stop='showDeleteDialog(item.id)'></i>
                            </span>
                        </div>
                    </div>
                </div>
                <div class="width-66">  
                    <div class="width-50">
                        <div class="title">硬件</div>
                        <div class="dic-content">
                            <div v-for="(item,index) in hardware"  :key="index" @click="getHeadNameAndValue(item.serial_number)" class="info">
                                <span>{{item.serial_number}}</span>
                            </div>
                        </div>           
                    </div>
                    <div class="width-50">
                        <div class="title">固件</div>
                        <div class="dic-content">
                            <div v-for="(item,index) in firmware" :key="index" @click="firmwareDetail(item.parent_id,item.serial_number)"  class="info">
                                <span>{{item.serial_number}}</span>
                            </div>
                        </div>
                    </div>
                    <div class="width-50">
                        <div class="title">参数</div>
                        <div class="dic-content">
                            <div v-for="(item,index) in params" :key="index"  class="info normal" @click="getHeadNameAndValue(item.serial_number)">
                                <span>{{item.serial_number}}</span>
                            </div>
                        </div>           
                    </div>
                    <div class="width-50">
                        <div class="title">属性</div>
                        <div class="dic-content">
                            <div v-for="(item,index) in attrs" :key="index" class="info normal">
                                <span class="mr-10">{{item.property_name+":"}}</span><span>{{item.property_list.toString()}}</span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="table-content-holder">
                <table class="table-content"  cellspacing="0" cellpadding="0">
                    <thead > 
                        <tr>
                            <th  v-for="(item, index) in headData" :key="index" >{{item.head_name}}</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td v-for="(item, index) in headData" :key="index">{{item.value == '' ? '/' : item.value}}</td> 
                        </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="headData.length<1 && !loading">
                    {{ 'noData' }}
                </div>
            </div>
        </div>
        <el-dialog :title="$t('Delete Dictionary')" :visible.sync="dialogDeleteVisible" size="tiny">
            <p> {{ $t('Are you sure delete the dictionary?') }} </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="deleteDictionary">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>
        <el-dialog title="Edit Dictionary Data" :visible.sync="dialogEditVisible" custom-class="editDialog">
            <div>
                <div class="editDiv" >
                    <div class="params">
                        <div class="title">硬件：</div>
                        <div class="input max">
                            <div>
                                <div class="selectList" v-for="(item,index) in hardwareValue" :key="index">
                                    <el-select v-model="item.serial_number"  @change="changeData(item,hardwareValue)" filterable>
                                        <el-option v-for="(value,key) in hardwareList"  :key="key" :label="value.serial_number" :value="value.serial_number">
                                        </el-option>
                                    </el-select>
                                    <span class="icon">
                                        <i class="el-icon-delete" v-on:click.stop='deleteData(item,hardwareValue)'></i>
                                    </span>
                                </div>
                                <div style="display:inline-block;" v-if="hardwareValue.length<4">
                                </div>
                            </div>
                             
                        </div>
                        <el-button class="add-btn" @click="addData(hardwareValue)" :class="hardwareValue.length == 0 ? 'noData' : hardwareValue.length >=4 ? 'overflow' : '' " type="primary" >
                            <span class="icon">
                                <i class="el-icon-plus"></i>
                            </span>
                        </el-button>
                        <div class="clear" style="clear:both"></div>
                    </div>
                    <div class="params">
                        <div class="title">固件：</div>
                        <div class="input max">
                            <div>
                                <div class="selectList" v-for="(item,index) in firmwareValue" :key="index"> 
                                    <el-select v-model="item.serial_number" @change="changeData(item,firmwareValue)" filterable>
                                        <el-option v-for="(value,key) in firmwareList" :key="key" :label="value.serial_number" :value="value.serial_number">
                                        </el-option>
                                    </el-select>
                                    <span class="icon">
                                        <i class="el-icon-delete" v-on:click.stop='deleteData(item,firmwareValue)'></i>
                                    </span>
                                </div>
                                <div style="display:inline-block;" v-if="firmwareValue.length<4">
                                </div>
                            </div>
                        </div>
                        <el-button class="add-btn" @click="addData(firmwareValue)" :class="firmwareValue.length == 0 ? 'noData' : firmwareValue.length >=4 ? 'overflow' : '' " type="primary" >
                            <span class="icon">
                                <i class="el-icon-plus"></i>
                            </span>
                        </el-button> 
                    
                        <div class="clear" style="clear:both"></div>
                    </div>
                   <div class="params">
                        <div class="title">参数：</div>
                        <div class="input max" >
                            <div>
                                <div class="selectList" v-for="(item,index) in paramsValue" :key="index">
                                    <el-select v-model="item.serial_number" @change="changeData(item,paramsValue)" filterable>
                                        <el-option v-for="(value,key) in paramsList" :key="key" :label="value.serial_number" :value="value.serial_number">
                                        </el-option>
                                    </el-select>
                                    <span class="icon">
                                        <i class="el-icon-delete" v-on:click.stop='deleteData(item,paramsValue)'></i>
                                    </span>
                                </div>
                                <div style="display:inline-block;" v-if="paramsValue.length<4">
                                </div> 
                            </div>
                        </div>
                        <el-button class="add-btn" @click="addData(paramsValue)" :class="paramsValue.length == 0 ? 'noData' : paramsValue.length >=4 ? 'overflow' : '' " type="primary" >
                            <span class="icon">
                                <i class="el-icon-plus"></i>
                            </span>
                        </el-button>
                        <div class="clear" style="clear:both"></div>
                    </div>
                </div>
                <div class="editDiv top">
                     <div class="params">
                        <div class="title">属性：</div>
                        <div class="input">
                            <div>
                                <div class="selectList"  v-for="(item,index) in attrValue" :key="index">
                                    <el-select v-model="item.property_name" @change="getAttrDetail(item,attrValue)" filterable>
                                        <el-option v-for="(attr,key) in attrList" :label="attr.propertyName" :value="attr.propertyName" :key="key">
                                        </el-option>
                                    </el-select>
                                    <span class="icon" style="margin-right:10px;">
                                        <i class="el-icon-delete" v-on:click.stop='deleteData(item,attrValue)'></i>
                                    </span>
                                    <span class="icon" style="margin-right:10px;">
                                        <i class="el-icon-view"></i>
                                    </span>
                                    <div class="checkBoxGroup" v-if="item.property_name">
                                        <el-checkbox-group v-model="item.property_list">
                                            <el-checkbox v-for="(attr,index) in attrDetail[item.property_name]" :label="attr" :key="index" >{{attr}}</el-checkbox>
                                        </el-checkbox-group>
                                    </div>
                                </div>
                                <div style="display:inline-block; ">
                                </div> 
                            </div>
                        </div>
                        <el-button class="add-btn" @click="addData(attrValue)" type="primary" :class="attrValue.length == 0 ? 'noData' : ''">
                            <span class="icon">
                                <i class="el-icon-plus"></i>
                            </span>
                        </el-button>
                        <div class="clear" style="clear:both"></div>
                    </div>    
                </div>   
            </div>
                <el-button type="primary" @click="saveEditData" style="margin-top:10px;">{{ $t('save') }}</el-button>
        </el-dialog>
    </div>
</template>
<script>
    import Core from 'core/core'
    export default {
        inject: ['reload'],
        data() {
            return{
                dictionaryName:'',
                dictionaryCode: '',
                dicList:{},
                hardware:[],
                firmware:[],
                params:[],
                headData: [],
                loading: false,
                active:'',
                name:'',
                dialogDeleteVisible:false,
                dialogEditVisible:false,
                selectId:'',
                detailId:'',
                attrs:[],
                hardwareList: [],
                firmwareList: [],
                paramsList: [],
                attrList:[],
                firmwareValue:[],
                hardwareValue:[],
                paramsValue:[],
                attrValue:[],
                attrDetail: {},
                serialNumberList:[],
                categoryTypeData:[
                    {value:1,name: "MOTOR"},
                    {value:2,name: "CONTROLLER"},
                    {value:3,name: "HMI"},
                    {value:4,name: "SENSOR"},
                    {value:5,name: "BATTERY"},
                    {value:6,name: "CHARGER"},
                    {value:7,name: "TOOL"},
                    {value:8,name: "WIRELESS"},
                    {value:9,name: "DC/DC模块"}
                ],
                attrDetailList:[]
            }
        },
        methods:{
            getDicList() {
                this.loading = true;
                this.dictionaryName = this.dictionaryName.trim();
                Core.Api.request({
                    url: '/phrase/fuzzySearchPhraseName',
                    method: 'GET',
                    data:{
                        name:this.dictionaryName,
                    }
                }).then(res=>{
                    if(res.length>0){
                        this.loading = false;
                        this.dicList = res;
                    }else{
                        this.dicList = {};
                        this.loading = false;
                        this.$message({
                            type: 'warning',
                            message: 'the entry name is not existed!'
                        });
                    }

                }).catch(err=>{
                    console.log(err);
                })
            },
            getDicDetail(val){
                this.loading = true;
                this.detailId = val.id;
                this.active = val.name;
                this.name = val.name;
                this.hardware = [];
                this.firmware = [];
                this.params = [];
                this.attrs = [];
                Core.Api.request({
                    url: '/phrase/selectDataById',
                    method: 'GET',
                    data:{
                        id:val.id,
                    }
                }).then(res=>{
                    this.loading = false;
                    this.hardware = res.data.hardware;
                    this.firmware = res.data.firmware;
                    this.params = res.data.parameter;
                    this.attrs = res.data.property 
                })
            },
            showDeleteDialog(id){
                this.dialogDeleteVisible = true;
                this.selectId = id;
            },
            showEditDialog(id){
                this.hardwareValue = [];
                this.firmwareValue = [];
                this.paramsValue = [];
                this.attrValue = [];
                Core.Api.request(
                    {
                    url: '/phrase/selectDataById',
                    method: 'GET',
                    data:{
                        id:id,
                    }
                }).then(res=>{
                    this.hardwareValue = res.data.hardware;
                    var subcode = 0;
                    if(this.hardwareValue.length>0){
                        subcode = this.hardwareValue[0].subcode
                    }
                    this.firmwareValue = res.data.firmware;
                    this.paramsValue = res.data.parameter;
                    this.attrValue = res.data.property;
                    this.selectId = id;
                    this.dialogEditVisible = true;
                    Core.Api.request({
                        method: "get",
                        url: "/param/category/findSnByLikeName",
                        data:{
                            sn:'',
                            name:subcode == 0 ? '' : this.categoryTypeData[subcode-1].name
                        },
                    }).then((res) => {
                        this.hardwareList = res.data.list;
                    });
                    Core.Api.request({
                        method: "get",
                        url: "/param/body/findSNByRootIdPage",
                        data:{
                            rootId:2,
                            currentPage:1,
                            pageSize:999

                        },
                    }).then((res) => {
                        if(res){
                            this.firmwareList = res;
                        }
                    });
                    Core.Api.request({
                        method: "get",
                        url: "/param/body/findSNByRootIdPage",
                        data:{
                            rootId:3,
                            currentPage:1,
                            pageSize:999

                        },
                    }).then((res) => {
                        if(res){
                            this.paramsList = res;
                        }
                    });
                    Core.Api.request({
                        url: '/besst/project/findAllCategory',
                        method: 'GET',
                        data:{
                            "subcode":subcode
                        }
                    }).then(res=>{
                        this.attrList = res.data;
                        res.data.forEach((item)=>{
                            this.attrDetail[item.propertyName] = item.propertyValue;
                        })
                    })
                });
               

            },
            changeData(val,type){
                var code = 1;
                if(type == this.firmwareValue){
                    this.serialNumberList = this.firmwareList;
                    code = 2;
                }else if(type == this.paramsValue){
                    this.serialNumberList = this.paramsList;
                    code = 3
                }else {
                    this.serialNumberList = this.hardwareList;          
                }
                let repeat = true;
                type.forEach((value,index)=>{
                    if(value.serial_number == val.serial_number){
                        repeat = !repeat;
                    }
                })
                if(repeat){
                    this.$message({
                        type: 'warning',
                        message: 'contains the same serial_number'
                    })
                    val.serial_number = '';
                }
                if(!repeat){
                    this.serialNumberList.forEach((item,index)=>{
                        if(item.serial_number == val.serial_number){
                            Core.Api.request({
                                url: '/intermediate/bulk',
                                method: 'POST',
                                data:[{
                                        "id":val.id ? val.id : 0,   
                                        "code":code,
                                        "serial_number": val.serial_number,
                                        "parent_id": this.selectId,
                                        "category_id":item.category_id ? item.category_id : null
                                }]
                            }).then(res=>{
                                if(res && res.length>0){
                                    val.id =  res[0].id;
                                    // this.$message({
                                    //     type: 'success',
                                    //     message: 'save successfully！！！！'
                                    // })
                                }
                            })
                        }
                    });
                }
            },
            getAttrDetail(val,type){
                let repeat = true;
                type.forEach((value,index)=>{
                    if(value.property_name == val.property_name){
                        repeat = !repeat;
                    }
                })
                if(repeat){
                    this.$message({
                        type: 'warning',
                        message: 'contains the same serial_number'
                    })
                    val.property_name = '';
                }
            },
            addData(type){
                var hasObj = false;
                if(type != this.attrValue){
                    type.forEach(function (val, index) {
                        if(Object.keys(val).length  === 1){
                            hasObj = true;
                        }
                    });
                    if(!hasObj){
                        type.push({serial_number: ''});
                    }   
                }else{
                    type.forEach(function (val, index) {
                        if(Object.keys(val).length  === 1){
                            hasObj = true;
                        }
                    });
                    if(!hasObj){          
                        type.push({property_list:[]});
                    }
                }        
            },
            saveEditData(){
                if(JSON.stringify(this.attrs) != JSON.stringify(this.attrValue)){
                    var paramList = [];
                    var attrStr = '';
                    this.attrList.forEach((item,index)=>{
                        this.attrValue.forEach((selectedItem)=>{
                            if(item.propertyName == selectedItem.property_name){
                                var attrStr = selectedItem.property_list.join(",");
                                var paramItem = {
                                    "id":selectedItem.id ? selectedItem.id : 0,
                                    "code": 4,
                                    "serial_number": '',
                                    "parent_id": this.selectId,
                                    "property_id":item.id,
                                    "property_name":selectedItem.property_name,        
                                    "property_list":attrStr
                                }
                                paramList.push(paramItem);  
                            }
                        })
                    });                   
                    if(paramList.length>0){
                        Core.Api.request({
                            url: '/intermediate/bulk',
                            method: 'POST',
                            data:paramList
                        }).then(res=>{
                            this.updateData();
                        })
                    }
                }else{
                    
                    this.updateData();
                }
               
                

            },
            //刷新数据
            updateData(){
                if(this.detailId == this.selectId){
                    Core.Api.request({
                        url: '/phrase/selectDataById',
                        method: 'GET',
                        data:{
                            id:this.selectId,
                        }
                    }).then(res=>{
                        this.loading = false;
                        this.hardware = res.data.hardware;
                        this.firmware = res.data.firmware;
                        this.params = res.data.parameter;
                        this.attrs = res.data.property ;
                        this.dialogEditVisible = false;
                        this.$message({
                            type: 'success',
                            message: 'save successfully'
                        })
                    })
                }else{
                    this.dialogEditVisible = false;
                    this.$message({
                        type: 'success',
                        message: 'save successfully'
                    })
                }
            },
            //删除字典里的数据
            deleteData(item,type){
                Core.Api.request({
                    url: '/intermediate/deleteRedundancyData?id='+item.id,
                    method: 'POST',
                }).then(res=>{
                    type.forEach(function (val, index) {
                        if(val == item){
                            type.splice(index,1);
                        }
                    });
                }).catch(err=>{
                    console.log(err);
                })

            },
            //删除字典
            deleteDictionary(){
                Core.Api.request({
                    url: '/phrase/deletePhrase?id='+this.selectId,
                    method: 'POST',
                }).then(res=>{
                    this.dialogDeleteVisible = false;
                    if(res.code == 0){
                        this.$message({
                            message: this.$t('Delete Success'),
                            type: 'success'
                        });
                        this.getDicList();
                    }
                })
            },
            getHeadNameAndValue(sn){
                this.loading = true;
                Core.Api.request({
                    url: '/param/body/findHeadNameAndValueBySN',
                    method: 'GET',
                    data:{
                        sn:sn,
                    }
                }).then(res=>{
                    this.loading = false;
                    this.headData = res;

                })
            },
            firmwareDetail(id,sn){
                // this.getParam(id,sn);
                this.getHeadNameAndValue(sn);
            },
            addDictionaryList(){
                this.$router.push({name: 'createDictionary'});
            }   
        },
        mounted(){
            let that = this;
            that.getDicList();
            document.onkeydown = function(e) {
            var key = window.event.keyCode;
                if (key == 13) {
                that.getDicList();
                }
            }
        }
   }
</script>
<style lang="scss" rel="stylesheet/scss">
    .dictionary-page{
        width: 100%;
        height: 100%;
        overflow: auto;
        display:flex;
        flex-direction:column;
        .header{
            height: auto;
            .dictionary-base-info{
                border-bottom: 1px solid #2B2F3B;
                background: #383d49;
                .el-form-item{
                     margin-bottom: 0;
                    .el-radio__label{
                        color: #fff;
                    }
                    .el-input{
                        width:200px;
                    }
                }
            }
            .flex{
                margin-top:15px;
                display: flex;
                justify-content: space-between;
                line-height:40px;
                height:440px;
                .title{
                       padding-left:28px;
                       height:40px;
                       line-height: 40px;
                       font-weight: 600;
                       background: #2B2F3B;
                       margin:0 !important;
                       border-bottom: 1px solid #393D49;
                }
                .dic-content{
                    overflow: auto;
                    height: 360px;
                    .info{                                 
                        padding-left:28px;
                        cursor: pointer;
                        span.icon{
                            float: right; margin-right: 20px;
                            i{
                                margin-right:10px;
                            }
                        }
                        &:hover{
                            background: #2B2F3B ;
                        }&.active{
                            background: #2B2F3B ;
                        }
                    }
                    .normal{
                        cursor: default !important;
                    }
                    .mr-10{
                        margin-right: 10px;
                    }
                }
               .width-33{
                   width:33%;
                   border: 1px solid #2B2F3B;
                   height:427px;                   
               }
               .width-66{
                   width:66%;
                   display:flex;
                   flex-direction:row;
                   flex-wrap:wrap;
                   justify-content:space-between;
                   .width-50{
                       width:49.2%;
                       border: 1px solid #2B2F3B;
                       height:206px;
                       .dic-content{
                            overflow: auto;
                            height: 160px;
                        }
                   }
               }  
            }
            .table-content-holder{
                margin:0 auto;
                width:90%;
                overflow: auto;
                border:1px solid #2B2F3B;
                text-align: center;
                .table-content{
                    width:100%;
                    thead{
                        th{
                            min-width:150px;
                            max-width:200px;
                            text-overflow: ellipsis;
                            overflow: hidden;
                            font-size: 14px;
                            color: #A1A5B1;
                            border-bottom: 1px solid #2B2F3B;
                            line-height: 20px;
                            padding:10px 6px;
                        }
                    }
                }
                .no-data-content{
                    background-color: #393D49;
                    color: rgb(130, 106, 94);
                    min-height: 60px;
                    text-align: center
                }
            }
        }
        .el-radio__label{
            color:#fff;
        }
        .editDialog{
            width:810px;
            color:#fff;
            .el-dialog__body {
                padding: 26px 28px;
            }
            .editDiv{
                width: 49%;
                display: inline-block;
                color:#fff;
                .params{
                    .title{
                        float:left;
                        line-height:37px;
                        height:37px;
                        font-weight: bold;
                    }
                    .input{
                        float:left;
                        width:324px;
                        padding-left:8px;   
                        font-size:0;
                        .selectList{
                            margin-top:5px;
                            font-size:0;
                            .checkBoxGroup{
                                padding:10px 9px 4px;
                                font-size:13px;
                                .el-checkbox{
                                    margin-left:0;
                                    margin-right:15px;
                                }
                            }
                        }
                    }
                    .max{
                        max-height: 143px;
                        overflow: auto;
                    }
                    .add-btn{
                        width:250px;
                        height:30px;
                        margin-left:50px;          
                        margin-bottom: 13px;
                        margin-top:-7px;
                    }
                    .noData{
                        margin-left: 8px;
                        transform: translateY(-5px);
                        margin-bottom: 7px;
                    }
                    .overflow{
                        margin-top:10px;
                    }


                }&.top{
                    vertical-align: top;
                }
                .el-select {
                    width:252px;
                    margin-right:10px;
                    height:30px !important;
                }
               
            }
            span.icon{
                display: inline-block;
                cursor: pointer;
            }
            
        }
    }
.el-select-dropdown__item {
    width:224px;
    // float: left;
    // height:0;
}
</style>


// WEBPACK FOOTER //
// detail.vue?1dfa8ede