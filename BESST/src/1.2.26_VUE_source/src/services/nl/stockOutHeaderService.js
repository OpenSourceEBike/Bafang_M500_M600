import Core from "core/core";

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStockHeader/filter2",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}

function findOne(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlOutStockHeader/" + id,
    });
}


function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlOutStockHeader/" + id
    });
}

function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlOutStockHeader/save2",
        data: data
    });
}

function approved(id) {
    return Core.HttpHelp.request({
        method: "PUT",
        url: "/NlOutStockHeader/" + id + "/approved",
    });
}


const StockOutHeaderService = {
    filter: filter,
    save: save,
    remove: remove,
    findOne: findOne,
    approved: approved
}

export default StockOutHeaderService;



// WEBPACK FOOTER //
// ./src/services/nl/stockOutHeaderService.js