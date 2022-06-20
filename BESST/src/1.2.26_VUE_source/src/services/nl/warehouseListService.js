import Core from "core/core";

function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlWarehouse/findAll"
    });
}

function listFilter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouse/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}
// 新增
function addOneWarehouse(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouse",
        data: data
    });
}

// 删除
function deleteOneWarehouse(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlWarehouse/" + id,
    });
}

// 开关仓库

function sSwitch(id) {
    return Core.HttpHelp.request({
        method: "get",
        url: "/NlWarehouse/" + id + "/sswitch"
    });
}


const WarehouseListService = {
    findAll: findAll,
    addOneWarehouse: addOneWarehouse,
    deleteOneWarehouse: deleteOneWarehouse,
    listFilter: listFilter,
    sSwitch: sSwitch
}

export default WarehouseListService;



// WEBPACK FOOTER //
// ./src/services/nl/warehouseListService.js