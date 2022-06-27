<template>
  <div class="cashier-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">cashier Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">cashier Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Customer</label>
              <span>{{conversionCustomer(formData.customer_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Date</label>
              <span>{{formData.date | dateFormat}}</span>
            </div>
            <div class="item">
              <label class="label">Bank</label>
              <span>{{formData.account_no}}</span>
            </div>
            <div class="item">
              <label class="label">Type</label>
              <span>{{conversionType(formData.type)}}</span>
            </div>
            <div class="item">
              <label class="label">Amount</label>
              <span>{{formData.currency |currencyType}}{{formData.amount}}</span>
            </div>
            <div class="item">
              <label class="label">Operator</label>
              <span>{{formData.operator}}</span>
            </div>
            <div class="item note">
              <label class="label">Note</label>
              <p>{{formData.note }}</p>
            </div>
          </div>
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

export default {
  data() {
    return {
      formData: {
        customer_id: "",
        currency: 1,
        account_no: "",
        type: "",
        amount: "",
        note: "",
        operator: "",
        data: "",
      },
      typeList: [
        {
          label: "Receipt",
          value: 0,
          id: 0
        },
        {
          label: "Payment",
          value: 1,
          id: 1
        }
      ],
      customerData: [],
      loading: false
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
  computed: {},
  mounted() {
    this.loading = true;
    this.getcashierById(this.$route.query.id)
      .then(res => {
        if (res.code === -2) {
          throw res.messages;
        }
        this.formData = res;
      })
      .then(() => {
        return this.getAllCustomer();
      })
      .then(res => {
        this.loading = false;
        if (res.code === -2) {
          throw res.messages;
        }
        this.customerData = res;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });
  },

  methods: {
    getcashierById(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/finance/account/transaction/" + id
      });
    },
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    conversionCustomer(id) {
      if (this.customerData.length > 0) {
        const data = this.customerData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        }
      }
    },
    conversionType(id) {
      if (this.typeList.length > 0) {
        const data = this.typeList.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].label;
        }
      }
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.cashier-details {
  width: 100%;
  height: 100%;

  .table-wrap {
    width: 100%;
    height: calc(100% - 392px);
    box-shadow: 0 -1px #2b2f3b;
    overflow-x: hidden;
  }

  span.required {
    color: #e26829;
    display: inline-block;
  }
  .content {
    border: 1px solid #2b2f3b;
    .order-info {
      width: 100%;
      height: 350px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 300px;
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
          }
        }
        .note {
          width: 100%;
          height: 100px;
          display: flex;
          p {
            line-height: 28px;
            padding-top: 16px;
            padding-right: 15px;
            width: 100%;
          }
        }
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// cashier-details.vue?7887c447