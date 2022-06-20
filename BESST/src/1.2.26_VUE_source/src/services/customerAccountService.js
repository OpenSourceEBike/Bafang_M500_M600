import Core from "core/core";

function postCompany(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nl/nlPublic/company",
        data: data
    });
}

const CustomerAccountService = {
    postCompany: postCompany
}

export default CustomerAccountService;


// WEBPACK FOOTER //
// ./src/services/customerAccountService.js