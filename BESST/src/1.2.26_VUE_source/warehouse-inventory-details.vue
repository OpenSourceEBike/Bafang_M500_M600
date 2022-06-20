<template>
  <div class="set-header">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">Inventory details</label>
      <div class="btn-group-top">
        <div class="icons">
          <div
            class="icon-item add-v2"
            @click="onSetMinInventory"
            v-if="true"
            title="add"
            v-ilpermission="'warehouse-area-add'"
          ></div>
        </div>
      </div>
    </div>

    <!-- Table Items List -->
    <div class="table-wrap" v-loading="loading">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="orderItems.length <= 0 && !loading">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0">
          <thead>
            <td v-for="item in tableHead">{{item}}</td>
          </thead>
          <tbody>
            <tr v-for="(item, index) in orderItems">
              <td>{{item.item_name}}</td>
              <td>{{item.item_code}}</td>
              <td>
                {{item.remind_quantity}}
                <!-- <el-input-number class="input-number" v-model="item.remind_quantity" :min="1"></el-input-number> -->
              </td>
              <td>
                <a class="btn-underline" @click.stop="onEdit(item)">edit</a>
                <button
                  class="btn-underline"
                  @click.stop="deleteItem(item,index)"
                >{{ $t('delete') }}</button>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
      <div class="table-footer">
        <el-pagination
          @current-change="handleCurrentChange"
          :current-page.sync="currentPage"
          :page-size="10"
          layout="prev, pager, next, jumper"
          :total="total"
        ></el-pagination>
        <p class="total">total: {{total}}</p>
      </div>
    </div>
    <!-- 新增弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogAddVisible" class="dialog-area-list">
      <div class="item">
        <div class="item-box">
          <label class="label">
            Item name
            <span class="required">*</span>
          </label>
          <el-input v-model="invenEidtPostData.item_name" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Item code
            <span class="required">*</span>
          </label>
          <el-input v-model="invenEidtPostData.item_code" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Remind quantity
            <span class="required">*</span>
          </label>
          <el-input v-model="invenEidtPostData.remind_quantity" class="input"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogAddVisible = false">cancel</el-button>
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
      queryItemList: [],
      selectItems: [],
      orderItems: [],
      queryItemCondition: "",

      currentPage: 1,
      total: 0,

      selectCategoryItem: {
        id: "",
        name: ""
      },
      //edit post data
      invenEidtPostData: {},
      // 新增弹窗 标题
      dialogAddVisible: false,
      dialogTitle: ""
    };
  },
  i18n: {
    messages: {}
  },
  computed: {},
  mounted: function() {
    const scope = this;
    this.warehouse_id = this.$route.query.warehouse_id;
    this.loading = true;
    this.getAllItemInven(this.warehouse_id);
  },

  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "warehouseArea" });
    },
    onSetMinInventory() {
      this.$router.push({
        name: "warehouseSetMinInventory",
        query: {
          warehouse_id: this.warehouse_id
        }
      });
    },

    // 操作 编辑、删除
    onEdit(event) {
      this.dialogTitle = "Edit item inventory";
      this.invenEidtPostData = JSON.parse(JSON.stringify(event));
      this.dialogAddVisible = true;
    },
    deleteItem(item, index) {
      const scope = this;
      if (item.id) {
        scope.loading = true;
        scope.deleteInvenDetail(item.id).then(
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
    // 弹框 确认-取消
    dialogConfirm() {
      if (!this.checkParams()) {
        return;
      }
      this.loading = true;
      this.postInvenReminData(this.invenEidtPostData).then(
        res => {
          this.$message({
            message: "Edit Successfully",
            type: "success"
          });
          this.loading = false;
          this.dialogAddVisible = false;
          this.getAllItemInven(this.warehouse_id);
        },
        err => {
          this.loading = false;
        }
      );
    },

    clearCondition() {
      this.selectItems = [];
      this.queryItemList = [];
      this.queryItemCondition = "";
      this.selectCategoryItem = "";
      this.total = 0;
      this.currentPage = 1;
    },

    // 弹窗必填
    checkParams() {
      if (!this.invenEidtPostData.remind_quantity) {
        this.$message({
          message: "Please add the number of items",
          type: "warning"
        });
        return false;
      }
      return true;
    },

    // 保存新增的库存限制信息
    postInvenReminData(data) {
      return Core.Api.request({
        method: "POST",
        url: "/nl/warehouse/inventoryReminder",
        data: data
      });
    },

    //获取所有warehouse下的item
    getAllItemInven(id) {
      const scope = this;
      return Core.Api.request({
        method: "POST",
        url: "/nl/warehouse/inventoryReminder/filter",
        data: {
          warehouse_id: id,
        },
        params:{
          page: this.currentPage,
          size: 10
        }
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.loading = false;
          scope.orderItems = res.content;
          this.total = res.totalElements;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },

    // 分页查询
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getAllItemInven();
    },

    //删除单列表数据
    deleteInvenDetail(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/warehouse/inventoryReminder/" + id
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

  .dialog-area-list {
    .el-dialog__body {
      padding: 30px 67px;
    }
    .item {
      margin: 20px 0;
      font-size: 0;
      height: 30px;
      .item-box {
        line-height: 30px;
      }
      .label {
        display: inline-block;
        width: 120px;
        height: 30px;
        vertical-align: middle;
      }
      .input {
        margin-left: 40px;
        width: 260px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// warehouse-inventory-details.vue?4c661982