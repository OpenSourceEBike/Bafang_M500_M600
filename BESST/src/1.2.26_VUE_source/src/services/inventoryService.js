import Core from "core/core";

function uploadBitchInventory(data, params) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/warehouse/inventory/upload-bitch-inventory",
        data: data,
        params: params
    });
}



const InventoryService = {
    uploadBitchInventory: uploadBitchInventory,
}

export default InventoryService;



// WEBPACK FOOTER //
// ./src/services/inventoryService.js