<template>
  <div class="receipt-order-item-list">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">{{ orderId ? 'Edit Package List' : 'Add Package List' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content content-set">
        <div class="table-wrap" v-loading.body="loading">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-if="isActive">
                <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
              </td>
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <td v-if="isActive">
                  <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                </td>
                <td>{{item.category_root_id | categoryRoot}}</td>
                <td>{{item.item_part_no}}</td>
                <td>{{item.name || item.item_name}}</td>
                <td v-if="isActive">
                  <el-input-number class="input-number" v-model="item.receipt_quantity" :min="1"></el-input-number>
                </td>
                <td v-else>{{item.receipt_quantity}}</td>
                <td>
                  <button
                    class="btn-underline"
                    @click.stop="deleteItem(item,index)"
                    :disabled="deleteStatus"
                  >{{ $t('delete') }}</button>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list" v-if="isActive">
          <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
        </div>
        <div class="footer">
          <el-button type="delete" v-if="isActive" @click="deleteOrderItem">{{ $t('delete') }}</el-button>
          <el-button type="primary" v-if="isActive" @click="createOrder">{{ $t('saveOrder') }}</el-button>
          <el-button
            type="primary"
            @click="createAdvice"
            v-if="orderItems.length>0 && !isActive"
          >Advice</el-button>
        </div>
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
</template>

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";

import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      warehouseList: [],
      departmentList: [],
      areaCodeList: [],
      tableHead: [
        "Item Name",
        "BF Art Nr",
        "Specification",
        "Quantity",
        "Operation"
      ],
      itemHeadData: ["Image", "Name", "Part No.", "Firmware"],
      queryItemList: [],
      orderItems: [],
      selectItems: [],

      orderId: 0,
      queryItemCondition: "",

      allChecked: false,
      currentPage: 1,
      total: 0,
      dialogCreateVisible: false,
      dialogSelectCategoryVisible: false,
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
      isActive: false,
      deleteStatus: false
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
  mounted: function() {
    const scope = this;
    scope.areaCodeList = areaCodeInDB.init();

    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      this.loading = true;
      this.getReceiptStatus(scope.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          res.status_name === "Created"
            ? ((scope.isActive = true), (scope.deleteStatus = false))
            : ((scope.isActive = false), (scope.deleteStatus = true));
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });

      scope.loading = true;
      this.filterReceiptDetail({
        receipt_order_id: scope.orderId
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.orderItems = res.content.map(orderItem => {
            orderItem.selected = false;
            orderItem.item_part_no = orderItem.item_code;
            return orderItem;
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

    scope.loading = true;
    this.getAllDepartment().then(res => {
      this.departmentList = res;
      scope.loading = false;
    });
    scope.loading = true;
    this.getAllWarehouse().then(res => {
      this.warehouseList = res;
      scope.loading = false;
    });

    this.getAllCategory();
  },

  methods: {
    //获取状态
    getReceiptStatus(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id + "/status"
      });
    },
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

    goOrderList() {
      this.$router.push({ path: "list" });
    },

    deleteItem(item, index) {
      const scope = this;
      if (item.id) {
        scope.loading = true;
        scope.deleteReceiptDetail(item.id).then(
          res => {
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
              receipt_quantity: 1,
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
    //创建建议单
    createAdvice() {
      this.$router.push({
        name: "receiptPutAwayAdvice",
        query: { orderId: this.orderId }
      });
    },
    createOrder() {
      const scope = this;
      if (!this.checkParams()) {
        return;
      }
      const data = this.orderItems.map(orderItem => {
        orderItem.item_code = orderItem.item_part_no;
        orderItem.receipt_order_id = this.orderId - 0;
        return orderItem;
      });
      this.loading = true;
      this.postReceiptDetail(data)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          return scope.filterReceiptDetail({
            receipt_order_id: scope.orderId
          });
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.orderItems = res.content.map(orderItem => {
            orderItem.selected = false;
            orderItem.item_part_no = orderItem.item_code;
            return orderItem;
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
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      return true;
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
          this.deleteReceiptDetail(data.item.id).then(
            res => {
              this.$message({
                message: "Delete Successfully",
                type: "success"
              });
              this.loading = false;
            },
            err => {
              this.loading = false;
            }
          );
          this.orderItems.splice(data.index, 1);
        } else {
          this.orderItems.splice(data.index, 1);
        }
      });
    },

    selectCategory() {
      this.dialogSelectCategoryVisible = true;
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

    handleCurrentPageChange(page) {
      this.currentPage = page;
      this.queryOrderItem();
    },
    //获取所有部门
    getAllDepartment() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/department/all"
      });
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    //保存入库所需物件列表
    postReceiptDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/receipt/detail/bulk",
        data: data
      });
    },
    //查询入库物件列表
    filterReceiptDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/receipt/detail/find-receipt-detail-by-order-no",
        data: data,
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //删除入库单列表数据
    deleteReceiptDetail(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/order/receipt/detail/" + id
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.receipt-order-item-list {
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
  .content {
    border: 1px solid #2b2f3b;
    .parts-list {
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
// receipt-order-item-list.vue?4b23a493