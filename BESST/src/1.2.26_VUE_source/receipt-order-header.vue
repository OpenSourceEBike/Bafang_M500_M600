<template>
  <div class="receipt-order-header">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label
        class="title"
      >{{ orderId ? 'Edit Receipt Order Customer Info' : 'Add Receipt Order Customer Info' }}</label>
    </div>
    <div class="content-wrap">
      <div class="content" v-loading.body="loading">
        <div class="order-info">
          <label class="title">Customer Info</label>
          <div class="left">
            <!-- <div class="item">
              <label class="label">
                Receipt No.
                <span class="required">*</span>
              </label>
              <el-input
                class="input"
                v-model="formData.receipt_order_no"
                placeholder="Receipt No."
                disabled
              ></el-input>
            </div> -->
            <div class="item">
              <label class="label">
                Order No.
                <span class="required">*</span>
              </label>
              <el-input
                class="input input-sn"
                v-model="formData.order_no"
                @change="getEecOrderDetails()"
              ></el-input>
            </div>
            <div class="item">
              <label class="label">
                Warehouse
                <span class="required">*</span>
              </label>
              <el-select v-model="formData.warehouse_id" slot="append" class="input" filterable>
                <el-option
                  v-for="item in warehouseList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <!-- <div class="item">
              <label class="label">
                Storer
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.store_id" filterable slot="prepend">
                <el-option label="test" value="1"></el-option>
              </el-select>
            </div>-->
            <div class="item">
              <label class="label">
                Supplier
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.supplier_id" filterable slot="prepend">
                <el-option
                  v-for="item in supplierData"
                  :key="item.id"
                  :label="item.company_name"
                  :value="item.id"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Source Type
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.from_type_code" filterable slot="prepend">
                <el-option
                  v-for="item in sourceType"
                  :key="item.id"
                  :label="item.value"
                  :value="item.value"
                ></el-option>
              </el-select>
            </div>
            <!-- <div class="item">
              <label class="label">
                Receipt Type
                <span class="required">*</span>
              </label>
              <el-select
                class="input"
                v-model="formData.receipt_type_code"
                filterable
                slot="prepend"
              >
                <el-option label="test" value="1"></el-option>
              </el-select>
            </div>-->
            <div class="item">
              <label class="label">Receipt Date</label>
              <el-date-picker
                class="input"
                v-model="receiptDate"
                type="date"
                placeholder="Receipt Date"
              ></el-date-picker>
            </div>
            <!-- <div class="item">
              <label class="label">Receipt User</label>
              <el-select class="input" v-model="formData.receipt_user" filterable slot="prepend">
                <el-option label="test" value="0"></el-option>
              </el-select>
            </div>-->
            <!-- <div class="item">
              <label class="label">Check User</label>
              <el-select
                class="input"
                v-model="formData.quality_check_user"
                filterable
                slot="prepend"
              >
                <el-option label="test" value="0"></el-option>
              </el-select>
            </div>-->
            <div class="item">
              <label class="label">Description</label>
              <el-input class="input" v-model="formData.description"></el-input>
            </div>
          </div>
        </div>
        <!-- Table Items List -->
        <div class="table-wrap">
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
                <td>{{item.item_name}}</td>
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
        <div class="parts-list">
          <el-button type="primary" @click="createOrder" v-if="isActive">SAVE</el-button>
          <!-- <el-button type="primary" @click="goAddItemPage" v-if="isEditPage">NEXT</el-button> -->

          <el-button
            type="primary"
            @click="createAdvice"
            v-if="orderItems.length>0 && !isActive"
          >Advice</el-button>
        </div>
        <!--  Dialog for Item  -->
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

