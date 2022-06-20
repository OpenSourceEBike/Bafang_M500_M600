import Core from "core/core";

function postPIDetailsBulk(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/sales/proformaInvoice/detail/bulk",
        data: data
    });
}

const PIDetailsService = {
    postPIDetailsBulk: postPIDetailsBulk
}

export default PIDetailsService;


// WEBPACK FOOTER //
// ./src/services/proformaInvoiceDetailService.js