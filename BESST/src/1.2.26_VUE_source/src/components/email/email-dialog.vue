function injectStyle (ssrContext) {
  require("!!../../../node_modules/extract-text-webpack-plugin/dist/loader.js?{\"omit\":1,\"remove\":true}!vue-style-loader!css-loader?{\"minimize\":true,\"sourceMap\":true}!../../../node_modules/vue-loader/lib/style-compiler/index?{\"vue\":true,\"id\":\"data-v-44db805a\",\"scoped\":true,\"hasInlineConfig\":false}!sass-loader?{\"sourceMap\":true}!../../../node_modules/vue-loader/lib/selector?type=styles&index=0!./email-dialog.vue")
}
var Component = require("!../../../node_modules/vue-loader/lib/component-normalizer")(
  /* script */
  require("!!babel-loader!../../../node_modules/vue-loader/lib/selector?type=script&index=0!./email-dialog.vue"),
  /* template */
  require("!!../../../node_modules/vue-loader/lib/template-compiler/index?{\"id\":\"data-v-44db805a\",\"hasScoped\":true}!../../../node_modules/vue-loader/lib/selector?type=template&index=0!./email-dialog.vue"),
  /* styles */
  injectStyle,
  /* scopeId */
  "data-v-44db805a",
  /* moduleIdentifier (server only) */
  null
)

module.exports = Component.exports



//////////////////
// WEBPACK FOOTER
// ./src/components/email/email-dialog.vue
// module id = 2168
// module chunks = 9 10 11 12 13 14 15