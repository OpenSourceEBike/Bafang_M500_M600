<template>
    <div class="vehicle-search-holder">
        <div class="header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item :label="$t('search.bikeId')">
                    <el-input v-model="sn"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="getVehicleDetail" :title="$t('search.searchBtn')"></div>
                        <div class="icon-item clear" @click="clear" :title="$t('search.clear')"></div>
                    </div>
                </div>
            </el-form>
        </div>

        <div class="content-wrap" v-if="vehicleId">
            <div class="content">
                <div class="content-holder">
                    <div class="vehicle-item">
                        <div class="content-item">
                            <label class="title">{{ $t('sn') }}</label>
                            <div class="desc">
                                <span>{{vehicle.sn}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('key') }}</label>
                            <div class="desc">
                                <span>{{vehicle.secret_key ? vehicle.secret_key : '-'}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('brand') }}</label>
                            <div class="desc">
                                <span>{{vehicle.brand.name}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('itemName') }}</label>
                            <div class="desc">
                                <span>{{vehicle.item.name}}</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('productTime') }}</label>
                            <div class="desc">
                                <span>{{vehicle.produce_time | timeFormat("Y-m-d")}}</span>
                            </div>
                        </div>

                        <div class="content-item">
                            <label class="title">{{ $t('saleTime') }}</label>
                            <div class="desc">
                                <span
                                    v-if="vehicle.sale_time && vehicle.sale_time != 0">{{vehicle.sale_time | dateFormat}}</span>
                                <span v-else>-</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('customerName') }}</label>
                            <div class="desc">
                                <span v-if="vehicle.customer && vehicle.customer.name">{{vehicle.customer.name}}</span>
                                <span v-else>-</span>
                            </div>
                        </div>
                        <div class="content-item">
                            <label class="title">{{ $t('customerEmail') }}</label>
                            <div class="desc">
                                <span
                                    v-if="vehicle.customer && vehicle.customer.email">{{vehicle.customer.email}}</span>
                                <span v-else>-</span>
                            </div>
                        </div>
                    </div>

                    <el-collapse accordion>
                        <el-collapse-item>
                            <template slot="title">
                                <label>{{ $t('part')}}</label>
                                <div class="btn-div">
                                    <a class="btn-underline" @click.stop="onClickHistory">{{ $t('history')}}</a>
                                </div>
                            </template>

                            <div class="bom-content-holder">
                                <div class="bom-item" v-for="bom in bomAttrList" v-if="bomAttrList.length > 0">
                                    <div class="bom-header">
                                        <label class="bom-title">
                                            {{bom.rootName}}
                                        </label>
                                        <el-button type="text" class="btn-replace"
                                                   @click="showUpdateDialog(bom.sn, bom.id)">
                                            {{ $t('replace') }}
                                        </el-button>
                                    </div>
                                    <div class="bom-content">
                                        <label class="title">SN</label>
                                        <div class="desc">
                                            <span>{{bom.sn || '-'}}</span>
                                        </div>
                                    </div>
                                    <div class="bom-content">
                                        <label class="title">Name</label>
                                        <div class="desc">
                                            <span>{{bom.name || '-'}}</span>
                                        </div>
                                    </div>
                                    <div class="bom-content" v-for="item in bom.attrList">
                                        <label class="title">
                                            {{item.attr_def_name}}
                                        </label>
                                        <div class="desc">
                                            <span>{{item.value || '-'}}</span>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </el-collapse-item>
                    </el-collapse>
                </div>
            </div>
        </div>

        <el-dialog :title="$t('replace')" :visible.sync="dialogReplaceVisible" class="dialog-password">
            <div class="item">
                <div class="item-box">
                    <label class="label">{{ $t('oldSn') }}</label>
                    <label class="input old-sn">{{selectSn}}</label>
                </div>
            </div>
            <div class="item new-password">
                <div class="item-box">
                    <label class="label">{{ $t('newSn') }}</label>
                    <el-input v-model="newSn" class="input scan-input"></el-input>
                </div>
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="updateSn">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogReplaceVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script type="text/ecmascript-6">
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        data() {
            return {
                org: Core.Data.getOrg() || "",
                sn: '',
                vehicleId: '',

                vehicle: {
                    sn: '',
                    brand: {},
                    item: {},
                    order: {sn: ''},
                },

                selectSn: '',
                selectId: '',
                dialogReplaceVisible: false,

                newSn: '',
                bomList: [],
                bomAttrList: [],
            }
        },
        i18n: {
            messages: {
                en: EN.Dealer.Vehicle.Search,
                zh: ZH.Dealer.Vehicle.Search,
                de: DE.Dealer.Vehicle.Search,
                nl: NL.Dealer.Vehicle.Search
            }
        },
        mounted() {
            this.getVehicleDetail();
        },

        watch: {
            newSn: function (newValue, oldValue) {
                let vm = this;
                if (newValue === oldValue || newValue === '') {
                    return;
                }
                if (typeof oldValue !== 'undefined') {
                    let time = new Date().getTime();
                    vm.isScanner = time - vm.lastInputTime <= 100; // 间隔小于100ms的就认为是扫描枪输入
                    vm.lastInputTime = time;
                } else {
                    vm.lastInputTime = new Date().getTime();
                }
                if (!vm.scannerTimeout) {
                    vm.scannerTimeout = setTimeout(function () {
                        if (vm.isScanner) {
                            let value = vm.newSn.replace(/\s+/g, "");
                            // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                            if (value.indexOf('DP') > -1) {
                                let array = value.split('DP');
                                if (array.length > 1) {
                                    value = 'DP' + array[array.length - 1];
                                }
                                vm.newSn = value;
                            }else if( value.indexOf('SR') > -1 ){
                                let array = value.split('SR');
                                if (array.length > 1) {
                                    value = 'SR' + array[array.length - 1];
                                }
                                vm.newSn = value;
                            }
                        }
                        clearTimeout(vm.scannerTimeout);
                        vm.scannerTimeout = undefined;
                    }, 1000)
                }
            }
        },

        methods: {
            getVehicleDetail() {
                if (!this.sn.trim()) {
                    return;
                }
                Core.Api.Vehicle.vehicleDetailBySn(this.sn.trim(), this.org.id ).then(res => {
                    this.vehicle = res.vehicle;
                    this.vehicleId = this.vehicle.id;
                    this.bomList = this.vehicle.bom_list;
                    this.getBomInfo(this.bomList);
                });
            },

            clear() {
                this.vehicleId = '';
                this.sn = '';
            },

//            getBom(sn) {
//                Core.Api.Vehicle.getVehicleBomBySn(sn).then(res => {
//                    this.bomList = res.bom;
//                    this.getBomInfo(res.bom);
//                });
//            },

            getBomInfo(bomList) {
                this.bomAttrList = [];
                let $this = this;
                bomList.forEach(function (bom, index) {
                    let item = {
                        attrList: [],
                    };

                    item.sn = bom.sn;
                    item.id = bom.id;
                    item.name = bom.item ? bom.item.name : '';
                    let attrList = bom.item ? bom.item.attr_list : [];

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.MOTOR) {
                        item.rootName = 'Motor';
                        if (attrList !== null && attrList.length > 0) {

                            for (let index in attrList) {
                                if (['power', 'voltage', 'wheel_size', 'rpm'].includes(attrList[index].attr_def_key)) {
                                    item.attrList.push(attrList[index]);
                                }
                            }
                        }
                        $this.bomAttrList.push(item);
                        return;
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.SENSOR) {
                        item.rootName = 'Sensor';

                        if (attrList !== null && attrList.length > 0) {
                            for (let index in attrList) {
                                if (['soft_ver', 'hard_ver'].includes(attrList[index].attr_def_key)) {
                                    item.attrList.push(attrList[index]);
                                }
                            }
                        }
                        $this.bomAttrList.push(item);
                        return;
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.BATTERY) {
                        item.rootName = 'Battery';
                        if (attrList !== null && attrList.length > 0) {
                            for (let index in attrList) {
                                if (['voltage', 'capacity', 'soft_ver', 'hard_ver'].includes(attrList[index].attr_def_key)) {
                                    item.attrList.push(attrList[index]);
                                }
                            }
                        }
                        $this.bomAttrList.push(item);
                        return;
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.HMI) {
                        item.rootName = 'HMI';
                        if (attrList !== null && attrList.length > 0) {
                            for (let index in attrList) {
                                if (['speed_limit', 'wheel_size', 'soft_ver', 'hard_ver'].includes(attrList[index].attr_def_key)) {
                                    if(attrList[index].attr_def_key == 'speed_limit'){
                                        attrList[index].value = bom.test_data['speed_limit'];
                                    }
                                    if(attrList[index].attr_def_key == 'wheel_size'){
                                        attrList[index].value = bom.test_data['wheel_size'];
                                    }
                                    if(attrList[index].attr_def_key == 'soft_ver'){
                                        attrList[index].value = bom.test_data['soft_ver'];
                                    }
                                    item.attrList.push(attrList[index]);
                                }
                            }

                        }
                        $this.bomAttrList.push(item);
                        return;
                    }

                    if (bom.category_root_id === Core.Const.CATEGORY_ROOT_TYPE.CONTROLLER) {
                        item.rootName = 'Controller';

                        if (attrList !== null && attrList.length > 0) {
                            for (let index in attrList) {
                                if (['power', 'current_limit', 'pas_signals', 'voltage', 'low_voltage', 'over_voltage', 'soft_ver', 'hard_ver'].includes(attrList[index].attr_def_key)) {
                                    if(attrList[index].attr_def_key == 'soft_ver'){
                                        attrList[index].value = bom.test_data['soft_ver'];
                                    }
                                    item.attrList.push(attrList[index]);
                                }
                            }
                        }
                        $this.bomAttrList.push(item);
                    }
                });
            },

            showUpdateDialog(sn, id) {
                this.selectSn = sn;
                this.selectId = id;
                this.newSn = '';
                this.dialogReplaceVisible = true;
            },

            updateSn() {
                Core.Api.Vehicle.bomChange(this.vehicleId, this.selectId, this.newSn.replace(/\s+/g, "")).then(res => {
                    this.dialogReplaceVisible = false;
                    this.selectSn = '';
                    this.newSn = '';
                    this.getVehicleDetail();
                })
            },

            onClickHistory() {
                this.$router.push({name: 'dealerReplaceReportList', query: {"vehicleId": this.vehicleId}});
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .vehicle-search-holder {
        height: 100%;

        .content {
            height: 100%;
            .content-holder {
                padding: 26px 60px;
                overflow: hidden;
                .btn-div {
                    display: inline-block;
                    position: absolute;
                    right: 80px;
                }
                .vehicle-item {
                    overflow: hidden;
                    padding-bottom: 20px;
                }
                .content-item {
                    width: 50%;
                    float: left;
                    font-size: 0;
                    line-height: 50px;
                    overflow: hidden;
                    .title {
                        display: inline-block;
                        vertical-align: top;
                        width: 140px;
                        font-size: 14px;
                        color: rgba(255, 255, 255, 0.6);
                        padding-left: 10px;
                    }
                    .desc {
                        display: inline-block;
                        vertical-align: top;
                        width: calc(100% - 140px);
                        span {
                            font-size: 14px;
                            color: #FFFFFF;
                            &.strong {
                                font-weight: bold;
                            }
                        }
                        .input {
                            width: 200px;
                        }
                    }
                }
                .width {
                    overflow: hidden;
                    width: 100%;
                }

                .bom-content-holder {
                    padding: 10px 20px;
                }

                .bom-item {
                    width: 100%;
                    overflow: hidden;
                    .bom-header {
                        overflow: hidden;
                        line-height: 40px;
                        .bom-title {
                            font-size: 14px;
                            color: white;
                            padding-left: 5px;
                        }
                        .btn-replace {
                            float: right;
                            margin-right: 40px;
                        }
                    }
                    .bom-content {
                        float: left;
                        display: inline-block;
                        width: 50%;
                        font-size: 0;
                        line-height: 30px;
                        overflow: hidden;
                        .title {
                            display: inline-block;
                            vertical-align: top;
                            width: 140px;
                            font-size: 12px;
                            color: rgba(255, 255, 255, 0.6);
                            padding-left: 10px;
                        }
                        .desc {
                            display: inline-block;
                            vertical-align: top;
                            width: calc(100% - 140px);
                            span {
                                word-break: break-all;
                                font-size: 12px;
                                color: #FFFFFF;
                                &.strong {
                                    font-weight: bold;
                                }
                            }

                        }
                    }
                }
            }
        }

        .report-table {
            width: 100%;
            border-top: 1px solid rgba(255, 255, 255, 0.1);
            tr {
                line-height: 30px;
            }
            .head {
                color: white;
            }
            .report-td {
                padding: 0 6px;
                width: 160px;
                border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                font-size: 14px;
                text-align: center;
            }
        }

        .new-password {
            .input {
                .el-input__inner {
                    padding-right: 40px;
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// search.vue?396aade2