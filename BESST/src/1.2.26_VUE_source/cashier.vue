<template>
  <div class="cashier-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Date">
          <el-date-picker
            v-model="formSearch.date"
            type="daterange"
            placeholder="Select date range"
          ></el-date-picker>
        </el-form-item>
        <el-form-item label="Bank">
          <el-input v-model="formSearch.account_no" placeholder="Bank"></el-input>
        </el-form-item>
        <el-form-item label="Type">
          <el-select v-model="formSearch.type" placeholder="Type">
            <el-option
              v-for="item in typeList"
              :key="item.id"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Customer">
          <el-select v-model="formSearch.customer_id" filterable placeholder="Customer">
            <el-option
              v-for="item in customerList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
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
            v-ilpermission="'cashier-add'"
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
              <td>{{ item.date | dateFormat }}</td>
              <td>{{ item.account_no }}</td>
              <td>{{ typePipe(item.type) }}</td>
              <td>{{ item.account_name }}</td>
              <td>{{ item.currency | currencyType }}{{ item.amount }}</td>
              <td>{{ item.account_name }}</td>
              <td>{{ statusPipe(item.status) }}</td>
              <td>
                <a class="btn-underline" @click.stop="onView(item)">view</a>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'cashier-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'cashier-delete'"
                >delete</a>
                <el-dropdown
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item command="Used" :disabled="!( item.status === 0 )">Used</el-dropdown-item>
                  </el-dropdown-menu>
                </el-dropdown>
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
          Customer
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            class="land"
            v-model="formData.customer_id"
            @change="handleItemChange"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in customerList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Date
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-date-picker class="input" v-model="oDate" type="date" placeholder="Date"></el-date-picker>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Bank
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.account_no" placeholder="Bank" disabled></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Type
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select class="land" v-model="formData.type" filterable slot="prepend">
            <el-option
              v-for="item in typeList"
              :key="item.id"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Amount
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="formData.amount">
            <el-select v-model="formData.currency" slot="prepend">
              <el-option
                v-for="item in currencies"
                :key="item.currency"
                :label="item.label"
                :value="item.currency"
                :value-key="item.currency"
              ></el-option>
            </el-select>
          </el-input>
        </div>
      </div>
      <div class="item-row">
        <div class="content text-area">
          <el-input
            type="textarea"
            v-model="formData.note"
            placeholder="Notes"
            :rows="5"
            :maxlength="250"
          ></el-input>
          <el-upload
            class="btn btn-primary upload-btn"
            :action="upload.url"
            :show-file-list="false"
            :on-success="imageUploaded"
            :data="upload.data"
          >
            <img
              v-if="!upload.photographSrc"
              class="upload-icon"
              src="../../../assets/images/icon/icon-upload@2x.png"
            >
            <img v-else class="upload-icon" :src="upload.photographSrc | getImgUrl">
            <a
              v-if="upload.photographSrc"
              :src="upload.photographSrc | getImgUrl"
              @click="download"
            >Download</a>
          </el-upload>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete " :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 审核弹窗 -->
    <el-dialog title="Review Shipment Order" :visible.sync="dialogReviewVisible" size="tiny">
      <p>Are you sure to do it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewInvitation">yes</el-button>
        <el-button type="primary" @click="dialogReviewVisible = false">no</el-button>
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
      //搜索数据
      formSearch: { date: "", account_no: "", type: "", customer_id: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //弹窗表单数据,
      formData: {
        customer_id: "",
        customer_name: "",
        currency: 1,
        account_no: "",
        account_name: "",
        type: "",
        amount: 0,
        note: "",
        operator: ""
      },
      //天窗名称
      dialogTitle: "",
      //操作弹窗显示
      dialogVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //审核弹窗
      dialogReviewVisible: false,
      //当前操作ID
      currentID: "",
      //当前权限数据
      currentPermisstion: [],
      //操作日期
      oDate: 0,
      oOperator: "",
      //类型
      typeList: [
        {
          label: "Receipt",
          value: 0,
          id: 0
        },
        {
          label: "Payment",
          value: 1,
          id: 1
        }
      ],
      //币种
      currencies: [
        {
          currency: 1,
          label: "€"
        },
        {
          currency: 2,
          label: "$"
        },
        {
          currency: 3,
          label: "DM."
        },
        {
          currency: 4,
          label: "￥"
        },
        {
          currency: 5,
          label: "￡"
        }
      ],
      //上传
      upload: {
        url: Core.Const.NET.IMG_UPLOAD_END_POINT,
        data: {
          token: Core.Data.getToken()
        },
        photographSrc: ""
      },
      customerList: []
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Date",
      "Bank",
      "Type",
      "Customer",
      "Amount",
      "Operator",
      "Status",
      "Operation"
    ];
    this.loading = true;
    this.getAllCustomer()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.customerList = res;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.dialogVisible = true;
        this.loading = false;
      });

    this.filterAccountTransaction();
  },
  methods: {
    //根据status返回相应的状态
    statusPipe(num) {
      if (num === "" || num === undefined) return "";
      switch (num) {
        case 0:
          return "Un Used";
          break;
        case 1:
          return "Used";
          break;
        default:
          return "";
          break;
      }
    },
    //改变状态
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    //确认弹框
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      scope
        .putCashierStatus(this.currentID, status)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Review Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.filterAccountTransaction();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    typePipe(data) {
      const typeData = this.typeList.find(res => {
        return res.value === data;
      });
      if (typeData) {
        return typeData.label;
      } else {
        return "-";
      }
    },
    //照片上传
    imageUploaded(res) {
      this.upload.photographSrc = res.data.file.name;
    },

    download(event) {
      window.open(event.currentTarget.attributes.src.value);
    },

    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.filterAccountTransaction();
    },
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = {
        date: "",
        account_no: "",
        type: "",
        customer_id: ""
      };
      this.filterAccountTransaction();
    },
    //查看
    onView(item) {
      this.$router.push({ name: "cashierDetails", query: { id: item.id } });
    },
    // 操作
    onEdit(event) {
      this.oDate = Core.Util.changeDateForm(event.date * 1000);
      this.formData = JSON.parse(JSON.stringify(event));
      this.upload.photographSrc = event.file_id;
      this.dialogTitle = "Update ";
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
      //在下方添加分页查询逻辑
      this.filterAccountTransaction();
    },

    // 添加
    dialogAdd() {
      this.dialogTitle = "Add ";
      this.dialogVisible = true;
      this.oDate = new Date();
      this.upload.photographSrc = "";
      this.oOperator = Core.Data.getUser() ? Core.Data.getUser().name : "";
      this.formData = {
        note: "",
        date: "",
        account_no: "",
        type: "",
        customer_id: "",
        amount: 0,
        currency: 1,
        operator: Core.Data.getUser() ? Core.Data.getUser().id : "",
        file_id: ""
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteAccountTransaction(scope.currentID)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.filterAccountTransaction();
          scope.dialogDeleteVisible = false;
          scope.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogVisible = true;
          this.loading = false;
        });
    },
    dialogConfirm() {
      this.formData.date = Core.Util.parseDate(this.oDate) || 0;
      this.formData.file_id = this.upload.photographSrc;
      if (!this.checkParams()) {
        return;
      }
      this.loading = true;
      this.saveAccountTransaction(this.formData)
        .then(res => {
          if (res.code === -2 || res.code === 5) {
            throw res.message;
          }
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          this.filterAccountTransaction();
          this.dialogVisible = false;
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogVisible = true;
          this.loading = false;
        });
    },
    dialogCancel() {
      this.dialogVisible = false;
      this.formData = {};
    },
    // 弹窗必填
    checkParams() {
      if (this.formData.date === "") {
        this.$message({
          message: "Please add Date",
          type: "warning"
        });
        return false;
      }

      if (this.formData.account_no === "") {
        this.$message({
          message: "Please add Bank Account",
          type: "warning"
        });
        return false;
      }

      if (this.formData.type === "") {
        this.$message({
          message: "Please add Type",
          type: "warning"
        });
        return false;
      }

      if (this.formData.amount === "" && this.formData.amount <= 0) {
        this.$message({
          message: "Please add Amount",
          type: "warning"
        });
        return false;
      }

      if (this.formData.customer_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    //顾客信息联动
    handleItemChange(id) {
      const data = this.customerList.find(res => {
        return res.id === id;
      });
      this.customerList.forEach(res => {
        if (res.id === id) {
          this.formData.account_name = res.name;
        }
      });
      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.account_no = data.account_no;
      // this.formData.account_name = data.account_name;
    },
    initCustomeInfo() {
      this.formData.account_no = "";
    },
    //获取顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    //查询财务帐户交易
    filterAccountTransaction() {
      this.loading = true;
      let _date = "";
      if (this.formSearch.date) {
        _date = {
          start: Core.Util.parseDate(this.formSearch.date[0]) || "",
          end: Core.Util.parseDate(this.formSearch.date[1]) || ""
        };
      }
      Core.Api.request({
        method: "post",
        url: "/nl/finance/account/transaction/filter",
        data: {
          type: this.formSearch.type,
          account_no: this.formSearch.account_no,
          date: _date,
          customer_id: this.formSearch.customer_id
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.tableList = res.content;
          this.total = res.totalElements;
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
    //保存财务帐户交易
    saveAccountTransaction(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/finance/account/transaction",
        data: data
      });
    },
    //删除财务帐户交易
    deleteAccountTransaction(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/finance/account/transaction/" + id
      });
    },

    //更改当前status状态
    putCashierStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/finance/account/transaction/" + id + "/status",
        params: {
          status: status
        }
      });
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.cashier-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  .upload-icon {
    width: 102px;
    height: 102px;
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
      width: 70px;
      text-align: left;
      margin-left: 5px;
    }
    .el-form-item__content {
      width: 206px !important;
    }

    .el-date-editor {
      width: 206px;
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
  .el-date-editor {
    width: 250px;
  }
  .dialog-sm {
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

  .dialog-md {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px;
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
        width: 102px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
      }
    }

    .item:nth-child(even) {
      padding-left: 31px;
    }

    .item-row {
      width: 100%;
      .text-area {
        display: flex;
        align-items: center;
        justify-content: space-around;
        width: 100%;
        margin: 10px 0;
        .el-textarea {
          margin-right: 10px;
        }
        .el-upload--text {
          display: flex;
          flex-wrap: wrap;
          justify-content: center;
        }
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// cashier.vue?1754f7b2