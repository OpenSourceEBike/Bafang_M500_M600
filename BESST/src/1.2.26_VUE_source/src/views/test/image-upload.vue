function injectStyle (ssrContext) {
  require("!!../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-77193986\",\"scoped\":false,\"hasInlineConfig\":false}!../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./image-upload.vue")
}
var Component = require("!../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../node_modules/vue-loader/lib/selector?type=script&index=0!./image-upload.vue"),
  /* template */
  require("!!../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-77193986\",\"hasScoped\":false}!../../../node_modules/vue-loader/lib/selector?type=template&index=0!./image-upload.vue"),
  /* styles */
  injectStyle,
  /* scopeId */
  null,
  /* moduleIdentifier (server only) */
  null
)

module.exports = Component.exports



//////////////////
// WEBPACK FOOTER
// ./src/views/test/image-upload.vue
// module id = 1920
// module chunks = 73