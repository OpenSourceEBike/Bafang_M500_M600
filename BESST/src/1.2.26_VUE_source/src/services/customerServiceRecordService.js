import Core from "core/core";

// 获取客户服务记录列表
function filterCustomerServiceRecord(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/customerServiceRecord/filter",
        data: {
            customer_id: formSearch.customer_id,
            service_no: formSearch.service_no
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

// 获取客户服务记录详情
function getCustomerServiceRecord(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/customerServiceRecord/" + id,
    });
}

// 删除客户服务记录信息
function deleteCustomerServiceRecord(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/customerServiceRecord/" + id,
    });
}

// 保存客户服务记录单信息
function postCustomerServiceRecord(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/customerServiceRecord",
        data: data
    });
}


// 修改客户服务记录单状态
function changeCSRecordStatus(id, status) {
    return Core.HttpHelp.request({
        method: "put",
        url: "/nl/afterSales/customerServiceRecord/" + id + "/status",
        params: {
            status: status
        }
    });
}


const CustomerServiceRecordService = {
    filterCustomerServiceRecord: filterCustomerServiceRecord,
    postCustomerServiceRecord: postCustomerServiceRecord,
    getCustomerServiceRecord: getCustomerServiceRecord,
    deleteCustomerServiceRecord: deleteCustomerServiceRecord,
    changeCSRecordStatus: changeCSRecordStatus
}

export default CustomerServiceRecordService;


// WEBPACK FOOTER //
// ./src/services/customerServiceRecordService.js