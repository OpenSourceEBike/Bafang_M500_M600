<template>
    <div class="create-project">
        <div class="icons" @click="back"><div title="Back" class="icon-item back"></div></div>
       <el-form :inline="true" class="demo-form-inline">
         <!-- <el-form-item label="客户名称:" >
              <el-input v-model="customerNameInput" placeholder="Customer Name" clearable></el-input>
          </el-form-item> -->
          <!-- <el-form-item label="客户代号:" >
              <el-input v-model="customerNumberInput" placeholder="Customer No." clearable></el-input>
          </el-form-item> -->
          <!-- <el-form-item label="项目名称:" >
              <el-input v-model="projectNameInput" placeholder="Project Name" clearable></el-input>
          </el-form-item> -->
            <el-form-item label="项目代号:" >
                <el-input v-model="projectNumberInput" placeholder="Project No." clearable></el-input>
            </el-form-item>
          <!-- <el-form-item label="项目类型:"  >
            <el-select
                v-model="projectType"
                placeholder="请选择"
                @change="getProjectType"
                >
                <el-option
                v-for="(item,index) in options"
                :key="index"
                :label="item.name"
                :value="item.value">

                </el-option>
            </el-select>
          </el-form-item>
        <el-form-item label="审核状态:">未审核</el-form-item>-->
        <el-form-item label="车辆系统:" >
            <el-input v-model="vehicleSystem" placeholder="vehicle system" clearable></el-input>
        </el-form-item>
        <!--<el-form-item label="车辆系统状态:" >
            未审核
            <el-input v-model="vehicleSystemStatus" placeholder="Project No." clearable></el-input>
        </el-form-item>
        <el-form-item label="车辆系统代号:" >
            <el-input v-model="vehicleSystemCode" placeholder="vehicle system code." clearable></el-input>
        </el-form-item>-->
        <el-form-item>
            <div class="project-btn">
                <el-button class="save-project" @click="saveProject">{{ saveType }}</el-button>
            </div>
            <!--<el-button @click="goToTest">test</el-button>-->
        </el-form-item>
      </el-form>
      <el-collapse v-model="activeNames" @change="handleChange" accordion class="show-el-collapse" >
        <el-collapse-item :title="item.name" :name="item.name" v-for="(item,index) in elCollapseData" :key="index">
            <!--<p class="add-new-param">
                <el-button icon="plus" @click="addNewParam(item.type)" class="add-param-btn"></el-button>
            </p>-->
            <template slot="title">
                <img class="icon" :src="item.img" />
                <span>{{item.name}}</span>
            </template>
            <el-form>
                <el-form-item label="name:" v-if="item.type!='param'">
                <el-select
                    v-model="item.phraseName"
                    placeholder="请选择"
                    @change="getSelectPhraseName">
                    <el-option
                        v-for="(item,index) in selectablePhraseNameList"
                        :key="index"
                        :label="item.name"
                        :value="item.value">
                    </el-option>
                </el-select>
            </el-form-item>
                <el-form-item v-for="(paramItem,paramIndex) in item.paramList" :key="paramIndex" :label="paramItem.name+':'">
                    <el-input v-model="projectInfo[item.type+paramItem.type+'name']" @click.native="addData(paramItem,item)"></el-input>
                </el-form-item>
            </el-form>
        </el-collapse-item>
    </el-collapse>

    <el-dialog
        class="dialog"
        :title="'Add '+ phraseType + ' Data'"
        :visible.sync="addPhraseVisible"
        :custom-class="categoryDialog"
        size="large"
        top="10%">
        <el-select
            v-if="isCreate"
            v-model="selectedParam"
            placeholder="请选择"
            @change="getSelectedParam">
            <el-option
                v-for="(item,index) in selectableParamList"
                :key="index"
                :label="item.name"
                :value="item.value">
            </el-option>
        </el-select>
        <el-radio-group v-model="selectedPhraseName" v-else class="select-phrase" @change="changeSelectedParam">
            <el-radio :label="item.id" v-for="(item,index) in allPhraseData" :key="index">{{item.name}}</el-radio>
        </el-radio-group>
        <div slot="footer" class="dialog-footer">
            <el-button type="preview" @click="savePhraseData">confirm</el-button>
            <el-button type="primary" @click="addPhraseVisible = false">cancel</el-button>
        </div>
    </el-dialog>
    <el-dialog
        class="dialog add-param-dialog"
        :title="'add new param'"
        :visible.sync="addNewParamVisible"
        :custom-class="categoryDialog"
        size="large"
        top="10%">
        <el-form>
            <el-form-item label="name:">
                <el-input v-model="paramName"></el-input>
            </el-form-item>
            <el-form-item label="type:">
                <el-select
                    v-model="selectedType"
                    placeholder="请选择"
                    @change="getSelectedType">
                    <el-option
                        v-for="(item,index) in categoryTypeData"
                        :key="index"
                        :label="item.name"
                        :value="item.value">
                    </el-option>
                </el-select>
            </el-form-item>
            <el-form-item label="value:">
                <el-input v-model="paramValue"></el-input>
            </el-form-item>
            <el-form-item class="save-param">
                <el-button @click="saveParam">save</el-button>
            </el-form-item>
        </el-form>
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
                selectedType: '',
                isCreate: false,
                selectableParamList: [],
                selectedParam: '',
                paramParentType: '',
                addNewParamVisible: false,
                paramCode: '',
                paramName: '',
                selectedPhraseName: '',
                phraseType: '',
                addPhraseVisible: false,
                projectNumberInput: '',
                projectNameInput: '',
                customerNameInput: '',
                customerNumberInput: '',
                options: [{
                    value: '0',
                    name: '八方标准系统',
                }, {
                    value: '1',
                    name: '自定义系统'
                }],
                projectType: '',
                motor: '',
                controller: '',
                instrument: '',
                battery: '',
                sensor: '',
                charger: '',
                serviceTool: '',
                attachment: '',
                motorTreeData: [],
                saveType: '保存',
                dialogCreateVisible: false,
                orgList:[],
                brandList: [],
                param: '',
                enclosure: '',
                firmware: '',
                hardware: '',
                dialogHardwareVisible: false,
                 defaultProps: {
                    id: 'id',
                    children: 'child',
                    label: 'name',
                },
                hardwareCategoryList: [],
                categoryList: [],
                categoryDialog: 'select-category',
                paramDialog: 'select-param',
                categoryType: '',
                hardwareCode: '',
                firmwareCode: '',
                enclosureCode: '',
                activeNames: ['1'],
                defaultChecked: [],
                testPlan: '',
                vehicleSystem: '',
                vehicleSystemCode: '',
                checkStatus: '0',
                collapseType: '',
                categoryItem: {},
                MOTORSN: '',
                HMISN: '',
                CONTROLLERSN: '',
                SENSORSN: '',
                BATTERYSN: '',
                typeList: {
                    motor: this.motorSN,
                    controller: this.controllerSN,
                    instrument: this.instrumentSN,
                    battery: this.batterySN,
                    sensor: this.sensorSN,
                },
                elCollapseData: [
                    {
                        type: 'MOTOR',
                        name: '电机',
                        phraseName: '',
                        MOTORSN:'',
                        img: require('../../../assets/images/icon/icon-motor.png'),
                        paramList: [
                            {
                                name: '硬件',
                                value: '',
                                type:'hardware',
                                MOTORhardwarename:'',
                                MOTORhardwareid:'',
                            },
                            {
                                name: '固件',
                                value: '',
                                type:'firmware',
                                MOTORfirmwarename:'',
                                MOTORfirmwareid:'',
                            },
                            // {
                            //     name: '参数',
                            //     value: '',
                            //     type:'param',
                            //     MOTORparamname:'',
                            //     MOTORparamid:'',
                            // },
                        ]
                    },
                    {
                        type: 'CONTROLLER',
                        name: '控制器',
                        phraseName: '',
                        CONTROLLERSN:'',
                        img: require('../../../assets/images/icon/icon-controller.png'),
                        paramList: [
                            {
                                name: '硬件',
                                value: '',
                                type:'hardware',
                                CONTROLLERhardwarename:'',
                                CONTROLLERhardwareid:'',
                            },
                            {
                                name: '固件',
                                value: '',
                                type:'firmware',
                                CONTROLLERfirmwarename:'',
                                CONTROLLERfirmwareid:'',
                            },
                            // {
                            //     name: '参数',
                            //     value: '',
                            //     type:'param',
                            //     CONTROLLERparamname:'',
                            //     CONTROLLERparamid:'',
                            // },
                        ]
                    },
                    {
                        type: 'SENSOR',
                        name: '传感器',
                        phraseName: '',
                        SENSORSN:'',
                        img: require('../../../assets/images/icon/icon-sensor.png'),
                        paramList: [
                            {
                                name: '硬件',
                                value: '',
                                type:'hardware',
                                SENSORhardwarename:'',
                                SENSORhardwareid:'',
                            },
                            {
                                name: '固件',
                                value: '',
                                type:'firmware',
                                SENSORfirmwarename:'',
                                SENSORfirmwareid:'',
                            },
                            // {
                            //     name: '参数',
                            //     value: '',
                            //     type:'param',
                            //     SENSORparamname:'',
                            //     SENSORparamid:'',
                            // },
                        ]
                    },
                    {
                        type: 'HMI',
                        name: '仪表',
                        phraseName: '',
                        img: require('../../../assets/images/icon/icon-hmi.png'),
                        HMISN:'',
                        paramList: [
                            {
                                name: '硬件',
                                value: '',
                                type:'hardware',
                                HMIhardwarename:'',
                                HMIhardwareid:'',
                            },
                            {
                                name: '固件',
                                value: '',
                                type:'firmware',
                                HMIfirmwarename:'',
                                HMIfirmwareid:'',
                            },
                            // {
                            //     name: '参数',
                            //     value: '',
                            //     type:'param',
                            //     HMIparamname:'',
                            //     HMIparamid:'',
                            // },
                        ]
                    },
                    {
                        type: 'BATTERY',
                        name: '电池',
                        phraseName: '',
                        BATTERYSN:'',
                        img: require('../../../assets/images/icon/icon-battery.png'),
                        paramList: [
                            {
                                name: '硬件',
                                value: '',
                                type:'hardware',
                                BATTERYhardwarename:'',
                                BATTERYhardwareid:'',
                            },
                            {
                                name: '固件',
                                value: '',
                                type:'firmware',
                                BATTERYfirmwarename:'',
                                BATTERYfirmwareid:'',
                            },
                            // {
                            //     name: '参数',
                            //     value: '',
                            //     type:'param',
                            //     BATTERYparamname:'',
                            //     BATTERYparamid:'',
                            // },
                        ]
                    },
                    {
                        type: 'param',
                        name: '参数',
                        img: require('../../../assets/images/icon/icon-motor.png'),
                        paramList: [
                            {
                                name: '属性',
                                value: '',
                                type:'property',
                            }
                        ]
                    }
                ],
                // elCollapseData:[],
                allPhraseData: [],
                selectedParamName: '',
                projectInfo:{},
                categoryTypeList:{
                    MOTOR: '电机',
                    CONTROLLER: '控制器',
                    HMI: '仪表',
                    SENSOR: '传感器',
                    BATTERY: '电池',
                    hardware: '硬件',
                    firmware: '固件',
                    param: '参数'
                },
                categoryTypeData: [
                    {
                        name: 'MOTOR',
                        value: 1,
                    },
                    {
                        name: 'CONTROLLER',
                        value: 2,
                    },
                    {
                        name: 'HMI',
                        value: 3

                    },

                    {
                        name: 'SENSOR',
                        value: 4,
                    },

                    {
                        name:'BATTERY',
                        value: '5',
                    },
                    {
                        name: 'CHARGER',
                        value: 6,
                    },
                    {
                        name: 'TOOL',
                        value: 7,
                    },
                    {
                        name: 'WIRELESS',
                        value: 8
                    },
                    {
                        name: 'param',
                        value: 0
                    }

                ],
                categoryTypeId:{
                    MOTOR: '1',
                    CONTROLLER: '2',
                    HMI: '3',
                    SENSOR: '4',
                    BATTERY: '5',
                    CHARGER: '6',
                    TOOL: '7',
                    WIRELESS: '8',
                    param: '0'
                },
                MOTORInfo: {},
                CONTROLLERInfo: {},
                HMIInfo: {},
                BATTERYInfo:{},
                SENSORInfo:{},
                paramInfo: {},
                newParam:[],
                paramValue: '',
                selectedParamList: [],
                selectCategoryId: '',
                phraseData:{},
                phraseList: [],
                selectCategoryName: '',
                selectablePhraseNameList: [],
                saveVos: [],
                selectedTypeId: '',
                firmwareCategoryId: ''
            }
        },
        methods: {
            getSelectPhraseName(val){
                let id;
                if(isNaN(val) &&this.collapseType==''){
                    if(!id){
                        return false;
                    }
                }else{
                    if(this.selectablePhraseNameList.length<=0){
                        return false;
                    }
                    this.selectablePhraseNameList.forEach((item)=>{
                        if(item.value == val){
                            let info = [this.collapseType+'Info'];
                            this[info]["name"] = item.name;
                        }
                    })
                    id = this.selectablePhraseNameList[val].id;
                    Core.Api.request({
                        method: 'GET',
                        url: '/besst/project/findSnByPhraseId?id='+id
                    }).then(res=>{
                        let that = this;
                        if(res.code == 0){
                                let code = that.categoryTypeId[that.collapseType];
                                let info = [that.collapseType+'Info'];
                                that[info]["property_id"] = '';
                                that.projectInfo[that.collapseType+'hardware'+'name'] = '';
                                that.projectInfo[that.collapseType+'firmware'+'name'] = '';
                                that.projectInfo[that.collapseType+'param'+'name'] = '';
                                that.phraseData.hardware = [];
                                that.elCollapseData.forEach((item)=>{
                                    if(item.type == that.collapseType){
                                        item.paramList.forEach((childItem,childIndex)=>{
                                            if(childItem.orgId == 1){
                                                item.paramList.splice(childIndex);
                                            }
                                        })
                                    }
                                })
                                that.phraseData.firmware = [];
                                that[info]['hardware_sn'] = '';
                                that[info]['firmware_sn'] = '';
                                that[info]['firmware_category_id'] = '';
                                if(res.data.hardware.length>0){
                                    res.data.hardware.forEach((item)=>{
                                        let firmwareItem = {
                                            id: item.id,
                                            name: item.sn,
                                            sn: item.phraseName
                                        }
                                        that.phraseData.hardware.push(firmwareItem);
                                    })
                                    that.phraseData.type = that.collapseType;
                                }
                                if(res.data.firmware&&res.data.firmware.length>0){
                                    res.data.firmware.forEach((item)=>{
                                        let firmwareItem = {
                                            id: item.id,
                                            name: item.sn,
                                            sn: item.sn,
                                            categoryId: item.categoryId
                                        }
                                        that.phraseData.firmware.push(firmwareItem);
                                    })
                                }
                                if(res.data.property&&res.data.property.length>0){
                                    res.data.property.forEach((propertyItem,propertyIndex)=>{
                                        that.elCollapseData.forEach((item)=>{
                                            if(item.type == that.collapseType){
                                                this.saveVos.forEach((saveItem,saveIndex)=>{
                                                    if(saveItem.code == code && saveItem.pripertyName == propertyItem.propertyName){
                                                        console.log(saveIndex);
                                                        this.saveVos.splice(saveIndex,1);
                                                    }
                                                })
                                                let info = [item.type+'Info'];
                                                // that[info]["property_id"] = that[info]["property_id"]&&that[info]["property_id"] !=''?that[info]["property_id"]:'';
                                                that.$set(that.projectInfo,item.type+'SN','');
                                                // item[item.type+'SN'] = propertyItem.typeValue;
                                                // item.id = propertyItem.id;
                                                var nextItem = {};
                                                var nextType = propertyItem.propertyName;
                                                nextItem.name = propertyItem.propertyName;
                                                nextItem[nextType+'SN'] = propertyItem.value;
                                                nextItem.type = nextType;
                                                nextItem.selectableParamList = nextItem.selectableParamList&&nextItem.selectableParamList.length>0?nextItem.selectableParamList:[];
                                                propertyItem.propertyList.forEach((propertyChildItem,propertyChildIndex)=>{
                                                    var selectedItem = {
                                                        name: propertyChildItem,
                                                        value: propertyChildIndex,
                                                        id: propertyItem.id
                                                    }
                                                    nextItem.selectableParamList.push(selectedItem);
                                                })
                                                nextItem.orgId = 1;
                                                that.$set(that.projectInfo,item.type+''+nextType+'name','');
                                                item.paramList.push(nextItem);

                                            }
                                        })
                                    })
                                }else{
                                    that.elCollapseData.forEach((item)=>{
                                        if(item.type == that.collapseType){
                                            item.paramList.forEach((childItem,childIndex)=>{
                                                if(childItem.orgId == 1){
                                                    item.paramList.splice(childIndex);
                                                }
                                            })
                                        }
                                    })
                                }
                            }
                    }).catch(err=>{
                        console.log(err);
                    })
                }

            },
            goToTest(){
                this.$router.push({
                    path: 'testPage'
                })
            },
            getSelectedType(val){
                console.log(val);
            },
            changeSelectedParam(val){
                if(val !=''){
                    this.allPhraseData.forEach((item)=>{
                        if(item.id == val){
                            this.firmwareCategoryId = item.categoryId;
                        }
                    })
                }
                if(this.phraseType == 'param'){
                    let info = [this.phraseType+'Info'],
                        phraseName = '';
                    this.allPhraseData.forEach((item)=>{
                        if(item.id == val){
                            phraseName = item.name;
                        }
                    })
                    this[info]['param_sn'] = phraseName;
                    this.$set(this[info],'param_sn',phraseName);
                    this.$set(this[info],'name',phraseName);
                    this.$set(this.projectInfo,'paramparamname',phraseName);
                }
            },
            getSelectedParam(value){
                if(value !== ''){
                    this.selectableParamList.forEach((item)=>{
                        if(item.value == value){
                            this.selectCategoryId = item.id;
                            this.selectCategoryName = item.name;
                            this.selectedTypeId = item.id;
                        }
                    })

                }
            },
            saveParam(){
                let data = {
                    "code": this.selectedType,
                    "name": this.paramName,
                    // "type": this.paramCode,
                    "value": this.paramValue
                }
                Core.Api.request({
                    url: '/besst/project/besstSaveType',
                    method: 'POST',
                    data: data
                }).then(res=>{
                    this.addNewParamVisible = false;
                    if(res.code == 0){
                        this.$message({
                            type: 'success',
                            message: 'add successfully!'
                        });
                        //新创建的属性统一在一个数组里，在全部保存时放到对应category的caetgory_id里
                        let paramType ='';
                        this.categoryTypeData.forEach((item)=>{
                            if(item.value == this.selectedType){
                                paramType = item.name;
                            }
                        })
                        if(this.newParam.length>0){
                            this.newParam.forEach((item)=>{
                                if(item.type == paramType){
                                    item.code.push(res.data);
                                }else{
                                    var nextItem = {
                                        type: paramType,
                                        code:[]
                                    }
                                    nextItem.code.push(res.data);
                                    this.newParam.push(nextItem);
                                }
                            })
                        }else{
                            var item = {
                                type: paramType,
                                code: []
                            }
                            item.code.push(res.data);
                            this.newParam.push(item);
                        }
                        //展示新创建的属性，并把新增的值放到select选择框里
                        this.elCollapseData.forEach((item)=>{
                            if(item.type == paramType){
                                let paramItem = {
                                    name: this.paramName,
                                    type: this.paramName,
                                    value: '',
                                    id: res.data,
                                    orgId: 1,
                                    selectableParamList: []
                                }
                                let selectableParamList =  this.paramValue.indexOf(',')>-1?this.paramValue.split(','):this.paramValue.split('，');
                                selectableParamList.forEach((item,index)=>{
                                    let selectableItem = {
                                        name: item,
                                        value: index
                                    }
                                    paramItem.selectableParamList.push(selectableItem);
                                })
                                let info = this.collapseType+'Info';
                                this[info]["property_id"] = '';
                                this.newParam.forEach((item)=>{
                                    if(item.type == paramType){
                                        if(!this[info]["property_id"]){
                                            this[info]["property_id"] = item.code.join(',')
                                        }
                                    }
                                })
                                paramItem.categoryId = res.data;
                                paramItem[this.paramName+'SN'] = '';
                                item.paramList.push(paramItem);
                                this.$set(this.projectInfo,item.type+''+paramItem.type+'name','');
                            }
                        })
                        this.paramName = '';
                        this.paramValue = '';
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },
            addNewParam(type){
                this.selectableParamList = [];
                this.selectedParam = '';
                this.paramParentType = type;
                this.addNewParamVisible = true;
            },
            savePhraseData(){
                let type = this.phraseType+this.selectedParamName,
                phraseName = '',
                phraseSN = '',
                that = this;
                if(that.isCreate){
                     that.selectableParamList.forEach((item)=>{
                        if(item.value == that.selectedParam){
                            phraseName = item.name;
                        }
                    })
                    that.$set(that.projectInfo,type+'id', that.selectedParam);
                    that.$set(that.projectInfo,type+'name',phraseName);
                }else{
                    if(that.phraseType == that.phraseData.type){
                        if(that.selectedParamName == 'hardware'){
                            that.allPhraseData.forEach((item)=>{
                                 if(item.id == that.selectedPhraseName){
                                    that.$set(that.projectInfo,type+'id',item.id);
                                }
                            })
                            if(that.phraseData[that.selectedParamName]&&that.phraseData[that.selectedParamName].length>0){
                                that.phraseData[that.selectedParamName].forEach((item)=>{
                                    phraseName = item.name;
                                    phraseSN = item.sn;
                                    that.$set(that.projectInfo,type+'name',phraseName);
                                })
                            }
                        }else{
                            if(that.phraseData[that.selectedParamName]&&that.phraseData[that.selectedParamName].length>0){
                                that.phraseData[that.selectedParamName].forEach((item)=>{
                                    if(item.id == that.selectedPhraseName){
                                        phraseName = item.sn?item.sn:item.name;
                                        that.$set(that.projectInfo,type+'id',item.id);
                                        that.$set(that.projectInfo,type+'name',phraseName);
                                    }
                                })
                            }

                        }
                    }else{
                        if(that.phraseType !='param'){
                            that.allPhraseData = [];
                        }else{
                            that.allPhraseData.forEach((item)=>{
                                if(item.id == that.selectedPhraseName){
                                    phraseName = item.sn?item.sn:item.name;
                                    that.$set(that.projectInfo,type+'id',item.id);
                                    that.$set(that.projectInfo,type+'name',phraseName);
                                }
                            })
                        }
                    }
                }
                let info = that.phraseType+'Info';
                let childrenType = that.selectedParamName+'_sn';
                if(that.selectedParamName && phraseName !=''){
                    that[info][childrenType] = phraseName;
                }
                if(that.selectedParamName=='hardware'){
                    that[info]['firmware_sn'] = '';
                    that[info]['param_sn'] = '';
                    that[info]['property_value'] = '';
                    that[info]['property_id'] = '';
                }else if(that.selectedParamName=='firmware' && this.firmwareCategoryId !=''){
                    that[info]['firmware_category_id'] = this.firmwareCategoryId;
                }
                if(that.isCreate){
                    if(that['saveVos'].length<=0 && that.phraseType !='param'){
                        var item = {
                            "code": that.categoryTypeId[that.phraseType],
                            "pripertyId": this.selectedTypeId,
                            "pripertyName": this.selectedParamName,
                            "pripertyValue": this.selectCategoryName
                        }
                        that['saveVos'].push(item)
                    }else{
                        let haveSame = false;
                        that['saveVos'].forEach((item)=>{
                            if(item.pripertyName ==this.selectedParamName && item.code == that.categoryTypeId[that.phraseType]){
                                haveSame = true;
                                item.pripertyId = this.selectedTypeId;
                                item.pripertyValue = this.selectCategoryName;
                            }
                        })
                        if(!haveSame){
                                var item = {
                                "code": that.categoryTypeId[that.phraseType],
                                "pripertyId": this.selectedTypeId,
                                "pripertyName": this.selectedParamName,
                                "pripertyValue": this.selectCategoryName
                            }
                            that['saveVos'].push(item)
                        }
                        haveSame = false;
                    }
                }
                that[info]["code"] = that.categoryTypeId[that.phraseType];

                if(that.phraseType =='param'){
                    that[info][that.phraseType+'_sn'] = phraseName;
                    that[info]['name'] = phraseName;
                }else{
                    if(that.selectedParamName =='hardware'){
                        that[info]["name"] = phraseSN;
                    }
                }
                that.addPhraseVisible = false;
                that.selectedParam = '';
                if(phraseName !=''){
                    that.$set(that.projectInfo,[type+'name'],phraseName);
                }
                // that.projectInfo[type+'name'] = phraseName;
            },
            addData(param,item){
                this.phraseType = item.type;
                this.selectedParamName = param.type;
                if(param.orgId == 1){
                    this.selectableParamList = param.selectableParamList;
                    this.isCreate = true;
                    let type = item.type+param.type;
                    this.selectableParamList.forEach((item)=>{
                        if(item.name == this.projectInfo[type+'name']){
                            this.selectedParam = item.value;
                        }
                    })
                    // this.selectedParam = 0;
                }else{
                    this.isCreate = false;
                    if(this.phraseData.type == undefined){
                        if(this.selectedParamName == 'hardware'){
                            this.allPhraseData = this.phraseList;
                        }else if(this.selectedParamName == 'param'){
                            this.allPhraseData = [];
                        }else{
                             this.allPhraseData = [];
                        }
                    }
                    if(this.phraseType == this.phraseData.type){
                        if(this.selectedParamName == 'firmware'){
                            this.allPhraseData = this.phraseData.firmware;
                            let info = this.phraseType+'Info';
                            if(this[info]['firmware_sn']){
                                 this.allPhraseData.forEach((item)=>{
                                     if(item.name == this[info]['firmware_sn']){
                                         this.selectedPhraseName = item.id;
                                     }
                                 })
                             }
                        }else if(this.selectedParamName == 'param'){
                            this.allPhraseData = this.phraseData.param;
                        }else if(this.selectedParamName == 'hardware'){
                             this.allPhraseData = this.phraseData.hardware;
                             let info = this.phraseType+'Info';
                             if(this[info]['hardware_sn']){
                                 this.allPhraseData.forEach((item)=>{
                                     if(item.name == this[info]['hardware_sn']){
                                         this.selectedPhraseName = item.id;
                                     }
                                 })
                             }
                        }
                    }
                    if(this.phraseType == 'param'){
                        this.allPhraseData = [];
                        Core.Api.request({
                            url: '/besst/project/findPhraseBySubCode',
                            data: {
                                "subCode": this.categoryTypeId[this.phraseType]
                            },
                            method: 'GET'
                        }).then(res=>{
                            // console.log(res);
                            if(res.data&&res.data.length>0){
                                this.allPhraseData = res.data;
                            }
                        }).catch(err=>{
                            console.log(err);
                        })
                    }

                }
                // this.selectedPhraseName = '';
                this.addPhraseVisible = true;
            },
            handleChange(val){
                let that = this;
                if(val !=''){
                    for(var item in that.categoryTypeList){
                        if(that.categoryTypeList[item] == val){
                            that.collapseType = item =='param'?item:item.toUpperCase();
                        }
                    }
                    let phraseName = '';
                    that.elCollapseData.forEach((item)=>{
                        if(item.type == that.collapseType){
                            if(item.phraseName&&item.phraseName !=''){
                                phraseName = item.phraseName;
                            }
                        }
                    })
                    if(phraseName !=''){
                        if(this.collapseType!='param'){
                            that.selectablePhraseNameList = [];
                            Core.Api.request({
                                url: '/besst/project/findIntermediateByName',
                                method: 'GET',
                                data: {
                                    "name": phraseName
                                },
                            }).then(res=>{
                                if(res.code == 0 &&res.data){
                                    that.phraseData.type = that.collapseType;
                                    if(res.data.phrase&&res.data.phrase.length>0){
                                        res.data.phrase.forEach((item,index)=>{
                                            let phraseNameItem = {
                                                name: item.name,
                                                value: index,
                                                id: item.id
                                            };
                                            that.selectablePhraseNameList.push(phraseNameItem);
                                        })
                                    }
                                    that.phraseData.hardware = [];
                                    if(res.data.hardware&&res.data.hardware.length>0){
                                        res.data.hardware.forEach((item,index)=>{
                                            let hardwareItem = {
                                                name: item.serial_number,
                                                value: index,
                                                id: item.id
                                            }
                                            that.phraseData.hardware.push(hardwareItem)
                                        })
                                    }
                                    that.phraseData.firmware = [];
                                    if(res.data.firmware&&res.data.firmware.length>0){
                                        res.data.firmware.forEach((item,index)=>{
                                            let firmwareItem = {
                                                name: item.serial_number,
                                                value: index,
                                                id: item.id,
                                                categoryId: item.category_id
                                            }
                                            that.phraseData.firmware.push(firmwareItem)

                                        })
                                    }
                                    if(res.data.property&&res.data.property.length>0){
                                        that.elCollapseData.forEach((item)=>{
                                            if(item.type == that.collapseType){
                                                item.paramList.forEach((childItem,childIndex)=>{
                                                    if(childItem.orgId == 1){
                                                        item.paramList.splice(childIndex);
                                                    }
                                                })
                                            }
                                        })
                                        let haveSameProperty = false;
                                        res.data.property.forEach((propertyItem)=>{
                                            that.elCollapseData.forEach((item)=>{
                                                if(item.type == that.collapseType){
                                                    item.paramList.forEach((paramItem)=>{
                                                        if(paramItem.name == propertyItem.property_name){
                                                            haveSameProperty = true;
                                                            paramItem.selectableParamList = [];
                                                            propertyItem.property_list.split(',').forEach((nameItem,nameIndex)=>{
                                                                let nextItem = {
                                                                    name: nameItem,
                                                                    id: propertyItem.property_id,
                                                                    value: nameIndex
                                                                }
                                                                if(paramItem.selectableParamList.indexOf(nextItem)<=-1){
                                                                    paramItem.selectableParamList.push(nextItem);
                                                                }
                                                            })
                                                        }
                                                    })
                                                }
                                            })
                                        })
                                        if(!haveSameProperty){
                                             res.data.property.forEach((propertyItem)=>{
                                                that.elCollapseData.forEach((item)=>{
                                                    if(item.type == that.collapseType){
                                                        if(item.name == val){
                                                            var childItem = {
                                                                name: propertyItem.property_name,
                                                                orgId: 1,
                                                                id: propertyItem.property_id,
                                                                type: propertyItem.property_name
                                                            }
                                                            childItem.selectableParamList = [];
                                                            propertyItem.property_list.split(',').forEach((nameItem,nameIndex)=>{
                                                                let nextItem = {
                                                                    name: nameItem,
                                                                    id: propertyItem.property_id,
                                                                    value: nameIndex
                                                                }
                                                                if(childItem.selectableParamList.indexOf(nextItem)<=-1){
                                                                    childItem .selectableParamList.push(nextItem);
                                                                }
                                                            })
                                                            item.paramList.push(childItem)
                                                        }
                                                    }

                                                })
                                            })
                                        }
                                    }
                                }
                            }).catch(err=>{
                                console.log(err);
                            })
                        }else{
                            that.allPhraseData = [];
                             Core.Api.request({
                                method: 'GET',
                                data: {
                                    "subCode": that.categoryTypeId[that.collapseType]
                                },
                                url: '/besst/project/findPhraseBySubCode'
                            }).then(res=>{
                                let that = this;
                                if(res.data&&res.data.length>0){
                                    res.data.forEach((item,index)=>{
                                        let phraseNameItem = {
                                            name: item.name,
                                            value: index,
                                            id: item.id
                                        };
                                        that.allPhraseData.push(phraseNameItem);
                                    })
                                }
                            })
                        }

                    }else{
                        that.selectablePhraseNameList = [];
                        Core.Api.request({
                            method: 'GET',
                            data: {
                                "subCode": that.categoryTypeId[that.collapseType]
                            },
                            url: '/besst/project/findPhraseBySubCode'
                        }).then(res=>{
                            let that = this;
                            if(res.data&&res.data.length>0){
                                res.data.forEach((item,index)=>{
                                    let phraseNameItem = {
                                        name: item.name,
                                        value: index,
                                        id: item.id
                                    };
                                    if(that.collapseType == 'param'){
                                        that.allPhraseData.push(phraseNameItem);
                                    }else{
                                        that.selectablePhraseNameList.push(phraseNameItem);
                                    }
                                })
                            }
                        })
                    }
                }
            },
            back() {
                this.$router.go(-1);
            },
            deleteRow() {

            },
            editRow() {

            },
            searchProject(val) {

            },
            getProjectType() {
                // console.log(this.projectType);
                // if (this.projectType == '1') {
                //     this.$router.push({
                //         path: '/component/createParam/index'
                //     });
                // }
            },
            showSelectDialog(){
                this.dialogCreateVisible=true;
            },
            saveProject() {
                let data = {};
                data.listBesstDictionary = [];
                data.systemCode = this.projectNumberInput;
                data.systemName = this.vehicleSystem;
                data.saveVos = this.saveVos.filter((item)=>{
                    return item.pripertyId!=''&& item.pripertyValue!='';
                })
//                console.log(this.categoryTypeId)
                for(var i in this.categoryTypeId){
                    let info = this[i+'Info'];
                    if(info!=undefined&& info.hasOwnProperty('name')&&info['name']!=''){
                        data.listBesstDictionary.push(info);
                    }
                }
                if(this.$route.query.id){
                    data.systemId =  this.$route.query.id;
                }
                Core.Api.request({
                    url: '/besst/project/besstSave',
                    data: JSON.stringify(data),
                    method: 'POST'
                }).then(res=>{
                    if(res.code == 0){
                        this.$message({
                            type: 'success',
                            message: 'save successfully!'
                        });
                        this.projectNumberInput = '';
                        this.vehicleSystem = '';
                        for(var i in this.categoryTypeId){
                            let info = this[i+'Info'];
                            if(info!=undefined&&info.name&& info.name!=''){
                                this[i+'Info'] = {};
                            }
                        }
                        for(var i in this.projectInfo){
                            this.projectInfo[i] = '';
                        }
                        this.$router.push({
                            name: 'project'
                        })
                    }
                })
            },
            getAllCategoryData() {
                var userId = 24;
                var that = this;
                Core.Api.request({
                    url: '/param/category/find-all-category?user_id=' + userId,
                    method: 'GET',
                }).then((res) => {
                    for (var i of res.data.param_category_list) {
                        if (i.name == '硬件') {
                            for (var j of i.child) {
                                if (j.name == '电机') {
                                    that.motorTreeNode = j;
                                }
                            }
                        }
                    }
                    that.$message({
                        message: 'save success'
                    })
                }).catch((err)=>{
                    console.log(err);
                })
            },
            //将要编辑的项目数据逐一加载
            getProjectData() {
                this.projectNumberInput = this.$route.query.projectData ? this.$route.query.projectData.systemNumber : '';
                this.projectType = this.$route.query.projectData ? this.$route.query.projectData.name : '';
                this.motor = this.$route.query.projectData ? this.$route.query.projectData.motor : '';
                this.controller = this.$route.query.projectData ? this.$route.query.projectData.controller : '';
                this.instrument = this.$route.query.projectData ? this.$route.query.projectData.instrument : '';
                this.battery = this.$route.query.projectData ? this.$route.query.projectData.battery : '';
                this.sensor = this.$route.query.projectData ? this.$route.query.projectData.sensor : '';
                this.charger = this.$route.query.projectData ? this.$route.query.projectData.charger : '';
                this.serviceTool = this.$route.query.projectData ? this.$route.query.projectData.serviceTool : '';
            },
            selectHardware(){

            },
            selectFirmware(){

            },
            selectParam(){

            },
            selectTestPlan(){

            },
            selectEnclosure(){

            },
            addItems(){

            },
            saveCategoryInfo(){

            },
            saveSelectedCategory(){
                this.dialogHardwareVisible = false;
            },
            getAllPhraseData(){
                Core.Api.request({
                    url: '/phrase/filterAll',
                    method: 'POST'
                }).then(res=>{
                    this.phraseList = res;
                    this.allPhraseData  = this.phraseList;
                }).catch(err=>{
                    console.log(err);
                })
            },
            // getSystemInfo(data){
            //     if(data!={}){
            //         this.projectNumberInput = data.projectNumber;
            //         this.projectType = data.projectType;
            //         this.vehicleSystem = data.vehicleSystem;
            //         this.vehicleSystemCode = data.vehicleSystemCode;
            //         this.typeList.motor = data.motorName;
            //         this.typeList.battery = data.batteryName;
            //         this.typeList.controller = data.controllerName;
            //         this.typeList.instrument = data.instrumentName;
            //         this.typeList.sensor = data.sensorName;
            //     }
            // },
            getEditSystemInfo(){
                 if(this.$route.query!={}){
                    if(this.$route.query.id){
                        // this.elCollapseData = [];
                        let that = this;
                        Core.Api.request({
                            url: '/besst/project/besstDetailByID',
                            method: 'GET',
                            data:{
                                id: this.$route.query.id
                            }
                        }).then(res=>{
                            if(res.data!=null){
                                for(var i in res.data){
                                    if(i !== 'system'){
                                        let item = {};
                                        item.type = i;
                                        item.paramList = [];
                                        res.data[i].forEach((childItem,index)=>{
                                            if(childItem !=null){
                                                    item.phraseName = childItem.name;
                                                    item.name = that.categoryTypeList[i];
                                                    let info = i+'Info';
                                                    if(that[info]!=undefined){
                                                        let propertyId = childItem["property_id"]!=null? (childItem["property_id"].indexOf(',')>-1?childItem["property_id"].split(','):childItem["property_id"].split()):[],
                                                            typeName = childItem['type_name']!=null?(childItem['type_name'].indexOf(',')>-1?childItem['type_name'].split(','):childItem['type_name'].split()):[],
                                                            propertyValue = childItem["property_value"]!=null?(childItem["property_value"].indexOf(',')>-1?childItem["property_value"].split(','):childItem["property_value"].split()):[];
                                                        if(propertyId.length>0){
                                                             propertyId.forEach((propertyItem,propertyIndex)=>{
                                                                var item = {
                                                                    "code": childItem.code,
                                                                    "pripertyId": propertyItem,
                                                                    "pripertyName": typeName[propertyIndex],
                                                                    "pripertyValue": propertyValue[propertyIndex]
                                                                }
                                                                that['saveVos'].push(item)
                                                            })
                                                        }

                                                        // that[info]["property_id"] = childItem["property_id"];
                                                        // that[info]["property_value"] = childItem["property_value"];
                                                        that[info]["code"] = that[info]["code"]?that[info]["code"]:childItem.code;
                                                        that[info]["name"] = that[info]["name"]?that[info]["name"]:childItem.name;
                                                        that[info]["id"] = that[info]["id"]?that[info]["id"]:childItem.id;
                                                        that.$set(that.projectInfo,item.type+'SN',childItem.name);
                                                        item[i+'SN'] = childItem.name;
                                                        item.id = childItem.id;
                                                        that[info]['create_time'] = childItem.create_time;
                                                        that[info]['create_user_id'] = childItem.create_user_id;
                                                        if(childItem.categoryId&&childItem.categoryId!=null){
                                                            that[info]['firmware_category_id'] = childItem.categoryId;
                                                            this.firmwareCategoryId = childItem.categoryId;
                                                        }
                                                        for(var j in childItem){
                                                            if(j!=null&&j.indexOf('_sn')>-1){
                                                                var nextItem = {},
                                                                    nextType = j.substring(0,j.indexOf('_sn'));
                                                                    let childrenType = nextType+'_sn';
                                                                    that[info][childrenType] = that[info][childrenType]?that[info][childrenType]:childItem[j];
                                                                nextItem.name = that.categoryTypeList[nextType];
                                                                nextItem[nextType+'SN'] = childItem[j];
                                                                nextItem.type = nextType;
                                                                that.$set(that.projectInfo,item.type+''+nextType+'name',childItem[j]);
                                                                if(i=='param'){
                                                                    if(j.indexOf('hardware')<=-1 && j.indexOf('firmware')<=-1){
                                                                        nextItem.name = '属性';
                                                                        item.paramList.push(nextItem);
                                                                        item.img = require('../../../assets/images/icon/icon-motor.png');
                                                                    }
                                                                }else{
                                                                    if(j.indexOf('hardware')>-1 || j.indexOf('firmware')>-1){
                                                                        nextItem.orgId = 0;
                                                                        item.paramList.push(nextItem);
                                                                        item.img = require('../../../assets/images/icon/icon-'+item.type.toLowerCase()+'.png');
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        if(typeName.length>0){
                                                            typeName.forEach((propertyItem,propertyIndex)=>{
                                                                var nextItem = {};
                                                                nextItem.name = propertyItem;
                                                                nextItem.type = propertyItem;
                                                                nextItem.id = propertyId[propertyIndex]
                                                                nextItem[propertyItem+'SN'] = propertyValue[propertyIndex];
                                                                nextItem.orgId = 1;
                                                                nextItem.selectableParamList = [];
                                                                item.paramList.push(nextItem);
                                                                that.$set(that.projectInfo,item.type+''+propertyItem+'name',propertyValue[propertyIndex]);
                                                            })
                                                        }
                                                    }
                                                // }else if(childItem['org_id']&&childItem['org_id'] == 1){
                                                //     that.$set(that.projectInfo,item.type+'SN','');
                                                //     item[i+'SN'] = childItem.typeValue;
                                                //     item.id = childItem.id;
                                                //     let info = i+'Info';
                                                //     that.$set(that[info],childItem.name+'_sn',childItem.typeValue)
                                                //     var nextItem = {};
                                                //     var nextType = childItem.name;
                                                //     nextItem.name = childItem.name;
                                                //     nextItem[nextType+'SN'] = childItem.typeValue;
                                                //     nextItem.type = nextType;
                                                //     nextItem.selectableParamList = [];
                                                //     nextItem.orgId = 1;
                                                //     childItem.value.split(',').forEach((item,index)=>{
                                                //         var selectedItem = {
                                                //             name: item,
                                                //             value: index
                                                //         }
                                                //         nextItem.selectableParamList.push(selectedItem);
                                                //     })
                                                //     that.$set(that.projectInfo,item.type+''+nextType+'name',childItem.typeValue);
                                                //     item.paramList.push(nextItem);
                                                // }
                                            }

                                        })
                                        that.elCollapseData.forEach((elItem,elIndex)=>{
                                            if(elItem.type == i){
                                                that.elCollapseData.splice(elIndex,1,item);
                                            }

                                        })
                                        console.log(that.elCollapseData)
                                    }else{
                                        for(var y of res.data[i]){
                                            that.projectNumberInput = y.code;
                                            that.vehicleSystem = y.name;
                                        }
                                    }

                                }
                            }

                        }).catch(err=>{
                            console.log(err);
                        })
                    }
                }
            }
        },

        mounted() {
            // this.getAllPhraseData();
            this.getEditSystemInfo();
        },
    }
</script>
<style lang="scss" rel="stylesheet/scss">
.create-project{
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
    .select-category{
        width: 400px;
    }
    .select-category .el-tree .el-tree-node__expand-icon:before{
        content: '';
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
            // height: 240px;
            overflow: auto;
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
}

</style>



// WEBPACK FOOTER //
// createProject.vue?46edf220