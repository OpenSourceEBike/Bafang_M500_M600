<template>
    <div class="upper-attr">

        <div class="item" v-if="item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.TEXT">
            <label class="title">{{item.name}}</label>
            <div class="desc">
                <el-input v-model="inputValue"
                          @change="inputChange($event, item.key)"></el-input>
            </div>
        </div>

        <div class="item" v-else-if="item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.SELECT">
            <label class="title">{{item.name}}</label>
            <div class="desc">
                <el-select v-model="selectedValue"
                           @change="selectChange($event, item.key)"
                           slot="prepend">
                    <el-option
                        v-for="item in selectList"
                        :key="item.id"
                        :label="item.name"
                        :value="item.id">
                    </el-option>
                </el-select>
            </div>
        </div>

        <div class="item imgs"
             v-else-if="item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.PICS ||
                        item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.PIC">
            <label class="title">{{item.name}}</label>
            <div class="desc">
                <vue-clip :options="item.options" :on-complete="imgUploadCompleteCallback" class="img-upload">
                    <template slot="clip-uploader-action">
                        <img class="upload-icon" src="../assets/images/icon/icon-upload@2x.png"/>
                    </template>
                    <template slot="clip-uploader-body">
                        <div v-for="(file, index) in imgUploadList">
                            <img v-bind:src="Core.Const.NET.FILE_URL_PREFIX + file.hash + '.' + file.ext" class="pic"/>
                            <div class="pic remove" @click="removeImg(index, file)">&nbsp; </div>
                        </div>
                    </template>
                </vue-clip>
            </div>
        </div>

        <div class="file-item"
             v-else-if="item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILE ||
                   item.input_type == Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILES">
            <label class="title">{{item.name}}</label>
            <vue-clip :options="item.options" :on-complete="fileUploadCompleteCallback" class="desc">
                <template slot="clip-uploader-body">
                    <div v-for="(file, index) in fileUploadList">
                        {{ file.name }}
                        <!--<el-button type="text" class="btn">View</el-button>-->
                        <el-button type="text" @click="removedFile(index, file)">Remove</el-button>
                    </div>
                </template>

                <template slot="clip-uploader-action">
                    <a href="javascript:;" class="btn-underline">Add</a>
                </template>
            </vue-clip>
        </div>

    </div>

</template>

