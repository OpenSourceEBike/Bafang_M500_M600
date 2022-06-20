// 登录页面相关
const Login = {
    declare: "HINWEIS",
    readmore: "mehr...",
    declareP1: "BESST ist ein Verwaltungsprogramm der Antriebssysteme, das für die Elektrofahrrad-Marken entwickelt wird. Die Marken können eine Einladungscode an ihre Händler, Fabriken und Distributoren senden, damit sie eigenes Konto registrieren. Jede Rolle muss garantieren, dass die registrierten Konten und Passwörter nicht an Dritte weitergegeben werden und die Marken für die Schulung der eingeladenen Beziehungen verantwortlich sind.",
    declareP2: "Alle Benutzer müssen das BESST-Tool von den Marken oder Softwareeigner einkaufen, um den Funktionen der Software zur Verfügung zu haben.",
    declareP3: "Die Markenunternehmen",
    declareP6: "1. Die Markenunternehmens werden von Softwareeigner autorisiert:",
    declareP7: "Die Markenunternehmens sollen am Softwareeigner einen schriftlichen Antrag stellen, z.B.: den Eintragungsname des Unternehmens, den Staat, die Adresse, PLZ, rechtliche E-mail-Adresse des Managements, das Markeneigentumszertifikat, die Webseite, usw.",
    declareP8: "Softwareeigner bestätigen die Authentizität und Rechtmäßigkeit der vorgelegten informationen;",
    declareP9: "Softwareeigner erstellen Konto und senden Autorisierung per E-Mail;",
    declareP10: "Nach dem Herunterladen und der Installierung der Software im Autorisierung müssen die Markenunternehmen gemäß dem Registrierungsprozess die relevanten Informationen genau ausfüllen;",
    declareP11: 'Im "Einstellungen" modifiziert man die informationen des Unternehmens und ändert das password',
    declareP12: 'Im "Marke" erstellt man eigene Marken-List des Unternehmens',
    declareP13: 'Im "Modell" fügt man das Modell hinzu',
    declareP14: `Im "Händler" sendet man den Einladungscode an die Händler durch E-mail`,
    declareP15: 'Im "Erstausrüster" sendet man den Einladungscode an den Erstausrüster durch E-mail',
    declareP16: `Im "Konto" erstellen Sie selbst die Nebenkonten für Mitarbeiter, wenn Sie sich mit dem Hauptkonto einloggen`,
    declareP17: "DOWNLOADEN:",
    declareP18: "Händler und Distributoren",
    declareP19: "3. Die Händler und Distributoren werden von Markenunternehmen autorisiert:",
    declareP20: 'Händler und Distributoren sollen am Markenunternehmen einen “Einladungscode” fragen;',
    declareP21: 'Für neuer Benutzer kann man in der Seite "Neu anmelden" den "Einladungscode" eingeben, um die E-mail-Adresse als Kontonamen zu überprüfen;',
    declareP22: "Stellen Sie das Passwort ein, nachdem Sie durch den E-Mail einen Bestätigungscode erhalten haben;",
    declareP23: 'Für Benutzer des alten BESST Systems füllen Sie den "Einladungscode" in "Einladung annehmen" im Tab "Marke" aus und akzeptieren die neue Marke.',
    declareP24: "Nach der Einstellung des Passwortes füllen die Händler und Distributoren gemäß dem Registrierungsprozess die relevanten Informationen genau aus, und bestätigen die Authentizität und Rechtmäßigkeit der vorgelegten informationen, um die Registrierung zu beenden.",
    declareP25: 'Next time you log in, fill in your email number and password in the account bar. It is better to mark "Remember me" so that you can log in easily next time;',
    declareP26: 'Im "Brand list": nach Anmeldung und Einloggen werden die Daten von Markenunternehmen automatisch geladen.',
    declareP27: 'Im "Modell" wird die Informationen zu allen Marken automatisch versammeln',
    declareP28: 'Im "Verkauf" kann man die Kundeninformationen eintragen, das Fahrrad-IDs, Verkaufslist erstellen und die After-Sales-Richtlinie bestimmen, und erinnern Sie sich, die Daten auf dem Serverzu speichern.',
    declareP29: "Die Fabriken",
    declareP30: "2.Die Fabriken werden von Markenunternehmen autorisiert:",
    declareP31: 'Die Fabriken sollen am Markenunternehmen einen “Einladungscode” fragen;',
    declareP32: 'Für neuer Benutzer kann man in der Seite "Neu anmelden" den "Einladungscode" eingeben, um die E-mail-Adresse als Kontonamen zu überprüfen.',
    declareP33: 'Im "Brand list": nach Anmeldung und Einloggen werden die Daten von Markenunternehmen automatisch geladen.',
    declareP34: 'Im "Auftrag" und "Fahrzeug" wird die Informationen der Markenunternehmen automatisch versammeln, beim Erstellung der neuen Aufträge muss man zuerst die Marke und dann die Modelle auswählen.',

    chooseLanguage: "Sprache",
    chinese: "中文",
    deutsch: "Deutsch",
    english: "English",
    dutch: "Nederlands",
    login: "Einloggen",
    userNamePlaceholder: "Benutzername",
    passwordPlaceholder: "Passwort",
    rememberMe: "Kennwort merken",
    forgetPassword: "Passwort vergessen?",
    loginBtnTxt: "Einloggen",
    signUp: "Neu anmelden",
    offLine: "Off-Line Betrieb",
    inputUser: "Please input username",
    inputPassword: "Please input password",
    accountProblem: "The account has some problems. Please contact the administrator besst@bafang-e.com",
    limitedIP: 'IP verification is enabled for this account. IP verification failed, please login in regular location.',
    emptyIP: 'IP verification is enabled for this account. Please wait or try later.',
    emptyIPThree: 'IP verification is enabled, please contact the administrator.',
};

// 注册页面相关
const RegisterAccount = {
    login: "Einloggen",
    verifyCode: "Code überprüfen",
    createAccount: "Konto erstellen",
    createCompany: "Firma erstellen",
    company: "Firma",
    email: "E-mail",
    emailValidationCode: "E-mail Validierungscode",
    password: "Passwort",
    confirmPassword: "Passwort bestätigen",
    next: "Nächste",
    verify: "Überprüfen",
};

const RegisterCompany = {
    login: "Einloggen",
    verifyCode: "Code überprüfen",
    createAccount: "Konto erstellen",
    createCompany: "Firma erstellen",
    yourRole: "Deine Rolle",
    logo: "Logo",
    uploadImgTip: "Bitte laden sie ein quadratisches Bild mit Hintergrund hoch. Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB.",
    contact: "Kontakt",
    companyName: "Firmenname",
    mailAddress: "Postanschrift",
    phoneNumber: "Telefonnummer",
    website: "Webseite",
    postalAddress: "Postadresse",
    post: "Postleitzahl",
    address: "Adresse",
    save: "Speichern",
    country: "Land",
};

