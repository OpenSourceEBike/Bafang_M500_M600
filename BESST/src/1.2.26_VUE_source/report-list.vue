<template>
    <div class="report-list">
        <div class="header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item :label="$t('search.vehicleSn')">
                    <el-input v-model="search.vehicleSn"></el-input>
                </el-form-item>
                <div class="btn-group-inline">
                    <div class="icons">
                        <div class="icon-item search" @click="getReportList" :title="$t('search.searchBtn')"></div>
                    </div>
                </div>
            </el-form>
        </div>
        <div class="table-wrap">
            <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                <thead>
                <tr>
                    <td v-for="item in tableHead">{{item}} </td>
                </tr>
                </thead>
                <tbody>
                <tr v-for="item in reportList" @click="goToReport(item)">
                    <td>{{item.vehicle_sn}}</td>
                    <td>{{item.item_name}}</td>
                    <td>{{item.create_time | dateFormat}}</td>
                    <td>
                        <a class="btn-underline" @click.stop="showDeleteDialog(item)">{{ $t('delete') }}</a>
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="reportList.length <= 0">
                {{ $t('noResult') }}
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="pageChange"
                    :current-page.sync="currentPage"
                    :page-size="10"
                    layout="prev, pager, next"
                    :total="total">
                </el-pagination>
                <p class="total">{{ $t('total') }} {{total}}</p>
            </div>
        </div>
        <!--删除弹框-->
        <el-dialog
            :title="$t('deleteDialog.title')"
            :visible.sync="dialogDeleteVisible"
            size="tiny">
            <span>{{ $t('deleteDialog.content') }}</span>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteReport">{{ $t('yes') }}</el-button>
                 <el-button type="primary" @click="dialogDeleteVisible = false">{{ $t('no') }}</el-button>
            </span>
        </el-dialog>
    </div>
</template>

<script>
    import Core from "core/core";
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        data() {
            return {
                org: Core.Data.getOrg(),
                tableHead: ["Vehicle SN", "Model Name", "Time", "Operation"],

                reportList: [],
                total: 0,
                currentPage: Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) ? Core.Data.get(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE) : 1,

                search: {
                    vehicleSn: ''
                },

                dialogDeleteVisible: false,
                selectItem: ''
            }
        },
        i18n: {
            messages: {
                en: EN.Components.Diagnosis.reportList,
                zh: ZH.Components.Diagnosis.reportList,
                de: DE.Components.Diagnosis.reportList,
                nl: NL.Components.Diagnosis.reportList
            }
        },
        mounted: function () {
            // 国际化
            this.tableHead = [this.$t('tableHead.vehicleSn'), this.$t('tableHead.modelName'), this.$t('tableHead.time'), this.$t('tableHead.operation')];
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.tableHead = [vm.$t('tableHead.vehicleSn'), vm.$t('tableHead.modelName'), vm.$t('tableHead.time'), vm.$t('tableHead.operation')];
                }, 100);
            });
            this.getReportList();
        },

        methods: {
            pageChange(page) {
                this.currentPage = page;
                Core.Data.set(Core.Const.DATA.KEY_COMPONENT_ORDER_LIST_PAGE, this.currentPage);
                this.getReportList();
            },

            goToReport(report) {
                if (this.org.type === Core.Const.ORG_TYPE.DEALER) {
                    this.$router.push({name: "dealerDiagnosisReport", query: {id: report.id}});
                }
                if (this.org.type === Core.Const.ORG_TYPE.OEM) {
                    this.$router.push({name: "assemblerDiagnosisReport", query: {id: report.id}});
                }
                if (this.org.type === Core.Const.ORG_TYPE.BRAND) {
                    this.$router.push({name: "brandDiagnosisReport", query: {id: report.id}});
                }
                if (this.org.type === Core.Const.ORG_TYPE.COMPONENT) {
                    this.$router.push({name: "componentDiagnosisReport", query: {id: report.id}});
                }
                if (this.org.type === Core.Const.ORG_TYPE.SPECIAL_DEAlER) {
                    this.$router.push({name: "dealerDiagnosisReport", query: {id: report.id}});
                }
            },

            getReportList(){
                Core.Api.Vehicle.getReportListOfOrg(this.search.vehicleSn, this.currentPage).then(
                    res => {
                        this.reportList = res.report_list;
                        this.total = res.count;
                    }
                );
            },

            showDeleteDialog(item) {
                this.dialogDeleteVisible = true;
                this.selectItem = item;
            },

            deleteReport(){
                Core.Api.Vehicle.deleteReport(this.selectItem.id).then(res => {
                    this.$message({
                        message: 'Delete successfully',
                        type: 'success'
                    });
                    this.dialogDeleteVisible = false;
                    this.getReportList();
                })
            },
        }
    }
</script>

<style lang="scss">
    .report-list {
        height: 100%;
    }
</style>



// WEBPACK FOOTER //
// report-list.vue?ea08c7f4