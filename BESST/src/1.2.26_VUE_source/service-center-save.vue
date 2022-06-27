<template>
    <div class="service-center-save">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goAfterIndex" title="back"></div>
                </div>
            </div>
            <label class="title">{{ orderId ? 'Edit Service Center' : 'Add Service Center' }}</label>
        </div>
        <div class="content-wrap">
            <div class="content" v-loading.body="loading">
                <div class="order-info">
                    <label class="title">Customer Info</label>
                    <div class="left">
                        <div class="item">
                            <label class="label">
                                Customer
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.customer_id"
                                @change="handleItemChange"
                                filterable
                                slot="prepend"
                                :disabled="isEditPage"
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
                            <label class="label">
                                Sales Order No.
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.sales_order_no"
                                @change="handleSalesChange"
                                filterable
                                placeholder="Sales Order No."
                                slot="prepend"
                                :disabled="isEditPage"
                            >
                                <el-option
                                    v-for="item in PIList"
                                    :key="item.id"
                                    :label="item.pi_no"
                                    :value="item.pi_no"
                                ></el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label">Customer Name</label>
                            <el-input class="input" v-model="formData.customer_name" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Customer No.</label>
                            <el-input class="input" v-model="formData.customer_no" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Contact User</label>
                            <el-input
                                class="input"
                                v-model="formData.customer_contact_user"
                                disabled
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Contact Phone</label>
                            <el-input
                                class="input"
                                v-model="formData.customer_contact_phone"
                                disabled
                            >
                                <el-select
                                    v-model="formData.customer_contact_country_code"
                                    filterable
                                    slot="prepend"
                                    disabled
                                >
                                    <el-option
                                        v-for="item in areaCodeList"
                                        :key="item.area_code"
                                        :label="item.area_code"
                                        :value="item.area_code"
                                    ></el-option>
                                </el-select>
                            </el-input>
                        </div>
                        <div class="item">
                            <label class="label">Country</label>
                            <el-input class="input" v-model="formData.customer_country" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Province</label>
                            <el-input class="input" v-model="formData.customer_province" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">City</label>
                            <el-input class="input" v-model="formData.customer_city" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">District</label>
                            <el-input class="input" v-model="formData.customer_district" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Address</label>
                            <el-input class="input" v-model="formData.customer_address" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Star</label>
                            <el-input class="input" v-model="formData.customer_star" disabled></el-input>
                        </div>
                        <div class="item-row">
                            <div class="text-area">
                                <el-input
                                    type="textarea"
                                    v-model="formData.problem_description"
                                    placeholder="Problem Description"
                                    :rows="4"
                                    :maxlength="250"
                                ></el-input>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="order-info">
                    <label class="title">Sales Oreder Info</label>
                    <div class="left">
                        <div class="item-row">
                            <label class="label">
                                Category
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input category-input"
                                v-model="categoryList"
                                value-key="id"
                                multiple
                                placeholder="SELECT"
                                :disabled="isEditPage"
                            >
                                <el-option
                                    v-for="item in PIDetailList"
                                    :key="item.id"
                                    :label="item.bf_art_no"
                                    :value="item"
                                >
                                    <span
                                        style="float: left;color: #fff; font-size: 13px"
                                    >{{ item.bf_art_no }}</span>
                                    <span
                                        style="float: right; color: #8492a6; font-size: 12px"
                                    >{{ item.category_root_id | categoryRoot }}</span>
                                </el-option>
                            </el-select>
                        </div>
                    </div>
                </div>
                <div class="table-wrap table-overflow" v-if="categoryList.length>0">
                    <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                        <thead>
                            <td v-for="(item, index) in headData" :key="index">{{item}}</td>
                        </thead>
                        <tbody>
                            <tr v-for="(item, index) in categoryList" :key="index">
                                <td>{{item.category_root_id | categoryRoot}}</td>
                                <td>{{item.bf_art_no}}</td>
                                <td>{{item.customer_art_no}}</td>
                                <td>{{item.description}}</td>
                                <td>{{item.currency}}{{item.unit_price}}</td>
                                <td>{{item.vat_rate}}%</td>
                                <td>{{item.quantity}}</td>
                                <td>{{item.amount}}</td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div class="order-info">
                    <label class="title">Service Center Oreder Info</label>
                    <div class="left">
                        <div class="item">
                            <label class="label">
                                Order No.
                                <span class="required">*</span>
                            </label>
                            <el-input
                                class="input"
                                v-model="formData.order_no"
                                placeholder="Sales Order No."
                                disabled
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                Priority
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.priority"
                                filterable
                                slot="prepend"
                                :disabled="isEditPage"
                            >
                                <el-option
                                    v-for="item in priorityArr"
                                    :key="item.id"
                                    :label="item.label"
                                    :value="item.id"
                                ></el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label">
                                Type
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.type"
                                filterable
                                slot="prepend"
                                :disabled="isEditPage"
                            >
                                <el-option
                                    v-for="item in typeArr"
                                    :key="item.id"
                                    :label="item.label"
                                    :value="item.id"
                                ></el-option>
                            </el-select>
                        </div>
                    </div>
                </div>
                <div class="parts-list" v-if="CSRecordData && CSRecordData.id">
                    <a @click="goToCSRecord()">Customer Service Record Link</a>
                </div>
                <div class="parts-list">
                    <el-button type="primary" @click="createOrder" v-if="!isEditPage">SAVE</el-button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";
