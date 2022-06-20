<template>
  <div class="service-center">
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
        <el-form-item label="Order No.">
          <el-input v-model="formSearch.order_no" placeholder="Order No."></el-input>
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
            v-ilpermission="'service-center-add'"
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
              <td>{{ item.customer_name }}</td>
              <td>{{ item.customer_no }}</td>
              <td>{{ item.customer_contact_user }}</td>
              <td>{{ item.customer_contact_phone }}</td>
              <td>{{ item.order_no }}</td>
              <td>{{ priorityPipe(item.priority) }}</td>
              <td>{{ typePipe(item.type) }}</td>
              <td>{{ statusPipe(item.status) }}</td>
              <td>
                <a
                  class="btn-underline"
                  v-ilpermission="'service-center-add'"
                  @click.stop="onEdit(item)"
                >edit</a>
                <a
                  class="btn-underline"
                  v-ilpermission="'service-center-delete'"
                  @click.stop="onDelete(item)"
                >delete</a>
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
  </div>
</template>;
    
<script>
import Core from "core/core";
import AfterSalesService from "src/services/afterSalesService";
import WarehouseService from "src/services/warehouseService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { customer_id: "", order_no: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前操作ID
      currentID: "",
      //当前权限数据
      currentPermisstion: [],
      customerList: [],
      priorityArr: [
        {
          id: 1,
          label: "normal"
        },
        {
          id: 2,
          label: "popular"
        },
        {
          id: 3,
          label: "expedited"
        }
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
      ],
      statusList: [
        "Created",
        "Wait",
        "Receive",
        "Check",
        "Repair",
        "Wait_Payment",
        "Paid",
        "Renew",
        "Deliver",
        "Refund",
        "Complete",
        "Evaluate"
      ]
    };
  },
  i18n: {
    messages: {}
  },
  created() {
    const scope = this;
    this.headData = [
      "Customer Name",
      "Customer No.",
      "Contact User",
      "Contact Phone",
      "Order No.",
      "Priority",
      "Type",
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

    scope.loadSalesServiceData();
  },
  computed: {},
  mounted() {},
  methods: {
    // pipe
    statusPipe(status) {
      return this.statusList[status] ? this.statusList[status] : "-";
    },
    priorityPipe(id) {
      const data = this.priorityArr.find(res => {
        return res.id === id;
      });
      return data ? data.label : "-";
    },
    typePipe(id) {
      const data = this.typeArr.find(res => {
        return res.id === id;
      });
      return data ? data.label : "-";
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.loadSalesServiceData();
    },
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = { customer_id: "", order_no: "" };
      this.loadSalesServiceData();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "serviceCenterSave",
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
      this.loadSalesServiceData();
    },
    // 添加
    dialogAdd() {
      this.$router.push({ name: "serviceCenterSave" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      AfterSalesService.deleteServiceCenter(this.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.loadSalesServiceData();
          scope.dialogDeleteVisible = false;
          scope.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.loading = false;
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
    loadSalesServiceData() {
      const scope = this;
      this.loading = true;
      AfterSalesService.filterServiceCenter(this.currentPage, this.formSearch)
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
.service-center {
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
// service-center.vue?4c8420ba