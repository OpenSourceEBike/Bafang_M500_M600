<template>
  <div>
    <div class="header">
      <div class="btn-group-inline">
        <div class="icons">
          <div class="icon-item back" @click="goAfterIndex" title="back"></div>
        </div>
      </div>
      <label class="title">{{ id ? 'Edit Stock in' : 'Add Stock in' }}</label>
      <div class="btn-group-top">
        <div class="icons">
          <div
            class="icon-item save"
            @click="onSave"
            v-if="!editControl || formData.status==0"
            title="save"
          ></div>
        </div>
      </div>
    </div>
    <div class="content-wrap" v-loading.body="loading">
      <div class="content">
        <div class="order-info">
          <label class="title">Order Info</label>
          <div class="left">
            <div class="item">
              <label class="label">
                Order No.
                <span class="required">*</span>
              </label>
              <el-input class="input input-sn" v-model="formData.order_no"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Source From
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.source_from"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Invoice No.
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.invoice_no"></el-input>
            </div>
            <div class="item">
              <label class="label">Transport Method</label>
              <el-select v-model="formData.transport_method" class="input">
                <el-option
                  v-for="(item,index) in transportMethodList"
                  :key="index"
                  :label="item"
                  :value="item"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Trade term
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.trade_term" filterable slot="prepend">
                <el-option
                  v-for="(item,index) in tradeTermList"
                  :key="index"
                  :label="item.label"
                  :value="item.value"
                ></el-option>
              </el-select>
            </div>
            <div class="item">
              <label class="label">
                Tracking No.
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model="formData.tracking_no"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Freight
                <span class="required">*</span>
              </label>
              <el-input class="input" v-model.number="formData.freight" min="0" type="number"></el-input>
            </div>
            <div class="item">
              <label class="label">
                Date
                <span class="required">*</span>
              </label>
              <el-date-picker class="input" v-model="formData.date" type="date" placeholder="Date"></el-date-picker>
            </div>
          </div>
        </div>
        <div class="table-wrap">
          <table class="table-content" border="0" cellspacing="0" cellpadding="0">
            <thead>
              <td v-for="(item,index) in headData" :key="index">{{item}}</td>
            </thead>
            <tbody>
              <tr v-for="(item,index) in tableList" :key="item.id">
                <td>{{ item.category_name || '-' }}</td>
                <td>{{ item.product_name }}</td>
                <td>{{ item.product_code }}</td>
                <td>{{ item.warehouse_name }}</td>
                <td>{{ item.warehouse_area_name }}</td>
                <td>{{ item.location_code }}</td>
                <td>{{ item.qty }}</td>
                <td>{{ item.type ?'Pay':'Free' }}</td>
                <td>{{ item.duty }}</td>
                <td>{{ item.create_time | timeFormat("Y-m-d") }}</td>
                <td>{{ item.create_user_id }}</td>
                <td>
                  <button
                    class="btn-underline"
                    :disabled="editControl && formData.status==1"
                    @click.stop="onEdit(item)"
                  >Edit</button>
                  <button
                    class="btn-underline"
                    :disabled="editControl && formData.status==1"
                    @click.stop="onDelete(item,index)"
                  >Delete</button>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="parts-list">
          <el-button
            type="primary"
            class="att-btn"
            :disabled="editControl && formData.status==1"
            @click="onAdd"
          >+</el-button>
        </div>
      </div>
    </div>
    <!-- 新增弹窗 -->
    <el-dialog :title.sync="dialogTitle" :visible.sync="dialogAddVisible" class="dialog-region">
      <div class="item">
        <div class="item-box">
          <label class="label">
            Warehouse
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.warehouse_id"
            @change="selectWarehouse"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in warehouseList"
              :key="item.id"
              :label="item.warehouse_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Area
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.warehouse_area_id"
            filterable
            slot="prepend"
            @change="selectArea"
          >
            <el-option
              v-for="item in getArea"
              :key="item.id"
              :label="item.area_name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Location Code
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.location_id"
            @change="selectLocation"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in getLocation"
              :key="item.id"
              :label="item.location_code"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Category
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.category_id"
            @change="selectCategory"
            filterable
            slot="prepend"
          >
            <el-option
              v-for="item in categoryList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Material Code
            <span class="required">*</span>
          </label>
          <el-select
            class="input"
            v-model="materialData.product_code"
            filterable
            @change="selectHandle"
            slot="prepend"
          >
            <el-option
              v-for="item in getProducts"
              :key="item.id"
              :label="'【'+item.product_code+'】'+ '- ' + item.name"
              :value="item.product_code"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Material Name
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.product_name" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Specification
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.specification" class="input" :disabled="true"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Quantity
            <span class="required">*</span>
          </label>
          <el-input
            v-model="materialData.qty"
            @change="setDuty"
            class="input"
            type="number"
            min="0"
          ></el-input>
        </div>
      </div>
      <!-- <div class="item">
        <div class="item-box">
          <label class="label">
            Transport
            <span class="required">*</span>
          </label>
          <el-select v-model="materialData.transport" class="input">
            <el-option
              v-for="(item,index) in transportList"
              :key="index"
              :label="item"
              :value="item"
            ></el-option>
          </el-select>
        </div>
      </div>-->
      <!-- <div class="item">
        <div class="item-box">
          <label class="label">
            Transport No.
            <span class="required">*</span>
          </label>
          <el-input v-model="materialData.transport_no" class="input" type="text"></el-input>
        </div>
      </div>-->
      <div class="item">
        <div class="item-box">
          <label class="label">Currency</label>
          <el-select class="input" v-model="materialData.currency" slot="prepend">
            <el-option
              v-for="item in currencies"
              :key="item.currency"
              :label="item.label"
              :value="item.currency"
              :value-key="item.currency"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">Freight</label>
          <el-input
            disabled
            v-model.number="materialData.freight"
            min="0"
            type="number"
            class="input"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Total Duty
            <span class="required">*</span>
          </label>
          <el-input
            v-model.number="materialData.total_duty"
            @change="setDuty"
            min="0"
            class="input"
            type="number"
          ></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Duty
            <span class="required">*</span>
          </label>
          <el-input v-model.number="materialData.duty" min="0" class="input" type="number" disabled></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Payment Type
            <span class="required">*</span>
          </label>
          <el-select v-model="materialData.type" class="input">
            <el-option
              v-for="item in payTypeArr"
              :key="item.id"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </div>
      </div>
      <div class="item" v-show="getPayType">
        <div class="item-box">
          <label class="label">
            Payment amount
            <span class="required">*</span>
          </label>
          <el-input class="input" v-model.number="materialData.price" min="0" type="number"></el-input>
        </div>
      </div>
      <div class="item">
        <div class="item-box">
          <label class="label">
            Remark
            <span class="required"></span>
          </label>
          <el-input
            type="textarea"
            :rows="2"
            resize="none"
            class="input"
            v-model="materialData.description"
            placeholder=""
          ></el-input>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button type="preview" @click="dialogConfirm">confirm</el-button>
        <el-button type="primary" @click="dialogCancel">cancel</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Core from "core/core";
