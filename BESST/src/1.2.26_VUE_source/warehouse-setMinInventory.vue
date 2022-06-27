<template>
  <div class="set-header">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">Setting Min Inventory</label>
    </div>
    <div class="content-wrap">
      <div class="content" >
        <!-- Table Items List -->
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <!-- <td>
                <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
              </td>-->
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <!-- <td>
                  <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                </td>-->
                <td>{{item.item_name}}</td>
                <td>{{item.item_code}}</td>
                <td>
                  <!-- {{item.remind_quantity}} -->
                  <el-input-number class="input-number" v-model="item.remind_quantity" :min="1"></el-input-number>
                </td>
                <!-- <td>{{item.receipt_quantity}}</td> -->
                <td>
                  <button
                    class="btn-underline"
                    @click.stop="deleteItem(item,index)"
                    :disabled="deleteStatus"
                  >delete</button>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list">
          <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
        </div>
        <div class="parts-list">
          <el-button type="primary" @click="createOrder" v-if="isSave">SAVE</el-button>
        </div>
        <!--  Dialog for All-Item-select  -->
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
                  <div class="icon-item search" @click="queryOrderItem" title="search"></div>
                  <div class="icon-item clear" @click="clearCondition" title="clear"></div>
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
export default {
  data() {
    return {
      loading: false,
      warehouse_id: "",
      tableHead: ["Item Name", "Item Code", "Remind quantity", "Operation"],
      itemHeadData: ["Image", "Name", "Part No.", "Firmware"],
      queryItemList: [],
      selectItems: [],
      orderItems: [],

      isSave: false,
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
      isSave: false,
      deleteStatus: false,
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
    }
  },
  mounted: function() {
    const scope = this;
    this.warehouse_id = this.$route.query.warehouse_id;
    this.loading = true;
    // 获取所有的零件信息
    scope.getAllCategory();
  },

  methods: {
    //保存设置的库存最小值信息
    createOrder() {
      const scope = this;
      const data = this.orderItems.map(orderItem => {
        orderItem.item_code = orderItem.item_code;
        orderItem.item_name = orderItem.item_name;
        orderItem.remind_quantity = orderItem.remind_quantity;
        return orderItem;
      });
      this.postInvenSave(data)
        .then( res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.isSave = false;
          this.$router.go(-1);
        })
        .catch( err => {
          if(err.indexOf("DuplicateKeyException") > 0){
            scope.$message({
              message: "This item already exists",
              type: "error"
            });            
          } else {
            scope.$message({
                message: err,
                type: "error"
            });              
          }
          scope.loading = false;
        });
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ 
        name: "warehouseInventoryDetails" ,
        query: {
          warehouse_id: this.warehouse_id
        }
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
    // 操作
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
      const scope = this;
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
            scope.orderItems.push({
              item_name: addItem.name,
              item_code: addItem.ext_part_no,
              selected: false,
              type: 0,
              warehouse_id: scope.warehouse_id
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
      this.isSave = true;
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

    // 弹窗必填
    checkParams() {
      if (!this.warehouse_id) {
        this.$message({
          message: "Please add Warehouse",
          type: "warning"
        });
        return false;
      }
      return true;
    },

    // 保存新增的库存限制信息
    postInvenSave(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/warehouse/inventoryReminder/bulk",
        data: data
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
.set-header {
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
// warehouse-setMinInventory.vue?b533f536