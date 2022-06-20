<template>
  <div class="customer-list">
    <div class="search-header">
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Name">
          <el-input v-model="formSearch.name" placeholder="Name"></el-input>
        </el-form-item>
        <el-form-item label="Customer No.">
          <el-input v-model="formSearch.company_no" placeholder="Customer No."></el-input>
        </el-form-item>
        <el-form-item label="Country">
          <el-select v-model="formSearch.country" filterable placeholder="Country">
            <el-option
              v-for="(item, index) in areaCodeList"
              :key="index"
              :label="item.area_en"
              :value="item.area_en"
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
            title="add"
            v-ilpermission="'customer-add'"
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
              <td>{{ item.name }}</td>
              <td>{{ item.company_no }}</td>
              <td>{{ item.country }}</td>
              <td>{{ item.email }}</td>
              <td>{{ item.mobile }}</td>
              <td>{{ item.street }}</td>
              <td>{{ item.address_no }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'customer-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'customer-delete'"
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
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Customer" :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>;

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { name: "", org_id: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //删除弹窗
      dialogDeleteVisible: false,
      //当前操作ID
      currentID: "",
      //当前权限数据
      currentPermisstion: [],
      areaCodeList: []
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.areaCodeList = areaCodeInDB.init();
    this.headData = [
      "Name",
      "Customer No.",
      "Country",
      "Email",
      "Phone",
      "Street",
      "No.",
      "Operation"
    ];
    this.filterCustomer();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.filterCustomer();
    },
    onClean() {
      const scope = this;
      this.currentPage = 1;
      this.formSearch = {
        name: "",
        company_no: "",
        country: ""
      };
      this.filterCustomer();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "customerAccount",
        query: { orderId: event.id }
      });
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.filterCustomer();
    },

    // 添加
    dialogAdd() {
      this.$router.push({
        name: "customerAccount"
      });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      this.deleteCustomer(scope.currentID)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.$message({
            message: "Delete Successfully",
            type: "success"
          });
          scope.filterCustomer();
          scope.dialogDeleteVisible = false;
          scope.loading = false;
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogVisible = true;
          this.loading = false;
        });
    },
    //查询顾客信息
    filterCustomer() {
      this.loading = true;
      Core.Api.request({
        method: "post",
        url: "/nl/nlPublic/company/filter",
        data: {
          name: this.formSearch.name,
          company_no: this.formSearch.company_no,
          country: this.formSearch.country
        },
        params: {
          page: this.currentPage,
          size: 10
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
          this.$message({
            message: err,
            type: "error"
          });
          this.loading = false;
        });
    },
    //删除顾客信息
    deleteCustomer(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/nlPublic/company/" + id
      });
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.customer-list {
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
      width: 100px;
      text-align: left;
      margin-left: 5px;
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

  .dialog-sm {
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

  .dialog-md {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 54px;
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
        width: 90px;
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
// customer-list.vue?c15e3e8a