<template>
    <div class="dealer-brand-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
        </div>
        <!--<div class="content-holder">-->
        <div class="iframeWrap content-holder">
            <!--<div id="parentdiv">-->
                <!--<iframe src="https://www.bafang-e.com" frameborder="0" style="width:100%; height:100%;border:0px;" >-->
                <!--</iframe>-->
            <!--</div>-->
            <!--<iframe src="about:blank" frameborder="0" style="width:100%; height:80%;border:0px;"  class="webFrame" allow-top-navigation ></iframe>-->
            <div class="carouselWrap">
                <el-carousel indicator-position="none" :interval="3000" arrow="always" ref="carousel"
                             @change="change">
                    <el-carousel-item v-for="item in productList" :key="item">
                        <div class="img-box">
                            <img :src="item | getImgUrl" width="500"/>
                        </div>
                    </el-carousel-item>
                </el-carousel>
                <div class="thumb-img-box">
                    <div class="thumb"
                         :class="{'selected': index == productSelected}"
                         v-for="(item, index) in productList"
                         :key="item"
                         @click="switchDemonstration(index)">
                        <img class="thumb-img" :src='item | getImgUrl'/>
                    </div>
                </div>
            </div>
            <!--<div class="webWrap">-->
                <!--<div class="webcontent">-->
                    <!--<label class="label">{{ $t('webLink') }}</label>-->
                    <!--<el-input v-model="link" @click.native="" class="input" disabled></el-input>-->
                    <!--<a :href="helpUrl" target="_blank">-->
                        <!--<el-button @click="showWebView" type="primary">{{ $t('view') }}</el-button>-->
                    <!--</a>-->

                <!--</div>-->
            <!--</div>-->
            <div class="operationWrap">
                <p class="operation">
                    <el-button type="primary" @click="goToOrderDetail()">{{ $t('createOrder') }}</el-button>
                    <el-button type="primary" @click="goToOrderList()">{{ $t('orderList') }}</el-button>
                </p>
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
    import ElButton from "../../../../node_modules/element-ui/packages/button/src/button";

    export default{
        components: {ElButton},
        data(){
            return {
                Core: Core,
                ownOrg: Core.Data.getOrg(),
                invitationCode: '',
                tableHead: ['Name', 'Phone', 'Website', 'Country', 'Operation'],
                currentPage: 1,
                total: 0,
                brandList: [],
                selectBrand: {},
                partnerBrand: {},
                dialogDetailVisible: false,
                dialogCodeVisible: false,

                productList: [],
                productSelected: '',
                link: '',
                brandItem: '',
                helpUrl: "" ,

            }
        },
        i18n: {
            messages: {
                en: EN.Dealer.Partner.List,
                zh: ZH.Dealer.Partner.List,
                de: DE.Dealer.Partner.List,
                nl: NL.Dealer.Partner.List
            }
        },
        mounted: function () {
            // 国际化
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });

            this.brandItem = this.$route.query.web;
            this.getImage()
        },

        methods: {
            getImage(){
                Core.Api.Item.queryByBrand(this.brandItem.id).then((response) => {

                    let res = response.result;

                    console.log( res);

                    for( let i=0; i< res.length; i++){
                        let item = res[i].item_list;
                        for( let j=0; j< item.length; j++){
                            if(item[j].img){
                                this.productList.push( item[j].img);
                            }
                        }
                    }
                    console.log(this.productList);
                    /**
                    brand_id: 97
                    category_id: 1
                    category_root_id: 1
                    create_time: 1520261410
                    desc: "test"
                    ext_part_no: "11111"
                    id: 1097
                    img: "71188f82f4f04c3c8e7b4b36430fccb5b28024386447e3b32280461489a09f64.jpg"
                    is_default: 1
                    is_show: 1
                    name: "test1111"
                    org_id: 65
                    parent_id: 0
                    set_id: 0
                    update_time: 1520262075
                    user_id: 66
                    weight: 0
                    **/

                });
            },
//            getHeight(){
//                if(document.readyState == 'complete'){
//
//                    let height = (document.body.clientHeight-200) + 'px';
//                    $('#parentdiv',window.parent.document).css('height',height);
//                    window.clearInterval(this.ht);
//                    console.log(height);
//                }
//            },

            goToOrderDetail(){
                this.$router.push({name:'dealerSaleOrderSave',query:{'brandId':this.brandItem.id}});
            },
            goToOrderList(){
                this.$router.push({name:'dealerSaleOrderList',query:{'brandId':this.brandItem.id}});
            },
            change(item) {
                this.productSelected = item;
//                this.updateLink();
            },

            switchDemonstration(index) {
                this.$refs.carousel.setActiveItem(index);

            },
//            updateLink(link){
//                this.link = link;
//                if(this.link && this.link.substr(0,5).indexOf('http') > -1){
//                    this.helpUrl = this.link + '/?t=' + Core.Util.time();
//                }else{
//                    this.helpUrl = 'http://' + this.link + '/?t=' + Core.Util.time();
//                }
//            },
            showWebView(){

            },
//            $("#copy").bind('click',function() {
//                var Url2 = document.getElementById("textAreas");
//                //文本框内容
//                Url2.select();
//                //选中内容
//                document.execCommand("Copy");
//                //调用复制
//                alert("复制成功！");
//            }




        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .dealer-brand-list {
        height: 100%;
        .header {
            .btn-accept {
                background-color: #98B42C;
            }
        }
        /**
        #parentdiv{
            //transform:translate(-20px,0);
            //-webkit-transform: translate(-20px,0);
            margin-right:-20px;
        }
        **/
        .webWrap{
            height:60px;
            min-width:500px;
            margin: 0 auto;
            line-height:60px;;

                .input{
                    width:200px;
                }
                .webcontent{
                    margin-left: 33px;
                    height: 60px;
                    line-height: 60px;
                    display: flex;
                    align-items: center;
                    .label {
                        width: 210px;
                        display: inline-block;
                        line-height: 20px;
                        vertical-align: middle;
                        word-wrap:break-word;
                    }
                    .input {
                        width: 400px;
                    }
                    > button {
                        margin-right: 12px;
                        width:120px;
                    }

                }
        }
        .iframeWrap{
            width:100%;
            height:100%;
            iframe::-webkit-scrollbar {
                width: 0px;
                height: 0px;
            }
        }
        .operationWrap{
            margin-top: 20px;
            margin-right:-20px;
            text-align: center;
        }
        .content-holder{
            .carouselWrap {
                min-width: 800px;
                height: auto;
                overflow: auto;
                background: #fff;
                .el-carousel {
                    width: 800px;
                    height: auto;
                    margin: 0 auto;
                    .el-carousel__container {
                        width: 100%;
                        height: 350px;
                        /*height: "234px"*/
                    }
                    .el-carousel__item .img-box {
                        width: 100%;
                        height: 100%;
                        text-align: center;
                        img {
                            width: 500px;
                            height: auto;
                        }
                    }
                }
            }
            .thumb-img-box {
                width: 100%;
                height: 72px;
                font-size: 0;
                text-align: center;
                .thumb {
                    display: inline-block;
                    width: 72px;
                    height: 72px;
                    margin-right: 10px;
                    /*background: url('../../../assets/images/components/carousel-example@2x.png');*/
                    background-size: cover;
                    border: 1px solid rgba(39, 43, 55, 0.20);
                    cursor: pointer;
                    overflow: hidden;
                    .thumb-img {
                        width: 70px;
                        height: 70px;
                    }
                    &:last-child {
                        margin-right: 0;
                    }
                    &.selected {
                        border: 2px solid rgba(226, 104, 41, 1);
                    }
                }
            }
        }

    }
    @media (min-width: 768px){
        //>=768的设备
        .iframeWrap{

        }
    }

    @media (min-width: 992px){
        //>=992的设备
    }

    @media (min-width: 1200px){
        //>=1200的设备
    }
</style>



// WEBPACK FOOTER //
// website.vue?a67dd3e4