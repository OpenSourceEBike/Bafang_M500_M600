function injectStyle (ssrContext) {
  require("!!../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-35351d96\",\"scoped\":true,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../node_modules/vue-loader/lib/selector?type=styles&index=0!./currency-input.vue")
}
var Component = require("!../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../node_modules/vue-loader/lib/selector?type=script&index=0!./currency-input.vue"),
  /* template */
  require("!!../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-35351d96\",\"hasScoped\":true}!../../node_modules/vue-loader/lib/selector?type=template&index=0!./currency-input.vue"),
  /* styles */
  injectStyle,
  /* scopeId */
  "data-v-35351d96",
  /* moduleIdentifier (server only) */
  null
)

module.exports = Component.exports



//////////////////
// WEBPACK FOOTER
// ./src/components/currency-input.vue
// module id = 1598
// module chunks = 73