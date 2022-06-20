<template>
    <div class="basic-bat-params">
        <div class="header">
            <el-form :inline="true" class="demo-form-inline">
                <!-- <el-form-item :label="$t('search.poNo')"> -->
                <el-form-item label="Po No.">
                    <el-input v-model="batSpecName"></el-input>
                    <el-input style="display: none"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="getSpecList" title="search"></div>
                        <div class="icon-item add" @click="addSpecRecord" title="add" v-if="authList[Core.Const.AUTH_LIST.productionAdd]"></div>
                    </div>
                </div>
            </el-form>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <td v-for="(item, index) in headData">{{item}}</td>
                    </thead>
                    <tbody v-if="!loading">
                    <tr v-for="item in batRecord">
                        <td class="td" >{{item.specifications}}</td>
                        <td class="td" >{{item.standardVoltage}}</td>
                        <td class="td" >{{item.standardCapacity}}</td>
                        <td class="td" >{{item.minimumCapacity}}</td>
                        <td class="td" >{{item.standardDischargeRate}}</td>
                        <td class="td" >{{item.maximumDischargeRate}}</td>
                        <td>
                            <a class="btn-underline" @click.stop="showSpecDetail(item)">detail</a>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="batRecord.length <= 0 && !loading">
                    noResult
                </div>
                <div class="loading" v-if="loading">
                    <img src="../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
                </div>
            </div>
        </div>

        <el-dialog title="Battery Specification"
                   :visible.sync="dialogEditVisible"
                   size="small">
            <div>
                <el-button @click="getAttrData">Get Attribute Data</el-button>
                <div style="margin: 15px 0;"></div>
            </div>
            <div>
                <template>
                    <span>环境温度:</span>
                    <el-checkbox :indeterminate="isIndeterminateTemp" v-model="checkAllTemp" @change="handleCheckTempChange">全选</el-checkbox>
                    <div style="margin: 15px 0;"></div>
                    <el-checkbox-group v-model="checkedTemp" @change="handleCheckedTempChange">
                        <el-checkbox v-for="temp in tempOptions" :label="temp" :key="temp">{{temp}}</el-checkbox>
                    </el-checkbox-group>
                </template>
                <template>
                    <span>倍率:</span>
                    <el-checkbox :indeterminate="isIndeterminateMagni" v-model="checkAllMagni" @change="handleCheckMagniChange">全选</el-checkbox>
                    <div style="margin: 15px 0;"></div>
                    <el-checkbox-group v-model="checkedMagni" @change="handleCheckedMagniChange">
                        <el-checkbox v-for="magni in magnificationOptions" :label="magni" :key="magni">{{magni}}</el-checkbox>
                    </el-checkbox-group>
                </template>
            </div>
            <div>
                <el-button @click="createTableData">Create Table Data</el-button>
                <div style="margin: 15px 0;"></div>
            </div>
            <div>
                <div class="table-wrap">
                    <div class="table-content-holder">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                            <td v-for="item in tempData">{{item}}</td>
                            </thead>
                            <tbody>
                            <tr v-for="item in tableData">
                                <td class="short-td" v-for="i in item['temp_Value']">
                                    <el-input v-model="i.value"></el-input>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
                <div>
                    <label>电池规格</label>
                    <el-input class="input" v-model="batSpec" ></el-input>
                    <div class="item">
                        <label>标称电压</label>
                        <el-input class="input" v-model="noVoltage" ></el-input>
                    </div>
                    <div class="item">
                        <label>标称容量</label>
                        <el-input class="input" v-model="noCapacity" ></el-input>
                    </div>
                    <div class="item">
                        <label>最低容量</label>
                        <el-input class="input" v-model="minCapacity" ></el-input>
                    </div>
                    <div class="item">
                        <label>标准放电倍率</label>
                        <el-input class="input" v-model="standMagnification" ></el-input>
                    </div>
                    <div class="item">
                        <label>最大放电倍率</label>
                        <el-input class="input" v-model="maxMagnification" ></el-input>
                    </div>
                </div>
            </div>
            <div>
                <el-button @click="saveBatSpecification">Save Specification</el-button>
                <div style="margin: 15px 0;"></div>
            </div>
        </el-dialog>
    </div>
