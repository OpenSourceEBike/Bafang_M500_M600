<template>
    <div class="brand-model-save">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <label class="title">{{title}}</label>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item edit" @click="dialogAttrVisible = true" :title="$t('editAttr')"></div>
                    <div class="icon-item save" @click="onClickSave" :title="$t('save')"></div>
                </div>
            </div>
        </div>
        <div class="content-wrap">
            <div class="content">
                <div class="content-box">
                    <div class="base-attr">
                        <div class="attr-item logo">
                            <label class="title">{{ $t('picture') }}</label>
                            <div class="title-desc">
                                <el-upload
                                    class="btn btn-primary block"
                                    :action=uploadUrl
                                    :show-file-list="false"
                                    :on-success="imageUploaded"
                                    :data="data" style="position: relative;">
                                    <img v-if="!photographSrc" class="upload-icon"
                                         src="../../../assets/images/icon/icon-upload@2x.png"/>
                                    <img v-else class="upload-icon" style="background: #fff;"
                                         :src="photographSrc | getImgUrl"/>
                                    <label class="tipOne" v-if="!photographSrc">PNG,JPG</label>
                                    <label class="tipTwo" v-if="!photographSrc">&lt;3M</label>
                                    <label class="tipThree" v-if="!photographSrc">300*300/400*400</label>
                                </el-upload>
                                
                            </div>
                        </div>
                        <div class="attr-item">
                            <label class="title">{{ $t('name') }}</label>
                            <div class="desc">
                                <el-input v-model="name"></el-input>
                            </div>
                        </div>
                        <div class="attr-item">
                            <label class="title">{{ $t('brand') }}</label>
                            <div class="desc">
                                <span>{{brandName}}</span>
                            </div>
                        </div>
                        <div class="attr-item">
                            <label class="title">{{ $t('partNo') }}</label>
                            <div class="desc">
                                <el-input v-model="partNo"></el-input>
                            </div>
                        </div>
                        <div class="attr-item description">
                            <label class="title">{{ $t('description') }}</label>
                            <div class="desc">
                                <el-input
                                    resize="none"
                                    type="textarea"
                                    :rows="5"
                                    placeholder=""
                                    v-model="desc">
                                </el-input>
                            </div>
                        </div>
                        <div class="attr-item">
                            <label class="title">{{ $t('link') }}</label>
                            <div class="desc">
                                <el-input
                                    resize="none"
                                    type="textarea"
                                    :rows="1"
                                    placeholder=""
                                    v-model="website">
                                </el-input>
                            </div>
                        </div>
                    </div>

                    <!--商品属性列表-->
                    <div class="upper-attr" v-for="(item, index) in attrList">
                        <item-attr-input-group
                            :item="item"
                            @input-value-return="itemAttrInputValueReturn"
                            @file-upload-return="itemAttrFileUploadReturn"
                            @file-upload-remove="itemAttrFileUploadRemove"
                        >
                            {{item.name}}
                        </item-attr-input-group>
                    </div>

                </div>
            </div>
        </div>

        <el-dialog :title="$t('attrList')" :visible.sync="dialogAttrVisible">

            <el-checkbox-group
                v-model="showAttrList">
                <el-checkbox v-for="attr in totalAttrList" :label="attr.id" :key="attr.id">{{attr.name}}</el-checkbox>
            </el-checkbox-group>

            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="getUpperAttrDefList">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogAttrVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

    </div>
</template>

