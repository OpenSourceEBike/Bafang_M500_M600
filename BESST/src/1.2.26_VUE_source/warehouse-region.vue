<template>
  <div class="warehouse-region">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Region Code">
          <el-input v-model="formSearch.region_code" placeholder="Region Code"></el-input>
        </el-form-item>
        <el-form-item label="Region Name">
          <el-input v-model="formSearch.region_name" placeholder="Region Name"></el-input>
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
            @click="addRegion"
            v-if="true"
            title="add"
            v-ilpermission="'warehouse-region-add'"
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
            <tr v-for="item in tableList" :key="item.id" class="areaList">
              <td>{{ item.warehouse_region_code }}</td>
              <td>{{ item.warehouse_region_name }}</td>
              <td>{{ item.remark }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'warehouse-region-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'warehouse-region-delete'"
                >delete</a>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
      <div class="loading" v-if="loading">
        <img src="../../../assets/images/icon/loading-bars.svg" alt="Loading icon">
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
            Region Code
            <span class="required">*</span>
          </label>
          <el-input v-model="regionData.warehouse_region_code" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Region Name
            <span class="required">*</span>
          </label>
          <el-input v-model="regionData.warehouse_region_name" class="input"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">Remark</label>
          <el-input v-model="regionData.remark" class="input"></el-input>
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

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        region_code: "",
        region_name: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      // 所有仓库地区
      warehouseRegin: [],
      // 弹窗标题
      dialogTitle: "",
      regionData: {},
      // 新增弹窗
      dialogAddVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 当前操作ID
      currentID: ""
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
    this.getWarehouseRegion();
    this.headData = ["Region Code", "Region Name", "Remark", "Operation"];
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.getWarehouseRegion();
    },
    // 清楚搜索
    onClean() {
      this.formSearch.region_name = "";
      this.formSearch.region_code = "";
      this.getWarehouseRegion();
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Region";
      this.regionData = JSON.parse(JSON.stringify(event));
      this.dialogAddVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getWarehouseRegion();
    },
    // 获取地方仓库区域
    getWarehouseRegion() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/warehouse/region/filter",
        data: {
          warehouse_region_code: this.formSearch.region_code,
          warehouse_region_name: this.formSearch.region_name
        },
        params: {
          size: 10,
          page: this.currentPage
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
    // 弹框确认
    dialogConfirm() {
      const proceed = this.required();
      if (!proceed) {
        return;
      }
      this.loading = true;
      this.postRegion(this.regionData).then(
        res => {
          this.loading = false;
          this.dialogAddVisible = false;
          this.getWarehouseRegion();
        },
        err => {
          this.loading = false;
        }
      );
    },
    // 弹框取消
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    // 新增弹窗打开事件
    addRegion() {
      this.regionData = {
        remark: "",
        warehouse_region_code: "",
        warehouse_region_name: ""
      };
      this.dialogTitle = "Add Region";
      this.dialogAddVisible = true;
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteRegion(scope.currentID).then(
        res => {
          if( res.code === 201) {
            this.$message({
              message: "You can't delete the Region because it contains the warehouse that is being used.",
              type: "warning"
            });
          } else {
            scope.getWarehouseRegion(this.formSearch.region);
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;       
        },
        err => {
          this.loading = false;
        }
      );
    },
    postRegion(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse/region",
        data: data
      });
    },
    deleteRegion(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/warehouse/region/" + id
      });
    },
    // 弹窗必填
    required() {
      let flag = true;
      for (const item in this.regionData) {
        if (
          item === "remark" ||
          item === "update_user_id" ||
          item === "update_time"
        ) {
          continue;
        }
        if (
          this.regionData[item] === "" ||
          this.regionData[item] == null ||
          this.regionData[item] == undefined
        ) {
          this.$message({
            message: "Please input " + item,
            type: "warning"
          });
          flag = false;
        }
      }
      return flag;
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.warehouse-region {
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
    .areaList:hover {
      background: #ffffff26;
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
      }
      .input {
        margin-left: 40px;
        width: 260px;
      }
    }
  }

  .modal-select {
    margin-left: 40px;
    width: 260px;
  }
}
</style>



// WEBPACK FOOTER //
// warehouse-region.vue?403bb018