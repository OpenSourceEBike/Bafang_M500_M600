import Vue from 'vue'
import Router from 'vue-router'

// 登陆
import Login from "src/views/login"
import ForgetPassword from "src/views/forget-password"

// 注册
import Register from "src/views/register/register"
import Invitation from "src/views/register/invitation"
import Account from "src/views/register/account"
import Company from "src/views/register/company"

// 工具
import ToolBattery from "src/components/tool/battery"
import ToolController from "src/components/tool/controller"
import ToolHmi from "src/components/tool/hmi"
import ToolUpdate from "src/components/tool/update"
import ToolRecordList from 'src/components/tool/record'
import OperationTraceRecordList from 'src/components/tool/operationTraceRecord'
import BasicParams from 'src/components/tool/basicParams'
import ToolSensor from 'src/components/tool/sensor'
import componentSave from 'src/components/tool/componentSave'


// 账户
import AccountList from "src/components/account/account"

// 零部件替换历史记录
import ReplaceReportList from "src/components/replace-report/index"

// 服务中心 公用
import ServiceCenterAccountList from "src/components/service-center/account-list"
import TicketDetail from "src/components/service-center/ticket-detail"
import ServiceCenterList from "src/components/service-center/service-center-list"
import CenterTicketList from "src/components/service-center/center-ticket-list"
import ServiceCenterMyTicketList from "src/components/service-center/my-ticket-list"
import ErrorShooting from 'src/components/error/shooting.vue'
import ErrorShootingVideoList from 'src/components/error/video-list.vue'

// 诊断离线
// import DiagnosisNormal from "src/components/diagnosis/diagnosis-normal"
// import DiagnosisTest from "src/components/diagnosis/diagnosis-test"
// 诊断在线
import DiagnosisReport from "src/components/diagnosis/report"
import DiagnosisReportList from "src/components/diagnosis/report-list"

// import NewDiagnosisNormal from "src/components/new-diagnosis/diagnosis"
// import NewDiagnosisPre from "src/components/new-diagnosis/diagnosis-pre"
import CreateParam from 'src/components/createParam/index.vue'
import HeadList from 'src/components/headList/list.vue'
import createDictionary from 'src/components/dictionary/index.vue'
import dictionaryDetail from 'src/components/dictionary/detail.vue'

// 3rd
import Diagnosis from "src/components/3rd-diagnosis/diagnosis"
import DiagnosisResult from 'src/components/3rd-diagnosis/result'
import MechanicDetect from 'src/components/3rd-diagnosis/mechanic-detect.vue'


// 品牌
import BrandIndex from '../views/brand/index.vue'
import BrandList from '../views/brand/brand/list.vue'
import BrandPartnerDealerList from '../views/brand/dealer/list.vue'
import BrandPartnerDealerInvite from '../views/brand/dealer/invite.vue'
import BrandPartnerOEMList from '../views/brand/oem/list.vue'
import BrandPartnerOEMInvite from '../views/brand/oem/invite.vue'
import BrandPartnerComList from '../views/brand/component/list.vue'
import BrandModelDetail from '../views/brand/model/detail.vue'
import BrandModelList from '../views/brand/model/list.vue'
import BrandModelSave from '../views/brand/model/save.vue'
import BrandVehicleList from '../views/brand/vehicle/list.vue'
import BrandVehicleDetail from '../views/brand/vehicle/detail.vue'
import BrandModelBom from '../views/brand/model/bom-list.vue'
import BrandAuthList from '../views/brand/auth/list.vue'


// 主页4模块
import DealerStatistics from '../views/brand/statiatics/dealer-sale'
import DealerStatisticsDetail from '../views/brand/statiatics/dealer-sale-detail'
import AssemblerStatistics from '../views/brand/statiatics/assembler-vehicle'
import AssemblerStatisticsDetail from '../views/brand/statiatics/assembler-vehicle-detail'
import ModelStatistics from '../views/brand/statiatics/model'
import AfterSaleStatistics from '../views/brand/statiatics/after-sale'

// 经销商
import DealerIndex from '../views/dealer/index.vue'
import DealerSaleOrderList from '../views/dealer/sale/list.vue'
import DealerSaleOrderSave from '../views/dealer/sale/save.vue'
import DealerSaleOrderDetail from '../views/dealer/sale/detail.vue'
import DealerSaleOrderEdit from '../views/dealer/sale/edit.vue'
import DealerBrandList from '../views/dealer/partner/list.vue'
import DealerBrandWebsite from '../views/dealer/partner/website.vue'
import DealerBrandInvitation from '../views/dealer/partner/invitation.vue'
import DealerTicketList from '../views/dealer/ticket/list.vue'
import DealerTicketDetail from '../views/dealer/ticket/detail.vue'
import DealerVehicleSearch from '../views/dealer/vehicle/search.vue'
import DealerBomDetail from '../views/dealer/sale/product-detail.vue'
import DealerBomList from '../views/dealer/sale/bom-list.vue'
import DealerVideoList from '../views/dealer/sale/video-list.vue'


// 组装厂
import AssemblerIndex from '../views/assembler/index.vue'
import AssemblerOrderList from '../views/assembler/order/list.vue'
import AssemblerOrderSave from '../views/assembler/order/save.vue'
import AssemblerOrderDetail from '../views/assembler/order/detail.vue'
import AssemblerOrderProduce from '../views/assembler/order/produce.vue'
import AssemblerPartner from '../views/assembler/partner/list.vue'
import AssemblerVehicleList from '../views/assembler/vehicle/list.vue'
import AssemblerVehicleDetail from '../views/assembler/vehicle/detail.vue'
// import AssemblerDiagnosisTest from '../views/assembler/order/diagnosis-test.vue'
// 新版诊断
import AssemblerDiagnosisTest from '../views/assembler/order/assembler-diagnosis.vue'


