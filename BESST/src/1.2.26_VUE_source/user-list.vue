<template>
  <div class="user-list">
    <div class="search-header">
      <div class="btn-group-inline">User List</div>
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <!-- <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Role Type">
          <el-select v-model="formSearch.country" placeholder="Role Type">
            <el-option
              v-for="item in roleTypeData"
              :key="item.id"
              :label="item.label"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Role Name">
          <el-input v-model="formSearch.name" placeholder="Role Name"></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>-->
      <div class="btn-group-top">
        <div class="icons">
          <div
            class="icon-item add-v2"
            v-ilpermission="'user-nl-add'"
            @click="addRole"
            v-if="true"
            title="add"
          ></div>
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
            <tr v-for="item in tableList" :key="item.id" class="areaList">
              <!-- <td>{{ roleType(item.country) }}</td> -->
              <td>{{ item.uid }}</td>
              <td>{{ item.email }}</td>
              <td>{{ item.name || "-"}}</td>
              <td>
                <!-- <a class="btn-underline" @click.stop="onEdit(item)">edit</a> -->
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'user-nl-delete'"
                >delete</a>
                <!-- <a class="btn-underline" @click.stop="onSetPermission(item)">permission</a> -->
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
    <!-- 新增弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogAddVisible" class="dialog-region">
      <div class="item">
        <div class="item-box">
          <label class="label">
            Country
            <span class="required">*</span>
          </label>
          <!-- <el-input v-model="roleData.country" class="input"></el-input> -->
          <el-select class="input" v-model="roleData.country" filterable slot="prepend">
            <el-option
              v-for="(item, index) in areaCodeList"
              :key="index"
              :label="item.area_en"
              :value="item.area_code"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Email
            <span class="required">*</span>
          </label>
          <el-input v-model="roleData.email" class="input" type="email"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Name
            <span class="required">*</span>
          </label>
          <el-input v-model="roleData.name" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Password
            <span class="required">*</span>
          </label>
          <el-input v-model="roleData.password" class="input"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogDeleteVisible" size="tiny">
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
import areaCodeInDB from "core/areaCode";
import EN from "src/assets/lang/en";

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        country: "",
        name: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      // 用户类型
      roleTypeData: [
        {
          id: 0,
          value: 0,
          label: "Ordinary account"
        },
        {
          id: 1,
          value: 1,
          label: "Administrator"
        },
        {
          id: 2,
          value: 2,
          label: "Super administrator"
        }
      ],
      // 弹窗标题
      dialogTitle: "",
      roleData: {},
      // 新增弹窗
      dialogAddVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 当前操作ID
      currentID: "",
      // 国家选择
      areaCodeList: []
    };
  },
  i18n: {
    messages: {
      en: EN.Dealer.Index
    }
  },
  created() {},
  computed: {},
  mounted() {
    this.getUserList();
    this.areaCodeList = areaCodeInDB.init();
    this.headData = ["Country", "Email", "Name", "Operation"];
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getUserList();
    },
    onClean() {
      this.formSearch.country = "";
      this.formSearch.name = "";
      this.getUserList();
    },
    // 操作
    // onEdit(event) {
    //   this.dialogTitle = "Edit Role";
    //   this.dialogAddVisible = true;
    //   this.roleData = JSON.parse(JSON.stringify(event));
    // },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 设置权限
    // onSetPermission(event) {
    //   const scope = this;
    //   this.$router.push({
    //     name: "rolePermission",
    //     query: { roleId: event.id }
    //   });
    // },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getUserList();
    },
    // 获取用户
    getUserList() {
      this.loading = true;
      Core.Api.request({
        method: "GET",
        url: "/user",
        data: {
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
    // 弹框确认
    dialogConfirm() {
      const proceed = this.required();
      if (!proceed) {
        return;
      }
      this.loading = true;
      this.postUser(this.roleData).then(
        res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Created Successfully",
            type: "success"
          });
          this.loading = false;
          this.dialogAddVisible = false;
          this.getUserList();
        },
        err => {
          this.$message({
            message: err.body.error,
            type: "error"
          });
          this.loading = false;
        }
      );
    },
    // 弹框取消
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    // 新增弹窗打开事件
    addRole() {
      this.roleData = {
        country: "",
        email: "",
        name: "",
        password: ""
      };
      this.dialogTitle = "Add Role";
      this.dialogAddVisible = true;
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteUser(scope.currentID).then(
        res => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.getUserList();
        },
        err => {
          this.loading = false;
        }
      );
    },
    postUser(data) {
      return Core.Api.request({
        method: "POST",
        url: "/user/user-NL",
        params: {
          country: data.country,
          email: data.email,
          name: data.name,
          password: data.password
        }
      });
    },
    deleteUser(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/user/" + id
      });
    },
    // 弹窗必填
    required() {
      const _email = this.roleData.email.trim();
      if (this.roleData.country === "") {
        this.$message({
          message: "Please input your country !",
          type: "warning"
        });
        return false;
      }
      if (!Core.Util.isEmail(_email)) {
        this.$message({
          message: "Please check email format !",
          type: "warning"
        });
        return false;
      }
      if (this.roleData.name === "") {
        this.$message({
          message: "Please input your name !",
          type: "warning"
        });
        return false;
      }

      if (this.roleData.password === "") {
        this.$message({
          message: "Please input your login password.",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    roleType(id) {
      if (this.roleTypeData.length > 0) {
        const data = this.roleTypeData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].label;
        } else {
          return "-";
        }
      }
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.user-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  .search-header {
    .btn-group-inline {
      height: 60px;
      line-height: 60px;
      font-size: 18px;
    }
  }

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
      padding: 11px 12px;
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
    .areaList:hover {
      background: #ffffff26;
    }
  }

  .dialog-region {
    .el-dialog__body {
      padding: 30px 67px;
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
        i {
          color: #ffffff99;
        }
      }
    }
  }

  .modal-select {
    margin-left: 40px;
    width: 260px;
  }

  .dialog-permission {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
    }
  }

  .transfer {
    text-align: left;
    display: inline-block;
    ::-webkit-scrollbar {
      width: 3px;
      height: 14px;
    }

    .el-transfer-panel {
      width: 310px;
    }
  }
}
</style>



// WEBPACK FOOTER //
// user-list.vue?6035ca8a