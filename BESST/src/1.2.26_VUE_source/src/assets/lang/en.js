// 登录页面相关
const Login = {
    declare: "DECLARE",
    readmore: "read more",
    declareP1: "BESST is a management software developed for the brands that is used for electrical driven systems. The brands send  invitation codes to its dealers, factories and distributors, who will then register their own account. Each role must ensure that the registered accounts and passwords are not shared with third parties, and the brands are responsible for training the invited relationships",
    declareP2: "Note: all users need to purchase BESST TOOL from brand companies or software owners to realize all functions of the software.",
    declareP3: "The Brand company",
    declareP6: "1. The brand company is authorized by the software owner:",
    declareP7: "The brand company shall submit a written application to the software owner, including: the registered name of the brand company, country, address, postal code, legal email address of the management account, brand ownership certificate, website domain name, etc.",
    declareP8: "Software owners confirm the authenticity and legality of information;",
    declareP9: "Software owners establish account information through the software background and send authorization information by mail;",
    declareP10: "After downloading the software through authorization information, brand companies must fill in the relevant information accurately according to the software registration process to improve the registration process;",
    declareP11: 'Enter "SETTING" to set company information and change the original password',
    declareP12: 'Enter "Brand list" and add the Brand list of your own company',
    declareP13: 'Enter "Model list" to add model information',
    declareP14: `Enter "Dealer" and send the invitation code according to the Dealer's email`,
    declareP15: 'Enter "OEM" and send the invitation code according to the OEM email address',
    declareP16: `Enter the "Account" through the main Account and distribute the employee's Account by themselves`,
    declareP17: "The software download path:",
    declareP18: "Dealers and Distributors",
    declareP19: "3. Dealers and distributors are authorized by the brand company:",
    declareP20: 'Dealers and distributors shall ask the brand company for the "invitation code";',
    declareP21: 'For new registered users, fill in "invitation code" in the "Sign up" registration interface to verify the email address as the account name;',
    declareP22: "After receiving the verification code through the mailbox, set the password;",
    declareP23: 'For users of the old BESST system, enter "Brand List", fill "Accept Invitation" with" invitation code ", and Accept new Brand invitation.',
    declareP24: "After the password is successfully set, start to fill in the information of dealers and distributors in accordance with the software process, and confirm the authenticity and validity of the information, and end the registration.",
    declareP25: 'Next time you log in, fill in your email number and password in the account bar. It is better to mark "Remember me" so that you can log in easily next time;',
    declareP26: '"Brand list": after the registration of the Invitation code, the system automatically collects and loads the information of the Brand',
    declareP27: 'Enter "Bike Model", get access to all brands of product information',
    declareP28: 'Enter "Bike Sales", register customer information, confirm vehicle ID, generate Sales order, determine after-sales policy, be sure to confirm the data saved to the server.',
    declareP29: "The Manufacturing Plant",
    declareP30: "2.	The manufacturing plant is authorized by the brand company:",
    declareP31: 'The manufacturing factory needs to ask the brand company for the "invitation code";',
    declareP32: "After the password is successfully set, start to fill in the manufacturing factory information according to the software process, and confirm the authenticity and validity of the information, and end the registration. ",
    declareP33: '"Brand list": after the registration of the Invitation code, the system automatically collects and loads the information of the Brand',
    declareP34: '"Order" and "Vehicle List" collect brand company information through invitation codes. When creating a new sales Order, the brand should be selected first, and then the Vehicle type should be selected.',

    chooseLanguage: "Language",
    chinese: "中文",
    deutsch: "Deutsch",
    english: "English",
    dutch: "Nederlands",
    login: "Login",
    userNamePlaceholder: "User Name",
    passwordPlaceholder: "Password",
    rememberMe: "Remember me",
    forgetPassword: "Forget Password?",
    loginBtnTxt: "Log in",
    signUp: "Sign up",
    offLine: "Offline Mode",
    inputUser: "Please input username",
    inputPassword: "Please input password",
    accountProblem: "The account has some problems. Please contact the administrator besst@bafang-e.com",
    limitedIP: 'IP verification is enabled for this account. IP verification failed, please login in regular location.',
    emptyIP: 'IP verification is enabled for this account. Please wait or try later.',
    emptyIPThree: 'IP verification is enabled, please contact the administrator.',
};

// 注册页面相关
const RegisterAccount = {
    login: "log in",
    verifyCode: "Verify the code",
    createAccount: "Create an account",
    createCompany: "Create a company",
    company: "Company",
    email: "E-mail",
    emailValidationCode: "E-mail validation code",
    password: "Password",
    confirmPassword: "Confirm password",
    next: "Next",
    verify: "Verify",
};

const RegisterCompany = {
    login: "log in",
    verifyCode: "Verify the code",
    createAccount: "Create an account",
    createCompany: "Create a company",
    yourRole: "Your role",
    logo: "Logo",
    uploadImgTip: "You'd better upload square picture with background, support jpg,png and 300*300/400*400, up to 3MB",
    contact: "Contact",
    companyName: "Company name",
    mailAddress: "Mail address",
    phoneNumber: "Phone number",
    website: "Website",
    postalAddress: "Postal address",
    post: "Post",
    address: "Address",
    save: "Save",
    country: "Country",
};

const RegisterInvitation = {
    login: "log in",
    verifyCode: "Verify the code",
    createAccount: "Create an account",
    createCompany: "Create a company",
    invitationCode: "Invitation Code",
    typeTheCodeTip: "Type the code you receive from brand.",
    tryAgainTip: "If you have used the code but not register successfully, you can try again.",
    verify: "Verify",
    noCompanyTip: "This account needs to be supplemented with company information.",
    reCodeTip: "Please input the invitation code used for creating this account to create company.",

    getAccess: "How to get access?",
    backend: "BACKEND",
    brand: "BRAND",
    dealers: "DEALERS",
    assemblers: "ASSEMBLERS",
    createAP: "Create account and initial password",
    createCode: "Create invitation codes",
    useCode: "Use the invitation code. Follow the prompts of the received email to complete the registration and activation of the account.",

};

const Register = {
    Account: RegisterAccount,
    Company: RegisterCompany,
    Invitation: RegisterInvitation
};

//忘记密码页面
const ForgetPassword = {
    login: "login",
    email: "E-mail",
    verify: "Verify",
    emailValidationCode: "E-mail validation code",
    password: "Password",
    confirmPassword: "Confirm password",
    resetPassword: "Reset Password",
    emptyEmailTip: "Please input E-mail",
    errorEmailTip: "E-mail is illegitimate",
    emptyCodeTip: "Please input code",
    emptyPasswordTip: "Please input password",
    emptyConfirmPasswordTip: "Please input confirm password",
    passwordTip: "Two passwords are not the same",
    resetPasswordSuccess: "Reset password successfully",
    codeHasBeenSent: "Code has been sent",
    passwordFormatTip: "Password must be 8-16 characters, only A-Za-z0-9, case-sensitive.",
    filterUserEmail: "User email is not registered in BESST. Please check the email whether it is correct. "
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
    brand: "Brand List",
    model: "Model List",
    vehicle: "Vehicle List",
    dealer: "Dealer",
    dealerList: "Dealer List",
    invitation: "Invitation",
    oem: "OEM",
    componentList: "Component Company",
    oemList: "OEM List",
    serviceCenter: "Service Center",
    serviceCenters: "Service Centers",
    complaints: "Complaints",
    tasks: "Tasks",
    account: "Account",
    order: "Order",
    diagnosis: "Diagnosis",
    report: "Report",
    errorCode: "Error Code",
    tool: "Tools",
    hmi: "HMI",
    controller: "Controller",
    battery: "Battery",
    sensor: "Sensor",
    update: "Update",
    auth: "Auth",
    product: "Product",
    component: "Trace",
    centerTicket: "Center Ticket",
    myTicket: "My Ticket",
    help: "Help",
    setting: "Setting",
    logout: "Logout",
    statistics: "Statistics",

    record: "Record",
    testFeedback: "Feedback Center",
    material: "Production Kanban",
    admin: "Administration",
    bikeModel: "Bike Model",
    bikeSales: "Bike Sales",
    bikeInfo: "Bike Info",
    author: "Authorization",
    mechanic: "Mechanic Defect",
    faq: "FAQ",
    tt: Title,
};

const Layout = {
    Sidebar: Sidebar,
    NavBar: NavBar,
};

//Article All
const ArticleSet = {
    image: "Image",
    model: "Model",
    mechanical: "Mechanical property",
    firmVer: "Firmware Ver",
    hardVer: "Hardware Ver.",
    connector: "Connector Code",
    operation: "Operation",
    articleNo: "Article Number",
    paramVer: "Params Ver.",
    detail: "detail",
};
//  Operation
const Operation = {
    look: 'Look',
    edit: 'Edit',
    delete: 'Delete',
    view: 'View',
    add: 'Add',
    save: 'Save',
    cancel: 'Cancel',
    confirm: 'Confirm',
    account: 'Account',
    reset: 'Reset password',
    search: 'Search',
    export: 'Export',
    send: 'Send',
    upload: 'Upload',
    select: 'Select',
    close: 'Close',
    yes: 'Yes',
    no: 'No',
};

