<template>
    <div class="PI-header">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goPIList" title="back"></div>
                </div>
            </div>
            <label class="title">{{ orderId ? 'Edit Proforma Invoice' : 'Add Proforma Invoice' }}</label>
        </div>
        <div class="content-wrap">
            <div class="content content-width" v-loading.body="loading">
                <div class="order-info">
                    <label class="title">Proforma Invoice Info</label>
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
                                value-key="id"
                                slot="prepend"
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
                        <div class="item">
                            <label class="label">
                                Type
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                name="type"
                                v-model="formData.type"
                                filterable
                                value-key="id"
                                slot="prepend"
                            >
                                <el-option
                                    v-for="item in typeList"
                                    :key="item.id"
                                    :label="item.name"
                                    :value="item.id"
                                ></el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label">Customer Art Nr.</label>
                            <el-input
                                class="input input-sn"
                                v-model="formData.customer_order_no"
                                @change="getOrderInfo()"
                                placeholder="Customer Art Nr."
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                Country
                                <span class="required">*</span>
                            </label>
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
                            <label class="label">
                                Address
                                <span class="required">*</span>
                            </label>
                            <el-input
                                class="input"
                                v-model="formData.address"
                                placeholder="Address"
                                :disabled="true"
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                Tel
                                <span class="required">*</span>
                            </label>
                            <el-input
                                class="input"
                                v-model="formData.telephone"
                                :disabled="true"
                                placeholder="Tel"
                            >
                                <el-select
                                    v-model="formData.country_code"
                                    filterable
                                    slot="prepend"
                                    :disabled="true"
                                    class="country-code"
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
                            <label class="label">
                                VAT No.
                                <span class="required">*</span>
                            </label>
                            <el-input
                                class="input"
                                v-model="formData.vat_no"
                                placeholder="VAT No."
                                disabled
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                ETD
                                <span class="required">*</span>
                            </label>
                            <el-date-picker
                                class="input"
                                v-model="formData.etd"
                                type="date"
                                placeholder="ETD"
                            ></el-date-picker>
                        </div>
                        <div class="item">
                            <label class="label">
                                Trade Terms
                                <span class="required">*</span>
                            </label>
                            <el-select
                                v-model="formData.trade_terms"
                                slot="append"
                                class="input"
                                filterable
                                disabled
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
                            <label class="label">
                                Payment Term
                                <span class="required">*</span>
                            </label>
                            <el-select
                                v-model="formData.payment_term"
                                slot="append"
                                class="input"
                                filterable
                                disabled
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
                            <label class="label">
                                Date
                                <span class="required">*</span>
                            </label>
                            <el-date-picker
                                class="input"
                                v-model="formData.date"
                                type="date"
                                placeholder="Date"
                            ></el-date-picker>
                        </div>
                        <div class="item">
                            <label class="label">
                                Shipment
                                <span class="required">*</span>
                            </label>
                            <el-select v-model="formData.shipment" slot="append" class="input">
                                <el-option
                                    v-for="item in shipmentTerms"
                                    :key="item.id"
                                    :label="item.label"
                                    :value="item.label"
                                ></el-option>
                            </el-select>
                        </div>
                        <div class="item">
                            <label class="label">
                                Ship from
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.origin"
                                filterable
                                slot="prepend"
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
                            <label class="label">
                                Ship to
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.destination"
                                filterable
                                slot="prepend"
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
                            <label class="label">
                                Currency/Amount
                                <span class="required">*</span>
                            </label>
                            <el-input class="input" v-model="formData.amount" disabled>
                                <el-select v-model="formData.currency" slot="prepend" disabled>
                                    <el-option
                                        v-for="item in currencies"
                                        :key="item.currency"
                                        :label="item.label"
                                        :value="item.currency"
                                        :value-key="item.currency"
                                    ></el-option>
                                </el-select>
                            </el-input>
                        </div>
                    </div>
                </div>
                <div class="table-wrap table-overflow">
                    <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                        <thead>
                            <td>
                                <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
                            </td>
                            <td v-for="item in headData">{{item}}</td>
                        </thead>
                        <tbody>
                            <tr v-for="(item, index) in orderItems">
                                <td>
                                    <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                                </td>
                                <!-- <td>{{item.category_root_id | categoryRoot}}</td> -->
                                <td>{{item.model_name}}</td>
                                <td>{{item.bf_art_no}}</td>
                                <td>
                                    <el-input
                                        v-model="item.customer_art_no"
                                        placeholder="Customer Art No."
                                    ></el-input>
                                </td>
                                <td>
                                    <el-input v-model="item.description" placeholder="Description"></el-input>
                                </td>
                                <td>
                                    <el-input class="input" v-model="item.unit_price">
                                        <el-select
                                            class="currency-input"
                                            v-model="formData.currency"
                                            slot="prepend"
                                        >
                                            <el-option
                                                v-for="item in currencies"
                                                :key="item.currency"
                                                :label="item.label"
                                                :value="item.currency"
                                                :value-key="item.currency"
                                            ></el-option>
                                        </el-select>
                                    </el-input>
                                </td>
                                <td>
                                    <el-input
                                        type="number"
                                        v-model="item.vat_rate"
                                        :min="0"
                                        @change="vatRateRelation($event)"
                                    ></el-input>%
                                </td>
                                <td>
                                    <el-input-number
                                        class="input-number"
                                        v-model="item.quantity"
                                        :min="1"
                                    ></el-input-number>
                                </td>
                                <td>{{item.amount}}</td>
                                <td>
                                    <a
                                        class="btn-underline"
                                        v-on:click="deleteItem(item,index)"
                                    >{{ $t('delete') }}</a>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div class="parts-list">
                    <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
                    <span
                        class="att-total"
                    >total: {{formData.currency | currencyType}}{{ totalCount }}</span>
                </div>
                <div class="footer">
                    <el-button type="delete" @click="deleteOrderItem">{{ $t('delete') }}</el-button>
                    <el-button type="primary" @click="onCreate">Save</el-button>
                </div>
                <!-- Add dialog -->
                <el-dialog
                    class="dialog"
                    :title="$t('addItem')"
                    :visible.sync="dialogCreateVisible"
                    size="large"
                    top="10%"
                >
                    <div class="content">
                        <div class="header">
                            <el-input
                                :placeholder="$t('category')"
                                class="search-content"
                                v-model="selectCategoryItem.name"
                                @click.native="selectCategory"
                            ></el-input>
                            <el-input
                                :placeholder="$t('modelNamePN')"
                                class="search-content"
                                v-model="queryItemCondition"
                            ></el-input>
                            <div class="btn-group-inline">
                                <div class="icons">
                                    <div
                                        class="icon-item search"
                                        @click="queryOrderItem"
                                        :title="$t('search')"
                                    ></div>
                                    <div
                                        class="icon-item clear"
                                        @click="clearCondition"
                                        :title="$t('clear')"
                                    ></div>
                                </div>
                            </div>
                        </div>
                        <div class="table-wrap">
                            <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                                <thead>
                                    <td>
                                        <el-checkbox v-model="allSelectedForAdd"></el-checkbox>
                                    </td>
                                    <td v-for="item in itemHeadData">{{item}}</td>
                                </thead>
                                <tbody>
                                    <tr v-for="(item, index) in selectItems">
                                        <td>
                                            <el-checkbox
                                                class="table-checkbox"
                                                v-model="item.selected"
                                            ></el-checkbox>
                                        </td>
                                        <td>
                                            <img :src="item.img | getImgUrl">
                                        </td>
                                        <td>{{item.name}}</td>
                                        <td>{{item.ext_part_no}}</td>
                                        <td>{{index+1}}.bin</td>
                                    </tr>
                                </tbody>
                            </table>
                            <el-pagination
                                class="pagination"
                                @current-change="handleCurrentPageChange"
                                :current-page.sync="currentPage"
                                :page-size="10"
                                layout="prev, pager, next"
                                :total="total"
                            ></el-pagination>
                        </div>
                        <div class="footer">
                            <el-button type="primary" @click="addItems">{{ $t('addItems') }}</el-button>
                        </div>
                    </div>
                </el-dialog>
            </div>
        </div>
        <!-- 类别弹窗 -->
        <el-dialog
            class="dialog"
            :title="$t('selectCategory')"
            :visible.sync="dialogSelectCategoryVisible"
            size="large"
        >
            <div class="content" style="max-height: 500px; overflow: auto">
                <div class="category-tree">
                    <el-tree
                        :data="categoryList"
                        :props="defaultProps"
                        @node-click="handleNodeClick"
                    ></el-tree>
                </div>
            </div>
        </el-dialog>
        <!-- 添加客户弹窗 -->
        <el-dialog
            :title.sync="dialogTitle"
            :visible.sync="dialogCustomerVisible"
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
                    <el-input
                        class="input"
                        v-model="customeFormData.company_no"
                        placeholder="Nick name"
                    ></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Currency
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-select
                        class="input input-special"
                        v-model="customeFormData.currency"
                        slot="prepend"
                    >
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
                    <el-input
                        class="input"
                        v-model="customeFormData.email"
                        placeholder="Service email"
                    ></el-input>
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
                            v-for="(item, index) in areaCodeList"
                            :key="index"
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
                    <el-input
                        class="input"
                        v-model="customeFormData.account_no"
                        placeholder="Account number"
                    ></el-input>
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
                    <el-input-number
                        class="input input-special"
                        v-model="customeFormData.star"
                        :min="1"
                    ></el-input-number>
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
import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";
import PurchaseService from "src/services/purchaseService";
import CustomerAccountService from "src/services/customerAccountService";
import WarehouseService from "src/services/warehouseService";

