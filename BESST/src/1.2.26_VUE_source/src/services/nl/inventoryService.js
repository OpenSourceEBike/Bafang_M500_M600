import Core from "core/core";

function find(page, size) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInventory",
        data: {
            page: page || 0,
            size: size || 10
        }
    });
}

function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInventory/all"
    });
}

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInventory/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}

function filterByThreshold(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInventory/filter2",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}


function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInventory/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}




function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlInventory/" + id
    });
}

// 保存产品信息
function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlInventory/save2",
        data: data
    });
}

// 导出

function exportfile(params) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlInventory/export",
        data: params,
        responseType: "blob"
    });
}




const InventoryService = {
    filter: filter,
    find: find,
    findAll: findAll,
    save: save,
    remove: remove,
    exportfile: exportfile,
    filterByThreshold: filterByThreshold
}

export default InventoryService;



// WEBPACK FOOTER //
// ./src/services/nl/inventoryService.js