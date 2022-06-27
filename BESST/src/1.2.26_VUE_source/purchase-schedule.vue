<template>
  <div class="purchase-shedule">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goPurchaseList" title="back"></div>
        </div>
      </div>
      <label class="title">Purchase Schedule</label>
    </div>
    <div class="content-wrap">
      <div class="content">
        <div class="step-warp">
          <el-steps :active="active" align-center>
            <el-step
              @click.native="stepChange(stepData)"
              v-for="(stepData, index) in stepDatas"
              :title="stepData.title"
              :description="stepData.time"
              :status="stepData.status"
              :key="index"
            ></el-step>
          </el-steps>
        </div>
        <div class="content-record" v-loading.body="loading">
          <div v-show="currentSetp ==1">
            <purchase-tab :tableList="purchaseList"></purchase-tab>
          </div>
          <div v-show="currentSetp ==2">
            <!-- deliver table -->
            <deliver-tab :tableList="deliverList"></deliver-tab>
          </div>
          <div v-show="currentSetp ==3">
            <stock-out-tab :tableList="stockOutList"></stock-out-tab>
          </div>
          <div v-show="currentSetp ==4">
            <clean-tab :tableList="cleanList"></clean-tab>
          </div>
          <div v-show="currentSetp == 5">
            <!-- reception table(stock in table) -->
            <stockIn-tab :tableList="stockInList"></stockIn-tab>
          </div>
          <div v-show="currentSetp == 6">
            <!-- Invoice table -->
            <invoice-tab :tableList="invoiceList"></invoice-tab>
          </div>
        </div>
      </div>
    </div>
    <el-dialog :visible.sync="dialogItemListVisible" title="title" size="large">
      <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="dialogItemListVisible = false">Close</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
import OrderScheduleService from "src/services/orderScheduleService.js";
import DeliveryDetailsService from "src/services/deliveryDetailsService";
import StockOutService from "src/services/stockOutService";
import CleanService from "src/services/cleanService";
import StockInService from "src/services/stockInService";
import InvoiceService from "src/services/invoiceService";
import PurchaseService from "src/services/purchaseService";

import DeliverTab from "src/components/step-table-temp/deliver-tab";
import CleanTab from "src/components/step-table-temp/clean-tab";
import StockOutTab from "src/components/step-table-temp/stock-out-tab";
import StockInTab from "src/components/step-table-temp/stockIn-tab";
import InvoiceTab from "src/components/step-table-temp/invoice-tab";
import PurchaseTab from "src/components/step-table-temp/purchase-tab";

