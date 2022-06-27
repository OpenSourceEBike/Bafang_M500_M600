// 登录页面相关
const Login = {
    declare: "声明",
    readmore: "更多",
    declareP1: "BESST是为品牌公司开发的电动助力车驱动系统管理软件， 这些品牌将邀请代码发送给其经销商，工厂和分销商，然后注册自己的帐户。每个角色必须确保注册的帐户和密码不与第三方共享，并且品牌负责培训受邀的关系。",
    declareP2: "注：所有使用者需向品牌公司或软件所有者购买BESST服务工具，才能实现软件的全部功能。",
    declareP3: "品牌公司",
    declareP6: "1、品牌公司由软件所有者授权：",
    declareP7: "品牌公司向软件所有者提出书面申请，申请内容包括：品牌公司注册名称，国家，地址，邮编，管理账户的合法邮箱，品牌所有权证明，网址等；",
    declareP8: "软件所有者对信息真实性，合法性进行确认；",
    declareP9: "软件所有者通过软件后台建立账户信息，并通过邮件发送授权信息；",
    declareP10: "品牌公司通过授权信息，下载该软件后，务必根据软件注册流程准确填写相关资料，完善信息；",
    declareP11: '进入“设置”, 设置公司信息，更改原始密码；',
    declareP12: '进入“品牌列表”，添加自己公司品牌列表；',
    declareP13: '进入“车型列表”，添加车型信息；',
    declareP14: '进入“经销商”，根据经销商联系邮箱发送邀请码；',
    declareP15: '进入“组装厂”， 根据组装厂联系邮箱发送邀请码；',
    declareP16: '通过主账号进入“账户”，可自行分发员工账号；',
    declareP17: "软件下载路径：",
    declareP18: "经销商和分销商",
    declareP19: "3、经销商和分销商由品牌公司授权：",
    declareP20: "经销商和分销商需向品牌公司索要”邀请码”；",
    declareP21: "对新注册用户，在“注册”界面填入“邀请码”，验证该邮箱作为账户名；",
    declareP22: "通过该邮箱接收验证码后，设置密码；",
    declareP23: "对老的BESST系统的用户，进入“品牌列表”, 把“邀请码”填入“接受邀请”,接受新的品牌邀请；",
    declareP24: "密码设置成功后，开始按照软件流程填写经销商和分销商信息，并确认信息真实有效，结束注册。",
    declareP25: "下次登录在账户栏填入邮箱号码，并填入设置的密码即可，最好标记“记住密码”，以便下次方便登录；",
    declareP26: '“品牌列表”：注册好邀请码后，自动收集和加载品牌公司的信息；',
    declareP27: '进入“自行车车型”, 自动获取所有品牌的产品信息，无需自行加载；',
    declareP28: '进入“自行车销售”，注册客户信息，确认整车ID，生成销售单，确定售后政策，务必确认资料保存到服务器。',
    declareP29: "制造工厂",
    declareP30: "2、制造工厂由品牌公司授权：",
    declareP31: "制造工厂需向品牌公司索要“邀请码”；",
    declareP32: "密码设置成功后，开始按照软件流程填写制造工厂信息，并确认信息真实有效，结束注册;",
    declareP33: "“品牌列表”：注册好邀请码后，自动收集和加载品牌公司的信息；",
    declareP34: '“订单”和“自行车列表”通过邀请码，收集品牌公司信息，新建销售订单时，需要先选择品牌，然后选择车型;',

    chooseLanguage: "语言",
    chinese: "中文",
    deutsch: "Deutsch",
    english: "English",
    dutch: "Nederlands",
    login: "登录",
    userNamePlaceholder: "用户名",
    passwordPlaceholder: "密码",
    rememberMe: "记住账号",
    forgetPassword: "忘记密码?",
    loginBtnTxt: "登录",
    signUp: "注册",
    offLine: "离线模式",
    inputUser: "请输入用户名",
    inputPassword: "请输入密码",
    accountProblem: "账户有些问题。请联系管理员besst@bafang-e.com",
    limitedIP: '当前账户开启IP验证。IP验证没有通过，请在常用地区登录',
    emptyIP: '当前账户开启IP验证。请稍等或稍后再试。',
    emptyIPThree: '当前账户开启IP验证。请联系管理员。',
};

// 注册页面相关
const RegisterAccount = {
    login: "登录",
    verifyCode: "验证码",
    createAccount: "创建账号",
    createCompany: "创建公司",
    company: "公司",
    email: "邮箱",
    emailValidationCode: "邮箱验证码",
    password: "密码",
    confirmPassword: "确认密码",
    next: "下一步",
    verify: "验证",
};

const RegisterCompany = {
    login: "登录",
    verifyCode: "验证码",
    createAccount: "创建账号",
    createCompany: "创建公司",
    yourRole: "你的角色",
    logo: "商标",
    uploadImgTip: "最好上传有背景的正方形图片，支持JPG，PNG，300*300/400*400，不超过3MB",
    contact: "联系方式",
    companyName: "公司名称",
    mailAddress: "邮箱地址",
    phoneNumber: "手机号码",
    website: "公司网址",
    postalAddress: "通讯地址",
    post: "邮编",
    address: "详细地址",
    save: "保存",
    country: "国家",
};

const RegisterInvitation = {
    login: "登录",
    verifyCode: "验证码",
    createAccount: "创建账号",
    createCompany: "创建公司",
    invitationCode: "邀请码",
    typeTheCodeTip: "输入您收到来自品牌商的邀请码。",
    tryAgainTip: "如果您已使用该邀请码但是没有注册成功，请重新尝试。",
    verify: "验证",
    noCompanyTip: "该账号没有公司信息。",
    reCodeTip: "请输入创建账号时的邀请码来创建公司。",

    getAccess: "如果获得登录权限?",
    backend: "后台",
    brand: "品牌商",
    dealers: "经销商",
    assemblers: "组装厂",
    createAP: "创建账户并初始化密码",
    createCode: "创建邀请吗",
    useCode: "使用邀请代码。按照收到的电子邮件的提示完成帐户的注册和激活",

};

const Register = {
    Account: RegisterAccount,
    Company: RegisterCompany,
    Invitation: RegisterInvitation
};

//忘记密码页面
const ForgetPassword = {
    login: "登录",
    email: "电子邮箱",
    verify: "验证",
    emailValidationCode: "邮箱验证码",
    password: "密码",
    confirmPassword: "确认密码",
    resetPassword: '重置密码',
    emptyEmailTip: "请输入邮箱",
    errorEmailTip: "邮箱格式不正确",
    emptyCodeTip: "请输入验证码",
    emptyPasswordTip: "请输入密码",
    emptyConfirmPasswordTip: "请输入确认密码",
    passwordTip: "两次输入的密码不相同",
    resetPasswordSuccess: "重置密码成功",
    codeHasBeenSent: "验证码已发送",
    passwordFormatTip: "密码只能包含英文与数字，长度在8到16。",
    filterUserEmail: "用户邮箱没有在BESST注册。请检查邮箱是否正确"
};

//Layout 页面
const NavBar = {};

const Title = {
    system: "System",
    dictionary: "Dictionary",
    client: "Client",
    headList: "Head List",
    param: "Params",
    firmware: "Firmware",
    testPlan: "Test Plan",
    repair: "Repair",
    pdOrder: "Production Order",
    rpOrder: "Repair Order",
};

const Sidebar = {
    brand: "品牌列表",
    model: "车型列表",
    vehicle: "自行车列表",
    dealer: "经销商",
    dealerList: "经销商列表",
    invitation: "邀请列表",
    oem: "组装厂",
    oemList: "组装厂列表",
    componentList: "零件商公司",
    serviceCenter: "服务中心",
    serviceCenters: "服务中心列表",
    complaints: "投诉列表",
    tasks: "任务列表",
    account: "账户",
    order: "订单",
    diagnosis: "诊断",
    report: "报告",
    errorCode: "错误代码",
    tool: "工具",
    hmi: "仪表",
    controller: "控制器",
    battery: "电池",
    sensor: "传感器",
    update: "升级",
    auth: "权限",
    product: "产品",
    component: "追溯",
    centerTicket: "中心工单列表",
    myTicket: "我的工单",
    help: "帮助",
    setting: "设置",
    logout: '退出',
    statistics: '统计',

    record: "记录",
    testFeedback: "反馈中心",
    material: "生产看板",
    admin: "管理",
    bikeModel: "自行车车型",
    bikeSales: "销售",
    bikeInfo: "自行车详情",
    author: "授权",
    mechanic: "机械毛病",
    faq: "常见问题",
    tt: Title,
};

const Layout = {
    Sidebar: Sidebar,
    NavBar: NavBar,
};

//Article All
const ArticleSet = {
    image: "图片",
    model: "型号",
    mechanical: "机械属性",
    firmVer: "固件版本",
    hardVer: "硬件版本",
    connector: "接头代号",
    operation: "操作",
    articleNo: "物料号",
    paramVer: "参数代号",
    detail: "详情",
};
//  Operation
const Operation = {
    look: '查看',
    edit: '编辑',
    delete: '删除',
    view: '查看',
    add: '添加',
    save: '保存',
    cancel: '取消',
    confirm: '确认',
    account: '账户',
    reset: '重置密码',
    search: '搜素',
    export: '导出',
    send: '发送',
    upload: '上传',
    select: 'Select',
    close: 'Close',
    yes: 'Yes',
    no: 'No',
};

