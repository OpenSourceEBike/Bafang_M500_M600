import Core from "core/core";

function getInvoice(id) {
    return Core.HttpHelp.request({
      method: "get",
      url: "/nl/finance/invoice/" + id
    });
}

const InvoiceService = {
    getInvoice: getInvoice
}

export default InvoiceService;


// WEBPACK FOOTER //
// ./src/services/invoiceService.js