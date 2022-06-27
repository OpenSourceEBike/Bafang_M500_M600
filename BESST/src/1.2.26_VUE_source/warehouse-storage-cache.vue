<template>
  <div class="warehouse-storage-cache">
    <div class="search-header">
      <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="backArea" title="back"></span>
        </div>
      </div>
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Parcel No.">
          <el-input v-model="formSearch.parcel_no" placeholder="Parcel No."></el-input>
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
          <div class="icon-item add-v2" @click="addstorageCache" v-if="true" title="add"></div>
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
              <td>{{ item.customer_name }}</td>
              <td>{{ item.parcel_no }}</td>
              <td>{{ item.warehouse_code }}</td>
              <td>{{ item.create_user_name }}</td>
              <td>{{ item.create_time | dateFormat}}</td>
              <td>
                <a class="btn-underline" @click.stop="onEdit(item)">edit</a>
                <a class="btn-underline" @click.stop="onDelete(item)">delete</a>
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
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogAddVisible"
      custom-class="edit-dialog"
      class="dialog-area-details"
    >
      <div class="item">
        <label class="title">
          Warehouse Code
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.warehouse_code"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Parcel No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            class="input"
            v-model="formData.parcel_no"
            filterable
            slot="prepend"
            @change="changeParcel(formData.parcel_no)"
          >
            <el-option
              v-for="item in parcelList"
              :key="item.id"
              :label="item.parcel_no"
              :value="item.parcel_no"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Customer Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.customer_name" disabled></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogDeleteVisible" size="tiny">
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
import ZH from "src/assets/lang/zh";
import DE from "src/assets/lang/de";
import NL from "src/assets/lang/nl";
import StorageCacheService from "src/services/storageCacheService";
import ParcelInformationService from "src/services/parcelInformationService";

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        parcel_no: "",
        warehouse_region_id: this.$route.query.warehouse_region_id,
        warehouse_id: this.$route.query.warehouse_id,
        area_id: this.$route.query.area_id
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      // 弹窗标题
      dialogTitle: "",
      // 新增弹窗
      dialogAddVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 当前操作ID
      currentID: "",
      formData: {},
      parcelList: []
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
  created() {},
  computed: {},
  async mounted() {
    this.loading = true;
    await ParcelInformationService.filterParcelInfoByStatus(1)
      .then(res => {
        if (res.data.code === -2) {
          throw res.data.message;
        }
        this.parcelList = res.data.content;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });

    this.filterStorageCache();
    this.headData = [
      "Warehouse Code",
      "Parcel No.",
      "Customer Name",
      "Create User",
      "Creat time",
      "Operation"
    ];
  },
  methods: {
    // 返回路由
    backArea() {
      const data = [
        this.$route.query.warehouse_region_id,
        this.$route.query.warehouse_id
      ];
      this.$store.commit("setChangeFilter", data);
      this.$store.commit("changeTrueStatus");
      this.$router.push({ name: "warehouseArea" });
    },
    // 搜索
    onSearch() {
      this.filterStorageCache();
    },
    onClean() {
      this.formSearch.parcel_no = "";
      this.filterStorageCache();
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Area";
      this.formData = JSON.parse(JSON.stringify(event));
      this.dialogAddVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      const setPost = "get";
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
    },
    // 获取仓库地区
    filterStorageCache() {
      this.loading = true;
      StorageCacheService.filterStorageCache(this.currentPage, this.formSearch)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.tableList = res.data.content;
          this.total = res.data.totalElements;
          this.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 新增弹窗打开事件
    addstorageCache() {
      this.dialogTitle = "Add";
      this.dialogAddVisible = true;
      this.formData = {
        area_id: this.$route.query.area_id,
        warehouse_id: this.$route.query.warehouse_id,
        warehouse_region_id: this.$route.query.warehouse_region_id,
        warehouse_code: "",
        parcel_no: ""
      };
    },
    // 增/改 弹窗确认事件
    dialogConfirm() {
      if (!this.checkParams()) {
        return;
      }
      this.loading = true;
      StorageCacheService.postStorageCache(this.formData)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.loading = false;
          this.dialogAddVisible = false;
          this.filterStorageCache();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 弹窗取消事件
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      StorageCacheService.deleteStorageCache(scope.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.filterStorageCache();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 弹窗必填
    checkParams() {
      if (this.formData.warehouse_code === "") {
        this.$message({
          message: "Please add Warehouse Code",
          type: "warning"
        });
        return false;
      }

      if (this.formData.parcel_no === "") {
        this.$message({
          message: "Please add Parcel No.",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    changeParcel(parcel_no) {
      const data = this.parcelList.find(res => {
        return res.parcel_no === parcel_no;
      });
      if (data) {
        this.formData.customer_name = data.customer_name;
      }
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.warehouse-storage-cache {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  .header {
    .title {
      color: #e26829;
    }
  }

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
      width: 110px;
      margin-left: 5px;
      text-align: left;
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

  .dialog-area-details {
    overflow: hidden;
    .el-dialog__body {
      padding: 30px 20px 30px 50px;
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
        width: 142px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 200px;
      }
    }
  }
  .dialog-category {
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
}
</style>



// WEBPACK FOOTER //
// warehouse-storage-cache.vue?97fee0a4