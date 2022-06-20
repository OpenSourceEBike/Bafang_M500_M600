<template>
  <div class="customer-service-record">
    <div class="search-header">
      <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
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
        <el-form-item label="Service No.">
          <el-input v-model="formSearch.service_no" placeholder="Service No."></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
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
              <td>{{ customerPipe(item.customer_id) }}</td>
              <td>{{ item.service_no }}</td>
              <td>{{ item.sales_order_no }}</td>
              <td>{{ typePipe(item.service_type) }}</td>
              <td>{{statusPipe(item.status)}}</td>
              <td>
                <a
                  class="btn-underline"
                  v-ilpermission="'customer-service-record-add'"
                  @click.stop="onEdit(item)"
                >edit</a>
                <a
                  class="btn-underline"
                  v-ilpermission="'customer-service-record-delete'"
                  @click.stop="onDelete(item)"
                >delete</a>
                <a v-if="item.status === 2" class="btn-underline" @click.stop="onAllot(item)">allot</a>
                <el-dropdown
                  v-ilpermission="'customer-service-record-review'"
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
                      command="Finished"
                      :disabled="item.status === 1 || item.status === 2 || item.status === 3"
                    >Processed</el-dropdown-item>
                    <el-dropdown-item
                      command="Technical_Support"
                      :disabled="item.status === 2 || item.status === 3"
                    >Technical Support</el-dropdown-item>
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
    <el-dialog
      title="Delete Customer Service Record"
      :visible.sync="dialogDeleteVisible"
      size="tiny"
    >
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 审核弹窗 -->
    <el-dialog
      title="Review Customer Service Record"
      :visible.sync="dialogReviewVisible"
      size="tiny"
    >
      <p>Are you sure to do it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewInvitation">yes</el-button>
        <el-button type="primary" @click="dialogReviewVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 审核弹窗 -->
    <el-dialog
      title="Allot Technical User"
      :visible.sync="dialogAllotVisible"
      custom-class="edit-dialog"
      class="dialog-md"
      size="tiny"
    >
      <div class="item">
        <label class="title">
          Technical User
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select v-model="formData.technical_id" filterable>
            <el-option
              v-for="item in technicalUserList"
              :key="item.id"
              :label="item.kf_fullName"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewAllot">yes</el-button>
        <el-button type="primary" @click="dialogAllotVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>;
    
    <script>
import Core from "core/core";
import WarehouseService from "src/services/warehouseService";
import CustomerServiceRecordService from "src/services/customerServiceRecordService";
import TechnicalUserService from "src/services/technicalUserService";
import TechnicalService from "src/services/technicalService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { customer_id: "", service_no: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //弹窗表单数据,
      formData: {},
      //弹窗名称
      dialogTitle: "",
      //操作弹窗显示
      dialogVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
      //审核弹窗
      dialogReviewVisible: false,
      //分配弹窗
      dialogAllotVisible: false,
      //当前操作ID
      currentID: "",
      customerList: [],
      technicalUserList: [],
      statusList: [
        "Unprocessed",
        "Processed",
        "Technical Support",
        "Allocated"
      ],
      typeArr: [
        {
          id: 1,
          label: "inspection"
        },
        {
          id: 2,
          label: "repair"
        },
        {
          id: 3,
          label: "replacement"
        }
      ]
    };
  },
  i18n: {
    messages: {}
  },
  async created() {
    const scope = this;
    this.loading = true;
    await WarehouseService.getAllCustomer()
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
    scope.loading = true;
    TechnicalUserService.getAllUser()
      .then(res => {
        if (res.data.code === -2) {
          throw res.data.message;
        }
        scope.technicalUserList = res.data;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.loadCustomerServiceRecordData();
  },
  computed: {},
  mounted() {
    this.headData = [
      "Customer",
      "Service No.",
      "Sales Order",
      "Service Type",
      "Status",
      "Operation"
    ];
  },
  methods: {
    // pipe
    statusPipe(status) {
      return this.statusList[status] ? this.statusList[status] : "-";
    },
    typePipe(id) {
      const data = this.typeArr.find(res => {
        return res.id === id;
      });
      return data ? data.label : "-";
    },
    customerPipe(id) {
      const data = this.customerList.find(res => {
        return res.id === id;
      });
      if (data) {
        return data.name;
      }
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.loadCustomerServiceRecordData();
    },
    onClean() {
      this.currentPage = 1;
      this.formSearch = { customer_id: "", service_no: "" };
      this.loadCustomerServiceRecordData();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "customerServiceRecordSave",
        query: { orderId: event.id }
      });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    onAllot(event) {
      const scope = this;
      //设置分配状态
      scope.currentID = event.id;
      scope.status = "Allocated";

      scope.loading = true;
      TechnicalService.filterTechnicalServiceAllotById(event.service_id)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = res.data.content[0];
          scope.dialogAllotVisible = true;
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
    // 分配技术人员
    reviewAllot() {
      const scope = this;
      scope.loading = true;
      this.formData.allocated_date = Core.Util.parseDate(new Date());
      TechnicalService.saveTechnicalServiceAllot(this.formData)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.$message({
            message: "allot successfully",
            type: "success"
          });
          return scope.reviewInvitation();
        })
        .then(res => {
          scope.loadCustomerServiceRecordData();
          scope.dialogAllotVisible = false;
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
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.loadCustomerServiceRecordData();
    },
    // 更改状态
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
      CustomerServiceRecordService.changeCSRecordStatus(this.currentID, status)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.loadCustomerServiceRecordData();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },

    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      CustomerServiceRecordService.deleteCustomerServiceRecord(this.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.loadCustomerServiceRecordData();
          scope.dialogDeleteVisible = false;
          scope.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.loading = true;
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
    // 加载客服服务记录数据
    loadCustomerServiceRecordData() {
      const scope = this;
      this.loading = true;
      CustomerServiceRecordService.filterCustomerServiceRecord(
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
.customer-service-record {
  span.required {
    color: #e26829;
    display: inline-block;
  }
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
      width: 86px;
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
        width: 150px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 185px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// customer-service-record.vue?6900dec7