const RegisterInvitation = {
    login: "Einloggen",
    verifyCode: "Code überprüfen",
    createAccount: "Konto erstellen",
    createCompany: "Firma erstellen",
    invitationCode: "Erhaltener code",
    typeTheCodeTip: "Geben Sie den Code ein, den Sie erhalten haben.",
    tryAgainTip: "Bei nicht erfolgreicher Regestrierung wiederholen sie bitte den Vorgang.",
    verify: "Überprüfen",
    noCompanyTip: "Das Konto ohne Firmeninformation.",
    reCodeTip: "Bitte geben Sie den erhaltener Code ein, mit dem dieses Konto erstellt wurde, um eine Firma zu erstellen.",

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
    login: "Einloggen",
    email: "E-mail",
    verify: "Überprüfen",
    emailValidationCode: "E-mail Validierungscode",
    password: "Passwort",
    confirmPassword: "Passwort bestätigen",
    resetPassword: "Passwort zurücksetzen",
    emptyEmailTip: "Bitte E-Mail eingeben.",
    errorEmailTip: "Die E-mail ist unberechtigt.",
    emptyCodeTip: "Bitte Code eingeben",
    emptyPasswordTip: "Bitte Passwort eingeben",
    emptyConfirmPasswordTip: "Bitte Passwort bestätigen",
    passwordTip: "Passwörter stimmen nicht überein.",
    resetPasswordSuccess: "Passwort erfolgreich zurücksetzen",
    codeHasBeenSent: "Code wurde gesendet",
    passwordFormatTip: "Passwort kann nur Buchstaben und Zahlen enthalten, 8 bis 16 Zeichen in der Länge, Groß-/Kleinschreibung beachten.",
    filterUserEmail: "Die Email-Adresse des Benutzers ist nicht in BESST registriert. Bitte überprüfen Sie die E-Mail, ob sie korrekt ist."
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
    brand: "Marke",
    model: "Modell",
    vehicle: "Fahrzeug",
    dealer: "Händler",
    dealerList: "Händlerliste",
    invitation: "Einladung",
    oem: "Erstausrüster",
    oemList: "Erstausrüsterliste",
    componentList: "Komponentenhersteller",
    serviceCenter: "Servicezentrum",
    serviceCenters: "Servicezentren",
    complaints: "Reklamationen",
    tasks: "Aufgaben",
    account: "Konto",
    order: "Auftrag",
    diagnosis: "Diagnose",
    report: "Bericht",
    errorCode: "Fehlercode",
    tool: "Werkzeug",
    hmi: "Display",
    controller: "Kontroller",
    battery: "Akku",
    sensor: "Sensor",
    update: "Update",
    auth: "Befugnis",
    product: "Produkt",
    component: "Spur",
    centerTicket: "Zentrumticket",
    myTicket: "Mein Ticket",
    help: "Hilfe",
    setting: "Einstellungen",
    logout: "Ausloggen",
    statistics: "Statistiken",

    record: "Aufzeichnung",
    testFeedback: "Feedback Center",
    material: "Production Kanban",
    admin: "Verwaltung",
    bikeModel: "Modell",
    bikeSales: "Verkauf",
    bikeInfo: "Information",
    author: "Autorisierung",
    mechanic: "Mechanische Defekte",
    faq: "FAQ",
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
    look: 'look',
    edit: 'edit',
    delete: 'delete',
    view: 'view',
    add: 'add',
    save: 'save',
    cancel: 'cancel',
    confirm: 'confirm',
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
    controller: "Kontroller",
    hmi: "Display",
    battery: "Akku",
    motor: "Motor",
    sensor: "Sensor",
    key: "Schlüssel",
    sn: "Seriennummer",
    charger: "Charger",
    other: "Other",
    bike: "Bike",

    model: "Modell",
    softwareVersion: "Software Ver.",
    hardwareVersion: "Hardware Ver.",
    tryAccountsTip: "Dieses Konto ist für die Testversion vorgesehen. Einige Vorgänge sind eingeschränkt.",

    bootLoad: "Bootloader Ver.",
    errorCode: "Error Code",
    totalMileage: "Gesamte Fahrdistanz",
    clientCode: "Kunden Nr.",
    wheelDiameter: "Raddurchmesser",
    speedLimit: "Geschwindigkeitsbegrenzung",
    totalGear: "Gesamt Schaltung",
    serviceMileage: "Warnung vor Beibehaltungsbeseitigung",
    setTotalMileage: "Set Total Mileage",
    setSingleMileage: "Set Single Mileage",
    setTimeZone: "Zeitzone einstellen",
    time: "Time",
    keyCard: "Schlüsselkarte",
    circumference: "Umfang",
    singleMileage: "Single Mileage",
    speedMax: "Höchste Geschwindigkeit",
    speedAvg: "Durchschnittliche Geschwindigkeit",
    hmiMode: "Modus",
    hmiLevel: " Aktuelle Unterstützungsstufe",
    hmiBoost: "BOOST Modus",
    hmiBoostIn: "in BOOST",
    hmiBoostOut: "nicht in BOOST",
    hmiTurnOffTime: "Abschaltzeit",
    hmiNoTurnOff: "nicht",
    hmiLight: "Fahrradlicht",
    hmiLightOn: "an",
    hmiLightOff: "aus",
    hmiButton: "Taste “+”",
    hmiBtnPress: "gedrückt",
    hmiBtnPressNot: "nicht gedrückt",
    hmiSingleMileageClear: "Tageskilometer rückstellen",
    hmiClean: "ja",
    hmiNotClean: "nein",

    spdMeterType: "Geschwindigkeitsanzeige typ",
    manName: "Hersteller",
    nmlVolt: "Nennspannung",
    maxCur: "Max Strom",
    spdMeterSignalNum: "Geschwindigkeitsmesser-Signal",
    limitVolt: "Schwacher Batterieschutz(M)",
    limitCurrent: "Grenzwert Strom",
    limitSpd: "Maximal Geschwindigkeit",
    assit: "Unterstützungsstufe",
    positionSensor: "Positionssensor kalibrieren",
    torqueSenCali: "Torque sensor kalibrieren",
    conTemperature: "Kontrollertemperatur",
    motorTemperature: "Motortemperatur",
    calorie: "Energieverbrauch",
    assistStatus: "Schiebehilfe",
    assistOn: "an",
    assistOff: "aus",
    conTorque: "Drehmomentspannung",
    conCadence: "Trittfrequenz",
    conSingleTrip: "Tageskilometer",
    conSingleTripLeft: "Restreichweite",
    conSpeed: "Geschwindigkeit",
    conCurrent: "Strom",
    conVoltage: "Spannung",

    ASOC: "ASOC",
    packVoltage: "Batteriespannung",
    fullChargeCapacity: "Volle Ladekapazität",
    current: "Strom",
    cell8_14: "Zelle 8-14",
    currentChargeInterval: "Aktuelles Ladeintervall",
    RSOC: "RSOC",
    capacityLeft: "Restkapazität",
    temperature: "Temperatur",
    cell1_7: "Zelle 1-7",
    cycleLife: "Anzahl der Zyklen",
    maxChargeInterval: "Max Ladeintervall",
    produceDate: ' Produce Date',
    SOH: "SOH",
    useStage: "Maintenance Stage",
    result: "Result",
    afterSalesTitle: "Maintenance Status Check",

    power: "Leistung",
    voltage: "Spannung",
    wheelSize: "Radgröße",
    rpm: "Umdrehung pro minute",

    torque: "Drehmomentspannung",
    cadence: "Trittfrequenz",
    sensorTip: "Die Hauptdaten sind leer. Überprüfen Sie die Verbindung des Sensors.",

    component: "Komponente",
    com: "COM",
    write: "Schreiben",
    connect: "Verbinden",
    read: "Lesen",
    reset: "Zurücksetzen",
    set: "Set",
    update: "Update",
    test: "Test",
    clear: "löschen",
    start: "Start",
    stop: "Stoppen",
    cancel: "Stornieren",
    confirm: "Confirm",
    select: "Auswählen",
    restart: "Restart",
    save: "Speichern",
    saveSuccess: "Erfolgreich gespeichert",
    writeSuccess: "Erfolgreich beschrieben",
    selectComTip: "Bitte wählen Sie COM",
    comDisconnect: "Connection is disconnected.",
    noComConnect: "Bitte verbinden Sie COM",
    writeFail: "Schreibfehler. Please try again.",
    snEmptyTip: "Bitte eingeben Sie die Seriennummer ein",
    note: "Notiz",
    notice: "Hinweis",
    message: "Nachricht",
    success: "erfolgreich",
    operationFail: "Die Operation ist erfolglos.",
    speedLimitNote: "Achtung: Die Geschwindigkeitsbegrenzung liegt über 25Km/h. Bitte passen Sie die Geschwindigkeitsbegrenzung unter der vom lokalen Recht vorgeschriebenen Grenzen an.",
    hmiUART: "Das aktuelle Antriebssystem wird als UART-Kommunikation beurteilt.",
    hmiUARTText1: "Falls das Display im PC-Modus oder der Arbeitsschnittstelle ist, müssen Sie das Display zunächst ausschalten.",
    hmiUARTText2: "Drücken Sie die “Reset” Taste auf dem BESST Tool.",
    hmiUARTText3: "Drücken Sie die “An/Aus” Taste auf dem Display, damit es einschaltet und in den PC-Modus wechselt.",
    hmiUARTText4: "Drücken Sie die “Lesen” Taste, um die Parameter zu lesen. Wenn die gelesene Informationen unvollständig sind, lesen Sie erneut.",

    mainData: "Hauptdaten",
    parameter: "Parameter",
    setting: "Einstellungen",
    operationTip: "Tipps",
    txtTitle1: "Vorgänge",
    txtTitleHid: "Der Kommunikationsmodus, den BESST zum Computer verwendet.",
    txtTitleCom: "Der Kommunikationsmodus, den BESST zum Computer verwendet.",
    txtTitleCon: "Wenn die Verbindung zwischen BESST-Tool und PC erfordert. USB HID wird automatisch verbinden.",
    txtTitleRead: "Dadurch werden die Daten der Komponenten abgelesen.",
    txtTitleWrt: "Dadurch werden die Schlüsselparameter der Komponenten verändert.",
    txtTitleReset: "Der Verbindungsstatus zurücksetzen, diese Funktion ist die gleiche wie Reset-Taste auf BESST-Tool. Auch alle vorhandenen Daten würden gelöscht.",
    txtTitleTest: "Achtung: Dieses Tool kann prüfen, ob die Komponente das BESST unterstützt. Durch das Tool kann die Hauptdaten der Komponente anzeigen. Muss man zuerst die Daten ablesen, bevor verändern. Wir tragen jede Vorgänge zusammen mit SN ein.",
    txtTitleUpt: "Dadurch werden die Firmware auf die Komponente aktualisiert.",
    txtTitleClear: "Dadurch werden die Daten geräumt.",
    txtTitle2: "Vorgangstipps",
    txtNote1: "Dieses Tool kann prüfen, ob die Komponente das BESST unterstützt. Durch das Tool kann die Hauptdaten der Komponente anzeigen.",
    txtNote2: "Man muss zuerst die Daten ablesen. Wir tragen jede Vorgänge zusammen mit SN ein.",
    txtNote3: "Man muss zuerst die Daten ablesen, bevor verändern. Wir tragen jede Vorgänge zusammen mit SN ein.",
    updateGetFileSucc: "Die Files wurde erfolgreich heruntergeladen.",
    updateStarting: "Starten... Wenn in 10 Sekunden kein Fortschrittsbalken vorhanden ist, bitte stellen Sie zurück und aktualisieren erneut.",
    resetTip: "Das BESST-Tool wurde zurückgestellt. Dann Display auszuschalten und wieder einzuschalten.",

    offlineNo: "Händler und Montagewerke können keine größeren Werte festlegen.",
    needReadSn: "SN ist leer. Bitte lesen Sie zuerst ab, um die SN zu erhalten. ",
    offlineCon: "Bitte Netzwerk verbinden",
    offlineHandle: "Daten processing. Bitte versuchen Sie es nach einiger Zeit erneut.",

    updateSuccessTip: "Sie haben die Firmware erfolgreich in die BOX geladen, dann aktualisieren Sie die Geräte nach Anweisung",
    updateFailTip: "Das Werkzeug könnte die Firmware nicht erkennen, und es ist ein Übertragungsfehler aufgetreten. Bitte laden Sie die Firmware erneut herunter.",
    updateToolReconnectTip: 'Ziehen Sie das Werkzeug zu PC ab und schließen Sie es mit PC erneut an. Wenn der USB-HID-Anzeiger grün wird, klicken Sie erneut auf "Aktualisieren" Button. ',
    updateMsg: "Firmware aktualisieren...",
    updateMsg0: "Bitte verbinden Sie zuerst",
    updateMsg1: "Falls das Display angeschlossen ist, bittte ziehen Sie das Display ab und dann schließen Sie es wieder an. ",
    updateMsg2: "Falls der Kontroller angeschlossen ist, bittte ziehen Sie der Kontroller ab und dann schließen Sie den wieder an.",
    updateMsg3: "Bitte halten Sie die Verbindung zwischen Tools und Computer. ",
    updateSelectFile: "Bitte wählen Sie die Files.",
    updateOnedriveNote: "Die Dateien sind im OneDrive Ordner, es wird jetzt nicht mehr unterstützt. Bitte ziehen sie aus.",
    updateReadFail: "Fehler beim Lesen von Daten oder Dateien, versuchen Sie es erneut.",
    updateDataError: "Der Dateianfangskennsatz der Firmware ist falsch.",
    updateCrcError: "Diese Datei könnte einen Prüfsummenfehler haben. Wenn die Aktualisierung fehlerhaft ist, überprüfen Sie die Datei.",
    updateMsg4: "BESST aktualisiert das Display.",
    updateMsg5: "BESST aktualisiert den Kontroller.",
    updateMsg6: "BESST aktualisiert die Batterie.",
    updateMsg7: "BESST aktualisiert den Sensor.",
    updateMsg8: "Aktualisierung der BESST-Box",
    updateMsg10: "BESST box aktualisiert",
    updateMsg11: "Die Aktualisierung könnte einen Fehler haben. Bitte aktualisieren Sie besser erneut.",
    updateMsg20: "Die Aktualisierung is fertig.",
    updateMsg30: "Bitte klicken Sie auf die Taste “Lesen” , um die SN und Software-Version zu erhalten.",
    updateMsgLO: "Bitte wählen Sie die Datei (.bin) aus der Lokalen oder Online.",
    updateMsgL: "Bitte wählen Sie die Datei (.bin) aus der Lokalen.",
    updateMsgTip: "Neue Softwareversion wird erkannt. Sie können die neueste Version auswählen und aktualisieren. Wenn Sie diese Vorgang nicht kennen, folgen Sie bitte die Schnitt “ Werkzeug--Update",

    hasHmiSn: "Bitte überprüfen Sie SN des Displays.",
    hasCtlSn: "Bitte überprüfen Sie SN des Kontrollers.",
    hasBatSn: "Bitter überprüfen Sie SN der Batterie.",
    hasMotorSn: "Bitte überprüfen Sie SN des Motors.",
    hasSensorSn: "Bitte überprüfen Sie SN des Sensors.",
    updateStep0: ' Anleitung für Aktualisierung',
    updateStep1: 'Klicken Sie auf die Taste "Verbinden", um die Verbindung zwischen Computer und BESST-Tool herzustellen.',
    updateStep2: 'Wählen Sie den Komponenten aus, den Sie aktualisieren möchten. ',
    updateStep3: 'Klicken Sie auf die Taste "Lesen", um SN und Softwareversion zu ablesen. Falls die SN leer ist, ist die Aktualisierung nicht zulässig.',
    updateStep4: 'Klicken Sie auf die Taste "Datei aus der Cloud herunterladen" oder "Datei aus lokalen herunterladen".',
    updateStep5: 'Trennen Sie die Komponente ab, und klicken Sie auf "Update", um Software in das BESST-Tool herunterzuladen. Während dieses Prozesses sehen Sie die "PC" LED auf dem Tool blinken. Falls die "PC" LED immer leuchtet, müssen Sie die Taste “reset” auf dem BESST-Tool drücken.',
    updateStep6: 'Nachdem die Software ins BESST-Tool erfolgreich heruntergeladen ist, können Sie die Komponente wieder anschließen, und das BESST-Tool würde die Software darauf schreiben. Für weitere Details klicken Sie auf das Symbol "?" rechts oben.',

    snMaxLength: "Die SN dürfte maximal bis zu 40 Zeichen haben",
    customerMaxLength: "Die Kunden Nr. dürfte maximal bis zu 24 Zeichen haben.",
    speedLimitEmpty: "Die Geschwindigkeitsbegrenzung ist leer. Bitte lesen Sie es ab, um die Geschwindigkeitsbegrenzung der Komponente zu erhalten.",
    whileSizeRange: "Die Umfang liegt außer der Reichweite gemäß dem Raddurchmesser. ",

    keyCardStart: "Die Schlüsselkarte fängt an zu erkennen.",
    keyCardStop: "Die Schlüsselkarte hört auf zu erkennen.",

    noCanConnect: "BESST box is not connected or not support can",
    writeModelTip: "Model data is empty or does not meet specifications",
    writeHardwareTip: "Hardware data is empty or does not meet specifications",

    pcMode: "PC-Modus",
    pcModeTxt: 'PC" Modus wird verwendet, um Daten aus dem Display im Vergleich zum Arbeits-Modus abzulesen. Wenn Display mit BESST-Tool verbunden und eingeschaltet würden, würden Display in den "PC"-Modus wechseln.',
    pcModeTxtLCD: ' Beim LCD Display: Auf dem Bildschirm zeigt "PC" oder "Connected OK". ',
    pcModeTxtLED: ' Beim LED Display: Das Licht des höchsten Level ist an.',
    paramsName: "Paramater Version",
    paramsCloud: "Aus der Cloud herunterladen",
    paramsCombine: "Die Daten vereinen",

    batUnderMt: "Auf der Garantie",
    batOutMt: "Aus der Garantie",
    batUseTime: "Die Batterie ist seit mehr als 30 Monaten im Einsatz. Sie ist aus der Garantie. ",
    strNaming: 'Diese zeichen "\ / ^ : * ? " < > |" sind nicht erlaubt. ',
    getDataTip: "Die Daten werden vom Server runterladen. Warten Sie mal kurz.",

    remark: "Remark",
    defect: "Defect",
    image: "Image",
    description: "Description",
    video: "Video",
    updateSteps: 'Update Verfahren',
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

    checkSnHmi: "Die SN Nummer von HMI sollte mit 'DP' Anfang sein.",
    checkSnCtrl: "Die SN Nummer von Kontroller sollte mit 'CR' Anfang sein.",
    checkSnSensor: "Die SN Nummer von Sensor sollte mit 'SR' Anfang sein.",
    checkSnTool: "Die SN Nummer von BESST sollte mit 'BE' Anfang sein.",

    inputValueTip: "Please input value",
    readDateTip: "Please read data",
};

