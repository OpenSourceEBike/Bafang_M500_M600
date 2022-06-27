<template>
    <div class="company">
        <nav-bar :title="title"></nav-bar>
        <div class="nav-btn">
            <el-button type="primary" icon="arrow-left" @click="goLogin">{{ $t('login') }}</el-button>
        </div>
        <div class="main-warp">
            <div class="head">
                <el-steps :space="145" :active="3">
                    <el-step :title="$t('verifyCode')"></el-step>
                    <el-step :title="$t('createAccount')"></el-step>
                    <el-step :title="$t('createCompany')"></el-step>
                </el-steps>
            </div>
            <p class="text">{{ $t('yourRole') }}</p>
            <div class="icons">
                <div class="icon1"></div>
                <div class="icon2"></div>
                <div class="icon-company"></div>
                <div class="icon4"></div>
                <div class="icon5"></div>
            </div>
            <p>{{typeText}}</p>
            <div class="main-content" @keyup.13="finish">
                <div class="photo">
                    <p>{{ $t('logo') }}</p>
                    <div class="photo-main">
                        <el-upload
                            class="btn btn-primary"
                            :action="upload.url"
                            :show-file-list="false"
                            :on-success="imageUploaded"
                            :data="upload.data">
                            <div v-if="!upload.photographSrc" class="upload-icon">
                                <div class="upload"></div>
                                <p class="illustration">
                                    {{ $t('uploadImgTip') }}
                                </p>
                            </div>
                            <img v-else class="upload-icon"
                                 :src="Core.Const.NET.IMG_URL_PREFIX + upload.photographSrc"/>
                        </el-upload>
                    </div>
                </div>
                <div class="contact">
                    <p>{{ $t('contact') }}</p>
                    <el-input class="company-name item" :placeholder="$t('companyName')" v-model="companyName"></el-input>
                    <span class="tip">*</span>
                    <el-input class="mail-address item" :placeholder="$t('mailAddress')" v-model="mailAddress" disabled></el-input>
                    <el-input class="phone-number" :placeholder="$t('phoneNumber')" v-model="phoneNumber">
                        <template slot="prepend">+{{areaCode}}</template>
                    </el-input>
                    <el-input class="website item" :placeholder="$t('website')" v-model="website"></el-input>
                </div>

                <div class="postal-address">
                    <p>{{ $t('postalAddress') }}</p>
                    <div class="street item">
                        <el-input class="" :placeholder=" $t('post')" v-model="postcode">
                            <template slot="append">No.</template>
                        </el-input>
                    </div>
                    <el-input class="address item" :placeholder="$t('address')" v-model="address"></el-input>
                    <span class="tip">*</span>
                    <el-select v-model="country" slot="append" class="land item" placeholder="Select the right country" filterable @change="showAreaCode">
                        <el-option
                            v-for="(item, index) in areaCodeList"
                            :key="index"
                            :label="item.area_en"
                            :value="item.area_en">
                        </el-option>
                    </el-select>
                    <span class="tip">*</span>
                </div>
            </div>
            <el-button class="finish" @click="finish">{{ $t('save') }}</el-button>
        </div>
        <!--确认公司信息的弹窗-->
        <el-dialog :visible.sync="dialogCompanyVisible"
                   custom-class="edit-dialog"
                   class="dialog-company">
            <div class="tip-item">
                <span class="tip">
                    <label>Tip:</label> <br/>
                    <label>Please confirm the country you selected which would not changed after saved. If your country is not exist in the option list, please contact besst@bafang-e.com. </label> <br/>
                </span>
            </div>
            <div class="clear-10"></div>
            <div class="company-item">
                <label class="title">{{ $t('companyName') }}</label>
                <div class="content">
                    <el-input v-model="companyName" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('mailAddress') }}</label>
                <div class="content">
                    <el-input v-model="mailAddress" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('phoneNumber') }}</label>
                <div class="content">
                    <el-input v-model="phoneNumber" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('website') }}</label>
                <div class="content">
                    <el-input v-model="website" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('country') }}</label>
                <div class="content">
                    <el-input v-model="country" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('address') }}</label>
                <div class="content">
                    <el-input v-model="address" disabled></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('post') }}</label>
                <div class="content">
                    <el-input v-model="postcode" disabled></el-input>
                </div>
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="updateCompany">{{ $t('save') }}</el-button>
                <el-button type="primary" @click="dialogCompanyVisible = false">Cancel</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import NavBar from "../layout/navbar.vue"
    import Core from "core/core";
    import VueCoreImageUpload from 'vue-core-image-upload'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import areaCodeInDB from 'core/areaCode'

    export default {
        name: 'company',
        components: {
            NavBar,
            VueCoreImageUpload
        },
        data() {
            return {
                // logo单图片上传
                upload: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    photographSrc: "",
                    data: {
                        token: Core.Data.getToken()
                    }
                },
                Core: Core,
                typeText: "",
                companyName: "",
                mailAddress: "",
                phoneNumber: "",
                address: "",
                postcode: "",
                website: "",
                avatar: "",
                country: '',
                areaCodeList: [],
                areaCode: '',
                countryCode: 0,

                type: '',
                title: 'BESST',

                user: "",
                dialogCompanyVisible: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Register.Company,
                zh: ZH.Register.Company,
                de: DE.Register.Company,
                nl: NL.Register.Company
            }
        },
        mounted: function () {
            this.areaCodeList = areaCodeInDB.init();
            // 国际化
            this.initData();
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
//                    this.initData();
                }, 100);
            });

            let type = Core.Data.getRoleEmailType(Core.Data.getUser().email);
            this.type = type;

            if (type === Core.Const.ORG_TYPE.DEALER || type === Core.Const.ORG_TYPE.SPECIAL_DEAlER) {
                this.typeText = 'Dealer';
            }

            if (type === Core.Const.ORG_TYPE.OEM) {
                this.typeText = 'OEM';
            }

            this.user = Core.Data.getUser();
            this.mailAddress = this.user.email;
        },
        methods: {
            showAreaCode(val) {
                console.log(val);
                this.areaCodeList.forEach((item)=>{
                    if(val === item.area_en){
                        this.areaCode = item.area_code;
                        this.countryCode = item.country_code || 0;
                    }
                })
            },
            initData() {
//                this.options = [
//                    {country: this.$t('america')},
//                    {country: this.$t('china')},
//                    {country: this.$t('germany')}
//                ];
//                this.country = this.$t('china');
            },
            //logo上传
            imageUploaded(res) {
//                console.log(res);
                this.upload.photographSrc = res.data.file && res.data.file.name ? res.data.file.name : '';
            },

            goLogin() {
                this.$router.push({name: "login"})
            },

            checkParams() {
                if (!this.companyName) {
                    this.$message({
                        message: "Please input company name",
                        type: "warning"
                    });
                    return false;
                }
                 if (!this.country) {
                     this.$message({
                         message: "Please select country",
                         type: "warning"
                     });
                     return false;
                 }
                if (!this.address) {
                    this.$message({
                        message: "Please input address",
                        type: "warning"
                    });
                    return false;
                }
//                if (!this.postcode) {
//                    this.$message({
//                        message: "Please input postcode",
//                        type: "warning"
//                    });
//                    return false;
//                }
//                if (!this.phoneNumber) {
//                    this.$message({
//                        message: "Please input phone number",
//                        type: "warning"
//                    });
//                    return false;
//                }
//                if (!this.website) {
//                    this.$message({
//                        message: "Please input website",
//                        type: "warning"
//                    });
//                    return false;
//                }
//                if (!this.avatar) {
//                    this.$message({
//                        message: "Please upload company logo",
//                        type: "warning"
//                    });
//                    return false;
//                }
                return true;

            },
            finish(){
                this.dialogCompanyVisible = true;
            },

            updateCompany() {
                this.avatar = this.upload.photographSrc;
                if (!this.checkParams()) {
                    return;
                }
//                let phone = `${this.areaCode} ${this.phoneNumber}`;
                Core.Api.Org.orgSave(0, this.type?this.type:2, this.companyName, this.mailAddress, this.country, this.address, this.postcode, this.phoneNumber, this.website, this.avatar, this.countryCode).then(res => {
                    this.$message({
                        message: 'Create company successfully',
                        type: 'success',
                    });
                    this.$router.push({name: 'login'});
                });
            },
        }

    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .company {
        .nav-btn {
            margin: 17px 33px;
        }
        .main-warp {
            text-align: center;
            position: absolute;
            left: 50%;
            transform: translate(-50%, 0);
            padding-bottom: 40px;
        }
        .head {
            margin-top: 71px;
            height: 80px;
            width: 431px;
            margin-left: auto;
            margin-right: auto;
        }
        .text {
            margin-top: 56px;
            color: #A1A5B1;
        }
        .icons {
            width: 740px;
            height: 80px;
            margin-top: 26px;
            margin-bottom: 16px;
            overflow: hidden;
            & > div {
                margin-top: 6px;
                float: left;
                width: 68px;
                height: 68px;
            }
            .icon1 {
                background: url("../../assets/images/register/icon1.png");
                background-size: cover;
            }
            .icon2 {
                margin-left: 88px;
                background: url("../../assets/images/register/icon2.png");
                background-size: cover;
            }
            .icon-company {
                width: 80px;
                height: 80px;
                margin: 0 88px;
                background: url("../../assets/images/register/icon-company.png");
                background-size: cover;
            }
            .icon4 {
                background: url("../../assets/images/register/icon4.png");
                background-size: cover;
            }
            .icon5 {
                margin-left: 86px;
                background: url("../../assets/images/register/icon5.png");
                background-size: cover;
            }
        }
        .main-content {
            margin-top: 40px;
            overflow: hidden;
            width: 740px;
            p {
                color: #A1A5B1;
            }
            & > div {
                float: left;
                text-align: left;
            }
            .photo {
                width: 218px;
                .photo-main {
                    margin-top: 16px;
                    background: #2B2F3B;
                    height: 176px;
                    overflow: hidden;
                    .upload-icon {
                        width: 218px;
                        height: 176px;
                        display: flex;
                        justify-content: space-between;
                        align-items: center;
                        padding: 0 10px;
                        .upload {
                            width: 44px;
                            height: 44px;
                            background: url("../../assets/images/register/btn-upload.png") center no-repeat;
                            background-size: cover;
                        }
                        .illustration {
                            color: #858890;
                            font-size: 10px;
                            width: 120px;
                            text-align: left;
                        }
                    }
                }
            }
            .contact {
                width: 220px;
                margin-left: 30px;
                font-size: 0;
                .item{
                    width: 210px;
                    display: inline-block;
                }
                .company-name {
                    margin-top: 14px;
                }
                .mail-address {
                    margin: 20px 0;
                }
                .phone-number {
                    width: 135px;
                    .el-select .el-input__inner {
                        width: 75px;
                    }
                    input {
                        font-size: 12px;
                        width: 135px;
                    }
                }
                .website {
                    margin-top: 20px;
                }
            }
            .postal-address {
                width: 220px;
                margin-left: 30px;
                .land {
                    width: 210px;
                }
                input {
                    font-size: 12px;
                }
                .street {
                    margin-top: 14px;
                    & > div {
                        color: #393D49;
                        font-size: 12px;
                    }
                }
                .address {
                    margin: 20px 0;
                }
                .item{
                    width: 210px;
                    display: inline-block;
                }
            }
            .tip {
                /*vertical-align: middle;*/
                padding-left: 6px;
                width: 2px;
                display: inline-block;
                color: #E26829;
            }
        }
        .dialog-company {
            overflow: auto;
            .el-dialog__body {
                font-size: 0;
            }
            .company-item {
                width: 50%;
                line-height: 30px;
                display: inline-block;
                .land {
                    width: 250px;
                }
                .title {
                    display: inline-block;
                    width: 80px;
                    line-height: 20px;
                    vertical-align: middle;
                }
                .content {
                    margin: 10px;
                    display: inline-block;
                    width: 250px;
                    .upload-icon {
                        max-width: 142px;
                        max-height: 142px;
                    }
                }
            }
            .tip-item{
                .tip{
                    font-size: 16px;
                    color: #E26829
                }
            }
        }
        .finish {
            font-size: 14px;
            width: 168px;
            margin-top: 46px;
        }
    }

</style>



// WEBPACK FOOTER //
// company.vue?1972e0ac