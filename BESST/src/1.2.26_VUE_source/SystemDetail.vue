<template>
    <div class="system-detail">
        <div class="icons" @click="back"><div title="Back" class="icon-item back"></div></div>
       <el-form :inline="true" class="demo-form-inline">
            <el-form-item label="项目代号:" >
                <el-input v-model="projectNumberInput" placeholder="Project No." clearable></el-input>
            </el-form-item>
          <el-form-item label="项目类型:"  >
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
        <el-form-item label="审核状态:">未审核</el-form-item>
        <el-form-item label="车辆系统:" >
            <el-input v-model="vehicleSystem" placeholder="vehicle system" clearable></el-input>
        </el-form-item>
        <el-form-item label="车辆系统状态:" >
            未审核
            <!--<el-input v-model="vehicleSystemStatus" placeholder="Project No." clearable></el-input>-->
        </el-form-item>
            <el-form-item label="车辆系统代号:" >
            <el-input v-model="vehicleSystemCode" placeholder="vehicle system code." clearable></el-input>
        </el-form-item>
      </el-form>
      <el-collapse v-model="activeNames" @change="handleChange" accordion class="param-detail">
        <el-collapse-item :title="(item.name+'  '+item.SN)" :name="index" v-for="(item,index) in systemList" :key="index">
            <template slot="title">
                <img class="icon" :src="item.img" style="width: 20px;height: 20px;"/>
                <span>{{(item.name+'  '+item.SN)}}</span>
            </template>
            <div v-if="item.code>=1">
                 <h3 class="type-name" v-if="item.hardwareData">硬件</h3>
                <el-table :data="item.hardwareData" v-if="item.hardwareData">
                    <el-table-column   v-for="(item, index) in item.hardwareHeadData" :key="index" :prop="item.name"  :label="item.label" align="center">
                    </el-table-column>
                </el-table>
                <h3 class="type-name" v-if="item.firmwareData">固件</h3>
                <el-table :data="item.firmwareData" v-if="item.firmwareData">
                    <el-table-column   v-for="(item, index) in item.firmwareHeadData" :key="index" :prop="item.name"  :label="item.label" align="center">
                    </el-table-column>
                    <el-table-column   :render-header="renderHeader" v-if="item.name == '控制器'">
                        <template slot-scope="scope">
                            <span class="upload-firmware-file" @click="getUrl(firmwareBin[item.name])">{{firmwareBin.firmwareName}}</span>
                             <!--<div class="btn-group">
                                <a :href="firmwareBin[item.name] | getFileUrl('')" :download="firmwareBin[item.name]"
                                    class="btn-underline mr-15">
                                    {{ firmwareBin[item.name] }}</a>
                            </div>-->
                        </template>
                    </el-table-column>
                </el-table>
                <h3 class="type-name" v-if="item.propertyData">属性</h3>
                <el-table :data="item.propertyData"  v-if="item.propertyData">
                    <el-table-column   v-for="(item, index) in item.propertyHeadData" :key="index" :prop="item.name"  :label="item.label" align="center">
                    </el-table-column>
                </el-table>
            </div>
            <ul class="param-list" v-else>
                <li class="param-list-item" v-for="(childItem,childItemIndex) in item.paramList" :key="childItemIndex" :title="childItem.name" v-html="">
                    <span class="param-name">{{childItem.name.substring(0,6)}}</span>
                    <span>:{{childItem.value}}</span>
                </li>
            </ul>
        </el-collapse-item>
    </el-collapse>
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
                HMI: '',
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
                testPlan: '',
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
                paramCode: '',
                enclosureCode: '',
                activeNames: ['1'],
                defaultChecked: [],
                vehicleSystem: '',
                vehicleSystemCode: '',
                checkStatus: '0',
                collapseType: '',
                categoryItem: {},
                firmwareBin: {},
                // systemList: [
                //     {
                //         name: '电机',
                //         SN: 'MM G320/28*7T-001',
                //         hardwareData: [
                //             {
                //                 SN: 'MM G320-001',
                //                 PCBA: 'BBS01-XT9',
                //                 motorReductionRatio:'1:21.9',
                //                 speedSignalsNumber: '10K',
                //                 windingParameters: '7T',
                //                 thickPlateFold: '28',
                //                 plateDiameter: '98'
                //             }
                //         ],
                //         hardwareHeadData: [
                //             {
                //                 name: 'SN',
                //                 label: '序列号'
                //             },
                //             {
                //                 name: 'PCBA',
                //                 label: 'PCBA代号'
                //             },
                //             {
                //                 name: 'motorReductionRatio',
                //                 label: '电机减速比'
                //             },
                //             {
                //                 name: 'speedSignalsNumber',
                //                 label: '测速信号数'
                //             },
                //             {
                //                 name: 'windingParameters',
                //                 label: '绕组参数'
                //             },
                //             {
                //                 name: 'thickPlateFold',
                //                 label: '冲片叠厚'
                //             },
                //             {
                //                 name: 'plateDiameter',
                //                 label: '冲片直径'
                //             }
                //         ],
                //         firmwareData: [
                //             {
                //                 SN: 'MM G320/28*7T-001',
                //                 schematicDiagramNumber:'LD-0001',
                //                 firmwareVersion: 'V1.0'
                //             }
                //         ],
                //         firmwareHeadData:[
                //             {
                //                 name: 'SN',
                //                 label: '序列号'
                //             },
                //             {
                //                 name: 'schematicDiagramNumber',
                //                 label: '原理图编号'
                //             },
                //             {
                //                 name: 'firmwareVersion',
                //                 label: '固件版本号'
                //             },
                //         ],
                //         parameterData: [
                //             {
                //                 SN: 'MM G320/28*7T-001',
                //                 parameterVersion:'X0001',
                //             }
                //         ],
                //         parameterHeadData: [
                //             {
                //                 name: 'SN',
                //                 label: '序列号'
                //             },
                //             {
                //                 name: 'parameterVersion',
                //                 label: '参数版本号'
                //             },
                //         ]
                //     }
                // ],
                systemList: []
            }
        },
        methods: {
            getUrl(val){
                window.location = val;
            },
            handleChange(val){
                this.collapseType = val;
            },
            back() {
                this.$router.push(
                    {
                        name: 'project'
                    }
                );
            },
            getProjectType() {
                if (this.projectType == '1') {
                    this.$router.push({
                        path: '/component/createParam/index'
                    });
                }
            },
            showSelectDialog(){
                this.dialogCreateVisible=true;
            },
            renderHeader(h, data){
                return (
                    <span>
                        <span>固件bin文件下载</span>
                    </span>
                )

            },
            handleSystemInfo(type,data,systemItem,typeList,typeHeaderList,nextItem){
                if(type == 'property'){
                    data.forEach((childItem)=>{
                        let propertyHeadItem = {};
                        nextItem[childItem.property_name] = childItem.property_value;
                        propertyHeadItem.name = childItem.property_name,
                        propertyHeadItem.label=childItem.property_name
                        typeHeaderList.push(propertyHeadItem);
                    })
                    typeList.push(nextItem);
                    return systemItem;
                }else{
                    data.forEach((childItem)=>{
                        let hardeareHeadItem = {};
                        nextItem[childItem.head_name] = childItem.value;
                        hardeareHeadItem.name = childItem.head_name;
                        hardeareHeadItem.label = childItem.head_name
                        typeHeaderList.push(hardeareHeadItem);
                    })
                    typeList.push(nextItem);
                    return systemItem;
                }

            },

            getAllData(){
                Core.Api.request({
                    url: '/besst/project/findSystemDetail',
                    method: 'GET',
                    data: {
                        id: this.$route.query.id
                    }
                }).then(res=>{
                    if(res.code ==0 ){
                        console.log(res);
                        this.projectNumberInput = res.data[0].code;
                        this.vehicleSystem = res.data[0].name;
                        res.data.forEach((item)=>{
                            if(item.code === '1'){
                                let systemItem = {
                                    name: '电机',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-motor.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                console.log(systemItem);
                                this.systemList.push(systemItem);
                            }else if(item.code == '2'){
                                let systemItem = {
                                    name: '控制器',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-controller.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '0'){
                                 let systemItem = {
                                    name: '参数',
                                    code: item.code,
                                    SN: item.phraseName,
                                    paramList: [],
                                    img: require('../../../assets/images/icon/icon-motor.png'),
                                }
                                if(item.paramBodyList.param&&item.paramBodyList.param.length>0){
                                    item.paramBodyList.param.sort((a,b)=>{
                                        return a.sub_code-b.sub_code;
                                    })
                                    item.paramBodyList.param.forEach((childItem)=>{
                                        let paramNextItem = {};
                                        let name = childItem.head_name;
                                        if(name.length<6){
                                            let spacing='';
                                            for(var i = 0;i<(6-name.length);i++){
                                                spacing+='　'
                                            }
                                            paramNextItem.name = spacing+childItem.head_name;
                                        }else{
                                            paramNextItem.name = childItem.head_name;
                                        }
                                        paramNextItem.value = childItem.value;
                                        systemItem.paramList.push(paramNextItem);
                                    })
                                    this.systemList.push(systemItem);
                                }

                            }else if(item.code === '3'){
                                let systemItem = {
                                    name: '仪表',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-hmi.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '4'){
                                let systemItem = {
                                    name: '传感器',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-sensor.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '5'){
                                let systemItem = {
                                    name: '电池',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-battery.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '6'){
                                let systemItem = {
                                    name: '充电器',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-motor.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                   if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '7'){
                                let systemItem = {
                                    name: '工具',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-motor.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '8'){
                                let systemItem = {
                                    name: '蓝牙',
                                    code: item.code,
                                    SN: item.phraseName,
                                    hardwareData: [],
                                    firmwareData: [],
                                    propertyData: [],
                                    hardwareHeadData: [],
                                    firmwareHeadData: [],
                                    propertyHeadData: [],
                                    img: require('../../../assets/images/icon/icon-motor.png'),
                                }
                                let hardwareNextItem = hardwareNextItem?hardwareNextItem:{};
                                let firmwareNextItem = firmwareNextItem?firmwareNextItem:{};
                                let propertyNextItem = propertyNextItem?propertyNextItem:{};
                                for(var i in item.paramBodyList){
                                    if(i == 'hardware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.hardwareData,systemItem.hardwareHeadData,hardwareNextItem)
                                        }

                                    }else if(i == 'firmware'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.firmwareData,systemItem.firmwareHeadData,firmwareNextItem)
                                        }
                                    }else if(i == 'property'){
                                        if(item.paramBodyList[i]&&item.paramBodyList[i].length>0){
                                            this.handleSystemInfo(i,item.paramBodyList[i],systemItem,systemItem.propertyData,systemItem.propertyHeadData,propertyNextItem)
                                        }
                                    }
                                }
                                this.systemList.push(systemItem);
                            }else if(item.code === '100'){
                                this.systemList.forEach((systemItem)=>{
                                    if(systemItem.code == 2){
                                        if(item.paramBodyList.bin&&item.paramBodyList.bin.length>0){
                                            this.firmwareBin[systemItem.name] = item.paramBodyList.bin[0].firmware_file?item.paramBodyList.bin[0].firmware_file:'';
                                            this.firmwareBin.firmwareName = item.paramBodyList.bin[0].name;
                                        }
                                    }
                                })
                            }
                        })
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },
        },

        mounted() {
            this.getAllData();
        },
    }
</script>
<style lang="scss" rel="stylesheet/scss" scoped>
    .system-detail{
        .demo-form-inline .el-form-item__label {
            float: left;
        }

        .el-form--inline .el-form-item {
            width: 30%;
            margin-left: 20px;
        }

        .el-form {
            padding-top: 20px;
        }

        .project-btn {
            display: flex;
            justify-content: flex-end;
        }

        .save-project {
            margin-right: 30%;
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
        .type-name{
            text-indent: 40px;
        }
        .param-list{
            display: flex;
            flex-wrap: wrap;
            height: 400px;
            overflow: auto;
            .param-list-item{
                flex: 0 0 30%;
                margin-left:3.3%;
                margin-bottom: 10px;
                .param-name{
                    text-align:right;
                }
            }
        }
        .upload-firmware-file{
            cursor:pointer;
        }
        .upload-firmware-file:hover{
            color: red;
        }
        .param-detail{
            .el-collapse-item{
                .el-collapse-item__header{
                    display: flex;
                    align-items: center;
                    img{
                        margin-right: 20px;
                    }
                }
            }
        }
    }

</style>



// WEBPACK FOOTER //
// SystemDetail.vue?5d84f9bc