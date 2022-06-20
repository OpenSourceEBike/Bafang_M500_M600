<template>
    <div class="shipment-order-header">
        <div class="header">
            <div class="btn-group-inline">
                <div class="icons">
                    <div class="icon-item back" @click="goAfterIndex" title="back"></div>
                </div>
            </div>
            <label
                class="title"
            >{{ orderId ? 'Edit Shipment Order Customer Info' : 'Add Shipment Order Customer Info' }}</label>
        </div>
        <div class="content-wrap">
            <div class="content" v-loading.body="loading">
                <div class="order-info">
                    <label class="title">Customer Info</label>
                    <div class="left">
                        <div class="item">
                            <label class="label">Shipment Order</label>
                            <el-input class="input" v-model="formData.shipment_order_no" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                Order No.
                                <span class="required">*</span>
                            </label>
                            <el-input
                                class="input input-sn"
                                v-model="formData.order_no"
                                @change="getOrderNo"
                                :disabled="orderNo"
                            ></el-input>
                        </div>
                        <div class="item">
                            <label class="label">
                                Customer
                                <span class="required">*</span>
                            </label>
                            <el-select
                                class="input"
                                v-model="formData.customer_id"
                                @change="handleItemChange"
                                filterable
                                slot="prepend"
                            >
                                <el-option
                                    v-for="item in customerList"
                                    :key="item.id"
                                    :label="item.name"
                                    :value="item.id"
                                ></el-option>
                            </el-select>
                        </div>
                        <!-- <div class="item">
              <label class="label">
                Shipment Type
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.shipment_type" filterable slot="prepend">
                <el-option
                  v-for="item in shipmentType"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
                        </div>-->
                        <!-- <div class="item">
              <label class="label">
                Origin Type
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.from_type_code" filterable slot="prepend">
                <el-option
                  v-for="item in sourceType"
                  :key="item.id"
                  :label="item.vlaue"
                  :value="item.value"
                ></el-option>
              </el-select>
                        </div>-->
                        <!-- <div class="item">
              <label class="label">
                Warehouse
                <span class="required">*</span>
              </label>
              <el-select class="input" v-model="formData.warehouse_id" filterable slot="prepend">
                <el-option
                  v-for="item in warehouseList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
                        </div>-->
                        <!-- <div class="item">
              <label class="label">Department</label>
              <el-select class="input" v-model="formData.department_id" filterable slot="prepend">
                <el-option
                  v-for="item in departmentList"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                ></el-option>
              </el-select>
                        </div>-->
                        <!-- 暂时用不到：warehouse、department、shipment type、origin type、remark -->
                        <div class="item">
                            <label class="label">Expected Date</label>
                            <el-date-picker
                                class="input"
                                v-model="expectedDate"
                                type="date"
                                placeholder="Expected Date"
                            ></el-date-picker>
                        </div>
                        <div class="item">
                            <label class="label">Order Memo</label>
                            <el-input class="input" v-model="formData.orderMemo" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Receiver Name</label>
                            <el-input class="input" v-model="formData.receiver_name" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Province</label>
                            <el-input class="input" v-model="formData.province" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">City</label>
                            <el-input class="input" v-model="formData.city" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">District</label>
                            <el-input class="input" v-model="formData.district" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Zip Code</label>
                            <el-input class="input" v-model="formData.zipCode" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Address</label>
                            <el-input class="input" v-model="formData.address" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Mobile</label>
                            <el-input class="input" v-model="formData.mobile" disabled>
                                <el-select
                                    v-model="formData.country_code"
                                    filterable
                                    slot="prepend"
                                >
                                    <el-option
                                        v-for="item in areaCodeList"
                                        :key="item.area_code"
                                        :label="item.area_code"
                                        :value="item.area_code"
                                    ></el-option>
                                </el-select>
                            </el-input>
                        </div>
                        <div class="item">
                            <label class="label">Telephone</label>
                            <el-input class="input" v-model="formData.telephone" disabled></el-input>
                        </div>
                        <div class="item">
                            <label class="label">Email</label>
                            <el-input class="input" v-model="formData.email" disabled></el-input>
                        </div>
                        <!-- <div class="item">
              <label class="label">Remark</label>
              <el-input class="input" v-model="formData.remark" disabled></el-input>
                        </div>-->
                        <div class="item">
                            <label class="label">Description</label>
                            <el-input class="input" v-model="formData.description"></el-input>
                        </div>
                    </div>
                </div>
                <div class="table-wrap" v-loading.body="loading">
                    <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                        <thead>
                            <td v-if="isActive">
                                <el-checkbox v-model="allSelectedForDelete"></el-checkbox>
                            </td>
                            <td v-for="item in tableHead">{{item}}</td>
                        </thead>
                        <tbody>
                            <tr v-for="(item, index) in orderItems">
                                <td v-if="isActive">
                                    <el-checkbox class="table-checkbox" v-model="item.selected"></el-checkbox>
                                </td>
                                <td>{{item.category_root_id | categoryRoot}}</td>
                                <td>{{item.item_part_no}}</td>
                                <td>{{item.item_name}}</td>
                                <td v-if="isActive">
                                    <el-input-number
                                        class="input-number"
                                        v-model="item.quantity"
                                        :min="1"
                                    ></el-input-number>
                                </td>
                                <td v-else>{{item.quantity}}</td>
                                <td>
                                    <button
                                        class="btn-underline"
                                        @click.stop="deleteItem(item,index)"
                                        :disabled="deleteStatus"
                                    >{{ $t('delete') }}</button>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div class="parts-list" v-if="isActive">
                    <el-button type="primary" class="att-btn" @click="dialogCreateVisible = true">+</el-button>
                </div>
                <div class="parts-list">
                    <el-button type="primary" @click="createOrder" v-if="isActive">SAVE</el-button>
                    <el-button
                        type="primary"
                        @click="createAllocate"
                        v-if="orderItems.length>0 && !isActive"
                    >Allocate</el-button>
                </div>
                <el-dialog
                    class="dialog"
                    :title="$t('addItem')"
                    :visible.sync="dialogCreateVisible"
                    size="large"
                    top="10%"
                >
                    <div class="content">
                        <div class="header">
                            <el-input
                                :placeholder="$t('category')"
                                class="search-content"
                                :disabled="true"
                                v-model="selectCategoryItem.name"
                                @click.native="selectCategory"
                            ></el-input>
                            <el-input
                                :placeholder="$t('modelNamePN')"
                                class="search-content"
                                v-model="queryItemCondition"
                            ></el-input>
                            <div class="btn-group-inline">
                                <div class="icons">
                                    <div
                                        class="icon-item search"
                                        @click="queryOrderItem"
                                        :title="$t('search')"
                                    ></div>
                                    <div
                                        class="icon-item clear"
                                        @click="clearCondition"
                                        :title="$t('clear')"
                                    ></div>
                                </div>
                            </div>
                        </div>
                        <div class="table-wrap">
                            <table class="table-content" border="0" cellspacing="0" cellpadding="0">
                                <thead>
                                    <td>
                                        <el-checkbox v-model="allSelectedForAdd"></el-checkbox>
                                    </td>
                                    <td v-for="item in itemHeadData">{{item}}</td>
                                </thead>
                                <tbody>
                                    <tr v-for="(item, index) in selectItems">
                                        <td>
                                            <el-checkbox
                                                class="table-checkbox"
                                                v-model="item.selected"
                                            ></el-checkbox>
                                        </td>
                                        <td>
                                            <img :src="item.img | getImgUrl">
                                        </td>
                                        <td>{{item.name}}</td>
                                        <td>{{item.ext_part_no}}</td>
                                        <td>{{index+1}}.bin</td>
                                    </tr>
                                </tbody>
                            </table>
                            <el-pagination
                                class="pagination"
                                @current-change="handleCurrentPageChange"
                                :current-page.sync="currentPage"
                                :page-size="10"
                                layout="prev, pager, next"
                                :total="total"
                            ></el-pagination>
                        </div>
                        <div class="footer">
                            <el-button type="primary" @click="addItems">{{ $t('addItems') }}</el-button>
                        </div>
                    </div>
                </el-dialog>
            </div>
        </div>
        <el-dialog
            class="dialog"
            :title="$t('selectCategory')"
            :visible.sync="dialogSelectCategoryVisible"
            size="large"
        >
            <div class="content" style="max-height: 500px; overflow: auto">
                <div class="category-tree">
                    <el-tree
                        :data="categoryList"
                        :props="defaultProps"
                        @node-click="handleNodeClick"
                    ></el-tree>
                </div>
            </div>
        </el-dialog>
    </div>
