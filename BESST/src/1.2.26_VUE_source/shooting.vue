<template>
    <div class="error-shooting">
        <div class="header">
            <span class="title">{{ $t('errorCode') }}</span>
            <el-select v-model="errorId" @change="changeErrorCode()">
                <el-option
                    v-for="code in errorCodeList"
                    :key="code.id"
                    :label="code.name"
                    :value="code.id">
                </el-option>
            </el-select>
            <el-button @click="showErrDialog('Edit Error Message','edit')" v-if="user === 24 || user === 627">Edit</el-button>
            <el-button @click="showErrDialog('Create Error Message','create')" v-if="user === 24 || user === 627">Create</el-button>
        </div>

        <div class="content-wrap">
            <div class="content" v-if="selectErrorCode.id > 0">
                <div class="content-item">
                    <label class="title">{{ $t('name') }}</label>
                    <div class="desc">
                        <span>{{selectErrorCode.name}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('remark') }}</label>
                    <div class="desc">
                        <span>{{selectErrorCode.remark}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('description') }}</label>
                    <div class="desc">
                        <span>{{selectErrorCode.desc}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('target') }}</label>
                    <div class="desc">
                        <span>{{selectErrorCode.target}}</span>
                    </div>
                </div>
                <div class="content-item">
                    <label class="title">{{ $t('solution') }}</label>
                    <div class="desc">
                        <p v-for="(item, index) in selectErrorCode.solution" v-if="item">{{index + 1}}. {{item}}</p>
                    </div>
                </div>
                <!--<div class="content-item">-->
                <!--<label class="title">Video</label>-->
                <!--<div class="desc">-->
                <!--<el-button type="primary">Download</el-button>-->
                <!--</div>-->
                <!--</div>-->
                <div class="tip">
                    <label class="tip-label">{{ $t('tip') }}: </label>
                    <div class="tip-content">
                        {{ $t('tipContent') }}
                    </div>
                </div>
                <div class="help">
                    <label class="help-label">{{ $t('help') }}:</label>
                    <div class="help-content">
                        <p>{{ $t('helpContent') }} <a class="btn-underline" @click="linkVideo">{{ $t('link') }}</a></p>
                    </div>
                </div>
            </div>
        </div>
        <el-dialog :visible.sync="errDialog" class="edit-error-dialog">
            <p class="dialog-title">{{errTitle}}</p>
            <el-form>
                <el-form-item label="Code">
                    <el-input v-model="errCode"></el-input>
                </el-form-item>
                <el-form-item label="Name">
                    <el-input v-model="errName"></el-input>
                </el-form-item>
                <el-form-item label="Remark">
                    <el-input v-model="errRemark"></el-input>
                </el-form-item>
                <el-form-item label="Description">
                    <el-input v-model="errDescription"></el-input>
                </el-form-item>
                <el-form-item label="Target">
                    <el-input v-model="errTarget"></el-input>
                </el-form-item>
                <el-form-item label="Solution">
                    <el-input v-model="errSolution" type="textarea"></el-input>
                </el-form-item>
            </el-form>
            <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="errDialog = false">cancel</el-button>
                <el-button type="success" @click="saveErrorMessage">confirm</el-button>
            </span>
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
        data() {
            return {
                errorId: '',
                errorCodeList: [],
                selectErrorCode: {},
                errDialog: false,
                errTitle: '',
                errCode: '',
                errName: '',
                errRemark: '',
                errDescription: '',
                errTarget: '',
                errSolution: '',
                errId: '',

                user: Core.Data.getUser().id,
            }
        },
        i18n: {
            messages: {
                en: EN.Dealer.Error.Shooting,
                zh: ZH.Dealer.Error.Shooting,
                de: DE.Dealer.Error.Shooting,
                nl: NL.Dealer.Error.Shooting
            }
        },
        mounted() {
            this.getComponentErrorList();
        },
        methods: {
            clearErrorMessage(){
                this.errCode = '';
                this.errName = '';
                this.errRemark = '';
                this.errDescription = '';
                this.errTarget = '';
                this.errSolution = '';
                this.errId = '';
                this.getComponentErrorList();
            },
            showErrDialog(info,type) {
                if(type === 'create'){
                    this.errCode = '';
                    this.errName = '';
                    this.errRemark = '';
                    this.errDescription = '';
                    this.errTarget = '';
                    this.errSolution = '';
                    this.errId = '';
                }else{
                   this.errCode =this.selectErrorCode.code;
                   this.errName =this.selectErrorCode.name;
                   this.errRemark =this.selectErrorCode.remark;
                   this.errDescription =this.selectErrorCode.desc;
                   this.errTarget =this.selectErrorCode.target;
                   this.errSolution = '';
                   if(Array.isArray(this.selectErrorCode.solution)){
                       this.selectErrorCode.solution.forEach((item)=>{
                           this.errSolution+=(item+'-');
                       })
                       this.errSolution = this.errSolution.substr(0,this.errSolution.length-1);
                   }else{
                       this.errSolution = this.selectErrorCode.solution;
                   }
                   this.errId =this.selectErrorCode.id;
                }
                this.errTitle = info;
                this.errDialog = true;
            },
            saveErrorMessage() {
                let errSolution = Array.isArray(this.errSolution)?JSON.stringify(this.errSolution):JSON.stringify(this.errSolution.split('-'));
                Core.Api.Error.saveErrorMessage(Number(this.errCode), this.errDescription, this.errId?this.errId:0, this.errName, this.errRemark, errSolution, this.errTarget).then(res=>{
                    if(res === 'success'){
                        this.$message({
                            type: 'success',
                            message: 'Save Successfully!'
                        });
                        this.errDialog = false;
                        this.clearErrorMessage();
                    }
                }).catch(err=>{
                    console.log(err);
                })
            },
            changeErrorCode() {
                let $this = this;
                $this.errorCodeList.some(function (e) {
                    if ($this.errorId === e.id) {
                        $this.selectErrorCode = e;
                        return true;
                    }
                });
            },
            getComponentErrorList() {
                let $this = this;
                Core.Api.Error.componentErrorList().then(res => {
                    $this.errorCodeList = res.error_list;
                    $this.selectErrorCode = $this.errorCodeList[0];
                    // $this.errId = $this.selectErrorCode.id;
                    $this.errorId = $this.selectErrorCode.id;
                });
            },
            linkVideo() {
                this.$router.push({name: 'brandErrorShootingVideoList', query: {"code": this.selectErrorCode.code}});
            }
        }
    }