//components Tool
const AttributeName = {
    controller: "Controller",
    hmi: "HMI",
    battery: "Battery",
    motor: "Motor",
    sensor: "Sensor",
    key: "KEY",
    sn: "Series Number",
    charger: "Charger",
    other: "Other",
    bike: "Bike",

    model: "Model",
    softwareVersion: "Software Ver.",
    hardwareVersion: "Hardware Ver.",
    tryAccountsTip: "This account is for trial use. Some operations are restricted.",

    bootLoad: "Bootloader Ver.",
    errorCode: "Error Code",
    totalMileage: "Total Mileage",
    clientCode: "Customer No.",
    wheelDiameter: "Wheel Size",
    speedLimit: "Speed Limit",
    totalGear: "Total Level",
    serviceMileage: "Remove Maintenance Warning",
    setTotalMileage: "Set Total Mileage",
    setSingleMileage: "Set Single Mileage",
    setTimeZone: "Set Time Zone",
    time: "Time",
    keyCard: "Key Card",
    circumference: " Circumference",
    singleMileage: "Single Mileage",
    speedMax: "Max. Speed",
    speedAvg: "Average Speed",
    hmiMode: "Mode",
    hmiLevel: "Current level",
    hmiBoost: "BOOST mode",
    hmiBoostIn: "In BOOST",
    hmiBoostOut: "Not in BOOST",
    hmiTurnOffTime: "Shutdown time",
    hmiNoTurnOff: "Never turn off",
    hmiLight: "Head Light",
    hmiLightOn: "On",
    hmiLightOff: "Off",
    hmiButton: " '+' key status",
    hmiBtnPress: "Pressed",
    hmiBtnPressNot: "Not pressed",
    hmiSingleMileageClear: "Clear Single mileage",
    hmiClean: "Clear",
    hmiNotClean: "Not clear",

    spdMeterType: "Spd Meter Type",
    manName: "Manufacturer",
    nmlVolt: "Nominal Voltage",
    maxCur: "Max. Current",
    spdMeterSignalNum: "Spd Meter Signal",
    limitVolt: "Low Voltage Protect",
    limitCurrent: "Limit Current",
    limitSpd: "Limit Spd",
    assit: "Assist",
    positionSensor: "Position sensor calibration",
    torqueSenCali: "Torque sensor calibration",
    conTemperature: "Controller temperature",
    motorTemperature: "Motor temperature",
    calorie: "Calories",
    assistStatus: "Walk-assist status",
    assistOn: "On",
    assistOff: "Off",
    conTorque: "Torque output voltage",
    conCadence: "Cadence",
    conSingleTrip: "Single trip mileage",
    conSingleTripLeft: "Remaining mileage",
    conSpeed: "Speed",
    conCurrent: "Current",
    conVoltage: "Voltage",

    ASOC: "ASOC",
    packVoltage: "Battery Voltage",
    fullChargeCapacity: "Full Charge Capacity",
    current: "Current",
    cell8_14: "Cell 8-14 Voltage",
    currentChargeInterval: "Current uncharged interval",
    RSOC: "RSOC",
    capacityLeft: "Remaining capacity",
    temperature: "Temperature",
    cell1_7: "Cell 1-7 Voltage",
    cycleLife: "Charging cycle number",
    maxChargeInterval: "Max. uncharged interval",
    produceDate: "Produce Date",
    SOH: "SOH",
    useStage: "Maintenance Stage",
    result: "Result",
    afterSalesTitle: "Maintenance Status Check",

    power: "Power",
    voltage: "Voltage",
    wheelSize: "Wheel Size",
    rpm: "RPM",

    torque: "Torque output voltage",
    cadence: "Cadence",
    sensorTip: "Main data is empty. Please check sensor connection.",

    component: "Component",
    com: "COM",
    write: "Write",
    connect: "Connect",
    read: "Read",
    reset: "Reset",
    set: "Set",
    update: "Update",
    test: "Test",
    clear: "Clear",
    start: "Start",
    stop: "Stop",
    cancel: "Cancel",
    confirm: "Confirm",
    select: "Select",
    restart: "Restart",
    save: "Save",
    saveSuccess: "Save successfully.",
    writeSuccess: "Write successfully.",
    selectComTip: "Please select COM.",
    comDisconnect: "Connection is disconnected.",
    noComConnect: "Please connect COM.",
    writeFail: "Write fail. Please try again.",
    snEmptyTip: "Please input SN.",
    note: "Note",
    notice: "Notice",
    message: "Message",
    success: "Successfully",
    operationFail: "Operation Fail.",
    speedLimitNote: "Notice: Speed limit is set above 25Km/h. Please adjust the speed limit within the limits prescribed by local law.",
    hmiUART: "The current drive system is regarded as UART communication",
    hmiUARTText1: "In the case of the display PC mode or working interface, you must first manually turn off the display",
    hmiUARTText2: "Press the BESST TOOL MODE button to reset",
    hmiUARTText3: "Press the display power button to turn on the display and switch to PC mode",
    hmiUARTText4: "Press the Read button to read the parameters. If the information is incomplete, read again",

    mainData: "Main Data",
    parameter: "Parameter Content",
    setting: "Setting",
    operationTip: "Operation Tips",
    txtTitle1: "Operation",
    txtTitleHid: "Communication mode",
    txtTitleCom: "Communication mode",
    txtTitleCon: "You need the connection between BESST tool and PC. USB HID connect automatically.",
    txtTitleRead: "You could read the basic information.",
    txtTitleWrt: "You could rewrite the key parameter of component.",
    txtTitleReset: "You reset the connection status. This function is same as reset button on BESST tool. Also all the existing data would be cleared.",
    txtTitleTest: "You can check whether the display support diagnosis. Connect HMI to BEEST tool, open the HMI, it will go into the abnormal mode depend different types. Now click test, it will jump to normal interface if you connect controller.",
    txtTitleUpt: "You can update firmware to component.",
    txtTitleClear: "You can clear the information.",
    txtTitle2: "Operation Note",
    txtNote1: "This tool can check whether the component support BESST. It can show the basic information of component itself. ",
    txtNote2: "You need read data first before you write. We record every operation together with SN for trace. ",
    txtNote3: "You need read data first before you update. We record every operation together with SN for trace. ",
    updateGetFileSucc: "File is successfully downloaded.",
    updateStarting: "Update starting... If no progress bar in 10 seconds, please reset and update again",
    resetTip: "BESST tool has been reset. Remember to turn off HMI and turn on again.",

    offlineNo: "Dealer or Assembler can't change it to bigger",
    needReadSn: "Sn is empty. Please read first to get SN",
    offlineCon: "Please online",
    offlineHandle: "Data processing， please try again after moment",

    updateSuccessTip: "You download the firmware to service tool successfully, then update the components as instruction",
    updateFailTip: "The service tool fail to verify the firmware and there was a transmission error. Please download the firmware again",
    updateToolReconnectTip: 'Please unplug the service tool and connect PC again. When USB HID status dot turn green, click "Update" again',
    updateMsg: "Update Firmware Process",
    updateMsg0: "Please connect first",
    updateMsg1: "Please unplug the HMI if HMI plugged, and plug back again.",
    updateMsg2: "Please unplug the Controller if Controller plugged, and plug back again.",
    updateMsg3: "Please keep connection between box and computer.",
    updateSelectFile: "Please select file",
    updateOnedriveNote: "Your file is in OneDrive folder, it is not supported now. Please move out.",
    updateReadFail: "Fail to read data or file, please try again.",
    updateDataError: "The header format of firmware data is incorrect",
    updateCrcError: "This file is suspected of a checksum error. If the update fails, please check the file.",
    updateMsg4: "BESST box update HMI",
    updateMsg5: "BESST box update Controller",
    updateMsg6: "BESST box update Battery",
    updateMsg7: "BESST box update Sensor",
    updateMsg8: "Update BESST box",
    updateMsg10: "BESST box updating",
    updateMsg11: "Update process might get some error. Better to update again.",
    updateMsg20: "Update is finished",
    updateMsg30: "Please click read to get SN and Software version.",
    updateMsgLO: "Please select bin file from local or get file online.",
    updateMsgL: "Please select bin file from local.",
    updateMsgTip: "New software version is detected.You could select the latest version and update.If you don't know the steps, please go to section Tool-Update",

    hasHmiSn: "Please check HMI sn.",
    hasCtlSn: "Please check controller sn.",
    hasBatSn: "Please check battery sn.",
    hasMotorSn: "Please check motor sn.",
    hasSensorSn: "Please check sensor sn.",
    updateStep0: 'Update Step',
    updateStep1: 'Click "Connect" to make the connection between computer and service tool.',
    updateStep2: 'Select component type that you want to update. ',
    updateStep3: 'Click "Read" to get SN and software version. If SN is empty, update is not allowed.',
    updateStep4: 'Click "Get a file from cloud" or "Get a file from local".',
    updateStep5: 'Unplug the component and click "Update" to download software to service tool. During this process,you will see service tool "PC" LED blinking. If the "PC" LED is always on, you need reset service tool to normal state and restart.',
    updateStep6: 'After downloading software to service tool successfully, you could plug the component and service tool would write software to it. For more detail, click "help" icon on right top.',

    snMaxLength: "SN length maximum is 40",
    customerMaxLength: "Maximum length is 24",
    speedLimitEmpty: "Speed limit is empty. Please read to get the speed limit of component.",
    whileSizeRange: "Circumference is out of range according wheel size",

    keyCardStart: "Key card start pair",
    keyCardStop: "Key card stop pair",

    noCanConnect: "BESST box is not connected or not support can",
    writeModelTip: "Model data is empty or does not meet specifications",
    writeHardwareTip: "Hardware data is empty or does not meet specifications",

    pcMode: "PC mode",
    pcModeTxt: ' "PC" mode is used to read data from display compared with working mode. When HMI connect to BESST tool,if HMI could be switched on, HMI go into "PC" mode.',
    pcModeTxtLCD: ' LCD Display: The screen show "PC" or "Connected OK". ',
    pcModeTxtLED: ' LED Display: The light of highest level is on. ',
    paramsName: "Params Version",
    paramsCloud: "Get from cloud",
    paramsCombine: "Merge data",

    batUnderMt: "Under Maintenance",
    batOutMt: "Out of Maintenance",
    batUseTime: "Battery has been in use for more than 30 months. It is out of maintenance.",
    strNaming: ' data not support character "\ / ^ : * ? " < > |" ',
    getDataTip: "Getting server data, please be patient",

    remark: "Remark",
    defect: "Defect",
    image: "Image",
    description: "Description",
    video: "Video",
    updateSteps: 'Update Steps',
    connectorType: 'If the connector has the same type of figure 1, please click "Uart" to check detail of instruction. If the connector has the same type of figure 2, please click "CAN Bus" to check detail of instruction.',
    updateSoftwareMethod: 'How to update software from service tool to component?',
    HMIFirst: '1. Plug the HMI to service tool.',
    HMISecond: '2. Press the power button of HMI to init update process. If update starting, service tool "Uart" LED keeps blinking. ',
    HMIThird: '3. If update is successful, the "Test" LED of service tool turn on.',
    HMIForth: '4. Reset the box to normal status. Otherwise, you would get step 2 repeatedly.',
    ControllerFirst: '1. Plug the controller to service tool to init update process. If update starting, service tool "Uart" LED keeps blinking. ',
    ControllerSecond: '2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller.',
    ControllerThird: '3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.',
    HMIFifth: '1. Plug the HMI to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. ',
    HMISixth: '2. If update is successful, the "Test" LED of service tool turn on. And then unplug the HMI.',
    HMISeventh: '3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.',
    ControllerForth: '1. Plug the controller to service tool to init update process. If update starting, service tool "CAN Bus" LED keeps blinking. ',
    ControllerFifth: '2. If update is successful, the "Test" LED of service tool turn on. And then unplug the controller.',
    ControllerSixth: '3. Reset the box to normal status. Otherwise, you would get step 1 repeatedly.',
    secondVersionTitle: 'Besides first version of service tool, other service tool support update software to meet user\'s new requirements',
    secondVersion: 'Second version',
    thirdVersion: 'Third version',
    secondVersionFirst: '1. Probably, you would get the update from online',
    secondVersionSecond: '2. Double click "Update" to download software to service tool and update itself then.During this process,you will see service tool "PC" LED blinking. If "PC" LED keeps on, you need reset service tool to normal.If "Test" LED already turn on, you just need click "Update" once again.',
    secondVersionThird: '3. If update is successful, service tool change to normal state.',
    figureFirst: 'figure 1',
    figureSecond: 'figure 2',
    thirdVersionFirst: '2. Click "Update" once to download software to service tool and update itself then.During this process,you will see service tool "PC" LED blinking. If "PC" LED keeps on, you need reset service tool to normal. If "Test" LED already turn on, you need click "Update" again.',
    // defect
    bikeLabel_1: "No power assist function",
    bikeLabel_2: "Motor can't stop",
    bikeLabel_3: "Bike can't run",
    bikeLabel_4: "Motor works with no power assist",
    bikeLabel_5: "Power assist is not continuous",
    bikeLabel_6: "Power assist is weakened",
    bikeLabel_7: "Battery power and display does not match",
    bikeLabel_8: "The vehicle resonance noise or jitter",
    bikeLabel_9: "The light can't work properly",
    bikeLabel_10: "The rear light does not work properly",
    bikeLabel_11: "Mileage is shorten",
    bikeLabel_12: "No speed on display",
    motorLabel_1: "The noise is large and not continuous",
    motorLabel_2: "No power, high transmission noise",
    motorLabel_3: "Connector and wire damage",
    motorLabel_4: "Spindle loose",
    motorLabel_5: "Cover damage",
    motorLabel_6: "Running trembling",
    motorLabel_7: "The motor housing temperature is too high",
    motorLabel_8: "The motor is not waterproof",
    motorLabel_9: "The motor burned out",
    motorLabel_10: "Fixed bolt hole damage",
    hmiLabel_1: "The cover broken",
    hmiLabel_2: "The button does not work",
    hmiLabel_3: "Screen fog",
    hmiLabel_4: "Connector and wire damage",
    hmiLabel_5: "The display stand is damaged",
    hmiLabel_6: "Show abnormality",
    hmiLabel_7: "The screen is damaged",
    hmiLabel_8: "Bluetooth cannot be online",
    hmiLabel_9: "The vibration function does not work",
    ctlLabel_1: "Controller burned",
    ctlLabel_2: "Fixed part is loose",
    ctlLabel_3: "Controller is not waterproof",
    ctlLabel_4: "Connector and wire damage",
    batLabel_1: "The cover broken",
    batLabel_2: "Skateboard damage",
    batLabel_3: "Discharge port damage",
    batLabel_4: "The cable is broken",
    batLabel_5: "Can't charge",
    batLabel_6: "Battery has no output",
    batLabel_7: "The dust plug damage",
    batLabel_8: "Power level and button damage",
    batLabel_9: "Key damaged",
    batLabel_10: "Key lost",
    batLabel_11: "Battery disassembly difficult",
    sensor_1: "Axial loose",
    sensor_2: "The lock nut is loose",
    sensor_3: "Sensor rust",
    sensor_4: "Connector and wire damage",
    sensor_5: "Sensor bearing damage",
    sensor_6: "Left wrist damage",
    sensor_7: "Right wrist damage",

    checkSnHmi: "The SN of HMI should start with 'DP'.",
    checkSnCtrl: "The SN of controller should start with 'CR'.",
    checkSnSensor: "The SN of sensor should start with 'SR'.",
    checkSnTool: "The SN of BESST should start with 'BE'.",

    inputValueTip: "Please input value",
    readDateTip: "Please read data",
};

