<template>
  <el-dialog
    :title="title"
    :visible.sync="dialogVisible"
    :show-close="false"
    :close-on-click-modal="false"
    :close-on-press-escape="false"
    size="tiny"
    class="dialog"
  >
    <div class="item">
      <label class="title">
        Email
        <span class="required">*</span>
      </label>
      <div class="content">
        <el-input v-model="email"></el-input>
      </div>
    </div>
    <div slot="footer" class="dialog-footer">
      <el-button type="primary" @click="dialogInvitation">Send</el-button>
      <el-button type="primary" @click="dialogCancel">Cancel</el-button>
    </div>
  </el-dialog>
</template>

<script>
import Core from "core/core";
export default {
  props: {
    title: {
      type: String,
      default: "Send Email"
    },
    dialogVisible: {
      type: Boolean,
      required: true
    }
  },
  data() {
    return {
      email: ""
    };
  },
  methods: {
    dialogInvitation() {
      if (!this.required()) {
        return;
      }
      this.$emit("send", this.email);
    },
    dialogCancel() {
      this.$emit("cancel");
    },
    required() {
      if (!Core.Util.isEmail(this.email)) {
        this.$message({
          message: "E-mail is illegitimate",
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
.dialog {
  span.required {
    color: #e26829;
    display: inline-block;
    padding-left: 4px;
  }
  overflow: hidden;
  .el-dialog__body {
    font-size: 0;
  }
  .item {
    line-height: 30px;
    display: inline-block;
    .title {
      display: inline-block;
      width: 80px;
    }
    .content {
      margin: 10px;
      display: inline-block;
      width: 250px;
    }
  }
}
</style>


// WEBPACK FOOTER //
// email-dialog.vue?40440ce7