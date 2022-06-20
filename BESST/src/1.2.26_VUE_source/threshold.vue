<template>
  <div class="threshold-box">
    <div class="search-header">
      <div class="searchWrap">
        <el-form :inline="true">
          <el-form-item>
            <el-select placeholder="Category" v-model="formSearch.category_id" filterable>
              <el-option
                v-for="item in categoryList"
                :key="item.id"
                :label="item.name"
                :value="item.id"
              ></el-option>
            </el-select>
            <el-select placeholder="Product" v-model="formSearch.product_code" filterable>
              <el-option
                v-for="item in filterProducts"
                :key="item.id"
                :label="'【'+item.product_code+'】'+ '- ' + item.name"
                :value="item.product_code"
              ></el-option>
            </el-select>
            <div class="btn-group-inline">
              <div class="icons">
                <span class="icon-item search" @click="goSearch" title="search"></span>
                <div class="icon-item clear" @click="onClean" title="clear"></div>
                <div class="icon-item back" @click="goAfterIndex" title="back"></div>
              </div>
            </div>
          </el-form-item>
        </el-form>
        <div class="btn-group-top">
          <div class="icons">
            <div class="icon-item add-v2" @click="onAdd" title="add"></div>
          </div>
        </div>
      </div>
    </div>
    <div class="table-wrap" v-loading.body="loading" element-loading-text="loading...">
      <div class="table-content-holder">
        <table class="table-content" bor der="0" cellspacing="0" cellpadding="0">
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody>
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.warehouse_name }}</td>
              <td>{{ item.category_name}}</td>
              <td>{{ item.product_name }}</td>
              <td>{{ item.product_code }}</td>
              <td>{{ item.threshold_qty }}</td>
              <td>{{ item.create_time | timeFormat("Y-m-d") }}</td>
              <td>{{ item.create_user_id }}</td>
              <td>
                <button class="btn-underline" @click.stop="onEdit(item)">Edit</button>
                <button class="btn-underline" @click.stop="onDelete(item)">Delete</button>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
    <!-- 新增弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogAddVisible" class="dialog-region">
      <div class="item">
        <div class="item-box">
          <label class="label">
            Warehouse
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.warehouse_name" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Category
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.category_id"
            @change="selectCategory"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in categoryList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Product Code
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.product_code"
            filterable
            @change="selectHandle"
            slot="prepend"
          >
            <el-option
              v-for="item in getProducts"
              :key="item.id"
              :label="'【'+item.product_code+'】'+ '- ' + item.name"
              :value="item.product_code"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Product Name
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.product_name" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Threshold Qty
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.threshold_qty" class="input" type="number" min="0"></el-input>
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
import CommonService from "src/services/nl/commonService";
import WarehouseAreaService from "src/services/nl/warehouseAreaService";
import ProductService from "src/services/nl/productService";
import ThresholdService from "src/services/nl/thresholdService";

