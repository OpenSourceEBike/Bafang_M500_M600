<template>
  <div class="warehouse-location">
    <div class="search-header">
      <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="backArea" title="back"></span>
        </div>
      </div>
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Location No.">
          <el-input v-model="formSearch.location_no" placeholder="Location No."></el-input>
        </el-form-item>
        <el-form-item label="Location Name">
          <el-input v-model="formSearch.location_name" placeholder="Location Name"></el-input>
        </el-form-item>
        <el-form-item label="Type Code">
          <el-input v-model="formSearch.type_code" placeholder="Type Code"></el-input>
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
          <div class="icon-item add-v2" @click="addAreaLocation" v-if="true" title="add"></div>
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
              <td>{{ item.location_no }}</td>
              <td>{{ item.location_name }}</td>
              <td>{{ item.max_quantity }}</td>
              <td>{{ item.max_cartons }}</td>
              <td>{{ item.weight_capacity }}</td>
              <td>{{ item.category_code}}</td>
              <td>{{ item.cube}}</td>
              <!-- <td>{{ item.height}}</td>
              <td>{{ item.length}}</td>
              <td>{{ item.width}}</td>
              <td>{{ item.weight}}</td>
              <td>{{ item.type_code}}</td>
              <td>{{ item.x_coord}}</td>
              <td>{{ item.y_coord}}</td>
              <td>{{ item.z_coord}}</td>-->
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
          Location No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.location_no"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Location Name
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.location_name"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Max Quantity
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.max_quantity"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Max Cartons
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.max_cartons"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Weight Capacity (Kg)
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.weight_capacity"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          {{$t('category')}}
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input
            :placeholder="$t('category')"
            class="input"
            :disabled="true"
            v-model="selectCategoryItem.name"
            @click.native="selectCategory"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Cube (m&sup3;)</label>
        <div class="content">
          <el-input v-model="formData.cube"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Height (m)</label>
        <div class="content">
          <el-input v-model="formData.height"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Length (m)</label>
        <div class="content">
          <el-input v-model="formData.length"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Width (m)</label>
        <div class="content">
          <el-input v-model="formData.width"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Type Code</label>
        <div class="content">
          <el-input v-model="formData.type_code"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">X Coord (m)</label>
        <div class="content">
          <el-input v-model="formData.x_coord"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Y Coord (m)</label>
        <div class="content">
          <el-input v-model="formData.y_coord"></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">Z Coord (m)</label>
        <div class="content">
          <el-input v-model="formData.z_coord"></el-input>
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
    <!--  -->
    <el-dialog
      class="dialog-category"
      :title="$t('selectCategory')"
      :visible.sync="dialogSelectCategoryVisible"
      size="large"
    >
      <div class="content" style="max-height: 500px; overflow: auto">
        <div class="category-tree">
          <el-tree :data="categoryList" :props="defaultProps" @node-click="handleNodeClick"></el-tree>
        </div>
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

export default {
  data() {
    return {
      loading: false,
      formSearch: {
        location_no: "",
        location_name: "",
        type_code: ""
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
      //
      defaultProps: {
        id: "id",
        children: "child",
        label: "name"
      },
      categoryList: [],
      selectCategoryItem: {
        id: "",
        name: ""
      },
      dialogSelectCategoryVisible: false
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
  mounted() {
    this.getAreaLocation();

    this.getAllCategory();

    this.headData = [
      "Location No.",
      "Location Name",
      "Max Quantity",
      "Max Cartons",
      "Weight Capacity",
      "Category Code",
      "Cube",
      // "Height",
      // "Length",
      // "Width",
      // "Weight",
      // "Type Code",
      // "X Coord",
      // "Y Coord",
      // "Z Coord",
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
    handleNodeClick(data) {
      if (data.hasOwnProperty("child")) {
        let childList = data.child;
        if (childList.length <= 0) {
          this.selectCategoryItem = data;
          this.dialogSelectCategoryVisible = false;
        }
      }
    },
    getAllCategory() {
      this.loading = true;
      Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
      ).then(res => {
        this.categoryList = res.category_list;
        this.loading = false;
      });
    },

    selectCategory() {
      this.dialogSelectCategoryVisible = true;
    },
    // 搜索
    onSearch() {
      this.getAreaLocation();
    },
    onClean() {
      this.formSearch.location_name = "";
      this.formSearch.location_no = "";
      this.formSearch.type_code = "";
      this.getAreaLocation();
    },
    // 操作
    onEdit(event) {
      this.dialogTitle = "Edit Area";
      this.formData = JSON.parse(JSON.stringify(event));
      this.selectCategoryItem.name = this.formData.category_code;
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
    getAreaLocation() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/warehouse/location/filter2",
        data: {
          warehouse_region_id: this.$route.query.warehouse_region_id,
          warehouse_id: this.$route.query.warehouse_id,
          area_id: this.$route.query.area_id,
          location_no: this.formSearch.location_no,
          location_name: this.formSearch.location_name,
          type_code: this.formSearch.type_code
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
    // 获取api
    postAreaLocation(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/warehouse/location",
        data: data
      });
    },
    // 新增弹窗打开事件
    addAreaLocation() {
      this.dialogTitle = "Add";
      this.dialogAddVisible = true;
      this.formData = {
        max_quantity: "",
        max_cartons: "",
        location_no: "",
        weight_capacity: "",
        x_coord: "",
        category_code: "",
        length: "",
        active: true,
        z_coord: "",
        area_id: this.$route.query.area_id,
        location_name: "",
        width: "",
        y_coord: "",
        cube: "",
        warehouse_id: this.$route.query.warehouse_id,
        warehouse_region_id: this.$route.query.warehouse_region_id,
        height: "",
        type_code: ""
      };
    },
    // 增/改 弹窗确认事件
    dialogConfirm() {
      this.formData.category_code = this.selectCategoryItem.name;
      if (!this.checkParams()) {
        return;
      }
      this.loading = true;
      this.postAreaLocation(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.loading = false;
          this.dialogAddVisible = false;
          this.getAreaLocation();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
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
      this.deleteAreaLocation(scope.currentID).then(
        res => {
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.getAreaLocation();
        },
        err => {
          this.loading = false;
        }
      );
    },
    // 弹窗必填
    checkParams() {
      if (this.formData.location_no === "") {
        this.$message({
          message: "Please add Location No.",
          type: "warning"
        });
        return false;
      }

      if (this.formData.location_name === "") {
        this.$message({
          message: "Please add Location Name",
          type: "warning"
        });
        return false;
      }

      if (this.formData.max_quantity === "") {
        this.$message({
          message: "Please add Max Quantity",
          type: "warning"
        });
        return false;
      }

      if (this.formData.max_cartons === "") {
        this.$message({
          message: "Please add Max Cartons",
          type: "warning"
        });
        return false;
      }

      if (this.formData.weight_capacity === "") {
        this.$message({
          message: "Please add Weight Capacity",
          type: "warning"
        });
        return false;
      }

      if (this.formData.category_code === "") {
        this.$message({
          message: "Please add Category",
          type: "warning"
        });
        return false;
      }

      return true;
    },
    deleteAreaLocation(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/warehouse/location/" + id
      });
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.warehouse-location {
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
// warehouse-location.vue?ba5f3658