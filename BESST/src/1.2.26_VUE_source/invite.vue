<template>
    <div class="dealer-invite">
        <div class="header">
            <label class="status-label">{{ $t('codeStatus') }}</label>
            <el-select v-model="selectStatus" slot="append" @change="getInviteList()">
                <el-option
                    v-for="item in options"
                    :key="item.status"
                    :label="item.label"
                    :value="item.status">
                </el-option>
            </el-select>
            <!--搜索-->
            <el-input class="search-input" v-model="queryContent">
                <!--<el-button slot="append" icon="search" :click="queryOrgInvite"></el-button>-->
            </el-input>
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item search" @click="getInviteList"></div>
                </div>
            </div>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item upload" @click="dialogContactsVisible = true" :title="$t('uploadContacts')"></div>
                    <div class="icon-item add" @click="dialogCreateVisible = true" :title="$t('newCode')"></div>
                    <div class="icon-item send" v-show="selectStatus!=3" @click="showBatchDialog" :title="$t('batchSend')"></div>
                </div>
            </div>
        </div>
        <!--<div>-->
            <!--<el-form class="invitation-code">-->
                <!--<el-form-item label="获取邀请码">-->
                    <!--<el-input class="input" v-model="getInvitationCode" placeholder="邀请码" :disabled="true"></el-input>-->
                    <!--<el-button @click="getInvitationCodeByUserId">获取/更新</el-button>-->
                    <!--<el-button -->
                        <!--v-clipboard:copy="getInvitationCode"-->
                        <!--v-clipboard:success="onCopy"-->
                        <!--v-clipboard:error="onError">复制</el-button>-->
                    <!--<el-button @click="deleteInvitationCodeByUserId">删除</el-button>-->
                    <!---->
                <!--</el-form-item>-->
            <!--</el-form>-->
        <!--</div>-->
        <div class="table-wrap">
            <div class="table-content-holder">
                <table border="0" cellspacing="0" cellpadding="0" class="table-content">
                    <thead>
                    <td v-for="item in tableHead">{{item}}</td>
                    <td v-if="selectStatus != 3">{{ $t('operation') }}</td>
                    </thead>
                    <tbody>
                    <tr v-for="item in inviteList">
                        <td>{{item.code}}</td>
                        <td>{{item.target_org_name}}</td>
                        <td>{{item.contact}}</td>
                        <td>{{item.target_org_email}}</td>
                        <td>{{item.target_org_type === 2 ? $t('dealerType1') : $t('dealerType2')}}</td>
                        <td>{{item.create_time | timeFormat('Y-m-d')}}</td>
                        <td v-if='selectStatus == 1' class="icon-td">
                            <div class="icons">
                                <div class="icon-item send" v-on:click.stop="sendInvitation(item)"></div>
                            </div>
                            <div class="icons">
                                <div class="icon-item delete" v-on:click.stop="showDeleteDialog(item)"></div>
                            </div>
                            <!-- <a class="btn-underline" v-on:click.stop="sendInvitation(item)">{{ $t('send') }}</a> -->
                            <!-- <a class="btn-underline" v-on:click.stop="showDeleteDialog(item)">{{ $t('delete') }}</a> -->
                        </td>
                        <td v-if='selectStatus == 2' class="icon-td">
                            <div class="icons">
                                <div class="icon-item send" v-on:click.stop="sendInvitation(item)" :title="$t('op.send')"></div>
                            </div>
                            <div class="icons">
                                <div class="icon-item delete" v-on:click.stop="showDeleteDialog(item)" :title="$t('op.delete')"></div>
                            </div>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div class="no-data-content" v-show="inviteList.length <= 0">
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

        <!--创建邀请码dialog-->
        <el-dialog :title="$t('dealerInvitation')" :visible.sync="dialogCreateVisible" class="dialog">
            <el-form ref="form" :model="form" label-width="120px">
                <el-form-item :label=" $t('email') ">
                    <el-input v-model="form.email" class="input"></el-input>
                </el-form-item>
                <el-form-item :label="$t('companyName')">
                    <el-input v-model="form.orgName" class="input"></el-input>
                </el-form-item>
                <el-form-item :label="$t('dealerType')">
                    <el-select v-model="orgType" @change="checkDealerType()">
                        <el-option
                            v-for="item in orgTypeOptions"
                            :key="item.status"
                            :label="item.label"
                            :value="item.status">
                        </el-option>
                    </el-select>
                    <p class="dealerTypeNote">{{$t('dealerTypeNote')}}</p>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="createInvite" :disabled="orgType == 0">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogCreateVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

        <!--批量发送dialog-->
        <el-dialog :title="$t('selectAccount')" :visible.sync="dialogBatchSendVisible" class="batch-send-dialog">
            <div>
                <el-transfer
                    class="transfer"
                    v-model="targetName"
                    filterable
                    :filter-placeholder= "$t('enterKeyWord')"
                    :titles="[ $t('source') , $t('target')]"
                    :button-texts="[$t('toLeft'), $t('toRight')]"
                    :footer-format="{
                        noChecked: '${total}',
                        hasChecked: '${checked}/${total}'
                    }"
                    @change="handleChange"
                    :data="batchSendData">
                    <el-button class="transfer-footer" slot="right-footer" size="small" @click="batchSend">
                        {{ $t('batchSend') }}
                    </el-button>
                </el-transfer>
            </div>
        </el-dialog>

        <!-- 删除弹窗 -->
        <el-dialog :title="$t('deleteCode')" :visible.sync="dialogDeleteVisible" size="tiny">
            <p> {{$t('deleteTip')}} </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteInvitation">{{$t('yes')}}</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">{{$t('no')}}</el-button>
            </div>
        </el-dialog>

        <!-- 上传失败弹窗 -->
        <el-dialog :title="$t('uploadFailList')" custom-class="video-dialog" :visible.sync="dialogUploadFailVisible">
            <table border="1">
                <tr>
                    <td>{{$t('orgName')}}</td>
                    <td>{{$t('email')}}</td>
                    <td>{{$t('reason')}}</td>
                </tr>
                <tr v-for="item in failList">
                    <td>{{item.org_name}}</td>
                    <td>{{item.org_email}}</td>
                    <td>{{item.reason}}</td>
                </tr>
            </table>
        </el-dialog>

        <!--联系人dialog-->
        <el-dialog :title="$t('uploadContacts')" :visible.sync="dialogContactsVisible" class="dialog">
            <div class="pt-30">
                <label>{{ $t('template') }}</label>
                <a class="btn-underline" href="/static/Contacts_format_for_BESST.xlsx" download="Contacts_format_for_BESST.xlsx">{{ $t('download') }}</a>
            </div>
            <div class="pt-30">
                <label>{{ $t('dealerType') }}</label>
                <span>
                    <el-select v-model="orgType" @change="checkDealerType()">
                        <el-option
                            v-for="item in orgTypeOptions"
                            :key="item.status"
                            :label="item.label"
                            :value="item.status">
                        </el-option>
                    </el-select>
                </span>
                <p class="dealerType">{{ $t('dealerTypeNote') }}</p>
            </div>
            <el-upload
                class="upload-contact"
                :action="uploadOptions.url"
                :data="uploadOptions"
                :show-file-list="false"
                :on-success="handleUploadContact"
                :on-change="changeLoading">
                <el-button :disabled="orgType == 0">Upload</el-button>
                <!--<div class="icon-item upload" :title="$t('uploadContacts')"></div>-->
            </el-upload>
            <vue-loading class="loading" type="spin" color="#E26829" v-if="loading"></vue-loading>
        </el-dialog>
    </div>