// 零件厂
import ComponentProductIndex from '../views/component/product/index.vue'
import ComponentProductSave from '../views/component/product/save.vue'
import ComponentProductList from '../views/component/product/list.vue'
import ComponentProductDetail from '../views/component/product/detail.vue'
import ComponentProductBom from '../views/component/product/bom-list.vue'
import ComponentProductVideo from '../views/component/product/video-list.vue'
import ComponentProductItemVideo from '../views/component/product/item-viedo-list.vue'
import ComponentProductEditCategory from '../views/component/product/edit-category.vue'
import ComponentProductAddAttribute from '../views/component/product/add-attribute.vue'
import ComponentProductAttribute from '../views/component/product/attribute-list.vue'
import ComponentComponentList from '../views/component/component/list.vue'
import ComponentComponentDetail from '../views/component/component/detail.vue'
import ComponentOrderSave from '../views/component/order/save.vue'
import ComponentOrderDetail from '../views/component/order/detail.vue'
import ComponentOrderList from '../views/component/order/list.vue'
import ComponentOrderProduce from '../views/component/order/produce.vue'
import ComponentOrderTestReport from '../views/component/order/test-report.vue'
import ComponentTicketStatistics from '../views/component/service-center/statistics.vue'
import ComponentTestFeedback from '../views/component/feedback-center/list.vue'
import ComponentTestFeedbackSave from '../views/component/feedback-center/save.vue'
import ComponentTestFeedbackDetail from '../views/component/feedback-center/detail.vue'
import ComponentOrderDataConnect from '../views/component/order/data-connect.vue'
import ComponentMaterialList from '../views/component/material/list.vue'
import ComponentMaterialDetail from '../views/component/material/detail.vue'
import ComponentApply from '../views/component/audit-apply/apply.vue'
import ComponentAudit from '../views/component/audit-apply/audit.vue'
import ComponentIPVerification from '../views/component/ip-verification/index.vue'

//服务中心 org_type = 5
import ServiceCenterTicketList from '../views/service-center/center-ticket-list.vue'
import MyTicketList from '../views/service-center/my-ticket-list.vue'
import HandleUpTicketList from '../views/service-center/handle-up-ticket-list.vue'
import ServiceCenterTicketDetail from '../views/service-center/ticket-detail.vue'

// 售后
import afterSalesIndex from '../views/service-center/after-sales/aftersales.vue'
import afterSalesPackage from '../views/service-center/after-sales/after-package-list.vue'
import afterSalesPaDetail from '../views/service-center/after-sales/after-package-detail.vue'
import afterSalesPaEdit from '../views/service-center/after-sales/after-package-edit.vue'
import afterSalesPIStep from '../views/service-center/after-sales/after-pi-step.vue'
import afterSalesPI from '../views/service-center/after-sales/after-pi.vue'
import afterSalesPIEdit from '../views/service-center/after-sales/after-pi-edit.vue'
import afterSalesPIDetail from '../views/service-center/after-sales/after-pi-detail.vue'
// 测试
import Test from "src/views/test/test"
import TestImageUpload from "src/views/test/image-upload"
import TestUpload from "src/views/test/file-upload"
import TestClip from "src/views/test/vue-clip-test"
import TestDevice from "src/views/test/device"
import TestPrint from "src/views/test/print"
import TestPage from 'src/components/3rd-diagnosis/result'
import Project from 'src/views/component/project/index'
import CreateProject from 'src/views/component/project/createProject'
import SystemDetail from 'src/views/component/project/SystemDetail'
import testPage from 'src/components/newComponent/index'
import TestPlan from 'src/components/testPlan/testPlan'
import TestPlanReport from 'src/components/testPlan/testPlanReport'
import ReportCharts from 'src/components/testPlan/reportCharts'
import InspectorRecord from 'src/components/testPlan/inspectorRecord'
import clientIndex from 'src/views/component/client/index'
import clientCreate from 'src/views/component/client/create'

// 主页
import Index from '../views/index.vue';

