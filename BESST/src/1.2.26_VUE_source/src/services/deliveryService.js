import Core from "core/core";

function getDeliveryPIDetails(id) {
    return Core.Api.request({
        method: "get",
        url: "/nl/sales/delivery/" + id + "/pi/detail"
    });
}


function printDelivery(id) {
    return Core.HttpHelp.request({
        method: "Get",
        url: "/nl/sales/delivery/" + id + "/print",
        responseType: 'blob'
    })
}

const DeliveryService = {
    getDeliveryPIDetails: getDeliveryPIDetails,
    printDelivery: printDelivery
}

export default DeliveryService;


// WEBPACK FOOTER //
// ./src/services/deliveryService.js