const Hmi = {
    hmi: "Display",
    auth: "Befugnis",
    applyHmiAuth: "Display-Berechtigung beantragen",
    applyAllAuth: "Alle Berechtigung beantragen",
    noHmiAuthTip: "Sie haben keine Berechtigung, um dewn Kontroler zu pogrammieren. Bitte fragen Sie den Hersteller um Erlaubnis.",
    applySuccess: "Erfolgreich beantragt. Bitte warten Sie auf die Autorisierung.",
    brandEmptyTip: "Sie haben keine Partnermarke.",
    getApplySuccess: "Erfolgreich Zuständigkeit enthalten",
    hmiTip: 'Die Hauptdaten sind leer. Für UART-System bitte schalten Sie das Display ein, um in den "PC"-Modus zu wechseln, oder für CAN-System bitte überprüfen Sie, ob Display eingeschaltet is.',
    hmiTipCan: "Die Hauptdaten sind leer. Wenn Display angeschlossen und eingeschaltet ist, müssten die Software möglicherweise aktualisiert werden.",
    mileageTotalSupport: "Nur die Ganzzahl ist gültig, z. B. 1000 km, und nur der ganzzahlige Bruchteil wird akzeptiert",
    mileageTotalMax: "Die Maximale ist 99999. Bitte überprüfen Sie den eingegebenen Wert.",
    mileageSingleSupport: "Nur die Ganzzahl ist gültig, z. B. 1000 km. Nur der ganzzahlige Bruchteil und eine Dezimalbruch wird akzeptiert.",
    mileageSingleMax: "Die Maximale ist 9999.9. Bitte überprüfen Sie den eingegebenen Wert.",
    mileageSingleLess: "Einzelne Kilometerleistung sollte nicht größer als gesamtkilometerleistung sein.",
    mileageTotalLarger: "Gesamte kilometerleistung sollte nicht kleiner als Einzelne Kilometerleistung sein.",

    pairCardIn: "Geben Sie den Status der erkannten Schlüsselkarte ein.",
    pairCardSuccess: "Erfolgreich, Die Schlüsselkarte ist erkannt.",

    cNam: AttributeName
};

const Controller = {
    controller: "Kontroller",
    auth: "Autorisierung",
    applyControllerAuth: "Kontroler Autorisierung beantragen",
    applyAllAuth: "Alle Autorisierungen beantragen",
    noControllerAuthTip: "Sie haben keine Autorisierung, um den Kontroler zu pogrammieren. Bitte fragen Sie die den Hersteller um Autorisierung.",
    applySuccess: "Erfolgreich beantragt. Bitte warten Sie auf Autorisierung. ",
    brandEmptyTip: "Sie haben keine Partnermarke.",
    getApplySuccess: "Erfolgreich Autorisierung enthalten",
    controllerTip: "Die Hauptdaten sind leer. Bitte überprüfen Sie, ob der Kontroller eingeschaltet is.",
    cNam: AttributeName
};

const Battery = {
    battery: "Akku",
    batteryTip: "Die Hauptdaten sind leer. Bitte überprüfen Sie, ob die Batterie eingeschaltet is.",
    cNam: AttributeName
};

const Update = {
    update: "Update",
    fileCloudToUpdate: "Eine Datei aus Cloud herunterladen",
    fileToUpdate: "Eine Datei vom lokalen herunterladen ",
    itemName: "Punktname",
    tip: "Tipp",
    updateTip: "Bitte wählen Sie zuerst den Punkt aus, ldanach aden Sie die Hardware-Datei herunter und aktualisieren Sie diese",
    noHardwareFileTip: "Es gibt keine Hardware-Datei in diesem Punkt.",
    noResultTip: "Es gibt keine entsprechende daten.",
    picture: "Bild",
    name: "Name",
    partNo: "Artikelnummer",
    operation: "Vorgang",
    version: "Version",
    updateTime: "Update-Zeit",
    remark: "Bemerkung",
    select: "Auswählen",
    firmwareList: "Firmwareliste",
    objectEmptyTip: "Es gibt keinen Punkt in dieser Kategorie.",
    componentTypeTip: "Bitte wählen sie Komponente",

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
    setting: "Einstellungen",
    language: "Sprache",
    version: "Version",
    versionDate: "Versionsdatum",
    userInfo: "Konto",
    searchForUpdate: "Nach dem Update suchen",
    changePassword: "Passwort ändern",
    editCompanyInfo: "Firmeninformation bearbeiten",
    password: 'Passwort',
    oldPassword: "Altes Password",
    newPassword: "Neues Password",
    confirmPassword: "Passwort bestätigen",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    authTipOne: "Sie haben keine Befugnis.",
    authTipTwo: "Bitte kontaktieren Sie den Administrator der Firma.",
    warning: "Warnung",
    editCompany: "Firmeninformation bearbeiten",
    logo: "Logo",
    uploadTipOne: "Bitte laden sie ein quadratisches Bild hoch.",
    uploadTipTwo: "Unterstüzende Formate: jpg, png und 300*400*400, bis zu 3MB.",
    name: "Name",
    email: "E-mail",
    phone: "Telefon",
    website: "Webseite",
    address: "Adresse",
    postcode: "Postleitzahl",
    company: "Firma",
    country: "Land",

    languages: {
        german: "Deutsch",
        english: "Englisch",
        chinese: "中文",
        dutch: "Nederlands",
    },
    orgType: {
        brand: "MARKE",
        dealer: "HÄNDLER",
        assembler: "MONTEUR",
        component: "KOMPONENTE",
        serviceCenter: "SERVICE CENTER",
    },
    updToolNote1: "Das BESST Service-Werkzeug benötigen neue Software zu aktualisieren, damit die Funktionen der Komponenten besser zur Verfügung stehen. ",
    updToolNote: "Hinweis",
    updToolNote2: "Bitte schließen Sie dieses Dialogfenster erst, wenn Sie erfolgreich aktualisiert haben.",
    updToolNote3: "Wenn die Aktualisierung erfolgreich durchgeführt wird, wird das Service-Werkzeug zum Normalstatus wechseln.",
    updToolNote4: 'Wenn der Fortschrittsbalken für die Aktualisierung nicht angezeigt wird, trennen Sie das Service-Werkzeug am Computer und stellen Sie erneut eine Verbindung zum PC her. Klicken Sie dann auf "Aktualisieren" Button wieder.',
    updStarting: "Aktualisierung startet...",
    updSuccess: "Erfolgreich aktualisieren?",
    close: "Schließen",
    cNam: AttributeName,
    upd: Update,
    op: Operation

};

const Repair = {
    cNam: AttributeName
};

// bike basic
const VehicleDetail = {
    back: "Zurück",
    sn: "Seriennummer",
    key: "Schlüssel",
    productTime: "Produktionszeit",
    itemName: "Modellname",
    itemPartNo: "Artikelteil Nr.",
    orderSn: "Auftragsseriennummer",
    orderSourceId: "Bestellnummer",
    customerId: "Kundenname",
    customerPartNo: "Kunde Artikelnummer",
    customerPoNo: "Kunden Bestellnummer",
    replace: "Ersetzen",
    oldSn: "Alte SN",
    newSn: "Neue SN",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    save: "Speichern",
    edit: "Bearbeiten",
    add: "Hinzufügen",
    reportList: "Diagnosebericht",
    detail: "Detail",
    operation: "Vorgang",
    date: "Datum",
    author: "Urheber",
    emptyListTip: "Das Fahrzeug hat keinen Bericht.",
    assembler: "Montagearbeiter",
    dealer: "Händler",
    saleTime: "Verkaufszeit",
    customerName: "Kundenname",
    customerEmail: "Kunde E-mail",
    part: "Komponente",
    history: "Verlauf"
};

const VehicleList = {
    oem: "Erstausrüster",
    modelName: "Modellname",
    brand: "Marke",
    vehicleSn: "Fahrzeug SN",
    bomSn: "Komponente SN",
    search: "Suchen",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    deleteVehicle: "Das Fahrzeug löschen",
    deleteTip: "Sind Sie sicher, es zu löschen?",
    yes: "Ja",
    no: "Nein",
    delete: "Löschen",
    detail: "Detail",
    noSale: "Unverkauft",
    tableHead: {
        vehicleSn: "Fahrzeug SN",
        brandName: "Markenname",
        ModelName: "Modellname",
        OrderSn: "Auftragsseriennummer",
        saleDate: "Verkaufsdatum",
        operation: "Vorgang",
    },
    deleteSuccess: "Erfolgreich gelöscht",
    export: "Ausfuhr",
    start: "Start",
    end: "Am Ende",
    op: Operation,
};


// 服务中心相关
const SCCenterTicketList = {
    category: "Kategorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Zielseriennummer",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang",
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fahrzeug",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Akku",
        light: "Licht",
        controller: "Kontroller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        pendingAssign: "Ausstehend Zuweisen",
        processing: "In Bearbeitung",
        complete: "Fertiggestellt"
    },
    noResult: "Kein Ergebnis！",
    total: "Total"
};

const SCHandleUpTicketList = {
    category: "Kategorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Zielseriennummer",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang",
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fahrzeug",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Akku",
        light: "Licht",
        controller: "Kontroller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        pendingAssign: "Ausstehend Zuweisen",
        processing: "In Bearbeitung",
        complete: "Fertiggestellt"
    },
    noResult: "Kein Ergebnis！",
    total: "Total"
};

const SCMyTicketList = {
    category: "Kategorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Zielseriennummer",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang",
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fahrzeug",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Akku",
        light: "Licht",
        controller: "Kontroller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        pendingAssign: "Ausstehend Zuweisen",
        processing: "In Bearbeitung",
        complete: "Fertiggestellt"
    },
    noResult: "Kein Ergebnis！",
    total: "Total",
    addTicketDialog: {
        dialogTitle: "Service-Ticket hinzufügen",
        category: "Kategorie",
        title: "Titel",
        description: "Beschreibung",
        bike: "E-Bike",
        sn: "Seriennummer",
        serviceCenter: "Service Center",
        image: "Bild",
        video: "Video",
        clickToUpload: "Klicken zum Hochladen"
    },
};

