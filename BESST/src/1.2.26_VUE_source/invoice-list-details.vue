<template>
  <div class="invoice-detail">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">Invoice Info Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Invoice Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Customer</label>
              <span>{{formData.customer_name}}</span>
            </div>
            <div class="item">
              <label class="label">Customer No.</label>
              <span>{{formData.customer_no}}</span>
            </div>
            <!-- <div class="item">
              <label class="label">Salesman</label>
              <span>{{formData.salesman}}</span>
            </div>-->
            <div class="item">
              <label class="label">Currency/Amount</label>
              <span>{{formData.currency | currencyType}}{{formData.invoice_amount}}</span>
            </div>
            <div class="item">
              <label class="label">VAT No.</label>
              <span>{{formData.vat_no}}</span>
            </div>
            <div class="item">
              <label class="label">Invoice No.</label>
              <span>{{formData.invoice_no}}</span>
            </div>
            <div class="item">
              <label class="label">Trade Terms</label>
              <span>{{formData.trade_terms}}</span>
            </div>
            <div class="item">
              <label class="label">Invoice Date</label>
              <span>{{formData.invoice_date | dateFormat}}</span>
            </div>
            <div class="item">
              <label class="label">Payment Term</label>
              <span>{{formData.payment_term}}</span>
            </div>
          </div>
        </div>
        <div class="order-info-two">
          <label class="title">Add PI</label>
          <div class="pi-left">
            <el-tag size="small" :key="tag.id" v-for="tag in dynamicTags">{{tag.pi_no}}</el-tag>
          </div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in invoiceItems">
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
          <span class="att-total">total: {{formData.currency | currencyType}}{{totalCount}}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";

import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";
import { throws } from "assert";

export default {
  data() {
    return {
      loading: false,
      isEditPage: false,
      tableHead: [
        "Model Name",
        "BF Art Nr.",
        "Customer Art Nr.",
        "Description",
        "Unit Pricee",
        "VAT rate",
        "Quantity",
        "Amount"
      ],
      orderId: 0,
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
      ],
      formData: {},
      index: 0,
      invoiceItems: [],
      dynamicTags: [],
      //   customerList: [],
      tradeTerms: Core.ConstData.TRADE_TERMS,
      paymentTerms: Core.ConstData.PAYMENT_TERMS
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
  computed: {
    //计算总金额
    totalCount: function() {
      let total = 0;
      this.invoiceItems.forEach(res => {
        total += res.amount;
      });
      return Math.round(total * 100) / 100;
    }
  },
  mounted() {
    const scope = this;
    scope.orderId = this.$route.query.orderId;
    scope.loading = true;
    scope.isEditPage = true;
    scope
      .getFinanceInvoiceById(scope.orderId)
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        return (scope.formData = res);
      })
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.formData.salesman = Core.Data.getOrg().name;
        return scope.getInvoicePIref(res.id);
      })
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.invoiceItems = res;
        return scope.getInvoiceDetail();
      })
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.dynamicTags = res.content;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err.message,
          type: "error"
        });
        scope.loading = false;
      });
    // this.loading = true;
    // this.getAllCustomer()
    //   .then(res => {
    //     if (res.code === -2) {
    //       throw res.message;
    //     }
    //     this.customerList = res;
    //     this.loading = false;
    //   })
    //   .catch(err => {
    //     this.$message({
    //       message: err,
    //       type: "error"
    //     });
    //     this.loading = false;
    //   });
  },

  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "invoiceList" });
    },
    //获取顾客信息
    // getAllCustomer() {
    //   return Core.Api.request({
    //     method: "GET",
    //     url: "/nl/nlPublic/company/all"
    //   });
    // },
    //查询发票信息
    getFinanceInvoiceById(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/finance/invoice/" + id
      });
    },
    //查询发票pi详情
    getInvoicePIref(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/finance/invoice/" + id + "/pi/detail"
      });
    },
    // 获取发票详情
    getInvoiceDetail() {
      return Core.Api.request({
        method: "post",
        url: "/nl/finance/invoice/detail/filter",
        data: {
          invoice_id: this.orderId
        },
        params: {
          page: 1,
          size: 999
        }
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.invoice-detail {
  width: 100%;
  height: 100%;

  .content {
    border: 1px solid #2b2f3b;
    width: 1080px;
    .order-info {
      width: 100%;
      height: 332px;
      border-bottom: 1px solid #2b2f3b;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 282px;
        background: #353945;
        box-shadow: 0 -1px #2b2f3b;
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
            padding-left: 10px;
          }
        }
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

    .parts-list {
      text-align: right;
      padding: 0 40px;
      font-size: 12px;
      overflow: hidden;
      height: 50px;
      line-height: 50px;
      width: 100%;
      box-shadow: 0 -1px #2b2f3b;
      position: relative;
      .att-total {
        display: inline-block;
        position: absolute;
        top: 0;
        right: 25px;
        color: #e26829;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// invoice-list-details.vue?a93a0ab2