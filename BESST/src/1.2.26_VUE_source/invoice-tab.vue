<template>
    <div class="invoice-tab">
        <div class="table-wrap">
            <div class="table-content-holder">
                <div class="no-data-content" v-if="tableList.length === 0 ">noResult</div>
                <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
                    <thead>
                        <td v-for="(item,index) in headData" :key="index">{{item}}</td>
                    </thead>
                    <tbody v-if="tableList.length > 0">
                        <tr v-for="item in tableList" :key="item.id">
                            <td>{{ item.invoice_no}}</td>
                            <td>{{ item.create_time | dateFormat }}</td>
                            <td>{{ item.deliver_order_no }}</td>
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
  name: "invoice-tab",
  props: ["tableList"],
  data() {
    return {
      headData: [
        "Invoice No",
        "Time",
        "Delivery order no",
        "Operator",
      ]
    };
  },
  mounted() {},
  methods: {
    onView(event) {
      this.$router.push({
        name: "invoiceListDetails",
        query: { orderId: event.id }
      });
    },
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.invoice-tab {
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
// invoice-tab.vue?01329c8a