export default {
  data() {
    return {
      loading: false,
      headData: [],
      currentPage: 1,
      total: 0,
      orderId: 0,
      //弹窗
      dialogCreateVisible: false,
      dialogSelectCategoryVisible: false,
      //当前操作ID
      currentID: "",
      //顾客
      customerList: [],
      date: new Date(),
      formData: {
        customer_id: "",
        customer_name: "",
        customer_order_no: "",
        address: "",
        country: "",
        sales_name: Core.Data.getUser().name,
        atten: "",
        pi_no: "",
        telephone: "",
        vat_no: "",
        etd: new Date(),
        country_code: "+86",
        trade_terms: "",
        payment_term: "",
        date: new Date(),
        status: 0,
        create_user_id: Core.Data.getUser() ? Core.Data.getUser().id : "",
        destination: "",
        origin: "",
        shipment: "",
        amount: 0,
        currency: 1,
        type: 0
      },
      categoryList: [],
      typeList: [
        {
          id: 0,
          name: "SALE"
        },
        {
          id: 1,
          name: "AFTERSALE"
        }
      ],
      currencies: [
        {
          currency: 1,
          label: "€"
        },
        {
          currency: 2,
          label: "$"
        },
        {
          currency: 3,
          label: "DM."
        },
        {
          currency: 4,
          label: "￥"
        },
        {
          currency: 5,
          label: "￡"
        }
      ],
      defaultProps: {
        id: "id",
        children: "child",
        label: "name"
      },
      selectCategoryItem: {
        id: "",
        name: ""
      },
      itemHeadData: ["Image", "Name", "Part No.", "Firmware"],
      selectItems: [],
      queryItemCondition: "",
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
      tradeTerms: Core.ConstData.TRADE_TERMS,
      paymentTerms: Core.ConstData.PAYMENT_TERMS,
      shipmentTerms: Core.ConstData.SHIPMENT_TERMS,
      areaCodeList: [],
      pi_amout: "",
      timer: "",
      dialogCustomerVisible: false,
      customeFormData: {},
      dialogTitle: "",
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
  async created() {
    this.loading = true;
    await this.getAllCustomer();
  },
  computed: {
    //计算总金额
    totalCount: function() {
      let total = 0;
      const scope = this;
      this.orderItems.forEach(res => {
        res.currency = scope.formData.currency;
        const num = res.quantity * res.unit_price * (1 + res.vat_rate / 100);
        res.amount = Math.round(num * 100) / 100;
        total = Math.round((total + res.amount) * 100) / 100;
        scope.formData.amount = scope.formData.type ? 0 : total;
      });
      return total;
    },
    allSelectedForAdd: {
      get: function() {
        return this.selectItems.reduce(function(prev, curr) {
          return prev && curr.selected;
        }, true);
      },
      set: function(newValue) {
        this.selectItems.forEach(item => {
          item.selected = newValue;
        });
      }
    },
    allSelectedForDelete: {
      get: function() {
        return this.orderItems.reduce(function(prev, curr) {
          return prev && curr.selected;
        }, true);
      },
      set: function(newValue) {
        this.orderItems.forEach(item => {
          item.selected = newValue;
        });
      }
    }
  },
  mounted() {
    this.headData = [
      "Item Name",
      "BF Art No.",
      "Customer Art No.",
      "Description",
      "Unit Price",
      "VAT rate",
      "Quantity",
      "Amount",
      "Operation"
    ];

    this.getAllCategory();
    const scope = this;
    scope.areaCodeList = areaCodeInDB.init();

    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.loading = true;
      scope
        .getPIById(scope.orderId)
        .then(res => {
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.formData.date = Core.Util.changeDateForm(
            scope.formData.date * 1000
          );
          scope.formData.etd = Core.Util.changeDateForm(
            scope.formData.etd * 1000
          );
          return scope.filterPIDetails(scope.formData.pi_no);
        })
        .then(res => {
          scope.orderItems = res.content.map(data => {
            data.selected = false;
            data.item_part_no = data.item_code;
            return data;
          });
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
  },
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
    // 添加客户
    dialogAdd() {
      this.dialogTitle = "Add Customer";
      this.dialogCustomerVisible = true;
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
          this.dialogCustomerVisible = false;
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
      this.dialogCustomerVisible = false;
    },
    //税率关联
    vatRateRelation(evt) {
      this.orderItems.forEach(res => {
        res.vat_rate = evt;
      });
    },
    //返回
    goPIList() {
      this.$router.push({ name: "PIList" });
    },
    //顾客输入搜索
    createStateFilter(queryString) {
      return state => {
        return (
          state.value.toLowerCase().indexOf(queryString.toLowerCase()) === 0
        );
      };
    },
    //根据采购单联动数据
    getOrderInfo() {
      if (this.formData.customer_order_no === "") return;
      if (this.timer) {
        clearTimeout(this.timer);
      }
      this.timer = setTimeout(() => {
        this.loading = true;
        const _orderNo = this.formData.customer_order_no.trim();
        PurchaseService.getPurchase(_orderNo)
          .then(res => {
            if (res.data.code === -2) {
              throw res.data.message;
            }
            if (res.data.content.length > 0) {
              const purchase_order_id = res.data.content[0].id;
              return PurchaseService.filterAllPurchaseDetails(
                purchase_order_id
              );
            }
            throw "Not find purchase order";
          })
          .then(res => {
            if (res.data.code === -2) {
              throw res.data.message;
            }
            this.orderItems = res.data.content.map(orderItem => {
              orderItem.id = "";
              orderItem.model_name = orderItem.item_name;
              orderItem.bf_art_no = orderItem.item_code;
              orderItem.item_part_no = orderItem.item_code;
              orderItem.customer_art_no = _orderNo;
              orderItem.description = "";
              orderItem.unit_price = 1;
              orderItem.currency = 1;
              orderItem.vat_rate = 17;
              orderItem.amount = 1;
              orderItem.selected = false;
              return orderItem;
            });
            this.loading = false;
          })
          .catch(err => {
            this.$message({
              message: err,
              type: "error"
            });
            this.loading = false;
          });
      }, 1000);
    },
    //顾客信息联动
    handleItemChange(id) {
      if (this.customerList.length === 0) return;
      const data = this.customerList.find(res => {
        return res.id === id;
      });

      this.customerList.forEach(res => {
        if (res.id === id) {
          this.formData.customer_name = res.name;
        }
      });

      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.country = data.country;
      this.formData.country_code = data.country_code;
      this.formData.address = data.address;
      this.formData.telephone = data.mobile;
      // this.formData.atten = data.
      this.formData.vat_no = data.vat_no;
      this.formData.trade_terms = data.trade_term;
      this.formData.payment_term = data.payment_term;
    },
    // 获取类别
    getAllCategory() {
      this.loading = true;
      Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
      )
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.categoryList = res.category_list;
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
    queryOrderItem() {
      let org = Core.Data.getOrg();
      let queryItemCondition = this.queryItemCondition.trim();
      if (org !== null) {
        Core.Api.Item.queryOrderItem(
          25,
          this.selectCategoryItem.id,
          "",
          queryItemCondition,
          this.currentPage
        ).then(res => {
          this.queryItemList = res.query;
          this.total = res.count;
          if (!this.queryItemList.length > 0) {
            this.$message({
              message: "There are no matching data",
              type: "warning"
            });
            return;
          }
          this.selectItems = [];
          this.queryItemList.forEach((item, index) => {
            let selectItem = {
              selected: true,
              key: index,
              name: item.name,
              img: item.img,
              ext_part_no: item.ext_part_no,
              id: item.id,
              category_root_id: item.category_root_id,
              category_id: item.category_id
            };
            this.selectItems.push(selectItem);
          });
        });
      }
    },
    clearCondition() {
      this.selectItems = [];
      this.queryItemList = [];
      this.queryItemCondition = "";
      this.selectCategoryItem = "";
      this.total = 0;
      this.currentPage = 1;
    },
    handleNodeClick(data) {
      if (data.hasOwnProperty("child")) {
        let childList = data.child;
        if (childList.length <= 0) {
          this.selectCategoryItem = data;
          this.dialogSelectCategoryVisible = false;
        }
      }
    },
    // 类别弹窗
    selectCategory() {
      this.dialogSelectCategoryVisible = true;
    },
    deleteItem(item, index) {
      const scope = this;
      if (item.id) {
        scope.loading = true;
        scope
          .deletePurchaseDetails(item.id)
          .then(res => {
            if (res.code === -2) {
              throw res.messages;
            }
            scope.$message({
              message: "Delete Successfully",
              type: "success"
            });
            scope.orderItems.splice(index, 1);
            scope.loading = false;
          })
          .catch(err => {
            scope.$message({
              message: err.message,
              type: "error"
            });
            scope.loading = false;
          });
      } else {
        scope.orderItems.splice(index, 1);
      }
    },
    addItems() {
      this.selectItems.forEach((item, index) => {
        if (item.selected === true) {
          let isAdded = false;
          this.orderItems.forEach(orderItem => {
            if (orderItem.item_id === item.id) {
              isAdded = true;
            }
          });
          if (!isAdded) {
            let addItem = this.selectItems[index];
            this.orderItems.push({
              item_id: addItem.id,
              category_root_id: addItem.category_root_id,
              category_id: addItem.category_id,
              model_name: addItem.name,
              customer_art_no: "",
              bf_art_no: addItem.ext_part_no,
              quantity: 1,
              description: "",
              unit_price: 1,
              currency: 1,
              vat_rate: 17,
              selected: false,
              amount: 1,
              create_user_id: Core.Data.getUser() ? Core.Data.getUser().id : ""
            });
          }
        }
      });
      this.closeChooseItem();
    },
    closeChooseItem() {
      this.selectItems = [];
      this.queryItemList = [];
      this.queryItemCondition = "";
      this.dialogCreateVisible = false;
    },
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      if (this.formData.country === "") {
        this.$message({
          message: "Please add Country",
          type: "warning"
        });
        return false;
      }
      if (this.formData.customer_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    // 操作
    onCreate() {
      if (!this.checkParams()) {
        return;
      }
      const scope = this;
      scope.loading = true;
      this.formData.date = Core.Util.parseDate(this.formData.date);
      this.formData.etd = Core.Util.parseDate(this.formData.etd);
      this.postPIList(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.formData.date = Core.Util.changeDateForm(res.date * 1000);
          scope.formData.etd = Core.Util.changeDateForm(res.etd * 1000);
          //   scope.$router.push({
          //     name: "PIHeader",
          //     query: { orderId: res.id }
          //   });
          return res.pi_no;
        })
        .then(res => {
          const data = scope.orderItems.map(item => {
            item.pi_no = res;
            item.currency = scope.formData.currency;
            return item;
          });
          return scope.bulkPIDetails(data);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }

          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.orderItems = res.map(data => {
            data.selected = false;
            data.item_part_no = data.item_code;
            return data;
          });
          scope.loading = false;
          scope.$router.go(-1);
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    deleteOrderItem() {
      let deleteIndexs = [];
      this.orderItems.forEach((orderItem, index) => {
        if (orderItem.selected === true) {
          deleteIndexs.unshift({
            item: orderItem,
            index: index
          });
        }
      });
      deleteIndexs.forEach(data => {
        if (data.item.id) {
          this.loading = true;
          this.deletePurchaseDetails(data.item.id)
            .then(res => {
              if (res.code === -2) {
                throw res.message;
              }
              this.$message({
                message: "Delete Successfully",
                type: "success"
              });
              this.orderItems.splice(data.index, 1);

              this.loading = false;
            })
            .catch(err => {
              scope.$message({
                message: err.message,
                type: "error"
              });
              scope.loading = false;
            });
        } else {
          this.orderItems.splice(data.index, 1);
        }
      });
    },
    // 分页
    handleCurrentPageChange(page) {
      this.currentPage = page;
      this.queryOrderItem();
    },
    //获取所有顾客信息
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
    // 根据id获取PI详情
    getPIById(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/sales/proformaInvoice/" + id
      });
    },
    // 添加PI详情
    bulkPIDetails(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/detail/bulk",
        data: data
      });
    },
    // 添加PI
    postPIList(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice",
        data: data
      });
    },
    //筛选PI详情
    filterPIDetails(id) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/detail/filter",
        data: {
          pi_no: id
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //删除采购单详情
    deletePurchaseDetails(id) {
      return Core.Api.request({
        method: "DELETE",
        url: "/nl/sales/proformaInvoice/detail/" + id
      });
    },
    initCustomeInfo() {
      this.formData.country = "";
      this.formData.country_code = "";
      this.formData.address = "";
      this.formData.telephone = "";
      // this.formData.atten = data.
      this.formData.vat_no = "";
      this.formData.trade_terms = "";
      this.formData.payment_term = "";
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.PI-header {
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
  .content-width {
    min-width: 1325px;
  }
  .content {
    border: 1px solid #2b2f3b;
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
        height: 500px;
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
            width: 125px;
            margin-left: 15px;
          }
          .input,
          .customer-input {
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

    .el-input-group__prepend .el-select .el-input .el-input__inner {
      width: 92px;
    }

    .parts-list {
      position: relative;
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
        position: absolute;
        top: 0;
        right: 40px;
        color: #e26829;
      }
      .labelAdd {
        margin-left: 14px;
      }
      .cancel-button {
        float: left;
        margin-top: 12px;
      }
    }

    .table-wrap {
      .el-input-group__prepend
        .el-select.currency-input
        .el-input
        .el-input__inner {
        width: 75px;
      }
      .el-input-group--prepend .el-input__inner {
        width: 93px;
      }

      .el-input-group__prepend .el-select .el-input .el-input__inner {
        width: 108px;
      }

      .el-select .el-input.is-disabled .el-input__inner:hover {
        border-color: transparent;
      }
    }

    .footer {
      width: 100%;
      height: 60px;
      line-height: 60px;
      padding-left: 40px;
      box-shadow: 0 -1px #2b2f3b;
    }

    .table-overflow {
      overflow: hidden;
      thead td {
        padding: 10px 10px;
      }
      tr td {
        padding: 0 10px;
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
// proforma-invoice-header.vue?3a46bab6