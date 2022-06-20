<template>
    <div class="audit-list-container">
        <div class="container-wrap">
            <el-table :data="applyList">
                <el-table-column v-for="(item,index) in applyHeadList" :label="item.label" :prop="item.name"></el-table-column>
                <el-table-column label="Operation">
                    <template slot-scope="scope">
                        <el-button @click="toAuditApply(scope.row)" :disabled="isAuditing" v-show="scope.row.audit_type==='未审批'">Audit</el-button>
                    </template>
                </el-table-column>
            </el-table>
        </div>
    </div>
    <!--<el-dialog title="Audit Apply" :visible="auditVisible">
        <p>Are you audit the apply?</p>
   </el-dialog>-->
</template>
<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';
    export default {
        data () {
            return {
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
                    // {
                    //     name: 'specific_information',
                    //     label: 'Describe',
                    // },
                ],
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
                        value: '6',
                        label: 'Special Dealer',
                    },
                    {
                        value: '3',
                        label: 'Assembler',
                    },
                    {
                        value: '4',
                        label: 'Component',
                    },
                    {
                        value: '5',
                        label: 'Service Center',
                    }
                ],
                authMemberList: [],
                auditTypeInfo: {
                    0: '未审批',
                    1: '已审批'
                },
                isAuditing: false,
            }
        },
        methods: {
            getApplyList() {
                Core.Api.request({
                    url: '/orgAudit/selectAllOrgAudit',
                    method: 'GET'
                }).then( (res)=> {
                    console.log(res);
                     if(Array.isArray(res.data.list) && res.data.list.length >0 ){
                        this.applyList = res.data.list;
                        this.applyList.forEach((item)=>{
                            this.typeList.forEach((typeItem)=>{
                                if(item.type === Number(typeItem.value)) {
                                    item.type = typeItem.label;
                                }
                            })
                            this.authMemberList.forEach((authItem)=>{
                                if(item.reviewer === authItem.value) {
                                    item.reviewer = authItem.account;
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
                    console.log(res);
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
            toAuditApply(item) {
//                console.log(item);
                this.isAuditing = true;
                const data = {
                    id: item.id,
                };
                Core.Api.request({
                    url: '/orgAudit/reviewer',
                    method: 'GET',
                    data: data,
                }).then((res)=>{
                    if(res.code === 0){
                        this.$message({
                            type: 'success',
                            message: 'Audit Successfully!',
                        })
                        this.selectApply = {};
                        this.editOrg = false;
                        this.countryCode = 0;
                        this.getApplyList();
                        this.isAuditing = false;
                    }
                }).catch((err)=>{
                    this.isAuditing = false;
                    console.log(err);
                })
            },
        },
        mounted() {
            this.getApplyList();
        },
    }
</script>
<style lang="scss" scoped>
    .audit-list-container{
        width: 100%;
        height: 100%;
        display:flex;
        flex-direction:column;
        .container-wrap{
            width: 100%;
            height: 100%;
            overflow: auto;
        }
    }
</style>



// WEBPACK FOOTER //
// audit.vue?194ba78c