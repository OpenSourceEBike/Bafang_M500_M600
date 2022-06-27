<template>
    <div class="my-ticket-list">
        <div class="header">
            <el-form :inline="true" class="demo-form-inline">
                <el-form-item :label="$t('category')">
                    <el-select v-model="search.category" placeholder="" @change="getMyTicketList">
                        <el-option
                            v-for="item in categoryList"
                            :key="item.id"
                            :label="item.value"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="$t('status')">
                    <el-select v-model="search.status" placeholder="" @change="getMyTicketList">
                        <el-option
                            v-for="item in statusList"
                            :key="item.id"
                            :label="item.value"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add" @click="showSaveDialog"></div>
                </div>
            </div>
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
                        <td>{{item.category | ticketCategoryType}}</td>
                        <td>{{item.title}}</td>
                        <td>{{item.target_sn}}</td>
                        <td>{{item.create_time | timeFormat("Y-m-d")}}</td>
                        <td>{{item.status | ticketStatus( Core.Data.getLang() )}}</td>
                        <td class="icon-td">
                            <div class="icons">
                                <div class="icon-item eye"  @click="goToTicketDetailPage(item)" :title=" $t('look')"></div>
                            </div>
                            <!-- <el-button class="btn-underline" type="text" @click="goToTicketDetailPage(item)">{{ $t('look') }}</el-button> -->
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
                <p class="total">{{ $t('total') }}: {{total}}</p>
            </div>
        </div>
        <!-- 添加ticket -->
        <el-dialog :title="$t('addTicketDialog.dialogTitle')" custom-class="save-ticket-dialog" :visible.sync="dialogSaveVisible">
            <el-form label-position="left" :model="ticket" label-width="160px">
                <el-form-item :label="$t('addTicketDialog.category')" required>
                    <el-select v-model="ticket.category" placeholder="">
                        <el-option
                            v-for="item in categoryOptions"
                            :key="item.id"
                            :label="item.value"
                            :value="item.id">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.title')" required>
                    <el-input v-model="ticket.title" :maxlength="45"></el-input>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.description')">
                    <el-input type="textarea" :autosize="{ minRows: 2}" v-model="ticket.description"></el-input>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.bike')" required>
                    <el-input class="scan-input scan-input-top-8" v-model="ticket.targetSn" :placeholder="$t('addTicketDialog.sn')"
                              @blur="blurObjectSn"></el-input>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.serviceCenter')" v-if="!hideServiceCenter" required>
                    <el-select v-model="ticket.serviceCenterOrgId" placeholder="" v-if="showServiceCenterList">
                        <el-option
                            v-for="item in serviceCenterList"
                            :key="item.id"
                            :label="item.name"
                            :value="item.id">
                        </el-option>
                    </el-select>
                    <el-input v-else v-model="brandOrg.name" :disabled="true"></el-input>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.image')">
                    <vue-clip :options="upload" :on-complete="complete">
                        <template slot="clip-uploader-action">
                            <div>
                                <div class="dz-message">
                                    <img class="upload-icon" src="../../assets/images/icon/icon-upload@2x.png"/>
                                </div>
                            </div>
                        </template>
                        <template slot="clip-uploader-body" slot-scope="props">
                            <div class="img-list" v-for="file in props.files">
                                <img v-bind:src="file.dataUrl"/>
                            </div>
                        </template>
                    </vue-clip>
                </el-form-item>
                <el-form-item :label="$t('addTicketDialog.video')">
                    <el-upload
                        :action="upload.url"
                        :on-change="handleChangeVideo"
                        list-type="picture"
                        accept="video/*"
                        :data="upload.params"
                        :file-list="fileList">
                        <div class="btn-group-inline">
                            <div class="icons">
                                <div class="icon-item upload"  :title="$t('addTicketDialog.clickToUpload')"></div>
                                <span>support:avi/mov/mp4/mp3/webm/wav</span>
                            </div>
                        </div>
                        <!--<el-button size="small" type="primary">{{ $t('addTicketDialog.clickToUpload') }}</el-button>-->
                    </el-upload>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="addTicket">{{ $t('confirm') }}</el-button>
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
        name: "brandMyTicketList",
        data(){
            return {
                Core: Core,

                tableHead: ['Service No.', 'Category', 'Title', 'Target SN', 'Create Time', 'Status', 'Operation'],
                currentPage: 1,
                ticketList: [],
                total: 0,

                categoryList: [],
                statusList: [],
                search: {
                    category: 0,
                    status: 200,
                },
                // dialog
                dialogSaveVisible: false,
                dialogDeleteVisible: false,

                // ticket
                ticket: {
                    category: '',
                    title: '',
                    description: '',
                    targetId: '',
                    targetSn: '',
                    serviceCenterOrgId: ''
                },

                categoryOptions: [
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
                upload: {
                    url: Core.Const.NET.FILE_UPLOAD_END_POINT,
                    paramName: 'file',
                    params: {'token': Core.Data.getToken()}
                },
                fileList: [],
                imgList: [],
                serviceCenterList: [],
                hideServiceCenter: true,
                showServiceCenterList: false,
                brandOrg: {},
            }
        },
        i18n: {
            messages: {
                en: EN.Components.ServiceCenter.myTicketList,
                zh: ZH.Components.ServiceCenter.myTicketList,
                de: DE.Components.ServiceCenter.myTicketList,
                nl: NL.Components.ServiceCenter.myTicketList
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
            this.getMyTicketList();
        },
        methods: {
            initData() {
                this.tableHead = [this.$t('tableHead.serviceNo'), this.$t('tableHead.category'), this.$t('tableHead.title'), this.$t('tableHead.targetSn'), this.$t('tableHead.createTime'), this.$t('tableHead.status'), this.$t('tableHead.operation')];
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
                    {id: 200, value: this.$t('statusList.processing')},
                    {id: 300, value: this.$t('statusList.complete')},
                    {id: 400, value: this.$t('statusList.handleToAnother')},
                ];
            },

            handleCurrentChange(val) {
                this.currentPage = val;
                this.getMyTicketList();
            },

            getMyTicketList(){
                Core.Api.Ticket.getMyTicket(this.search.category, this.search.status, this.currentPage).then(res => {
                    this.ticketList = res.list;
                    this.total = res.count;
                });
            },

            goToTicketDetailPage(ticket) {
                this.$router.push({path: "ticket-detail", query: {id: ticket.id}})
            },

            showSaveDialog() {
                this.dialogDeleteVisible = false;
                this.dialogSaveVisible = true;
            },

            handleChangeVideo(file) {
                this.fileList = [file]
            },

            complete(file, status, xhr) {
                if (status === 'success') {
                    let res = JSON.parse(xhr.response);
                    if (res.code === 0 && res.data.file) {
                        this.imgList.push(res.data.file.hash + '.' + res.data.file.ext);
                    }
                }
            },

            blurObjectSn() {
                // 根据sn获取object详情
                if (!this.ticket.targetSn) {
                    return
                }
                console.log(this.ticket.targetSn);
                let $this = this;

                Core.Api.Object.objectDetailBySn(this.ticket.targetSn)
                    .then(res => {
                        let object = res.object;
                        if(!object){
                            this.$message({
                                message: 'Please check vehicle SN',
                                type: 'warning'
                            });
                            return;
                        }
                        let brandOrgId = object.brand_org_id;
                        $this.brandOrg = object.brand_org;
                        $this.ticket.targetId = object.id;
                        return Core.Api.Org.allServiceCenterList(brandOrgId)
                    }).then(data => {
                    $this.hideServiceCenter = false;
                    $this.serviceCenterList = data.org_list;
                    $this.showServiceCenterList = $this.serviceCenterList.length > 0;
                });
            },

            addTicket() {
                if (!this.checkParams()) {
                    return
                }

                let $this = this;
                let category = this.ticket.category;
                let title = this.ticket.title;
                if(title.length > 20){

                }
                let description = this.ticket.description;
                let targetId = this.ticket.targetId;
                let targetSn = this.ticket.targetSn;
                let orgId = this.ticket.serviceCenterOrgId ? this.ticket.serviceCenterOrgId : this.brandOrg.id;
                let img = '';
                let video = '';
                let testData = '';

                if (this.imgList.length > 0) {
                    img = JSON.stringify(this.imgList);
                }

                if (this.fileList.length > 0) {
                    video = this.fileList[0].response.data.file.hash + '.' + this.fileList[0].response.data.file.ext;
                }

                if (this.ticket.serviceCenterOrgId) {
                    Core.Api.Ticket.createTicket(category, title, description, targetId, targetSn, orgId, img, video, testData).then(res => {
                        this.$message({
                            message: 'Create successfully',
                            type: 'success'
                        });
                        $this.dialogSaveVisible = false;
                        $this.ticket = {
                            category: '',
                            title: '',
                            description: '',
                            targetId: '',
                            targetSn: '',
                            serviceCenterOrgId: ''
                        };
                        $this.getMyTicketList();
                    });
                } else {
                    Core.Api.Ticket.createTicketWithOrgId(category, title, description, targetId, targetSn, orgId, img, video, testData).then(res => {
                        this.$message({
                            message: 'Create successfully',
                            type: 'success'
                        });
                        $this.dialogSaveVisible = false;
                        $this.ticket = {
                            category: '',
                            title: '',
                            description: '',
                            targetId: '',
                            targetSn: '',
                            serviceCenterOrgId: ''
                        };
                        $this.getMyTicketList();
                    });
                }
            },


        }
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .my-ticket-list {
        height: 100%;
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
    }
</style>



// WEBPACK FOOTER //
// my-ticket-list.vue?90ee1c5c