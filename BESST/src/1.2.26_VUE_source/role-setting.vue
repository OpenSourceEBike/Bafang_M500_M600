<template>
  <div class="permission-setting">
    <div class="header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <!-- <div class="btn-group-inline">
          <div class="icons">
            <span class="icon-item back" @click="goAfterIndex" title="back"></span>
          </div>
        </div> -->
        <el-form-item>
          <el-input v-model="formSearch.email" placeholder="Email" class="input">
            <el-button slot="append" @click="onSearch">
              <i class="el-icon-search"></i>
            </el-button>
          </el-input>
        </el-form-item>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item save" @click="onSave" v-if="hasRole" title="save"></div>
        </div>
      </div>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content">
        <div class="parts-list">
          <div class="item">
            <label class="label">User ID :</label>
            <span>{{userInfo.user && userInfo.user.id || '-'}}</span>
          </div>
          <div class="item">
            <label class="label">Name :</label>
            <span>{{userInfo.user && userInfo.user.name || '-'}}</span>
          </div>
          <div class="item">
            <label class="label">Email :</label>
            <span>{{userInfo.user && userInfo.user.email || '-'}}</span>
          </div>
          <div class="item">
            <label class="label">Organization ID :</label>
            <span>{{userInfo.org && userInfo.org.id || '-'}}</span>
          </div>
          <div class="item">
            <label class="label">Organization Name :</label>
            <span>{{userInfo.org && userInfo.org.name || '-'}}</span>
          </div>
          <div class="item" v-if="!loading && roleTransferData.length>0 && hasRole">
            <label class="label">Department :</label>
            <el-select class="land" v-model="currentDepartment" filterable slot="prepend">
              <el-option
                v-for="item in departmentList"
                :key="item.id"
                :label="item.name"
                :value="item.id"
              ></el-option>
            </el-select>
          </div>
        </div>
      </div>
      <div class="no-data-content" v-if="!hasRole && !loading">noResult</div>
      <div class="transfer-box" v-if="!loading && roleTransferData.length>0 && hasRole">
        <el-transfer
          class="transfer"
          v-model="setRoleData"
          filterable
          :render-content="renderFunc"
          :titles="['All Role', 'Set Role']"
          :button-texts="['Del','Add']"
          :format="{
                        noChecked: '${total}',
                        hasChecked: '${checked}/${total}'
                    }"
          @change="handleChange"
          :data="roleTransferData"
        ></el-transfer>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";
export default {
  data() {
    return {
      formSearch: {
        email: ""
      },
      roleTransferData: [],
      setRoleData: [],
      renderFunc(h, option) {
        return (
          <span>
            {option.role_no.toUpperCase()} - {option.role_name}
          </span>
        );
      },
      loading: false,
      userInfo: {},
      hasRole: false,
      role: {
        department_id: 0,
        org_id: 0,
        remark: "",
        role_name: "",
        role_no: "",
        role_type: null,
        status: 0,
        update_time: 0,
        update_user_id: 0,
        user_id: 0
      },
      // 弹窗
      dialogTitle: "",
      //部门
      departmentList: "",
      //当前选中部门
      currentDepartment: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    const scope = this;
    this.loading = true;
    this.getAllRole()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.roleTransferData = res.map(res => {
          res.key = res.id;
          res.label = res.role_name;
          return res;
        });
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });

    this.loading = true;
    this.getAllDepartment()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.departmentList = res;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });
  },
  methods: {
    clearData() {
      this.setRoleData = [];
      this.userInfo = {};
      this.hasRole = false;
      this.loading = false;
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      if (!this.formSearch.email) {
        this.$message({
          message: "Please input email",
          type: "warning"
        });
        this.clearData();
        return;
      }

      this.loading = true;
      this.getUserByEmail(this.formSearch.email)
        .then(
          res => {
            if (res.code === 5 || res.code === 2) {
              this.$message({
                message: res.message,
                type: "warning"
              });
              return false;
            } else {
              this.userInfo = res.data;
              return this.getUserRoleDepRelByEmail(this.formSearch.email);
            }
          },
          err => {
            this.$message({
              message: "has error",
              type: "error"
            });
            return false;
          }
        )
        .then(res => {
          if (res === false) {
            this.clearData();
            return;
          }

          if (res.code === 5 || res.code === 2) {
            this.$message({
              message: res.message,
              type: "warning"
            });
            this.clearData();
          } else {
            this.loading = false;
            this.hasRole = true;
            this.currentDepartment =
              res.data.rel[0] && res.data.rel[0].department_id;
            this.setRoleData = res.data.rel.map(res => {
              return res.role_id;
            });
          }
        });
    },
    onSave() {
      if (!this.currentDepartment) {
        this.$message({
          message: "Please select department",
          type: "warning"
        });
        return;
      }

      const data = this.setRoleData.map(res => {
        return {
          role_id: res,
          user_id: this.userInfo.user.id,
          department_id: this.currentDepartment
        };
      });
      this.loading = true;
      this.saveUserRoleDepRel(data, this.userInfo.user.email).then(
        res => {
          this.loading = false;
        },
        err => {
          this.$message({
            message: "has error",
            type: "error"
          });
          this.clearData();
        }
      );
    },
    handleChange(value, direction, movedKeys) {
      console.log(value, direction, movedKeys);
    },
    // 查询用户ByEmail
    getUserByEmail(email) {
      return Core.Api.request({
        method: "GET",
        url: "/user/user-by-email",
        data: {
          email: email
        }
      });
    },
    // 查询角色部门关系ByEmail
    getUserRoleDepRelByEmail(email) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/userRoleDepartmentRel/email-to-rel",
        data: {
          email: email
        }
      });
    },
    // 查询所有Role
    getAllRole() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/role/all"
      });
    },
    //获取所有部门
    getAllDepartment() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/department/all"
      });
    },
    //保存角色部门关系
    saveUserRoleDepRel(data, email) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/userRoleDepartmentRel/bulk-to-email",
        data: data,
        params: {
          email: email
        }
      });
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.permission-setting {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  .list-form-inline {
    height: 100%;
    line-height: 50px;
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__content {
      width: 220px !important;
    }
  }

  .parts-list {
    display: flex;
    padding: 27px 40px;
    overflow: hidden;
    width: 100%;
    justify-content: left;
    align-items: center;
    flex-wrap: wrap;
    .item {
      width: 45%;
      height: 30px;
      margin: 0 20px;
      &.label {
        display: inline-block;
        width: 150px;
      }
    }
  }

  .no-data-content {
    position: relative;
    text-align: center;
    width: 80%;
    margin: 0 auto;
    padding: 15px 30%;
    color: #fff;
    text-align: left;
  }
  .transfer-box {
    text-align: center;
  }
  .transfer {
    text-align: left;
    display: inline-block;
    ::-webkit-scrollbar {
      width: 3px;
      height: 14px;
    }

    .el-transfer-panel {
      width: 350px;
    }
  }

  .dialog-per-setting {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .land {
        width: 250px;
      }
      .title {
        display: inline-block;
        width: 80px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// role-setting.vue?5e76f07e