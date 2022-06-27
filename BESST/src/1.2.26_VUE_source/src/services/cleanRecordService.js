import Core from "core/core";

function filterCleanRecord(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/clean/cleanRecord/filter",
        data: {
            ci_no: formSearch.ci_no
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function postCleanRecord(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/clean/cleanRecord",
        data: data
    });
}

function getCleanRecordId(id) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/clean/cleanRecord/" + id,
    });
}

function deleteCleanRecord(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/clean/cleanRecord/" + id,
    });
}

function statusCleanRecord(id, status) {
    return Core.HttpHelp.request({
        method: "PUT",
        url: "/nl/clean/cleanRecord/" + id + "/status",
        params: {
            status: status
        }
    });
}

const CleanRecordService = {
    filterCleanRecord: filterCleanRecord,
    postCleanRecord: postCleanRecord,
    deleteCleanRecord: deleteCleanRecord,
    getCleanRecordId: getCleanRecordId,
    statusCleanRecord: statusCleanRecord
}

export default CleanRecordService;


// WEBPACK FOOTER //
// ./src/services/cleanRecordService.js