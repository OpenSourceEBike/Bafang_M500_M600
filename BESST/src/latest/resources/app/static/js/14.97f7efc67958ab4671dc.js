webpackJsonp([14],{2126:function(e,t,i){var n=i(2)(i(2510),i(2511),function(e){i(2508)},null,null);e.exports=n.exports},2168:function(e,t,i){var n=i(2)(i(2171),i(2172),function(e){i(2169)},"data-v-44db805a",null);e.exports=n.exports},2169:function(e,t,i){var n=i(2170);"string"==typeof n&&(n=[[e.i,n,""]]),n.locals&&(e.exports=n.locals);i(2089)("5371be7e",n,!0,{})},2170:function(e,t,i){(e.exports=i(2088)(!0)).push([e.i,".dialog[data-v-44db805a]{overflow:hidden}.dialog span.required[data-v-44db805a]{color:#e26829;display:inline-block;padding-left:4px}.dialog .el-dialog__body[data-v-44db805a]{font-size:0}.dialog .item[data-v-44db805a]{line-height:30px;display:inline-block}.dialog .item .title[data-v-44db805a]{display:inline-block;width:80px}.dialog .item .content[data-v-44db805a]{margin:10px;display:inline-block;width:250px}","",{version:3,sources:["C:/Users/dd/Desktop/besst-build/clientBESST/src/components/email/email-dialog.vue"],names:[],mappings:"AACA,yBACE,eAAiB,CAClB,AACD,uCACI,cAAe,AACf,qBAAsB,AACtB,gBAAkB,CACrB,AACD,0CACI,WAAa,CAChB,AACD,+BACI,iBAAkB,AAClB,oBAAsB,CACzB,AACD,sCACM,qBAAsB,AACtB,UAAY,CACjB,AACD,wCACM,YAAa,AACb,qBAAsB,AACtB,WAAa,CAClB",file:"email-dialog.vue",sourcesContent:["\n.dialog[data-v-44db805a] {\n  overflow: hidden;\n}\n.dialog span.required[data-v-44db805a] {\n    color: #e26829;\n    display: inline-block;\n    padding-left: 4px;\n}\n.dialog .el-dialog__body[data-v-44db805a] {\n    font-size: 0;\n}\n.dialog .item[data-v-44db805a] {\n    line-height: 30px;\n    display: inline-block;\n}\n.dialog .item .title[data-v-44db805a] {\n      display: inline-block;\n      width: 80px;\n}\n.dialog .item .content[data-v-44db805a] {\n      margin: 10px;\n      display: inline-block;\n      width: 250px;\n}\n"],sourceRoot:""}])},2171:function(e,t,i){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var n,a=i(3),s=(n=a)&&n.__esModule?n:{default:n};t.default={props:{title:{type:String,default:"Send Email"},dialogVisible:{type:Boolean,required:!0}},data:function(){return{email:""}},methods:{dialogInvitation:function(){this.required()&&this.$emit("send",this.email)},dialogCancel:function(){this.$emit("cancel")},required:function(){return!!s.default.Util.isEmail(this.email)||(this.$message({message:"E-mail is illegitimate",type:"warning"}),!1)}}}},2172:function(e,t){e.exports={render:function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("el-dialog",{staticClass:"dialog",attrs:{title:e.title,visible:e.dialogVisible,"show-close":!1,"close-on-click-modal":!1,"close-on-press-escape":!1,size:"tiny"},on:{"update:visible":function(t){e.dialogVisible=t}}},[i("div",{staticClass:"item"},[i("label",{staticClass:"title"},[e._v("\n      Email\n      "),i("span",{staticClass:"required"},[e._v("*")])]),e._v(" "),i("div",{staticClass:"content"},[i("el-input",{model:{value:e.email,callback:function(t){e.email=t},expression:"email"}})],1)]),e._v(" "),i("div",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[i("el-button",{attrs:{type:"primary"},on:{click:e.dialogInvitation}},[e._v("Send")]),e._v(" "),i("el-button",{attrs:{type:"primary"},on:{click:e.dialogCancel}},[e._v("Cancel")])],1)])},staticRenderFns:[]}},2508:function(e,t,i){var n=i(2509);"string"==typeof n&&(n=[[e.i,n,""]]),n.locals&&(e.exports=n.locals);i(2089)("1e4c321e",n,!0,{})},2509:function(e,t,i){(e.exports=i(2088)(!0)).push([e.i,".purchase-list{display:-webkit-box;display:-ms-flexbox;display:flex;-webkit-box-orient:vertical;-webkit-box-direction:normal;-ms-flex-direction:column;flex-direction:column;width:100%;height:100%}.purchase-list .list-form-inline{height:100%;line-height:50px}.purchase-list .list-form-inline .el-form-item{margin-right:2px!important;margin-bottom:0}.purchase-list .list-form-inline .el-form-item__label{color:#fff;width:83px;text-align:left;margin-left:5px}.purchase-list .list-form-inline .el-form-item__content{width:180px!important}.purchase-list .table-wrap .long-td{max-width:270px;text-overflow:ellipsis;overflow:hidden;text-align:left}.purchase-list .dialog-warehouse{overflow:hidden}.purchase-list .dialog-warehouse .el-dialog__body{font-size:0}.purchase-list .dialog-warehouse .item{width:50%;line-height:30px;display:inline-block}.purchase-list .dialog-warehouse .item .land{width:250px}.purchase-list .dialog-warehouse .item .title{display:inline-block;width:80px}.purchase-list .dialog-warehouse .item .content{margin:10px;display:inline-block;width:250px}","",{version:3,sources:["C:/Users/dd/Desktop/besst-build/clientBESST/src/views/nl/purchase/purchase-list.vue"],names:[],mappings:"AACA,eACE,oBAAqB,AACrB,oBAAqB,AACrB,aAAc,AACd,4BAA6B,AAC7B,6BAA8B,AAC1B,0BAA2B,AACvB,sBAAuB,AAC/B,WAAY,AACZ,WAAa,CACd,AACD,iCACI,YAAa,AACb,gBAAkB,CACrB,AACD,+CACM,2BAA6B,AAC7B,eAAiB,CACtB,AACD,sDACM,WAAY,AACZ,WAAY,AACZ,gBAAiB,AACjB,eAAiB,CACtB,AACD,wDACM,qBAAwB,CAC7B,AACD,oCACI,gBAAiB,AACjB,uBAAwB,AACxB,gBAAiB,AACjB,eAAiB,CACpB,AACD,iCACI,eAAiB,CACpB,AACD,kDACM,WAAa,CAClB,AACD,uCACM,UAAW,AACX,iBAAkB,AAClB,oBAAsB,CAC3B,AACD,6CACQ,WAAa,CACpB,AACD,8CACQ,qBAAsB,AACtB,UAAY,CACnB,AACD,gDACQ,YAAa,AACb,qBAAsB,AACtB,WAAa,CACpB",file:"purchase-list.vue",sourcesContent:["\n.purchase-list {\n  display: -webkit-box;\n  display: -ms-flexbox;\n  display: flex;\n  -webkit-box-orient: vertical;\n  -webkit-box-direction: normal;\n      -ms-flex-direction: column;\n          flex-direction: column;\n  width: 100%;\n  height: 100%;\n}\n.purchase-list .list-form-inline {\n    height: 100%;\n    line-height: 50px;\n}\n.purchase-list .list-form-inline .el-form-item {\n      margin-right: 2px !important;\n      margin-bottom: 0;\n}\n.purchase-list .list-form-inline .el-form-item__label {\n      color: #fff;\n      width: 83px;\n      text-align: left;\n      margin-left: 5px;\n}\n.purchase-list .list-form-inline .el-form-item__content {\n      width: 180px !important;\n}\n.purchase-list .table-wrap .long-td {\n    max-width: 270px;\n    text-overflow: ellipsis;\n    overflow: hidden;\n    text-align: left;\n}\n.purchase-list .dialog-warehouse {\n    overflow: hidden;\n}\n.purchase-list .dialog-warehouse .el-dialog__body {\n      font-size: 0;\n}\n.purchase-list .dialog-warehouse .item {\n      width: 50%;\n      line-height: 30px;\n      display: inline-block;\n}\n.purchase-list .dialog-warehouse .item .land {\n        width: 250px;\n}\n.purchase-list .dialog-warehouse .item .title {\n        display: inline-block;\n        width: 80px;\n}\n.purchase-list .dialog-warehouse .item .content {\n        margin: 10px;\n        display: inline-block;\n        width: 250px;\n}\n"],sourceRoot:""}])},2510:function(e,t,i){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var n=s(i(3)),a=s(i(2168));function s(e){return e&&e.__esModule?e:{default:e}}t.default={components:{EmailDialog:a.default},data:function(){return{loading:!1,formSearch:{purchase_order_no:""},headData:[],tableList:[],currentPage:1,total:0,dialogDeleteVisible:!1,dialogReviewVisible:!1,currentID:"",emailDialogVisible:!1,currentPurchaseNo:"",supplierData:[],purpose:[{id:0,name:"Warehouse stocking"},{id:1,name:"Sales purchase"},{id:2,name:"After-sales procurement"}],statusList:["Created","Approved","Finished"],status:""}},i18n:{messages:{}},created:function(){},computed:{},mounted:function(){this.headData=["Order No.","Object","Purpose","Amount","Create Time","Status","Operation"],this.getPurchaseList();var e=this;e.lodaing=!0,e.getSupplier().then(function(t){e.lodaing=!1,e.supplierData=t}).catch(function(t){e.$message({message:t.message,type:"error"}),e.loading=!1})},methods:{statusPipe:function(e){return this.statusList[e]?this.statusList[e]:"-"},handleCommand:function(e,t){this.status=e,this.currentID=t.id,this.dialogReviewVisible=!0},reviewInvitation:function(){var e=this,t=this.status;e.dialogReviewVisible=!1,e.loading=!0,e.putPurchaseStatus(this.currentID,t).then(function(t){e.$message({message:"Successful Review",type:"success"}),e.loading=!1,e.getPurchaseList()},function(t){e.loading=!1})},goAfterIndex:function(){this.$router.push({name:"afterIndex"})},onSearch:function(){this.getPurchaseList()},onClean:function(){this.formSearch={purchase_order_no:""},this.getPurchaseList()},onSchedule:function(e){this.$router.push({name:"purchaseSchedule",query:{orderId:e.id}})},onView:function(e){this.$router.push({name:"purchaseListDetails",query:{orderId:e.id}})},onSendEmail:function(e){this.currentID=e.id,this.currentPurchaseNo=e.purchase_order_no,this.emailDialogVisible=!0},sendEmail:function(e){n.default.Api.Common.sendEmailStaff(this.currentPurchaseNo,e),this.emailDialogVisible=!1},sendCancel:function(e){this.emailDialogVisible=!1},onEdit:function(e){this.$router.push({name:"purchaseOrderDetails",query:{orderId:e.id}})},onDelete:function(e){this.currentID=e.id,this.dialogDeleteVisible=!0},onAdd:function(){this.$router.push({name:"purchaseOrderDetails"})},handleCurrentChange:function(e){this.currentPage=e,n.default.Data.set(n.default.Const.DATA.KEY_AFTER_SALES_PAGE,this.currentPage),this.getPurchaseList()},deleteInvitation:function(){var e=this,t=this;this.loading=!0,this.deletePurchaseList(this.currentID).then(function(){e.loading=!1,e.dialogDeleteVisible=!1,t.getPurchaseList()}).catch(function(t){e.$message({message:t,type:"error"}),e.dialogDeleteVisible=!1,e.loading=!1})},getPurchaseList:function(){var e=this;return e.loading=!0,n.default.Api.request({method:"POST",url:"/nl/order/purchase/filter",data:{purchase_order_no:this.formSearch.purchase_order_no},params:{page:this.currentPage,size:10}}).then(function(t){if(e.loading=!1,-2===t.code)throw t.message;e.tableList=t.content,e.total=t.totalElements}).catch(function(t){e.$message({message:t,type:"error"}),e.loading=!1})},deletePurchaseList:function(e){return n.default.Api.request({method:"DELETE",url:"/nl/order/purchase/"+e})},putPurchaseStatus:function(e,t){return n.default.Api.request({method:"put",url:"/nl/order/purchase/"+e+"/status",params:{status:t}})},getSupplier:function(){return n.default.Api.request({method:"GET",url:"/nl/nlPublic/supplier/all"})},conversionSupplier:function(e){if(this.supplierData.length>0){var t=this.supplierData.filter(function(t){return t.id===e});return t.length>0?t[0].company_name:"--"}},conversionPurpose:function(e){if(this.purpose.length>0){var t=this.purpose.filter(function(t){return t.id===e});if(t.length>0)return t[0].name}},changeDate:function(e){return n.default.Util.changeDateForm(e)}},watch:{}}},2511:function(e,t){e.exports={render:function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("div",{staticClass:"purchase-list"},[i("div",{staticClass:"search-header"},[i("el-form",{staticClass:"list-form-inline",attrs:{inline:!0,model:e.formSearch}},[i("el-form-item",{attrs:{label:"Order No."}},[i("el-input",{attrs:{placeholder:"Order No."},model:{value:e.formSearch.purchase_order_no,callback:function(t){e.$set(e.formSearch,"purchase_order_no",t)},expression:"formSearch.purchase_order_no"}})],1),e._v(" "),i("div",{staticClass:"btn-group-inline-icon"},[i("div",{staticClass:"icons"},[i("div",{staticClass:"icon-item search",attrs:{title:"search"},on:{click:e.onSearch}}),e._v(" "),i("div",{staticClass:"icon-item clear",attrs:{title:"clear"},on:{click:e.onClean}})])]),e._v(" "),i("div",{staticClass:"btn-group-top"},[i("div",{staticClass:"icons"},[i("div",{directives:[{name:"ilpermission",rawName:"v-ilpermission",value:"purchase-add",expression:"'purchase-add'"}],staticClass:"icon-item add-v2",attrs:{title:"add"},on:{click:e.onAdd}})])])],1)],1),e._v(" "),i("div",{directives:[{name:"loading",rawName:"v-loading.body",value:e.loading,expression:"loading",modifiers:{body:!0}}],staticClass:"table-wrap"},[i("div",{staticClass:"table-content-holder"},[e.tableList.length<=0&&!e.loading?i("div",{staticClass:"no-data-content"},[e._v("noResult")]):i("table",{staticClass:"table-content",attrs:{border:"0",cellspacing:"0",cellpadding:"0"}},[i("thead",e._l(e.headData,function(t,n){return i("td",{key:n},[e._v(e._s(t))])})),e._v(" "),!e.loading&&e.tableList.length>0?i("tbody",e._l(e.tableList,function(t){return i("tr",{key:t.id},[i("td",[e._v(e._s(t.purchase_order_no))]),e._v(" "),i("td",[e._v(e._s(e.conversionSupplier(t.supplier_id)))]),e._v(" "),i("td",[e._v(e._s(e.conversionPurpose(t.purchase_type)))]),e._v(" "),i("td",[e._v(e._s(t.total_quantity))]),e._v(" "),i("td",[e._v(e._s(e._f("dateFormat")(t.date)))]),e._v(" "),i("td",[e._v(e._s(e.statusPipe(t.status)))]),e._v(" "),i("td",[i("button",{staticClass:"btn-underline",on:{click:function(i){i.stopPropagation(),e.onSchedule(t)}}},[e._v("schedule")]),e._v(" "),i("button",{staticClass:"btn-underline",on:{click:function(i){i.stopPropagation(),e.onView(t)}}},[e._v("view")]),e._v(" "),i("button",{staticClass:"btn-underline",on:{click:function(i){i.stopPropagation(),e.onSendEmail(t)}}},[e._v("email")]),e._v(" "),i("button",{directives:[{name:"ilpermission",rawName:"v-ilpermission",value:"purchase-add",expression:"'purchase-add'"}],staticClass:"btn-underline",on:{click:function(i){i.stopPropagation(),e.onEdit(t)}}},[e._v("edit")]),e._v(" "),i("button",{directives:[{name:"ilpermission",rawName:"v-ilpermission",value:"purchase-delete",expression:"'purchase-delete'"}],staticClass:"btn-underline",on:{click:function(i){i.stopPropagation(),e.onDelete(t)}}},[e._v("delete")]),e._v(" "),i("el-dropdown",{staticClass:"statusDrop",attrs:{trigger:"click"},on:{command:function(i){e.handleCommand(i,t)}}},[i("span",{staticClass:"el-dropdown-link"},[e._v("\n                  action\n                  "),i("i",{staticClass:"el-icon-caret-bottom el-icon--right"})]),e._v(" "),i("el-dropdown-menu",{attrs:{slot:"dropdown"},slot:"dropdown"},[i("el-dropdown-item",{attrs:{command:"Approved",disabled:!(1===t.status||0===t.status)}},[e._v("Approved")]),e._v(" "),i("el-dropdown-item",{attrs:{command:"Finished",disabled:!(2===t.status||1===t.status||0===t.status)}},[e._v("Finished")])],1)],1)],1)])})):e._e()])]),e._v(" "),i("div",{staticClass:"table-footer"},[i("el-pagination",{attrs:{"current-page":e.currentPage,"page-size":10,layout:"prev, pager, next, jumper",total:e.total},on:{"current-change":e.handleCurrentChange,"update:currentPage":function(t){e.currentPage=t}}}),e._v(" "),i("p",{staticClass:"total"},[e._v("total: "+e._s(e.total))])],1)]),e._v(" "),i("el-dialog",{attrs:{title:"Delete Invoice",visible:e.dialogDeleteVisible,size:"tiny"},on:{"update:visible":function(t){e.dialogDeleteVisible=t}}},[i("p",[e._v("Are you sure to delete it ?")]),e._v(" "),i("div",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[i("el-button",{attrs:{type:"primary"},on:{click:e.deleteInvitation}},[e._v("yes")]),e._v(" "),i("el-button",{attrs:{type:"primary"},on:{click:function(t){e.dialogDeleteVisible=!1}}},[e._v("no")])],1)]),e._v(" "),i("el-dialog",{attrs:{title:"Review Shipment Order",visible:e.dialogReviewVisible,size:"tiny"},on:{"update:visible":function(t){e.dialogReviewVisible=t}}},[i("p",[e._v("Are you sure to do it ?")]),e._v(" "),i("div",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[i("el-button",{attrs:{type:"primary"},on:{click:e.reviewInvitation}},[e._v("yes")]),e._v(" "),i("el-button",{attrs:{type:"primary"},on:{click:function(t){e.dialogReviewVisible=!1}}},[e._v("no")])],1)]),e._v(" "),i("email-dialog",{attrs:{dialogVisible:e.emailDialogVisible},on:{send:function(t){e.sendEmail(t)},cancel:e.sendCancel}})],1)},staticRenderFns:[]}}});
//# sourceMappingURL=14.97f7efc67958ab4671dc.js.map