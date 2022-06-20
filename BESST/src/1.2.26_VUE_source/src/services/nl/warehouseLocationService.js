import Core from "core/core";

function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlWarehouseLocation/all"
    });
}

function locationFilter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouseLocation/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}
// 新增
function addOneLocation(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouseLocation",
        data: data
    });
}

// 删除
function deleteOneLocation(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlWarehouseLocation/" + id,
    });
}


const WarehouseLocationService = {
    findAll: findAll,
    addOneLocation: addOneLocation,
    deleteOneLocation: deleteOneLocation,
    locationFilter: locationFilter
}

export default WarehouseLocationService;



// WEBPACK FOOTER //
// ./src/services/nl/warehouseLocationService.js