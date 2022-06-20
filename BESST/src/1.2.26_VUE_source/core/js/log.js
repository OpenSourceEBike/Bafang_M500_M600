import Const from './const';
import Config from './config';


function Log() {
    const LevelMap = {
        error: Const.SYSTEM.LOG_LEVEL_ERROR,
        warn: Const.SYSTEM.LOG_LEVEL_WARN,
        info: Const.SYSTEM.LOG_LEVEL_INFO,
        trace: Const.SYSTEM.LOG_LEVEL_TRACE,
        debug: Const.SYSTEM.LOG_LEVEL_DEBUG
    };

    return {
        debug: debug,
        d: debug,

        info: info,
        i: info,

        warn: warn,
        w: warn,

        error: error,
        e: error,

        trace: trace,
        t: trace
    };

    function getLevelName(level) {
        for (let i in LevelMap) {
            if (LevelMap.hasOwnProperty(i)) {
                let l = LevelMap[i];
                if (l === level) {
                    return i;
                }
            }
        }

        return '';
    }

    function rawLog() {
        let args = Array.prototype.slice.call(arguments);
        let level = args.shift();
        let levelName = getLevelName(level);

        if (level > Config.LOG_LEVEL) {
            return;
        }

        args.unshift('[' + levelName.toUpperCase() + ']');
        args.unshift(new Date().toString());
        console.log.apply(console, args);
    }

    function debug() {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(Const.SYSTEM.LOG_LEVEL_DEBUG);
        rawLog.apply(null, args);
    }

    function trace() {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(Const.SYSTEM.LOG_LEVEL_TRACE);
        rawLog.apply(null, args);
    }

    function info() {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(Const.SYSTEM.LOG_LEVEL_INFO);
        rawLog.apply(null, args);
    }

    function warn() {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(Const.SYSTEM.LOG_LEVEL_WARN);
        rawLog.apply(null, args);
    }

    function error() {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(Const.SYSTEM.LOG_LEVEL_ERROR);
        rawLog.apply(null, args);
    }
}

export default new Log()



//////////////////
// WEBPACK FOOTER
// ./core/js/log.js
// module id = 74
// module chunks = 73