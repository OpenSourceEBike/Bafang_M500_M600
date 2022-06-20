import Core from "../core"
import Const from "./const"
import axios from 'axios';

let http = axios.create({
    baseURL: Const.NET.END_POINT,
    withCredentials: true,
    headers: {
        "Content-Type": "application/json;"
    }
});


function request(params) {
    const options = params['method'] && params['method'].toLowerCase();
    switch (options) {
        case 'post':
            return postServers(params['method'], params['url'], params['data'], params['params'], params['responseType']);
        case 'put':
            return putServers(params['method'], params['url'], params['data'], params['params']);
        case 'get':
            return getServers(params['headers'], params['method'], params['url'], params['data'], params['responseType']);
        case 'delete':
            return deleteServers(params['method'], params['url']);
        default:
            break;
    }
}



function getServers(headers = {}, method, url, params = {}, responseType = '') {
    const token = Core.Data.getToken() || "";
    let _params = {
        token: token
    };
    params = Object.assign(params, _params);

    return http({
        headers: headers,
        method: method,
        url: url,
        params: params,
        responseType: responseType
    });
}

function postServers(method, url, body = {}, params = {}, responseType = '') {
    const token = Core.Data.getToken() || "";
    let _params = {
        token: token
    };
    params = Object.assign(params, _params);

    return http({
        method: method,
        url: url,
        data: body,
        params: params,
        responseType: responseType
    });
}

function putServers(method, url, data = {}, params = {}) {
    const token = Core.Data.getToken() || "";
    let _params = {
        token: token
    };
    params = Object.assign(params, _params);
    return http({
        method: method,
        url: url,
        data: data,
        params: params
    });
}

function deleteServers(method, url) {
    const token = Core.Data.getToken() || "";
    return http({
        method: method,
        url: url,
        params: {
            token: token
        }
    });
}

const HttpHelp = {
    request: request
}
export default HttpHelp;



//////////////////
// WEBPACK FOOTER
// ./core/js/httpHelp.js
// module id = 698
// module chunks = 73