<template>
    <div class="attribute-list">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="toEditCategory" :title="$t('back')"></div>
                </div>
            </div>
            <div class="btn-group-top">
                <div class="icons">
                    <div class="icon-item add" @click="dialogAddVisible = true" title="Add New Attribute"></div>
                </div>
            </div>
        </div>
        <div class="table-content">
            <table class="c-table" border="0" cellspacing="0" cellpadding="0">
                <thead>
                <td v-for="item in headData">{{item}}</td>
                </thead>
                <tbody>
                <tr v-for="item in attributeList" @click="test(item)">
                    <td>{{item.name}}</td>
                    <td>{{item.input_type | inputType}}</td>
                    <td>{{item.value || 'N/A'}}</td>
                    <td>
                        <a class="btn-underline" v-on:click.stop="showEditDialog(item.id)">Edit</a>
                        <a class="btn-underline" v-on:click.stop="showDeleteDialog(item.id)">Delete</a>
                    </td>
                </tr>
                </tbody>
            </table>
        </div>
        <el-dialog title="Add New Attribute" :visible.sync="dialogAddVisible" class="dialog">
            <el-input placeholder="New Attribute Name" class="input" v-model="attributeName"></el-input>
            <el-select v-model="attributeInputType" class="select">
                <el-option
                    v-for="item in inputTypes"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value"></el-option>
            </el-select>
            <el-input placeholder="Attribute Value" class="input" v-model="attributeValue"></el-input>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="addAttribute">Confirm</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">Cancel</el-button>
            </div>
        </el-dialog>

        <el-dialog title="Edit Attribute" :visible.sync="dialogEditVisible" class="dialog">
            <el-input placeholder="Attribute New Name" class="input" v-model="attributeName"></el-input>
            <el-select v-model="attributeInputType" class="select">
                <el-option
                    v-for="item in inputTypes"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value"></el-option>
            </el-select>
            <el-input placeholder="Attribute New Value" class="input" v-model="attributeValue"></el-input>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="editAttribute">Confirm</el-button>
                <el-button type="primary" @click="dialogEditVisible = false">Cancel</el-button>
            </div>
        </el-dialog>

        <el-dialog title="Delete Attribute" :visible.sync="dialogDeleteVisible"  size="tiny">
            <p> Are you sure to delete it? </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteAttribute">Yes</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">No</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script>
    import Core from 'core/core'
    export default{

        name: 'attributeList',
        categoryId: '',

        data(){
            return {
                dialogDeleteVisible: false,
                dialogEditVisible: false,
                dialogAddVisible: false,
                categoryId: '',
                currentId: '',
                attributeName: '',
                attributeValue: '',
                inputTypes: [{
                    value: 1,
                    label: 'Text',
                }, {
                    value: 2,
                    label: 'Select',
                }, {
                    value: 3,
                    label: 'Radio',
                }, {
                    value: 4,
                    label: 'CheckBox',
                }],
                attributeInputType: 1,

                attributeList: [],
                tableHead: ['name', 'type', 'value', 'Operation'],
            }
        },
        methods: {
            getTabeleData(id){
                Core.Api.Category.getAttrDefList(id).then(
                    res => {
                        console.log(res);
                        this.attributeList = res.attr_def_list;
                    }
                );
            },

            toEditCategory(){
                this.$router.push({name: 'componentProductEditCategory'});
            },
            showEditDialog(id){
                this.currentId = id;
                this.dialogEditVisible = true;
            },
            showDeleteDialog(id){
                this.currentId = id;
                this.dialogDeleteVisible = true;
            },
            deleteAttribute(){
                Core.Api.Category.deleteAttrDef(this.currentId).then(res => {
                    this.dialogDeleteVisible = false;
                    this.$message({
                        message: "Delete successfully",
                        type: 'success'
                    });
                    this.getTabeleData(this.categoryId);
                });
            },
            addAttribute(){
                Core.Api.Category.saveAttrDef('', this.categoryId, this.attributeInputType, this.attributeName, this.attributeValue).then(res => {
                    this.dialogAddVisible = false;
                    this.$message({
                        message: "Add successfully",
                        type: 'success'
                    });
                    this.getTabeleData(this.categoryId);
                    this.initValue();
                });

            },
            editAttribute(){
                Core.Api.Category.saveAttrDef(this.currentId, this.categoryId, this.attributeInputType, this.attributeName, this.attributeValue).then(res => {
                    this.dialogEditVisible = false;
                    this.$message({
                        message: "Edit successfully",
                        type: 'success'
                    });
                    this.getTabeleData(this.categoryId);
                    this.initValue();
                });
            },
            initValue(){
                this.attributeName = '';
                this.attributeValue = '';
                this.attributeInputType = 1;
            }
        },

        mounted: function() {
            this.categoryId = this.$route.params.categoryId;
            this.getTabeleData(this.categoryId);
        },

    }
</script>

<style lang="scss" rel="stylesheet/scss">
    .attribute-list {
        width: 100%;
        height: 100%;
        .header {
            height: 60px;;
            border-bottom: 1px solid #2B2F3B;
            overflow: hidden;
            padding: 17px 30px;
            .right-btn {
                float: right;
                margin-right: 30px;
            }
        }
        .dialog {
            .select {
                margin: 20px 0;
            }

        }
    }
</style>



// WEBPACK FOOTER //
// attribute-list.vue?26c5248b