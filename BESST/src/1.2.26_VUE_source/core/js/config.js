import Const from './const'

function Config() {

    let Core = {
        LOG_LEVEL: Const.SYSTEM.LOG_LEVEL_DEBUG,

        DateFormatNoTime: 'Y.m.d',
        DateFormatNoYear: 'm-d H:i',

        isInDebug: isInDebug,

    };

    return Core;

    function isInDebug() {
        return Core.LOG_LEVEL === Const.SYSTEM.LOG_LEVEL_DEBUG;
    }

}

export default new Config();


//////////////////
// WEBPACK FOOTER
// ./core/js/config.js
// module id = 313
// module chunks = 73