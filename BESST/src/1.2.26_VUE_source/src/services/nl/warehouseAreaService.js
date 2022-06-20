import Core from "core/core";

async function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlWarehouseArea/all"
    });
}

function areaFilter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouseArea/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}
// 新增
function addOneArea(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlWarehouseArea",
        data: data
    });
}

// 删除
function deleteOneArea(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlWarehouseArea/" + id,
    });
}


const WarehouseAreaService = {
    findAll: findAll,
    addOneArea: addOneArea,
    deleteOneArea: deleteOneArea,
    areaFilter: areaFilter
}

export default WarehouseAreaService;



// WEBPACK FOOTER //
// ./src/services/nl/warehouseAreaService.js