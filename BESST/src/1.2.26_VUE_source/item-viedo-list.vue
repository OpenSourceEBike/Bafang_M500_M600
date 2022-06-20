<template>
    <div class="item-video-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add" @click="showAddDialog" v-if="authList[Core.Const.AUTH_LIST.productProductTrainingEdit]" :title="$t('addVideo')"></div>
                </div>
            </div>
        </div>
        <div class="table-wrap">
            <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                <thead>
                <tr>
                    <td v-for="item in tableHead">
                        {{item}}
                    </td>
                </tr>
                </thead>
                <tbody>
                <tr v-for="item in videoList">
                    <td>{{ item.name }}</td>
                    <td>{{ item.category | videoCategoryType }}</td>
                    <td>
                        <a class="btn-underline" @click="preview(item)">{{$t('preview')}}</a>
                        <!--<a class="btn-underline" :href="item.file | getFileUrl('')"-->
                           <!--:download="item.name">{{ $t('download') }}</a>-->
                        <a class="btn-underline" @click="showDeleteDialog(item)"
                           v-if="authList[Core.Const.AUTH_LIST.productProductTrainingDelete]">{{$t('delete')}}</a>
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="videoList.length <= 0 && !loading">
                {{ $t('noResult') }}
            </div>
            <div class="loading" v-if="loading">
                <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon" />
            </div>
        </div>


        <!--添加视频弹窗-->
        <el-dialog title="Add Video" :visible.sync="AddDialogVisible" :before-close="closeChooseItem">
            <div>
                <label class="label">{{$t('category')}}</label>
                <el-select v-model="category" slot="append" @change="getVideoByCategory" class="select">
                    <el-option-group
                        :key="categoryOptions[1].label"
                        :label="categoryOptions[1].label">
                        <el-option
                            v-for="item in categoryOptions[1].options"
                            :key="item.id"
                            :label="item.name"
                            :value="item.id">
                        </el-option>
                    </el-option-group>
                    <el-option
                        v-for="item in categoryOptions[2].options"
                        :key="item.id"
                        :label="item.name"
                        :value="item.id">
                    </el-option>
                </el-select>
            </div>
            <div class="clear-20"></div>
            <table border="0" cellspacing="0" cellpadding="0" class="video-table">
                <thead>
                <tr>
                    <td class="video-td head">
                        <el-checkbox v-model="allSelectedForAdd"></el-checkbox>
                    </td>
                    <td class="video-td head">
                        {{$t('name')}}
                    </td>
                    <td class="video-td head">
                        {{$t('category')}}
                    </td>
                    <td class="video-td head">
                        {{$t('operation')}}
                    </td>
                </tr>
                </thead>
                <tr v-for="item in showList">
                    <td class="video-td head">
                        <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                    </td>
                    <td class="video-td">{{item.name}}</td>
                    <td class="video-td">{{ item.category | videoCategoryType }}</td>
                    <td class="video-td"><a class="btn-underline" @click="preview(item)">{{$t('preview')}}</a></td>
                </tr>
            </table>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="addVideo"> {{$t('confirm')}} </el-button>
                <el-button type="primary" @click="closeChooseItem">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

        <!--预览弹窗-->
        <el-dialog :title="selectVideo.name" custom-class="video-dialog"
                   :visible.sync="dialogPreviewVisible"
                   :before-close="closeVideoDialog">
            <div class="dialog-container">
                <iframe :src="selectVideo.file"
                        frameborder="0"
                        class="video-iframe"
                        v-if="dialogPreviewVisible">
                </iframe>
            </div>
        </el-dialog>

        <!--删除弹窗-->
        <el-dialog :title="$t('deleteVideo')" :visible.sync="videoListDeleteDialogVisible" size="tiny">
            <p>{{ $t('deleteTip') }}</p>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="deleteVideo">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="videoListDeleteDialogVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import Core from 'core/core'
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        name: "ItemVideoList",
        data() {
            return {
                Core: Core,
                categoryOptions: [
                    {
                        options: [
                            { id: 0, name: 'All' },
                        ]
                    },
                    {
                        label: 'System Error',
                        options: [
                            { id: 1, name: 'LCD' },
                            { id: 2, name: 'LED' },
                        ]
                    },
                    {
                        options: [
                            {id: 3, name: "HMI"},
                            {id: 4, name: "Motor"},
                            {id: 5, name: "Controller"},
                            {id: 6, name: "Sensor"},
                            {id: 7, name: "Battery"},
                            {id: 8, name: "Charger"},
                        ]
                    }
                ],
                category: 1,
                itemId: "",
                AddDialogVisible: false,
                dialogPreviewVisible: false,
                showList: [],
                videoList: [],
                tableHead: ['Name', 'Category', 'Operation'],
                videoIdList: [],
                selectVideo: {},
                videoListDeleteDialogVisible: false,
                authList: [],
                loading: false,
            }
        },
        i18n: {
            messages: {
                en: EN.Component.Product.ItemVideoList,
                zh: ZH.Component.Product.ItemVideoList,
                de: DE.Component.Product.ItemVideoList,
                nl: NL.Component.Product.ItemVideoList
            }
        },

        mounted: function () {
            // 获取权限
            this.authList = this.$store.state.userAuth;

            this.itemId = this.$route.query.itemId;
            this.getItemVideoList(this.itemId);
            // 国际化
            this.tableHead = [this.$t('name'), this.$t('category'), this.$t('operation')];
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.tableHead = [vm.$t('name'), vm.$t('category'), vm.$t('operation')];
                }, 100);
            });
        },

        methods: {
            getItemVideoList(id) {
                this.loading = true;
                Core.Api.Item.videoList(id).then(res => {
                    if (res.video_list !== null && res.video_list !== []) {
                        this.videoList = res.video_list;
                    }
                    this.loading = false;
                })
            },

            getVideoByCategory() {
                let queryList = [];
                Core.Api.Component.getVideoListByCategory(this.category).then(res => {
                    queryList = res.list;
                    this.showList = [];
                    if (queryList.length <= 0) {
                        this.$message({
                            message: "There are no video in the category",
                            type: 'warning'
                        });
                        return;
                    }

                    this.getShowList(queryList, this.showList);
                    console.log(this.showList);
                });
            },

            getShowList(list, showList) {
                list.forEach((item, index) => {
                    showList.push({
                        id: item.id,
                        name: item.name,
                        file: item.file,
                        selected: false,
                        category: item.category,
                    })
                })
            },

            showAddDialog() {
                this.AddDialogVisible = true;
                this.getVideoByCategory();
            },

            addVideo() {
                this.showList.forEach((item, index) => {
                    if (item.selected === true) {
                        let isAdded = false;
                        this.videoList.forEach((videoItem) => {
                            if (videoItem.id === item.id) {
                                isAdded = true;
                            }
                        });
                        if (!isAdded) {
                            let item = this.showList[index];
                            this.videoList.push({
                                id: item.id,
                                name: item.name,
                                file: item.file,
                                category: item.category,
                            });

                        }
                    }
                });
                this.saveVideo();

            },

            saveVideo() {
                this.getVideoIdList();
                let videoList = this.videoIdList.join();
                Core.Api.Item.saveVideo(this.itemId, videoList).then(res => {
                    console.log(res);
                    this.closeChooseItem();
                })

            },

            closeChooseItem() {
                this.getItemVideoList(this.itemId);
                this.showList = [];
                this.AddDialogVisible = false;
            },

            getVideoIdList() {
                this.videoIdList = [];
                this.videoList.forEach((item, index) => {
                    this.videoIdList.push(item.id);
                })
            },

            preview(item) {
                this.selectVideo = item;
                this.dialogPreviewVisible = true;
            },

            closeVideoDialog() {
                this.selectVideo = {};
                this.dialogPreviewVisible = false;
            },

            showDeleteDialog(item) {
                this.selectVideo = item;
                this.videoListDeleteDialogVisible = true;

            },

            deleteVideo() {
                Core.Api.Item.videoDelete(this.itemId, this.selectVideo.id).then(res => {
                    this.$message({
                        message: "Delete successfully",
                        type: "success"
                    });
                    this.getItemVideoList(this.itemId);
                    this.videoListDeleteDialogVisible = false;
                });
            }
        },

        computed: {
            allSelectedForAdd: {
                get: function () {
                    return this.showList.reduce(function (prev, curr) {
                        return prev && curr.selected;
                    }, true);
                },
                set: function (newValue) {
                    this.showList.forEach((item) => {
                        item.selected = newValue;
                    });
                }
            },
            allSelectedForDelete: {
                get: function () {
                    return this.showList.reduce(function (prev, curr) {
                        return prev && curr.selected;
                    }, true);
                },
                set: function (newValue) {
                    this.showList.forEach((item) => {
                        item.selected = newValue;
                    });
                }
            }
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .item-video-list {
        height: 100%;
        padding-left: 272px;
        .label {
            color: white;
            margin-right: 15px;
        }
        .video-table {
            width: 100%;
            border-top: 1px solid rgba(255, 255, 255, 0.1);
            tr {
                line-height: 30px;
            }
            .head {
                color: white;
            }
            .video-td {
                padding: 0 6px;
                width: 160px;
                border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                font-size: 14px;
                text-align: center;
            }

        }
        .video-dialog {
            width: 650px;
            .dialog-container {
                font-size: 0;
                .video-iframe {
                    width: 650px;
                    height: 360px
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// item-viedo-list.vue?e1944c1a