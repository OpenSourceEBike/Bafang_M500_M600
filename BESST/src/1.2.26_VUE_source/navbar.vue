<template>
    <div class="nav-bar">
        <div class="logo-box" @click="goToIndex">
            <img class="logo-img" src="../../assets/images/icon-logo.png"/>
            <label class="nav-text">{{title}}</label>
            <!--<input v-model="sn" class="input" />-->
            <!--<input v-model="soft" class="input"/>-->
            <!--<el-button type="preview" @click.stop="com">Compare</el-button>-->
            <!--<el-button type="preview" @click.stop="get">Get</el-button>-->
        </div>
    </div>
</template>

<script>
    import Core from "core/core"

    export default {
        name: 'nav-bar',
        props: ['title', 'type'],
        data() {
            return {
                authList:[],
//                sn: 'CRS10F.350.SN.U1.1K02F8TB060977',
//                soft: 'CRS10F4313E010005.2',
//                sn: 'DPC171.C1.0702F8UC039990',
//                soft: 'DPC171CM10101.0',
//                sn: 'CRX10V.350.FC1.0A42F5U3095528',
//                soft: 'CRX10VC4313h101010.5',
                sn: 'DPBLE.CAN5.0F0TC059999',
                soft: 'BLE2.0',
            }
        },
        mounted() {
            this.authList = this.$store.state.userAuth;
        },
        methods: {
            goToIndex() {
//                console.log(this.title, this.type);
                switch (this.type) {
                    case 1:
                        this.$router.push({name: 'brandIndex'});
                        break;
                    case 2:
                        this.$router.push({name: 'dealerIndex'});
                        break;
                    case 3:
                        this.$router.push({name: 'assemblerIndex'});
                        break;
                    case 4:
                        if(this.authList['production.menu']){
                            this.$router.push({name: "componentOrderList"});
                        }else{
                            this.$router.push({name: "componentErrorShooting"});
                        }
                        break;
                    case 5:
                        this.$router.push({name: 'centerTicketList'});
                        break;
                    case 6:
                        // this.$router.push({name: 't'});
                        break;
                }
            },
            com() {
                Core.Api.Object.objectVersionCompare(this.sn.trim(), this.soft.trim()).then(res => {
                    this.$message({
                        message: res.flag,
                        type: 'error',
                        duration: 2000,
                        showClose: true
                    });
                });
            },
            get() {
                Core.Api.Object.objectVersionCompareAndGetFirmware(this.sn.trim(), this.soft.trim()).then(res => {
                    console.log(res);
//                    this.$message({
//                        message: res.flag,
//                        type: 'error',
//                        duration: 2000,
//                        showClose: true
//                    });
                });
            }

        }

    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
    .nav-bar {
        height: 50px;
        background: #E26829;
        line-height: 50px;
        z-index: 100;
        font-size: 0;

        .logo-box {
            /*width: 220px;*/
            cursor: pointer;
            overflow: hidden;
            /*display: inline-block;*/
            .input {
                background-color: #2B2F3B;
                background-image: none;
                border-radius: 15px;
                border: 1px solid #2B2F3B;
                box-sizing: border-box;
                color: #fff;
                display: inline-block;
                font-size: inherit;
                height: 30px;
                line-height: 1;
                outline: none;
                padding: 3px 10px;
                transition: border-color .2s cubic-bezier(.645,.045,.355,1);
                width: 300px;
            }
            .logo-img {
                width: 40px;
                height: 26px;
                margin: 0 18px;
                vertical-align: middle;
            }

            .nav-text {
                display: inline-block;
                vertical-align: top;
                font-size: 18px;
                font-weight: bold;
                cursor: pointer;
            }
        }
    }
</style>



// WEBPACK FOOTER //
// navbar.vue?136e3698