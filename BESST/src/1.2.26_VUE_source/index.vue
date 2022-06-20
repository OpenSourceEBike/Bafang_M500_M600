<template>
  <div class="inventory-index">
    <div class="search-header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item>
          <el-select class="input" v-model="formSearch.warehouse_area_id" filterable>
            <el-option
              v-for="item in areaList"
              :key="item.id"
              :label="item.area_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-select v-model="formSearch.location_id" filterable placeholder="Location Code">
            <el-option
              v-for="item in locationList"
              :key="item.id"
              :label="item.location_code"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-select v-model="formSearch.category_id" placeholder="Category" filterable>
            <el-option
              v-for="item in categoryList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-input v-model="formSearch.product_code" placeholder="Material Code"></el-input>
        </el-form-item>
        <el-form-item>
          <el-input v-model="formSearch.product_name" placeholder="Materia Name"></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <span class="icon-item search" @click="goSearch" title="search"></span>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <button type="button" @click="onExport" class="el-button el-button--preview">Export</button>
        </div>
      </div>
    </div>
    <div class="container">
      <div class="category-tree">
        <ul>
          <li
            :class="{'is-active':item.isActive}"
            v-for="item in warehouseList"
            :key="item.id"
            @click="onTreeSelect(item)"
          >
            <img src="../../../assets/images/warehouse/warehouse.png" class="icon">
            <div class="title-box">
              <p class="title">{{item.area_name}}</p>
              <p class="title">{{item.warehouse_name}}</p>
            </div>
          </li>
        </ul>
      </div>
      <div class="list-container" v-loading.body="loading" element-loading-text="loading...">
        <div class="table-wrap">
          <div class="table-content-holder">
            <table class="table-content" border="0" cellspacing="0" cellpadding="0">
              <thead>
                <td v-for="(item,index) in headData" :key="index">{{item}}</td>
              </thead>
              <tbody>
                <tr
                  :class="{ 'isUnder': item.IsUnderYuZhi }"
                  v-for="item in inventoryList"
                  :key="item.id"
                >
                  <td>{{ item.nlInventory.warehouse_area_name}}</td>
                  <td>{{ item.nlInventory.location_code}}</td>
                  <td>{{ item.nlInventory.category_name}}</td>
                  <td>{{ item.nlInventory.product_name }}</td>
                  <td>{{ item.nlInventory.product_code }}</td>
                  <td>{{ item.nlInventory.instock_qty }}</td>
                  <td>{{ item.nlInventory.price }}</td>
                  <td>{{ item.nlInventory.price*item.nlInventory.instock_qty | money }}</td>
                  <!-- <td>{{ item.sales_qty }}</td> -->
                  <!-- <td>
                    <button class="btn-underline" @click.stop="onEdit(item)">Edit</button>
                    <button class="btn-underline" @click.stop="onDelete(item)">Delete</button>
                  </td>-->
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
      </div>
    </div>
    <!-- 保存弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogVisible" class="dialog">
      <div class="item">
        <label class="title">
          Warehouse Area
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select class="land" v-model="formData.warehouse_code" filterable slot="prepend">
            <el-option
              v-for="item in warehouseList"
              :key="item.id"
              :label="'【'+item.warehouse_name+'】'+ '-'+ item.area_name"
              :value="item.area_code"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Material
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select class="land" v-model="formData.product_code" filterable slot="prepend">
            <el-option
              v-for="item in productList"
              :key="item.id"
              :label="'【'+item.product_code+'】'+ '- ' + item.name"
              :value="item.product_code"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Instock Qty
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.instock_qty"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Stock Qty
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.stock_qty"></el-input>
        </div>
      </div>
      <!-- <div class="item">
        <label class="title">
          Sales Qty
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.sales_qty"></el-input>
        </div>
      </div>-->
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <dialog-delete
      dialogDeleteTitle="Delete Material"
      :dialogDeleteVisible="dialogDeleteVisible"
      @dialogConfirm="onDeleteConfirm"
      @dialogCancel="onDeleteCancel"
    ></dialog-delete>
  </div>
