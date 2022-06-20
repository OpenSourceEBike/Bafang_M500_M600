function injectStyle (ssrContext) {
  require("!!../../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-77a95cfa\",\"scoped\":false,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./shipment-allocate.vue")
}
var Component = require("!../../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../../node_modules/vue-loader/lib/selector?type=script&index=0!./shipment-allocate.vue"),
  /* template */
  require("!!../../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-77a95cfa\",\"hasScoped\":false}!../../../../node_modules/vue-loader/lib/selector?type=template&index=0!./shipment-allocate.vue"),
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
// ./src/views/nl/shipment-order/shipment-allocate.vue
// module id = 2115
// module chunks = 56