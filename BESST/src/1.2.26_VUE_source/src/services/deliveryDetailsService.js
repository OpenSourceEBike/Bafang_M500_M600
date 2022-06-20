import Core from "core/core";

function filterDeliveryDetails(currentPage, formSearch, id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/sales/delivery/detail/filter",
        data: {
            delivery_order_id: id,
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function getDeliveryById(id) {
    return Core.HttpHelp.request({
        method: "get",
        url: "/nl/sales/delivery/" + id
    });
}

const DeliveryDetailsService = {
    getDeliveryById: getDeliveryById,
    filterDeliveryDetails: filterDeliveryDetails
}

export default DeliveryDetailsService;


// WEBPACK FOOTER //
// ./src/services/deliveryDetailsService.js