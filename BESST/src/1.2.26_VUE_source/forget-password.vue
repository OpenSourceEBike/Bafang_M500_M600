<template>
    <div class="forget">
        <nav-bar :title="title"></nav-bar>
        <div class="nav-btn">
            <el-button type="primary" icon="arrow-left" @click="$router.back()">{{ $t('login') }}</el-button>
        </div>
        <div class="main-warp">
            <div @keyup.13="sendCode">
                <el-input class="email" :placeholder="$t('email')" v-model="account.email">
                    <el-button type="primary" slot="append" :disabled="isSend" @click="checkUserBeforeSend">{{text}}</el-button>
                </el-input>
            </div>
            <div @keyup.13="resetPassword">
                <el-input class="input" :maxlength="4" :placeholder="$t('emailValidationCode')"
                          v-model="account.validationCode"></el-input>
                <br/>
                <el-input class="input" :placeholder="$t('password')" v-model="account.password" type="password"></el-input>
                <br/>
                <el-input class="input" :placeholder="$t('confirmPassword')" v-model="account.rePassword"
                          type="password"></el-input>
            </div>
            <el-button type="primary" class="next" @click="resetPassword">{{ $t('resetPassword') }}</el-button>
        </div>
    </div>
</template>

<script>
    import NavBar from "./layout/navbar.vue"
    import router from "../router/index"
    import Core from "core/core"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default{
        name: 'forgetPassword',
        components: {
            NavBar,
        },
        data(){
            return {
                title: 'BESST',
                isSend: false,
                text: "Verify",
                counter: "",
                account: {
                    email: "",
                    validationCode: "",
                    password: "",
                    rePassword: "",
                }
            }
        },
        i18n: {
            messages: {
                en: EN.ForgetPassword,
                zh: ZH.ForgetPassword,
                de: DE.ForgetPassword,
                nl: NL.ForgetPassword
            }
        },

        mounted () {
            this.text = this.$t('verify');
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.text = vm.$t('verify');
                }, 100);
            });
        },
        methods: {
            checkUserBeforeSend(){
                if (this.isSend) {
                    return;
                }
                if (!this.account.email) {
                    this.$message({
                        message: this.$t('emptyEmailTip'),
                        type: 'warning'
                    });
                    return;
                }
                if (!Core.Util.isEmail(this.account.email)) {
                    this.$message({
                        message: this.$t('errorEmailTip'),
                        type: 'warning'
                    });
                    return;
                }
                Core.Api.request({
                    url: '/user/user-by-email',
                    method: 'GET',
                    data: {
                        email: this.account.email.trim()
                    }
                }).then(res=>{
//                    console.log(res)
                    if(res.code === 5){
                        this.$message({
                            message: this.$t('filterUserEmail'),
                            type: 'warning'
                        });
                        return;
                    }else if(res.code === 0){
                        this.sendCode();
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },

            sendCode(){

                Core.Api.Common.sendEmailVCode(this.account.email).then(res => {

                    this.$message.success(this.$t('codeHasBeenSent'));

                    this.isSend = true;
                    this.text = "60";
                    let that = this;
                    this.counter = setInterval(() => {
                        if (that.text > 0) {
                            that.text -= 1;
                        } else {
                            that.text = that.$t('verify');
                            that.isSend = false;
                            clearInterval(that.counter);
                        }
                    }, 1000);
                });
            },


            resetPassword(){
                if (!this.checkParams()) {
                    return;
                }
                Core.Api.Common.checkEmailVCode(this.account.email, this.account.validationCode).then(
                    res => {
                        Core.Api.User.resetPassword(this.account.email, this.account.password, this.account.validationCode).then(res => {
                            this.$message({
                                message: this.$t('resetPasswordSuccess'),
                                type: 'success',
                            });
                            router.push({path: 'login'});
                        });
                    });

            },

            checkParams() {
                this.account.email = this.account.email.trim();
                if (!this.account.email) {
                    this.$message({
                        message: this.$t('emptyEmailTip'),
                        type: 'warning'
                    });
                    return false;
                }

                if (!this.account.validationCode) {
                    this.$message({
                        message: this.$t('emptyCodeTip'),
                        type: 'warning'
                    });
                    return false;
                }

                if (!this.account.password) {
                    this.$message({
                        message: this.$t('emptyPasswordTip'),
                        type: 'warning'
                    });
                    return false;
                }

                if (!this.account.rePassword) {
                    this.$message({
                        message:  this.$t('emptyConfirmPasswordTip'),
                        type: 'warning'
                    });
                    return false;
                }

                if (!Core.Util.verifyPassword(this.account.password)) {
                    this.$message({
                        message:  this.$t('passwordFormatTip'),
                        type: 'warning'
                    });
                    return false;
                }

                if (this.account.password !== this.account.rePassword) {
                    this.$message({
                        message:  this.$t('passwordTip'),
                        type: 'warning'
                    });
                    return false;
                }
                return true;
            },
        },
        beforeDestroy(){
            if(this.counter){
                clearInterval(this.counter);
            }
        }

    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .forget {
        .nav-btn {
            margin: 18px 34px;
        }
        .main-warp {
            text-align: center;
            position: absolute;
            left: 50%;
            transform: translate(-50%, 0);
            input {
                text-align: center;
            }
            .tip {
                margin-top: 16px;
                font-size: 12px;
                color: #858890;
            }
            .error {
                color: #EB2226;
            }
            .email {
                width: 310px;
                margin-top: 111px;
                & > input {
                    padding-left: 10px;
                }
            }
            .input {
                width: 310px;
                margin-top: 16px;
            }

        }
        .next {
            margin-top: 36px;
            width: 168px;
        }
    }
</style>



// WEBPACK FOOTER //
// forget-password.vue?e563dcc8