// 仓库
const warehouseList = (resolve) => require(['../views/nl/warehouse/warehouse-list.vue'], resolve);
const warehouseArea = (resolve) => require(['../views/nl/warehouse/warehouse-area.vue'], resolve);
const warehouseLocation = (resolve) => require(['../views/nl/warehouse/warehouse-location.vue'], resolve);
const warehouseRegion = (resolve) => require(['../views/nl/warehouse/warehouse-region.vue'], resolve);
const warehouseInventory = (resolve) => require(["../views/nl/warehouse/warehouse-inventory.vue"], resolve);
const warehouseAreaType = (resolve) => require(["../views/nl/warehouse/warehouse-area-type.vue"], resolve);
const warehouseStorageCache = (resolve) => require(["../views/nl/warehouse/warehouse-storage-cache.vue"], resolve);
const warehouseSetMinInventory = (resolve) => require(["../views/nl/warehouse/warehouse-setMinInventory.vue"], resolve);
const warehouseInventoryDetails = (resolve) => require(["../views/nl/warehouse/warehouse-inventory-details.vue"], resolve);
// 财务
const invoiceList = (resolve) => require(["../views/nl/finance/invoice-list.vue"], resolve);
const invoiceListHeader = (resolve) => require(["../views/nl/finance/invoice-list-header.vue"], resolve);
const invoiceListDetails = (resolve) => require(["../views/nl/finance/invoice-list-details.vue"], resolve);
const cashierList = (resolve) => require(["../views/nl/finance/cashier.vue"], resolve);
const cashierDetails = (resolve) => require(["../views/nl/finance/cashier-details.vue"], resolve);
const receptionList = (resolve) => require(["../views/nl/finance/reception.vue"], resolve);
const receptionDetails = (resolve) => require(["../views/nl/finance/reception-details.vue"], resolve);
// 顾客
const customerList = (resolve) => require(["../views/nl/customer/customer-list.vue"], resolve);
const customerAccount = (resolve) => require(["../views/nl/customer/customer-account.vue"], resolve);
// 入库
const receiptOrderList = (resolve) => require(['../views/nl/receipt-order/receipt-order-list.vue'], resolve);
const receiptOrderHeader = (resolve) => require(["../views/nl/receipt-order/receipt-order-header.vue"], resolve);
const receiptOrderItemList = (resolve) => require(["../views/nl/receipt-order/receipt-order-item-list.vue"], resolve);
const receiptPutAwayAdvice = (resolve) => require(["../views/nl/receipt-order/receipt-put-away-advice.vue"], resolve);
const receiptOrderDetails = (resolve) => require(["../views/nl/receipt-order/receipt-order-details.vue"], resolve);
// 出库
const shipmentOrderList = (resolve) => require(["../views/nl/shipment-order/shipment-order-list.vue"], resolve);
const shipmentOrderHeader = (resolve) => require(["../views/nl/shipment-order/shipment-order-header.vue"], resolve);
const shipmentAllocate = (resolve) => require(["../views/nl/shipment-order/shipment-allocate.vue"], resolve);
const shipmentOrderDetails = (resolve) => require(["../views/nl/shipment-order/shipment-order-details.vue"], resolve);
// 拣货
const pickList = (resolve) => require(["../views/nl/warehouse-pick/pick-list.vue"], resolve);
const pickListAdd = (resolve) => require(["../views/nl/warehouse-pick/pick-list-add.vue"], resolve);
// 权限
const permissionList = (resolve) => require(['../views/nl/permission/permission-list.vue'], resolve);
const permissionCategory = (resolve) => require(["../views/nl/permission/permission-category.vue"], resolve);

// 角色
const roleList = (resolve) => require(['../views/nl/role/role-list.vue'], resolve);
const roleSetting = (resolve) => require(['../views/nl/role/role-setting.vue'], resolve);
const rolePermission = (resolve) => require(['../views/nl/role/role-permission.vue'], resolve);

// 用户
const userList = (resolve) => require(['../views/nl/user/user-list.vue'], resolve)

// 部门
const departmentList = (resolve) => require(['../views/nl/department/department-list.vue'], resolve);
// 采购
const purchaseList = (resolve) => require(['../views/nl/purchase/purchase-list.vue'], resolve);
const purchaseListDetails = (resolve) => require(['../views/nl/purchase/purchase-list-details.vue'], resolve);
const purchaseOrderDetails = (resolve) => require(['../views/nl/purchase/purchase-order-details.vue'], resolve);
const purchaseSchedule = (resolve) => require(['../views/nl/purchase/purchase-schedule.vue'], resolve);

// 销售
const PIList = (resolve) => require(['../views/nl/sales/proforma-invoice.vue'], resolve);
const PIHeader = (resolve) => require(['../views/nl/sales/proforma-invoice-header.vue'], resolve);
const PIDetails = (resolve) => require(['../views/nl/sales/proforma-invoice-details.vue'], resolve);
const deliveryList = (resolve) => require(['../views/nl/sales/delivery-list.vue'], resolve);
const deliveryListHeader = (resolve) => require(['../views/nl/sales/delivery-list-header.vue'], resolve);
const deliveryListDetails = (resolve) => require(['../views/nl/sales/delivery-list-details.vue'], resolve);
// PI进度状态
const orderSchedule = (resolve) => require(['../views/nl/sales/order-schedule.vue'], resolve);
// 售后
const serviceCenter = (resolve) => require(['../views/nl/after-sales/service-center/service-center.vue'], resolve);
const serviceCenterSave = (resolve) => require(['../views/nl/after-sales/service-center/service-center-save.vue'], resolve);
const customerServiceRecord = (resolve) => require(['../views/nl/after-sales/service-center/customer-service-record.vue'], resolve);
const customerServiceRecordSave = (resolve) => require(['../views/nl/after-sales/service-center/customer-service-record-save.vue'], resolve);
const technicalService = (resolve) => require(['../views/nl/after-sales/technical-service/technical-service.vue'], resolve);
const technicalServiceSave = (resolve) => require(['../views/nl/after-sales/technical-service/technical-service-save.vue'], resolve);
// 客服
const customerServiceUser = (resolve) => require(['../views/nl/after-sales/customer-service/customer-service-user.vue'], resolve);
const technicalServiceUser = (resolve) => require(['../views/nl/after-sales/customer-service/technical-service-user.vue'], resolve);
const ParcelInfo = (resolve) => require(['../views/nl/after-sales/customer-service/parcel-information.vue'], resolve);
const ParcelInfoSave = (resolve) => require(['../views/nl/after-sales/customer-service/parcel-information-save.vue'], resolve);
// 清关
const commerialInvoice = (resolve) => require(['../views/nl/clean-management/commerial-invoice.vue'], resolve);
const commerialInvoiceSave = (resolve) => require(['../views/nl/clean-management/commerial-invoice-save.vue'], resolve);
const commerialInvoiceDetails = (resolve) => require(['../views/nl/clean-management/commerial-invoice-details.vue'], resolve);
const cleanRecord = (resolve) => require(['../views/nl/clean-management/clean-record.vue'], resolve);
// 状态进度
const afterSalesSchedule = (resolve) => require(['../views/nl/after-sales/service-center/after-sales-schedule'], resolve);
// 承运单
const carrierOrder = (resolve) => require(['../views/nl/after-sales/customer-service/carrier-order'], resolve);


// 产品
const productsIndex = () =>
    import ('../views/nl-view/products/index');
