<template>
    <div class="customer-service">
        <div class="search-header">
            <div class="btn-group-inline">
                <div class="icons">
                    <span class="icon-item back" @click="$router.back()" title="back"></span>
                </div>
            </div>
            <el-form :inline="true" :model="formSearch" class="list-form-inline">
                <el-form-item label="Employee No.">
                    <el-input
                        v-model="formSearch.logistics_order_no"
                        placeholder="Logistics Order No."
                    ></el-input>
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
                    <div class="icon-item add-v2" @click="dialogAdd" title="add"></div>
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
                            <td>{{ item.logistics_order_no }}</td>
                            <td>{{ item.company_name }}</td>
                            <td>{{ item.contacts }}</td>
                            <td>{{ item.amount }}</td>
                            <td>{{ item.status }}</td>
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
        <!-- 添加or修改弹窗 -->
        <el-dialog
            :title.sync="dialogTitle"
            :visible.sync="dialogVisible"
            custom-class="edit-dialog"
            class="dialog-md"
        >
            <div class="item">
                <label class="title">
                    Logistics Order No.
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input
                        v-model="formData.logistics_order_no"
                        placeholder="Logistics Order No."
                    ></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Company Name
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.company_name" placeholder="Company Name"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Contacts
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.contacts" placeholder="Contacts"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Country
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-select class="input" v-model="formData.country" filterable slot="prepend">
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
                    Address
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.address" placeholder="Address"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Transportain Type
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.transportain_type" placeholder="Transportain Type"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Amount
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.amount" placeholder="Amount"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">
                    Metering
                    <span class="required">*</span>
                </label>
                <div class="content">
                    <el-input v-model="formData.metering" placeholder="Metering"></el-input>
                </div>
            </div>
            <div class="item">
                <label class="title">Priority</label>
                <div class="content">
                    <el-input-number
                        class="input-special"
                        v-model="formData.priority"
                        controls-position="right"
                        :min="1"
                        :max="5"
                    ></el-input-number>
                </div>
            </div>
            <div slot="footer" class="dialog-footer">
                <el-button type="preview" @click="dialogConfirm">confirm</el-button>
                <el-button type="primary" @click="dialogCancel">cancel</el-button>
            </div>
        </el-dialog>
        <!-- 删除弹窗 -->
        <el-dialog title="Delete Carrier Order" :visible.sync="dialogDeleteVisible" size="tiny">
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
import CarrierService from "src/services/carrierService";

export default {
  data() {
    return {
      loading: false,
      //搜索数据
      formSearch: { logistics_order_no: "" },
      headData: [],
      tableList: [],
      currentPage: 1,
      total: 0,
      areaCodeList: [],
      //弹窗表单数据,
      formData: {
        address: "",
        amount: 0,
        company_name: "",
        contacts: "",
        country: "",
        logistics_order_no: "",
        metering: "",
        priority: 0,
        status: 0,
        transportain_type: 0
      },
      //弹窗名称
      dialogTitle: "",
      //操作弹窗显示
      dialogVisible: false,
      //删除弹窗
      dialogDeleteVisible: false,
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
    this.areaCodeList = areaCodeInDB.init();
    this.headData = [
      "Logistics Order No.",
      "Company Name",
      "Contacts",
      "Amount",
      "Status",
      "Operation"
    ];
    this.filterCarrier();
  },
  methods: {
    // 搜索
    onSearch() {
      this.filterCarrier();
    },
    onClean() {
      this.formSearch = {};
      this.filterCarrier();
    },
    // 操作
    onEdit(event) {
      this.formData = JSON.parse(JSON.stringify(event));
      this.date = this.formData.birthday * 1000;
      this.dialogTitle = "Update Carrier Order";
      this.dialogVisible = true;
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.filterCarrier();
    },
    // 添加
    dialogAdd() {
      this.dialogTitle = "Add Carrier Order";
      this.dialogVisible = true;
      this.formData = {
        address: "",
        amount: 0,
        company_name: "",
        contacts: "",
        country: "",
        logistics_order_no: "",
        metering: "",
        priority: 0,
        status: 0,
        transportain_type: 0
      };
    },
    // 删除
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      CarrierService.deleteCarrier(this.currentID)
        .then(() => {
          this.loading = false;
          this.dialogDeleteVisible = false;
          scope.filterCarrier();
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
    dialogConfirm() {
      const scope = this;
      scope.loading = true;
      // this.formData.birthday = Core.Util.parseDate(this.date) || "";
      CarrierService.postCarrier(this.formData)
        .then(() => {
          this.$message({
            message: "Successfully saved",
            type: "success"
          });
          this.dialogVisible = false;
          this.loading = false;
          this.filterCarrier();
        })
        .catch(err => {
          this.$message({
            message: err,
            type: "error"
          });
          this.dialogVisible = false;
          this.loading = false;
        });
    },
    dialogCancel() {
      this.dialogVisible = false;
      this.formData = {
        employee_no: "",
        fullName: "",
        nickName: "",
        star: "",
        gender: "",
        birthday: ""
      };
      this.date = "";
    },
    // 弹窗必填
    checkParams() {
      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    filterCarrier() {
      const scope = this;
      scope.loading = true;
      CarrierService.filterCarrier(this.currentPage, this.formSearch).then(
        res => {
          scope.tableList = res.data.content;
          scope.total = res.data.totalElements;
          scope.loading = false;
        }
      );
    }
  },
  watch: {}
};
</script>


<style lang="scss" rel="stylesheet/scss">
.customer-service {
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
        width: 200px;
      }
      .title {
        display: inline-block;
        width: 135px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 200px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// carrier-order.vue?9ed06b5a