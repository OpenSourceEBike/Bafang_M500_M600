<template>
  <div class="role-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Role Type">
          <el-select v-model="formSearch.role_type" placeholder="Role Type">
            <el-option
              v-for="item in roleTypeData"
              :key="item.id"
              :label="item.label"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Role Name">
          <el-input v-model="formSearch.role_name" placeholder="Role Name"></el-input>
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
          <div class="icon-item add-v2" @click="addRole" v-if="true" title="add"></div>
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
              <td>{{ item.role_no }}</td>
              <td>{{ roleType(item.role_type) }}</td>
              <td>{{ item.role_name }}</td>
              <td>{{ item.remark }}</td>
              <td>
                <a class="btn-underline" @click.stop="onEdit(item)">edit</a>
                <a class="btn-underline" @click.stop="onDelete(item)">delete</a>
                <a class="btn-underline" @click.stop="onSetPermission(item)">permission</a>
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
            Role No.
            <span class="required">*</span>
          </label>
          <el-input v-model="roleData.role_no" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Role Type
            <span class="required">*</span>
          </label>
          <el-select v-model="roleData.role_type" class="modal-select" placeholder="Role Type">
            <el-option
              v-for="item in roleTypeData"
              :label="item.label"
              :key="item.id"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Role Name
            <span class="required">*</span>
          </label>
          <el-input v-model="roleData.role_name" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">Remark</label>
          <el-input v-model="roleData.remark" class="input"></el-input>
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
import EN from "src/assets/lang/en";

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        role_type: "",
        role_name: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      // 角色类型
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
      currentID: ""
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
    this.getRole();

    this.headData = [
      "Role No.",
      "Role Type",
      "Role Name",
      "Remark",
      "Operation"
    ];
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getRole();
    },
    onClean() {
      this.formSearch.role_type = "";
      this.formSearch.role_name = "";
      this.getRole();
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Role";
      this.dialogAddVisible = true;
      this.roleData = JSON.parse(JSON.stringify(event));
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 设置权限
    onSetPermission(event) {
      const scope = this;
      this.$router.push({
        name: "rolePermission",
        query: { roleId: event.id }
      });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
    },
    // 获取角色
    getRole() {
      this.loading = true;
      Core.Api.request({
        method: "POST",
        url: "/nl/public/role/filter",
        data: {
          role_type: this.formSearch.role_type,
          role_name: this.formSearch.role_name
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          this.tableList = res.content;
          this.total = res.totalPages;
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
      this.postRole(this.roleData).then(
        res => {
          this.loading = false;
          this.dialogAddVisible = false;
          this.getRole();
        },
        err => {
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
        remark: "",
        role_name: "",
        role_no: "",
        role_type: 0
      };
      this.dialogTitle = "Add Role";
      this.dialogAddVisible = true;
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteRole(scope.currentID).then(
        res => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.getRole();
        },
        err => {
          this.loading = false;
        }
      );
    },
    postRole(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/role",
        data: data
      });
    },
    deleteRole(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/public/role/" + id
      });
    },
    // 弹窗必填
    required() {
      if (this.roleData.role_name === "") {
        this.$message({
          message: "Please input Role Name",
          type: "warning"
        });
        return false;
      }

      if (this.roleData.role_no === "") {
        this.$message({
          message: "Please input Role No.",
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
.role-list {
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
// role-list.vue?e47a5810