import AfterSalesService from "src/services/afterSalesService";
import WarehouseService from "src/services/warehouseService";
import CustomerServiceRecordService from "src/services/customerServiceRecordService";
import PIService from "src/services/PIService";

import EN from "src/assets/lang/en";
import ZH from "src/assets/lang/zh";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      isEditPage: false,
      formData: {
        sales_order_no: "",
        customer_id: "",
        customer_name: "",
        customer_no: "",
        customer_country: "",
        customer_province: "",
        customer_city: "",
        customer_district: "",
        customer_address: "",
        customer_star: "",
        customer_contact_country_code: "+86",
        customer_contact_user: "",
        customer_contact_phone: "",
        order_no: "",
        priority: "",
        type: "",
        status: "",
        problem_description: "",
        create_user_id: Core.Data.getUser() ? Core.Data.getUser().id : ""
      },
      areaCodeList: [],
      orderId: 0,
      customerList: [],
      //PI Arr
      PIList: [],
      PIDetailList: [],
      categoryList: [],
      priorityArr: [
        {
          id: 1,
          label: "normal"
        },
        {
          id: 2,
          label: "popular"
        },
        {
          id: 3,
          label: "expedited"
        }
      ],
      typeArr: [
        {
          id: 1,
          label: "inspection"
        },
        {
          id: 2,
          label: "repair"
        },
        {
          id: 3,
          label: "replacement"
        }
      ],
      CSRecordData: {},
      headData: [],
      CSDetail: []
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
  async created() {
    this.headData = [
      "Item Name",
      "BF Art No.",
      "Customer Art No.",
      "Description",
      "Unit Price",
      "VAT rate",
      "Quantity",
      "Amount"
    ];
    const scope = this;
    this.areaCodeList = areaCodeInDB.init();

    this.loading = true;
    await WarehouseService.getAllCustomer()
      .then(res => {
        if (res.data.code === -2) {
          throw res.data.message;
        }
        scope.customerList = res.data;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.isEditPage = true;
      this.loading = true;
      let _formData = {};
      await AfterSalesService.getServiceCenter(this.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          _formData = JSON.parse(JSON.stringify(res.data));
          const search = {
            customer_id: res.data.customer_id,
            service_no: res.data.order_no
          };
          scope.loadCSrecord(search);
          return AfterSalesService.getServiceCenterDetail(res.data.order_no);
        })
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = _formData;
          scope.CSDetail = res.data.content;
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
  mounted() {},
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "serviceCenter" });
    },
    goToCSRecord() {
      this.$router.push({
        name: "customerServiceRecordSave",
        query: { orderId: this.CSRecordData.id }
      });
    },
    createOrder() {
      const scope = this;
      if (!this.checkParams()) {
        return;
      }

      this.loading = true;
      AfterSalesService.postServiceCenter(this.formData)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          const search = {
            customer_id: res.data.customer_id,
            service_no: res.data.order_no
          };
          scope.loadCSrecord(search);
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.$router.push({
            name: "serviceCenterSave",
            query: { orderId: res.data.id }
          });
          scope.orderId = res.data.id;
          scope.isEditPage = true;
          return scope.bulkServiceCenterDetail(res.data.order_no);
        })
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.$router.go(-1);
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
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

      if (this.formData.sales_order_no === "") {
        this.$message({
          message: "Please add Sales Order No.",
          type: "warning"
        });
        return false;
      }

      if (this.categoryList.length === 0) {
        this.$message({
          message: "Please add Category",
          type: "warning"
        });
        return false;
      }

      if (this.formData.priority === "") {
        this.$message({
          message: "Please add Priority",
          type: "warning"
        });
        return false;
      }

      if (this.formData.type === "") {
        this.$message({
          message: "Please add type",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    //顾客信息联动
    handleItemChange(id) {
      if (this.isEditPage) {
        return;
      }
      if (!id || id === "") return;
      let data = this.customerList.find(res => {
        return res.id === id;
      });
      this.filterCustomerPI(id);
      this.formData.customer_name = (data && data.name) || "";
      this.formData.customer_no = (data && data.company_no) || "";
      this.formData.customer_country = (data && data.country) || "";
      this.formData.customer_province = (data && data.province) || "";
      this.formData.customer_city = (data && data.city) || "";
      this.formData.customer_district = (data && data.district) || "";
      this.formData.customer_address = (data && data.address) || "";
      this.formData.customer_contact_country_code =
        (data && data.country_code) || "+86";
      this.formData.customer_contact_user = (data && data.receiver_name) || "";
      this.formData.customer_contact_phone = (data && data.mobile) || "";
      this.formData.customer_star = (data && data.star) || 5;
    },
    handleSalesChange(pi_no) {
      if (!pi_no || pi_no === "") return;
      const scope = this;
      this.loading = true;
      this.categoryList = [];
      PIService.filterCustomerPIDetail(pi_no)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.PIDetailList = res.data.content;
          scope.CSDetail.forEach(csd => {
            scope.PIDetailList.forEach(pid => {
              if (pid.item_id === csd.item_id) {
                scope.categoryList.push(pid);
              }
            });
          });
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    loadCSrecord(search) {
      const scope = this;
      this.loading = true;
      CustomerServiceRecordService.filterCustomerServiceRecord(1, search)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.CSRecordData = (res.data.content && res.data.content[0]) || {};
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    filterCustomerPI(customer_id) {
      if (!customer_id || customer_id === "") return;
      const scope = this;
      this.loading = true;
      if (!this.isEditPage) {
        this.formData.sales_order_no = "";
      }
      PIService.filterCustomerPI(customer_id, 1)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.PIList = res.data.content;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    bulkServiceCenterDetail(orderNo) {
      const scope = this;
      this.loading = true;
      const data = this.categoryList.map(res => {
        return {
          service_center_order_no: orderNo,
          item_id: res.item_id,
          bf_art_no: res.bf_art_no,
          category_root_id: res.category_root_id,
          sn: "",
          damage_count: 0,
          production_date: 0,
          error_code: 0
        };
      });

      return AfterSalesService.bulkServiceCenterDetail(data);
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.service-center-save {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .content {
    border: 1px solid #2b2f3b;
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
        .item-row {
          overflow: hidden;
          display: inline-block;
          margin: 10px auto;
          width: 100%;
          .label {
            display: inline-block;
            width: 110px;
            margin-left: 15px;
          }

          .input {
            margin-left: 10px;
            border-radius: 0 0 10px 10px;
          }
          .text-area {
            display: flex;
            align-items: center;
            justify-content: space-around;
            width: 100%;
            margin: 10px 0;
            padding: 5px 15px;
            .el-upload--text {
              display: flex;
            }
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

      a:hover {
        cursor: pointer;
        text-decoration: underline;
      }
    }
  }
  .table-wrap {
    border-bottom: 1px solid #2b2f3b;
  }
  .dialog-category {
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
// service-center-save.vue?69e9a423