<template>
  <div class="clean-record">
    <div class="search-header">
      <!-- <div class="btn-group-inline">
        <div class="icons">
          <span class="icon-item back" @click="goAfterIndex" title="back"></span>
        </div>
      </div>-->
      <el-form :inline="true" :model="formSearch" class="list-form-inline">
        <el-form-item label="CI No.">
          <el-input v-model="formSearch.ci_no" placeholder="CI No."></el-input>
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
            v-if="true"
            title="add"
            v-ilpermission="'clean-record-add'"
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
              <td>{{ item.ci_no }}</td>
              <td>{{ pipeCustomer(item.customer_id) }}</td>
              <td>{{ pipeType(item.clean_type) }}</td>
              <td>{{ item.invoice_no }}</td>
              <td>{{ item.deliver_order_no }}</td>
              <td>{{ pipeStatus(item.status) }}</td>
              <td>
                <a
                  class="btn-underline"
                  @click.stop="onEdit(item)"
                  v-ilpermission="'clean-record-add'"
                >edit</a>
                <a
                  class="btn-underline"
                  @click.stop="onDelete(item)"
                  v-ilpermission="'clean-record-delete'"
                >delete</a>
                <el-dropdown
                  trigger="click"
                  class="statusDrop"
                  @command="handleCommand($event,item)"
                  v-ilpermission="'clean-record-review'"
                >
                  <span class="el-dropdown-link">
                    action
                    <i class="el-icon-caret-bottom el-icon--right"></i>
                  </span>
                  <el-dropdown-menu slot="dropdown">
                    <el-dropdown-item
                      command="EXPORT_CLEAN"
                      :disabled="item.status === 1 ||item.status === 2 || item.status === 3"
                    >EXPORT_CLEAN</el-dropdown-item>
                    <el-dropdown-item
                      command="IMPORT_CLEAN"
                      :disabled="item.status !== 1 ||item.status === 2 || item.status === 3"
                    >IMPORT_CLEAN</el-dropdown-item>
                    <el-dropdown-item
                      command="STATUS_FINISH"
                      :disabled="item.status === 3"
                    >STATUS_FINISH</el-dropdown-item>
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
    <!-- 添加or修改弹窗 -->
    <el-dialog
      :title.sync="dialogTitle"
      :visible.sync="dialogVisible"
      custom-class="edit-dialog"
      class="dialog-cleanRecord"
    >
      <div class="item">
        <label class="title">
          CI No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input input-sn" v-model="formData.ci_no" placeholder="CI No."></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Delivery Order No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-autocomplete
            style="width:100%"
            class="input"
            v-model="formData.deliver_order_no"
            :fetch-suggestions="querySearchAsync"
            placeholder="Delivery Order No."
            @select="handleInputConfirm"
          ></el-autocomplete>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Customer
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            v-model="formData.customer_id"
            @change="handleChange"
            slot="append"
            class="land"
            filterable
            placeholder="Customer"
          >
            <el-option
              v-for="item in customerList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Invoice No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input class="input input-sn" v-model="formData.invoice_no" placeholder="Invoice No."></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Clean Type
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            v-model="formData.clean_type"
            slot="append"
            class="land"
            filterable
            placeholder="Clean Type"
          >
            <el-option v-for="item in cleanType" :key="item.id" :label="item.name" :value="item.id"></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">Other No.</label>
        <div class="content">
          <el-input class="input input-sn" v-model="formData.other" placeholder="Other No."></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Country
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-select
            v-model="formData.country"
            slot="append"
            class="land"
            filterable
            placeholder="Country"
            disabled
          >
            <el-option
              v-for="item in areaCodeList"
              :key="item.area_en"
              :label="item.area_en"
              :value="item.area_en"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Vat No.
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-input v-model="formData.vat" placeholder="Vat No." disabled></el-input>
        </div>
      </div>
      <div class="item">
        <label class="title">
          Sailing Date
          <span class="required">*</span>
        </label>
        <div class="content">
          <el-date-picker
            style="width:100%"
            class="input"
            v-model="sailing_date"
            type="date"
            placeholder="Sailing Date"
          ></el-date-picker>
        </div>
      </div>
      <div class="item-row">
        <div class="content text-area">
          <div>
            <el-upload
              class="btn btn-primary upload-btn"
              :action="upload.url"
              :data="upload.params"
              :multiple="false"
              :on-change="ci_file_change"
              :on-remove="ci_file_remove"
              :on-success="ci_file_success"
              :on-preview="file_preview"
              :file-list="upload.ci_file"
            >
              <img
                v-if="upload.ci_file.length === 0"
                class="upload-icon"
                src="../../../assets/images/icon/icon-upload@2x.png"
              >
            </el-upload>
            <p>CI</p>
          </div>
          <div>
            <el-upload
              class="btn btn-primary upload-btn"
              :action="upload.url"
              :data="upload.params"
              :multiple="false"
              :on-change="invoice_file_change"
              :on-remove="invoice_file_remove"
              :on-success="invoice_file_success"
              :on-preview="file_preview"
              :file-list="upload.invoice_file"
            >
              <img
                v-if="upload.invoice_file.length === 0"
                class="upload-icon"
                src="../../../assets/images/icon/icon-upload@2x.png"
              >
            </el-upload>
            <p>Invoice</p>
          </div>
          <div>
            <el-upload
              class="btn btn-primary upload-btn"
              :action="upload.url"
              :data="upload.params"
              :multiple="false"
              :on-change="other_file_change"
              :on-remove="other_file_remove"
              :on-success="other_file_success"
              :on-preview="file_preview"
              :file-list="upload.other_file"
            >
              <img
                v-if="upload.other_file.length === 0"
                class="upload-icon"
                src="../../../assets/images/icon/icon-upload@2x.png"
              >
            </el-upload>
            <p>Other</p>
          </div>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
    <!-- 删除弹窗 -->
    <el-dialog title="Delete Warehous" :visible.sync="dialogDeleteVisible" size="tiny">
      <p>Are you sure to delete it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="deleteInvitation">yes</el-button>
        <el-button type="primary" @click="dialogDeleteVisible = false">no</el-button>
      </div>
    </el-dialog>
    <!-- 审核弹窗 -->
    <el-dialog title="Review Shipment Order" :visible.sync="dialogReviewVisible" size="tiny">
      <p>Are you sure to do it ?</p>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="reviewInvitation">yes</el-button>
        <el-button type="primary" @click="dialogReviewVisible = false">no</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
