<template>
    <div class="after-index">
        <div class="operation ">
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item">
                <div class="order-list"></div>
                <label class="label">Order</label>
            </div>
            <div class="item" :style ="bgcolor" @click="goPI()">
                <div class="order-list order-height"></div>
                <label class="label">Proforma Invoice</label>
            </div>
            <div class="item" :style ="bgcolor" @click="goPIStep()">
                <div class="order-list"></div>
                <label class="label">Step</label>
            </div>
            <!--<div class="item " :style ="bgcolor" @click="goPackage()" :active="{active:authList['order.menu']}">-->
            <div class="item " :style ="bgcolor" @click="goPackage()">
                <div class="order-list"></div>
                <label class="label">Package</label>
            </div>

        </div>

    </div>

</template>

<script>
    import Core from 'core/core';
    import EN from 'src/assets/lang/en';


    export default {
        data() {
            return {
                authList: {},
                flag: null,
                bgcolor: { 'background-color': '#98B42C'},
            }
        },
        i18n: {
            messages: {
                en: EN.Dealer.Index,
            }
        },
        created(){

        },
        computed:{

        },
        mounted(){
            this.authList['order.menu'] = true;
        },
        methods: {
            getAuth(){
            },
            goPI(){
                this.$router.push({name: 'afterPi', query:{"from": "home"}});
            },
            goPIStep(){
                this.$router.push({name: 'afterPiStep', query:{"from": "home"}});
            },
            goPackage(){
                this.$router.push({name: 'afterPackage', query:{"from": "home"}});
            },
        },
        watch:{
            authList:function(newVal, oldVal){
                this.bgcolor = "{ 'background-color': 'red'}"
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .after-index {
        width: 100%;
        height: 100%;
        display: flex;
        align-items: center;
        justify-content: center;
        min-width: 530px;
        min-height: 360px;
        overflow: auto;
        .operation {
            /*flex: 1;*/
            /*height: 0;*/
            /*width: 0;*/
            /*overflow: hidden;*/
            .item {
                text-align: center;
                display: inline-block;
                width: 200px;
                margin: 15px;
                border-radius: 10px;
                padding-bottom: 20px;
                cursor: pointer;
                background-color: grey;
                .order-list {
                    height: 160px;
                    background: url('../../../assets/images/icon/icon-order.png') no-repeat center;
                    background-size: 60px;
                }
                .order-height{
                    height: 128px;
                }
                .diagnosis {
                    height: 160px;
                    background: url('../../../assets/images/icon/icon-diagnosis.png') no-repeat center;
                    background-size: 80px;
                }

                .label {
                    margin: auto;
                    font-size: 26px;
                    font-weight: bold;
                }
            }
            .active{
                background-color: #98B42C;
            }

        }
    }
</style>



// WEBPACK FOOTER //
// aftersales.vue?29d38980