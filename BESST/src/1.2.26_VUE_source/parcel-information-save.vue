<template>
  <div class="parcel-information-add">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">{{ orderId ? 'Edit Parcel Information' : 'Add Parcel Information' }}</label>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content">
        <div class="order-info">
          <label class="title">Parcel Information</label>
          <div class="left">
            <div class="item">
              <label class="label">Parcel No.</label>
              <el-input
                class="input"
                v-model="formData.parcel_no"
                placeholder="Parcel No."
                disabled
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Carrier No./Name
                <span class="required">*</span>
              </label>
              <div class="input-mager">
                <el-input
                  class="input-mager-out input-sn"
                  v-model="formData.carrier_order_no"
                  placeholder="Carrier No."
                ></el-input>
                <el-input
                  slot="prepend"
                  class="input-mager-in"
                  v-model="formData.carrier_name"
                  placeholder="Carrier Name"
                ></el-input>
              </div>
              <!-- DHL 需求问题暂时隐藏 -->
              <!-- <a :href="DHLUrl" class="btn-underline" title="DHL" target="_blank">DHL</a> -->
            </div>
            <!-- <div class="item">
              <label class="label">
                Carrier Name
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.carrier_name" placeholder="Carrier Name"></el-input>
            </div>-->
            <div class="item">
              <label class="label">
                Company
                <span class="required">*</span>
              </label>
              <el-select
                class="input"
                v-model="formData.customer_id"
                @change="handleItemChange"
                filterable
                slot="prepend"
                placeholder="Company"
              >
                <el-option
                  v-for="item in customerList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
              <a class="btn-underline" @click.stop="dialogAdd">Add</a>
            </div>
            <!-- ETD暂时隐藏 -->
            <!-- <div class="item">
              <label class="label">
                Etd
                <span class="required">*</span>
              </label>
              <el-date-picker class="input" v-model="etdDate" type="date" placeholder="Date"></el-date-picker>
            </div> -->
            <div class="item">
              <label class="label">Telephone</label>
              <el-input class="input" v-model="formData.telephone" placeholder="Tel" disabled>
                <el-select v-model="formData.country_code" filterable slot="prepend" disabled>
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
              <label class="label">Address</label>
              <el-input class="input" v-model="formData.address" placeholder="Address" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Country</label>
              <el-select
                class="input"
                v-model="formData.country"
                filterable
                slot="prepend"
                disabled
              >
                <el-option
                  v-for="item in areaCodeList"
                  :key="item.area_code"
                  :label="item.area_en"
                  :value="item.area_en"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Atten</label>
              <el-input class="input" v-model="formData.atten" placeholder="Atten" disabled></el-input>
            </div>
            <div class="item">
              <label class="label">Vat No.</label>
              <el-input class="input" v-model="formData.vat_no" placeholder="Vat No." disabled></el-input>
            </div>
          </div>
        </div>
        <div class="footer">
          <el-button type="primary" @click="createInfo" :disabled="formData.status>=1">Save</el-button>
        </div>
      </div>
    </div>
    <!-- 添加or修改弹窗 -->
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogVisible"
      custom-class="edit-dialog"
      class="dialog-md"
    >
      <div class="item">
        <label class="title">
          Company Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="customeFormData.name" placeholder="Name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Customer No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.company_no" placeholder="Nick name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Currency
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select class="input input-special" v-model="customeFormData.currency" slot="prepend">
            <el-option
              v-for="item in currencies"
              :key="item.currency"
              :label="item.label"
              :value="item.currency"
              :value-key="item.currency"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Email
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.email" placeholder="Service email"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Phone
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.mobile">
            <el-select
              v-model="customeFormData.country_code"
              @change="PhoneRelatedCountry"
              filterable
              slot="prepend"
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
      </div>
      <div class="item">
        <label class="title">
          Country
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            class="input input-special"
            v-model="customeFormData.country"
            filterable
            slot="prepend"
            @change="CountryToPhone"
          >
            <el-option
              v-for="item in areaCodeList"
              :key="item.area_code"
              :label="item.area_en"
              :value="item.area_en"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">Province</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.province"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">City</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.city"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">District</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.district"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Address
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.address"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Account No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.account_no" placeholder="Account number"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">VAT No.</label>
        <div class="content">
          <el-input
            class="input"
            v-model="customeFormData.vat_no"
            placeholder="Customer company VAT.NO"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Payment Method
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            class="input"
            v-model="customeFormData.payment_method"
            placeholder="Select Payment Method"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in paymentMethodData"
              :key="item.id"
              :label="item.label"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Trade Terms
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            v-model="customeFormData.trade_term"
            slot="append"
            class="input"
            placeholder="Select Trade Terms"
            filterable
          >
            <el-option
              v-for="item in tradeTerms"
              :key="item.id"
              :label="item.label"
              :value="item.label"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Payment Term
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            v-model="customeFormData.payment_term"
            slot="append"
            class="input"
            placeholder="Select Payment Term"
            filterable
          >
            <el-option
              v-for="item in paymentTerms"
              :key="item.id"
              :label="item.label"
              :value="item.label"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Receiver Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input
            class="input"
            v-model="customeFormData.receiver_name"
            placeholder="Company contact"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <el-tooltip
          effect="dark"
          content="The bigger the number, the better the quota"
          placement="top"
        >
          <label class="title">Credit Line</label>
        </el-tooltip>
        <div class="content">
          <el-input-number
            class="input input-special"
            v-model="customeFormData.credit_line"
            :min="1"
          ></el-input-number>
        </div>
      </div>
      <div class="item">
        <el-tooltip
          effect="dark"
          content="The bigger the number, the better the credit"
          placement="top-start"
        >
          <label class="title">Credit Grade</label>
        </el-tooltip>
        <div class="content">
          <el-input-number
            class="input input-special"
            v-model="customeFormData.credit_grade"
            :min="1"
          ></el-input-number>
        </div>
      </div>
      <div class="item">
        <label class="title">Service level</label>
        <div class="content">
          <el-input-number class="input input-special" v-model="customeFormData.star" :min="1"></el-input-number>
        </div>
      </div>
      <div class="item">
        <label class="title">Remark</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.remark"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Order Memo</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.order_memo"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Telephone</label>
        <div class="content">
          <el-input class="input" v-model="customeFormData.telephone" :min="1"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";