const Hmi = {
    hmi: "HMI",
    auth: "Auth",
    applyHmiAuth: "Apply HMI auth",
    applyAllAuth: "Apply all auth",
    noHmiAuthTip: "You don't have write HMI auth, please ask brand for permission.",
    applySuccess: "Apply successfully, Please wait for authorization. ",
    brandEmptyTip: "You don't have any partner brand",
    getApplySuccess: "Get auth successfully",
    hmiTip: 'Main data is empty. Please turn on HMI to "PC" mode or check whether HMI is powered',
    hmiTipCan: "Main data is empty. If HMI is connected and switched on, the software might need update.",
    mileageTotalSupport: "Support integer values,like 1000km. Only the integer part will be written",
    mileageTotalMax: "Maximum value is 99999. Please check the value.",
    mileageSingleSupport: "Support values like 1000.1km. Only the integer part and the first fractional part will be written",
    mileageSingleMax: "Max value is 9999.9. Please check the value.",
    mileageSingleLess: "Single mileage should not larger than total mileage",
    mileageTotalLarger: "Total mileage should not less than single mileage",

    pairCardIn: "Switch to the state of matching key card",
    pairCardSuccess: "Successfully matched the key card",

    cNam: AttributeName
};

const Controller = {
    controller: "Controller",
    auth: "Auth",
    applyControllerAuth: "Apply controller auth",
    applyAllAuth: "Apply all auth",
    noControllerAuthTip: "You don't have write controller auth, please ask brand for permission.",
    applySuccess: "Apply successfully, Please wait for authorization. ",
    brandEmptyTip: "You don't have any partner brand",
    getApplySuccess: "Get auth successfully",
    controllerTip: "Main data is empty. Please check whether controller is powered",
    cNam: AttributeName
};

const Battery = {
    battery: "Battery",
    batteryTip: "Main data is empty. Please check whether battery is powered or switched on",
    cNam: AttributeName
};

const Update = {
    update: "Update",
    fileCloudToUpdate: "Get a file from cloud",
    fileToUpdate: "Get a file from local",
    itemName: "Item Name",
    tip: "Tip",
    updateTip: "Please select the item first, download hardware file, and update.",
    noHardwareFileTip: "The item has't any hardware file.",
    noResultTip: "There are no match condition data.",
    picture: "Picture",
    name: "Name",
    partNo: "Article No.",
    operation: "Operation",
    version: "Version",
    updateTime: "Update Time",
    remark: "Remark",
    select: "Select",
    firmwareList: "Firmware List",
    objectEmptyTip: "The category hasn't any item",
    componentTypeTip: "Please select component",

    componentReadTip: "Please read again.",
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
    setting: "Setting",
    language: "Language",
    version: "Version",
    versionDate: "Version Date",
    userInfo: "Account",
    searchForUpdate: "Search for Update",
    changePassword: "Change Password",
    editCompanyInfo: "Edit Company Info",
    password: 'Password',
    oldPassword: "Old Password",
    newPassword: "New Password",
    confirmPassword: "Confirm Password",
    confirm: "Confirm",
    cancel: "Cancel",
    authTipOne: "You do not have any permissions",
    authTipTwo: "Please contact the company's administrator",
    warning: "Warning",
    editCompany: "Edit Company",
    logo: "Logo",
    uploadTipOne: "You'd better upload square picture with background",
    uploadTipTwo: "support jpg,png and 300*300/400*400, up to 3MB",
    name: "Name",
    email: "E-mail",
    phone: "Phone",
    website: "Website",
    address: "Address",
    postcode: "Postcode",
    company: "Company",
    country: "Country",

    languages: {
        german: "Deutsch",
        english: "English",
        chinese: "中文",
        dutch: "Nederlands"
    },
    orgType: {
        brand: "BRAND",
        dealer: "DEALER",
        assembler: "ASSEMBLER",
        component: "COMPONENT",
        serviceCenter: "SERVICE CENTER",
    },
    updToolNote1: "BESST service tool need update new software to better match the component features.",
    updToolNote: "Note",
    updToolNote2: "Please do not close this dialog window until you update successfully.",
    updToolNote3: "If successfully update, service tool would go to normal status.",
    updToolNote4: "If update progress bar don't show, disconnect the service tool from computer and connect to PC again. Then click 'update'.",
    updStarting: "Update starting...",
    updSuccess: "Update successfully?",
    close: "Close",
    cNam: AttributeName,
    upd: Update,
    op: Operation

};

const Repair = {
    cNam: AttributeName
};

// bike basic
const VehicleDetail = {
    back: "Back",
    sn: "SN",
    key: "Key",
    productTime: "Produce Time",
    itemName: "Model Name",
    itemPartNo: "Item Part No.",
    orderSn: "Order SN",
    orderSourceId: "Po No.",
    customerId: "Customer Name",
    customerPartNo: "Customer Article No.",
    customerPoNo: "Customer Po No.",
    replace: "Replace",
    oldSn: "Old SN",
    newSn: "New SN",
    confirm: "Confirm",
    cancel: "Cancel",
    save: "Save",
    edit: "Edit",
    add: "Add",
    reportList: "Report List",
    detail: "Detail",
    operation: "Operation",
    date: "Date",
    author: "Author",
    emptyListTip: "The vehicle hasn't any report ",
    assembler: "Assembler",
    dealer: "Dealer",
    saleTime: "Sale Time",
    customerName: "Customer Name",
    customerEmail: "Customer E-mail",
    part: "Part",
    history: "History"
};

const VehicleList = {
    oem: "OEM",
    modelName: "Model Name",
    brand: "Brand",
    vehicleSn: "Vehicle SN",
    bomSn: "Component SN",
    search: "Search",
    noResult: "No Result！",
    total: "Total",
    deleteVehicle: "Delete Vehicle",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    delete: "Delete",
    detail: "Detail",
    noSale: "No sales",
    tableHead: {
        vehicleSn: "Vehicle SN",
        brandName: "Brand Name",
        ModelName: "Model Name",
        OrderSn: "Order SN",
        saleDate: "Sale Date",
        operation: "Operation",
    },
    deleteSuccess: "Delete successfully",
    export: "Export",
    start: "Start",
    end: "End",
    op: Operation,
};


// 服务中心相关
const SCCenterTicketList = {
    category: "Category",
    status: "Status",
    tableHead: {
        serviceNo: "Service No.",
        title: "Title",
        targetSn: "Vehicle SN",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation",
    },
    categoryList: {
        all: "All",
        vehicle: "Vehicle",
        motor: "Motor",
        hmi: "HMI",
        sensor: "Sensor",
        battery: "Battery",
        light: "Light",
        controller: "Controller",
        other: "Other",
    },
    statusList: {
        all: "All",
        pendingAssign: "Pending Assign",
        processing: "Processing",
        complete: "Complete"
    },
    noResult: "No Result！",
    total: "Total"
};

const SCHandleUpTicketList = {
    category: "Category",
    status: "Status",
    tableHead: {
        serviceNo: "Service No.",
        title: "Title",
        targetSn: "Vehicle SN",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation",
    },
    categoryList: {
        all: "All",
        vehicle: "Vehicle",
        motor: "Motor",
        hmi: "HMI",
        sensor: "Sensor",
        battery: "Battery",
        light: "Light",
        controller: "Controller",
        other: "Other",
    },
    statusList: {
        all: "All",
        pendingAssign: "Pending Assign",
        processing: "Processing",
        complete: "Complete"
    },
    noResult: "No Result！",
    total: "Total"
};

const SCMyTicketList = {
    category: "Category",
    status: "Status",
    tableHead: {
        serviceNo: "Service No.",
        title: "Title",
        targetSn: "Vehicle SN",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation",
    },
    categoryList: {
        all: "All",
        vehicle: "Vehicle",
        motor: "Motor",
        hmi: "HMI",
        sensor: "Sensor",
        battery: "Battery",
        light: "Light",
        controller: "Controller",
        other: "Other",
    },
    statusList: {
        all: "All",
        pendingAssign: "Pending Assign",
        processing: "Processing",
        complete: "Complete"
    },
    noResult: "No Result！",
    total: "Total",
    addTicketDialog: {
        dialogTitle: "Add Service Ticket",
        category: "Category",
        title: "Title",
        description: "Description",
        bike: "E-Bike",
        sn: "SN",
        serviceCenter: "Service Center",
        image: "Image",
        video: "Video",
        clickToUpload: "Click to upload"
    },
};

