<template>
    <div class="invitation">
        <nav-bar :title="title"></nav-bar>
        <div class="nav-btn">
            <el-button type="primary" icon="arrow-left" @click="$router.back()">{{ $t('login') }}</el-button>
        </div>
        <!--<div class="nav-btn-access">-->
            <!--<el-button type="primary" icon="arrow-right" @click="show()">{{ $t('getAccess') }}</el-button>-->
        <!--</div>-->
        <div class="main-warp">
            <div class="head">
                <el-steps :space="145" :active="active">
                    <el-step :title="$t('verifyCode')"></el-step>
                    <el-step :title="$t('createAccount')"></el-step>
                    <el-step :title="$t('createCompany')"></el-step>
                </el-steps>
            </div>
            <p class="invitation-label">{{ $t('invitationCode') }}</p>
            <div class="input-box" @keyup.13="goAccount">
                <el-input class="input" v-model="code"></el-input>
            </div>
            <div class="invitation-text">
                <div v-if="!haveUser">
                    <p>{{ $t('typeTheCodeTip') }}</p>
                    <p>{{ $t('tryAgainTip') }}</p>
                </div>
                <div v-else>
                    <p class="tip">{{ $t('noCompanyTip') }}</p>
                    <p>{{ $t('reCodeTip') }}</p>
                </div>
            </div>
            <el-button class="invitation-btn" @click="goAccount">{{ $t('verify') }}</el-button>
        </div>

        <el-dialog
            :visible.sync="accessVisible"
            size="middle">
            <div>
                <canvas id="mc" width="960" height="480">
                    您的浏览器版太低，请升级!
                </canvas>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import NavBar from "../layout/navbar.vue";
    import Core from  "core/core";
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default{
        name: 'invitation',
        components: {
            NavBar,
        },
        data(){
            return {
                code: '',
                haveUser: 0,
                title: 'BESST',
                active: 1,
                accessVisible: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Register.Invitation,
                zh: ZH.Register.Invitation,
                de: DE.Register.Invitation,
                nl: NL.Register.Invitation
            }
        },
        mounted(){
            this.haveUser = this.$route.query.haveUser;
            if(this.haveUser){ this.active = 2 }
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.haveUser = this.$route.query.haveUser;
                }, 100);
            });
            this.init();
        },
        methods: {

            goAccount(){
                if (!this.code) {
                    this.$message({
                        message: 'Please input invitation code',
                        type: 'warning'
                    });
                    return;
                }

                if(this.haveUser){
                    Core.Api.Org.checkInviteByUser(this.code).then(res => {
                        console.log(res);
//                    Core.Data.set('type', res.org.target_org_type);
                        let inviteInfo = res.invite;
                        Core.Data.set('email', inviteInfo.target_org_email);
                        Core.Data.setRoleEmailType(inviteInfo.target_org_email, inviteInfo.target_org_type);
                        console.log(inviteInfo.status);
                        if (inviteInfo.status === 3) {
                            this.$router.push({path: 'company'});
                            return;
                        }
                        this.$router.push({path: 'account'})
                    });
                }else {
                    Core.Api.Org.checkInvite(this.code).then(res => {
                        console.log(res);
//                    Core.Data.set('type', res.org.target_org_type);
                        let inviteInfo = res.invite;
                        Core.Data.set('email', inviteInfo.target_org_email);
                        Core.Data.setRoleEmailType(inviteInfo.target_org_email, inviteInfo.target_org_type);
                        console.log(inviteInfo.status);
                        if (inviteInfo.status === 3) {
                            this.$router.push({path: 'company'});
                            return;
                        }
                        this.$router.push({path: 'account'})
                    });
                }



            },
            show(){
                this.accessVisible = true;
                setTimeout(()=>{
                    this.draw();
                })
            },
            init(){

            },

            draw(){
                //获取canvas元素对应的DOM对象
                var canvas=document.getElementById('mc');
                //获取在canvas上绘图的CanvasRenderingContext2d对象
                var ctx =canvas.getContext('2d');
                //设置矩形颜色
                ctx.fillStyle = "#E26829";
                //四个矩形
                ctx.fillRect(80,150,150,50);
                ctx.fillRect(350,150,150,50);
                ctx.fillRect(620,40,150,50);
                ctx.fillRect(620,260,150,50);
                //文字颜色
                ctx.fillStyle = "#FFF";
                //字体样式
                ctx.font = '20px sans-serif';
                //基线居中
                ctx.textBaseline = 'middle';
                //填充文字
                ctx.fillText(this.$t("backend"),82 , 175);
                ctx.fillText(this.$t("brand"),358 , 175);
                ctx.fillText(this.$t("dealers"),628, 65);
                ctx.fillText(this.$t("assemblers"),628 , 285);
                //填充文字  备注说明
                //换行函数
                function drawTextOn(t,x,y,w){
                    var chr = t.split("");
                    var temp = "", row = [];
                    for(var a = 0; a < chr.length; a++){
                        if( ctx.measureText(temp).width < w){
                        }else{
                            if(chr[a].charCodeAt(0) >128 ? false : chr[a].charCodeAt(0) == 32 ?  false : temp.substr(temp.length-1,1) != ' ' ){
                                let index =temp.lastIndexOf(' ');
                                let str = temp;
                                temp = temp.substring(0,index);
                                row.push(temp.replace(/^\s*/g,''));
                                temp = str.substring(index+1);
                            }
                            else{
                                row.push(temp.replace(/^\s*/g,''));
                                temp = "";
                            }
                        }
                        temp += chr[a];
                    }
                    row.push(temp.replace(/^\s*/g,''));
                    for(var b = 0; b < row.length; b++){
                        ctx.fillText(row[b],x,y+(b+1)*20);
                    }
                }
                ctx.fillStyle = "#FFF";
                ctx.font = '14px  "PingFang SC", "PingFangSC-Semibold", "Helvetica Neue", Helvetica, "Nimbus Sans L", Arial, "Liberation Sans", "Hiragino Sans GB", "Source Han Sans CN", "Source Han Sans SC", "Microsoft YaHei", SimHei, sans-serif';
                drawTextOn(this.$t("createAP"),85 , 205,123);
                drawTextOn(this.$t("createCode"),358 , 205,150);
                drawTextOn(this.$t("useCode"),640 , 95,199);
                drawTextOn(this.$t("useCode"),640 , 315,199);
                //线段
                ctx.fillStyle = "#AFAFAF";
                ctx.beginPath();
                ctx.moveTo(230,175);
                ctx.lineTo(350,175);
                ctx.stroke();
                ctx.closePath();
                ctx.beginPath();
                ctx.moveTo(500,175);
                ctx.lineTo(560,175);
                ctx.lineTo(560,65);
                ctx.lineTo(620,65);
                ctx.stroke();
                ctx.closePath();
                ctx.beginPath();
                ctx.moveTo(560,175);
                ctx.lineTo(560,285);
                ctx.lineTo(620,285);
                ctx.stroke();
                ctx.closePath();
            }
        }

    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .invitation {
        .nav-btn {
            margin: 18px 34px;
        }
        .nav-btn-access {
            position: absolute;
            top: 70px;
            right: 20px;
        }
        .main-warp {
            text-align: center;
            position: absolute;
            left: 50%;
            transform: translate(-50%, 0);
            .head {
                height: 80px;
                width: 430px;
                margin: 70px auto 0 auto;
                position: relative;
                left: -10px;
                .el-steps{
                    .el-step__title{
                        font-size: 14px;
                    }
                }
            }
            .input-box {
                margin-top: 36px;
                .input {
                    width: 310px;
                    .el-input__inner{
                        text-align: center;
                    }
                }
            }
            .invitation-text {
                margin-top: 54px;
                p {
                    color: #858890;
                }
                .tip{
                    color: greenyellow;
                }
            }
            .invitation-btn {
                margin-top: 48px;
                width: 168px;
                font-size: 14px;
            }
        }
        .invitation-label {
            margin-top: 110px;
            font-size: 14px;
        }

    }
</style>



// WEBPACK FOOTER //
// invitation.vue?29886f34