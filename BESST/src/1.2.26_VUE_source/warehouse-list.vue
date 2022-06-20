<template>
  <div class="warehouse-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Region">
          <el-select v-model="formSearch.region_id" placeholder="Region">
            <el-option
              v-for="item in regionList"
              :key="item.id"
              :label="item.warehouse_region_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </el-form-item>
        <div class="btn-group-inline-icon">
          <div class="icons">
            <div class="icon-item search" @click="onSearch" title="search"></div>
            <div class="icon-item clear" @click="onClean" title="clear"></div>
          </div>
        </div>
      </el-form>
      <div class="btn-group-top">
        <div class="icons">
          <div
            class="icon-item add-v2"
            @click="dialogAdd"
            v-if="true"
            title="add"
            v-ilpermission="'warehouse-list-add'"
          ></div>
        </div>
      </div>
    </div>
    <div class="table-wrap" v-loading.body="loading">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length <= 0 && !loading">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody v-if="!loading && tableList.length > 0">
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.code }}</td>
              <td>{{ item.name }}</td>
              <td>{{ regionName(item) }}</td>
              <td>{{ item.country }}</td>
              <td>{{ item.city }}</td>
              <td>{{ item.district }}</td>
              <td>{{ item.address }}</td>
              <td>{{ item.phone }}</td>
              <td>{{ item.email }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'warehouse-list-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'warehouse-list-delete'"
                >delete</a>
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
    <!-- 添加or修改弹窗 -->
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogWarehouseVisible"
      custom-class="edit-dialog"
      class="dialog-warehouse"
    >
      <div class="item">
        <label class="title">
          Code
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.code"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Region
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select v-model="warehouse.region_id" slot="append" class="land" filterable>
            <el-option
              v-for="item in regionList"
              :key="item.id"
              :label="item.warehouse_region_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Country
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select class="land" v-model="warehouse.country" filterable slot="prepend">
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
        <label class="title">
          City
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.city"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          District
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.district"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Address
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.address"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Phone
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.phone">
            <el-select v-model="warehouse.country_code" filterable slot="prepend">
              <el-option
                v-for="item in areaCodeList"
                :key="item.area_code"
                :label="item.area_code"
                :value="item.area_code"
              ></el-option>
            </el-select>
          </el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Email
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="warehouse.email"></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Warehous" :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
import EN from "src/assets/lang/en";
import areaCodeInDB from "core/areaCode";
export default {
  data() {
    return {
      loading: false,
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      areaCodeList: [],
      regionList: [],
      //当前操作ID
      currentID: "",
      // 弹窗
      warehouse: {},
      dialogTitle: "",
      dialogWarehouseVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 查询条件
      formSearch: {
        region_id: ""
      }
    };
  },
  i18n: {
    messages: {
      en: EN.Dealer.Index
    }
  },
  created() {},
  computed: {},
  mounted() {
    const scope = this;
    scope.areaCodeList = areaCodeInDB.init();
    scope.loading = true;
    scope
      .getAllRegion()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.loading = false;
        scope.regionList = res;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.getWarehouseList();

    this.headData = [
      "Code",
      "Warehouse Name",
      "Region",
      "Country",
      "City",
      "Area",
      "Address",
      "Phone",
      "Email",
      "Operation"
    ];
  },
  methods: {
    // 设置区域
    regionName: function(item) {
      if (this.regionList.length > 0) {
        const data = this.regionList.filter(res => {
          return res.id === item.region_id;
        });
        if (data.length > 0) {
          return data[0].warehouse_region_name;
        } else {
          return "-";
        }
      }
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getWarehouseList();
    },
    onClean() {
      this.formSearch.region_id = "";
      this.getWarehouseList();
    },
    // 操作
    onEdit(event) {
      this.warehouse = JSON.parse(JSON.stringify(event));
      this.dialogTitle = "Update Warehous";
      this.dialogWarehouseVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 添加
    dialogAdd() {
      this.warehouse = {
        address: "",
        city: "",
        code: "",
        country_code: "+86",
        district: "",
        email: "",
        name: "",
        phone: "",
        country: ""
      };
      this.dialogTitle = "Add Warehous";
      this.dialogWarehouseVisible = true;
    },
    // 弹窗确认
    dialogConfirm() {
      if (!this.required()) {
        return;
      }
      this.loading = true;
      this.postWarehouse(this.warehouse)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.loading = false;
          this.dialogWarehouseVisible = false;
          this.getWarehouseList();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogWarehouseVisible = false;
          this.loading = false;
        });
    },
    // 弹窗取消
    dialogCancel() {
      this.dialogWarehouseVisible = false;
    },
    // 删除 弹窗确认
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteWarehouse(scope.currentID).then(
        res => {
          if (res.code === 201) {
            this.$message({
              message:
                "You can't delete the warehouse because it contains the area that is being used.",
              type: "warning"
            });
          } else {
            scope.getWarehouseList();
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        },
        err => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        }
      );
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getWarehouseList();
    },
    getAllRegion() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/region/all"
      });
    },

    getWarehouseList() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/warehouse/filter",
        data: {
          region_id: this.formSearch.region_id
        },
        params: {
          page: this.currentPage,
          size: 10
        }
      }).then(
        res => {
          this.tableList = res.content;
          this.total = res.totalElements;
          this.loading = false;
        },
        err => {
          this.loading = false;
        }
      );
    },
    postWarehouse(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse",
        data: data
      });
    },
    deleteWarehouse(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/warehouse/" + id
      });
    },
    // 弹窗必填
    required() {
      for (const item in this.warehouse) {
        if (item === "update_user_id" || item === "update_time") {
          continue;
        }
        if (!Core.Util.isEmail(this.warehouse.email)) {
          this.$message({
            message: "E-mail is illegitimate",
            type: "warning"
          });
          return false;
        }
        if (
          this.warehouse[item] === "" ||
          this.warehouse[item] == null ||
          this.warehouse[item] == undefined
        ) {
          this.$message({
            message: "Please input " + item,
            type: "warning"
          });
          return false;
        }
      }
      return true;
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.warehouse-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .list-form-inline {
    height: 100%;
    line-height: 50px;
    .el-form-item {
      margin-right: 2px !important;
      margin-bottom: 0;
    }
    .el-form-item__label {
      color: #fff;
      padding: 11px 12px;
    }
    .el-form-item__content {
      width: 180px !important;
    }
  }
  .table-wrap {
    .long-td {
      max-width: 270px;
      text-overflow: ellipsis;
      overflow: hidden;
      text-align: left;
    }
  }

  .dialog-warehouse {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 66px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .land {
        width: 250px;
      }
      .title {
        display: inline-block;
        width: 80px;
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
// warehouse-list.vue?df41bf8a