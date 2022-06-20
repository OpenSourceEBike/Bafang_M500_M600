import Const from "./const"
import Crc from 'crc';

function parseDate(date) {
    return parseInt(new Date(date).getTime() / 1000);
}

function clearData(data) {
    Object.keys(data).forEach(function(key) {
        if (is('object', data[key])) {
            data[key] = {};
        }
        if (is('array', data[key])) {
            data[key] = [];
        }
        if (is('number', data[key])) {
            data[key] = 0;
        }
        if (is('string', data[key])) {
            data[key] = "";
        }
    })
}

function is(type, obj) {
    let item;
    item = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && item.toLowerCase() === type.toLowerCase()
}

function clone(obj) {
    let o;
    if (typeof obj === "object") {
        if (obj === null) {
            o = null;
        } else {
            if (obj instanceof Array) {
                o = [];
                for (let i = 0, len = obj.length; i < len; i++) {
                    o.push(clone(obj[i]));
                }
            } else {
                o = {};
                for (let j in obj) {
                    o[j] = clone(obj[j]);
                }
            }
        }
    } else {
        o = obj;
    }
    return o;
}

function time(dat) {
    if(dat){
        return parseInt(new Date(dat).getTime() / 1000, 10);
    }else{
        return parseInt(new Date().getTime() / 1000, 10);
    }
}

function timestamp() {
    return new Date().getTime();
}

function getCurrentMonthFirst() {
    let t = new Date();
    t.setDate(1);
    let m = t.getMonth() + 1;
    m = m > 9 ? m : '0' + m;
    let r = new Date(t.getFullYear() + "-" + m + "-0" + t.getDate()).getTime();
    // // 返回本地时间与格林威治标准时间 (GMT) 的分钟差。
    let offsetMinute = t.getTimezoneOffset();
    return r + offsetMinute * 60 * 1000;
}

function transDay(day) {
    if (day == null) day = 0;
    if (day instanceof String) day = parseInt(day);
    let t = new Date();
    let m = t.getMonth() + 1;
    m = m > 9 ? m : '0' + m;
    let d = t.getDate() + day;
    d = d > 9 ? d : '0' + d;
    let r = new Date(t.getFullYear() + "-" + m + "-" + d).getTime();
    // 返回本地时间与格林威治标准时间 (GMT) 的分钟差。
    let offsetMinute = t.getTimezoneOffset();
    return r + offsetMinute * 60 * 1000;
}

function changeDateForm(date) {
    let t = new Date(date);
    let m = t.getMonth() + 1;
    m = m > 9 ? m : '0' + m;
    let d = t.getDate();
    d = d > 9 ? d : '0' + d;
    return t.getFullYear() + "-" + m + "-" + d;
}

function convertMoney(money) {
    if (!money) {
        return 0;
    }
    return parseInt(money) * 100;
}

function get(obj, key, defaultValue) {
    let value = key.split(".").reduce(function(o, x) {
        return (typeof o === "undefined" || o === null) ? o : o[x];
    }, obj);

    if (typeof value === "undefined" || value === null) {
        return defaultValue;
    }

    return value;
}

function digitLength(data, length) {
    return this.sprintf('%0' + length + 'd', data);
}

function getImgUrl(name) {
    return Const.IMG_URL_PREFIX + name;
}

function uniqueArray(a) {
    let prims = { "boolean": {}, "number": {}, "string": {} },
        objs = [];

    return a.filter(function(item) {
        let type = typeof item;
        if (type in prims)
            return prims[type].hasOwnProperty(item) ? false : (prims[type][item] = true);
        else
            return objs.indexOf(item) >= 0 ? false : objs.push(item);
    });
}

function calculateChecksumFFFF(hexData) {
    let sum = 0;
    if (!hexData) return 0;

    for (let i = 0; i < parseInt(hexData.length / 2); i++) {
        sum += parseInt(hexData.substring(i * 2, i * 2 + 2), 16);
    }

    return sum & 0xffff;
}

