<template>
    <div class="video-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="$router.back()" :title="$t('back')"></div>
                </div>
            </div>
            <label class="head-text">{{ $t('category') }}</label>
            <el-select v-model="category" slot="append" class="select" @change="changeCategory">
                <el-option
                    v-for="item in categoryOptions"
                    :key="item.id"
                    :label="item.name"
                    :value="item.id">
                </el-option>
            </el-select>
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
                        <a class="btn-underline" @click="preview(item)">{{ $t('preview') }}</a>
                        <a class="btn-underline" :href="item.file | getFileUrl('')"
                           :download="item.name">{{ $t('download') }}</a>
                    </td>
                </tr>
                </tbody>
            </table>
            <div class="no-data-content" v-show="videoList.length <= 0">
                {{ $t('noResult') }}
            </div>
        </div>

        <!--预览弹窗-->
        <el-dialog :title="selectVideo.name" custom-class="video-dialog"
                   :visible.sync="dialogPreviewVisible"
                   :before-close="closeVideoDialog">
            <div class="dialog-container">
                <video controls="controls" autoplay="autoplay" :src="selectVideo.file | getFileUrl('')">
                </video>
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
        name: "VideoList",
        data() {
            return {
                categoryOptions: [
                    { id: 0, name: 'All' },
                    { id: 1, name: 'LCD' },
                    { id: 2, name: 'LED' },
                ],
                category: 0,
                errorCode: 0,
                tableHead: ['Name', 'Category', 'Operation'],

                selectVideo: '',
                videoList: [],
                dialogPreviewVisible: false
            }
        },

        i18n: {
            messages: {
                en: EN.Component.Product.VideoList,
                zh: ZH.Component.Product.VideoList,
                de: DE.Component.Product.VideoList,
                nl: NL.Component.Product.VideoList
            }
        },

        mounted: function () {
            this.errorCode = this.$route.query.code || 0;

            this.getVideoList();
            // 国际化
            this.tableHead = [this.$t('name'), this.$t('category'), this.$t('operation')];
            let vm = this;
            this.$root.eventHub.$on(Core.Const.DATA.KEY_I18N_UPDATE, () => {
                setTimeout(() => {
                    vm.tableHead = [vm.$t('name'), vm.$t('Category'), vm.$t('Operation')];
                }, 100);
            });
        },

        methods: {
            changeCategory() {
                this.getVideoList();
            },

            handleCurrentChange(val) {
                this.getVideoList();
            },

            getVideoList() {
                Core.Api.Component.getVideoListByCategoryAndErrorCode(this.category, this.errorCode).then(res => {
                    this.videoList = res.list;
                });
            },

            preview(item) {
                this.selectVideo = item;
                this.dialogPreviewVisible = true;
            },

            closeVideoDialog() {
                this.selectVideo = {};
                this.dialogPreviewVisible = false;
            },
        },
    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .video-list {
        width: 100%;
        height: 100%;
        .head-text {
            color: #A1A5B1;
            margin: 0 15px;
        }
        .label {
            color: white;
            width: 100px;
            display: inline-block;
            margin-right: 15px;
        }
    }
</style>



// WEBPACK FOOTER //
// video-list.vue?123f1426