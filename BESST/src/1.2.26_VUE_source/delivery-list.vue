<template>
  <div class="delivery-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Delivery Date">
          <el-date-picker
            v-model="formSearch.delivery_date"
            type="daterange"
            placeholder="Select date range"
          ></el-date-picker>
        </el-form-item>
        <el-form-item label="Delivery Order No.">
          <el-input v-model="formSearch.delivery_order_no" placeholder="Invoice No."></el-input>
        </el-form-item>
        <el-form-item label="Order No.">
          <el-input v-model="formSearch.order_no" placeholder="PI No."></el-input>
        </el-form-item>
        <el-form-item label="Customer No.">
          <el-select v-model="formSearch.customer_no" filterable placeholder="Customer">
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
            v-ilpermission="'delivery-list-add'"
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
              <td>{{ item.delivery_date | dateFormat }}</td>
              <td>{{ item.delivery_order_no }}</td>
              <!-- <td>{{ item.order_no }}</td> -->
              <td>{{ item.customer_no }}</td>
              <td>{{ item.reviewer }}</td>
              <td>{{ item.reviewer_date| dateFormat }}</td>
              <td>{{ statusPipe(item) }}</td>
              <!-- <td v-if="item.status == 0">Created</td> -->
              <!-- <td v-if="item.status == 1">Approved</td> -->
              <!-- <td v-if="item.status == 2">Invoice Created</td> -->
              <!-- <td v-if="item.status == 3">Finished</td> -->
              <!-- <td v-if="item.status == 4">Canceled</td> -->
              <!-- <td v-if="item.status == 5">OutOfStock</td> -->

              <td>
                <button class="btn-underline" @click.stop="onSchedule(item)">schedule</button>
                <a class="btn-underline" @click.stop="onView(item)">view</a>
                <a class="btn-underline" @click.stop="onSendEmail(item)">email</a>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'delivery-list-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'delivery-list-delete'"
                >delete</a>
                <el-dropdown
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                  v-ilpermission="'delivery-list-review'"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item
                      command="Approved"
                      :disabled="item.status === 1 || item.status === 3 || item.status === 4 || item.status === 5 "
                    >Approved</el-dropdown-item>
                    <el-dropdown-item
                      command="Finished"
                      :disabled="item.status === 3 || item.status === 4 || item.status === 5 ||  item.status === 6"
                    >Finished</el-dropdown-item>
                    <el-dropdown-item
                      command="Canceled"
                      :disabled="item.status === 3 || item.status === 4 || item.status === 5 || item.status === 6"
                    >Canceled</el-dropdown-item>
                    <el-dropdown-item
                      command="OutOfStock"
                      :disabled="item.status === 3 || item.status === 4 || item.status === 5 || item.status === 6"
                    >OutOfStock</el-dropdown-item>
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

    <!-- Action：approved 库存不足提示outOfSotck -->
    <el-dialog title="Notice" :visible.sync="dialogActionFinish" size="tiny">
      <p
        v-for="(item, index) in outOfStockArr"
        :key="index"
        class="el_dialog_body_p"
      >{{item.item_id}}: out of stock!</p>
      <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="dialogActionFinish = false">yes</el-button>
      </span>
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
        delivery_date: "",
        delivery_order_no: "",
        order_no: "",
        customer_no: ""
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
      currentDeliveryNo: "",
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
      ],
      //状态
      statusAll: [
        {
          status: 0,
          label: "Created"
        },
        {
          status: 1,
          label: "Approved"
        },
        {
          status: 2,
          label: "Invoice Created"
        },
        {
          status: 3,
          label: "Finished"
        },
        {
          status: 4,
          label: "Canceled"
        },
        {
          status: 5,
          label: "OutOfStock"
        }
      ],
      //outOfStock item arr
      outOfStockArr: [],
      //Action：finish 库存提示
      dialogActionFinish: false,
      emailDialogVisible:false
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Delivery Date",
      "Delivery Order No.",
      // "Order No.",
      "Customer No.",
      "Remark",
      "Remark Date",
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

    scope.filterDelivery();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    //状态转换
    statusPipe(data) {
      const _status = this.statusAll.find(res => {
        return res.status === data.status;
      });
      return _status && _status.label;
    },
    // 搜索
    onSearch() {
      // 在下方添加查询逻辑
      const scope = this;
      this.currentPage = 1;
      this.loading = true;
      this.filterDelivery();
    },
    //clear
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = {
        delivery_date: "",
        delivery_order_no: "",
        order_no: "",
        customer_no: ""
      };
      this.loading = true;
      this.filterDelivery();
    },
    // 操作
    onView(event) {
      this.$router.push({
        name: "deliveryListDetails",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event){
      this.currentID = event.id;
      this.currentDeliveryNo = event.delivery_order_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email){
        Core.Api.Common.sendEmailStaff(this.currentDeliveryNo, email)

        this.emailDialogVisible = false;
    },
    sendCancel(event){
      this.emailDialogVisible = false;
    },
    onEdit(event) {
      this.$router.push({
        name: "deliveryListHeader",
        query: { orderId: event.id }
      });
    },
    onSchedule(event) {
      this.$router.push({
        name: "orderSchedule",
        query: { orderId: event.id }
      });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 状态改变
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      //在下方添加分页查询逻辑
      this.filterDelivery();
    },

    // 添加
    dialogAdd() {
      this.$router.push({ name: "deliveryListHeader" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteDelivery(scope.currentID).then(
        res => {
          if (res.code === 201) {
            this.$message({
              message:
                "You can't delete this delivery order because it is not a Creation status.",
              type: "warning"
            });
          } else {
            scope.filterDelivery();
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      if (status === "Approved") {
        scope
          .verifyInventoryInformation(this.currentID)
          .then(res => {
            if (res.code === -2) {
              throw res.message;
            }
            //返回值为空数组表示库存没问题
            if (res.length == 0) {
              return scope.putDeliveryStatus(this.currentID, status);
            } else {
              let inc = res.some(item => {
                if (item.result == "out of stock") {
                  return true;
                }
              });
              if (inc) {
                scope.dialogActionFinish = true;
                scope.outOfStockArr = res.filter(item => {
                  return item.result == "out of stock" && item;
                });
              } else {
                scope.putDeliveryStatus(this.currentID, status);
              }
            }
            scope.loading = false;
          })
          .catch(err => {
            scope.$message({
              message: err,
              type: "error"
            });
            scope.loading = false;
          });
      } else {
        scope.putDeliveryStatus(this.currentID, status);
      }
    },
    ///审核出库单状态
    putDeliveryStatus(id, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/sales/delivery/" + id + "/status",
        params: {
          status: status
        }
      })
        .then(res => {
          this.loading = false;
          this.filterDelivery();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 核实库存信息
    verifyInventoryInformation(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/sales/delivery/detail/" + id + "/allocate"
      });
    },
    //获取顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    // 获取发货单列表
    filterDelivery() {
      this.loading = true;
      let _date = "";
      if (this.formSearch.delivery_date) {
        _date = {
          start: Core.Util.parseDate(this.formSearch.delivery_date[0]) || "",
          end: Core.Util.parseDate(this.formSearch.delivery_date[1]) || ""
        };
      }
      Core.Api.request({
        method: "POST",
        url: "/nl/sales/delivery/filter",
        data: {
          delivery_date: _date,
          delivery_order_no: this.formSearch.delivery_order_no,
          order_no: this.formSearch.order_no,
          customer_no: this.formSearch.customer_no
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          if(res.code === -2) {
            throw res.messages;
          }
          this.tableList = res.content;
          this.total = res.totalElements;
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
    },
    // 删除发货单信息
    deleteDelivery(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/sales/delivery/" + id
      });
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.delivery-list {
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
    width: auto;
    text-align: left;
    margin-left: 5px;
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
// delivery-list.vue?96187b9e