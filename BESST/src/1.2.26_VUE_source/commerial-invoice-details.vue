<template>
  <div class="CI-header">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goCIList" title="back"></div>
        </div>
      </div>
      <label class="title">Commerial Invoice Details</label>
    </div>
    <div class="content-wrap">
      <div class="content content-width" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Commerial Invoice</label>
          <div class="left">
            <div class="item">
              <label class="label">Delivery Order No.</label>
              <span>{{formData.delivery_order_no}}</span>
            </div>
            <div class="item">
              <label class="label">Date</label>
              <span>{{formData.date | dateFormat}}</span>
            </div>
            <div class="item">
              <label class="label">Customer</label>
              <span>{{formData.customer_name}}</span>
            </div>
            <div class="item">
              <label class="label">Address</label>
              <span>{{formData.address}}</span>
            </div>
            <div class="item">
              <label class="label">CI No.</label>
              <span>{{formData.ci_no}}</span>
            </div>
            <div class="item">
              <label class="label">Tel</label>
              <span>{{formData.country_code}} {{formData.tel}}</span>
            </div>
            <div class="item">
              <label class="label">ETD</label>
              <span>{{formData.etd | dateFormat}}</span>
            </div>
            <div class="item">
              <label class="label">VAT No.</label>
              <span>{{formData.vat}}</span>
            </div>
            <div class="item">
              <label class="label">Shipment</label>
              <span>{{formData.shipment}}</span>
            </div>
            <div class="item">
              <label class="label">Trade Terms</label>
              <span>{{formData.trade_terms}}</span>
            </div>
            <div class="item">
              <label class="label">Origin</label>
              <span>{{formData.origin}}</span>
            </div>
            <div class="item">
              <label class="label">Destination</label>
              <span>{{formData.destination}}</span>
            </div>
            <div class="item">
              <label class="label">Receiver</label>
              <span>{{formData.atten}}</span>
            </div>
            <div class="item">
              <label class="label">Total Amount</label>
              <span>{{formData.total_amount}}{{formData.unit}}</span>
            </div>
            <div class="item">
              <label class="label">Payment Term</label>
              <span>{{formData.payment_term}}</span>
            </div>
            <div class="item">
              <label class="label">Total Volume</label>
              <span>{{formData.total_volume}}</span>
            </div>
          </div>
        </div>
        <div class="order-info-two">
          <label class="title">Pakeage List</label>
          <div class="pi-left">
            <el-tag size="small" :key="tag.id" v-for="tag in plTags">{{tag.pi_no}}</el-tag>
          </div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in headData">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <td>{{item.model_name}}</td>
                <td>{{item.bf_art_no}}</td>
                <td>{{item.customer_art_no}}</td>
                <td>{{item.description}}</td>
                <td>{{item.currency | currencyType}}{{item.unit_price}}</td>
                <td>{{item.vat_rate}}%</td>
                <td>{{item.quantity}}</td>
                <td>{{item.amount}}</td>
                <td>{{item.gw}}</td>
                <td>{{item.nw}}</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list">
          <span
            class="att-total first-total"
          >Total Amount to be paid: {{ currency | currencyType }}{{ totalAmountCount }}</span>
          <span class="att-total">Total Weight: {{ totalWeightCount }}kg</span>
        </div>
      </div>
    </div>
  </div>
</template>
    