const SCTicketDetail = {
    back: "Back",
    transferToUp: "Transfer To Up",
    transferToInside: "Transfer To Inside",
    take: "Take",
    history: "History",
    ticketInfo: {
        ticketTitle: "Complaint details",
        serviceNo: "Service No.",
        createTime: "Create Time",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Title",
        description: "Description",
        image: "Image",
        video: "Video",
    },
    userInfo: {
        address: "Address",
        website: "Website",
        email: "Email",
    },
    content: "Content",
    image: "Image",
    sendMessage: "Send Message",
    historyTicket: {
        title: "Ticket History",
        name: "Name",
        operation: "Operation",
        time: "Time",
    },
    handleInside: {
        title: "Handle Inside",
        tip: "Choice one person to handle the ticket",
        select: "Select",
        confirm: "Confirm",
        cancel: "Cancel"
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
    back: "Back",
    sn: "SN",
    category: "Category",
    modelName: "Model Name",
    partNo: "Article No.",
    orderNo: "Order No.",
    customerName: "Customer Name",
    customerPartNo: "Customer Article No.",
    customerPoNo: "Customer Po No.",
    produceTime: "Produce Time",
    saleTime: "Sale Time",
};

const ComponentComponentList = {
    yes: "Yes",
    no: "No",
    search: {
        poNo: "Article No.",
        orderNo: "Order No.",
        modelName: "Model Name",
        sn: "SN",
        customerName: "Customer Name",
        search: "Search",
        export: "Export",
    },
    headData: {
        poNo: "Article No.",
        orderNo: "Order No.",
        sn: "SN",
        category: "Category",
        modelName: "Model Name",
        customerName: "Customer Name",
        operation: "Operation"
    },
    noResult: "No Result！",
    total: "Total",
    detail: "Detail",
    delete: "Delete",
    batchDelete: "Batch Delete",
    deleteDialog: {
        title: "Delete component",
        content: "Are you sure to delete?",
    },
    exportDialog: {
        title: "Export Component",
        startNum: "Start Num",
        endNum: "End Num",
        export: "Export",
        cancel: "Cancel"
    },
    back: "Back",
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
        content: "Are you sure to delete it?",
        yes: "Yes",
        no: "No",
    },
    headData: {
        date: "Date",
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
    back: "Back",
    fileDetail: "Feedback Info",
    edit: "Edit",
    delete: "Delete",
    detail: {
        title: "File Info",
        email: "Email",
        fileName: "FileName",
        setTime: "Set Time",
        status: "Status",
        upload: "Upload Feedback",
        desc: "Desc"
    },
    deleteDialog: {
        title: "Delete Feedback",
        content: "Are you sur to delete it?",
        yes: "Yes",
        no: "No",
    },
    fileList: "fileList",
    saveFeedback: "Save Feedback",
    deleteFile: "Delete"
};

const ComponentTestFeedbackSave = {

    createFeedback: "Create Feedback",
    feedbackInfo: "Feedback Info",

    email: "Email",
    fileName: "FileName",
    setTime: "Set Time",
    upload: "UpLoad Feedback",
    status: "Status",
    desc: "Desc",

    file: "Upload",
    clickUpload: "Click UpLoad",

    delete: "Delete",
    download: "Download",
    fileList: "fileList",
    saveFeedback: "Save Feedback",
    deleteFile: "Delete",
};

const ComponentTestFeedback = {
    List: ComponentTestFeedbackList,
    Save: ComponentTestFeedbackSave,
    Detail: ComponentTestFeedbackDetail
};
const ComponentMaterialList = {
    orderNo: "Order No.",
    clientCode: 'Customer Code',
    modelDes: "Model Des",
    numQty: "Num/Qty",
    status: "Status",
    proLine: "Production Line",
    date: "Date",
    remark: "Remark",
    operation: "Operation",

    startDate: "Start Date",
    endDate: "End Date",
    connError: "Connection Error",
    search: "Search",
    batchDelete: "Batch Delete",
    upload: "Upload File",
    dialogUploadText: " Upload Production Order File",
    deleteDialogTitle: "Delete Production Order",
    deleteDialogContent: "Are you sure to delete?",
    yes: "Yes",
    no: "No",
    edit: "Edit",
    editDialogTile: "Update Information",
    noResult: "No Result！",
    total: "Total",

    delete: 'Delete',
    detail: 'Detail',

    all: "All",
    line1: "Line1",
    line2: "Line2",
    line3: "Line3",
    line4: "Line4",
    line5: "Line5",
    line6: "Line6",
    line7: "Line7",
    s1: "In plan",
    s2: "In preparation",
    s3: "In assembly",
    s4: "In storage",
    s5: "In test",
    s6: "Abnormal assembly",
    s7: "Abnormal test",

};

const ComponentMaterialDetail = {
    back: "Back",
    delete: "Delete",
    title1: "Order Info",
    title2: "Status Check",
    orderNo: "Order No.",
    user: "User",
    status: "Status",
    desc: "Desc",
    qty: "Quantity",
    num: "Number",
    drawing: "Drawing",
    stator: "Stator",
    exterior: "Appearance",

    deleteDialog: {
        title: "Delete Production Order",
        content: " Are you sure to delete it?",
        yes: "Yes",
        no: "No",
    },

    scanning: "Scan QR code",

    show1: "Show abnormal assembly QR code",
    show11: "Abnormal assembly",
    show2: "Show abnormal test QR code",
    show22: "Abnormal test",
    connError: "Connection Error",

    s1: "In plan",
    s2: "In preparation",
    s3: "In assembly",
    s4: "In storage",
    s5: "In test",
    s6: "Abnormal assembly",
    s7: "Abnormal test",
};
const ComponentMaterial = {
    List: ComponentMaterialList,
    Detail: ComponentMaterialDetail,
};
// 零部件相关 --- order
const ComponentOrderDetail = {
    back: "Back",
    orderDetail: "Order Detail",
    edit: "Edit",
    delete: "Delete",
    orderInfo: {
        title: "Order Info",
        orderNo: "Order No.",
        customerPartNo: "Customer Article No.",
        note: "Note",
        customerName: "Customer Name",
        customerPo: "Customer Po.",
    },
    itemsList: "Items List",
    produce: "Produce",
    testReport: "Test Report",
    componentList: "Component List",
    replace: "Replace",
    headData: {
        modelName: "Model Name",
        articleNo: "Article No.",
        customerArticleNo: "Customer Article No.",
        quantity: "Quantity",
        operation: "Operation",
    },
    deleteDialog: {
        title: "Delete Order",
        content: "Are you sure to delete it?",
        yes: "Yes",
        no: "No"
    },
    category: "Category",
    search: "Search",
    clear: "Clear",
    selectCategory: "Select Search",
    modelNamePN: "Model Name/Article No.",
    confirmDialog: {
        replace: "replace",
        yes: "Yes",
        no: "No"
    },

    dataConnect: "Data Connect"
};

const ComponentOrderList = {
    search: {
        orderNo: "Order No.",
        searchBtn: "Search",
        clearBtn: "Clear",
    },
    add: "Add",
    list: "List",
    detail: "Detail",
    delete: "Delete",
    noResult: "No Result！",
    total: "Total",
    headData: {
        date: "Date",
        orderNo: "Order No.",
        customerName: "Customer Name",
        pQ: "Produced/Quantity",
        status: "Status",
        operation: "Operation",
        model: "Model",
    },
    deleteDialog: {
        title: "Delete Order",
        content: "Are you sure to delete it?",
        yes: "Yes",
        no: "No"
    },
    op: Operation,

};

const ComponentOrderProduce = {
    back: "Back",
    connect: "Connect",
    reset: "Reset",
    update: "Update",
    readSn: "Read SN",
    writeSn: "Write SN",
    orderInfo: {
        title: "Order Info",
        model: "Model",
        customerName: "Customer Name",
        customerPoNo: "Customer Po No.",
        customerPartNo: "Customer Article",
    },
    programingInfo: {
        title: "Programing",
        com: "COM",
        file: "File",
        pleaseSelect: "Please select"
    },
    writeSnInfo: {
        title: "Write SN",
        com: "COM",
        pleaseSelect: "Please select",
        sn: "SN"
    },
    cNam: AttributeName,
    up: Update,
};

const ComponentOrderDataConnect = {
    back: "Back",
    save: "Binding",
    clear: "Clear Up",
    removeBinding: 'Remove Binding',
    orderInfo: {
        title: "Order Info",
        model: "Model",
        customerName: "Customer Name",
        customerPoNo: "Customer Po No.",
        customerPartNo: "Customer Article",
    },
    programingInfo: {
        ListTitle: "Result List",
        searchTitle: 'Search/Scan',
        motorTitle: 'Motor',
        result: 'Result',
        pleaseSelect: "search empty",
        sn: 'SN',
        cr: 'Controller',
        sr: 'Sensor',
        mm: 'Motor'
    },

};

const ComponentOrderSave = {
    back: "Back",
    createOrder: "Create Order",
    orderInfo: {
        title: "Order Info",
        poNo: "Po No.",
        customerInfo: "Customer Info",
        customerPartNo: "Customer Article No.",
        customerName: "Customer Name",
        customerPo: "Customer Po.",
        note: "Note",
    },
    itemList: "Item List",
    addItem: "Add Item",
    addItems: "Add Items",
    delete: "Delete",
    saveOrder: "Save Order",
    tableHead: {
        name: "Name",
        articleNo: "Article No.",
        customerArticleNo: "Customer Article No.",
        quantity: "Quantity",
        operation: "Operation",
    },
    itemHeadData: {
        image: "Image",
        name: "Name",
        partNo: "Article No."
    },
    category: "Category",
    search: "Search",
    clear: "Clear",
    selectCategory: "Select Search",
    modelNamePN: "Model Name/Article No."
};

const ComponentOrderTestReport = {
    back: "Back",
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
    back: "Back",
    add: "Add",
    save: "Save",
    edit: "Edit",
    print: "Print",
    export: "Export",
    delete: "Delete",
    detail: "Detail",
    articleNO: "Article No.",
    noPhoto: "No Photo",
    viewLargerImage: "View larger image",
    list: "List",
    partName: "Part name",
    yes: "Yes",
    no: "No",
    listHead: {
        itemNo: "ITEM NO.",
        partNo: "ARTICLE NO.",
        picture: "PICTURE",
        name: "NAME",
        spec: "SPEC",
        package: "PACKAGE",
        packageSize: "PACKAGE SIZE",
        price: "PRICE",
        linkBom: "LINK BOM",
        operation: "OPERATION"
    },
    selectHead: {
        partNo: "ARTICLE NO.",
        picture: "PICTURE",
        name: "NAME",
        spec: "SPEC",
        package: "PACKAGE",
        packageSize: "PACKAGE SIZE",
        price: "PRICE",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "Delete Product",
        content: "Are you sure to delete it? ",
    },
    addItemDialog: {
        title: "Add Item",
    },
    operation: "OPERATION",
    tip: "Tip",
    content01: "You'd better upload square picture with background",
    content02: "support jpg, png and 300*300/400*400, up to 3MB"
};

const ComponentProductDetail = {
    back: "Back",
    copy: "Copy",
    edit: "Edit",
    delete: "Delete",
    model: "Model",
    name: 'Name',
    partNo: "Article No.",
    articleList: "Article List",
    desc: "Description",
    download: "Download",
    preview: "Preview",
    history: "History",
    update: "Upload",
    view: "View",
    hardware: "Hardware Version",
    firmware: "Firmware",
    bomList: "BOM List",
    yes: "Yes",
    no: "No",
    confirm: "Confirm",
    cancel: "Cancel",
    deleteProductDialog: {
        title: "Delete Product",
        content: "Are you sure to delete it? ",
    },
    deleteFirmwareDialog: {
        title: "Delete Software",
        content: "Are you sure to delete this software? ",
    },
    historyDialog: {
        title: "Software History",
        version: "Version",
        updateTime: "Update Time",
        remark: "Remark",
        operation: "Operation",
        noResult: "No Result！"
    },
    editFirmwareDialog: {
        title: "Upload Firmware",
        file: "File",
        version: "Version",
        remark: "Remark",
        clickToUpload: "Click to upload",
    },
    updateFirmwareDialog: {
        title: "Upload Firmware",
        file: "File",
        version: "Version",
        remark: "Remark",
        clickToUpload: "Click to upload",
    },
    training: 'Training',
    set: ArticleSet

};

const ComponentProductEditCategory = {
    back: "Back",
    category: "Category",
    yes: "Yes",
    no: "No",
    confirm: "Confirm",
    cancel: "Cancel",
    deleteDialog: {
        title: "Delete Category",
        content: "Are you sure to delete it? ",
    },
    addSubCategoryDialog: {
        title: "Add Sub Category",
        namePH: "Category Name"
    },
    editCategoryDialog: {
        title: "Edit Category",
        namePH: "Category Name"
    },
    addSubCategory: "Add Sub Category",
    edit: "Edit",
    delete: "Delete",
};

const ComponentProductIndex = {
    editCategory: "Edit Category",
    training: 'Training'
};

const ComponentProductList = {
    add: "Add",
    noResult: "No Result！",
    total: "Total",
    set: ArticleSet
};

const ComponentProductSave = {
    back: "Back",
    logo: "Picture",
    name: "Name",
    partNo: "Article No.",
    desc: "Description",
    save: "Save",
    tip: {
        title: "Tip",
        content01: "You'd better upload square picture with background",
        content02: "support jpg,png and 300*300/400*400, up to 3MB",
    },
    add: "Add",
    copy: "Copy",
    edit: "Edit",
    set: ArticleSet

};

const ComponentVideoList = {
    back: "Back",
    category: "Category",
    add: "Add",
    download: "Download",
    delete: "Delete",
    noResult: " No result !",
    total: "Total",
    uploadVideo: "Upload Video",
    upload: "Upload",
    confirm: "Confirm",
    cancel: "Cancel",
    all: "All",
    systemError: "System Error",
    hmi: "HMI",
    motor: "Motor",
    controller: "Controller",
    sensor: "Sensor",
    battery: "Battery",
    charger: "Charger",
    name: "Name",
    operation: "Operation",
    videoEmptyTip: "Please upload video",
    saveSuccess: "Save successfully",
    yes: "Yes",
    no: "No",
    deleteVideo: "Delete Video",
    deleteTip: "Are you sure to delete it ?",
    preview: "Preview",
    errorCode: "Error Code",
    link: "Link",
    edit: "Edit"
};

const ComponentProductVideoList = {
    back: "Back",
    category: "Category",
    addVideo: "Add Video",
    download: "Download",
    delete: "Delete",
    noResult: " No result !",
    confirm: "Confirm",
    cancel: "Cancel",
    systemError: "System Error",
    hmi: "HMI",
    motor: "Motor",
    controller: "Controller",
    sensor: "Sensor",
    battery: "Battery",
    charger: "Charger",
    name: "Name",
    operation: "Operation",
    yes: "Yes",
    no: "No",
    preview: "Preview",
    deleteVideo: "Delete Video",
    deleteTip: "Are you sure to delete it ?",
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
    ticketStat: "Ticket Statistics",
    ticketByCategory: "Ticket By Category",
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
    order: "Order",
    diagnosis: "Diagnosis"
};

// 经销商相关 --- error
const DealerErrorShooting = {
    errorCode: "Error Code",
    name: "Name",
    remark: "Remark",
    description: "Description",
    target: "Target",
    solution: "Solution",
    tip: "Tip",
    tipContent: "If the above solution can't solve your problem, please contact with brand.",
    help: "Help",
    helpContent: "Please click here to get more support.",
    link: "Video Entrance"
};

const DealerError = {
    Shooting: DealerErrorShooting,
};

// 经销商相关 --- partner
const DealerPartnerList = {
    back: "Back",
    acceptInvitation: "Accept Invitation",
    tableHead: {
        name: "Name",
        phone: "Phone",
        website: "Website",
        country: "Country",
        operation: "Operation",
    },
    currentBrand: "Current Brand",
    noResult: "No Result！",
    total: "Total",
    brandInfo: {
        title: "Brand Info",
        logo: "Picture",
        name: "Name",
        type: "Type",
        email: "E-mail",
        website: "Website",
        tel: "Tel",
        postcode: "Postcode",
        address: "Address",
    },
    invitationCode: "Invitation Code",
    confirm: "Confirm",
    cancel: "Cancel",
    webLink: "Website Link",
    createOrder: "Create Order",
    orderList: "Order List",
    view: "View",
};

const DealerPartner = {
    List: DealerPartnerList,
};

// 经销商相关 --- sale
const DealerSaleDetail = {
    back: "Back",
    orderDetail: "Order Detail",
    view: "View",
    edit: "Edit",
    delete: "Delete",
    invoiceNo: "Invoice No.",
    invoiceDate: "Invoice Date",
    submit: "Submit",
    customerInfo: {
        title: "Customer Info",
        name: "Name",
        phone: "Phone",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Bike Info",
        brand: "Brand",
        bikeId: "Bike Id",
        model: "Model",
        warrantyPeriod: "Warranty Period"
    },
    detail: {
        title: "Details",
        orderNo: "Order No.",
        price: "Price",
        invoice: "Invoice",
        salesDate: "Sales Date",
        remark: "Remark",
    },
    deleteDialog: {
        title: "Delete Order",
        content: "Are you sure to delete it?",
        yes: "Yes",
        no: "No"
    }
};

const DealerSaleEdit = {
    back: "Back",
    save: "Save",
    orderEdit: "Order Edit",
    customerInfo: {
        title: "Customer Info",
        name: "Name",
        phone: "Phone",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Bike Info",
        brand: "Brand",
        bikeId: "Bike Id",
        model: "Model",
        warrantyPeriod: "Warranty Period"
    },
    detail: {
        title: "Details",
        orderNo: "Order No.",
        price: "Price",
        invoice: "Invoice",
        remark: "Remark",
        invoiceDate: "Invoice Date",
    },
};

const DealerSaleList = {
    add: "Add",
    tableHead: {
        orderNo: "Order No.",
        bikeId: "Bike ID",
        model: "Model",
        customerName: "Customer Name",
        date: "Date",
        status: "Status",
        operation: "Operation"
    },
    noResult: "No Result!",
    total: "Total",
    detail: "Detail",
    search: {
        bikeId: "Bike ID",
        searchBtn: "Search",
        clearBtn: "Clear"
    }
};

const DealerSaleSave = {
    back: "Back",
    addComponent: "Add Component",
    save: "Save",
    confirm: "Confirm",
    cancel: "Cancel",
    customerInfo: {
        title: "Customer Info",
        name: "Name",
        phone: "Phone",
        email: "E-mail",
        tip: "Tip",
        tip01: "E-mail associated with after-sales service and key",
        tip02: "please make sure E-mail is valid. "
    },
    bikeInfo: {
        title: "Bike Info",
        bikeId: "E-Bike Id",
        brand: "Brand",
        warrantyPeriod: "Warranty",
        date: "Date",
        price: "Price",
        remake: "Remark",
    },
    componentInfo: {
        add: "Add",
        title: "Component Info",
        replace: "Replace",
        sn: "SN",
        name: "Name"
    },
    replaceDialog: {
        oldSn: "Old SN",
        newSn: "New SN",
        category: "Category",
    },
    tipDialog: {
        title: "Tip",
        content: "The E-bike hasn't any battery, are you sure to save order?"
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
    back: "Back",
    markCompleted: "Mark Completed",
    ticketInfo: {
        InfoTitle: "Complaint details",
        serviceNo: "Service No.",
        status: "Status",
        createTime: "Create Time",
        serviceCenter: "Service Center",
        title: "Title",
        description: "Description",
        image: "Image",
        video: "Video",
    },
    user: {
        address: "Address",
        website: "Website",
        email: "E-mail",
    },
    replay: {
        content: "Content",
        image: "Image",
        sendMessage: "Send Message"
    },
};

const DealerTicketList = {
    add: "Add",
    confirm: "Confirm",
    cancel: "Cancel",
    tableHead: {
        serviceNo: "Service No",
        title: "Title",
        targetNo: "Target SN",
        ServiceCenter: "Service Center",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation",
    },
    detail: "Detail",
    delete: "Delete",
    noResult: "No Result!",
    total: "Total",
    addTicketDialog: {
        dialogTitle: "Add Service Ticket",
        category: "Category",
        title: "Title",
        description: "Description",
        bike: "E-Bike",
        sn: "SN",
        serviceCenter: "Service Center",
        image: "Image",
        video: "Video",
        clickToUpload: "Click to upload"
    },
    deleteTicket: {
        title: "Delete ticket",
        content: "Are you sure to delete it? "
    }
};

const DealerTicket = {
    Detail: DealerTicketDetail,
    List: DealerTicketList
};

const DealerVehicleSearch = {
    search: {
        bikeId: "Bike ID",
        searchBtn: "Search",
        clear: "Clear"
    },
    back: "Back",
    sn: "SN",
    key: "Key",
    productTime: "Produce Time",
    itemName: "Model Name",
    itemPartNo: "Item Part No.",
    orderSn: "Order SN",
    orderSourceId: "Order Source Id",
    customerId: "Customer ID",
    customerPartNo: "Customer Article No.",
    customerPoNo: "Customer Po No.",
    replace: "Replace",
    oldSn: "Old SN",
    newSn: "New SN",
    confirm: "Confirm",
    cancel: "Cancel",
    save: "Save",
    edit: "Edit",
    reportList: "Report List",
    detail: "Detail",
    operation: "Operation",
    date: "Date",
    author: "Author",
    emptyListTip: "The vehicle hasn't any report ",
    part: "Components",
    history: "Replace History",
    brand: "Brand",
    saleTime: "Sale Time",
    customerName: "Customer Name",
    customerEmail: "Customer Email"
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
    dailyVehicleOrderStatistics: "Order statistics",
    dailyVehicleAfterSalesStatistics: "After sales statistics",
    dailyDailyVehicleStatistics: "Dealer statistics",
    dailyAssemblerVehicleStatistics: "Assembler(OEM) statistics"
};

//品牌商
const BrandList = {
    add: "Add",
    edit: "Edit",
    delete: "Delete",
    noResult: "No Result!",
    total: "Total",
    deleteBrand: "Delete Brand",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    addBrand: "Add Brand",
    brandInfo: "Brand Info",
    logo: "Image",
    uploadTipOne: "You'd better upload square picture with background",
    uploadTipTwo: "support jpg,png and 300*300/400*400, up to 3MB",
    name: "Brand Name",
    introduction: "Introduction",
    confirm: "Confirm",
    cancel: "Cancel",
    operation: "Operation",
    emptyTip: {
        logo: "Please upload brand Logo",
        name: "Please input brand name",
        desc: "Please input brand introduction",
    },
    editSuccess: "Edit successfully",
    addSuccess: "Add successfully",
    deleteSuccess: "Delete successfully",

    link: "Link",
};

const DealerInvite = {
    codeStatus: "Code Status",
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        invitation: "Invitation Code",
        targetName: "Organization Name",
        contact: "Contacts",
        targetEmail: "Target Email",
        createTime: "Create Time",
    },
    codeStatusOptions: {
        readyToSend: "ready to send",
        sentNotUsed: "sent, but not used",
        used: "used",
    },
    emptyTip: {
        email: "Please input E-mail",
        name: "Please input company name",
    },
    sendSuccess: "Send successfully",
    uploadSuccess: "Upload successfully",
    selectTip: "Please select account",
    deleteSuccess: "Delete successfully",
    newCode: "New Code",
    batchSend: "Batch Send",
    send: "Send",
    delete: "Delete",
    dealerInvitation: "Dealer Invitation",
    selectAccount: "Select Account",
    uploadContacts: "Upload Contacts",
    operation: "Operation",
    confirm: "Confirm",
    cancel: "cancel",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    email: "E-mail",
    companyName: "Company Name",
    toLeft: "To left",
    toRight: "To right",
    deleteCode: "Delete Code",
    orgName: "Org Name",
    uploadFailList: "Upload Fail List",
    reason: "Reason",
    uploadFailTip: "Upload fail, Please check the file or all of the contacts exist",
    resend: "Resend",
    template: "Contacts Template",
    download: "Download",

    source: "Source",
    target: "Target",
    enterKeyWord: "Enter key word",
    noData: "No data",
    dealerType: "Dealer Type",
    dealerType1: "Dealer",
    dealerType2: "Special Dealer",
    dealerTypeNote: "Special Dealer only have diagnosis and tool function. Dealer has full function.",
    op: Operation,

};

const DealerList = {
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        logo: "Logo",
        name: "Name",
        postCode: "Post Code",
        tel: "Tel",
        website: "Website",
        address: "Address",
        operation: "Operation",
        email: "E-mail",
    },
    delete: "Delete",
    deleteDialog: {
        title: "Delete Dealer",
        tip: "Are you sure to delete it?"
    },
    yes: "Yes",
    no: "No",
    op: Operation,
};

