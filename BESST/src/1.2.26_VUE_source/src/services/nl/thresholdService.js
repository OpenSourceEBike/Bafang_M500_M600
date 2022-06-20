import Core from "core/core";


function find(page, size) {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nlIntermediate",
        data: {
            page: page || 0,
            size: size || 10
        }
    });
}

function findAll() {
    return Core.HttpHelp.request({
        method: "GET",
        url: "/nlIntermediate/all"
    });
}

function filter(data, page, size) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nlIntermediate/filter",
        data: data,
        params: {
            page: page || 0,
            size: size || 10
        }
    });
}

function filterAll(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nlIntermediate/filterAll",
        data: data
    });
}



function remove(id) {
    return Core.HttpHelp.request({
        method: "DELETE",
        url: "/nlIntermediate/" + id
    });
}

// 保存产品阈值信息
function save(data) {
    return Core.HttpHelp.request({
        method: "POST",
        url: "/nlIntermediate",
        data: data
    });
}



const ThresholdService = {
    filter: filter,
    find: find,
    findAll: findAll,
    save: save,
    remove: remove,
    filterAll: filterAll,
}

export default ThresholdService;



// WEBPACK FOOTER //
// ./src/services/nl/thresholdService.js