function injectStyle (ssrContext) {
  require("!!../../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-5bb88556\",\"scoped\":false,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./shipment-order-details.vue")
}
var Component = require("!../../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../../node_modules/vue-loader/lib/selector?type=script&index=0!./shipment-order-details.vue"),
  /* template */
  require("!!../../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-5bb88556\",\"hasScoped\":false}!../../../../node_modules/vue-loader/lib/selector?type=template&index=0!./shipment-order-details.vue"),
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
// ./src/views/nl/shipment-order/shipment-order-details.vue
// module id = 2116
// module chunks = 38