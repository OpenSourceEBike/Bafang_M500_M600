<template>
    <div class="statistics-dealer">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" title="back"></div>
                </div>
            </div>
            <label class="title">After-sales Service Statistics</label>
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
            <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-if="dataList.length > 0">
                <thead>
                <tr>
                    <td v-for="item in tableHead">{{item}}</td>
                </tr>
                </thead>
                <tbody>
                <tr class="large" v-for="item in dataList">
                  <!--['Name', 'Model', 'Vehicle', 'Motor', 'HMI', 'Sensor', 'Battery', 'Controller', 'Light', 'Other'],-->

                    <td>
                        {{ item.brand }}
                    </td>
                    <td>
                        {{ item.item }}
                    </td>
                    <td>
                        {{ item.Vehicle }}
                    </td>
                    <td>
                        {{ item.Motor }}
                    </td>
                    <td>
                        {{ item.Hmi }}
                    </td>
                    <td>
                        {{ item.Sensor }}
                    </td>
                    <td>
                        {{ item.Battery }}
                    </td>
                    <td>
                        {{ item.Controller }}
                    </td>
                    <td>
                        {{ item.Light }}
                    </td>
                    <td>
                        {{ item.Other }}
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="dataList.length <= 0">
                No Result
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="getAfterSaleStatList"
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

                tableHead: ['Name', 'Model', 'Vehicle', 'Motor', 'HMI', 'Sensor', 'Battery', 'Controller', 'Light', 'Other'],
                dataList: [],

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


                dataOptions: {
                    tooltip: {
                        trigger: 'item',
                        formatter: '{b} : {c}'
                    },
                    legend: {
                        orient: 'vertical',
                        left: 'left',
                        // data: ['Vehicle', 'Motor', 'Battery', 'Motor', 'Controller', 'HMI', 'Light', 'Sensor', 'Other'],
                        data: [],
                        textStyle: {
                            color: '#fff'
                        }
                    },
                    color: ['#BEC2C8', '#88A921', '#B4561C', '#6DB058', '#D6D312', '#B86B51', '#9BC592', '#6B6CBD'],
                    series: [{
                        data: [
                            {value: 335, name: '直接访问'},
                            {value: 310, name: '邮件营销'},
                            {value: 234, name: '联盟广告'},
                            {value: 135, name: '视频广告'},
                            {value: 1548, name: '搜索引擎'}
                        ],
                        type: 'pie',
                        smooth: true,
                        label: {
                            normal: {
                                formatter: '{d}%',
                                position: 'inside'
                            }
                        },
                    }]
                },

                dataSet: [],
                categoryList: [],
                valList: [],


            }
        },

        mounted: function () {
            this.orgId = Core.Data.getOrg().id;
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
                            valList.push({value: newVal[key], name: key});
                        }
                    }
                    this.dataOptions.legend.data = categoryList;
                    this.dataOptions.series[0].data = valList;
                },
                deep: true
            }
        },

        methods: {
            // 下面展示的 \list
            getAfterSaleStatList() {
                Core.Api.Brand.getAfterSaleStatList(this.orgId, this.timeType, this.currentPage).then(
                    res => {
                        console.log(res, 'getDealerSalesList');
                        this.dataList = res.list;
                        this.total = res.total;
                    }
                );
            },

            getAfterSaleStat() {
                Core.Api.Brand.getAfterSaleStat(this.orgId, this.timeType).then(res => {
                    this.dataSet = res.stat;
                })
            },


            getStatistic() {
                this.getAfterSaleStat();
                this.getAfterSaleStatList()
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
// after-sale.vue?558fc97b