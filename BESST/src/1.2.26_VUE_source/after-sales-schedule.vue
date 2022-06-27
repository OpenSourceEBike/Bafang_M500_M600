<template>
  <div class="after-sales-schedule">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterSalesList" title="back"></div>
        </div>
      </div>
      <label class="title">After Sales Schedule</label>
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
            <!-- table -->
          </div>
          <div v-show="currentSetp ==2"></div>
          <div v-show="currentSetp ==3"></div>
          <div v-show="currentSetp ==4">
            <!-- deliver table -->
            <deliver-tab :tableList="deliverList"></deliver-tab>
          </div>
          <div v-show="currentSetp == 5">
            <!-- stock out table -->
            <stock-out-tab :tableList="stockOutList"></stock-out-tab>
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
import OrderScheduleService from "src/services/orderScheduleService";
import DeliveryDetailsService from "src/services/deliveryDetailsService";
import StockOutService from "src/services/stockOutService";
import InvoiceService from "src/services/invoiceService";

import DeliverTab from "src/components/step-table-temp/deliver-tab";
import StockOutTab from "src/components/step-table-temp/stock-out-tab";
import InvoiceTab from "src/components/step-table-temp/invoice-tab";

export default {
  components: {
    deliverTab: DeliverTab,
    stockOutTab: StockOutTab,
    invoiceTab: InvoiceTab
  },
  data() {
    return {
      loading: false,
      active: 1,
      stepDatas: [],
      currentSetp: 1,
      dialogItemListVisible: false,
      deliverList: [],
      stockOutList: [],
      invoiceList: []
    };
  },
  created() {
    const scope = this;
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      // this.loading = true;
      scope.formatSetpData({
        service_id: 0,
        service_status: 0,
        service_time: 0,
        delivery_id: 0,
        delivery_status: 0,
        delivery_time: 0,
        service_record_id: 0,
        service_record_status: 0,
        service_record_time: 0,
        shipment_id: 0,
        shipment_status: 0,
        shipment_time: 0,
        invoice_id: 0,
        invoice_status: 0,
        invoice_time: 0,
        parcel_id: 0,
        parcel_status: 0,
        parcel_time: 0,
        progress: 1
      });
      // OrderScheduleService.filterAfterSalesStatus(scope.orderId)
      //   .then(res => {
      //     if (res.data.code === -2) {
      //       throw res.data.message;
      //     }
      //     scope.formatSetpData(res.data.content[0]);
      //     scope.loading = false;
      //   })
      //   .catch(err => {
      //     this.$message({
      //       message: err,
      //       type: "error"
      //     });
      //     this.loading = false;
      //   });
    }
  },
  mounted() {},
  methods: {
    formatSetpData(data) {
      const scope = this;
      let hash = {
        //customer_service_record
        service: {
          index: 1,
          title: "Service Center"
        },
        parcel: {
          index: 2,
          title: "Input Parcel info"
        },
        //technical_service_record
        service_record: {
          index: 3,
          title: "Service Record"
        },
        delivery: {
          index: 4,
          title: "Delivery"
        },
        shipment: {
          index: 5,
          title: "Stock out"
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
        case "service":
        //   return scope.deliveryType(item.status);
        case "parcel":
        //   return scope.cleanType(item.status);
        case "service_record":
        //   return scope.shipmentType(item.status);
        case "delivery":
          return scope.deliveryType(item.status);
        case "shipment":
          return scope.shipmentType(item.status);
        case "invoice":
          return scope.invoiceType(item.status);
        default:
          break;
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
    stepChange(item) {
      if (!item.id) return;
      this.currentSetp = item.index;
      switch (item.type) {
        case "service":
          break;
        case "parcel":
          break;
        case "service_record":
          break;
        case "delivery":
          DeliveryDetailsService.getDeliveryById(item.id).then(res => {
            this.deliverList = [res.data];
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
        default:
          break;
      }
    },
    goAfterSalesList() {
      this.$router.push({ name: "serviceCenter" });
    }
  },
  computed: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.after-sales-schedule {
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
// after-sales-schedule.vue?5349143d