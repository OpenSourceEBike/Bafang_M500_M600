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
              <td>{{ item.service_no }}</td>
              <!-- <td>{{ item.carrier_order_id }}</td> -->
              <td>{{ conversionResult(item.test_result )}}</td>
              <td>{{ item.advice }}</td>
              <td>{{ item.remark }}</td>
              <td>{{ item.status|statusPipe(statusData) }}</td>
              <td>
                <a
                  class="btn-underline"
                  v-ilpermission="'technical-service-add'"
                  @click.stop="onEdit(item)"
                >edit</a>
                <a
                  class="btn-underline"
                  v-ilpermission="'technical-service-delete'"
                  @click.stop="onDelete(item)"
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
                      command="Checking"
                      :disabled="item.status === 1 || item.status === 2"
                    >Checking</el-dropdown-item>
                    <el-dropdown-item command="Finished" :disabled="item.status === 2">Finished</el-dropdown-item>
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
    <!-- 审核弹窗 -->
    <el-dialog title="Review Technical Service" :visible.sync="dialogReviewVisible" size="tiny">
      <p>Are you sure to do it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewInvitation">yes</el-button>
        <el-button type="primary" @click="dialogReviewVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Technical Service" :visible.sync="dialogDeleteVisible" size="tiny">
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
import TechnicalService from "src/services/technicalService";
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
      //审核弹窗
      dialogReviewVisible: false,
      //当前操作ID
      currentID: "",
      //当前权限数据
      currentPermisstion: [],
      customerList: [],
      statusData: ["Waiting", "Checking", "Finished"],
      testResultData: [
        {
          id: 1,
          name: "Renewed"
        },
        {
          id: 2,
          name: "Maintain"
        }
      ]
    };
  },
  i18n: {
    messages: {}
  },
  async created() {
    const scope = this;
    this.headData = [
      "After Sale Order No.",
      // "Parcel No.",
      // "SN",
      "Test Results",
      "Advice",
      "Remark",
      "Status",
      "Operation"
    ];

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

    scope.technicalServiceData();
  },
  computed: {},
  mounted() {},
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.technicalServiceData();
    },
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = { customer_id: "", order_no: "" };
      this.technicalServiceData();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "technicalServiceSave",
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
      this.technicalServiceData();
    },
    // 添加
    // dialogAdd() {
    //   this.$router.push({ name: "technicalServiceSave" });
    // },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      TechnicalService.deleteTechnical(this.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.technicalServiceData();
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
    technicalServiceData() {
      const scope = this;
      this.loading = true;
      TechnicalService.filterTechnical(this.currentPage, this.formSearch)
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
    },
    conversionResult(id) {
      if (this.testResultData.length > 0) {
        const data = this.testResultData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    // 审核
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
      TechnicalService.putTechnicalStatus(this.currentID, status)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.technicalServiceData();
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
// technical-service.vue?7e222a01