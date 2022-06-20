<template>
  <div class="deliver-tab">
    <div class="table-wrap">
      <div class="table-content-holder">
        <div class="no-data-content" v-if="tableList.length === 0 ">noResult</div>
        <table class="table-content" border="0" cellspacing="0" cellpadding="0" v-else>
          <thead>
            <td v-for="(item,index) in headData" :key="index">{{item}}</td>
          </thead>
          <tbody v-if="tableList.length > 0">
            <tr v-for="item in tableList" :key="item.id">
              <td>{{ item.delivery_date | dateFormat }}</td>
              <td>{{ item.delivery_order_no }}</td>
              <td>{{ item.customer_no }}</td>
              <td>{{ item.reviewer }}</td>
              <td>{{ item.reviewer_date| dateFormat }}</td>
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
  name: "deliver-tab",
  props: ["tableList"],
  data() {
    return {
      headData: [
        "Delivery Date",
        "Delivery Order No.",
        "Customer No.",
        "Reviewer",
        "Reviewer Date",
        "Operator"
      ]
    };
  },
  mounted() {},
  methods: {
    onView(item) {
      this.$router.push({
        name: "deliveryListDetails",
        query: { orderId: event.id }
      });
    }
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.deliver-tab {
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
// deliver-tab.vue?68413a3a