<template>
  <div class="shipment-order-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Shipment Type">
          <el-input v-model="formSearch.shipment_type" placeholder="Shipment Type"></el-input>
        </el-form-item>
        <el-form-item label="Order No.">
          <el-input v-model="formSearch.order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <el-form-item label="Shipment NO.">
          <el-input v-model="formSearch.shipment_order_no" placeholder="Order No."></el-input>
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
            v-ilpermission="'shipment-add'"
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
              <td>{{ item.createUser }}</td>
              <td>{{ item.order_no }}</td>
              <td>{{ item.customerName }}</td>
              <td>{{ expectedDatePipe(item) }}</td>
              <td>{{ statusPipe(item) }}</td>
              <td>
                <button class="btn-underline" @click.stop="onView(item)">view</button>
                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                <button
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'shipment-add'"
                >edit</button>
                <button
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'shipment-delete'"
                >delete</button>
                <el-dropdown
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                  v-ilpermission="'shipment-review'"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item
                      command="Approved"
                      :disabled="item.status === 1 || item.status === 4 || item.status === 5 || item.status === 6"
                    >Approved</el-dropdown-item>
                    <el-dropdown-item
                      command="Finished"
                      :disabled="item.status === 4 || item.status === 5"
                    >Finished</el-dropdown-item>
                    <el-dropdown-item
                      command="Canceled"
                      :disabled="item.status === 4 || item.status === 5"
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
    <el-dialog title="Delete Shipment Order" :visible.sync="dialogDeleteVisible" size="tiny">
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

    <!-- Action：finish 库存不足提示 -->
    <el-dialog title="Notice" :visible.sync="dialogActionFinish" size="tiny">
      <p v-for="(item, index) in actionFinishTips" :key="index" class="el_dialog_body_p">{{item}}</p>
      <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="dialogActionFinish = false">yes</el-button>
      </span>
    </el-dialog>

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
import areaCodeInDB from "core/areaCode";

export default {
  components: {
    EmailDialog
  },
  data() {
    return {
      loading: false,
      //搜索数据,
      statusList: [
        "Created",
        "Approved",
        "Rejected",
        "Wait",
        "Finished",
        "Canceled",
        "Allocated"
      ],
      formSearch: { shipment_type: "", order_no: "", shipment_order_no: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //审核弹窗
      dialogReviewVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前操作ID
      currentID: "",
      departmentList: [],
      warehouseList: [],
      areaCodeList: [],
      status: "",
      //Action：finish 库存提示
      dialogActionFinish: false,
      emailDialogVisible: false,
      currentStockOutNo: "",
      actionFinishTips: []
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Operation User",
      // "Shipment Type",
      // "From Type",
      "Order No.",
      // "Warehouse",
      //   "Storer",
      //   "Department",
      "Customer",
      "Expected Date",
      // "Total Quantity",
      //   "Order Memo",
      // "Receiver Name",
      //   "Country Code",
      //   "Province",
      //   "City",
      //   "District",
      //   "Zip Code",
      //   "Address",
      //   "Mobile",
      //   "Telephone",
      //   "Email",
      //   "Remark",
      // "Amount",
      //   "Description",
      "Status",
      "Operation"
    ];
    const scope = this;
    scope.areaCodeList = areaCodeInDB.init();
    scope.loading = true;
    this.getAllDepartment().then(res => {
      this.departmentList = res;
      scope.loading = false;
    });

    scope.loading = true;
    this.getAllWarehouse().then(res => {
      scope.loading = false;
      this.warehouseList = res;
    });

    scope.loading = true;

    scope.getShipment();
  },
  methods: {
    // pipe
    statusPipe(item) {
      if (this.statusList[item.status]) {
        return this.statusList[item.status];
      }
      return "-";
    },
    warehousePipe(item) {
      if (this.warehouseList.length > 0) {
        const data = this.warehouseList.filter(res => {
          return res.id === item.warehouse_id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    departmentPipe(item) {
      if (this.departmentList.length > 0) {
        const data = this.departmentList.filter(res => {
          return res.id === item.department_id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    expectedDatePipe(item) {
      return Core.Util.changeDateForm(item.expected_date * 1000);
    },
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getShipment();
    },
    //clear
    onClean() {
      this.formSearch = {
        shipment_type: "",
        order_no: "",
        shipment_order_no: ""
      };
      this.getShipment();
    },

    // 操作
    onView(item) {
      this.$router.push({
        name: "shipmentOrderDetails",
        query: { id: item.id }
      });
    },
    onSendEmail(event) {
      this.currentID = event.id;
      this.currentStockOutNo = event.shipment_order_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email) {
      Core.Api.Common.sendEmailStaff(this.currentStockOutNo, email);

      this.emailDialogVisible = false;
    },
    sendCancel(event) {
      this.emailDialogVisible = false;
    },
    onEdit(event) {
      this.$router.push({
        name: "shipmentOrderHeader",
        query: { orderId: event.id }
      });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      scope
        .putShipmentStatus(this.currentID, status)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          if (res.detailInfo == null) {
            scope.$message({
              message: "Review Successfully",
              type: "success"
            });
          } else {
            this.dialogActionFinish = true;
            const obj = res.detailInfo;
            Object.keys(obj).forEach(index => {
              if (obj[index].is_lack == true) {
                let str = index + ": less than minimum stock";
                this.actionFinishTips.push(str);
              }
            });
          }
          scope.loading = false;
          scope.getShipment();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      //在下方添加分页查询逻辑
      this.getShipment();
    },

    // 添加
    dialogAdd() {
      this.$router.push({ name: "shipmentOrderHeader" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteShipment(scope.currentID)
        .then(res => {
          if (res.code === -2 || res.code === 400) {
            throw res.message;
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.getShipment();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        });
    },
    //获取出库单列表
    getShipment() {
      const scope = this;
      this.loading = true;
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/filter",
        data: {
          shipment_type: scope.formSearch.shipment_type,
          order_no: scope.formSearch.order_no,
          shipment_order_no: scope.formSearch.shipment_order_no,
          order: {
            direction: "desc",
            propertyName: "create_time"
          }
        },
        params: {
          page: scope.currentPage,
          size: 10
        }
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.tableList = res.content;
          scope.total = res.totalElements;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    //删除出库信息
    deleteShipment(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/order/shipment/" + id
      });
    },
    //获取所有部门
    getAllDepartment() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/department/all"
      });
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    //审核出库单状态
    putShipmentStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/order/shipment/" + id + "/status",
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
.shipment-order-list {
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
      width: 106px;
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

  .el_dialog_body_p {
    line-height: 24px !important;
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
      padding: 19px 64px 19px 64px;
      font-size: 0;
    }
    .el-dialog {
      top: 5% !important;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .land {
        width: 200px;
      }
      .title {
        display: inline-block;
        width: 100px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 200px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// shipment-order-list.vue?3ceb82ad