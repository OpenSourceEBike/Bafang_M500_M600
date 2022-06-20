function injectStyle (ssrContext) {
  require("!!../../../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-4707276a\",\"scoped\":false,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./parcel-information-save.vue")
}
var Component = require("!../../../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../../../node_modules/vue-loader/lib/selector?type=script&index=0!./parcel-information-save.vue"),
  /* template */
  require("!!../../../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-4707276a\",\"hasScoped\":false}!../../../../../node_modules/vue-loader/lib/selector?type=template&index=0!./parcel-information-save.vue"),
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
// ./src/views/nl/after-sales/customer-service/parcel-information-save.vue
// module id = 2146
// module chunks = 30