//components Tool
const AttributeName = {
    controller: "控制器",
    hmi: "仪表",
    battery: "电池",
    motor: "电机",
    sensor: "传感器",
    key: "钥匙",
    sn: "序列号",
    charger: "Charger",
    other: "Other",
    bike: "Bike",

    model: "型号",
    softwareVersion: "软件版本",
    hardwareVersion: "硬件版本",
    tryAccountsTip: "当前账户是试用的。一些操作受限。",

    bootLoad: "引导程序版本",
    errorCode: "错误代码",
    totalMileage: "总里程",
    clientCode: "客户代号",
    wheelDiameter: "轮径",
    speedLimit: "限速",
    totalGear: "总档位",
    serviceMileage: "清除里程提醒",
    setTotalMileage: "设置总里程",
    setSingleMileage: "设置单次里程",
    setTimeZone: "设置时区",
    time: "时间",
    keyCard: "钥匙卡",
    circumference: "轮胎周长",
    singleMileage: "单次里程",
    speedMax: "最大速度",
    speedAvg: "平均速度",
    hmiMode: "模式",
    hmiLevel: "当前档位",
    hmiBoost: "BOOST 模式",
    hmiBoostIn: "进入BOOST",
    hmiBoostOut: "不进入BOOST",
    hmiTurnOffTime: "关机时间",
    hmiNoTurnOff: "不关机",
    hmiLight: "头灯",
    hmiLightOn: "开",
    hmiLightOff: "关",
    hmiButton: "按键 +",
    hmiBtnPress: "按键+按下",
    hmiBtnPressNot: "按键+没按",
    hmiSingleMileageClear: "单里程清零",
    hmiClean: "单次里程清零",
    hmiNotClean: "单次里程不清零",

    spdMeterType: "速度传感器类型",
    manName: "生产商",
    nmlVolt: "额定电压",
    maxCur: "最大电流",
    spdMeterSignalNum: "速度传感器信号",
    limitVolt: "低电压保护",
    limitCurrent: "限流",
    limitSpd: "限速",
    assit: "助力档位",
    positionSensor: "位置传感器标定",
    torqueSenCali: "力矩传感器标定",
    conTemperature: "控制器温度",
    motorTemperature: "电机温度",
    calorie: "卡路里",
    assistStatus: "助推状态",
    assistOn: "开启",
    assistOff: "关闭",
    conTorque: "力矩输出电压",
    conCadence: "踏频",
    conSingleTrip: "单次里程",
    conSingleTripLeft: "剩余里程",
    conSpeed: "速度",
    conCurrent: "电流",
    conVoltage: "电压",

    ASOC: "绝对剩余容量",
    packVoltage: "电池电压",
    fullChargeCapacity: "满充电量",
    current: "电流",
    cell8_14: "电芯8-14电压",
    currentChargeInterval: "当前未充电间隔",
    RSOC: "相对剩余容量",
    capacityLeft: "剩余电量",
    temperature: "温度",
    cell1_7: "电芯1-7电压",
    cycleLife: "充电次数",
    maxChargeInterval: "最大未充电间隔",
    produceDate: "生产日期",
    SOH: "SOH",
    useStage: "维修阶段",
    result: "结论",
    afterSalesTitle: "维护状态确认",

    power: "功率",
    voltage: "电压",
    wheelSize: "轮径",
    rpm: "转速",

    torque: "力矩输出电压",
    cadence: "踏频",
    sensorTip: "主要数据没有读到。请检查传感器连接。",

    component: "零部件",
    com: "端口",
    write: "写入",
    connect: "连接",
    read: "读取",
    reset: "重置",
    set: "Set",
    update: "升级",
    test: "测试",
    clear: "清空",
    start: "开始",
    stop: "停止",
    cancel: "取消",
    confirm: "确认",
    select: "请选择",
    restart: "重新开始",
    save: "保存",
    saveSuccess: "保存成功",
    writeSuccess: "写入成功",
    selectComTip: "请选择端口",
    comDisconnect: "连接断开",
    noComConnect: "请连接端口",
    writeFail: "写入失败。请再次尝试。",
    snEmptyTip: "请输入SN",
    note: "注意",
    notice: "通知",
    message: "信息",
    success: "成功",
    operationFail: "操作失败",
    speedLimitNote: "注意: 设置限速超过了25Km/h。请在当地法律规定的范围内调整限速。",
    hmiUART: "当前系统被判断为UART通讯",
    hmiUARTText1: "在仪表PC模式或开机界面情况下，必须首先手动关闭仪表",
    hmiUARTText2: "按BESST TOOL的MODE键复位",
    hmiUARTText3: "按仪表电源键开启仪表，进入PC模式",
    hmiUARTText4: "按READ键读取参数，如果信息不完整，请再次按READ",

    mainData: "基础数据",
    parameter: "参数内容",
    setting: "设置",
    operationTip: "操作提示",
    txtTitle1: "操作",
    txtTitleHid: "通讯方式",
    txtTitleCom: "通讯方式。",
    txtTitleCon: "你需要获得BESST工具和电脑的连接。USB HID自动连接",
    txtTitleRead: "你能读取零件的基本信息。",
    txtTitleWrt: "你可以重写零件的重要参数。",
    txtTitleReset: "你重置了连接状态。这个功能和BESST工具上的重置按钮一样。当然，这里也会清除所有的数据。",
    txtTitleTest: "你可以检测仪表是否支持诊断。打开仪表，根据不同的类型，显示异常状态。这时候点击按钮, 他会进入正常的界面，前提是你连接控制器。",
    txtTitleUpt: "你可以更新零件的固件。",
    txtTitleClear: "你可以清除内容。",
    txtTitle2: "操作建议",
    txtNote1: "工具模块能检测零件是否支持BESST。他能展示零件的基本信息。",
    txtNote2: "你需要先读数据再重写。我们根据SN记录每一个操作以便追踪。",
    txtNote3: "你需要先读数据再更新。我们根据SN记录每一个操作以便追踪。",
    updateGetFileSucc: "升级文件下载成功。",
    updateStarting: "升级开始。。。",
    resetTip: "BESST工具重置了状态。记得要关闭仪表，再次打开。",

    offlineNo: "经销商和组装厂不能设置更大数值",
    needReadSn: "读取SN失败。请先读取来获取SN",
    offlineCon: "请连接网络",
    offlineHandle: "数据正在处理，请稍后操作",

    updateSuccessTip: "你成功的下载固件到盒子, \n 现在根据说明更新设备",
    updateFailTip: "盒子校验固件失败，出现传输错误。请重新下载固件",
    updateToolReconnectTip: '请拔出BEST工具盒子，然后重新连接PC。 当USB HID状态点变为绿色时，请再次单击“更新”。',
    updateMsg: "升级固件过程",
    updateMsg0: "请先连接。",
    updateMsg1: "如果仪表连接着请拔开, 然后再连接上。",
    updateMsg2: "如果控制器连接着请拔开, 然后再连接上。",
    updateMsg3: "请保持盒子和电脑的连接。",
    updateSelectFile: "请选择文件",
    updateOnedriveNote: "你的文件在OneDrive文件夹, 目前不支持。请移动到其他文件夹",
    updateReadFail: "读取文件和数据失败，请重试",
    updateDataError: "固件数据的标头格式不正确",
    updateCrcError: "这个文件有校验和错误的嫌疑，如果更新失败，请检查文件",
    updateMsg4: "BESST盒子升级仪表",
    updateMsg5: "BESST盒子升级控制器",
    updateMsg6: "BESST盒子升级电池",
    updateMsg7: "BESST盒子升级传感器",
    updateMsg8: "升级BESST盒子",
    updateMsg10: "BESST升级部件中",
    updateMsg11: "升级程序可能出现问题了，建议重新升级",
    updateMsg20: "升级完成了",
    updateMsg30: "请点击读取，获取SN和软件版本",
    updateMsgLO: "请从本地获取bin文件或者从网站获取文件",
    updateMsgL: "请从本地获取bin文件",
    updateMsgTip: "检测到新软件版本。您可以选择最新版本并进行更新。如果您不知道具体步骤，请转到工具更新模块",

    hasHmiSn: "Please check HMI sn。",
    hasCtlSn: "Please check controller sn。",
    hasBatSn: "Please check battery sn。",
    hasMotorSn: "Please check motor sn。",
    hasSensorSn: "Please check sensor sn。",
    updateStep0: '更新步骤',
    updateStep1: 'Click "Connect" to make the connection between computer and service tool。',
    updateStep2: 'Select component type that you want to update。',
    updateStep3: 'Click "Read" to get SN and software version。If SN is empty, update is not allowed。',
    updateStep4: 'Click "Get a file from cloud" or "Get a file from local"。',
    updateStep5: 'Unplug the component and click "Update" to download software to service tool。During this process,you will see service tool "PC" LED blinking。If the "PC" LED is always on, you need reset service tool to normal state and restart。',
    updateStep6: 'After downloading software to service tool successfully, you could plug the component and service tool would write software to it。For more detail, click "help" icon on right top。',

    snMaxLength: "SN length maximum is 40",
    customerMaxLength: "Maximum length is 24",
    speedLimitEmpty: "Speed limit is empty。Please read to get the speed limit of component。",
    whileSizeRange: "Circumference is out of range according wheel size",

    keyCardStart: "Key card start pair",
    keyCardStop: "Key card stop pair",

    noCanConnect: "BESST box is not connected or not support can",
    writeModelTip: "型号数据为空或者不符合规范",
    writeHardwareTip: "硬件版本数据为空或者不符合规范",

    pcMode: "PC mode",
    pcModeTxt: ' "PC" mode is used to read data from display compared with working mode。When HMI connect to BESST tool,if HMI could be switched on, HMI go into "PC" mode。',
    pcModeTxtLCD: ' LCD Display: The screen show "PC" or "Connected OK"。',
    pcModeTxtLED: ' LED Display: The light of highest level is on。',
    paramsName: "参数代号",
    paramsCloud: "从云端获取",
    paramsCombine: "合并数据",

    batUnderMt: "提供维护",
    batOutMt: "不提供维护",
    batUseTime: "电池使用超过30个月了。不提供维护。",
    strNaming: '数据不支持字符"\ / ^ : * ? " < > | "',
    getDataTip: "正在获取服务器数据，请耐心等待",

    remark: "备注",
    defect: "故障",
    image: "图片",
    description: "说明",
    video: "视频",
    updateSteps: '更新步骤',
    connectorType: '如果连接头的类型和图1相同，请单击“Uart”查看详细说明。如果如果连接头的类型和图2相同，请单击“CAN Bus”查看详细说明。',
    updateSoftwareMethod: '如何将软件从服务工具更新到零部件？',
    HMIFirst: '1.将HMI连接服务工具。',
    HMISecond: '2.按下HMI的电源按钮以启动更新过程。如果开始更新，则服务工具“ Uart” LED持续闪烁。',
    HMIThird: '3.如果更新成功，则服务工具的“测试” LED点亮。',
    HMIForth: '4.请重置服务具为正常状工态。否则，您将重复执行步骤2。',
    ControllerFirst: '1.将控制器连接服务工具以初始化更新过程。如果开始更新，则服务工具“ Uart” LED持续闪烁。',
    ControllerSecond: '2.如果更新成功，则服务工具的“测试” LED点亮。然后拔下控制器的插头。',
    ControllerThird: '3.请重置服务具为正常状工态。否则，您将重复执行步骤1。',
    HMIFifth: '1.将HMI连接服务工具以初始化更新过程。如果开始更新，则服务工具“ CAN Bus” LED持续闪烁。',
    HMISixth: '2.如果更新成功，则服务工具的“测试” LED点亮。然后拔下HMI。',
    HMISeventh: '3.将包装箱重置为正常状态。否则，您将重复执行步骤1。',
    ControllerForth: '1.将控制器连接服务工具以初始化更新过程。如果开始更新，则服务工具“ CAN Bus” LED持续闪烁。',
    ControllerFifth: '2.如果更新成功，则服务工具的“测试” LED点亮。然后拔下控制器的插头。',
    ControllerSixth: '3.将包装箱重置为正常状态。否则，您将重复执行步骤1。',
    secondVersionTitle: '除了服务工具的第一个版本之外，其他服务工具还支持更新软件以满足用户的新要求',
    secondVersion: '第二代产品',
    thirdVersion: '第三代产品',
    secondVersionFirst: '1.可能您会在线更新',
    secondVersionSecond: '2.双击“更新”将软件下载到服务工具并进行更新。在此过程中，您将看到服务工具“ PC” LED闪烁。如果“ PC”指示灯保持点亮，则需要将服务工具重置为正常状态。如果“测试”指示灯已经点亮，则只需再次单击“更新”。',
    secondVersionThird: '3.如果更新成功，则服务工具将更改为正常状态。',
    figureFirst: '图1',
    figureSecond: '图2',
    thirdVersionFirst: '2.单击一次“更新”，将软件下载到服务工具并进行更新。在此过程中，您将看到服务工具“ PC” LED闪烁。如果“ PC” LED保持点亮，则需要将服务工具重置为正常状态。如果“测试” LED已经点亮，则需要再次单击“更新”。',
    // defect
    bikeLabel_1: "无助力功能",
    bikeLabel_2: "不能停止电机运转",
    bikeLabel_3: "无法启动",
    bikeLabel_4: "电机可运行，助力功能不正常",
    bikeLabel_5: "助力不连续",
    bikeLabel_6: "助力功能减弱",
    bikeLabel_7: "电池电量和仪表显示不匹配",
    bikeLabel_8: "整车共振异响或抖动",
    bikeLabel_9: "前灯不亮，不能正常工作",
    bikeLabel_10: "后灯不亮，不能正常工作",
    bikeLabel_11: "骑行里程缩短",
    bikeLabel_12: "无速度显示",
    motorLabel_1: "运行时噪声大，不连续",
    motorLabel_2: "无电状态，传动噪声大",
    motorLabel_3: "接头和线损坏",
    motorLabel_4: "主轴松动",
    motorLabel_5: "外壳损坏",
    motorLabel_6: "运行颤抖",
    motorLabel_7: "电机壳体温度太高",
    motorLabel_8: "电机进水",
    motorLabel_9: "电机烧毁",
    motorLabel_10: "固定螺栓孔损坏",
    hmiLabel_1: "外壳不良",
    hmiLabel_2: "按键不工作",
    hmiLabel_3: "屏幕雾气",
    hmiLabel_4: "接头和线损坏",
    hmiLabel_5: "仪表支架损坏",
    hmiLabel_6: "显示异常",
    hmiLabel_7: "显示屏损坏",
    hmiLabel_8: "蓝牙不能联机",
    hmiLabel_9: "震动功能不工作",
    ctlLabel_1: "控制器烧毁",
    ctlLabel_2: "固定部分松脱",
    ctlLabel_3: "控制器进水",
    ctlLabel_4: "接头和线损坏",
    batLabel_1: "外壳不良",
    batLabel_2: "滑板损坏",
    batLabel_3: "放电口损坏",
    batLabel_4: "连接线损坏",
    batLabel_5: "不能充电",
    batLabel_6: "不能放电",
    batLabel_7: "充电口橡胶盖帽损坏",
    batLabel_8: "电池电量显示和按键损坏",
    batLabel_9: "钥匙片损坏",
    batLabel_10: "钥匙片丢失",
    batLabel_11: "电池拆卸困难",
    sensor_1: "轴向松动",
    sensor_2: "锁紧螺母松脱",
    sensor_3: "传感器锈蚀",
    sensor_4: "接头和线损坏",
    sensor_5: "传感器轴承损坏",
    sensor_6: "左护腕损坏",
    sensor_7: "右护腕损坏",

    checkSnHmi: "HMI的SN应该以'DP'开头。",
    checkSnCtrl: "控制器的SN应该以'CR'开头。",
    checkSnSensor: "传感器的SN应该以'SR'开头。",
    checkSnTool: "BESST的SN应该以'BE'开头。",

    inputValueTip: "请输入内容",
    readDateTip: "请读取数据",
};

