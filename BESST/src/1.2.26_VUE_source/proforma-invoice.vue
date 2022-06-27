<template>
  <div class="PI-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="PI No.">
          <el-input v-model="formSearch.pi_no" placeholder="PI No."></el-input>
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
        <el-form-item label="Order No.">
          <el-input v-model="formSearch.sales_order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <el-form-item label="Date">
          <el-date-picker
            v-model="formSearch.date"
            type="daterange"
            placeholder="Select date range"
          ></el-date-picker>
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
              v-ilpermission="'proforma-invoice-add'"
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
              <td>{{ item.date | dateFormat }}</td>
              <td>{{ item.pi_no }}</td>
              <td>{{ item.country_code + " " + item.telephone }}</td>
              <td>{{ item.customer_name }}</td>
              <td>{{ item.customer_order_no }}</td>
              <td>{{ item.createUser }}</td>
              <!-- <td>{{ conversionStatus(item.status) }}</td> -->
              <td v-if="item.status == 0">Created</td>
              <td v-if="item.status == 1">Delivered</td>
              <td v-if="item.status == 2">Used</td>
              <td>
                <button class="btn-underline" @click.stop="onView(item)">view</button>
                <button class="btn-underline" @click.stop="onSendEmail(item)">email</button>
                <button
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'proforma-invoice-add'"
                >edit</button>
                <button
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'proforma-invoice-delete'"
                >delete</button>
                <el-dropdown
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                  v-ilpermission="'proforma-invoice-review'"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <!-- <el-dropdown-item command="Confirm" :disabled="item.status === 1">Confirm</el-dropdown-item> -->
                    <!-- <el-dropdown-item
                      command="Created"
                      :disabled="item.status === 2 || item.status === 1 || item.status === 0"
                    >Created</el-dropdown-item>-->
                    <el-dropdown-item
                      command="Delivered"
                      :disabled="item.status === 1 || item.status === 2"
                    >Delivered</el-dropdown-item>
                    <el-dropdown-item command="Used" :disabled="item.status === 2">Used</el-dropdown-item>
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
        date: "",
        pi_no: "",
        sales_order_no: "",
        customer_id: ""
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
      currentPINo:"",
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
      statusData: ["Created", "Confirm"],
      status: "",
      customerList: [],
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
      "Date",
      "PI No.",
      "Telephone",
      "Customer",
      "Customer Art Nr.",
      "Creator",
      "Status",
      "Operation"
    ];
    this.getPIList();
    this.getAllCustomer();
    // const scope = this;
    // scope.lodaing = true;
    // scope
    //   .getSupplier()
    //   .then(res => {
    //     scope.lodaing = false;
    //     scope.supplierData = res.content;
    //   })
    //   .catch(err => {
    //     scope.$message({
    //       message: err.message,
    //       type: "error"
    //     });
    //     scope.loading = false;
    //   });
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getPIList();
    },
    //clear
    onClean() {
      this.formSearch = {
        date: "",
        pi_no: "",
        sales_order_no: "",
        customer_id: ""
      };
      this.getPIList();
    },
    // 操作
    onView(event) {
      this.$router.push({
        name: "PIDetails",
        query: { orderId: event.id }
      });
    },
    onEdit(event) {
      this.$router.push({
        name: "PIHeader",
        query: { orderId: event.id }
      });
    },
    onSendEmail(event){
      this.currentID = event.id;
      this.currentPINo = event.pi_no;
      this.emailDialogVisible = true;
    },
    sendEmail(email){
        Core.Api.Common.sendEmailStaff(this.currentPINo, email)
        // .then(
        // res => {
        //      debugger
        // },err=>{
        //      debugger
        //     console.log(err);
        // });
        this.emailDialogVisible = false;
    },
    sendCancel(event){
      this.emailDialogVisible = false;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    onAdd() {
      this.$router.push({ name: "PIHeader" });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getPIList();
    },
    deleteInvitation() {
      const scope = this;
      this.loading = true;
      this.deletePIList(this.currentID)
        .then(res => {
          if (res.code === 201) {
            this.$message({
              message:
                "You can't delete this PI order because it is not a Creation status.",
              type: "warning"
            });
          } else {
            scope.getPIList();
          }
          this.loading = false;
          this.dialogDeleteVisible = false;
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
    //获取顾客信息
    getAllCustomer() {
      this.loading = true;
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      })
        .then(res => {
          this.loading = false;
          this.customerList = res;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
//
    //获取采购列表
    getPIList() {
      const scope = this;
      scope.loading = true;
      let _date = "";
      if (this.formSearch.date) {
        _date = {
          start: Core.Util.parseDate(this.formSearch.date[0]) || "",
          end: Core.Util.parseDate(this.formSearch.date[1]) || ""
        };
      }
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/filter",
        data: {
          date: _date,
          pi_no: this.formSearch.pi_no,
          sales_order_no: this.formSearch.sales_order_no,
          customer_id: this.formSearch.customer_id
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
    deletePIList(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/sales/proformaInvoice/" + id
      });
    },
    putPIStatus(currentID, status) {
      return Core.Api.request({
        method: "put",
        url: "/nl/sales/proformaInvoice/" + currentID + "/status",
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
    //状态显示
    conversionStatus(data) {
      if (this.statusData[data]) {
        return this.statusData[data];
      }
      return "-";
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
      scope.putPIStatus(this.currentID, status).then(
        res => {
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.getPIList();
        },
        err => {
          scope.loading = false;
        }
      );
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.PI-list {
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
      width: 206px !important;
    }
    .el-date-editor {
      width: 206px;
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
// proforma-invoice.vue?5e3ba118