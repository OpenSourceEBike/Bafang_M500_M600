<template>
	<div class="statistics-dealer-detail">
        <div class="header">
            <!--<el-form :inline="true" class="demo-form-inline">-->
                <div class="btn-group-inline">
                <div class="icons">
                <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
                </div>
                <!--&lt;!&ndash;<el-form-item>&ndash;&gt;-->
                    <!--&lt;!&ndash;<el-date-picker&ndash;&gt;-->
                        <!--&lt;!&ndash;v-model="time"&ndash;&gt;-->
                        <!--&lt;!&ndash;type="daterange"&ndash;&gt;-->
                        <!--&lt;!&ndash;format="yyyy/MM/dd"&ndash;&gt;-->
                        <!--&lt;!&ndash;align="right"&ndash;&gt;-->
                        <!--&lt;!&ndash;placeholder="select date"&ndash;&gt;-->
                        <!--&lt;!&ndash;:picker-options="dateOptions">&ndash;&gt;-->
                    <!--&lt;!&ndash;</el-date-picker>&ndash;&gt;-->
                <!--&lt;!&ndash;</el-form-item>&ndash;&gt;-->
                <!--<el-select v-model="timeType" placeholder="date">-->
                    <!--<el-option-->
                        <!--v-for="item in timeList"-->
                        <!--:key= "item.type"-->
                        <!--:label="item.label"-->
                        <!--:value="item.type">-->
                    <!--</el-option>-->
                <!--</el-select>-->
                <!--<el-form-item>-->
                    <!--<el-select v-model="brandId" clearable placeholder="all brand">-->
                      <!--<el-option-->
                            <!--v-for="item in brandList"-->
                            <!--:key= "item.id"-->
                            <!--:label="item.name"-->
                            <!--:value="item.id">-->
                        <!--</el-option>-->
                    <!--</el-select>-->
                    <!--&lt;!&ndash;<el-input v-model="dealerName"></el-input>&ndash;&gt;-->
                <!--</el-form-item>-->
                <!--<div class="btn-group-inline">-->
                    <!--<div class="icons">-->
                        <!--<div class="icon-item search" title="search"></div>-->
                    <!--</div>-->
                <!--</div>-->
            <!--</el-form>-->
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <tr>
                        <td v-for="item in tableHead">{{item}}</td>
                    </tr>
                    </thead>
                    <tbody>
                    <tr class="large" v-for="item in modelList">
                        <td class="img">
                            <img :src="item.brand_logo | getImgUrl"/>
                        </td>
                        <td>
                            {{item.brand}}
                        </td>
                            {{item.model}}
                        <td>
                            {{item.item}}
                        </td>
                        <td>
                            {{item.count}}
                        </td>
                        <!--<td>-->
                        <!--{{selectBrand.name}}-->
                        <!--</td>-->
                    </tr>
                    </tbody>
                </table>
            </div>
            <div class="no-data-content" v-show="!modelList || modelList.length <= 0">
                No Result
            </div>

            <!--<div class="table-footer">-->
                <!--<el-pagination-->
                    <!--@current-change='getModelList'-->
                    <!--:current-page.sync=currentPage-->
                    <!--:page-size="10"-->
                    <!--layout="prev, pager, next"-->
                    <!--:total="total">-->
                <!--</el-pagination>-->
                <!--<p class="total">Total: {{total}}</p>-->
            <!--</div>-->
        </div>
	</div>
</template>

<script>
    import Core from 'core/core'

	export default {
		name: "",
		data() {
			return {
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

                timeType: 0,
                timeList: [
                    {type: 0, label: 'all'},
                    {type: 1, label: 'Today'},
                    {type: 2, label: 'Yesterday'},
                    {type: 3, label: 'This week(mon.-today)'},
                    {type: 4, label: 'Last week(mon.-sun.)'},
                    {type: 5, label: 'This month'},
                    {type: 6, label: 'Last month'}
                ],

                brandList: [],
                brandId: '',

                dealerId: '',

                tableHead: ['Logo', 'Brand', 'Model', 'Sales Volume'],

                modelList: [],
                currentPage: 1,
                total: 0,

                orgId: 0,

            }
		},

		mounted: function () {
            this.orgId = Core.Data.getOrg().id;
            // this.getAllBrandList();
            this.dealerId = this.$route.query['dealer-id']
            // this.dealerId = this.$route.query['dealer-id']
            this.getModelList()
		},

		methods: {
            getAllBrandList() {
                Core.Api.Brand.getAllBrandList().then(res => {
                    this.brandList = res.brand_list;
                })
            },

            getModelList() {
                Core.Api.Brand.getDealerSalesDetail(this.orgId, this.dealerId).then(res => {
                    this.modelList = res.stat;
                    console.log(res);
                })
            }
        },
	}
</script>

<style lang="scss" rel="stylesheet/scss">
    .statistics-dealer {
        width: 100%;
        height: 100%;
    }
</style>



// WEBPACK FOOTER //
// dealer-sale-detail.vue?8c366812