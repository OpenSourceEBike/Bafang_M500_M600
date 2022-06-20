import Core from "core/core";


// 获取全部stock数据
function allStock(params, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/stockHistoryItem/find",
        params: {
            productCode: params.product_code,
            warehouseCode: params.warehouse_code,
            page: page || 0,
            size: size || 10
        }
    });
}


function find(page, size) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInStock",
        data: {
            page: page || 0,
            size: size || 10
        }
    });
}

function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInStock/all"
    });
}

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStock/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}

function filterAll(id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStock/filterAll",
        data: {
            nl_in_stock_header_id: id
        }
    });
}



function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlInStock/" + id
    });
}

// 保存产品信息
function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStock",
        data: data
    });
}

// 打印

function print(data, params) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStock/printGrnPdf",
        data: data,
        params: params
    });
}

function saveHeader(data) {
    return Core.HttpHelp.request({
        header: {
            'Content-Type': undefined
        },
        method: "POST",
        url: "/NlInStock/save2",
        data: data
    });
}

// 导出

function exportfile(params) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInStock/exportNlInStock",
        data: params,
        responseType: "blob"
    });
}



const StockInService = {
    filter: filter,
    find: find,
    findAll: findAll,
    save: save,
    remove: remove,
    allStock: allStock,
    print: print,
    filterAll: filterAll,
    saveHeader: saveHeader,
    exportfile: exportfile
}

export default StockInService;



// WEBPACK FOOTER //
// ./src/services/nl/stockInService.js