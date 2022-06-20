import Core from "core/core";

function getCleanRecord(id) {
    return Core.HttpHelp.request({
        method: "get",
        url: "/nl/clean/cleanRecord/" + id
    });
}
const CleanService = {
    getCleanRecord: getCleanRecord
}

export default CleanService;


// WEBPACK FOOTER //
// ./src/services/cleanService.js