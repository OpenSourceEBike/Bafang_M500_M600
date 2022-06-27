<template>
  <div class="purchase-details">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goPurchaseList" title="back"></div>
        </div>
      </div>
      <label class="title">Purchase List Details</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Purchase Order</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Object
                <span class="required">*</span>
              </label>
              <el-select v-model="formData.supplier_id" slot="append" class="input" filterable>
                <el-option
                  v-for="item in object"
                  :key="item.id"
                  :label="item.company_name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Customer
                <span class="required">*</span>
              </label>
              <el-select
                class="input"
                v-model="formData.customer_id"
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
            </div>
            <div class="item">
              <label class="label">Order No.</label>
              <el-input
                class="input"
                :disabled="true"
                v-model="formData.purchase_order_no"
                placeholder="Order No."
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Purpose
                <span class="required">*</span>
              </label>
              <el-select v-model="formData.purchase_type" slot="append" class="input" filterable>
                <el-option
                  v-for="item in purpose"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">Creater</label>
              <el-input
                class="input"
                :disabled="true"
                v-model="formData.create_user_id"
                placeholder="Creater"
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Date
                <span class="required">*</span>
              </label>
              <el-date-picker class="input" v-model="date" type="date" placeholder="Date"></el-date-picker>
            </div>
          </div>
        </div>
        <div class="table-wrap">
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
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.item_part_no}}</td>
                <td>{{item.name || item.item_name}}</td>
                <td>
                  <el-input-number class="input-number" v-model="item.quantity" :min="1"></el-input-number>
                </td>
                <td>
                  <a class="btn-underline" v-on:click="deleteItem(item,index)">{{ $t('delete') }}</a>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list">
          <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
        </div>
        <div class="parts-list">
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
                :disabled="true"
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
                  <div class="icon-item search" @click="queryOrderItem" :title="$t('search')"></div>
                  <div class="icon-item clear" @click="clearCondition" :title="$t('clear')"></div>
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
                      <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
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
          <el-tree :data="categoryList" :props="defaultProps" @node-click="handleNodeClick"></el-tree>
        </div>
      </div>
    </el-dialog>
  </div>
</template>;

<script>
import Core from "core/core";
import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";
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
      date: new Date(),
      formData: {
        date: "",
        supplier_id: "",
        purchase_type: "",
        create_user_id: "",
        purchase_order_no: "",
        customer_id: ""
      },
      categoryList: [],
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
      object: [],
      customerList: []
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
  created() {},
  computed: {
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
      "BF Art Nr.",
      "Specification",
      "Quantity",
      "Operation"
    ];
    this.getAllCustomer();
    this.getAllCategory();
    const scope = this;
    scope.lodaing = true;
    scope.getSupplier().then(
      res => {
        scope.lodaing = false;
        scope.object = res.map(data => {
          return data;
        });
      },
      err => {
        scope.lodaing = false;
      }
    );
    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      scope.lodaing = true;
      scope
        .getPurchaseById(scope.orderId)
        .then(res => {
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.date = Core.Util.changeDateForm(scope.formData.date * 1000);
        })
        .then(() => {
          return scope.filterPurchaseDetails(scope.orderId);
        })
        .then(res => {
          scope.orderItems = res.content.map(data => {
            data.selected = false;
            data.item_part_no = data.item_code;
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
    }
  },
  methods: {
    //返回
    goPurchaseList() {
      this.$router.push({ name: "purchaseList" });
    },
    // 获取类别
    getAllCategory() {
      this.loading = true;
      Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
      ).then(
        res => {
          this.categoryList = res.category_list;
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
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
        scope.deletePurchaseDetails(item.id).then(
          res => {
            if (res.code === -2) {
              throw res.messages;
            }
            scope.$message({
              message: "Delete Successfully",
              type: "success"
            });
            scope.loading = false;
            scope.orderItems.splice(index, 1);
          },
          err => {
            scope.loading = false;
          }
        );
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
              item_name: addItem.name,
              item_part_no: addItem.ext_part_no,
              quantity: 1,
              selected: false
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
    // 操作
    onCreate() {
      const scope = this;
      scope.formData.date = Core.Util.parseDate(scope.date);
      if (!this.required()) {
        return;
      }
      scope.formData.create_user_id = Core.Data.getUser().id;
      scope.loading = true;
      this.postPurchase(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.date = Core.Util.changeDateForm(res.date * 1000);

          // scope.$router.push({
          //   name: "purchaseOrderDetails",
          //   query: { orderId: res.id }
          // });
          return (this.orderId = res.id);
        })
        .then(res => {
          const data = scope.orderItems.map(item => {
            item.item_code = item.item_part_no;
            item.purchase_order_id = scope.orderId - 0;
            item.quantity = item.quantity;
            return item;
          });
          return scope.bulkPurchaseDetails(data);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$router.go(-1);
          scope.$message({
            message: "Save Successfully",
            type: "success"
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
    // 分页
    handleCurrentPageChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
    },
    // 客户
    getAllCustomer() {
      const scope = this;
      scope.loading = true;
      WarehouseService.getAllCustomer()
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

    // 根据id获取采购单详情
    getPurchaseById(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/order/purchase/" + id
      });
    },
    // 添加采购单详情
    bulkPurchaseDetails(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/detail/bulk",
        data: data
      });
    },
    // 添加采购信息
    postPurchase(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase",
        data: data
      });
    },
    //获取供货商
    getSupplier() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/supplier/all"
      });
    },
    //筛选采购单详情
    filterPurchaseDetails(id) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/detail/filter",
        data: {
          purchase_order_id: id
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
        url: "/nl/order/purchase/detail/" + id
      });
    },
    //字段必填项
    required() {
      if (this.formData.supplier_id === "") {
        this.$message({
          message: "Please input Object",
          type: "warning"
        });
        return false;
      }

      if (this.formData.purchase_type === "") {
        this.$message({
          message: "Please input Purpose",
          type: "warning"
        });
        return false;
      }

      if (this.formData.date === "") {
        this.$message({
          message: "Please input Date",
          type: "warning"
        });
        return false;
      }

      // if (this.formData.purchase_order_no === "") {
      //   this.$message({
      //     message: "Please input Order No.",
      //     type: "warning"
      //   });
      //   return false;
      // }

      // if (this.formData.create_user_id === "") {
      //   this.$message({
      //     message: "Please input Creater",
      //     type: "warning"
      //   });
      //   return false;
      // }
      return true;
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.purchase-details {
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
  .content {
    border: 1px solid #2b2f3b;
    width: 1080px;
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
        height: 175px;
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
      .cancel-button {
        float: left;
        margin-top: 12px;
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

  .pagination {
    padding: 10px;
  }
}
</style>



// WEBPACK FOOTER //
// purchase-order-details.vue?b52f09de