</template>

<script>
    import Core from '../../../../core/core'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    import vueLoading from 'vue-loading-template'
    import ElButton from "../../../../node_modules/element-ui/packages/button/src/button.vue";

    export default {
        name: 'brandOrderInvitation',
        components: {
            ElButton,
            vueLoading
        },
        data() {
            return {
                Core: Core,
                tableHead: ['Invitation Code', 'Organization Name', 'Contacts', 'Target Email', 'Dealer Type', 'Create Time'],
                inviteList: [],
                currentPage: 1,
                selectStatus: 1,
                total: 0,

                form: {
                    email: '',
                    orgName: ''
                },
                queryContent: '',

                // 删除弹框
                dialogDeleteVisible: false,
                // 创建弹框
                dialogCreateVisible: false,
                // 批量发送弹框
                dialogBatchSendVisible: false,
                // 上传失败弹框
                dialogUploadFailVisible: false,
                // 联系人弹框
                dialogContactsVisible: false,

                selectDeleteInvitation: '',

                failList: [],
                notSendList: [],
                batchSendData: [],
                batchSendJson: '',
                targetName: [],

                orgType: 0,
                orgTypeOptions: [
                    {
                        status: 0,
                        label: 'Select'
                    },
                    {
                        status: Core.Const.ORG_TYPE.DEALER,
                        label: 'Dealer'
                    },
                    {
                        status: Core.Const.ORG_TYPE.SPECIAL_DEAlER,
                        label: 'Special Dealer'
                    },
                ],

                uploadOptions: {
                    url: Core.Const.NET.UPLOAD_CONTACT_URL,
                    token: Core.Data.getToken(),
                    org_type: ''
                },
                user: Core.Data.getUser() ? Core.Data.getUser() : {id:""},

                options: [
                    {
                        status: 1,
                        label: 'ready to send'
                    },
                    {
                        status: 2,
                        label: 'sent, but not used'
                    },
                    {
                        status: 3,
                        label: 'used'
                    }
                ],

                loading: false,
                getInvitationCode: ''
            }
        },
        i18n: {
            messages: {
                en: EN.Brand.DealerInvite,
                zh: ZH.Brand.DealerInvite,
                de: DE.Brand.DealerInvite,
                nl: NL.Brand.DealerInvite
            }
        },
        mounted: function () {
            this.initData();
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    this.initData();
                }, 100);
            });
            this.getInviteList();
            this.getInitInvitationCode();
        },
        methods: {
            onCopy(e) {
                this.$message({
                    type: 'success',
                    message: 'copy successfully!'
                });
            },
            onError(e) {
               this.$message({
                    type: 'warning',
                    message: 'copy failed!'
                });
            },
            getInitInvitationCode(){
                Core.Api.Org.selectInvitationCode(this.user.id).then(res=>{
//                    console.log(res);
                    if(res.code){
                        this.getInvitationCode = res.code;
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },
            initData() {
                this.tableHead = [
                    this.$t('tableHead.invitation'),
                    this.$t('tableHead.targetName'),
                    this.$t('tableHead.contact'),
                    this.$t('tableHead.targetEmail'),
                    this.$t('dealerType'),
                    this.$t('tableHead.createTime')
                ];

                this.options = [
                    {
                        status: 1,
                        label: this.$t('codeStatusOptions.readyToSend'),
                    },
                    {
                        status: 2,
                        label: this.$t('codeStatusOptions.sentNotUsed'),
                    },
                    {
                        status: 3,
                        label: this.$t('codeStatusOptions.used'),
                    }
                ];

                this.orgTypeOptions[0].label = this.$t('op.select');
                this.orgTypeOptions[1].label = this.$t('dealerType1');
                this.orgTypeOptions[2].label = this.$t('dealerType2');
            },

            handleCurrentChange(val) {
                this.currentPage = val;
                this.getInviteList();
            },

            // 获取邀请列表
            getInviteList() {
                Core.Api.Brand.getOrgDealerInvitationList(this.selectStatus, this.queryContent, this.currentPage).then(res => {
                    this.inviteList = res.invite_list;
                    this.total = res.count;
                });
            },

            checkDealerType() {
                this.uploadOptions.org_type = this.orgType;
            },

            // 创建弹框上点击confirm按钮
            createInvite() {
                let vm = this;
                if (this.orgType === 0){
                    vm.$message({
                        message: this.$t('dealerTypeTip'),
                        type: 'warning',
                    });
                    return;
                }
                if (!vm.form.email) {
                    vm.$message({
                        message: this.$t('emptyTip.email'),
                        type: 'warning',
                    });
                    return;
                }
                if (!vm.form.orgName) {
                    vm.$message({
                        message: this.$t('emptyTip.name'),
                        type: 'warning',
                    });
                    return;
                }

                Core.Api.Brand.invitationEmailSend(vm.form.orgName, vm.orgType, vm.form.email.replace(/\s+/g,""),2).then(res => {
                    vm.$message({
                        message: vm.$t('sendSuccess'),
                        type: 'success',
                    });
                    vm.form = {
                        email: '',
                        orgName: ''
                    };
                    vm.dialogCreateVisible = false;
                    vm.getInviteList();
                });
            },


            changeLoading() {
                this.loading = !this.loading;
            },

            // 上传联系人成功回调
            handleUploadContact(file) {
                if (file.code !== 0) {
                    this.$message({
                        message: this.$t('uploadFailTip'),
                        type: 'warning',
                    });
                    return;
                }
//                console.log(file);
                this.failList = file.data.fail_list;
                if (this.failList.length <= 0) {
                    this.$message({
                        message: this.$t('uploadSuccess'),
                        type: 'success',
                    });
                    this.dialogContactsVisible = false;
                } else {
                    this.dialogUploadFailVisible = true;
                }
                this.getInviteList();
            },

            handleChange(value, direction, movedKeys) {
                this.batchSendJson = value.join();
            },

            getAllList() {
                Core.Api.Brand.getOrgInvitationAllList(Core.Const.ORG_TYPE.DEALER, this.selectStatus).then(res => {
                    this.notSendList = res.invite_list;
                    this.generateData();
                });
            },

            // 批量发送
            batchSend() {
                let vm = this;
                if (!vm.batchSendJson) {
                    vm.$message({
                        message: this.$t('selectTip'),
                        type: 'error',
                    });
                    return;
                }
                Core.Api.Brand.invitationEmailBatchSend(vm.batchSendJson).then(
                    res => {
                        vm.$message({
                            message: this.$t('sendSuccess'),
                            type: 'success',
                        });
                        vm.getInviteList();
                        vm.dialogBatchSendVisible = false;
                    }
                );
            },

            showBatchDialog() {
                this.getAllList();
                this.batchSendData = [];
                this.dialogBatchSendVisible = true;
            },

            generateData() {
                this.batchSendData = [];
                this.notSendList.forEach((item, index) => {
                    this.batchSendData.push({
                        key: item.id,
                        label: item.target_org_name || item.target_org_email || item.code,
                    });
                });
            },

            // 点击发送按钮
            sendInvitation(invite) {
                Core.Api.Brand.sendInvitation(invite.id).then(
                    res => {
                        this.$message({
                            message: this.$t('sendSuccess'),
                            type: 'success',
                        });
                        this.getInviteList();
                    }
                );
            },

            showDeleteDialog(item) {
                this.dialogDeleteVisible = true;
                this.selectDeleteInvitation = item;
            },

            // 点击删除按钮
            deleteInvitation() {
                Core.Api.Brand.deleteInvitation(this.selectDeleteInvitation.id).then(
                    res => {
                        this.$message({
                            message: this.$t('deleteSuccess'),
                            type: 'success',
                        });
                        this.getInviteList();
                        this.dialogDeleteVisible = false;
                    }
                );
            },
             getInvitationCodeByUserId(){
                 if(this.getInvitationCode==''){
                    Core.Api.Org.getOrgInvitationCode(this.user.id).then(res=>{
                        if(res.code){
                            this.getInvitationCode = res.code;
                            this.$message({
                                type: 'success',
                                message: '更新成功!'
                            });
                        }
                    }).catch(err=>{
                        console.log(err);
                    })
                }else{
                    Core.Api.Org.getCodeNumber(this.getInvitationCode).then(res=>{
                        if(res.num!=undefined||res.num!=null){
                            if(res.num <= 0){
                                Core.Api.Org.getOrgInvitationCode(this.user.id).then(res=>{
                                    if(res.code){
                                        this.getInvitationCode = res.code;
                                        this.$message({
                                            type: 'success',
                                            message: '更新成功!'
                                        });
                                    }
                                }).catch(err=>{
                                    console.log(err);
                                })
                            }else{
                                this.$confirm(`当前邀请码有${res.num}个未激活, 是否继续更新?`, '提示', {
                                        confirmButtonText: '确定',
                                        cancelButtonText: '取消',
                                        type: 'warning'
                                    }).then(() => {
                                        Core.Api.Org.getOrgInvitationCode(this.user.id).then(res=>{
                                            if(res.code){
                                                this.getInvitationCode = res.code;
                                                this.$message({
                                                    type: 'success',
                                                    message: '更新成功!'
                                                });
                                            }
                                        }).catch(err=>{
                                            console.log(err);
                                        })
                                    }).catch(() => {
                                        this.$message({
                                            type: 'info',
                                            message: '已取消更新'
                                        });
                                });
                            }
                        }

                    }).catch(err=>{
                        console.log(err);
                    })
                }
            },
            deleteInvitationCodeByUserId(){
                 Core.Api.Org.getCodeNumber(this.getInvitationCode).then(res=>{
                    if(res.num!=undefined||res.num!=null){
                        if(res.num <= 0){
                            Core.Api.Org.deleteOrgInvitationCode(this.user.id).then(res=>{
                                if(res==''){
                                this.getInvitationCode = '';
                                this.$message({
                                    type: 'success',
                                    message: 'delete successfully'
                                })
                                }
                            }).catch(err=>{
                                console.log(err);
                            })
                        }else{
                            this.$confirm(`当前邀请码有${res.num}个未激活, 是否继续删除?`, '提示', {
                                confirmButtonText: '确定',
                                cancelButtonText: '取消',
                                type: 'warning'
                            }).then(() => {
                                Core.Api.Org.deleteOrgInvitationCode(this.user.id).then(res=>{
                                    if(res==''){
                                        this.getInvitationCode = '';
                                        this.$message({
                                            type: 'success',
                                            message: 'delete successfully'
                                        })
                                    }
                                }).catch(err=>{
                                    console.log(err);
                                })
                            }).catch(() => {
                                this.$message({
                                    type: 'info',
                                    message: '已取消删除'
                                });
                            });
                        }
                    }

                }).catch(err=>{
                    console.log(err);
                })

            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .dealer-invite {
        width: 100%;
        height: 100%;
        .upload-contact {
            display: inline-block;
        }
        .status-label {
            color: #A1A5B1;
            /*font-size: 12px;*/
            margin-right: 15px;
        }
        .search-input {
            width: 160px;
            margin-left: 15px;
        }
        .el-transfer-panel__filter .el-input__inner {
            height: 30px;
        }
        .transfer {
            margin: 0 auto;
            width: 600px;
        }
        .transfer-footer {
            margin-left: 20px;
        }
        .icon-td{
            display: flex;
            align-items: center;
            flex-direction: row;
            height:80px;
            width:105px;
            .icons{
                align-self: center;
                width:35px;
                height:35px;
                padding-bottom:36px;
            }
            .send {
                background: url('../../../assets/images/icon/icon-send.png') no-repeat center center / 22px;
                &:hover {
                    background: url('../../../assets/images/icon/icon-send-active.png') no-repeat center center / 22px;
                }
            }
        }
        table {
            border-collapse: collapse;
            border-color: rgba(255, 255, 255, .3);
            width: 100%;
            tr {
                td {
                    width: 25%;
                    text-align: center;
                    height: 36px;
                    line-height: 36px;
                    overflow: hidden;
                    text-overflow: ellipsis;
                    word-break: break-all;
                    input {
                        height: 34px;
                        background: #2B2F3B;
                        width: 100%;
                        padding: 0 10px;
                        color: #ffffff;
                    }
                }
            }
        }
        .loading {
            display: inline-block;
            vertical-align: text-bottom;
            position: relative;
            top: 4px;
            margin-left: 16px;
        }

        .pt-30 {
            padding-bottom: 30px;
            label {
                display: inline-block;
                width: 150px;
                color: #fff;
            }
            .dealerType {
                margin-left: 152px;
                line-height: 20px;
                /*display: inline-block;*/
                /*position: relative;*/
                /*float: left;*/
                /*margin-left: 150px;*/
            }
        }
        .dialog{
            .dealerTypeNote{
                line-height: 20px;
            }
        }
    }
    .invitation-code{
        margin-left: 40px;
        .el-form-item{
            display: flex;
            .el-form-item__label{
                margin-right: 8px;
            }
            .input {
                width: 222px;
                margin-right: 20px;
                // margin-top: 16px;
            }
        }
    }
    // .el-message-box{
    //     background: #353945;
    // }
</style>



// WEBPACK FOOTER //
// invite.vue?1ce2e492