function calculateChecksumFF(hexData) {
    let sum = 0;
    if (!hexData) return 0;

    for (let i = 0; i < parseInt(hexData.length / 2); i++) {
        sum += parseInt(hexData.substring(i * 2, i * 2 + 2), 16);
    }

    return sum & 0xff;
}

// ascii
function hexToString(hexString) {
    let str = '';
    if (!hexString) return str;

    for (let i = 0; i < hexString.length; i += 2) {
        let num = parseInt(hexString.substr(i, 2), 16);
        if (num > 31 && num < 123) {
            str += String.fromCharCode(num);
        }
    }
    return str;
}

function stringToHex(str) {
    let hex, i;
    let result = "";

    if (!str) return '';

    for (i = 0; i < str.length; i++) {
        hex = str.charCodeAt(i).toString(16);
        result += ("0" + hex).slice(-2);
    }

    return result
}

function isFunction(fn) {
    return Object.prototype.toString.call(fn) === '[object Function]';
}

function isNum(num) {
    let reNum = /^\d*$/;
    return (reNum.test(num));
}

function isEmail(email) {
    email = email + "";

    var myReg = /^(\w)+(\.\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/;

    return myReg.exec(email);

}

function verifyPassword(password) {
    password = password + "";
    //  支持数字与英文 长度 8到16
    let passwordReg = /^[A-Za-z0-9]{8,16}$/;
    return passwordReg.exec(password);
}

function verifyStringNaming(password) {
    password = password + "";
    //  匹配Windows不支持 命名字符
    let passwordReg = /[\\\/^:*?"<>|]/;
    return passwordReg.test(password);
}

function produceDateParse(sn) {
    // var sn = 'DPC11.U2.0605E1R7060006';
    let numStr = sn.slice(-4);
    let sn_y = sn.slice(-8, -7);
    let sn_m = sn.slice(-7, -6);
    let sn_d = sn.slice(-6, -4);

    if (sn_y > 'O' && parseInt(sn_m, 16) < 13  &&  parseInt(sn_d, 10) < 32 &&  parseInt(numStr, 10) < 10000) {
        let year = '';
        let month = '';

        switch (sn_y) {
            case 'P':
                year = '15';
                break;
            case 'Q':
                year = '16';
                break;
            case 'R':
                year = '17';
                break;
            case 'S':
                year = '18';
                break;
            case 'T':
                year = '19';
                break;
            case 'U':
                year = '20';
                break;
            case 'V':
                year = '21';
                break;
            case 'W':
                year = '22';
                break;
            case 'X':
                year = '23';
                break;
            case 'Y':
                year = '24';
                break;
            case 'Z':
                year = '25';
                break;
        }
        switch (sn_m) {
            case 'A':
                month = '10';
                break;
            case 'B':
                month = '11';
                break;
            case 'C':
                month = '12';
                break;
            default:
                month = '0' + sn_m;
        }

        return '20' + year + '-' + month + '-' + sn_d;
    } else {
        return '-';
    }


}

function sortListTree(l) {
    let list = l
    if(Array.isArray(list)){
        function compare(child) {
            for (let ll=0; ll<=child.length-2; ll++){
                for(let lll=ll+1; lll<=child.length-1; lll++){
                    if(child[lll].name < child[ll].name){
                        let tem = child[lll];
                        child[lll] = child[ll];
                        child[ll] = tem;
                    }
                }
            }
        }
        for(let l1=0;l1<list.length;l1++){
            for(let l2=0;l2<list[l1]['child'].length;l2++){
                if(list[l1]['child'][l2]['child'].length>0){
                    compare(list[l1]['child'][l2]['child']);
                    for(let l3=0;l3<list[l1]['child'][l2]['child'].length;l3++){
                        if(list[l1]['child'][l2]['child'][l3]['child'].length>0){
                            compare(list[l1]['child'][l2]['child'][l3]['child']);
                        }
                    }
                }
            }
        }
    }
    return list;
}

function encryption(data) {
    if(typeof data !== 'string'){
        return ''
    }
    let str = '';
    let alterText = [];
    let varCost = [];
    const TextLength = data.length;
    for (let i = 0; i < TextLength; i++) {
        let random = parseInt(Math.random() * 266);
        alterText[i] = data.charCodeAt(i) + random;
        varCost[i] = random;
    }
    for (let i = 0; i < TextLength; i++) {
        str += String.fromCharCode(alterText[i], varCost[i]);
    }
    return str;
}

//解密
function decrypt(text) {
    if(typeof text !== 'string'){
        return ''
    }
    let str = '';
    let alterText = [];
    let varCost = [];
    const TextLength = text.length;
    for (let i = 0; i < TextLength; i++) {
        alterText[i] = text.charCodeAt(i);
        varCost[i] = text.charCodeAt(i + 1);
    }
    for (let i = 0; i < TextLength; i = i + 2) {
        str += String.fromCharCode(alterText[i] - varCost[i]);
    }
    return str;
}


function checkCrc(arrBuf) {
    let crc16 = '0000'+ Crc.crc16xmodem(arrBuf).toString(16); //两位
    console.log(crc16);
    return crc16.substr(-4);
}

function checkSNByType(sn, typeStrNo){
    if (!sn) return -1
    let snCompare = sn.trim().substr(0,2).toUpperCase()
    let res = true;

    switch(typeStrNo){
        case 4:
        case 'hmi':
            if(snCompare !== 'DP'){
                res = false
            }
            break;
        case 7:
        case 'ctrl':
            if(snCompare !== 'CR'){
                res = false
            }
            break;
        case 6:
        case 'sensor':
            if(snCompare !== 'SR'){
                res = false
            }
            break;
        case 10:
        case 'tool':
            if(snCompare !== 'BE'){
                res = false
            }
            break;
    }

    return res
}

function  calculate_md5(string) {
    function md5_RotateLeft(lValue, iShiftBits) {
        return (lValue << iShiftBits) | (lValue >>> (32 - iShiftBits));
    }
    function md5_AddUnsigned(lX, lY) {
        var lX4, lY4, lX8, lY8, lResult;
        lX8 = (lX & 0x80000000);
        lY8 = (lY & 0x80000000);
        lX4 = (lX & 0x40000000);
        lY4 = (lY & 0x40000000);
        lResult = (lX & 0x3FFFFFFF) + (lY & 0x3FFFFFFF);
        if (lX4 & lY4) {
            return (lResult ^ 0x80000000 ^ lX8 ^ lY8);
        }
        if (lX4 | lY4) {
            if (lResult & 0x40000000) {
                return (lResult ^ 0xC0000000 ^ lX8 ^ lY8);
            } else {
                return (lResult ^ 0x40000000 ^ lX8 ^ lY8);
            }
        } else {
            return (lResult ^ lX8 ^ lY8);
        }
    }
    function md5_F(x, y, z) {
        return (x & y) | ((~x) & z);
    }
    function md5_G(x, y, z) {
        return (x & z) | (y & (~z));
    }
    function md5_H(x, y, z) {
        return (x ^ y ^ z);
    }
    function md5_I(x, y, z) {
        return (y ^ (x | (~z)));
    }
    function md5_FF(a, b, c, d, x, s, ac) {
        a = md5_AddUnsigned(a, md5_AddUnsigned(md5_AddUnsigned(md5_F(b, c, d), x), ac));
        return md5_AddUnsigned(md5_RotateLeft(a, s), b);
    };
    function md5_GG(a, b, c, d, x, s, ac) {
        a = md5_AddUnsigned(a, md5_AddUnsigned(md5_AddUnsigned(md5_G(b, c, d), x), ac));
        return md5_AddUnsigned(md5_RotateLeft(a, s), b);
    };
    function md5_HH(a, b, c, d, x, s, ac) {
        a = md5_AddUnsigned(a, md5_AddUnsigned(md5_AddUnsigned(md5_H(b, c, d), x), ac));
        return md5_AddUnsigned(md5_RotateLeft(a, s), b);
    };
    function md5_II(a, b, c, d, x, s, ac) {
        a = md5_AddUnsigned(a, md5_AddUnsigned(md5_AddUnsigned(md5_I(b, c, d), x), ac));
        return md5_AddUnsigned(md5_RotateLeft(a, s), b);
    };
    function md5_ConvertToWordArray(string) {
        var lWordCount;
        var lMessageLength = string.length;
        var lNumberOfWords_temp1 = lMessageLength + 8;
        var lNumberOfWords_temp2 = (lNumberOfWords_temp1 - (lNumberOfWords_temp1 % 64)) / 64;
        var lNumberOfWords = (lNumberOfWords_temp2 + 1) * 16;
        var lWordArray = Array(lNumberOfWords - 1);
        var lBytePosition = 0;
        var lByteCount = 0;
        while (lByteCount < lMessageLength) {
            lWordCount = (lByteCount - (lByteCount % 4)) / 4;
            lBytePosition = (lByteCount % 4) * 8;
            lWordArray[lWordCount] = (lWordArray[lWordCount] | (string.charCodeAt(lByteCount) << lBytePosition));
            lByteCount++;
        }
        lWordCount = (lByteCount - (lByteCount % 4)) / 4;
        lBytePosition = (lByteCount % 4) * 8;
        lWordArray[lWordCount] = lWordArray[lWordCount] | (0x80 << lBytePosition);
        lWordArray[lNumberOfWords - 2] = lMessageLength << 3;
        lWordArray[lNumberOfWords - 1] = lMessageLength >>> 29;
        return lWordArray;
    };
    function md5_WordToHex(lValue) {
        var WordToHexValue = "",
            WordToHexValue_temp = "",
            lByte, lCount;
        for (lCount = 0; lCount <= 3; lCount++) {
            lByte = (lValue >>> (lCount * 8)) & 255;
            WordToHexValue_temp = "0" + lByte.toString(16);
            WordToHexValue = WordToHexValue + WordToHexValue_temp.substr(WordToHexValue_temp.length - 2, 2);
        }
        return WordToHexValue;
    };
    function md5_Utf8Encode(string) {
        string = string.replace(/\r\n/g, "\n");
        var utftext = "";
        for (var n = 0; n < string.length; n++) {
            var c = string.charCodeAt(n);
            if (c < 128) {
                utftext += String.fromCharCode(c);
            } else if ((c > 127) && (c < 2048)) {
                utftext += String.fromCharCode((c >> 6) | 192);
                utftext += String.fromCharCode((c & 63) | 128);
            } else {
                utftext += String.fromCharCode((c >> 12) | 224);
                utftext += String.fromCharCode(((c >> 6) & 63) | 128);
                utftext += String.fromCharCode((c & 63) | 128);
            }
        }
        return utftext;
    };
    var x = Array();
    var k, AA, BB, CC, DD, a, b, c, d;
    var S11 = 7,
        S12 = 12,
        S13 = 17,
        S14 = 22;
    var S21 = 5,
        S22 = 9,
        S23 = 14,
        S24 = 20;
    var S31 = 4,
        S32 = 11,
        S33 = 16,
        S34 = 23;
    var S41 = 6,
        S42 = 10,
        S43 = 15,
        S44 = 21;
    string = md5_Utf8Encode(string);
    x = md5_ConvertToWordArray(string);
    a = 0x67452301;
    b = 0xEFCDAB89;
    c = 0x98BADCFE;
    d = 0x10325476;
    for (k = 0; k < x.length; k += 16) {
        AA = a;
        BB = b;
        CC = c;
        DD = d;
        a = md5_FF(a, b, c, d, x[k + 0], S11, 0xD76AA478);
        d = md5_FF(d, a, b, c, x[k + 1], S12, 0xE8C7B756);
        c = md5_FF(c, d, a, b, x[k + 2], S13, 0x242070DB);
        b = md5_FF(b, c, d, a, x[k + 3], S14, 0xC1BDCEEE);
        a = md5_FF(a, b, c, d, x[k + 4], S11, 0xF57C0FAF);
        d = md5_FF(d, a, b, c, x[k + 5], S12, 0x4787C62A);
        c = md5_FF(c, d, a, b, x[k + 6], S13, 0xA8304613);
        b = md5_FF(b, c, d, a, x[k + 7], S14, 0xFD469501);
        a = md5_FF(a, b, c, d, x[k + 8], S11, 0x698098D8);
        d = md5_FF(d, a, b, c, x[k + 9], S12, 0x8B44F7AF);
        c = md5_FF(c, d, a, b, x[k + 10], S13, 0xFFFF5BB1);
        b = md5_FF(b, c, d, a, x[k + 11], S14, 0x895CD7BE);
        a = md5_FF(a, b, c, d, x[k + 12], S11, 0x6B901122);
        d = md5_FF(d, a, b, c, x[k + 13], S12, 0xFD987193);
        c = md5_FF(c, d, a, b, x[k + 14], S13, 0xA679438E);
        b = md5_FF(b, c, d, a, x[k + 15], S14, 0x49B40821);
        a = md5_GG(a, b, c, d, x[k + 1], S21, 0xF61E2562);
        d = md5_GG(d, a, b, c, x[k + 6], S22, 0xC040B340);
        c = md5_GG(c, d, a, b, x[k + 11], S23, 0x265E5A51);
        b = md5_GG(b, c, d, a, x[k + 0], S24, 0xE9B6C7AA);
        a = md5_GG(a, b, c, d, x[k + 5], S21, 0xD62F105D);
        d = md5_GG(d, a, b, c, x[k + 10], S22, 0x2441453);
        c = md5_GG(c, d, a, b, x[k + 15], S23, 0xD8A1E681);
        b = md5_GG(b, c, d, a, x[k + 4], S24, 0xE7D3FBC8);
        a = md5_GG(a, b, c, d, x[k + 9], S21, 0x21E1CDE6);
        d = md5_GG(d, a, b, c, x[k + 14], S22, 0xC33707D6);
        c = md5_GG(c, d, a, b, x[k + 3], S23, 0xF4D50D87);
        b = md5_GG(b, c, d, a, x[k + 8], S24, 0x455A14ED);
        a = md5_GG(a, b, c, d, x[k + 13], S21, 0xA9E3E905);
        d = md5_GG(d, a, b, c, x[k + 2], S22, 0xFCEFA3F8);
        c = md5_GG(c, d, a, b, x[k + 7], S23, 0x676F02D9);
        b = md5_GG(b, c, d, a, x[k + 12], S24, 0x8D2A4C8A);
        a = md5_HH(a, b, c, d, x[k + 5], S31, 0xFFFA3942);
        d = md5_HH(d, a, b, c, x[k + 8], S32, 0x8771F681);
        c = md5_HH(c, d, a, b, x[k + 11], S33, 0x6D9D6122);
        b = md5_HH(b, c, d, a, x[k + 14], S34, 0xFDE5380C);
        a = md5_HH(a, b, c, d, x[k + 1], S31, 0xA4BEEA44);
        d = md5_HH(d, a, b, c, x[k + 4], S32, 0x4BDECFA9);
        c = md5_HH(c, d, a, b, x[k + 7], S33, 0xF6BB4B60);
        b = md5_HH(b, c, d, a, x[k + 10], S34, 0xBEBFBC70);
        a = md5_HH(a, b, c, d, x[k + 13], S31, 0x289B7EC6);
        d = md5_HH(d, a, b, c, x[k + 0], S32, 0xEAA127FA);
        c = md5_HH(c, d, a, b, x[k + 3], S33, 0xD4EF3085);
        b = md5_HH(b, c, d, a, x[k + 6], S34, 0x4881D05);
        a = md5_HH(a, b, c, d, x[k + 9], S31, 0xD9D4D039);
        d = md5_HH(d, a, b, c, x[k + 12], S32, 0xE6DB99E5);
        c = md5_HH(c, d, a, b, x[k + 15], S33, 0x1FA27CF8);
        b = md5_HH(b, c, d, a, x[k + 2], S34, 0xC4AC5665);
        a = md5_II(a, b, c, d, x[k + 0], S41, 0xF4292244);
        d = md5_II(d, a, b, c, x[k + 7], S42, 0x432AFF97);
        c = md5_II(c, d, a, b, x[k + 14], S43, 0xAB9423A7);
        b = md5_II(b, c, d, a, x[k + 5], S44, 0xFC93A039);
        a = md5_II(a, b, c, d, x[k + 12], S41, 0x655B59C3);
        d = md5_II(d, a, b, c, x[k + 3], S42, 0x8F0CCC92);
        c = md5_II(c, d, a, b, x[k + 10], S43, 0xFFEFF47D);
        b = md5_II(b, c, d, a, x[k + 1], S44, 0x85845DD1);
        a = md5_II(a, b, c, d, x[k + 8], S41, 0x6FA87E4F);
        d = md5_II(d, a, b, c, x[k + 15], S42, 0xFE2CE6E0);
        c = md5_II(c, d, a, b, x[k + 6], S43, 0xA3014314);
        b = md5_II(b, c, d, a, x[k + 13], S44, 0x4E0811A1);
        a = md5_II(a, b, c, d, x[k + 4], S41, 0xF7537E82);
        d = md5_II(d, a, b, c, x[k + 11], S42, 0xBD3AF235);
        c = md5_II(c, d, a, b, x[k + 2], S43, 0x2AD7D2BB);
        b = md5_II(b, c, d, a, x[k + 9], S44, 0xEB86D391);
        a = md5_AddUnsigned(a, AA);
        b = md5_AddUnsigned(b, BB);
        c = md5_AddUnsigned(c, CC);
        d = md5_AddUnsigned(d, DD);
    }
    return (md5_WordToHex(a) + md5_WordToHex(b) + md5_WordToHex(c) + md5_WordToHex(d)).toLowerCase();
}

function checkEncryption(inStr){
    var b = '';
    // a.forEach((item)=>{
    //     let tem = charCodeAt(item)
    //     b.push()
    // })
    for (var i = 0; i < inStr.length; i++) {
        // console.log(inStr.charCodeAt(i));
        // console.log('t'.charCodeAt(0));
        let hex = inStr.charCodeAt(i) ^('t'.charCodeAt(0)) ;

        b += String.fromCharCode(hex);
    }


    return b
}

const Util = {
    sprintf: require('locutus/php/strings/sprintf'),
    date: require('locutus/php/datetime/date'),
    trim: require('locutus/php/strings/trim'),
    parseDate: parseDate,
    clearData: clearData,
    is: is,
    clone: clone,
    time: time,
    timestamp: timestamp,
    convertMoney: convertMoney,
    get: get,
    digitLength: digitLength,
    getImgUrl: getImgUrl,
    uniqueArray: uniqueArray,
    calculateChecksumFF: calculateChecksumFF,
    calculateChecksumFFFF: calculateChecksumFFFF,
    hexToString: hexToString,
    stringToHex: stringToHex,
    isFunction: isFunction,
    isNum: isNum,
    isEmail: isEmail,
    verifyPassword: verifyPassword,
    verifyStringNaming: verifyStringNaming,
    produceDateParse: produceDateParse,
    getCurrentMonthFirst: getCurrentMonthFirst,
    transDay: transDay,
    changeDateForm: changeDateForm,
    sortListTree: sortListTree,
    decrypt,
    encryption,
    checkCrc,
    checkSNByType,
    calculate_md5,
    checkEncryption,
};


export default Util



//////////////////
// WEBPACK FOOTER
// ./core/js/util.js
// module id = 55
// module chunks = 73