const Hmi = {
    hmi: "仪表",
    auth: "权限",
    applyHmiAuth: "申请仪表权限",
    applyAllAuth: "申请所有权限权限",
    noHmiAuthTip: "你没有写人仪表的权限，请向品牌商申请权限。",
    applySuccess: "申请成功，请等待品牌商授权",
    brandEmptyTip: "你没有合作的品牌商",
    getApplySuccess: "获取权限成功",
    hmiTip: "主要数据没读出来。请打开HMI到“PC”模式或检查HMI是否已通电",
    hmiTipCan: "主要数据没读出来。如果已连接并打开HMI，则该软件可能需要更新。",
    milesTotalSupport: "支持整数值，如1000km。只会写入整数部分数值",
    milesTotalMax: "最大值为99999。请检查。",
    milesSingleSupport: "支持类似1000.1km的值。仅写入整数部分和第一位小数部分",
    milesSingleMax: "最大值为9999.9。请检查。",
    MileageSingleLess: "单个里程不应大于总里程",
    milesTotalLarger: "总里程不得少于单个里程",

    pairCardIn: "切换到匹配钥匙卡的状态",
    pairCardSuccess: "成功匹配钥匙卡",

    cNam: AttributeName
};

const Controller = {
    controller: "控制器",
    auth: "权限",
    applyControllerAuth: "申请控制器权限",
    applyAllAuth: "申请所有权限权限",
    noControllerAuthTip: "你没有写人控制器的权限，请向品牌商申请权限。",
    applySuccess: "申请成功，请等待品牌商授权",
    brandEmptyTip: "你没有合作的品牌商",
    getApplySuccess: "获取权限成功",
    controllerTip: "Main data is empty。Please check whether controller is powered",
    cNam: AttributeName
};

const Battery = {
    battery: "电池",
    batteryTip: "没有读到主要数据。请检查电池是否有电，或者开关是否打开",
    cNam: AttributeName
};

const Update = {
    update: "升级",
    fileCloudToUpdate: "从云端获取文件",
    fileToUpdate: "从本地获取文件",
    itemName: "设备型号",
    tip: "建议",
    updateTip: "请先选择设备型号，下载对应固件文件进行升级",
    noHardwareFileTip: "这个设备没有任何固件文件",
    noResultTip: "没有找到符合条件的记录",
    picture: "图片",
    name: "名称",
    partNo: "物料号",
    operation: "操作",
    version: "版本",
    updateTime: "更新时间",
    remark: "备注",
    select: "选择",
    firmwareList: "固件列表",
    objectEmptyTip: "这个分类下没有任何的型号",
    componentTypeTip: "请选择部件",

    componentReadTip: "请再读一遍",
    cNam: AttributeName
};

const ComponentsTool = {
    Hmi: Hmi,
    Controller: Controller,
    Battery: Battery,
    Update: Update,
};

//主页
const Index = {
    setting: "设置",
    language: "语言",
    version: "版本",
    versionDate: "版本日期",
    userInfo: "账号",
    searchForUpdate: "查询升级",
    changePassword: "更改密码",
    editCompanyInfo: "更改公司信息",
    password: "密码",
    oldPassword: "旧密码",
    newPassword: "新密码",
    confirmPassword: "确认密码",
    confirm: "确认",
    cancel: "取消",
    authTipOne: "你没有任何的权限",
    authTipTwo: "请联系公司管理员",
    warning: "警告",
    editCompany: "更改公司",
    logo: "图标",
    uploadTipOne: "最好上传有背景的正方形图片",
    uploadTipTwo: "支持JPG，PNG， 300*300/400*400, 不超过3M",
    name: "名字",
    email: "邮箱",
    phone: "电话",
    website: "网址",
    address: "地址",
    postcode: "邮编",
    company: "公司",
    country: "国家",

    languages: {
        german: "Deutsch",
        english: "English",
        chinese: "中文",
        dutch: "Nederlands",
    },
    orgType: {
        brand: "品牌商",
        dealer: "经销商",
        assembler: "组装厂",
        component: "零件商",
        serviceCenter: "服务中心",
    },
    updToolNote1: "BESST服务工具需要更新，可以更好的匹配零部件的特征",
    updToolNote: "注意",
    updToolNote2: "请不要关闭这个窗口，直到你更新成功",
    updToolNote3: "如果更新成功, 服务工具会回到正常状态",
    updToolNote4: "如果更新进度条没有出现, 把服务工具从电脑拔掉, 再接上电脑. 然后点击“更新”按钮",
    updStarting: "更新开始。。。",
    updSuccess: "更新成功了吗?",
    close: "关闭",
    cNam: AttributeName,
    upd: Update,
    op: Operation

};

const Repair = {
    cNam: AttributeName
};

// bike basic
const VehicleDetail = {
    back: "返回",
    sn: "序列号",
    key: "钥匙",
    productTime: "生产时间",
    itemName: "车型",
    itemPartNo: "物料号",
    orderSn: "订单序列号",
    orderSourceId: "订单号",
    customerId: "客户名称",
    customerPartNo: "客户物料号",
    customerPoNo: "客户订单号",
    replace: "替换",
    oldSn: "旧序列号",
    newSn: "新序列号",
    confirm: "确定",
    cancel: "取消",
    save: "保存",
    edit: "编辑",
    add: "新增",
    reportList: "诊断报告",
    detail: "详情",
    operation: "操作",
    date: "日期",
    author: "提交人",
    emptyListTip: "该车还没有诊断报告",
    assembler: "组装厂",
    dealer: "经销商",
    saleTime: "销售时间",
    customerName: "客户名称",
    customerEmail: "客户邮箱",
    part: "部件",
    history: "历史",
};

const VehicleList = {
    oem: "组装厂",
    modelName: "车型",
    brand: "品牌",
    vehicleSn: "车辆 SN",
    bomSn: "部件 SN",
    search: "查询",
    noResult: "没有记录！",
    total: "总数",
    deleteVehicle: "删除车辆",
    deleteTip: "你确定要删除它吗 ?",
    yes: "是",
    no: "否",
    delete: "删除",
    detail: "详情",
    noSale: "未出售",
    tableHead: {
        vehicleSn: "车辆 SN",
        brandName: "品牌",
        ModelName: "车型",
        OrderSn: "订单序列号",
        saleDate: "销售日期",
        operation: "操作",
    },
    deleteSuccess: "删除成功",
    export: "导出",
    start: "开始",
    end: "结束",
    op: Operation,
};


// 服务中心相关
const SCCenterTicketList = {
    category: "分类",
    status: "状态",
    tableHead: {
        serviceNo: "服务编号",
        title: "标题",
        targetSn: "车辆序列号",
        createTime: "创建时间",
        status: "状态",
        operation: "操作",
    },
    categoryList: {
        all: "所有",
        vehicle: "整车",
        motor: "电机",
        hmi: "仪表",
        sensor: "传感器",
        battery: "电池",
        light: " 灯",
        controller: "控制器",
        other: "其他",
    },
    statusList: {
        all: "所有",
        pendingAssign: "待分配",
        processing: "处理中",
        complete: "已完成"
    },
    noResult: "没有记录！",
    total: "总数"
};

const SCHandleUpTicketList = {
    category: "分类",
    status: "状态",
    tableHead: {
        serviceNo: "服务编号",
        title: "标题",
        targetSn: "车辆序列号",
        createTime: "创建时间",
        status: "状态",
        operation: "操作",
    },
    categoryList: {
        all: "所有",
        vehicle: "整车",
        motor: "电机",
        hmi: "仪表",
        sensor: "传感器",
        battery: "电池",
        light: " 灯",
        controller: "控制器",
        other: "其他",
    },
    statusList: {
        all: "所有",
        pendingAssign: "待分配",
        processing: "处理中",
        complete: "已完成"
    },
    noResult: "没有记录！",
    total: "总数"
};

const SCMyTicketList = {
    category: "分类",
    status: "状态",
    tableHead: {
        serviceNo: "服务编号",
        title: "标题",
        targetSn: "车辆序列号",
        createTime: "创建时间",
        status: "状态",
        operation: "操作",
    },
    categoryList: {
        all: "所有",
        vehicle: "整车",
        motor: "电机",
        hmi: "仪表",
        sensor: "传感器",
        battery: "电池",
        light: " 灯",
        controller: "控制器",
        other: "其他",
    },
    statusList: {
        all: "所有",
        pendingAssign: "待分配",
        processing: "处理中",
        complete: "已完成"
    },
    noResult: "没有记录！",
    total: "总数",
    addTicketDialog: {
        dialogTitle: "新增服务工单",
        category: "分类",
        title: "标题",
        description: "描述",
        bike: "整车",
        sn: "编号",
        serviceCenter: "服务中心",
        image: "图片",
        video: "视频",
        clickToUpload: "点击上传视频"
    },
};

const SCTicketDetail = {
    back: "返回",
    transferToUp: "提交给上级",
    transferToInside: "提交给内部员工",
    take: "领取工单",
    history: "历史记录",
    ticketInfo: {
        ticketTitle: "投诉信息",
        serviceNo: "服务编号",
        createTime: "创建时间",
        status: "状态",
        serviceCenter: "服务中心",
        title: "标题",
        description: "描述",
        image: "图片",
        video: "视频",
    },
    userInfo: {
        address: "公司地址",
        website: "公司网址",
        email: "邮箱",
    },
    content: "内容",
    image: "图片",
    sendMessage: "发送消息",
    historyTicket: {
        title: "工单历史记录",
        name: "操作者",
        operation: "动作",
        time: "操作时间",
    },
    handleInside: {
        title: "提交给内部员工",
        tip: "请选择一个员工提交给他工单",
        select: "请选择",
        confirm: "确认",
        cancel: "取消"
    }
};

const ServiceCenter = {
    centerTicketList: SCCenterTicketList,
    handleUpTicketList: SCHandleUpTicketList,
    myTicketList: SCMyTicketList,
    ticketDetail: SCTicketDetail
};

// 零部件相关
// 零部件相关 --- component
const ComponentComponentDetail = {
    back: "返回",
    sn: "序列号",
    category: "分类",
    modelName: "型号名称",
    partNo: "物料号",
    orderNo: "订单号",
    customerName: "客户名",
    customerPartNo: "客户物料号",
    customerPoNo: "客户订单号",
    produceTime: "生产日期",
    saleTime: "销售日期",
};

const ComponentComponentList = {
    yes: "是",
    no: "否",
    search: {
        poNo: "物料号",
        orderNo: "订单号",
        modelName: "型号名称",
        sn: "序列号",
        customerName: "客户名",
        search: "搜索",
        export: "导出",
    },
    headData: {
        poNo: "物料号",
        orderNo: "订单号",
        sn: "序列号",
        category: "分类",
        modelName: "型号名称",
        customerName: "客户名",
        operation: "操作"
    },
    noResult: "没有记录！",
    total: "总数",
    detail: "详情",
    delete: "删除",
    batchDelete: "批量删除",
    deleteDialog: {
        title: "删除零部件",
        content: "你确定要删除吗?",
    },
    exportDialog: {
        title: "导出零部件",
        startNum: "开始数",
        endNum: "结束数",
        export: "导出",
        cancel: "取消"
    },
    back: "返回",
};

const ComponentComponent = {
    Detail: ComponentComponentDetail,
    List: ComponentComponentList
};

// 零部件相关 --- TestFeedback
const ComponentTestFeedbackList = {
    search: {
        email: "Email",
        searchBtn: "Search",
    },
    add: "Add",
    noResult: "No result",
    total: "Total",
    deleteDialog: {
        title: "Delete Feedback",
        content: "Are you sur to delete it?",
        yes: "Yes",
        no: "No",
    },
    headData: {
        date: "date",
        email: "Email",
        fileName: "File Name",
        status: "Status",
        operation: "Operation",
        desc: "Desc"
    },
    detail: "Detail",
    delete: "Delete",
};

const ComponentTestFeedbackDetail = {
    back: 'Back',
    fileDetail: 'Feedback Info',
    edit: 'Edit',
    delete: 'Delete',
    detail: {
        title: 'File Info',
        email: 'Email',
        fileName: 'FileName',
        setTime: 'Set Time',
        status: 'Status',
        upload: 'UpLoad Feedback',
        desc: 'Desc'
    },
    deleteDialog: {
        title: "Delete Feedback",
        content: "Are you sur to delete it?",
        yes: "Yes",
        no: "No",
    },
    fileList: 'fileList',
    saveFeedback: 'Save Feedback',
    deleteFile: 'Delete'
};

