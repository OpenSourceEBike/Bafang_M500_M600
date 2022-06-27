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
                            <td>{{ item.ci_no}}</td>
                            <td>{{ item.deliver_order_no }}</td>
                            <td>{{ item.invoice_no }}</td>
                            <td>{{ item.sailing_date| dateFormat }}</td>
                            <td>
                                <button class="btn-underline" @click.stop="onView(item)">Detail</button>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <div class="order-info">
            <div class="upload-box">
                <div class="img-content">
                    <img
                        v-if="!oImage.src"
                        class="upload-icon"
                        src="../../assets/images/icon/icon-upload@2x.png"
                    >
                    <img v-else class="upload-icon" :src="oImage.src ">
                </div>
                <div class="link">
                    <a>Comericial Inovice</a>
                </div>
            </div>
            <div class="upload-box">
                <div class="img-content">
                    <img
                        v-if="!oImage.src"
                        class="upload-icon"
                        src="../../assets/images/icon/icon-upload@2x.png"
                    >
                    <img v-else class="upload-icon" :src="oImage.src ">
                </div>
                <div class="link">
                    <a>Bill of lading</a>
                </div>
            </div>
            <div class="upload-box">
                <div class="img-content">
                    <img
                        v-if="!oImage.src"
                        class="upload-icon"
                        src="../../assets/images/icon/icon-upload@2x.png"
                    >
                    <img v-else class="upload-icon" :src="oImage.src ">
                </div>
                <div class="link">
                    <a>Packing list</a>
                </div>
            </div>
            <div class="upload-box">
                <div class="img-content">
                    <img
                        v-if="!oImage.src"
                        class="upload-icon"
                        src="../../assets/images/icon/icon-upload@2x.png"
                    >
                    <img v-else class="upload-icon" :src="oImage.src ">
                </div>
                <div class="link">
                    <a>Others</a>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import Core from "core/core";

export default {
  name: "clean-tab",
  props: ["tableList"],
  data() {
    return {
      oImage: {
        src: ""
      },
      headData: [
        "CI No.",
        "Delivery Order No.",
        "Invoice No.",
        "Sailing Date",
        "Operator"
      ]
    };
  },
  mounted() {},
  methods: {
    onView(item) {}
  }
};
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
.deliver-tab {
  .order-info {
    padding: 10px;
    display: flex;
    align-items: center;
    border: 1px solid #2b2f3b;
    .upload-box {
      flex: 1;
      display: flex;
      align-items: center;
      flex-direction: column;
      .img-content {
        flex: 1;
        margin-bottom: 10px;
      }
      .link > a {
        color: #fff;
        text-decoration: underline;
      }
    }
  }
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
// clean-tab.vue?3697bcfe