<template>
  <div class="parcel-information">
    <div class="search-header">
      <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Parcel No.">
          <el-input v-model="formSearch.parcel_no" placeholder="Parcel No."></el-input>
        </el-form-item>
        <el-form-item label="Status">
          <el-select v-model="formSearch.status" filterable>
            <el-option v-for="item in statusList" :key="item" :label="item" :value="statusList.indexOf(item)"></el-option>
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
            v-ilpermission="'parcel-information-add'"
            @click="dialogAdd"
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
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.parcel_no }}</td>
              <td>{{ item.customer_name }}</td>
              <td>{{ item.vat_no }}</td>
              <td>{{ item.create_user_name}}</td>
              <td>{{ item.create_time | dateFormat}}</td>
              <td>{{ item.status|statusPipe(statusList) }}</td>
              <td>
                <a
                  class="btn-underline"
                  v-ilpermission="'parcel-information-add'"
                  @click.stop="onEdit(item)"
                >edit</a>
                <a
                  class="btn-underline"
                  v-ilpermission="'parcel-information-delete'"
                  @click.stop="onDelete(item)"
                >delete</a>
                <el-dropdown
                  v-ilpermission="'parcel-information-review'"
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item
                      command="Approved"
                      :disabled="item.status === 1 || item.status === 2"
                    >Approved</el-dropdown-item>
                    <el-dropdown-item command="Finished" :disabled="item.status === 2">Finished</el-dropdown-item>
                  </el-dropdown-menu>
                </el-dropdown>
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
    <!-- 审核弹窗 -->
    <el-dialog title="Review Shipment Order" :visible.sync="dialogReviewVisible" size="tiny">
      <p>Are you sure to do it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewInvitation">yes</el-button>
        <el-button type="primary" @click="dialogReviewVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Parcel Information" :visible.sync="dialogDeleteVisible" size="tiny">
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
import ParcelInformationService from "src/services/parcelInformationService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { parcel_no: "", status: "" },
      headData: [],
      tableList: [],
      statusList: ["Create", "Approved", "Finish", "Used"],
      currentPage: 1,
      total: 0,
      //删除弹窗
      dialogDeleteVisible: false,
      //审核弹窗
      dialogReviewVisible: false,
      //当前操作ID
      currentID: ""
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    this.headData = [
      "Parcel No.",
      "Customer",
      "Vat No.",
      "Operator",
      "Creat time",
      "Operation"
    ];
    this.filterParcelInfo();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      this.filterParcelInfo();
    },
    onClean() {
      this.formSearch = {};
      this.filterParcelInfo();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "ParcelInfoSave",
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
      this.filterParcelInfo();
    },
    // 添加
    dialogAdd() {
      this.$router.push({ name: "ParcelInfoSave" });
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      ParcelInformationService.deleteParcelInfo(this.currentID)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          this.loading = false;
          this.dialogDeleteVisible = false;
          scope.filterParcelInfo();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogDeleteVisible = false;
          this.loading = false;
        });
    },
    filterParcelInfo() {
      const scope = this;
      scope.loading = true;
      ParcelInformationService.filterParcelInfo(
        this.currentPage,
        this.formSearch
      )
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.tableList = res.data.content;
          scope.total = res.data.totalElements;
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      const status = this.status;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      ParcelInformationService.putParcelInfoStatus(this.currentID, status)
        .then(res => {
          if (res.data.code === -2) {
            throw res.data.message;
          }
          scope.$message({
            message: "Successful Review",
            type: "success"
          });
          scope.loading = false;
          scope.filterParcelInfo();
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
.parcel-information {
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

  .dialog-md {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 48px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .input-special {
        width: 250px;
      }
      .title {
        display: inline-block;
        width: 98px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 250px;
        .country-code {
          width: 94px;
        }
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// parcel-information.vue?53d465be