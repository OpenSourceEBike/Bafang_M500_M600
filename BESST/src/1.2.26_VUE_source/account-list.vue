<template>
    <div class="server-center-account-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()"></div>
                </div>
            </div>

            <div class="btn-group">
                <div class="icons">
                    <div class="icon-item add" @click="addDialogVisible = true"></div>
                </div>
            </div>
        </div>
        <div class="table-wrap">
            <table cellspacing="0" cellpadding="0" class="table-content">
                <thead>
                <td class="td" v-for="item in tableHead">{{item}}</td>
                </thead>
                <tbody>
                <tr v-for="item in accountList">
                    <td>{{item.user.email}}
                    </td>
                    <td>{{item.create_time | timeFormat('Y-m-d')}}</td>
                    <td class="icon-td">
                        <div class="icons">
                            <div class="icon-item resetPassword"  v-on:click.stop="showResetDialog(item)"></div>
                        </div>
                        <div class="icons">
                            <div class="icon-item delete" v-on:click.stop="showDeleteDialog(item)"></div>
                        </div>
                        <!-- <div>
                            <a class="btn-underline" v-on:click.stop="showResetDialog(item)">{{ $t('reset') }}</a>
                            <a class="btn-underline" v-on:click.stop="showDeleteDialog(item)">{{ $t('delete') }}</a>
                        </div> -->
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="accountList==false">
                {{ $t('noResult') }}
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

        <!-- 添加账号弹框 -->
        <el-dialog :title="$t('addAccountDialog.title')" :visible.sync="addDialogVisible">
            <div class="form-item">
                <label class="label">{{ $t('addAccountDialog.email') }}</label>
                <el-input v-model="email" class="input"></el-input>
            </div>
            <div class="form-item">
                <label class="label">{{ $t('addAccountDialog.password') }}</label>
                <el-input v-model="password" class="input"></el-input>
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="addAccount">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="addDialogVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

        <!-- 删除账号弹框 -->
        <el-dialog :title="$t('deleteDialog.title')" :visible.sync="deleteDialogVisible" size="tiny">
            <p> {{ $t('deleteDialog.content') }} </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteAccount">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="deleteDialogVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>

        <!--重置密码弹框-->
        <el-dialog :title="$t('resetPasswordDialog.title')" :visible.sync="resetDialogVisible">
            <p> {{ $t('resetPasswordDialog.content') }} </p>
            <P v-if="selectItem">{{selectItem.user.email}}</P>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="resetPassword">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="resetDialogVisible = false">{{ $t('cancel') }}</el-button>
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

    export default{
        data(){
            return {
                orgId: '',
                Core: Core,
                tableHead: ['Account', 'Create Time', 'Operation'],
                currentPage: 1,
                total: 0,
                accountList: [],
                email: '',
                password: '',
                selectItem: '',
                addDialogVisible: false,
                resetDialogVisible: false,
                deleteDialogVisible: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Components.ServiceCenter.accountList,
                zh: ZH.Components.ServiceCenter.accountList,
                de: DE.Components.ServiceCenter.accountList,
                nl: NL.Components.ServiceCenter.accountList
            }
        },
        mounted: function () {
            // 国际化
            this.tableHead = [this.$t('tableHead.account'), this.$t('tableHead.createTime'), this.$t('tableHead.operation')];
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.tableHead = [vm.$t('tableHead.account'), vm.$t('tableHead.createTime'), vm.$t('tableHead.operation')];
                }, 100);
            });
            this.orgId = this.$route.query.id;
            this.getAccountList();
        },
        methods: {
            handleCurrentChange(val) {
                this.currentPage = val;
                this.getAccountList();
            },
            getAccountList() {
                Core.Api.Org.getServiceCenterAccountList(this.orgId, this.currentPage).then(res => {
                    this.accountList = res.list;
                    this.total = res.count;
                });
            },

            addAccount(){
                if (!this.email) {
                    this.$message({
                        message: "E-mail is empty",
                        type: 'warning'
                    });
                    return;
                }
                if (!this.password) {
                    this.$message({
                        message: "Password is empty",
                        type: 'warning'
                    });
                    return;
                }

                Core.Api.Org.createServiceCenterAccount(this.orgId, this.email, this.password).then(res => {
                    this.addDialogVisible = false;
                    this.$message({
                        message: 'Add successfully',
                        type: 'success'
                    });
                    this.email = '';
                    this.password = '';
                    this.getAccountList();
                });
            },

            showDeleteDialog(item){
                this.deleteDialogVisible = true;
                this.selectItem = item;
            },

            deleteAccount(){
                Core.Api.Org.deleteServiceCenterAccount(this.selectItem.id).then(res => {
                    this.$message({
                        message: 'Delete successfully',
                        type: 'success'
                    });
                    this.getAccountList();
                });
                this.deleteDialogVisible = false;
            },

            showResetDialog(item){
                this.resetDialogVisible = true;
                this.selectItem = item;
            },


            resetPassword(){
                Core.Api.Org.resetServiceCenterAccount(this.selectItem.user_id).then(res => {
                    this.$message({
                        message: 'Reset successfully',
                        type: 'success'
                    });
                    this.getAccountList();
                });
                this.resetDialogVisible = false;
            },


        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .server-center-account-list {
        height: 100%;
        .btn-group {
            top: 10px;
        }
        .el-dialog__body {
            padding-bottom: 10px;
            .form-item {
                .label {
                    display: inline-block;
                    width: 100px;
                }
                .input {
                    width: 310px;
                    margin: 10px;
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

    }
</style>



// WEBPACK FOOTER //
// account-list.vue?8fe66b7a