import CommonService from "src/services/nl/commonService";
import WarehouseListService from "src/services/nl/warehouseListService.js";
import WarehouseAreaService from "src/services/nl/warehouseAreaService";
import WarehouseLocationService from "src/services/nl/warehouseLocationService.js";
import ProductService from "src/services/nl/productService";
import StockInHeaderService from "src/services/nl/stockInHeaderService.js";
import StockInService from "src/services/nl/stockInService";

export default {
  props: ["id"],
  computed: {
    getArea() {
      const scope = this;
      if (this.materialData && this.materialData.warehouse_id) {
        return this.areaList.filter(res => {
          return res.warehouse_id === scope.materialData.warehouse_id;
        });
      }
    },
    getLocation() {
      const scope = this;
      if (this.materialData && this.materialData.warehouse_area_id) {
        return this.locationList.filter(res => {
          return res.area_id === scope.materialData.warehouse_area_id;
        });
      }
    },
    getProducts() {
      const scope = this;
      if (this.materialData && this.materialData.category_id) {
        return this.productList.filter(res => {
          return res.category_id === scope.materialData.category_id;
        });
      }
    },
    getPayType() {
      if (this.materialData && this.materialData.type) {
        return this.materialData.type === 1 ? true : false;
      }
    }
  },
  data() {
    return {
      editControl: this.id == 0 ? false : true,
      org: Core.Data.getOrg(),
      headData: [
        "Category",
        "Material Name",
        "Material Code",
        "Warehouse",
        "Warehouse Area",
        "Loction Code",
        "Quantity",
        "Payment Type",
        "Duty",
        "Date",
        "User",
        "Operation"
      ],
      // 头部出库单主要信息
      formData: {
        order_no: "",
        source_from: "",
        date: "",
        tracking_no: "",
        invoice_no: "",
        freight: 0,
        trade_term: ""
      },
      // 用途列表
      PurposeList: Core.StatusData.PURPOSE,
      // 客户信息列表
      customerList: [],
      // 列表数据
      tableList: [],
      // 产品分类列表
      categoryList: [],
      // 添加产品数据
      materialData: {},
      // 新增弹框
      dialogTitle: "",
      dialogAddVisible: false,
      // 仓库选择
      warehouseList: [],
      // 库区选择
      areaList: [],
      // 库位选择
      locationList: [],
      // 产品列表
      productList: [],
      currencies: Core.StatusData.CURRENCIES,
      tradeTermList: Core.StatusData.TRADE_TERM,
      showPayType: false,
      payTypeArr: Core.StatusData.STOCKS_PAY_TYPE,
      transportMethodList: Core.StatusData.TRANSPORT_METHOD,
      isEdit: false,
      copyData: {}
    };
  },
  created() {
    this.loading = true;
    CommonService.getAllCustomer().then(res => {
      this.customerList = res.data;
      this.loading = false;
    });

    this.loading = true;
    CommonService.getAllCategory().then(res => {
      this.categoryList = res.category_list.map((category, index) => {
        return {
          id: category.id,
          name: category.name,
          isActive: index == 0 ? true : false
        };
      });
      this.loading = false;
    });
    this.loading = true;
    WarehouseListService.findAll().then(res => {
      this.warehouseList = res.data;
      this.loading = false;
    });

    this.loading = true;
    WarehouseAreaService.findAll().then(res => {
      this.areaList = res.data;
      this.loading = false;
    });

    this.loading = true;
    WarehouseLocationService.findAll().then(res => {
      this.locationList = res.data;
      this.loading = false;
    });

    this.loading = true;
    ProductService.findAll().then(res => {
      this.productList = res.data;
      this.loading = false;
    });

    // 判断是否为新增
    if (this.id == 0) return;

    this.loading = true;
    StockInHeaderService.findOne(this.id).then(res => {
      this.formData = res.data;
      this.formData.date = Core.Util.changeDateForm(res.data.date * 1000);
      this.loading = false;
    });

    this.loading = true;
    StockInService.filterAll(this.id).then(res => {
      this.tableList = res.data;
      this.loading = false;
    });
  },
  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "stockIn" });
    },
    selectHandle(e) {
      const obj = this.productList.find(item => item.product_code === e);
      if (obj) {
        this.materialData.product_name = obj.name;
        this.materialData.specification = obj.specification;
      }
    },
    selectCategory(e) {
      const obj = this.categoryList.find(item => item.id === e);
      if (obj) this.materialData.category_name = obj.name;
    },
    selectLocation(e) {
      const obj = this.locationList.find(item => item.id === e);
      if (obj) this.materialData.location_code = obj.location_code;
    },
    selectArea(e) {
      const obj = this.areaList.find(item => item.id === e);
      if (obj) this.materialData.warehouse_area_name = obj.area_name;
    },
    selectWarehouse(e) {
      const obj = this.warehouseList.find(item => item.id === e);
      if (obj) this.materialData.warehouse_name = obj.warehouse_name;
    },
    setDuty() {
      if (this.materialData && this.materialData.qty > 0) {
        this.materialData.duty =
          this.materialData.total_duty / this.materialData.qty;
      } else {
        this.materialData.duty = 0;
      }
    },
    onSave() {
      const scope = this;
      this.$confirm("Cannot be modified after saving, continue?", "prompt", {
        confirmButtonText: "Submit",
        cancelButtonText: "Cancel",
        type: "warning"
      })
        .then(() => {
          if (this.headerRequired()) {
            this.loading = true;
            this.formData.date = Core.Util.parseDate(this.formData.date);
            StockInService.saveHeader({
              nlInStockHeader: this.formData,
              nlInStocks: this.tableList
            })
              .then(res => {
                if (res.status !== 200) {
                  throw res.data.message;
                }
                scope.loading = false;
                scope.$message({
                  message: "Update Successful !",
                  type: "success"
                });
                scope.$router.push({ name: "stockIn" });
              })
              .catch(err => {
                scope.$message({
                  message: err,
                  type: "error"
                });
                scope.loading = false;
              });
          }
        })
        .catch(() => {
          this.$message({
            type: "info",
            message: "Cancelled"
          });
        });
    },
    // 添加产品列表
    onAdd() {
      this.isEdit = false;
      this.materialData = {
        id: 0,
        warehouse_id: "",
        warehouse_code: "",
        product_name: "",
        specification: "",
        product_code: "",
        qty: "",
        duty: 0,
        total_duty: 0,
        type: "",
        price: 0,
        currency: 2,
        description: "",
        freight: 0
      };

      const data = this.warehouseList.filter(item => {
        return item.country === this.org.country;
      });
      if (data.length > 0) {
        this.materialData.warehouse_id = data[0].id;
      }

      this.dialogTitle = "Add Material";
      this.dialogAddVisible = true;
    },
    onEdit(item) {
      this.dialogTitle = "Update Material";
      this.isEdit = true;
      this.copyData = JSON.parse(JSON.stringify(item));
      this.materialData = item;
      this.dialogAddVisible = true;
    },
    onDelete(item, index) {
      this.tableList.splice(index, 1);
    },
    dialogConfirm() {
      const scope = this;
      if (this.required()) {
        if (!this.isEdit) {
          this.tableList.push(this.materialData);
        }
        this.dialogAddVisible = false;
      }
    },
    dialogCancel() {
      Object.assign(this.materialData, this.copyData);
      this.dialogAddVisible = false;
    },
    required() {
      if (this.materialData.warehouse_id === "") {
        this.$message({
          message: "Please input Warehouse!",
          type: "warning"
        });
        return false;
      }

      if (this.materialData.warehouse_area_id === "") {
        this.$message({
          message: "Please input Area !",
          type: "warning"
        });
        return false;
      }

      if (this.materialData.location_id === "") {
        this.$message({
          message: "Please input Location Code !",
          type: "warning"
        });
        return false;
      }

      if (this.materialData.product_code === "") {
        this.$message({
          message: "Please input Material Code !",
          type: "warning"
        });
        return false;
      }
      if (this.materialData.qty === "") {
        this.$message({
          message: "Please input Quantity !",
          type: "warning"
        });
        return false;
      }
      if (this.materialData.duty === "") {
        this.$message({
          message: "Please input Duty!",
          type: "warning"
        });
        return false;
      }
      if (this.materialData.type === "") {
        this.$message({
          message: "Please input Payment Type!",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    headerRequired() {
      if (this.formData.order_no == "") {
        this.$message({
          message: "Please input Order No. !",
          type: "warning"
        });
        return false;
      }

      if (this.formData.source_from === "") {
        this.$message({
          message: "Please input Source From !",
          type: "warning"
        });
        return false;
      }

      if (this.formData.invoice_no === "") {
        this.$message({
          message: "Please input Invoice No. !",
          type: "warning"
        });
        return false;
      }

      if (this.formData.tracking_no === "") {
        this.$message({
          message: "Please input Tracking No. !",
          type: "warning"
        });
        return false;
      }

      if (this.formData.date === "") {
        this.$message({
          message: "Please input date !",
          type: "warning"
        });
        return false;
      }

      return true;
    }
  }
};
</script>

<style lang="scss" scoped>
.content {
  width: calc(100% - 400px) !important;
  min-width: 1250px;
  span.required {
    color: #e26829;
    display: inline-block;
  }

  border: 1px solid #2b2f3b;
  .order-info {
    width: 100%;
    height: 300px;
    .title {
      height: 50px;
      line-height: 50px;
      padding-left: 40px;
    }
    .left {
      float: left;
      width: 100%;
      height: 270px;
      background: #353945;
      box-shadow: 0 -1px #2b2f3b;
      .item {
        line-height: 60px;
        overflow: hidden;
        width: 49%;
        height: 50px;
        display: inline-block;
        .label {
          display: inline-block;
          width: 125px;
          margin-left: 15px;
        }
        .input {
          margin-left: 10px;
          width: 256px;
          border-radius: 0 0 10px 10px;
        }
        .input-left {
          margin-left: 10px;
          width: 200px;
        }
        .input-right {
          width: 200px;
        }
        .input-note {
          margin-left: 10px;
          width: 608px;
        }
      }
    }
  }
  .parts-list {
    text-align: right;
    padding: 0 40px;
    font-size: 12px;
    overflow: hidden;
    height: 50px;
    line-height: 50px;
    width: 100%;
    box-shadow: 0 -1px #2b2f3b;

    .att-btn {
      padding: 6px 9px;
      min-width: 0;
      margin: 11px auto;
      display: block;
    }
    .labelAdd {
      margin-left: 14px;
    }
  }
  .table-wrap {
    width: 100%;
    height: calc(100% - 392px);
    box-shadow: 0 -1px #2b2f3b;
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
      span {
        color: #e26829;
        display: inline-block;
      }
    }
    .input {
      margin-left: 40px;
      width: 260px;
      i {
        color: #ffffff99;
      }
    }
  }
}
</style>



// WEBPACK FOOTER //
// in-detail.vue?3ddfbf82