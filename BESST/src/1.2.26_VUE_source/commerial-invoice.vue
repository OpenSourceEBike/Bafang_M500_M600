<template>
    <div class="commerial-invoice">
        <div class="search-header">
            <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
            </div>-->
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-form-item label="CI No.">
                    <el-input v-model="formSearch.ci_no" placeholder="CI No."></el-input>
                </el-form-item>
                <el-form-item label="Customer">
                    <el-select v-model="formSearch.customer_id" filterable>
                        <el-option
                            v-for="item in customerList"
                            :key="item.id"
                            :label="item.name"
                            :value="item.id"
                        ></el-option>
                    </el-select>
                </el-form-item>
                <!-- <el-form-item label="Creator">
          <el-input v-model="formSearch.creator" placeholder="Creator"></el-input>
                </el-form-item>-->
                <!-- <el-form-item label="PI No.">
          <el-input v-model="formSearch.pi_no" placeholder="PI No."></el-input>
                </el-form-item>-->
                <el-form-item label="Date">
                    <el-date-picker v-model="formSearch.date" placeholder="Date"></el-date-picker>
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
                        v-ilpermission="'commerial-invoice-add'"
                        class="icon-item add-v2"
                        @click="dialogAdd"
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
                            <td>{{ item.date | dateFormat}}</td>
                            <td>{{ item.ci_no }}</td>
                            <!-- <td>{{ item.pi_no }}</td> -->
                            <td>{{ item.customer_name }}</td>
                            <td>{{ item.country }}</td>
                            <!-- <td>{{ item.creator }}</td> -->
                            <td>{{ conversionStatusPipe(item.status) }}</td>
                            <td>
                                <a class="btn-underline" @click.stop="onDetails(item)">details</a>
                                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                                <a
                                    class="btn-underline"
                                    @click.stop="onEdit(item)"
                                    v-ilpermission="'commerial-invoice-add'"
                                >edit</a>
                                <a
                                    class="btn-underline"
                                    @click.stop="onDelete(item)"
                                    v-ilpermission="'commerial-invoice-delete'"
                                >delete</a>
                                <el-dropdown
                                    trigger="click"
                                    class="statusDrop"
                                    @command="handleCommand($event,item)"
                                    v-ilpermission="'commerial-invoice-review'"
                                >
                                    <span class="el-dropdown-link">
                                        action
                                        <i class="el-icon-caret-bottom el-icon--right"></i>
                                    </span>
                                    <el-dropdown-menu slot="dropdown">
                                        <el-dropdown-item
                                            command="Approved"
                                            :disabled="item.status === 1 || item.status === 2 || item.status === 3"
                                        >Approved</el-dropdown-item>
                                        <el-dropdown-item
                                            command="Finished"
                                            :disabled="item.status === 2 || item.status === 3"
                                        >Finished</el-dropdown-item>
                                        <el-dropdown-item
                                            command="Canceled"
                                            :disabled="item.status === 3"
                                        >Canceled</el-dropdown-item>
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
        <el-dialog title="Delete Service Center" :visible.sync="dialogDeleteVisible" size="tiny">
            <p>Are you sure to delete it ?</p>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="deleteInvitation">yes</el-button>
                <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
            </div>
        </el-dialog>
        <!-- 审核弹窗 -->
        <el-dialog title="Review CI" :visible.sync="dialogReviewVisible" size="tiny">
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
import CommercialInvoiceService from "src/services/commerialInvoiceService";
import WarehouseService from "src/services/warehouseService";
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
        customer_id: "",
        ci_no: "",
        // creator: "",
        // pi_no: "",
        date: ""
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
      currentCINo: "",
      //当前权限数据
      currentPermisstion: [],
      customerList: [],
      statusData: [
        {
          id: 0,
          name: "Created"
        },
        {
          id: 1,
          name: "Approved"
        },
        {
          id: 2,
          name: "Finished"
        },
        {
          id: 3,
          name: "Canceled"
        }
      ],
      testResultData: [
        {
          id: 1,
          name: "Renewed"
        },
        {
          id: 2,
          name: "Maintain"
        }
      ],
      status: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {
    const scope = this;
    this.headData = [
      "Date",
      "CI No.",
      // "PI No.",
      "Customer",
      "Country",
      // "Creator",
      "Status",
      "Operation"
    ];

    this.loading = true;
    WarehouseService.getAllCustomer()
      .then(res => {
        if (res.data.code === -2) {
          throw res.data.message;
        }
        scope.customerList = res.data;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });
    scope.commercialInvoiceService();
  },
  computed: {},
  mounted() {},
  methods: {
    conversionStatusPipe(id) {
      const data = this.statusData.find(res => {
        return res.id === id;
      });
      return data ? data.name : "-";
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 改变状态
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      CommercialInvoiceService.putReceiptStatus(this.currentID, status).then(
        res => {
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.commercialInvoiceService();
        },
        err => {
          scope.loading = false;
        }
      );
    },
    // 搜索
    onSearch() {
      this.commercialInvoiceService();
    },
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = {
        customer_id: "",
        ci_no: "",
        creator: "",
        pi_no: "",
        date: ""
      };
      this.commercialInvoiceService();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "commerialInvoiceSave",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event) {
      this.currentID = event.id;
      this.currentCINo = event.ci_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email) {
      Core.Api.Common.sendEmailStaff(this.currentCINo, email);
      this.emailDialogVisible = false;
    },
    sendCancel(event) {
      this.emailDialogVisible = false;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    onDetails(event) {
      this.$router.push({
        name: "commerialInvoiceDetails",
        query: { orderId: event.id }
      });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.commercialInvoiceService();
    },
    // 添加
    dialogAdd() {
      this.$router.push({ name: "commerialInvoiceSave" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      CommercialInvoiceService.deleteCommercialInvoice(this.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.commercialInvoiceService();
          scope.dialogDeleteVisible = false;
          scope.$message({
            message: "Delete Successfully",
            type: "success"
          });
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.dialogDeleteVisible = false;
          scope.loading = false;
        });
    },
    // load
    commercialInvoiceService() {
      const scope = this;
      this.loading = true;
      CommercialInvoiceService.filterCommercialInvoice(
        this.currentPage,
        this.formSearch
      )
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.tableList = res.data.content;
          scope.total = res.data.totalElements;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.commerial-invoice {
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
    .el-form-item__label {
      color: #fff;
      width: 82px;
      text-align: left;
      margin-left: 5px;
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
// commerial-invoice.vue?66f8b86f