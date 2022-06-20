<template>
  <div class="delivery-list-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item print" @click="onPrint" title="print"></div>
        </div>
      </div>
      <label class="title">Delivery Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Delivery Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Delivery Order No.</label>
              <span>{{formData.delivery_order_no}}</span>
            </div>
            <div class="item">
              <label class="label">Delivery Date</label>
              <span>{{formData.deliveryDate | dateFormat}}</span>
            </div>
            <div class="item">
              <label class="label">Warehouse</label>
              <span>{{pipeWarehouse(formData.warehouse_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Customer</label>
              <span>{{pipeCustomer(formData.company_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Customer No.</label>
              <span>{{formData.customer_no}}</span>
            </div>
            <div class="item">
              <label class="label">Reviewer</label>
              <span>{{formData.reviewer}}</span>
            </div>
            <div class="item">
              <label class="label">Reviewer Date</label>
              <span>{{formData.reviewerDate | dateFormat}}</span>
            </div>
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
                <td>{{item.vat_rate}}</td>
                <td>{{item.quantity}}</td>
                <td>{{item.amount}}</td>
                <td v-if="item.status == 0">- -</td>
                <td v-if="item.status == 1">confirm</td>
                <td v-if="item.status == 2">outOfStock</td>
              </tr>
            </tbody>
          </table>
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

import DeliveryService from "src/services/deliveryService";
import saveAs from "file-saver";

export default {
  data() {
    return {
      loading: false,
      tableHead: [
        "Model Name",
        "BF Art Nr.",
        "Customer Art Nr.",
        "Description",
        "Unit Pricee",
        "VAT rate",
        "Quantity",
        "Amount",
        "Status"
      ],
      orderId: 0,
      formData: {
        delivery_order_no: "",
        customer_no: "",
        creator: "",
        reviewer: "",
        order_no: "",
        company_id: "",
        reviewer_date: new Date(),
        delivery_date: new Date(),
        warehouse_id: ""
      },
      invoiceItems: [],
      dynamicTags: [],
      inputVisible: false,
      customerList: [],
      warehouseData: []
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
  mounted: function() {
    const scope = this;
    this.getAllWarehouse();
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
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.loading = true;
      scope
        .getDeliveryById(scope.orderId)
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
          return scope.getDeliveryPIref(res.id);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.invoiceItems = res;
          return scope.getDeliveryDetail();
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
  },

  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "deliveryList" });
    },
    //获取仓库
    getAllWarehouse() {
      this.loading = true;
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      })
        .then(res => {
          this.loading = false;
          this.warehouseData = res;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    //获取顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    //查询发货单信息
    getDeliveryById(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/sales/delivery/" + id
      });
    },
    //保存发货单信息
    saveDeliveryInfo(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/sales/delivery",
        data: data
      });
    },
    //查询发货单pi详情
    getDeliveryPIref(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/sales/delivery/" + id + "/pi/detail"
      });
    },
    //保存发货单明细列表
    bulkDeliveryDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/sales/delivery/detail/bulk",
        data: data
      });
    },
    //获取pi
    getPiList() {
      return Core.Api.request({
        method: "get",
        url: "/nl/sales/proformaInvoice/all"
      });
    },
    // 获取发货单详情
    getDeliveryDetail() {
      return Core.Api.request({
        method: "post",
        url: "/nl/sales/delivery/detail/filter",
        data: {
          delivery_order_id: this.orderId
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    pipeWarehouse(id) {
      if (this.warehouseData.length === 0) return;
      const data = this.warehouseData.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    pipeCustomer(id) {
      if (this.customerList.length === 0) return;
      const data = this.customerList.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    // 打印pdf
    onPrint() {
      DeliveryService.printDelivery(this.orderId).then(res => {
        var now = new Date();
        var exportFileName =
          now.getFullYear() + "-" + now.getMonth() + "-" + now.getDay();
        const filename = this.formData.delivery_order_no || exportFileName;
        var blob = new Blob([res.data], { type: "text/html;charset=utf-8" });
        saveAs(blob, filename + ".html");
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.delivery-list-details {
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
            width: 120px;
            margin-left: 15px;
          }
          .input {
            margin-left: 10px;
            width: 256px;
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

      .att-btn {
        padding: 6px 9px;
        min-width: 0;
        margin: 11px auto;
        display: block;
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
// delivery-list-details.vue?9fbba35a