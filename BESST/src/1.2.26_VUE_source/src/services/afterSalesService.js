import Core from "core/core";

// 获取售后单列表
function filterServiceCenter(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/serviceCenterOrder/filter",
        data: {
            customer_id: formSearch.customer_id,
            order_no: formSearch.order_no,
            root_category_id: formSearch.root_category_id
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

// 获取售后单详情
async function getServiceCenter(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/serviceCenterOrder/" + id,
    });
}

// 删除售后信息
function deleteServiceCenter(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/serviceCenterOrder/" + id,
    });
}

// 获取全部售后单
async function getAllServiceCenter() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/serviceCenterOrder/all",
    });
}

// 保存售后单信息
function postServiceCenter(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/serviceCenterOrder",
        data: data
    });
}

// 保存售后单明细信息
function bulkServiceCenterDetail(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/serviceCenterOrderDetail/bulk",
        data: data
    });
}

// 查询售后单明细信息
function getServiceCenterDetail(service_center_order_no) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/serviceCenterOrderDetail/filter",
        data: {
            service_center_order_no: service_center_order_no
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

// 查询SN数据
function getDetailBySN(data) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/object/detail-by-sn",
        data: data
    });
}

const AfterSalesService = {
    filterServiceCenter: filterServiceCenter,
    postServiceCenter: postServiceCenter,
    getServiceCenter: getServiceCenter,
    deleteServiceCenter: deleteServiceCenter,
    bulkServiceCenterDetail: bulkServiceCenterDetail,
    getServiceCenterDetail: getServiceCenterDetail,
    getAllServiceCenter: getAllServiceCenter,
    getDetailBySN: getDetailBySN
}

export default AfterSalesService;


// WEBPACK FOOTER //
// ./src/services/afterSalesService.js