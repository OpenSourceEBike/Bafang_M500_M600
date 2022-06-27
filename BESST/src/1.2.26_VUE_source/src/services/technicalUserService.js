import Core from "core/core";

function filterUser(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceUser/filter",
        data: {
            employee_no: formSearch.employee_no
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function getAllUser() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/afterSales/technicalServiceUser/all"
    });
}

function postUser(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/technicalServiceUser",
        data: data
    });
}

function deleteUser(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/technicalServiceUser/" + id,
    });
}

const TechnicalUserService = {
    filterUser: filterUser,
    postUser: postUser,
    deleteUser: deleteUser,
    getAllUser:getAllUser
}

export default TechnicalUserService;


// WEBPACK FOOTER //
// ./src/services/technicalUserService.js