<template>
    <div class="statistics-dealer">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" title="back"></div>
                </div>
            </div>
            <label class="title">Dealer sales Statistics</label>
            <div style="display: inline-block" class="clear-float pull-right">
                <el-select v-model="timeType" placeholder="date" @change="getStatistic">
                    <el-option
                        v-for="item in timeList"
                        :key="item.type"
                        :label="item.label"
                        :value="item.type">
                    </el-option>
                </el-select>
                <!--<el-date-picker-->
                <!--v-model="time"-->
                <!--type="daterange"-->
                <!--format="yyyy/MM/dd"-->
                <!--align="right"-->
                <!--placeholder="select date"-->
                <!--:picker-options="dateOptions">-->
                <!--</el-date-picker>-->
                <el-select v-model="dealerId" placeholder="all dealer" filterable>
                    <el-option
                        v-for="item in dealerList"
                        :key="item.id"
                        :label="item.name"
                        :value="item.id">
                    </el-option>
                </el-select>
                <!--<el-input v-model="dealerName" placeholder="all dealer"></el-input>-->
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" title="search" @click="getStatistic"></div>
                    </div>
                </div>
            </div>
        </div>
        <div class="clear-10"></div>
        <div class="data-holder">
            <div class="data-header">
                <div>
                    <img src="../../../assets/images/icon/menu/dealer-white.png"/>
                    <div class="amount">
                        Dealer Amount <br/>
                        <b>{{dealerAmount}}</b>
                    </div>
                </div>
                <div>
                    <img src="../../../assets/images/icon/icon-line@2x.png"/>
                    <div class="amount">
                        Sales Amount<br/>
                        <b>{{saleAmount}}</b>
                    </div>
                </div>
            </div>
            <div class="canvas-holder">
                <IEcharts :option="dataOptions"/>
            </div>

        </div>
        <div class="table-wrap">
            <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-if="partnerList.length > 0">
                <thead>
                <tr>
                    <td v-for="item in tableHead">{{item}}</td>
                </tr>
                </thead>
                <tbody>
                <tr class="large" v-for="item in partnerList">
                    <td class="td name">
                        {{ item.name }}
                    </td>
                    <td>
                        {{ item.count }}
                    </td>
                    <td>
                        <a class="btn-underline" @click.stop="goToDetailPage(item.org_id)">Detail</a>
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="partnerList.length <= 0">
                No Result
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="getPartnerList"
                    :current-page.sync="currentPage"
                    :page-size="4"
                    layout="prev, pager, next"
                    :total="dealerAmount">
                </el-pagination>
                <p class="total"> Total: {{dealerAmount}}</p>
            </div>
        </div>
    </div>
</template>

