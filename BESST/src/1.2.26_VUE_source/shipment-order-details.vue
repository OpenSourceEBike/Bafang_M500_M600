<template>
  <div class="shipment-order-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <div class="btn-group-top">
        <div class="icons">
          <div class="icon-item print" @click="onPrint" title="print"></div>
        </div>
      </div>
      <label class="title">Shipment Order Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Order No.</label>
              <span>{{orderItems.order_no}}</span>
            </div>
            <div class="item">
              <label class="label">Operation User</label>
              <span>{{orderItems.operation_user_id}}</span>
            </div>
            <div class="item">
              <label class="label">Shipment Type</label>
              <span>{{orderItems.shipment_type}}</span>
            </div>
            <div class="item">
              <label class="label">From Type</label>
              <span>{{orderItems.from_type_code}}</span>
            </div>
            <div class="item">
              <label class="label">Warehouse</label>
              <span>{{warehousePipe(orderItems.warehouse_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Storer</label>
              <span>{{orderItems.storer_id}}</span>
            </div>
            <div class="item">
              <label class="label">Department</label>
              <span>{{departmentPipe(orderItems.department_id)}}</span>
            </div>
            <div class="item">
              <label class="label">Customer</label>
              <span>{{orderItems.customer_id}}</span>
            </div>
            <div class="item">
              <label class="label">Expected Date</label>
              <span>{{orderItems.expected_date | dateFormat }}</span>
            </div>
            <div class="item">
              <label class="label">Order Memo</label>
              <span>{{orderItems.orderMemo}}</span>
            </div>
            <div class="item">
              <label class="label">Receiver Name</label>
              <span>{{orderItems.receiver_name}}</span>
            </div>
            <div class="item">
              <label class="label">Province</label>
              <span>{{orderItems.province}}</span>
            </div>
            <div class="item">
              <label class="label">City</label>
              <span>{{orderItems.city}}</span>
            </div>
            <div class="item">
              <label class="label">District</label>
              <span>{{orderItems.district}}</span>
            </div>
            <div class="item">
              <label class="label">Zip Code</label>
              <span>{{orderItems.zipCode}}</span>
            </div>
            <div class="item">
              <label class="label">Address</label>
              <span>{{orderItems.address}}</span>
            </div>
            <div class="item">
              <label class="label">Mobile</label>
              <span>{{orderItems.mobile}}</span>
            </div>
            <div class="item">
              <label class="label">Telephone</label>
              <span>{{orderItems.telephone}}</span>
            </div>
            <div class="item">
              <label class="label">Email</label>
              <span>{{orderItems.email}}</span>
            </div>
            <div class="item">
              <label class="label">Remark</label>
              <span>{{orderItems.operation_user_id}}</span>
            </div>
            <div class="item">
              <label class="label">Amount</label>
              <span>{{orderItems.amount}}</span>
            </div>
            <div class="item">
              <label class="label">Description</label>
              <span>{{orderItems.description}}</span>
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
                <td>{{item.quantity}}</td>
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
import saveAs from "file-saver";

export default {
  data() {
    return {
      loading: false,
      tableHead: ["Item Name", "BF Art Nr", "Specification", "Quantity"],
      orderItems: [],
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

    const id = scope.$route.query.id;
    if (id) {
      scope.loading = true;
      scope.getShipment(id).then(
        res => {
          scope.loading = false;
          scope.orderItems = res;
        },
        err => {
          scope.loading = false;
        }
      );

      scope.loading = true;
      scope.getShipmentDetails(id).then(
        res => {
          scope.loading = false;
          scope.orderDetails = res.content;
        },
        err => {
          scope.loading = false;
        }
      );
    }

    scope.loading = true;
    this.getAllWarehouse().then(res => {
      this.warehouseList = res;
      scope.loading = false;
    });

    scope.loading = true;
    this.getAllDepartment().then(res => {
      scope.loading = false;
      this.departmentList = res;
    });
  },

  methods: {
    //查询出库单
    getShipment(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id
      });
    },
    //查询出库单详情
    getShipmentDetails(id) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/detail/filter",
        data: {
          shipment_id: id
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    //获取所有部门
    getAllDepartment() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/department/all"
      });
    },
    // 打印pdf
    onPrint() {

      return Core.Api.request({
        method: "GET",
        url: "/nl/order/shipment/"+this.$route.query.id+"/print"
      })
      .then(res => {
        var now = new Date();
        var exportFileName = now.getFullYear() + "-" + now.getMonth() + "-" + now.getDay();
        var blob = new Blob([res], {type: "text/html;charset=utf-8"});
        const filename = this.orderItems.order_no || exportFileName;
        saveAs(blob, filename + ".html");
      })
    },
    warehousePipe(warehouse_id) {
      if (this.warehouseList.length > 0) {
        const data = this.warehouseList.filter(res => {
          return res.id === warehouse_id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    departmentPipe(department_id) {
      if (this.departmentList.length > 0) {
        const data = this.departmentList.filter(res => {
          return res.id === department_id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.shipment-order-details {
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
      height: 651px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 600px;
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
// shipment-order-details.vue?5e81755b