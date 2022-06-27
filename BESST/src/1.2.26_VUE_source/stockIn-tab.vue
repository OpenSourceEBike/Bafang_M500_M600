<template>
    <div class="stockIn-tab">
        <div class="table-wrap">
            <div class="table-content-holder">
                <div class="no-data-content" v-if="tableList.length === 0 ">noResult</div>
                <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
                    <thead>
                        <td v-for="(item,index) in headData" :key="index">{{item}}</td>
                    </thead>
                    <tbody v-if="tableList.length > 0">
                        <tr v-for="item in tableList" :key="item.id">
                            <td>{{ item.receipt_order_no}}</td>
                            <td>{{ item.create_time | dateFormat }}</td>
                            <td>{{ item.order_no }}</td>
                            <td>{{ item.warehouse_name }}</td>
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
  name: "stockIn-tab",
  props: ["tableList"],
  data() {
    return {
      headData: [
        "Receipt No",
        "Time",
        "order no",
        "Warehouse",
        "Operator"
      ]
    };
  },
  mounted() {},
  methods: {
    onView(event) {
      this.$router.push({
        name: "receiptOrderDetails",
        query: { orderId: event.id }
      });
    },
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.stockIn-tab {
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
// stockIn-tab.vue?7b2c12ce