</template>
<script>
    import Core from 'core/core'
    export default {
        data(){
            return{
                Core: Core,
                headData:["电池规格","标称电压","标称容量","最低容量","标准放电倍率","最大放电倍率","操作"],
                batRecord: '',
                currentPage: Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) ? Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) : 1,

                total: 0,
                loading: false,
                batSpecName: "",
                authList: [],

                userId: Core.Data.getUser().id,


                checkAllTemp: false,
                checkedTemp: [],
                isIndeterminateTemp: true,
                tempOptions: ['40', '25', '15', '0', "-10"],
                checkAllMagni: false,
                checkedMagni: [],
                isIndeterminateMagni: true,
                magnificationOptions: ['0.5C', '1C', '1.5C', '2C', '2.5C'],

                tempData: [],
                tableData: [],

                deleteParamVisible: false,

                batSpec: "",
                noVoltage: "",
                noCapacity: "",
                minCapacity: "",
                standMagnification: "",
                maxMagnification: "",
                isCreate: false,
                selectSpecItem: "",
                dialogEditVisible: false,

            }
        },
        computed: {
            getCurrentTime(){
                let that = this;
                setTimeout(function(){
                    console.log(that.Core.Data.getFormatterDate(Date.now()))
                    that.time =  that.Core.Data.getFormatterDate(Date.now())
                },1000)
            }
        },
        mounted(){
            this.authList = this.$store.state.userAuth;
            this.getSpecList();
        },
        methods: {
            showSpecDetail(item){
                this.selectSpecItem = item;
                this.dialogEditVisible = true;
                this.isCreate = false;

                this.batSpec = item.specifications;
                this.noVoltage = item.standardVoltage;
                this.noCapacity = item.standardCapacity;
                this.minCapacity = item.minimumCapacity;
                this.standMagnification = item.standardDischargeRate;
                this.maxMagnification = item.maximumDischargeRate;
                this.tempData = JSON.parse(item.ambientTemperatureProperties);
                this.tableData = JSON.parse(item.magnificationAttribute);

            },

            getSpecList(){
                Core.Api.request({
                    url: '/BatteryCategory/battery_specifications_detail_select',
                    method: 'GET',
                    data: {
                        specifications: this.batSpecName.trim(),
                    }
                }).then(res=>{
                    console.log(res);
                    this.batRecord = res.data.list;
                }).catch(err=>{
                    console.log(err);
                })
            },

            addSpecRecord(){
                this.dialogEditVisible = true;
                this.isCreate = true;
            },

            saveBatSpecification(){
                let createId = this.selectSpecItem.id;
                let createTime = this.selectSpecItem.createTime;
                let createUserId = this.selectSpecItem.createUserId;
                if(this.isCreate){
                    createId = 0;
                    createTime = 0;
                    createUserId = this.userId;
                }

                Core.Api.request({
                    url: '/BatteryCategory/battery_specifications_detail_save',
                    method: 'POST',
                    data: {
                        "ambientTemperatureProperties": JSON.stringify(this.tempData),
                        "attribute": "",
                        "createTime": createTime,
                        "createUserId": createUserId,
                        "id": createId,
                        "magnificationAttribute": JSON.stringify(this.tableData),
                        "maximumDischargeRate": this.maxMagnification,
                        "minimumCapacity": this.minCapacity,
                        "specifications": this.batSpec,
                        "standardCapacity": this.noCapacity,
                        "standardDischargeRate": this.standMagnification,
                        "standardVoltage":  this.noVoltage,
                        "updateTime": 0,
                        "updateUserId": this.userId,
                        "weight": 0
                    }
                }).then(res=>{
                    console.log(res);
                }).catch(err=>{
                    console.log(err);
                })
            },

            getAttrData(){
                Core.Api.request({
                    url: '/BatteryCategory/findAll',
                    method: 'GET',
                }).then(res=>{
                    let tempTem = [];
                    let magniTem = [];
                    res.data.list.forEach((item)=>{
                        if(item.type === 1){
                            tempTem.push(item.value);
                        }else if(item.type === 2){
                            magniTem.push(item.value);
                        }
                    })

                    this.tempOptions = tempTem.sort().reverse();
                    this.magnificationOptions = magniTem.sort();

                }).catch(err=>{
                    console.log(err);
                })
            },


            handleCheckTempChange(val){
                this.checkedTemp = val ? this.tempOptions : [];
                this.isIndeterminateTemp = false;
            },
            handleCheckMagniChange(val){
                this.checkedMagni = val ? this.magnificationOptions : [];
                this.isIndeterminateMagni = false;
            },
            handleCheckedTempChange(value){
                let checkedCount = value.length;
                this.checkAllTemp = checkedCount === this.tempOptions.length;
                this.isIndeterminateTemp = checkedCount > 0 && checkedCount < this.tempOptions.length;
            },
            handleCheckedMagniChange(value){
                let checkedCount = value.length;
                this.checkAllMagni = checkedCount === this.magnificationOptions.length;
                this.isIndeterminateMagni = checkedCount > 0 && checkedCount < this.magnificationOptions.length;
            },
            createTableData(){
                if(this.checkedTemp.length === 0 || this.checkedTemp.length === 0){
                    this.$message({
                        message: "请先选择环境温度或者倍率",
                        type: 'warning',
                        duration: 3000
                    });
                    return;
                }

                this.tempData = this.checkedTemp;
                this.tableData = [];
                this.checkedMagni.forEach((i,index)=>{
                    this.tableData.push({"temp_Value":[],"magi":i})
                    this.checkedTemp.forEach((item)=>{
                        this.tableData[index]["temp_Value"].push({"value":"","temp":item})
                    })
                })
            },

        }
    }
</script>
<style lang="scss">
   .basic-bat-params{


   }
</style>



// WEBPACK FOOTER //
// basicParams.vue?5c7939ca