<template>
  <!-- 删除弹窗 -->
  <el-dialog :title.sync="dialogDeleteTitle" :visible.sync="dialogDeleteVisible" size="tiny">
    <p>Are you sure to delete it ?</p>
    <div slot="footer" class="dialog-footer">
      <el-button type="primary" @click="dialogConfirm">yes</el-button>
      <el-button type="primary" @click="dialogCancel">no</el-button>
    </div>
  </el-dialog>
</template>

<script>
export default {
  name: "delete-dialog",
  props: {
    dialogDeleteTitle: String,
    dialogDeleteVisible: Boolean
  },
  methods: {
    dialogConfirm() {
      this.$emit("dialogConfirm");
    },
    dialogCancel(){
        this.$emit("dialogCancel");
    }
  }
};
</script>

<style lang="scss" scoped>
</style>


// WEBPACK FOOTER //
// dialog-delete.vue?71fb07f5