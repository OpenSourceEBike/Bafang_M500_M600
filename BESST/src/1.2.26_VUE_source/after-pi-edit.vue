<template>
    <div class="after-pi-edit">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goPIList" title=" back "></div>
                </div>
            </div>
            <label class="title">Save {{title}}</label>  生成PDF的形式发票的日期和汇率 怎么决定？
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item save" @click="savePI" v-if="true"
                         title="save"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                <div class="order-info">
                    <div class="left">
                        <div class="col-left">
                            <div class="item">
                                <label class="label">Customer<span class="required">*</span></label>
                                <el-input class="input" v-model="customer"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Country<span class="required">*</span></label>
                                <el-input class="input " v-model="country"></el-input>
                            </div>
                            <div class="item" v-show="piType != '2' ">
                                <label class="label">Address<span class="required">*</span></label>
                                <el-input class="input " v-model="address"></el-input>
                            </div>
                            <div class="item" v-show="piType != '2' ">
                                <label class="label">Atten<span class="required">*</span></label>
                                <el-input class="input " v-model="atten"></el-input>
                            </div>
                            <div class="item" v-show="piType != '2' ">
                                <label class="label">Tel<span class="required">*</span></label>
                                <el-input class="input " v-model="tel"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">VAT No.<span class="required">*</span></label>
                                <el-input class="input " v-model="vatNo"></el-input>
                            </div>
                            <div class="item" v-show=" piType != '1' ">
                                <label class="label">ETD<span class="required">*</span></label>
                                <el-date-picker
                                    v-model="estimatedDate"
                                    type="date"
                                    placeholder="estimatedDate">
                                </el-date-picker>
                            </div>
                            <div class="item">
                                <label class="label">Trade Terms<span class="required">*</span></label>
                                <el-select v-model="tradeTerm" class="input">
                                    <el-option
                                        v-for="item in tradeTermType"
                                        :key="item.value"
                                        :label="item.label"
                                        :value="item.value">
                                    </el-option>
                                </el-select>
                            </div>

                        </div>

                        <div class="col-right">
                            <div class="item" v-show=" piType != '1' ">
                                <label class="label">PO No.</label>
                                <el-input class="input" v-model="poNo"></el-input>
                            </div>
                            <div class="item" v-show=" piType != '2' ">
                                <label class="label" >Orderr No.</label>
                                <el-input class="input" v-model="orderNo"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">Sales</label>
                                <el-input class="input" v-model="creator"></el-input>
                            </div>
                            <div class="item">
                                <label class="label">PI No.</label>
                                <el-input class="input" v-model="piNo"></el-input>
                            </div>
                            <div class="item" v-show=" piType != '2' ">
                                <label class="label">ETD<span class="required">*</span></label>
                                <el-date-picker
                                    v-model="estimatedDate"
                                    type="date"
                                    placeholder="estimatedDate">
                                </el-date-picker>
                            </div>
                            <div class="item">
                                <label class="label">Payment Term<span class="required">*</span></label>
                                <el-select v-model="paymentTerm" class="input">
                                    <el-option
                                        v-for="item in paymentTermType"
                                        :key="item.value"
                                        :label="item.label"
                                        :value="item.value">
                                    </el-option>
                                </el-select>
                            </div>
                        </div>
                    </div>

                </div>
                <div class="order-info border">
                    <div class="parts-list">
                        <label class="labelList">Item list</label>
                        <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
                        <label class="labelAdd">Add Item</label>
                    </div>

                    <div class="table-wrap">
                        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                            <thead>
                                <td>
                                    <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
                                </td>
                                <td v-for="item in tableHead">{{item}}</td>
                            </thead>
                            <tbody class="height2">
                                <tr v-for="(item, index) in piCompItems">
                                    <td>
                                        <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                                    </td>
                                    <td>{{item.a}}</td>
                                    <td>{{item.b}}</td>
                                    <td><el-input v-model="item.c"></el-input></td>
                                    <td class="long-td"><el-input v-model="item.d"></el-input></td>
                                    <td>{{item.e}} </td>
                                    <td><el-input v-model="item.f"></el-input></td>
                                    <td><el-input v-model="item.g"></el-input></td>
                                    <td>{{item.h}}</td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <td colspan="9">
                                    <span style="float:right;padding: 10px 60px 0 0;"> Total Amount to be paid:{{totalAmount}}</span>
                                </td>
                            </tfoot>
                        </table>
                    </div>
                    <div class="footer">
                        <div class="btn-left">
                            <el-button type="delete" @click="deletePIItem">delete</el-button>
                        </div>
                        <div class="btn-right">
                            <el-button type="primary" @click="onClickPrint">Click Preview</el-button>
                        </div>
                    </div>
                </div>

            </div>
        </div>

         <!--生成这个表，即生成售后记录，处理结果。-->
        <el-dialog
            class="dialog"
            title="addItem"
            :visible.sync="dialogCreateVisible"
            size="large"
            top="10%">
            <div class="content">
                <div class="header">
                    <!--<el-form :inline="true" class="list-form-inline">-->
                        <!--<el-form-item label="Client Code">-->
                        <!--<label class="label">SN</label>-->
                        <!--&lt;!&ndash;<el-input class="input input-sn" v-model=""></el-input>&ndash;&gt;-->
                        <!--</el-form-item>-->
                    <!--</el-form>-->
                    <el-input
                        placeholder="category"
                        class="search-content"
                        :disabled="true"
                        v-model="selectCategoryItem.name"
                        @click="selectCategory">
                    </el-input>
                    <el-input
                        placeholder="Item Name"
                        class="search-content"
                        v-model="queryItemCondition">
                    </el-input>
                    <div class="btn-group-inline">
                        <div class="icons">
                        <div class="icon-item search" @click="queryItem" title=" search "></div>
                        <div class="icon-item clear" @click="clearCondition" title=" clear "></div>
                        </div>
                    </div>
                </div>
                <div class="table-wrap">
                    <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                        <thead>
                            <td>
                            <el-checkbox v-model="allSelectedForAdd"></el-checkbox>
                            </td>
                            <td v-for="item in itemHeadData">{{item}}</td>
                        </thead>
                        <tbody>
                            <tr v-for="(item, index) in selectItems">
                            <td>
                            <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                            </td>
                            <td><img :src="item.img | getImgUrl"></td>
                            <td>{{item.name}}</td>
                            <td>{{item.ext_part_no}}</td>
                            </tr>
                        </tbody>
                    </table>

                    <el-pagination
                        class="pagination"
                        @current-change="handleCurrentPageChange"
                        :current-page.sync="currentPage"
                        :page-size="10"
                        layout="prev, pager, next"
                        :total="total">
                    </el-pagination>

                </div>
                <div class="footer">
                    <el-button type="primary" @click="addItems">addItems</el-button>
                </div>
            </div>
        </el-dialog>


        <el-dialog
            class="dialog"
            title="Add Component"
            :visible.sync="dialogAddComponentVisible"
            size="large">
            <div class="content" style="max-height: 500px; overflow: auto">
                <div class="category-tree">
                    <el-tree :data="categoryList" :props="defaultProps" @node-click="handleNodeClick"></el-tree>
                </div>
            </div>
        </el-dialog>


        <!--打印报告-->
        <div class="print-pi-wrap">
            <div class="printHead">
                <h2>{{comBF}}</h2>
                <p>{{comAdd}}<br>{{Tel}}</p>
                <h3><b>{{title}}</b></h3>
                <div class="dis_left">
                    <div class="">
                        <label class="">Attn:</label>
                        <span class=""></span>
                    </div>
                    <div class="">
                        <label class="">Buyer:</label>
                        <span class="">xx BV</span>
                    </div>
                    <div class="">
                        <label class="">Atten:</label>
                        <span class="">Alfred van der Heiden</span>
                    </div>
                    <div class="">
                        <label class="">Tel:</label>
                        <span class="">{{tel}}</span>
                    </div>
                    <div class="">
                        <label class="">Trade Terms:</label>
                        <span class="">{{tradeTerm}}</span>
                    </div>
                    <div class="">
                        <label class="">Payment Term:</label>
                        <span class="">{{paymentTerm}}</span>
                    </div>
                </div>
                <div class="dis_right">
                    <div class="">
                        <label class=""><br></label>
                        <span class=""></span>
                    </div>
                    <div class="">
                        <label class="">Date:</label>
                        <span class="">1999-09-11</span>
                    </div>
                    <div class="">
                        <label class="">PI Nr.:</label>
                        <span class="">{{piNo}}111111</span>
                    </div>
                    <div class="">
                        <label class="">Order Nr.:</label>
                        <span class="">{{orderNo}}111111</span>
                    </div>
                    <div class="">
                        <label class="">ETD/delivery <br> time:</label>
                        <span class="">11111111111</span>
                    </div>

                </div>
            </div>
            <div class="content">
                <div class="table-wrap">
                    <table class="table-content" style="" border="0" cellspacing="0" cellpadding="0">
                        <thead>
                            <tr>
                                <td v-for="item in tableHead">{{item}}</td>
                            </tr>
                        </thead>
                        <tbody class="">
                            <tr v-for="(item, index) in piCompItems">
                                <td style="width:10%">{{item.a}}</td>
                                <td style="width:12%">{{item.b}}</td>
                                <td style="width:12%">{{item.c}}</td>
                                <td style="width:30%">{{item.d}}</td>
                                <td style="width:9%">{{currency}}{{item.e}}</td>
                                <td style="width:9%">{{item.f}}</td>
                                <td style="width:9%">{{item.g}}</td>
                                <td style="width:9%">{{item.h}}</td>
                            </tr>
                        </tbody>
                    </table>
                    <div style="float:right">
                        <span style="float:right;padding: 10px 60px 0 0;">Total Amount to be paid: {{currency}}{{totalAmount}}</span>
                    </div>
                </div>
                <div>

                </div>
            </div>
            <div class="clear-20"></div>
            <div class="printFoot">
                <p class="txt"><i>SAY TOTAL US DOLLAR FIFTY FIVE THOUSAND  FIVE HUNDRED AND FORTY NINE AND  CENT FIVE ONLY.</i></p>
                <div class="dis_left">
                    <div class="">
                        <label class="name">Bank Name:</label>
                        <span class="">{{comBank[0]}}</span>
                    </div>
                    <div class="">
                        <label class="name">Account Name:</label>
                        <span class="">{{comBank[1]}}</span>
                    </div>
                    <div class="">
                        <label class="name">Account Number:</label>
                        <span class="">{{comBank[2]}}</span>
                    </div>
                    <div class="">
                        <label class="name">VAT Number:</label>
                        <span class="">{{comBank[3]}}</span>
                    </div>
                    <div class="">
                        <label class="name">CVC Number:</label>
                        <span class="">{{comBank[4]}}</span>
                    </div>
                    <div class="">
                        <label class="name">BIC/SWIFT:</label>
                        <span class="">{{comBank[5]}}</span>
                    </div>
                </div>
            </div>
            <div class="clear-20"></div>
            <div class="clear-20"></div>
            <div class="printFoot">
                <div class="dis_left">
                    <div class="">
                        <label class=""></label>
                        <span class=""></span>
                    </div>
                    <div class="">
                        <label class="">Buyer Signature:</label>
                        <span class=""></span>
                    </div>
                </div>
                <div class="dis_right">
                    <div class="">
                        <label class="" >{{comBF}}</label>
                    </div>
                    <div><span class="sign" >XX(Salesman name)   <br>issued by </span></div>

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

                //增加
                dialogAddComponentVisible: false,
                dialogCreateVisible: false,
                queryItemCondition: '',
                allSelectedForAdd: false,
                itemHeadData: ['SN','Status','Operation'],
                selectItems: [],
                currentPage: 1,
                total: 0,
                dialogSelectCategoryVisible: false,
                categoryList: [],
                allChecked: false,

                defaultProps: {
                    id: 'id',
                    children: 'child',
                    label: 'name',
                    unit: 'unit',
                    des: 'des',
                },
                selectCategoryItem: {
                    id: '',
                    name: ''
                },


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
            savePI(){

                this.$router.push({name:'afterPiDetail',query: {"piItem": this.piCompItems}});
            },



            getAllCategory() {
                Core.Api.Category.getAllCategoryList(Core.Const.COMPONENT_ROOT_CATEGORY).then(
                    res => {
                        this.categoryList = res.category_list;
                    }
                );
            },
            queryItem(){
                let org = Core.Data.getOrg();
                let queryItemCondition = this.queryItemCondition.trim();
                if (org !== null) {
                    Core.Api.Item.queryOrderItem(org.id, this.selectCategoryItem.id, '', queryItemCondition, this.currentPage).then(res => {
                        this.queryItemList = res.query;
                        this.total = res.count;
                        if (!this.queryItemList.length > 0) {
                            this.$message({
                                message: "There are no matching data",
                                type: 'warning'
                            });
                            return;
                        }
                        this.selectItems = [];
                        this.queryItemList.forEach((item, index) => {
                            let selectItem = {
                                selected: false,
                                key: index,
                                name: item.name,
                                img: item.img,
                                ext_part_no: item.ext_part_no,
                                id: item.id
                            };
                            this.selectItems.push(selectItem);
                        })
                    })
                }
            },
            clearCondition(){
//                this.selectItems = [];
//                this.queryItemList = [];
//                this.queryItemCondition = '';
//                this.selectCategoryItem = '';
//                this.total = 0;
//                this.currentPage  = 1;
            },

            addItems(){
//                this.selectItems.forEach((item, index) => {
//                    if (item.selected === true) {
//                        let isAdded = false;
//                        this.orderItems.forEach((orderItem) => {
//                            if (orderItem.item_id === item.id) {
//                                isAdded = true;
//                            }
//                        });
//                        if (!isAdded) {
//                            let addItem = this.selectItems[index];
//                            this.orderItems.push({
//                                item_id: addItem.id,
//                                item_name: addItem.name,
//                                part_no: '',
//                                item_part_no: addItem.ext_part_no,
//                                amount: 1,
//                                selected: false
//                            });
//                        }
//                    }
//                });
                this.closeChooseItem();

            },
            deleteItem(index){

            },
            selectCategory() {
                this.dialogSelectCategoryVisible = true;
            },

            handleNodeClick(data) {
                if (data.hasOwnProperty("child")) {
                    let childList = data.child;
                    if (childList.length <= 0) {
                        this.selectCategoryItem = data;
                        this.dialogSelectCategoryVisible = false;
                    }
                }
            },
            handleCurrentPageChange(page) {
//                this.currentPage = page;
//                this.queryOrderItem();
            },
            deletePIItem(){

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
            'totalAmount':function(){
                let sum = 0;
                this.piCompItems.forEach(item =>{
//                    sum += item.e*item.h*(1+ parseInt(item.f)/100);
                });

                return sum;
            }

        },
        components: {}
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .after-pi-edit{
        .content-wrap{
            .content{
                width:1200px;
            }
            .table-content{
                padding-bottom: 10px;
            }
        }
        .content {
            /*height: 100%;*/
            border: 1px solid #2B2F3B;
            margin-bottom: 10px;
            .order-info {
                width: 100%;
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
                    .col-left{
                        float: left;
                        margin-top: 9px;
                        width: 474px;
                        .el-date-editor {
                            width: 256px;
                            margin-left: 10px;
                        }
                    }
                    .col-right{
                        float: left;
                        margin-top: 9px;
                        width: 474px;
                        .el-date-editor {
                            width: 256px;
                            margin-left: 10px;
                        }
                    }
                    .note{
                        margin-bottom: 5px;
                    }
                    .item {
                        margin: 2px 0 0 40px;
                        line-height: 60px;
                        overflow: hidden;
                        height: 50px;
                        clear:both;
                        .label {
                            display: inline-block;
                            width: 110px;
                        }
                        .input {
                            margin-left: 10px;
                            width: 256px;
                            border-radius: 0 0 10px 10px;
                        }
                        .input-note {
                            margin-left: 10px;
                            width: 726px;
                        }
                    }
                }
                .parts-list {
                    float: left;
                    padding: 0 40px;
                    font-size: 12px;
                    overflow: hidden;
                    height: 50px;
                    line-height: 50px;
                    width: 100%;
                    box-shadow: 0 -1px #2B2F3B;
                    .labelList {
                        /*font-size: 12px;*/
                    }
                    .att-btn {
                        margin-left: 46px;
                        padding: 6px 9px;
                        min-width: 0;
                    }
                    .labelAdd {
                        margin-left: 14px;
                        /*font-size: 12px;*/
                    }
                }
                .table-wrap{
                    .long-td{
                        max-width: 270px;
                        text-overflow: ellipsis;
                        overflow: hidden;
                        text-align: left;
                    }
                }
                .footer {
                    width: 100%;
                    height: 60px;
                    line-height: 60px;
                    box-shadow: 0 -1px #2B2F3B;
                    .footer-pagination {
                        /*float: right;*/
                    }
                    .btn-left{
                        display:inline-block;
                        width:100px;
                        float:left;
                        padding-left: 40px;
                    }
                    .btn-right{
                        display:inline-block;
                        float:right;
                        width:100px;
                        margin-right:60px;
                    }
                }
            }


        }
        .height1{
            height:272px;
        }
        .height2{
            height:372px;
            box-shadow: 1px 0 0 1px #2B2F3B;
        }
        .dialog {
            .el-dialog--large {
                width: 1100px;
            }
            .header {
                .search-content {
                    width: 200px;
                    margin-right: 10px;
                }
                .search-btn {
                    position: absolute;
                    top: 15px;
                    left: 250px;
                    width: 30px;
                    height: 30px;
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
// after-pi-edit.vue?9836faac