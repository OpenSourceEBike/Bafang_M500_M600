/**
 * @author whis admin@wwhis.com
 * @Created 4/25/17
 */
import Util from 'core/js/util'
import Const from 'core/js/const'

function timeFormat(timestamp, format) {
    if (!format) {
        format = 'Y-m-d H:i:s';
    }
    if (!timestamp) {
        return ''
    }
    return Util.date(format, timestamp);
}

function dateFormat(timestamp, format) {
    if (!format) {
        format = 'Y-m-d';
    }
    if (timestamp instanceof Number) {
        if (timestamp.toString().length == 13) {
            timestamp = parseInt(timestamp / 1000);
        }
    } else if (timestamp instanceof String) {
        if (timestamp.length == 13) {
            timestamp = parseInt(timestamp / 1000);
        }
    }


    if (timestamp == 0) {
        return '/'
    }
    return Util.date(format, timestamp);
}

function currencyFormat(money, type) {
    money = parseInt(money);
    if (money % 100 === 0 && !type) {
        return money / 100;
    }
    return Util.sprintf('%.2f', parseInt(money) / 100);
}

function getImgUrl(name) {
    return Const.NET.IMG_URL_PREFIX + name;
}

function getImgUrlWithExt(name, ext) {
    return Const.NET.IMG_URL_PREFIX + name + "." + ext;
}

function getFileUrl(name, ext) {
    if (!name) {
        return;
    }
    if (!ext) {
        return Const.NET.FILE_URL_PREFIX + name;
    } else {
        return Const.NET.FILE_URL_PREFIX + name + "." + ext;
    }
}

function orgType(type) {
    return Const.ORG_CATEGORY_TYPE[parseInt(type)]
}

function orderStatus(status, lang) {
    switch (lang) {
        case "en":
            return Const.ORDER_STATUS.en[parseInt(status)];
            break;
        case "zh":
            return Const.ORDER_STATUS.zh[parseInt(status)];
            break;
        case "de":
            return Const.ORDER_STATUS.de[parseInt(status)];
            break;
        case "nl":
            return Const.ORDER_STATUS.nl[parseInt(status)];
            break;
    }
}

function fileStatus(status) {
    return Const.FILE_STATUS[parseInt(status)];
}

function currencyType(type) {
    return Const.CURRENCY_TYPE[parseInt(type)]
}

function inputType(type) {
    return Const.CATEGORY_ATTR_INPUT_TYPE[parseInt(type)]
}

function ticketStatus(type, lang) {
    switch (lang) {
        case "en":
            return Const.TICKET_TYPE.en[parseInt(type)];
            break;
        case "zh":
            return Const.TICKET_TYPE.zh[parseInt(type)];
            break;
        case "de":
            return Const.TICKET_TYPE.de[parseInt(type)];
            break;
        case "nl":
            return Const.TICKET_TYPE.nl[parseInt(type)];
            break;
    }
}

function ticketCategoryType(type) {
    return Const.TICKET_CATEGORY_TYPE_LIST[parseInt(type)]
}

function categoryRoot(root_id) {
    return Const.CATEGORY_ROOT[parseInt(root_id)]
}

function testErrorCode(code) {
    return Const.TEST_ERROR_CODE[parseInt(code)].error

}

function packUnitType(type) {
    return Const.PACK_UNIT[parseInt(type)]
}

function videoCategoryType(type) {
    return Const.VIDEO_CATEGORY_TYPE[parseInt(type)]
}

function materialStatus(status, lang) {
    switch (lang) {
        case "en":
            return Const.MATERIAL_STATUS.en[status];
            break;
        case "zh":
            return Const.MATERIAL_STATUS.zh[status];
            break;
        default:
            return Const.MATERIAL_STATUS.en[status];
            break;
    }
}

function statusPipe(status, statusList) {
    if (statusList && statusList.length > 0) {
        return statusList[status] ? statusList[status] : '-';
    } else {
        return 'error:check date'
    }
}

function sourceFormFilter(status) {
    const map = {
        0: "Order",
        1: "Repair",
    };
    return map[status];
}


function purposeFilter(status) {
    const map = {
        0: "Repair",
        1: "Send",
    };
    return map[status];
}


const _filters = {
    timeFormat: timeFormat,
    dateFormat: dateFormat,
    currencyFormat: currencyFormat,
    getImgUrl: getImgUrl,
    getFileUrl: getFileUrl,
    orgType: orgType,
    orderStatus: orderStatus,
    currencyType: currencyType,
    inputType: inputType,
    getImgUrlWithExt: getImgUrlWithExt,
    ticketStatus: ticketStatus,
    ticketCategoryType: ticketCategoryType,
    categoryRoot: categoryRoot,
    testErrorCode: testErrorCode,
    packUnitType: packUnitType,
    videoCategoryType: videoCategoryType,
    fileStatus: fileStatus,
    materialStatus: materialStatus,
    statusPipe: statusPipe,
    sourceFormFilter: sourceFormFilter,
    purposeFilter: purposeFilter
};

export default (Vue) => {
    Object.keys(_filters).forEach(key => {
        Vue.filter(key, _filters[key])
    })
}


//////////////////
// WEBPACK FOOTER
// ./core/filters/index.js
// module id = 2074
// module chunks = 73