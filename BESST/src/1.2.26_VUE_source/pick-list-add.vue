<template>
  <div class="pick-list-add">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">{{ orderId ? 'Edit Pick List' : 'Add Pick List' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content">
        <div class="order-info">
          <label class="title">Pick Info</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Shipment Order No.
                <span class="required">*</span>
              </label>
              <el-select v-model="formData.shipment_order_no" filterable slot="prepend">
                <el-option
                  v-for="item in shipmentOrderData"
                  :key="item.id"
                  :label="item.shipment_order_no"
                  :value="item.shipment_order_no"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Warehouse
                <span class="required">*</span>
              </label>
              <el-select v-model="formData.warehouse_id" filterable slot="prepend">
                <el-option
                  v-for="item in warehouseData"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Customer Order
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.customer_order"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Operation User
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.operation_user_id"></el-input>
            </div>
          </div>
        </div>
        <div class="footer">
          <el-button type="primary" @click="createOrder">{{ $t('saveOrder') }}</el-button>
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
        shipment_order_no: "",
        warehouse_id: "",
        customer_order: "",
        operation_user_id: ""
      },
      orderId: 0,
      warehouseData: [],
      shipmentOrderData: []
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
  created() {},
  mounted: function() {
    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.loading = true;
      this.getPick(this.orderId).then(
        res => {
          this.formData = JSON.parse(JSON.stringify(res.content[0]));
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
    }
    this.getAllWarehouse();
    this.getShipmentOrrder();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "pickList" });
    },
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    createOrder() {
      if (!this.required()) {
        return;
      }
      this.loading = true;
      this.postPick(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          this.$router.push({
            name: "pickListAdd",
            query: { orderId: res.id }
          });
          this.orderId = res.id;
          this.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 获取出货数据
    getShipmentOrrder() {
      this.loading = true;
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/filter",
        data: {
          status: 1
        },
        params: {
          size: 9999
        }
      }).then(
        res => {
          this.shipmentOrderData = res.content;
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
    },
    //获取所有仓库
    getAllWarehouse() {
      this.loading = true;
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.warehouseData = res;
          this.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 获取拣货单
    getPick(id) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/pick/filter",
        data: {
          id: id
        }
      });
    },
    // 新增api
    postPick(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/pick",
        data: data
      });
    },
    // 弹窗必填
    required() {
      for (const item in this.formData) {
        if (item === "update_user_id" || item === "update_time") {
          continue;
        }
        if (
          this.formData[item] === "" ||
          this.formData[item] == null ||
          this.formData[item] == undefined
        ) {
          this.$message({
            message: "Please input " + item,
            type: "warning"
          });
          return false;
        }
      }
      return true;
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.pick-list-add {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .el-select {
    margin-left: 10px;
    width: 256px;
  }

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
      height: 272px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 221px;
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
            width: 140px;
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

    .parts-list {
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
      text-align: right;
      padding-right: 40px;
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
// pick-list-add.vue?2bcfd413