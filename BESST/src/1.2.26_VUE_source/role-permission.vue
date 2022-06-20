<template>
  <div class="role-permission">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">Permission</label>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item save" @click="onSave" title="save"></div>
        </div>
      </div>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content">
        <el-row :gutter="20" class="header listItem">
          <el-col :span="8">
            <div class="grid-content">
              <label class="title">Module permissions</label>
            </div>
          </el-col>
          <el-col :span="16">
            <div class="grid-content">
              <label class="title">Functional authority</label>
            </div>
          </el-col>
        </el-row>
        <div v-for="(item, index) in roleForm" :key="index">
          <el-row :gutter="20" class="listItem">
            <el-col :span="8">
              <div class="grid-content">
                <el-form :inline="true" class="demo-form-inline">
                  <el-form-item :label="item.permissionName">
                    <el-switch
                      @change="onChange('ALL',item,$event)"
                      v-model="item.permissioned"
                      on-text="ON"
                      off-text="OFF"
                      on-color="#13ce66"
                      off-color="#ff4949"
                    ></el-switch>
                  </el-form-item>
                </el-form>
              </div>
            </el-col>
            <el-col :span="16">
              <div class="grid-content">
                <el-form
                  :inline="true"
                  class="demo-form-inline"
                  v-for="(comp, index) in item.components"
                  :key="index"
                >
                  <el-form-item :label="comp.permissionName">
                    <el-switch
                      @change="onChange('CHECK',item,$event)"
                      v-model="comp.permissioned"
                      on-text="ON"
                      off-text="OFF"
                      on-color="#13ce66"
                      off-color="#ff4949"
                    ></el-switch>
                  </el-form-item>
                </el-form>
              </div>
            </el-col>
          </el-row>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";

import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  inject: ["reload"],
  data() {
    return {
      roleId: "",
      loading: false,
      value3: false,
      roleForm: {}
    };
  },
  i18n: {
    messages: {
      en: EN.Component.Order.Save,
      zh: ZH.Component.Order.Save,
      de: DE.Component.Order.Save,
      nl: NL.Component.Order.Save
    }
  },
  computed: {},
  mounted: function() {
    const scope = this;
    this.roleId = this.$route.query.roleId;
    scope.loading = true;
    scope.getAllPermission().then(res => {
      scope.loading = false;
      scope.roleForm = res;
    });
  },

  methods: {
    onChange(type, item, data) {
      switch (type) {
        case "ALL":
          item.components.forEach(component => {
            component.permissioned = data;
          });
          break;
        case "CHECK":
          const count = item.components.filter(component => {
            return component.permissioned === true;
          });
          if (count.length > 0) {
            item.permissioned = true;
          }
          break;
        default:
          break;
      }
    },
    onSave() {
      const scope = this;
      this.loading = true;
      let data = [];
      this.roleForm
        .filter(res => {
          return res.permissioned === true;
        })
        .forEach(ele => {
          data.push({
            role_id: scope.roleId,
            permission_id: ele.permissionId
          });
          const compData = ele.components
            .filter(res => {
              return res.permissioned === true;
            })
            .map(res => {
              return {
                role_id: scope.roleId,
                permission_id: res.permissionId
              };
            });
          data = [...data, ...compData];
        });
      this.postPermissionBulk(data, {
        role_id: scope.roleId
      })
        .then(res => {
          this.loading = false;
          if (res.code === -2) {
            throw res.messages;
          }
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          this.reloadPermission();
        })
        .catch(err => {
          this.loading = false;
          this.$message({
            message: err,
            type: "error"
          });
        });
    },
    reloadPermission() {
      this.loading = true;
      Core.Api.request({
        method: "GET",
        url: "/nl/public/permission/permissioned"
      }).then(res => {
        let userPermissions = res;
        // 保存权限
        this.$store.commit("changePermissions", userPermissions);
        this.loading = false;
        this.loading = false;
        this.reload();
      });
    },
    //获取所有权限
    getAllPermission() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/permission/nodes",
        data: {
          role_id: this.roleId
        }
      });
    },
    // 修改权限
    postPermissionBulk(data, params) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/role/permission/rel/save-batch",
        data: data,
        params: params
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.role-permission {
  width: 100%;
  height: 100%;
  .content {
    border: 1px solid #2b2f3b;
    // padding: 0 15px;
    .el-form-item {
      margin-bottom: 5px;
    }
    .el-form--inline .el-form-item__label {
      width: 182px;
    }

    .el-form--inline .el-form-item__content {
      width: 90px;
    }

    .header {
      background: #353945;
    }
    .listItem {
      margin: 0 !important;
      margin-bottom: 20px;
      box-shadow: 0 -1px #2b2f3b;
    }

    .grid-content {
      text-align: center;
      margin: 15px auto;
      .title {
        display: block;
        font-size: 15px;
        border: 0;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// role-permission.vue?6948f4f1