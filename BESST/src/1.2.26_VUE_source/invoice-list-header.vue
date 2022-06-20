<template>
  <div class="invoice-header">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">{{ orderId ? 'Edit Invoice Info' : 'Add Invoice Info' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Invoice Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Invoice No.</label>
              <el-input
                class="input"
                v-model="formData.invoice_no"
                placeholder="Invoice No."
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Customer
                <span class="required">*</span>
              </label>
              <el-select
                class="input land"
                v-model="formData.customer_id "
                @change="handleItemChange"
                filterable
                slot="prepend"
              >
                <el-option
                  v-for="item in customerList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Customer No.</label>
              <el-input
                class="input"
                v-model="formData.customer_no"
                placeholder="Customer No."
                disabled
              ></el-input>
            </div>
            <!-- <div class="item">
              <label class="label">Salesman</label>
              <el-input class="input" v-model="formData.salesman" placeholder="Salesman" disabled></el-input>
            </div>-->
            <div class="item">
              <label class="label">VAT No.</label>
              <el-input class="input" v-model="formData.vat_no" placeholder="VAT No." disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Trade Terms</label>
              <el-select
                v-model="formData.trade_terms"
                slot="append"
                class="input"
                filterable
                placeholder="Trade Terms"
                disabled
              >
                <el-option
                  v-for="item in tradeTerms"
                  :key="item.id"
                  :label="item.label"
                  :value="item.label"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Payment Term</label>
              <el-select
                v-model="formData.payment_term"
                slot="append"
                class="input"
                filterable
                placeholder="Payment Term"
                disabled
              >
                <el-option
                  v-for="item in paymentTerms"
                  :key="item.id"
                  :label="item.label"
                  :value="item.label"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Currency/Amount
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.invoice_amount" disabled>
                <el-select v-model="formData.currency" slot="prepend" disabled>
                  <el-option
                    v-for="item in currencies"
                    :key="item.currency"
                    :label="item.label"
                    :value="item.currency"
                    :value-key="item.currency"
                  ></el-option>
                </el-select>
              </el-input>
            </div>
            <div class="item">
              <label class="label">
                Invoice Date
                <span class="required">*</span>
              </label>
              <el-date-picker
                class="input"
                v-model="invoiceDate"
                type="date"
                placeholder="Invoice Date"
              ></el-date-picker>
            </div>
          </div>
        </div>
        <div class="order-info-two" v-if="isEditPage">
          <label class="title">
            Add PI
            <span class="required">*</span>
          </label>
          <div class="pi-left">
            <el-tag
              size="small"
              :key="tag.id"
              v-for="tag in dynamicTags"
              :closable="true"
              :close-transition="false"
              @close="handleClose(tag)"
            >{{tag.pi_no}}</el-tag>
            <el-autocomplete
              class="input-new-tag"
              v-if="inputVisible"
              ref="saveTagInput"
              size="small"
              :fetch-suggestions="querySearchAsync"
              placeholder="Please enter the content"
              @select="handleInputConfirm"
            ></el-autocomplete>
            <el-button v-else class="button-new-tag" size="small" @click="showInput">+ New Tag</el-button>
          </div>
        </div>
        <div class="table-wrap" v-if="isEditPage">
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
        <div class="parts-list" v-if="isEditPage">
          <span class="att-total">total: {{currency | currencyType}}{{totalCount}}</span>
        </div>
        <div class="parts-list">
          <el-button class="cancel-button" type="primary" @click="saveInvoice">Save</el-button>
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
        "Unit Price",
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
      formData: {
        customer_id: "",
        vat_no: "",
        customer_no: "",
        trade_terms: "",
        payment_term: "",
        invoice_no: "",
        currency: 1,
        invoice_amount: 0,
        invoice_date: new Date()
      },
      invoiceDate: new Date(),
      index: 0,
      invoiceItems: [],
      dynamicTags: [],
      inputVisible: false,
      timeout: null,
      restaurants: [],
      customerList: [],
      tradeTerms: Core.ConstData.TRADE_TERMS,
      paymentTerms: Core.ConstData.PAYMENT_TERMS,
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
  computed: {
    //计算总金额
    totalCount: function() {
      let total = 0;
      this.invoiceItems.forEach(res => {
        this.formData.currency = res.currency;
        this.currency = res.currency;
        const num = res.quantity * res.unit_price * (1 + res.vat_rate / 100);
        res.amount = Math.round(num * 100) / 100;
        total = ((total + res.amount) * 100) / 100;
        this.formData.invoice_amount = total;
      });
      return total;
    }
  },
  mounted: function() {
    const scope = this;
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
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
    }
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
        this.loading = false;
      });

    scope
      .getPiList()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.restaurants = res.content
          .filter(res => {
            return res.status === 0;
          })
          .map(data => {
            data.value = data.pi_no;
            return data;
          });
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
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "invoiceList" });
    },
    //保存发票
    saveInvoice() {
      this.formData.invoice_date = Core.Util.parseDate(this.invoiceDate) || 0;
      if (!this.checkParams()) return;
      const scope = this;
      scope.loading = true;
      this.saveFinanceInvoice(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.formData = JSON.parse(JSON.stringify(res));
          this.invoiceDate = Core.Util.changeDateForm(res.invoice_date * 1000);
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });

          scope.$router.push({
            name: "invoiceListHeader",
            query: { orderId: res.id }
          });

          scope.orderId = res.id;
          scope.isEditPage = true;
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
    //顾客信息联动
    handleItemChange(id) {
      if (this.customerList.length === 0) return;
      const data = this.customerList.find(res => {
        return res.id === id;
      });
      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.vat_no = data.vat_no;
      this.formData.trade_terms = data.trade_term;
      this.formData.payment_term = data.payment_term;
      this.formData.customer_no = data.company_no;
    },
    initCustomeInfo() {
      this.formData.vat_no = "";
      this.formData.trade_terms = "";
      this.formData.payment_term = "";
      this.formData.customer_no = "";
    },
    //获取顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    //查询发票信息
    getFinanceInvoiceById(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/finance/invoice/" + id
      });
    },
    //保存发票信息
    saveFinanceInvoice(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/finance/invoice",
        data: data
      });
    },
    //查询发票pi详情
    getInvoicePIref(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/finance/invoice/" + id + "/pi/detail"
      });
    },
    //保存发票明细列表
    bulkInvoiceDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/finance/invoice/detail/bulk",
        data: data
      });
    },
    //获取pi
    getPiList() {
      return Core.Api.request({
        method: "get",
        url: "/nl/sales/proformaInvoice",
        params: {
          page: 1,
          size: 999
        }
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
    },
    // 删除发票详情
    deleteInvoiceDetail(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/finance/invoice/detail/" + id
      });
    },
    //删除标签
    handleClose(data) {
      const scope = this;
      this.loading = true;
      this.deleteInvoiceDetail(data.id)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.dynamicTags.splice(scope.dynamicTags.indexOf(data), 1);
          return scope.getInvoicePIref(scope.orderId);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.invoiceItems = res;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err.message,
            type: "error"
          });
          scope.loading = false;
          scope.inputVisible = false;
        });
    },
    //显示输入搜索框
    showInput() {
      this.inputVisible = true;
    },
    //输入搜索框确认
    handleInputConfirm(item) {
      const scope = this;
      const data = {
        pi_no: item.pi_no,
        customer_id: item.customer_id,
        invoice_id: scope.orderId
      };
      scope.loading = true;
      this.bulkInvoiceDetail([data])
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.dynamicTags.push(res[0]);
          scope.inputVisible = false;
          return scope.getInvoicePIref(scope.orderId);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.invoiceItems = res;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err.message,
            type: "error"
          });
          scope.loading = false;
          scope.inputVisible = false;
        });
    },
    // 远程搜索
    querySearchAsync(queryString, cb) {
      let arr = JSON.parse(JSON.stringify(this.restaurants));
      if (this.dynamicTags.length > 0) {
        this.dynamicTags.forEach((item, i) => {
          arr.forEach((res, j) => {
            if (res.pi_no === item.pi_no) {
              arr.splice(j, 1);
            }
          });
        });
      }
      var restaurants = arr;
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
    // 弹窗必填
    checkParams() {
      if (this.formData.customer_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }

      if (this.formData.invoice_amount === "") {
        this.$message({
          message: "Please add Amount",
          type: "warning"
        });
        return false;
      }

      if (this.formData.invoice_date === "") {
        this.$message({
          message: "Please add Invoice Date",
          type: "warning"
        });
        return false;
      }

      return true;
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.invoice-header {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

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
            width: 125px;
            margin-left: 15px;
          }
          .input {
            margin-left: 10px;
            width: 250px;
            border-radius: 0 0 10px 10px;
          }
          .input-left {
            margin-left: 10px;
            width: 200px;
          }
          .input-right {
            width: 200px;
          }
          .input-note {
            margin-left: 10px;
            width: 608px;
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
      .att-btn {
        padding: 6px 9px;
        min-width: 0;
        margin: 11px auto;
        display: block;
      }
      .att-total {
        display: inline-block;
        position: absolute;
        top: 0;
        right: 25px;
        color: #e26829;
      }
      .labelAdd {
        margin-left: 14px;
      }
      .cancel-button {
        margin-top: 12px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// invoice-list-header.vue?75906d29