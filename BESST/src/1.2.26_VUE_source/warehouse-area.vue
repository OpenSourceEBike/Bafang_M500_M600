<template>
    <div class="warehouse-area">
        <div class="search-header">
            <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
            </div>-->
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-cascader
                    class="cascader"
                    :options="options"
                    filterable
                    placeholder="Select warehouse area"
                    @change="handleItemChange"
                    :props="props"
                    :value="cascaderValue"
                ></el-cascader>
                <el-form-item label="Area Code">
                    <el-input v-model="formSearch.area_code" placeholder="Area Code"></el-input>
                </el-form-item>
                <el-form-item label="Area Name">
                    <el-input v-model="formSearch.area_name" placeholder="Area Name"></el-input>
                </el-form-item>
                <div class="btn-group-inline-icon">
                    <div class="icons">
                        <div class="icon-item search" @click="onSearch" title="search"></div>
                        <div class="icon-item clear" @click="onClean" title="clear"></div>
                        <div class="icon-item upload" @click="onImport" title="upload"></div>
                    </div>
                </div>
            </el-form>
            <div class="btn-group-top">
                <div class="icons">
                    <div
                        class="icon-item add-v2"
                        @click="addArea"
                        v-if="true"
                        title="add"
                        v-ilpermission="'warehouse-area-add'"
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
                            <td>{{ item.warehouse_region_name}}</td>
                            <td>{{ warehousePipe(item) }}</td>
                            <td>{{ item.area_code }}</td>
                            <td>{{ item.area_name }}</td>
                            <td>{{ item.type_code||'-' }}</td>
                            <td>{{ item.type_name||'-' }}</td>
                            <td>
                                <a
                                    class="btn-underline"
                                    @click.stop="onEdit(item)"
                                    v-ilpermission="'warehouse-area-add'"
                                >edit</a>
                                <a
                                    class="btn-underline"
                                    @click.stop="onDetails(item)"
                                    v-ilpermission="'warehouse-area-delete'"
                                >details</a>
                                <a class="btn-underline" @click.stop="onDelete(item)">delete</a>
                                <a
                                    class="btn-underline"
                                    @click.stop="onInventoryDetails(item)"
                                    title="set min value"
                                >setting</a>
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
            class="dialog-area-list"
        >
            <div class="item">
                <div class="item-box">
                    <label class="label">
                        Type Name
                        <span class="required">*</span>
                    </label>
                    <el-select
                        class="input"
                        v-model="areaData.type_code"
                        filterable
                        slot="prepend"
                        @change="changeType(areaData.type_code)"
                    >
                        <el-option
                            v-for="item in warehouseTypeList"
                            :key="item.type_code"
                            :label="item.type_name"
                            :value="item.type_code"
                        ></el-option>
                    </el-select>
                </div>
            </div>
            <div class="item">
                <div class="item-box">
                    <label class="label">
                        Type Code
                        <span class="required">*</span>
                    </label>
                    <el-input v-model="areaData.type_code" class="input" disabled></el-input>
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
            <div class="item">
                <div class="item-box">
                    <label class="label">
                        Area Name
                        <span class="required">*</span>
                    </label>
                    <el-input v-model="areaData.area_name" class="input"></el-input>
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
        <!-- 导入 -->
        <el-dialog
            class="dialog-import"
            title="Import File"
            :visible.sync="dialogImportVisible"
            size="tiny"
        >
            <div class="item">
                <label class="title">
                    Area
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-select v-model="areaId" placeholder="Area">
                        <el-option
                            v-for="item in areaList"
                            :key="item.id"
                            :label="item.area_name"
                            :value="item.id"
                        ></el-option>
                    </el-select>
                </div>
            </div>
            <div class="file">
                <label class="label" for="fileToImport">fileToImport</label>
                <br>
                <span>{{fileName}}</span>
                <input
                    class="file-input"
                    id="uploadFile"
                    ref="fileUpdate"
                    type="file"
                    @change="onChangeUpdateFile()"
                >
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="primary" @click="importInvitation">yes</el-button>
                <el-button type="primary" @click="dialogImportVisible = false">no</el-button>
            </div>
        </el-dialog>
    </div>
</template>

