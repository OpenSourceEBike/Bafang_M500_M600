import Const from './const'
import Util from './util'

function Data() {
    return {
        set,
        get,
        remove,
        getKey,
        setToken,
        getToken,
        setUser,
        getUser,
        setOrg,
        getOrg,
        setBrandPartner,
        getBrandPartner,
        setRoleEmailType,
        getRoleEmailType,
        clearAuthData,
        getTestError,
        setLang,
        getLang,
        setAuth,
        getAuth,
        setUpdateCount,
        getUpdateCount,
        setOffline,
        getOffline,
        setPort,
        getPort,
        setRecordList,
        getRecordList,
        setSavedRecordIndexList,
        getSavedRecordIndexList,
        setIsBatchRecordIng,
        getIsBatchRecordIng,
        setDiagnosisTest,
        getDiagnosisTest,
        setDiagnosisConnection,
        getDiagnosisConnection,
        deepClone,
        getFormatterDate,
        getWarrantyTime,
        setSpecialAccount,
        getSpecialAccount,

    };


    function getKey(key) {
        return Const.DATA.KEY_PREFIX + key;
    }

    function get(key) {
        if (key) {
            key = getKey(key);
            return JSON.parse(window.localStorage.getItem(key));
        }
    }

    function set(key, val) {
        if (key) {
            key = getKey(key);
            return window.localStorage.setItem(key, JSON.stringify(val));
        }
    }

    function remove(key) {
        if (key) {
            key = getKey(key);
            return window.localStorage.removeItem(key);
        }
    }

    function setToken(token) {
        if (token) {
            let key = getKey(Const.DATA.KEY_TOKEN);
            return window.localStorage.setItem(key, token);
        }
    }

    function getToken() {
        let key = getKey(Const.DATA.KEY_TOKEN);
        return window.localStorage.getItem(key);
    }

    function setUser(user) {
        let userTem = {
            id: user.id,
            email: user.email,
            ip_type: user.ip_type,
            ip: user.ip,
            status: user.status,
            audit_type: user.audit_type,
        }
        return set(Const.DATA.KEY_USER, userTem);
    }

    function getUser() {
        return get(Const.DATA.KEY_USER);
    }

    function setUpdateCount(list) {
        return set(Const.DATA.KEY_UPDATE_COUNT, list);
    }

    function getUpdateCount() {
        return get(Const.DATA.KEY_UPDATE_COUNT);
    }

    function setOrg(org) {
        let orgTem =  {
            country: org.country,
            country_status: org.country_status,
            id: org.id,
            root_id: org.root_id,
            type: org.type,
            avatar: org.avatar,
            name: org.name,
        }

        return set(Const.DATA.KEY_ORG, orgTem);
    }

    function getOrg() {
        return get(Const.DATA.KEY_ORG);
    }

    function getPrimaryOrg() {
        return get(Const.DATA.KEY_PRIMARYORG);
    }

    function setBrandPartner(orgId, org) {
        let orgTem =  {
            country: org.country,
            country_status: org.country_status,
            id: org.id,
            root_id: org.root_id,
            type: org.type,
            avatar: org.avatar,
            name: org.name,
        }
        return set("brand.partner." + orgId, orgTem);
    }

    function getBrandPartner(orgId) {
        return get("brand.partner." + orgId);
    }

    function setRoleEmailType(email, type) {
        return set("role.email." + email, type);
    }

    function getRoleEmailType(email) {
        return get("role.email." + email);
    }

    function clearAuthData() {
        setToken('');
        setUser('');
    }

    function getWarrantyTime(currentDate, month) {
        var expriedYear;
        var expriedMonth = (parseInt(currentDate.substring(4, 6)) + month) % 12;
        var expriedDay = currentDate.substring(6);
        if (parseInt(expriedMonth) + month <= 12) {
            expriedYear = parseInt(currentDate.substring(0, 4));
        } else {
            let nextYear = Math.floor(expriedMonth / 12);
            expriedYear = parseInt(currentDate.substring(0, 4)) + nextYear;
        }
        //???????????????????????????N???????????????????????????????????????????????????????????????????????????????????????????????????
        if (expriedMonth == '02' || expriedMonth == 2) {
            var monthEndDate = new Date(expriedYear, expriedMonth, 0).getDate();
            if (parseInt(expriedDay) > monthEndDate) { //???????????????
                //????????????????????????????????????
                expriedDay = monthEndDate;
            }
        }
        return expriedYear + expriedMonth + expriedDay;
    }

    function getTestError(code) {
        return Const.TEST_ERROR_CODE[parseInt(code)]
    }

    function setLang(lang) {
        return set(Const.DATA.KEY_LANG, lang);
    }

    function getLang() {
        return get(Const.DATA.KEY_LANG);
    }

    function setAuth(auth) {
        return set(Const.DATA.KEY_AUTH, auth);
    }

    function getAuth() {
        return get(Const.DATA.KEY_AUTH);
    }

    function setOffline(flag) {
        return set(Const.DATA.KEY_OFFLINE, flag);
    }

    function getOffline() {
        return get(Const.DATA.KEY_OFFLINE);
    }

    function setPort(port) {
        return set(Const.DATA.KEY_PORT, port);
    }

    function getPort() {
        return get(Const.DATA.KEY_PORT);
    }

    function setRecordList(list) {
        return set(Const.DATA.KEY_OFFLINE_OBJECT_RECORD, list);
    }

    function getRecordList() {
        return get(Const.DATA.KEY_OFFLINE_OBJECT_RECORD);
    }
    // diagnosis
    function setDiagnosisTest(select) {
        return set(Const.DATA.KEY_DIAGNOSIS_TEST, select);
    }

    function getDiagnosisTest() {
        return get(Const.DATA.KEY_DIAGNOSIS_TEST);
    }

    function setDiagnosisConnection(select) {
        return set(Const.DATA.KEY_DIAGNOSIS_CONNECTION, select);
    }

    function getDiagnosisConnection() {
        return get(Const.DATA.KEY_DIAGNOSIS_CONNECTION);
    }

    function setSavedRecordIndexList(list) {
        return set(Const.DATA.KEY_BATCH_RECORD, list);
    }

    function getSavedRecordIndexList() {
        return get(Const.DATA.KEY_BATCH_RECORD);
    }

    function getIsBatchRecordIng() {
        return get('is_batch_record');
    }

    function setIsBatchRecordIng(flag) {
        return set('is_batch_record', flag);
    }

    function getFormatterDate(time) {
        return new Intl.DateTimeFormat('zh', {
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit',
            hour12: false
        }).format(new Date(time)).replace(/\//g, '-');
    }
    // ???????????????????????????  ????????????target??????
    function deepClone(target) {
        // ??????????????????
        let result;
        // ???????????????????????????????????????????????????
        if (typeof target === 'object') {
            // ???????????????????????????
            if (Array.isArray(target)) {
                result = []; // ???result??????????????????????????????????????????
                for (let i in target) {
                    // ?????????????????????????????????
                    result.push(this.deepClone(target[i]))
                }
                // ???????????????????????????null????????????????????????null
            } else if (target === null) {
                result = null;
                // ?????????????????????????????????RegExp???????????????????????????
            } else if (target.constructor === RegExp) {
                result = target;
            } else {
                // ??????????????????????????????for in???????????????????????????????????????
                result = {};
                for (let i in target) {
                    result[i] = this.deepClone(target[i]);
                }
            }
            // ????????????????????????????????????????????????????????????????????????
        } else {
            result = target;
        }
        // ??????????????????
        return result;
    }

    function getSpecialAccount(type) {
        if (type) {
            switch (type) {
                case "try":
                    return get(Const.DATA.KEY_TRY_ACCOUNT);
                    break;
                case "tem":
                    return get(Const.DATA.KEY_TEM_ACCOUNT);
                    break;
            }
        } else {
            return false
        }
    }

    function setSpecialAccount(type, bollean) {
        if (type) {
            switch (type) {
                case "try":
                    return set(Const.DATA.KEY_TRY_ACCOUNT, bollean);
                    break;
                case "tem":
                    return set(Const.DATA.KEY_TEM_ACCOUNT, bollean);
                    break;
            }
        } else {
            return false
        }
    }

}

export default new Data();



//////////////////
// WEBPACK FOOTER
// ./core/js/Data.js
// module id = 697
// module chunks = 73