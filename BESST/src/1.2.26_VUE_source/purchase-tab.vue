<template>
  <div class="purchase-tab">
    <div class="table-wrap">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length === 0 ">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody v-if="tableList.length > 0">
            <tr v-for="item in tableList" :key="item.id">
              <td>{{item.date | dateFormat }}</td>
              <td>{{ item.purchase_order_no }}</td>
              <td>{{ conversionSupplier(item.supplier_id) }}</td>
              <td>{{ conversionPurpose(item.purchase_type) }}</td>
              <td>{{ item.total_quantity }}</td>
              <td>
                <button class="btn-underline" @click.stop="onView(item)">Detail</button>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script>
import Core from "core/core";
import PurchaseService from "src/services/purchaseService";

export default {
  name: "deliver-tab",
  props: ["tableList"],
  data() {
    return {
      headData: [
        "Create Time",
        "Order No.",
        "Object",
        "Purpose",
        "Amount",
        "Operation"
      ],
      supplierData: [],
      purpose: [
        {
          id: 0,
          name: "Warehouse stocking"
        },
        {
          id: 1,
          name: "Sales purchase"
        },
        {
          id: 2,
          name: "After-sales procurement"
        }
      ]
    };
  },
  async created() {
    const scope = this;
    scope.lodaing = true;
    await PurchaseService.getAllSupplier()
      .then(res => {
        if (res.data.code === -2) {
          throw res.data.message;
        }
        scope.supplierData = res.data;
        scope.lodaing = false;
      })
      .catch(err => {
        scope.$message({
          message: err.message,
          type: "error"
        });
        scope.loading = false;
      });
  },
  mounted() {},
  methods: {
    // 转换供应商
    conversionSupplier(id) {
      if (this.supplierData.length > 0) {
        const data = this.supplierData.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].company_name;
        } else {
          return "--";
        }
      }
    },
    conversionPurpose(id) {
      if (this.purpose.length > 0) {
        const data = this.purpose.filter(res => {
          return res.id === id;
        });
        if (data.length > 0) {
          return data[0].name;
        }
      }
    },
    onView(item) {
      this.$router.push({
        name: "purchaseListDetails",
        query: { orderId: item.id }
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.purchase-tab {
  .table-wrap .no-data-content {
    width: 100%;
    height: 100%;
    padding: 25px;
    font-size: 14px;
    text-align: center;
    color: #a1a5b1;
    background: none;
  }
}
</style>



// WEBPACK FOOTER //
// purchase-tab.vue?d76f9fcc