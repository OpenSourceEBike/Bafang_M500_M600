<template>
    <div class="dealer-order-edit">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <label class="title">{{ $t('orderEdit') }}</label>
        </div>
        <div class="content-wrap">
            <div class="content">
                <label class="title">{{ $t('customerInfo.title') }}</label>
                <div class="info">
                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('customerInfo.name') }}</label>
                            <el-input class="input" v-model="customerInfo.name"></el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('customerInfo.phone') }}</label>
                            <el-input class="input" v-model="phone">
                                <el-select v-model="areaCode" filterable slot="prepend">
                                    <el-option
                                        v-for="(item, index) in areaCodeList"
                                        :key="index"
                                        :label="item.area_code"
                                        :value="item.area_code">
                                    </el-option>
                                </el-select>
                            </el-input>
                        </div>
                    </div>
                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('customerInfo.email') }}<span class="required">*</span></label>
                            <el-input class="input" v-model="customerInfo.email"></el-input>
                        </div>
                    </div>
                </div>
                <label class="title">{{ $t('bikeInfo.title') }}</label>
                <div class="info">
                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('bikeInfo.brand') }}</label>
                            <!--<el-select v-model="detailOrder.brand_name" class="input" :disabled="true">-->
                                <!--<el-option v-for="item in brandList"-->
                                    <!--:key="item.name"-->
                                    <!--:label="item.name"-->
                                    <!--:value="item.name">-->
                                <!--</el-option>-->
                            <!--</el-select>-->
                            <el-input v-model="detailOrder.brand_name" class="input" :disabled="true"></el-input>
                        </div>

                        <div class="item">
                            <label class="label">{{ $t('bikeInfo.model') }}</label>
                            <el-input class="input" v-model="detailOrder.item_name" :disabled="true"></el-input>
                        </div>
                    </div>
                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('bikeInfo.bikeId') }}</label>
                            <el-input class="input" v-model="detailOrder.object_sn" :disabled="true">
                                <!--<template slot="append">-->
                                    <!--<img style="vertical-align: middle;" src="../../../assets/images/icon/scan.png"-->
                                         <!--width="20" height="20"/>-->
                                <!--</template>-->
                            </el-input>
                        </div>
                        <div class="item">
                            <label class="label">{{ $t('bikeInfo.warrantyPeriod') }}</label>
                            <el-select v-model="detailOrder.warranty" slot="append" class="input">
                                <el-option
                                    v-for="item in numbers"
                                    :key="item.value"
                                    :label="item.label"
                                    :value="item.value">
                                </el-option>
                            </el-select>
                        </div>
                    </div>
                </div>
                <label class="title">{{ $t('detail.title') }}</label>
                <div class="info detail">
                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('detail.orderNo') }}</label>
                            <el-input class="input" v-model="detailOrder.sn" :disabled="true"></el-input>
                        </div>
                        <div class="invoice">
                            <label class="label">{{ $t('detail.invoice') }}</label>
                            <div class="update-invoice">
                                <el-input class="input" v-model="invoiceNo"></el-input>
                                <el-date-picker
                                    class="invoice-date"
                                    v-model="invoiceDate"
                                    type="date"
                                    :placeholder="$t('detail.invoiceDate')"
                                    :picker-options="pickerOptions">
                                </el-date-picker>
                            </div>
                        </div>
                    </div>

                    <div class="part">
                        <div class="item">
                            <label class="label">{{ $t('detail.price') }}</label>
                            <el-input class="input" v-model="detailOrder.price">
                                <el-select v-model="currency" slot="prepend">
                                    <el-option
                                        v-for="item in currencies"
                                        :key="item.currency"
                                        :label="item.label"
                                        :value="item.currency">
                                    </el-option>
                                </el-select>
                            </el-input>
                        </div>
                        <div class="height-item">
                            <label class="label">{{ $t('detail.remark') }}</label>
                            <el-input
                                resize="none"
                                class="input"
                                type="textarea"
                                :rows="3"
                                v-model="detailOrder.remark">
                            </el-input>
                        </div>
                    </div>
                </div>
                <el-button class="save-btn" @click="updateSaleOrder">{{ $t('save') }}</el-button>
            </div>
        </div>
    </div>
</template>

