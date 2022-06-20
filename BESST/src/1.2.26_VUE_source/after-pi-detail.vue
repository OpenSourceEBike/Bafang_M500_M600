<template>
    <div class="after-pi-edit">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goPIList" title=" back "></div>
                </div>
            </div>
            <label class="title">{{title}} Detail</label>  拆分要不要单独一个表？   拆分的PI 要有记录，但是并没有实际的效果。 <br>
            总金额也是PI的一个关建的置，List的请求，虽然不会请求这个值， 但是 保存确认PI的时候，会把总金额更新到PI列表，而不是停留在详情里面，这样的话， 字子PI也要出现在PI列表，是否显现？？
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item edit" @click="goPIEdit" v-if="true"
                         title="save"></div>
                    <el-button type="primary" @click="splitPI">拆分</el-button>
                    <!--<div class="icon-item add" @click="goPISplit" v-if="true"-->
                         <!--title="split"></div>-->
                </div>
            </div>
        </div>
        <div class="loading" v-if="loading">
            <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon"/>
        </div>
        <div class="content-wrap">
            <div class="content">
                <div class="order-info">
                    <div class="left">
                    <div class="info-list">
                        <div class="col-left">
                            <ul class="list-group">
                                <li class="list-group-item">
                                    <span class="label">Customer</span>
                                    <span class="input" v-model="customer"></span>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">Country</span>
                                    <span class="input " v-model="country"></span>
                                </li>
                                <li class="list-group-item" v-show="piType != '2' ">
                                    <span class="label">Address</span>
                                    <span class="input " v-model="address"></span>
                                </li>
                                <li class="list-group-item" v-show="piType != '2' ">
                                    <span class="label">Atten</span>
                                    <span class="input " v-model="atten"></span>
                                </li>
                                <li class="list-group-item" v-show="piType != '2' ">
                                    <span class="label">Tel</span>
                                    <span class="input " v-model="tel"></span>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">VAT No.</span>
                                    <span class="input " v-model="vatNo"></span>
                                </li>
                                <li class="list-group-item" v-show=" piType != '1' ">
                                    <span class="label">ETD</span>
                                    <el-date-picker
                                        v-model="estimatedDate"
                                        type="date"
                                        placeholder="estimatedDate">
                                    </el-date-picker>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">Trade Terms</span>
                                    <el-select v-model="tradeTerm" class="input">
                                        <el-option
                                            v-for="item in tradeTermType"
                                            :key="item.value"
                                            :label="item.label"
                                            :value="item.value">
                                        </el-option>
                                    </el-select>
                                </li>
                            </ul>
                        </div>

                        <div class="col-right">
                            <ul class="list-group">
                                <li class="list-group-item" v-show=" piType != '1' ">
                                    <span class="label">PO No.</span>
                                    <span class="input" v-model="poNo"></span>
                                </li>
                                <li class="list-group-item" v-show=" piType != '2' ">
                                    <span class="label" >Orderr No.</span>
                                    <span class="input" v-model="orderNo"></span>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">Sales</span>
                                    <span class="input" v-model="creator"></span>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">PI No.</span>
                                    <span class="input" v-model="piNo"></span>
                                </li>
                                <li class="list-group-item" v-show=" piType != '2' ">
                                    <span class="label">ETD</span>
                                    <el-date-picker
                                        v-model="estimatedDate"
                                        type="date"
                                        placeholder="estimatedDate">
                                    </el-date-picker>
                                </li>
                                <li class="list-group-item">
                                    <span class="label">Payment Term</span>
                                    <el-select v-model="paymentTerm" class="input">
                                        <el-option
                                            v-for="item in paymentTermType"
                                            :key="item.value"
                                            :label="item.label"
                                            :value="item.value">
                                        </el-option>
                                    </el-select>
                                </li>
                                <li class="list-group-item">
                                    <span></span>
                                    <span></span>
                                </li>
                                <li class="list-group-item">
                                    <span></span>
                                    <span></span>
                                </li>
                            </ul>
                        </div>
                    </div>
                    </div>

                </div>
                <div class="order-info border">
                    <div class="parts-list">
                        <span class="labelList">Item list</span>
                    </div>

                    <div class="table-wrap">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                                <td v-for="item in tableHead">{{item}}</td>
                            </thead>
                            <tbody class="height2">
                                <tr v-for="(item, index) in piCompItems">
                                    <td>{{item.a}}</td>
                                    <td>{{item.b}}</td>
                                    <td>{{item.c}}</td>
                                    <td class="long-td">{{item.d}}</td>
                                    <td>{{item.e}} </td>
                                    <td>{{item.f}}</td>
                                    <td>{{item.g}}</td>
                                    <td>{{item.h}}</td>
                                </tr>
                            </tbody>
                        </table>
                    </div>

                </div>
            </div>
            <div class="clear-20"></div>
            <div class="content">
                <div class="order-info">
                    <div class="parts-list">
                        <label class="label1">{{title}} Split List</label>
                        <el-button type="primary" class="att-btn" @click="splitPI" v-show="tableHeadSplit.length > 0">+</el-button>
                        <label class="labelAdd">Split Proforma Invoice</label>
                    </div>
                    <div class="table-wrap">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                            <td v-for="item in tableHeadSplit">{{item}}</td>
                            </thead>
                            <tbody class="height2" id="split" >
                                <tr v-for="(item, index) in piCompItemsSplit">
                                </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
    import Core from 'core/core';
    import printArea from 'printArea';


    export default {
        data(){
            return {
                Core: Core,
                componentSn: '',
                allSelectedForDelete: false,
                tableHead: ['Item name','BF Art Nr.','Custom Art Nr.','Description','Unit Price','VAT rate', 'Quantity', 'Amount'],
                piItem: {},
                piCompItems: [],
                title: 'Proforma Invoice',
                createDate: '', //?? 创建的日期，可以改吗？
                currency: '$',  //?? 所有的都是 美元
                loading:false,

                //形式发票信息
                piType: '1',   //  1 sales , 2 aftersales

                customer: 'A',
                country: 'C',
                address: '',
                atten: 'C',
                tel: 'C',
                vatNo: '',
                tradeTermType:[
                    {
                        value: '1',
                        label: 'FOB SHANGHAI'
                    },
                    {
                        value: '2',
                        label: 'FOB SHANGHAI'
                    },
                    {
                        value: '3',
                        label: 'FOB SHANGHAI'
                    }],
                tradeTerm: '',
                estimatedDate: '',


                poNo: '',  // 采购订单
                orderNo: '',  // ???
                creator: '',
                piNo: '',  // 形式发票
                paymentTermType:[
                    {
                        value: '1',
                        label: '100% paid before delivery'
                    },
                    {
                        value: '2',
                        label: '70% down payment,30% paid before delivery'
                    },
                    {
                        value: '3',
                        label: '30% down payment,70% paid before delivery'
                    }],
                paymentTerm:'',



                // 打印预览
                comBF: 'Bafang Electric Motor Science Technology B.V',
                comAdd: 'Bijsterhuizen 2415, 6604LK Wijchen',
                Tel: '+ 0031(0)85 2737 102',
                comBank: [
                    'ING Bank N.V',
                    'Bafang Electric Motor Science Technology B.V',
                    'NL 46 INGB 0020 1637 46',
                    'NL 8516070 44 B01',
                    '55204279',
                    'INGBNL2A',
                ],

                // Split
                tableHeadSplit: [],
                piCompItemsSplit: [],
                splitContent: '',


            }
        },
        created(){
        },
        mounted(){
//            console.log(this.$route);
            if( this.$route.query.from =='list'){
                this.queryPIItem();
            }else if(this.$route.query.from =='detail'){
                let params = this.$route.query.piItem;
//                    this.paCode = params.paCode;
//                    this.paFrom = params.paFrom;
//                    this.paUsage = params.paUsage;
//                    this.paContact = params.paContact;
//                    this.receiveDate = params.receiveDate;
//                    this.address = params.address;
//                    this.note = params.note;
            }

        },
        methods: {
            createPI(){

            },
            queryPIItem(){

                this.piCompItems = [
                    {'a':1, 'b':1, 'c':1, 'd':'111111111111111111111111111111111111111', 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':'111111111111111', 'b':1, 'c':1, 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':'11111111111111111111111', 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.0, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.1, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':'11111111111111111', 'e':10.2, 'f':'15%', 'g':200, 'h':100},
                    {'a':1, 'b':1, 'c':1, 'd':1, 'e':10.3, 'f':'15%', 'g':200, 'h':100},
                ];
            },

            goPIList(){
                this.$router.push({name:'afterPi'});
            },
            goPIEdit(){
                this.$router.push({name:'afterPiEdit'});
            },
            goPISplit(){
                this.$router.push({name:'afterPiSplit'});
            },

            splitPI(){
                if( this.tableHeadSplit.length==0 ){
                    this.tableHeadSplit = ['Item Name','Quantity','PI_1','PI_2'];
                    let tem = {'a':'', 'h':'', 'j':'', 'k':0};
                    let arr = [];
                    this.piCompItems.forEach((item)=>{
                        tem.a = item.a;
                        tem.h = item.h;
                        tem.j = item.h;
                        arr.push(tem);
                    });
                    this.piCompItemsSplit = arr;
//                    console.log(this.piCompItemsSplit);
                }

            },

            onClickPrint() {
                //  window.print();
                let print = "div.print-pi-wrap";

                let extraCss = "";
                let mode = "iframe";
                let close = false;
                let retainAttr = ["class", "id", "style", "on"];
                let headElements =
                    '<meta charset="utf-8" />,<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';

                let options = {
                    mode: mode,
                    popClose: close,
                    extraCss: extraCss,
                    retainAttr: retainAttr,
                    extraHead: headElements
                };
                $(print).printArea(options);
            },


        },
        computed: {

        },
        components: {}
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .after-pi-edit{
        .content {
            /*height: 100%;*/
            /*overflow: auto;*/
            border: 1px solid #2B2F3B;
            .order-info {
                /*height: 272px;*/
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
                    .item {
                        margin-left: 40px;
                        line-height: 60px;
                        overflow: hidden;
                        height: 50px;
                        .label {
                            display: inline-block;
                            width: 156px;
                        }
                    }
                    .info-list {
                        font-size: 0;
                        .col-left {
                            display: inline-block;
                            width: 50%;
                        }
                        .col-right {
                            display: inline-block;
                            width: 50%;
                        }
                        .node{
                            width: 100%;
                            .label{
                                width:40%;
                            }
                        }
                        .list-group {
                            padding: 0 40px;
                            .list-group-item {
                                height: 50px;
                                line-height: 50px;
                                font-size: 0;
                                span {
                                    display: inline-block;
                                }
                                span:first-child {
                                    /*font-size: 12px;*/
                                    color: #FFFFFF;
                                    width: 40%;
                                }
                                span:last-child {
                                    /*font-size: 12px;*/
                                    color: #A1A5B1;
                                    width: 60%;
                                }
                            }
                        }
                    }
                }
                .parts-list {
                    float: left;
                    padding: 27px 40px;
                    /*font-size: 12px;*/
                    overflow: hidden;
                    height: 50px;
                    width: 100%;
                    box-shadow: 0 -1px #2B2F3B;
                    .label1 {
                        display: inline-block;
                        /*font-size: 12px;*/
                    }
                }
                .table-wrap {
                    box-shadow: 0 -1px #2B2F3B;
                    overflow-x: hidden;
                    .long-td{
                        max-width: 200px;
                        text-overflow: ellipsis;
                        overflow: hidden;
                    }
                    .table-content{
                        padding-bottom: 0;
                    }
                }
                .footer {
                    width: 100%;
                    height: 50px;
                    padding: 10px 38px;
                    clear: both;
                    .footer-pagination {
                        float: right;
                    }
                    .footer-delete {
                        margin-top: 3px;
                        background-color: #858890;
                    }
                }
            }
        }
    }
    .print-pi-wrap {
        display: none;
        .printHead {
            h2,h3{
                text-align: center;
            }
            h2{
                font-size:24px;
            }
            h3{
                font-size:14px;
            }
            .dis_left{
                float:left;
            }
            .dis_right{
                float:right;
                label{
                    width:120px;
                }
                span{
                    width:200px;
                    text-align: center;
                }
            }
            label{
                display:inline-block;
                /*text-align: center;*/
            }

        }
    }

</style>



// WEBPACK FOOTER //
// after-pi-detail.vue?b49e893a