</template>
<script>
import WarehouseLocationService from "src/services/nl/warehouseLocationService.js";
import WarehouseAreaService from "src/services/nl/warehouseAreaService";
import ProductService from "src/services/nl/productService";
import WarehouseService from "src/services/nl/warehouseListService";
import InventoryService from "src/services/nl/inventoryService";
import CommonService from "src/services/nl/commonService";
import dialogDelete from "src/components/nl/dialog-delete";
import saveAs from "file-saver";
import moment from "moment";
export default {
  components: {
    dialogDelete
  },
  filters: {
    money: function(val) {
      val = val.toString().replace(/\$|\,/g, "");
      if (isNaN(val)) {
        val = "0";
      }
      let sign = val == (val = Math.abs(val));
      val = Math.floor(val * 100 + 0.50000000001);
      let cents = val % 100;
      val = Math.floor(val / 100).toString();
      if (cents < 10) {
        cents = "0" + cents;
      }
      for (var i = 0; i < Math.floor((val.length - (1 + i)) / 3); i++) {
        val =
          val.substring(0, val.length - (4 * i + 3)) +
          "," +
          val.substring(val.length - (4 * i + 3));
      }

      return (sign ? "" : "-") + val + "." + cents;
    }
  },
  data() {
    return {
      formData: {},
      formSearch: {},
      currentPage: 1,
      total: 0,
      loading: false,
      headData: [
        "Warehouse Area",
        "Location Code",
        "Category",
        "Material Name",
        "Material Code",
        "Stock Qty",
        "Unite Price",
        "Total Price"
        // "Sales Qty",
        // "Operation"
      ],
      currArea: null,
      productList: [],
      warehouseList: [],
      // 库区选择
      areaList: [],
      inventoryList: [],
      // 库位选择
      locationList: [],
      // 产品分类列表
      categoryList: [],
      // 弹框
      dialogTitle: "",
      dialogVisible: false,
      // 删除确认弹窗
      dialogDeleteVisible: false,
      // 当前删除产品ID
      currentID: null
    };
  },
  async created() {
    this.loading = true;
    await WarehouseService.findAll().then(res => {
      this.warehouseList = res.data.map((item, index) => {
        item.isActive = index == 0 ? true : false;
        return item;
      });
      this.loading = false;
    });

    this.loading = true;
    WarehouseAreaService.findAll().then(res => {
      this.areaList = res.data;
      this.loading = false;
    });

    this.loading = true;
    WarehouseLocationService.findAll().then(
      res => {
        this.locationList = res.data;
        this.loading = false;
      },
      err => {
        this.loading = false;
      }
    );

    this.loading = true;
    CommonService.getAllCategory().then(res => {
      this.categoryList = res.category_list.map((category, index) => {
        return {
          id: category.id,
          name: category.name,
          isActive: index == 0 ? true : false
        };
      });
      this.loading = false;
    });

    this.loading = true;
    await ProductService.findAll().then(res => {
      this.productList = res.data;
      this.loading = false;
    });

    this.getData();
  },
  methods: {
    goSearch() {
      this.getData();
    },
    onClean() {
      this.formSearch.product_code = "";
      this.formSearch.product_name = "";
      this.formSearch.location_id = "";
      this.formSearch.category_id = "";
      this.formSearch.warehouse_area_id = "";
      this.getData();
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      this.getData();
    },
    // 树选中状态
    onTreeSelect(item) {
      this.warehouseList.forEach(el => {
        if (el.id == item.id) {
          item.isActive = true;
        } else {
          el.isActive = false;
        }
      });
      this.getData();
    },
    // 库存
    getData() {
      this.loading = true;
      if (this.warehouseList) {
        const currentWarehouse = this.warehouseList.filter(res => {
          return res.isActive === true;
        })[0];
        this.currArea =
          (currentWarehouse && currentWarehouse.area_code) || null;
        InventoryService.filterByThreshold(
          {
            warehouse_id: currentWarehouse.id,
            warehouse_area_id: this.formSearch.warehouse_area_id,
            product_code: this.formSearch.product_code,
            product_name: this.formSearch.product_name,
            location_id: this.formSearch.location_id,
            category_id: this.formSearch.category_id
          },
          this.currentPage - 1,
          10
        ).then(res => {
          this.inventoryList = res.data.content;
          this.total = res.data.totalElements;
          this.loading = false;
        });
      }
    },
    onExport() {
      if (this.warehouseList) {
        const currentWarehouse = this.warehouseList.filter(res => {
          return res.isActive === true;
        })[0];
        var obj = {};
        if (this.formSearch.location_id) {
          obj.locationCode = this.formSearch.location_id;
        }
        if (this.formSearch.category_id) {
          obj.categoryId = this.formSearch.category_id;
        }
        if (currentWarehouse.id) {
          obj.warehouseId = currentWarehouse.id;
        }
        InventoryService.exportfile(obj).then(res => {
          const filename = "inventory-" + moment().format("YYYY-MM-DD");
          saveAs(res.data, filename + ".xlsx");
        });
      }
    },
    // onAdd() {
    //   this.dialogTitle = "添加库存";
    //   this.formData = {
    //     warehouse_code: this.currArea,
    //     product_code: "",
    //     instock_qty: 0,
    //     sales_qty: 0,
    //     stock_qty: 0
    //   };
    //   this.dialogVisible = true;
    // },
    onEdit(item) {
      this.formData = item;
      this.dialogVisible = true;
    },
    onDelete(item) {
      this.currentID = item.id;
      this.dialogDeleteVisible = true;
    },
    dialogConfirm() {
      if (this.required()) {
        this.loading = true;
        InventoryService.save(this.formData).then(res => {
          this.getData();
          this.loading = false;
          this.dialogVisible = false;
        });
      }
    },
    dialogCancel() {
      this.dialogVisible = false;
    },
    // 删除
    onDeleteConfirm() {
      this.loading = true;
      InventoryService.remove(this.currentID).then(res => {
        this.getData();
        this.loading = false;
        this.dialogDeleteVisible = false;
      });
    },
    onDeleteCancel() {
      this.dialogDeleteVisible = false;
    },
    required() {
      if (this.formData.warehouse_code === "") {
        this.$message({
          message: "Please select warehouse!",
          type: "warning"
        });
        return false;
      }

      if (this.formData.product_code === "") {
        this.$message({
          message: "Please input product !",
          type: "warning"
        });
        return false;
      }
      return true;
    }
  }
};
</script>

