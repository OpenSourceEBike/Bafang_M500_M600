<template>
  <div class="stock-out-index">
    <div class="search-header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item>
          <el-input v-model.trim="formSearch.order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <el-form-item>
          <el-select v-model="formSearch.customer_id" filterable placeholder="Customer">
            <el-option
              v-for="item in customerList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-select v-model="formSearch.purpose" placeholder="Purpose">
            <el-option v-for="(item,index) in PurposeList" :key="index" :label="item" :value="item"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-date-picker v-model="formSearch.startDate" type="date" placeholder="Start Date"></el-date-picker>
        </el-form-item>
        <el-form-item>
          <el-date-picker v-model="formSearch.endDate" type="date" placeholder="End Date"></el-date-picker>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <span class="icon-item search" @click="goSearch" title="search"></span>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <button type="button" class="el-button el-button--preview" @click="add()">
            Add
            <i class="el-icon-plus btn-icon"></i>
          </button>
          <button type="button" @click="onExport" class="el-button el-button--preview">Export</button>
        </div>
      </div>
    </div>
    <div class="table-wrap" v-loading.body="loading" element-loading-text="loading...">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length <= 0 && !loading">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody>
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.order_no }}</td>
              <td>{{ item.customer_name }}</td>
              <td>{{ item.purpose }}</td>
              <td>{{ item.date | timeFormat("Y-m-d") }}</td>
              <td>{{ item.create_time | timeFormat("Y-m-d") }}</td>
              <td>{{ item.create_user_id }}</td>
              <td
                :class="[item.status ==0 ? 'status_create' : 'status_approved' ]"
              >{{ item.status ==0?'Create':'Approved' }}</td>
              <td>
                <button class="btn-underline" @click.stop="onPrint(item)">Print</button>
                <button class="btn-underline" @click.stop="onEdit(item)">Edit</button>
                <button
                  class="btn-underline"
                  :disabled="item.status === 1"
                  @click.stop="onDelete(item)"
                >Delete</button>
                <el-dropdown trigger="click" @command="onReview($event,item)">
                  <span class="el-dropdown-link">
                    Review
                    <i class="el-icon-arrow-down el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item
                      :disabled="item.status === 1"
                      icon="el-icon-plus"
                      command="approved"
                    >Approved</el-dropdown-item>
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
  </div>
</template>

