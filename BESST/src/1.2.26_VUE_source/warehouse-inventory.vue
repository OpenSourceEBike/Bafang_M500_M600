
<template>
    <div class="warehouse-inventory">
        <div class="category-tree">
            <div class="searchWrap">
                <el-form :inline="true" class="demo-form-inline">
                    <el-form-item>
                        <el-input v-model="part_no" placeholder="Item Code"></el-input>
                        <div class="btn-group-inline">
                            <div class="icons">
                                <span class="icon-item search" @click="getItemModel" title="search"></span>
                            </div>
                        </div>
                    </el-form-item>
                </el-form>
            </div>
            <el-tree
                :data="categoryList"
                :props="defaultProps"
                @node-click="handleNodeClick"
                node-key="id"
                :default-expanded-keys="categoryExId"
            ></el-tree>
        </div>
        <div class="inventory-list-container">
            <div class="search-header">
                <el-form :inline="true" :model="formSearch" class="list-form-inline">
                    <el-form-item label="Warehouse">
                        <el-cascader
                            class="cascader"
                            :options="options"
                            filterable
                            @change="handleItemChange"
                            :props="props"
                            :value="cascaderValue"
                        ></el-cascader>
                    </el-form-item>
                    <el-form-item label="Category">
                        <el-select v-model="formSearch.category_root_id" placeholder="Category">
                            <el-option
                                v-for="category in categoryList"
                                :key="category.id"
                                :label="category.name"
                                :value="category.id"
                            ></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Item Code">
                        <el-input
                            class="input"
                            v-model="formSearch.item_code"
                            placeholder="Item Code"
                        ></el-input>
                    </el-form-item>
                    <div class="btn-group-inline-icon">
                        <div class="icons">
                            <div class="icon-item search" @click="onSearch" title="search"></div>
                            <div class="icon-item clear" @click="onClean" title="clear"></div>
                            <div class="icon-item upload" @click="onImport" title="upload"></div>
                        </div>
                    </div>
                </el-form>
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
                                <td>{{ item.item_name }}</td>
                                <td>{{ item.item_code }}</td>
                                <td>{{ warehousePipe(item) }}</td>
                                <td>{{ item.areaName }}</td>
                                <td>{{ item.location_no }}</td>
                                <td>{{ item.on_hand_quantity }}</td>
                                <td>{{ item.allocated_quantity }}</td>
                                <td>{{ item.picked_quantity }}</td>
                                <td>{{ item.pallet_no }}</td>
                                <td>{{ item.carton_no }}</td>
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
</template>;

<script>
import Core from "core/core";
import WarehouseService from "src/services/warehouseService";
import InventoryService from "src/services/inventoryService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: {
        warehouse_id: "",
        category_root_id: "",
        item_code: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //当前操作ID
      currentID: "",
      warehouseList: [],
      // 级联数据
      options: [],
      props: {
        label: "name",
        value: "id",
        children: "warehouse"
      },
      cascaderValue: [],
      categoryList: [],
      defaultProps: {
        id: "id",
        // children: "child",
        label: "name"
      },
      categoryExId: [],
      part_no: "",
      dialogImportVisible: false,
      areaList: [],
      //上传
      areaId: "",
      file: "",
      fileName: ""
    };
  },
  i18n: {
    messages: {}
  },
  async created() {
    await WarehouseService.getAllArea().then(res => {
      this.areaList = res.data;
    });
  },
  computed: {},
  mounted() {
    const scope = this;
    this.headData = [
      "Item Name",
      "Item Code",
      "Warehouse",
      "Area",
      "Location No.",
      "On Hand Quantity",
      "Allocated Quantity",
      "Picked Quantity",
      "Pallet No.",
      "Carton No."
    ];

    this.loading = true;
    WarehouseService.getAllCategory().then(res => {
      scope.categoryList = res.category_list;
      scope.loading = false;
    });

    this.getWarehouseInventory();

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
          scope.loading = false;
        });
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });
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
    // 上传
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
      InventoryService.uploadBitchInventory(fd, {
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
    // 物料节点选中事件
    handleNodeClick(data) {
      if (data.hasOwnProperty("id")) {
        this.formSearch.category_root_id = data.id;
        this.getWarehouseInventory();
      }
    },
    // 物料查询
    getItemModel() {
      if (this.part_no) {
        this.formSearch.item_code = this.part_no;
        this.getWarehouseInventory();
      } else {
        return;
      }
    },
    handleItemChange(val) {
      this.formSearch.warehouse_id = val[1];
      this.getWarehouseInventory();
    },
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      // 在下方添加查询逻辑
      this.getWarehouseInventory();
    },
    onClean() {
      this.cascaderValue = [];
      this.formSearch = {
        warehouse_id: ""
      };
      this.getWarehouseInventory();
    },
    onImport() {
      this.dialogImportVisible = true;
      this.areaId = "";
      this.file = null;
      this.fileName = "";
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getWarehouseInventory();
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
    // 获取库存
    getWarehouseInventory() {
      this.loading = true;

      Core.Api.request({
        method: "post",
        url: "/nl/warehouse/inventory/findByCategoryRootId",
        data: {},
        params: {
          page: this.currentPage,
          size: 10,
          warehouseId: this.formSearch.warehouse_id,
          categoryRootId: this.formSearch.category_root_id,
          itemCode: this.formSearch.item_code
        }
      })
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.tableList = res.content;
          this.total = res.totalElements;
          this.loading = false;
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
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.warehouse-inventory {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  .inventory-list-container {
    width: 100%;
    height: 100%;
    padding-left: 272px;
  }
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

  .category-tree {
    position: absolute;
    left: 0;
    top: 0;
    overflow: auto;
    width: 272px;
    height: 100%;
    background: rgba(39, 43, 55, 0.75);
    border-right: 1px solid #383d49;
    .searchWrap {
      background: #383d49;
      height: 60px;
      .demo-form-inline {
        padding-top: 10px;
      }

    }
    .el-tree {
      width: 270px;
    }

    .tree-node {
      white-space: nowrap;
      .tree-node__content {
        height: 50px;
        line-height: 50px;
        padding-left: 0;
        cursor: pointer;
        .tree-node__label {
          padding-left: 6px;
        }
      }
    }

    .tree-btn {
      height: 50px;
      line-height: 50px;
      a {
        font-size: 14px;
        color: #e26829;
        font-weight: 500;
        text-decoration: underline;
        margin-left: 26px;
      }
      a:hover {
        cursor: pointer;
      }
    }
    .training-btn {
      a {
        color: white;
        text-decoration: none;
      }
    }
  }

  .search-header {
    padding: 0;
    height: 60px;
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
      width: 80px;
      margin-left: 5px;
      text-align: left;
    }
    .el-form-item__content {
      width: 180px !important;
    }
  }
  .loading {
    width: 100px;
    height: 100px;
    position: absolute;
    top: 50%;
    left: 50%;
    margin-top: -50px;
    margin-left: -50px;
  }

  .table-wrap {
    .long-td {
      max-width: 270px;
      text-overflow: ellipsis;
      overflow: hidden;
      text-align: left;
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
  .el-input-number {
    width: 220px;
  }
}
</style>



// WEBPACK FOOTER //
// warehouse-inventory.vue?d9eb27f8