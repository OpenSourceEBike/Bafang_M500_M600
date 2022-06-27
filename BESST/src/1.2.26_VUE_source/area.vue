<template>
  <div class="area-index">
    <div class="search-header">
      <el-form :inline="true" class="list-form-inline">
        <el-form-item>
          <el-input
            v-model.trim="search_value"
            @keyup.enter.native="goSearch"
            placeholder="Area Code"
          ></el-input>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <span class="icon-item search" @click="goSearch" title="search"></span>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons" @click="addArea()">
          <button type="button" class="el-button el-button--preview">
            Add
            <i class="el-icon-plus btn-icon"></i>
          </button>
        </div>
      </div>
    </div>
    <div class="table-wrap" v-loading.body="loading" element-loading-text="loading...">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length <= 0 && !loading">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody>
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.area_code }}</td>
              <td>{{ item.area_name }}</td>
              <td>{{ item.warehouse_name }}</td>
              <td>{{ item.country }}</td>
              <td>
                <!-- <button class="btn-underline" @click.stop="onThreshold(item)">Threshold</button> -->
                <button class="btn-underline" @click.stop="onEdit(item)">Edit</button>
                <button class="btn-underline" @click.stop="onDelete(item)">Delete</button>
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
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogAddVisible" class="dialog-region">
      <div class="item">
        <div class="item-box">
          <label class="label">
            Country
            <span class="required">*</span>
          </label>
          <el-select class="input" v-model="areaData.country" filterable slot="prepend">
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
        <div class="item-box">
          <label class="label">
            Warehouse Name
            <span class="required">*</span>
          </label>
          <el-select class="input" v-model="areaData.warehouse_id" filterable slot="prepend">
            <el-option
              v-for="item in getWarehouse"
              :key="item.id"
              :label="item.warehouse_name"
              :value="item.id"
            ></el-option>
          </el-select>
          <!-- <el-input v-model="areaData.warehouse_name" class="input"></el-input> -->
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Area Name
            <span class="required">*</span>
          </label>
          <el-input v-model="areaData.area_name" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Area Code
            <span class="required">*</span>
          </label>
          <el-input v-model="areaData.area_code" class="input"></el-input>
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
import WarehouseAreaService from "src/services/nl/warehouseAreaService.js";
import WarehouseListService from "src/services/nl/warehouseListService.js";
import areaCodeInDB from "core/areaCode";
export default {
  data() {
    return {
      loading: false,
      headData: [
        "Area Code",
        "Area Name",
        "Warehouse Name",
        "Country",
        "Operation"
      ],
      data: [
        {
          firm_name: "MM G510.1000 52V/1000W",
          warehouse_code: "1119010001001",
          warehouse_name: "BF-CN",
          warehouse_address: "suzhou China",
          postal_code: "S03-B04",
          date: "28-Jun-2019"
        },
        {
          firm_name: "MM G510.1000 52V/1000W",
          warehouse_code: "1119010001001",
          warehouse_name: "BF-CN",
          warehouse_address: "suzhou China",
          postal_code: "S03-B04",
          date: "28-Jun-2019"
        },
        {
          firm_name: "MM G510.1000 52V/1000W",
          warehouse_code: "1119010001001",
          warehouse_name: "BF-CN",
          warehouse_address: "suzhou China",
          postal_code: "S03-B04",
          date: "28-Jun-2019"
        },
        {
          firm_name: "MM G510.1000 52V/1000W",
          warehouse_code: "1119010001001",
          warehouse_name: "BF-CN",
          warehouse_address: "suzhou China",
          postal_code: "S03-B04",
          date: "28-Jun-2019"
        }
      ],
      tableList: [],
      search_value: undefined,
      currentPage: 1,
      total: 0,
      // 国家选择
      areaCodeList: [],
      // warehouse列表
      warehouseList: [],
      tempWarehouseList: [],
      //新增、删除弹框
      dialogTitle: "",
      dialogAddVisible: false,
      areaData: {}
    };
  },
  computed: {
    getWarehouse() {
      const scope = this;
      if (this.areaData && this.areaData.country) {
        return this.warehouseList.filter(res => {
          return res.country === scope.areaData.country;
        });
      }
    }
  },
  mounted() {
    this.getData();
    this.getAllWarehouseName();
    this.areaCodeList = areaCodeInDB.init();
  },
  methods: {
    goSearch() {
      this.getData();
    },
    onClean() {
      this.search_value = undefined;
      this.getData();
    },
    addArea() {
      this.areaData = {
        area_code: "",
        area_name: "",
        warehouse_id: ""
      };
      this.dialogTitle = "Add Area";
      this.dialogAddVisible = true;
    },
    onThreshold(item) {
      this.$router.push({
        name: "thresholdIndex",
        params: { id: item.id }
      });
    },
    onEdit(item) {
      this.areaData = item;
      this.dialogTitle = "Edit Area";
      this.dialogAddVisible = true;
    },
    onDelete(item) {
      this.$confirm("Are you sure you wish to delete this?", "Delete", {
        confirmButtonText: "Yes",
        cancelButtonText: "No",
        type: "warning"
      })
        .then(() => {
          this.deleteOneArea(item);
        })
        .catch(() => {});
    },
    dialogConfirm() {
      const pass = this.required();
      if (!pass) {
        return;
      }
      this.loading = true;
      const data = this.areaData;
      WarehouseAreaService.addOneArea(data)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
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
            message: err.body.error,
            type: "error"
          });
          this.dialogAddVisible = false;
          this.loading = false;
        });
    },
    dialogCancel() {
      this.tempWarehouseList = this.warehouseList;
      this.dialogAddVisible = false;
    },
    required() {
      if (this.areaData.area_code === "") {
        this.$message({
          message: "Please input Area Code !",
          type: "warning"
        });
        return false;
      }
      if (this.areaData.area_name === "") {
        this.$message({
          message: "Please input Area Name !",
          type: "warning"
        });
        return false;
      }
      if (this.areaData.warehouse_name === "") {
        this.$message({
          message: "Please input Warehouse Name !",
          type: "warning"
        });
        return false;
      }
      if (this.areaData.country === "") {
        this.$message({
          message: "Please input Country !",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      this.getData();
    },
    deleteOneArea(item) {
      this.loading = true;
      WarehouseAreaService.deleteOneArea(item.id)
        .then(res => {
          this.$message({
            type: "success",
            message: "Delete Success!"
          });
          this.getData();
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err.body.error,
            type: "error"
          });
          this.loading = false;
        });
    },
    getData() {
      this.loading = true;
      WarehouseAreaService.areaFilter(
        {
          area_code: this.search_value
        },
        this.currentPage
      )
        .then(res => {
          this.tableList = res.data.content;
          this.total = res.data.totalElements;
          this.loading = false;
        })
        .catch(err => {
          this.loading = false;
        });
    },
    //获取所有的warehouse name
    getAllWarehouseName() {
      const scope = this;
      WarehouseListService.findAll().then(
        res => {
          res.data &&
            ((scope.warehouseList = res.data),
            (scope.tempWarehouseList = res.data));
        },
        err => {}
      );
    }
  }
};
</script>

<style lang="scss" >
.area-index {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  //头部搜索框
  .list-form-inline {
    height: 100%;
    line-height: 50px;
    width: calc(100% - 130px);
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__label {
      color: #fff;
      width: 100px;
      text-align: left;
      margin-left: 5px;
    }
    .el-form-item__content {
      width: 180px !important;
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
        width: 160px;
        height: 30px;
        vertical-align: middle;
        span {
          color: #e26829;
          display: inline-block;
        }
      }
      .input {
        margin-left: 40px;
        width: 220px;
        i {
          color: #ffffff99;
        }
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// area.vue?b31b66c6