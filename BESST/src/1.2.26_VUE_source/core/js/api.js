import Const from "./const"
import Vue from "vue"
import VueResource from "vue-resource"
import Core from "../core"
import router from '../../src/router/index';
import {
    Message
} from 'element-ui';

function Api() {
    Vue.use(VueResource);

    const apiList = {
        Common: {
            sendEmailVCode: ['common/email-verification-code-send', 'email'],
            checkEmailVCode: ['common/email-verification-code-check', 'email', 'code'],
            sendEmailStaff: ['common/email-staff', 'no', 'email']
        },
        User: {
            login: ['user/login', 'email', 'password', 'ip'],
            register: ['user/register', 'email', 'password', 'code', 'name'],
            resetPassword: ['user/password-reset', 'email', 'password', 'code'],
            logout: ['user/logout'],
            changePassword: ['user/password-change', 'old_password', 'new_password'],
            findUserByEmail: ['/user/user-by-email', 'email', ''],
        },
        Brand: {
            // 品牌商邀请其他公司
            sendInvitation: ['invite/send', 'id'],
            deleteInvitation: ['invite/delete', 'id'],
            invitationEmailSend: ['invite/email-send', 'org_name', 'org_type', 'email', 'type'],
            invitationEmailBatchSend: ['invite/email-batch-send', 'invite_id_list'],
            getOrgInvitationList: ['invite/list', 'org_type', 'status', 'query', 'page'],
            getOrgDealerInvitationList: ['invite/dealer-list', 'status', 'query', 'page'],
            getOrgInvitationAllList: ['invite/all-list', 'org_type', 'status'],
            //获取所有的品牌
            getBrandList: ['brand/list', 'page'],
            getAllBrandList: ['brand/all-list'],
            saveBrand: ['brand/save', 'id', 'name', 'img', 'desc', 'website'],
            deleteBrand: ['brand/delete', 'id'],
            //根据id获取品牌列表
            getBrandListById: ['brand/all-list-by-org-id', 'id'],
            //首页统计
            getDealerSaleStatistic: ['stat/brand/dealer-sales', 'org_id', 'type', 'dealer_org_id'], // org_id 品牌商的org_id
            getDealerSalesList: ['stat/brand/dealer-sales-list', 'org_id', 'type', 'page'],
            getDealerSalesDetail: ['stat/brand/each-dealer-sales', 'org_id', 'dealer_org_id'],
            getAssemblerStat: ['stat/brand/assembler-stat', 'org_id', 'type'],
            getAssemblerStatList: ['stat/brand/assembler-stat-list', 'org_id', 'type', 'page'],
            getEachAssembleStat: ['stat/brand/each-assembler-stat', 'assembler_org_id', 'type', 'page'],
            getModelStat: ['stat/brand/item-stat'],
            getModelSale: ['stat/brand/item-sales', 'org_id', 'type'],
            getModelStatList: ['stat/brand/item-sales-list', 'org_id', 'type', 'page'],
            getAfterSaleStat: ['stat/brand/after-sales-stat', 'org_id', 'type'],
            getAfterSaleStatList: ['stat/brand/after-sales-stat-list', 'org_id', 'type', 'page'],
        },
        Org: {
            orgSave: ['org/save', 'id', 'type', 'name', 'email', 'country', 'address', 'postcode', 'phone', 'website', 'avatar', 'country_status'],
            // searchMemberList: ['org/org-member-list', 'org_id', 'page', 'status', 'email'],
            searchMemberList: ['org/org-member-list-like', 'org_id', 'page', 'status', 'email'],
            allOrgMemberList: ['org/all-org-member-list', 'org_id', 'status'],
            orgMemberDelete: ['org/member-delete', 'member_user_id'],
            orgMemberReset: ['org/member-reset', 'member_user_id', 'type'],   //  1 使用默认12345678， 0 默认， 随机密码
            // 组织内邀请
            createMember: ['org/invitation-email-send', 'email'],
            // 品牌商获取合作商
            getJuniorPartnerList: ['org/junior-partner-list', 'org_type', 'name', 'phone', 'page'],
            // 品牌商获取合作经销商
            getJuniorDealerPartnerList: ['org/junior-dealer-partner-list', 'name', 'phone', 'page'],
            // 合作商获取品牌商
            getPartnerBranList: ['org/superior-partner-list', 'page'],

            // 品牌商获取合作商不分页
            getAllJuniorPartnerList: ['org/all-junior-partner-list', 'org_type'],
            // 合作商获取品牌商不分页
            getAllPartnerBranList: ['org/all-superior-partner-list'],
            // 合作商接受品牌商邀请
            acceptInvite: ['invite/accept', 'code'],
            checkInviteByUser: ['invite/check', 'code'],
            checkInvite: ['invite/detail', 'code'],
            // 判断是否是manager
            isManager: ['org/is-manager', 'org_id', 'user_id'],
            deletePartnerByBrand: ['org/delete-partner-by-brand', 'org_id'],

            //服务中心
            allServiceCenterList: ['org/all-service-center-list', 'org_id'],
            serviceCenterList: ['org/service-center-list'],
            createServiceCenter: ['org/service-center-save', 'id', 'name', 'email', 'country', 'address', 'postcode', 'phone', 'website', 'avatar'],
            getServiceCenterList: ['org/service-center-list', 'page'],
            deleteServiceCenter: ['org/service-center-delete', 'id'],

            //服务中心账号
            createServiceCenterAccount: ['org/service-center-account-save', 'org_id', 'email', 'password'],
            getServiceCenterAccountList: ['org/service-center-account-list', 'org_id', 'page'],
            resetServiceCenterAccount: ['org/service-center-account-reset', 'user_id', 'password'],
            deleteServiceCenterAccount: ['org/service-center-account-delete', 'id'],
            getOrgInvitationCode: ['org/updateInvitationCode', 'user_id'],
            deleteOrgInvitationCode: ['org/deleteInvitationCode', 'user_id'],
            selectInvitationCode: ['org/selectInvitationCode', 'user_id'],
            getCodeNumber: ['invite/getCodeNumber', 'code'],
        },
        Dealer: {
            orderList: ['dealer/sale-order-list', 'page'],
            OrderSave: ['dealer/sale-order-save', 'order_id', 'customer_name', 'customer_gender', 'customer_email', 'customer_phone', 'object_id', 'price', 'currency', 'warranty', 'remark'],
        },
        Category: {
            getAllCategoryList: ['category/all-list', 'parent_id'],
            getAdminCategoryList: ['category/admin-category-list', 'parent_id'],
            getCategoryListOfAll: ['category/list-of-all'],
            getAllCategoryListOfBafang: ['category/all-list-of-bafang', 'parent_id'],
            saveCategory: ['category/save', 'name', 'parent_id'],
            editCategory: ['category/edit', 'id', 'name'],
            deleteCategory: ['category/delete', 'id'],
            //分组属性
            getAttrDefList: ['category/attr-def-list', 'category_id'],
            //id 为空或0 是创建，value
            saveAttrDef: ['category/attr-def-save', 'id', 'category_id', 'input_type', 'name', 'value'],
            deleteAttrDef: ['category/attr-def-delete', 'id'],
            //获取 上级分组属性列表
            getUpperAttrDefList: ['category/upper-attr-def-list', 'category_id'],
            //所有分组列表for升级
            queryUpdateComponent: ['category/all-list-for-update'],
            //底部列表
            queryEndCategory: ['category/query', 'name'],
            //搜素料号对应的详情
            selectCategoryPartNo: ['category/select-category-partno', 'partNo'],

        },
        Item: {
            selectPartNo: ['item/selectPartNo', 'part_no'],  // POST
            saveItem: ['item/save', 'id', 'category_id', 'parent_id', 'brand_id', 'name', 'part_no', 'img', 'desc', 'attr_list', 'set_id', 'website', 'hard_param', 'hard_version', 'firmware_version', 'param_version'],
            copyItem: ['item/copy', 'category_id', 'parent_id', 'brand_id', 'name', 'part_no', 'img', 'desc', 'attr_list', 'website', 'hard_param', 'hard_version', 'firmware_version', 'param_version'],
            getItemDetail: ['item/detail', 'id'],
            getListByPage: ['item/list', 'category_id', 'page', 'default', 'po_no'],
            //   category_id hard_param hard_version firmware_version param_version精确查找，po_no 模糊查找，default 是!=0的情况才作为条件执行
            getListForUpdate: ['item/list-for-update', 'category_id', 'page', 'default', 'po_no', 'hard_param', 'hard_version', 'firmware_version', 'param_version'],
            // item_id > item > category_id + item_name 找到所有数据
            // getItemListWithSameName: ['item/list-with-same-name', 'item_id'],
            getSpecList: ['item/spec-list', 'item_id', 'item_set_id'],
            editSpec: ['item/spec-edit', 'id', 'part_no', 'attr_list', 'status', 'hard_param', 'hard_version', 'firmware_version', 'param_version'],
            copySpec: ['item/create-item-by-copy-spec', 'item_id', 'part_no', 'attr_list', 'status', 'hard_param', 'hard_version', 'firmware_version', 'param_version'],
            getListOfBrand: ['item/list-of-brand', 'brand_id', 'page'],
            queryOrderItem: ['item/query', 'target_org_id', 'category_id', 'brand_id', 'query', 'page'],  // is_default 默认ID       set_id  属性类别 1-5 定义不同类别的属性
            deleteItem: ['item/delete', 'id'],
            saveBom: ['item/bom-save', 'id', 'parent_id', 'parent_type', 'child_list', 'img', 'img_item_no', 'spec'],
            getBomList: ['item/bom-list', 'parent_id', 'parent_type'],
            getBomDetail: ['item/bom-detail', 'id'],
            getBomDetailByItem: ['item/bom-detail-by-item', 'parent_id'],
            queryPartItem: ['item/part-query', 'query', 'page'],
            deleteBom: ['item/bom-delete', 'id'],
            queryComponentItem: ['item/component-query', 'category_id', 'query'],
            unbindAttrDef: ['item/attr-def-binding-unbind', 'item_id', 'attr_def_list'],
            saveVideo: ['item/video-save', 'item_id', 'video_list'],
            videoList: ['item/video-list', 'item_id'],
            videoDelete: ['item/video-delete', 'item_id', 'video_id'],
            switchShow: ['item/switch-show', 'item_id'],
            changeCategory: ['item/category-change', 'item_id', 'category_id'],
            changeSpecToOtherItem: ['item/change-spec-to-other-item', 'item_id', 'target_name'],
            //品牌商下面所有子品牌的车型数据
            queryByBrand: ['item/brand-item-model', 'org_id'],
            // 批量设置零部件的更新属性
            changeItemVersion: ['item/item-list-types', 'item_id', 'sn_list', 'status'],
            // 获取物料下面的软件版本信息
            getItemFirmwareFile: ['item/item-list-for-find', 'name'],
        },
        Order: {
            //销售单
            saleOrderSave: ['order/sale-order/save', 'order_id', 'brand_org_id', 'customer_name', 'customer_gender', 'customer_email', 'customer_phone', 'object_sn', 'price', 'currency', 'warranty', 'remark', 'brand_name'],
            getSaleOrderList: ['order/sale-order/list', 'brand_org_id', 'object_sn', 'page'],
            getSaleOrderListOfPeriod: ['order/sale-order/list-of-period', 'org_id', 'object_sn', 'start_time', 'end_time', 'page'],
            getDealerOrderDetail: ['order/sale-order/detail', 'id'],
            deleteSaleOrder: ['order/sale-order/delete', 'id'],
            updateSaleOrderInvoice: ['order/sale-order/invoice-update', 'order_id', 'invoice_no'],
            getCustomerInfo: ['order/sale-order/customer-info', 'email'],
            //生产单
            productionOrderSave: ['order/production-order/save', 'id', 'source_id', 'customer_id', 'customer_part_no', 'customer_po_no', 'user_org_id', 'remark', 'item_list', 'test_plan_id', 'test_plan_name'],
            //组装厂生产
            getProductionOrderList: ['order/production-order/list', 'source_id', 'start_date', 'end_date', 'page', 'model_name'],
            //零件厂生产
            getComponentProductionOrderList: ['order/production-order/list-by-param', 'source_id', 'page', 'customer_name', 'model', 'status'],
            getProductionOrderDetail: ['order/production-order/detail', 'id'],
            deleteProductionOrder: ['order/production-order/delete', 'id'],
            editProductionOrder: ['order/production-order/order-item-update', 'id', 'old_item_id', 'new_item_id'],
            //售后单
            afterSaleOrderSave: ['after-sale-order-save', 'order_id', ' object_id', ],
            //物料追踪
            traceSaleOrderDetail: ['order/sale-order-detail-sn', 'sn'],

        },
        ProgramBrush: {
            // 订单刷程序逻辑
            // 刷完之后保存, 可以重复保存
            saveBrush: ['programBrush/saveProgramBrush', 'category_id', 'category_root_id', 'item_id', 'part_no', 'order_id', 'sn', 'firmware_name', 'firmware_version'],
            //  搜索订单刷程序记录
            selectProgramBrushCount: ['programBrush/selectProgramBrushCount', 'part_no', 'sn', 'order_id'],
            //  搜索订单所有SN刷程序记录
            selectProgramBrushRecord: ['programBrush/selectSnByOrderId', 'order_id']

        },

        Object: {
            objectDetail: ['object/detail', 'object_id'],
            objectDetailBySn: ['object/detail-by-sn', 'sn'],
            vehicleListByOrgAndBrand: ['object/vehicle-list-by-org-and-brand', 'org_id', 'brand_id', 'page'],
            objectVersionCompare: ['attr/attr-compare', 'sn', 'firmware'],
            /*
             ** 1：参数值为空
             * 2：sn查询为空
             * 3：当前版本号（传入的参数）等于录入版本号
             * 4：当前版本号（传入的参数）低于录入版本号
             * 5：当前版本号（传入的参数）高于录入版本号
             * 6: 数据库中版本长度与参数版本长度不同
             * 7: 版本库查询为空
             * 8: item、object无需升级
             * 9: item不升级，object升级，无法升级
             * 10: 版本对比错误
             * 12: firwmare与物料固件版本长度不同
             * 13: 数据库版本低于物料属性
             * 14: 版本库数据隐藏
             */

            // 通过SN和软件版本号去获取固件文件地址 - APP
            objectVersionCompareAndGetFirmware: ['attr/getUpgradeFile', 'sn', 'firmware'],
            // 替换已经绑定车辆上的部件
            objectReplace: ['object/component-replace', 'vehicle_sn', 'component_sn'],
            // 组装厂查询组车过程中保存的零部件---------
            objectAssemblySavedComponent: ['object/assembly-component', 'page', 'size'],
            // 零件商查询组车过程中保存的零部件---------
            objectGetAssemblySavedComponent: ['object/component-all-list', 'sn', 'page', 'size'],
            // 根据SN的ID号，设置零部件的更新属性
            updateType: ['object/status', 'id', 'type'],
            // 从订单，根据物料号去改对应的已经生产的SN的更新属性
            updateOrderType: ['object/editSn', 'order_id', 'item_id', 'type'],

        },
        Vehicle: {
            saveVehicle: ['vehicle/save', 'order_id', 'item_id', 'sn', 'object_list', 'secret_key', 'produce_time'],
            getVehicleListForAssembler: ['vehicle/list-for-assembler_override', 'brand_org_id', 'source_id', 'item_name', 'sn', 'page'],
            // getVehicleListForAssembler: ['vehicle/list-for-assembler', 'brand_org_id', 'source_id', 'item_name', 'sn', 'page'],
            // getVehicleListForAssemblerSn: ['vehicle/list-for-assembler-sn', 'sn', 'page'],
            // getVehicleListForBrand: ['vehicle/list-for-brand', 'org_id', 'item_name', 'brand_id', 'vehicle_sn', 'page'],
            getVehicleListForBrand: ['vehicle/list-for-brand_override', 'org_id', 'item_name', 'brand_id', 'vehicle_sn', 'page'],
            deleteVehicle: ['vehicle/delete', 'id'],
            vehicleDetail: ['vehicle/detail', 'id'],
            vehicleDetailBySn: ['vehicle/detail-by-sn', 'sn', 'user_id'],
            vehicleIsSale: ['vehicle/is-sale', 'sn'],
            getVehicleBomBySn: ['vehicle/bom-by-sn', 'sn'],
            saveVehicleReport: ['vehicle/report-save', 'id', 'vehicle_id', 'report_data', 'defect', 'comment', 'file'],
            getVehicleReportList: ['vehicle/report-list', 'vehicle_id'],
            getReportListOfOrg: ['vehicle/report-list-of-org', 'sn', 'page'],
            getVehicleReportDetail: ['vehicle/report-detail', 'id'],
            editVehicle: ['vehicle/edit', 'id', 'sn', 'secret_key'],
            bomChange: ['vehicle/bom-change', 'vehicle_id', 'old_object_id', 'new_object_sn'],
            getVehicleByBomSn: ['vehicle/bom-by-component-sn', 'sn'],
            snIsExist: ['vehicle/is-exist', 'sn'],
            getReplaceReportList: ['vehicle/replace-report/list', 'vehicle_id', 'page'],
            deleteReport: ['vehicle/report-delete', 'report_id'],
        },
        Component: {
            saveComponent: ['component/save', 'id', 'order_id', 'sn', 'qc', 'test_data', 'produce_time', 'item_id'],
            // getComponentList: ['component/list', 'source_id', 'item_name', 'item_part_no', 'sn', 'customer_id', 'page'],
            getComponentList: ['component/listOverride', 'source_id', 'item_name', 'item_part_no', 'sn', 'customer_id', 'page'],
            exportComponentList: ['component/export-list', 'source_id', 'item_name', 'item_part_no', 'sn', 'customer_id', 'number'],
            exportComponentListOverride: ['component/override_export-list', 'source_id', 'item_name', 'item_part_no', 'sn', 'customer_id', 'number'],
            deleteComponent: ['component/delete', 'id'],
            batchDeleteComponent: ['component/list-delete', 'id_list'],
            componentDetail: ['component/detail', 'id'],
            // 检查Component 是否存在数据库
            componentDetailBySn: ['component/detail-by-sn', 'sn'],
            saveVideo: ['component/video-save', 'id', 'category', 'error_code', 'name', 'file'],
            getVideoList: ['component/video-list', 'page'],
            getVideoListByCategory: ['component/video-list-by-category', 'category'],
            getVideoListByCategoryAndErrorCode: ['component/video-list-by-category-and-code', 'category', 'error_code'],
            deleteVideo: ['component/video-delete', 'video_id'],
            saveSimpleComponent: ['component/simple-save', 'category_id', 'sn'],
        },
        File: {
            imgUpload: ['file/img-upload', 'file'],
            fileUpload: ['file/file-upload', 'file', 'file_id']
        },

        Material: {
            saveMaterial: ['material/material-insert', 'order_id', 'sn', 'user_id', 'allot', 'root_id', 'reminder', 'success', 'desc'],
            updateMaterial: ['material/material-update', 'id', 'order_id', 'sn', 'success', 'num', 'date'],
            findByUserId: ['material/material-user-list', 'page'],
            findByRootId: ['material/material-root-list', 'root_id'],
            findByOrderId: ['material/material-order-no', 'order_id'],
            findBySn: ['material/material-sn', 'sn'],
            findByParam: ['material/material-select', 'status', 'order_id', 'line', 'start_date', 'end_date', 'page'],
            deleteById: ['material/material-delete', 'id'],
            deleteBatch: ['material/material-delete-batch', 'material_list'],
        },


        Ticket: {
            createTicket: ['ticket/create', 'category', 'title', 'description', 'target_id', 'target_sn', 'service_center_org_id', 'img', 'video', 'test_data'],
            createTicketWithOrgId: ['ticket/create-with-org-id', 'category', 'title', 'description', 'target_id', 'target_sn', 'org_id', 'img', 'video', 'test_data'],
            ticketList: ['ticket/list', 'page'],
            ticketDetail: ['ticket/detail', 'ticket_id'],
            deleteTicket: ['ticket/delete', 'ticket_id'],
            saveTicketMessage: ['ticket/ticket-message-save', 'ticket_id', 'message', 'img', 'video'],
            ticketActionList: ['ticket/action-message-list', 'ticket_id'],
            ticketUnassignedList: ['ticket/unassigned-list', 'category', 'page'],
            ticketAssignedList: ['ticket/assigned-list', 'category', 'page'],
            takeUnassignedTicket: ['ticket/ticket-take', 'ticket_id'],
            handleInsideTicket: ['ticket/ticket-handle-inside-update', 'ticket_id', 'handler_id'], // 内部调度
            handleUpTicket: ['ticket/ticket-handle-up-update', 'ticket_id', 'handler_org_id'],

            getTicketListOfAssignedToServiceCenter: ['ticket/list-of-assigned-to-service-center', 'org_id', 'category', 'status', 'page'],
            takeTicket: ['ticket/ticket-take', 'ticket_id'],
            //我在处理的工单
            getMyTicket: ['ticket/list-of-assigned-to-me', 'category', 'status', 'page'],
            // 品牌商子服务中心抛出工单给品牌商
            handleUpTicketByBrandServiceCenter: ['ticket/ticket-handle-up-by-brand-service-center', 'ticket_id'],
            // 品牌商抛出工单给八方
            handleUpTicketByBrand: ['ticket/ticket-handle-up-by-brand', 'org_id', 'ticket_id'],
            //我丢出去的工单
            getTicketOfHandleUp: ['ticket/list-of-mine-handle-up', 'category', 'status', 'page'],

            // 子服务中心抛给品牌商的工单列表
            getTicketListOfHandleUpByServiceCenter: ['ticket/list-of-handle-up-by-service-center', 'category', 'status', 'page'],
            // 完成工单
            completeTicket: ['ticket/complete', 'ticket_id'],
            //操作记录
            getActionList: ['ticket/action-list', 'ticket_id'],
            //分类统计
            getStatOfCategory: ['ticket/stat-of-category'],
            //获取服务中心的总处理单量以及平均时间
            getStatOfServiceCenter: ['ticket/stat-of-service-center', 'org_id'],

        },
        Auth: {
            authList: ['auth/list'],
            saveAuth: ['auth/save', 'user_id', 'auth_list'],
            authDetail: ['auth/detail'],
            authAuth: ['auth/user-auth', 'user_id']
        },
        Error: {
            componentErrorList: ['error/list'],
            componentErrorDetail: ['error/detail', 'code'],
            saveErrorMessage:  ['error/saveComponentError',  'code',  'desc',  'id',  'name',  'remark',  'solution',  'target']
        },
        Firmware: {
            saveFirmware: ['firmware/save', 'id', 'item_id', 'version', 'file', 'remark', 'firmware_name', "check_sum", "type"], // type 1显示(默认)，0不显示
            getLatest: ['firmware/latest', 'item_id'],
            getHistory: ['firmware/record-list', 'item_id'],
            getHistoryBySn: ['firmware/record-list-by-sn', 'sn'],
            delete: ['firmware/delete', 'id'],
        },
        Tool: {
            applyAuth: ['tool/auth-save', 'target_org_id', 'auth_hmi', 'auth_controller'],
            authApplyList: ['tool/auth-list-for-apply'],
            authActive: ['tool/auth-active', 'id'],
            authDetail: ['tool/auth'],
        },
        ObjectRecord: {
            getListBySn: ['object-record/list', 'sn'],
            save: ['object-record/save', 'sn', 'data', 'time', 'type'],
            batchSave: ['object-record/batch-save', 'record_lit'],
            getListForBrand: ['object-record/list-for-brand', 'page'],
            getListForBF: ['object-record/list-for-bafang', 'page'],
            searchList: ['object-record/list-for-search', 'sn', 'data', 'email', 'page', 'size', "type"], //  tpye 2 brand, 1 BF

        },
        Binding: {
            saveBinding: ['binding/connect-binding', 'controller_sn', 'sensor_sn', 'motor_sn', 'orderId'],
            updateBinding: ['binding/connect-update', 'controller_sn', 'sensor_sn', 'motor_sn', 'binding_id'],
            deleteBinding: ['binding/connect-binding-clear', 'id'],
            findBySpecialSn: ['binding/specifiedSn', 'controller_sn', 'sensor_sn', 'motor_sn'],
            findByOrderIdCount: ['binding/connect-binding-count', 'orderId'],
            findRecordList: ['binding/specifiedSn-report-byOrderId', 'order_id', 'controller_sn', 'sensor_sn', 'motor_sn', "page", "size"],
        },
        FileError: {
            getFilesList: ['feedback-center/list', 'query', 'page'],
            getFilesOne: ['feedback-center/one', 'id'],
            saveFileError: ['feedback-center/save', 'email', 'file_name', 'file_id', 'status', 'desc'],
            updateFileError: ['feedback-center/update', 'id', 'email', 'file_id', 'status', 'desc'],
            deleteFileError: ['feedback-center/delete', 'id'],
            findFileInfo: ['feedback-center/findFile', 'id'],
            findFile: ['feedback-center/findFile2', 'id']
        },
        param: {
            saveParam: ['component/param', 'entity']
        },
    };

    function generateApi() {
        let API = this;
        for (let moduleKey in apiList) {

            let moduleApiList = apiList[moduleKey];
            API[moduleKey] = {};

            for (let functionName in moduleApiList) {
                let config = moduleApiList[functionName];

                API[moduleKey][functionName] = ((config) => {
                    return (...args) => {
                        return post(config, ...args)
                    };
                })(config);
            }
        }
    }

    function getUrl(api, data) {
        let token = Core.Data.getToken() || "";
        let url = `${Const.NET.END_POINT}/${api}`;
        let params = {
            token: token,
            client: Const.NET.CLIENT,
            version: Const.NET.VERSION,
        };

        data = Object.assign(params, data);
        url = url + '?' + transformObjectToUrlencodedData(data);
        return url;
    }

    this.getUrl = getUrl;


    function post(api, ...args) {
        let token = Core.Data.getToken() || '';
        let url = `${Const.NET.END_POINT}/${api[0]}`;
        let body = '';

        let params = {
            token: token,
            client: Const.NET.CLIENT,
            version: Const.NET.VERSION,
        };

        let requestArr = {};
        for (let i = 1; i < api.length; i++) {
            let param = args[i - 1];
            param = param === undefined ? "" : param;
            // body += `&${api[i]}=${param}`
            requestArr[api[i]] = param;
        }

        body = Object.assign(params, requestArr);

        // console.log(url + "?" + transformObjectToUrlencodedData(body));

        let headers = {
            'Content-Type': 'application/x-www-form-urlencoded'
        };
        return new Promise((resolve, reject) => {
            Vue.http.post(url, transformObjectToUrlencodedData(body), {
                headers: headers
            }).then(
                response => {
                    let body = response.body;
                    if (response.data.hasOwnProperty('code') && response.data.code === Const.ERROR.ERROR_TOKEN_INVALID) {
                        Core.Data.clearAuthData();
                        router.push({ path: '/login' });
                        return;
                    }
                    if (body.hasOwnProperty('code') && body.code === 0) {
                        if (body.hasOwnProperty('error')) {
                            console.error(body.error);
                            handleError(body.error);
                            return reject(body.error);
                        } else {
                            return resolve(body.data)
                        }
                    }

                    handleError(body.message);
                    return reject(body);
                },
                error => {
                    handleError(error.message);
                    reject(error);
                }
            );
        })
    }

    // function getFile(url) {
    //     Vue.http.get(url).then(
    //         response => {
    //             let body = response.body;
    //             // console.log(body);
    //             return reject(body);
    //         },
    //         error => {
    //             handleError(error.message);
    //             reject(error);
    //         }
    //     );
    // }

    function handleError(message) {
        if (!message) {
            let lan = Core.Data.getLang() || 'en';
            switch(lan){
                case 'en':
                    message = 'Unable to connect to remote service. Please check your network or contact besst@bafang-e.com'
                    break;
                case 'zh':
                    message = '服务器没有相应。请检查网络或者联系besst@bafang-e.com'
                    break;
                case 'nl':
                    message = 'Kan geen verbinding maken met de externe service. Controleer uw netwerk of neem contact op met besst@bafang-e.com'
                    break;
                case 'de':
                    message = 'Der Fernzugriff auf den Server ist nicht erreichbar oder versuchen später noch einmal. Sie können auch Email an besst@bafang-e.com senden.'
                    break;
            }
        }
        Message({
            type: "error",
            message: message,
            duration: 5000
        });
    }

    function transformObjectToUrlencodedData(obj) {
        let p = [];
        for (let key in obj) {
            if (obj.hasOwnProperty(key)) {
                p.push(encodeURIComponent(key) + '=' + encodeURIComponent(obj[key]));
            }
        }
        return p.join('&');
    }

    //new request
    function request(params) {
        const options = params['method'] && params['method'].toLowerCase();
        switch (options) {
            case 'post':
                return postServers(params['url'], params['data'], params['params']);
            case 'put':
                return putServers(params['url'], params['data'], params['params']);
            case 'get':
                return getServers(params['url'], params['data']);
            case 'delete':
                return deleteServers(params['url']);
            default:
                break;
        }
    }


    function getServers(url, params = {}) {
        return new Promise((resolve, reject) => {
            const token = Core.Data.getToken() || "";
            let _params = {
                token: token
            };
            params = Object.assign(params, _params);

            Vue.http.get(
                Const.NET.END_POINT + url, {
                    params: params
                }).then(res => {
                let body = res.body;
                resolve(body);
            }, err => {
                handleError(err.message);
                reject(err);
            });
        });
    }

    function postServers(url, body = {}, params = {}) {
        return new Promise((resolve, reject) => {
            const token = Core.Data.getToken() || "";
            let _params = {
                token: token
            };
            params = Object.assign(params, _params);
            Vue.http.post(
                Const.NET.END_POINT + url,
                body, {
                    params: params
                }
            ).then(res => {
                let body = res.body;
                resolve(body);
            }, err => {
                handleError(err.message);
                reject(err);
            });
        });
    }

    function putServers(url, body = {}, params = {}) {
        return new Promise((resolve, reject) => {
            const token = Core.Data.getToken() || "";
            let _params = {
                token: token
            };
            params = Object.assign(params, _params);
            Vue.http.put(
                Const.NET.END_POINT + url,
                body, {
                    params: params
                }
            ).then(res => {
                let body = res.body;
                resolve(body);
            }, err => {
                handleError(err.message);
                reject(err);
            });
        });
    }

    function deleteServers(url) {
        return new Promise((resolve, reject) => {
            const token = Core.Data.getToken() || "";
            Vue.http.delete(
                Const.NET.END_POINT + url, {
                    params: {
                        token: token
                    }
                }
            ).then(res => {
                let body = res.body;
                resolve(body);
            }, err => {
                handleError(err.message);
                reject(err);
            });
        });
    }

    this.request = request;

    generateApi.call(this);

    return this;
}

export default new Api();



//////////////////
// WEBPACK FOOTER
// ./core/js/api.js
// module id = 635
// module chunks = 73