const OemInvite = {
    codeStatus: "Code Status",
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        invitation: "Invitation Code",
        targetName: "Organization Name",
        contact: "Contacts",
        targetEmail: "Target Email",
        createTime: "Create Time",
    },
    codeStatusOptions: {
        readyToSend: "ready to send",
        sentNotUsed: "sent, but not used",
        used: "used",
    },
    emptyTip: {
        email: "Please input E-mail",
        name: "Please input company name",
    },
    sendSuccess: "Send successfully",
    uploadSuccess: "Upload successfully",
    selectTip: "Please select account",
    deleteSuccess: "Delete successfully",
    newCode: "New Code",
    batchSend: "Batch Send",
    send: "Send",
    delete: "Delete",
    oemInvitation: "OEM Invitation",
    selectAccount: "Select Account",
    uploadContacts: "Upload Contacts",
    operation: "Operation",
    confirm: "Confirm",
    cancel: "cancel",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    email: "E-mail",
    companyName: "Company Name",
    toLeft: "To left",
    toRight: "To right",
    deleteCode: "Delete Code",
    orgName: "Org Name",
    uploadFailList: "Upload Fail List",
    reason: "Reason",
    uploadFailTip: "Upload fail, Please check the file or all of the contacts exist",
    resend: "Resend",
    download: "Download",
    template: "Contacts Template",
    op: Operation,

};

const OemList = {
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        logo: "Logo",
        name: "Name",
        postCode: "Post Code",
        tel: "Tel",
        website: "Website",
        address: "Address",
        operation: "Operation",
        email: "E-mail",
    },
    delete: "Delete",
    deleteDialog: {
        title: "Delete OEM",
        tip: "Are you sure to delete it?"
    },
    yes: "Yes",
    no: "No",
    op: Operation,
};

