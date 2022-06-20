import Core from "core/core";

function getAllRegion() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/warehouse/region/all"
    });
}

async function getAllCustomer() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
    });
}

function getAllCategory() {
    return Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
    )
}

async function getAllDelivery() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/sales/delivery/all"
    });
}

async function getAllArea() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl//warehouse/area/all"
    });
}

// 获取库区类型
async function getAllWarehouseType() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/warehouse/type/all"
    });
}

function getAllPermission() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/public/permission/permissioned"
    })
}


const WarehouseService = {
    getAllRegion: getAllRegion,
    getAllCustomer: getAllCustomer,
    getAllCategory: getAllCategory,
    getAllDelivery: getAllDelivery,
    getAllPermission: getAllPermission,
    getAllWarehouseType: getAllWarehouseType,
    getAllArea: getAllArea
}

export default WarehouseService;



// WEBPACK FOOTER //
// ./src/services/warehouseService.js