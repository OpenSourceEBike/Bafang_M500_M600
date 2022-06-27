import Core from "core/core";

function getReceipt(id) {
  return Core.HttpHelp.request({
    method: "get",
    url: "/nl/order/receipt/" + id
  });
}

function printReceipt(id) {
  return Core.HttpHelp.request({
    method: "get",
    url: "nl/order/receipt/" + id + "/print"
  });
}

const StockInService = {
  getReceipt: getReceipt,
  printReceipt: printReceipt
}

export default StockInService;



// WEBPACK FOOTER //
// ./src/services/stockInService.js