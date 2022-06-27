<!--<template>-->
  <!--<div>-->
    <!--<div v-for="attr in attrList">-->
      <!--<vue-clip :options="attr.options" :on-complete="complete">-->
        <!--<template slot="clip-uploader-action">-->
          <!--<div>-->
            <!--<div class="dz-message"><h2> Click or Drag and Drop files here upload </h2></div>-->
          <!--</div>-->
        <!--</template>-->

        <!--<template slot="clip-uploader-body" scope="props">-->
          <!--<div v-for="file in props.files">-->
            <!--<img v-bind:src="file.dataUrl" />-->
            <!-file.name }}file.status }}-->
          <!--</div>-->
        <!--</template>-->

      <!--</vue-clip>-->

    <!--</div>-->
  <!--</div>-->
<!--</template>-->

<!--<script type="text/ecmascript-6">-->
  <!--import Core from "../../../core/core"-->

  <!--export default {-->
      <!--data() {-->
          <!--return {-->
            <!--options: {-->
              <!--url: 'http://localhost:9898/file/file-upload',-->
              <!--paramName: 'file',-->
              <!--params: {'attr_def_id': 1}-->
            <!--},-->
            <!--attrList: []-->
          <!--}-->
      <!--},-->
      <!--mounted: function () {-->
        <!--console.log('mounted')-->

        <!--Core.Api.Category.getUpperAttrDefList(2)-->
          <!--.then(res => {-->
              <!--this.attrList = res.attr_list;-->
              <!--let attrList = this.attrList;-->
              <!--this.attrList.forEach(function (item, index) {-->
                  <!--attrList[index].options = {-->
                    <!--url: Core.Const.NET.FILE_UPLOAD_END_POINT,-->
                    <!--params: {'attr_def_id': item.id}-->
                  <!--}-->
              <!--});-->
              <!--this.attrList = attrList;-->
              <!--console.log(this.attrList);-->
          <!--});-->
      <!--},-->
      <!--methods: {-->

        <!--complete (file, status, xhr) {-->
          <!--if (status === 'success') {-->
              <!--let res = JSON.parse(xhr.response);-->
              <!--if (res.code == 0 && res.data.file) {-->
                <!--console.log('attr_def_id ' + res.data.file.attr_def_id)-->
                <!--console.log(res.data.file);-->
                <!--comsole.log(res.data);-->
              <!--}-->
          <!--}-->
        <!--}-->
      <!--}-->

  <!--}-->
<!--</script>-->

<!--<style>-->

<!--</style>-->
<template>
    <div>
        <vue-clip :options="attr.options" :on-complete="complete">
            <template slot="clip-uploader-action">
                <div>
                    <div class="dz-message"><h2> Click or Drag and Drop files here upload </h2></div>
                </div>
            </template>

            <template slot="clip-uploader-body">
                <div v-for="file in files">
                    <img v-bind:src="Core.Const.NET.FILE_URL_PREFIX + file" @click="removeFile(file)"/>
                </div>
            </template>
        </vue-clip>
    </div>
</template>

<script type="text/ecmascript-6">
    import Core from "../../../core/core"

    export default {
        data() {
            return {
                Core: Core,
                attr: {
                    id: 23,
                    input_type: 8,
                    key: "display_img",
                    name: "Display",
                    options: {
                        url: "http://test.api.besst.bafang-service.com/file/file-upload",
                        params: {
                            attr_def_id: 23,
                            token: Core.Data.getToken()
                        }
                    },
                    value: ""
                },
                files: ["7302bc02980af5888658e133948dbf261eca4deda37c64e27ce365dc3b249d49.jpg"]
            }
        },
        mounted: function () {
            console.log('mounted');
        },
        methods: {
            removeFile(file) {
                this.files.splice(this.files.indexOf(file), 1);
            },
            complete (file, status, xhr) {
                if (status === 'success') {
                    let res = JSON.parse(xhr.response);
                    if (res.code === 0 && res.data.file) {
                        this.files.push(res.data.file.hash + '.' + res.data.file.ext);
                    }
                }
            }
        }

    }
</script>

<style>

</style>



// WEBPACK FOOTER //
// vue-clip-test.vue?eef8898a