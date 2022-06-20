<template>
  <div class="receipt-order-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">Receipt Order Details</label>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item print" @click="onPrint" title="print"></div>
        </div>
      </div>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Warehouse</label>
              <span>{{formData.warehouse_name}}</span>
            </div>
            <div class="item">
              <label class="label">Supplier</label>
              <span>{{formData.supplier_name}}</span>
            </div>
            <div class="item">
              <label class="label">Source Type</label>
              <span>{{formData.from_type_code}}</span>
            </div>
            <div class="item">
              <label class="label">Order No.</label>
              <span>{{formData.receipt_order_no}}</span>
            </div>
            <div class="item">
              <label class="label">Receipt Date</label>
              <span>{{formData.receipt_date | dateFormat }}</span>
            </div>
            <div class="item">
              <label class="label">Description</label>
              <span>{{formData.description}}</span>
            </div>
          </div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderDetails">
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.item_code}}</td>
                <td>{{item.item_name}}</td>
                <td>{{item.receipt_quantity}}</td>
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
import StockInService from "src/services/stockInservice";
import saveAs from "file-saver";

export default {
  data() {
    return {
      loading: false,
      tableHead: ["Item Name", "BF Art Nr", "Specification", "Quantity"],
      formData: [],
      orderDetails: [],
      departmentList: [],
      warehouseList: []
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

    const id = scope.$route.query.orderId;
    if (id) {
      scope.loading = true;
      scope.getReceipt(id).then(
        res => {
          scope.loading = false;
          scope.formData = res;
        },
        err => {
          scope.loading = false;
        }
      );

      scope.loading = true;
      scope.getReceiptOrderDetails(id).then(
        res => {
          scope.loading = false;
          scope.orderDetails = res.content;
        },
        err => {
          scope.loading = false;
        }
      );
    }
  },

  methods: {
    //查询出库单
    getReceipt(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id
      });
    },
    //查询出库单详情
    getReceiptOrderDetails(id) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/receipt/detail/filter",
        data: {
          receipt_order_id: id
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    onPrint() {
      let id = this.$route.query.orderId;
      StockInService.printReceipt(id).then(res => {
        var blob = new Blob([res.data], { type: "text/html;charset=utf-8" });
        var now = new Date();
        var exportFileName =
          now.getFullYear() + "-" + now.getMonth() + "-" + now.getDay();
        const filename = this.formData.receipt_order_no || exportFileName;
        saveAs(blob, filename + ".html");
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.receipt-order-details {
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
      height: 332px;
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
    }
    .footer {
      width: 100%;
      height: 60px;
      line-height: 60px;
      padding-left: 40px;
      box-shadow: 0 -1px #2b2f3b;
      .footer-pagination {
        float: right;
      }
    }
  }
  .dialog {
    .el-dialog--large {
      width: 1100px;
    }
    .header {
      .search-content {
        width: 200px;
        margin-right: 10px;
      }
      .search-btn {
        position: absolute;
        top: 15px;
        left: 250px;
        width: 30px;
        height: 30px;
      }
    }
  }

  .table-wrap .table-content {
    padding-bottom: 0;
  }
  .pagination {
    padding: 10px;
  }
}
</style>



// WEBPACK FOOTER //
// receipt-order-details.vue?6b21827b