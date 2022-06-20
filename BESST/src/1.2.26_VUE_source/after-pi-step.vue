<template>
    <div class="screen">
            <div class="header">
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item back" @click="goPIList" title="back" ></div>
                    </div>
                </div>
                <label class="title">orderDetail</label>
                <!--<div class="btn-group-top">-->
                <!--<div class="icons">-->
                <!--&lt;!&ndash;orderData.status 为 1时 标识 订单已经完成 不可编辑&ndash;&gt;-->
                <!--<div class="icon-item export" @click="exportOrder()"></div>-->
                <!--<div class="icon-item edit" @click="goToEditOrder()" v-if="authList[Core.Const.AUTH_LIST.productionEdit]" :title="$t('edit')"></div>-->
                <!--<div class="icon-item delete" @click="dialogDeleteVisible = true" v-if="authList[Core.Const.AUTH_LIST.productionDelete]" :title="$t('delete')"></div>-->
                <!--</div>-->
                <!--</div>-->
            </div>
            <div class="content-wrap">
                <div class="content">
                    <div style="height:114px ">
                        <el-steps :active="active" align-center style="border:1px solid #333" >
                            <el-step v-for="(stepData, index) in stepDatas"
                                     :title="stepData.title"
                                     :description = "stepData.desc"
                                     :status = "stepData.status"
                                     :key="index"
                            ></el-step>

                        </el-steps>
                    </div>
                    <div class="step-select">
                        <ul class>
                            <li @click = "update(1)"><el-button>1</el-button></li>
                            <li @click = "update(2)"><el-button>2</el-button></li>
                            <li @click = "update(3)"><el-button>3</el-button></li>
                            <li @click = "update(4)"><el-button>4</el-button></li>
                            <li @click = "update(5)"><el-button>5</el-button></li>
                            <li @click = "update(6)"><el-button>6</el-button></li>
                        </ul>

                        <el-button style="margin-top: 12px;" @click="back">上一步</el-button>
                        <el-button style="margin-top: 12px;" @click="next">下一步</el-button>

                    </div>

                    <div class="content-record">
                        <div class="table-wrap" v-show="active ==1 ">
                            <div class="table-content-holder">
                                <table class="table-content">
                                    <tbody>
                                    <tr>
                                        <td>Editor</td>
                                        <td>James Zhou</td>
                                        <td>2018.7.13</td>
                                        <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                    </tr>
                                    <tr>
                                        <td>Editor</td>
                                        <td>James Zhou</td>
                                        <td>2018.7.14</td>
                                        <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                    </tr>
                                    <tr>
                                        <td>Last Editor</td>
                                        <td>James Zhou</td>
                                        <td>2018.7.15</td>
                                        <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                    </tr>
                                    </tbody>
                                </table>
                            </div>
                            <el-button @click="statusConfirm">success</el-button>
                        </div>
                        <div v-show="active ==2 ">
                            <h3>

                            </h3>
                            <label class="title"> Last editor:James Zhou  Time:2018-07-15 </label>

                            <div class="table-wrap" >
                                <div class="table-content-holder">
                                    <table class="table-content">
                                        <thead>
                                        <tr>
                                            <td>PI No.</td>
                                            <td>ETD</td>
                                            <td>Operation</td>
                                        </tr>
                                        </thead>
                                        <tbody>
                                        <tr>
                                            <td>00001-1</td>
                                            <td>2018-07-12</td>
                                            <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                        </tr>
                                        <tr>
                                            <td>00001-2</td>
                                            <td>2018-08-13</td>
                                            <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                        </tr>
                                        <tr>
                                            <td>00001-3</td>
                                            <td>2018-09-12</td>
                                            <td><a class="btn-underline" @click="dialogItemListVisible = true">Detail</a></td>
                                        </tr>
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                            <el-button @click="statusConfirm">success</el-button>

                        </div>
                        <div v-show="active ==3 ">
                            <span>3</span>
                            <el-button @click="statusConfirm">success</el-button>

                        </div>
                        <div v-show="active ==4 ">
                            <span>4</span>
                            <el-button @click="statusConfirm">success</el-button>
                        </div>
                        <div v-show="active ==5 ">
                            <span>5</span>
                            <el-button @click="statusConfirm">success</el-button>
                        </div>
                        <div v-show="active ==6 ">
                            <span>6</span>
                            <el-button @click="statusConfirm">success</el-button>
                        </div>
                    </div>

                </div>

            </div>

            <el-dialog :visible.sync = "dialogItemListVisible" title="title" size="large">
                <span slot="footer" class="dialog-footer">
                    <el-button type="primary" @click="dialogItemListVisible = false">Close</el-button>
                </span>
            </el-dialog>

        <el-button type="button" @click="toggle" class="full_btn">Fullscreen</el-button>
    </div>

</template>

<script>
    import Core from 'core/core';
    import ElButton from "../../../../node_modules/element-ui/packages/button/src/button";

    export default {
        components: {
            ElButton,
            },
        data() {
            return {
                Core: Core,

                fullscreen: false,
                active: 1,
                stepDatas:[
                    {title:'Proforma Invoice',
                        desc:'2018-7-15',
                        status:'wait'},
                    {title:'PI Confirmed',
                        desc:'',
                        status:'process'},
                    {title:'Stock in',
                        desc:'',
                        status:'finish'},
                    {title:'Stock out',
                        desc:'',
                        status:'wait'},
                    {title:'Commercial Invoice',
                        desc:'',
                        status:'error'},
                    {title:'Invoice',
                        desc:'',
                        status:'success'},

                ],

                dialogItemListVisible:false,
            }
        },
        created(){},
        mounted(){},
        methods: {
            toggle () {
                this.$refs['fullscreen'].toggle()
            },
            fullscreenChange (fullscreen) {
                this.fullscreen = fullscreen
            },
            next() {
//                if(this.stepDatas[this.active-1].status != "success"){
//                    this.$message("please confrim the work");
//                    return;
//                }
                this.active = this.active + 1;
                if (this.active > 6){
                    this.active = 6;
                    return;
                }
                console.log(this.active);
//                this.stepDatas[this.active].status = "success";

            },
            back(){
                this.active = this.active - 1;
                if (this.active < 1){
                    this.active = 1;
                    return;
                }
                console.log(this.active);
            },

            update(index){
                this.active = index;
            },
            statusConfirm(){
                this.stepDatas[this.active-1].status = "success";
            },

            goPIList(){
                this.$router.push({name:'afterIndex'});
            },
        },
        computed: {

        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .screen{
        .step-select{
            ul li{
                display:inline-block;
            }
        }
        .content-record{
            margin-top: 20px;
            h3{
                margin:10px 0;
            }

        }
        .full_btn{
            position:fixed;
            bottom:10px;
            right:10px;
        }
    }
</style>



// WEBPACK FOOTER //
// after-pi-step.vue?96fa69aa