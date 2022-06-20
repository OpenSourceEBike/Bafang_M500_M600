import Core from "core/core";


function filterStorageCache(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "post",
        url: "/nl/warehouse/StorageCache/filter",
        data: {
            warehouse_region_id: formSearch.warehouse_region_id,
            warehouse_id: formSearch.warehouse_id,
            area_id: formSearch.area_id,
            parcel_no: formSearch.parcel_no
        },
        params: {
            size: 10,
            page: currentPage
        }
    });
}

function postStorageCache(data) {
    return Core.HttpHelp.request({
        method: "post",
        url: "/nl/warehouse/StorageCache",
        data: data
    });
}


function deleteStorageCache(id) {
    return Core.HttpHelp.request({
        method: "delete",
        url: "/nl/warehouse/StorageCache/" + id
    });
}



const StorageCacheService = {
    filterStorageCache: filterStorageCache,
    postStorageCache: postStorageCache,
    deleteStorageCache: deleteStorageCache,
}

export default StorageCacheService;


// WEBPACK FOOTER //
// ./src/services/storageCacheService.js