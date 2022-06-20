<template>
    <div class="warehouse-area-type">
        <div class="search-header">
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-form-item label="Type Code">
                    <el-input v-model="formSearch.type_code" placeholder="Type Code"></el-input>
                </el-form-item>
                <el-form-item label="Type Name">
                    <el-input v-model="formSearch.type_name" placeholder="Type Name"></el-input>
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
                        v-ilpermission="'warehouse-area-type-add'"
                        title="add"
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
                            <td>{{ item.type_code }}</td>
                            <td>{{ item.type_name }}</td>
                            <td>
                                <a class="btn-underline"   v-ilpermission="'warehouse-area-type-add'" @click.stop="onEdit(item)">edit</a>
                                <a class="btn-underline"   v-ilpermission="'warehouse-area-type-delete'" @click.stop="onDelete(item)">delete</a>
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
                        Type Code
                        <span class="required">*</span>
                    </label>
                    <el-select class="input" v-model="formData.type_code" filterable slot="prepend">
                        <el-option
                            v-for="(item,index) in areaTypeList"
                            :key="index"
                            :label="item"
                            :value="item"
                        ></el-option>
                    </el-select>
                </div>
            </div>
            <div class="item">
                <div class="item-box">
                    <label class="label">
                        Type Name
                        <span class="required">*</span>
                    </label>
                    <el-input v-model="formData.type_name" class="input"></el-input>
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
import WarehouseAreaTypeService from "src/services/warehouseAreaTypeService";

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        type_code: "",
        type_name: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      // 弹窗标题
      dialogTitle: "",
      formData: {},
      // 新增弹窗
      dialogAddVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 当前操作ID
      currentID: "",
      areaTypeList: ["Common", "Storage Cache", "Retirement", "Other"]
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
    this.filterWarehouseAreaType();
    this.headData = ["Type Code", "Type Name", "Operation"];
  },
  methods: {
    // 搜索
    onSearch() {
      this.filterWarehouseAreaType();
    },
    // 清楚搜索
    onClean() {
      this.formSearch.type_name = "";
      this.formSearch.type_code = "";
      this.filterWarehouseAreaType();
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Area Type";
      this.formData = JSON.parse(JSON.stringify(event));
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
      this.filterWarehouseAreaType();
    },
    // 获取货区类型
    filterWarehouseAreaType() {
      this.loading = true;
      WarehouseAreaTypeService.filterWarehouseAreaType(
        this.currentPage,
        this.formSearch
      )
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
    // 弹框确认
    dialogConfirm() {
      const proceed = this.required();
      if (!proceed) {
        return;
      }
      this.loading = true;
      WarehouseAreaTypeService.postWarehouseAreaType(this.formData)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.loading = false;
          this.dialogAddVisible = false;
          this.filterWarehouseAreaType();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 弹框取消
    dialogCancel() {
      this.dialogAddVisible = false;
    },
    // 新增弹窗打开事件
    addRegion() {
      this.formData = {
        type_code: "",
        type_name: ""
      };
      this.dialogTitle = "Add Type";
      this.dialogAddVisible = true;
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      WarehouseAreaTypeService.deleteWarehouseAreaType(scope.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.filterWarehouseAreaType();
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    // 弹窗必填
    required() {
      let flag = true;
      for (const item in this.formData) {
        if (item === "update_user_id" || item === "update_time") {
          continue;
        }
        if (
          this.formData[item] === "" ||
          this.formData[item] == null ||
          this.formData[item] == undefined
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
.warehouse-area-type {
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
// warehouse-area-type.vue?519d6d48