<script>
    import Core from 'core/core'
    import VueCoreImageUpload from 'vue-core-image-upload'
    import ItemAttrInputGroup from '../../../components/item-attr-input-group.vue'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        components: {
            VueCoreImageUpload,
            ItemAttrInputGroup
        },
        data() {
            return {
                Core: Core,
                // logo单图片上传
                uploadUrl: Core.Const.NET.IMG_UPLOAD_END_POINT,
                photographSrc: "",
                name: '',
                title: 'Add',
                partNo: '',
                desc: '',
                website: '',
                logoName: '',
                brandId: '',
                brandName:'',
                attrList: [],
                fileList: [],
                valueList: [],
                isSaving: false,
                textList: [],
                selectList: [],
                itemId: '',
                data: {
                    token: Core.Data.getToken(),
                },
                categoryId: 1,
                operationType: 'add',
                totalAttrList: [{id: 21, name: this.$t('specification')},
                    {id: 24, name: this.$t('currency')},
                    {id: 1, name: this.$t('year')},
                    {id: 2, name: this.$t('color')},
                    {id: 3, name: this.$t('size')},
                    {id: 4, name: this.$t('gender')},
                    {id: 5, name: this.$t('price')}],
                dialogAttrVisible: false,
                showAttrList: [1, 2, 3, 4, 5, 21, 24],
            }
        },
        i18n: {
            messages: {
                en: EN.Brand.ModelSave,
                zh: ZH.Brand.ModelSave,
                de: DE.Brand.ModelSave,
                nl: NL.Brand.ModelSave
            }
        },
        mounted: function () {
            // 国际化
//            this.title = this.$t('add');
//            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
//                setTimeout(() => {
//                    this.title = this.$t('add');
//                }, 100);
//            });
            this.brandId = this.$route.query.brandId;
            this.itemId = this.$route.query.itemId ? this.$route.query.itemId : 0;
            this.operationType = this.$route.query.type;
            this.init();

        },

        methods: {


            //获取品牌名称
            getBrandName(){
                let that = this;
                Core.Api.Brand.getAllBrandList().then(res => {
                    if (!res.brand_list || res.brand_list.length <= 0) {
                        return;
                    }
                    let brandList = res.brand_list.filter(item =>item.id == that.$route.query.brandId);
                    this.brandName = brandList[0].name;
                })
            },

            // edit 进来，获取相应数据
            getItemDetail() {
                Core.Api.Item.getItemDetail(this.itemId).then(res => {
                    console.log('getItemDetail');
                    console.log(res);
                    let vm = this;
                    let attrValueList = res.item.attr_list;
                    let item = res.item;
                    this.photographSrc = item.img;
                    this.brandId = item.brand_id;
                    this.name = item.name;
                    this.desc = item.desc;
                    this.website = item.website;
                    this.partNo = item.ext_part_no;

                    function getValueOfAttrId(attrId) {
                        for (let i in attrValueList) {
                            let attrValue = attrValueList[i];
                            let mAttrId = attrValue['attr_def_id'];
                            if (mAttrId === attrId) {
                                return attrValue['value']
                            }
                        }

                        return '';
                    }

                    this.attrList.forEach((attr, i) => {
                        let attrId = attr['id'];
                        let value = getValueOfAttrId(attrId);
                        attr.v = value;
                        if (attr.key === 'price') {
                            value = value ? value/100 : 0;
                        }
                        attr.value = value;
                        attr.extra = {
                            'inputType': attr['input_type'],
                            'value': value
                        };
                        this.attrList[i] = attr;
                    });

                    // console.log(this.attrList);


                });
            },

            // 获取属性列表
            getUpperAttrDefList() {
                this.attrList = [];
                this.fileList = [];
                this.valueList = [];
                Core.Api.Category.getUpperAttrDefList(this.categoryId).then(res => {
                    let attrList = res.attr_list.filter((item) => {
                        return this.showAttrList.indexOf(item.id) !== -1
                    });
                    attrList.forEach(function (item, index) {
                        attrList[index].options = {
                            url: Core.Const.NET.FILE_UPLOAD_END_POINT,
                            params: {
                                'attr_def_id': item.id,
                                'token': Core.Data.getToken()
                            }
                        };
                        attrList[index]['v'] = '';
                        attrList[index]['extra'] = '';
                    });
                    this.attrList = attrList;
                    console.log(this.attrList,11111111);

                    if (this.itemId) {
                        this.getItemDetail();
                    }
                    this.dialogAttrVisible = false;
                });
            },

            //单图片
            imageUploaded(res) {
                this.photographSrc = res.data.file.name;
            },

            //获取传入接口的JSON
            getAttrListJson() {
                if (this.showAttrList.length <= 0) {
                    return '';
                }

                let $this = this;
                this.attrList.forEach(function (attr, index) {
                    let attrValue = [];
                    let attrKey = attr['key'];
                    if ($this.textList[attrKey]) {
                        if (attrKey === 'price') {
                            let price = $this.textList[attrKey]['value'];
                            price = price ? parseInt(parseFloat(price) * 100) : 0;
                            console.log('price', price)
                            $this.textList[attrKey]['value'] = price;
                        }
                        $this.valueList.push({'attr_def_id': attr.id, 'value': $this.textList[attrKey]['value']});
                        return;
                    }
                    $this.fileList.forEach(function (item) {
                        if (attr.id === item.attr_id) {
                            attrValue.push(item.file_id);
                        }
                    });
                    $this.valueList.push({'attr_def_id': attr.id, 'value': attrValue.join()});
                });
                console.log(JSON.stringify($this.valueList));
                return JSON.stringify($this.valueList);
            },

            checkParams() {
                if (!this.photographSrc) {
                    this.$message({
                        message: this.$t('emptyTip.picture'),
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.name) {
                    this.$message({
                        message: this.$t('emptyTip.name'),
                        type: 'warning'
                    });
                    return false;
                }
//                if (!this.partNo) {
//                    this.$message({
//                        message: this.$t('emptyTip.partNo'),
//                        type: 'warning'
//                    });
//                    return false;
//                }
//                if (!this.desc) {
//                    this.$message({
//                        message: this.$t('emptyTip.desc'),
//                        type: 'warning'
//                    });
//                    return false;
//                }
                return true;
            },

            onClickSave() {
                if (!this.checkParams()) {
                    return;
                }
                if (this.isSaving) {
                    this.$message({
                        message: this.$t('saveTip'),
                        type: 'warning'
                    });
                    return;
                }

                let itemId = this.itemId;
                let parentId = 0;

                if (this.operationType === "copy") {
                    parentId = itemId;
                    itemId = 0;
                }

                this.isSaving = true;
                let categoryId = this.categoryId;
                let brandId = this.brandId;
                let Photograph = this.photographSrc;
                let name = this.name;
                let partNo = this.partNo;
                let desc = this.desc;
                let website = this.website;
                let attrJson = this.getAttrListJson();
                if (!brandId) {
                    this.$message({
                        message: this.$t('brandWrong'),
                        type: 'warning'
                    });
                    return;
                }
                //  执行saveItem

                if (this.operationType === "copy") {
                    Core.Api.Item.copyItem(categoryId, parentId, brandId, name, partNo, Photograph, desc, attrJson)
                        .then(res => {
                            this.isSaving = false;
                            this.$message({
                                message: this.$t('saveSuccess'),
                                type: 'success'
                            });
                            this.$router.go(-2);
                        }).catch(err => {
                            this.isSaving = false;
                        }
                    );
                } else {
                    Core.Api.Item.saveItem(itemId, categoryId, parentId, brandId, name, partNo, Photograph, desc, attrJson, '', website)
                        .then(res => {
                            this.isSaving = false;
                            this.$message({
                                message: this.$t('saveSuccess'),
                                type: 'success'
                            });
                            this.$router.back();
                        }).catch(err => {
                            this.isSaving = false;
                        }
                    );
                }

                this.valueList = [];
            },

            itemAttrInputValueReturn(data) {
                console.log('return data ', data);
                this.textList[data['key']] = data;

            },
            itemAttrFileUploadReturn(data) {
                console.log('return file data', data);

                this.fileList.push(data);
                console.log("file list", this.fileList);
            },

            itemAttrFileUploadRemove(file) {
                this.fileList.forEach((item, index) => {
                    if (file.id === item.file_id) {
                        this.fileList.splice(index, 1);
                    }
                });
                console.log("file list", this.fileList);
            },

            init() {
                this.title = this.$t('add');
                if (this.operationType === "copy") {
                    this.title = this.$t('copy');
                } else if (this.operationType === "edit") {
                    this.title = this.$t('edit');
                }

                let $this = this;
                this.getBrandName();
                if (!$this.itemId) {
                    this.getUpperAttrDefList();
                    return;
                }
                Core.Api.Item.getItemDetail(this.itemId).then(res => {
                    let attrValueList = res.item.attr_list;
//                    console.log(attrValueList, 11111);
                    $this.showAttrList = [];
                    attrValueList.forEach((item, index) => {
                        $this.showAttrList.push(item.attr_def_id);
                    });
                    this.getUpperAttrDefList();
                });
            },
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
    .brand-model-save {
        height: 100%;
        .content {
            .content-box {
                .base-attr {
                    margin-top: 20px;
                    .attr-item {
                        height: 50px;
                        line-height: 50px;
                        font-size: 0;
                        overflow: hidden;
                        .title {
                            position: relative;
                            display: inline-block;
                            padding: 0 20px;
                            width: 165px;
                            font-size: 12px;
                            color: #fff;
                            font-weight: 500;
                        }
                        .title-desc {
                            display: inline-block;
                            vertical-align: top;
                            width: calc(100% - 165px);
                            

                            .g-core-image-upload-btn {
                                width: 50px;
                                height: 50px;
                            }
                            .upload-icon {
                                width: 142px;
                                height: 142px;
                            }
                            .tipOne{
                                position: absolute;
                                top:18px;
                                left:38px;
                            }
                            .tipTwo{
                                position: absolute;
                                top:38px;
                                left:100px;
                                font-size:12px;
                            }
                            .tipThree{
                                position: absolute;
                                top:106px;
                                left:17px;
                            }
                               
                            .block  { 
                                display: block;
                            }
                        }
                        .desc {
                            display: inline-block;
                            vertical-align: top;
                            width: 638px;
                            height: 50px;
                            .el-input {
                                width: 310px;
                            }
                        }
                        &.description {
                            height: 150px;
                            padding: 15px 0;
                            .title {
                                position: relative;
                                top: -15px;
                            }
                        }
                    }
                    .logo {
                        height: 182px;
                        line-height: 20px;
                    }
                    .btn {
                        display: inline-block;
                    }

                }

                .save-btn {
                    margin: 22px 40px;
                    width: 116px;
                }
            }
        }
        .el-checkbox-group .el-checkbox {
            margin: 4px 0;
            display: inline-block;
            height: 22px;
            width: 50%;
            padding-left: 10px;
        }
    }
</style>



// WEBPACK FOOTER //
// save.vue?7b3034ab