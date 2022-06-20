<template>
    <div class="ticket-detail">
        <div class="header top-box">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <div class="btn-group">
                <el-button v-if="showHandlerUp" style="top: 15px;" type="primary" @click="handleUpTicket">
                    {{ $t('handleUp') }}
                </el-button>
                <el-button type="primary" @click='getActionList'>{{ $t('history') }}</el-button>
            </div>
        </div>

        <div class="content-wrap">
            <div class="content-card-item">
                <label class="content-header">{{ $t('ticketInfo.infoTitle') }}</label>
                <div class="content-body">
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.ticketNo') }}</label>
                        <span>{{ticket.sn}}</span>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.createTime') }}</label>
                        <span>{{ticket.create_time | timeFormat("Y-m-d")}}</span>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.status') }}</label>
                        <span>{{ticket.status | ticketStatus( Core.Data.getLang() )}}</span>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.serviceCenter') }}</label>
                        <span>{{ ticket.handler_org ? ticket.handler_org.name : ''}}</span>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.title') }}</label>
                        <div class="lh-20">{{ticket.title}}</div>
                    </div>
                    <div class="item">
                        <label class="label">{{ $t('ticketInfo.desc') }}</label>
                        <div class="lh-20">{{ticket.description}}</div>
                    </div>
                    <div class="item" v-if="ticket.processImageList">
                        <label class="label">{{ $t('ticketInfo.image') }}</label>
                        <div class="lh-20">
                            <img v-for="item in ticket.processImageList" :src="Core.Const.NET.FILE_URL_PREFIX + item"/>
                        </div>
                    </div>
                    <div class="item" v-if="ticket.video">
                        <label class="label">{{ $t('ticketInfo.video') }}</label>
                        <video :src="Core.Const.NET.FILE_URL_PREFIX + ticket.video" controls="controls">
                            您的浏览器不支持 video 标签。
                        </video>
                    </div>
                </div>
            </div>
            <div class="message-box">
                <ul class="message" :class="showMessage ? '' : 'message-100'">
                    <li v-for="item in ticketActionList">
                        <p class="time">
                            <span>{{ item.create_time | timeFormat("Y-m-d H:i:s") }}</span>
                        </p>
                        <div class="main" :class="{ self: item.org.type != 2 }">
                            <div class="avatar-box">
                                <el-tooltip placement="bottom" effect="light">
                                    <div slot="content">
                                        <div class="tip-content">
                                            <div class="tip-avatar">
                                                <div><img :src="item.org.avatar | getImgUrl"/></div>
                                                <div class="org-name">
                                                    {{ item.org.name }}
                                                </div>
                                            </div>
                                            <div class="tip-item">
                                                <span class="title">{{ $t('user.address') }}</span>
                                                <span class="content">{{ item.org.country }} {{ item.org.address
                                                    }}</span>
                                            </div>
                                            <div class="tip-item">
                                                <span class="title">{{ $t('user.website') }}</span>
                                                <span class="content">{{ item.org.website }}</span>
                                            </div>
                                            <div class="tip-item">
                                                <span class="title">{{ $t('user.email') }}</span>
                                                <span class="content">{{ item.org.email }}</span>
                                            </div>
                                        </div>
                                    </div>
                                    <img v-if="item.org.type == 2" class="avatar" width="30" height="30"
                                         src="../../assets/images/icon/icon-avatar.png"/>
                                    <img v-else class="avatar" width="30" height="30"
                                         src="../../assets/images/icon/icon-default-servicer.png"/>
                                </el-tooltip>
                            </div>
                            <div class="text-box">
                                <p v-if="item.org.type == 2">{{ item.org.type | orgType }}</p>
                                <div class="text">
                                    {{ item.message }}
                                    <div v-if="item.img" class="text-img" v-for="img in item.img">
                                        <img :src="img | getImgUrl"/>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </li>
                </ul>
                <div class="reply-holder" v-if="showMessage">
                    <el-form label-position="left" :model="ticket" label-width="120px">
                        <el-form-item :label="$t('replay.content')">
                            <el-input type="textarea" :autosize="{ minRows: 2, maxRows: 3}" v-model="desc"></el-input>
                        </el-form-item>
                        <el-form-item :label="$t('replay.image')">
                            <el-button class="submit" type="primary" @click="addTicketAction">
                                {{ $t('replay.sendMessage') }}
                            </el-button>
                            <vue-clip :options="upload" :on-complete="complete">
                                <template slot="clip-uploader-action">
                                    <div class="upload-action">
                                        <div class="dz-message">
                                            <img class="upload-icon" src="../../assets/images/icon/icon-upload@2x.png"/>
                                        </div>
                                    </div>
                                </template>
                                <template slot="clip-uploader-body" slot-scope="props">
                                    <div class="img-item" v-for="file in imgList" v-if="imgList.length">
                                        <img v-bind:src="file | getImgUrl"/>
                                    </div>
                                </template>
                            </vue-clip>
                        </el-form-item>
                    </el-form>
                </div>
            </div>
        </div>

        <!--弹框-->
        <el-dialog
            :title="$t('handleUpTicketDialog.title')"
            :visible.sync="dialogHandleUpVisible"
            custom-class="dialog-w-400"
            size="tiny">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item :label="$t('handleUpTicketDialog.serviceCenter')">
                    <el-select v-model="selectedServiceCenter" placeholder="">
                        <el-option
                            v-for="item in serviceCenterList"
                            :key="item.id"
                            :label="item.name"
                            :value="item">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="handleUpToBaFang">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogHandleUpVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

        <!--工单的历史记录弹窗-->
        <el-dialog :title="$t('historyDialog.title')" :visible.sync="historyDialogVisible">
            <table border="0" cellspacing="0" cellpadding="0" class="history-table">
                <thead>
                <tr>
                    <td class="history-td head">
                        {{ $t('historyDialog.name') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('historyDialog.operation') }}
                    </td>
                    <td class="history-td head">
                        {{ $t('historyDialog.time') }}
                    </td>
                </tr>
                </thead>
                <tr>
                    <td class="history-td">{{ticket.org.name}}</td>
                    <td class="history-td">create ticket</td>
                    <td class="history-td">{{ticket.create_time | timeFormat("Y-m-d H:i:s")}}</td>
                </tr>

                <tr v-for="(item,index) in historyList">
                    <td class="history-td">{{item.operator_org.name}}</td>
                    <td class="history-td">{{item.name}}</td>
                    <td class="history-td">{{item.create_time | timeFormat("Y-m-d H:i:s")}}</td>
                </tr>

                <tr v-if="ticket.status == 300">
                    <td class="history-td">{{ticket.org.name}}</td>
                    <td class="history-td">complete ticket</td>
                    <td class="history-td">{{ticket.update_time | timeFormat("Y-m-d H:i:s")}}</td>
                </tr>
            </table>
        </el-dialog>
    </div>
