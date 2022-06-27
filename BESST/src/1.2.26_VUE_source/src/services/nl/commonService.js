import Core from "core/core";


// 获取所有产品分类
async function getAllCategory() {
    return Core.Api.Category.getAllCategoryListOfBafang(
        Core.Const.COMPONENT_ROOT_CATEGORY
    )
}

// 获取所有客户信息
async function getAllCustomer() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nl/nlPublic/company/all"
    });
}


const CommonService = {
    getAllCategory: getAllCategory,
    getAllCustomer: getAllCustomer
}
export default CommonService;



// WEBPACK FOOTER //
// ./src/services/nl/commonService.js