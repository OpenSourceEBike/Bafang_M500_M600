<template>
  <div class="stocks-index">
    <div class="search-header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item>
          <el-input v-model.trim="formSearch.order_no" placeholder="Order No."></el-input>
        </el-form-item>
        <el-form-item>
          <el-input v-model.trim="formSearch.source_from" placeholder="Source From"></el-input>
        </el-form-item>
        <el-form-item>
          <el-input v-model.trim="formSearch.invoice_no" placeholder="Invoice No."></el-input>
        </el-form-item>
        <el-form-item>
          <el-input v-model.trim="formSearch.tracking_no" placeholder="Tracking No."></el-input>
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
          <button type="button" @click="onAdd" class="el-button el-button--preview">
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
        <table class="table-content" bor der="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody>
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.order_no }}</td>
              <td>{{ item.source_from }}</td>
              <td>{{ item.invoice_no }}</td>
              <td>{{ item.tracking_no }}</td>
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
import StockInService from "src/services/nl/stockInService";
import StockInHeaderService from "src/services/nl/stockInHeaderService.js";
import moment from "moment";
import Core from "core/core";
import saveAs from "file-saver";
export default {
  data() {
    return {
      headData: [
        "Order No.",
        "Source From",
        "Invoice No.",
        "Tracking No.",
        "Date",
        "Create Date",
        "User",
        "Status",
        "Operation"
      ],
      tableList: [],
      printData: {},
      formSearch: {},
      currentPage: 1,
      total: 0,
      loading: false,
      payTypeArr: Core.StatusData.STOCKS_PAY_TYPE
    };
  },
  async created() {
    this.getData();
  },
  methods: {
    goSearch() {
      this.getData();
    },
    onClean() {
      this.formSearch.order_no = "";
      this.formSearch.source_from = "";
      this.formSearch.invoice_no = "";
      this.formSearch.tracking_no = "";
      this.formSearch.startDate = "";
      this.formSearch.endDate = "";
      this.getData();
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      this.getData();
    },
    // 库存
    getData() {
      this.loading = true;
      let filterData = {
        order_no: this.formSearch.order_no,
        source_from: this.formSearch.source_from,
        invoice_no: this.formSearch.invoice_no,
        tracking_no: this.formSearch.tracking_no,
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
      StockInHeaderService.filter(filterData, this.currentPage).then(res => {
        this.tableList = res.data.content.map(data => {
          data.selected = false;
          return data;
        });
        this.total = res.data.totalElements;
        this.loading = false;
      });
    },
    // 打印
    async onPrint(item) {
      var list = [];
      this.loading = true;
      await StockInService.filterAll(item.id).then(res => {
        list = res.data.map(res => {
          return res.id;
        });
        this.loading = false;
      });
      StockInService.print(list, {
        headerId: item.id
      }).then(res => {
        const filename = "stock in-" + moment().format('YYYY-MM-DD');
        var blob = new Blob([res.data], {
          type: "text/html;charset=utf-8"
        });
        saveAs(blob, filename + ".html");
      });
    },
    onAdd() {
      this.$router.push({
        name: "StockInDetail",
        params: {
          id: 0
        }
      });
    },
    onExport() {
      let filterData = {
        orderNo: this.formSearch.order_no || "",
        sourceFrom: this.formSearch.source_from || "",
        invoice_no: this.formSearch.invoice_no || ""
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
      StockInService.exportfile(filterData).then(res => {
        const filename = "stock in-" + moment().format('YYYY-MM-DD');
        saveAs(res.data, filename + ".xlsx");
      });
    },
    onEdit(item) {
      this.$router.push({
        name: "StockInDetail",
        params: {
          id: item.id
        }
      });
    },
    onDelete(item) {
      this.$confirm("Are you sure you wish to delete this?", "Delete", {
        confirmButtonText: "Yes",
        cancelButtonText: "No",
        type: "warning"
      })
        .then(() => {
          this.loading = true;
          StockInHeaderService.remove(item.id).then(res => {
            this.$message({
              type: "success",
              message: "Delete Success!"
            });
            this.getData();
            this.loading = false;
          });
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
            this.approvedStockIn(item.id);
          });
          break;
        default:
          break;
      }
    },
    approvedStockIn(id) {
      this.loading = true;
      StockInHeaderService.approved(id)
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
    }
  }
};
</script>

<style lang="scss">
.stocks-index {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%; //头部搜索框
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
  .dialog {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 66px;
    }
    .item {
      span.required {
        color: #e26829;
        display: inline-block;
      }
      line-height: 30px;
      display: inline-block;
      .title,
      .land {
        display: inline-block;
        width: 120px;
      }
      .land {
        width: 250px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
      }
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
// in.vue?0a9f1a6a