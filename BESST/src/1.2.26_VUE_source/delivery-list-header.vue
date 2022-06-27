<template>
    <div class="delivery-list-header">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goAfterIndex" title="back"></div>
                </div>
            </div>
            <label class="title">{{ orderId ? 'Edit Delivery' : 'Add Delivery' }}</label>
        </div>
        <div class="content-wrap">
            <div class="content" v-loading.body="loading">
                <div class="order-info">
                    <label class="title">Delivery Info</label>
                    <div class="left">
                        <div class="item">
                            <label class="label">Delivery Order No.</label>
                            <el-input
                                class="input"
                                v-model="formData.delivery_order_no"
                                placeholder="Delivery Order No."
                                disabled
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Delivery Date</label>
                            <el-date-picker
                                class="input"
                                v-model="deliveryDate"
                                type="date"
                                placeholder="Delivery Date"
                            ></el-date-picker>
                        </div>
                        <div class="item">
                            <label class="label">Warehouse</label>
                            <el-select
                                class="input land"
                                v-model="formData.warehouse_id "
                                filterable
                                slot="prepend"
                            >
                                <el-option
                                    v-for="item in warehouseData"
                                    :key="item.id"
                                    :label="item.name"
                                    :value="item.id"
                                ></el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label">Customer</label>
                            <el-select
                                class="input land"
                                v-model="formData.company_id"
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
                                :disabled="true"
                                placeholder="Customer No."
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Remark Date</label>
                            <el-date-picker
                                class="input"
                                v-model="reviewerDate"
                                type="date"
                                placeholder="Remark Date"
                            ></el-date-picker>
                        </div>
                        <div class="row-textarea">
                            <label class="label">Remark</label>
                            <el-input
                                type="textarea"
                                :rows="4"
                                resize="none"
                                class="input"
                                v-model="formData.reviewer"
                                placeholder="Remark"
                            ></el-input>
                        </div>
                    </div>
                </div>
                <div class="order-info-two" v-if="isEditPage">
                    <label class="title">Add PI</label>
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
                        <el-button
                            v-else
                            class="button-new-tag"
                            size="small"
                            @click="showInput"
                        >+ New Tag</el-button>
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
                <div class="parts-list" v-if="isEditPage">
                    <span
                        class="att-total"
                    >total: {{formData.currency | currencyType}}{{ totalCount }}</span>
                </div>
                <div class="parts-list">
                    <el-button class="cancel-button" type="primary" :disabled="!isActive" @click="saveDelivery">Save</el-button>
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
        warehouse_id: "",
        total_amount: 0,
        currency: 1
      },
      deliveryDate: new Date(),
      reviewerDate: new Date(),
      invoiceItems: [],
      dynamicTags: [],
      inputVisible: false,
      restaurants: [],
      restaurantsClone: [],
      customerList: [],
      warehouseData: [],
      isActive: true
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
      const scope = this;
      this.invoiceItems.forEach(res => {
        scope.formData.currency = res.currency;
        total = Math.round((total + res.amount) * 100) / 100;
      });
      scope.formData.total_amount = total;
      return total;
    }
  },
  mounted: function() {
    const scope = this;
    this.getAllWarehouse();
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.loading = true;
      scope.isEditPage = true;
      scope
        .getDeliveryById(scope.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          if(res.status && res.status>0){
               this.isActive = false;
          }else{
              this.isActive = true;
          }
          // scope.formData = res;
          // scope.formData = JSON.parse(JSON.stringify(res));
          return (scope.formData = res);
        })
        .then(res => {
          scope.deliveryDate = Core.Util.changeDateForm(
            scope.formData.delivery_date * 1000
          );
          scope.reviewerDate = Core.Util.changeDateForm(
            scope.formData.reviewer_date * 1000
          );
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
        scope.restaurants = res
          .filter(res => {
            return res.status === 0;
          })
          .map(data => {
            data.value = data.pi_no;
            return data;
          });
        scope.restaurantsClone = res
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
      this.$router.push({ name: "deliveryList" });
    },
    //保存发货单
    saveDelivery() {
      if (!this.checkParams()) {
        return;
      }
      const scope = this;
      scope.loading = true;
      this.formData.delivery_date = Core.Util.parseDate(this.deliveryDate) || 0;
      this.formData.reviewer_date = Core.Util.parseDate(this.reviewerDate) || 0;
      this.saveDeliveryInfo(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.deliveryDate = Core.Util.changeDateForm(
            res.delivery_date * 1000
          );
          scope.reviewerDate = Core.Util.changeDateForm(
            res.reviewer_date * 1000
          );
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });

        //   scope.$router.push({
        //     name: "deliveryListHeader",
        //     query: { orderId: res.id }
        //   });
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
      this.restaurants = this.restaurantsClone.filter(res => {
        return res.customer_id === id;
      });
      this.formData.customer_no = data.company_no;
    },
    initCustomeInfo() {
      this.formData.customer_no = "";
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
    // 删除发货单详情
    deleteDeliveryDetail(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/sales/delivery/detail/" + id
      });
    },
    //删除标签
    handleClose(data) {
      const scope = this;
      this.loading = true;
      this.deleteDeliveryDetail(data.id)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.dynamicTags.splice(this.dynamicTags.indexOf(data), 1);
          return scope.getDeliveryPIref(scope.orderId);
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
            message: err.body.message,
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
        delivery_order_id: scope.orderId
      };
      scope.loading = true;
      this.bulkDeliveryDetail([data])
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
          return scope.getDeliveryPIref(scope.orderId);
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
    checkParams() {
      if (this.formData.customer_no === "") {
        this.$message({
          message: "Please add Customer No.",
          type: "warning"
        });
        return false;
      }

      if (this.formData.company_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }

      if (this.formData.reviewer === "") {
        this.$message({
          message: "Please add Reviewer",
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
.delivery-list-header {
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
        height: 310px;
        background: #353945;
        box-shadow: 0 -1px #2b2f3b;
        .row-textarea {
          overflow: hidden;
          width: 82%;
          display: inline-block;
          margin-left: 15px;
          .label {
            display: inline-block;
            line-height: 40px;
          }
        }
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

      .att-total {
        display: inline-block;
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
// delivery-list-header.vue?0913d247