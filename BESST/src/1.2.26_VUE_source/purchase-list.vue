<template>
  <div class="purchase-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Order No.">
          <el-input v-model="formSearch.purchase_order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
        <div class="btn-group-top">
          <div class="icons">
            <div
              class="icon-item add-v2"
              @click="onAdd"
              title="add"
              v-ilpermission="'purchase-add'"
            ></div>
          </div>
        </div>
      </el-form>
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
              <td>{{ item.purchase_order_no }}</td>
              <td>{{ conversionSupplier(item.supplier_id) }}</td>
              <td>{{ conversionPurpose(item.purchase_type) }}</td>
              <td>{{ item.total_quantity }}</td>
              <td>{{ item.date | dateFormat }}</td>
              <td>{{ statusPipe(item.status) }}</td>
              <td>
                <button class="btn-underline" @click.stop="onSchedule(item)">schedule</button>
                <button class="btn-underline" @click.stop="onView(item)">view</button>
                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                <button
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'purchase-add'"
                >edit</button>
                <button
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'purchase-delete'"
                >delete</button>
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
                      command="Finished"
                      :disabled="!(item.status === 2 || item.status ===  1 || item.status === 0 )"
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

    <email-dialog :dialogVisible="emailDialogVisible" @send="sendEmail($event)" @cancel="sendCancel"></email-dialog>

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
        purchase_order_no: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //删除弹窗
      dialogDeleteVisible: false,
      //审核弹窗
      dialogReviewVisible: false,
      //当前操作ID
      currentID: "",
      emailDialogVisible: false,
      currentPurchaseNo: "",
      supplierData: [],
      purpose: [
        {
          id: 0,
          name: "Warehouse stocking"
        },
        {
          id: 1,
          name: "Sales purchase"
        },
        {
          id: 2,
          name: "After-sales procurement"
        }
      ],
      statusList: ["Created", "Approved", "Finished"],
      status: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Order No.",
      "Object",
      "Purpose",
      "Amount",
      "Create Time",
      "Status",
      "Operation"
    ];
    this.getPurchaseList();
    const scope = this;
    scope.lodaing = true;
    scope
      .getSupplier()
      .then(res => {
        scope.lodaing = false;
        scope.supplierData = res;
      })
      .catch(err => {
        scope.$message({
          message: err.message,
          type: "error"
        });
        scope.loading = false;
      });
  },
  methods: {
    // pipe
    statusPipe(status) {
      return this.statusList[status] ? this.statusList[status] : "-";
    },
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
      scope.putPurchaseStatus(this.currentID, status).then(
        res => {
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.getPurchaseList();
        },
        err => {
          scope.loading = false;
        }
      );
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getPurchaseList();
    },
    //clear
    onClean() {
      this.formSearch = {
        purchase_order_no: ""
      };
      this.getPurchaseList();
    },
    //
    onSchedule(event) {
      this.$router.push({
        name: "purchaseSchedule",
        query: { orderId: event.id }
      });
    },
    // 操作
    onView(event) {
      this.$router.push({
        name: "purchaseListDetails",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event){
      this.currentID = event.id;
      this.currentPurchaseNo = event.purchase_order_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email){
        Core.Api.Common.sendEmailStaff(this.currentPurchaseNo, email)

        this.emailDialogVisible = false;
    },
    sendCancel(event){
      this.emailDialogVisible = false;
    },
    onEdit(event) {
      this.$router.push({
        name: "purchaseOrderDetails",
        query: { orderId: event.id }
      });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    onAdd() {
      this.$router.push({ name: "purchaseOrderDetails" });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getPurchaseList();
    },
    deleteInvitation() {
      const scope = this;
      this.loading = true;
      this.deletePurchaseList(this.currentID)
        .then(() => {
          this.loading = false;
          this.dialogDeleteVisible = false;
          scope.getPurchaseList();
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
//
    //获取采购列表
    getPurchaseList() {
      const scope = this;
      scope.loading = true;
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/filter",
        data: {
          purchase_order_no: this.formSearch.purchase_order_no
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      })
        .then(res => {
          scope.loading = false;
          if (res.code === -2) {
            throw res.message;
          }
          scope.tableList = res.content;
          scope.total = res.totalElements;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    //删除采购列表
    deletePurchaseList(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/order/purchase/" + id
      });
    },
    //更新status！
    putPurchaseStatus(currentID, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/order/purchase/" + currentID + "/status",
        params: {
          status: status
        }
      });
    },
    //获取供货商
    getSupplier() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/supplier/all"
      });
    },
    // 转换供应商
    conversionSupplier(id) {
      if (this.supplierData.length > 0) {
        const data = this.supplierData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].company_name;
        } else {
          return "--";
        }
      }
    },
    conversionPurpose(id) {
      if (this.purpose.length > 0) {
        const data = this.purpose.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        }
      }
    },
    changeDate(date) {
      return Core.Util.changeDateForm(date);
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.purchase-list {
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
      width: 83px;
      text-align: left;
      margin-left: 5px;
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

  .dialog-warehouse {
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
// purchase-list.vue?7f2f5f90