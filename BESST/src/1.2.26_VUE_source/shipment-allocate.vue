<template>
  <div class="shipment-allocate">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">Shipment Allocate</label>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content content-set">
        <div class="parts-list" v-if="orderItems.length <= 0 && !loading">
          <div class="no-data-content">Insufficient stock, procurement or not</div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.item_code}}</td>
                <td>{{item.item_name}}</td>
                <td>{{item.allocated_quantity}}</td>
                <td>{{item.picked_quantity}}</td>
                <td>{{item.shipped_quantity}}</td>
                <td>{{item.location_no || 'Out of stock'}}</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="footer" v-if="isAdvice">
          <el-button type="primary" v-if="noLocation" @click="saveAllocate">Save Allocate</el-button>
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
      loading: false,
      tableHead: [
        "Item Name",
        "Item Code",
        "Specification",
        "Allocated Quantity",
        "Picked Quantity",
        "Shipped Quantity",
        "Location No."
      ],
      orderItems: [],
      orderId: 0,
      warehouseList: [],
      isAdvice: false,
      noLocation: false
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

    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.loading = true;
      scope
        .getShipmentStatus(scope.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          if (res.status_name === "Approved") {
            scope.isAdvice = true;
            return scope.getAllocateAdvice(scope.orderId);
          } else {
            scope.isAdvice = false;
            return scope.getAllocate(scope.orderId);
          }
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.orderItems = res;
          scope.noLocation = this.checkLocation(res);
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

  methods: {
    //判断库位货物是否充足
    checkLocation(list) {
      const data = list.filter(res => {
        return res.location_no === null;
      });
      if(data.length === 0) {
        return true;
      }else {
        return false;
      }
    },
    saveAllocate() {
      const scope = this;
      const id = this.$route.query.orderId;
      const data = this.orderItems;
      scope.loading = true;
      scope
        .postAllocate(id, data)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.isAdvice = false;
          return scope.getAllocate(id);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.orderItems = res;
          scope.noLocation = this.checkLocation(res);
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
    //保存出库分配
    postAllocate(id, data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/" + id + "/allocate/advice",
        data: data
      });
    },
    //创建出库分配
    getAllocateAdvice(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id + "/allocate/advice"
      });
    },
    //获取状态
    getShipmentStatus(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id + "/status"
      });
    },
    getAllocate(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id + "/allocate"
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.shipment-allocate {
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
  .content-set {
    width: 1280px;
  }
  .content-wrap {
    .content {
      border: 1px solid #2b2f3b;
      .parts-list {
        position: relative;
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
        .no-data-content {
          display: inline-block;
        }
        .btn-box {
          display: inline-block;
          text-align: right;
          margin-left: 15px;
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
  }

  .table-wrap .table-content {
    padding-bottom: 0;
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
}
</style>



// WEBPACK FOOTER //
// shipment-allocate.vue?843d9476