const ComponentTestFeedbackSave = {

    createFeedback: 'Create Feedback',
    feedbackInfo: 'Feedback Info',

    email: 'Email',
    fileName: 'FileName',
    setTime: 'Set Time',
    upload: 'UpLoad Feedback',
    status: 'Status',
    desc: 'Desc',

    file: 'Upload',
    clickUpload: 'Click UpLoad',

    delete: "Delete",
    download: "Download",
    fileList: 'fileList',
    saveFeedback: 'Save Feedback',
    deleteFile: 'Delete',
};

const ComponentTestFeedback = {
    List: ComponentTestFeedbackList,
    Save: ComponentTestFeedbackSave,
    Detail: ComponentTestFeedbackDetail
};
const ComponentMaterialList = {
    orderNo: "订单号",
    clientCode: '客户编号',
    modelDes: "型号介绍",
    numQty: "测试数/数量",
    status: "状态",
    proLine: "生产线",
    date: "日期",
    remark: "备注",
    operation: "操作",

    startDate: "开始日期",
    endDate: "结束日期",
    connError: "网络连接错误",
    search: "搜索",
    batchDelete: "批量删除",
    upload: "上传文件",
    dialogUploadText: "上传生产订单文件",
    deleteDialogTitle: "删除生产订单",
    deleteDialogContent: "你确定要删除吗?",
    yes: "确定",
    no: "取消",
    edit: "编辑",
    editDialogTile: "更新信息",
    noResult: "没有结果",
    total: "共",

    delete: '删除',
    detail: '详情',

    all: "所有",
    line1: "一号线",
    line2: "二号线",
    line3: "三号线",
    line4: "四号线",
    line5: "五号线",
    line6: "六号线",
    line7: "七号线",
    s1: "计划中",
    s2: "备料中",
    s3: "装配中",
    s4: "已入库",
    s5: "测试中",
    s6: "装配异常",
    s7: "测试异常",

};

const ComponentMaterialDetail = {
    back: '返回',
    delete: '删除',
    title1: '订单信息',
    title2: '状态更新',
    orderNo: '订单号',
    user: '销售',
    status: '状态',
    desc: '介绍',
    qty: "数量",
    num: "测试数",
    drawing: "图纸",
    stator: "定子",
    exterior: "外观",

    deleteDialog: {
        title: "删除生产订单",
        content: " 你确定要删除吗?",
        yes: "确定",
        no: "取消",
    },

    scanning: '扫描 二维码',

    show1: "显示装配异常二维码",
    show11: "装配异常",
    show2: "显示测试异常二维码",
    show22: "测试异常",
    connError: "网络连接错误",

    s1: "计划中",
    s2: "备料中",
    s3: "装配中",
    s4: "已入库",
    s5: "测试中",
    s6: "装配异常",
    s7: "测试异常",
};
const ComponentMaterial = {
    List: ComponentMaterialList,
    Detail: ComponentMaterialDetail,
};
// 零部件相关 --- order
const ComponentOrderDetail = {
    back: "返回",
    orderDetail: "订单详情",
    edit: "编辑",
    delete: "删除",
    orderInfo: {
        title: "订单信息",
        orderNo: "订单号",
        customerPartNo: "客户物料号",
        note: "备注",
        customerName: "客户名",
        customerPo: "客户订单号",
    },
    itemsList: "型号列表",
    produce: "生产",
    testReport: "测试报告",
    componentList: "产品列表",
    replace: "替换",
    headData: {
        modelName: "型号名称",
        articleNo: "物料号",
        customerArticleNo: "客户物料号",
        quantity: "数量",
        operation: "操作",
    },
    deleteDialog: {
        title: "删除订单",
        content: "确定要删除该订单吗?",
        yes: "是",
        no: "否"
    },
    category: "分类",
    search: "搜索",
    clear: "清除",
    selectCategory: "选择分类",
    modelNamePN: "型号/物料号",
    confirmDialog: {
        replace: "替换",
        yes: "是",
        no: "否"
    },

    dataConnect: "数据绑定"
};

const ComponentOrderList = {
    search: {
        orderNo: "订单号",
        searchBtn: "搜索",
        clearBtn: "清除",
    },
    add: "新增",
    list: "列表",
    detail: "详情",
    delete: "删除",
    noResult: "没有记录！",
    total: "总数",
    headData: {
        date: "日期",
        orderNo: "订单号",
        customerName: "客户名",
        pQ: "已生产/数量数",
        status: "状态",
        operation: "操作",
        model: "型号",
    },
    deleteDialog: {
        title: "删除订单",
        content: "确定要删除该订单吗?",
        yes: "是",
        no: "否"
    },
    op: Operation,

};

const ComponentOrderProduce = {
    back: "返回",
    connect: "连接",
    reset: "重置",
    update: "更新",
    readSn: "读入序列号",
    writeSn: "写入序列号",
    orderInfo: {
        title: "订单信息",
        model: "型号",
        customerName: "客户名称",
        customerPoNo: "客户订单号",
        customerPartNo: "客户物料号",
    },
    programingInfo: {
        title: "程序",
        com: "端口",
        file: "文件",
        pleaseSelect: "请选择"
    },
    writeSnInfo: {
        title: "写入序列号",
        com: "端口",
        pleaseSelect: "请选择",
        sn: "序列号"
    },
    cNam: AttributeName,
    up: Update,
};

const ComponentOrderDataConnect = {
    back: "返回",
    save: "绑定",
    clear: "清空",
    removeBinding: '解除绑定',
    orderInfo: {
        title: "订单信息",
        model: "型号",
        customerName: "客户名称",
        customerPoNo: "客户订单号",
        customerPartNo: "客户物料号",
    },
    programingInfo: {
        ListTitle: "状态列表",
        searchTitle: '搜索/扫描',
        motorTitle: 'Motor',
        result: 'Result',
        pleaseSelect: "search empty",
        sn: 'SN',
        cr: '控制器',
        sr: '传感器',
        mm: '电机'
    },

};

const ComponentOrderSave = {
    back: "返回",
    createOrder: "创建订单",
    orderInfo: {
        title: "订单详情",
        poNo: "订单号",
        customerInfo: "客户信息",
        customerPartNo: "客户物料号",
        customerName: "客户名",
        customerPo: "客户订单号",
        note: "备注",
    },
    itemList: "型号列表",
    addItem: "添加型号",
    addItems: "添加型号",
    delete: "删除",
    saveOrder: "保存订单",
    tableHead: {
        name: "名称",
        articleNo: "物料号",
        customerArticleNo: "客户物料号",
        quantity: "数量",
        operation: "操作",
    },
    itemHeadData: {
        image: "图片",
        name: "名称",
        partNo: "物料号"
    },
    category: "分类",
    search: "搜索",
    clear: "清除",
    selectCategory: "选择分类",
    modelNamePN: "型号/物料号"
};

const ComponentOrderTestReport = {
    back: "返回",
    cNam: AttributeName
};

const ComponentOrder = {
    Detail: ComponentOrderDetail,
    List: ComponentOrderList,
    Produce: ComponentOrderProduce,
    Save: ComponentOrderSave,
    TestReport: ComponentOrderTestReport,
    DataConnect: ComponentOrderDataConnect
};

// 零部件相关 --- product
const ComponentProductBomList = {
    back: "返回",
    add: "新增",
    save: "保存",
    edit: "编辑",
    print: "打印",
    export: "导出",
    delete: "删除",
    detail: "详情",
    articleNO: "物料号",
    noPhoto: "无图片",
    viewLargerImage: "浏览大图",
    list: "列表",
    partName: "物料名称",
    yes: "是",
    no: "否",
    listHead: {
        itemNo: "型号编号",
        partNo: "物料号",
        picture: "图片",
        name: "名称",
        spec: "尺寸",
        package: "包",
        packageSize: "容量",
        price: "价格",
        linkBom: "LINK BOM",
        operation: "操作"
    },
    selectHead: {
        partNo: "物料号",
        picture: "图片",
        name: "名称",
        spec: "尺寸",
        package: "包",
        packageSize: "容量",
        price: "价格",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "删除产品",
        content: "确定要删除该产品吗?",
    },
    addItemDialog: {
        title: "添加型号",
    },
    operation: "操作",
    tip: "备注",
    content01: "最好上传有背景的正方形图片",
    content02: "支持JPG，PNG， 300*300/400*400, 不超过3MB"
};

const ComponentProductDetail = {
    back: "返回",
    copy: "复制",
    edit: "编辑",
    delete: "删除",
    model: "型号",
    partNo: "物料号",
    name: '名称',
    articleList: "物料列表",
    desc: "描述",
    download: "下载",
    preview: "预览",
    history: "历史记录",
    update: "上传更新",
    view: "查看",
    hardware: "固件版本",
    firmware: "版本号",
    bomList: "部件列表",
    yes: "是",
    no: "否",
    confirm: "确认",
    cancel: "取消",
    deleteProductDialog: {
        title: "删除产品",
        content: "确定要删除该产品吗?",
    },
    deleteFirmwareDialog: {
        title: "删除软件",
        content: "你确定要删除这个软件吗? ",
    },
    historyDialog: {
        title: "固件历史记录",
        version: "版本",
        remark: "备注",
        updateTime: "上传时间",
        operation: "操作",
        noResult: "没有记录！"
    },
    editFirmwareDialog: {
        title: "上传固件",
        file: "文件",
        version: "版本号",
        remark: "备注",
        clickToUpload: "点击上传",
    },
    updateFirmwareDialog: {
        title: "上传固件",
        file: "文件",
        version: "版本",
        remark: "备注",
        clickToUpload: "点击上传",
    },
    training: '培训视频',
    set: ArticleSet

};

const ComponentProductEditCategory = {
    back: "返回",
    category: "分类",
    yes: "是",
    no: "否",
    confirm: "确认",
    cancel: "取消",
    deleteDialog: {
        title: "删除分类",
        content: "确定要删除该分类吗?",
    },
    addSubCategoryDialog: {
        title: "添加子分类",
        namePH: "分类名称"
    },
    editCategoryDialog: {
        title: "编辑分类",
        namePH: "分类名称"
    },
    addSubCategory: "添加子分类",
    edit: "编辑",
    delete: "删除",
};

const ComponentProductIndex = {
    editCategory: "编辑分类",
    training: 'Training'
};

const ComponentProductList = {
    add: "新增",
    noResult: "没有记录！",
    total: "总数",
    set: ArticleSet
};

const ComponentProductSave = {
    back: "返回",
    logo: "图标",
    name: "名称",
    partNo: "物料号",
    desc: "描述",
    save: "保存",
    tip: {
        title: "备注",
        content01: "最好上传有背景的正方形图片",
        content02: "支持JPG，PNG， 300*300/400*400, 不超过3MB",
    },
    add: "新增",
    copy: "复制",
    edit: "编辑",
    set: ArticleSet

};

const ComponentVideoList = {
    back: "返回",
    category: "分类",
    add: "添加",
    download: "下载",
    delete: "删除",
    noResult: " 没有记录!",
    total: "总数",
    uploadVideo: "上传视频",
    upload: "上传",
    confirm: "确认",
    cancel: "取消",
    all: "全部",
    systemError: "系统错误",
    hmi: "仪表",
    motor: "电机",
    controller: "控制器",
    sensor: "传感器",
    battery: "电池",
    charger: "充电器",
    name: "名称",
    operation: "操作",
    videoEmptyTip: "请上传视频",
    saveSuccess: "保存成功",
    yes: "是",
    no: "否",
    deleteVideo: "删除视频",
    deleteTip: "你确定要删除它吗？",
    preview: "预览",
    errorCode: "错误编码",
    link: "链接",
    edit: "编辑"
};

const ComponentProductVideoList = {
    back: "返回",
    category: "分类",
    addVideo: "添加视频",
    download: "下载",
    delete: "删除",
    noResult: "没有记录!",
    confirm: "确认",
    cancel: "取消",
    systemError: "系统错误",
    hmi: "仪表",
    motor: "电机",
    controller: "控制器",
    sensor: "传感器",
    battery: "电池",
    charger: "充电器",
    name: "名称",
    operation: "操作",
    yes: "是",
    no: "否",
    preview: "预览",
    deleteVideo: "删除视频",
    deleteTip: "你确定要删除它吗？",
};

const ComponentProduct = {
    BomList: ComponentProductBomList,
    Detail: ComponentProductDetail,
    EditCategory: ComponentProductEditCategory,
    Index: ComponentProductIndex,
    List: ComponentProductList,
    Save: ComponentProductSave,
    VideoList: ComponentVideoList,
    ItemVideoList: ComponentProductVideoList,
};

// 零部件相关 --- service-center
const ComponentSCStat = {
    ticketStat: "工单统计",
    ticketByCategory: "工单分类统计图",
};

