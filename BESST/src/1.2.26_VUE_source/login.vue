<template>
    <div class="login">
        <nav-bar :title="title"></nav-bar>
        <div class="chooseLanguage">
            <div  class="languageList">
                <el-tabs v-model="language" @tab-click="handleClick()">
                    <el-tab-pane v-for="item in languages" :name="item.value" :key="item.value" :label="item.label" :value="item.value"></el-tab-pane>
                </el-tabs>
                <div class="clear"></div>
            </div>
        </div>
        <div class="main-container">
            <div class="declare">
                <h2>{{ $t('declare') }}</h2>
                <p>{{$t('declareP1')}}</p>
                <p class="bodytext">{{$t('declareP2')}}</p>
                <el-button type="primary" icon="arrow-right" class="showMore" @click="dialogVisible=!dialogVisible">{{ $t('readmore') }}</el-button>
            </div>
            <div class="main-warp" @keyup.13="login">
                <div class="head">
                    <img class="icon-login" src="../assets/images/login/icon-login.png" v-if="!org">
                    <div v-else class="icon-login">
                        <img class="logo-img" :src="org.avatar | getImgUrl">
                    </div>
                </div>
                <p class="login-text">{{ $t('login') }}</p>
                <el-input type="text" class="username" :placeholder="$t('userNamePlaceholder')"
                          v-model="userName"></el-input>
                <el-input type="password" class="password" :placeholder="$t('passwordPlaceholder')"
                          v-model="password"></el-input>
                <div class="check-div">
                    <div class="check-item" @click="isCheck">
                        <div class="icon-check" v-if="check"></div>
                    </div>
                    <div class="remember-text">{{ $t('rememberMe') }}</div>
                    <p class="forget" @click="toForget">{{ $t('forgetPassword') }}</p>
                </div>
                <el-button class="login-btn" @click="login" type="primary">{{ $t('loginBtnTxt') }}</el-button>
                <br>
                <el-button class="sign" @click="toSign" type="delete">{{ $t('signUp') }}</el-button>
                <br>
                <el-button class="off-line-btn" @click="offLine" type="delete" v-show="offlineBtn">{{ $t('offLine') }}</el-button>
            </div>
        </div>
        <el-dialog :visible.sync="dialogVisible" class="dialog" custom-class="dialogInfo">
            <div>
                <div class="flex">
                    <div class="item">
                        <h2>{{$t('declareP3')}}</h2>
                        <div class="overflow">
                            <div class="declareInfo">
                                <P><span></span>{{$t('declareP1')}}</p>
                                <div class="special">
                                    <p><span></span>{{$t('declareP2')}}</p><img src="../assets/images/login/besst-tool.png" title="BESST TOOL">
                                </div>
                                <br>
                                <h3>{{$t('declareP6')}}</h3>
                                <P><span>{{$t('declareP7')}}</span></p>
                                <P><span>{{$t('declareP8')}}</span></p>
                                <P><span>{{$t('declareP9')}}</span></p>
                                <P><span>{{$t('declareP10')}}</span></p>
                                <br>
                                <P><span>{{$t('declareP11')}}</span></p>
                                <P><span>{{$t('declareP12')}}</span></p>
                                <P><span>{{$t('declareP13')}}</span></p>
                                <P><span>{{$t('declareP14')}}</span></p>
                                <P><span>{{$t('declareP15')}}</span></p>
                                <P><span>{{$t('declareP16')}}</span></p>
                                <P><span>{{$t('declareP17')}}</span></p>
                                <P><span></span>
                                    <a class="btn-underline" href="https://www.bafang-e.com/en/oem-area/service/besst/" target="_blank">
                                        https://www.bafang-e.com/en/oem-area/service/besst/</a>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="item">
                        <h2>{{$t('declareP29')}}</h2>
                        <div class="overflow">
                            <div class="declareInfo">
                                <h3>{{$t('declareP30')}}</h3>
                                <P><span>{{$t('declareP31')}}</span></p>
                                <P><span>{{$t('declareP21')}}</span></p>
                                <P><span>{{$t('declareP22')}}</span></p>
                                <P><span>{{$t('declareP23')}}</span></p>
                                <P><span>{{$t('declareP32')}}</span></p>
                                <br>
                                <!--<P><span>{{$t('declareP25')}}</span></p>-->
                                <P><span>{{$t('declareP11')}}</span></p>
                                <P><span>{{$t('declareP33')}}</span></p>
                                <P><span>{{$t('declareP34')}}</span></p>
                                <P><span>{{$t('declareP16')}}</span></p>
                                <P><span></span>{{$t('declareP17')}}
                                </p>
                                <P><span></span>
                                    <a class="btn-underline" href="https://www.bafang-e.com/en/oem-area/service/besst/" target="_blank">
                                        https://www.bafang-e.com/en/oem-area/service/besst/</a>
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="item last">
                        <h2>{{$t('declareP18')}}</h2>
                        <div class="overflow">
                            <div class="declareInfo">
                                <h3>{{$t('declareP19')}}</h3>
                                <P><span>{{$t('declareP20')}}</span></p>
                                <P><span>{{$t('declareP21')}}</span></p>
                                <P><span>{{$t('declareP22')}}</span></p>
                                <P><span>{{$t('declareP23')}}</span></p>
                                <P><span>{{$t('declareP24')}}</span></p>
                                <br>
                                <!--<P><span>{{$t('declareP25')}}</span></p>-->
                                <P><span>{{$t('declareP11')}}</span></p>
                                <P><span>{{$t('declareP26')}}</span></p>
                                <P><span>{{$t('declareP27')}}</span></p>
                                <P><span>{{$t('declareP28')}}</span></p>
                                <P><span>{{$t('declareP16')}}</span></p>
                                <P><span></span>{{$t('declareP17')}}
                                </p>
                                <P><span></span>
                                    <a class="btn-underline" href="https://www.bafang-e.com/en/oem-area/service/besst/" target="_blank">
                                        https://www.bafang-e.com/en/oem-area/service/besst/</a>
                                </p>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </el-dialog>
    </div>
