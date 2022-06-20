import Core from "core/core";

function uploadBitchInventory(data, params) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/warehouse/location/upload-bitch-location",
        data: data,
        params: params
    });
}



const LocationService = {
    uploadBitchInventory: uploadBitchInventory,
}

export default LocationService;



// WEBPACK FOOTER //
// ./src/services/locationService.js