<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import areaCodeInDB from 'core/areaCode'

    export default{
        data(){
            return {
                invoiceDate: '',
                invoiceNo: '',
                invoice: '',
                dialogVisible: false,
                orderId: '',
                brandList: [],
                detailOrder: {
                    brand_org_id: '',
                    brand_name: '',
                    item_name: '',
                    object_sn: '',
                    warranty: '',
                    sn: '',
                    price: '',
                    remark: '',

                },
                customerInfo: {
                    name: '',
                    phone: '',
                    email: '',
                    gender: '',
                },
                pickerOptions: {
                    disabledDate(time) {
                        return time.getTime() >= Date.now();
                    }
                },

                currency: 1,
                currencies: [{
                    currency: 1,
                    label: '€'
                }, {
                    currency: 2,
                    label: '$'
                }, {
                    currency: 3,
                    label: 'DM.'
                }, {
                    currency: 4,
                    label: '￥'
                }, {
                    currency: 5,
                    label: '￡'
                }],

                numbers: [{
                    value: 6,
                    label: '6 mon.'
                }, {
                    value: 12,
                    label: '12 mon. '
                }, {
                    value: 24,
                    label: '24 mon. '
                }, {
                    value: 36,
                    label: '36 mon.'
                } ],

                areaCodeList: [],
                areaCode: '+86',

                phone: '',
            }
        },
        i18n: {
            messages: {
                en: EN.Dealer.Sale.Edit,
                zh: ZH.Dealer.Sale.Edit,
                de: DE.Dealer.Sale.Edit,
                nl: NL.Dealer.Sale.Edit
            }
        },
        mounted: function () {
            this.areaCodeList = areaCodeInDB.init();
            this.orderId = this.$route.query.id;
            this.getOrderDetail();
        },
        methods: {
            getOrderDetail(){
                Core.Api.Order.getDealerOrderDetail(this.orderId).then(res => {
                    this.detailOrder = res.order;
                    this.invoice = res.order.invoice_no;
                    this.customerInfo = res.order.customer;
                    this.currency = res.order.currency;
                    this.detailOrder.price = res.order.price / 100;
                    this.getInvoice();
                    this.getBranList();
                    this.getPhone();
                });
            },

            getPhone(){
            	let phoneArr = this.customerInfo.phone.split(" ");
                    this.areaCode = phoneArr[0];
                    this.phone = phoneArr[1];
            },

            checkParams(){

                if (!this.customerInfo.email) {
                    this.$message({
                        message: 'Please input customer email',
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.detailOrder.sn) {
                    this.$message({
                        message: 'Please input bike sn',
                        type: 'warning'
                    });
                    return false;
                }
//                if (!this.detailOrder.price) {
//                    this.$message({
//                        message: 'Please input bike price',
//                        type: 'warning'
//                    });
//                    return false;
//                }

//                if (!this.invoiceNo) {
//                    this.$message({
//                        message: 'Please input invoiceNo',
//                        type: 'warning'
//                    });
//                    return false;
//                }
//                if (!this.invoiceDate) {
//                    this.$message({
//                        message: 'Please input choice invoice date',
//                        type: 'warning'
//                    });
//                    return false;
//                }
                return true;
            },

            getBranList(){
                Core.Api.Brand.getBrandListById(this.detailOrder.brand_org_id).then(res => {
                    this.brandList = res.brand_list;
                });
            },

            getInvoice(){
                let index = this.invoice.indexOf('date');
                if (index >= 0) {
                    this.invoiceDate = new Date().setTime(this.invoice.substring(index + 4) * 1000);
                    this.invoiceNo = this.invoice.substring(0, index);
                }
            },

            updateInvoice(){
            	if(!this.invoiceNo.trim()){
                    this.$router.back();
            		return;
                }
                if (this.invoiceDate) {
                	if(typeof (this.invoiceDate) !== 'number'){
                        this.invoiceDate = Date.parse(this.invoiceDate) / 1000;
                    }else{
                        this.invoiceDate = this.invoiceDate / 1000;
                    }
                    this.invoice = this.invoiceNo + 'date' + this.invoiceDate;
                }
                Core.Api.Order.updateSaleOrderInvoice(this.orderId, this.invoice).then(res => {
                    this.$message({
                        message: 'Edit successfully',
                        type: 'success'
                    });
                    this.$router.back();
                });
            },

            updateSaleOrder(){
            	if (!this.checkParams()) {
                    return;
                }
                let customerPhone  = `${this.areaCode} ${this.phone}`;
                Core.Api.Order.saleOrderSave(this.orderId, this.detailOrder.brand_org_id, this.customerInfo.name, this.customerInfo.gender,
                    this.customerInfo.email, customerPhone, this.detailOrder.object_sn, this.detailOrder.price*100,
                    this.detailOrder.currency, this.detailOrder.warranty, this.detailOrder.remark, this.detailOrder.brand_name).then(res => {
                    this.updateInvoice();

                });
            },
        }

    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
    .dealer-order-edit {
        height: 100%;
        .content {
            .title {
                display: block;
                margin-top: 15px;
                padding: 9px 33px;
                font-size: 18px;
            }

            .info {
                margin-top: 15px;
                background-color: #353945;
                height: 135px;
                border-bottom: 1px dotted #2B2F3B;
                overflow: hidden;
                .part {
                    float: left;
                    width: 465px;
                    .item {
                        margin-left: 33px;
                        line-height: 60px;
                        overflow: hidden;
                        height: 60px;
                        .label {
                            display: inline-block;
                            width: 150px;
                        }
                        .input {
                            margin-left: 5px;
                            width: 260px;
                        }

                    }
                }
            }
            .detail {
                height: 175px;
                .invoice {
                    font-size: 0;
                    margin-top: 12px;
                    margin-left: 33px;
                    .label {
                        display: inline-block;
                        width: 150px;
                        vertical-align: top;
                    }
                    .update-invoice {
                        margin-left: 10px;
                        width: 260px;
                        display: inline-block;
                        .input {
                            width: 260px;
                        }
                        .invoice-date {
                            width: 260px;
                            margin-top: 15px;
                        }
                    }
                }
            }
            .save-btn {
                margin: 22px 40px;
                width: 116px;
            }
            .required{
                display: inline-block;
                padding-left: 6px;
                color: #E26829;
            }
            .height-item {
                margin-left: 33px;
                height: 120px;
                overflow: hidden;
                .label {
                    display: inline-block;
                    width: 150px;
                    position: relative;
                    top: -55px;
                }
                .input {
                    margin-left: 10px;
                    margin-top: 15px;
                    width: 260px;
                }
            }

        }
    }
</style>



// WEBPACK FOOTER //
// edit.vue?16341a24