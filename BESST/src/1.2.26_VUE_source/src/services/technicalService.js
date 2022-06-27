import Core from "core/core";

function filterTechnical(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceRecord/filter",
        data: {
            order_no: formSearch.order_no,
            customer_id: formSearch.customer_id
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function getTechnical(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/technicalServiceRecord/" + id,
    });
}

function postTechnical(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceRecord",
        data: data
    });
}

function deleteTechnical(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/technicalServiceRecord/" + id,
    });
}

// 检修单状态审核
function putTechnicalStatus(id,status) {
    return Core.HttpHelp.request({
        method: "put",
        url: "/nl/afterSales/technicalServiceRecord/" + id + "/status",
        params: {
            status: status
        }
    });
}



function filterTechnicalServiceAllotById(service_id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceAllot/filter",
        data: {
            service_id: service_id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

function bulkTechnicalServiceAllot(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceAllot/bulk2",
        data: data
    });
}

function saveTechnicalServiceAllot(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceAllot",
        data: data
    });
}





const TechnicalService = {
    filterTechnical: filterTechnical,
    getTechnical: getTechnical,
    postTechnical: postTechnical,
    deleteTechnical: deleteTechnical,
    putTechnicalStatus: putTechnicalStatus,
    filterTechnicalServiceAllotById: filterTechnicalServiceAllotById,
    bulkTechnicalServiceAllot: bulkTechnicalServiceAllot,
    saveTechnicalServiceAllot: saveTechnicalServiceAllot
}

export default TechnicalService;


// WEBPACK FOOTER //
// ./src/services/technicalService.js