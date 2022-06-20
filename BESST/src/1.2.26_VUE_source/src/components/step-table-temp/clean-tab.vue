function injectStyle (ssrContext) {
  require("!!../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-45496f7f\",\"scoped\":true,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./clean-tab.vue")
}
var Component = require("!../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../node_modules/vue-loader/lib/selector?type=script&index=0!./clean-tab.vue"),
  /* template */
  require("!!../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-45496f7f\",\"hasScoped\":true}!../../../node_modules/vue-loader/lib/selector?type=template&index=0!./clean-tab.vue"),
  /* styles */
  injectStyle,
  /* scopeId */
  "data-v-45496f7f",
  /* moduleIdentifier (server only) */
  null
)

module.exports = Component.exports



//////////////////
// WEBPACK FOOTER
// ./src/components/step-table-temp/clean-tab.vue
// module id = 2336
// module chunks = 4 5