const SCTicketDetail = {
    back: "Zurück",
    transferToUp: "nach oben aufgeben",
    transferToInside: "nach innen aufgeben",
    take: "Übernehmen",
    history: "Verlauf",
    ticketInfo: {
        ticketTitle: "Reklamationsdetail",
        serviceNo: "Service Nr.",
        createTime: "Erstellungszeit",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Titel",
        description: "Beschreibung",
        image: "Bild",
        video: "Video",
    },
    userInfo: {
        address: "Adresse",
        website: "Webseite",
        email: "Email",
    },
    content: "Inhalt",
    image: "Bild",
    sendMessage: "Nachricht senden",
    historyTicket: {
        title: "Ticket-Verlauf",
        name: "Name",
        operation: "Vorgang",
        time: "Zeit",
    },
    handleInside: {
        title: "Intern anfassen",
        tip: "Wählen Sie eine Person für die Bearbeitung des Tickets.",
        select: "Auswählen",
        confirm: "Bestätigen",
        cancel: "Stornieren"
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
    back: "Zurück",
    sn: "Seriennummer",
    category: "Kategorie",
    modelName: "Modellname",
    partNo: "Artikelnummer",
    orderNo: "Auftrag Nr.",
    customerName: "Kundenname",
    customerPartNo: "Kunde Artikelnummer",
    customerPoNo: "Kunden Bestellnummer",
    produceTime: "Herstellungszeit",
    saleTime: "Verkaufszeit",
};

const ComponentComponentList = {
    yes: "Ja",
    no: "Nein",
    search: {
        poNo: "Artikel Nr.",
        orderNo: "Auftrag Nr.",
        modelName: "Modellname",
        sn: "Seriennummer",
        customerName: "Kundenname",
        search: "Suchen",
        export: "Exportieren",
    },
    headData: {
        poNo: "Artikel Nr.",
        orderNo: "Auftrag Nr.",
        sn: "Seriennummer",
        category: "Kategorie",
        modelName: "Modellname",
        customerName: "Kundenname",
        operation: "Vorgang"
    },
    noResult: "Kein Ergebnis！",
    total: "Total",
    detail: "Detail",
    delete: "Löschen",
    batchDelete: "Stapel löschen",
    deleteDialog: {
        title: "Komponente löschen",
        content: "Sind Sie sicher, sie zu löschen?",
    },
    exportDialog: {
        title: "Komponente exportieren",
        startNum: "Startnummer",
        endNum: "Endnummer",
        export: "Exportieren",
        cancel: "Stornieren"
    },
    back: "Zurück",
};

const ComponentComponent = {
    Detail: ComponentComponentDetail,
    List: ComponentComponentList
};

// 零部件相关 --- TestFeedback
const ComponentTestFeedbackList = {
    search: {
        email: "Email",
        searchBtn: "Suchen",
    },
    add: "Hinzufügen",
    noResult: "Kein Ergebnis!",
    total: "Total",
    deleteDialog: {
        title: "Ergebnis löschen",
        content: "Sind Sie sicher, sie zu löschen?",
        yes: "Ja",
        no: "Nein",
    },
    headData: {
        date: "Datum",
        email: "Email",
        fileName: "Dateiname",
        status: "Status",
        operation: "Vorgang",
        desc: "Desc"
    },
    detail: "Detail",
    delete: "Löschen",
};

const ComponentTestFeedbackDetail = {
    back: "Zurück",
    fileDetail: "Ergebnisinformation",
    edit: "Bearbeiten",
    delete: "Löschen",
    detail: {
        title: "Datei Info",
        email: 'Email',
        fileName: "Dateiname",
        setTime: "Zeit erstellen",
        status: "Status",
        upload: "Ergebnis hochladen",
        desc: "Desc",
    },
    deleteDialog: {
        title: "Ergebnis löschen",
        content: "Sind Sie sicher, sie zu löschen?",
        yes: "Ja",
        no: "Nein",
    },
    fileList: "Dateiliste",
    saveFeedback: "Ergebnis speichern",
    deleteFile: "Datei löschen",
};

const ComponentTestFeedbackSave = {

    createFeedback: "Ergebnis erstellen",
    feedbackInfo: "Ergebnisinformation",

    email: "Email ",
    fileName: "Dateiname",
    setTime: "Zeit erstellen",
    upload: "Ergebnis hochladen",
    status: "Status",
    desc: "Desc",

    file: "Hochladen",
    clickUpload: "Klicken Sie auf hochladen",

    delete: "Löschen",
    download: "Download",
    fileList: "Dateiliste",
    saveFeedback: "Ergebnis speichern",
    deleteFile: "Löschen",
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
    back: 'Back',
    delete: 'Delete',
    title1: 'Order Info',
    title2: 'Status Check',
    orderNo: 'Order No.',
    user: 'User',
    status: 'Status',
    desc: 'Desc',
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

    scanning: 'Scan QR code',

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
    back: "Zurück",
    orderDetail: "Bestellinformation",
    edit: "Bearbeiten",
    delete: "Löschen",
    orderInfo: {
        title: "Auftragsinfomation",
        orderNo: "Auftragsinfomation",
        customerPartNo: "Kunde Artikelnummer",
        note: "Notizen",
        customerName: "Kundenname",
        customerPo: "Kundenbestellung",
    },
    itemsList: "Artikelliste",
    produce: "Produktion",
    testReport: "Test Bericht",
    componentList: "Komponentenliste",
    replace: "Ersetzen",
    headData: {
        modelName: "Modellname",
        articleNo: "Artikelnummer",
        customerArticleNo: "Kunden Artikelnummer",
        quantity: "Menge",
        operation: "Vorgang",
    },
    deleteDialog: {
        title: "Auftrag löschen",
        content: " Sind Sie sicher, ihn zu löschen?",
        yes: "Ja",
        no: "Nein"
    },
    category: "Kategorie",
    search: "Suchen",
    clear: "Löschen",
    selectCategory: "Kategorie auswählen",
    modelNamePN: "Modellname/Artikelnummer",
    confirmDialog: {
        replace: "Ersetzen",
        yes: "Ja",
        no: "Nein"
    },

    dataConnect: "Daten verbinden"
};

const ComponentOrderList = {
    search: {
        orderNo: "Auftrag Nr.",
        searchBtn: "Suchen",
        clearBtn: "Löschen",
    },
    add: "Hinzufügen",
    list: "Liste",
    detail: "Detail",
    delete: "Löschen",
    noResult: "Kein Ergebnis！",
    total: "Total",
    headData: {
        date: "Datum",
        orderNo: "Auftrag Nr.",
        customerName: "Kundenname",
        pQ: "Produkt/Menge",
        status: "Status",
        operation: "Vorgang",
        model: "Modell",
    },
    deleteDialog: {
        title: "Den Auftrag löschen",
        content: "Sind Sie sicher, ihn zu löschen?",
        yes: "Ja",
        no: "Nein",
    },
    op: Operation,

};

const ComponentOrderProduce = {
    back: "Zurück",
    connect: "Verbinden",
    reset: "Zurücksetztzen",
    update: "Update",
    readSn: "Lese SN",
    writeSn: "Schreibe SN",
    orderInfo: {
        title: "Auftragsinfo",
        model: "Modell",
        customerName: "Kundenname",
        customerPoNo: "Kunden Bestellnummer",
        customerPartNo: "Kunde Artikelnummer",
    },
    programingInfo: {
        title: "Programm",
        com: "COM",
        file: "Datei",
        pleaseSelect: "Bitte wähle"
    },
    writeSnInfo: {
        title: "Schreibe SN",
        com: "COM",
        pleaseSelect: "Bitte wähle",
        sn: "Seriennummer"
    },
    cNam: AttributeName,
    up: Update,
};

const ComponentOrderDataConnect = {
    back: "zurück",
    save: "Speichern",
    clear: "Löschen",
    removeBinding: "Verbindung trennen",
    orderInfo: {
        title: "Auftragsinfo",
        model: "Modell",
        customerName: "Kundenname",
        customerPoNo: "Kunden Bestellnummer",
        customerPartNo: "Kunden Artikelnummer",
    },
    programingInfo: {
        ListTitle: "Ergebnisliste",
        searchTitle: "Suchen",
        motorTitle: "Motor",
        result: "Ergebnis",
        pleaseSelect: "Bitte wählen Sie",
        sn: "Seriennumer",
        cr: "Kontroller",
        sr: "Sensor",
        mm: "Motor",
    },

};

const ComponentOrderSave = {
    back: "Zurück",
    createOrder: "Auftrag erstellen",
    orderInfo: {
        title: "Auftragsinformation",
        poNo: "Bestellnummer",
        customerInfo: "Kundeninforamtion",
        customerPartNo: "Kunde Artikelnummer",
        customerName: "Kundenname",
        customerPo: "Kundenbestellung",
        note: "Notizen",
    },
    itemList: "Artikelliste",
    addItem: "Element hinzufügen",
    addItems: "Elemente hinzufügen",
    delete: "Löschen",
    saveOrder: "Auftrag speichern",
    tableHead: {
        name: "Name",
        articleNo: "Artikelnummer",
        customerArticleNo: "Kunden Artikelnummer",
        quantity: "Menge",
        operation: "Vorgang",
    },
    itemHeadData: {
        image: "Bild",
        name: "Name",
        partNo: "Artikelnummer"
    },
    category: "Kategorie",
    search: "Suchen",
    clear: "Löschen",
    selectCategory: "Kategorie auswählen",
    modelNamePN: "Modellname/Artikelnummer"
};

const ComponentOrderTestReport = {
    back: "Zurück",
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
    back: "Zurück",
    add: "Hinzufügen",
    save: "Speichern",
    edit: "Bearbeiten",
    print: "Drucken",
    export: "Exportieren",
    delete: "Löschen",
    detail: "Detail",
    articleNO: "Artikelnummer",
    noPhoto: "Kein Foto",
    viewLargerImage: "Größeres Bild anzeigen",
    list: "Liste",
    partName: "Bezeichnung Teil",
    yes: "Ja",
    no: "Nein",
    listHead: {
        itemNo: "ARTIKELNUMMER",
        partNo: "ARTIKELNUMMER",
        picture: "BILD",
        name: "NAME",
        spec: "MAß",
        package: "VERPACKUNG",
        packageSize: "VERPACKUNGSGRÖßE",
        price: "PREIS",
        linkBom: "LINK STÜCKLISTE",
        operation: "Vorgang",
    },
    selectHead: {
        partNo: "ARTIKELNUMMER",
        picture: "BILD",
        name: "NAME",
        spec: "MAß",
        package: "VERPACKUNG",
        packageSize: "VERPACKUNGSGRÖßE",
        price: "PREIS",
        linkBom: "LINK STÜCKLISTE"
    },
    deleteProductDialog: {
        title: "Produkt löschen",
        content: "Sind sie sicher, das Produkt zu löschen?",
    },
    addItemDialog: {
        title: "Element hinzufügen",
    },
    operation: "Vorgang",
    tip: "Tipp",
    content01: "Bitte laden sie ein qudratisches Bild hoch",
    content02: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB."
};

const ComponentProductDetail = {
    back: "Zurück",
    copy: "Kopieren",
    edit: "Bearbeiten",
    delete: "Löschen",
    model: "Modell",
    partNo: "Artikelnummer",
    name: "Name",
    articleList: "Artiklelliste",
    desc: "Beschreibung",
    download: "Herunterladen",
    preview: "Vorschau",
    history: "Verlauf",
    update: "Hochladen",
    view: "Nachsehen",
    hardware: "Hardware Version",
    firmware: "Firmware",
    bomList: "Stückliste",
    yes: "Ja",
    no: "Nein",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    deleteProductDialog: {
        title: "Produkt entfernen",
        content: "Sind Sie sicher, das Produkt zu entfernen? ",
    },
    deleteFirmwareDialog: {
        title: "Software entfernen",
        content: "Sind Sie sicher, die Software zu entfernen?",
    },
    historyDialog: {
        title: "Softwareverlauf",
        version: "Version",
        updateTime: "Update-Zeit",
        remark: "Bemerkung",
        operation: "Vorgang",
        noResult: "Kein Ergebnis！"
    },
    editFirmwareDialog: {
        title: "Firmware hochladen",
        file: "Datei",
        version: "Version",
        remark: "Bemerkung",
        clickToUpload: "Zum Hochladen klicken",
    },
    updateFirmwareDialog: {
        title: "Firmware hochladen",
        file: "Datei",
        version: "Version",
        remark: "Bemerkung",
        clickToUpload: "Zum Hochladen klicken",
    },
    training: 'Training',
    set: ArticleSet

};

const ComponentProductEditCategory = {
    back: "Zurück",
    category: "Kategorie",
    yes: "Ja",
    no: "Nein",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    deleteDialog: {
        title: "Kategorie löschen",
        content: "Sind sie sicher, die Kategorie zu löschen? ",
    },
    addSubCategoryDialog: {
        title: "Unterkategorie hinzufügen",
        namePH: "Kategoriename"
    },
    editCategoryDialog: {
        title: "Kategorie bearbeiten",
        namePH: "Kategoriename"
    },
    addSubCategory: "Unterkategorie hinzufügen",
    edit: "Bearbeiten",
    delete: "Löschen",
};

const ComponentProductIndex = {
    editCategory: "Kategorie bearbeiten",
    training: 'Training'
};

const ComponentProductList = {
    add: "Hinzufügen",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    set: ArticleSet
};

const ComponentProductSave = {
    back: "Zurück",
    logo: "Bild",
    name: "Name",
    partNo: "Artikelnummer",
    desc: "Beschreibung",
    save: "Speichern",
    tip: {
        title: "Tipp",
        content01: "Bitte laden sie ein quadratisches Bild hoch",
        content02: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB.",
    },
    add: "Hinzufügen",
    copy: "Kopieren",
    edit: "Bearbeiten",
    set: ArticleSet

};

const ComponentVideoList = {
    back: "Zurück",
    category: "Kategorie",
    add: "Hinzufügen",
    download: "Herunterladen",
    delete: "Löschen",
    noResult: " Kein Ergebnis！",
    total: "Gesamt",
    uploadVideo: "Video hochladen",
    upload: "Hochladen",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    all: "Alle",
    systemError: "Systemfehler",
    hmi: "Display",
    motor: "Motor",
    controller: "Kontroller",
    sensor: "Sensor",
    battery: "Akku",
    charger: "Ladegerät",
    name: "Name",
    operation: "Vorgang",
    videoEmptyTip: "Bitte Video hochladen",
    saveSuccess: "Erfolgreich speichern",
    yes: "Ja",
    no: "Nein",
    deleteVideo: "Video löschen",
    deleteTip: "Sind Sie sicher, das Vedio zu löschen?",
    preview: "Vorschau",
    errorCode: "Fehlercode",
    link: "Verbindung",
    edit: "Bearbeiten"
};

const ComponentProductVideoList = {
    back: "Zurück",
    category: "Kategorie",
    addVideo: "Video hinzufügen",
    download: "Herunterladen",
    delete: "Löschen",
    noResult: " Kein Ergebnis！",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    systemError: "Systemfehler",
    hmi: "Display",
    motor: "Motor",
    controller: "Kontroller",
    sensor: "Sensor",
    battery: "Akku",
    charger: "Ladegerät",
    name: "Name",
    operation: "Vorgang",
    yes: "Ja",
    no: "Nein",
    preview: "Vorschau",
    deleteVideo: "Video löschen",
    deleteTip: "Sind Sie sicher, das Vedio zu löschen?",
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
    ticketStat: "Ticket Statistiken",
    ticketByCategory: "Ticket Kategorie",
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
    order: "Auftrag",
    diagnosis: "Diagnose"
};

// 经销商相关 --- error
const DealerErrorShooting = {
    errorCode: "Fehlercode",
    name: "Name",
    remark: "Bemerkung",
    description: "Beschreibung",
    target: "Ziel",
    solution: "Lösung",
    tip: "Tipp",
    tipContent: "Wenn die obige Lösung Ihr Problem nicht lösen kann, wenden Sie sich bitte an den Hersteller.",
    help: "Hilfe",
    helpContent: "Bitte klicken Sie hier, um mehr Unterstützung zu erhalten.",
    link: "Video Eingang"
};

const DealerError = {
    Shooting: DealerErrorShooting,
};

// 经销商相关 --- partner
const DealerPartnerList = {
    back: "Zurück",
    acceptInvitation: "Einladung annehmen",
    tableHead: {
        name: "Name",
        phone: "Telefon",
        website: "Webseite",
        country: "Land",
        operation: "Vorgang",
    },
    currentBrand: "Aktuelle Marke",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    brandInfo: {
        title: "Markeninfo",
        logo: "Bild",
        name: "Name",
        type: "Typ",
        email: "E-mail",
        website: "Webseite",
        tel: "Telefon",
        postcode: "Postleitzahl",
        address: "Adresse",
    },
    invitationCode: "Einladungscode",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    webLink: "Webseite",
    createOrder: "Auftrag erstellen",
    orderList: "Aufträgelist ",
    view: "Nachsehen",
};

const DealerPartner = {
    List: DealerPartnerList,
};

// 经销商相关 --- sale
const DealerSaleDetail = {
    back: "Zurück",
    orderDetail: "Bestellinformation",
    view: "Nachsehen",
    edit: "Bearbeiten",
    delete: "Löschen",
    invoiceNo: "Rechnung Nr.",
    invoiceDate: "Rechnungsdatum",
    submit: "Einreichen",
    customerInfo: {
        title: "Kundeninfo",
        name: "Name",
        phone: "Telefon",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Fahrradinfo",
        brand: "Marke",
        bikeId: "Fahrrad Id",
        model: "Modell",
        warrantyPeriod: "Garantiezeit"
    },
    detail: {
        title: "Details",
        orderNo: "Auftrag Nr.",
        price: "Preis",
        invoice: "Rechnung",
        salesDate: "Verkaufsdatum",
        remark: "Bemerkung",
    },
    deleteDialog: {
        title: "Den Auftrag Löschen",
        content: "Sind Sie sicher, ihn zu löschen?",
        yes: "Ja",
        no: "Nein"
    }
};

const DealerSaleEdit = {
    back: "Zurück",
    save: "Speichern",
    orderEdit: "Auftrag bearbeiten",
    customerInfo: {
        title: "Kundeninfo",
        name: "Name",
        phone: "Telefon",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Fahrradinfo",
        brand: "Marke",
        bikeId: "Fahrrad Id",
        model: "Modell",
        warrantyPeriod: "Garantiezeit"
    },
    detail: {
        title: "Details",
        orderNo: "Auftrag Nr.",
        price: "Preis",
        invoice: "Rechnung",
        remark: "Bemerkung",
        invoiceDate: "Rechnungsdatum",
    },
};

const DealerSaleList = {
    add: "Hinzufügen",
    tableHead: {
        orderNo: "Auftrag Nr.",
        bikeId: "Fahrrad ID",
        model: "Modell",
        customerName: "Kundenname",
        date: "Datum",
        status: "Status",
        operation: "Operation"
    },
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    detail: "Detail",
    search: {
        bikeId: "Fahrrad ID",
        searchBtn: "Suchen",
        clearBtn: "Beseitigen"
    }
};

const DealerSaleSave = {
    back: "Zurück",
    addComponent: "Komponente hinzufügen",
    save: "Speichern",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    customerInfo: {
        title: "Kundeninfo",
        name: "Name",
        phone: "Telefon",
        email: "E-mail",
        tip: "Tipp",
        tip01: "E-Mail im Zusammenhang mit Kundendienst und Schlüssel",
        tip02: "Bitte vergewissern Sie sich, dass E-Mail gültig ist. "
    },
    bikeInfo: {
        title: "Fahrradinfo",
        bikeId: "E-Bike Id",
        brand: "Marke",
        warrantyPeriod: "Garantie",
        date: "Datum",
        price: "Preis",
        remake: "Bemerkung",
    },
    componentInfo: {
        add: "Hinzufügen",
        title: "Komponenteninfo",
        replace: "Ersetzen",
        sn: "Seriennummer",
        name: "Name"
    },
    replaceDialog: {
        oldSn: "Alte SN",
        newSn: "Neue SN",
        category: "Kategorie",
    },
    tipDialog: {
        title: "Tipp",
        content: "Das E-Bike hat keine Batterie, sind Sie sicher?"
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
    back: "Zurück",
    markCompleted: "Marke fertiggestellt",
    ticketInfo: {
        InfoTitle: "Reklamationsdetail",
        serviceNo: "Service Nr.",
        status: "Status",
        createTime: "Erstellungszeit",
        serviceCenter: "Service Center",
        title: "Titel",
        description: "Beschreibung",
        image: "Bild",
        video: "Video",
    },
    user: {
        address: "Adresse",
        website: "Webseite",
        email: "E-mail",
    },
    replay: {
        content: "Inhalt",
        image: "Bild",
        sendMessage: "Nachricht senden"
    },
};

const DealerTicketList = {
    add: "Hinzufügen",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetNo: "Seriennummer",
        ServiceCenter: "Service Center",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang",
    },
    detail: "Detail",
    delete: "Löschen",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    addTicketDialog: {
        dialogTitle: "Service-Ticket hinzufügen",
        category: "Kategorie",
        title: "Titel",
        description: "Beschreibung",
        bike: "E-Bike",
        sn: "Seriennummer",
        serviceCenter: "Service Center",
        image: "Bild",
        video: "Video",
        clickToUpload: "Klicken zum Hochladen"
    },
    deleteTicket: {
        title: "Ticket löschen",
        content: "Sind Sie sicher, es zu löschen? "
    }
};

const DealerTicket = {
    Detail: DealerTicketDetail,
    List: DealerTicketList
};

const DealerVehicleSearch = {
    search: {
        bikeId: "Fahrrad ID",
        searchBtn: "Suchen",
        clear: "Beseitigen"
    },
    back: "Zurück",
    sn: "Seriennummer",
    key: "Schlüssel ",
    productTime: "Produktionszeit",
    itemName: "Modellname",
    itemPartNo: "Artikelteil Nr.",
    orderSn: "Auftragsseriennummer",
    orderSourceId: "Auftrasquelle Id",
    customerId: "Kunde ID",
    customerPartNo: "Kunde Artikelnummer",
    customerPoNo: "Kunden Bestellnummer",
    replace: "Ersetzen",
    oldSn: "Alte SN",
    newSn: "Neue SN",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    save: "Speichern",
    edit: "Bearbeiten",
    reportList: "Diagnoseberichten",
    detail: "Detail",
    operation: "Vorgang",
    date: "Datum",
    author: "Urheber",
    emptyListTip: "Das Fahrzeug hat keinen Bericht.",
    part: "Komponenten",
    history: "Ersetzungsverlauf",
    brand: "Marke",
    saleTime: "Verkaufszeit",
    customerName: "Kundenname",
    customerEmail: "Kunde Email"
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
    dailyVehicleOrderStatistics: "Tägliche Fahrzeugbestellstatistiken",
    dailyVehicleAfterSalesStatistics: "Tägliche Fahrzeug-After-Sale-Statistiken",
    dailyDailyVehicleStatistics: "Tägliche Händlerfahrzeugstatistiken",
    dailyAssemblerVehicleStatistics: "Tägliche Montagefahrzeugstatistiken"
};

//品牌商
const BrandList = {
    add: "Hinzufügen",
    edit: "Bearbeiten",
    delete: "Löschen",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    deleteBrand: "Die Marke löschen",
    deleteTip: "Sind Sie sicher, sie zu löschen?",
    yes: "Ja",
    no: "Nein",
    addBrand: "Marke hinzufügen",
    brandInfo: "Markeninfo",
    logo: "Logo",
    uploadTipOne: "Bitte laden sie ein quadratisches Bild hoch",
    uploadTipTwo: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB.",
    name: "Name",
    introduction: "Einführung",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    operation: "Vorgang",
    emptyTip: {
        logo: "Bitte Markenlogo hochladen",
        name: "Bitte Markenname eingeben",
        desc: "Bitte Markenbeschreibung eingeben",
    },
    editSuccess: "Erfolgreich editiert",
    addSuccess: "Erfolgreich hinzugefügt",
    deleteSuccess: "Erfolgreich gelöscht",

    link: "Verbindung",
};

const DealerInvite = {
    codeStatus: "Code Status",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        invitation: "Einladungscode",
        targetName: "Firmenname",
        contact: "Kontakte",
        targetEmail: "Ziel-Email",
        createTime: "Erstellungszeit",
    },
    codeStatusOptions: {
        readyToSend: "versandfertig",
        sentNotUsed: "gesendet, wurde aber nicht aktiviert",
        used: "wurde erfolgreich aktiviert",
    },
    emptyTip: {
        email: "Bitte E-mail eingeben",
        name: "Bitte Firmenname eingeben",
    },
    sendSuccess: "Erfolgreich gesendet",
    uploadSuccess: "Erfolgreich hochgeladen",
    selectTip: "Bitte wählen Sie ein Konto",
    deleteSuccess: "Erfolgreich gelöscht",
    newCode: "Neuer Code",
    batchSend: "Stapel senden",
    send: "Senden",
    delete: "Löschen",
    dealerInvitation: "Händler-Einladung",
    selectAccount: "Konto auswählen",
    uploadContacts: "Kontakte hochladen",
    operation: "Vorgang",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    deleteTip: "Sind Sie sicher, sie zu löschen?",
    yes: "Ja",
    no: "Nein",
    email: "E-mail",
    companyName: "Firmenname",
    toLeft: "Nach links",
    toRight: "Nach rechts",
    deleteCode: "Den Code löschen",
    orgName: "Firmenname",
    uploadFailList: "Fehlerliste hochladen",
    reason: "Grund",
    uploadFailTip: "Upload fehlgeschlagen. Bitte überprüfen Sie die hochgeladene Datei oder ob alle Kontakte vorhanden sind.",
    resend: "Erneut senden",
    template: "Kontaktevorlage",
    download: "Herunterladen",

    source: "Quelle",
    target: "Ziel",
    enterKeyWord: "Passwort eingeben",
    noData: "keine Daten",
    dealerType: "Händlertyp",
    dealerType1: "Händler",
    dealerType2: "Spezieller Händler",
    dealerTypeNote: "Spezieller Händler haben nur Diagnose- und Werkzeug-funktion. Händler hat alle Funktionen.",
    op: Operation,

};

