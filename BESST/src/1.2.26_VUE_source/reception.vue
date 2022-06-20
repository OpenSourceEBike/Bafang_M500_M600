<template>
  <div class="reception-list">
    <div class="search-header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Date">
          <el-date-picker
            v-model="formSearch.date"
            type="daterange"
            placeholder="Select date range"
          ></el-date-picker>
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
        <el-form-item label="Delivery No.">
          <el-input v-model="formSearch.delivery_order_no" placeholder="Delivery No."></el-input>
        </el-form-item>
        <el-form-item label="Status">
          <el-select v-model="formSearch.status" placeholder="stastus">
            <el-option
              v-for="item in statusAll"
              :key="item.status"
              :label="item.label"
              :value="item.status"
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
            title="add"
            v-ilpermission="'finance-reception-add'"
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
              <td>{{ item.delivery_order_no }}</td>
              <td>{{ item.account_name }}</td>
              <td>{{ item.account_no }}</td>
              <td>{{ item.currency | currencyType }}{{ item.amount }}</td>
              <td>{{ item.createUser }}</td>
              <td>{{ statusPipe(item) }}</td>
              <td>
                <a class="btn-underline" @click.stop="onView(item)">view</a>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'finance-reception-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'finance-reception-delete'"
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
                    <el-dropdown-item
                      command="First_Paid"
                      :disabled="!(item.status === 1 || item.status === 0 )"
                    >First Paid</el-dropdown-item>
                    <el-dropdown-item
                      command="Balance_Unpaid"
                      :disabled="!(item.status === 2 || item.status ===  1 || item.status === 0 )"
                    >Balance Unpaid</el-dropdown-item>
                    <el-dropdown-item
                      command="Dunned"
                      :disabled="!(item.status === 3 || item.status === 2 || item.status === 1 || item.status === 0 )"
                    >Dunned</el-dropdown-item>
                    <el-dropdown-item
                      command="Finished"
                      :disabled="!(item.status === 4 || item.status === 3 || item.status === 2 || item.status === 1 || item.status === 0 )"
                    >Finished</el-dropdown-item>
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
            v-model="formData.customer_id "
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
          Delivery No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input
            class="input input-sn"
            v-model="formData.delivery_order_no"
            placeholder="Delivery No."
            @change="getDeliveryDetails"
          ></el-input>
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
          Amount
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="formData.amount" disabled>
            <el-select v-model="formData.currency" slot="prepend" disabled>
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
      <div class="item">
        <label class="title">
          Date
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-date-picker class="input" v-model="oDate" type="date" placeholder="Date"></el-date-picker>
        </div>
      </div>
      <div class="item item-row">
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
</template>;
    
<script>
import Core from "core/core";
export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: {
        date: "",
        delivery_order_no: "",
        customer_id: "",
        status: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //弹窗表单数据,
      formData: {
        customer_id: "",
        account_name: "",
        account_no: "",
        amount: 0,
        currency: 1,
        data: new Date(),
        delivery_order_no: ""
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
      //状态
      statusAll: [
        {
          status: 0,
          label: "Created"
        },
        {
          status: 1,
          label: "First Paid"
        },
        {
          status: 2,
          label: "Balance Unpaid"
        },
        {
          status: 3,
          label: "Dunned"
        },
        {
          status: 4,
          label: "Finished"
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
      oImage: {
        src: ""
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
      "Delivery No.",
      "Customer",
      "Bank",
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
    statusPipe(data) {
      const _status = this.statusAll.find(res => {
        return res.status === data.status;
      });
      if (_status) {
        return _status.label;
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
        .putReceptionStatus(this.currentID, status)
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

    // 根据pi获取总金额
    getDeliveryDetails() {
      this.loading = true;
      this.filterDelivery(this.formData.delivery_order_no)
        .then(res => {
          this.loading = false;
          if (res.content.length > 0) {
            this.formData.customer_id = res.content[0].company_id;
            this.formData.amount = res.content[0].total_amount;
            this.formData.currency = res.content[0].currency;
          }
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    //照片上传
    imageUploaded(res) {
      this.upload.photographSrc = res.data.file.name;
    },
    download(event) {
      window.open(event.currentTarget.attributes.src.value);
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
        delivery_order_no: "",
        customer_id: "",
        status: ""
      };
      this.filterAccountTransaction();
    },
    //查看
    onView(item) {
      this.$router.push({ name: "receptionDetails", query: { id: item.id } });
    },
    // 操作
    onEdit(event) {
      this.oDate = Core.Util.changeDateForm(event.date * 1000);
      this.formData = JSON.parse(JSON.stringify(event));
      // 获取图片
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
        date: new Date(),
        account_no: "",
        delivery_order_no: "",
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
          if (res.code === -2) {
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

      if (this.formData.delivery_order_no === "") {
        this.$message({
          message: "Please add Delivery No.",
          type: "warning"
        });
        return false;
      }

      if (this.formData.amount === 0) {
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
      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.account_name = data.name;
      this.formData.account_no = data.account_no;
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
        url: "/nl/finance/pi/reception/filter",
        data: {
          delivery_order_no: this.formSearch.delivery_order_no,
          date: _date,
          customer_id: this.formSearch.customer_id,
          status: this.formSearch.status
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
        url: "/nl/finance/pi/reception",
        data: data
      });
    },
    //删除财务帐户交易
    deleteAccountTransaction(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/finance/pi/reception/" + id
      });
    },
    filterDelivery(delivery_order_no) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/delivery/filter",
        data: {
          delivery_order_no: delivery_order_no
        }
      });
    },

    //更改当前status状态
    putReceptionStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/finance/pi/reception/" + id + "/status",
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
.reception-list {
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
        width: 86px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
        .input-sn:after {
          top: 5px !important;
        }
      }
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
// reception.vue?2f24a2c8