<template>
  <div class="CS-record-save">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goCSRecord" title="back"></div>
        </div>
      </div>
      <label class="title">Edit Customer Service Record</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Info</label>
          <div class="left">
            <div class="item">
              <label class="label">Customer</label>
              <el-select
                class="input"
                v-model="formData.customer_id"
                filterable
                slot="prepend"
                disabled
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
              <label class="label">Sales Order</label>
              <el-input class="input" v-model="formData.sales_order_no" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Service No</label>
              <el-input class="input" v-model="formData.service_no" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Service Type</label>
              <el-select
                class="input"
                v-model="formData.service_type"
                filterable
                slot="prepend"
                disabled
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
        <div class="table-wrap table-overflow" v-if="categoryList.length>0">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="(item, index) in headData" :key="index">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="item in categoryList" :key="item.id">
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.bf_art_no}}</td>
                <td>
                  <el-input
                    class="input"
                    v-model="item.sn"
                    placeholder="SN"
                    :disabled="formData.status > 0"
                  ></el-input>
                </td>
                <td>
                  <el-input
                    class="input"
                    v-model="item.error_code"
                    placeholder="Error Code"
                    :disabled="formData.status > 0"
                  ></el-input>
                </td>
                <td>
                  <el-input
                    class="number"
                    type="number"
                    v-model="item.damage_count"
                    placeholder="Damage Count"
                    :disabled="formData.status > 0"
                  ></el-input>
                </td>
                <td>
                  <el-date-picker
                    class="input"
                    v-model="item.production_date"
                    type="date"
                    placeholder="Date"
                    :disabled="formData.status > 0"
                  ></el-date-picker>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list" v-if="formData.status === 0">
          <el-button type="primary" @click="createOrder">SAVE</el-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";
import CustomerServiceRecordService from "src/services/customerServiceRecordService";
import WarehouseService from "src/services/warehouseService";
import AfterSalesService from "src/services/afterSalesService";
import EN from "src/assets/lang/en";
import ZH from "src/assets/lang/zh";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      formData: {
        customer_id: "",
        sales_order_no: "",
        service_no: "",
        service_type: "",
        status: 0
      },
      orderId: 0,
      customerList: [],
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
      headData: [],
      categoryList: []
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
      "SN",
      "Error Code",
      "Damage Count",
      "Production Date"
    ];
    const scope = this;
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

    this.loading = true;
    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.loading = true;
      CustomerServiceRecordService.getCustomerServiceRecord(this.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          return AfterSalesService.getServiceCenterDetail(res.data.service_no);
        })
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.categoryList = res.data.content.map(res => {
            res.production_date =
              (res.production_date && res.production_date * 1000) || new Date();
            return res;
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
    }
  },
  mounted() {},
  methods: {
    // 返回路由
    goCSRecord() {
      this.$router.push({ name: "customerServiceRecord" });
    },
    createOrder() {
      const scope = this;
      this.loading = true;

      const data = this.categoryList.map(res => {
        res.production_date = Core.Util.parseDate(res.production_date);
        return res;
      });
      AfterSalesService.bulkServiceCenterDetail(data)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
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
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.CS-record-save {
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
            width: 147px;
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
          width: 100%;
          padding-top: 15px;
          label {
            margin-left: 15px;
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
    }
  }
}
</style>



// WEBPACK FOOTER //
// customer-service-record-save.vue?3cefb3a5