import Core from "core/core";

function getShipment(id) {
    return Core.HttpHelp.request({
        method: "get",
        url: "/nl/order/shipment/" + id
    });
}

const StockOutService = {
    getShipment: getShipment
}

export default StockOutService;


// WEBPACK FOOTER //
// ./src/services/stockOutService.js