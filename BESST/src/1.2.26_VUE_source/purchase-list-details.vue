<template>
  <div class="purchase-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goPurchaseList" title="back"></div>
        </div>
      </div>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item print" @click="onPrint" title="print"></div>
        </div>
      </div>
      <label class="title">Purchase List Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Purchase Order</label>
          <div class="left">
            <div class="item">
              <label class="label">Object</label>
              <span>{{conversionSupplier(formData.supplier_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Order No.</label>
              <span>{{formData.purchase_order_no}}</span>
            </div>
            <div class="item">
              <label class="label">Purpose</label>
              <span>{{conversionPurpose(formData.purchase_type)}}</span>
            </div>
            <div class="item">
              <label class="label">Creater</label>
              <span>{{formData.create_user_id}}</span>
            </div>
            <div class="item">
              <label class="label">Date</label>
              <span>{{changeDate(formData.date)}}</span>
            </div>
          </div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in headData">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.item_part_no}}</td>
                <td>{{item.name || item.item_name}}</td>
                <td>{{item.quantity}}</td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  </div>
</template>;

<script>
import Core from "core/core";
import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

import PurchaseService from "src/services/purchaseService";
import saveAs from "file-saver";

export default {
  data() {
    return {
      loading: false,
      headData: [],
      orderId: 0,
      formData: {
        date: "",
        supplier_id: "",
        purchase_type: "",
        create_user_id: "",
        purchase_order_no: ""
      },
      orderItems: [],
      purpose: [
        {
          id: 0,
          name: "Warehouse stocking"
        },
        {
          id: 1,
          name: "Sales purchase"
        },
        {
          id: 2,
          name: "After-sales procurement"
        }
      ],
      supplierData: []
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
  computed: {},
  mounted() {
    this.headData = ["Item Name", "BF Art Nr.", "Specification", "Quantity"];
    const scope = this;
    scope.orderId = this.$route.query.orderId;
    scope.loading = true;
    scope
      .getPurchaseById(scope.orderId)
      .then(res => {
        scope.formData = JSON.parse(JSON.stringify(res));
      })
      .then(() => {
        return scope.filterPurchaseDetails(scope.orderId);
      })
      .then(res => {
        scope.orderItems = res.content.map(data => {
          data.item_part_no = data.item_code;
          return data;
        });
      })
      .then(() => {
        return scope.getSupplier();
      })
      .then(res => {
        scope.loading = false;
        scope.supplierData = res;
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
    goPurchaseList() {
      this.$router.push({ name: "purchaseList" });
    },

    // 根据id获取采购单详情
    getPurchaseById(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/order/purchase/" + id
      });
    },
    // 添加采购单详情
    bulkPurchaseDetails(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/detail/bulk",
        data: data
      });
    },
    // 添加采购信息
    postPurchase(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase",
        data: data
      });
    },
    //获取供货商
    getSupplier() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/supplier/all"
      });
    },
    //筛选采购单详情
    filterPurchaseDetails(id) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/detail/filter",
        data: {
          purchase_order_id: id
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //转换时间
    changeDate(data) {
      return Core.Util.changeDateForm(data * 1000);
    },
    // 转换供应商
    conversionSupplier(id) {
      if (this.supplierData.length > 0) {
        const data = this.supplierData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].company_name;
        } else {
          return "--";
        }
      }
    },
    conversionPurpose(id) {
      if (this.purpose.length > 0) {
        const data = this.purpose.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        }
      }
    },
    // 打印pdf
    onPrint() {
      PurchaseService.printPurchase(this.orderId).then(res => {
        var now = new Date();
        var exportFileName =
          now.getFullYear() + "-" + now.getMonth() + "-" + now.getDay();
        const filename = this.formData.purchase_order_no || exportFileName;
        var blob = new Blob([res.data], { type: "text/html;charset=utf-8" });
        saveAs(blob, filename + ".html");
      });
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.purchase-details {
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
  .content {
    border: 1px solid #2b2f3b;
    width: 1080px;
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
        height: 175px;
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
          .input {
            margin-left: 10px;
            width: 256px;
            border-radius: 0 0 10px 10px;
          }
          span {
            display: inline-block;
          }
        }
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// purchase-list-details.vue?87a8d8f6