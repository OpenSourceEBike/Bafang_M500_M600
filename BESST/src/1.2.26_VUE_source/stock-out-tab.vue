<template>
  <div class="stock-out-tab">
    <div class="table-wrap">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length === 0 ">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody v-if="tableList.length > 0">
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.createUser }}</td>
              <td>{{ item.expected_date | dateFormat }}</td>
              <td>{{ item.order_no }}</td>
              <td>{{ pipeWarehouse(item.warehouse_id) }}</td>
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

export default {
  name: "stock-out-tab",
  props: ["tableList"],
  data() {
    return {
      headData: [
        "Operation User",
        "Expected Date",
        "Delivery Order No.",
        "Warehouse",
        "Operator"
      ],
      warehouseList: []
    };
  },
  mounted() {
    const scope = this;
    this.loading = true;
    this.getAllWarehouse()
      .then(res => {
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
  },
  methods: {
    onView(item) {
      this.$router.push({
        name: "shipmentOrderDetails",
        query: { id: item.id }
      });
    },
    pipeWarehouse(id) {
      const data = this.warehouseList.filter(res => {
        return res.id === id;
      });
      if (data.length > 0) {
        return data[0].name;
      } else {
        return "/";
      }
    },
    getAllWarehouse() {
      return Core.Api.request({
        method: "GET",
        url: "/nl/warehouse/all"
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.stock-out-tab {
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
// stock-out-tab.vue?e95d13ca