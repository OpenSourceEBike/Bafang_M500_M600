<template>
  <div class="permission-category">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Category Code">
          <el-input v-model="formSearch.permission_category_code" placeholder="Category Code"></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item add-v2" @click="dialogAdd" v-if="true" title="add"></div>
        </div>
      </div>
    </div>
    <div class="table-wrap" v-loading.body="loading">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length <= 0 && !loading">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody v-if="!loading && tableList.length > 0">
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.id }}</td>
              <td>{{ item.permission_category_code }}</td>
              <td>{{ item.permission_category_name }}</td>
              <td>
                <a class="btn-underline" @click.stop="onEdit(item)">edit</a>
                <a class="btn-underline" @click.stop="onDelete(item)">delete</a>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
      <div class="table-footer">
        <el-pagination
          @current-change="handleCurrentChange"
          :current-page.sync="currentPage"
          :page-size="10"
          layout="prev, pager, next, jumper"
          :total="total"
        ></el-pagination>
        <p class="total">total: {{total}}</p>
      </div>
    </div>
    <!-- 添加or修改弹窗 -->
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogCategoryVisible"
      class="dialog-permission"
    >
      <div class="item">
        <div class="item-box">
          <label class="label">Category Code
            <span class="required">*</span>
          </label>
          <el-input v-model="category.permission_category_code" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">Category Name
            <span class="required">*</span>
          </label>
          <el-input v-model="category.permission_category_name" class="input"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Permission" :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
export default {
  data() {
    return {
      loading: false,
      formSearch: {
        permission_category_code: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,

      //弹窗
      category: {
        permission_category_code: "",
        permission_category_name: ""
      },
      dialogTitle: "",
      dialogCategoryVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前删除ID
      currentID: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.getPermissionCategory();
    this.headData = ["ID", "Category Code", "Category Name", "Operation"];
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getPermissionCategory();
    },
    onClean() {
      this.formSearch = {
        permission_category_code: ""
      };
      this.getPermissionCategory();
    },
    // 操作
    onEdit(event) {
      this.category = JSON.parse(JSON.stringify(event));
      this.dialogTitle = "Update Category";
      this.dialogCategoryVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getPermissionCategory();
    },

    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Category";
      this.dialogCategoryVisible = true;
      this.category = {
        permission_category_code: "",
        permission_category_name: ""
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deletePermissionCategory(scope.currentID).then(
        res => {
          scope.loading = false;
          this.dialogDeleteVisible = false;
          scope.getPermissionCategory();
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    dialogConfirm() {
      if (!this.category.permission_category_code) {
        this.$message({
          message: "Please input category code",
          type: "warning"
        });
        return;
      }
      if (!this.category.permission_category_name) {
        this.$message({
          message: "Please input category name",
          type: "warning"
        });
        return;
      }

      this.loading = true;
      const scope = this;
      this.postPermissionCategory(this.category).then(
        res => {
          scope.loading = false;
          scope.getPermissionCategory();
          scope.dialogCategoryVisible = false;
        },
        err => {
          scope.loading = false;
          scope.dialogCategoryVisible = false;
        }
      );
    },
    dialogCancel() {
      this.dialogCategoryVisible = false;
      this.category = {};
    },
    // 获取权限分类
    getPermissionCategory() {
      this.loading = true;
      const scope = this;
      Core.Api.request({
        method: "post",
        url: "/nl/public/permission/category/filter",
        data: {
          permission_category_code: this.formSearch.permission_category_code
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          scope.loading = false;
          this.tableList = res.content;
          this.total = res.totalPages * res.size;
        },
        err => {
          scope.loading = false;
        }
      );
    },
    postPermissionCategory(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/permission/category",
        data: data
      });
    },
    deletePermissionCategory(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/public/permission/category/" + id
      });
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.permission-category {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .list-form-inline {
    height: 100%;
    line-height: 50px;
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__content {
      width: 180px !important;
    }
  }
  .table-wrap {
    .long-td {
      max-width: 270px;
      text-overflow: ellipsis;
      overflow: hidden;
      text-align: left;
    }
  }

  .dialog-permission {
    .el-dialog__body {
      padding: 30px 66px;
    }
    .item {
      margin: 20px 0;
      font-size: 0;
      height: 30px;
      .item-box {
        line-height: 30px;
      }
      .label {
        display: inline-block;
        width: 120px;
        height: 30px;
        vertical-align: middle;
      }
      .input {
        margin-left: 40px;
        width: 260px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// permission-category.vue?7102250c