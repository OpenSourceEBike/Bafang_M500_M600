<template>
    <div class="account">
        <nav-bar :title="title"></nav-bar>
        <div class="nav-btn">
            <el-button type="primary" icon="arrow-left" @click="goLogin">{{ $t('login') }}</el-button>
        </div>
        <div class="main-warp">
            <div class="head">
                <el-steps :space="145" :active="2">
                    <el-step :title="$t('verifyCode')"></el-step>
                    <el-step :title="$t('createAccount')"></el-step>
                    <el-step :title="$t('createCompany')"></el-step>
                </el-steps>
            </div>
            <div @keyup.13="sendCode">
                <el-input class="email" :placeholder="$t('email')" v-model="account.email" :disabled="emailDisabled">
                    <el-button slot="append" :disabled="isSend" @click="sendCode">{{text}}</el-button>
                </el-input>
            </div>
            <div>
                <el-input class="input" :placeholder="$t('company')" v-model="companyName" v-if="isEmail"></el-input>
            </div>
            <div v-show="show" class="tip" :class="{error: isError}"><p>{{message}}</p></div>
            <div @keyup.13="goCompany">
                <el-input class="input" :maxlength="4" :placeholder="$t('emailValidationCode')"
                          v-model="account.validationCode"></el-input>
                <br/>
                <el-input class="input" :placeholder="$t('password')" v-model="account.password" type="password"></el-input>
                <br/>
                <el-input class="input" :placeholder="$t('confirmPassword')" v-model="account.rePassword"
                          type="password"></el-input>
            </div>
            <el-button class="next" @click="goCompany" type="primary">{{ $t('next') }}</el-button>
        </div>
    </div>
</template>

<script>
    import NavBar from "../layout/navbar.vue"
    import Core from "core/core"
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default{
        name: 'account',
        components: {
            NavBar,
        },
        data(){
            return {
                companyName: '',
                show: false,
                message: "",
                isError: false,
                isSend: false,
                text: "Verify",
                counter: "",
                isEmail: Core.Data.get('email')===null,
                account: {
                    email: Core.Data.get('email'),
                    validationCode: "",
                    password: "",
                    rePassword: "",
                },
                title: 'BESST',
                emailDisabled: false
            }
        },
        i18n: {
            messages: {
                en: EN.Register.Account,
                zh: ZH.Register.Account,
                de: DE.Register.Account,
                nl: NL.Register.Account
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
            this.emailDisabled = this.account.email!=null?true:false;
        },
        methods: {
            goLogin(){
                this.$router.push({name: "login"})
            },

            sendCode(){
                if (this.isSend) {
                    return;
                }
                this.message = "";
                this.isError = false;
                this.show = false;
                Core.Api.Common.sendEmailVCode(this.account.email.replace(/\s+/g,"")).then(res => {
                    this.$message({
                        message: 'Code has been sent ',
                        type: 'success'
                    });
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
            checkParams(){
            	if(!this.account.validationCode){
                    this.$message({
                        message: 'Please input validation code',
                        type: 'warning'
                    });
                    return false;
                }
                if(!this.account.password){
                    this.$message({
                        message: 'Please input password',
                        type: 'warning'
                    });
                    return false;
                }
                if (!Core.Util.verifyPassword(this.account.password)) {
                    this.$message({
                        message:  "Password must be 8-16 characters, only A-Za-z0-9, case-sensitive.",
                        type: 'warning'
                    });
                    return false;
                }
                if(!this.account.rePassword){
                    this.$message({
                        message: 'Please input confirm password',
                        type: 'warning'
                    });
                    return false;
                }
                if (this.account.password !== this.account.rePassword) {
                    this.$message({
                        message: 'Two passwords are not the same',
                        type: 'warning'
                    });
                    return false;
                }
                return true;
            },

            goCompany(){
                if(!this.checkParams()){
                	return;
                }
                    Core.Api.User.register(this.account.email, this.account.password, this.account.validationCode,this.companyName).then(res => {
                        this.$message({
                            message: 'Create account successfully',
                            type: 'success',
                        });
                        Core.Data.setUser(res.user);
                        Core.Data.setToken(res.token);
                        this.$router.push({path: 'company'});
                    });
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
    .account {
        .nav-btn {
            margin: 18px 34px;
        }
        .main-warp {
            text-align: center;
            position: absolute;
            left: 50%;
            transform: translate(-50%, 0);
            .head {
                margin-top: 70px;
                height: 80px;
                width: 432px;
                margin-left: auto;
            }
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
// account.vue?b73ad5fc