<template>
  <div class="technical-service-save">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">Edit Technical Service</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Info</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Service No.
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.service_no" disabled></el-input>
            </div>
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
              >
                <el-option
                  v-for="item in customerList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <!-- <div class="item">
              <label class="label">Problem Doc</label>
              <el-input class="input" v-model="formData.problem_doc_id"></el-input>
            </div>-->
            <div class="item">
              <label class="label">Test Results</label>
              <el-select class="input" v-model="formData.test_result" filterable slot="prepend">
                <el-option
                  v-for="item in testResultData"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Advice</label>
              <el-input class="input" v-model="formData.advice"></el-input>
            </div>
            <div class="item">
              <label class="label">Remark</label>
              <el-input class="input" v-model="formData.remark"></el-input>
            </div>
            <div class="item">
              <label class="label">Production Date</label>
              <el-date-picker class="input" v-model="productionDate" type="date" placeholder="Date"></el-date-picker>
            </div>
          </div>
        </div>
        <!-- 物品展示列表 -->
        <div class="table-wrap table-overflow" v-if="categoryList.length>0">
          <el-table :data="categoryList" style="width: 100%;border:none;">
            <el-table-column type="expand">
              <template>
                <el-form label-position="left" inline class="demo-table-expand">
                  <el-form-item label="Item Name">
                    <span>{{props.row.category_root_id | categoryRoot}}</span>
                  </el-form-item>
                  <el-form-item label="BF Art No.">
                    <span>{{ props.row.bf_art_no }}</span>
                  </el-form-item>
                  <el-form-item label="SN">
                    <span>{{ props.row.sn }}</span>
                  </el-form-item>
                  <el-form-item label="Error Code">
                    <el-input
                      class="input"
                      v-onlyNum
                      v-model=" props.row.error_code"
                      placeholder="Error Code"
                    ></el-input>
                  </el-form-item>
                  <el-form-item label="Fault Des.">
                    <el-input class="input" v-model="props.row.fault_desc" placeholder="Fault Des."></el-input>
                  </el-form-item>
                  <el-form-item label="Responsibility">
                    <el-input
                      class="input"
                      v-onlyNum
                      v-model="props.row.division_responsibilities"
                      placeholder="Responsibility"
                    ></el-input>
                  </el-form-item>
                  <el-form-item label="Diagnose">
                    <span>{{ props.row.diagnosis_id }}</span>
                  </el-form-item>
                  <el-form-item label="Production Date">
                    <span>{{ props.row.production_date | dateFormat}}</span>
                  </el-form-item>
                </el-form>
              </template>
            </el-table-column>
            <el-table-column label="Item Name">
              <template>
                <div>{{scope.row.category_root_id | categoryRoot}}</div>
              </template>
            </el-table-column>
            <el-table-column label="SN" prop="sn"></el-table-column>
            <el-table-column label="Error Code" prop="error_code"></el-table-column>
            <el-table-column label="Production Date" prop="create_time">
              <template>
                <div>{{scopes.row.production_date | dateFormat}}</div>
              </template>
            </el-table-column>
          </el-table>
        </div>
        <div class="parts-list">
          <el-button type="primary" @click="createOrder">SAVE</el-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";
import TechnicalService from "src/services/technicalService";
import AfterSalesService from "src/services/afterSalesService";
import WarehouseService from "src/services/warehouseService";
import EN from "src/assets/lang/en";
import ZH from "src/assets/lang/zh";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      formData: {
        productionDate: "",
        service_no: "",
        carrier_order_id: "",
        customer_id: "",
        sn: "",
        customer_star: "",
        sales_order_id: "",
        error_code: "",
        problem_doc_id: ""
      },
      orderId: 0,
      customerList: [],
      productionDate: "",
      headData: [],
      categoryList: [],
      testResultData: [
        {
          id: 1,
          name: "Renewed"
        },
        {
          id: 2,
          name: "Maintain"
        }
      ]
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
      "Fault Des.",
      "Responsibility",
      "Diagnose",
      // "Damage Count",
      "Production Date",
      "Operation"
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

    // this.loading = true;
    // WarehouseService.getAllCategory().then(res => {
    //   scope.categoryList = res.category_list;
    //   scope.loading = false;
    // });

    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.loading = true;
      TechnicalService.getTechnical(this.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          scope.productionDate = Core.Util.changeDateForm(
            scope.formData.create_time * 1000
          );
          scope.loading = false;

          return AfterSalesService.getServiceCenterDetail(res.data.service_no);
        })
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.categoryList = res.data.content;
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
      this.$router.push({ name: "technicalService" });
    },
    createOrder() {
      const scope = this;
      // if (!this.checkParams()) {
      //   return;
      // }
      this.loading = true;
      this.formData.productionDate = Core.Util.parseDate(this.productionDate);
      TechnicalService.postTechnical(this.formData)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          scope.orderId = res.data.id;
          const data = this.categoryList.map(cate => {
            cate.production_date = Core.Util.parseDate(cate.production_date);
            return cate;
          });
          return AfterSalesService.bulkServiceCenterDetail(data);
        })
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.categoryList = res.data;
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

      // AfterSalesService.bulkServiceCenterDetail(data)
      //   .then(res => {
      //     if (res.code === -2) {
      //       throw res.data.message;
      //     }
      //     scope.$message({
      //       message: "Save Successfully",
      //       type: "success"
      //     });
      //     this.categoryList = res.data.map(cate => {
      //       cate.production_date = cate.production_date * 1000;
      //       return cate;
      //     });
      //     scope.loading = false;
      //   })
      //   .catch(err => {
      //     scope.$message({
      //       message: err,
      //       type: "error"
      //     });
      //     scope.loading = false;
      //   });
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
      if (this.formData.root_category_id === "") {
        this.$message({
          message: "Please add Category",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    //顾客信息联动
    handleItemChange(id) {
      let data = this.customerList.find(res => {
        return res.id === id;
      });
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
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.technical-service-save {
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

    .table-wrap {
      .el-table::before,
      .el-table::after {
        height: 0;
      }
      .demo-table-expand {
        font-size: 0;
      }
      .demo-table-expand label {
        width: 150px;
        color: #99a9bf;
      }
      .demo-table-expand .el-form-item {
        margin-right: 0;
        margin-bottom: 0;
        width: 50%;
      }
      .pick-list-add .content .order-info .el-table tr {
        background-color: #353945;
      }
      .el-table__header-wrapper thead div,
      .el-table__footer-wrapper thead div {
        background: #353945;
        color: #fff;
      }
      .el-table__expanded-cell {
        background: #353945;
        box-shadow: 0 -1px #2b2f3b;
      }
      .el-table__expanded-cell:hover {
        background-color: #353945 !important;
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
  }
}
</style>



// WEBPACK FOOTER //
// technical-service-save.vue?e76b0032