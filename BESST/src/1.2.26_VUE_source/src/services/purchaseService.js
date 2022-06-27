import Core from "core/core";

//获取采购单信息
function getPurchaseByid(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/order/purchase/" + id
    });
}

//获取供货商
async function getAllSupplier() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/nlPublic/supplier/all"
    });
}


//获取采购单信息并且采购单为可用状态(status:Approved(1))
function getPurchase(orderNo) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/order/purchase/filter",
        data: {
            purchase_order_no: orderNo,
            status: 1
        }
    });
}
//获取采购单的详细item信息
function filterAllPurchaseDetails(id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/order/purchase/detail/filter",
        data: {
            purchase_order_id: id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

function printPurchase(id) {
    return Core.HttpHelp.request({
        method: "Get",
        url: "/nl/order/purchase/" + id + "/print",
        responseType: 'blob'
    })
}

const PurchaseService = {
    getAllSupplier: getAllSupplier,
    getPurchase: getPurchase,
    filterAllPurchaseDetails: filterAllPurchaseDetails,
    getPurchaseByid: getPurchaseByid,
    printPurchase: printPurchase
}

export default PurchaseService;


// WEBPACK FOOTER //
// ./src/services/purchaseService.js