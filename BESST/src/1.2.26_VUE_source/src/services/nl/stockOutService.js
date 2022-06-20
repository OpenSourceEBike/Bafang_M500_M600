import Core from "core/core";

function stockOutFilter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStock/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}
// 新增
function addStockOut(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStock",
        data: data
    });
}

// 删除
function delOneStockOut(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlOutStock/" + id,
    });
}
// 打印

function print(data, params) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStock/printOutboundOrderPdf",
        data: data,
        params: params
    });
}


function filterAll(id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStock/filterAll",
        data: {
            nl_out_stock_header_id: id
        }
    });
}

function saveHeader(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStock/save2",
        data: data
    });
}


// 导出

function exportfile(params) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlOutStock/exportNlOutStock",
        data: params,
        responseType: "blob"
    });
}


const StockOutService = {
    addStockOut: addStockOut,
    delOneStockOut: delOneStockOut,
    stockOutFilter: stockOutFilter,
    print: print,
    filterAll: filterAll,
    saveHeader: saveHeader,
    exportfile: exportfile
}

export default StockOutService;



// WEBPACK FOOTER //
// ./src/services/nl/stockOutService.js