</template>
<script>
    import NavBar from "./layout/navbar.vue";
    import router from "../router/index";
    import Core from "core/core";
    import ZH from "src/assets/lang/zh";
    import EN from "src/assets/lang/en";
    import DE from "src/assets/lang/de";
    import NL from "src/assets/lang/nl";
    export default {
        name: "login",
        components: {
            NavBar
        },
        data() {
            return {
                title: "BESST",
                userName: Core.Data.get("userName") || "",
                password: "",
                check: false,
                org: Core.Data.getOrg() || "",
                orgTypeNum: null,
                language: "en",
                offlineBtn: false,
                languages: [
                    {
                        value: "zh",
                        label: "中文",
                        icon: "../../static/icon/Chinese.png"
                    },
                    {
                        value: "de",
                        label: "Deutsch",
                        icon: "../../static/icon/German.png"
                    },
                    {
                        value: "en",
                        label: "English",
                        icon: "../../static/icon/English.png"
                    },
                    {
                        value: "nl",
                        label: "Nederlands",
                        icon: "../../static/icon/Dutch.png"
                    }
                ],
                authList: [],
                dialogVisible:false,
                firstHtml:  '',
                ip: "",
                ipCount: 0,
                ipCountCheck: false,

                myDB: {db: ''},
                checkDB: false,
            }
        },

        i18n: {
            messages: {
                en: EN.Login,
                zh: ZH.Login,
                de: DE.Login,
                nl: NL.Login
            }
        },
        mounted() {
            this.language = Core.Data.getLang() || "en";

//            console.log( "md5" );  //
//            console.log( Core.Util.calculate_md5('12345678') );  //
//            console.log( Core.Util.checkEncryption('12345678') );  //
            this.offlineCheck();
//            this.userName = this.getCookie("userBESST");
            this.checkPassword();

        },
        methods: {
            checkPassword() {
                var indexedDB = window.indexedDB || window.webkitIndexedDB || window.mozIndexedDB || window.msIndexedDB;
                if(!indexedDB){
                    console.log("Your browser don't support IndexedDB");
                    return
                }

                var request = indexedDB.open('bs', 2);

                request.onerror = function(e) {
                    console.log(e.currentTarget.error.message);
                };

                let vm = this;
                request.onsuccess = function(e) {
                    vm.myDB.db = e.target.result;
//                    console.log('成功打开DB');
                    vm.read();
                };

                request.onupgradeneeded = function(e) {
                    vm.myDB.db = e.target.result;
                    if (!vm.myDB.db.objectStoreNames.contains('person')) {
//                        console.log("我需要创建一个新的存储对象");
                        //如果表格不存在，创建一个新的表格（keyPath，主键 ； autoIncrement,是否自增），会返回一个对象（objectStore）
                        var objectStore = vm.myDB.db.createObjectStore('person', {
                            keyPath: "id",
                            autoIncrement: true
                        });

                        //指定可以被索引的字段，unique字段是否唯一
                        objectStore.createIndex("name", "name", {
                            unique: false
                        });

                        objectStore.createIndex("phone", "phone", {
                            unique: true
                        });
                    }
                };

            },

            add(p){
                var transaction = this.myDB.db.transaction(['person'], 'readwrite');
                var store = transaction.objectStore('person');
//                console.log(store);

                var requestData = store.add({id: 1, name: 'ps', phone: p});
                let vm = this;

                requestData.onsuccess = function(e) {
//                    console.log(e);
                };

                requestData.onerror = function (e) {
                    console.log('数据写入失败',e);
                }
            },

            read() {
                var transaction = this.myDB.db.transaction(['person']);
                var objectStore = transaction.objectStore('person');
                var request = objectStore.get(1);

                request.onerror = function(event) {
                    console.log('事务失败');
                };
                let vm = this;

                request.onsuccess = function(event) {
                    if (request.result) {
//                        console.log('Name: ' + request.result.name);
//                        console.log('Phone: ' + request.result.phone);
                        vm.check = request.result.phone;
                        vm.checkDB = true;
                        vm.password = Core.Util.decrypt(request.result.phone)
                    } else {
                        console.log('未获得数据记录');
                        vm.check = false;
                        vm.checkDB = false;
                        vm.password = '';
                    }
                };
            },

            update(p) {
                var request = this.myDB.db.transaction(['person'], 'readwrite')
                    .objectStore('person')
                    .put({ id: 1, name: 'ps', phone: p});

                request.onsuccess = function (event) {
                    console.log('数据更新成功');
                };

                request.onerror = function (event) {
                    console.log('数据更新失败');
                }
            },

            offlineCheck(){

                let findOfflineOrg = Core.Const.OFFLINE_ORG.indexOf(this.org.id)>-1;

                let user = Core.Data.getUser();
                if(user && user.ip_type === 1){
//                    console.log(user);
                    this.initIp();
                    let findOfflineUser = Core.Data.get('accountType').length>0 && Core.Data.get('accountType').indexOf(6)>-1;
                    if (findOfflineOrg || findOfflineUser) {
                        this.offlineBtn = true;
                    }
                }else{
                    if (findOfflineOrg) {
                        this.offlineBtn = true;
                    }
                }
            },

            initIp() {
                let that = this;
                // 方法1
                // var script = document.createElement('script');
                // script.type = 'text/javascript';

                // // 传参并指定回调执行函数为onBack
                // script.src = 'http://pv.sohu.com/cityjson?ie=utf-8&&callback=onBack';
                // document.head.appendChild(script);

                // 回调执行函数
//                window['onBack'] = (res) => {
//                    console.log("this", this ,res );
//                }

                // 方法2
                $.ajax({
                    url: 'http://pv.sohu.com/cityjson?ie=utf-8',
                    dataType: "script",
                    method: 'GET',
                }).done(function () {
                    that.ip = returnCitySN.cip;
                    if(that.ipCountCheck){
                        that.login();
                        that.ipCountCheck = false;
                    }
//                    console.log(this.ip);
//                    console.log(returnCitySN);//440306
                });

            },
            handleClick(tab, event) {
//                console.log(tab, event);
//                this.language = this.value;
                Core.Data.setLang(this.language);
//                window.location.reload();
                this.$root.eventHub.$emit(Core.Const.DATA.KEY_I18N);
            },

            toSign() {
                router.push({ path: "register/invitation" });
            },
            toForget() {
                router.push({ path: "forget-password" });
            },
            isCheck() {
                if (this.check) {
                    this.check = false;
                } else {
                    this.check = true;
                }
            },

            login() {
                if(!navigator.onLine){
                    this.$message({
                        message: "Please check internet. Now you are offline",
                        type: "warning"
                    });
                    return;
                }
                Core.Data.setLang(this.language);

//                console.log(this.ip);
                // if(window.returnCitySN !== undefined || this.ip !== ''){
                //     this.ip = window.returnCitySN.ip;
                // }else{
                //     let vm = this;
                //     this.initIp();
                //     setTimeout(()=>{
                //         vm.login();
                //     },1000)
                // }

                const scope = this;
                this.userName = this.userName.trim();
                if (!this.userName) {
                    this.$message({
                        message: "Please input username",
                        type: "warning"
                    });
                    return;
                }
                if (!this.password) {
                    this.$message({
                        message: "Please input password",
                        type: "warning"
                    });
                    return;
                }
                Core.Api.User.login(this.userName, this.password.trim(), this.ip)
                    .then(res => {
                        if(Core.Data.get("password")){
                            localStorage.clear();
                        }
                        if(res.code === '21'){
                            this.$message({
                                type: 'warning',
                                message: scope.$t('limitedIP'),
                            })
                            return;
                        }
                        if(res.code === '22'){
                            this.ipCount++;
                            if(this.ipCount > 3){
                                this.ipCountCheck = false;
                                this.$message({
                                    type: 'error',
                                    message: scope.$t('emptyIPThree'),
                                })
                                return;
                            }else{
                                this.ipCountCheck = true;
                                this.initIp();
                                this.$message({
                                    type: 'warning',
                                    message: scope.$t('emptyIP'),
                                })
                                return;
                            }
                        }
                        if(res.account_type && res.account_type.length>0){
                            Core.Data.set("accountType", res.account_type);
                        }else {
                            Core.Data.set("accountType", []);
                        }
                        //console.log(res);
                        Core.Data.setOffline(false);

                        let org = res.user.org;
                        Core.Data.setUser(res.user);
                        Core.Data.setToken(res.token);

                        if (org === null) {
                            this.$router.push({ name: "invitation", query: { haveUser: 1 } });
                            return;
                        }

                        Core.Data.setOrg(res.user.org);
                        this.$store.commit("userAccount", this.userName.toLowerCase());

                        if (this.check) {
                            Core.Data.set("userName", this.userName.toLowerCase());
//                            Core.Data.set("password", "");
//                            this.setCookie("userBESST", Core.Util.encryption(this.password.trim()), 14)
//                            this.setCookie("passwordBESST", Core.Util.encryption(this.password.trim()), 14)
                            if (this.checkDB){
                                this.update(Core.Util.encryption(this.password.trim()));
                            }else{
                                this.add(Core.Util.encryption(this.password.trim()));
                            }
                        } else {
                            Core.Data.set("userName", "");
//                            Core.Data.set("password", "");
//                            this.setCookie("userBESST", "", 0)
                            if (this.checkDB){
                                this.update('');
                            }
                        }



//                        Core.Data.set("userIP", res.user.ip || "");

                        scope.orgTypeNum = org.type;
//                        if (scope.orgTypeNum !== 5) {
                            this.userRouterGo(scope.orgTypeNum, "");
//                        }
                    })
//                    .then(() => {
//                        if (scope.orgTypeNum === 5) {
//                            this.getUserRoleDepartmentRel().then(data => {
//                                if(data.data){
//                                    const _userType = data.data.department.type;
//                                    const _department = data.data.department.name;
//                                    localStorage.setItem("department", _department);
//                                    this.userRouterGo(scope.orgTypeNum, _userType);
//                                }else{
//                                    this.userRouterGo(scope.orgTypeNum, "");
//                                }
//                            });
//                        }
//                    });
//                    }).catch((err) => {
////                        console.log(err);
//                        if(err.code === 3){
//                            this.setCookie("passwordBESST", "", 0)
//                        }
//                        if (scope.orgTypeNum === 5) {
//                            this.getUserRoleDepartmentRel().then(data => {
//                                const _userType = data.data.department.type;
//                                const _department = data.data.department.name;
//                                localStorage.setItem("department", _department);
//                                this.userRouterGo(scope.orgTypeNum, _userType);
//                            });
//                        }
//                });
            },
            setCookie(cname,cvalue,exdays){
                let d = {};
                if(exdays === 0){
                    d = new Date('1970-1-1');
                }else{
                    d = new Date();
                }
                d.setTime(d.getTime()+(exdays*24*60*60*1000));
                let expires = "expires="+d.toGMTString();
                window.document.cookie = cname + "=" + cvalue + "; " + expires;
            },
            getCookie(cname){
                let name = cname + "=";
                let ca = window.document.cookie.split(';');
                for(let i=0; i<ca.length; i++){
                    let c = ca[i].trim();
                    if (c.indexOf(name) === 0) return Core.Util.decrypt( c.substring(name.length,c.length) );
                }
                return "";
            },
            offLine() {
                Core.Data.setOffline(true);
                if (this.org.type === Core.Const.ORG_TYPE.BRAND) {
                    this.$router.push({ name: "brandDiagnosisNormal" });
                    //                    this.$router.push({name: "brandToolHmi"});
                }
                if (this.org.type === Core.Const.ORG_TYPE.DEALER) {
                    this.$router.push({ name: "dealerDiagnosisNormal" });
                    //                    this.$router.push({name: "dealerToolHmi"});
                }
                if (this.org.type === Core.Const.ORG_TYPE.COMPONENT) {
                    this.$router.push({ name: "componentDiagnosisNormal" });
                    //                    this.$router.push({name: "componentToolHmi"});
                }
            },
            //根据登录用户的不同跳转不同的页面
            userRouterGo(typeNum, userType) {
                switch (typeNum) {
                    case 1:
                        this.$router.push({ name: "brandIndex", query: { from: 'login'} },);
                        break;
                    case 2:
                        this.$router.push({ name: "dealerBrandList", query: { from: 'login'} });
                        break;
                    case 3:
                        this.$router.push({ name: "assemblerIndex", query: { from: 'login'} });
                        break;
                    case 4:
                        this.$router.push({ name: "componentProductList", query: { from: 'login'} });
                        break;
                    case 5:
//                        this.$message({
//                            message: 'This type of Organization has been closed',
//                            type: "warning"
//                        });
                        if (userType === 0) {
                            this.$router.push({ name: "PIList" });
                        } else if (userType === 1) {
                            this.$router.push({ name: "invoiceList" });
                        } else if (userType === 2) {
                            this.$router.push({ name: "inventoryIndex" });
                        } else {
                            this.$router.push({ name: "centerTicketList" });
                        }
                        break;
                    case 6:
                        this.$router.push({ name: "dealerToolUpdate", query: { from: 'login'} });
                        break;
                    default:
                        Core.Data.setOffline(false);
                        this.$message({
                            message: this.$t('accountProblem'),
                            type: "warning"
                        });
                }
            },
            //获取用户角色信息
            getUserRoleDepartmentRel() {
                return Core.Api.request({
                    method: "GET",
                    url: "/nl/public/userRoleDepartmentRel/finad-one-to-user-id"
                });
            }
        }
    };