const ComponentSC = {
    Stat: ComponentSCStat
};

const Component = {
    Component: ComponentComponent,
    Order: ComponentOrder,
    Product: ComponentProduct,
    ServiceCenter: ComponentSC,
    TestFeedback: ComponentTestFeedback,
    Material: ComponentMaterial,
};

// 经销商相关
const DealerIndex = {
    order: "订单",
    diagnosis: "诊断"
};

// 经销商相关 --- error
const DealerErrorShooting = {
    errorCode: "错误代码",
    name: "名称",
    remark: "备注",
    description: "描述",
    target: "检查对象",
    solution: "解决方案",
    tip: "提示",
    tipContent: "如果以上解决方案不能解决你的问题，请联系品牌商",
    help: "帮助",
    helpContent: "请点击这里获取更多支持!",
    link: "视频入口"
};

const DealerError = {
    Shooting: DealerErrorShooting,
};

// 经销商相关 --- partner
const DealerPartnerList = {
    back: "返回",
    acceptInvitation: "接受邀请",
    tableHead: {
        name: "名称",
        phone: "电话",
        website: "网址",
        country: "国家",
        operation: "操作",
    },
    currentBrand: "当前品牌",
    noResult: "没有记录！",
    total: "总数",
    brandInfo: {
        title: "品牌信息",
        logo: "商标",
        name: "名称",
        type: "类型",
        email: "邮箱",
        website: "网址",
        tel: "座机",
        postcode: "邮编",
        address: "地址",
    },
    invitationCode: "邀请码",
    confirm: "确认",
    cancel: "取消",
    webLink: "网站链接",
    createOrder: "创建订单",
    orderList: "订单列表",
    view: "查看",
};

const DealerPartner = {
    List: DealerPartnerList,
};

// 经销商相关 --- sale
const DealerSaleDetail = {
    back: "返回",
    orderDetail: "订单详情",
    view: "查看",
    edit: "编辑",
    delete: "删除",
    invoiceNo: "发票编号",
    invoiceDate: "发票日期",
    submit: "提交",
    customerInfo: {
        title: "客户信息",
        name: "名字",
        phone: "电话",
        email: "邮箱",
    },
    bikeInfo: {
        title: "整车信息",
        brand: "品牌",
        bikeId: "整车编号",
        model: "型号",
        warrantyPeriod: "保修期"
    },
    detail: {
        title: "订单详情",
        orderNo: "订单编号",
        price: "单价",
        invoice: "发票",
        salesDate: "销售日期",
        remark: "备注",
    },
    deleteDialog: {
        title: "删除订单",
        content: "你确定要删除该订单吗?",
        yes: "是",
        no: "否"
    }
};

const DealerSaleEdit = {
    back: "返回",
    save: "保存",
    orderEdit: "订单编辑",
    customerInfo: {
        title: "整车信息",
        name: "名字",
        phone: "电话",
        email: "邮箱",
    },
    bikeInfo: {
        title: "整车信息",
        brand: "品牌",
        bikeId: "整车编号",
        model: "车型",
        warrantyPeriod: "保修期"
    },
    detail: {
        title: "订单详情",
        orderNo: "订单编号",
        price: "单价",
        invoice: "发票",
        remark: "备注",
        invoiceDate: "发票日期",
    },
};

const DealerSaleList = {
    add: "新增",
    tableHead: {
        orderNo: "订单编号",
        bikeId: "整车编号",
        model: "车型",
        customerName: "客户名称",
        date: "日期",
        status: "状态",
        operation: "Operation"
    },
    noResult: "没有记录！",
    total: "总数",
    detail: "Detail",
    search: {
        bikeId: "整车编号",
        searchBtn: "搜索",
        clearBtn: "清空"
    }
};

const DealerSaleSave = {
    back: "返回",
    addComponent: "新增零件",
    save: "保存",
    confirm: "确认",
    cancel: "取消",
    customerInfo: {
        title: "顾客信息",
        name: "姓名",
        phone: "电话",
        email: "邮箱",
        tip: "备注",
        tip01: "邮箱与售后服务和钥匙相关，",
        tip02: "请确保邮箱有效。"
    },
    bikeInfo: {
        title: "整车信息",
        bikeId: "整车编号",
        brand: "品牌",
        warrantyPeriod: "保修期",
        date: "日期",
        price: "单价",
        remake: "备注",
    },
    componentInfo: {
        title: "零部件信息",
        replace: "替换",
        add: "新增",
        sn: "序列号",
        name: "名称"
    },
    replaceDialog: {
        oldSn: "旧序列号",
        newSn: "新序列号",
        category: "类别",
    },
    tipDialog: {
        title: "提示",
        content: "该整车没有电池，你确定要保存订单吗?"
    }
};

const DealerSale = {
    Detail: DealerSaleDetail,
    Edit: DealerSaleEdit,
    List: DealerSaleList,
    Save: DealerSaleSave,
};

// 经销商相关 --- ticket
const DealerTicketDetail = {
    back: "返回",
    markCompleted: "标记完成",
    ticketInfo: {
        InfoTitle: "投诉信息",
        serviceNo: "服务编号",
        status: "状态",
        createTime: "创建时间",
        serviceCenter: "服务中心",
        title: "标题",
        description: "描述",
        image: "图片",
        video: "视频",
    },
    user: {
        address: "地址",
        website: "网址",
        email: "邮箱",
    },
    replay: {
        content: "内容",
        image: "图片",
        sendMessage: "发送消息"
    },
};

const DealerTicketList = {
    add: "新增",
    confirm: "确定",
    cancel: "删除",
    tableHead: {
        serviceNo: "服务编号",
        title: "标题",
        targetNo: "目标序列号",
        ServiceCenter: "服务中心",
        createTime: "创建时间",
        status: "状态",
        operation: "操作",
    },
    detail: "详情",
    delete: "删除",
    noResult: "没有记录！",
    total: "总数",
    addTicketDialog: {
        dialogTitle: "新增服务工单",
        category: "分类",
        title: "标题",
        description: "描述",
        bike: "整车",
        sn: "编号",
        serviceCenter: "服务中心",
        image: "图片",
        video: "视频",
        clickToUpload: "点击上传视频"
    },
    deleteTicket: {
        title: "删除工单",
        content: "你确定要删除该工单吗? "
    }
};

const DealerTicket = {
    Detail: DealerTicketDetail,
    List: DealerTicketList
};

const DealerVehicleSearch = {
    search: {
        bikeId: "整车序列号",
        searchBtn: "搜索",
        clear: "清空"
    },
    back: "返回",
    sn: "序列号",
    key: "",
    productTime: "生产时间",
    itemName: "车型",
    itemPartNo: "物料号",
    orderSn: "订单序列号",
    orderSourceId: "订单物料号",
    customerId: "客户ID",
    customerPartNo: "客户物料号",
    customerPoNo: "客户订单号",
    replace: "替换",
    oldSn: "旧序列号",
    newSn: "新序列号",
    confirm: "确定",
    cancel: "取消",
    save: "保存",
    edit: "编辑",
    reportList: "诊断报告",
    detail: "详情",
    operation: "操作",
    date: "日期",
    author: "提交人",
    emptyListTip: "该车还没有诊断报告",
    part: "零部件",
    history: "更换记录",
    brand: "品牌商",
    saleTime: "销售时间",
    customerName: "客户姓名",
    customerEmail: "客户邮箱"
};

const DealerVehicle = {
    Search: DealerVehicleSearch
};

const Dealer = {
    Index: DealerIndex,
    Error: DealerError,
    Partner: DealerPartner,
    Sale: DealerSale,
    Ticket: DealerTicket,
    Vehicle: DealerVehicle
};

const BrandIndex = {
    dailyVehicleOrderStatistics: "每日车辆订单统计",
    dailyVehicleAfterSalesStatistics: "每日车辆售后统计",
    dailyDailyVehicleStatistics: "每日经销商车辆统计",
    dailyAssemblerVehicleStatistics: "每日组装车辆统计"
};

//品牌商
const BrandList = {
    add: "添加",
    edit: "编辑",
    delete: "删除",
    noResult: "没有记录！",
    total: "总数",
    deleteBrand: "删除品牌",
    deleteTip: "你确定要删除它吗 ?",
    yes: "是",
    no: "否",
    addBrand: "添加品牌",
    brandInfo: "品牌信息",
    logo: "图片",
    uploadTipOne: "最好上传有背景的正方形图片",
    uploadTipTwo: "支持JPG，PNG， 300*300/400*400, 不超过3M",
    name: "名字",
    introduction: "介绍",
    confirm: "确认",
    cancel: "取消",
    operation: "操作",
    emptyTip: {
        logo: "请上传品牌图标",
        name: "请输入品牌名称",
        desc: "请输入品牌介绍",
    },
    editSuccess: "修改成功",
    addSuccess: "添加成功",
    deleteSuccess: "删除成功",

    link: "Link",
};

const DealerInvite = {
    codeStatus: "邀请码状态",
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        invitation: "邀请码",
        targetName: "公司名称",
        contact: "联系人",
        targetEmail: "目标邮箱",
        createTime: "创建时间",
    },
    codeStatusOptions: {
        readyToSend: "未发送",
        sentNotUsed: "已发送但未激活",
        used: "已激活",
    },
    emptyTip: {
        email: "请输入邮箱",
        name: "请输入公司名称",
    },
    sendSuccess: "发送成功",
    uploadSuccess: "上传成功",
    selectTip: "请选择账户",
    deleteSuccess: "删除成功",
    newCode: "创建邀请",
    batchSend: "批量发送",
    send: "发送",
    delete: "删除",
    dealerInvitation: "邀请经销商",
    selectAccount: "选择账号",
    uploadContacts: "上传联系人",
    operation: "操作",
    confirm: "确认",
    cancel: "取消",
    deleteTip: "你确定删除它吗? ",
    yes: "是",
    no: "否",
    email: "邮箱",
    companyName: "公司名称",
    toLeft: "左移",
    toRight: "右移",
    deleteCode: "删除邀请码",
    orgName: "公司名字",
    uploadFailList: "上传失败列表",
    reason: "结果",
    uploadFailTip: "上传失败，请检查上传文件或者所有的联系人都已经存在了",
    resend: "重新发送",
    template: "联系人模版",
    download: "下载",

    source: "来源",
    target: "目标",
    enterKeyWord: "输入关键词",
    noData: "没有数据",
    dealerType: "经销商类型",
    dealerType1: "经销商",
    dealerType2: "特殊经销商",
    dealerTypeNote: "特殊经销商只有诊断和工具部分的功能。经销商有全部功能",
    op: Operation,

};

const DealerList = {
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        logo: "图标",
        name: "名字",
        postCode: "邮编",
        tel: "电话",
        website: "网站",
        address: "地址",
        operation: "操作",
        email: "邮箱",
    },
    delete: "删除",
    deleteDialog: {
        title: "删除经销商",
        tip: "确定要删除经销商吗?"
    },
    yes: "是",
    no: "否",
    op: Operation,
};

const OemInvite = {
    codeStatus: "邀请码状态",
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        invitation: "邀请码",
        targetName: "公司名称",
        contact: "联系人",
        targetEmail: "目标邮箱",
        createTime: "创建时间",
    },
    codeStatusOptions: {
        readyToSend: "未发送",
        sentNotUsed: "已发送但未激活",
        used: "已激活",
    },
    emptyTip: {
        email: "请输入邮箱",
        name: "请输入公司名称",
    },
    sendSuccess: "发送成功",
    uploadSuccess: "上传成功",
    selectTip: "请选择账户",
    deleteSuccess: "删除成功",
    newCode: "创建邀请",
    batchSend: "批量发送",
    send: "发送",
    delete: "删除",
    oemInvitation: "邀请组装厂",
    selectAccount: "选择账号",
    uploadContacts: "上传联系人",
    operation: "操作",
    confirm: "确认",
    cancel: "取消",
    deleteTip: "你确定删除它吗? ",
    yes: "是",
    no: "否",
    email: "邮箱",
    companyName: "公司名称",
    toLeft: "向左",
    toRight: "向右",
    deleteCode: "删除邀请码",
    orgName: "公司名字",
    uploadFailList: "上传失败列表",
    reason: "结果",
    uploadFailTip: "上传失败，请检查上传文件或者所有的联系人都已经存在了",
    resend: "重新发送",
    download: "下载",
    template: "模板",
    op: Operation,

};

const OemList = {
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        logo: "图标",
        name: "名字",
        postCode: "邮编",
        tel: "电话",
        website: "网站",
        address: "地址",
        operation: "操作",
        email: "邮箱",
    },
    delete: "删除",
    deleteDialog: {
        title: "删除组装厂",
        tip: "确定要删除组装厂吗?"
    },
    yes: "是",
    no: "否",
    op: Operation,
};

