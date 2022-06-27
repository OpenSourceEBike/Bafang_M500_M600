<template>
  <div class="department-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formInline" class="list-form-inline">
        <el-form-item label="Parent Type">
          <el-select v-model="formInline.parentType" placeholder="Parent Type" class="land">
            <el-option
              v-for="(item,index) in parentTypeList"
              :key="index"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
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
          <div
            class="icon-item add-v2"
            @click="dialogAdd"
            v-ilpermission="'department-add'"
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
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.id }}</td>
              <td>{{ item.name }}</td>
              <td>{{ item.parent_id }}</td>
              <td>{{ parentType(item) }}</td>
              <td>{{ item.phone }}</td>
              <td>{{ item.tel }}</td>
              <td>{{ item.fax }}</td>
              <td>{{ item.remark }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'department-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'department-delete'"
                >delete</a>
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
      :visible.sync="dialogDepartmentVisible"
      custom-class="edit-dialog"
      class="dialog-department"
    >
      <div class="item">
        <label class="title">
          Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="department.name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Parent Type
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select v-model="department.type" placeholder="Parent Type" class="land">
            <el-option
              v-for="(item,index) in parentTypeList"
              :key="index"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Parent ID
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="department.parent_id"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Phone
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="department.phone"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Tel
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="department.tel"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Fax
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="department.fax"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Remark</label>
        <div class="content">
          <el-input v-model="department.remark"></el-input>
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
      formInline: {
        parentType: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,

      //弹窗
      department: {
        fax: "",
        name: "",
        parent_id: "",
        phone: "",
        remark: "",
        tel: "",
        type: 0
      },
      dialogTitle: "",
      dialogDepartmentVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前删除ID
      currentID: "",
      parentTypeList: [
        {
          id: 0,
          value: 0,
          label: "Sales"
        },
        {
          id: 1,
          value: 1,
          label: "Finance"
        },
        {
          id: 2,
          value: 2,
          label: "Warehouse"
        }
      ]
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.getDepartment();
    this.headData = [
      "ID",
      "Name",
      "Parent ID",
      "Type",
      "Phone",
      "Tel",
      "Fax",
      "Remark",
      "Operation"
    ];
  },
  methods: {
    // 设置表格仓库分类
    parentType: function(item) {
      if (this.parentTypeList.length > 0) {
        const data = this.parentTypeList.filter(res => {
          return res.id === item.type;
        });
        if (data.length > 0) {
          return data[0].label;
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
      this.getDepartment();
    },
    // 清楚搜索
    onClean() {
      this.formInline.parentType = "";
      this.getDepartment();
    },
    // 操作
    onEdit(event) {
      this.department = JSON.parse(JSON.stringify(event));
      this.dialogTitle = "Update Department";
      this.dialogDepartmentVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getDepartment();
    },

    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Department";
      this.dialogDepartmentVisible = true;
      this.department = {
        parent_id: "",
        type: 0,
        name: "",
        phone: "",
        remark: "",
        tel: "",
        fax: ""
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteDepartment(scope.currentID).then(
        res => {
          scope.loading = false;
          this.dialogDeleteVisible = false;
          scope.getDepartment();
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    dialogConfirm() {
      if (!this.required()) {
        return;
      }
      this.loading = true;
      this.postDepartment(this.department).then(
        res => {
          this.loading = false;
          this.getDepartment();
          this.dialogDepartmentVisible = false;
        },
        err => {
          this.loading = false;
          this.dialogDepartmentVisible = false;
        }
      );
    },
    dialogCancel() {
      this.dialogDepartmentVisible = false;
      this.department = {};
    },
    // 获取权限分类
    getDepartment() {
      this.loading = true;
      const scope = this;
      Core.Api.request({
        method: "POST",
        url: "/nl/public/department/filter",
        data: {
          type: this.formInline.parentType
        },
        parmas: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          scope.loading = false;
          this.tableList = res.content;
          this.total = res.totalElements;
        },
        err => {
          scope.loading = false;
        }
      );
    },
    postDepartment(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/public/department",
        data: data
      });
    },
    deleteDepartment(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/public/department/" + id
      });
    },
    // 弹窗必填
    required() {
      if (this.department.name === "") {
        this.$message({
          message: "Please input Name",
          type: "warning"
        });
        return false;
      }

      if (this.department.parent_id === "") {
        this.$message({
          message: "Please input Parent ID",
          type: "warning"
        });
        return false;
      }

      if (this.department.phone === "") {
        this.$message({
          message: "Please input Phone",
          type: "warning"
        });
        return false;
      }

      if (this.department.tel === "") {
        this.$message({
          message: "Please input Tel",
          type: "warning"
        });
        return false;
      }

      if (this.department.fax === "") {
        this.$message({
          message: "Please input Fax",
          type: "warning"
        });
        return false;
      }
      return true;
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.department-list {
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

  .dialog-department {
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
        width: 85px;
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
// department-list.vue?7658f7f8