<script>
import CommonService from "src/services/nl/commonService";
import StockOutService from "src/services/nl/stockOutService.js";
import StockOutHeaderService from "src/services/nl/stockOutHeaderService.js";
import Core from "core/core";
import saveAs from "file-saver";
import moment from "moment";
export default {
  data() {
    return {
      loading: false,
      headData: [
        "Order No.",
        "Customer",
        "Purpose",
        "Date",
        "Create Date",
        "User",
        "Status",
        "Operation"
      ],
      tableList: [],
      formSearch: {},
      currentPage: 1,
      total: 0,
      formData: {},
      // 用途列表
      PurposeList: Core.StatusData.PURPOSE,
      //
      customerList: []
    };
  },
  async created() {
    this.loading = true;
    CommonService.getAllCustomer().then(res => {
      this.customerList = res.data;
      this.loading = false;
    });

    this.getData();
  },
  methods: {
    goSearch() {
      this.getData();
    },
    onClean() {
      this.formSearch.order_no = "";
      this.formSearch.purpose = "";
      this.formSearch.customer_id = "";
      this.formSearch.startDate = "";
      this.formSearch.endDate = "";
      this.getData();
    },
    // 打印
    async onPrint(item) {
      var list = [];
      this.loading = true;
      await StockOutService.filterAll(item.id).then(res => {
        list = res.data.map(res => {
          return res.id;
        });
        this.loading = false;
      });

      StockOutService.print(list, {
        headerId: item.id
      }).then(res => {
        const filename = "stock out-" + moment().format('YYYY-MM-DD');
        var blob = new Blob([res.data], { type: "text/html;charset=utf-8" });
        saveAs(blob, filename + ".html");
      });
    },
    add() {
      this.$router.push({
        name: "StockOutDetail",
        params: { id: 0 }
      });
    },
    onExport() {
      let filterData = {
        orderNo: this.formSearch.order_no,
        customerId: this.formSearch.customer_id
      };

      if (this.formSearch.startDate) {
        const startOf = moment(this.formSearch.startDate)
          .startOf("day")
          .valueOf();
        filterData.startDate = Core.Util.parseDate(startOf);
      }

      if (this.formSearch.endDate) {
        const endOf = moment(this.formSearch.endDate)
          .endOf("day")
          .valueOf();
        filterData.endDate = Core.Util.parseDate(endOf);
      }

      StockOutService.exportfile(filterData).then(res => {
        const filename = "stock out-" + moment().format('YYYY-MM-DD');
        saveAs(res.data, filename + ".xlsx");
      });
    },
    onEdit(item) {
      this.$router.push({
        name: "StockOutDetail",
        params: { id: item.id }
      });
    },
    onDelete(item) {
      this.$confirm("Are you sure you wish to delete this?", "Delete", {
        confirmButtonText: "Yes",
        cancelButtonText: "No",
        type: "warning"
      })
        .then(() => {
          this.deleteStockOut(item);
        })
        .catch(() => {});
    },
    onReview(e, item) {
      const scope = this;
      switch (e) {
        case "approved":
          this.$confirm("Do you agree to operate?", "Review", {
            confirmButtonText: "Yes",
            cancelButtonText: "No",
            type: "warning"
          }).then(() => {
            this.approvedStockOut(item.id);
          });
          break;
        default:
          break;
      }
    },
    approvedStockOut(id) {
      this.loading = true;
      StockOutHeaderService.approved(id)
        .then(res => {
          this.$message({
            type: "success",
            message: "Review Success!"
          });
          this.getData();
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err.body.error,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      this.getData();
    },
    deleteStockOut(item) {
      this.loading = true;
      StockOutHeaderService.remove(item.id)
        .then(res => {
          this.$message({
            type: "success",
            message: "Delete Success!"
          });
          this.getData();
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err.body.error,
            type: "error"
          });
          this.loading = false;
        });
    },
    getData() {
      this.loading = true;

      let filterData = {
        order_no: this.formSearch.order_no,
        purpose: this.formSearch.purpose,
        customer_id: this.formSearch.customer_id,
        date: {}
      };

      if (this.formSearch.startDate) {
        const startOf = moment(this.formSearch.startDate)
          .startOf("day")
          .valueOf();
        filterData.date.start = Core.Util.parseDate(startOf);
      }

      if (this.formSearch.endDate) {
        const endOf = moment(this.formSearch.endDate)
          .endOf("day")
          .valueOf();
        filterData.date.end = Core.Util.parseDate(endOf);
      }

      StockOutHeaderService.filter(filterData, this.currentPage)
        .then(res => {
          this.tableList = res.data.content.map(data => {
            data.selected = false;
            return data;
          });
          this.total = res.data.totalElements;
          this.loading = false;
        })
        .catch(err => {
          this.loading = false;
        });
    }
  }
};
</script>

<style lang="scss" >
.stock-out-index {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  //头部搜索框
  .list-form-inline {
    height: 100%;
    line-height: 50px;
    width: calc(100% - 130px);
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__label {
      color: #fff;
      width: 100px;
      text-align: left;
      margin-left: 5px;
    }
    .el-form-item__content {
      width: 180px !important;
    }
  }

  .status_approved {
    color: #98b42c;
  }

  .status_create {
    color: #e26829;
  }

  .dialog-region {
    .el-dialog__body {
      padding: 30px 67px;
    }
    .item {
      margin: 20px 0;
      font-size: 0;
      height: 30px;
      .item-box {
        line-height: 30px;
      }
      .label {
        display: inline-block;
        width: 115px;
        height: 30px;
        vertical-align: middle;
        span {
          color: #e26829;
          display: inline-block;
        }
      }
      .input {
        margin-left: 40px;
        width: 260px;
        i {
          color: #ffffff99;
        }
      }
    }
  }
}
</style>


// WEBPACK FOOTER //
// out.vue?607c4944