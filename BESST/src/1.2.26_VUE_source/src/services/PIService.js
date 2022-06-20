import Core from "core/core";

// 获取顾客PI单
function filterCustomerPI(customer_id, status = "") {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/filter",
        data: {
            customer_id: customer_id,
            status: status
        },
        params: {
            page: 1,
            size: 999
        }
    })
}

// 获取顾客PI明细列表
function filterCustomerPIDetail(pi_no) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/detail/filter",
        data: {
            pi_no: pi_no
        },
        params: {
            page: 1,
            size: 999
        }
    })
}

function printPI(id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/print_PI",
        params: {
            id: id
        },
        responseType: 'blob'
    })
}



const PIService = {
    filterCustomerPI: filterCustomerPI,
    filterCustomerPIDetail: filterCustomerPIDetail,
    printPI: printPI

}

export default PIService;


// WEBPACK FOOTER //
// ./src/services/PIService.js