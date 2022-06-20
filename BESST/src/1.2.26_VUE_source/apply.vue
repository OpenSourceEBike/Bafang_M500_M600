<template>
    <div class="apply-list-container">
        <div class="container-wrap">
            <div class="apply-btn">
                <el-button type="primary" @click="toCreateOrg">Create</el-button>
            </div>
            <div class="apply-list">
                <el-table :data="applyList">
                    <el-table-column v-for="(item,index) in applyHeadList" :label="item.label" :prop="item.name" :key="index"></el-table-column>
                    <el-table-column label="Operation" width="100">
                        <template slot-scope="scope">
                            <a class="btn-underline" @click.stop="clickEditSpec(scope.row)"
                               v-if="scope.row.audit_type == '未审批' ">Edit</a>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
            <el-dialog :visible.sync="createOrg" title="Create organization account for customer">
                <el-form  :model="orgForm" :rules="rules" ref="orgForm" label-width="100px" >
                    <el-form-item label="Logo">
                        <el-upload
                            :action="options.url"
                            :on-success="onSuccess"
                            :multiple="true"
                            :auto-upload= true
                            :show-file-list = false
                            :data="options.params">
                            <el-button size="small" type="primary">点击上传</el-button>
                            <div slot="tip" class="el-upload__tip">只能上传jpg/png文件，且不超过500kb</div>
                        </el-upload>
                        <img :src="showImgURL" style="width: 32px;height: 32px;"/>
                    </el-form-item>
                    <el-form-item label="Name" prop="name">
                        <el-input v-model="orgForm.name"></el-input>
                    </el-form-item>
                    <el-form-item label="Type" prop="type">
                        <el-select v-model="orgForm.type" clearable placeholder="请选择">
                            <el-option
                                v-for="item in typeList"
                                :key="item.value"
                                :label="item.label"
                                :value="item.value">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Country" prop="country">
                        <el-select v-model="orgForm.country" class="land" filterable @change="showAreaCode">
                            <el-option
                                v-for="(item, index) in areaCodeList"
                                :key="index"
                                :label="item.area_en"
                                :value="item.area_en">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Address" prop="address">
                        <el-input v-model="orgForm.address"></el-input>
                    </el-form-item>
                    <el-form-item label="Postcode" prop="postcode">
                        <el-input v-model="orgForm.postcode"></el-input>
                    </el-form-item>
                    <el-form-item label="Email" prop="email">
                        <el-input v-model="orgForm.email"></el-input>
                    </el-form-item>
                    <el-form-item label="Phone" prop="phone">
                        <el-input v-model="orgForm.phone">
                            <template slot="prepend">+{{areaCode}}</template>
                        </el-input>
                    </el-form-item>
                    <el-form-item label="Website" prop="website">
                        <el-input v-model="orgForm.website"></el-input>
                    </el-form-item>
                    <el-form-item label="Applicant" prop="applicant">
                        <el-input v-model="orgForm.applicant"></el-input>
                    </el-form-item>
                    <el-form-item label="Auditor"  prop="reviewer" v-if="authMemberList.length>0">
                        <el-select v-model="orgForm.reviewer" clearable placeholder="请选择审核人" @change="showAuditor">
                          <el-option
                            v-for="item in authMemberList"
                            :label="item.account"
                            :value="item.account"
                            :key="item.value">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Describe">
                        <el-input v-model="orgForm.specific_information"></el-input>
                    </el-form-item>
                </el-form>
                <span slot="footer" class="dialog-footer">
                    <el-button type="primary" @click="createOrg = false">cancel</el-button>
                    <el-button type="success" @click="saveOrgList">save</el-button>
                </span>
            </el-dialog>
            <el-dialog :visible.sync="editOrg" title="Edit organization account for customer">
                <el-form  :model="selectApply" :rules="rules" ref="orgForm" label-width="100px" >
                    <el-form-item label="Logo">
                        <el-upload
                            :action="options.url"
                            :on-success="onSuccess"
                            :multiple="true"
                            :auto-upload= true
                            :show-file-list = false
                            :data="options.params">
                            <el-button size="small" type="primary">点击上传</el-button>
                            <div slot="tip" class="el-upload__tip">只能上传jpg/png文件，且不超过500kb</div>
                        </el-upload>
                        <img :src="showImgURL" style="width: 32px;height: 32px;"/>
                    </el-form-item>
                    <el-form-item label="Name" prop="name">
                        <el-input v-model="selectApply.name"></el-input>
                    </el-form-item>
                    <el-form-item label="Type" prop="type">
                        <el-select v-model="selectApply.type" clearable placeholder="请选择">
                            <el-option
                                v-for="item in typeList"
                                :key="item.value"
                                :label="item.label"
                                :value="item.value">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Country" prop="country">
                        <el-select v-model="selectApply.country" class="land" filterable>
                            <el-option
                                v-for="(item, index) in areaCodeList"
                                :key="index"
                                :label="item.area_en"
                                :value="item.area_en">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Address" prop="address">
                        <el-input v-model="selectApply.address"></el-input>
                    </el-form-item>
                    <el-form-item label="Postcode" prop="postcode">
                        <el-input v-model="selectApply.postcode"></el-input>
                    </el-form-item>
                    <el-form-item label="Email" prop="email">
                        <el-input v-model="selectApply.email" :disabled="editOrg" placeholder="必须准确"></el-input>
                    </el-form-item>
                    <el-form-item label="Phone" prop="phone">
                        <el-input v-model="selectApply.phone">
                            <template slot="prepend">+{{areaCode}}</template>
                        </el-input>
                    </el-form-item>
                    <el-form-item label="Website" prop="website">
                        <el-input v-model="selectApply.website"></el-input>
                    </el-form-item>
                    <el-form-item label="Applicant"  prop="applicant">
                        <el-input v-model="selectApply.applicant"></el-input>
                    </el-form-item>
                    <el-form-item label="Auditor"  prop="reviewer" v-if="authMemberList.length>0">
                        <el-select v-model="selectApply.reviewer" clearable placeholder="请选择审核人" @change="showAuditor">
                            <el-option
                                v-for="item in authMemberList"
                                :label="item.account"
                                :value="item.account"
                                :key="item.value">
                            </el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Describe">
                        <el-input v-model="selectApply.specific_information"></el-input>
                    </el-form-item>
                </el-form>
                <span slot="footer" class="dialog-footer">
                    <el-button type="primary" @click="editOrg = false">cancel</el-button>
                    <el-button type="success" @click="saveOrgList">save</el-button>
                </span>
            </el-dialog>
            <el-dialog :visible.sync="historyRecord" size="large">
                <div v-show = "recordType == 1">
                    <p>This email account was applied by other people.</p>
                    <el-table :data="recordApply">
                        <el-table-column v-for="(item, index) in applyHeadList" :label="item.label" :prop="item.name" :key="index"></el-table-column>
                    </el-table>
                </div>
                <div v-show = "recordType == 3">
                    <p>This email account has registered before.</p>
                    <el-table :data="recordApply">
                        <el-table-column v-for="(item, index) in userHeadList" :label="item.label" :prop="item.name" :key="index"></el-table-column>
                    </el-table>
                </div>
                <span slot="footer" class="dialog-footer">
                <el-button type="primary" @click="historyRecord = false">cancel</el-button>
                </span>
            </el-dialog>
        </div>
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
        data () {
            return {
                createOrg: false,
                editOrg: false,
                historyRecord: false,
                orgForm: {
                    id: 0,
                    url: '',
                    name: '',
                    type: '',
                    country: '',
                    address: '',
                    postcode: '',
                    email: '',
                    phone: '',
                    website: '',
                    applicant: '',
                    reviewer: '',
                    specific_information: '',
                },
                options: {
                    url: Core.Const.NET.IMG_UPLOAD_END_POINT,
                    paramName: 'image',
                    params: {
                        token: Core.Data.getToken(),
                    },
                },
                showImgURL: '',
                rules: {
                    name: [
                        { required: true, message: '请输入公司名称', trigger: 'blur' },
                    ],
                    type: [
                        { required: true, message: '请选择类型', trigger: ['blur','change'] },
                    ],
                    country: [
                        { required: true, message: '请选择国家', trigger: 'blur' },
                    ],
                    address: [
                        { required: true, message: '请输入地址', trigger: 'blur' },
                    ],
                    postcode: [
                        { required: true, message: '请输入邮编', trigger: 'blur' },
                    ],
                    email: [
                        { required: true, message: '请输入邮箱', trigger: 'blur' },
                    ],
                    applicant: [
                        { required: true, message: '请输入申请人', trigger: 'blur' },
                    ],
                    reviewer: [
                        { required: true, message: '请选择审核人', trigger: ['blur','change'] },
                    ],
                },
                authMemberList: [],
                typeList: [
                    {
                        value: '1',
                        label: 'Brand',
                    },
                    {
                        value: '2',
                        label: 'Dealer',
                    },
                    {
                        value: '3',
                        label: 'Assembler',
                    }
                ],
                applyList: [],
                applyHeadList: [
                    {
                        name: 'name',
                        label: 'Name',
                    },
                    {
                        name: 'type',
                        label: 'Type',
                    },
                    {
                        name: 'email',
                        label: 'Email',
                    },
                    {
                        name: 'country',
                        label: 'Country',
                    },
                    {
                        name: 'address',
                        label: 'Address',
                    },
                    {
                        name: 'postcode',
                        label: 'Post Code',
                    },
                    {
                        name: 'applicant',
                        label: 'Applicant',
                    },
                    {
                        name: 'reviewer',
                        label: 'Auditor',
                    },
                    {
                        name: 'audit_type',
                        label: 'Audit Status',
                    }
                ],
                auditTypeInfo: {
                    0: '未审批',
                    1: '已审批'
                },
                userHeadList: [
                    {
                        name: 'id',
                        label: 'ID',
                    },
                    {
                        name: 'email',
                        label: 'Email',
                    },
                    {
                        name: 'create_time',
                        label: 'Create Time',
                    }
                ],
                selectApply: {},
                recordApply: [],
                recordType: 0,

                areaCodeList: [],
                areaCode: '',
                countryCode: 0,

                isSaving: false,

            }
        },
        methods: {
            showAreaCode(val) {
                console.log(val);
                this.areaCodeList.forEach((item)=>{
                    if(val === item.area_en){
                        this.areaCode = item.area_code;
                        this.countryCode = item.country_code || 0;
                    }
                })
            },
            showAuditor(val) {
                console.log(val);
            },
            toCreateOrg() {
                this.createOrg = true;
            },
            saveOrgList() {
                this.isSaving = true;
                let org;
                if(this.createOrg && !this.editOrg){
                    org = this.orgForm;
                    this.recordType = 0;
                    this.recordApply = [];
                }else if(!this.createOrg && this.editOrg){
                    org = Core.Data.deepClone(this.selectApply);
                    let res = this.typeList.filter((item)=>{return item.label == this.selectApply.type }) ;
                    org.type = parseInt(res[0].value);
                    org.audit_type = this.selectApply.audit_type == "未审批" ? 0 : 1;
                }

                if ( !org.name ) {
                    this.showErrMessage( '名字');
                    return false;
                }
                if ( !org.type ) {
                    this.showErrMessage( '类型');
                    return false;
                }
                if ( !org.country ) {
                    this.showErrMessage( '国家');
                    return false;
                }
                if ( !org.address ) {
                    this.showErrMessage( '地址');
                    return false;
                }
                if ( !org.postcode ) {
                    this.showErrMessage( '邮编');
                    return false;
                }
                if ( !org.email ) {
                    this.showErrMessage( '邮箱');
                    return false;
                }
                const regEmail = /^[a-z0-9]+([._\\-]*[a-z0-9])*@([a-z0-9]+[-a-z0-9]*[a-z0-9]+.){1,63}[a-z0-9]+$/;
                if ( !( regEmail.test( org.email ) ) ) {
                    this.$message({
                        type: 'warning',
                        message: '邮箱不合法，请重新输入!',
                    });
                    return false;
                }
                const data = org;
                data.specificInformation = org.specific_information;
                data.country_status = this.countryCode;
                Core.Api.request({
                    url: '/orgAudit/save',
                    method: 'GET',
                    data: data,
                    headers: {
                        'content-type': 'application/x-www-form-urlencoded;charset=utf-8',
                    },
                }).then( (res)=> {
                    if(this.createOrg && res.data.hasOwnProperty("code")){
                        if( res.data.code === 0 ) {
                            this.$message({
                                type: 'success',
                                message: 'Save Successfully!',
                            })
                            this.orgForm = {
                                id: 0,
                                url: '',
                                name: '',
                                type: '',
                                country: '',
                                address: '',
                                postcode: '',
                                email: '',
                                phone: '',
                                website: '',
                                applicant: '',
                                reviewer: '',
                                specific_information: '',
                            };
                            this.createOrg = false;
                            this.countryCode = 0;
                            this.getApplyList();
                        } else if(res.data.code === 1) {
                            // 审核重复
                            this.historyRecord = true;
                            this.recordType = 1;
                            this.recordApply.push(res.data.data);
                        } else if(res.data.code === 3){
                            // 用户重复
                            this.historyRecord = true;
                            this.recordType = 3;
                            let temArr = res.data.data;
                            temArr.create_time = Core.Data.getFormatterDate(temArr.create_time * 1000);
                            this.recordApply.push(temArr);
                        }
                    }else if(this.editOrg && res.data.hasOwnProperty("code")){
                        if(res.data.code === 0){
                            this.$message({
                                type: 'success',
                                message: 'Save Successfully!',
                            })
                            this.selectApply = {};
                            this.editOrg = false;
                            this.countryCode = 0;
                            this.getApplyList();
                        }
                    }
                    this.isSaving = false;
                }).catch( (err)=>{
                    this.isSaving = false;
                    console.log(err);
                })
            },
            showErrMessage(msg){
                this.$message({
                    message: "Please check" + msg,
                    type: 'warning',
                });
            },
            onSuccess( response, file, fileList ) {
                if ( response.code !== 0 ) {
                    this.$message({
                        message: response.message,
                        type: 'error',
                    });
                } else {
                    this.showImgURL = Core.Const.NET.IMG_URL_PREFIX + response.data.file.name;
                    this.$set( this.orgForm, 'avatar', response.data.file.name );
                }
            },
            getApplyList() {
                Core.Api.request({
                    url: '/orgAudit/selectAllOrgAudit',
                    method: 'GET'
                }).then( (res)=> {
                     if(Array.isArray(res.data.list) && res.data.list.length >0 ){
                        this.applyList = res.data.list;
                        this.applyList.forEach((item)=>{
                            this.typeList.forEach((typeItem)=>{
                                if(item.type === Number(typeItem.value)) {
                                    item.type = typeItem.label;
                                }
                            })
                            item.audit_type = this.auditTypeInfo[item.audit_type];
                        })
                    }
                }).catch( (err) => {
                    console.log(err);
                })
            },
            getAuthMemberList() {
                Core.Api.request({
                    url: '/orgAudit/selectAll',
                    method: 'GET',
                    data: {
                        account_type: 3,
                    },
                }).then( (res)=> {
                    if(Array.isArray(res.data.list) && res.data.list.length >0 ){
                        this.authMemberList = res.data.list;
                        this.authMemberList.forEach((item)=>{
                            item.value = item.id.toString();
                        })
                    }
                }).catch( (err) => {
                    console.log(err);
                })
            },
            clickEditSpec(item) {
                this.selectApply = item;
                this.showImgURL = Core.Const.NET.IMG_URL_PREFIX + item.avatar;
                this.editOrg = true;
            },
        },
        mounted() {
            this.getAuthMemberList();
            this.getApplyList();
            this.areaCodeList = areaCodeInDB.init();
        },
    }
</script>
<style lang="scss" scoped>
   .apply-list-container {
       width: 100%;
       height: 100%;
       display:flex;
       flex-direction:column;
       .container-wrap{
           width: 100%;
           height: 100%;
           overflow: auto;
           .apply-btn{
               padding: 20px;
           }
       }
   }
</style>



// WEBPACK FOOTER //
// apply.vue?14d28277