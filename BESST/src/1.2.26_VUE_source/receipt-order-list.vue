<template>
    <div class="receipt-order">
        <div class="search-header">
            <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
            </div>-->
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-form-item label="Receipt Order No.">
                    <el-input v-model="formSearch.receipt_order_no" placeholder="Receipt Order No."></el-input>
                </el-form-item>
                <div class="btn-group-inline-icon">
                    <div class="icons">
                        <div class="icon-item search" @click="onSearch" title="search"></div>
                        <div class="icon-item clear" @click="onClean" title="clear"></div>
                    </div>
                </div>
            </el-form>
            <div class="btn-group-top" v-ilpermission="'warehouse-receipt-add'">
                <div class="icons">
                    <div class="icon-item add-v2" @click="dialogAdd" title="add"></div>
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
                            <!-- <td>{{ conversion(item.warehouse_id) }}</td> -->
                            <!-- <td>{{ item.store_id }}</td> -->
                            <!-- <td>{{ item.supplier_id }}</td> -->
                            <td>{{ item.receipt_order_no }}</td>
                            <!-- <td>{{ item.from_type_code }}</td> -->
                            <!-- <td>{{ item.receipt_type_code }}</td> -->
                            <td>{{ item.receipt_date | dateFormat }}</td>
                            <!-- <td>{{ item.total_quantity }}</td> -->
                            <!-- <td>{{ item.total_pallet_quantity }}</td> -->
                            <!-- <td>{{ item.total_carton_quantity }}</td> -->
                            <!-- <td>{{ item.total_net_weight }}</td> -->
                            <!-- <td>{{ item.total_gross_weight }}</td> -->
                            <!-- <td>{{ item.total_cube }}</td> -->
                            <!-- <td>{{ item.receipt_user }}</td> -->
                            <!-- <td>{{ item.quality_check_user }}</td> -->
                            <!-- <td>{{ item.description }}</td> -->
                            <td>{{ statusPipe(item) }}</td>
                            <td>
                                <a class="btn-underline" @click.stop="onView(item)">view</a>
                                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                                <a
                                    class="btn-underline"
                                    @click.stop="onEdit(item)"
                                    v-ilpermission="'warehouse-receipt-add'"
                                >edit</a>
                                <a
                                    class="btn-underline"
                                    @click.stop="onDelete(item)"
                                    v-ilpermission="'warehouse-receipt-delete'"
                                >delete</a>
                                <el-dropdown
                                    trigger="click"
                                    class="statusDrop"
                                    @command="handleCommand($event,item)"
                                    v-ilpermission="'receipt-review'"
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
        <el-dialog title="Delete Receipt Order" :visible.sync="dialogDeleteVisible" size="tiny">
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
      statusList: [
        "Created",
        "Approved",
        "Rejected",
        "Wait",
        "Finished",
        "Canceled",
        "Allocated"
      ],
      //搜索数据
      formSearch: { receipt_order_no: "" },
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
      currentReceiptOrderNo: "",
      warehouseList: [],
      status: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    const scope = this;

    scope.loading = true;
    this.getAllWarehouse().then(res => {
      this.warehouseList = res;
      scope.loading = false;
    });

    scope.loading = true;
    scope.getReceipt();

    this.headData = [
      // "Warehouse Id",
      // "Store Id",
      // "Supplier Id",
      "Receipt Order No.",
      // "From Type Code",
      // "Receipt Type",
      "Receipt Date",
      // "Total Quantity",
      // "Total Pallet Quantity",
      // "Total Carton Quantity",
      // "Total Net Weight",
      // "Total Gross Weight",
      // "Total Cube",
      // "Receipt User",
      // "Quality Check User",
      // "Description",
      "Status",
      "Operation"
    ];
  },
  methods: {
    // pipe
    statusPipe(item) {
      if (this.statusList[item.status]) {
        return this.statusList[item.status];
      }
      return "-";
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getReceipt();
    },
    onClean() {
      this.formSearch.receipt_order_no = "";
      this.getReceipt();
    },
    // 操作
    onView(event) {
      this.$router.push({
        name: "receiptOrderDetails",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event) {
      this.currentID = event.id;
      this.currentReceiptOrderNo = event.receipt_order_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email) {
      Core.Api.Common.sendEmailStaff(this.currentReceiptOrderNo, email);
      this.emailDialogVisible = false;
    },
    sendCancel(event) {
      this.emailDialogVisible = false;
    },
    onEdit(event) {
      this.$router.push({
        name: "receiptOrderHeader",
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
      this.getReceipt();
    },

    // 添加
    dialogAdd() {
      this.$router.push({ name: "receiptOrderHeader" });
    },
    // 删除
    deleteInvitation() {
      this.loading = true;
      this.deleteReceipt(this.currentID).then(
        res => {
          this.loading = false;
          this.dialogDeleteVisible = false;
          this.getReceipt();
        },
        err => {
          this.loading = false;
          this.dialogDeleteVisible = false;
        }
      );
    },
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    // 获取入库数据
    getReceipt() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/order/receipt/filter",
        data: {
          receipt_order_no: this.formSearch.receipt_order_no
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
    // 删除
    deleteReceipt(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/order/receipt/" + id
      });
    },
    // 转换仓库id
    conversion(id) {
      if (this.warehouseList.length > 0) {
        const data = this.warehouseList.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        }
      }
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      scope.putReceiptStatus(this.currentID, status).then(
        res => {
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.getReceipt();
        },
        err => {
          scope.loading = false;
        }
      );
    },
    //保存状态
    putReceiptStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/order/receipt/" + id + "/status",
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
.receipt-order {
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
      padding: 30px 40px 30px 64px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      margin-bottom: 10px;
      .land {
        width: 207px;
      }
      .title {
        display: inline-block;
        width: 132px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 207px;
      }
    }
    .el-dialog__footer {
      padding: 10px 20px 15px;
    }
  }
}
</style>



// WEBPACK FOOTER //
// receipt-order-list.vue?1dfbf419