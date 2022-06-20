import Core from "core/core";

function find(page, size) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlProduct",
        data: {
            page: page || 0,
            size: size || 10
        }
    });
}

async function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlProduct/findNlProductByOrgId"
    });
}

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlProduct/filter2",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}



function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/NlProduct/" + id
    });
}

// 保存产品信息
function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlProduct/save2",
        data: data
    });
}


// 保存产品信息
function importfile(data, override = false) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/NlProduct/importProduct",
        data: data,
        params: {
            override: override
        }
    });
}


// 下载模板
function exportTemplate() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/NlProduct/exportTemplate",
        responseType: "blob"
    });
}


const ProductService = {
    filter: filter,
    find: find,
    findAll: findAll,
    save: save,
    remove: remove,
    importfile: importfile,
    exportTemplate: exportTemplate
}

export default ProductService;



// WEBPACK FOOTER //
// ./src/services/nl/productService.js