</script>

<style lang="scss"  scoped>
    .error-shooting {
        .content-item {
            width: 100%;
            padding: 16px 0;
            border-bottom: 1px solid rgba(255, 255, 255, 0.10);
            font-size: 0;
            .title {
                display: inline-block;
                vertical-align: top;
                width: 140px;
                font-size: 14px;
                color: rgba(255, 255, 255, 0.6);
                padding: 0 5px;
            }
            .desc {
                display: inline-block;
                vertical-align: top;
                width: calc(100% - 140px);
                span {
                    font-size: 14px;
                    color: #FFFFFF;
                    &.strong {
                        font-weight: bold;
                    }
                }
                p {
                    font-size: 14px;
                    color: #FFFFFF;
                }
            }
        }
        .tip {
            display: flex;
            padding: 30px 5px;
            .tip-label {
                padding-right: 10px;
                color: #E26829;
            }
            .tip-content {
                flex: 1;
                color: #E26829;
            }
        }
        .help {
            display: flex;
            padding: 0 5px;
            .help-label {
                padding-right: 10px;
                color: #E26829;
            }
            .help-content {
                flex: 1;
                color: #E26829;
            }
        }
        .edit-error-dialog{
            .dialog-title{
                padding: 20px 0;
                text-align: center;
            }
            .el-form{
                .el-form-item{
                    .el-form-item__label{
                        width: 120px;
                        text-align: left!important;
                    }
                    .el-form-item__content{
                        .el-input{
                            width: 320px;
                        }
                        .el-textarea{
                            width: 320px;
                        }
                    }
                }
            }

        }
    }
</style>



// WEBPACK FOOTER //
// shooting.vue?a7729b4e