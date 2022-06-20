<template>
    <div class="app-content">
        <router-view></router-view>
    </div>
</template>

<script type="text/ecmascript-6">
    export default{
        name: 'app-content',
        data(){
            return {}
        },
        methods: {}
    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>

    .app-content {
        position: relative;
        height: 100%;
        width: calc(100% - 208px);
        margin-left: 208px;
    }
</style>



// WEBPACK FOOTER //
// content.vue?21517d34