const DealerList = {
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        logo: "Logo",
        name: "Name",
        postCode: "Postleitzahl",
        tel: "Telefon",
        website: "Webseite",
        address: "Adresse",
        operation: "Vorgang",
        email: "E-mail",
    },
    delete: "Löschen",
    deleteDialog: {
        title: "Den Händler löschen",
        tip: "Sind Sie sicher, ihn zu löschen?"
    },
    yes: "Ja",
    no: "Nein",
    op: Operation,
};

const OemInvite = {
    codeStatus: "Code Status",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        invitation: "Einladungscode",
        targetName: "Firmenname",
        contact: "Kontakte",
        targetEmail: "Ziel-Email",
        createTime: "Erstellungszeit",
    },
    codeStatusOptions: {
        readyToSend: "versandfertig",
        sentNotUsed: "gesendet, aber nicht aktiviert wird",
        used: "Erfolgreich aktiviert wurde",
    },
    emptyTip: {
        email: "Bitte E-Mail eingeben",
        name: "Bitte Firmenname eingeben",
    },
    sendSuccess: "Erfolgreich gesendet",
    uploadSuccess: "Erfolgreich hochgeladen",
    selectTip: "Bitte wählen Sie ein Konto",
    deleteSuccess: "Erfolgreich gelöscht",
    newCode: "Neuer Code",
    batchSend: "Stapel senden",
    send: "Senden",
    delete: "Löschen",
    oemInvitation: "Erstausrüster einladen",
    selectAccount: "Konto auswählen",
    uploadContacts: "Kontakte hochladen",
    operation: "Vorgang",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    deleteTip: "Sind Sie sicher, sie zu löschen?",
    yes: "Ja",
    no: "Nein",
    email: "E-mail",
    companyName: "Firmenname",
    toLeft: "Nach links",
    toRight: "Nach rechts",
    deleteCode: "Den Code löschen",
    orgName: "Firmenname",
    uploadFailList: "Fehlerliste hochladen",
    reason: "Grund",
    uploadFailTip: "Upload fehlgeschlagen. Bitte überprüfen Sie die hochgeladene Datei oder oballe Kontakte vorhanden sind.",
    resend: 'Erneut senden',
    download: "Herunterladen",
    template: "Kontaktevorlage",
    op: Operation,

};

const OemList = {
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        logo: "Logo",
        name: "Name",
        postCode: "Postleitzahl",
        tel: "Telefon",
        website: "Webseite",
        address: "Adresse",
        operation: "Vorgang",
        email: "E-mail",
    },
    delete: "Löschen",
    deleteDialog: {
        title: "Den Erstausrüster löschen",
        tip: "Sind Sie sicher, ihn zu löschen?"
    },
    yes: "Ja",
    no: "Nein",
    op: Operation,
};

const AuthList = {
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        name: "Name",
        type: "Typ",
        status: "Status",
        time: "Zeit",
        operation: "Vorgang",
    },
    authorization: "Autorisierung",
    authorizationSuccess: "Autorisierung erfolgreich",
    status: "Status",
    authorized: "Autorisiert",
    noAuthorization: "Nicht autorisiert",
    hmi: "Display",
    controller: "Kontroller"
};

const RecordList = {
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        sn: "Seriennummer",
        vehicle: "Fahrzeug",
        operator: "Bediener",
        account: "Konto",
        status: "Status",
        time: "Zeit",
        type: 'Typ',
        data: "Daten"
    },
    status: "Status",
    recordTitle: "Aktionsdatensatz der Komponentenseriennummer",
    edit: "Bearbeiten"
};


const ModelDetail = {
    back: "Zurück",
    delete: "Löschen",
    edit: "Bearbeiten",
    copy: "Kopieren",
    name: "Name",
    brand: "Marke",
    partNo: "Artikelnummer",
    description: "Beschreibung",
    deleteModel: "Modell löschen",
    deleteTip: "Sind Sie sicher, es zu löschen?",
    yes: "Ja",
    no: "Nein",
    download: "Herunterladen",
    preview: "Vorschau",
    cancel: "Stornieren",
    confirm: "Bestätigen",
    view: "Nachsehen",
    bom: "Stückliste",

    specification: "Spezifizierung",
    currency: "Währung",
    year: "Jahr",
    color: "Farbe ",
    size: "Größe",
    gender: "Geschlecht",
    price: "Preis",

    link: "Verbindung",
};

