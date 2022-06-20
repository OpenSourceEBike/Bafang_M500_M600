<template>
  <div class="permission-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="name">
          <el-input v-model="formSearch.permission_name" placeholder="name"></el-input>
        </el-form-item>
        <el-form-item label="method">
          <el-input v-model="formSearch.http_method" placeholder="method"></el-input>
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
              <td>{{ item.permission_name }}</td>
              <td>{{categoryName(item)}}</td>
              <td>{{ item.http_method }}</td>
              <td>{{ item.url }}</td>
              <td>{{ item.modifier }}</td>
              <td>{{ item.parent_resource_id }}</td>
              <td>{{ item.resource_id }}</td>
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
      :visible.sync="dialogPermissionVisible"
      class="dialog-permission"
    >
      <div class="item">
        <div class="item-box">
          <label class="label">
            Permission Name
            <span class="required">*</span>
          </label>
          <el-input v-model="permission.permission_name" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Category
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="permission.permission_category_id"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="(item,index) in permissionCategoryList"
              :key="index"
              :label="item.permission_category_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Method
          </label>
          <el-select class="input" v-model="permission.http_method" filterable slot="prepend">
            <el-option
              v-for="(item,index) in httpMethodList"
              :key="index"
              :label="item.name"
              :value="item.value"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            URL
          </label>
          <el-input v-model="permission.url" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Modifier
            <span class="required">*</span>
          </label>
          <el-input v-model="permission.modifier" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">Parent Resource Id</label>
          <el-input v-model="permission.parent_resource_id" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Resource Id
          </label>
          <el-input v-model="permission.resource_id" class="input"></el-input>
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
        permission_name: "",
        http_method: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,

      //弹窗
      httpMethodList: [
        {
          name: "GET",
          value: "get"
        },
        {
          name: "POST",
          value: "post"
        },
        {
          name: "DELETE",
          value: "delete"
        },
        {
          name: "PUT",
          value: "put"
        }
      ],
      permission: {
        http_method: "",
        modifier: "",
        permission_name: "",
        url: "",
        permission_category_id: "",
        parent_resource_id: "",
        resource_id: ""
      },
      permissionCategoryList: [],
      dialogTitle: "",
      dialogPermissionVisible: false,
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
    this.getPermission();
    this.loading = true;
    this.getPermissionCategory().then(
      res => {
        this.permissionCategoryList = res.content;
        this.loading = false;
      },
      err => {
        this.loading = false;
      }
    );
    this.headData = [
      "ID",
      "Name",
      "Category",
      "Method",
      "URL",
      "Modifier",
      "Parent Resource Id",
      "Resource Id",
      "Operation"
    ];
  },
  methods: {
    // 设置表格仓库分类
    categoryName: function(item) {
      if (this.permissionCategoryList.length > 0) {
        const data = this.permissionCategoryList.filter(res => {
          return res.id === item.permission_category_id;
        });
        if (data.length > 0) {
          return data[0].permission_category_name;
        } else {
          return "-";
        }
      }
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getPermission();
    },
    onClean() {
      this.formSearch = {
        permission_name: "",
        http_method: ""
      };
      this.getPermission();
    },
    // 操作
    onEdit(event) {
      this.permission = JSON.parse(JSON.stringify(event));
      this.dialogTitle = "Update Permission";
      this.dialogPermissionVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getPermission();
    },

    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Permission";
      this.dialogPermissionVisible = true;
      this.permission = {
        http_method: "",
        modifier: "",
        permission_name: "",
        url: "",
        permission_category_id: "",
        parent_resource_id: "",
        resource_id: ""
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deletePermission(scope.currentID).then(
        res => {
          scope.loading = false;
          this.dialogDeleteVisible = false;
          scope.getPermission();
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    dialogConfirm() {
      if (!this.permission.permission_name) {
        this.$message({
          message: "Please input Permission Name",
          type: "warning"
        });
        return;
      }

      // if (!this.permission.permission_category_id) {
      //   this.$message({
      //     message: "Please input Category",
      //     type: "warning"
      //   });
      //   return;
      // }

      // if (!this.permission.http_method) {
      //   this.$message({
      //     message: "Please input Method",
      //     type: "warning"
      //   });
      //   return;
      // }

      if (!this.permission.modifier) {
        this.$message({
          message: "Please input Modifier",
          type: "warning"
        });
        return;
      }

      // if (!this.permission.url) {
      //   this.$message({
      //     message: "Please input Url",
      //     type: "warning"
      //   });
      //   return;
      // }

      // if (!this.permission.resource_id) {
      //   this.$message({
      //     message: "Please input Resource Id",
      //     type: "warning"
      //   });
      //   return;
      // }
      this.loading = true;
      const scope = this;
      this.postPermission(this.permission).then(
        res => {
          scope.loading = false;
          scope.getPermission();
          scope.dialogPermissionVisible = false;
        },
        err => {
          scope.loading = false;
          scope.dialogPermissionVisible = false;
        }
      );
    },
    dialogCancel() {
      this.dialogPermissionVisible = false;
      this.permission = {};
    },
    // 获取权限
    getPermission() {
      this.loading = true;
      Core.Api.request({
        method: "POST",
        url: "/nl/public/permission/filter",
        data: {
          http_method: this.formSearch.http_method,
          permission_name: this.formSearch.permission_name
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          this.tableList = res.content;
          this.total = res.totalElements;
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
    },
    postPermission(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/permission",
        data: data
      });
    },
    deletePermission(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/public/permission/" + id
      });
    },
    // 获取权限分类
    getPermissionCategory() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/permission/category",
        data: {
          page: 1,
          size: 999
        }
      });
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.permission-list {
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
    .el-form-item__label {
      color: #fff;
      width: 60px;
      text-align: left;
      margin-left: 5px;
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
      padding: 30px 62px;
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
        width: 130px;
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
// permission-list.vue?eb808bca