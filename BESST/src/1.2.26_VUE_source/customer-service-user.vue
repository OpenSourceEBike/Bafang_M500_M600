<template>
  <div class="customer-service-user">
    <div class="search-header">
      <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Employee No.">
          <el-input v-model="formSearch.employee_no" placeholder="Order No."></el-input>
        </el-form-item>
        <!-- <el-form-item label="Order No.">
          <el-input v-model="formSearch.order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <el-form-item label="Root Category">
          <el-select v-model="formSearch.root_category_id" placeholder="Root Category">
            <el-option label="test" value="test"></el-option>
          </el-select>
        </el-form-item>-->
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item add-v2"   v-ilpermission="'customer-service-user-add'" @click="dialogAdd" title="add"></div>
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
              <td>{{ item.employee_no }}</td>
              <td>{{ item.fullName }}</td>
              <td>{{ item.nickName }}</td>
              <td>{{ item.star }}</td>
              <td>{{ conversionGender(item.gender) }}</td>
              <td>{{ item.birthday | dateFormat }}</td>
              <td>
                <a class="btn-underline"  v-ilpermission="'customer-service-user-add'" @click.stop="onEdit(item)">edit</a>
                <a class="btn-underline"  v-ilpermission="'customer-service-user-delete'" @click.stop="onDelete(item)">delete</a>
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
      :visible.sync="dialogVisible"
      custom-class="edit-dialog"
      class="dialog-md"
    >
      <div class="item">
        <label class="title">
          Employee No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.employee_no" placeholder="Employee No."></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Full Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.fullName" placeholder="Full Name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Nick Name</label>
        <div class="content">
          <el-input v-model="formData.nickName" placeholder="Nick Name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Star
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input-number
            class="input-special"
            v-model="formData.star"
            controls-position="right"
            :min="1"
            :max="5"
          ></el-input-number>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Gender
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            class="input-special"
            v-model="formData.gender"
            filterable
            slot="prepend"
            placeholder="Gender"
          >
            <el-option
              v-for="item in genderData"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">Birthday</label>
        <div class="content">
          <el-date-picker class="input-special" v-model="date" type="date" placeholder="Date"></el-date-picker>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="DeleteCustomer Service User" :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>;
    
    <script>
import Core from "core/core";
import CustomerUserService from "src/services/customerUserService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { employee_no: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //弹窗表单数据,
      formData: {
        employee_no: "",
        fullName: "",
        nickName: "",
        star: "",
        gender: "",
        birthday: ""
      },
      //弹窗名称
      dialogTitle: "",
      //操作弹窗显示
      dialogVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前操作ID
      currentID: "",
      genderData: [
        {
          id: 1,
          name: "male"
        },
        {
          id: 2,
          name: "female"
        }
      ],
      date: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Employee No.",
      "Full Name",
      "Nick Name",
      "Star",
      "Gender",
      "Birthday",
      "Operation"
    ];
    this.filterUser();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.filterUser();
    },
    onClean() {
      this.formSearch = {};
      this.filterUser();
    },
    // 操作
    onEdit(event) {
      this.formData = JSON.parse(JSON.stringify(event));
      this.date = this.formData.birthday * 1000;
      this.dialogTitle = "Update Customer Service User";
      this.dialogVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.filterUser();
    },
    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Customer Service User";
      this.dialogVisible = true;
      this.formData = {
        employee_no: "",
        fullName: "",
        nickName: "",
        star: "",
        gender: "",
        birthday: ""
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      CustomerUserService
        .deleteUser(this.currentID)
        .then(() => {
          this.loading = false;
          this.dialogDeleteVisible = false;
          scope.filterUser();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogDeleteVisible = false;
          this.loading = false;
        });
    },
    dialogConfirm() {
      const scope = this;
      scope.loading = true;
      this.formData.birthday = Core.Util.parseDate(this.date) || "";
      CustomerUserService
        .postUser(this.formData)
        .then(() => {
          this.$message({
            message: "Successfully saved",
            type: "success"
          });
          this.dialogVisible = false;
          this.loading = false;
          this.filterUser();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogVisible = false;
          this.loading = false;
        });
    },
    dialogCancel() {
      this.dialogVisible = false;
      this.formData = {
        employee_no: "",
        fullName: "",
        nickName: "",
        star: "",
        gender: "",
        birthday: ""
      };
      this.date = "";
    },
    // 弹窗必填
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    conversionGender(id) {
      if (this.genderData.length > 0) {
        const data = this.genderData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "/";
        }
      }
    },
    filterUser() {
      const scope = this;
      scope.loading = true;
      CustomerUserService
        .filterUser(this.currentPage, this.formSearch)
        .then(res => {
          scope.tableList = res.data.content;
          scope.total = res.data.totalElements;
          scope.loading = false;
        });
    }
  },
  watch: {}
};
</script>
    
    
<style lang="scss" rel="stylesheet/scss">
.customer-service-user {
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
  .loading {
    width: 100px;
    height: 100px;
    position: absolute;
    top: 50%;
    left: 50%;
    margin-top: -50px;
    margin-left: -50px;
  }

  .table-wrap {
    .long-td {
      max-width: 270px;
      text-overflow: ellipsis;
      overflow: hidden;
      text-align: left;
    }
  }

  .dialog-md {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 48px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .input-special {
        width: 250px;
      }
      .title {
        display: inline-block;
        width: 98px;
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
// customer-service-user.vue?060758ab