const AuthList = {
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        name: "Name",
        type: "Type",
        status: "Status",
        time: "Time",
        operation: "Operation"
    },
    authorization: "Authorization",
    authorizationSuccess: "Authorization successfully",
    status: "Status",
    authorized: "Authorized",
    noAuthorization: "No Authorization",
    hmi: "HMI",
    controller: "Controller"
};

const RecordList = {
    noResult: "No Result!",
    total: "Total",
    tableHead: {
        sn: "SN",
        vehicle: "Vehicle",
        operator: "Operator",
        account: "Account",
        status: "Status",
        time: "Time",
        type: 'Type',
        data: "Data"
    },
    status: "Status",
    recordTitle: "Component SN Operation Record",
    edit: "Edit"
};


const ModelDetail = {
    back: "Back",
    delete: "Delete",
    edit: "Edit",
    copy: "Copy",
    name: "Model Name",
    brand: "Brand",
    partNo: "Article No.",
    description: "Description",
    deleteModel: "Delete Model",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    download: "DownLoad",
    preview: "Preview",
    cancel: "Cancel",
    confirm: "Confirm",
    view: "View",
    bom: "Bom",

    specification: "Specification",
    currency: "Currency",
    year: "Year",
    color: "Color",
    size: "Size",
    gender: "Gender",
    price: "Price",

    link: "Link",
};

const ModelList = {
    brand: "Select bike brand",
    addModel: "Add Model",
    noResult: "No Result！",
    total: "Total",
    deleteModel: 'Delete Model',
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    edit: "Edit",
    delete: "Delete",
    detail: "Detail",
    tableHead: {
        name: "Model Name",
        logo: "Image",
        partNo: "Article No.",
        introduction: "Introduction",
        operation: "Operation",
        show: "Show"
    },
    deleteSuccess: "Delete successfully",
    link: "Link",
    op: Operation,
};

const ModelSave = {
    uploadTipOne: "You'd better upload square picture with background",
    uploadTipTwo: "support jpg,png and 300*300/400*400, up to 3MB",
    add: "Add",
    edit: "Edit",
    copy: "Copy",
    back: "Back",
    save: "Save",
    emptyTip: {
        picture: "Please upload model logo",
        name: "Please input model name",
        partNo: "Please input article No.",
        desc: "Please input model description",
    },
    saveTip: "Please wait save",
    brandWrong: "Brand is wrong",
    saveSuccess: "Save successfully",
    picture: "Picture",
    name: "Name",
    partNo: "Article No.",
    introduction: "Introduction",
    description: "Description",
    cancel: "Cancel",
    confirm: "Confirm",
    editAttr: "Edit Attr",
    attrList: "Attr List",

    specification: "Specification",
    currency: "Currency",
    year: "Year",
    color: "Color",
    size: "Size",
    gender: "Gender",
    price: "Price",
    brand: "Brand",
    link: "Link",
};


const VehicleBomList = {
    back: "Back",
    add: "Add",
    save: "Save",
    edit: "Edit",
    print: "Print",
    export: "Export",
    delete: "Delete",
    detail: "Detail",
    articleNO: "Article No.",
    noPhoto: "No Photo",
    viewLargerImage: "View larger image",
    list: "List",
    partName: "Model Name/Article No.",
    yes: "Yes",
    no: "No",
    listHead: {
        itemNo: "ITEM NO.",
        partNo: "ARTICLE NO.",
        picture: "PICTURE",
        name: "NAME",
        spec: "SPEC",
        package: "PACKAGE",
        packageSize: "PACKAGE SIZE",
        price: "PRICE",
        linkBom: "LINK BOM",
        warranty: "WARRANTY",
        operation: "OPERATION"
    },
    selectHead: {
        partNo: "ARTICLE NO.",
        picture: "PICTURE",
        name: "NAME",
        spec: "SPEC",
        package: "PACKAGE",
        packageSize: "PACKAGE SIZE",
        price: "PRICE",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "Delete Product",
        content: "Are you sure to delete it? ",
    },
    addItemDialog: {
        title: "Add Item",
    },
    operation: "OPERATION",
    selectCategory: "Select Category",
    category: "Category",
    search: "Search",
    clear: "Clear",
    tip: "Tip",
    content01: "You'd better upload square picture with background",
    content02: "support jpg,png and 300*300/400*400, up to 3MB"
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
    add: "Add",
    self: "Self",
    manager: "Manager",
    permission: "Permission",
    reset: "Reset",
    delete: "Delete",
    total: "Total",
    addAccount: "Add Account",
    resetPassword: "Reset Password",
    deleteAccount: "Delete Account",
    email: "E-mail",
    confirm: "Confirm",
    cancel: "Cancel",
    deleteTip: "Are you sure to delete it? ",
    yes: "Yes",
    no: "No",
    resetPasswordTip: "Do you want to set the password for this account to 12345678 ?",
    tableHead: {
        account: "Account",
        createTime: "Create Time",
        operation: "Operation",
    },
    emptyEmailTip: "Please input E-mail",
    addSuccess: "Add successfully",
    deleteSuccess: "Delete Successfully",
    resetSuccess: "Reset successfully",
    saveAuthSuccess: "Save auth successfully",
    noResult: "No Result！",
    invitationLength1: "The role of Dealer could only create 10 sub-accounts",
    invitationLength2: "The role of Assembler could only create 20 sub-accounts",
    invitationLength3: "The role of Brand could only create 20 sub-accounts",
    invitationLength4: "The role of Component could only create 20 sub-accounts",

    cNam: AttributeName
};

const ComponentsReplaceReport = {
    tableHead: {
        category: "Category",
        oldComponent: "Old Component SN",
        newComponent: "New Component SN",
    },
    noResult: "No Result！",
    total: "Total",
    back: "Back"
};

const report = {
    back: "Back",
    diagnosisReportTitle: "E-BIKE Diagnosis Report",
    print: "Print",
    orgLabel: "Org ",
    orgTitle: "Org Info ",
    dealerLabel: "Dealer ",
    dealerTitle: "Dealer Info ",
    assemblerLabel: "Assembler ",
    assemblerTitle: "Assembler Info ",
    name: "Name",
    address: "Address",
    email: "E-mail",
    tel: "Tel.",
    reportDate: "Report Date",
    bike: {
        title: "Bike Info"
    },
    diagnosis: {
        data: "Diagnosis Data",
        result: "Diagnosis Result",
        hmi: "Diagnosis Data - HMI",
        battery: "Diagnosis Data - Battery",
        controller: "Diagnosis Data - Controller",
        sensor: "Diagnosis Data - Sensor",
    },
    cNam: AttributeName,

};

