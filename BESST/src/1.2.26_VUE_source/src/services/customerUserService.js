import Core from "core/core";

function filterUser(currentPage, formSearch) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/customerServiceUser/filter",
        data: {
            employee_no: formSearch.employee_no
        },
        params: {
            page: currentPage,
            size: 10
        }
    });
}

function postUser(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/afterSales/customerServiceUser",
        data: data
    });
}

function deleteUser(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nl/afterSales/customerServiceUser/" + id,
    });
}

const CustomerUserService = {
    filterUser: filterUser,
    postUser: postUser,
    deleteUser: deleteUser
}

export default CustomerUserService;


// WEBPACK FOOTER //
// ./src/services/customerUserService.js