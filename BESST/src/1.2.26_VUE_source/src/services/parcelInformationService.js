import Core from "core/core";

function filterParcelInfo(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/parcelInformation/filter",
        data: {
            parcel_no: formSearch.parcel_no,
            status: formSearch.status
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

async function filterParcelInfoByStatus(status) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/parcelInformation/filter",
        data: {
            status: status
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

function postParcelInfo(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/parcelInformation",
        data: data
    });
}

function getParcelInfo(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/parcelInformation/" + id,
    });
}

function deleteParcelInfo(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/parcelInformation/" + id,
    });
}

// 更改包裹状态
function putParcelInfoStatus(id, status) {
    return Core.HttpHelp.request({
        method: "put",
        url: "/nl/afterSales/parcelInformation/" + id + "/status",
        params: {
            status: status
        }
    });
}

// 保存包裹明细
function bulkParcelInfoDetail(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/parcelInformationDetail/bulk2",
        data: data
    });
}



//获取全部包裹明细
function getParcelInfoDetail(parcel_id) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/parcelInformationDetail/filter",
        data: {
            parcel_id: parcel_id
        },
        params: {
            page: 1,
            size: 999
        }
    });
}

const ParcelInformationService = {
    filterParcelInfo: filterParcelInfo,
    postParcelInfo: postParcelInfo,
    deleteParcelInfo: deleteParcelInfo,
    getParcelInfo: getParcelInfo,
    getParcelInfoDetail: getParcelInfoDetail,
    bulkParcelInfoDetail: bulkParcelInfoDetail,
    putParcelInfoStatus: putParcelInfoStatus,
    filterParcelInfoByStatus: filterParcelInfoByStatus
}

export default ParcelInformationService;


// WEBPACK FOOTER //
// ./src/services/parcelInformationService.js