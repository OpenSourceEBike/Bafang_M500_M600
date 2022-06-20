function injectStyle (ssrContext) {
  require("!!../../../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-587fc1ce\",\"scoped\":false,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./technical-service.vue")
}
var Component = require("!../../../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../../../node_modules/vue-loader/lib/selector?type=script&index=0!./technical-service.vue"),
  /* template */
  require("!!../../../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-587fc1ce\",\"hasScoped\":false}!../../../../../node_modules/vue-loader/lib/selector?type=template&index=0!./technical-service.vue"),
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
// ./src/views/nl/after-sales/technical-service/technical-service.vue
// module id = 2141
// module chunks = 40