export default {
  components: {
    cleanTab: CleanTab,
    deliverTab: DeliverTab,
    stockInTab: StockInTab,
    invoiceTab: InvoiceTab,
    stockOutTab: StockOutTab,
    purchaseTab:PurchaseTab
  },
  data() {
    return {
      loading: false,
      active: 1,
      stepDatas: [],
      currentSetp: 1,
      dialogItemListVisible: false,
      deliverList: [],
      cleanList: [],
      stockOutList: [],
      stockInList: [],
      invoiceList: [],
      purchaseList: []
    };
  },
  created() {
    const scope = this;
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      this.loading = true;
      OrderScheduleService.filterPurchaseStatus(scope.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formatSetpData(res.data.content[0]);
          scope.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    }
  },
  mounted() {},
  methods: {
    formatSetpData(data) {
      const scope = this;
      let hash = {
        purchase: {
          index: 1,
          title: "Purchase Order"
        },
        delivery: {
          index: 2,
          title: "Delivery Order"
        },
        shipment: {
          index: 3,
          title: "Stock out"
        },
        clean: {
          index: 4,
          title: "Clean"
        },
        receipt: {
          index: 5,
          title: "Stock in"
        },
        invoice: {
          index: 6,
          title: "Invoice"
        }
      };
      let stepArr = [];
      scope.active = data.progress;
      Object.keys(data).forEach(function(key) {
        let name = key.replace(/([_][^_]+)$/, "");
        if (hash[name]) {
          const _key = key.match(/([^_]+)$/)[1];
          hash[name][_key] = data[key];
          hash[name].type = name;
        }
      });
      Object.keys(hash).forEach(function(key) {
        stepArr.push(hash[key]);
      });
      scope.stepDatas = stepArr.map(res => {
        res.time =
          (res.time && Core.Util.changeDateForm(res.time * 1000)) || "";
        res.status = scope.checkType(res);
        return res;
      });
      // 第一次默认load第一个状态
      scope.stepChange(scope.stepDatas[0]);
    },
    checkType(item) {
      const scope = this;
      switch (item.type) {
        case "purchase":
          return scope.purchaseType(item.status);
        case "delivery":
          return scope.deliveryType(item.status);
        case "clean":
          return scope.cleanType(item.status);
        case "shipment":
          return scope.shipmentType(item.status);
        case "invoice":
          return scope.invoiceType(item.status);
        case "receipt":
          return scope.receptionType(item.status);
        default:
          break;
      }
    },
    purchaseType(status) {
      switch (status) {
        case 0:
          return "wait";
        case 1:
          return "process";
        case 2:
          return "success";
        default:
          return "wait";
      }
    },
    cleanType(status) {
      switch (status) {
        case 0:
          return "wait";
        case 1:
          return "process";
        case 2:
          return "success";
        default:
          return "wait";
      }
    },
    deliveryType(status) {
      switch (status) {
        case 0:
        case 2:
        case 5:
          return "wait";
        case 1:
          return "process";
        case 3:
          return "success";
        case 4:
          return "error";
        default:
          return "wait";
      }
    },
    shipmentType(status) {
      switch (status) {
        case 0:
          return "wait";
        case 1:
          return "process";
        case 2:
          return "error";
        case 3:
          return "success";
        default:
          return "wait";
      }
    },
    invoiceType(status) {
      switch (status) {
        case 0:
          return "wait";
        case 1:
        case 2:
        case 3:
          return "process";
        case 4:
          return "success";
        default:
          return "wait";
      }
    },
    receptionType(status) {
      switch (status) {
        case 0:
          return "wait";
        case 1:
          return "process";
        case 2:
          return "success";
        default:
          return "wait";
      }
    },
    stepChange(item) {
      if (!item.id) return;
      this.currentSetp = item.index;
      switch (item.type) {
        case "purchase":
          PurchaseService.getPurchaseByid(item.id).then(res => {
            this.purchaseList = [res.data];
          });
          break;
        case "delivery":
          DeliveryDetailsService.getDeliveryById(item.id).then(res => {
            this.deliverList = [res.data];
          });
          break;
        case "clean":
          this.loading = true;
          CleanService.getCleanRecord(item.id)
            .then(res => {
              if (res.data.code === -2) {
                throw res.data.message;
              }
              this.cleanList = [res.data];
              this.loading = false;
            })
            .catch(err => {
              this.$message({
                message: err,
                type: "error"
              });
              this.loading = false;
            });
          break;
        case "shipment":
          StockOutService.getShipment(item.id).then(res => {
            this.stockOutList = [res.data];
          });
          break;
        case "invoice":
          InvoiceService.getInvoice(item.id).then(res => {
            this.invoiceList = [res.data];
          });
          break;
        case "receipt":
          StockInService.getReceipt(item.id).then(res => {
            this.stockInList = [res.data];
          });
          break;
        default:
          break;
      }
    },
    goPurchaseList() {
      this.$router.push({ name: "purchaseList" });
    }
  },
  computed: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.purchase-shedule {
  .content {
    border: 1px solid #2b2f3b;
    margin: 20px auto 0 auto;
  }
  .step-warp {
    border-bottom: 1px solid #333;
    padding: 5px 0;
  }

  .el-step:hover {
    cursor: pointer;
  }

  .content-record {
    h3 {
      margin: 10px 0;
    }
  }
}
</style>



// WEBPACK FOOTER //
// purchase-schedule.vue?24588d91