import WarehouseService from "src/services/warehouseService";
import ParcelInformationService from "src/services/parcelInformationService";
import CustomerAccountService from "src/services/customerAccountService";

import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      DHLUrl: "https://www.logistics.dhl/cn-zh/home/tracking.html",
      formData: {
        parcel_no: "",
        carrier_order_no: "",
        carrier_name: "",
        customer_id: "",
        customer_name: "",
        country_code: "+86",
        telephone: "",
        address: "",
        country: "",
        atten: "",
        // etd: "",
        vat_no: "",
        status: 0
      },
      orderId: 0,
      areaCodeList: [],
      customerList: [],
      // etdDate: "",
      //弹窗名称
      dialogTitle: "",
      //操作弹窗显示
      dialogVisible: false,
      customeFormData: {},
      tradeTerms: Core.ConstData.TRADE_TERMS,
      payment_types: Core.ConstData.PAYMENT_TYPES,
      paymentTerms: Core.ConstData.PAYMENT_TERMS,
      currencies: Core.ConstData.CURRENCIES,
      paymentMethodData: Core.ConstData.PAYMENT_METHOD
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
    const scope = this;
    this.areaCodeList = areaCodeInDB.init();
    this.orderId = this.$route.query.orderId;
    await this.getAllCustomer();
    if (this.orderId) {
      this.loading = true;
      ParcelInformationService.getParcelInfo(this.orderId)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res.data));
          // scope.etdDate = scope.formData.etd * 1000;
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
  mounted: function() {},
  methods: {
    //国家、地区相互转化
    PhoneRelatedCountry(countryCode) {
      const _coCode = this.areaCodeList.find(res => {
        return res.area_code === countryCode;
      });
      this.customeFormData.country = _coCode.area_en;
    },
    CountryToPhone(area_en) {
      const _phone = this.areaCodeList.find(res => {
        return res.area_en === area_en;
      });
      _phone && (this.customeFormData.country_code = _phone.area_code);
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "ParcelInfo" });
    },
    checkParams() {
      if (!this.formData.customer_id) {
        this.$message({
          message: "Please add Company",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.carrier_order_no) {
        this.$message({
          message: "Please add Carrier Order No.",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.carrier_name) {
        this.$message({
          message: "Please add Carrier Name",
          type: "warning"
        });
        return false;
      }
      // if (!this.formData.etd) {
      //   this.$message({
      //     message: "Please add Etd",
      //     type: "warning"
      //   });
      //   return false;
      // }
      return true;
    },
    createInfo() {
      const scope = this;
      // this.formData.etd = Core.Util.parseDate(this.etdDate);
      if (this.checkParams()) {
        scope.loading = true;
        ParcelInformationService.postParcelInfo(this.formData)
          .then(res => {
            if (res.data.code === -2) {
              throw res.data.message;
            }
            scope.$message({
              message: "Successfully saved",
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
    },
    filterParcelInfo() {
      const scope = this;
      scope.loading = true;
      ParcelInformationService.filterParcelInfo(
        this.currentPage,
        this.formSearch
      )
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.tableList = res.data.content;
          scope.total = res.data.totalElements;
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
    async getAllCustomer() {
      const scope = this;
      scope.loading = true;
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
    },
    //顾客信息联动
    handleItemChange(id) {
      let data = this.customerList.find(res => {
        return res.id === id;
      });
      const _coCode = this.areaCodeList.find(res => {
        return res.area_code === data.country_code;
      });
      this.formData.customer_name = (data && data.name) || "";
      this.formData.telephone = (data && data.mobile) || "";
      this.formData.address = (data && data.address) || "";
      this.formData.country_code = (data && data.country_code) || "+86";
      this.formData.vat_no = (data && data.vat_no) || "";
      this.formData.atten = (data && data.receiver_name) || "";
      this.formData.country =
        (data && data.country) || (_coCode && _coCode.area_en);
    },
    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Company";
      this.dialogVisible = true;
      this.customeFormData = {
        name: "",
        company_no: "",
        account_no: "",
        payment_method: "",
        credit_grade: "",
        credit_line: "",
        mobile: "",
        address: "",
        country_code: "+86",
        remark: "",
        star: "",
        currency: 1,
        customer_id: 0,
        province: "",
        city: "",
        district: "",
        order_memo: "",
        receiver_name: "",
        telephone: "",
        postcode: "",
        country: "",
        vat_no: ""
      };
    },
    checkDialog() {
      if (!this.formData.receiver_name) {
        this.$message({
          message: "Please add Receiver name",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.name) {
        this.$message({
          message: "Please add name",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.address) {
        this.$message({
          message: "Please add address",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.mobile) {
        this.$message({
          message: "Please add mobile",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.country_code) {
        this.$message({
          message: "Please add country code",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    dialogConfirm() {
      this.loading = true;
      CustomerAccountService.postCompany(this.customeFormData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          this.getAllCustomer();
          this.dialogVisible = false;
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    dialogCancel() {
      this.dialogVisible = false;
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" >
.parcel-information-add {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .input-mager {
    width: 255px;
    display: inline-block;
    margin-left: 10px;
    position: relative;

    .input-mager-out {
      width: 49%;
      input {
        border-top-right-radius: 0;
        border-bottom-right-radius: 0;
      }
    }
    .input-mager-in {
      width: 49%;
      input {
        border-top-left-radius: 0;
        border-bottom-left-radius: 0;
      }
    }
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
        height: 280px;
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
            width: 125px;
            margin-left: 15px;
          }
          .input {
            margin-left: 10px;
            width: 255px;
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

    .footer {
      width: 100%;
      height: 60px;
      line-height: 60px;
      padding-left: 40px;
      box-shadow: 0 -1px #2b2f3b;
      text-align: right;
      padding-right: 40px;
      display: inline-block;
    }
  }

  .dialog-md {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 48px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .input-special {
        width: 240px;
      }
      .title {
        display: inline-block;
        width: 120px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 240px;
        border: none;
      }
    }
  }
  .pagination {
    padding: 10px;
  }
}
</style>



// WEBPACK FOOTER //
// parcel-information-save.vue?514fc370