<script>
import Core from "core/core";
import CommercialInvoiceService from "src/services/commerialInvoiceService";
import DeliveryDetailsService from "src/services/deliveryDetailsService";
import DeliveryService from "src/services/deliveryService";
import PIDetailsService from "src/services/proformaInvoiceDetailService";
import WarehouseService from "src/services/warehouseService";
import areaCodeInDB from "core/areaCode";
import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      headData: [],
      currentPage: 1,
      total: 0,
      orderId: 0,
      //顾客
      customerList: [],
      // 交货单
      deliveryNoData: [],
      formData: {
        delivery_order_no: "",
        delivery_order_id: "",
        address: "",
        atten: "",
        ci_no: "",
        customer_name: "",
        sales_name: "",
        customer_id: "",
        etd: "",
        id: "",
        payment_term: "",
        shipment: "",
        tel: "",
        total_amount: 0,
        trade_terms: "",
        vat: "",
        date: "",
        total_volume: 0,
        unit: "Pallet",
        origin: "",
        destination: "",
        country_code: ""
      },
      shippedData: [
        {
          id: 1,
          name: "By Train"
        },
        {
          id: 2,
          name: "By Ship"
        },
        {
          id: 3,
          name: "By Aircraft"
        }
      ],
      defaultProps: {
        id: "id",
        children: "child",
        label: "name"
      },
      selectCategoryItem: {
        id: "",
        name: ""
      },
      orderItems: [],
      areaCodeList: [],
      plData: [],
      plTags: [],
      nw: "",
      gw: "",
      currency: ""
    };
  },
  i18n: {
    messages: {
      en: EN.Component.Order.Save,
      zh: ZH.Component.Order.Save,
      de: DE.Component.Order.Save,
      nl: NL.Component.Order.Save
    }
  },
  async created() {
    const scope = this;
    this.areaCodeList = areaCodeInDB.init();
    await WarehouseService.getAllCustomer()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.customerList = res.data;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });

    this.loading = true;
    await WarehouseService.getAllDelivery()
      .then(item => {
        let data = item.data.filter(res => {
          return res.status === 3;
        });
        scope.deliveryNoData = data.map(res => {
          return {
            id: res.id,
            company_id: res.company_id,
            value: res.delivery_order_no,
            status: res.status,
            sales_name: res.create_user_id,
            shipment: res.shipment,
            origin: res.origin,
            destination: res.destination
          };
        });
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.loading = true;
      CommercialInvoiceService.getCommercialInvoiceId(scope.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          return scope.formData.delivery_order_id;
        })
        .then(deliveryId => {
          this.getDeliveryPiDetails(deliveryId);
          this.filterDeliveryDetails(deliveryId);
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
  computed: {
    //计算总金额
    totalAmountCount: function() {
      let total = 0;
      this.orderItems.forEach(res => {
        const num = res.quantity * res.unit_price * (1 + res.vat_rate / 100);
        res.amount = Math.round(num * 100) / 100;
        total += res.amount;
      });
      return Math.round(total * 100) / 100;
    },
    totalWeightCount: function() {
      let total = 0;
      this.orderItems.forEach(res => {
        const num = Math.round(res.gw * 100) / 100;
        total += num;
      });

      return Math.round(total * 100) / 100;
    }
  },
  mounted() {
    this.headData = [
      "Item Name",
      "BF Art No.",
      "Customer Art No.",
      "Description",
      "Unit Price",
      "VAT rate",
      "Quantity",
      "Amount",
      "G.W.(KGS)",
      "N.W.(KGS)"
    ];
  },
  methods: {
    //返回
    goCIList() {
      this.$router.push({ name: "commerialInvoice" });
    },
    // 根据id查询pi
    filterDeliveryDetails(id) {
      const scope = this;
      DeliveryDetailsService.filterDeliveryDetails(1, {}, id)
        .then(res => {
          scope.plTags = res.data.content;
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
    // 根据id查询pi详情
    getDeliveryPiDetails(id) {
      const scope = this;
      DeliveryService.getDeliveryPIDetails(id)
        .then(res => {
          scope.orderItems = res.filter(item => {
            item.currency = item.currency - 0;
            scope.currency = item.currency;
            return item;
          });
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
.CI-header {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

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
    width: 180px !important;
  }
  .content-width {
    min-width: 1285px;
  }
  .content {
    border: 1px solid #2b2f3b;
    margin: 20px auto 0 auto;
    .order-info {
      width: 100%;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 440px;
        background: #353945;
        box-shadow: 0 -1px #2b2f3b;
        border-bottom: 1px solid #2b2f3b;
        .item {
          line-height: 60px;
          overflow: hidden;
          width: 49%;
          height: 50px;
          display: inline-block;
          .label {
            display: inline-block;
            width: 130px;
            margin-left: 15px;
          }
          .input {
            margin-left: 10px;
            width: 256px;
            border-radius: 0 0 10px 10px;
          }

          .el-input-group__prepend .el-select .el-input .el-input__inner {
            width: 92px;
          }
          .el-input-group__prepend .el-select .el-input .el-input__inner:hover {
            border-color: transparent;
          }
        }
      }
    }

    .parts-list {
      position: relative;
      text-align: right;
      padding: 0 40px;
      font-size: 12px;
      overflow: hidden;
      height: 50px;
      line-height: 50px;
      width: 100%;
      box-shadow: 0 -1px #2b2f3b;
      .first-total {
        margin-right: 48px;
      }
      .att-total {
        display: inline-block;
        color: #e26829;
      }
    }

    .order-info-two {
      width: 100%;
      height: 111px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .pi-left {
        float: left;
        width: 100%;
        height: 60px;
        background: #353945;
        box-shadow: 0 -1px #2b2f3b;
        padding: 16px;
        border-bottom: 1px solid #2b2f3b;
        .el-tag + .el-tag {
          margin-left: 10px;
        }
        .button-new-tag {
          margin-left: 6px;
        }
      }
    }

    .footer {
      width: 100%;
      height: 60px;
      line-height: 60px;
      padding-left: 40px;
      box-shadow: 0 -1px #2b2f3b;
    }
  }
}
</style>



// WEBPACK FOOTER //
// commerial-invoice-details.vue?ba4eb1e4