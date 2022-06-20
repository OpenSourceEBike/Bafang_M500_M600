<template>
  <div class="PI-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goPIList" title="back"></div>
        </div>
      </div>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item print" @click="onPrint" title="print"></div>
        </div>
      </div>
      <label class="title">Proforma Invoice Details</label>
    </div>
    <div class="content-wrap">
      <div class="content content-width" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Proforma Invoice Info</label>
          <div class="left">
            <div class="item">
              <label class="label">PI No.</label>
              <span>{{formData.pi_no}}</span>
            </div>
            <div class="item">
              <label class="label">Sales</label>
              <span>{{formData.sales_name}}</span>
            </div>
            <div class="item">
              <label class="label">Customer</label>
              <span>{{formData.customer_name}}</span>
            </div>
            <div class="item">
              <label class="label">Country</label>
              <span>{{formData.country}}</span>
            </div>
            <div class="item">
              <label class="label">Address</label>
              <span>{{formData.address}}</span>
            </div>
            <div class="item">
              <label class="label">Tel</label>
              <span>{{formData.country_code}}&nbsp;{{formData.telephone}}</span>
            </div>
            <div class="item">
              <label class="label">VAT No.</label>
              <span>{{formData.vat_no}}</span>
            </div>
            <div class="item">
              <label class="label">ETD</label>
              <span>{{formData.etd}}</span>
            </div>
            <div class="item">
              <label class="label">Trade Terms</label>
              <span>{{formData.trade_terms}}</span>
            </div>
            <div class="item">
              <label class="label">Payment Term</label>
              <span>{{formData.payment_term}}</span>
            </div>
            <div class="item">
              <label class="label">Date</label>
              <span>{{formData.date}}</span>
            </div>
            <div class="item">
              <label class="label">Currency/Amount</label>
              <span>{{formData.currency | currencyType}}{{formData.amount}}</span>
            </div>
          </div>
        </div>
        <div class="table-wrap table-overflow">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in headData">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <!-- <td>{{item.category_root_id | categoryRoot}}</td> -->
                <td>{{item.model_name}}</td>
                <td>{{item.bf_art_no}}</td>
                <td>{{item.customer_art_no}}</td>
                <td>{{item.description}}</td>
                <td>{{item.currency | currencyType}}{{item.unit_price}}</td>
                <td>{{item.vat_rate}}%</td>
                <td>{{item.quantity}}</td>
                <td>{{item.amount}}</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list">
          <span class="att-total">total: {{currency | currencyType}}{{totalCount}}</span>
        </div>
      </div>
    </div>
  </div>
</template>;
    
<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";
import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

import PIService from "src/services/PIService";
import saveAs from "file-saver";

export default {
  data() {
    return {
      loading: false,
      headData: [],
      total: 0,
      orderId: 0,
      //顾客
      customerList: [],
      formData: {},
      orderItems: [],
      currency: "",
      currencies: [
        {
          currency: 1,
          label: "€"
        },
        {
          currency: 2,
          label: "$"
        },
        {
          currency: 3,
          label: "DM."
        },
        {
          currency: 4,
          label: "￥"
        },
        {
          currency: 5,
          label: "￡"
        }
      ]
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
  created() {},
  computed: {
    //计算总金额
    totalCount: function() {
      let total = 0;
      this.orderItems.forEach(res => {
        total += res.amount;
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
      "Amount"
    ];
    this.loading = true;
    this.getAllCustomer()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.customerList = res;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.dialogVisible = true;
        this.loading = false;
      });

    const scope = this;
    scope.orderId = this.$route.query.orderId;
    scope.lodaing = true;
    scope
      .getPIById(scope.orderId)
      .then(res => {
        scope.formData = JSON.parse(JSON.stringify(res));
        scope.formData.date = Core.Util.changeDateForm(
          scope.formData.date * 1000
        );
        scope.formData.etd = Core.Util.changeDateForm(
          scope.formData.etd * 1000
        );
        return scope.filterPIDetails(scope.formData.pi_no);
      })
      .then(res => {
        scope.orderItems = res.content.map(data => {
          scope.currency = data.currency;
          data.item_part_no = data.item_code;
          return data;
        });
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err.message,
          type: "error"
        });
        scope.loading = false;
      });
  },
  methods: {
    //返回
    goPIList() {
      this.$router.push({ name: "PIList" });
    },
    //获取所有顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    // 根据id获取PI详情
    getPIById(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/sales/proformaInvoice/" + id
      });
    },
    //筛选PI详情
    filterPIDetails(id) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/detail/filter",
        data: {
          pi_no: id
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    // 打印pdf
    onPrint() {
      PIService.printPI(this.orderId).then(res => {
        var now = new Date();
        var exportFileName =
          now.getFullYear() + "-" + now.getMonth() + "-" + now.getDay();

        const filename = this.formData.pi_no || exportFileName;
        saveAs(res.data, filename + ".pdf");
      });
    }
  },
  watch: {}
};
</script>
    
    
<style lang="scss" rel="stylesheet/scss">
.PI-details {
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
    min-width: 1246px;
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
        height: 345px;
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
            width: 110px;
            margin-left: 15px;
          }
          span {
            display: inline-block;
          }
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
    .att-total {
      display: inline-block;
      position: absolute;
      top: 0;
      right: 44px;
      color: #e26829;
    }
  }
}
</style>



// WEBPACK FOOTER //
// proforma-invoice-details.vue?1be7b362