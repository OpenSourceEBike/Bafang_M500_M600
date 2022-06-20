import Core from "core/core";

function filterCarrier(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/order/carrier/filter",
        data: {
            logistics_order_no: formSearch.logistics_order_no
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function postCarrier(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/order/carrier",
        data: data
    });
}


function deleteCarrier(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/order/carrier/" + id,
    });
}


const CarrierService = {
    filterCarrier: filterCarrier,
    postCarrier: postCarrier,
    deleteCarrier: deleteCarrier,
}

export default CarrierService;


// WEBPACK FOOTER //
// ./src/services/carrierService.js