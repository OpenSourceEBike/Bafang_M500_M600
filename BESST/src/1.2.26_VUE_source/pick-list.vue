<template>
  <div class="pick-list">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div> -->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="Shipment Order No.">
          <el-input v-model="formSearch.shipment_order_no" placeholder="Shipment Order No."></el-input>
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
            v-ilpermission="'warerhouse-pick-add'"
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
              <td>{{ item.shipment_order_no }}</td>
              <td>{{ conversion(item.warehouse_id) }}</td>
              <td>{{ item.customer_order }}</td>
              <td>{{ item.operation_user_id }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'warerhouse-pick-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'warerhouse-pick-delete'"
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
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogDeleteVisible" size="tiny">
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
export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: {
        shipment_order_no: ""
      },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      //当前操作ID
      currentID: "",
      dialogDeleteVisible: false,
      dialogTitle: "",
      warehouseData: [],
      shipmentOrderData: []
    };
  },
  i18n: {
    messages: {}
  },
  created() {},
  computed: {},
  mounted() {
    const scope = this;
    this.headData = [
      "Shipment Order No.",
      "Warehouse",
      "Customer Order",
      "Operation User",
      "Operation"
    ];

    scope.getPick();
    scope.getAllWarehouse();
    scope.getShipment();
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 搜索
    onSearch() {
      // 在下方添加查询逻辑
      this.getPick();
    },
    onClean() {
      this.formSearch.shipment_order_no = "";
      this.getPick();
    },
    // 操作
    onEdit(event) {
      this.$router.push({
        name: "pickListAdd",
        query: {
          orderId: event.id
        }
      });
    },
    onDelete(event) {
      this.dialogTitle = "Delete Pick List";
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      //在下方添加分页查询逻辑
      this.getPick();
    },
    // 添加
    dialogAdd() {
      this.$router.push({ name: "pickListAdd" });
    },
    // 删除
    deleteInvitation() {
      this.loading = true;
      // 在下方添加删除逻辑
      this.deletePick(this.currentID).then(
        res => {
          this.loading = false;
          this.dialogDeleteVisible = false;
          this.getPick();
        },
        err => {
          this.loading = false;
          this.dialogDeleteVisible = false;
        }
      );
    },
    // 获取出货数据
    getShipment() {
      const scope = this;
      scope.loading = true;
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/filter",
        data: {
          status: 1
        },
        params: {
          page: 1,
          size: 10,
        }
      }).then(
        res => {
          scope.shipmentOrderData = res.content;
          scope.loading = false;
        },
        err => {
          scope.loading = false;
        }
      );
    },
    //获取所有仓库
    getAllWarehouse() {
      const scope = this;
      scope.loading = true;
      return Core.Api.request({
        method: "get",
        url: "/nl/warehouse"
      }).then(
        res => {
          scope.warehouseData = res.content;
          scope.loading = false;
        },
        err => {
          scope.loading = false;
        }
      );
    },
    // 获取拣货单
    getPick() {
      this.loading = true;
      return Core.Api.request({
        method: "post",
        url: "/nl/order/pick/filter",
        data: {
          shipment_order_no: this.formSearch.shipment_order_no
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
    // 删除拣货单
    deletePick(id) {
      this.loading = true;
      return Core.Api.request({
        method: "delete",
        url: "/nl/order/pick/" + id
      });
    },
    // 改变仓库显示
    conversion(warehouseId) {
      if (this.warehouseData.length > 0) {
        const data = this.warehouseData.filter(res => {
          return warehouseId === res.id;
        });
        if (data.length > 0) {
          return data[0].name;
        } else {
          return "-";
        }
      }
    },
    // changeView(id) {
    //   if (this.shipmentOrderData.length > 0) {
    //     const data = this.shipmentOrderData.filter(res => {
    //       return id == res.id;
    //     });
    //     if (data.length > 0) {
    //       return data[0].order_no;
    //     } else {
    //       return "-";
    //     }
    //   }
    // }
  },
  watch: {}
};
</script>
    
                                            
<style lang="scss" rel="stylesheet/scss">
.pick-list {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;
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
// pick-list.vue?2f042178