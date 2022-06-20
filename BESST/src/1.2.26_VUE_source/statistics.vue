<template>
    <div class="stat-of-category">
        <div class="header">
            <label class="title">{{ $t('ticketStat') }}</label>
        </div>
        <div class="chart-container">
            <div class="echarts">
                <IEcharts :option="categoryOption"></IEcharts>
            </div>
        </div>
    </div>
</template>

<script>
    import IEcharts from 'vue-echarts-v3/src/full';
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default{
        name: "ticketStat",

        components: {
            IEcharts
        },

        data(){
            return {
                categoryOption: {
                    color: ['#c23531', '#5FC554', '#61A0A8', '#D48265', '#91c7AE', '#8E388E', '#00CDCD', '#E3E3E3'],
                    title: {
                        text: 'Ticket by Category',
//                        subtext: 'by category',
                        x: 'center',
                        textStyle: {
                            color: '#FFF'
                        },
                    },
                    tooltip: {
                        trigger: 'item',
                        formatter: "{a} <br/>{b} : {c} ({d}%)"
                    },
                    legend: {
                        orient: 'vertical',
                        left: 'left',
                        data: ['Vehicle', 'Motor', 'HMI', 'Sensor', 'Battery', 'Light', 'Controller', 'Other'],
                        textStyle: {
                            color: '#FFF'
                        },
                    },
                    series: [
                        {
                            name: 'Ticket Category',
                            type: 'pie',
                            radius: '55%',
                            center: ['50%', '55%'],
                            data: [{
                                value: 0,
                                name: 'Vehicle'
                            }, {
                                value: 0,
                                name: 'Motor'
                            }, {
                                value: 0,
                                name: 'HMI'
                            }, {
                                value: 0,
                                name: 'Sensor'
                            }, {
                                value: 0,
                                name: 'Battery'
                            }, {
                                value: 0,
                                name: 'Light'
                            }, {
                                value: 0,
                                name: 'Controller'
                            }, {
                                value: 0,
                                name: 'Other'
                            }],
                            itemStyle: {
                                emphasis: {
                                    shadowBlur: 10,
                                    shadowOffsetX: 0,
                                    shadowColor: 'rgba(0, 0, 0, 0.5)'
                                }
                            }
                        }]
                },
                values: {},

            }
        },
        i18n: {
            messages: {
                en: EN.Component.ServiceCenter.Stat,
                zh: ZH.Component.ServiceCenter.Stat,
                de: DE.Component.ServiceCenter.Stat,
                nl: NL.Component.ServiceCenter.Stat
            }
        },
        mounted: function () {
            // 国际化
            this.categoryOption.title.text = this.$t('ticketByCategory');
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.categoryOption.title.text = vm.$t('ticketByCategory');
                }, 100);
            });
            this.getStatOfCategory();
        },
        methods: {
            getStatOfCategory(){
                Core.Api.Ticket.getStatOfCategory().then(res => {
                    this.values = res;
                    this.partValues(res);
                })
            },

            partValues(res){

                this.categoryOption.series[0].data[0].value = res['vehicle'];
                this.categoryOption.series[0].data[1].value = res['motor'];
                this.categoryOption.series[0].data[2].value = res['hmi'];
                this.categoryOption.series[0].data[3].value = res['sensor'];
                this.categoryOption.series[0].data[4].value = res['battery'];
                this.categoryOption.series[0].data[5].value = res['light'];
                this.categoryOption.series[0].data[6].value = res['controller'];
                this.categoryOption.series[0].data[7].value = res['other'];

            }
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .stat-of-category {
        height: 100%;
        width: 100%;
        overflow: auto;
        .chart-container {
            width: 100%;
            height: 100%;
            padding-top: 20px;
            /*margin: 10px auto;*/
            .echarts {
                margin-left: 25%;
                margin-top: 15%;
                padding: 20px;
                /*border: 1px solid silver;*/
                display: inline-block;
                width: 50%;
                height: 50%;
                min-width: 520px;
                min-height: 330px;
                /*background-color: white;*/
                canvas {

                    height: 100% !important;
                }
            }
            .echarts:first-child {
                /*padding-top: 20px;*/
            }
        }
    }
</style>



// WEBPACK FOOTER //
// statistics.vue?0b0d6b0d