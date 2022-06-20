<template>
  <div class="CI-header-save">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goCIList" title="back"></div>
        </div>
      </div>
      <label class="title">{{ orderId ? 'Edit Commerial Invoice' : 'Add Commerial Invoice' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content content-width" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Commerial Invoice</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Delivery Order No.
                <span class="required">*</span>
              </label>
              <el-autocomplete
                class="input"
                v-model="formData.delivery_order_no"
                ref="saveTagInput"
                size="small"
                :fetch-suggestions="querySearchAsync"
                placeholder="Delivery Order No."
                @select="handleInputConfirm"
              ></el-autocomplete>
            </div>
            <div class="item">
              <label class="label">
                Date
                <span class="required">*</span>
              </label>
              <el-date-picker class="input" v-model="date" type="date" placeholder="Date"></el-date-picker>
            </div>
            <div class="item">
              <label class="label">Customer</label>
              <el-input
                class="input"
                v-model="formData.customer_name"
                placeholder="Customer"
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Address
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.address" placeholder="Address" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">CI No.</label>
              <el-input class="input" v-model="formData.ci_no" placeholder="CI No." disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Tel</label>
              <el-input class="input" v-model="formData.tel" :disabled="true" placeholder="Tel">
                <el-select
                  v-model="formData.country_code"
                  filterable
                  slot="prepend"
                  :disabled="true"
                  class="country-code"
                >
                  <el-option
                    v-for="item in areaCodeList"
                    :key="item.area_code"
                    :label="item.area_code"
                    :value="item.area_code"
                  ></el-option>
                </el-select>
              </el-input>
            </div>
            <div class="item">
              <label class="label">
                ETD
                <span class="required">*</span>
              </label>
              <el-date-picker class="input" v-model="etd" type="date" placeholder="ETD"></el-date-picker>
            </div>
            <div class="item">
              <label class="label">VAT No.</label>
              <el-input class="input" v-model="formData.vat" placeholder="VAT No." disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Shipment</label>
              <el-input class="input" v-model="formData.shipment" placeholder="Shipment" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Trade Terms</label>
              <el-input
                class="input"
                v-model="formData.trade_terms"
                placeholder="Trade Terms"
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">Origin</label>
              <el-input class="input" v-model="formData.origin" placeholder="Origin" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Destination</label>
              <el-input
                class="input"
                v-model="formData.destination"
                placeholder="Destination"
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Receiver
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.atten" placeholder="Receiver" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">
                Total Amount
                <span class="required">*</span>
              </label>
              <div class="input" style="display: inline-block;height: 60px;">
                <el-input
                  class="input"
                  style="width: 137px;margin-left: 0;"
                  v-model="formData.total_amount"
                  placeholder="Total Amount"
                ></el-input>
                <el-input
                  class="input"
                  style="width: 105px"
                  v-model="formData.unit"
                  :disabled="true"
                ></el-input>
              </div>
            </div>
            <div class="item">
              <label class="label">Payment Term</label>
              <el-input
                class="input"
                v-model="formData.payment_term"
                placeholder="Payment Terms"
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Total Volume
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.total_volume" placeholder="Total Volume">
                <template slot="append">CBM</template>
              </el-input>
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
                <td>
                  <el-input class="input" v-model="item.gw" placeholder="G.W.(KGS)"></el-input>
                </td>
                <td>
                  <el-input class="input" v-model="item.nw" placeholder="N.W.(KGS)"></el-input>
                </td>
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
        <div class="footer">
          <el-button type="primary" @click="onPreview">CI Preview</el-button>
          <el-button type="primary" @click="onCreate">Save</el-button>
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
      date: new Date(),
      etd: "",
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
          scope.etd = scope.formData.etd * 1000;
          scope.date = scope.formData.date * 1000;
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
    // 远程搜索
    querySearchAsync(queryString, cb) {
      var restaurants = this.deliveryNoData;
      var results = queryString
        ? restaurants.filter(this.createStateFilter(queryString))
        : restaurants;
      cb(results);
    },
    // 输入搜索
    createStateFilter(queryString) {
      return state => {
        return state.value.indexOf(queryString.toLowerCase()) !== -1;
      };
    },
    handleInputConfirm(item) {
      this.formData.delivery_order_id = item.id;
      if (this.customerList.length === 0 && this.plData.length === 0) return;
      const data = this.customerList.find(res => {
        return res.id === item.company_id;
      });
      this.filterDeliveryDetails(item.id);
      this.getDeliveryPiDetails(item.id);
      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.shipment = item.shipment;
      this.formData.origin = item.origin;
      this.formData.destination = item.destination;
      this.formData.sales_name = item.sales_name;
      this.formData.customer_id = data.id;
      this.formData.customer_name = data.name;
      this.formData.country = data.country;
      this.formData.address = data.address;
      this.formData.tel = data.phone;
      this.formData.country_code = data.country_code;
      this.formData.vat = data.vat_no;
      this.formData.trade_terms = data.trade_term;
      this.formData.payment_term = data.payment_term;
      this.formData.atten = data.receiver_name;
    },
    initCustomeInfo() {
      this.formData.shipment = "";
      this.formData.origin = "";
      this.formData.destination = "";
      this.formData.sales_name = "";
      this.formData.customer_id = "";
      this.formData.customer_name = "";
      this.formData.country = "";
      this.formData.country_code = "";
      this.formData.address = "";
      this.formData.tel = "";
      this.formData.vat = "";
      this.formData.trade_terms = "";
      this.formData.payment_term = "";
      this.formData.atten = "";
    },
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      if (this.formData.delivery_order_no === "") {
        this.$message({
          message: "Please add Delivery Order No. ",
          type: "warning"
        });
        return false;
      }
      if (this.formData.etd === "" || this.etd <= 0) {
        this.$message({
          message: "Please add ETD",
          type: "warning"
        });
        return false;
      }
      if (this.formData.date === "" || this.date <= 0) {
        this.$message({
          message: "Please add Date ",
          type: "warning"
        });
        return false;
      }
      if (
        this.formData.total_amount <= 0 ||
        this.formData.total_amount === ""
      ) {
        this.$message({
          message: "Please add Total Amount",
          type: "warning"
        });
        return false;
      }
      if (
        this.formData.total_volume <= 0 ||
        this.formData.total_volume === ""
      ) {
        this.$message({
          message: "Please add Total Volume",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    // 打印
    onPreview() {},
    // 操作
    onCreate() {
      const scope = this;
      this.formData.etd = Core.Util.parseDate(this.etd) || 0;
      this.formData.date = Core.Util.parseDate(this.date) || 0;
      scope.loading = true;
      CommercialInvoiceService.postCommercialInvoice(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          scope.date = scope.formData.date * 1000;
          scope.etd = scope.formData.etd * 1000;
          scope.orderId = res.data.id;
          scope.$router.push({
            name: "commerialInvoiceSave",
            query: { orderId: res.data.id }
          });
        })
        .then(() => {
          return PIDetailsService.postPIDetailsBulk(this.orderItems);
        })
        .then(res => {
          scope.orderItems = JSON.parse(JSON.stringify(res.data));
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.loading = false;
          this.$router.go(-1);
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
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
.CI-header-save {
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
            width: 135px;
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
      text-align: right;
      padding-right: 40px;
    }
  }
}
</style>



// WEBPACK FOOTER //
// commerial-invoice-save.vue?4702ca59