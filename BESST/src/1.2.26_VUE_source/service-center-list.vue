<template>
    <div class="brand-service-center-list">
        <div class="header">
            <label class="title">{{ $t('serviceCenter') }}</label>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add" @click="showAddDialog"
                         v-if="authList[Core.Const.AUTH_LIST.ticketSCAdd]"></div>
                </div>
            </div>
        </div>
        <div class="table-wrap">
            <div class="table-content-holder">
                <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                    <thead>
                    <td v-for="item in tableHead">{{item}}</td>
                    </thead>
                    <tbody>
                    <tr @click="showDetailDialog(item)" v-for="item in orgList">
                        <!-- <td>
                            <img class="img" :src="item.avatar | getImgUrl"/>
                        </td> -->
                        <td>{{item.name}}</td>
                        <td>{{item.phone}}</td>
                        <td>{{item.email}}</td>
                        <td>{{item.country}}</td>
                        <td>{{item.address}}</td>
                        <td class="icon-td">
                            <div class="icons">
                                <div class="icon-item edit"  @click.stop="showEditDialog(item)" v-if="authList[Core.Const.AUTH_LIST.ticketSCEdit]" :title="$t('op.edit')"></div>
                            </div>
                            <div class="icons">
                                <div class="icon-item account" @click.stop="goToAccountList(item)" :title="$t('op.account')"></div>
                            </div>
                            <div class="icons">
                                <div class="icon-item delete" @click.stop="showDeleteDialog(item)" :title="$t('op.delete')"></div>
                            </div>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="orgList.length <= 0">
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
                <p class="total">{{ $t('total') }}: {{total}}</p>
            </div>
        </div>
        <!-- 删除弹窗 -->
        <el-dialog :title="$t('deleteDialog.title')" :visible.sync="dialogDeleteCenterVisible" size="tiny">
            <p> {{ $t('deleteDialog.content') }} </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteServiceCenter">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="dialogDeleteCenterVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>

        <!--  Edit弹窗 -->
        <el-dialog :title="dialogText" :visible.sync="dialogEditServiceCenterVisible" custom-class="edit-dialog"
                   class="dialog-company">


            <!-- <div class="company-item">
                <label class="title logo">{{ $t('editDialog.picture') }}</label>
                <div class="content" :class="upload.photographSrc ? 'edit-img-box' : '' ">
                    <el-upload
                        :action=upload.url
                        :show-file-list="false"
                        :on-success="imageUploaded"
                        :data="upload.data">
                        <img class="upload-icon logo-img" v-if="upload.photographSrc"
                             :src="upload.photographSrc | getImgUrl"/>
                        <img v-else class="upload-icon"
                             src="../../assets/images/icon/icon-upload@2x.png"/>
                    </el-upload>
                </div>
            </div>
            <div class="company-item">
                <span class="logo-tip">
                    <label>{{ $t('editDialog.tip') }}:</label> <br/>
                    <label>{{ $t('editDialog.content01') }}</label> <br/>
                    <label>{{ $t('editDialog.content02') }}</label>
                </span>
            </div> -->
            <div class="clear-10"></div>
            <div class="company-item">
                <label class="title">{{ $t('editDialog.name') }}</label>
                <div class="content">
                    <el-input v-model="selectOrg.name"></el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('editDialog.email') }}</label>
                <div class="content">
                    <el-input v-model="selectOrg.email"></el-input>
                </div>
            </div>

            <!-- <div class="company-item">
                <label class="title">{{ $t('editDialog.website') }}</label>
                <div class="content">
                    <el-input v-model="selectOrg.website"></el-input>
                </div>
            </div> -->
            <div class="company-item">
                <label class="title">{{ $t('editDialog.address') }}</label>
                <div class="content">
                    <el-input v-model="selectOrg.address">
                    </el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('editDialog.postcode') }}</label>
                <div class="content">
                    <el-input v-model="selectOrg.postcode">
                        <template slot="append"></template>
                    </el-input>
                </div>
            </div>
            <div class="company-item">
                <label class="title">{{ $t('editDialog.phone') }}</label>
                <div class="content">
                    <el-input v-model="phoneNumber">
                        <el-select v-model="areaCode" slot="prepend" filterable popper-class="phone">
                            <el-option
                                v-for="(item, index) in areaCodeList"
                                :key="index"
                                :label="item.area_code"
                                :value="item.area_code">
                            </el-option>
                        </el-select>
                    </el-input>
                </div>
            </div>
            <p class="declare">
                NOTE:&nbsp;请如实填写以上信息，以便及时沟通，准确投送物流包裹等。
            </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="editServiceCenter">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogEditServiceCenterVisible = false">{{ $t('cancel') }}</el-button>
            </div>

        </el-dialog>

        <!--Detail弹窗-->
        <el-dialog :title="$t('detailDialog.title')" :visible.sync="dialogDetailServiceCenterVisible"
                   custom-class="edit-dialog"
                   class="dialog-company">
            <!-- <div class="detail-item img-item">
                <label class="title logo">{{ $t('detailDialog.picture') }}</label>
                <div class="content">
                    <img class="upload-icon"
                         :src="selectOrg.avatar | getImgUrl"/>
                </div>
            </div> -->
            <div class="clear-10"></div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.name') }}</label>
                <div class="content">
                    <label class="label">{{selectOrg.name}}</label>
                </div>
            </div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.email') }}</label>
                <div class="content">
                    <label class="label">{{selectOrg.email}}</label>
                </div>
            </div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.phone') }}</label>
                <div class="content">
                    <label class="label">{{selectOrg.phone}}</label>
                </div>
            </div>
            <!-- <div class="detail-item">
                <label class="title">{{ $t('detailDialog.website') }}</label>
                <div class="content">
                    <label class="label">{{selectOrg.website}}</label>
                </div>
            </div> -->
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.address') }}</label>
                <div class="content">
                    <!-- <label class="label">{{selectOrg.country}} {{selectOrg.address}}</label> -->
                    <label class="label">{{selectOrg.address}}</label>
                </div>
            </div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.postcode') }}</label>
                <div class="content">
                    <label class="label">{{selectOrg.postcode}} </label>
                </div>
            </div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.ticketCount') }}</label>
                <div class="content">
                    <label class="label">{{ticketCount}}</label>
                </div>
            </div>
            <div class="detail-item">
                <label class="title">{{ $t('detailDialog.aveHandleTime') }}</label>
                <div class="content">
                    <label class="label">{{averageTime}}</label>
                </div>
            </div>
        </el-dialog>
    </div>