const inventoryIndex = () =>
    import ('../views/nl-view/inventory/index');

// 仓库
const warehouseListIndex = () =>
    import ('../views/nl-view/warehouse/warehouseList.vue');
const warehouseAreaIndex = () =>
    import ('../views/nl-view/warehouse/area');
const warehouseLocationIndex = () =>
    import ('../views/nl-view/warehouse/location');
// 阈值
const thresholdIndex = () =>
    import ('../views/nl-view/warehouse/threshold');
// 入库
const stockIn = () =>
    import ('../views/nl-view/stocks/in');
const StockInDetail = () =>
    import ('../views/nl-view/stocks/in-detail.vue');
// 出库
const stockOut = () =>
    import ('../views/nl-view/stocks/out.vue');
const StockOutDetail = () =>
    import ('../views/nl-view/stocks/out-detail.vue');


Vue.use(Router);

let routes = [
    // 荷兰项目v2
    // 仓库
    {
        path: '/warehouse',
        component: Index,
        children: [{
            path: 'list',
            name: 'warehouseListIndex',
            component: warehouseListIndex
        }, {
            path: 'area',
            name: 'warehouseAreaIndex',
            component: warehouseAreaIndex
        }, {
            path: 'location',
            name: 'warehouseLocationIndex',
            component: warehouseLocationIndex
        }, {
            path: 'threshold/:id',
            name: 'thresholdIndex',
            component: thresholdIndex,
            props: true
        }]
    },
    // 产品模块
    {
        path: '/products',
        component: Index,
        children: [{
            path: 'index',
            name: 'productsIndex',
            component: productsIndex
        }]
    },
    //库存
    {
        path: '/inventory',
        component: Index,
        children: [{
            path: 'index',
            name: 'inventoryIndex',
            component: inventoryIndex
        }]
    },
    // 出入库
    {
        path: '/stocks',
        component: Index,
        children: [{
            path: 'in',
            name: 'stockIn',
            component: stockIn
        }, {
            path: 'in-detail/:id',
            name: 'StockInDetail',
            component: StockInDetail,
            props: true
        }, {
            path: 'out',
            name: 'stockOut',
            component: stockOut
        }, {
            path: 'out-detail/:id',
            name: 'StockOutDetail',
            component: StockOutDetail,
            props: true
        }]
    },
    { path: '/login', name: "login", component: Login },

    { path: '/forget-password', name: "forgetPassword", component: ForgetPassword },

    // 注册
    {
        path: '/register',
        name: '',
        component: Register,
        children: [
            { path: 'invitation', name: 'invitation', component: Invitation },
            { path: 'account', name: 'account', component: Account },
            { path: 'company', name: 'company', component: Company },
        ]
    },

    //品牌
    {
        path: '/brand',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [
            { path: 'brand/index', name: 'brandIndex', component: BrandIndex },
            { path: 'brand/list', name: 'brandList', component: BrandList },
            { path: 'statistics/dealer-sale', name: 'statisticsDealerSale', component: DealerStatistics },
            { path: 'statistics/dealer-sale-detail', name: 'statisticsDealerSaleDetail', component: DealerStatisticsDetail },
            { path: 'statistics/assembler-vehicle', name: 'statisticsAssemblerStatistics', component: AssemblerStatistics },
            { path: 'statistics/assembler-vehicle-detail', name: 'statisticsAssemblerStatisticsDetail', component: AssemblerStatisticsDetail },
            { path: 'statistics/model', name: 'statisticsModelStatistics', component: ModelStatistics },
            { path: 'statistics/after-sale', name: 'statisticsAfterSaleStatistics', component: AfterSaleStatistics },
            { path: 'service-center/ticket/service-center-list', name: 'brandServiceCenterList', component: ServiceCenterList },
            { path: 'service-center/ticket/center-ticket-list', name: 'brandServiceCenterTicketList', component: CenterTicketList },
            { path: 'service-center/ticket/ticket-detail', name: 'brandServiceCenterTicketDetail', component: TicketDetail },
            { path: 'service-center/ticket/account-list', name: 'BrandServiceCenterAccountList', component: ServiceCenterAccountList },
            { path: 'service-center/ticket/my-ticket-list', name: 'brandServiceCenterMyTicketList', component: ServiceCenterMyTicketList },

            { path: 'partner/dealer/list', name: 'brandPartnerDealerList', component: BrandPartnerDealerList },
            { path: 'partner/dealer/invite', name: 'brandPartnerDealerInvite', component: BrandPartnerDealerInvite },
            { path: 'partner/oem/list', name: 'brandPartnerOEMList', component: BrandPartnerOEMList },
            { path: 'partner/oem/invite', name: 'brandPartnerOEMInvite', component: BrandPartnerOEMInvite },
            { path: 'partner/component/list', name: 'brandPartnerComList', component: BrandPartnerComList },
            { path: 'model/detail', name: 'brandModelDetail', component: BrandModelDetail },
            { path: 'model/list', name: 'brandModelList', component: BrandModelList },
            { path: 'model/save', name: 'brandModelSave', component: BrandModelSave },
            { path: 'account/list', name: 'brandAccountList', component: AccountList },
            { path: 'vehicle/list', name: 'brandVehicleList', component: BrandVehicleList },
            { path: 'vehicle/detail', name: 'brandVehicleDetail', component: BrandVehicleDetail },

            { path: 'model/bom', name: 'brandModelBom', component: BrandModelBom },
            { path: 'tool/battery', name: 'brandToolBattery', component: ToolBattery },
            { path: 'tool/controller', name: 'brandToolController', component: ToolController },
            { path: 'tool/hmi', name: 'brandToolHmi', component: ToolHmi },
            { path: 'tool/update', name: 'brandToolUpdate', component: ToolUpdate },
            { path: 'tool/auth', name: 'brandAuthList', component: BrandAuthList },
            { path: 'error/shooting', name: 'brandErrorShooting', component: ErrorShooting },
            { path: 'error/video-list', name: 'brandErrorShootingVideoList', component: ErrorShootingVideoList },

            { path: 'vehicle/replace-report-list', name: 'brandReplaceReportList', component: ReplaceReportList },

            // { path: 'diagnosis/new-diagnosis', name: 'brandNewDiagnosisNormal', component: NewDiagnosisNormal },
            // { path: 'diagnosis/pre-diagnosis', name: 'brandNewDiagnosisPre', component: NewDiagnosisPre },

            // { path: 'diagnosis/diagnosis', name: 'brandDiagnosisNormal', component: DiagnosisNormal },
            // { path: 'diagnosis/diagnosis-test', name: 'brandDiagnosisTest', component: DiagnosisTest},
            // { path: 'diagnosis/diagnosis', name: 'brandNewDiagnosis', component: NewDiagnosis },
            // { path: 'diagnosis/result', name: 'brandNewDiagnosisResult', component: NewDiagnosisResult },

            { path: 'diagnosis/3rd-diagnosis', name: 'brandDiagnosisNormal', component: Diagnosis },
            { path: 'diagnosis/result', name: 'brandDiagnosisResult', component: DiagnosisResult },
            { path: 'diagnosis/report', name: 'brandDiagnosisReport', component: DiagnosisReport },
            { path: 'diagnosis/report-list', name: 'brandDiagnosisReportList', component: DiagnosisReportList },

            { path: 'tool/record', name: 'brandRecordList', component: ToolRecordList }

        ]
    },

    // 经销商
    {
        path: '/dealer',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [
            { path: 'index', name: 'dealerIndex', component: DealerIndex },
            { path: 'sale/list', name: 'dealerSaleOrderList', component: DealerSaleOrderList },
            { path: 'sale/save', name: 'dealerSaleOrderSave', component: DealerSaleOrderSave },
            { path: 'sale/detail', name: 'dealerSaleOrderDetail', component: DealerSaleOrderDetail },
            { path: 'sale/bom-detail', name: 'dealerBomDetail', component: DealerBomDetail },
            { path: 'sale/bom-list', name: 'dealerBomList', component: DealerBomList },
            { path: 'sale/video-list', name: 'dealerVideoList', component: DealerVideoList },
            { path: 'sale/edit', name: 'dealerSaleOrderEdit', component: DealerSaleOrderEdit },

            { path: 'account/list', name: 'dealerAccountList', component: AccountList },
            { path: 'tool/battery', name: 'dealerToolBattery', component: ToolBattery },
            { path: 'tool/controller', name: 'dealerToolController', component: ToolController },
            { path: 'tool/hmi', name: 'dealerToolHmi', component: ToolHmi },
            { path: 'tool/update', name: 'dealerToolUpdate', component: ToolUpdate },
            { path: 'partner/list', name: 'dealerBrandList', component: DealerBrandList },
            { path: 'partner/website', name: 'dealerBrandWebsite', component: DealerBrandWebsite },
            { path: 'partner/invitation', name: 'dealerBrandInvitation', component: DealerBrandInvitation },
            { path: 'ticket/list', name: 'dealerTicket', component: DealerTicketList },
            { path: 'ticket/detail', name: 'dealerTicketDetail', component: DealerTicketDetail },
            { path: 'error/shooting', name: 'dealerErrorShooting', component: ErrorShooting },

            { path: 'vehicle/search', name: 'dealerVehicleSearch', component: DealerVehicleSearch },
            { path: 'replace-report-list', name: 'dealerReplaceReportList', component: ReplaceReportList },

            // { path: 'diagnosis/new-diagnosis', name: 'dealerNewDiagnosisNormal', component: NewDiagnosisNormal },
            // { path: 'diagnosis/pre-diagnosis', name: 'dealerNewDiagnosisPre', component: NewDiagnosisPre },
            // { path: 'diagnosis/diagnosis', name: 'dealerDiagnosis', component: DiagnosisNormal },
            // { path: 'diagnosis/diagnosis-test', name: 'dealerDiagnosisTest', component: DiagnosisTest },

            { path: 'diagnosis/3rd-diagnosis', name: 'dealerDiagnosisNormal', component: Diagnosis },
            { path: 'diagnosis/result', name: 'dealerDiagnosisResult', component: DiagnosisResult },
            { path: 'diagnosis/report', name: 'dealerDiagnosisReport', component: DiagnosisReport },
            { path: 'diagnosis/report-list', name: 'dealerDiagnosisReportList', component: DiagnosisReportList },
            { path: 'mechanic-detect', name: 'DealerMechanicDetect', component: MechanicDetect },

        ]
    },

    // 组装厂
    {
        path: '/assembler',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [

            { path: 'index', name: 'assemblerIndex', component: AssemblerIndex },
            { path: 'order/list', name: 'assemblerOrderList', component: AssemblerOrderList },
            { path: 'order/save', name: 'assemblerOrderSave', component: AssemblerOrderSave },
            { path: 'order/detail', name: 'assemblerOrderDetail', component: AssemblerOrderDetail },
            { path: 'order/produce', name: 'assemblerOrderProduce', component: AssemblerOrderProduce },
            { path: 'partner/list', name: 'assemblerPartner', component: AssemblerPartner },
            { path: 'account/list', name: 'assemblerAccountList', component: AccountList },
            { path: 'vehicle/list', name: 'assemblerVehicleList', component: AssemblerVehicleList },
            { path: 'vehicle/detail', name: 'assemblerVehicleDetail', component: AssemblerVehicleDetail },
            { path: 'error/shooting', name: 'assemblerErrorShooting', component: ErrorShooting },

            { path: 'vehicle/replace-report-list', name: 'assemblerReplaceReportList', component: ReplaceReportList },

            { path: 'diagnosis/3rd-diagnosis', name: 'assemblerDiagnosisNormal', component: Diagnosis },
            { path: 'diagnosis/result', name: 'assemblerDiagnosisResult', component: DiagnosisResult },
            { path: 'diagnosis/report', name: 'assemblerDiagnosisReport', component: DiagnosisReport },
            { path: 'diagnosis/report-list', name: 'assemblerDiagnosisReportList', component: DiagnosisReportList },

            // { path: 'diagnosis/new-diagnosis', name: 'assemblerNewDiagnosisNormal', component: NewDiagnosisNormal },
            // { path: 'diagnosis/pre-diagnosis', name: 'assemblerNewDiagnosisPre', component: NewDiagnosisPre },

        ]
    },

    // 零件
    {
        path: '/component',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
                path: 'product',
                name: 'productIndex',
                component: ComponentProductIndex,
                children: [
                    { path: 'list', name: 'componentProductList', component: ComponentProductList },
                    { path: 'save', name: 'componentProductSave', component: ComponentProductSave },
                    { path: 'detail', name: 'componentProductDetail', component: ComponentProductDetail },
                    { path: 'bom-list', name: 'componentProductBom', component: ComponentProductBom },
                    { path: 'video-list', name: 'componentProductItemVideo', component: ComponentProductItemVideo }
                ]
            },
            { path: 'video-list', name: 'componentProductVideo', component: ComponentProductVideo },
            { path: 'attribute', name: 'componentProductAttribute', component: ComponentProductAttribute },
            { path: 'edit-category', name: 'componentProductEditCategory', component: ComponentProductEditCategory },
            { path: 'add-attribute', name: 'componentProductAddAttribute', component: ComponentProductAddAttribute },
            { path: 'component/detail', name: "componentComponentDetail", component: ComponentComponentDetail },
            { path: 'component/list', name: "componentComponentList", component: ComponentComponentList },
            { path: 'order/list', name: "componentOrderList", component: ComponentOrderList },
            { path: 'order/save', name: "componentOrderSave", component: ComponentOrderSave },
            { path: 'order/detail', name: "componentOrderDetail", component: ComponentOrderDetail },
            { path: 'order/produce', name: "componentOrderProduce", component: ComponentOrderProduce },
            { path: 'order/test-report', name: "componentOrderTestReport", component: ComponentOrderTestReport },
            { path: 'order/data-connect', name: "componentOrderDataConnect", component: ComponentOrderDataConnect },

            { path: 'tool/battery', name: 'componentToolBattery', component: ToolBattery },
            { path: 'tool/controller', name: 'componentToolController', component: ToolController },
            { path: 'tool/hmi', name: 'componentToolHmi', component: ToolHmi },
            { path: 'tool/update', name: 'componentToolUpdate', component: ToolUpdate },
            { path: 'tool/sensor', name: 'componentToolSensor', component: ToolSensor },
            { path: 'tool/save', name: 'componentOEMSave', component: componentSave },
            { path: 'error/shooting', name: 'componentErrorShooting', component: ErrorShooting },
            { path: 'account/list', name: 'componentAccountList', component: AccountList },

            // { path: 'diagnosis/diagnosis', name: 'componentDiagnosisNormal', component: DiagnosisNormal },
            // { path: 'diagnosis/diagnosis-test', name: 'componentDiagnosisTest', component: DiagnosisTest },
            { path: 'diagnosis/3rd-diagnosis', name: 'componentDiagnosisNormal', component: Diagnosis },
            { path: 'diagnosis/result', name: 'componentDiagnosisResult', component: DiagnosisResult },
            { path: 'diagnosis/report', name: 'componentDiagnosisReport', component: DiagnosisReport },
            { path: 'diagnosis/report-list', name: 'componentDiagnosisReportList', component: DiagnosisReportList },

            // { path: 'diagnosis/new-diagnosis', name: 'componentNewDiagnosisNormal', component: NewDiagnosisNormal },
            // { path: 'diagnosis/pre-diagnosis', name: 'componentNewDiagnosisPre', component: NewDiagnosisPre },

            { path: 'tool/record', name: 'componentRecordList', component: ToolRecordList },
            { path: 'tool/operationTraceRecord', name: 'operationTraceRecord', component: OperationTraceRecordList },
            { path: 'tool/basic-params', name: 'basicParams', component: BasicParams },
            { path: 'feedback-center/list', name: 'componentTestFeedback', component: ComponentTestFeedback },
            { path: 'feedback-center/save', name: 'componentTestFeedbackSave', component: ComponentTestFeedbackSave },
            { path: 'feedback-center/detail', name: 'componentTestFeedbackDetail', component: ComponentTestFeedbackDetail },
            { path: 'material/list', name: 'ComponentMaterialList', component: ComponentMaterialList },
            { path: 'material/detail', name: 'componentMaterialDetail', component: ComponentMaterialDetail },
            { path: 'service-center/ticket/service-center-list', name: 'componentServiceCenterAssignList', component: ServiceCenterList },
            { path: 'service-center/ticket/center-ticket-list', name: 'componentServiceCenterTicketList', component: CenterTicketList },
            { path: 'service-center/ticket/my-ticket-list', name: 'componentMyTicketList', component: ServiceCenterMyTicketList },
            { path: 'service-center/ticket/ticket-detail', name: 'componentServiceCenterTicketDetail', component: TicketDetail },
            { path: 'service-center/ticket/account-list', name: 'componentCenterAccountList', component: ServiceCenterAccountList },
            { path: 'service-center/ticket/ticket-statistics', name: 'componentTicketStatistics', component: ComponentTicketStatistics },
            { path: 'project/index', name: 'project', component: Project },
            { path: 'project/createProject', name: 'createProject', component: CreateProject },
            { path: 'project/testPage', name: 'testPage', component: testPage },
            { path: 'project/systemDetail', name: 'systemDetail', component: SystemDetail },
            { path: 'createParam/index', name: 'createParam', component: CreateParam },
            { path: 'headList/index', name: 'headList', component: HeadList },
            { path: 'dictionary/index', name: 'createDictionary', component: createDictionary },
            { path: 'dictionary/detail', name: 'dictionaryDetail', component: dictionaryDetail },
            { path: 'client/index', name: 'clientIndex', component: clientIndex },
            { path: 'client/create', name: 'clientCreate', component: clientCreate },
            { path: 'testPlan/testPlan', name: 'testPlan', component: TestPlan },
            { path: 'testPlan/testPlanReport', name: 'testPlanReport', component: TestPlanReport },
            { path: 'testPlan/reportCharts', name: 'reportCharts', component: ReportCharts },
            { path: 'testPlan/inspectorRecord', name: 'inspectorRecord', component: InspectorRecord },
            { path: 'audit-apply/apply', name: 'componentApply', component: ComponentApply },
            { path: 'audit-apply/audit', name: 'componentAudit', component: ComponentAudit },
            { path: 'audit-apply/ip', name: 'ComponentIPVerification', component: ComponentIPVerification },


        ]
    },

    //服务中心
    {
        path: '/service-center',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [
            { path: 'center-ticket-list', name: 'centerTicketList', component: ServiceCenterTicketList },
            { path: 'my-ticket-list', name: 'myTicketList', component: MyTicketList },
            { path: 'handle-up-ticket-list', name: 'handleUpTicketList', component: HandleUpTicketList },
            { path: 'detail', name: 'serviceCenterTicketDetail', component: ServiceCenterTicketDetail },
            { path: 'error/shooting', name: 'serviceCenterErrorShooting', component: ErrorShooting },

            // 售后
            { path: 'mechanic-detect', name: 'mechanicDetect', component: MechanicDetect },
            { path: 'after-index', name: 'afterIndex', component: afterSalesIndex },
            { path: 'after-package', name: 'afterPackage', component: afterSalesPackage },
            { path: 'after-pa-detail', name: 'afterPaDetail', component: afterSalesPaDetail },
            { path: 'after-pa-edit', name: 'afterPaEdit', component: afterSalesPaEdit },

            { path: 'after-pi-step', name: 'afterPiStep', component: afterSalesPIStep },
            { path: 'after-pi', name: 'afterPi', component: afterSalesPI },
            { path: 'after-pi-edit', name: 'afterPiEdit', component: afterSalesPIEdit },
            { path: 'after-pi-detail', name: 'afterPiDetail', component: afterSalesPIDetail },

        ]
    },
    //仓库 warehouse
    {
        path: '/warehouse',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
                path: 'warehouse-list',
                name: 'warehouseList',
                component: warehouseList
            },
            {
                path: 'warehouse-area',
                name: 'warehouseArea',
                component: warehouseArea,
            }, {
                path: 'warehouse-location',
                name: 'warehouseLocation',
                component: warehouseLocation,
            }, {
                path: 'warehouse-region',
                name: 'warehouseRegion',
                component: warehouseRegion,
            }, {
                path: 'warehouse-inventory',
                name: 'warehouseInventory',
                component: warehouseInventory,
            },
            {
                path: 'warehouse-area-type',
                name: 'warehouseAreaType',
                component: warehouseAreaType,
            },
            {
                path: 'warehouse-storage-cache',
                name: 'warehouseStorageCache',
                component: warehouseStorageCache,
            },
            {
                path: 'warehouse-setMinInventory',
                name: 'warehouseSetMinInventory',
                component: warehouseSetMinInventory
            }, {
                path: 'warehouse-inventory-details',
                name: 'warehouseInventoryDetails',
                component: warehouseInventoryDetails
            },
            // 出库
            {
                path: 'shipment-order-list',
                name: 'shipmentOrderList',
                component: shipmentOrderList,
            }, {
                path: 'shipment-order-header',
                name: 'shipmentOrderHeader',
                component: shipmentOrderHeader,
            }, {
                path: 'shipment-allocate',
                name: 'shipmentAllocate',
                component: shipmentAllocate,
            }, {
                path: 'shipment-order-details',
                name: 'shipmentOrderDetails',
                component: shipmentOrderDetails,
            },
            // 入库
            {
                path: 'receipt-order-list',
                name: 'receiptOrderList',
                component: receiptOrderList
            }, {
                path: 'receipt-order-header',
                name: 'receiptOrderHeader',
                component: receiptOrderHeader
            }, {
                path: 'receipt-order-item-list',
                name: 'receiptOrderItemList',
                component: receiptOrderItemList,
            }, {
                path: 'receipt-put-away-advice',
                name: 'receiptPutAwayAdvice',
                component: receiptPutAwayAdvice,
            }, {
                path: 'receipt-order-details',
                name: 'receiptOrderDetails',
                component: receiptOrderDetails,
            },
            // 拣货
            {
                path: 'pick-list',
                name: 'pickList',
                component: pickList,
            }, {
                path: 'pick-list-add',
                name: 'pickListAdd',
                component: pickListAdd,
            },
        ]
    },
    // 采购
    {
        path: '/purchase',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'purchase-list',
            name: 'purchaseList',
            component: purchaseList
        }, {
            path: 'purchase-list-details',
            name: 'purchaseListDetails',
            component: purchaseListDetails
        }, {
            path: 'purchase-order-details',
            name: 'purchaseOrderDetails',
            component: purchaseOrderDetails
        }, {
            path: 'purchase-schedule',
            name: 'purchaseSchedule',
            component: purchaseSchedule
        }]
    },
    // 销售
    {
        path: '/sales',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'proforma-invoice',
            name: 'PIList',
            component: PIList
        }, {
            path: 'proforma-invoice-header',
            name: 'PIHeader',
            component: PIHeader
        }, {
            path: 'proforma-invoice-details',
            name: 'PIDetails',
            component: PIDetails
        }, {
            path: 'delivery-list',
            name: 'deliveryList',
            component: deliveryList
        }, {
            path: 'delivery-list-header',
            name: 'deliveryListHeader',
            component: deliveryListHeader

        }, {
            path: 'delivery-list-details',
            name: 'deliveryListDetails',
            component: deliveryListDetails

        }, {
            path: 'order-schedule',
            name: 'orderSchedule',
            component: orderSchedule
        }]
    },
    // 清关
    {
        path: '/clean-manage',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'commerial-invoice',
            name: 'commerialInvoice',
            component: commerialInvoice
        }, {
            path: 'commerial-invoice-save',
            name: 'commerialInvoiceSave',
            component: commerialInvoiceSave
        }, {
            path: 'commerial-invoice-details',
            name: 'commerialInvoiceDetails',
            component: commerialInvoiceDetails
        }, {
            path: 'clean-record',
            name: 'cleanRecord',
            component: cleanRecord
        }]
    },
    // 售后
    {
        path: '/after-sales',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'service-center',
            name: 'serviceCenter',
            component: serviceCenter
        }, {
            path: 'service-center-save',
            name: 'serviceCenterSave',
            component: serviceCenterSave
        }, {
            path: 'customer-service-record',
            name: 'customerServiceRecord',
            component: customerServiceRecord
        }, {
            path: 'customer-service-record-save',
            name: 'customerServiceRecordSave',
            component: customerServiceRecordSave
        }, {
            path: 'technical-service',
            name: 'technicalService',
            component: technicalService
        }, {
            path: 'technical-service-save',
            name: 'technicalServiceSave',
            component: technicalServiceSave
        }, {
            path: 'after-sales-schedule',
            name: 'afterSalesSchedule',
            component: afterSalesSchedule
        }, {
            path: 'parcel-information',
            name: 'ParcelInfo',
            component: ParcelInfo
        }, {
            path: 'parcel-information-save',
            name: 'ParcelInfoSave',
            component: ParcelInfoSave
        }]
    },
    // 客服
    {
        path: '/customer-service',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'customer-service-user',
            name: 'customerServiceUser',
            component: customerServiceUser
        }, {
            path: 'technical-service-user',
            name: 'technicalServiceUser',
            component: technicalServiceUser
        }, {
            path: 'carrier-order',
            name: 'carrierOrder',
            component: carrierOrder
        }]
    },
    // 顾客
    {
        path: '/customer',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'customer-list',
            name: 'customerList',
            component: customerList
        }, {
            path: 'customer-account',
            name: 'customerAccount',
            component: customerAccount
        }]
    },
    // 发票
    {
        path: '/finance',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'invoice-list',
            name: 'invoiceList',
            component: invoiceList
        }, {
            path: 'invoice-list-header',
            name: 'invoiceListHeader',
            component: invoiceListHeader
        }, {
            path: 'invoice-list-details',
            name: 'invoiceListDetails',
            component: invoiceListDetails
        }, {
            path: 'cashier',
            name: 'cashierList',
            component: cashierList
        }, {
            path: 'cashier-details',
            name: 'cashierDetails',
            component: cashierDetails
        }, {
            path: 'reception',
            name: 'receptionList',
            component: receptionList
        }, {
            path: 'reception-details',
            name: 'receptionDetails',
            component: receptionDetails
        }]

    },
    // 权限
    {
        path: '/permission',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'permission-list',
            name: 'permissionList',
            component: permissionList
        }, {
            path: 'permission-category',
            name: 'permissionCategory',
            component: permissionCategory
        }]
    },
    // 部门
    {
        path: '/department',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'department-list',
            name: 'departmentList',
            component: departmentList
        }]
    },

    // 角色
    {
        path: '/role',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'role-list',
            name: 'roleList',
            component: roleList
        }, {
            path: 'role-setting',
            name: 'roleSetting',
            component: roleSetting
        }, {
            path: 'role-permission',
            name: 'rolePermission',
            component: rolePermission
        }]
    },

    // 用户
    {
        path: '/user',
        name: '',
        component: Index,
        iconCls: 'el-icon-message',
        leaf: true,
        icon: "icon-home",
        public: true,
        children: [{
            path: 'user-list',
            name: 'userList',
            component: userList
        }]
    },

    //离线2页面
    // { path: '/diagnosis/diagnosis', name: 'diagnosisNormal', component: DiagnosisNormal },
    // { path: '/diagnosis/diagnosis-test', name: 'diagnosisTest', component: DiagnosisTest },

    { path: '/index', name: "index", component: Index },
    { path: '/index', hidden: true, redirect: { path: 'login' } },

    { path: '/test', name: "test", component: Test },
    { path: '/test-upload', name: "testUpload", component: TestUpload },
    { path: '/test-clip', name: "testClip", component: TestClip },
    { path: '/test-image', name: "TestImageUpload", component: TestImageUpload },
    { path: '/test-device', name: "TestDevice", component: TestDevice },
    { path: '/test-print', name: "TestPrint", component: TestPrint },

    { path: '/', hidden: true, redirect: { path: 'login' } }

];

export default new Router({
    routes: routes
})



// WEBPACK FOOTER //
// ./src/router/index.js