<template>
	<div class="statistics-assembler-detail">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder" >
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <tr>
                        <td v-for="(item,index) in tableHead" >{{item}}</td>
                    </tr>
                    </thead>
                    <tbody v-if="!loading">
                    <tr class="large" v-for="item in List">
                        <td>{{item.create_time | dateFormat}}</td>
                        <td>{{item.sn}}</td>
                        <td>{{item.brand && item.brand.name}}</td>
                        <td>{{item.item && item.item.name}}</td>
                        <!--<td>-->
                        <!--{{selectBrand.name}}-->
                        <!--</td> v-if="loading" -->
                    </tr>
                    </tbody>
                </table>
            </div>
            <div class="no-data-content" v-show="List.length <= 0 && !loading">
                No Result
            </div>
            <div class="loading" >
                <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change='getDataList'
                    :current-page.sync=currentPage
                    :page-size="10"
                    layout="prev, pager, next"
                    :total="total">
                </el-pagination>
                <p class="total">Total: {{total}}</p>
            </div>
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
                timeType: '',

                brandList: [],
                brandId: '',

                customerId: '',

                tableHead: ['Date', 'SN', 'Brand', 'Model',],

                List: [],
                currentPage: 1,
                total: 0,

                orgId: 0,
                loading: false
            }
		},

		mounted: function () {
            this.orgId = Core.Data.getOrg().id;
            // this.getAllBrandList();
            this.timeType = this.$route.query['time_type'];
            this.customerId = this.$route.query['org_id'];
            // this.dealerId = this.$route.query['dealer-id']
            this.getDataList()
		},

		methods: {
            getModelText(arr) {
                if (!arr || arr.length === 0) {
                    return;
                }
                let itemList = [];
                let exModelText = "";
                let modelText = "";
                arr.forEach((item, index) => {
                    console.log(item)
                    exModelText = "";
                    itemList = item.item_list;
                    if (itemList !== null) {
                        itemList.forEach((model, idx) => {
                            let part_no = model.item_part_no ? model.item_part_no : '-';

                            if (idx === 0) {
                                modelText = model.item_name + ' (' + part_no + ')';
                            }
                            if (idx === 1) {
                                exModelText = model.item_name + ' (' + part_no + ')';
                            }
                            if (idx > 1) {
                                exModelText = exModelText + ', ' + model.item_name + ' (' + part_no + ')';
                            }
                        });
                        item['model_text'] = modelText;
                        item['ex_model_text'] = exModelText;
                    }
                });
                return arr;
            },

            getDataList() {
                this.loading = true;
                Core.Api.Brand.getEachAssembleStat(this.customerId, this.timeType, this.currentPage).then(res => {
                    this.List = res.list;
                    this.total = res.total;
                    this.loading = false;
                })
            }
        },
	}
</script>

<style lang="scss" rel="stylesheet/scss">
    .statistics-assembler-detail {
        width: 100%;
        height: 100%;
        .long-td {
            max-width: 300px;
            text-overflow: ellipsis;
            overflow: hidden;
        }
        .long-td div {
            max-width: 300px;
            text-overflow: ellipsis;
            overflow: hidden;
        }
        .loading {
            width: 100%;
            display: flex;
            align-items: center;
            justify-content: center;
        }
    }
</style>



// WEBPACK FOOTER //
// assembler-vehicle-detail.vue?f61bf186