<script type="text/javascript">

    import Core from 'core/core'
    import CurrencyInput from './currency-input.vue';


    export default{
        name: 'ItemAttrInputGroup',

        components: {
        	CurrencyInput
        },
        props: {
            item: {
                type: Object,
                default: {}
            },
        },
        watch: {
//            'item.v': function (value) {
//                this.inputValue = value;
//            },

            'item.extra': function (value) {
                switch (value['inputType']) {
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.SELECT:
                    	if(value['value']){
                        this.selectedValue = parseInt(value['value']);
                    	}
                        break;

                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILE:
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILES:
                        if (value['value']) {
                            this.fileUploadList = value['value'];
                            let $this = this;
                            let attrId = this.item.id;
                                this.item.value.forEach(function (v,index) {
//                                    console.log(v);
                                    $this.fileChange(v.id, attrId);
                                });
                        }
                        break;

                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.PICS:
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.PIC:
                        if (value['value']) {
                            this.imgUploadList = value['value'];
                            let $this = this;
                            let attrId = this.item.id;
                            this.item.value.forEach(function (v,index) {
                                $this.fileChange(v.id, attrId);
                            });
                        }
                        break;
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.TEXT:
                        this.inputValue = value['value'];
                        this.inputChange(this.item.value, this.item.key);
                        break;
                    default:
                        break;
                }
            },
        },
        data() {
            return {
                Core: Core,
                inputValue: "",
                selectList: [],
                selectedValue: "",
                fileUploadList: [],
                imgUploadList: []
            }
        },
        mounted() {
            this.initData();
        },
        methods: {
            initData () {
//                console.log(this.item, 999);
                switch (this.item.input_type) {
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.TEXT:
                        this.inputValue = this.value;
                        break;

                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.SELECT:
                        this.selectList = JSON.parse(this.item.value);
//                        console.log(this.selectList, 212121);
                        this.selectedValue = this.selectList[0]['id'];
                        break;

                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILE:
                    case Core.Const.CATEGORY_ATTR_INPUT_TYPE.FILES:
                        break;

                    default:
                        break;
                }

//                this.$root.eventHub.$on('attr-key-' + this.item.key, (data) => {
//                	console.log('event');
//                	console.log(data);
//                	this.inputValue = data.v;
//                })
            },

            removedFile(index, file) {
                this.fileUploadList.splice(index, 1);

                this.$emit("file-upload-remove", file);
            },

            removeImg(index, file){
                this.imgUploadList.splice(index, 1);

                this.$emit("file-upload-remove", file);
            },

            imgUploadCompleteCallback    (file, status, xhr) {
                let res = JSON.parse(xhr.response);
                if (res.code === 0 && res.data.file) {
                    let fileItem = res.data.file;

                    this.imgUploadList.push(fileItem);

                    this.fileChange(fileItem.id, fileItem.attr_def_id);
                }
            },

            //多文件
            fileUploadCompleteCallback (file, status, xhr) {
                let res = JSON.parse(xhr.response);
//                console.log('file upload res', res);
                if (res.code === 0 && res.data.file) {
                    let fileItem = res.data.file;
                    this.fileUploadList.push(fileItem);

//                    console.log(this.fileUploadList);

                    this.fileChange(fileItem.id, fileItem.attr_def_id);

                }
            },

            fileChange(value, key) {
                let returnData = {
                    'attr_id': key,
                    'file_id': value
                };
                this.$emit("file-upload-return", returnData);
            },

            inputChange(value, key) {
                let returnData = {
                    'key': key,
                    'value': value
                };
                this.$emit("input-value-return", returnData);
            },

            selectChange(value, key) {
                let returnData = {
                    'key': key,
                    'value': value
                };
                this.$emit("input-value-return", returnData);
            }
        }

    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .upper-attr {
        padding: 5px 0;
        &:first-child {
            padding: 0;
        }
        .item {
            position: relative;
            height: 50px;
            line-height: 50px;
            font-size: 0;
            overflow: hidden;
            .title {
                display: inline-block;
                padding: 0 20px;
                width: 165px;
                font-size: 12px;
                color: #fff;
                font-weight: 500;
            }
            .desc {
                display: inline-block;
                vertical-align: top;
                width: 400px;
                .el-input {
                    width: 310px;
                }
            }
        }

        .file-item {
            position: relative;
            /*height: 50px;*/
            margin-top: 20px;
            line-height: 30px;
            font-size: 0;
            overflow: hidden;
            .title {
                display: inline-block;
                padding: 0 20px;
                width: 165px;
                font-size: 12px;
                color: #fff;
                font-weight: 500;
            }
            .desc {
                display: inline-block;
                vertical-align: top;
                width: 400px;
                .label {
                    display: inline-block;
                    width: 110px;
                }
                .btn {
                    margin-left: 10px;
                }
            }
        }
        .imgs {
            position: relative;
            height: 50px;
            line-height: 50px;
            font-size: 0;
            overflow: hidden;
            .title {
                display: inline-block;
                padding: 0 20px;
                width: 165px;
                font-size: 12px;
                color: #fff;
                font-weight: 500;
            }
            .desc {
                display: inline-block;
                vertical-align: top;
                width: 400px;
                overflow: hidden;
                .img-upload > div {
                    float: left;
                    height: 50px;
                    .pic{
                        width: 50px;
                        height: 50px;
                        margin-right: 10px;

                    }
                    .remove{
                        width: 15px;
                        height: 15px;
                        margin-left: 35px;
                        position: absolute;
                        top: 0;
                        z-index: 2;
                        background: url('../assets/images/icon-close.png') no-repeat center;
                        &:hover{
                            background: url('../assets/images/icon-close-red.png') no-repeat center;
                            width: 15px;
                            height: 15px;
                        }
                    }

                }

                .img-upload {
                    display: inline-block;
                    .upload-icon {
                        width: 50px;
                        height: 50px;
                    }
                }
            }
        }
    }
</style>



// WEBPACK FOOTER //
// item-attr-input-group.vue?550b2619