</script>

<style lang="scss" rel="stylesheet/scss">
   .el-select-dropdown__item.selected{
        background: #2d3344 !important;
    }
    .el-select-dropdown__item:hover{
        background: #E26829 !important;
    }
    .login {
        width: 100%;
        height: 100%;
        min-width: 350px;
        min-height: 500px;
        position: relative;
        /*.login-header {*/
        /*overflow: hidden;*/
        /*text-align: center;*/
        /*.language {*/
        /*!*margin-top: 18px;*!*/
        /*float: right;*/
        /*!*margin-right: 30px;*!*/
        /*cursor: pointer;*/
        /*.title {*/
        /*text-align: center;*/
        /*line-height: 20px;*/
        /*}*/
        /*.icon {*/
        /*margin: 10px;*/
        /*.img-icon {*/
        /*border-radius: 50%;*/
        /*margin: 0 4px;*/
        /*width: 60px;*/
        /*height: 60px;*/
        /*display: inline-block;*/
        /*text-align: center;*/
        /*& img {*/
        /*margin-top: 1px;*/
        /*border-radius: 50%;*/
        /*width: 58px;*/
        /*height: 58px;*/
        /*}*/
        /*&.active,*/
        /*&:hover {*/
        /*background: #e26829;*/
        /*}*/
        /*}*/
        /*.earth {*/
        /*width: 100px;*/
        /*height: 100px;*/
        /*!*margin-right: 30px;*!*/
        /*}*/
        /*}*/
        /*.text {*/
        /*margin-left: 10px;*/
        /*.label {*/
        /*text-align: center;*/
        /*display: inline-block;*/
        /*width: 68px;*/
        /*line-height: 20px;*/
        /*}*/
        /*}*/
        /*}*/
        /*.language.mr-30 {*/
        /*margin-right: 30px;*/
        /*}*/

        .chooseLanguage{
            height:150px;
            .languageList{
                position: absolute;
                top:68px;
                right:30px;
                width:auto;
                .el-tabs__item{
                    color:#fff;
                }& :hover {
                     color: #E26829 !important;
                 }
            }
        }
        .main-container {
            height: calc(100% - 300px);
            position: relative;
            .main-warp {
                overflow: auto;
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
                text-align: center;
                font-size: 0;
                .head {
                    font-size: 0;
                    margin: 0 auto;
                    border-radius: 50%;
                    width: 210px;
                    height: 210px;
                    background: white;
                    overflow: hidden;
                    .icon-login {
                        width: 210px;
                        height: 210px;
                        .logo-img {
                            position: relative;
                            top: 105px;
                            transform: translate(0, -50%);
                            width: 100%;
                            height: auto;
                        }
                    }
                }
                .login-text {
                    font-size: 18px;
                    margin-top: 20px;
                    margin-bottom: 32px;
                    line-height: 24px;
                }
                .username,
                .password {
                    width: 240px;
                    font-size: 12px;
                    display: block;
                    left: 50%;
                    margin-left: -120px;
                    & > input {
                        text-align: center;
                        height: 40px;
                    }
                }
                .username {
                    & > input {
                        border-radius: 15px 15px 0 0;
                        font-size: 14px;
                    }
                }
                .password {
                    margin-top: 1px;
                    & > input {
                        border-radius: 0 0 15px 15px;
                        font-size: 14px;
                    }
                }
                .check-div {
                    overflow: hidden;
                    margin-top: 24px;
                    font-size: 0;
                    .remember-text {
                        float: left;
                        margin-left: 9px;
                        font-size: 12px;
                        line-height: 14px;
                    }
                    .forget {
                        color: #858890;
                        margin-left: 30px;
                        float: right;
                        text-decoration: underline;
                        font-size: 12px;
                        cursor: pointer;
                        line-height: 14px;
                        &:hover {
                            color: #00ffff;
                        }
                    }
                    .check-item {
                        float: left;
                        background: #2b2f3b;
                        width: 16px;
                        height: 16px;
                        border-radius: 50%;
                        .icon-check {
                            width: 8px;
                            height: 8px;
                            background: #e26829;
                            border-radius: 50%;
                            position: relative;
                            left: 4px;
                            top: 4px;
                        }
                    }
                }
                .login-btn {
                    height: 29px;
                    width: 240px;
                    margin-top: 27px;
                    & span {
                        font-size: 16px;
                    }
                }
                .sign {
                    height: 29px;
                    width: 240px;
                    & span {
                        font-size: 16px;
                    }
                }
            }
            .declare {
                position: absolute;
                left: 90px;
                top: -50px;
                width: 300px;
                text-align: left;
                h2 {
                    color: #fff;
                    border-bottom: 1px solid #fff;
                    padding-bottom: 0.8em;
                    font-size: 180%;
                    font-weight: 400;
                }
                p {
                    padding-top: 15px;
                    line-height: 1.5;
                    letter-spacing: .8px;
                }
                & bodytext {
                    font-size: 12px;
                }
                .showMore {
                    margin-top: 10px;
                    padding: 6px 9px;
                    .el-icon-arrow-right {
                        font-size: 12px;
                        margin-right: -5px;
                    }
                }
            }
            /*.language-dialog {*/
            /*top: 50px;*/
            /*right: 0;*/
            /*transform: translateX(0%);*/
            /*left: initial;*/
            /*}*/
        }
        .dialogInfo {
            width: 100%;
            background: #2B2F3B;
            padding-right: 0;
            top: 0 !important;
        }
        .dialog {
            position: absolute;
            top: 111px;
            left: 60px;
            width: 94%;
            height: 620px;
            overflow: hidden;
            .flex {
                display: flex;
                flex-wrap: nowrap;
                .item {
                    width: 33.3%;
                    display: flex;
                    background: #666;
                    flex-direction: column;
                    align-content: flex-start;
                    h2 {
                        margin: 0;
                        padding: 0;
                        padding-left: 22px;
                        padding-bottom: 0.8em;
                        width: 100%;
                        font-size: 22px;
                        color: #fff;
                        background: #2B2F3B;
                    }
                }
                .last {
                    width: 33.4% !important;
                }
            }
            .el-dialog__header {
                height: 23px;
                padding: 0;
                padding-right: 20px;
            }
            .el-dialog__headerbtn {
                transform: translateY(18px);
            }
            .el-dialog__body {
                // padding:15px 0 20px 15px;
                padding: 0;
                background: #fff;
                .overflow {
                    padding: 8px;
                    padding-bottom: 15px;
                    margin-right: 1px;
                    background: #fff;
                    .declareInfo {
                        padding: 10px 30px 15px 15px;

                        min-height: 520px;
                        border: 1px solid #eaeaea;
                        height: 520px;
                        overflow: auto;
                        color: #666;
                        background: #fff;
                        letter-spacing: .5px;
                        h3 {
                            margin-bottom: 5px;
                            margin-top: 2px;
                        }
                        p {
                            font-size: 14px;
                            span {
                                padding-left: 2em;
                            }
                        }
                        .special {
                            p {
                                display: inline-block;
                                width: 84.5%;
                                font-size: 13px;
                                font-weight: 600;
                            }
                            img {
                                display: inline-block;
                                width: 15%;
                            }
                        }
                    }
                }
            }
        }
    }
</style>




// WEBPACK FOOTER //
// login.vue?0fd25243