const ModelList = {
    brand: "Marke",
    addModel: "Modell hinzufügen",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    deleteModel: 'Das Modell löschen',
    deleteTip: "Sind Sie sicher, es zu löschen?",
    yes: "Ja",
    no: "Nein",
    edit: "Bearbeiten",
    delete: "Löschen",
    detail: "Detail",
    tableHead: {
        name: "Name",
        logo: "Logo",
        partNo: "Artikelnummer",
        introduction: "Einführung",
        operation: "Vorgang",
        show: "Zeigen"
    },
    deleteSuccess: "Erfolgreich gelöscht",
    link: "Verbindung",
    op: Operation,
};

const ModelSave = {
    uploadTipOne: "Bitte laden sie ein quadratisches Bild hoch",
    uploadTipTwo: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB.",
    add: "Hinzufügen",
    edit: "Bearbeiten",
    copy: "Kopieren",
    back: "Zurück",
    save: "Speichern",
    emptyTip: {
        picture: "Bitte das Modell-Logo hochladen.",
        name: "Bitte die Modellname eingeben.",
        partNo: "Bitte die Artikelnummer eingeben.",
        desc: "Bitte die Modellbeschreibung eingeben.",
    },
    saveTip: "Bitte warten Sie auf das Speichern.",
    brandWrong: "Die Marke ist falsch. Bitte wählen Sie erneut.",
    saveSuccess: "Erfolgreich gespeichert",
    picture: "Bild",
    name: "Name",
    partNo: "Artikelnummer",
    introduction: "Einführung",
    description: "Beschreibung",
    cancel: "Stornieren",
    confirm: "Bestätigen",
    editAttr: "Eigenschaft bearbeiten",
    attrList: "Eigenschaftliste",

    specification: "Spezifizierung",
    currency: "Währung",
    year: "Jahr",
    color: "Farbe ",
    size: "Größe",
    gender: "Geschlecht",
    price: "Preis",
    brand: "Marke",
    link: "Verbindung",
};


const VehicleBomList = {
    back: "Zurück",
    add: "Hinzufügen",
    save: "Speichern",
    edit: "Bearbeiten",
    print: "Drucken",
    export: "Exportieren",
    delete: "Löschen",
    detail: "Detail",
    articleNO: "Artikelnummer",
    noPhoto: "Kein Bild",
    viewLargerImage: "Größeres Bild anzeigen",
    list: "Liste",
    partName: "Modellname/Artikelnummer",
    yes: "Ja",
    no: "Nein",
    listHead: {
        itemNo: "ARTIKELNUMMER",
        partNo: "ARTIKELNUMMER",
        picture: "BILD",
        name: "NAME",
        spec: "SPEZIFIKATION",
        package: "VERPACKUNG",
        packageSize: "VERPACKUNGSGRÖßE",
        price: "PREIS",
        linkBom: "LINK ARTIKELLISTE",
        warranty: "GARANTIE",
        operation: "VORGANG"
    },
    selectHead: {
        partNo: "ARTIKELNUMMER",
        picture: "BILD",
        name: "NAME",
        spec: "SPEZIFIKATION",
        package: "VERPACKUNG",
        packageSize: "VERPACKUNGSGRÖßE",
        price: "PREIS",
        linkBom: "LINK ARTIKELLISTE"
    },
    deleteProductDialog: {
        title: "Produkt löschen",
        content: "Sind Sie sicher, das produkt zu löschen? ",
    },
    addItemDialog: {
        title: "Artikel hinzufügen",
    },
    operation: "Vorgang",
    selectCategory: "Kategorie auswählen",
    category: "Kategorie",
    search: "Suchen",
    clear: "Beseitigen",
    tip: "Tipp",
    content01: "Bitte laden sie ein qudratisches Bild hoch",
    content02: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB."
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
    add: "Hinzufügen",
    self: "Selbst",
    manager: "Manager",
    permission: "Berechtigung",
    reset: "Zurücksetzen",
    delete: "Löschen",
    total: "Gesamt",
    addAccount: "Konto hinzufügen",
    resetPassword: "Passwort zurücksetzen",
    deleteAccount: "Konto löschen",
    email: "E-mail",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    deleteTip: "Sind Sie sicher, es zu löschen?",
    yes: "Ja",
    no: "Nein",
    resetPasswordTip: "Möchten Sie das Kennwort für dieses Konto auf 12345678 festlegen?",
    tableHead: {
        account: "Konto",
        createTime: "Erstellungszeit",
        operation: "Vorgang",
    },
    emptyEmailTip: "Bitte E-Mail eingeben.",
    addSuccess: "Erfolgreich hinzugefügt",
    deleteSuccess: "Erfolgreich gelöscht",
    resetSuccess: "Erfolgreich zurückgesetzt",
    saveAuthSuccess: "Erfolgreich gespeichert",
    noResult: "Kein Ergebnis！",
    invitationLength1: "The role of Dealer could only create 10 sub-accounts",
    invitationLength2: "The role of Assembler could only create 20 sub-accounts",
    invitationLength3: "The role of Brand could only create 20 sub-accounts",
    invitationLength4: "The role of Component could only create 20 sub-accounts",

    cNam: AttributeName
};

const ComponentsReplaceReport = {
    tableHead: {
        category: "Kategorie",
        oldComponent: "Alte Komponentennummer",
        newComponent: "Neue Komponentennummer",
    },
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    back: "Zurück"
};

