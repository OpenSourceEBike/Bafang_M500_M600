<template>
    <div class="bom-product-detail">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <span class="title">{{itemDetail.name}}</span>

            <div class="btn-group">
                <el-button type="primary" @click="goToSaveView('edit')" v-if="authList[Core.Const.AUTH_LIST.productProductEdit]">{{ $t('edit') }}</el-button>
                <el-button type="primary" @click="goToSaveView('copy')" v-if="authList[Core.Const.AUTH_LIST.productProductAdd] || authList[Core.Const.AUTH_LIST.productProductEdit]">{{ $t('copy') }}</el-button>
                <el-button type="delete" @click="deleteDialogVisible = true" v-if="authList[Core.Const.AUTH_LIST.productProductDelete]">{{ $t('delete') }}</el-button>
            </div>
        </div>
        <div class="content-holder">
            <div class="carousel">
                <el-carousel height="234px" indicator-position="none" :interval="5000" arrow="always" ref="carousel"
                             @change="change">
                    <el-carousel-item v-for="item in productList" :key="item">
                        <div class="img-box">
                            <img :src='item'/>
                        </div>
                    </el-carousel-item>
                </el-carousel>
                <div class="thumb-img-box">
                    <div class="thumb"
                         :class="{'selected': index == productSelected}"
                         v-for="(item, index) in productList"
                         :key="item"
                         @click="switchDemonstration(index)">
                        <img class="thumb-img" :src='item'/>
                    </div>
                </div>
            </div>
            <div class="content-box">
                <div class="content-item">
                    <label class="title">{{ $t('model') }}</label>
                    <div class="desc">
                        <span class="strong">{{itemDetail.name}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('partNo') }}</label>
                    <div class="desc">
                        <span class="strong">{{itemDetail.ext_part_no}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('desc') }}</label>
                    <div class="desc">
                        <span>
                          {{itemDetail.desc}}
                        </span>
                    </div>
                </div>

                <div class=" extra-div" v-for="item in itemDetail.attrList">
                    <!--input   text-->
                    <div class="extra text"
                         v-if="item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.TEXT">
                        <label class="title">{{item.attr_def_name}}</label>
                        <div class="desc text" v-show="item.value">{{item.value}}</div>
                        <div class="desc text" v-show="!item.value">/</div>
                    </div>
                    <!--input   select-->
                    <div class="extra text"
                         v-if="item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.SELECT">
                        <label class="title">{{item.attr_def_name}}</label>
                        <div class="desc text" v-if="item.attr_def_name == 'Currency'">{{item.value | currencyType}}
                        </div>
                        <div class="desc text" v-else-if="item.attr_def_name == 'Pack Unit'">{{item.value | packUnitType}}</div>
                        <div class="desc text" v-else>{{item.value}}</div>
                    </div>
                    <!--input   file-->
                    <div class="extra file"
                         v-if="item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILE||item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILES">
                        <label class="title">{{item.attr_def_name}}</label>
                        <div class="desc">
                            <div class="fileContent" v-for="item in item.value">
                                <label> {{item.name}}</label>
                                <div class="btn-group">
                                    <a :href="item.hash | getFileUrl(item.ext)" :download="item.name"
                                       class="btn-underline mr-15">
                                        {{ $t('download') }}</a>
                                    <!--<el-button type="preview" >{{ $t('download') }}</el-button>-->
                                    <el-button type="preview" @click="preview(item)">{{ $t('preview') }}</el-button>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="extra img"
                         v-if="item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.PIC||item.attr_def_type==Core.Const.CATEGORY_ATTR_INPUT_TYPE.PICS">
                        <label class="title">{{item.attr_def_name}}</label>
                        <div class="desc img-group">
                            <div v-for="value in item.value">
                                <img :src='value.hash | getFileUrl(value.ext)'/>
                            </div>
                        </div>
                    </div>
                </div>

                <!--//bom 应该不可以查看吧-->
                <div class="content-item" v-if="item.category_root_id !== 11">
                    <label class="title">{{ $t('bomList') }}</label>
                    <div class="desc h-24">
                        <div class="icons jc-start">
                            <div class="icon-item eye" @click="goBomView" :title="$t('view')"></div>
                        </div>
                    </div>
                </div>

                <div class="content-item">
                    <label class="title">{{ $t('training') }}</label>
                    <div class="desc h-24">
                        <div class="icons jc-start">
                            <div class="icon-item eye" @click="goTrainingView" :title="$t('view')"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!--图片预览弹窗-->
        <el-dialog title="Larger-Image" :visible.sync="previewDialogVisible" size="large" class="preview-dialog">
            <img :src="selectItem.hash | getFileUrl(selectItem.ext)" class="img"/><br/>
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
                Core: Core,
                itemId: '',
                item: '',
                imgUrlPrefix: Core.Const.NET.FILE_URL_PREFIX,
                editDialogVisible: false,
                deleteDialogVisible: false,
                itemDetail: {
                    name: '',
                    desc: '',
                    img: '',
                    attrList: [],
                    ext_part_no:''
                },
                productList: [],
                valueList: [],
                productSelected: "",
                historyDialogVisible: false,
                updateHWDialogVisible: false,


                upload: {
                    url: Core.Const.NET.FILE_UPLOAD_END_POINT,
                    paramName: 'file',
                    params: {'token': Core.Data.getToken()}
                },
                file: {},
                remark: '',
                HWVersion: '',
                HWInfo: {},
                HWRecord: [],
                fileList: [],
                selectItem: {},
                previewDialogVisible: false,

                authList: []
            }
        },
        i18n: {
            messages: {
                en: EN.Component.Product.Detail,
                zh: ZH.Component.Product.Detail,
                de: DE.Component.Product.Detail,
                nl: NL.Component.Product.Detail
            }
        },
        mounted: function () {
            // 获取权限
            this.authList = this.$store.state.userAuth;

            this.itemId = this.$route.query.id;
            Core.Api.Item.getItemDetail(this.itemId).then(res => {
                let item = res.item;
                console.log(res,'res')
                this.itemDetail.name = item.name;
                this.itemDetail.ext_part_no = item.ext_part_no;
                this.itemDetail.desc = item.desc;
                this.itemDetail.img = Core.Const.NET.IMG_URL_PREFIX + item.img;
                if( res.item.category_root_id >2){
                    let len= item.attr_list.length;
                    for(let i=0; i<len; i++){
                        if(item.attr_list[i].attr_def_key != "currency" && item.attr_list[i].attr_def_key != "price"){
                            this.itemDetail.attrList.push( item.attr_list[i] );
                        }
                    }
                }else{
                    this.itemDetail.attrList = item.attr_list;

                }

                this.item = item;
                this.productList.push(this.itemDetail.img);
                this.getShowPic();

                if (item.category_id != 3 && item.category_id != 11) {
                    this.getHwInfo();
                }
            });

        },
        methods: {
            change(item) {
                this.productSelected = item;
            },

            switchDemonstration(index) {
                this.$refs.carousel.setActiveItem(index);
                this.productSelected = index;
            },

            getShowPic() {
                if (!this.itemDetail.attrList) {
                    return;
                }
                let contain = false;
                let vm = this;
                this.itemDetail.attrList.forEach(function (item, index) {
                    if (item.attr_def_name === 'Display') {
                        contain = true;
                        vm.valueList = item.value;
                        vm.valueList.forEach(function (item) {
                            vm.productList.push(Core.Const.NET.FILE_URL_PREFIX + item.hash + '.' + item.ext);
                        });
                        //为了不让它在下部呈现，把它从数组里面去掉
                        vm.itemDetail.attrList.splice(index, 1);
                    }
                });
                if (contain && vm.productList.length > 1) {
                    //当包含展示图时，去掉原来的logo图
                    vm.productList.splice(0, 1);
                }
            },

            //删除方法
            deleteProduct() {
                Core.Api.Item.deleteItem(this.itemId).then(res => {
                    this.deleteDialogVisible = false;
                    this.$root.eventHub.$emit(Core.Const.EVENT_HUB.REFRESH_ITEM_LIST_BY_CATEGORY_ID, this.item.category_id);
                    this.$router.back();
                });
            },

            goToSaveView(type) {
                this.$router.push({
                    name: 'componentProductSave',
                    query: {"itemId": this.itemId, "categoryId": this.item.category_id, "type": type}
                });
            },

            goBomView() {
                this.$router.push({
                    name: 'dealerBomList',
                    query: {"id": this.itemId, "partNo": this.itemDetail.ext_part_no}
                });
            },

            showHistoryDialog() {
                this.historyDialogVisible = true;
                this.getHWHistoryList();
            },

            showUpdateHwDialog() {
                this.updateHWDialogVisible = true;
                this.fileList = [];
                this.HWVersion = '';
            },

            handleChangeFile(file) {
                this.fileList = [file];
            },

            //获取固件历史信息
            getHWHistoryList() {
                Core.Api.Firmware.getHistory(this.itemId).then(res => {
                    this.HWRecord = res.record;
                });
            },

            //上传固件
            uploadHW() {
                if (this.fileList.length <= 0) {
                    this.$message({
                        message: "Please upload file",
                        type: 'warning'
                    });
                    return;
                }
                if (!this.HWVersion) {
                    this.$message({
                        message: "Please input version",
                        type: 'warning'
                    });
                    return;
                }
                let fileName = this.fileList[0].response.data.file.hash + '.' + this.fileList[0].response.data.file.ext;
                Core.Api.Firmware.saveFirmware('', this.itemId, this.HWVersion, fileName, this.remark).then(res => {
                    this.updateHWDialogVisible = false;
                    this.getHwInfo();
                });
            },

            //获取固件信息
            getHwInfo() {
                Core.Api.Firmware.getLatest(this.itemId).then(res => {
                    this.HWInfo = res.item_firmware;
                });
            },

            goTrainingView() {
                this.$router.push({name: 'dealerVideoList', query: {itemId: this.itemId}});
            },

            preview(item) {
                this.selectItem = item;
                this.previewDialogVisible = true;
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .bom-product-detail {
        height: 100%;
        overflow: hidden;
        .content-holder {
            height: calc(100% - 60px);
            overflow: auto;
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
            .content-box {
                padding: 26px 60px;
                .content-item {
                    padding: 16px 0;
                    border-top: 1px solid rgba(255, 255, 255, 0.10);
                    font-size: 0;
                    &.border-none {
                        border: none;
                        padding-top: 0;
                    }
                    .title {
                        display: inline-block;
                        vertical-align: top;
                        width: 140px;
                        font-size: 14px;
                        color: rgba(255, 255, 255, 0.6);
                        padding: 0 5px;
                        &.strong {
                            color: #FFFFFF;
                            font-weight: bold;
                        }
                    }
                    .desc {
                        display: inline-block;
                        vertical-align: top;
                        width: calc(100% - 140px);
                        &.h-24 {
                            height: 24px;
                        }
                        span {
                            font-size: 14px;
                            color: #FFFFFF;
                            &.strong {
                                font-weight: bold;
                            }
                        }
                    }
                }
                .extra-div {
                    padding: 16px 0;
                    border-top: 1px solid rgba(255, 255, 255, 0.10);
                    .title {
                        display: inline-block;
                        vertical-align: top;
                        width: 140px;
                        font-size: 14px;
                        color: rgba(255, 255, 255, 0.6);
                        padding: 0 5px;
                    }
                    .extra {
                        .desc {
                            display: inline-block;
                            vertical-align: top;
                            position: relative;
                            top: -16px;
                            .fileContent {
                                margin-top: 16px;
                                .title {
                                    display: inline-block;
                                }
                                .btn-group {
                                    display: inline-block;
                                    margin-left: 6px;
                                }
                            }
                        }

                        .img-group {
                            margin-top: 10px;
                            img {
                                width: 50px;
                                height: 50px;
                            }

                        }
                        .text {
                            top: 0;
                        }
                    }
                }
                .version {
                    .version-label {
                        display: inline-block;
                        margin-right: 40px;
                    }
                }
            }
        }
        .history-table {
            width: 100%;
            border-top: 1px solid rgba(255, 255, 255, 0.1);
            tr {
                line-height: 30px;
            }
            .head {
                color: white;
            }
            .history-td {
                padding: 0 6px;
                width: 160px;
                border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                font-size: 14px;
                text-align: center;
            }
        }
        .upload-label {
            display: inline-block;
            width: 100px;
            vertical-align: top;
            padding-left: 20px;
        }
        .HWUpload {
            display: inline-block;
        }
        .remark {
            width: calc(100% - 140px);
        }
    }
</style>



// WEBPACK FOOTER //
// product-detail.vue?ebbda5ec