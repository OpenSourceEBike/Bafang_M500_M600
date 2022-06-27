import Const from './js/const'
import Config from './js/config'
import Log from './js/log'
import Util from './js/util'
import Api from './js/api'
import Data from './js/Data'
//新增http请求方法
import HttpHelp from './js/httpHelp'
//数据
import ConstData from './js/constData'
// nl-v2 静态数据
import StatusData from './js/statusData'

const Core = {
    Config: Config,
    Const: Const,
    Log: Log,
    Util: Util,
    Api: Api,
    Data: Data,
    HttpHelp: HttpHelp,
    ConstData: ConstData,
    StatusData: StatusData
};
export default Core;



//////////////////
// WEBPACK FOOTER
// ./core/core.js
// module id = 3
// module chunks = 73