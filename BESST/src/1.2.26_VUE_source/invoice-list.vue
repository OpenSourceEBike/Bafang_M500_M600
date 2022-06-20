<template>
    <div class="invoice-list">
        <div class="search-header">
            <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
            </div>-->
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-form-item label="Date">
                    <el-date-picker
                        v-model="formSearch.invoice_date"
                        type="daterange"
                        placeholder="Select date range"
                    ></el-date-picker>
                </el-form-item>
                <el-form-item label="Invoice No.">
                    <el-input v-model="formSearch.invoice_no" placeholder="Invoice No."></el-input>
                </el-form-item>
                <el-form-item label="Deliver order no" class="deliver-no">
                    <el-input v-model="formSearch.deliver_order_no" placeholder="Deliver order no"></el-input>
                </el-form-item>
                <el-form-item label="Customer">
                    <el-select v-model="formSearch.customer_id " filterable placeholder="Customer">
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
                        title="add"
                        v-ilpermission="'invoice-add'"
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
                            <td>{{ item.invoice_date | dateFormat }}</td>
                            <td>{{ item.invoice_no }}</td>
                            <td>{{ item.customer_name }}</td>
                            <td>{{ item.currency | currencyType }}{{ item.invoice_amount}}</td>
                            <td>{{ statusPipe(item.status) }}</td>
                            <td>
                                <a class="btn-underline" @click.stop="onView(item)">view</a>
                                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                                <a
                                    class="btn-underline"
                                    @click.stop="onEdit(item)"
                                    v-ilpermission="'invoice-add'"
                                >edit</a>
                                <a
                                    class="btn-underline"
                                    @click.stop="onDelete(item)"
                                    v-ilpermission="'invoice-delete'"
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
                                            command="Approved"
                                            :disabled="!(item.status === 1 || item.status === 0 )"
                                        >Approved</el-dropdown-item>
                                        <el-dropdown-item
                                            command="Balance_Unpaid"
                                            :disabled="!(item.status === 2 || item.status ===  1 || item.status === 0 )"
                                        >Balance_Unpaid</el-dropdown-item>
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
        <!-- 删除弹窗 -->
        <el-dialog title="Delete Invoice" :visible.sync="dialogDeleteVisible" size="tiny">
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
        <!-- 邮件 -->
        <email-dialog
            :dialogVisible="emailDialogVisible"
            @send="sendEmail($event)"
            @cancel="sendCancel"
        ></email-dialog>
    </div>
</template>;

<script>
import Core from "core/core";
import EmailDialog from "../../../components/email/email-dialog.vue";

export default {
  components: {
    EmailDialog
  },
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: {
        invoice_date: "",
        invoice_no: "",
        // pi_no: "",
        customer_id: "",
        deliver_order_no: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //删除弹窗
      dialogDeleteVisible: false,
      //审核弹窗
      dialogReviewVisible: false,
      //发送邮件弹窗
      emailDialogVisible: false,
      //当前操作ID
      currentID: "",
      currentInvoiceNo: "",
      customerList: [],
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
      ]
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
      "Invoice No.",
      "Customer",
      "Amount",
      "Status",
      "Operation"
    ];
    const scope = this;

    this.loading = true;
    this.getAllCustomer()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.customerList = res;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });

    scope.filterFinanceInvoice();
  },
  methods: {
    //根据status返回相应的状态
    statusPipe(num) {
      if (num === "" || num === undefined) return "";
      switch (num) {
        case 0:
          return "Created";
          break;
        case 1:
          return "Approved";
          break;
        case 2:
          return "Balance Unpaid";
          break;
        case 3:
          return "Dunned";
          break;
        case 4:
          return "Finished";
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
        .putInvoiceStatus(this.currentID, status)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Review Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.filterFinanceInvoice();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      // 在下方添加查询逻辑
      const scope = this;
      this.currentPage = 1;
      this.loading = true;
      this.filterFinanceInvoice();
    },
    //clear
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = {
        invoice_date: "",
        invoice_no: "",
        pi_no: "",
        customer_id: ""
      };
      this.loading = true;
      this.filterFinanceInvoice();
    },
    // 操作
    onView(event) {
      this.$router.push({
        name: "invoiceListDetails",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event) {
      this.currentID = event.id;
      this.currentInvoiceNo = event.invoice_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email) {
      Core.Api.Common.sendEmailStaff(this.currentInvoiceNo, email);
      this.emailDialogVisible = false;
    },
    sendCancel(event) {
      this.emailDialogVisible = false;
    },
    onEdit(event) {
      this.$router.push({
        name: "invoiceListHeader",
        query: { orderId: event.id }
      });
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
      this.filterFinanceInvoice();
    },

    // 添加
    dialogAdd() {
      this.$router.push({ name: "invoiceListHeader" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteFinanceInvoice(scope.currentID).then(
        res => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.filterFinanceInvoice();
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    //获取顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    // 获取发票列表
    filterFinanceInvoice() {
      this.loading = true;
      let _date = "";
      if (this.formSearch.invoice_date) {
        _date = {
          start: Core.Util.parseDate(this.formSearch.invoice_date[0]) || "",
          end: Core.Util.parseDate(this.formSearch.invoice_date[1]) || ""
        };
      }
      Core.Api.request({
        method: "post",
        url: "/nl/finance/invoice/filter",
        data: {
          invoice_no: this.formSearch.invoice_no,
          deliver_order_no: this.formSearch.deliver_order_no,
          customer_id: this.formSearch.customer_id,
          invoice_date: _date
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
    // 删除发票信息
    deleteFinanceInvoice(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/finance/invoice/" + id
      });
    },

    //更改当前status状态
    putInvoiceStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/finance/invoice/" + id + "/status",
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
.invoice-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  .el-form-item {
    margin-right: 2px !important;
    margin-bottom: 0;
  }

  .el-form-item__label {
    color: #fff;
    width: 83px;
    text-align: left;
    margin-left: 5px;
  }

  .deliver-no .el-form-item__label {
    width: 120px;
  }

  .el-form-item__content {
    width: 206px !important;
  }
  .el-date-editor {
    width: 206px;
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
// invoice-list.vue?0434c91a