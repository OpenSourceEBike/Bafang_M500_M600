import Core from "core/core";

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStockHeader/filter2",
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
        url: "/NlInStockHeader/" + id,
    });
}


function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlInStockHeader/" + id
    });
}

function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInStockHeader/save2",
        data: data
    });
}

function approved(id) {
    return Core.HttpHelp.request({
        method: "PUT",
        url: "/NlInStockHeader/" + id + "/approved",
    });
}


const stockInHeaderService = {
    filter: filter,
    save: save,
    remove: remove,
    findOne: findOne,
    approved: approved
}

export default stockInHeaderService;



// WEBPACK FOOTER //
// ./src/services/nl/stockInHeaderService.js