const report = {
    back: "Zurück",
    diagnosisReportTitle: "E-BIKE Diagnosis Bericht",
    print: "Drucken",
    orgLabel: "Org ",
    orgTitle: "Org Info ",
    dealerLabel: "Händler",
    dealerTitle: "Händler Info ",
    assemblerLabel: "Montagearbeiter",
    assemblerTitle: "Montagearbeiterinformation ",
    name: "Name",
    address: "Adresse",
    email: "E-mail",
    tel: "Telefon",
    reportDate: "Report Date",
    bike: {
        title: "Bike Info"
    },
    diagnosis: {
        data: "Diagnose Daten",
        result: "Diagnose Ergebnis",
        hmi: "Diagnose Daten - Display",
        battery: "Diagnose Daten - Akku",
        controller: "Diagnose Daten - Kontroller",
        sensor: "Diagnose Daten - Sensor",
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
    connection: "Verbindung",
    newVer: "Neue Version",
    ok: "Ok",
    fail: "Fail",
    con: "Connected",
    disCon: "Disconnected",
    readOk: "Read Ok",
    readFail: "Read Fail",
    detail: "Detail",
    testSys: "Systemtest",
    cNam: AttributeName,
    rp: report,
    op: Operation,
    step: "Schritt",
    help: "Hilfe",
    continue: "Weiter",
    case: "Option",
    test: "Test",
    finish: "Fertig",
    name: "Name",
    remark: "Bemerkung ",
    desc: "Beschreibung ",
    target: "Ziel",
    solution: "Lösung",
    errTitle: "Fehlerlösung",
    reconnect: "Erneut verbinden",
    instruction: "Instruction",

    pedalSensor: "Pedalsensor",
    throttle: "Gasgriff ",
    brake: "Bremse",
    light: "Licht",
    support: "Assist mode",
    walk: "Schiebehilfe",
    speed: "Speedsignal",
    cadence: "Cadence sensor",
    torque: "Torque sensor",
    motor: "Motor",

    click: "Klicken",
    step11: "Bitte verbinden Sie BESST Tool mit dem Computer.",
    step12: "Bitte wählen Sie den richtigen COM-Port.",
    step13: "Das Licht wird grün.",
    step21: "Bitte wählen Sie die Komponenten des E-bikes aus.",
    step22: 'Connect BESST Tool to HMI and Controller.And press the "Reset" button on BESST box',
    step23: 'Press HMI power button to switch HMI to "PC" mode',
    step232: 'Press HMI power button four seconds turn on system',
    case11: "Das Display/HMI nicht eingeschaltet werden konnte:",
    case111: "System is not switched on:",
    case12: "Please check cable connection. If still no, you need a new HMI to move on. ",
    case121: "Please check cable connection. Unplug the HMI and plug back.",
    case13: 'There is no "PC" mode',
    case17: "Drücken Sie",
    case18: 'on the box and reopen HMI.You could repeat a few times.',
    case181: "on the box. If still no, you need a new HMI to move on. ",
    case20: 'Wenn es wieder ausfällt, brauchen Sie ein neues Display/HMI, um weiterzumachen.',
    case21: 'Tipp: Wenn Sie ein neues Display/HMI geändert haben, klicken Sie auf "Lesen", um die Verbindung wieder herzustellen.',
    test11: "Bitte hängen Sie das Fahrrad auf, um Verletzungen oder Beschädigungen zu vermeiden.",
    test12: "Bitte vergewissern Sie sich, dass das Rad frei drehbar ist, bevor Sie den Knopf drücken!",
    test13: "Wählen Sie die Funktionen.",
    test14: 'Select "All" or "S" , "All" for overall test, "S" for testing one by one;',
    test15: "Starten Sie den Testprozess. ",
    createReport: "Bericht erstellen ",

    start11: "Bitte drehen Sie den Gasgriff, dann sollte der Motor funktionieren. Sie sollten auch das grüne Licht sehen. ",
    start13: "Wenn Motor funktioniert mit kein grünes Licht, ist der Gasgriff in Ordnung. Andernfalls klicken Sie bitte ",
    start14: "und dann ersetzen Sie das Teil.",
    start15: "Bitte bremsen und lösen Sie den Bremshebel mindestens zweimal, dann sollten Sie grünes Licht sehen.",
    start16: "Wenn der Motor stoppt, aber kein grünes Licht leuchtet, ist der Schalter von der Bremse in Ordnung. Andernfalls klicken Sie bitte ",
    start17: 'Bitte drücken und halten Sie den Knopf" ',
    start18: '"auf dem Display/HMI. Sie sollten sehen, dass das Licht des Fahrrads an oder aus geht, das grüne Licht geht an.',
    start19: "Wenn das Licht des Fahrrads nicht funktioniert, Sie aber trotzdem sehen, dass das grüne Licht leuchtet, dann ersetzen Sie das Vordere oder hintere Rücklichtlicht. Wenn das Licht des Fahrrads nicht funktioniert oder das grüne Licht nicht leuchtet, klicken Sie bitte auf den Button",
    start20: "und dann das Dispay/HMI oder den Kontroller austauschen.",
    start21: 'Bitte drücken Sie abwechselnd "+" und "-", dann sollten Sie das grüne Licht sehen.',
    start22: "Wenn kein grünes Licht leuchtet, klicken Sie bitte den Botton ",
    start23: " und ersetzen Sie dann das Display/HMI.",
    start24: 'Bitte drücken und halten Sie den Knopf "',
    start25: '" oder "--". Der Motor sollte mit langsamer Geschwindigkeit arbeiten, und Sie sollten das grüne Licht sehen.',
    start26: "Bitte drehen Sie das Rad von Hand nach vorne, dann sehen Sie die Geschwindigkeit oben und leuchtet das grüne Licht.",
    start27: "Wenn kein grünes Licht leuch den Bottontet, bitte den Magneten in die richtige Induktionsposition bringen. Wenn es immer noch nicht funktioniert, klicken Sie bitte auf den Botton ",
    start28: "Bitte ersetzen Sie das Sensor und Magnet.",
    start29: "Bitte hängen Sie das Fahrrad auf, um Verletzungen oder Beschädigungen zu vermeiden.",
    start30: "Bitte vergewissern Sie sich, dass das Rad frei drehbar ist!",
    start31: "Bitte drehen Sie das Pedal von Hand nach vorne, dann sollten Sie das grüne Licht sehen.",
    start33: "Drehmomentsensor starten",
    start34: "Bitte drücken Sie den Bremshebel und setzen ca. 10kg auf das Pedal, dann sollten Sie das grüne Licht sehen.",
    start35: "Dann den Pedalsensor austauschen.",
    start36: "< Zurück",
    start37: " Nächste >",

    toolTitle1: "Die erste Version des BESST-Tools",
    toolTitle2: "PC modus",
    tool11: "Die erste Version des BESST-Tools unterstützt nur das Uart Protokoll, mit Mini USB Port auf der einen Seite und zwei USB Typ B Ports auf der anderen Seite. ",
    tool12: ' "Der Modus "PC" dient zum Lesen von Daten aus dem Display im Vergleich zum Arbeitsmodus. Wenn das HMI an das BESST-Tool angeschlossen ist und das Display/HMI eingeschaltet werden konnte, geht es in den Modus "PC" über.',
    tool13: "LCD Display",
    tool14: 'Der Bildschirm zeigt "PC" oder "Connected OK" an, wenn das Display/HMI eingeschaltet ist.',
    tool15: "LED Display",
    tool16: " Das Licht geht auf die höchste Stufe, wenn das HMI eingeschaltet ist.",

    updateTitle: 'Software Version',
    updateHistory: 'Software History',
    update111: "Während Sie die Firmware in die BESST-Box herunterladen, schalten Sie bitte das HMI aus.",
    update1111: "While download firmware to BESST box, please unplug HMI.",
    update112: "Während Sie die Firmware in die BESST-Box herunterladen, schalten Sie bitte, den Kontroller aus.",
    update11: " Wenn die Firmware erfolgreich in die BOX geladen wurde,",
    update12: 'schalten Sie das HMI ein, bis Sie "BEEP" hören. Das HMI schaltet sich dann aus.',
    update122: 'then plug the HMI until you hear "BEEP" and the HMI is off.',
    update13: "Dann ist das Update abgeschlossen.",
    update14: "Bitte drücken Sie den 'RESET' Button auf dem Bildschirm.",
    update15: "Einschalten Sie bitte das Display/HMI.",
    update16: 'dann stecken Sie den Kontroller ein, bis Sie "BEEP" hören.',
    update17: "dann ist das Update abgeschlossen. Ziehen Sie bitter den Kontroller aus.",
    update19: "Schalten Sie den Kontroller wieder ein",
    update20: 'bis Sie "BEEP" hören, dann ist das Update abgeschlossen.',

    error: "Fehler",
    errorDesc: "Es werden keine Daten gespeichert. Es könnte ein Verbindungsabbruch sein. ",
    errorNote: 'Bitte klicken Sie auf "Wieder verbinden", um die Verbindung wiederherzustellen. ',
    errorHmi: 'Seriennummer ist leer. Display/HMI ist bei BESST nicht registriert. Bitte schließen Sie das HMI wieder an oder prüfen Sie, ob es sich im "PC"-Modus befindet.',
    errorController: 'SN ist leer. Der Kontroller ist bei BESST nicht registriert. Bitte schließen Sie den Kontroller wieder an oder im Bereich "Werkzeug" testen.',
    errorBattery: 'SN ist leer. Akku ist bei BESST nicht registriert. Bitte schließen Sie das Akku wieder an oder im Bereich "Werkzeug" testen.',
    errorSensor: "Sn is empty. Sensor is not registered on BESST. Please reconnect or test on Tool section.",

    goTest: "Gehen sie in den Test-Modus.",

    resultTitle: "E-BIKE Diagnosebericht ",
    resultBike: "E-BIKE Modell:",
    print: "Drucken",
    bikeId: "E-Bike ID",
    key: "Code des Schlüssels",
    result: "Ergebnis",
    saveReport: " Bericht speichern",
    offLineTip: 'Im Offline-Modus können Sie das Fahrzeug SN eingeben und den Bericht lokal speichern. Bitte klicken Sie auf "Drucken" oben auf dieser Seite.',
    motorSNTip: "Der eingegebene Motor-SN unterscheidet sich von den Daten des Fahrrads.",
    bikeSNTip1: "Bitte geben Sie die Fahrrad-ID ein.",
    bikeSNTip2: "Fahrrad-SN wurde geändert. Bitte überprüfen Sie das.",

    canStep1: "Bitte warten Sie einen Augenblick...",
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
    brake08: "Brake power off function is not normal",
    light01: "The headlight button is",
    light02: 'Press the headlight button to turn it on and off. Please select "Communication indicator is normal" and "Invalid indicator light". ',
    light022: 'Please check whether the headlight is normally turned on and off, please select "Headlights work normally", otherwise please select "Headlight does not work properly" ',
    light03: 'Check if the vehicle has a headlight configuration, please select with or without, if not, press "Next"',
    light04: 'Communication indicator judgement',
    light05: 'Headlight working status',
    light06: 'If the indicator light does not turn green, or the indicator does not turn gray after it turns green, click "Fail". ',
    light07: 'The indicator is normal',
    light08: 'The indicator is not normal',
    light09: 'The headlight does not work normally',
    light10: 'The headlights are not working properly',
    support02: 'Press the “+/-” button to cycle through the lowest level to the highest level twice , observe whether this window and display are consistent and responsive in time. You need to select “Work normally” or “Work abnormally”. ',
    support03: 'Work normally',
    support04: 'Work abnormally',
    speed01: 'Keep the drive system switched on, observe the vehicle speed detection sensor position, and rotate the exact wheel according to the direction of the vehicle. Observe the speed of this window and the display. Please press "Speed sensor works normally", otherwise "Speed sensor works abnormally. " ',
    speed04: 'The speed sensor works normally',
    speed05: 'The speed sensor works abnormally',
    cadence01: 'Set the level above 1st level, rotate the pedal according to the riding direction, the observation window should display the speed, please keep 20RPM at a constant speed for more than 10 seconds, it will automatically judge that "Cadence sensor works normally", otherwise press "Cadence sensor works abnormally." ',
    cadence03: 'Cadence sensor works abnormally',
    torque01: 'Set the level above 1st gear, quickly rotate the middle pedal according to the riding direction, the observation window will display at least greater than 0.75V, less than 5V, and change with the magnitude of the applied force to normal, the value is greater than 0.75V, the system automatically Make sure the function is qualified, otherwise press "Fail". ',
    torque02: 'If you confirm that the vehicle is using a torque sensor, please select "S/T" and adjust to "T".',
    walk01: 'Hold the display',
    walk02: ' /"-" for three seconds or more, if the motor rotates and the speed is less than 6km/h, the system will automatically judge OK, otherwise please click "WALK communication failure" ',
    walk03: 'Please press',
    walk04: '/"-"  and observation window shows "WALK", otherwise please press "WALK communication failure" ',
    walk05: 'WALK communication failure',
    walk06: 'If the bicycle has a flywheel on the rear wheel, place the chain on the smallest chain ring.',
    motor01: 'In the cadence working mode, if the motor running current is less than 3A, and the automatic evaluation is normal',
    motor02: ' "WALK" mode, the motor running current is less than 3A, and the automatic evaluation is normal',
    motor03: 'If there is still no indication that the motor is working normal, please put the vehicle drive wheel in a damped and rotatable state, start the motor with the pedal, observe the normal value of current, speed and cadence or torque voltage information, the system will utomatically judge "motor works normally", otherwise "Fail" ',

    defectDes: "Problem description",
    defects: "Mechanical defect",
    image: "Image",
    video: "Video",
    videoMaxSize: "Video maximum size is 10M",
    defectLength: "Maximum 600 character including space",

    notice19_00: "Für 7 Tage nicht mehr anzeigen",
    notice19_01: "Erklärung der Aktualisierung",
    notice19_02: "Die Diagnosefunktion wurde am 19.12.2019 aktualisiert. Seitdem passt Diese Diagnosefunktion möglichweise nicht gut zu einigen CAN-Produkten, die vor Oktober 2019 hergestellt wurden.",
    notice19_03: "Weiterhin, aufgrund des Unterschiedes zwischen der Hardware des BESST-Tools gibt es einige Betriebsunterschiede, wenn das BESST-Tool mit der Hardware-Version von BESST.UC 3.0 verwendet wird",
    notice19_04: "BESST wurde schon aktualisiert, um den Unterschiede zwischen Hardware des Service-Tools zu beseitigen.",
    notice19_05: "Hinweis: Wenn Sie kontinuierlich ein CAN-System in der Diagnose stellen, müssen Sie jedes Mal die Stromversorgung des Kontrollers über 20 Sekunden herunterfahren.",

};


const diagnosisNormal = {
    com: "COM",
    select: "Auswählen",
    connect: "Verbinden",
    readAgain: "Erneut Aktualisieren",
    refresh: "Aktualisieren",
    tip: "Tipp",
    saveReport: "Report speichern",
    sendReport: "Report senden",
    selectError: "Fehler auswählen",
    tipContent01: "Bitte prüfen Sie die folgenden grundlegenden Informationen.",
    tipContent02: "Bitte prüfen sie die ID auf Ihren E-Bike",
    title: {
        name: "Name",
        sn: "Seriennummer",
        diagnosisResult: "Diagnose Ergebnis",
        mechanicalFailure: "Mechanisches Versagen",
        serviceBtn: "Service",
    },
    bike: "E-Bike",
    sn: "Seriennummer",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    solution: "Lösung",
    hmiDefectOptions: {
        displayNotWork: "Display funktioniert nicht",
        displayWaterInside: "Wasser im Display",
        displayBtnDamage: "Display Knopf beschädigt",
        displayCableDamage: "Display Kabel beschädigt",
        displayScreenDamage: "Display Bilschirm beschädigt",
        displayBracketDamage: "Display Halterung beschädigt",
        displayScrewLoosen: "Displayschraube hat sich gelöst",
        displayBlackScreen: "Display Schwarzer Bildschirm",
        displayMessyCode: "Display zeigt nicht deffinierbaren Code an.",
    },
    controllerDefectOptions: {
        noAssistance: "Keine Unterstützung",
        noLightFunc: "Keine Licht Funktion",
        brakeCannotCutOffPower: "Keine Funktion Bremsgriffschalter",
        tripDistanceShort: "Fahrstrecke zu kurz",
    },
    batteryDefectOptions: {
        batteryDamage: "Batterie beschädigt",
        chargerNotWork: "Ladegerät defekt",
        BatteryCannotCharge: "Batterie kann nicht aufgeladen werden",
    },
    motorDefectOptions: {
        motorNotWork: "Der Motor funktioniert nicht, aber die Anzeige funktioniert normal ",
        motorShake: "Motor vebriert",
        motorNoisy: "Motor Geräusche",
        motorLessPower: "Motor zu wenig Leistung",
        motorRunIntermittently: "Motor arbeitet nur Zeitweise",
        noFunction: "Keine 6 Km/h Funktion",
    },
    defectOptions: {
        motorNotWork: "Der Motor funktioniert nicht",
        motorNoisy: " Motor Geräusche",
        motorDamage: "Der Motor hat einen Schaden",
        hmiFailed: "Das Display fehlgeschlagen",
        hmiErrorCode: "Das Display hat einen Fehlercode",
        hmiDamage: "Das Display hat einen Schaden",
        disorderedIndication: "Ungeordnete Anzeige",
        outOfControl: "Außer Kontrolle",
        eBrakeFailed: "E-Brake fehlgeschlagen",
        eBrakeDamage: "E-Brake hat einen Schaden",
        lightFailed: "Beleuchtungsfehler",
        walkAssistanceFailed: "Schiebehilfe ausgefallen",
        throttleFailed: "Beschleunigungsgriff ausgefallen",
        throttleDamage: "Beschleunigungsgriff defekt",
        shortRange: "Kurze Rechweite",
        cableDamage: "Kabelschaden",
        batteryProblem: "Akku Problem",
        batteryDamage: "Akkuschaden",
    },
    noDefect: "Kein defekt",
    read: "Lesen",
    test: "TEST",
    errorSolutionDialog: {
        title: "Fehler Lösung",
        name: "Name",
        remark: "Bemerkung",
        description: "Beschreibung",
        target: "Ziel",
        solution: "Lösung",
    },
    connectTipVideoDialog: {
        title: "TIPP Vidio ansehen",
        tip: "Ihr Broser unterstützt kein Video."
    },
    connectTipDialog: {
        title: "TIPP Vidio ansehen",
        tip: "Ihr Broser unterstützt kein Video.",
        content: "Bitte schließen sie das BESST Tool an das Display und das EB-BUS an. Schalten Sie das Display an: Das Display zeigt folgendes an: \"PC\"(LCD type)oder der Top Modus LED Licht ( LED type)",
    },
    confirmSnDialog: {
        title: "Bestätigung SN",
        content: "Bitte überprüfen Sie mit dem SN auf dem Fahrzeug, um festzustellen, ob es richtig ist."
    },
    print: "Drucken",
    bikeInfo: "Bike Info",
    diagnosis: {
        result: "Diagnose Ergebnis",
        hmi: " Display",
        battery: "Akku",
        controller: "Kontroller",
        rpmSensor: "Drehzahlsensor",
        pedalSensor: "Pedalsensor",
        motor: "Motor",
    },
    emptySNTip: "Fehlende Fahrzeug Seriennummer",
    emptySNText: "Fahrzeug Seriennummer ist leer, und der Seriennummer wird im Bericht angezeigt, fahren Sie mit der Testansicht fort.",
    back: "Zurück",
    storeInfo: "E-Bike Händlerinformation",

    preTipA: "HMI einschalten, Box wird zweimal piepen, dann Klick.",
    continue: "Weiter",
    preTipB: "Wenn kein Beeb dann bitte klicken",
    here: "Mehr",

};

const diagnosisTest = {
    back: "Zurück",
    driveSystemTest: "Antriebssystem Test",
    hmiMode: "Unterstützungslevel",
    currentMode: "Aktuelles Level",
    light: "Licht",
    title: {
        name: "Name",
        sn: "Seriennummer",
        diagnosisResult: "Diagnose Ergebnis",
        mechanicalFailure: "Mechanisches Versagen",
    },
    help: "Hilfe",
    save: "Speichern",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    hmiDefectOptions: {
        displayNotWork: "Das Display arbeitet nicht",
        displayWaterInside: "Wasser im Display",
        displayBtnDamage: "Displayknopf ist defekt",
        displayCableDamage: "Displaykabel ist defekt",
        displayScreenDamage: " Display ist beschädigt",
        displayBracketDamage: "Displayhalter ist beschädigt",
        displayScrewLoosen: "Display Schraube hat sich gelockert",
        displayBlackScreen: "Display Schwarzer Bilschirm",
        displayMessyCode: "Display zeigt nicht deffinierbaren Code an.",
    },
    controllerDefectOptions: {
        noAssistance: "Keine Unterstützung",
        noLightFunc: "Keine Licht Funktion",
        brakeCannotCutOffPower: "Keine Funktion Bremsgriffschalter",
        tripDistanceShort: "Fahrstrecke zu kurz",
    },
    batteryDefectOptions: {
        batteryDamage: "Batterie beschädigt",
        chargerNotWork: "Ladegerät defekt",
        BatteryCannotCharge: "Batterie kann nicht aufgeladen warden",
    },
    motorDefectOptions: {
        motorNotWork: "Der Motor funktioniert nicht, aber die Anzeige funktioniert normal ",
        motorShake: "Motor vebriert",
        motorNoisy: "Motor Geräusche",
        motorLessPower: "Motor zu wenig Leistung",
        motorRunIntermittently: "Motor arbeitet nur Zeitweise",
        noFunction: "Keine 6 Km/h Funktion",
    },
    defectOptions: {
        motorNotWork: "Der Motor funktioniert nicht",
        motorNoisy: "Motor Geräusche",
        motorDamage: "Der Motor hat einen Schaden",
        hmiFailed: "Das Display fehlgeschlagen",
        hmiErrorCode: "Das Display hat einen Fehlercode ",
        hmiDamage: "Das Display hat einen Schaden",
        disorderedIndication: "Ungeordnete Anzeige",
        outOfControl: "Außer Kontrolle",
        eBrakeFailed: "E-Brake fehlgeschlagen",
        eBrakeDamage: "E-Brake hat einenSchaden",
        lightFailed: "Beleuchtungsfehler",
        walkAssistanceFailed: "Schiebehilfe ausgefallen",
        throttleFailed: "Beschleunigungsgriff ausgefallen",
        throttleDamage: " Beschleunigungsgriff defekt",
        shortRange: "Kurze Rechweite",
        cableDamage: "Kabelschaden",
        batteryProblem: "Akku Problem",
        batteryDamage: "Akkuschaden",
    },
    noDefect: "Kein defekt",
    selectError: "Fehler auswählen",
    errorSolutionDialog: {
        title: "Fehler Lösung",
        name: "Name",
        remark: "Bemerkung",
        description: "Beschreibung",
        target: "Ziel",
        solution: "Lösung",
    },
};


const reportList = {
    tableHead: {
        vehicleSn: "Fahrzeug SN",
        modelName: "Modellname",
        time: "Zeit",
        operation: "Vorgang"
    },
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    search: {
        vehicleSn: 'Fahrzeug SN',
        searchBtn: "Suchen"
    },
    deleteDialog: {
        title: "Bericht löschen",
        content: "Sind Sie sicher, ihn zu löschen?"
    },
    delete: "Löschen",
    yes: "Ja",
    no: "Nein"
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
    back: "Zurück",
    add: 'Hinzufügen',
    reset: "Zurücksetzen",
    delete: "Löschen",
    tableHead: {
        account: "Konto",
        createTime: "Erstellungszeit",
        operation: "Vorgang",
    },
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    yes: "Ja",
    no: "Nein",
    addAccountDialog: {
        title: "Konto hinzufügen",
        email: "E-mail",
        password: "Passwort"
    },
    deleteDialog: {
        title: "Konto löschen",
        content: "Sind Sie sicher, das Konto zu löschen?"
    },
    resetPasswordDialog: {
        title: "Passwort zurücksetzen",
        content: "Möchten Sie das Kennwort für dieses Konto auf 123456 festlegen?"
    }
};

const centerTicketList = {
    serviceCenter: "Service Center",
    category: "Kategorie",
    status: "Status",
    look: "Look",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Zielseriennummer",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang"
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fahrzeug",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Akku",
        light: "Licht",
        controller: "Kontroller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        pendingAssign: "Ausstehend Zuweisen",
        processing: "In Bearbeitung",
        complete: "Fertiggestellt"
    }
};