<script>
    import Core from 'core/core'
    import IEcharts from 'vue-echarts-v3/src/full';

    export default {
        name: "",

        components: {
            IEcharts
        },

        data() {
            return {
                orgId: '',
                partnerList: [],
                total: 0,
                currentPage: 1,
                tableHead: ["Dealer Name", "Sales Volume", "More"],

                timeType: 3,
                timeList: [
                    {type: 0, label: 'all'},
                    {type: 1, label: 'Today'},
                    {type: 2, label: 'Yesterday'},
                    {type: 3, label: 'This week(mon.-today)'},
                    {type: 4, label: 'Last week(mon.-sun.)'},
                    {type: 5, label: 'This month'},
                    {type: 6, label: 'Last month'}
                ],

                dealerName: '',
                time: '',
                dateOptions: {
                    shortcuts: [{
                        text: 'today',
                        onClick(picker) {
                            const today = new Date();
                            picker.$emit('pick', [today, today]);
                        }
                    }, {
                        text: 'yesterday',
                        onClick(picker) {
                            const end = new Date();
                            const start = new Date();
                            start.setTime(start.getTime() - 3600 * 1000 * 24);
                            picker.$emit('pick', [start, end]);
                        }
                    }, {
                        text: 'recent 30 days',
                        onClick(picker) {
                            const end = new Date();
                            const start = new Date();
                            start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
                            picker.$emit('pick', [start, end]);
                        }
                    }]
                },

                dealerList: [],
                dealerId: 0,

                dataOptions: {
                    tooltip: {
                        trigger: 'axis'
                    },
                    xAxis: {
                        type: 'category',
                        data: [],
                        axisLine: {
                            lineStyle: {
                                color: '#fff'
                            }
                        }
                    },
                    yAxis: {
                        type: 'value',
                        axisLine: {
                            show: false,
                            lineStyle: {
                                color: '#fff'
                            }
                        },
                        minInterval: 1
                    },
                    series: [{
                        data: [],
                        type: 'line',
                        smooth: true
                    }]
                },

                dealerAmount: 0,
                saleAmount: 0,
                dataSet: [],
                categoryList: [],
                valList: [],


            }
        },

        mounted: function () {
            this.orgId = Core.Data.getOrg().id;
            this.getAllJuniorPartnerList();
            this.getStatistic();
        },

        watch: {
            dataSet: {
                handler(newVal, oldVal) {
                    let categoryList = [];
                    let valList = [];
                    if (newVal) {
                        for (let key in newVal) {
                            categoryList.push(key);
                            valList.push(newVal[key]);
                        }
                    }
                    this.dataOptions.xAxis.data = categoryList;
                    this.dataOptions.series[0].data = valList;
                },
                deep: true
            }
        },

        methods: {
            // 下面展示的 dealer_list
            getPartnerList() {
                Core.Api.Brand.getDealerSalesList(this.orgId, this.timeType, this.currentPage).then(
                    res => {
                        console.log(res, 'getDealerSalesList');
                        this.partnerList = res.list;
                    }
                );
            },

            getAllJuniorPartnerList() {
                Core.Api.Org.getAllJuniorPartnerList(Core.Const.ORG_TYPE.DEALER).then(res => {
                    this.dealerList = res.org_list;
                    this.dealerAmount = res.org_list.length;
                    this.dealerList.unshift({
                        id: 0,
                        name: "all dealer"
                    })
                });
            },

            getDealerSaleStatistic() {
                Core.Api.Brand.getDealerSaleStatistic(this.orgId, this.timeType, this.dealerId).then(res => {
                    console.log(res);
                    this.saleAmount = res.total;
                    this.dataSet = res.data;
                })
            },

            goToDetailPage(dealerId) {
                console.log(dealerId);
                // this.$router.push({name: 'brandModelDetail', query: {"modelId": id}});
                this.$router.push({
                    name: 'statisticsDealerSaleDetail',
                    query: {"dealer-id": dealerId, "time-type": this.timeType}
                });
            },

            getStatistic() {
                this.getPartnerList();
                this.getDealerSaleStatistic();
            }
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .statistics-dealer {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        .header {
            width: 100%;
        }
        .data-holder {
            .data-header {
                display: flex;
                flex-direction: row;
                justify-content: space-around;
                > div {
                    display: flex;
                    flex-direction: row;
                    justify-content: center;
                    align-items: center;
                    width: 170px;
                    height: 66px;
                    border-radius: 10px;
                    background: #98B42C;
                    > img {
                        width: 50px;
                    }
                    .amount {
                        margin-left: 10px;
                        text-align: center;
                    }
                }
            }
            .canvas-holder {
                /*margin-top: 20px;*/
                width: 100%;
                height: 350px;
                /*background: white;*/
            }
        }
        .table-wrap {
            flex: 1;
            margin-bottom: 60px;
            .table-content {
                padding-bottom: 0;
            }
            .no-data-content {
                bottom: 0;
                height: 100%;
                position: relative;
            }
        }

    }
</style>



// WEBPACK FOOTER //
// dealer-sale.vue?694b43d9