</template>

<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import areaCodeInDB from 'core/areaCode'

    export default {
        name: "serviceCenterList",
        data() {
            return {
                Core: Core,

                upload: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    data: {
                        token: Core.Data.getToken()
                    },
                    photographSrc: Core.Data.getOrg().avatar,
                },

                dialogEditServiceCenterVisible: false,
                dialogDeleteCenterVisible: false,
                dialogDetailServiceCenterVisible: false,
                tableHead: ['Name', 'Phone', 'E-mail', 'Country', 'Address', 'Operation'],
                currentPage: 1,
                total: 0,

                orgList: [],
                selectOrg: {
                    country: '',
                },

                dialogText: 'Create Service Center',
                ticketCount: '',
                averageTime: '',

                authList: [],
                areaCode: '',
                phoneNumber: '',
                areaCodeList: [],
            }
        },
        i18n: {
            messages: {
                en: EN.Components.ServiceCenter.serviceCenterList,
                zh: ZH.Components.ServiceCenter.serviceCenterList,
                de: DE.Components.ServiceCenter.serviceCenterList,
                nl: NL.Components.ServiceCenter.serviceCenterList
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
            this.areaCodeList = areaCodeInDB.init();
            // 获取权限
            this.authList = this.$store.state.userAuth;

            this.getServiceCenterList();
        },
        methods: {
            initData() {
                this.dialogText = this.$t('createSC');
                this.tableHead = [this.$t('tableHead.name'), this.$t('tableHead.phone'), this.$t('tableHead.email'), this.$t('tableHead.country'), this.$t('tableHead.address'), this.$t('tableHead.operation')];
            },

            handleCurrentChange(val) {
                this.currentPage = val;
                this.getServiceCenterList();
            },

            getServiceCenterList() {
                Core.Api.Org.getServiceCenterList(this.currentPage).then(res => {
                    console.log(res);
                    this.orgList = res.list;
                    this.total = res.count;
                })
            },

            checkParams() {
                if (!this.selectOrg.name) {
                    this.$message({
                        message: 'Please input service center name',
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.selectOrg.email) {
                    this.$message({
                        message: 'Please input service center email',
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.phoneNumber) {
                    this.$message({
                        message: 'Please input service center phone',
                        type: 'warning'
                    });
                    return false;
                }
                if (!this.selectOrg.address) {
                    this.$message({
                        message: 'Please input service center address',
                        type: 'warning'
                    });
                    return false;
                }
//                if (!this.upload.photographSrc) {
//                    this.$message({
//                        message: 'Please upload service center logo',
//                        type: 'warning'
//                    });
//                    return false;
//                }
                return true;
            },

            editServiceCenter() {
                if (!this.checkParams()) {
                    return;
                }
                let message = 'Edit successfully';
                if (!this.selectOrg.id) {
                    this.selectOrg.id = 0;
                    message = 'Add successfully';
                }
                let phone = `${this.areaCode} ${this.phoneNumber}`;
                Core.Api.Org.createServiceCenter(
                    this.selectOrg.id,
                    this.selectOrg.name,
                    this.selectOrg.email,
                    this.selectOrg.country,
                    this.selectOrg.address,
                    this.selectOrg.postcode,
                    phone,
                    this.selectOrg.website,
                    this.upload.photographSrc,
                ).then(res => {
                    this.dialogEditServiceCenterVisible = false;
                    this.$message({
                        message: message,
                        type: 'success'
                    });
                    this.getServiceCenterList();
                });
            },

            deleteServiceCenter() {
                Core.Api.Org.deleteServiceCenter(this.selectOrg.id).then(res => {
                    this.getServiceCenterList();
                    this.$message({
                        message: 'Delete successfully',
                        type: 'success'
                    });
                    this.dialogDeleteCenterVisible = false;
                });
            },

            imageUploaded(res) {
                this.upload.photographSrc = res.data.file.name;
            },

            showAddDialog() {
                this.selectOrg = {country: ''};
                this.dialogText = this.$t('createSC');
                this.upload.photographSrc = '';
                this.dialogEditServiceCenterVisible = true;
                this.areaCode = "";
                this.phoneNumber ="";
            },

            showEditDialog(org) {
                this.selectOrg = org;
                this.dialogText = this.$t('editSC');
                this.upload.photographSrc = org.avatar;
                this.areaCode = "";
                this.phoneNumber ="";
                if (org.phone) {
                    let phoneArr = org.phone.split(" ");
                    this.areaCode = phoneArr[0];
                    this.phoneNumber = phoneArr[1];
                }
                this.dialogEditServiceCenterVisible = true;
            },

            showDeleteDialog(org) {
                this.selectOrg = org;
                this.dialogDeleteCenterVisible = true;
            },

            showDetailDialog(org) {
                this.selectOrg = org;
                this.dialogEditServiceCenterVisible = false;
                this.dialogDetailServiceCenterVisible = true;

                this.getStatOfServiceCenter();
            },

            goToAccountList(org) {
                let orgId = org.id;
                this.$router.push({path: 'account-list', query: {id: orgId}});
            },

            getStatOfServiceCenter() {
                this.ticketCount = '';
                this.averageTime = '';
                Core.Api.Ticket.getStatOfServiceCenter(this.selectOrg.id).then(res => {
                    this.ticketCount = res.stat_ticket_count;
                    this.averageTime = res.stat_average_handle_time;
                });
            }

        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .brand-service-center-list {
        width: 100%;
        height: 100%;
        .table-wrap .table-content .img {
            width: auto;
        }
        .el-input-group__prepend .el-select .el-input .el-input__inner {
            width: 110px;
        }
        .declare{
            transform: translateY(15px);
            font-size:13px;
            color:#fff;
            font-weight: bold;
            margin-top:40px;
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

        .company-item {
            .label {
                color: white;
            }
            .logo {
                vertical-align: top;
                margin-top: 10px;
            }
            .upload-icon {
                margin-top: 10px;
                max-width: 142px;
                max-height: 142px;
            }
            .title {
                width: 120px;
            }
            .phone{
                width:80px;
            }
            .el-input-group__prepend{
                .el-input__inner {
                    text-align: left;
                    width: 90px !important;
                }
            }
        }
        .stat-item {
            display: inline-block;
            width: 50%;
            .title {
                display: inline-block;
                width: 82px;
            }

            .content {
                display: inline-block;
                height: 40px;
                line-height: 40px;
                width: 260px;
                margin-left: 8px;
                vertical-align: top;
                .label {
                    color: white;
                }
            }

        }

    }

</style>



// WEBPACK FOOTER //
// service-center-list.vue?10dc225e