</template>

<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        components: {},
        name: "ticketDetail",
        data() {
            return {
                org: Core.Data.getOrg(),
                ticketId: 0,
                Core: Core,
                ticket: {org: {}},
                ticketActionList: [],

                upload: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    paramName: 'file',
                    params: {'token': Core.Data.getToken()}
                },
                desc: '',
                imgList: [],
                showMessage: false,
                showHandlerUp: false,
                dialogHandleUpVisible: false,
                serviceCenterList: [],
                selectedServiceCenter: {},
                baFangOrgId: 25,
                historyDialogVisible: false,
                historyList: [],
                ticketActionTask: ''
            }
        },
        i18n: {
            messages: {
                en: EN.Components.ServiceCenter.ticketDetail,
                zh: ZH.Components.ServiceCenter.ticketDetail,
                de: DE.Components.ServiceCenter.ticketDetail,
                nl: NL.Components.ServiceCenter.ticketDetail,
            }
        },
        mounted: function () {
            let ticketId = this.$route.query.id;
            this.ticketId = ticketId;
            // 获取ticket detail
            this.getTicketDetail(ticketId);
            // 获取聊天记录
            this.getTicketActionList(ticketId);
        },
        destroyed: function() {
            if (this.ticketActionTask) {
            	clearInterval(this.ticketActionTask)
            }
        },
        methods: {
            getTicketDetail(ticketId) {
                let $this = this;
                let userId = Core.Data.getUser().id;
                Core.Api.Ticket.ticketDetail(ticketId).then(res => {
                    $this.ticket = res.ticket;
                    if (this.org.type === 5) {
                        $this.showMessage = $this.ticket.handler_id === userId;
                    } else {
                        $this.showMessage = $this.ticket.handler_org_id === this.org.id;
                    }

                    $this.showHandlerUp = $this.showMessage;

                    if (this.org.type === 4) {
                        $this.showHandlerUp = false;
                    }
                    if ($this.ticket.status === 300) {
                        $this.showMessage = false;
                    }
                    if ($this.showMessage) {
                        $this.ticketActionTask = setInterval(() => {
                            $this.getTicketActionList(ticketId);
                        }, 3000)
                    }
                    if (res.ticket.img) {
                        $this.ticket.processImageList = JSON.parse(res.ticket.img)
                    }
                });
            },
            getTicketActionList(ticketId) {
                let $this = this;
                Core.Api.Ticket.ticketActionList(ticketId).then(res => {
                    $this.ticketActionList = res.list;
                });
            },
            addTicketAction() {
                if (!this.desc) {
                    this.$message({
                        message: 'Please input description',
                        type: 'warning'
                    });
                    return;
                }
                let $this = this;
                let ticketId = $this.ticketId;
                let message = this.desc;
                let img = '';
                let video = '';

                if (this.imgList.length > 0) {
                    img = JSON.stringify(this.imgList);
                }

                Core.Api.Ticket.saveTicketMessage(ticketId, message, img, video).then(res => {
                    $this.desc = '';
                    $this.imgList = [];
                    $this.getTicketActionList(ticketId);
                });
            },
            complete(file, status, xhr) {
                if (status === 'success') {
                    let res = JSON.parse(xhr.response);
                    if (res.code === 0 && res.data.file) {
                        this.imgList.push(res.data.file.name);
                    }
                }
            },
            handleUpTicket() {
                let $this = this;

                if (this.ticket.handler_type === 5) {
                    Core.Api.Ticket.handleUpTicketByBrandServiceCenter($this.ticketId).then(res => {
                        $this.$message({
                            message: 'Handle up ticket successfully',
                            type: 'success'
                        });
                        $this.$router.back();
                    });
                } else {
                    $this.dialogHandleUpVisible = true;
                    Core.Api.Org.allServiceCenterList($this.baFangOrgId).then(res => {
                        $this.serviceCenterList = res.org_list;
                    });
                }
            },

            handleUpToBaFang() {
                let $this = this;
                let orgId = $this.selectedServiceCenter.id;
                let ticketId = $this.ticket.id;
                Core.Api.Ticket.handleUpTicketByBrand(orgId, ticketId).then(res => {
                    $this.$message({
                        message: "Handle up successfully",
                        type: "success"
                    });
                    $this.$router.back();
                });
            },
            getActionList(){
                Core.Api.Ticket.getActionList(this.ticket.id).then(res => {
                    this.historyList = res.list;
                    this.historyDialogVisible = true;
                });
            }
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .ticket-detail {
        height: 100%;
        .content-wrap {
            overflow: hidden;
            padding-top: 0;
        }
        .content-card-item {
            width: 400px;
            height: 100%;
            float: left;
            border-right: 1px solid #2B2F3B;
            overflow: auto;
            .content-header {
                display: block;
                padding: 10px 43px;
                font-size: 14px;
                height: 50px;
                line-height: 30px;
                font-weight: bold;
                box-sizing: border-box;
            }

            .content-body {
                font-size: 0;
                background: #353945;
                overflow: auto;
                padding: 20px 0;
                height: calc(100% - 50px);
                .item {
                    width: 100%;
                    padding: 0 43px;
                    line-height: 50px;
                    label {
                        vertical-align: top;
                        display: inline-block;
                        width: 100px;
                        font-size: 14px;
                        color: #A1A5B1;
                    }
                    span {
                        font-size: 14px;
                    }
                    .lh-20 {
                        width: calc(100% - 104px);
                        vertical-align: top;
                        padding: 16px 0;
                        font-size: 14px;
                        display: inline-block;
                        line-height: 20px;
                        img {
                            width: 100px;
                            height: 100px;
                            &:nth-child(2n+1) {
                                margin-right: 6px;
                            }
                        }
                    }
                    video {
                        width: 100%;
                    }
                }
            }
        }

        .message-box {
            float: right;
            width: calc(100% - 400px);
            height: 100%;
            .message {
                height: calc(100% - 160px);
                background-color: #2B2F3B;
                padding: 10px 15px;
                overflow-y: auto;

                li {
                    margin-bottom: 15px;
                }
                .time {
                    margin: 7px 0;
                    text-align: center;

                    > span {
                        display: inline-block;
                        padding: 0 10px;
                        font-size: 12px;
                        color: #fff;
                        border-radius: 2px;
                        /*background-color: #dcdcdc;*/
                    }
                }
                .avatar-box {
                    float: left;
                    margin: 0 10px 0 0;
                    text-align: center;
                    .avatar {
                        border-radius: 3px;
                    }
                    p {
                        font-size: 10px;
                        color: #A1A5B1;
                    }
                }
                .text-box {
                    & > p {
                        font-size: 10px;
                        color: #A1A5B1;
                        margin-bottom: 2px;
                    }
                }
                .text {
                    display: inline-block;
                    position: relative;
                    padding: 0 10px;
                    max-width: calc(100% - 40px);
                    min-height: 30px;
                    line-height: 2.5;
                    font-size: 12px;
                    text-align: left;
                    word-break: break-all;
                    background-color: #fafafa;
                    border-radius: 4px;
                    color: #A1A5B1;

                    .text-img img {
                        max-width: 500px;
                    }

                    &:before {
                        content: " ";
                        position: absolute;
                        top: 9px;
                        right: 100%;
                        border: 6px solid transparent;
                        border-right-color: #fafafa;
                    }
                }

                .self {
                    text-align: right;

                    .avatar-box {
                        float: right;
                        margin: 0 0 0 10px;
                    }
                    .text {
                        background-color: #b2e281;
                        color: #ffffff;

                        &:before {
                            right: inherit;
                            left: 100%;
                            border-right-color: transparent;
                            border-left-color: #b2e281;
                        }
                    }
                }
            }

            .message-100 {
                height: 100%;
            }

            .reply-holder {
                height: 160px;
                overflow-y: auto;
                padding: 20px 40px 16px 40px;
                .el-form-item {
                    margin-bottom: 10px;
                    &:last-child {
                        height: 60px;
                        overflow: hidden;
                        margin-bottom: 0;
                    }
                }
                .upload-action {
                    height: 60px;
                }
                .dz-clickable {
                    display: inline-block;
                    width: 60px;
                    height: 60px;
                    .dz-message {
                        width: 100%;
                        height: 100%;
                        img {
                            width: 60px;
                            height: 60px;
                        }
                    }
                }

                .img-item {
                    float: left;
                    img {
                        width: 60px;
                        height: 60px;
                        margin-right: 6px;
                    }
                }
            }

            .el-form-item__label {
                color: #A1A5B1;
            }

            .submit {
                position: absolute;
                right: 0;
                top: 30px;
            }
        }

    }

    .demo-form-inline .el-select {
        width: 240px;
    }

    .history-table {
        width: 100%;
        border-top: 1px solid rgba(255, 255, 255, 0.1);
        tr {
            line-height: 30px;
        }
        .head {
            color: white;
        }
        .history-td {
            padding: 0 6px;
            width: 160px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.1);
            font-size: 12px;
            text-align: center;
        }
    }

    .tip-content {
        .tip-avatar {
            width: 100%;
            max-width: 300px;
            height: auto;
            margin-bottom: 10px;
            img {
                width: 100%;
                max-width: 300px;
                height: auto;
            }
            .org-name {
                width: 100%;
                height: 30px;
                background: #98B42C;
                line-height: 30px;
                text-align: center;
                color: #fff;
            }
        }
        .tip-item {
            width: 100%;
            max-width: 300px;
            line-height: 30px;
            span.title {
                width: 100px;
                color: #A1A5B1;
                display: inline-block;
                vertical-align: top;
            }
            span.content {
                width: 190px;
                color: #ddd;
                display: inline-block;
            }
        }
    }
</style>



// WEBPACK FOOTER //
// ticket-detail.vue?72e975d9