export default {
  data() {
    return {
      loading: false,
      isEditPage: false,
      receiptDate: new Date(),
      formData: {
        warehouse_id: "",
        store_id: "",
        supplier_id: "",
        receipt_order_no: "",
        from_type_code: "",
        receipt_type_code: "",
        receipt_date: "",
        receipt_user: "",
        quality_check_user: "",
        description: "",
        order_no: ""
      },
      tableHead: [
        "Item Name",
        "BF Art Nr",
        "Specification",
        "Quantity",
        "Operation"
      ],
      itemHeadData: ["Image", "Name", "Part No.", "Firmware"],
      queryItemList: [],
      selectItems: [],
      orderItems: [],

      orderId: 0,
      isActive: false,
      queryItemCondition: "",
      purchase_order_id: "",

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
      warehouseList: [],
      departmentList: [],
      areaCodeList: [],
      orderId: 0,
      isActive: false,
      supplierData: [],
      sourceType: [
        {
          id: 0,
          value: "Purchase"
        },
        {
          id: 1,
          value: "Other"
        }
      ],
      deleteStatus: false,
      timer: ""
    };
  },
  i18n: {
    messages: {}
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
    this.areaCodeList = areaCodeInDB.init();
    this.orderId = this.$route.query.orderId;
    this.getSupplier();
    if (this.orderId) {
      this.isEditPage = true;

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

      this.loading = true;
      this.getReceipt(this.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.receiptDate = Core.Util.changeDateForm(res.receipt_date * 1000);
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
      // 查询之前添加过的Items
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
    } else {
      this.isActive = true;
    }

    this.loading = true;
    this.getAllDepartment()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.loading = false;
        scope.departmentList = res;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    this.loading = true;
    this.getAllWarehouse()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.loading = false;
        scope.warehouseList = res;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    // 获取所有的零件信息
    scope.getAllCategory();
  },

  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "receiptOrderList" });
    },
    //根据order No查询信息，然后进行页面联动
    getEecOrderDetails() {
      if (this.formData.order_no === "") return;
      if (this.timer) {
        clearTimeout(this.timer);
      }
      this.timer = setTimeout(() => {
        this.loading = true;
        const _orderNo = this.formData.order_no.trim();
        this.getPurchase(_orderNo)
          .then(res => {
            this.loading = false;
            if (res.content.length > 0) {
              this.formData.supplier_id = res.content[0].supplier_id;
              this.purchase_order_id = res.content[0].id;
              return this.filterPurchaseDetails(this.purchase_order_id);
            }
            throw "Order No. not find";
          })
          .then(res => {
            this.orderItems = res.content.map(orderItem => {
              orderItem.receipt_quantity = orderItem.quantity;
              orderItem.selected = false;
              orderItem.item_part_no = orderItem.item_code;
              return orderItem;
            });
            // this.currentPage = page.;
            this.totalForDetail = res.totalElements;
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

    goAddItemPage() {
      this.$router.push({
        name: "receiptOrderItemList",
        query: { orderId: this.orderId }
      });
    },

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
      this.loading = true;
      this.formData.receipt_date = Core.Util.parseDate(this.receiptDate) || 0;
      this.formData.create_user_id = Core.Data.getUser()
        ? Core.Data.getUser().id
        : "";
      this.postReceipt(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.formData = JSON.parse(JSON.stringify(res));
          this.receiptDate = Core.Util.changeDateForm(res.receipt_date * 1000);

          this.orderId = res.id;
          this.isEditPage = true;
          this.loading = false;

          const data = this.orderItems.map(orderItem => {
            orderItem.item_code = orderItem.item_part_no;
            orderItem.receipt_order_id = this.orderId;
            return orderItem;
          });

          return this.postReceiptDetail(data);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          return scope.filterReceiptDetail({
            receipt_order_id: scope.orderId
          });
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.orderItems = res.content.map(orderItem => {
            orderItem.selected = false;
            orderItem.item_part_no = orderItem.item_code;
            return orderItem;
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

      // const data = this.orderItems.map(orderItem => {
      //   orderItem.item_code = orderItem.item_part_no;
      //   orderItem.receipt_order_id = this.orderId - 0;
      //   return orderItem;
      // });

      // this.postReceiptDetail(data)
      //   .then(res => {
      //     if (res.code === -2) {
      //       throw res.message;
      //     }
      //     scope.$message({
      //       message: "Save Successfully",
      //       type: "success"
      //     });
      //     return scope.filterReceiptDetail({
      //       receipt_order_id: scope.orderId
      //     });
      //   })
      //   .then(res => {
      //     if (res.code === -2) {
      //       throw res.message;
      //     }
      //     scope.orderItems = res.content.map(orderItem => {
      //       orderItem.selected = false;
      //       orderItem.item_part_no = orderItem.item_code;
      //       return orderItem;
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

    //获取purchase order number的详细信息
    getPurchase(orderNo) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/order/purchase/filter",
        data: {
          purchase_order_no: orderNo
        }
      });
    },
    //获取采购单的详细item信息
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
    // 弹窗必填
    checkParams() {
      if (!this.formData.order_no) {
        this.$message({
          message: "Please add Order No.",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.warehouse_id) {
        this.$message({
          message: "Please add Warehouse",
          type: "warning"
        });
        return false;
      }
      if (!this.formData.supplier_id) {
        this.$message({
          message: "Please add Supplier",
          type: "warning"
        });
        return false;
      }
    if (!this.formData.from_type_code) {
        this.$message({
          message: "Please add Source type",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    //获取状态
    getReceiptStatus(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id + "/status"
      });
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
    getReceipt(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id
      });
    },
    // 保存入库信息
    postReceipt(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/receipt",
        data: data
      });
    },
    //获取供应商
    getSupplier() {
      this.loading = true;
      return Core.Api.request({
        method: "get",
        url: "/nl/nlPublic/supplier/all"
      })
        .then(res => {
          if (res.code === -2) {
            throw res.messages;
          }
          this.supplierData = res;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
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
.receipt-order-header {
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
      height: 332px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 282px;
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
  .pagination {
    padding: 10px;
  }
}
</style>



// WEBPACK FOOTER //
// receipt-order-header.vue?96858686