// components diagnosis
const diagnosis3rd = {
    goElectronic: "Do you want to do electronic diagnosis?",
    goMechanic: "Do you want to do mechanical diagnosis?",
    diagnosisElectronic: "Electronic diagnosis",
    diagnosisMechanic: "Mechanical diagnosis",
    eBikeID: "Electric Bicycle ID",
    connection: "Connection",
    newVer: "New version",
    ok: "Ok",
    fail: "Fail",
    con: "Connected",
    disCon: "Disconnected",
    readOk: "Read Ok",
    readFail: "Read Fail",
    detail: "Detail",
    testSys: "Test System",
    cNam: AttributeName,
    rp: report,
    op: Operation,
    step: "Step",
    help: "Help",
    continue: "Continue",
    case: "Case",
    test: "Test",
    finish: "Finish",
    name: "Name",
    remark: "Remark",
    desc: "Description",
    target: "Target",
    solution: "Solution",
    errTitle: "Error Solution",
    reconnect: "Reconnect",
    instruction: "Instruction",

    pedalSensor: "Pedal Sensor",
    throttle: "Throttle",
    brake: "Brake",
    light: "Light",
    support: "Assist mode",
    walk: "Walk Assistance",
    speed: "Speed Signal",
    cadence: "Cadence sensor",
    torque: "Torque sensor",
    motor: "Motor",

    click: "Click",
    step11: "Connect BESST Tool to Computer.",
    step12: "Select the right COM port. ",
    step13: "The light will turn green. ",
    step21: "Select the components of electric bicycle.",
    step22: 'Connect BESST Tool to HMI and Controller. And press the "Reset" button on BESST box',
    step23: 'Press HMI power button to switch HMI to "PC" mode',
    step232: 'Press HMI power button four seconds to turn on system',
    case11: "HMI could not be switched on:",
    case111: "System is not switched on:",
    case12: "Please check cable connection. If still not possible to turn on the display, you will need a new display to continue.",
    case121: "Please check cable connection. Unplug the HMI and plug back.",
    case13: 'The display does not indicate "PC" mode',
    case17: "Press",
    case18: 'on the box and reopen HMI. You could repeat a few times.',
    case181: "on the box. If still still not possible to turn on the display, you need a new HMI to continue.",
    case20: 'If still no "PC" mode in the display, it does not support BESST and you need a new HMI to continue',
    case21: 'Note: If you changed the HMI to move on, then click "Connection" to reconnect.',
    test11: "Suspend the bicycle to prevent injury or damage.",
    test12: "Make sure the wheel is free to rotate before pressing the button!",
    test13: "Select the functions",
    test14: 'Select "All" or "S" , "All" for overall test, "S" for testing one by one;',
    test15: "Start your test process",
    createReport: "Create Report",

    start11: "Please rotate the throttle, then motor should work, then you should see the green light.",
    start13: "If motor works but no green light, throttle is ok. Otherwise, please click",
    start14: "and then replace the part.",
    start15: "Please brake and release the brake lever at least two times, then you should see green light.",
    start16: "If the motor do stop but no green light, brake switch is ok. Otherwise, please click",
    start17: 'Please press and hold the button " ',
    start18: ' " on the HMI twice to turn on or turn off bike light, then you should see the green light.',
    start19: "If the light doesn't work, but you still see the green light, then replace the light. If no light works or you see don't green light, please click",
    start20: "and then replace display unit or controller.",
    start21: ' Please press "+" and "-" alternately, then you should see the green light.',
    start22: "If no green light, please click",
    start23: "and then replace the Display.",
    start24: 'Please press and hold the button "',
    start25: '" or "--", then the motor should work at slow speed, then you should see the green light.',
    start26: "Please turn the wheel forward by hand, then you will see the speed above and get the green light.",
    start27: "If no green light, please adjust the magnet in the right induction position. If still doesn't work, please click",
    start28: "and then replace the sensor and magnet.",
    start29: "Suspend the bicycle to prevent injury or damage.",
    start30: "Make sure the wheel is free to rotate!",
    start31: "Please turn the pedal forward by hand, then you should see the green light.",
    start33: "Start Torque",
    start34: "Please squeeze the brake lever and put around 10kg pressure on the pedal, then you should see the green light.",
    start35: "and then replace the pedal sensor.",
    start36: "< Back",
    start37: "Next >",

    toolTitle1: "First version BESST tool ",
    toolTitle2: "PC mode",
    tool11: " Only support Uart Protocol with Mini USB port in one side and two USB Type B ports on the other side. ",
    tool12: ' "PC" mode is used to read data from display compared with working mode. When HMI connect to BESST tool, if HMI could be switched on, HMI go into "PC" mode.',
    tool13: "LCD Display",
    tool14: 'The screen show "PC" or "Connected OK" while you turn on HMI.',
    tool15: "LED Display",
    tool16: "The light of highest level is on while you turn on HMI.",

    updateTitle: 'Software Version',
    updateHistory: 'Software History',
    update111: "While download firmware to BESST box, please turn off HMI.",
    update1111: "While download firmware to BESST box, please unplug HMI.",
    update112: "While download firmware to BESST box, please unplug controller.",
    update11: "If you have downloaded the firmware to BOX successfully,",
    update12: 'then turn on the HMI until you hear "BEEP" and the HMI is off.',
    update122: 'then plug the HMI until you hear "BEEP" and the HMI is off.',
    update13: "then the update is done.",
    update14: "Press reset button on BESST Software.",
    update15: "Turn on HMI.",
    update16: 'then pulg the controller until you hear "BEEP" .',
    update17: "then the update is done and unplug the controller.",
    update19: "Repower the controller.",
    update20: 'until you hear "BEEP",then the update is done.',

    error: "Error",
    errorDesc: "No data collected. It might be connection failure.",
    errorNote: 'Please click "Reconnect" to connect again.',
    errorHmi: 'Sn is empty. HMI is not registered on BESST. Please reconnect or check whether HMI is in "PC" mode.',
    errorController: "Sn is empty. Controller is not registered on BESST. Please reconnect or test on Tool section.",
    errorBattery: "Sn is empty. Battery is not registered on BESST. Please reconnect or test on Tool section.",
    errorSensor: "Sn is empty. Sensor is not registered on BESST. Please reconnect or test on Tool section.",

    goTest: "Go into test mode successfully.",

    resultTitle: "E-BIKE Diagnosis Report",
    resultBike: "E-BIKE Model:",
    print: "Print",
    bikeId: "E-Bike ID",
    key: "Key Code",
    result: "Result",
    saveReport: "Save Report",
    offLineTip: "In offline mode, you could type vehicle SN and save report on local. Please click print icon on the top of this page.",
    motorSNTip: "The input motor sn is different with the data belong to the bike.",
    bikeSNTip1: "Please type bicycle id.",
    bikeSNTip2: "Bike SN has been changed. Please check.",

    canStep1: "Please wait for one or two seconds...",
    canStep2: "Send command of closing controller successfully.",
    canStep3: "Fail to send command of closing controller. Try to start test again.",
    canStep4: "Controller is off. Please press HMI power button over 3s to open controller",
    canStep5: "Controller is open. Close this window to move on.",
    selectDevice1: "Please select component. HMI and Controller is necessary",
    selectDevice2: "Not select any available device",

    failDesc: "Main data is not collected. It might be connection failure or software problem.",
    failNote: 'Please click "Reconnect" to connect again.',
    failNoteMotor: 'The motor information is empty because no bike traced.',

    analogLight: 'Communication indicator',
    communicationOk: 'Communication is normal',
    brake01: 'Check if the brake of vehicle is equipped with power off function. If brake does not has this function, please click "Next" ',
    brake02: 'Hold the brake lever, and release, repeat multiple times, observe the indicator light, can normally turn green and turn gray, please select "Brakes work normally", otherwise "Brake power off function is not normal" ',
    brake06: 'If the indicator light does not turn green, or the indicator light does not turn gray after it turns green, click "Fail" ',
    brake07: "Brakes work normally",
    brake08: "Brake power off function is abnormal",
    light01: "The light button is",
    light02: 'Press the light button to turn it on and off. Please select "Indicator is normal" and "Indicator is not normal". ',
    light022: 'Please check whether the light is normally turned on and off, please select "Light works normally", otherwise please select "Light is abnormal" ',
    light03: 'Check if the vehicle has a light configuration, if not, press "Next"',
    light04: 'Communication indicator judgement',
    light05: 'Light working status',
    light06: 'If the indicator light does not turn green, or the indicator does not turn gray after it turns green, click "Fail". ',
    light07: 'Indicator is normal',
    light08: 'Indicator is abnormal',
    light09: 'Light works normally',
    light10: 'Light is abnormal',
    support02: 'Press the “+/-” button to cycle through the lowest level to the highest level twice , observe whether this window and display are consistent and responsive in time. You need to select “Buttons work normally” or “Buttons are abnormal”. ',
    support03: 'Buttons work normally',
    support04: 'Buttons are abnormal',
    speed01: 'Keep the drive system switched on, observe the vehicle speed detection sensor position, and rotate the exact wheel according to the direction of the vehicle. Observe the speed of this window and the display. Please press "Speed sensor works normally", otherwise "Speed sensor is abnormal. " ',
    speed04: 'Speed sensor works normally',
    speed05: 'Speed sensor is abnormal',
    cadence01: 'Set the level above 1st level, rotate the pedal according to the riding direction, the observation window should display the speed, please keep 20RPM at a constant speed for more than 10 seconds, it will automatically judge that "Cadence sensor works normally", otherwise press "Cadence sensor is abnormal." ',
    cadence03: 'Cadence sensor is abnormal',
    torque01: 'Set the level above 1st gear, quickly rotate the pedal according to the riding direction, the observation window will display at least greater than 0.75V, less than 5V, and change with the magnitude of the applied force to normal, the value is greater than 0.75V, the system automatically Make sure the function is qualified, otherwise press "Fail". ',
    torque02: 'If you confirm that the vehicle is using a torque sensor, please select "S/T" and adjust to "T".',
    walk01: 'Hold the display',
    walk02: ' /"-" for three seconds or more, if the motor rotates and the speed is less than 6km/h, the system will automatically judge OK, otherwise please click "WALK communication failure" ',
    walk03: 'Please press',
    walk04: '/"-"  and observation window shows "WALK", otherwise please press "WALK communication failure" ',
    walk05: 'WALK communication failure',
    walk06: 'If the bicycle has gears, please change to a higher gear before activate the walk assistance function.',
    motor01: '1. In the cadence mode, if the motor running current is less than 3A, and BESST will mark the motor as OK',
    motor02: '2. In the "WALK" mode, the motor running current is less than 3A, and the automatic evaluation is normal',
    motor03: '3. If there is still no indication that the motor is working normal, please put the vehicle drive wheel in a damped and rotatable state, start the motor with the pedal, observe the normal value of current, speed and cadence or torque voltage information, the system will automatically judge "motor works normally", otherwise "Fail" ',

    defectDes: "Problem description",
    defects: "Mechanical defect",
    image: "Image",
    video: "Video",
    videoMaxSize: "Video maximum size is 10M",
    defectLength: "Maximum 600 character including space",

    notice19_00: "No more reminders in 7 days",
    notice19_01: "Explanation for changes after 2019.12.19",
    notice19_02: "Diagnosis function was updated on date of 2019.12.19. The diagnosis function might not suit well with some CAN products produced before October 2019.",
    notice19_03: "Additionally, because of the difference between service tool's hardware, there is some operation difference when use service tool with hardware version of 'BESST.UC 3.0'.",
    notice19_04: "BESST has been adjusted to remove the difference between hardware of service tool.",
    notice19_05: "Note: if you continually diagnosis a CAN system, you must shutdown power of controller over 20 seconds every time.",

};


const diagnosisNormal = {
    com: "COM",
    select: "Select",
    connect: "Connect",
    readAgain: "Read Again",
    refresh: "Refresh",
    tip: "Tip",
    saveReport: "Save Report",
    sendReport: "Send Report",
    selectError: "Select Defect",
    tipContent01: "Please check the following basic information and",
    tipContent02: "check the consistency of ID on  your bike.",
    title: {
        name: "Name",
        sn: "Series Number",
        diagnosisResult: "Diagnosis Result",
        mechanicalFailure: "Mechanical Failure",
        serviceBtn: "Service",
    },
    bike: "E-Bike",
    sn: "SN",
    confirm: "Confirm",
    cancel: "Cancel",
    solution: "Solution",
    hmiDefectOptions: {
        displayNotWork: "Display doesn't work",
        displayWaterInside: "Display water inside",
        displayBtnDamage: "Display button damaged",
        displayCableDamage: "Display cable damaged",
        displayScreenDamage: "Display screen damaged",
        displayBracketDamage: "Display bracket damaged",
        displayScrewLoosen: "Display screw loosened",
        displayBlackScreen: "Display black screen",
        displayMessyCode: "Display messy code",
    },
    controllerDefectOptions: {
        noAssistance: "No assistance",
        noLightFunc: "No Lighting function",
        brakeCannotCutOffPower: "Brake can't cut off the power",
        tripDistanceShort: "Trip distance too short",
    },
    batteryDefectOptions: {
        batteryDamage: "Battery damaged",
        chargerNotWork: "Charger doesn't work",
        BatteryCannotCharge: "Battery can't be charged",
    },
    motorDefectOptions: {
        motorNotWork: "Motor doesn't work,but the display work normally",
        motorShake: "Motor shakes",
        motorNoisy: "Motor noisy",
        motorLessPower: "Motor less power",
        motorRunIntermittently: "Motor runs intermittently",
        noFunction: "No 6KM/H function",
    },
    defectOptions: {
        motorNotWork: "Motor doesn't work",
        motorNoisy: "Motor's noisy",
        motorDamage: "Motor got damage",
        hmiFailed: "HMI failed",
        hmiErrorCode: "HMI got error code",
        hmiDamage: "HMI got damage",
        disorderedIndication: "Disordered indication",
        outOfControl: "Out of control",
        eBrakeFailed: "E-brake failed",
        eBrakeDamage: "E-brake got damage",
        lightFailed: "Light failed",
        walkAssistanceFailed: "Walk assistance failed",
        throttleFailed: "Throttle failed",
        throttleDamage: "Throttle got damage",
        shortRange: "Short range",
        cableDamage: "Cable got damage",
        batteryProblem: "Battery problems",
        batteryDamage: "Battery got damage"
    },
    noDefect: "No defect",
    read: "Read",
    test: "TEST",
    errorSolutionDialog: {
        title: "Error Solution",
        name: "Name",
        remark: "Remark",
        description: "Description",
        target: "Target",
        solution: "Solution",
    },
    connectTipVideoDialog: {
        title: "Connect tip video",
        tip: "Your browser does not support video tag."
    },
    connectTipDialog: {
        title: "Connect tip",
        tip: "Your browser does not support video tag.",
        content: "Please connect BESST to HMI and EB-BUS and switch on theHMI, and then check if the HMI displays \"PC\"(LCD type)or the Top mode LED light(LED type)"
    },
    confirmSnDialog: {
        title: "Confirm SN",
        content: "Please check with the sn on the vehicle to determine if it's right"
    },
    print: "Print",
    bikeInfo: "Bike Info",
    diagnosis: {
        result: "Diagnosis Result",
        hmi: " HMI",
        battery: "Battery",
        controller: "Controller",
        rpmSensor: "RPM Sensor",
        pedalSensor: "Pedal Sensor",
        motor: "Motor",
    },
    emptySNTip: "Vehicle SN Empty",
    emptySNText: "Vehicle SN is empty and the sn will appear in report，continue to go to test view？",
    back: "Back",
    storeInfo: "E-Bike Store Info",

    preTipA: 'Turn on HMI, BOX will "BEEP" twice,then Click',
    continue: "Continue",
    preTipB: 'If no "BEEP", then click',
    here: "More"

};

const diagnosisTest = {
    back: "Back",
    driveSystemTest: "Drive System Test",
    hmiMode: "Assistance Level",
    currentMode: "Current Level",
    light: "Light",
    title: {
        name: "Name",
        sn: "Series Number",
        diagnosisResult: "Diagnosis Result",
        mechanicalFailure: "Mechanical Failure",
    },
    help: "Help",
    save: "SAVE",
    confirm: "Confirm",
    cancel: "Cancel",
    hmiDefectOptions: {
        displayNotWork: "Display doesn't work",
        displayWaterInside: "Display water inside",
        displayBtnDamage: "Display button damaged",
        displayCableDamage: "Display cable damaged",
        displayScreenDamage: "Display screen damaged",
        displayBracketDamage: "Display bracket damaged",
        displayScrewLoosen: "Display screw loosened",
        displayBlackScreen: "Display black screen",
        displayMessyCode: "Display messy code",
    },
    controllerDefectOptions: {
        noAssistance: "No assistance",
        noLightFunc: "No Lighting function",
        brakeCannotCutOffPower: "Brake can't cut off the power",
        tripDistanceShort: "Trip distance too short",
    },
    batteryDefectOptions: {
        batteryDamage: "Battery damaged",
        chargerNotWork: "Charger doesn't work",
        BatteryCannotCharge: "Battery can't be charged",
    },
    motorDefectOptions: {
        motorNotWork: "Motor doesn't work,but the display work normally",
        motorShake: "Motor shakes",
        motorNoisy: "Motor noisy",
        motorLessPower: "Motor less power",
        motorRunIntermittently: "Motor runs intermittently",
        noFunction: "No 6KM/H function",
    },
    defectOptions: {
        motorNotWork: "Motor doesn't work",
        motorNoisy: "Motor's noisy",
        motorDamage: "Motor got damage",
        hmiFailed: "HMI failed",
        hmiErrorCode: "HMI got error code",
        hmiDamage: "HMI got damage",
        disorderedIndication: "Disordered indication",
        outOfControl: "Out of control",
        eBrakeFailed: "E-brake failed",
        eBrakeDamage: "E-brake got damage",
        lightFailed: "Light failed",
        walkAssistanceFailed: "Walk assistance failed",
        throttleFailed: "Throttle failed",
        throttleDamage: "Throttle got damage",
        shortRange: "Short range",
        cableDamage: "Cable got damage",
        batteryProblem: "Battery problems",
        batteryDamage: "Battery got damage"
    },
    noDefect: "No defect",
    selectError: "Select Defect",
    errorSolutionDialog: {
        title: "Error Solution",
        name: "Name",
        remark: "Remark",
        description: "Description",
        target: "Target",
        solution: "Solution",
    },
};


