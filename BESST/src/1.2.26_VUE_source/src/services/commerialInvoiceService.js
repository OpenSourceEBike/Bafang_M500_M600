import Core from "core/core";

function filterCommercialInvoice(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/clean/commercialInvoice/filter",
        data: {
            ci_no: formSearch.ci_no,
            date: formSearch.date,
            customer_id: formSearch.customer_id
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function postCommercialInvoice(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/clean/commercialInvoice",
        data: data
    });
}

function getCommercialInvoiceId(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/clean/commercialInvoice/" + id,
    });
}

function deleteCommercialInvoice(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/clean/commercialInvoice/" + id,
    });
}

function putReceiptStatus(id, status) {
    return Core.HttpHelp.request({
        method: "PUT",
        url: "/nl/clean/commercialInvoice/" + id + "/status",
        params: {
            status: status
        }
    });
}

const CommercialInvoiceService = {
    filterCommercialInvoice: filterCommercialInvoice,
    postCommercialInvoice: postCommercialInvoice,
    deleteCommercialInvoice: deleteCommercialInvoice,
    getCommercialInvoiceId: getCommercialInvoiceId,
    putReceiptStatus: putReceiptStatus
}

export default CommercialInvoiceService;



// WEBPACK FOOTER //
// ./src/services/commerialInvoiceService.js