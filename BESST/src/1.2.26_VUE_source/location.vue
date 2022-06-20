<template>
  <div class="location-index">
    <div class="search-header">
      <el-form :inline="true" class="list-form-inline">
        <el-form-item>
          <el-input
            v-model.trim="search_value"
            @keyup.enter.native="goSearch"
            placeholder="Location Code"
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
        <div class="icons" @click="addLocation()">
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
              <td>{{ item.area_code}}</td>
              <td>{{ item.location_code }}</td>
              <td>{{ item.location_address }}</td>
              <!-- <td>{{ item.country }}</td> -->
              <td>
                <button class="btn-underline" @click.stop="onEdit(item)">edit</button>
                <button class="btn-underline" @click.stop="onDelete(item)">delete</button>
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
            Warehouse
            <span class="required">*</span>
          </label>
          <el-select class="input" v-model="locationData.warehouse_id" filterable slot="prepend">
            <el-option
              v-for="item in warehouseList"
              :key="item.id"
              :label="item.warehouse_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Area Code
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="locationData.area_id"
            filterable
            slot="prepend"
            @change="selectArea"
          >
            <el-option
              v-for="item in getArea"
              :key="item.id"
              :label="'【'+item.area_code +'】-'+ item.warehouse_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Location Code
            <span class="required">*</span>
          </label>
          <el-input v-model="locationData.location_code" class="input"></el-input>
        </div>
      </div>
      <!-- <div class="item">
        <div class="item-box">
          <label class="label">
            Location Name
            <span class="required">*</span>
          </label>
          <el-input v-model="locationData.location_name" class="input"></el-input>
        </div>
      </div>-->
      <div class="item">
        <div class="item-box">
          <label class="label">
            Location Address
            <span class="required">*</span>
          </label>
          <el-input v-model="locationData.location_address" class="input"></el-input>
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
import WarehouseListService from "src/services/nl/warehouseListService.js";
import WarehouseLocationService from "src/services/nl/warehouseLocationService.js";
import WarehouseAreaService from "src/services/nl/warehouseAreaService.js";
// import areaCodeInDB from "core/areaCode";
export default {
  computed: {
    getArea() {
      const scope = this;
      if (this.locationData && this.locationData.warehouse_id) {
        return this.areaList.filter(res => {
          return res.warehouse_id === scope.locationData.warehouse_id;
        });
      }
    }
  },
  data() {
    return {
      loading: false,
      headData: [
        "Area Code",
        "Location Code",
        "Location Address",
        // "Country",
        "Operation"
      ],
      tableList: [],
      search_value: undefined,
      currentPage: 1,
      total: 0,
      areaList: [],
      //新增、删除弹框
      dialogTitle: "",
      dialogAddVisible: false,
      locationData: {},
      // 仓库选择
      warehouseList: []
    };
  },
  created() {
    this.loading = true;
    WarehouseListService.findAll().then(res => {
      this.warehouseList = res.data;
      this.loading = false;
    });

    this.loading = true;
    WarehouseAreaService.findAll().then(res => {
      this.areaList = res.data;
      this.loading = false;
    });
  },
  mounted() {
    this.getData();
    // this.areaCodeList = areaCodeInDB.init();
  },
  methods: {
    selectArea(e) {
      const obj = this.areaList.find(item => item.id === e);
      if (obj) this.locationData.area_code = obj.area_code;
    },
    goSearch() {
      this.getData();
    },
    onClean() {
      this.search_value = undefined;
      this.getData();
    },
    addLocation() {
      this.locationData = {
        location_code: "",
        location_name: "",
        location_address: "",
        area_id: ""
        // country: ""
      };
      this.dialogTitle = "Add Location";
      this.dialogAddVisible = true;
    },
    onEdit(item) {
      this.locationData = item;
      this.dialogTitle = "Edit Location";
      this.dialogAddVisible = true;
    },
    onDelete(item) {
      this.$confirm("Are you sure you wish to delete this?", "Delete", {
        confirmButtonText: "Yes",
        cancelButtonText: "No",
        type: "warning"
      })
        .then(() => {
          this.deleteOneLocation(item);
        })
        .catch(() => {});
    },
    dialogConfirm() {
      const pass = this.required();
      if (!pass) {
        return;
      }
      this.loading = true;
      const data = this.locationData;
      WarehouseLocationService.addOneLocation(data)
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
      this.dialogAddVisible = false;
    },
    required() {
      if (this.locationData.location_code === "") {
        this.$message({
          message: "Please input Location Code !",
          type: "warning"
        });
        return false;
      }
      // if (this.locationData.location_name === "") {
      //   this.$message({
      //     message: "Please input Location Name !",
      //     type: "warning"
      //   });
      //   return false;
      // }
      if (this.locationData.location_address === "") {
        this.$message({
          message: "Please input Location Address !",
          type: "warning"
        });
        return false;
      }
      if (this.locationData.area_id === "") {
        this.$message({
          message: "Please select Area Code!",
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
    deleteOneLocation(item) {
      this.loading = true;
      WarehouseLocationService.deleteOneLocation(item.id)
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
      WarehouseLocationService.locationFilter(
        {
          location_code: this.search_value
        },
        this.currentPage
      )
        .then(res => {
          this.tableList = res.data.content;
          this.total = res.data.numberOfElements;
          this.loading = false;
        })
        .catch(err => {
          this.loading = false;
        });
    }
  }
};
</script>

<style lang="scss">
.location-index {
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
}
</style>


// WEBPACK FOOTER //
// location.vue?6731d78b