export default {
  props: ["id"],
  computed: {
    getProducts() {
      const scope = this;
      if (this.materialData && this.materialData.category_id) {
        return this.productList.filter(res => {
          return res.category_id === scope.materialData.category_id;
        });
      }
    },
    filterProducts() {
      const scope = this;
      if (this.formSearch && this.formSearch.category_id) {
        return this.productList.filter(res => {
          return res.category_id === scope.formSearch.category_id;
        });
      }
    }
  },
  data() {
    return {
      warehouseObj: {},
      formSearch: {},
      headData: [
        "Warehouse Name",
        "Category",
        "Product Name",
        "Product Code",
        "Quantity",
        "Create Date",
        "User",
        "Operation"
      ],
      loading: false,
      // 添加产品阈值数据
      materialData: {},
      // 新增弹框
      dialogTitle: "",
      dialogAddVisible: false,
      // 库区选择
      areaList: [],
      // 产品分类列表
      categoryList: [],
      // 产品列表
      productList: [],
      tableList: []
    };
  },
  created() {
    this.loading = true;
    this.warehouseObj = this.$route.params;
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
    WarehouseAreaService.findAll().then(res => {
      this.areaList = res.data;
      this.loading = false;
    });

    this.loading = true;
    ProductService.findAll().then(res => {
      this.productList = res.data;
      this.loading = false;
    });

    if (!this.warehouseObj) return;

    this.getData();
  },
  methods: {
    goSearch() {
      this.getData();
    },
    onClean() {
      this.formSearch.category_id = "";
      this.formSearch.product_code = "";
      this.getData();
    },
    // 返回路由
    goAfterIndex() {
      this.$router.back();
    },
    selectHandle(e) {
      const obj = this.productList.find(item => item.product_code === e);
      if (obj) {
        this.materialData.product_name = obj.name;
        this.materialData.product_id = obj.id;
      }
    },
    selectCategory(e) {
      const obj = this.categoryList.find(item => item.id === e);
      if (obj) this.materialData.category_name = obj.name;
    },
    onAdd() {
      this.materialData = {
        id: 0,
        warehouse_id: this.warehouseObj.id - 0,
        warehouse_name: this.warehouseObj.warehouse_name,
        product_name: "",
        product_code: "",
        threshold_qty: "",
        category_name: "",
        product_id: 0
      };
      this.dialogTitle = "Add";
      this.dialogAddVisible = true;
    },
    onEdit(item) {
      this.materialData = item;
      this.dialogTitle = "Edit";
      this.dialogAddVisible = true;
    },
    onDelete(item) {
      this.$confirm("Are you sure you wish to delete this?", "Delete", {
        confirmButtonText: "Yes",
        cancelButtonText: "No",
        type: "warning"
      }).then(() => {
        ThresholdService.remove(item.id).then(res => {
          this.$message({
            type: "success",
            message: "Delete Success!"
          });
          this.getData();
          this.loading = false;
        });
      });
    },
    dialogConfirm() {
      const scope = this;
      if (this.required()) {
        this.loading = true;
        ThresholdService.save(this.materialData)
          .then(res => {
            if (res.data.code === -2 || res.data.code === 400) {
              throw res.data.message;
            }
            this.$message({
              message: "Successfully",
              type: "success"
            });
            this.getData();
            this.dialogAddVisible = false;
            this.loading = false;
          })
          .catch(err => {
            this.$message({
              message: err,
              type: "error"
            });
            this.dialogAddVisible = false;
            this.loading = false;
          });
      }
    },
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    getData() {
      this.loading = true;
      ThresholdService.filterAll({
        category_id: this.formSearch.category_id,
        warehouse_id: +this.warehouseObj.id,
        product_code: this.formSearch.product_code
      }).then(res => {
        this.tableList = res.data;
        this.loading = false;
      });
    },
    required() {
      if (this.materialData.warehouse_id === "") {
        this.$message({
          message: "Please input Warehouse Code !",
          type: "warning"
        });
        return false;
      }

      if (this.materialData.product_code === "") {
        this.$message({
          message: "Please input Product Code !",
          type: "warning"
        });
        return false;
      }
      if (this.materialData.threshold_qty === "") {
        this.$message({
          message: "Please input Threshold Quantity !",
          type: "warning"
        });
        return false;
      }

      return true;
    }
  }
};
</script>

<style lang="scss" >
.threshold-box {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  //头部搜索框
  .search-header {
    z-index: 1;
    padding: 0;
    height: 60px;
    .searchWrap {
      .btn-group-top {
        top: 17px;
        display: flex;
        height: auto;
        .icons {
          margin: 5px;
        }
      }
      .btn-icon {
        font-size: 12px;
        margin-left: 8px;
      }
      background: #383d49;
      height: 100%;
      .el-input {
        float: left;
        width: 200px;
        padding-left: 15px;
        line-height: 60px;
      }
      .el-form-item {
        padding: 0;
        margin: 0;
        line-height: 60px;
      }
      .btn-group-inline {
        padding-top: 9px;
      }
      .el-form-item__content {
        width: 100% !important;
      }
    }
  }
}

.dialog-region {
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
      span {
        color: #e26829;
        display: inline-block;
      }
    }
    .input {
      margin-left: 40px;
      width: 260px;
      i {
        color: #ffffff99;
      }
    }
  }
}
</style>


// WEBPACK FOOTER //
// threshold.vue?6cbb6e2c