<script>
import Core from "core/core";
import EN from "src/assets/lang/en";
import WarehouseService from "src/services/warehouseService";
import LocationService from "src/services/locationService";

export default {
  data() {
    return {
      loading: false,
      regionId: "",
      warehouseId: "",
      formSearch: {
        area_code: "",
        area_name: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      warehouseList: [],
      // 弹窗标题
      dialogTitle: "",
      areaData: {},
      // 新增弹窗
      dialogAddVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      // 当前操作ID
      currentID: "",
      // 级联数据
      options: [],
      props: {
        label: "name",
        value: "id",
        children: "warehouse"
      },
      cascaderValue: [],
      warehouseTypeList: [],
      dialogImportVisible: false,
      areaList: [],
      //上传
      areaId: "",
      file: "",
      fileName: ""
    };
  },
  i18n: {
    messages: {
      en: EN.Dealer.Index
    }
  },
  async created() {
    if (this.$store.state.currentBackStatus) {
      if (JSON.stringify(this.$store.state.changeFilter) != "{}") {
        const data = [
          this.$store.state.changeFilter[0],
          this.$store.state.changeFilter[1]
        ];
        this.cascaderValue = data;
        this.handleItemChange(data);
      }
    }
    await WarehouseService.getAllArea().then(res => {
      this.areaList = res.data;
    });
  },
  computed: {},
  async mounted() {
    const scope = this;
    scope.loading = true;

    await WarehouseService.getAllWarehouseType()
      .then(res => {
        if (res.data.code === -2) {
          throw res.message;
        }
        scope.warehouseTypeList = res.data;
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.loading = true;
    scope
      .getAllRegion()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.options = res.map(res => {
          return {
            name: res.warehouse_region_name,
            id: res.id,
            warehouse: []
          };
        });
        return scope.getAllWarehouse();
      })
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        const _warehouseList = res;
        this.warehouseList = res;
        this.options.forEach(res => {
          res.warehouse = _warehouseList
            .filter(_warehouse => {
              return _warehouse.region_id === res.id;
            })
            .map(res => {
              return {
                name: res.name,
                id: res.id
              };
            });
        });
        scope.loading = false;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    this.getWarehouseArea();

    this.headData = [
      "Region",
      "Warehouse",
      "Area Code",
      "Area Name",
      "Type Code",
      "Type Name",
      "Operation"
    ];
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
    }, // 上传
    importInvitation() {
      if (!this.areaId) {
        this.$message({
          message: "Please add the area",
          type: "warning"
        });
        return;
      }
      if (!this.file) {
        this.$message({
          message: "Please select the file",
          type: "warning"
        });
        return;
      }

      let fd = new FormData();
      fd.append("file", this.file);
      LocationService.uploadBitchInventory(fd, {
        area_id: this.areaId
      }).then(
        res => {
          this.$message({
            message: "Import Successfully",
            type: "success"
          });
          this.dialogImportVisible = false;
        },
        err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogImportVisible = false;
        }
      );
    },
    // 选择文件
    onChangeUpdateFile() {
      if (this.$refs.fileUpdate.files.length > 0) {
        this.file = this.$refs.fileUpdate.files[0];
        this.fileName = this.file.name;
        console.log(this.file);
      } else {
        this.file = null;
        this.fileName = "";
      }
    },
    handleItemChange(val) {
      this.regionId = val[0];
      this.warehouseId = val[1];
      this.getWarehouseArea();
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
      this.$store.commit("changeFalseStatus");
      this.cascaderValue = [];
    },
    // 搜索
    onSearch() {
      this.getWarehouseArea();
    },

    onClean() {
      this.cascaderValue = [];
      this.regionId = "";
      this.warehouseId = "";
      this.formSearch.area_code = "";
      this.formSearch.area_name = "";
      this.getWarehouseArea();
    },
    onImport() {
      this.dialogImportVisible = true;
      this.areaId = "";
      this.file = null;
      this.fileName = "";
    },

    onInventoryDetails(evt) {
      this.$router.push({
        name: "warehouseInventoryDetails",
        query: {
          warehouse_id: evt.warehouse_id
        }
      });
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Area";
      this.areaData = JSON.parse(JSON.stringify(event));
      this.dialogAddVisible = true;
    },
    // 详情
    onDetails(data) {
      const type = data.type_code;
      switch (type) {
        case "Common":
          this.$router.push({
            name: "warehouseLocation",
            query: {
              warehouse_region_id: data.warehouse_region_id,
              warehouse_id: data.warehouse_id,
              area_id: data.id
            }
          });
          break;
        case "Storage Cache":
          this.$router.push({
            name: "warehouseStorageCache",
            query: {
              warehouse_region_id: data.warehouse_region_id,
              warehouse_id: data.warehouse_id,
              area_id: data.id
            }
          });
          break;
        default:
          this.$router.push({
            name: "warehouseLocation",
            query: {
              warehouse_region_id: data.warehouse_region_id,
              warehouse_id: data.warehouse_id,
              area_id: data.id
            }
          });
          break;
      }

      // this.$store.commit("setChangeFilter", {
      //   warehouse_region_id: data.warehouse_region_id,
      //   warehouse_id: data.warehouse_id
      // });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getWarehouseArea();
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    // 获取仓库
    getAllRegion() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/region/all"
      });
    },
    // 获取所选仓库地区详情
    getWarehouseArea() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/warehouse/area/filter2",
        data: {
          warehouse_region_id: this.regionId,
          warehouse_id: this.warehouseId,
          area_code: this.formSearch.area_code,
          area_name: this.formSearch.area_name
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
    // 弹框确认
    dialogConfirm() {
      if (!this.required()) {
        return;
      }
      this.loading = true;
      this.postArea(this.areaData).then(
        res => {
          this.loading = false;
          this.dialogAddVisible = false;
          this.getWarehouseArea(this.warehouseId);
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
    addArea() {
      if (!this.regionId && !this.warehouseId) {
        this.$message({
          message: "Please select the warehouse area first",
          type: "warning"
        });
        return;
      } else {
        this.areaData = {
          type_code: "",
          type_name: "",
          area_name: "",
          area_code: "",
          warehouse_region_id: this.regionId,
          warehouse_id: this.warehouseId
        };
        this.dialogTitle = "Add Area";
        this.dialogAddVisible = true;
      }
    },
    // 删除弹窗 确认事件
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteArea(scope.currentID).then(
        res => {
          if (res.code === 201) {
            this.$message({
              message:
                "You can't delete the area because it contains the location that is being used.",
              type: "warning"
            });
          } else {
            scope.getWarehouseArea(this.warehouseId);
          }
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        },
        err => {
          this.loading = false;
        }
      );
    },
    postArea(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse/area",
        data: data
      });
    },
    deleteArea(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/warehouse/area/" + id
      });
    },
    // 弹窗必填
    required() {
      for (const item in this.areaData) {
        if (item === "update_user_id" || item === "update_time") {
          continue;
        }
        if (
          this.areaData[item] === "" ||
          this.areaData[item] == null ||
          this.areaData[item] == undefined
        ) {
          this.$message({
            message: "Please input " + item,
            type: "warning"
          });
          return false;
        }
      }
      return true;
    },
    // 改变区域类型
    changeType(type_code) {
      const data = this.warehouseTypeList.find(res => {
        return res.type_code === type_code;
      });
      if (data) {
        this.areaData.type_name = data.type_name;
      }
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.warehouse-area {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
  .file {
    position: relative;
    .label {
      display: inline-block;
      height: 26px;
      line-height: 26px;
      background-color: #e26829;
      border-radius: 26px;
      padding-left: 25px;
      padding-right: 30px;
    }
    span {
      display: block;
      width: 400px;
      height: 20px;
      margin: 10px 5px;
      overflow: hidden;
      text-overflow: ellipsis;
      -o-text-overflow: ellipsis;
      white-space: nowrap;
    }
    .file-input {
      width: 300px;
      position: absolute;
      left: 0;
      top: 0;
      opacity: 0;
      z-index: 2;
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
        width: 260px;
        margin-right: 5px;
      }
    }
  }
  .dialog-import {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 66px;
    }
    .item {
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
  .cascader {
    .el-cascader__label {
      line-height: inherit;
    }

    ::-webkit-scrollbar {
      width: 5px;
      height: 14px;
    }
  }
}
</style>



// WEBPACK FOOTER //
// warehouse-area.vue?e32368c8