import EN from "src/assets/lang/en";
import areaCodeInDB from "core/areaCode";
import CleanRecordService from "src/services/cleanRecordService";
// import CommercialInvoiceService from "src/services/commerialInvoiceService";
import WarehouseService from "src/services/warehouseService";
export default {
  data() {
    return {
      loading: false,
      // 查询条件
      formSearch: {
        ci_no: ""
      },
      headData: [],
      tableList: [],
      areaCodeList: [],
      currentPage: 1,
      total: 0,
      //当前操作ID
      currentID: "",
      // 交货单
      deliveryNoData: [],
      // 弹窗
      formData: {
        clean_type: 0,
        org_id: Core.Data.getOrg().id,
        customer_id: 0,
        ci_no: "",
        ci_file_id: "",
        country: "",
        vat: "",
        sailing_date: "",
        invoice_no: "",
        invoice_file_id: "",
        other: "",
        other_file_id: "",
        operator_id: Core.Data.getUser() ? Core.Data.getUser().id : "",
        deliver_order_no: ""
      },
      //上传
      upload: {
        url: Core.Const.NET.FILE_UPLOAD_END_POINT,
        paramName: "file",
        params: { token: Core.Data.getToken() },
        ci_file: [],
        other_file: [],
        invoice_file: []
      },
      dialogTitle: "",
      dialogVisible: false,
      // 删除弹窗
      dialogDeleteVisible: false,
      dialogReviewVisible: false,
      customerList: [],
      sailing_date: new Date(),
      statusData: [
        {
          id: 0,
          name: "STATUS_READY"
        },
        {
          id: 1,
          name: "EXPORT_CLEAN"
        },
        {
          id: 2,
          name: "IMPORT_CLEAN"
        },
        {
          id: 3,
          name: "STATUS_FINISH"
        }
      ],
      cleanType: [
        {
          id: 0,
          name: "TYPE_IN"
        },
        {
          id: 1,
          name: "TYPE_OUT"
        }
      ],
      ciList: [],
      timer: ""
    };
  },
  i18n: {
    messages: {
      en: EN.Dealer.Index
    }
  },
  async created() {
    const scope = this;
    this.areaCodeList = areaCodeInDB.init();
    this.getCleanRecord();

    this.loading = true;
    await WarehouseService.getAllDelivery()
      .then(item => {
        let data = item.data.filter(res => {
          return res.status === 3;
        });
        scope.deliveryNoData = data.map(res => {
          return {
            id: res.id,
            company_id: res.company_id,
            value: res.delivery_order_no,
            status: res.status,
            sales_name: res.create_user_id,
            shipment: res.shipment,
            origin: res.origin,
            destination: res.destination
          };
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

    scope.loading = true;
    await WarehouseService.getAllCustomer()
      .then(res => {
        scope.customerList = res.data;
        scope.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.loading = false;
      });
  },
  computed: {},
  mounted() {
    this.headData = [
      "CI No.",
      "Customer",
      "Clean Type",
      "Invoice No.",
      "Deliver Order No.",
      "Status",
      "Operation"
    ];
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "afterIndex" });
    },
    // 改变状态
    handleCommand(event, item) {
      this.status = event;
      this.currentID = item.id;
      this.dialogReviewVisible = true;
    },
    // 输入搜索
    createStateFilter(queryString) {
      return state => {
        return state.value.indexOf(queryString.toLowerCase()) !== -1;
      };
    },
    querySearchAsync(queryString, cb) {
      var restaurants = this.deliveryNoData;
      var results = queryString
        ? restaurants.filter(this.createStateFilter(queryString))
        : restaurants;
      cb(results);
    },
    handleInputConfirm(item) {
      this.formData.deliver_order_no = item.value;
      this.formData.customer_id = item.company_id;
    },
    // CI联动
    // ciChange() {
    //   if (this.formData.ci_no) {
    //     if (this.timer) {
    //       clearTimeout(this.timer);
    //     }
    //     this.timer = setTimeout(() => {
    //       const scope = this;
    //       this.loading = true;
    //       CommercialInvoiceService.filterCommercialInvoice(1, {
    //         ci_no: this.formData.ci_no
    //       })
    //         .then(res => {
    //           scope.loading = false;
    //           if(res.data.content)
    //         })
    //         .catch(err => {
    //           scope.$message({
    //             message: err,
    //             type: "error"
    //           });
    //           scope.loading = false;
    //         });
    //     }, 1000);
    //   }
    // },
    // 客户联动
    handleChange(id) {
      const data = this.customerList.find(res => {
        return res.id === id;
      });
      if (!data) {
        this.formData.vat = "";
        this.formData.country = "";
        return;
      }
      this.formData.vat = data.vat_no;
      this.formData.country = data.country;
    },
    //ci 上传
    ci_file_success(response, file, fileList) {
      this.formData.ci_file_id = response.data.file.id;
    },
    ci_file_change(response, fileList) {
      this.upload.ci_file = fileList.slice(-1);
    },
    ci_file_remove(file, fileList) {
      this.upload.ci_file = fileList;
    },
    file_preview(file) {
      let url_post;
      if (file.response) {
        const _file = file.response.data.file;
        url_post =
          Core.Const.NET.FILE_URL_PREFIX + _file.hash + "." + _file.ext;
      } else {
        url_post = Core.Const.NET.FILE_URL_PREFIX + file.hash + "." + file.ext;
      }
      window.open(url_post);
    },
    //invoice 上传
    invoice_file_success(response, file, fileList) {
      this.formData.invoice_file_id = response.data.file.id;
    },
    invoice_file_change(response, fileList) {
      this.upload.invoice_file = fileList.slice(-1);
    },
    invoice_file_remove(file, fileList) {
      this.upload.invoice_file = fileList;
    },
    //ci 上传
    other_file_success(response, file, fileList) {
      this.formData.other_file_id = response.data.file.id;
    },
    other_file_change(response, fileList) {
      this.upload.other_file = fileList.slice(-1);
    },
    other_file_remove(file, fileList) {
      this.upload.other_file = fileList;
    },
    // 上传 end

    // 状态
    pipeStatus(id) {
      const data = this.statusData.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    // 客户
    pipeCustomer(id) {
      const data = this.customerList.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    // 清关类型
    pipeType(id) {
      const data = this.cleanType.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    //确认审核
    reviewInvitation() {
      const scope = this;
      scope.dialogReviewVisible = false;
      scope.loading = true;
      CleanRecordService.statusCleanRecord(this.currentID, this.status)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.$message({
            message: "Review Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.getCleanRecord();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 搜索
    onSearch() {
      this.getCleanRecord();
    },
    onClean() {
      this.currentPage = 1;
      this.formSearch = {
        ci_no: ""
      };
      this.getCleanRecord();
    },
    // 操作
    async onEdit(event) {
      this.upload = {
        url: Core.Const.NET.FILE_UPLOAD_END_POINT,
        paramName: "file",
        params: { token: Core.Data.getToken() },
        ci_file: [],
        other_file: [],
        invoice_file: []
      };
      const fileList = await this.loadImg();
      this.formData = JSON.parse(JSON.stringify(event));

      this.dialogTitle = "Update Clean Record";
      this.dialogVisible = true;
    },
    async loadImg() {
      const ciPro = new Promise((resolve, reject) => {
        Core.Api.FileError.findFile(this.formData.ci_file_id).then(
          res => {
            this.upload.ci_file.push(res.file);
            resolve(res);
          },
          err => {
            reject(err);
          }
        );
      });

      const invoicePro = new Promise((resolve, reject) => {
        Core.Api.FileError.findFile(this.formData.invoice_file_id).then(
          res => {
            this.upload.invoice_file.push(res.file);
            resolve(res);
          },
          err => {
            reject(err);
          }
        );
      });
      const otherPro = new Promise((resolve, reject) => {
        Core.Api.FileError.findFile(this.formData.other_file_id).then(
          res => {
            this.upload.other_file.push(res.file);
            resolve(res);
          },
          err => {
            reject(err);
          }
        );
      });

      return Promise.all([ciPro, invoicePro, otherPro]);
    },
    onDelete(event) {
      this.currentID = event.id;
      this.dialogDeleteVisible = true;
    },
    // 添加
    dialogAdd() {
      this.formData = {
        clean_type: 0,
        org_id: Core.Data.getOrg().id,
        customer_id: "",
        ci_no: "",
        ci_file_id: "",
        country: "",
        vat: "",
        sailing_date: "",
        invoice_no: "",
        invoice_file_id: "",
        other: "",
        other_file_id: "",
        operator_id: Core.Data.getUser() ? Core.Data.getUser().id : "",
        deliver_order_no: ""
      };
      this.upload = {
        url: Core.Const.NET.FILE_UPLOAD_END_POINT,
        paramName: "file",
        params: { token: Core.Data.getToken() },
        ci_file: [],
        other_file: [],
        invoice_file: []
      };
      this.dialogTitle = "Add Clean Record";
      this.dialogVisible = true;
    },
    // 弹窗确认
    dialogConfirm() {
      if (!this.required()) {
        return;
      }
      this.loading = true;
      this.formData.sailing_date = Core.Util.parseDate(this.sailing_date);
      CleanRecordService.postCleanRecord(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          this.loading = false;
          this.dialogVisible = false;
          this.getCleanRecord();
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
    // 弹窗取消
    dialogCancel() {
      this.dialogVisible = false;
    },
    // 删除 弹窗确认
    deleteInvitation() {
      const scope = this;
      scope.loading = true;
      CleanRecordService.deleteCleanRecord(scope.currentID)
        .then(res => {
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          scope.loading = false;
          scope.dialogDeleteVisible = false;
          scope.getCleanRecord();
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        });
    },
    // 分页
    handleCurrentChange(val) {
      this.currentPage = val;
      Core.Data.set(Core.Const.DATA.KEY_AFTER_SALES_PAGE, this.currentPage);
      this.getCleanRecord();
    },
    getCleanRecord() {
      this.loading = true;
      CleanRecordService.filterCleanRecord(this.currentPage, this.formSearch)
        .then(res => {
          this.tableList = res.data.content;
          this.total = res.data.totalElements;
          this.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
          scope.dialogDeleteVisible = false;
        });
    },
    // 弹窗必填
    required() {
      if (this.formData.ci_no === "") {
        this.$message({
          message: "Please add CI No.",
          type: "warning"
        });
        return false;
      }

      if (this.formData.customer_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }
      if (this.formData.clean_type === "") {
        this.$message({
          message: "Please add Clean Type",
          type: "warning"
        });
        return false;
      }
      if (this.formData.invoice_no === "") {
        this.$message({
          message: "Please add Invoice No.",
          type: "warning"
        });
        return false;
      }
      return true;
    }
  },
  watch: {}
};
</script>

<style lang="scss" rel="stylesheet/scss">
.clean-record {
  display: flex;
  flex-direction: column;
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .el-date-editor {
    width: 250px;
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
  }

  .input-sn:after {
    top: 5px;
  }

  .item-row {
    width: 100%;
    .text-area {
      display: flex;
      align-items: center;
      justify-content: space-around;
      width: 100%;
      margin: 40px 0 10px 0;
      p {
        text-align: center;
        padding-top: 20px;
      }
    }
  }

  .dialog-cleanRecord {
    overflow: hidden;
    .el-dialog__body {
      font-size: 0;
      padding: 30px 20px 30px 35px;
    }
    .item {
      width: 50%;
      line-height: 30px;
      display: inline-block;
      .land {
        width: 230px;
      }
      .title {
        display: inline-block;
        width: 130px;
      }
      .content {
        margin: 10px;
        display: inline-block;
        width: 230px;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// clean-record.vue?b9fad2b0