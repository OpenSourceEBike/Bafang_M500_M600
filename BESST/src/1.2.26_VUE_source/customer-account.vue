<template>
  <div class="customer-account">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">{{ 'Customer Account' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Account Info</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Name
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.name" placeholder="Company name"></el-input>
            </div>
            <div class="item">
              <label class="label">Customer No.</label>
              <el-input class="input" v-model="formData.company_no" placeholder="Nick name"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Email
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.email" placeholder="Service email"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Phone
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.mobile">
                <el-select
                  v-model="formData.country_code"
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
            <!-- <div class="item">
              <label class="label">
                Address
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.address" placeholder="Receive mail address"></el-input>
            </div>-->
            <div class="item">
              <label class="label">
                Street
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.street" placeholder="Street"></el-input>
            </div>
            <div class="item">
              <label class="label">
                No.
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.address_no" placeholder=" No."></el-input>
            </div>
            <div class="item">
              <label class="label">Currency</label>
              <el-select class="input" v-model="formData.currency" slot="prepend">
                <el-option
                  v-for="item in currencies"
                  :key="item.currency"
                  :label="item.label"
                  :value="item.currency"
                  :value-key="item.currency"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Country</label>
              <el-select
                class="input"
                v-model="formData.country"
                filterable
                slot="prepend"
                @change="CountryToPhone"
              >
                <el-option
                  v-for="(item, index) in areaCodeList"
                  :key="index"
                  :label="item.area_en"
                  :value="item.area_en"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Province</label>
              <el-input class="input" v-model="formData.province"></el-input>
            </div>
            <div class="item">
              <label class="label">City</label>
              <el-input class="input" v-model="formData.city"></el-input>
            </div>
            <div class="item">
              <label class="label">District</label>
              <el-input class="input" v-model="formData.district"></el-input>
            </div>
            <div class="item">
              <label class="label">Zip Code</label>
              <el-input class="input" v-model="formData.postcode" placeholder="Postal address"></el-input>
            </div>
            <div class="item">
              <label class="label">Account No.</label>
              <el-input class="input" v-model="formData.account_no" placeholder="Account number"></el-input>
            </div>
            <div class="item">
              <label class="label">VAT No.</label>
              <el-input
                class="input"
                v-model="formData.vat_no"
                placeholder="Customer company VAT.NO"
              ></el-input>
            </div>
            <div class="item">
              <label class="label">Payment Method</label>
              <el-select
                class="input"
                v-model="formData.payment_method"
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
            <div class="item">
              <label class="label">Trade Terms</label>
              <el-select
                v-model="formData.trade_term"
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
            <div class="item">
              <label class="label">Payment Term</label>
              <el-select
                v-model="formData.payment_term"
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
            <div class="item">
              <label class="label">Receiver Name</label>
              <el-input
                class="input"
                v-model="formData.receiver_name"
                placeholder="Company contact"
              ></el-input>
            </div>
            <div class="item">
              <!-- <label class="label">Credit Line</label> -->
              <el-tooltip
                effect="dark"
                content="The bigger the number, the better the quota"
                placement="top"
              >
                <label class="label">Credit Line</label>
              </el-tooltip>
              <el-input-number class="input" v-model="formData.credit_line" :min="1"></el-input-number>
            </div>
            <div class="item">
              <!-- <label class="label">Credit Grade</label> -->
              <el-tooltip
                effect="dark"
                content="The bigger the number, the better the credit"
                placement="top-start"
              >
                <label class="label">Credit Grade</label>
              </el-tooltip>
              <el-input-number class="input" v-model="formData.credit_grade" :min="1"></el-input-number>
            </div>
            <div class="item">
              <label class="label">Service level</label>
              <el-input-number class="input" v-model="formData.star" :min="1"></el-input-number>
            </div>
            <div class="item">
              <label class="label">Remark</label>
              <el-input class="input" v-model="formData.remark"></el-input>
            </div>
            <div class="item">
              <label class="label">Order Memo</label>
              <el-input class="input" v-model="formData.order_memo"></el-input>
            </div>
          </div>
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

export default {
  data() {
    return {
      loading: false,
      isEditPage: false,
      formData: {
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
        postcode: "",
        country: "",
        vat_no: "",
        create_user_id: Core.Data.getUser() ? Core.Data.getUser().id : ""
      },
      orderId: 0,
      areaCodeList: [],
      tradeTerms: Core.ConstData.TRADE_TERMS,
      payment_types: Core.ConstData.PAYMENT_TYPES,
      paymentTerms: Core.ConstData.PAYMENT_TERMS,
      currencies: Core.ConstData.CURRENCIES,
      paymentMethodData: Core.ConstData.PAYMENT_METHOD
    };
  },
  i18n: {
    messages: {}
  },
  computed: {},
  mounted: function() {
    const scope = this;
    this.areaCodeList = areaCodeInDB.init();
    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.formData.customer_id = this.orderId;
      this.loading = true;
      this.getCompanyById(this.orderId);
    }
  },
  methods: {
    //国家、地区相互转化
    PhoneRelatedCountry(countryCode) {
      const _coCode = this.areaCodeList.find(res => {
        return res.area_code === countryCode;
      });
      this.formData.country = _coCode.area_en;
    },
    CountryToPhone(area_en) {
      const _phone = this.areaCodeList.find(res => {
        return res.area_en === area_en;
      });
      _phone && (this.formData.country_code = _phone.area_code);
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "customerList" });
    },
    createOrder() {
      if (!this.checkParams()) {
        return;
      }
      console.log(JSON.stringify(this.formData));

      this.postCompany(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.formData = JSON.parse(JSON.stringify(res));
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          //   this.$router.push({
          //     name: "customerAccount",
          //     query: { orderId: res.id }
          //   });
          this.orderId = res.id;
          this.loading = false;
          this.$router.go(-1);
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 弹窗必填
    checkParams() {
      if (this.formData.name === "") {
        this.$message({
          message: "Please add Name",
          type: "warning"
        });
        return false;
      }
      if (!Core.Util.isEmail(this.formData.email)) {
        this.$message({
          message: "E-mail is illegitimate",
          type: "warning"
        });
        return false;
      }
      if (this.formData.country_code === "") {
        this.$message({
          message: "Please add Country Code",
          type: "warning"
        });
        return false;
      }

      if (this.formData.mobile === "") {
        this.$message({
          message: "Please add Phome",
          type: "warning"
        });
        return false;
      }

      // if (this.formData.address === "") {
      //   this.$message({
      //     message: "Please add Address",
      //     type: "warning"
      //   });
      //   return false;
      // }

      if (this.formData.street === "") {
        this.$message({
          message: "Please add Street",
          type: "warning"
        });
        return false;
      }

      if (this.formData.address_no === "") {
        this.$message({
          message: "Please add No.",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    // 查询顾客账户信息
    getCompanyById(id) {
      this.loading = true;
      Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/" + id
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.formData = res;
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
    postCompany(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/nlPublic/company",
        data: data
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.customer-account {
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
      height: 708px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 658px;
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
            width: 130px;
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

          .el-input-number__increase {
            top: 15px;
            border: 0;
          }
          .el-input-number__decrease {
            top: 15px;
            border: 0;
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
  }
}
</style>



// WEBPACK FOOTER //
// customer-account.vue?10615818