const AuthList = {
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        name: "名称",
        type: "类型",
        status: "状态",
        time: "日期",
        operation: "操作"
    },
    authorization: "授权",
    authorizationSuccess: "授权成功",
    authorized: "已授权",
    noAuthorization: "未授权",
    hmi: "仪表",
    controller: "控制器"
};


const RecordList = {
    noResult: "没有记录",
    total: "总数",
    tableHead: {
        sn: "SN",
        vehicle: "车辆",
        operator: "账户类别",
        account: "账户",
        status: "状态",
        time: "时间",
        type: '类型',
        data: "数据"
    },
    status: "状态",
    recordTitle: "零部件SN操作记录",
    edit: "编辑"
};


const ModelDetail = {
    back: "返回",
    delete: "删除",
    edit: "编辑",
    copy: "复制",
    name: "名称",
    brand: "品牌",
    partNo: "物料号",
    description: "描述",
    deleteModel: "删除车型",
    deleteTip: "你确定要删除它吗 ?",
    yes: "是",
    no: "否",
    download: "下载",
    preview: "预览",
    cancel: "取消",
    confirm: "确认",
    view: "查看",
    bom: "部件",

    specification: "规格书",
    currency: "币种",
    year: "年份",
    color: "颜色",
    size: "尺寸",
    gender: "性别",
    price: "价格",

    link: "链接",
};

const ModelList = {
    brand: "品牌",
    addModel: "添加车型",
    noResult: "没有记录！",
    total: "总数",
    deleteModel: '删除车型',
    deleteTip: "你确定要删除它吗 ?",
    yes: "是",
    no: "否",
    edit: "编辑",
    delete: "删除",
    detail: "详情",
    tableHead: {
        name: "名称",
        logo: "图片",
        partNo: "物料号",
        introduction: "介绍",
        operation: "操作",
        show: "显示"
    },
    deleteSuccess: "删除成功",
    link: "链接",
    op: Operation,
};

const ModelSave = {
    uploadTipOne: "最好上传有背景的正方形图片",
    uploadTipTwo: "支持JPG，PNG， 300*300/400*400, 不超过3M",
    add: "添加",
    edit: "编辑",
    copy: "复制",
    back: "返回",
    save: "保存",
    emptyTip: {
        picture: "请上传车型图片",
        name: "请输入车型名称",
        partNo: "请输入车型物料号",
        desc: "请输入车型描述",
    },
    saveTip: "请等待保存",
    brandWrong: "品牌错误，请重新选择",
    saveSuccess: "保存成功",
    picture: "图片",
    name: "名字",
    partNo: "物料号",
    introduction: "介绍",
    description: "描述",
    cancel: "取消",
    confirm: "确认",
    editAttr: "编辑属性",
    attrList: "属性列表",

    specification: "规格书",
    currency: "币种",
    year: "年份",
    color: "颜色",
    size: "尺寸",
    gender: "性别",
    price: "价格",
    brand: "品牌",
    link: "链接",
};


const VehicleBomList = {
    back: "返回",
    add: "新增",
    save: "保存",
    edit: "编辑",
    print: "打印",
    export: "导出",
    delete: "删除",
    detail: "详情",
    articleNO: "物料号",
    noPhoto: "无图片",
    viewLargerImage: "浏览大图",
    list: "列表",
    partName: "型号名称/物料号",
    yes: "是",
    no: "否",
    listHead: {
        itemNo: "型号编号",
        partNo: "物料号",
        picture: "图片",
        name: "名称",
        spec: "尺寸",
        package: "包",
        packageSize: "容量",
        price: "价格",
        linkBom: "LINK BOM",
        warranty: "保修期",
        operation: "操作"
    },
    selectHead: {
        partNo: "物料号",
        picture: "图片",
        name: "名称",
        spec: "尺寸",
        package: "包",
        packageSize: "容量",
        price: "价格",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "删除产品",
        content: "确定要删除该产品吗?",
    },
    addItemDialog: {
        title: "添加型号",
    },
    operation: "操作",
    selectCategory: "选择分类",
    category: "分类",
    search: "搜索",
    clear: "重置",
    tip: "备注",
    content01: "最好上传有背景的正方形图片",
    content02: "支持JPG，PNG， 300*300/400*400, 不超过3MB"
};

const Brand = {
    Index: BrandIndex,
    BrandList: BrandList,
    DealerInvite: DealerInvite,
    DealerList: DealerList,
    OemInvite: OemInvite,
    OemList: OemList,
    AuthList: AuthList,
    ModelDetail: ModelDetail,
    ModelList: ModelList,
    ModelSave: ModelSave,
    VehicleDetail: VehicleDetail,
    VehicleList: VehicleList,
    VehicleBomList: VehicleBomList,

    RecordList: RecordList
};

//零件厂 account
const ComponentsAccount = {
    add: "添加",
    self: "自己",
    manager: "管理员",
    permission: "权限",
    reset: "重置密码",
    delete: "删除",
    total: "总数",
    addAccount: "添加账户",
    resetPassword: "重置密码",
    deleteAccount: "删除账户",
    email: "邮箱",
    confirm: "确定",
    cancel: "取消",
    deleteTip: "你确定删除它吗? ",
    yes: "是",
    no: "否",
    resetPasswordTip: "你确定将该账号的密码重置为 12345678 ？",
    tableHead: {
        account: "账户",
        createTime: "创建时间",
        operation: "操作",
    },
    emptyEmailTip: "请输入邮箱",
    addSuccess: "添加成功",
    deleteSuccess: "删除成功",
    resetSuccess: "重置成功",
    saveAuthSuccess: "保存权限成功",
    noResult: "没有记录！",
    invitationLength1: "经销商角色只能创建最多10个子账户",
    invitationLength2: "组装厂角色只能创建最多20个子账户",
    invitationLength3: "品牌商角色只能创建最多20个子账户",
    invitationLength4: "零件商角色只能创建最多20个子账户",

    cNam: AttributeName
};

const ComponentsReplaceReport = {
    tableHead: {
        category: "分类",
        oldComponent: "旧零部件序列号",
        newComponent: "新零部件序列号",
    },
    noResult: "没有记录！",
    total: "总数",
    back: "返回"
};

const report = {
    back: "返回",
    diagnosisReportTitle: "整车诊断报告",
    print: "打印",
    orgLabel: "公司",
    orgTitle: "公司信息",
    dealerLabel: "经销商",
    dealerTitle: "经销商信息",
    assemblerLabel: "组装厂",
    assemblerTitle: "组装厂信息",
    name: "名称",
    address: "地址",
    email: "邮箱",
    tel: "电话",
    reportDate: "报告日期",
    bike: {
        title: "整车信息"
    },
    diagnosis: {
        data: "诊断数据",
        result: "诊断结果",
        hmi: "诊断数据 - 仪表",
        battery: "诊断数据 - 电池",
        controller: "诊断数据 - 控制器",
        sensor: "诊断数据 - 助力传感器",
    },
    cNam: AttributeName,

};

