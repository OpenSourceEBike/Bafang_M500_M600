<template>
    <div id="app">
        <router-view v-if="isRouterAlive"></router-view>
    </div>
</template>

<script>
export default {
  name: "app",
  provide() {
    return {
      reload: this.reload
    };
  },
  data() {
    return {
      isRouterAlive: true
    };
  },
  methods: {
    reload() {
      this.isRouterAlive = false;
      this.$nextTick(function() {
        this.isRouterAlive = true;
      });
    }
  }
};
</script>

<style scoped rel="stylesheet/scss">
	body {
		margin: 0;
		padding: 0;
		font-family: 'Helvetica Neue, Helvetica, PingFang SC, Hiragino Sans GB, Microsoft YaHei, SimSun, sans-serif';
		font-size: 14px;
		-webkit-font-smoothing: antialiased;
	}

	#app {
		height: 100%;
	}
</style>



// WEBPACK FOOTER //
// App.vue?a540486c