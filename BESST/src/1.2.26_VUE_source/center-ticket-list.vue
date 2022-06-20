<template>
	<div class="service-center-ticket-list">
        <div class="header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item :label="$t('serviceCenter')">
                    <el-select v-model="search.center" placeholder="" @change="getTicketList">
                        <el-option
                            v-for="item in centerList"
                            :key="item.id"
                            :label="item.name"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="$t('category')">
                    <el-select v-model="search.category" placeholder="" @change="getTicketList">
                        <el-option
                            v-for="item in categoryList"
                            :key="item.id"
                            :label="item.value"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="$t('status')">
                    <el-select v-model="search.status" placeholder="" @change="getTicketList">
                        <el-option
                            v-for="item in statusList"
                            :key="item.id"
                            :label="item.value"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <td v-for="(item, index) in tableHead" :class="{'al-right': index === 0}">{{ item }}</td>
                    </thead>
                    <tbody>
                    <tr v-for="item in ticketList">
                        <td class="al-right">{{item.sn}}</td>
                        <td>{{item.title}}</td>
                        <td>{{item.target_sn}}</td>
                        <td>{{item.create_time | timeFormat("Y-m-d")}}</td>
                        <td>{{item.status | ticketStatus( Core.Data.getLang() )}}</td>
                        <td class="icon-td">
                            <div class="icons">
                                <div class="icon-item eye"  @click="goToTicketDetailPage(item)" :title=" $t('look')"></div>
                            </div>
                            <!-- <el-button class="btn-underline" type="text" @click="goToTicketDetailPage(item)">{{ $t('look') }}
                            </el-button> -->
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="ticketList==false">
                    {{ $t('noResult') }}
                </div>
            </div>
            <div class="table-footer">
                <el-pagination
                    @current-change="handleCurrentChange"
                    :current-page.sync="currentPage"
                    :page-size="10"
                    layout="prev, pager, next"
                    :total="total">
                </el-pagination>
                <p class="total">{{ $t('total') }} {{total}}</p>
            </div>
        </div>
	</div>
</template>

<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

	export default{
		name: "serviceCenterTicketList",
		data(){
			return {
                Core: Core,
				org:Core.Data.getOrg(),

                tableHead: ['Service No.', 'Title', 'Target SN', 'Create Time', 'Status', 'Operation'],
                currentPage: 1,
                ticketList: [],
                total: 0,

                // ticket
                ticket: {
                    category: '',
                    title: '',
                    description: '',
                    targetId: '',
                    targetSn: '',
                    serviceCenterOrgId: ''
                },

                categoryList: [
                    {id: 0, value: 'All'},
                    {id: 1, value: 'Vehicle'},
                    {id: 2, value: 'Motor'},
                    {id: 3, value: 'HMI'},
                    {id: 4, value: 'Sensor'},
                    {id: 5, value: 'Battery'},
                    {id: 6, value: 'Controller'},
                    {id: 7, value: 'Light'},
                    {id: 8, value: 'Other'}
                ],
                statusList: [
                    {id: -1, value: 'All'},
                    {id: 100, value: 'Pending Assign'},
                    {id: 200, value: 'Processing'},
                    {id: 300, value: 'Complete'},
                ],
                centerList: [],
                search: {
                    category: 0,
                    status: -1,
                    center: '',
                },
            }
		},
        i18n: {
            messages: {
                en: EN.Components.ServiceCenter.centerTicketList,
                zh: ZH.Components.ServiceCenter.centerTicketList,
                de: DE.Components.ServiceCenter.centerTicketList,
                nl: NL.Components.ServiceCenter.centerTicketList
            }
        },
		mounted: function () {
		    // 国际化
            this.initData();
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });
            this.getCenterList();
		},

		methods: {
            initData() {
                this.tableHead = [this.$t('tableHead.serviceNo'), this.$t('tableHead.title'), this.$t('tableHead.targetSn'), this.$t('tableHead.createTime'), this.$t('tableHead.status'), this.$t('tableHead.operation')];
                this.categoryList = [
                    {id: 0, value: this.$t('categoryList.all')},
                    {id: 1, value: this.$t('categoryList.vehicle')},
                    {id: 2, value: this.$t('categoryList.motor')},
                    {id: 3, value: this.$t('categoryList.hmi')},
                    {id: 4, value: this.$t('categoryList.sensor')},
                    {id: 5, value: this.$t('categoryList.battery')},
                    {id: 6, value: this.$t('categoryList.controller')},
                    {id: 7, value: this.$t('categoryList.light')},
                    {id: 8, value: this.$t('categoryList.other')}
                ];
                this.statusList = [
                    {id: -1, value: this.$t('statusList.all')},
                    {id: 100, value: this.$t('statusList.pendingAssign')},
                    {id: 200, value: this.$t('statusList.processing')},
                    {id: 300, value: this.$t('statusList.complete')},
                ];
            },

            handleCurrentChange(val) {
                this.currentPage = val;
                this.getTicketList();
            },

            getTicketList(){
            	if(!this.centerList.length > 0){
            		return;
                }
                Core.Api.Ticket.getTicketListOfAssignedToServiceCenter(this.search.center, this.search.category, this.search.status, this.currentPage).then(res => {
                    this.ticketList = res.list;
                    this.total = res.count;
                });
            },

            takeTicket(ticket){
                Core.Api.Ticket.takeTicket(ticket.id).then(res => {
                    console.log(res);
                    this.getTicketList();
                });
            },

            goToTicketDetailPage(ticket) {
                this.$router.push({path: "ticket-detail", query: {id: ticket.id}})
            },

            getCenterList(){
                Core.Api.Org.allServiceCenterList(this.org.id).then(res => {
                	if(res.org_list.length > 0){
                        this.centerList = res.org_list;
                        this.search.center = this.centerList[0].id;
                        this.getTicketList();
                    }
                });
            }
        },
	}
</script>

<style lang="scss" rel="stylesheet/scss">
.service-center-ticket-list{
    height: 100%;
    display:flex;
    flex-direction:column;
    .dz-started {
        display: inline-block;
        width: 120px;
        height: 120px;
        .dz-message {
            width: 100%;
            height: 100%;
            img {
                width: 120px;
                height: 120px;
            }
        }
    }
    .icon-td{
        display: flex;
        align-items: center;
        flex-direction: row;
        height:80px;
        .icons{
            align-self: center;
            width:35px;
            height:35px;
            padding-bottom:36px;
        }
    }

    .img-list {
        float: left;
        img {
            width: 120px;
            height: 120px;
            margin-right: 10px;
        }
    }
    .el-upload-list__item img {
        display: none;
    }

    .el-form-item__label {
        color: #A1A5B1;
    }
    .table-wrap{
        flex:1;
    }
    .header {
        height: auto;
        .el-form-item {
            margin-bottom: 0;
        }
    }

}
</style>



// WEBPACK FOOTER //
// center-ticket-list.vue?3bfe99d3