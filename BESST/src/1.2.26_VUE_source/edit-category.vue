<template>
    <div class="edit-category">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="toList" :title="$t('back')"></div>
                </div>
            </div>
        </div>
        <div class="content">
            <label class="label">{{ $t('category') }}</label>
            <div class="category-tree">
                <el-tree
                    :data="categoryList"
                    :props="defaultProps"
                    default-expand-all
                    :render-content="renderContent"
                >
                </el-tree>
            </div>
        </div>

        <el-dialog :title="$t('deleteDialog.title')" :visible.sync="dialogDeleteVisible"  size="tiny">
            <p> {{ $t('deleteDialog.content') }} </p>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="deleteCategory">{{ $t('yes') }}</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">{{ $t('no') }}</el-button>
            </div>
        </el-dialog>
        <!-- 增加 Category-->
        <el-dialog :title="$t('addSubCategoryDialog.title')" :visible.sync="dialogAddVisible">
            <el-input v-model="categoryName" :placeholder="$t('addSubCategoryDialog.namePH')"></el-input>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="addSubCategory">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogAddVisible = false">{{ $t('cancel') }}</el-button>
            </div>
        </el-dialog>

        <el-dialog :title="$t('editCategoryDialog.title')" :visible.sync="dialogEditVisible">
            <el-input v-model="categoryName" :placeholder="$t('editCategoryDialog.namePH')"></el-input>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="editCategory">{{ $t('confirm') }}</el-button>
                <el-button type="primary" @click="dialogEditVisible = false">{{ $t('cancel') }}</el-button>
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

    export default {
        data() {
            return {
                dialogDeleteVisible: false,
                dialogAddVisible: false,
                dialogEditVisible: false,
                currentID: '',
                categoryName: '',
                org: Core.Data.getOrg(),

                categoryList: [],
                defaultProps: {
                    id: 'id',
                    children: 'child',
                    label: 'name',
                }
            }
        },
        i18n: {
            messages: {
                en: EN.Component.Product.EditCategory,
                zh: ZH.Component.Product.EditCategory,
                de: DE.Component.Product.EditCategory,
                nl: NL.Component.Product.EditCategory
            }
        },
        mounted: function() {
            this.getList();
        },
        methods: {
            getList(){
                Core.Api.Category.getAllCategoryList(Core.Const.COMPONENT_ROOT_CATEGORY).then(
                    res => {
                        this.categoryList = Core.Util.sortListTree(res.category_list);
                    }
                );
            },

            toList(){
                this.$router.push({name: 'componentProductList'});
            },


            append(store, data) {
                console.log(data);
                console.log(store);
            },

            showDeleteDialog(store, data) {
                if (data.org_id !== this.org.id) {
                    this.$message({
                        message: 'You have no permission',
                        type: 'warning',
                    });
                    return;
                }
                this.dialogDeleteVisible = true;
                this.currentID = data.id;
            },

            showEditDialog(store, data) {
//                console.log(data);
//                console.log(data.org_id); // 25
//                console.log(this.org.id);
                if (data.org_id !== this.org.id) {
                    this.$message({
                        message: 'You have no permission',
                        type: 'warning',
                    });
                    return;
                }
                this.categoryName = data.name;
                this.currentID = data.id;
                this.dialogEditVisible = true;
            },

            showAddDialog(store, data) {
            	if (data.level === 2 && data.org_id !== this.org.id) {
                    this.$message({
                        message: 'You have no permission',
                        type: 'warning',
                    });
            		return;
                }
                this.categoryName = '';
                this.currentID = data.id;
                this.dialogAddVisible = true;
            },

            toAttribute(data){
                this.currentID = data.id;
                this.$router.push({name: 'componentProductAttribute', params: {"categoryId": this.currentID}});
            },
            //删除当前分组
            deleteCategory(){
                Core.Api.Category.deleteCategory(this.currentID).then(res => {
                    this.dialogDeleteVisible = false;
                    this.getList();
                    this.$message({
                        message: 'Delete successfully',
                        type: 'success',
                    });
                });
            },
            //修改当前分租
            editCategory(){
                Core.Api.Category.editCategory(this.currentID, this.categoryName).then(res => {
                    this.dialogEditVisible = false;
                    this.getList();
                    this.$message({
                        message: 'Edit successfully',
                        type: 'success',
                    });
                });
            },
            //添加子分组
            addSubCategory(){
                Core.Api.Category.saveCategory(this.categoryName, this.currentID).then(res => {
                    this.dialogAddVisible = false;
                    this.getList();
                    this.$message({
                        message: 'Add successfully',
                        type: 'success',
                    });
                });
            },

            renderContent(h, {node, data, store}) {
                return (
                  <span>
                    <span>
                        <span>{node.label}</span>
                    </span>
                    <span style="float: right; margin-right: 20px">
                    {
                        node.level>0 && node.level<4 ? <el-button size="mini" type='text' on-click={ (e) => {e.stopPropagation(); this.showAddDialog(store, data)}} >{this.$t('addSubCategory')}</el-button>
                        : null
                    }
                    {
                        node.level>1 && node.level<5 ? <el-button size="mini" type='text' on-click={ (e) => {e.stopPropagation(); this.showEditDialog(store, data)}} >{this.$t('edit')}</el-button>
                        : null
                    }
                    {
                        node.level>2 && node.level<4 ? <el-button size="mini" type='text' on-click={ (e) => {e.stopPropagation(); this.showDeleteDialog(store, data)}} >{this.$t('delete')}</el-button>
                        : null
                    }

                    </span>
                </span>);
            }
        }
    };
</script>

<style lang="scss" rel="stylesheet/scss">
    .edit-category {
        width: 100%;
        height: 100%;
        .header {
            line-height: 60px;
            height: 60px;
            padding: 0 30px;
            border-bottom: 1px solid #2B2F3B;
        }
        .content {
            height: calc(100% - 60px);
            background: #353945;
            overflow: auto;
            .label {
                font-size: 16px;
                color: #A1A5B1;
                padding: 9px 33px;
                display: inline-block;
            }
            .category-tree {
                margin-left: 34px;
            }
        }
    }
</style>



// WEBPACK FOOTER //
// edit-category.vue?04eb7fd3