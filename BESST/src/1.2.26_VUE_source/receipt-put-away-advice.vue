<template>
  <div class="receipt-put-away-advice">
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="$router.back()" title="back"></div>
        </div>
      </div>
      <label class="title">Receipt Put Away Advice</label>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content content-set">
        <div class="parts-list" v-if="orderItems.length <= 0 && !loading">
          <div class="no-data-content">No material space was found. Please create a space.</div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="item in tableHead">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item, index) in orderItems">
                <td>{{item.item_name}}</td>
                <td>{{item.item_code}}</td>
                <td>{{warehousePipe(item)}}</td>
                <td>{{item.location_no || 'No location'}}</td>
                <td>{{item.put_away_quantity}}</td>
                <td>{{item.received_quantity}}</td>
                <td>
                  <el-button
                    type="primary"
                    @click="createOrder(item)"
                    :disabled="item.location_id?true:false"
                  >Create</el-button>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="footer" v-if="isAdvice && !noLocation">
          <el-button type="primary" @click="saveAdvice">Save Advice</el-button>
        </div>
      </div>
    </div>
    <!-- 新增弹窗 -->
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogAddVisible"
      custom-class="edit-dialog"
      class="dialog-area-details"
    >
      <div class="item">
        <label class="title">
          Area
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select v-model="areaLocationData.area_id" slot="append" class="input" filterable>
            <el-option
              v-for="item in areaList"
              :key="item.id"
              :label="item.area_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Location No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="areaLocationData.location_no"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Location Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="areaLocationData.location_name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          {{$t('category')}}
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input
            :placeholder="$t('category')"
            class="input"
            :disabled="true"
            v-model="selectCategoryItem.name"
            @click.native="selectCategory"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Max Quantity
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="areaLocationData.max_quantity"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Max Cartons</label>
        <div class="content">
          <el-input v-model="areaLocationData.max_cartoons"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Weight Capacity (Kg)</label>
        <div class="content">
          <el-input v-model="areaLocationData.weight_capacity"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Cube (m&sup3;)</label>
        <div class="content">
          <el-input v-model="areaLocationData.cube"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Height (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.height"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Length (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.length"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Width (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.width"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Type Code</label>
        <div class="content">
          <el-input v-model="areaLocationData.type_code"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">X Coord (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.x_coord"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Y Coord (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.y_coord"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Z Coord (m)</label>
        <div class="content">
          <el-input v-model="areaLocationData.z_coord"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 分类  -->
    <el-dialog
      class="dialog-category"
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

import ZH from "src/assets/lang/zh";
import EN from "src/assets/lang/en";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";

export default {
  data() {
    return {
      loading: false,
      tableHead: [
        "Item Name",
        "Item Code",
        "Warehouse",
        "Location No.",
        "Put Away Quantity",
        "Received Quantity",
        "Operation"
      ],
      orderItems: [],
      orderId: 0,
      warehouseList: [],
      isAdvice: false,
      noLocation: false,
      //货区表单
      areaLocationData: {},
      // 弹窗标题
      dialogTitle: "Add",
      // 弹窗隐显
      dialogAddVisible: false,
      defaultProps: {
        id: "id",
        children: "child",
        label: "name"
      },
      categoryList: [],
      selectCategoryItem: {
        id: "",
        name: ""
      },
      dialogSelectCategoryVisible: false,
      areaList: []
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
  mounted: function() {
    const scope = this;

    scope.orderId = this.$route.query.orderId;
    if (scope.orderId) {
      this.getAllCategory();

      scope.loading = true;
      this.getAllWarehouse().then(res => {
        this.warehouseList = res;
        scope.loading = false;
      });

      scope.loading = true;
      scope
        .getReceiptStatus(scope.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          if (res.status_name === "Approved") {
            scope.isAdvice = true;
            return scope.getPutawayAdvice(scope.orderId);
          } else {
            scope.isAdvice = false;
            return scope.getPutware(scope.orderId);
          }
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.loading = false;
          scope.orderItems = res;
          scope.noLocation = scope.checkLocation(res);
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

  methods: {
    // pipe
    warehousePipe(item) {
      if (this.warehouseList.length > 0) {
        const data = this.warehouseList.filter(res => {
          return res.id === item.warehouse_id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    //创建库位
    createOrder(item) {
      this.selectCategoryItem.name = item.item_name;
      this.selectCategoryItem.id = item.item_id;
      this.areaLocationData.warehouse_id = item.warehouse_id;
      this.getWarehouseArea(item.warehouse_id).then(res => {
        this.areaList = res.content;
        this.dialogAddVisible = true;
      });
    },
    //判断库位是否充足
    checkLocation(list) {
      let noLocation = false;
      const no_location_count = list.filter(res => {
        return !res.location_id;
      });
      if (no_location_count.length > 0) {
        noLocation = true;
      }
      return noLocation;
    },
    checkParams() {
      if (!this.areaLocationData.area_id) {
        this.$message({
          message: "Please add area",
          type: "warning"
        });
        return false;
      }

      if (!this.areaLocationData.location_no) {
        this.$message({
          message: "Please add Location No.",
          type: "warning"
        });
        return false;
      }
      if (!this.areaLocationData.location_name) {
        this.$message({
          message: "Please add Location Name",
          type: "warning"
        });
        return false;
      }
      if (!this.selectCategoryItem.name) {
        this.$message({
          message: "Please add Category",
          type: "warning"
        });
        return false;
      }

      if (!this.areaLocationData.max_quantity) {
        this.$message({
          message: "Please add Max Quantity",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    // 弹窗确认事件
    dialogConfirm() {
      if (!this.checkParams()) {
        return;
      }
      this.areaLocationData.category_code = this.selectCategoryItem.name;
      this.loading = true;
      this.postAreaLocation(this.areaLocationData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          return this.getPutawayAdvice(this.orderId);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.dialogAddVisible = false;
          this.loading = false;
          this.orderItems = res;
          this.areaLocationData = {};
          this.noLocation = this.checkLocation(res);
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogAddVisible = false;
          this.loading = false;
        });
    },
    // 弹窗取消事件
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    // 选择分类显示弹窗
    selectCategory() {
      this.dialogSelectCategoryVisible = true;
    },
    // 分类选中
    handleNodeClick(data) {
      if (data.hasOwnProperty("child")) {
        let childList = data.child;
        if (childList.length <= 0) {
          this.selectCategoryItem = data;
          this.dialogSelectCategoryVisible = false;
        }
      }
    },
    // 保存建议单
    saveAdvice() {
      const scope = this;
      const id = this.$route.query.orderId;
      scope.loading = true;
      scope
        .postPutawayAdvice(id, scope.orderItems)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.isAdvice = false;
          return this.getPutware(id);
        })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.orderItems = res;
          this.$router.go(-1);
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 获取所有分类
    getAllCategory() {
      this.loading = true;
      Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
      ).then(res => {
        this.categoryList = res.category_list;
        this.loading = false;
      });
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    // 获取所选仓库地区详情
    getWarehouseArea(warehouseId) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse/area/filter2",
        data: {
          warehouse_id: warehouseId
        },
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //保存入库所需物件列表
    postPutawayAdvice(id, data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/receipt/" + id + "/putware/advice",
        data: data
      });
    },
    //查询入库建议列表
    getPutawayAdvice(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id + "/putware/advice"
      });
    },
    //获取状态
    getReceiptStatus(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id + "/status"
      });
    },
    getPutware(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/receipt/" + id + "/putware"
      });
    },
    //新增保存库位
    postAreaLocation(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse/location",
        data: data
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.receipt-put-away-advice {
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
  .content-wrap {
    .content {
      border: 1px solid #2b2f3b;
      .parts-list {
        position: relative;
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
        .no-data-content {
          display: inline-block;
        }
        .btn-box {
          display: inline-block;
          text-align: right;
          margin-left: 15px;
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
  }

  .table-wrap .table-content {
    padding-bottom: 0;
  }
  .pagination {
    padding: 10px;
  }
  .dialog-area-details {
    overflow: hidden;
    .el-dialog__body {
      padding: 19px 64px 19px 64px;
      font-size: 0;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .land {
        width: 200px;
      }
      .title {
        display: inline-block;
        width: 135px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 200px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// receipt-put-away-advice.vue?74e1cca4