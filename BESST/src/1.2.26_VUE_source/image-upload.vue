<template>
  <div class="components">
    <!--<div class="center">-->
      <!--<img class="avatar" :src="src"/>-->
    <!--</div>-->
    <div class="center">
      <vue-core-image-upload
        class="btn btn-primary"
        :crop="false"
        @imageuploaded="imageuploaded"
        :max-file-size="5242880"
        inputOfFile="file"
        :data="data"
        url="http://test.api.besst.bafang-service.com/file/img-upload">
        <img class="avatar" :src="src"/>
      </vue-core-image-upload>
    </div>
  </div>
</template>

<style>

</style>

<script>
  import VueCoreImageUpload from 'vue-core-image-upload'
  export default {
    components: {
      VueCoreImageUpload,
    },
    data() {
      return {
        src: 'http://img1.vued.vanthink.cn/vued0a233185b6027244f9d43e653227439a.png',
          data: {
            token: 'daac705b3e66fec1e6a7f7d38239c1c4'
          }
      }
    },
    methods: {
      imageuploaded(res) {
//        this.src = res.data.src;
//        console.log(this.step, 1111);
          console.log(res);
        this.src = "http://static.besst.innotick.com/besst/img/" + res.data.file.hash + ".jpg";
      }
    }
  }
</script>



// WEBPACK FOOTER //
// image-upload.vue?53e58f31