// components diagnosis
const diagnosis3rd = {
    goElectronic: "要跳转到电子诊断吗？",
    goMechanic: "要跳转到机械诊断吗？",
    diagnosisElectronic: "电子诊断",
    diagnosisMechanic: "机械诊断",
    eBikeID: "电动自行车车架号",
    connection: "连接",
    newVer: "新软件",
    ok: "正常",
    fail: "失败",
    con: "已连接",
    disCon: "没有连接",
    readOk: "读取成功",
    readFail: "读取失败",
    detail: "详情",
    testSys: "测试系统",
    cNam: AttributeName,
    rp: report,
    op: Operation,
    step: "步骤",
    help: "帮助",
    continue: "继续",
    case: "情况",
    test: "测试",
    finish: "完成",
    name: "名称",
    remark: "备注",
    desc: "描述",
    target: "检查目标",
    solution: "解决方案",
    errTitle: "错误解决方案",
    reconnect: "重新连接",
    instruction: "说明",

    pedalSensor: "助力传感器",
    throttle: "转把",
    brake: "刹把",
    light: "大灯",
    support: "档位模式",
    walk: "助推功能",
    speed: "速度信号",
    cadence: "踏频传感器",
    torque: "力矩传感器",
    motor: "电机",

    click: "点击",
    step11: "连接BESST工具盒子到电脑",
    step12: "选择正确的COM口",
    step13: "指示灯会变绿",
    step21: "选择电动自行车上的零部件",
    step22: '把BESST工具盒子连接到仪表和控制器中间，按一下工具盒子的复位键',
    step23: '按住仪表电源键切换仪表进入PC模式',
    step232: '按住仪表电源键4S开启系统',
    case11: "如果仪表不能打开:",
    case111: "如果系统没有打开:",
    case12: "请检查线的连接。如果仍然打不开仪表, 你需要一个新仪表再继续。",
    case121: "请检查线的连接。拔掉仪表然后再接上。",
    case13: '仪表没有显示“PC”模式',
    case17: "请按盒子上",
    case18: '按钮。可以多按几次。',
    case181: "按钮。如果仍然打不开仪表, 你需要一个新仪表再继续。",
    case20: '如果仪表还没有“PC”模式, 仪表不支持BESST， 你需要一个新仪表再继续。',
    case21: '注意: 如果你换了一个仪表, 请点击“读取”来重新连接。',
    test11: "悬挂自行车来防止受伤或损坏",
    test12: "在按下按钮之前，确保滚轮可以自由旋转!",
    test13: "选择要测试的功能",
    test14: '选择 "All" 或 "S", "All" 是整体测试, "S" 是逐一测试;',
    test15: "开始测试流程",
    createReport: "创建报告",

    start11: "Please rotate the throttle, then motor should work, and also you should see the green light。",
    start13: "If motor works but no green light, throttle is ok。Otherwise, please click",
    start14: "and then replace the part。",
    start15: "Please brake and release the brake lever at least two times, then you should see green light。",
    start16: "If the motor do stop but no green light, brake switch is ok。Otherwise, please click",
    start17: 'Please press and hold the button " ',
    start18: ' " on the HMI, then you should see the light on or off, and also you should see the green light。',
    start19: "If the light doesn't work, but you still see the green light, then replace the light。If no light works or you see don't green light, please click",
    start20: "and then replace display unit or controller。",
    start21: ' Please press "+" and "-" alternately, then you should see the green light。',
    start22: "If no green light, please click",
    start23: "and then replace the Display。",
    start24: 'Please press and hold the button "',
    start25: '" or "--", then the motor should work at slow speed, and also you should see the green light。',
    start26: "Please turn the wheel forward by hand, then you will see the speed above and get the green light。",
    start27: "If no green light, please adjust the magnet in the right induction position。If still doesn't work, please click",
    start28: "and then replace the sensor and magnet。",
    start29: "Suspend the bicycle to prevent injury or damage。",
    start30: "Make sure the wheel is free to rotate!",
    start31: "Please turn the pedal forward by hand, then you should see the green light。",
    start33: "Start Torque",
    start34: "Please squeeze the brake lever and put around 10kg pressure on the pedal, then you should see the green light。",
    start35: "and then replace the pedal sensor。",
    start36: "< 返回",
    start37: "下一个 >",

    toolTitle1: "First version BESST tool ",
    toolTitle2: "PC mode - Uart",
    tool11: " Only support Uart Protocol with Mini USB port in one side and two USB Type B ports on the other side。",
    tool12: ' "PC" mode is used to read data from display compared with working mode。When HMI connect to BESST tool, if HMI could be switched on, HMI go into "PC" mode。',
    tool13: "LCD Display",
    tool14: 'The screen show "PC" or "Connected OK" while you turn on HMI。',
    tool15: "LED Display",
    tool16: "The light of highest level is on while you turn on HMI。",

    updateTitle: 'Software Version',
    updateHistory: 'Software History',
    update111: "While download firmware to BESST box, please turn off HMI。",
    update1111: "While download firmware to BESST box, please unplug HMI。",
    update112: "While download firmware to BESST box, please unplug controller。",
    update11: "If you have downloaded the firmware to BOX successfully,",
    update12: 'then turn on the HMI until you hear "BEEP" and the HMI is off。',
    update122: 'then plug the HMI until you hear "BEEP" and the HMI is off。',
    update13: "then the update is done。",
    update14: "Press reset button on BESST Software。",
    update15: "Turn on HMI。",
    update16: 'then pulg the controller until you hear "BEEP" 。',
    update17: "then the update is done and unplug the controller。",
    update19: "Repower the controller。",
    update20: 'until you hear "BEEP",then the update is done。',

    error: "Error",
    errorDesc: "No data collected。It might be connection failure。",
    errorNote: 'Please click "Reconnect" to connect again。',
    errorHmi: 'Sn is empty。HMI is not registered on BESST。Please reconnect or check whether HMI is in "PC" mode。',
    errorController: "Sn is empty。Controller is not registered on BESST。Please reconnect or test on Tool section。",
    errorBattery: "Sn is empty。Battery is not registered on BESST。Please reconnect or test on Tool section。",
    errorSensor: "Sn is empty。Sensor is not registered on BESST。Please reconnect or test on Tool section。",

    goTest: "成功进入测试模式",

    resultTitle: "整车诊断报告",
    resultBike: "整车车型:",
    print: "打印",
    bikeId: "整车车架号",
    key: "钥匙卡",
    result: "结果",
    saveReport: "保存报告",
    offLineTip: "In offline mode, you could type vehicle SN and save report on local。Please click print icon on the top of this page。",
    motorSNTip: "The input motor sn is different with the data belong to the bike。",
    bikeSNTip1: "Please type bicycle id。",
    bikeSNTip2: "Bike SN has been changed。Please check。",

    canStep1: "Please wait for one or two seconds。。。",
    canStep2: "Send command of closing controller successfully。",
    canStep3: "Fail to send command of closing controller。Try to start test again。",
    canStep4: "Controller is off。Please press HMI power button over 3s to open controller",
    canStep5: "Controller is open。Close this window to move on。",
    selectDevice1: "Please select component。HMI and Controller is necessary",
    selectDevice2: "Not select any available device",

    failDesc: "Main data is not collected。It might be connection failure or software problem。",
    failNote: 'Please click "Reconnect" to connect again。',
    failNoteMotor: 'The motor information is empty because no bike traced。',

    analogLight: '通讯指示灯',
    communicationOk: '通信正常',
    brake01: '观察车辆是否配置刹车断电功能，无功能配置，请点击"下一步" ',
    brake02: '手动刹车，并松开刹车，重复多次，观察指示灯，能否正常点亮和关闭，请选择“刹车工作正常”，否则“刹车断电功能不正常”',
    brake06: '如果指示灯不能变绿，或者指示灯变绿后不能变灰，请点击"失败" ',
    brake07: "刹车工作正常",
    brake08: "刹车断电功能不正常",
    light01: "大灯键为",
    light02: '手动按仪表大灯键开启和关闭，请选择“指示灯正常”和“指示灯不正常”。',
    light022: '观察大灯是否正常开启和关闭，请选择“大灯正常工作”，否则请选择“大灯不能正常工作',
    light03: '观察车辆是否有大灯配置，请选择有或者没有，如果没有，按“下一步”',
    light04: '通讯指示灯评判',
    light05: '大灯工作状态',
    light06: '如果指示灯不能变绿，或者指示灯变绿后不能变灰,请点击"失败"。',
    light07: '指示灯正常',
    light08: '指示灯不正常',
    light09: '大灯正常工作',
    light10: '大灯不能正常工作',
    support02: '按仪表“+/-”键，依次从最低档位到最高档位循环2次，观察窗口和仪表显示是否对应一致，并及时响应，需选择“正常工作”或者“非正常工作” ',
    support03: '正常工作',
    support04: '非正常工作',
    speed01: '保持驱动系统开机状态，观察整车速度检测传感器位置，并按照车辆行驶方向拨动车轮旋转，观察窗口和仪表出现速度显示，请按“速度传感器工作正常”，否则“速度传感器非正常” ',
    speed04: '速度传感器工作正常',
    speed05: '速度传感器非正常',
    cadence01: "设置档位1档以上，按照骑行方向旋转脚踏，观察窗口应该显示速度，请匀速保持20RPM并持续10秒以上，将会自动判断“踏频传感器工作正常”，否则请按“踏频传感器不正常”",
    cadence03: '踏频传感器不正常" ',
    torque01: '设置档位1档以上，按照骑行方向快速旋转中轴，观察窗口将显示至少大于0.75V，小于5V的值，并随施加力量大小变化为正常，该值大于0.75V，系统自动判断该功能合格，否则请按“Fail”',
    torque02: '如果确认车辆采用的是力矩传感器，请选择“S/T”，调整到“T”',
    walk01: '按住仪表 ',
    walk02: ' /"-"3秒以上, 如果电机转动，并且速度小于6km/h，则系统自动判断OK，否则请点击“WALK通讯故障” ',
    walk03: '请按',
    walk04: '/“-”至仪表和观察窗口显示“WALK”，否则请按“WALK通讯故障”',
    walk05: 'WALK通讯故障',
    walk06: '如果自行车的后轮上有飞轮，则将链条放在最小的链轮上',
    motor01: '1.踏频工作模式下，电机工作电流小于3A，自动评判正常',
    motor02: '2.“WALK”模式下，电机工作电流小于3A，自动评判正常',
    motor03: '3.如果仍然没有没有显示电机工作正常，请将车辆驱动轮置于有阻尼并能转动状态，用脚踏启动电机，观察窗口正常显示电流值，速度和踏频或力矩电压信息，系统将自动判断“电机工作正常”，否则“Fail” ',

    defectDes: "故障描述",
    defects: "机械故障",
    image: "图片",
    video: "视频",
    videoMaxSize: "视频",
    defectLength: "最多400个字符, 包括空格",

    notice19_00: "7天内不再提醒",
    notice19_01: "2019年12月19号之后变化的解释",
    notice19_02: "诊断功能已于2019年12月19号更新。该诊断功能可能不适用于某些在2019年10月之前生产的CAN产品",
    notice19_03: "此外，由于服务工具的硬件之间存在差异，因此在使用硬件版本为'BESST.UC 3.0'的服务工具时，会有一些操作差异",
    notice19_04: "已经调整好BESST，消除了服务工具硬件之间的差异。",
    notice19_05: "注意：如果要持续诊断CAN系统，则每次必须关闭控制器电源超过20秒",

};


const diagnosisNormal = {
    com: "端口",
    select: "请选择",
    connect: "连接",
    readAgain: "再次读入",
    refresh: "刷新",
    tip: "帮助",
    saveReport: "保存报告",
    sendReport: "发送报告",
    selectError: "错误选择",
    tipContent01: "请检查以下基本信息并检查",
    tipContent02: "你的整车和所填ID的一致性。",
    title: {
        name: "名称",
        sn: "序列号",
        diagnosisResult: "诊断结果",
        mechanicalFailure: "机械故障",
        serviceBtn: "帮助",
    },
    bike: "车辆",
    sn: "序列号",
    confirm: "确认",
    cancel: "取消",
    solution: "解决方案",
    hmiDefectOptions: {
        displayNotWork: "显示器不工作",
        displayWaterInside: "显示里面有水",
        displayBtnDamage: "显示按钮已损坏",
        displayCableDamage: "显示电缆已损坏",
        displayScreenDamage: "显示屏幕已损坏",
        displayBracketDamage: "显示支架已损坏",
        displayScrewLoosen: "显示螺丝松开",
        displayBlackScreen: "显示黑屏",
        displayMessyCode: "显示乱码",
    },
    controllerDefectOptions: {
        noAssistance: "没有帮助",
        noLightFunc: "没有照明功能",
        brakeCannotCutOffPower: "刹车不能切断电源",
        tripDistanceShort: "旅行的距离太短",
    },
    batteryDefectOptions: {
        batteryDamage: "电池已损坏",
        chargerNotWork: "充电器不工作",
        BatteryCannotCharge: "电池不能充电",
    },
    motorDefectOptions: {
        motorNotWork: "电机不工作, 但显示正常工作",
        motorShake: "电机震动",
        motorNoisy: "电机噪声大",
        motorLessPower: "电机功率低",
        motorRunIntermittently: "电机间歇性运行",
        noFunction: "没有6KM/H功能",
    },
    defectOptions: {
        motorNotWork: "电机不工作",
        motorNoisy: "电机噪音大",
        motorDamage: "电机有损坏",
        hmiFailed: "仪表读写失败",
        hmiErrorCode: "仪表报错",
        hmiDamage: "仪表有损坏",
        disorderedIndication: "无序指示",
        outOfControl: "不受控制",
        eBrakeFailed: "整车报错",
        eBrakeDamage: "整车有损坏",
        lightFailed: "灯报错",
        walkAssistanceFailed: "走路援助报错",
        throttleFailed: "节气门报错",
        throttleDamage: "节气门已损坏",
        shortRange: "变化很小",
        cableDamage: "电缆有损害",
        batteryProblem: "电池报错",
        batteryDamage: "电池有损害"
    },
    noDefect: "没有缺陷",
    read: "读入",
    test: "测试",
    errorSolutionDialog: {
        title: "错误解决方案",
        name: "名称",
        remark: "备注",
        description: "描述",
        target: "目标",
        solution: "解决方案",
    },
    connectTipVideoDialog: {
        title: "连接帮助视频",
        tip: "你的浏览器不支持video标签。"
    },
    connectTipDialog: {
        title: "连接帮助",
        tip: "你的浏览器不支持video标签。",
        content: "请连接BESST到HMI和EB-BUS, 然后开启HMI, 检查是否HMI上显示'PC'(LCD类型)或者顶部模式LED灯(LED类型)"
    },
    confirmSnDialog: {
        title: "检查序列号",
        content: "请检查车辆序列号是否正确"
    },
    print: "打印",
    bikeInfo: "车辆信息",
    diagnosis: {
        result: "诊断结果",
        hmi: " HMI",
        battery: "Battery",
        controller: "Controller",
        rpmSensor: "RPM Sensor",
        pedalSensor: "Pedal Sensor",
        motor: "Motor",
    },
    emptySNTip: "车辆SN未输入提示",
    emptySNText: "车辆的SN未输入，仍要跳转到测试页面？",
    back: "返回",
    storeInfo: "整车经销商信息",

    preTipA: '打开仪表，此时工具会发出”滴滴”两声，然后点击',
    continue: "继续",
    preTipB: '如果没有听到”滴滴”声，则请点击',
    here: "更多"

};

const diagnosisTest = {
    back: "返回",
    driveSystemTest: "驱动系统测试",
    hmiMode: "仪表模式",
    currentMode: "当前模式",
    light: "大灯",
    title: {
        name: "名称",
        sn: "序列号",
        diagnosisResult: "诊断结果",
        mechanicalFailure: "机械故障",
    },
    help: "帮助",
    save: "保存",
    confirm: "确认",
    cancel: "取消",
    hmiDefectOptions: {
        displayNotWork: "显示器不工作",
        displayWaterInside: "显示里面有水",
        displayBtnDamage: "显示按钮已损坏",
        displayCableDamage: "显示电缆已损坏",
        displayScreenDamage: "显示屏幕已损坏",
        displayBracketDamage: "显示支架已损坏",
        displayScrewLoosen: "显示螺丝松开",
        displayBlackScreen: "显示黑屏",
        displayMessyCode: "显示乱码",
    },
    controllerDefectOptions: {
        noAssistance: "没有帮助",
        noLightFunc: "没有照明功能",
        brakeCannotCutOffPower: "刹车不能切断电源",
        tripDistanceShort: "旅行的距离太短",
    },
    batteryDefectOptions: {
        batteryDamage: "电池已损坏",
        chargerNotWork: "充电器不工作",
        BatteryCannotCharge: "电池不能充电",
    },
    motorDefectOptions: {
        motorNotWork: "电机不工作, 但显示正常工作",
        motorShake: "电机震动",
        motorNoisy: "电机噪声大",
        motorLessPower: "电机功率低",
        motorRunIntermittently: "电机间歇性运行",
        noFunction: "没有6KM/H功能",
    },
    defectOptions: {
        motorNotWork: "电机不工作",
        motorNoisy: "电机噪音大",
        motorDamage: "电机有损坏",
        hmiFailed: "仪表读写失败",
        hmiErrorCode: "仪表报错",
        hmiDamage: "仪表有损坏",
        disorderedIndication: "无序指示",
        outOfControl: "不受控制",
        eBrakeFailed: "整车报错",
        eBrakeDamage: "整车有损坏",
        lightFailed: "灯报错",
        walkAssistanceFailed: "走路援助报错",
        throttleFailed: "节气门报错",
        throttleDamage: "节气门已损坏",
        shortRange: "变化很小",
        cableDamage: "电缆有损害",
        batteryProblem: "电池报错",
        batteryDamage: "电池有损害"
    },
    noDefect: "没有缺陷",
    selectError: "错误选择",
    errorSolutionDialog: {
        title: "错误解决方案",
        name: "名称",
        remark: "备注",
        description: "描述",
        target: "目标",
        solution: "解决方案",
    },
};