<style lang="scss">
.inventory-index {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  background: #2d3341;

  .list-form-inline {
    height: 100%;
    line-height: 50px;
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__label {
      color: #fff;
      width: 105px;
      text-align: left;
      margin-left: 5px;
    }
    .el-form-item__content {
      width: 180px !important;
    }
  }
  //树桩左侧筛选栏
  .category-tree {
    position: absolute;
    left: 0;
    overflow: auto;
    width: 272px;
    height: 100%;
    background: #383d49;

    ul {
      height: 100%;
      width: 100%;
    }

    li {
      border-left: 7px solid transparent;
      display: block;
    }
    li,
    .icon,
    .title {
      height: 50px;
    }

    li:hover {
      cursor: pointer;
      background-color: #2d3341;
    }

    li.is-active {
      color: #fff;
      font-weight: 700;
      border-left: 7px solid #e4662a;
      background-color: #2d3341;
    }
    .title-box {
      display: inline-block;
      height: 100%;
      width: 180px;
      .title {
        display: inline-block;
        height: auto;
        width: 100%;
        margin: 4px;
      }
    }

    .icon {
      vertical-align: middle;
      padding: 7px;
      margin: auto 15px;
      display: inline-block;
      float: left;
    }
  }
  .list-container {
    width: 100%;
    height: 100%;
    padding-left: 272px;

    .table-wrap {
      border-left: 1px solid #2d3341;
      height: 100%;
    }
    .isUnder {
      color: #e26829;
    }
  }
  .dialog {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 66px;
    }
    .item {
      span.required {
        color: #e26829;
        display: inline-block;
      }
      line-height: 30px;
      display: inline-block;
      .title,
      .land {
        display: inline-block;
        width: 120px;
      }
      .land {
        width: 250px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
      }
    }
  }
}
</style>


// WEBPACK FOOTER //
// index.vue?17f647e4