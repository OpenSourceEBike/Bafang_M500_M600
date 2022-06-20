<template>
    <div class="statistics-assembler">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" title="back"></div>
                </div>
            </div>
            <label class="title">Assembler Vehicle Statistics</label>
            <el-select v-model="timeType" placeholder="date" @change="getStatistic" class="clear-float pull-right">
                <el-option
                    v-for="item in timeList"
                    :key="item.type"
                    :label="item.label"
                    :value="item.type">
                </el-option>
            </el-select>
        </div>
        <div class="clear-10"></div>
        <div class="data-holder">
            <div class="canvas-holder">
                <IEcharts :option="dataOptions"/>
            </div>

        </div>
        <div class="table-wrap">
            <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-show="partnerList.length > 0">
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
                    :total="total">
                </el-pagination>
                <p class="total"> Total: {{total}}</p>
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
                tableHead: ["Assembler Name", "Quantity", "More"],

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
                        type: 'bar',
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
            // this.getAllJuniorPartnerList();
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
            // 下面展示的 list
            getPartnerList() {
                Core.Api.Brand.getAssemblerStatList(this.orgId, this.timeType, this.currentPage).then(
                    res => {
                        console.log(res, 'getDealerSalesList');
                        this.total = res.total;
                        this.partnerList = res.list;
                    }
                );
            },

            getAssemblerStat() {
                Core.Api.Brand.getAssemblerStat(this.orgId, this.timeType).then(res => {
                    console.log(res);
                    this.saleAmount = res.total;
                    this.dataSet = res.stat;
                })
            },

            goToDetailPage(orgId) {
                this.$router.push({
                    name: 'statisticsAssemblerStatisticsDetail',
                    query: {"org_id": orgId, "time_type": this.timeType}
                });
            },

            getStatistic() {
                this.getPartnerList();
                this.getAssemblerStat();
            }
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .statistics-assembler {
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
// assembler-vehicle.vue?017bcb0e