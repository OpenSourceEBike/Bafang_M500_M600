import Core from "core/core";

function filterOrderSchedule(delivery_id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/nlPublic/orderSchedule/filter",
        data: {
            delivery_id: delivery_id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}


function filterAfterSalesStatus(service_id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/nlPublic/AfterSalesStatus/filter",
        data: {
            service_id: service_id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

function filterPurchaseStatus(purchase_id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/nlPublic/orderSchedule/filter",
        data: {
            purchase_id: purchase_id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}


const OrderScheduleService = {
    filterOrderSchedule: filterOrderSchedule,
    filterAfterSalesStatus: filterAfterSalesStatus,
    filterPurchaseStatus:filterPurchaseStatus
}

export default OrderScheduleService;


// WEBPACK FOOTER //
// ./src/services/orderScheduleService.js