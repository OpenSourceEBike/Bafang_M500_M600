<template>
    <div class="model-bom-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="back" :title="$t('back')"></div>
                </div>
            </div>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item" :class="[!isEdit ? 'edit' : 'save']" @click="editBom" v-if="authList[Core.Const.AUTH_LIST.itemBomEdit]" :title="btnText"></div>
                    <div class="icon-item print" @click="printBom" v-show="!isEdit" :title="$t('print')"></div>
                    <div class="icon-item export" @click="exportBom" v-show="!isEdit" :title="$t('export')"></div>
                    <div class="icon-item delete" @click="showDeleteDialog" v-if="authList[Core.Const.AUTH_LIST.itemBomDelete]" :title="$t('delete')"></div>
                </div>
            </div>
        </div>
        <div class="content-holder">
            <div class="content">
                <div class="part top">
                    <label>{{ $t('articleNO') }} :</label>
                    <label class="art-no">{{artNo}}</label>
                    <div class="img-div">
                        <el-upload
                            class="upload"
                            :action="upload.url"
                            :show-file-list="false"
                            :on-success="imageUploaded"
                            :data="upload.data"
                            v-if="isEdit">
                            <div v-if="!upload.photographSrc">
                                <img src="../../../assets/images/icon/icon-upload@2x.png" class="upload-icon">
                            </div>
                            <img v-else class="img" :src="Core.Const.NET.IMG_URL_PREFIX + upload.photographSrc"/>
                        </el-upload>
                        <div v-else>
                            <div v-if="!upload.photographSrc" class="no-img">
                                <span>{{ $t('noPhoto') }}</span>
                            </div>
                            <img v-else class="img" :src="upload.photographSrc | getImgUrl"/>
                        </div>
                        <div class="tip" v-if="isEdit">
                            <div class="title">{{ $t('tip') }}:</div>
                            <div class="content">
                                {{ $t('content01') }}<br/>{{ $t('content02') }}
                            </div>
                        </div>
                        <el-button type="primary" @click="largeImgDialogVisible = true"
                                   :disabled="!upload.photographSrc">{{ $t('viewLargerImage') }}
                        </el-button>
                    </div>
                </div>
                <div class="part list">
                    <label class="list-title">
                        {{ $t('list') }}
                    </label>
                    <el-input
                        v-show="isEdit"
                        :placeholder="$t('category')"
                        class="search-input"
                        :disabled="true"
                        v-model="selectCategoryItem.name"
                        @click.native="dialogSelectCategoryVisible = true">
                    </el-input>

                    <el-input
                        v-show="isEdit"
                        :placeholder="$t('partName')"
                        class='search-input'
                        v-model="partName"
                    >
                    </el-input>

                    <div class="btn-group-inline" style="display: inline-block; vertical-align: middle">
                        <div class="icons" v-show="isEdit">
                            <div class="icon-item search" @click="queryOrderItem" :title="$t('search')"></div>
                            <div class="icon-item clear" @click="clearCondition" :title="$t('clear')"></div>
                        </div>
                    </div>

                    <table class="bom-list" border="0" cellspacing="0" cellpadding="0" v-if="childList.length > 0">
                        <thead>
                        <tr>
                            <td class="td head">{{$t('listHead.itemNo')}}</td>
                            <td class="td head">{{$t('listHead.partNo')}}</td>
                            <td class="td head">{{$t('listHead.partNo')}}<span>*</span></td>
                            <td class="td head">{{$t('listHead.picture')}}</td>
                            <td class="td head">{{$t('listHead.name')}}</td>
                            <td class="td head">{{$t('listHead.warranty')}}</td>
                            <!--<td class="td head">{{$t('listHead.price')}}</td>-->
                            <td class="td head">{{$t('listHead.operation')}}</td>
                        </tr>
                        </thead>
                        <tr v-for="(item,index) in childList">
                            <td>
                                <label v-show="!isEdit">{{item.item_no || '-'}}</label>
                                <el-input v-show="isEdit" v-model="item.item_no" class="item-input"></el-input>
                            </td>
                            <td>
                                <label v-show="!isEdit">{{item.article_no || '-'}}</label>
                                <el-input v-show="isEdit" v-model="item.article_no" class="article-input"></el-input>
                            </td>
                            <td>{{item.ext_part_no || '-'}}</td>
                            <td v-if="item.img">
                                <div class="td-img">
                                    <img :src="item.img | getImgUrl"/>
                                </div>
                            </td>
                            <td v-else> - </td>
                            <td>{{item.name || '-'}}</td>
                            <td>{{item.warranty || '-'}}</td>
                            <!--<td v-show="org.type == 4">{{item.price ? parseFloat(item.price)/100 : '-'}}</td>-->
                            <td>
                                <div class="btn-group-inline">
                                    <div class="icons">
                                        <div v-if="isEdit" class="icon-item delete" @click="deleteItem(index)" :title="$t('delete')"></div>
                                        <div class="icon-item eye" @click="goToItemDetail(item)" :title="$t('detail')"></div>
                                    </div>
                                </div>
                            </td>
                        </tr>
                    </table>

                    <!--选择零件添加弹窗-->
                    <el-dialog
                        class="dialogAddItem"
                        title="Add Item"
                        :visible.sync="dialogAddItemVisible"
                        size="large"
                        :before-close="closeChooseItem"
                        top="10%">
                        <div>
                            <table class="bom-list" border="0" cellspacing="0" cellpadding="0">
                                <thead>
                                <td>
                                    <el-checkbox v-model="allSelectedForAdd"></el-checkbox>
                                </td>
                                <td v-for="item in selectHead">{{item}}</td>
                                </thead>
                                <tbody>
                                <tr v-for="(item, index) in selectItems">
                                    <td>
                                        <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                                    </td>
                                    <td>{{item.ext_part_no}}</td>
                                    <td>
                                        <div class="td-img">
                                            <img :src="item.img | getImgUrl"/>
                                        </div>
                                    </td>
                                    <td>{{item.name}}</td>
                                </tr>
                                </tbody>
                            </table>
                        </div>
                        <div slot="footer" class="dialog-footer">
                            <el-button type="primary" @click="addItems">{{ $t('add') }}</el-button>
                        </div>
                    </el-dialog>
                </div>
            </div>
            <!--打印页面-->
            <div class="print-bom-content-wrap">
                <div class="content">
                    <div class="part top">
                        <label>{{ $t('articleNO') }} :</label>
                        <label class="art-no">{{artNo}}</label>
                        <div class="img-div">
                            <div v-if="!upload.photographSrc" class="no-img">
                                <span>{{ $t('noPhoto') }}</span>
                            </div>
                            <img v-else class="img" :src="upload.photographSrc | getImgUrl"/>
                        </div>
                    </div>
                    <div class="part list" v-if="childList.length > 0">
                        <label class="list-title">
                            {{ $t('list') }}
                        </label>
                        <table class="bom-list"  border="0" cellspacing="0" cellpadding="0" >
                            <thead>
                            <tr>
                                <td class="td head">{{$t('listHead.itemNo')}}</td>
                                <td class="td head">{{$t('listHead.partNo')}}</td>
                                <td class="td head">{{$t('listHead.partNo')}}<span>*</span></td>
                                <td class="td head">{{$t('listHead.picture')}}</td>
                                <td class="td head">{{$t('listHead.name')}}</td>
                                <td class="td head">{{$t('listHead.warranty')}}</td>
                                <td class="td head">{{$t('listHead.price')}}</td>
                                <td class="td head">{{$t('listHead.operation')}}</td>
                            </tr>
                            </thead>
                            <tr v-for="(item,index) in childList">
                                <td>
                                    <label v-show="!isEdit">{{item.item_no || '-'}}</label>
                                </td>
                                <td>{{item.article_no || '-'}}</td>
                                <td>{{item.ext_part_no || '-'}}</td>
                                <td v-if="item.img">
                                    <div class="td-img">
                                        <img :src="item.img | getImgUrl"/>
                                    </div>
                                </td>
                                <td v-else> - </td>
                                <td>{{item.name || '-'}}</td>
                                <td>{{item.warranty || '-'}}</td>
                                <td>{{item.price ?  parseFloat(item.price)/100 : '-'}}</td>
                            </tr>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <!--删除 BomList弹窗-->
        <el-dialog :title="$t('deleteProductDialog.title')" :visible.sync="bomListDeleteDialogVisible" size="tiny">
            <p>{{ $t('deleteProductDialog.content') }}</p>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="deleteBom">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="bomListDeleteDialogVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>

        <!--大图弹窗-->
        <el-dialog title="Larger-Image" :visible.sync="largeImgDialogVisible" size="large" class="large-image">
            <img :src="upload.photographSrc | getImgUrl" class="img"/><br/>
        </el-dialog>


        <el-dialog
            class="dialog"
            :title="$t('selectCategory')"
            :visible.sync="dialogSelectCategoryVisible"
            size="small">
            <div class="content" style="max-height: 500px; overflow: auto">
                <div class="category-tree">
                    <el-tree :data="categoryList" :props="defaultProps" @node-click="handleNodeClick"></el-tree>
                </div>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import printArea from 'printArea';

    export default{
        components: {},
        name: 'product-bom-list',
        data(){
            return {
                artNo: '',

                bomListDeleteDialogVisible: false,
                largeImgDialogVisible: false,
                isEdit: false,
                btnText: 'Edit',
                Core: Core,
                org: Core.Data.getOrg() ? Core.Data.getOrg() : '',
                partName: '',
                parentId: '',
                parentType: 2,
                upload: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    photographSrc: "",
                    data: {
                        token: Core.Data.getToken()
                    }
                },
                childList: [],
                hasBom: false,

                dialogAddItemVisible: false,
                selectItems: [],
                allChecked: false,
                childIdList: [],
                selectHead: ['ARTICLE NO.', 'PICTURE', 'NAME',],

                bomId: '',
                itemNoList: '',
                bomDetail: '',

                dialogSelectCategoryVisible: false,
                categoryList: [],
                defaultProps: {
                    id: 'id',
                    children: 'child',
                    label: 'name',
                },
                selectCategoryItem: {
                    id: '',
                    label: ''
                },

                authList: []
            }
        },
        i18n: {
            messages: {
                en: EN.Brand.VehicleBomList,
                zh: ZH.Brand.VehicleBomList,
                de: DE.Brand.VehicleBomList,
                nl: NL.Brand.VehicleBomList
            }
        },
        mounted: function () {
            // 国际化
            this.initData();

            // 获取权限
            this.authList = this.$store.state.userAuth;

            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });

            this.btnText = this.$t('edit');

            if (this.isEdit) {
                this.btnText = this.$t('save');
            }
            this.parentId = this.$route.query.id;
            this.artNo = this.$route.query.partNo;
            this.getBomDetail(this.parentId);
            this.getAllCategory();
        },

        methods: {
            initData() {
                this.selectHead = [this.$t('selectHead.partNo'), this.$t('selectHead.picture'), this.$t('selectHead.name')];
            },

            back(){
                if (this.isEdit && this.bomId) {
                    this.isEdit = false;
                    this.btnText = this.$t('edit');
                    this.getBomDetail(this.parentId);
                } else {
                    this.$router.back();
                }
            },

            getBomDetail(parentId){
                Core.Api.Item.getBomDetailByItem(parentId).then(res => {
                    this.bomDetail = res.bom;
                    this.bomId = res.bom.id;
                    this.upload.photographSrc = res.bom.img;
                    if (res.bom.child_list) {
                        this.childList = [];
                        this.getShowList(res.bom.child_list, this.childList);
                    }
                    console.log(this.childList, 111);
                    if (!this.bomId) {
                        this.isEdit = true;
                        this.btnText = this.$t('save');
                    }
                });
            },

            addBom(){
            },

            deleteBom(){
                Core.Api.Item.deleteBom(this.bomId).then(res => {
                    this.$message({
                        message: 'Delete Successfully',
                        type: 'success'
                    });
                    this.dialogDeleteVisible = false;
                    this.$router.back();
                })
            },

            editBom(){
                if (this.isEdit) {
                    this.getChildIdListAndItemNoList(this.childList);
                    console.log(this.childIdList.join(), '......');
                    let childIdList = this.childIdList.join();
                    let item_no = '';
                    if (this.itemNoList.length > 0) {
                        item_no = JSON.stringify(this.itemNoList);
                    }
                    Core.Api.Item.saveBom(this.bomId, this.parentId, this.parentType, childIdList, this.upload.photographSrc, item_no).then(res => {
                        console.log(res);
                        this.isEdit = false;
                        this.btnText = this.$t('edit');
                        this.getBomDetail(this.parentId);
                    });
                } else {
                    this.isEdit = true;
                    this.btnText = this.$t('save');
                    this.selectCategoryItem.id = '';
                }
            },

            printBom(){
                let print = "div.print-bom-content-wrap";

                let extraCss = "";
                let mode = "iframe";
                let close = false;
                let retainAttr = ['class', 'id', 'style', 'on'];
                let headElements = '<meta charset="utf-8" />,<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';

                let options = { mode : mode, popClose : close, extraCss : extraCss, retainAttr : retainAttr, extraHead : headElements };
                $(print).printArea(options);
            },

            exportBom() {
                let data = {
                    id: this.bomId
                };
                window.location.href = Core.Api.getUrl('item/bom-export', data);
            },

            showDeleteDialog(){
                this.bomListDeleteDialogVisible = true;
            },


            deleteItem(index){
                this.bomListDeleteDialogVisible = false;
                this.childList.splice(index, 1)
            },


            queryOrderItem() {
                let queryList = [];
                Core.Api.Item.queryComponentItem(this.selectCategoryItem.id, this.partName).then(res => {
                    queryList = res.query;
                    if (!queryList.length > 0) {
                        this.$message({
                            message: "There are no match condition data",
                            type: 'warning'
                        });
                        return;
                    }
                    this.getShowList(queryList, this.selectItems);
                    console.log(this.selectItems);
                    this.dialogAddItemVisible = true;

                })

            },

            imageUploaded(res) {
                console.log(res);
                this.upload.photographSrc = res.data.file.name;
            },

            addItems() {
                this.selectItems.forEach((item, index) => {
                    if (item.selected === true) {
                        let isAdded = false;
                        this.childList.forEach((childItem) => {
                            if (childItem.id === item.id) {
                                isAdded = true;
                            }
                        });
                        if (!isAdded) {
                            let addItem = this.selectItems[index];
                            this.childList.push({
                                id: addItem.id,
                                ext_part_no: addItem.ext_part_no,
                                name: addItem.name,
                                img: addItem.img,
                            });
                        }
                    }
                });
                this.closeChooseItem();
            },

            closeChooseItem() {
                this.selectItems = [];
                this.partName = '';
                this.dialogAddItemVisible = false;
            },

            getShowList(list, showList){
                list.forEach((item, index) => {
                    let needPart = {
                        id: item.id,
                        selected: false,
                        name: item.name,
                        img: item.img,
                        ext_part_no: item.ext_part_no,
                        item_no: item.item_no,
                        article_no: item.article_no
                    };
                    item.attr_list.forEach(attr => {
                        if (attr.attr_def_key == "warranty") {
                            needPart.warranty = attr.value
                        }

                        if (attr.attr_def_key == "price") {
                            needPart.price = attr.value
                        }
                    });
                    showList.push(needPart);
                });
            },

            getChildIdListAndItemNoList(list){
                let childIdList = [];
                let itemNoList = [];
                list.forEach((item, index) => {
                    childIdList.push(item.id);

                    itemNoList.push({
                        id: item.id,
                        item_no: item.item_no,
                        article_no: item.article_no,
                    });
                });

                this.childIdList = childIdList;
                this.itemNoList = itemNoList;
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

            getAllCategory() {
                Core.Api.Category.getCategoryListOfAll().then(
                    res => {
                        this.categoryList = res.category_list;
                    }
                );
            },

            goToItemDetail(item) {
                this.$router.push({name: 'brandModelDetail', query: {"modelId": item.id}});
            },

            clearCondition(){
                this.selectCategoryItem = '';
                this.partName = '';
            }
        },


        computed: {
            allSelectedForAdd: {
                get: function () {
                    return this.selectItems.reduce(function (prev, curr) {
                        return prev && curr.selected;
                    }, true);
                },
                set: function (newValue) {
                    this.selectItems.forEach((item) => {
                        item.selected = newValue;
                    });
                }
            },
            allSelectedForDelete: {
                get: function () {
                    return this.childList.reduce(function (prev, curr) {
                        return prev && curr.selected;
                    }, true);
                },
                set: function (newValue) {
                    this.childList.forEach((item) => {
                        item.selected = newValue;
                    });
                }
            }
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .model-bom-list {
        height: 100%;
        .content-holder {
            height: calc(100% - 60px);
            overflow: auto;
            .print-bom-content-wrap{
                display: none;
            }
            .content {
                width: 100%;
                .part {
                    margin: 10px auto;
                    width: 950px;
                    background-color: #353945;
                    padding: 20px 40px;
                }
                .top {
                    margin: 10px auto;
                    width: 950px;
                    background-color: #353945;
                    padding: 20px 40px;
                    .art-no {
                        display: inline-block;
                        width: 200px;
                        margin-left: 20px;
                        color: #A1A5B1;
                        line-height: 30px;
                    }
                    .img-div {
                        margin-top: 20px;
                        text-align: center;
                        position: relative;
                        .upload-icon {
                            width: 102px;
                            height: 102px;
                            margin: 15px;
                        }
                        .img {
                            max-width: 400px;
                            max-height: 300px;
                            margin: 15px;
                        }
                        .no-img {
                            padding: 60px;
                        }
                        .tip {
                            padding-bottom: 10px;
                            .title {
                                margin-right: 4px;
                                font-size: 12px;
                                color: #E26829;
                            }
                            .content {
                                font-size: 12px;
                                color: #E26829;
                            }
                        }
                    }
                }
                .list {
                    .list-title {
                        display: inline-block;
                        height: 30px;
                        line-height: 30px;
                        vertical-align: top;
                    }
                    .search-input {
                        width: 200px;
                        margin-left: 30px;
                    }
                    .bom-list {
                        margin-top: 15px;
                        width: 100%;
                        overflow: auto;
                        /*padding-bottom: 60px;*/
                        border: 1px solid #2B2F3B;;
                        thead td {
                            font-size: 12px;
                            padding: 10px;
                            text-align: center;
                            white-space: nowrap;
                            color: #A1A5B1;
                        }
                        tr {
                            height: 40px;
                            &:nth-child(2n+1) {
                                background: rgba(39, 43, 55, 0.20);
                                /*box-shadow: 1px 0 0 1px #2B2F3B;*/
                            }
                            td {
                                font-size: 12px;
                                padding: 0 5px;
                                text-align: center;
                                max-width: 150px;
                                overflow: hidden;
                                text-overflow: ellipsis;
                                & img {
                                    vertical-align: middle;
                                    width: 30px;
                                    height: 30px;
                                }
                                .item-input {
                                     width: 40px;
                                     .el-input__inner {
                                         text-align: center;
                                         height: 20px;
                                         vertical-align: middle;
                                     }
                                 }
                                .article-input {
                                    width: 110px;
                                    .el-input__inner {
                                        text-align: center;
                                        height: 20px;
                                        vertical-align: middle;
                                    }
                                }
                            }
                        }
                    }

                }

            }
        }
        .large-image {
            text-align: center;
            .el-dialog--large {
                width: auto;
            }
            .el-dialog__header {
                display: none;
            }
            .el-dialog__body {
                font-size: 0;
                width: auto;
                max-width: 1000px;
                padding: 0;
                background-color: white;
                .img {
                    max-width: 1000px;
                    height: auto;
                }
            }
        }
        .dialogAddItem {
            .el-dialog--large {
                width: 1100px;
            }
        }
        .td-img {
            margin: 0 auto;
            width: 30px;
            height: 30px;
            background-color: white;
        }
    }
</style>



// WEBPACK FOOTER //
// bom-list.vue?043a7c74