const reportList = {
    tableHead: {
        vehicleSn: "整车序列号",
        modelName: "型号名称",
        time: "日期",
        operation: "操作"
    },
    noResult: "没有记录！",
    total: "总数",
    search: {
        vehicleSn: "整车序列号",
        searchBtn: "搜索"
    },
    deleteDialog: {
        title: "删除报告",
        content: "确定要删除该报告吗?"
    },
    delete: "删除",
    yes: "是",
    no: "否"
};

const ComponentsDiagnosis = {
    diagnosisNormal: diagnosisNormal,
    diagnosisTest: diagnosisTest,
    report: report,
    reportList: reportList,
    diagnosis3rd: diagnosis3rd,
};

// components serviceCenter
const accountList = {
    back: "返回",
    add: '新增',
    reset: "重置",
    delete: "删除",
    tableHead: {
        account: "账号",
        createTime: "创建时间",
        operation: "操作",
    },
    noResult: "没有记录！",
    total: "总数",
    confirm: "确认",
    cancel: "删除",
    yes: "是",
    no: "否",
    addAccountDialog: {
        title: "新增账号",
        email: "邮箱",
        password: "密码"
    },
    deleteDialog: {
        title: "删除账号",
        content: "确定要删除该账号吗?"
    },
    resetPasswordDialog: {
        title: "重置密码",
        content: "确定要重置该账号的密码为123456吗?"
    }
};

const centerTicketList = {
    serviceCenter: "服务中心",
    category: "分类",
    status: "状态",
    look: "查看",
    noResult: "没有记录！",
    total: "总数",
    tableHead: {
        serviceNo: "服务编号",
        title: "标题",
        targetSn: "车辆序列号",
        createTime: "创建时间",
        status: "状态",
        operation: "操作"
    },
    categoryList: {
        all: "所有",
        vehicle: "整车",
        motor: "电机",
        hmi: "仪表",
        sensor: "传感器",
        battery: "电池",
        light: "灯",
        controller: "控制器",
        other: "其他",
    },
    statusList: {
        all: "所有",
        pendingAssign: "待分配",
        processing: "处理中",
        complete: "已完成"
    }
};

const myTicketList = {
    category: "分类",
    status: "状态",
    look: "查看",
    tableHead: {
        serviceNo: "服务编号",
        category: "分类",
        title: "标题",
        targetSn: "车辆序列号",
        createTime: "创建时间",
        status: "状态",
        operation: "操作"
    },
    categoryList: {
        all: "所有",
        vehicle: "整车",
        motor: "电机",
        hmi: "仪表",
        sensor: "传感器",
        battery: "电池",
        light: "灯",
        controller: "控制器",
        other: "其他",
    },
    statusList: {
        all: "所有",
        processing: "处理中",
        complete: "已完成",
        handleToAnother: "提交给其他",
    },
    noResult: "没有记录！",
    total: "总数",
    addTicketDialog: {
        dialogTitle: "新增服务工单",
        category: "分类",
        title: "标题",
        description: "描述",
        bike: "整车",
        sn: "编号",
        serviceCenter: "服务中心",
        image: "图片",
        video: "视频",
        clickToUpload: "点击上传视频"
    },
    confirm: "确认"

};

const serviceCenterList = {
    serviceCenter: "服务中心",
    add: "新增",
    edit: "编辑",
    account: "账号",
    noResult: "没有记录！",
    total: "总数",
    confirm: "确认",
    cancel: "取消",
    yes: "是",
    no: "否",
    tableHead: {
        logo: "图标",
        name: "名称",
        phone: "电话",
        email: "邮箱",
        country: "国家",
        address: "地址",
        operation: "操作",
    },
    deleteDialog: {
        title: "删除服务中心",
        content: "确定要删除该服务中心吗?"
    },
    createSC: "创建服务中心",
    editSC: "编辑服务中心",
    editDialog: {
        picture: "图片",
        tip: "备注",
        content01: "最好上传有背景的正方形图片",
        content02: "支持JPG，PNG， 300*300/400*400, 不超过3MB",
        name: "名称",
        email: "邮箱",
        phone: "电话",
        website: "网址",
        address: "地址",
        postcode: "邮编",
    },
    detailDialog: {
        title: "服务中心详情",
        picture: "图片",
        name: "名称",
        email: "邮箱",
        phone: "电话",
        website: "网址",
        address: "地址",
        postcode: "邮编",
        ticketCount: "工单个数",
        aveHandleTime: "平均处理时间",
    },
    op: Operation,
};

const ticketDetail = {
    back: "返回",
    handleUp: '向上级提交',
    history: "历史记录",
    ticketInfo: {
        infoTitle: "投诉信息",
        ticketNo: "工单编号",
        createTime: "创建时间",
        status: "状态",
        serviceCenter: "服务中心",
        title: "标题",
        desc: "描述",
        image: "图片",
        video: "视频",
    },
    user: {
        address: "地址",
        website: "网址",
        email: "邮箱",
    },
    replay: {
        content: "内容",
        image: "图片",
        sendMessage: "发送消息"
    },
    confirm: "确认",
    cancel: "取消",
    handleUpTicketDialog: {
        title: "向上级提交工单",
        serviceCenter: "服务中心"
    },
    historyDialog: {
        title: "工单历史记录",
        name: "名称",
        operation: "操作",
        time: "日期"
    }
};

const ComponentsServiceCenter = {
    accountList: accountList,
    centerTicketList: centerTicketList,
    myTicketList: myTicketList,
    serviceCenterList: serviceCenterList,
    ticketDetail: ticketDetail
};

const Components = {
    Account: ComponentsAccount,
    ReplaceReport: ComponentsReplaceReport,
    Diagnosis: ComponentsDiagnosis,
    ServiceCenter: ComponentsServiceCenter,
    Tool: ComponentsTool,
    Repair: Repair
};

//组装厂
const AssemblerIndex = {
    order: "订单",
    vehicle: "整车"
};

const AssemblerOrderDetail = {
    back: "返回",
    orderDetail: "订单详情",
    edit: "编辑",
    delete: "删除",
    orderInfo: "订单信息",
    orderNo: "订单序列号",
    customerName: "客户名称",
    note: "记录",
    poNo: "订单号",
    itemsList: "车型列表",
    produce: "生产",
    vehicleList: "车辆列表",
    deleteOrder: '删除订单',
    deleteTip: "你确定要删除它吗？",
    yes: "是",
    no: "否",
    tableHead: {
        modelName: "车型名称",
        partNo: "物料号",
        quantity: "数量",
        operation: "操作",
    },
    replace: "替换",
    confirmDialog: {
        replace: "替换",
        yes: '是',
        no: '否'
    },
    itemHead: {
        image: "图片",
        name: "名称",
        partNo: "物料号"
    },
    search: "搜索",
    brandPlaceholder: "请选择品牌",
    operation: "操作",
    modelName: "车型",
};

const AssemblerOrderList = {
    add: "添加",
    noResult: "没有记录！",
    vehicleList: "车辆列表",
    detail: "详情",
    total: "总数",
    tableHead: {
        date: "日期",
        orderNo: "订单序列号",
        poNo: "订单号",
        customerName: "客户名称",
        model: "车型",
        produced_qty: "完成/总数",
        status: "状态",
        operation: "操作"
    },
    poNo: "订单序列号",
    search: "搜索",
    startDate: "开始时间",
    endDate: "结束时间",
    model: '车型',
};

const AssemblerOrderProduce = {
    back: "返回",
    orderInfo: "订单信息",
    brand: "品牌",
    model: "车型",
    poNo: "订单号",
    produced_qty: "完成/总数",
    pleaseSelect: "请选择",
    reconnect: "重连",
    openConnectTip: "打开连接提示",
    partName: "部件",
    sn: "序列号",
    hw: "固件版本",
    sw: "软件版本",
    connectTipVideo: "连接视频提示",
    connectTip: "连接提示",
    connectTipText: "请连接BESST到HMI和EB-BUS, 然后开启HMI, 检查是否HMI上显示'PC'(LCD类型)或者顶部模式LED灯(LED类型)",
    confirm: "确定",
    itemFinishTip: "这个车型已经完成",
    selectTip: "请选择端口",
    tip: '提示',
    batteryBindTip: "是否将电池绑定？",
    yes: "是",
    no: "否",
    goDiagnosisTip: "是否去诊断页面？",
    bindBattery: "绑定电池",
    vehicleSn: "车辆序列号",
    componentSn: "零部件序列号",
    videoTip: "你的浏览器不支持视频标签播放",
    error: "错误",
    historyDialog: {
        title: "固件历史记录",
        version: "版本",
        remark: "备注",
        updateTime: "上传时间",
        operation: "操作",
        noResult: "没有记录！"
    },
    update: "升级",
    reset: "重置",

    isUsed: "已经被组装车了",
    canNotUsed: "不能使用了",
    inputBikeSn: "请输入车辆序列号",
    inputMotorSn: "请输入电机序列号",
    inputHmiSn: "请输入仪表序列号",
    inputCtlSn: "请输入控制器序列号",
    inputBatSn: "请输入电池序列号",
    inputSensorSn: "请输入传感器序列号",

    bikeSnExt: "车辆的序列号存在",
    saveSnText: 'BESST中不存在以下组件SN（序列号）。 请检查SN。 如果一切正常，您仍然可以通过单击“保存”将SN绑定到自行车。 如果这种情况连续发生，请联系自行车品牌或组件供应商。',

    cNam: AttributeName
};

const AssemblerOrderSave = {
    back: "返回",
    createOrder: "创建订单",
    editOrder: "编辑订单",
    orderInfo: "订单信息",
    poNo: "订单号",
    customerInfo: "客户信息",
    customerName: "客户名称",
    note: "记录",
    itemList: "车型列表",
    addItem: "添加车型",
    delete: "删除",
    modelName: "车型",
    addItems: "添加车型",
    saveOrder: "保存订单",
    tableHead: {
        name: "名称",
        partNo: "物料号",
        quantity: "数量",
        operation: "操作"
    },
    itemHead: {
        image: "图片",
        name: "名称",
        partNo: "物料号"
    },
    search: "搜索",
    brandPlaceholder: "请选择品牌"
};

const AssemblerPartnerList = {
    back: "返回",
    acceptInvitation: "接受邀请",
    currentBrand: "当前品牌",
    noResult: "没有记录！",
    total: "总数",
    brandInfo: "品牌信息",
    logo: "图标",
    name: "名称",
    type: "类型",
    brand: "品牌",
    email: "邮箱",
    website: "网址",
    tel: "电话",
    postcode: "邮编",
    address: "地址",
    invitationCode: "邀请码",
    confirm: "确认",
    cancel: "取消",
    hoverTip: "选择作为当前品牌",
    tableHead: {
        name: "名称",
        phone: "电话",
        website: "网址",
        country: "国家",
        operation: "操作"
    },
    codeEmptyTip: "请输入邀请码",
    acceptSuccess: "接受成功"
};

const AssemblerVehicleDetail = {
    vd: VehicleDetail
};

const AssemblerVehicleList = {
    brand: "品牌",
    poNo: "订单号",
    modelName: "车型",
    bomSn: '部件序列号',
    search: "搜索",
    export: "导出",
    delete: "删除",
    detail: "详情",
    total: "总数",
    deleteVehicle: "删除车辆",
    deleteTip: "你确定要删除它吗？",
    yes: "是",
    no: "否",
    back: "返回",
    tableHead: {
        vehicleSn: "车辆序列号",
        brandLogo: "品牌图标",
        brandName: "品牌名称",
        modelName: "车型",
        poNo: "订单号",
        produceDate: "生产日期",
        saleDate: "售出日期",
        operation: "操作",
        noSale: "没有销售"
    },
    noResult: "没有记录！",
    deleteSuccess: "删除成功",
    noSale: "未出售",
    exportDialog: {
        title: "导出车辆",
        startNum: "开始数",
        endNum: "结束数",
        export: "导出",
        cancel: "取消"
    }
};

const Assembler = {
    Index: AssemblerIndex,
    AssemblerOrderDetail: AssemblerOrderDetail,
    AssemblerOrderList: AssemblerOrderList,
    AssemblerOrderProduce: AssemblerOrderProduce,
    AssemblerOrderSave: AssemblerOrderSave,
    AssemblerPartnerList: AssemblerPartnerList,
    AssemblerVehicleDetail: AssemblerVehicleDetail,
    AssemblerVehicleList: AssemblerVehicleList,
};

export default {
    Login: Login,
    Register: Register,
    Layout: Layout,
    Index: Index,
    ServiceCenter: ServiceCenter,
    Component: Component,
    Dealer: Dealer,
    ForgetPassword: ForgetPassword,
    Components: Components,
    Brand: Brand,
    Assembler: Assembler
};



// WEBPACK FOOTER //
// ./src/assets/lang/zh.js