const reportList = {
    tableHead: {
        vehicleSn: "Vehicle SN",
        modelName: "Model Name",
        time: "Time",
        operation: "Operation"
    },
    noResult: "No Result！",
    total: "Total",
    search: {
        vehicleSn: 'Vehicle SN',
        searchBtn: "Search"
    },
    deleteDialog: {
        title: "Delete Report",
        content: "Are you sure to delete it?"
    },
    delete: "Delete",
    yes: "Yes",
    no: "No"
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
    back: "Back",
    add: 'Add',
    reset: "Reset",
    delete: "Delete",
    tableHead: {
        account: "Account",
        createTime: "Create Time",
        operation: "Operation",
    },
    noResult: "No Result！",
    total: "Total",
    confirm: "Confirm",
    cancel: "Cancel",
    yes: "Yes",
    no: "No",
    addAccountDialog: {
        title: "Add Account",
        email: "E-mail",
        password: "Password"
    },
    deleteDialog: {
        title: "Delete Account",
        content: "Are you sure to delete it?"
    },
    resetPasswordDialog: {
        title: "Reset Password",
        content: "Do you want to set the password for this account to 123456?"
    }
};

const centerTicketList = {
    serviceCenter: "Service Center",
    category: "Category",
    status: "Status",
    look: "Look",
    noResult: "No Result！",
    total: "Total",
    tableHead: {
        serviceNo: "Service No.",
        title: "Title",
        targetSn: "Vehicle SN",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation"
    },
    categoryList: {
        all: "All",
        vehicle: "Vehicle",
        motor: "Motor",
        hmi: "HMI",
        sensor: "Sensor",
        battery: "Battery",
        light: "Light",
        controller: "Controller",
        other: "Other",
    },
    statusList: {
        all: "All",
        pendingAssign: "Pending Assign",
        processing: "Processing",
        complete: "Complete"
    }
};

const myTicketList = {
    category: "Category",
    status: "Status",
    look: "Look",
    tableHead: {
        serviceNo: "Service No.",
        category: "Category",
        title: "Title",
        targetSn: "Vehicle SN",
        createTime: "Create Time",
        status: "Status",
        operation: "Operation",
    },
    categoryList: {
        all: "All",
        vehicle: "Vehicle",
        motor: "Motor",
        hmi: "HMI",
        sensor: "Sensor",
        battery: "Battery",
        light: "Light",
        controller: "Controller",
        other: "Other",
    },
    statusList: {
        all: "All",
        processing: "Processing",
        complete: "Complete",
        handleToAnother: "Handle To Another",
    },
    noResult: "No Result！",
    total: "Total",
    addTicketDialog: {
        dialogTitle: "Add Service Ticket",
        category: "Category",
        title: "Title",
        description: "Description",
        bike: "E-Bike",
        sn: "SN",
        serviceCenter: "Service Center",
        image: "Image",
        video: "Video",
        clickToUpload: "Click to upload"
    },
    confirm: "Confirm"

};

const serviceCenterList = {
    serviceCenter: "Service Centers",
    add: "Add",
    edit: "Edit",
    account: "Account",
    noResult: "No Result！",
    total: "Total",
    confirm: "Confirm",
    cancel: "Cancel",
    yes: "Yes",
    no: "No",
    tableHead: {
        logo: "Logo",
        name: "Name",
        phone: "Phone",
        email: "E-mail",
        country: "Country",
        address: "Address",
        operation: "Operation",
    },
    deleteDialog: {
        title: "Delete Service Center",
        content: "Are you sure to delete it?"
    },
    createSC: "Create Service Center",
    editSC: "Edit Service Center",
    editDialog: {
        picture: "Picture",
        tip: "Tip",
        content01: "You'd better upload square picture with background",
        content02: "support jpg,png and 300*300/400*400, up to 3MB",
        name: "Name",
        email: "E-mail",
        phone: "Phone",
        website: "Website",
        address: "Address",
        postcode: "Postcode",
    },
    detailDialog: {
        title: "Detail",
        picture: "Picture",
        name: "Name",
        email: "E-mail",
        phone: "Phone",
        website: "Website",
        address: "Address",
        postcode: "Postcode",
        ticketCount: "Ticket Count",
        aveHandleTime: "Ave Handle Time",
    },
    op: Operation,
};

const ticketDetail = {
    back: "Back",
    handleUp: 'Handle Up',
    history: "History",
    ticketInfo: {
        infoTitle: "Complaint details",
        ticketNo: "Ticket No.",
        createTime: "Create Time",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Title",
        desc: "Description",
        image: 'Image',
        video: "Video"
    },
    user: {
        address: "Address",
        website: "Website",
        email: "E-mail",
    },
    replay: {
        content: "Content",
        image: "Image",
        sendMessage: "Send Message"
    },
    confirm: "Confirm",
    cancel: "Cancel",
    handleUpTicketDialog: {
        title: "Handle Up Ticket",
        serviceCenter: "Service Center"
    },
    historyDialog: {
        title: "Ticket History",
        name: "Name",
        operation: "Operation",
        time: "Time"
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
    order: "Order",
    vehicle: "Bike"
};

const AssemblerOrderDetail = {
    back: "Back",
    orderDetail: "Order Detail",
    edit: "Edit",
    delete: "Delete",
    orderInfo: "Order Info",
    orderNo: "Order No.",
    customerName: "Customer Name",
    note: "Note",
    poNo: "Po No.",
    itemsList: "Items List",
    produce: "Produce",
    vehicleList: "Bike List",
    deleteOrder: 'Delete Order',
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    tableHead: {
        modelName: "Model Name",
        partNo: "Article No.",
        quantity: "Quantity",
        operation: "Operation",
    },
    replace: "Replace",
    confirmDialog: {
        replace: "replace",
        yes: 'Yes',
        no: 'No'
    },
    itemHead: {
        image: "Image",
        name: "Name",
        partNo: "Article No."
    },
    search: "Search",
    brandPlaceholder: "Select brand",
    operation: "Operation",
    modelName: "Model Name",
};

const AssemblerOrderList = {
    add: "Add",
    noResult: "No Result！",
    vehicleList: "Bike List",
    detail: "Detail",
    total: "Total",
    tableHead: {
        date: "Date",
        orderNo: "Order No.",
        poNo: "Po No.",
        customerName: "Customer Name",
        model: "Model",
        produced_qty: "Produced/Qty",
        status: "Status",
        operation: "Operation"
    },
    poNo: "Po No.",
    search: "Search",
    startDate: "Select start date",
    endDate: "Select end date",
    model: 'Model',
};

const AssemblerOrderProduce = {
    back: "Back",
    orderInfo: "Order Info",
    brand: "Brand",
    model: "Model",
    poNo: "Po No.",
    produced_qty: "Produced/Qty",
    pleaseSelect: "Please select",
    reconnect: "Reconnect",
    openConnectTip: "Open Connect Tip",
    partName: "Name",
    sn: "SN",
    hw: "HW",
    sw: "SW",
    connectTipVideo: "Connect tip video",
    connectTip: "Connect Tip",
    connectTipText: " Please connect BESST to HMI and EB-BUS and switch on the HMI, and then check if the HMI displays 'PC'(LCD type) or the Top mode LED light(LED type)",
    confirm: "Confirm",
    itemFinishTip: "The item is finished",
    selectTip: "Please select com",
    tip: 'Tip',
    batteryBindTip: "Do you want to bind the battery?",
    yes: "Yes",
    no: "No",
    goDiagnosisTip: "Do you want to go to the diagnosis view?",
    bindBattery: "Bind Battery",
    vehicleSn: "Bike SN",
    componentSn: "Component SN",
    videoTip: "Your browser does not support video tag.",
    error: "Error",
    historyDialog: {
        title: "Software History",
        version: "Version",
        updateTime: "Update Time",
        remark: "Remark",
        operation: "Operation",
        noResult: "No Result！"
    },
    update: "Update",
    reset: "Reset",

    isUsed: "has been used.",
    canNotUsed: "can't be used.",
    inputBikeSn: "Please input bike sn.",
    inputMotorSn: "Please input motor sn.",
    inputHmiSn: "Please input HMI sn.",
    inputCtlSn: "Please input controller sn.",
    inputBatSn: "Please input battery sn.",
    inputSensorSn: "Please input sensor sn.",

    bikeSnExt: "The bike sn exists.",
    saveSnText: 'The following component SN(serial number) is not exist in BESST. Please check the SN. If everything is all right, you can still bind the SN to bike by clicking "Save". If this situation contiune occur, please contact bike brand or component supplier.',

    cNam: AttributeName,
};

const AssemblerOrderSave = {
    back: "Back",
    createOrder: "Create Order",
    editOrder: "Edit Order",
    orderInfo: "Order Info",
    poNo: "Po No.",
    customerInfo: "Customer Info",
    customerName: "Customer Name",
    note: "Note",
    itemList: "Items List",
    addItem: "Add Item",
    delete: "Delete",
    modelName: "Model Name",
    addItems: "Add Items",
    saveOrder: "Save Order",
    tableHead: {
        name: "Name",
        partNo: "Article No.",
        quantity: "Quantity",
        operation: "Operation"
    },
    itemHead: {
        image: "Image",
        name: "Name",
        partNo: "Article No."
    },
    search: "Search",
    brandPlaceholder: "Select brand"
};

const AssemblerPartnerList = {
    back: "Back",
    acceptInvitation: "Accept Invitation",
    currentBrand: "Current Brand",
    noResult: "No Result！",
    total: "Total",
    brandInfo: "Brand Info",
    logo: "Logo",
    name: "Name",
    type: "Type",
    brand: "Brand",
    email: "E-mail",
    website: "Website",
    tel: "Tel",
    postcode: "Postcode",
    address: "Address",
    invitationCode: "Invitation Code",
    confirm: "Confirm",
    cancel: "Cancel",
    hoverTip: "Set as current brand",
    tableHead: {
        name: "name",
        phone: "Phone",
        website: "Website",
        country: "Country",
        operation: "Operation"
    },
    codeEmptyTip: "Please input code",
    acceptSuccess: "Accept successfully"
};

const AssemblerVehicleDetail = {
    vd: VehicleDetail
};

const AssemblerVehicleList = {
    brand: "Brand",
    poNo: "Po No.",
    modelName: "Model Name",
    bomSn: 'Component SN',
    search: "Search",
    export: "Export",
    delete: "Delete",
    detail: "Detail",
    total: "Total",
    deleteVehicle: "Delete Vehicle",
    deleteTip: "Are you sure to delete it ?",
    yes: "Yes",
    no: "No",
    back: "Back",
    tableHead: {
        vehicleSn: "Vehicle SN",
        brandLogo: "Brand Logo",
        brandName: "Brand Name",
        modelName: "Model Name",
        poNo: "Po No.",
        produceDate: "Produce Date",
        saleDate: "Sale Date",
        operation: "Operation",
        noSale: "No Sales"
    },
    noResult: "No Result !",
    deleteSuccess: "Delete successfully",
    noSale: "No Sales",
    exportDialog: {
        title: "Export Vehicle",
        startNum: "Start Num",
        endNum: "End Num",
        export: "Export",
        cancel: "Cancel"
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
// ./src/assets/lang/en.js