const myTicketList = {
    category: "Kategorie",
    status: "Status",
    look: "Schauen",
    tableHead: {
        serviceNo: "Service Nr.",
        category: "Kategorie",
        title: "Titel",
        targetSn: "Zielseriennummer",
        createTime: "Erstellungszeit",
        status: "Status",
        operation: "Vorgang",
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fahrzeug",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Akku",
        light: "Licht",
        controller: "Kontroller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        processing: "Processing",
        complete: "Fertiggestellt",
        handleToAnother: "Wechsel zu einen anderen",
    },
    noResult: "Kein Ergebnis！",
    total: "Total",
    addTicketDialog: {
        dialogTitle: "Service-Ticket hinzufügen",
        category: "Kategorie",
        title: "Titel",
        description: "Beschreibung",
        bike: "E-Bike",
        sn: "Seriennummer",
        serviceCenter: "Service Center",
        image: "Bild",
        video: "Video",
        clickToUpload: "Klicken zum Hochladen"
    },
    confirm: "Bestätigen"

};

const serviceCenterList = {
    serviceCenter: "Servicezentren",
    add: "Hinzufügen",
    edit: "Bearbeiten",
    account: "Konto",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    yes: "Ja",
    no: "Nein",
    tableHead: {
        logo: "Logo",
        name: "Name",
        phone: "Telefon",
        email: "E-mail",
        country: "Land",
        address: "Adresse",
        operation: "Vorgang",
    },
    deleteDialog: {
        title: "Servicezentrum löschen",
        content: "Sind Sie sicher, es zu löschen?"
    },
    createSC: "Servicezentrum erstellen",
    editSC: "Servicezentrum bearbeiten",
    editDialog: {
        picture: "Bild",
        tip: "Tipp",
        content01: "Bitte laden sie ein qudratisches Bild hoch",
        content02: "Unterstüzende Formate: jpg,png und 300*400*400, bis zu 3MB.",
        name: "Name",
        email: "E-mail",
        phone: "Telefon",
        website: "Webseite",
        address: "Adresse",
        postcode: "Postleitzahl",
    },
    detailDialog: {
        title: "Detail",
        picture: "Bild",
        name: "Name",
        email: "E-mail",
        phone: "Telefon",
        website: "Webseite",
        address: "Adresse",
        postcode: "Postleitzahl",
        ticketCount: "Ticket Count",
        aveHandleTime: "Ave Zugriffszeit",
    },
    op: Operation,
};

const ticketDetail = {
    back: "Zurück",
    handleUp: "Nach oben befördern",
    history: "Verlauf",
    ticketInfo: {
        infoTitle: "Reklamationsdetail",
        ticketNo: "Ticket Nr.",
        createTime: "Erstellungszeit",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Titel",
        desc: "Beschreibung",
        image: 'Bild',
        video: "Video"
    },
    user: {
        address: "Adresse",
        website: "Webseite",
        email: "E-mail",
    },
    replay: {
        content: "Inhalt",
        image: "Bild",
        sendMessage: "Nachricht senden"
    },
    confirm: "Bestätigen",
    cancel: "Stornieren",
    handleUpTicketDialog: {
        title: "Handle Up Ticket",
        serviceCenter: "Service Center"
    },
    historyDialog: {
        title: "Ticket Verlauf",
        name: "Name",
        operation: "Vorgang",
        time: "Zeit"
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
    order: "Auftrag",
    vehicle: "Fahrzeug"
};

const AssemblerOrderDetail = {
    back: "Zurück",
    orderDetail: "Bestellinformation",
    edit: "Bearbeiten",
    delete: "Löschen",
    orderInfo: "Auftragsinformation",
    orderNo: "Auftrag Nr.",
    customerName: "Kundenname",
    note: "Notizen",
    poNo: "Bestellnummer",
    itemsList: "Artikelliste",
    produce: "Herstellen",
    vehicleList: "Fahrzeugliste",
    deleteOrder: 'Den Auftrag löschen',
    deleteTip: "Sind Sie sicher, ihn zu löschen?",
    yes: "Ja",
    no: "Nein",
    tableHead: {
        modelName: "Modellname",
        partNo: "Artikelnummer",
        quantity: "Menge",
        operation: "Vorgang",
    },
    replace: "Ersetzen",
    confirmDialog: {
        replace: "ersetzen",
        yes: 'Ja',
        no: 'Nein'
    },
    itemHead: {
        image: "Bild",
        name: "Name",
        partNo: "Artikelnummer"
    },
    search: "Suchen",
    brandPlaceholder: "Marke auswählen",
    operation: "Vorgang",
    modelName: "Modellname",
};

const AssemblerOrderList = {
    add: "Hinzufügen",
    noResult: "Kein Ergebnis！",
    vehicleList: "Fahrzeugliste",
    detail: "Detail",
    total: "Gesamt",
    tableHead: {
        date: "Datum",
        orderNo: "Auftrag Nr.",
        poNo: "Bestellnummer",
        customerName: "Kundenname",
        model: "Modell",
        produced_qty: "Hergestellt/Quantität",
        status: "Status",
        operation: "Vorgang",
    },
    poNo: "Bestellnummer",
    search: "Suchen",
    startDate: "Wählen Sie das Startdatum",
    endDate: "Wählen Sie das Enddatum",
    model: 'Modell',
};

const AssemblerOrderProduce = {
    back: "Zurück",
    orderInfo: "Auftragsinformation",
    brand: "Marke",
    model: "Modell",
    poNo: "Bestellnummer",
    produced_qty: "Hergestellt/Menge",
    pleaseSelect: "Bitte wählen Sie",
    reconnect: "Wieder verbinden",
    openConnectTip: "Verbindungstipp offnen",
    partName: "Name",
    sn: "Seriennummer",
    hw: "HW",
    sw: "SW",
    connectTipVideo: "Mit dem Video-Tipp verbinden",
    connectTip: "Mit dem Tipp verbinden",
    connectTipText: "Bitte verbinden Sie BEBitte verbinden Sie BESST mit HMI und EB-BUS und schalten Sie das HMI ein, und überprüfen Sie dann, ob das HMI 'PC' (LCD-Typ) oder die Top Mode LED-Licht(LED-Typ) anzeigt.SST mit HMI und EB-BUS und schalten Sie das HMI ein, und überprüfen Sie dann, ob das HMI 'PC' (LCD-Typ) oder die Top Mode LED-Licht(LED-Typ) anzeigt.",
    confirm: "Bestätigen",
    itemFinishTip: "Dieses Modell ist fertig",
    selectTip: "Bitte wählen Sie com.",
    tip: 'Tipp',
    batteryBindTip: "Möchten Sie die Batterie binden?",
    yes: "Ja",
    no: "Nein",
    goDiagnosisTip: "Möchten Sie in die Diagnoseansicht wechseln?",
    bindBattery: "Batterie binden",
    vehicleSn: "Fahrzeug SN",
    componentSn: "Komponente SN",
    videoTip: "Ihr Browser unterstützt kein Video Format.",
    error: "Fehler",
    historyDialog: {
        title: "Software-Historie",
        version: "Version",
        updateTime: "Update-Zeit",
        remark: "Bemerkung",
        operation: "Vorgang",
        noResult: "Kein Ergebnis！"
    },
    update: "Update",
    reset: "Zurücksetzen",

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

    cNam: AttributeName
};

const AssemblerOrderSave = {
    back: "Zurück",
    createOrder: "Auftrag erstellen",
    editOrder: "Auftrag bearbeiten",
    orderInfo: "Auftragsseriennummer",
    poNo: "Bestellnummer",
    customerInfo: "Kundeninforamtion",
    customerName: "Kundenname",
    note: "Notizen",
    itemList: "Artikelliste",
    addItem: "Element hinzufügen",
    delete: "Löschen",
    modelName: "Modellname",
    addItems: "Elemente hinzufügen",
    saveOrder: "Den Auftrag speichern",
    tableHead: {
        name: "Name",
        partNo: "Artikelnummer",
        quantity: "Menge",
        operation: "Vorgang",
    },
    itemHead: {
        image: "Bild",
        name: "Name",
        partNo: "Artikelnummer"
    },
    search: "Suchen",
    brandPlaceholder: "Marke auswählen"
};

const AssemblerPartnerList = {
    back: "Zurück",
    acceptInvitation: "Einladung annehmen",
    currentBrand: "Aktuelle Marke",
    noResult: "Kein Ergebnis！",
    total: "Gesamt",
    brandInfo: "Markeninfo",
    logo: "Logo",
    name: "Name",
    type: "Typ",
    brand: "Marke",
    email: "E-mail",
    website: "Webseite",
    tel: "Telefon",
    postcode: "Postleitzahl",
    address: "Adresse",
    invitationCode: "Einladungscode",
    confirm: "Bestätigen",
    cancel: "Stornieren",
    hoverTip: "Als aktuelle Marke einstellen",
    tableHead: {
        name: "name",
        phone: "Telefon",
        website: "Webseite",
        country: "Land",
        operation: "Vorgang",
    },
    codeEmptyTip: "Bitte Code eingeben ",
    acceptSuccess: "Erfolgreich akzeptieren"
};

const AssemblerVehicleDetail = {
    vd: VehicleDetail,
};

const AssemblerVehicleList = {
    brand: "Marke",
    poNo: "Bestellnummer",
    modelName: "Modellname",
    bomSn: 'Komponente SN',
    search: "Suchen",
    export: "Exportieren",
    delete: "Löschen",
    detail: "Detail",
    total: "Gesamt",
    deleteVehicle: "Das Fahrzeug löschen",
    deleteTip: "Sind Sie sicher, es zu löschen?",
    yes: "Ja",
    no: "Nein",
    back: "Zurück",
    tableHead: {
        vehicleSn: "Fahrzeug SN",
        brandLogo: "Markenlogo",
        brandName: "Markenname",
        modelName: "Modellname",
        poNo: "Bestellnummer",
        produceDate: "Herstellungsdatum",
        saleDate: "Verkaufsdatum",
        operation: "Vorgang",
        noSale: "Unverkauft"
    },
    noResult: "Kein Ergebnis！",
    deleteSuccess: "Erfolgreich löschen",
    noSale: "Unverkauft",
    exportDialog: {
        title: "Fahrzeug exportieren",
        startNum: "Startnummer",
        endNum: "Endnummer",
        export: "Exportieren",
        cancel: "Stornieren"
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
// ./src/assets/lang/de.js