</template>

<script>
import Core from "core/core";
import areaCodeInDB from "core/areaCode";

export default {
  data() {
    return {
      loading: false,
      expectedDate: new Date(),
      formData: {
        operation_user_id: "",
        shipment_type: "",
        from_type_code: "",
        order_no: "",
        warehouse_id: "",
        storer_id: "",
        department_id: "",
        customer_id: "",
        expected_date: "",
        orderMemo: "",
        receiver_name: "",
        country_code: "+86",
        province: "",
        city: "",
        district: "",
        zipCode: "",
        address: "",
        mobile: "",
        telephone: "",
        email: "",
        remark: "",
        description: ""
      },
      warehouseList: [],
      departmentList: [],
      areaCodeList: [],
      shipmentType: [
        {
          id: 0,
          name: "Sales out of stock"
        },
        {
          id: 1,
          name: "After sale"
        }
      ],
      orderId: 0,
      customerList: [],
      isActive: false,
      sourceType: [
        {
          id: 0,
          value: "Purchase"
        },
        {
          id: 1,
          value: "Sale"
        },
        {
          id: 2,
          value: "Show"
        },
        {
          id: 3,
          value: "other"
        }
      ],
      tableHead: [
        "Item Name",
        "BF Art Nr",
        "Specification",
        "Quantity",
        "Operation"
      ],
      itemHeadData: ["Image", "Name", "Part No.", "Firmware"],
      queryItemList: [],
      orderItems: [],
      selectItems: [],

      orderId: 0,
      isActive: false,
      queryItemCondition: "",

      allChecked: false,
      currentPage: 1,
      total: 0,
      dialogCreateVisible: false,
      dialogSelectCategoryVisible: false,
      categoryList: [],
      defaultProps: {
        id: "id",
        children: "child",
        label: "name"
      },
      selectCategoryItem: {
        id: "",
        name: ""
      },
      deleteStatus: false,
      timer: "",
      orderNo: false
    };
  },
  i18n: {
    messages: {}
  },
  computed: {
    allSelectedForAdd: {
      get: function() {
        return this.selectItems.reduce(function(prev, curr) {
          return prev && curr.selected;
        }, true);
      },
      set: function(newValue) {
        this.selectItems.forEach(item => {
          item.selected = newValue;
        });
      }
    },
    allSelectedForDelete: {
      get: function() {
        return this.orderItems.reduce(function(prev, curr) {
          return prev && curr.selected;
        }, true);
      },
      set: function(newValue) {
        this.orderItems.forEach(item => {
          item.selected = newValue;
        });
      }
    }
  },
  mounted: function() {
    const scope = this;
    scope.areaCodeList = areaCodeInDB.init();

    this.orderId = this.$route.query.orderId;
    if (this.orderId) {
      this.loading = true;
      this.orderNo = true;
      this.getShipment(this.orderId)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.expectedDate = Core.Util.changeDateForm(
            res.expected_date * 1000
          );
          scope.loading = false;
          return scope.filterShipmentDetail({
            shipment_id: scope.orderId
          });
        })
        .then(res => {
          scope.loading = false;
          scope.orderItems = res.content.map(orderItem => {
            orderItem.selected = false;
            orderItem.item_part_no = orderItem.item_code;
            return orderItem;
          });
        })
        .then(() => {
          return scope.getShipmentStatus(this.orderId);
        })
        .then(res => {
          res.status_name === "Created"
            ? ((scope.isActive = true), (scope.deleteStatus = false))
            : ((scope.isActive = false), (scope.deleteStatus = true));
          scope.loading = false;
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    } else {
      scope.isActive = true;
    }

    this.loading = true;
    this.getAllCustomer()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        this.customerList = res;
        this.loading = false;
      })
      .catch(err => {
        this.$message({
          message: err,
          type: "error"
        });
        this.dialogVisible = true;
        this.loading = false;
      });

    scope.loading = true;
    this.getAllDepartment()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.loading = false;
        scope.departmentList = res;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.loading = true;
    this.getAllWarehouse()
      .then(res => {
        if (res.code === -2) {
          throw res.message;
        }
        scope.loading = false;
        scope.warehouseList = res;
      })
      .catch(err => {
        scope.$message({
          message: err,
          type: "error"
        });
        scope.loading = false;
      });

    scope.getAllCategory();
  },

  methods: {
    // 返回路由
    goAfterIndex() {
      this.$router.push({ name: "shipmentOrderList" });
    },
    //Order NO联动时候，根据返回的status进行提示
    statusErrorTip(status) {
      let tips = "";
      if (status == "" || status == undefined) return;
      switch (status) {
        case 0:
          tips = "Please approve";
          break;
        case 2:
          tips = "Invoice was created";
          break;
        case 3:
          tips = "This order was finished";
          break;
        case 4:
          tips = "This order was canceled";
          break;
        case 5:
          tips = "Out of stock";
          break;
        default:
          break;
      }
      this.$message({
        message: tips,
        type: "warning"
      });
    },
    getOrderNo() {
      if (this.formData.order_no) {
        if (this.timer) {
          clearTimeout(this.timer);
        }
        this.timer = setTimeout(() => {
          const scope = this;
          scope.loading = true;
          scope.orderItems = [];
          scope
            .filterDelivery(scope.formData.order_no)
            .then(res => {
              scope.loading = false;
              if (res.content[0] == undefined) {
                scope.$message({
                  message: 'Cannot find Order Info.',
                  type: "error"
                });
                return;
              }
              if (res.content[0].status !== 1) {
                this.statusErrorTip(res.content[0].status);
                scope.formData.order_no = "";
              } else {
                this.formData.customer_id = res.content[0].company_id;
                return scope.filterDeliveryPIDetails(res.content[0].id);
              }
            })
            .then(res => {
              scope.loading = false;
              if (res) {
                scope.orderItems = res.map(orderItem => {
                  orderItem.selected = false;
                  orderItem.item_part_no = orderItem.bf_art_no;
                  orderItem.item_name = orderItem.model_name;
                  delete orderItem.create_user_id;
                  delete orderItem.create_time;
                  delete orderItem.id;
                  delete orderItem.update_user_id;
                  delete orderItem.update_time;
                  return orderItem;
                });
              }
            })
            .then(res => {})
            .catch(err => {
              scope.$message({
                message: err,
                type: "error"
              });
              scope.loading = false;
            });
        }, 1000);
      }
    },
    createOrder() {
      const scope = this;
      if (!this.checkParams()) {
        return;
      }
      this.loading = true;
      this.formData.expected_date = Core.Util.parseDate(this.expectedDate) || 0;
      this.formData.operation_user_id = Core.Data.getUser()
        ? Core.Data.getUser().id
        : "";
      this.postShipment(this.formData)
        .then(res => {
          if (res.code === -2) {
            throw res.message;
          }
          scope.formData = JSON.parse(JSON.stringify(res));
          scope.expectedDate = Core.Util.changeDateForm(
            res.expected_date * 1000
          );
          scope.$router.push({
            name: "shipmentOrderHeader",
            query: { orderId: res.id }
          });
          scope.orderId = res.id;
        })
        .then(() => {
          const data = scope.orderItems.map(orderItem => {
            orderItem.item_code = orderItem.item_part_no;
            orderItem.shipment_id = this.orderId;
            return orderItem;
          });
          return this.postShipmentDetail(data);
        })
        .then(res => {
          scope.loading = false;
          scope.$message({
            message: "Save Successfully",
            type: "success"
          });
          this.$router.go(-1);
        })
        .catch(err => {
          scope.$message({
            message: err,
            type: "error"
          });
          scope.loading = false;
        });
    },
    // 弹窗必填
    checkParams() {
      if (this.formData.order_no === "") {
        this.$message({
          message: "Please add Order No.",
          type: "warning"
        });
        return false;
      }

      // if (this.formData.shipment_type === "") {
      //   this.$message({
      //     message: "Please add Shipment Type",
      //     type: "warning"
      //   });
      //   return false;
      // }

      // if (this.formData.warehouse_id === "") {
      //   this.$message({
      //     message: "Please add Warehouse",
      //     type: "warning"
      //   });
      //   return false;
      // }

      // if (this.formData.department_id === "") {
      //   this.$message({
      //     message: "Please add Department",
      //     type: "warning"
      //   });
      //   return false;
      // }

      if (this.formData.customer_id === "") {
        this.$message({
          message: "Please add Customer",
          type: "warning"
        });
        return false;
      }

      if (!this.orderItems.length) {
        this.$message({
          message: "Please add item",
          type: "warning"
        });
        return false;
      }
      return true;
    },
    //顾客信息联动
    handleItemChange(id) {
      if (!this.isActive) {
        return;
      }
      if (this.customerList.length === 0) return;
      const data = this.customerList.find(res => {
        return res.id === id;
      });
      if (!data) {
        this.initCustomeInfo();
        return;
      }
      this.formData.orderMemo = data.order_memo;
      this.formData.receiver_name = data.receiver_name;
      this.formData.country_code = data.country_code;
      this.formData.province = data.province;
      this.formData.city = data.city;
      this.formData.district = data.district;
      this.formData.zipCode = data.postcode;
      this.formData.address = data.address;
      this.formData.mobile = data.mobile;
      this.formData.telephone = data.telephone;
      this.formData.email = data.email;
      this.formData.remark = data.remark;
    },
    initCustomeInfo() {
      this.formData.orderMemo = "";
      this.formData.receiver_name = "";
      this.formData.country_code = "";
      this.formData.province = "";
      this.formData.city = "";
      this.formData.district = "";
      this.formData.zipCode = "";
      this.formData.address = "";
      this.formData.mobile = "";
      this.formData.telephone = "";
      this.formData.email = "";
      this.formData.remark = "";
    },
    //获取所有顾客信息
    getAllCustomer() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
      });
    },
    //获取所有部门
    getAllDepartment() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/public/department/all"
      });
    },
    //获取所有仓库
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    },
    getShipment(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id
      });
    },
    //保存出库信息
    postShipment(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment",
        data: data
      });
    },
    //获取状态信息
    getShipmentStatus(id) {
      return Core.Api.request({
        method: "get",
        url: "/nl/order/shipment/" + id + "/status"
      });
    },
    //查询出库物件列表
    filterShipmentDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/detail/filter",
        data: data,
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //保存出库所需物件列表
    postShipmentDetail(data) {
      return Core.Api.request({
        method: "post",
        url: "/nl/order/shipment/detail/bulk",
        data: data
      });
    },
    // 查询出库单
    filterDelivery(order_no) {
      return Core.Api.request({
        method: "post",
        url: "/nl/sales/delivery/filter",
        data: {
          delivery_order_no: order_no
        }
      });
    },
    filterDeliveryPIDetails(id) {
      return Core.Api.request({
        method: "GET",
        url: "/nl/sales/delivery/" + id + "/pi/detail",
        params: {
          page: 1,
          size: 999
        }
      });
    },
    //删除出库单列表数据
    deleteShipmentDetail(id) {
      return Core.Api.request({
        method: "delete",
        url: "/nl/order/shipment/detail/" + id
      });
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
    deleteItem(item, index) {
      const scope = this;
      if (item.id) {
        scope.loading = true;
        scope.deleteShipmentDetail(item.id).then(
          res => {
            scope.$message({
              message: "Delete Successfully",
              type: "success"
            });
            scope.loading = false;
            scope.orderItems.splice(index, 1);
          },
          err => {
            scope.loading = false;
          }
        );
      } else {
        scope.orderItems.splice(index, 1);
      }
    },
    addItems() {
      this.selectItems.forEach((item, index) => {
        if (item.selected === true) {
          let isAdded = false;
          this.orderItems.forEach(orderItem => {
            if (orderItem.item_id === item.id) {
              isAdded = true;
            }
          });
          if (!isAdded) {
            let addItem = this.selectItems[index];
            this.orderItems.push({
              item_id: addItem.id,
              category_root_id: addItem.category_root_id,
              category_id: addItem.category_id,
              item_name: addItem.name,
              item_part_no: addItem.ext_part_no,
              quantity: 1,
              selected: false
            });
          }
        }
      });
      this.closeChooseItem();
    },

    closeChooseItem() {
      this.selectItems = [];
      this.queryItemList = [];
      this.queryItemCondition = "";
      this.dialogCreateVisible = false;
    },
    queryOrderItem() {
      let org = Core.Data.getOrg();
      let queryItemCondition = this.queryItemCondition.trim();
      if (org !== null) {
        Core.Api.Item.queryOrderItem(
          25,
          this.selectCategoryItem.id,
          "",
          queryItemCondition,
          this.currentPage
        ).then(res => {
          this.queryItemList = res.query;
          this.total = res.count;
          if (!this.queryItemList.length > 0) {
            this.$message({
              message: "There are no matching data",
              type: "warning"
            });
            return;
          }
          this.selectItems = [];
          this.queryItemList.forEach((item, index) => {
            let selectItem = {
              selected: true,
              key: index,
              name: item.name,
              img: item.img,
              ext_part_no: item.ext_part_no,
              id: item.id,
              category_root_id: item.category_root_id,
              category_id: item.category_id
            };
            this.selectItems.push(selectItem);
          });
        });
      }
    },
    clearCondition() {
      this.selectItems = [];
      this.queryItemList = [];
      this.queryItemCondition = "";
      this.selectCategoryItem = "";
      this.total = 0;
      this.currentPage = 1;
    },

    deleteOrderItem() {
      let deleteIndexs = [];
      this.orderItems.forEach((orderItem, index) => {
        if (orderItem.selected === true) {
          deleteIndexs.unshift({
            item: orderItem,
            index: index
          });
        }
      });
      deleteIndexs.forEach(data => {
        if (data.item.id) {
          this.loading = true;
          this.deleteShipmentDetail(data.item.id).then(
            res => {
              this.$message({
                message: "Delete Successfully",
                type: "success"
              });
              this.loading = false;
            },
            err => {
              this.loading = false;
            }
          );
          this.orderItems.splice(data.index, 1);
        } else {
          this.orderItems.splice(data.index, 1);
        }
      });
    },

    selectCategory() {
      this.dialogSelectCategoryVisible = true;
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
    handleCurrentPageChange(page) {
      this.currentPage = page;
      this.queryOrderItem();
    },
    createAllocate() {
      this.$router.push({
        name: "shipmentAllocate",
        query: { orderId: this.orderId }
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss">
.shipment-order-header {
  width: 100%;
  height: 100%;

  span.required {
    color: #e26829;
    display: inline-block;
  }

  .content {
    border: 1px solid #2b2f3b;
    .order-info {
      width: 100%;
      height: 460px;
      .title {
        height: 50px;
        line-height: 50px;
        padding-left: 40px;
      }
      .left {
        float: left;
        width: 100%;
        height: 460px;
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
            width: 110px;
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
      overflow-x: hidden;
    }
  }

  .dialog {
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

  .footer {
    width: 100%;
    height: 60px;
    line-height: 60px;
    padding-left: 40px;
    box-shadow: 0 -1px #2b2f3b;
    .footer-pagination {
      float: right;
    }
  }
  .pagination {
    padding: 10px;
  }
}
</style>



// WEBPACK FOOTER //
// shipment-order-header.vue?fd489d38