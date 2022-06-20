import Core from "core/core";


function filterWarehouseAreaType(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "post",
        url: "/nl/warehouse/type/filter",
        data: {
            type_code: formSearch.type_code,
            type_name: formSearch.type_name
        },
        params: {
            size: 10,
            page: currentPage
        }
    });
}

function postWarehouseAreaType(data) {
    return Core.HttpHelp.request({
        method: "post",
        url: "/nl/warehouse/type",
        data: data
    });
}


function deleteWarehouseAreaType(id) {
    return Core.HttpHelp.request({
        method: "delete",
        url: "/nl/warehouse/type/" + id
    });
}



const WarehouseAreaTypeService = {
    filterWarehouseAreaType: filterWarehouseAreaType,
    postWarehouseAreaType: postWarehouseAreaType,
    deleteWarehouseAreaType: deleteWarehouseAreaType,
}

export default WarehouseAreaTypeService;


// WEBPACK FOOTER //
// ./src/services/warehouseAreaTypeService.js