// 登录页面相关
const Login = {
    declare: "Opmerking",
    readmore: "verder lezen",
    declareP1: "BESST is beheer software die wordt gebruikt voor elektrisch aangedreven systemen die voor de merken zijn ontwikkeld. De merken sturen uitnodigingscodes naar zijn dealers, fabrieken en distributeurs die vervolgens hun eigen account en wachtwoord registreren. Elke rol moet ervoor zorgen dat de geregistreerde accounts en wachtwoorden niet worden gedeeld met derden en de merken zijn verantwoordelijk voor het trainen van de uitgenodigde relaties.",
    declareP2: "Opmerking: alle gebruikers moeten de BESST TOOL van het merk kopen om alle functies van de software te realiseren.",
    declareP3: "Het merk",
    declareP6: "1. Het merk is geautoriseerd door de software-eigenaar:",
    declareP7: " Het merk dient een schriftelijke aanvraag in bij de software-eigenaar, Inclusief: de geregistreerde naam van het merk, land, adres, postcode, wettelijk e-mailadres van de managementaccount, merkeigendom certificaat, domeinnaam website, etc.",
    declareP8: " Software-eigenaar bevestigen de authenticiteit en wettigheid van informatie;",
    declareP9: " Software-eigenaren stellen accountinformatie vast via de software-achtergrond en sturen autorisatie-informatie per mail;",
    declareP10: "Na het downloaden van de software via autorisatie-informatie, moeten merkbedrijven de relevante informatie nauwkeurig invullen volgens het softwareregistratie  proces om het registratieproces te verbeteren;",
    declareP11: 'Ga naar "INSTELLING" om bedrijfsinformatie in te stellen en het oorspronkelijke wachtwoord te wijzigen.',
    declareP12: 'Ga naar "merkenlijst" en voeg de merkenlijst van uw eigen bedrijf toe. ',
    declareP13: 'Ga naar "Modellijst" om de modelinformatie toe te voegen.',
    declareP14: `Ga naar "Dealer" en verzend de uitnodigingscode volgens het e-mailadres van de Dealer.`,
    declareP15: 'Ga naar "OEM" en verzend de uitnodigingscode volgens het OEM-e-mailadres.',
    declareP16: `Voer via "account", in het hoofdaccount, accounts in voor werknemers.`,
    declareP17: "De BESST software download link:",
    declareP18: "Dealers en distributeurs",
    declareP19: "3. Dealers en distributeurs zijn geautoriseerd door het merk:",
    declareP20: 'Dealers en distributeurs vragen het merk om de "uitnodigingscode. ";',
    declareP21: 'Vul voor nieuwe geregistreerde gebruikers de "uitnodigingscode" in bij de registratie-interface "Aanmelden" om het e-mailadres als accountnaam te verifiëren;',
    declareP22: " Stel het wachtwoord in nadat u de verificatiecode via E-mail heeft ontvangen;",
    declareP23: `Voor gebruikers van het oude BESST-systeem, ga naar "Merk" en vul "Uitnodiging accepteren" in met de "uitnodigingscode" en accepteer het nieuwe merk.`,
    declareP24: "Nadat het wachtwoord met succes is ingesteld, begint u de informatie van dealer of distributeur in te vullen in overeenstemming met het softwareproces, bevestigt u de authenticiteit en geldigheid van de informatie en voltooid u de registratie.",
    declareP25: 'Vul de volgende keer dat u inlogt uw e-mailadres en wachtwoord in de accountbalk. Het is beter om "Onthoud mij" te markeren, zodat u de volgende keer gemakkelijk kunt inloggen;',
    declareP26: '"Lijst met merken" : na de registratie van de uitnodigingscode verzamelt en laadt het systeem automatisch de informatie van het merk.',
    declareP27: 'Ga naar "Type fiets" en krijg automatische toegang tot alle merken met productinformatie.',
    declareP28: 'Ga naar "Fietsverkoop" en registreer klantinformatie, bevestig voertuig-ID, genereer de verkooporder, bepaal het after-sales beleid, vergeet niet de gegevens die op de server zijn opgeslagen te bevestigen.',
    declareP29: "De fabriek",
    declareP30: "2. De fabriek is geautoriseerd door het merk:",
    declareP31: 'De fabriek moet het merk om de "uitnodigingscode vragen."',
    declareP32: "Nadat het wachtwoord met succes is ingesteld, begint u de fabrieksinformatie in te vullen volgens het softwareproces, bevestigt u de authenticiteit en geldigheid van de informatie en voltooid u de registratie. ",
    declareP33: ' "Merk": nadat de uitnodigingscode is geregistreerd wordt de informatie  van de fiets modellen automatisch verzameld. ',
    declareP34: ' "Order" en "Voertuig" verzamelen bedrijfsinformatie van het merk via uitnodigingscodes. Bij het maken van een nieuwe verkooporder moet eerst het merk worden geselecteerd en vervolgens het voertuigtype.',

    chooseLanguage: "Taal",
    chinese: "中文",
    deutsch: "Deutsch",
    english: "English",
    dutch: "Nederlands",
    login: "Inloggen",
    userNamePlaceholder: "Gebruikersnaam",
    passwordPlaceholder: "Wachtwoord",
    rememberMe: "Onthoud mijn inlog gegevens",
    forgetPassword: "Wachtwoord vergeten?",
    loginBtnTxt: "Inloggen",
    signUp: "Aanmelden",
    offLine: "Offline",
    inputUser: "Please input username",
    inputPassword: "Please input password",
    accountProblem: "The account has some problems. Please contact the administrator besst@bafang-e.com",
    limitedIP: 'IP verification is enabled for this account. IP verification failed, please login in regular location.',
    emptyIP: 'IP verification is enabled for this account. Please wait or try later.',
    emptyIPThree: 'IP verification is enabled, please contact the administrator.',
};

// 注册页面相关
const RegisterAccount = {
    login: "Inloggen",
    verifyCode: "Code verifiëren",
    createAccount: "Account aanmaken",
    createCompany: "Bedrijf aanmaken",
    company: "Bedrijf",
    email: "E-mail",
    emailValidationCode: "E-mail Validatiecode",
    password: "Wachtwoord",
    confirmPassword: "Wachtwoord bevestigen",
    next: "Volgende",
    verify: "Verifiëren",
};

const RegisterCompany = {
    login: "Inloggen",
    verifyCode: "Code verifiëren",
    createAccount: "Account aanmaken",
    createCompany: "Bedrijf aanmaken",
    yourRole: "Functie",
    logo: "Logo",
    uploadImgTip: " U kunt het beste een vierkante afbeelding uploaden met achtergrond, ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB.",
    contact: "Contact",
    companyName: "Bedrijfs naam",
    mailAddress: "Email adres",
    phoneNumber: "Telefoonnummer",
    website: "Website",
    postalAddress: "Postadres",
    post: "Postcode",
    address: "Adres",
    save: "Opslaan",
    country: "Land",
};

const RegisterInvitation = {
    login: "Inloggen",
    verifyCode: "Code verifiëren",
    createAccount: "Account aanmaken",
    createCompany: "Bedrijf aanmaken",
    invitationCode: "Uitnodigings code",
    typeTheCodeTip: " Voer de code in die u hebt verkregen van het merk.",
    tryAgainTip: " Als u de code heeft gebruikt maar niet succesvol hebt geregistreerd, kun je het opnieuw proberen.",
    verify: "Verifiëren",
    noCompanyTip: "Het account zonder bedrijfsinformatie.",
    reCodeTip: "Voer de uitnodigingscode in die is gebruikt voor het maken van dit account om een bedrijf te maken.",

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
    login: "Inloggen",
    email: "E-mail",
    verify: "Controleren",
    emailValidationCode: "E-mail Validatiescode",
    password: "Wachtwoord",
    confirmPassword: "Wachtwoord bevestigen",
    resetPassword: "Wachtwoord herstellen",
    emptyEmailTip: "Emailadres ingeven.",
    errorEmailTip: "Deze E-mail is niet geautoriseerd.",
    emptyCodeTip: "Graag code invoeren",
    emptyPasswordTip: "Graag uw wachtwoord invoeren",
    emptyConfirmPasswordTip: "Graag uw wachtwoord bevestigen",
    passwordTip: "De wachtwoorden komen niet overeen.",
    resetPasswordSuccess: "Opnieuw instellen wachtwoord is succesvol verlopen",
    codeHasBeenSent: "De code is verstuurd",
    passwordFormatTip: " Wachtwoord mag alleen letters en cijfers bevatten, 8 tot 16 tekens lang, hoofdlettergevoelig.",
    filterUserEmail: "Het e-mailadres van de gebruiker is niet geregistreerd in BESST. Controleer of het e-mailadres correct is."
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
    brand: "Merk",
    model: "Model",
    vehicle: "Voertuig",
    dealer: "Dealer",
    dealerList: "Dealerlijst",
    invitation: "Uitnodigen",
    oem: "OEM",
    oemList: "OEM lijst",
    componentList: "Component Bedrijf",
    serviceCenter: "Service Center",
    serviceCenters: "Service Centers",
    complaints: "Klachten",
    tasks: "Taken",
    account: "Account",
    order: "Order",
    diagnosis: "Diagnose",
    report: "Bericht",
    errorCode: "Foutcode",
    tool: "Gereedschap",
    hmi: "Display",
    controller: "Controller",
    battery: "Batterij",
    sensor: "Sensor",
    update: "Update",
    auth: "Machtiging",
    product: "Product",
    component: "Onderdeel",
    centerTicket: "Ticket center ",
    myTicket: "Mijn Ticket",
    help: "Help",
    setting: "Instellen",
    logout: "Uitloggen",
    statistics: "Statistieken",

    record: "Registreren",
    testFeedback: "Feedback Center",
    material: "Production Kanban",
    admin: "Beheer",
    bikeModel: "Type Fiets",
    bikeSales: "Fiets Verkoop",
    bikeInfo: "Fiets Informatie",
    author: "Authorisatie",
    mechanic: "Machanisch Defect",
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
    controller: "Controller",
    hmi: "Display",
    battery: "Batterij",
    motor: "Motor",
    sensor: "Sensor",
    key: "Key",
    sn: "Serie Nummer",
    charger: "Lader",
    other: "Andere",
    bike: "Fiets",

    model: "Model",
    softwareVersion: "Software Versie",
    hardwareVersion: "Hardware Versie",
    tryAccountsTip: "Dit account is voor testdoeleinden. Sommige bewerkingen zijn beperkt.",

    bootLoad: "Bootloader Ver.",
    errorCode: "Error Code",
    totalMileage: "Totaal afstand",
    clientCode: "Klant Nr.",
    wheelDiameter: "Wiel diameter",
    speedLimit: "Snelheidslimiet",
    totalGear: "Algemeen schema",
    serviceMileage: "Onderhoudswaarschuwing",
    setTotalMileage: "Totale kilometerstand instellen",
    setSingleMileage: "Dag afstand instellen",
    setTimeZone: "Tijdzone instellen",
    time: "Tijd",
    keyCard: "Sleutelkaart",
    circumference: "Omtrek",
    singleMileage: "Dag afstand",
    speedMax: "Max Snelheid",
    speedAvg: "Gemiddelde snelheid",
    hmiMode: "Modus",
    hmiLevel: "Huidige ondersteunings stand",
    hmiBoost: "BOOST modus",
    hmiBoostIn: "In BOOST",
    hmiBoostOut: "Niet in BOOST",
    hmiTurnOffTime: "Uitschakel tijd",
    hmiNoTurnOff: "Schakelt niet uit",
    hmiLight: "Verlichting",
    hmiLightOn: "Verlichting ingeschakeld",
    hmiLightOff: "Verlichting uitgeschakeld",
    hmiButton: "status + knop",
    hmiBtnPress: "Ingedrukt",
    hmiBtnPressNot: "Niet ingedrukt",
    hmiSingleMileageClear: "Verwijder Dagafstand",
    hmiClean: "Verwijderd",
    hmiNotClean: "Niet verwijderd",

    spdMeterType: "Snelheidsmeter type",
    manName: "Fabrikant",
    nmlVolt: "Nominale Spanning",
    maxCur: "Max Stroom",
    spdMeterSignalNum: "Snelheids signaal",
    limitVolt: "Laagspanning beveiliging ",
    limitCurrent: "Begrenzing Stroom",
    limitSpd: "Snelheidslimiet ",
    assit: "Ondersteuning",
    positionSensor: "Kalibratie van de positiesensor",
    torqueSenCali: "Kalibratie van de torque sensor",
    conTemperature: "Controller temperatuur",
    motorTemperature: "Motor temperatuur",
    calorie: "Calorie",
    assistStatus: "Ondersteunings Status",
    assistOn: "Ingeschakeld",
    assistOff: "Uitgeschakeld",
    conTorque: "Torque Voltage",
    conCadence: "Cadans",
    conSingleTrip: "Dagafstand",
    conSingleTripLeft: "Actieradius",
    conSpeed: "Snelheid",
    conCurrent: "Stroom",
    conVoltage: "Voltage",

    ASOC: "ASOC",
    packVoltage: "Batterijspanning",
    fullChargeCapacity: "Capaciteit volledig geladen",
    current: "Stroom",
    cell8_14: "Cel 8-14",
    currentChargeInterval: "Actuele laadinterval",
    RSOC: "RSOC",
    capacityLeft: "Rest capaciteit",
    temperature: "Temperatuur",
    cell1_7: "Cel 1-7",
    cycleLife: " Aantal cycli ",
    maxChargeInterval: "Max laadinterval",
    produceDate: "Productie Datum",
    SOH: "SOH",
    useStage: "Onderhoudsfase",
    result: "Resultaat",
    afterSalesTitle: "Onderhoudsfase status controle ",

    power: "Power",
    voltage: "Voltage",
    wheelSize: "Wielomtrek",
    rpm: "RPM",

    torque: "Torque waarde",
    cadence: "Cadans",
    sensorTip: "Hoofdgegevens zijn leeg. Controleer de sensor aansluiting.",

    component: "Onderdeel",
    com: "COM",
    write: "Schrijven",
    connect: "Verbinden",
    read: "Lezen",
    reset: "Reset",
    set: "Set",
    update: "Update",
    test: "Test",
    clear: "Verwijderen",
    start: "Start",
    stop: "Stop",
    cancel: "Annuleren",
    confirm: "Bevestigen",
    select: "Selecteren",
    restart: "Herstarten",
    save: "Opslaan",
    saveSuccess: "Succesvol opgeslagen",
    writeSuccess: "Succelvol overschreven",
    selectComTip: "Selecteer Com poort",
    comDisconnect: "De verbinding is verbroken",
    noComConnect: "Graag verbinden met juiste COM poort",
    writeFail: "Error tijdens overschrijven. Probeer het opnieuw.",
    snEmptyTip: "Voer het serienummer in",
    note: "Opmerking",
    notice: "Opmerking",
    message: "Bericht",
    success: "Succesvol",
    operationFail: "Operatie mislukt.",
    speedLimitNote: " Opmerking: de snelheidslimiet is hoger dan 25 km / u. Pas de snelheidslimiet aan binnen de limieten voorgeschreven door de lokale wetgeving.",
    hmiUART: " Het huidige aandrijfsysteem wordt beoordeeld als UART-communicatie.",
    hmiUARTText1: " In het geval dat het display in pc-modus of normaal werkende modus is, moet u het display eerst handmatig uitschakelen ",
    hmiUARTText2: " Druk op de BESST TOOL MODE-knop om te resetten ",
    hmiUARTText3: " Druk op de aan / uit-knop van het display om het scherm in te schakelen en naar de pc-modus te gaan ",
    hmiUARTText4: " Druk op de knop Lezen om de parameters te lezen. Als de informatie onvolledig is, lees dan opnieuw",

    mainData: " Hoofdgegevens",
    parameter: " Parameter Inhoud",
    setting: " Instelling ",
    operationTip: "Tip",
    txtTitle1: "Gebruik",
    txtTitleHid: "De computer zal detecteren wanneer Besst box is aangesloten",
    txtTitleCom: "De computer detecteerd automatisch wanneer er een BESST tool wordt aangesloten.",
    txtTitleCon: " U dient de juiste COM poort te selecteren om een connectie te maken tussen BESST en uw PC. De USB HID zal automatisch verbinden",
    txtTitleRead: "U kunt de basis informatie uitlezen.",
    txtTitleWrt: "U kunt parameters van onderdelen overschrijven.",
    txtTitleReset: "U reset de verbindingsstatus tussen BESST en uw PC. Deze functie is hetzelfde als de resetknop op het BESST-tool. Ook worden alle bestaande gegevens gewist.",
    txtTitleTest: "U kunt controleren of het display de diagnose ondersteunt. Verbind het display met de BEEST-tool en zet het display aan, deze zal in de abnormale modus gaan afhankelijk van verschillende types. Klik nu op test, deze springt naar de normale modus als u ook de controller aansluit.",
    txtTitleUpt: "U kunt de software naar het betreffende onderdeel uploaden.",
    txtTitleClear: "U kunt de informatie verwijderen.",
    txtTitle2: "Opmerking",
    txtNote1: "Deze tool kan controleren of de componenten BESST ondersteunen. Het kan de basisinformatie van component weergeven.",
    txtNote2: " De gegevens moeten eerst worden gelezen voordat u gaat overschrijven. We registreren elke operatie samen met het SN voor eventuele verdere ondersteuning. ",
    txtNote3: " De gegevens moeten eerst worden gelezen voordat u het update. We registreren elke operatie samen met het SN voor eventuele verdere ondersteuning.",
    updateGetFileSucc: "Het bestand is succesvol gedownload.",
    updateStarting: "Update starten... ",
    resetTip: "De BESST tool is gereset. Vergeet niet de HMI uit te schakelen en weer in te schakelen.",

    offlineNo: "Dealer en assemblage kunnen de snelheid niet vergroten.",
    needReadSn: "Sn is leeg. Lees dit eerst om het SN te krijgen",
    offlineCon: "Graag online",
    offlineHandle: " Gegevensverwerking, probeer het later opnieuw.",

    updateSuccessTip: "U heeft de Software met succes naar BOX gedownload, werk dan de apparaten bij zoals beschreven in de instructie.",
    updateFailTip: "De servicetool kan de firmware niet verifiëren en er is een transmissiefout opgetreden. Download de firmware opnieuw",
    updateToolReconnectTip: 'Ontkoppel de servicetool van uw PC en sluit deze opnieuw aan. Wanneer de USB HID-statusstip groen wordt, klikt u nogmaals op "Update" ',
    updateMsg: "Software update proces.",
    updateMsg0: " Maak eerst verbinding. ",
    updateMsg1: "Ontkoppel het display als deze aangesloten is en sluit hem opnieuw aan.",
    updateMsg2: "Ontkoppel de controller als deze is aangesloten en sluit hem opnieuw aan.",
    updateMsg3: "Houdt de verbinding tussen de BESST box en uw computer.",
    updateSelectFile: "Selecteer een bestand",
    updateOnedriveNote: "Het bestand staat in een Microsoft Onedrive map, deze wordt niet ondersteund. Gelieve deze lokaal op uw PC op te slaan.",
    updateReadFail: " Kan gegevens of bestand niet lezen, probeer het opnieuw.",
    updateDataError: " Het headerformaat van de software gegevens is onjuist.",
    updateCrcError: " Dit bestand wordt verdacht van een controlefout. Als de update mislukt, controleer dan het bestand op juistheid.",
    updateMsg4: "De BESST box update het display",
    updateMsg5: "De BESST box update de controller",
    updateMsg6: "De BESST box update de batterij",
    updateMsg7: "De BESST box update de sensor",
    updateMsg8: "Update de BESST box",
    updateMsg10: "De BESST box is aan het updaten",
    updateMsg11: "Het updateproces kan een foutmelding krijgen. Beter om opnieuw te updaten.",
    updateMsg20: " De update is voltooid.",
    updateMsg30: " Klik op lezen om het serienummer en softwareversie te krijgen.",
    updateMsgLO: " Selecteer een bin-bestand van uw PC of download het bestand online.",
    updateMsgL: " Selecteer een bin-bestand van uw PC.",
    updateMsgTip: " Nieuwe softwareversie wordt gedetecteerd. U kunt de nieuwste versie selecteren en bijwerken. Als u de volg stappen niet kent, gaat u naar de sectie Tool-Update",

    hasHmiSn: "Controleer het display sn.",
    hasCtlSn: " Controleer het controller sn.",
    hasBatSn: " Controleer het batterij sn.",
    hasMotorSn: " Controleer het motor sn.",
    hasSensorSn: " Controleer het sensor sn.",
    updateStep0: 'Update vervolgstappen',
    updateStep1: 'Klik op "Verbinden" om de verbinding tussen uw computer en servicetool te maken.',
    updateStep2: 'Selecteer het componenttype dat u wilt updaten. ',
    updateStep3: 'Klik op "Lezen" om het serienummer en softwareversie te krijgen. Als serienummer leeg is, is updaten niet toegestaan.',
    updateStep4: 'Klik op "Download een bestand uit de Cloud" of "Haal een bestand op van uw PC".',
    updateStep5: 'Koppel het component los en klik op "Update" om de software naar de servicetool te downloaden. Tijdens dit proces ziet u het lampje van de servicetool "PC" knipperen. Als de LED "PC" altijd brandt, moet u de servicetool resetten naar de normale status en opnieuw opstarten.',
    updateStep6: 'Na het succesvol downloaden van software naar servicetool, kun je het component opnieuw aansluiten en zal de servicetool de software herschrijven. Klik rechtsboven op het pictogram "Help" voor meer informatie.',

    snMaxLength: "De maximale lengte van SN is 40",
    customerMaxLength: "De maximale lengte van het klant nummer is 24",
    speedLimitEmpty: "Snelheidslimiet is leeg. Lees alsjeblieft om de snelheidslimiet van het component te krijgen.",
    whileSizeRange: "De omtrek ligt buiten het bereik van de ingestelde wielmaat",

    keyCardStart: "Sleutelkaart start met koppelen",
    keyCardStop: "Sleutelkaart is gestopt met koppelen",

    noCanConnect: "BESST box is niet verbonden of ondersteunt CAN niet",
    writeModelTip: "De modelgegevens zijn leeg of voldoen niet aan specificities",
    writeHardwareTip: "De hardwaregegevens zijn leeg of voldoen niet aan de specificaties ",

    pcMode: "PC modus",
    pcModeTxt: ' "PC" -modus wordt gebruikt om gegevens van het display te lezen in vergelijking met de normale werkmodus. Wanneer het display verbinding maakt met BESST-tool, als display kan worden ingeschakeld, gaat het display in de "PC" modus.',
    pcModeTxtLCD: ' LCD Display: Op het scherm wordt "PC" of "Connected OK" weergegeven.',
    pcModeTxtLED: ' LED Display: De LED van het hoogste ondersteunings niveau is aan. ',
    paramsName: "Params Versie",
    paramsCloud: " Haal uit de Cloud ",
    paramsCombine: "Gegevens samenvoegen",

    batUnderMt: "In garantie",
    batOutMt: "Buiten garantie",
    batUseTime: " Batterij is meer dan 30 maanden in gebruik. En valt buiten de garantie.",
    strNaming: ' gegevens ondersteunen geen karakters "\ / ^ : * ? " < > |" ',
    getDataTip: " Servergegevens worden opgehaald, even geduld",

    remark: "Opmerking",
    defect: "Defect",
    image: "Afbeedling",
    description: "Omschrijving",
    video: "Video",
    updateSteps: 'Update proces',
    connectorType: 'Als de connector hetzelfde type als afbeelding 1 heeft, klik dan op "UART" om de details van de instructie te controleren. Als de connector hetzelfde type als afbeelding 2 heeft, klik dan op "CAN bus" om de details van de instructie te controleren.',
    updateSoftwareMethod: 'Hoe software van servicetool naar het component te updaten?',
    HMIFirst: '1. Sluit het display aan op de servicetool.',
    HMISecond: '2. Druk op de aan / uit knop van het display om het update proces te starten. Als de update start blijft de servicetool "Uart" LED knipperen. ',
    HMIThird: '3. Als de update is geslaagd gaat de LED "Test" van de servicetool branden.',
    HMIForth: '4. Reset de servicetool naar de normale status. Anders zou u stap 2 herhaaldelijk krijgen.',
    ControllerFirst: '1. Sluit de controller aan op de servicetool om het updateproces te starten. Als de update start, blijft de servicetool "Uart" LED knipperen. ',
    ControllerSecond: '2. Als de update is geslaagd, gaat de LED "Test" van de servicetool branden. En koppel vervolgens de controller los.',
    ControllerThird: '3. Reset de servicetool naar de normale status. Anders zou u stap 1 herhaaldelijk krijgen.',
    HMIFifth: '1. Sluit het display aan op de servicetool om het updateproces te starten. Als de update start, blijft de servicetool "CAN Bus" LED knipperen. ',
    HMISixth: '2. Als de update is geslaagd, gaat de LED "Test" van de servicetool branden. Ontkoppel vervolgens het display.',
    HMISeventh: '3. Reset de servicetool naar de normale status. Anders zou u stap 1 herhaaldelijk krijgen.',
    ControllerForth: '1. Sluit de controller aan op de servicetool om het updateproces te starten. Als de update start, blijft de servicetool "CAN Bus" LED knipperen. ',
    ControllerFifth: '2. Als de update is geslaagd, gaat de LED "Test" van de servicetool branden. En koppel vervolgens de controller los.',
    ControllerSixth: '3. Reset de servicetool naar de normale status. Anders zou u stap 1 herhaaldelijk krijgen.',
    secondVersionTitle: 'Naast de eerste versie van de servicetool, ondersteunen de 2e en 3e versie servicetool ook software updates om aan de nieuwe eisen van de gebruiker te voldoen.',
    secondVersion: ' Tweede versie',
    thirdVersion: 'Derde versie',
    secondVersionFirst: '1. Waarschijnlijk kunt u de update online krijgen',
    secondVersionSecond: '2. Dubbelklik op "Update" om software naar de servicetool te downloaden en zichzelf vervolgens bij te werken. Tijdens dit proces ziet u de servicetool "PC" LED knipperen. Als de LED "PC" blijft branden, moet u het servicetool resetten naar de normale status. Als de LED "Test" al brandt, hoeft u alleen nog maar op "Update" te klikken.',
    secondVersionThird: '3. Als de update is geslaagd, verandert de servicetool in de normale status.',
    figureFirst: 'Afbeelding 1',
    figureSecond: 'Afbeelding 2',
    thirdVersionFirst: '2. Klik eenmaal op "Update" om de software naar de servicetool te downloaden en zichzelf vervolgens bij te werken. Tijdens dit proces ziet u de servicetool "PC" LED knipperen. Als de LED "PC" blijft branden, moet u het servicetool resetten. Als de LED "Test" al brandt, hoeft u enkel opnieuw op "Update" te klikken. ',
    // defect
    bikeLabel_1: "Geen ondersteuning",
    bikeLabel_2: " Demotor stopt niet",
    bikeLabel_3: "Het wiel / de motor is geblokkeerd",
    bikeLabel_4: " Motor werkt zonder ondersteuning",
    bikeLabel_5: " Ondersteunt niet continu",
    bikeLabel_6: "Zwakke ondersteuning",
    bikeLabel_7: " Batterijcapaciteit en display komen niet overeen",
    bikeLabel_8: "De fiets resoneert of voelt nerveus aan",
    bikeLabel_9: " Het licht werkt niet correct",
    bikeLabel_10: " Het achterlicht werkt niet correct",
    bikeLabel_11: " Kilometerstand is te kort",
    bikeLabel_12: " Geen snelheid op het display",
    motorLabel_1: "Veel geluid en niet continu",
    motorLabel_2: "Geen ondersteuning, hoog transmissiegeluid",
    motorLabel_3: "Connector- en kabelschade",
    motorLabel_4: "Spindle los",
    motorLabel_5: "Cover schade",
    motorLabel_6: "Trillen / vibreren",
    motorLabel_7: "De temperatuur van het motorhuis is te hoog",
    motorLabel_8: "De motor is niet waterdicht",
    motorLabel_9: "De motor is doorgebrand",
    motorLabel_10: "Draas schade boutgaten",
    hmiLabel_1: "De kap is gebroken",
    hmiLabel_2: "De knop werkt niet",
    hmiLabel_3: "Condens in het scherm",
    hmiLabel_4: "Connector- en kabelschade",
    hmiLabel_5: " De displayhouder is beschadigd ",
    hmiLabel_6: "Afwijkend display",
    hmiLabel_7: "Het scherm is beschadigd",
    hmiLabel_8: "Bluetooth is niet toegankelijk",
    hmiLabel_9: "De trilfunctie werkt niet",
    ctlLabel_1: "Controller verbrand",
    ctlLabel_2: "Vast onderdeel zit los",
    ctlLabel_3: "Controller is niet waterdicht",
    ctlLabel_4: "Connector- en kabelschade",
    batLabel_1: "De kap is gebroken",
    batLabel_2: "Accu geleider beschadigd",
    batLabel_3: "Laad en ontlaad poort schade",
    batLabel_4: "De kabel is gebroken",
    batLabel_5: "Kan niet opladen",
    batLabel_6: "Batterij heeft geen uitgangsspanning",
    batLabel_7: "De stofkap is beschadigd",
    batLabel_8: "Ondersteuningssniveau en knopschade",
    batLabel_9: "Sleutel beschadigd",
    batLabel_10: "Sleutel verloren",
    batLabel_11: "Demontage van de batterij moeilijk",
    sensor_1: "Axiale speling",
    sensor_2: "De borgmoer zit los",
    sensor_3: "Sensor roest",
    sensor_4: "Connector- en kabelschad",
    sensor_5: "Lagerschade sensor",
    sensor_6: "Linker crank beschadigd",
    sensor_7: "Rechter crank beschadigd",

    checkSnHmi: "De SN van HMI moet beginnen met 'DP'.",
    checkSnCtrl: "De SN van de controller moet beginnen met 'CR'.",
    checkSnSensor: "De SN van de sensor moet beginnen met 'SR'.",
    checkSnTool: " De SN van BESST moet beginnen met 'BE'.",

    inputValueTip: "Please input value",
    readDateTip: "Please read data",
};

const Hmi = {
    hmi: "Display",
    auth: "Machtigen",
    applyHmiAuth: "Machtiging Display toevoegen",
    applyAllAuth: "Alle machtigingen toevoegen",
    noHmiAuthTip: " U bent niet bevoegd om de display te schrijven. Vraag het merk om toestemming.",
    applySuccess: "Succesvol verzonden, wacht op authorisatie.",
    brandEmptyTip: " U heeft geen gelieerd merk.",
    getApplySuccess: "Succevol toegevoegd",
    hmiTip: 'Hoofdgegevens zijn leeg. Schakel het display in naar de "PC" -modus of controleer of het display wordt gevoed',
    hmiTipCan: "De hoofdgegevens zijn leeg. Als de HMI is aangesloten en is ingeschakeld, moet de software mogelijk worden bijgewerkt.",
    mileageTotalSupport: "Ondersteunt gehele waarden, zoals 1000 km. Alleen het gehele getal wordt geschreven ",
    mileageTotalMax: "Maximale waarde is 99999. Controleer de waarde.",
    mileageSingleSupport: "Ondersteunt waardes zoals 1000.1km. Alleen het gehele deel en het eerste fractionele deel worden geschreven",
    mileageSingleMax: " Max waarde is 9999.9. Controleer de waarde.",
    mileageSingleLess: "Single mileage should not larger than total mileage",
    mileageTotalLarger: "Total mileage should not less than single mileage",

    pairCardIn: "Voer de status van de bijpassende sleutelkaart in",
    pairCardSuccess: "Succesvol, bijpassende sleutelkaart",

    cNam: AttributeName
};

const Controller = {
    controller: "Controller",
    auth: "Machtiging",
    applyControllerAuth: "Controller machtiging aanvragen",
    applyAllAuth: "Alle machtigingen aanvragen",
    noControllerAuthTip: " U bent niet bevoegd om de controller te schrijven. Vraag het merk om toestemming.",
    applySuccess: "Succesvol verzonden, wacht op autorisatie. ",
    brandEmptyTip: "U heeft geen partner merk.",
    getApplySuccess: "Succesvol autorisatie ontvangen",
    controllerTip: "Hoofdgegevens zijn leeg. Controleer of de controller is ingeschakeld",
    cNam: AttributeName
};

const Battery = {
    battery: "Batterij",
    batteryTip: "De hoofdgegevens zijn leeg. Controleer of de batterij is geladen of is ingeschakeld",
    cNam: AttributeName
};

const Update = {
    update: "Update",
    fileCloudToUpdate: " Download een bestand uit de cloud ",
    fileToUpdate: " Haal een bestand op van uw PC",
    itemName: "Item naam",
    tip: "Tip",
    updateTip: " Selecteer eerst het item, download het hardwarebestand en update het onderdeel.",
    noHardwareFileTip: " Het item bevat geen hardwarebestand.",
    noResultTip: "Er zijn geen overeenkomstige gegevens.",
    picture: "Afbeelding",
    name: "Naam",
    partNo: "Artikelnummer",
    operation: "Afdeling",
    version: "Versie",
    updateTime: "Update-tijd",
    remark: "Opmerking",
    select: "Selecteren",
    firmwareList: "Firmwarelijst",
    objectEmptyTip: "Deze categorie bezit geen onderdelen.",
    componentTypeTip: "Graag component selecteren",

    componentReadTip: "Graag nogmaals lezen",
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
    setting: "Instellingen",
    language: "Taal",
    version: "Versie",
    versionDate: "Versiedatum",
    userInfo: "Gebruikers informatie",
    searchForUpdate: "Zoeken naar update",
    changePassword: "Wachtwoord wijzigen",
    editCompanyInfo: "Bedrijfsinformatie wijzigen",
    password: 'Wachtwoord',
    oldPassword: "Oud wachtwoord",
    newPassword: "Nieuw wachtwoord",
    confirmPassword: "Wachtwoord bevestigen",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    authTipOne: " U heeft geen toestemming.",
    authTipTwo: " Neem contact op met de beheerder.",
    warning: "Waarschuwing",
    editCompany: "Bedrijfsinformatie wijzigen",
    logo: "Logo",
    uploadTipOne: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
    uploadTipTwo: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB ",
    name: "Naam",
    email: "E-mail",
    phone: "Telefoon",
    website: "Website",
    address: "Adres",
    postcode: "Postcode",
    company: "Bedrijf",
    country: "Land",

    languages: {
        german: "Deutsch",
        english: "English",
        chinese: "中文",
        dutch: "Nederlands",
    },
    orgType: {
        brand: "Merk",
        dealer: "Dealer",
        assembler: "Assembleerder",
        component: "Onderdeel",
        serviceCenter: "Service Center",
    },
    updToolNote1: "De BESST-servicetool heeft een update van nieuwe software nodig om deze beter af te stemmen op de kenmerken van de componenten.",
    updToolNote: "Opmerking",
    updToolNote2: "Sluit dit dialoogvenster niet voordat de update is voltooid.",
    updToolNote3: "Als de update is gelukt, gaat de servicetool terug naar de normale status.",
    updToolNote4: "Als de voortgangsbalk van de update niet wordt weergegeven, koppelt u de servicetool los van de computer en sluit deze opnieuw aan. Klik vervolgens op de 'update' knop.",
    updStarting: "De update begint...",
    updSuccess: "Update succesvol?",
    close: "Sluiten",
    cNam: AttributeName,
    upd: Update,
    op: Operation

};

const Repair = {
    cNam: AttributeName
};

// bike basic
const VehicleDetail = {
    back: "Terug",
    sn: "Seriennummer",
    key: "Sleutel",
    productTime: "Productie tijd",
    itemName: "Model naam",
    itemPartNo: "Artikel Nr.",
    orderSn: "Order serienummer",
    orderSourceId: "Po No.",
    customerId: "Klantnaam",
    customerPartNo: "Klant artikelnummer",
    customerPoNo: "Klant Po No.",
    replace: "Vervang",
    oldSn: "Oud SN",
    newSn: "Nieuw SN",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    save: "Opslaan",
    edit: "Wijzigen",
    add: "Toevoegen",
    reportList: "Diagnoseberichten",
    detail: "Detail",
    operation: "Afdeling",
    date: "Datum",
    author: "Auteur",
    emptyListTip: "De fiets data is leeg.",
    assembler: "Monteur",
    dealer: "Dealer",
    saleTime: "Verkoop data",
    customerName: "Klantnaam",
    customerEmail: "Klant E-mail",
    part: "Onderdelen",
    history: "Geschiedenis"
};

const VehicleList = {
    oem: "OEM",
    modelName: "Modellnaam",
    brand: "Merk",
    vehicleSn: "Fiets SN",
    bomSn: "Component SN",
    search: "Zoeken",
    noResult: "Geen resultaat！",
    total: "Totaal",
    deleteVehicle: "Deze fiets verwijderen",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    delete: "Verwijderen",
    detail: "Detail",
    noSale: "Niet verkocht",
    tableHead: {
        vehicleSn: "Fiets SN",
        brandName: "Merknaam",
        ModelName: "Modelnaam",
        OrderSn: "Ordernummer",
        saleDate: "Verkoopdatum",
        operation: "Afdeling",
    },
    deleteSuccess: "Succelvol verwijderd",
    export: "Exporteer",
    start: "Start",
    end: "Einde",
    op: Operation,
};


// 服务中心相关
const SCCenterTicketList = {
    category: "Categorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Onderdeel serienummer",
        createTime: "Tijd",
        status: "Status",
        operation: "Afdeling",
    },
    categoryList: {
        all: "Allemaal",
        vehicle: "Fiets",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Batterij",
        light: "Licht",
        controller: "Controller",
        other: "Andere",
    },
    statusList: {
        all: "Allemaal",
        pendingAssign: "Toewijzen",
        processing: "In Bewerking",
        complete: "Voltooid"
    },
    noResult: "Geen resultaat！",
    total: "Totaal"
};

const SCHandleUpTicketList = {
    category: "Categorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Onderdeel serienummer",
        createTime: "Tijd",
        status: "Status",
        operation: "Afdeling",
    },
    categoryList: {
        all: "Allemaal",
        vehicle: "Voertuig",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Batterij",
        light: "Licht",
        controller: "Controller",
        other: "Andere",
    },
    statusList: {
        all: "Allemaal",
        pendingAssign: "Toewijzen",
        processing: "In Bewerking",
        complete: "Voltooid"
    },
    noResult: "Geen resultaat！",
    total: "Totaal"
};

const SCMyTicketList = {
    category: "Categorie",
    status: "Status",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Onderdeel serienummer",
        createTime: "Tijd",
        status: "Status",
        operation: "Afdeling",
    },
    categoryList: {
        all: "Allemaal",
        vehicle: "Fiets",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Batterij",
        light: "Licht",
        controller: "Controller",
        other: "Andere",
    },
    statusList: {
        all: "Allemaal",
        pendingAssign: "Toewijzen",
        processing: "In Bewerking",
        complete: "Voltooid"
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    addTicketDialog: {
        dialogTitle: "Service-Ticket toevoegen",
        category: "Categorie",
        title: "Titel",
        description: "Omschrijving",
        bike: "Fiets",
        sn: "Serienummer",
        serviceCenter: "Service Center",
        image: "Afbeelding",
        video: "Video",
        clickToUpload: "Klik om te uploaden"
    },
};

const SCTicketDetail = {
    back: "Terug",
    transferToUp: "Naar boven",
    transferToInside: "Openen",
    take: "Aannemen",
    history: "Geschiedenis",
    ticketInfo: {
        ticketTitle: "Klachten detail",
        serviceNo: "Service Nr.",
        createTime: "Tijd",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Titel",
        description: "Omschrijving",
        image: "Afbeelding",
        video: "Video",
    },
    userInfo: {
        address: "Adres",
        website: "Website",
        email: "Email",
    },
    content: "Inhoud",
    image: "Afbeelding",
    sendMessage: "Bericht versturen",
    historyTicket: {
        title: "Onderwerp",
        name: "Naam",
        operation: "Afdeling",
        time: "Tijd",
    },
    handleInside: {
        title: "Onderwerp",
        tip: "Kies een person om dit ticket af te handelen.",
        select: "selecteren",
        confirm: "Bevestigen",
        cancel: "Annuleren"
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
    back: "Terug",
    sn: "Serienummer",
    category: "Categorie",
    modelName: "Model Naam",
    partNo: "Artikelnummer",
    orderNo: "Order No.",
    customerName: "Klantnaam",
    customerPartNo: "Klant Artikelnummer",
    customerPoNo: "Klant Po No.",
    produceTime: "Productie Tijd",
    saleTime: "Verkoopt data",
};

const ComponentComponentList = {
    yes: "Ja",
    no: "Nee",
    search: {
        poNo: "Artikel No.",
        orderNo: "Order nr.",
        modelName: "Model Naam",
        sn: "Serienummer",
        customerName: "Klant naam",
        search: "Zoeken",
        export: "Exporteren",
    },
    headData: {
        poNo: "Artikel No.",
        orderNo: "Order No.",
        sn: "Serienummer",
        category: "Kategorie",
        modelName: "Model Naam",
        customerName: "Klant naam",
        operation: "Afdeling"
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    detail: "Detail",
    delete: "Verwijderen",
    batchDelete: "Batch Verwijderen",
    deleteDialog: {
        title: "Verwijder Onderdeel",
        content: "Weet u zeker dat u dit wilt verwijderen?",
    },
    exportDialog: {
        title: "Exporteren Onderdeel",
        startNum: "Start Nr.",
        endNum: "Eind Nr.",
        export: "Exporteren",
        cancel: "Annuleren"
    },
    back: "Terug",
};

const ComponentComponent = {
    Detail: ComponentComponentDetail,
    List: ComponentComponentList
};

// 零部件相关 --- TestFeedback
const ComponentTestFeedbackList = {
    search: {
        email: "Email",
        searchBtn: "Zoeken",
    },
    add: "Toevoegen",
    noResult: "Geen resultaat",
    total: "Totaal",
    deleteDialog: {
        title: "Verwijder terugkoppeling",
        content: " Weet u zeker dat u dit wilt verwijderen?",
        yes: "Ja",
        no: "Nee",
    },
    headData: {
        date: "datum",
        email: "Email",
        fileName: "Dossier Naam",
        status: "Status",
        operation: "Afdeling",
        desc: "Omschrijving"
    },
    detail: "Detail",
    delete: "Verwijderen",
};

const ComponentTestFeedbackDetail = {
    back: 'Terug',
    fileDetail: 'Feedback Informatie',
    edit: 'Wijzigen',
    delete: 'Verwijderen',
    detail: {
        title: 'Dossier informatie',
        email: 'Email',
        fileName: 'Dossiernaam',
        setTime: 'TIjd Instellen',
        status: 'Status',
        upload: 'UpLoad Feedback',
        desc: 'Omschrijving'
    },
    deleteDialog: {
        title: "Verwijder terugkoppeling",
        content: " Weet u zeker dat u dit wilt verwijderen?",
        yes: "Ja",
        no: "Nee",
    },
    fileList: 'Dossier Lijst',
    saveFeedback: 'Feedback opslaan',
    deleteFile: 'Verwijderen'
};

const ComponentTestFeedbackSave = {

    createFeedback: 'Feedback creëren',
    feedbackInfo: 'Feedback Informatie',

    email: 'Email',
    fileName: 'DossierName',
    setTime: 'TIjd Instellen',
    upload: 'UpLoad Terugkoppeling',
    status: 'Status',
    desc: 'Omschrijving',

    file: 'Upload',
    clickUpload: 'Klik op UpLoad',

    delete: "Verwijderen",
    download: "Download",
    fileList: 'DossierList',
    saveFeedback: "Terugkoppeling opslaan",
    deleteFile: 'Verwijderen',
};

const ComponentTestFeedback = {
    List: ComponentTestFeedbackList,
    Save: ComponentTestFeedbackSave,
    Detail: ComponentTestFeedbackDetail
};
const ComponentMaterialList = {
    orderNo: "Order Nr.",
    clientCode: 'Klant Code',
    modelDes: "Model Des",
    numQty: "Nr.",
    status: "Status",
    proLine: "Productie Lijn",
    date: "Dateum",
    remark: "Opmerking",
    operation: "Bewerking",

    startDate: "Start Datum",
    endDate: "Eind Datum",
    connError: "Verbindingsfout",
    search: "Zoeken",
    batchDelete: "Batch Verwijderen",
    upload: "Upload document",
    dialogUploadText: " Upload productieorderbestand",
    deleteDialogTitle: "Productieorder verwijderen",
    deleteDialogContent: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    edit: "Bewerk",
    editDialogTile: "Update Informatie",
    noResult: "Geen resultaat！",
    total: "Totaal",

    delete: 'Verwijderen',
    detail: 'Detail',

    all: "AAlles",
    line1: "Lijn 1",
    line2: "Lijn 2",
    line3: "Lijn 3",
    line4: "Lijn 4",
    line5: "Lijn 5",
    line6: "Lijn 6",
    line7: "Lijn 7",
    s1: "In planning",
    s2: "In voorbereiding",
    s3: "In productie",
    s4: "In opslag",
    s5: "In test",
    s6: "Abnormale montage",
    s7: "Abnormale test",

};

const ComponentMaterialDetail = {
    back: 'Terug',
    delete: 'Verwijder',
    title1: 'Order Info',
    title2: 'Status Check',
    orderNo: 'Order Nr.',
    user: 'Gebruiker',
    status: 'Status',
    desc: 'Omschrijving',
    qty: "Aantal",
    num: "Nummer",
    drawing: "Tekening",
    stator: "Stator",
    exterior: "Uiterlijk",

    deleteDialog: {
        title: "Productieorder verwijderen",
        content: " Weet je zeker dat je het wilt verwijderen?",
        yes: "YJa",
        no: "Nee",
    },

    scanning: 'Scan QR code',

    show1: "Geef de QR-code van de abnormale montage weer",
    show11: "Abnormale montage",
    show2: "Toon de QR-code van de abnormale test",
    show22: "Abnormale test",
    connError: "Verbindingsfout",

    s1: "In planning",
    s2: "In voorbereiding",
    s3: "In productie",
    s4: "In opslag",
    s5: "In test",
    s6: "Abnormale montage",
    s7: "Abnormale test",
};
const ComponentMaterial = {
    List: ComponentMaterialList,
    Detail: ComponentMaterialDetail,
};
// 零部件相关 --- order
const ComponentOrderDetail = {
    back: "Terug",
    orderDetail: "Order Detail",
    edit: "Wijzigen",
    delete: "Verwijderen",
    orderInfo: {
        title: "Order Informatie",
        orderNo: "Po No.",
        customerPartNo: "Klant Artikelnummer",
        note: "Opmerking",
        customerName: "Klantnaam",
        customerPo: "Klant Po.",
    },
    itemsList: "Items Lijst",
    produce: "Produceren",
    testReport: "Test Rapport",
    componentList: "Onderdelen lijst",
    replace: "Vervangen",
    headData: {
        modelName: "Model Naam",
        articleNo: "Artikel Nr.",
        customerArticleNo: "Klant artikel Nr.",
        quantity: "Aantal",
        operation: "Onderneming",
    },
    deleteDialog: {
        title: "Order verwijderen",
        content: "Weet u zeker dat u dit wilt verwijderen?",
        yes: "Ja",
        no: "Nee"
    },
    category: "Categorie",
    search: "Zoeken",
    clear: "Verwijderen",
    selectCategory: "Selecteer zoeken",
    modelNamePN: "Model Naam/Artikel No.",
    confirmDialog: {
        replace: "vervangen",
        yes: "Ja",
        no: "Nee"
    },

    dataConnect: "Data Connect"
};

const ComponentOrderList = {
    search: {
        orderNo: "Order Nr.",
        searchBtn: "Zoeken",
        clearBtn: "Bevestigen",
    },
    add: "Toevoegen",
    list: "Lijst",
    detail: "Detail",
    delete: "Verwijderen",
    noResult: "Geen resultaat！",
    total: "Totaal",
    headData: {
        date: "Datum",
        orderNo: "Order Nr.",
        customerName: "Klantnaam",
        pQ: "Geproduceert aantal",
        status: "Status",
        operation: "Operatie",
        model: "Model",
    },
    deleteDialog: {
        title: "Verwijder Order",
        content: "Weet u zeker dat u dit wilt verwijderen?",
        yes: "Ja",
        no: "Nee"
    },
    op: Operation,

};

const ComponentOrderProduce = {
    back: "Terug",
    connect: "Verbinden",
    reset: "Reset",
    update: "Update",
    readSn: "SN lezen",
    writeSn: "SN schrijven",
    orderInfo: {
        title: "Order Informatie",
        model: "Model",
        customerName: "Klantnaam",
        customerPoNo: "Klantnaam Po Nr.",
        customerPartNo: "Klant Artikelnummer",
    },
    programingInfo: {
        title: "Programmeren",
        com: "COM",
        file: "Document",
        pleaseSelect: "Selectreren "
    },
    writeSnInfo: {
        title: "SN schrijven",
        com: "COM",
        pleaseSelect: "Slecteren",
        sn: "Serienummer"
    },
    cNam: AttributeName,
    up: Update,
};

const ComponentOrderDataConnect = {
    back: "Terug",
    save: "Opslaan",
    clear: "Verwijderen",
    removeBinding: 'Verwijder koppeling',
    orderInfo: {
        title: "Order Informatie",
        model: "Model",
        customerName: "Klantnaam",
        customerPoNo: "Klant Po No.",
        customerPartNo: "Klant artikel",
    },
    programingInfo: {
        ListTitle: "Resultaatlijst",
        searchTitle: 'Zoeken',
        motorTitle: 'Motor',
        result: 'Resultaat',
        pleaseSelect: "selecteren",
        sn: 'SN',
        cr: 'Controller_Sn',
        sr: 'Sensor_Sn',
        mm: 'Motor_Sn'
    },

};

const ComponentOrderSave = {
    back: "Terug",
    createOrder: "Order Creëren",
    orderInfo: {
        title: "Order Informatie",
        poNo: "Po No.",
        customerInfo: "Klant informatie",
        customerPartNo: "Klant Artikelnummer",
        customerName: "Klantnaam",
        customerPo: "Klant Po.",
        note: "Notitie",
    },
    itemList: "Item Lijst",
    addItem: "Item toevoegen",
    addItems: "Items toevoegen",
    delete: "Verwijderen",
    saveOrder: "Order opslaan",
    tableHead: {
        name: "Naam",
        articleNo: "Artikelnummer",
        customerArticleNo: "Klant artikel No.",
        quantity: "Aantal",
        operation: "Ondernemen",
    },
    itemHeadData: {
        image: "afbeelding",
        name: "Naam",
        partNo: "Artikelnummer"
    },
    category: "Categorie",
    search: "Zoeken",
    clear: "verwijderen",
    selectCategory: "Zoeken selecteren",
    modelNamePN: "Model naam/Artikel No."
};

const ComponentOrderTestReport = {
    back: "Terug",
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
    back: "Terug",
    add: "Toevoegen",
    save: "Opslaan",
    edit: "Wijzigen",
    print: "Afdrukken",
    export: "Exporteren",
    delete: "Verwijderen",
    detail: "Detail",
    articleNO: "Artikelnummer",
    noPhoto: "Geen Foto",
    viewLargerImage: "Vergroot de afbeelding",
    list: "Lijst",
    partName: "Onderdeel naam",
    yes: "Ja",
    no: "Nee",
    listHead: {
        itemNo: "ITEM NR.",
        partNo: "ARTIKELNUMMER",
        picture: "Afbeelding",
        name: "Naam",
        spec: "Specificaties",
        package: "pakket",
        packageSize: "Afmetingen pakket",
        price: "PRIJS",
        linkBom: "LINK BOM",
        operation: "Operatie"
    },
    selectHead: {
        partNo: "ARTIKELNUMMER",
        picture: "Afbeelding",
        name: "Naam",
        spec: "Specificaties",
        package: "Pakket",
        packageSize: "Afmetingen pakket",
        price: "PRIJS",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "Verwijder Product",
        content: "Weet u zeker dat u dit wilt verwijderen ",
    },
    addItemDialog: {
        title: "Item toevoegen",
    },
    operation: "Ondernemen",
    tip: "Tip",
    content01: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
    content02: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB "
};

const ComponentProductDetail = {
    back: "Terug",
    copy: "Kopiëren",
    edit: "Wijzigen",
    delete: "Verwijderen",
    model: "Model",
    name: "Naam",
    partNo: "Artikelnummer",
    articleList: "Artikel lijst",
    desc: "Omschrijving",
    download: "Downloaden",
    preview: "Voorbeeld",
    history: "Geschiedenis",
    update: "Update",
    view: "Bekijken",
    hardware: "Hardware Versie",
    firmware: "Firmware",
    bomList: "BOM lijst",
    yes: "Ja",
    no: "Nee",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    deleteProductDialog: {
        title: "Verwijder product",
        content: "Bent u zeker dat u dit wilt verwijderen? ",
    },
    deleteFirmwareDialog: {
        title: "Verwijder Software",
        content: " Bent u zeker dat u deze software wilt verwijderen? ",
    },
    historyDialog: {
        title: "Software Geschiedenis",
        version: "Versie",
        updateTime: "Update Tijd",
        remark: "Opmerking",
        operation: "Operatie",
        noResult: "Geen resultaat！"
    },
    editFirmwareDialog: {
        title: "Upload Firmware",
        file: "Document",
        version: "Versie",
        remark: "Opmerking",
        clickToUpload: "Klik om te uploaden",
    },
    updateFirmwareDialog: {
        title: "Upload Firmware",
        file: "Document",
        version: "Version",
        remark: "Opmerking",
        clickToUpload: "Klik om te uploaden",
    },
    training: 'Training',
    set: ArticleSet

};

const ComponentProductEditCategory = {
    back: "Terug",
    category: "Categorie",
    yes: "Ja",
    no: "Nee",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    deleteDialog: {
        title: "Verwijder Categorie",
        content: "Weet u zeker dat u dit wilt verwijderen? ",
    },
    addSubCategoryDialog: {
        title: "Subcategorie toevoegen",
        namePH: "Categorie Naam"
    },
    editCategoryDialog: {
        title: "Wijzigen Categorie",
        namePH: "Categorie naam"
    },
    addSubCategory: "Subcategorie toevoegen",
    edit: "Wijzigen",
    delete: "Verwijderen",
};

const ComponentProductIndex = {
    editCategory: "Wijzigen Categorie",
    training: 'Training'
};

const ComponentProductList = {
    add: "Toevoegen",
    noResult: "Geen resultaat！",
    total: "Totaal",
    set: ArticleSet
};

const ComponentProductSave = {
    back: "Terug",
    logo: "Afbeelding",
    name: "Naam",
    partNo: "Artikelnummer",
    desc: "Omschrijving",
    save: "Opslaan",
    tip: {
        title: "Tip",
        content01: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
        content02: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB "
    },
    add: "Toevoegen",
    copy: "Kopiëren",
    edit: "Wijzigen",
    set: ArticleSet

};

const ComponentVideoList = {
    back: "Terug",
    category: "Categorie",
    add: "Toevoegen",
    download: "Downloaden",
    delete: "Verwijderen",
    noResult: " Geen resultaat！",
    total: "Totaal",
    uploadVideo: "Upload Video",
    upload: "Upload",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    all: "Allemaal",
    systemError: "Systeem Error",
    hmi: "Display",
    motor: "Motor",
    controller: "Controller",
    sensor: "Sensor",
    battery: "Batterij",
    charger: "Oplader",
    name: "Naam",
    operation: "Ondernemen",
    videoEmptyTip: "Upload video AUB",
    saveSuccess: "Succesvol opgeslagen",
    yes: "Ja",
    no: "Nee",
    deleteVideo: "Verwijder Video",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    preview: "Voorbeeld",
    errorCode: "Error code",
    link: "Link",
    edit: "Wijzigen"
};

const ComponentProductVideoList = {
    back: "Terug",
    category: "Categorie",
    addVideo: " Video toevoegen",
    download: "Downloaden",
    delete: "Verwijderen",
    noResult: " Geen resultaat！",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    systemError: "Systeem Error",
    hmi: "Display",
    motor: "Motor",
    controller: "Controller",
    sensor: "Sensor",
    battery: "Batterij",
    charger: "Oplader",
    name: "Naam",
    operation: "Ondernemen",
    yes: "Ja",
    no: "Nee",
    preview: "Voorbeeld",
    deleteVideo: "Verwijder Video",
    deleteTip: "Weet u het zeker dat u dit wilt verwijderen?",
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
    ticketStat: " Ticketstatistieken ",
    ticketByCategory: "Ticket per Categorie",
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
    diagnosis: "Diagnose"
};

// 经销商相关 --- error
const DealerErrorShooting = {
    errorCode: "Error code",
    name: "Naam",
    remark: "Opmerking",
    description: "Omschrijving",
    target: "Doel",
    solution: "Oplossing",
    tip: "Tip",
    tipContent: " Als de bovenstaande oplossing uw probleem niet kan oplossen, neem dan contact op met het merk.",
    help: "Help",
    helpContent: "Klik hier voor meer ondersteuning.",
    link: "Link"
};

const DealerError = {
    Shooting: DealerErrorShooting,
};

// 经销商相关 --- partner
const DealerPartnerList = {
    back: "Terug",
    acceptInvitation: "Uitnodiging accepteren",
    tableHead: {
        name: "Naam",
        phone: "Telefoon",
        website: "Website",
        country: "Land",
        operation: "Onderneming",
    },
    currentBrand: "Huidig merk",
    noResult: "Geen resultaat！",
    total: "Totaal",
    brandInfo: {
        title: "Merk informatie",
        logo: "Logo",
        name: "Naam",
        type: "Type",
        email: "E-mail",
        website: "Website",
        tel: "Telefoon",
        postcode: "Postcode",
        address: "Adres",
    },
    invitationCode: "Uitnodigings code",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    webLink: "Website Link",
    createOrder: "Order creëren",
    orderList: "Bestellijst ",
    view: "Bekijken",
};

const DealerPartner = {
    List: DealerPartnerList,
};

// 经销商相关 --- sale
const DealerSaleDetail = {
    back: "Terug",
    orderDetail: "Order detail",
    view: "Voorbeeld",
    edit: "Wijzigen",
    delete: "verwijderen",
    invoiceNo: "Rekening Nr.",
    invoiceDate: "Verkoop datum",
    submit: "Toevoegen",
    customerInfo: {
        title: "Klant informatie",
        name: "Naam",
        phone: "Telefoon",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Model informatie",
        brand: "Merk",
        bikeId: "Frame nummer",
        model: "Model",
        warrantyPeriod: "Garantie periode"
    },
    detail: {
        title: "Details",
        orderNo: "Order Nr.",
        price: "Prijs",
        invoice: "Rekening",
        salesDate: "Verkoop datum",
        remark: "Opmerking",
    },
    deleteDialog: {
        title: "Wis de bestellen",
        content: "Weet u zeker dat u dit wilt verwijderen?",
        yes: "Ja",
        no: "Nee"
    }
};

const DealerSaleEdit = {
    back: "Terug",
    save: "Opslaan",
    orderEdit: "Order wijzigen",
    customerInfo: {
        title: "Klant informatie",
        name: "Naam",
        phone: "Telefoon",
        email: "E-mail",
    },
    bikeInfo: {
        title: "Type informatie",
        brand: "Merken",
        bikeId: "Frame nummer",
        model: "Model",
        warrantyPeriod: "Garantie periode"
    },
    detail: {
        title: "Details",
        orderNo: "Order Nr.",
        price: "Prijs",
        invoice: "Rekening",
        remark: "Opmerking",
        invoiceDate: "Factuur datum",
    },
};

const DealerSaleList = {
    add: "Toevoegen",
    tableHead: {
        orderNo: "Order Nr.",
        bikeId: "Fiets ID",
        model: "Model",
        customerName: "Klantnaam",
        date: "Datum",
        status: "Status",
        operation: "Operatie"
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    detail: "Detail",
    search: {
        bikeId: "Fiets ID",
        searchBtn: "Zoeken",
        clearBtn: "Bevestigen"
    }
};

const DealerSaleSave = {
    back: "Terug",
    addComponent: "Onderdeel toevoegen",
    save: "Opslaan",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    customerInfo: {
        title: "Klantinformatie",
        name: "Naam",
        phone: "Telefoon",
        email: "E-mail",
        tip: "Tip",
        tip01: " E-mail met betrekking tot klantenservice en code",
        tip02: " Zorg ervoor dat e-mail geldig is. "
    },
    bikeInfo: {
        title: "Fiets informatie",
        bikeId: "E-Bike Id",
        brand: "Merk",
        warrantyPeriod: "Garantie periode",
        date: "Datum",
        price: "Prijs",
        remake: "Opmerking",
    },
    componentInfo: {
        add: "Toevoegen",
        title: "Componeneten informatie",
        replace: "Vervangen",
        sn: "Seriennummer",
        name: "Naam"
    },
    replaceDialog: {
        oldSn: "Oud SN",
        newSn: "Nieuw SN",
        category: "Categorie",
    },
    tipDialog: {
        title: "Tip",
        content: " De e-bike heeft geen batterij, u weet zeker dat u de bestelling opslaat?"
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
    back: "Terug",
    markCompleted: "Merk Voltooid",
    ticketInfo: {
        InfoTitle: "Klachten detail",
        serviceNo: "Service Nr.",
        status: "Status",
        createTime: "Tijd",
        serviceCenter: "Service Center",
        title: "Titel",
        description: "Omschrijving",
        image: "Afbeelding",
        video: "Video",
    },
    user: {
        address: "Adres",
        website: "Website",
        email: "E-mail",
    },
    replay: {
        content: "Inhoud",
        image: "Afbeelding",
        sendMessage: "Bericht versturen"
    },
};

const DealerTicketList = {
    add: "Toevoegen",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetNo: "Doel seriennummer",
        ServiceCenter: "Service Center",
        createTime: "Tijd",
        status: "Status",
        operation: "Operatie",
    },
    detail: "Detail",
    delete: "Verwijderen",
    noResult: "Geen resultaat！",
    total: "Totaal",
    addTicketDialog: {
        dialogTitle: "Service-Ticket toevoegen",
        category: "Categorie",
        title: "Titel",
        description: "Omschrijving",
        bike: "E-Bike",
        sn: "Serienummer",
        serviceCenter: "Service Center",
        image: "Afbeelding",
        video: "Video",
        clickToUpload: "Klik om te uploaden"
    },
    deleteTicket: {
        title: "Ticket verwijderen",
        content: "Weet u zeker dat u dit wilt verwijderen? "
    }
};

const DealerTicket = {
    Detail: DealerTicketDetail,
    List: DealerTicketList
};

const DealerVehicleSearch = {
    search: {
        bikeId: "FIets ID",
        searchBtn: "Zoeken",
        clear: "Verwijderen"
    },
    back: "Terug",
    sn: "Seriennummer",
    key: "Key ",
    productTime: "Productie tijd",
    itemName: "Modelnaam",
    itemPartNo: "Artikelonderdeel Nr.",
    orderSn: "Orderseriennummer",
    orderSourceId: "Order Id",
    customerId: "Klant ID",
    customerPartNo: "Klant Artiklenummer",
    customerPoNo: "Klant Po No.",
    replace: "vervangen",
    oldSn: "Oud SN",
    newSn: "Nieuw SN",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    save: "Opslaan",
    edit: "Wijzigen",
    reportList: "Diagnoseberichten",
    detail: "Detail",
    operation: "Operatie",
    date: "Datum",
    author: "Auteur",
    emptyListTip: "Deze fiets heeft geen informatie.",
    part: "Onderdelen",
    history: "Geschiedenis",
    brand: "Merk",
    saleTime: "Verkoop data",
    customerName: "Klantnaam",
    customerEmail: "Klant Email"
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
    dailyVehicleOrderStatistics: "Dagelijkse fiets order statistieken",
    dailyVehicleAfterSalesStatistics: "Dagelijkse fiets service statistieken",
    dailyDailyVehicleStatistics: " Dagelijkse dealerstatistieken ",
    dailyAssemblerVehicleStatistics: " Dagelijkse montagevoertuigstatistieken "
};

//品牌商
const BrandList = {
    add: "Toevoegen",
    edit: "Wijzigen",
    delete: "Verwijderen",
    noResult: "Geen resultaat！",
    total: "Totaal",
    deleteBrand: "Het Merk verwijderen",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen.",
    yes: "Ja",
    no: "Nee",
    addBrand: "Merk Toevoegen",
    brandInfo: "Merk informatie",
    logo: "Logo",
    uploadTipOne: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
    uploadTipTwo: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB ",
    name: "Naam",
    introduction: "Introductie",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    operation: "ondernemen",
    emptyTip: {
        logo: "Graag het merk logo uploaden",
        name: "Graag de Merk naam invoeren",
        desc: "Graag de omschrijving van het merk invoeren",
    },
    editSuccess: "Succevol gewijzigd",
    addSuccess: "Succesvol toegevoegd",
    deleteSuccess: "Succesvol verwijderd",

    link: "Link",
};

const DealerInvite = {
    codeStatus: "Code Status",
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        invitation: "invitatie code",
        targetName: "Bedrijfs naam",
        contact: "Contact",
        targetEmail: "E-mail",
        createTime: "Tijd",
    },
    codeStatusOptions: {
        readyToSend: "verzendklaar",
        sentNotUsed: "Verstuurd maar niet geäctiveerd",
        used: "gebruikt",
    },
    emptyTip: {
        email: "Graag uw email ingeven",
        name: "Graag de bedrijfsnaam invoeren",
    },
    sendSuccess: "Succesvol verzonden",
    uploadSuccess: "Uploaden succesvol",
    selectTip: "Kies een account",
    deleteSuccess: "Succesvol verwijderd",
    newCode: "Nieuwe Code",
    batchSend: "Serie verzonden",
    send: "Versturen",
    delete: "verwijderen",
    dealerInvitation: "Dealer Uitnodiging",
    selectAccount: "selecteer een account",
    uploadContacts: "Contacten uploaden",
    operation: "Ondernemen",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    email: "E-mail",
    companyName: "Bedrijfsnaam",
    toLeft: "Naar links",
    toRight: "Naar rechts",
    deleteCode: "De code verwijderen",
    orgName: "Bedrijfsnaam",
    uploadFailList: "Upload error lijst",
    reason: "Reden",
    uploadFailTip: " Upload mislukt. Controleer het geüploade bestand of alle contacten zijn aanwezig.",
    resend: "Opnieuw versturen",
    template: "template",
    download: "Download",

    source: "Bron",
    target: "Doel",
    enterKeyWord: "Voer code in",
    noData: "Geen data",
    dealerType: "Dealer Type",
    dealerType1: "Dealer",
    dealerType2: "Special Dealer",
    dealerTypeNote: "Special dealers only have the diagnostic and tool function. A dealer has the full function.",
    op: Operation,

};

const DealerList = {
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        logo: "Logo",
        name: "Naam",
        postCode: "Postcode",
        tel: "Telefoon",
        website: "Website",
        address: "Adres",
        operation: "Operatie",
        email: "E-mail",
    },
    delete: "Verwijderen",
    deleteDialog: {
        title: "De dealer verwijderen",
        tip: "Weet u zeker dat dat u dit wilt verwijderen?"
    },
    yes: "Ja",
    no: "Nee",
    op: Operation,
};

const OemInvite = {
    codeStatus: "Code Status",
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        invitation: "Uitnodigingscode",
        targetName: "Bedrijfsnaam",
        contact: "Contact",
        targetEmail: "Email",
        createTime: "Tijd",
    },
    codeStatusOptions: {
        readyToSend: "Verzendklaar",
        sentNotUsed: "verzonden maar nog niet geactiveerd",
        used: " Succesvol geactiveerd ",
    },
    emptyTip: {
        email: "Graag e-mail ingeven",
        name: "Graag bedrijfsnaam ingeven",
    },
    sendSuccess: "Succesvol verzonden",
    uploadSuccess: "Uploaden succesvol",
    selectTip: "Selecteer een account",
    deleteSuccess: "Succesvol verwijderd",
    newCode: "Nieuwe Code",
    batchSend: "Serie verzonden",
    send: "Versturen",
    delete: "Verwijderen",
    oemInvitation: "OEM uitnodigen",
    selectAccount: "Selecteer account",
    uploadContacts: "Contacten Uploaden",
    operation: "Operatie",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    email: "E-mail",
    companyName: "Bedrijfsnaam",
    toLeft: "Naar links",
    toRight: "Naar rechts",
    deleteCode: "De Code verwijderen",
    orgName: "Bedrijfsnaam",
    uploadFailList: "Error lijst uploaden",
    reason: "Reden",
    uploadFailTip: " Upload mislukt. Controleer het geüploade bestand of alle contacten zijn aanwezig.",
    resend: 'Opnieuw versturen',
    download: "Download",
    template: "Template",
    op: Operation,

};

const OemList = {
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        logo: "Logo",
        name: "Naam",
        postCode: "Postcode",
        tel: "Telefoon",
        website: "Website",
        address: "Adres",
        operation: "Operatie",
        email: "E-mail",
    },
    delete: "Verwijderen",
    deleteDialog: {
        title: "Verwijder OEM",
        tip: "Weet u zeker dat u dit wilt verwijderen?"
    },
    yes: "Ja",
    no: "Nee",
    op: Operation,
};

const AuthList = {
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        name: "Naam",
        type: "Type",
        status: "Status",
        time: "Tijd",
        operation: "Operatie"
    },
    authorization: "Machtigen",
    authorizationSuccess: "Machtiging succesvol",
    status: "Status",
    authorized: "Gemachtigd",
    noAuthorization: "Niet gemachtigd",
    hmi: "Display",
    controller: "Controller"
};

const RecordList = {
    noResult: "Geen resultaat！",
    total: "Totaal",
    tableHead: {
        sn: "Serienummer",
        vehicle: "Voertuig",
        operator: "Operator",
        account: "Account",
        status: "Status",
        time: "Tijd",
        type: 'Type',
        data: "Data"
    },
    status: "Status",
    recordTitle: " Actiegegevensrecord van het componentserienummer ",
    edit: "Wijzigen"
};


const ModelDetail = {
    back: "Terug",
    delete: "Verwijderen",
    edit: "Wijzigen",
    copy: "Kopiëren",
    name: "Naam",
    brand: "Merk",
    partNo: "Artikelnummer",
    description: "Omschrijving",
    deleteModel: "Model verwijderen",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    download: "Downloaden",
    preview: "Voorbeeld",
    cancel: "annuleren",
    confirm: "Bevestigen",
    view: "Contoleren",
    bom: "BOM",

    specification: "Specificatie",
    currency: "Valuta",
    year: "Jaar",
    color: "Kleur ",
    size: "Afmeting",
    gender: "Geslacht",
    price: "Prijs",

    link: "Link",
};

const ModelList = {
    brand: "Merk",
    addModel: "Model toevoegen",
    noResult: "Geen resultaat！",
    total: "Totaal",
    deleteModel: 'Model verwijderen',
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    edit: "Wijzigen",
    delete: "Verwijderen",
    detail: "Detail",
    tableHead: {
        name: "Naam",
        logo: "Logo",
        partNo: "Artikelnummer",
        introduction: "Introductie",
        operation: "Operatie",
        show: "Show"
    },
    deleteSuccess: "Succesvol verwijderd",
    link: "Link",
    op: Operation,
};

const ModelSave = {
    uploadTipOne: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
    uploadTipTwo: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB ",
    add: "Toevoegen",
    edit: "Wijzigen",
    copy: "Kopiëren",
    back: "Terug",
    save: "Opslaan",
    emptyTip: {
        picture: "Graag het modellogo uploaden.",
        name: "Graag de modelnaam opgeven.",
        partNo: "Graag het artikel nummer opgeven.",
        desc: "Graag de modelomschrijving opgeven.",
    },
    saveTip: "Graag wachten op opslaan.",
    brandWrong: "Het merk is verkeerd kies opnieuw het juiste merk.",
    saveSuccess: "Succesvol opgeslagen",
    picture: "Afbeelding",
    name: "Naam",
    partNo: "Artikelnummer",
    introduction: "Introductie",
    description: "Omschrijving",
    cancel: "Annuleren",
    confirm: "Bevestigen",
    editAttr: "Eigenschappen bewerken",
    attrList: "Eigenschapslijst",

    specification: "Specificatie",
    currency: "Valuta",
    year: "Jaar",
    color: "Kleur ",
    size: "Afmeting",
    gender: "Geslacht",
    price: "Prijs",
    brand: "Merk",
    link: "Link",
};


const VehicleBomList = {
    back: "Terug",
    add: "Toevoegen",
    save: "Opslaan",
    edit: "Wijzigen",
    print: "Afdrukken",
    export: "Exporteren",
    delete: "verwijderen",
    detail: "Detail",
    articleNO: "Artikelnummer",
    noPhoto: "Geen afbeelding",
    viewLargerImage: "Bekijk groter beeld",
    list: "Lijst",
    partName: "Modelnaam/Artikelnummer",
    yes: "Ja",
    no: "Nee",
    listHead: {
        itemNo: "Item NR.",
        partNo: "Artikel nummer",
        picture: "Afbeelding",
        name: "Naam",
        spec: "Afmeting",
        package: "Verpakking",
        packageSize: "Afmeting pakket",
        price: "Prijs",
        linkBom: "LINK BOM",
        warranty: "Garantie",
        operation: "Operatie"
    },
    selectHead: {
        partNo: "ARTIKELNUMMER",
        picture: "Afbeelding",
        name: "Naam",
        spec: "Specificaties",
        package: "Pakket",
        packageSize: "Afmeting pakket",
        price: "Prijs",
        linkBom: "LINK BOM"
    },
    deleteProductDialog: {
        title: "Product verwijderen",
        content: "Weet u het zeker dat u dit wilt verwijderen? ",
    },
    addItemDialog: {
        title: "Artikel toevoegen",
    },
    operation: "Onderneming",
    selectCategory: "Categorie selecteren",
    category: "Categorie",
    search: "Zoeken",
    clear: "Verwijderen",
    tip: "Tip",
    content01: " U kunt het beste een vierkante afbeelding met achtergrond uploaden.",
    content02: " ondersteuning voor jpg, png en 300 * 300/400 * 400, tot 3 MB "
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
    add: "Toevoegen",
    self: "Zelf",
    manager: "Manager",
    permission: "Toestemming",
    reset: "Reset",
    delete: "Verwijderen",
    total: "Totaal",
    addAccount: "Account toevoegen",
    resetPassword: "Opnieuw instellen Wachtwoord",
    deleteAccount: "Account toevoegen",
    email: "E-mail",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    resetPasswordTip: " Wilt u het wachtwoord voor dit account instellen op 12345678?",
    tableHead: {
        account: "Account",
        createTime: "Tijd",
        operation: "Operatie",
    },
    emptyEmailTip: "Graag email ingeven.",
    addSuccess: "Succesvol toegevoegd",
    deleteSuccess: "Succesvol verwijderd",
    resetSuccess: "Reset successvol",
    saveAuthSuccess: "Opslaan authorisatie succesvol",
    noResult: "Geen resultaat！",
    invitationLength1: "The role of Dealer could only create up to 10 sub-accounts",
    invitationLength2: "The role of Assembler could only create up to 20 sub-accounts",
    invitationLength3: "The role of Brand could only create up to 20 sub-accounts",
    invitationLength4: "The role of Component could only create up to 20 sub-accounts",

    cNam: AttributeName
};

const ComponentsReplaceReport = {
    tableHead: {
        category: "Categorie",
        oldComponent: "Oud onderdeel SN",
        newComponent: "Nieuw onderdeel SN",
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    back: "Terug"
};

const report = {
    back: "Terug",
    diagnosisReportTitle: "E-BIKE Diagnose Report",
    print: "Afdrukken",
    orgLabel: "Org ",
    orgTitle: "Org Informatie ",
    dealerLabel: "Dealer ",
    dealerTitle: "Dealer Informatie ",
    assemblerLabel: " assemblage werknemer ",
    assemblerTitle: "Assemblage informatie ",
    name: "Naam",
    address: "Adres",
    email: "E-mail",
    tel: "Telefoon.",
    reportDate: "Report Datum",
    bike: {
        title: "Fiets informatie"
    },
    diagnosis: {
        data: "Diagnose Data",
        result: "Diagnose Resultaat",
        hmi: "Diagnose Data - HMI",
        battery: "Diagnose Data - Battery",
        controller: "Diagnose Data - Controller",
        sensor: "Diagnose Data - Sensor",
    },
    cNam: AttributeName,

};

// components diagnosis
const diagnosis3rd = {
    goElectronic: "Wilt u een elektronische diagnose stellen?",
    goMechanic: "Wilt u een mechanische diagnose stellen?",
    diagnosisElectronic: "Elektronische diagnose ",
    diagnosisMechanic: "Mechanische diagnose ",
    eBikeID: "E-Bike-ID",
    connection: "Verbinding",
    newVer: "Nieuwe versie",
    ok: "OK",
    fail: "Mislukt",
    con: "Verbonden",
    disCon: "Losgekoppeld",
    readOk: "Goed gelezen",
    readFail: "Lezen mislukt",
    detail: "Detail",
    testSys: "Test Systeem",
    cNam: AttributeName,
    rp: report,
    op: Operation,
    step: "Stap",
    help: "Help",
    continue: "Doorgaan",
    case: "Geval",
    test: "Test",
    finish: "Voltooien",
    name: "Naam",
    remark: "Opmerking",
    desc: "Omschrijving",
    target: "Doel",
    solution: "Oplossing",
    errTitle: "Error Oplossing",
    reconnect: "Opnieuw verbinden",
    instruction: "Instructie",

    pedalSensor: "Pedaal Sensor",
    throttle: "Gashendel",
    brake: "Rem",
    light: "Verlichting",
    support: "Ondersteunings modus",
    walk: "Meeloop functie",
    speed: "Snelheidssignaal",
    cadence: "Cadans sensor",
    torque: "Torque sensor",
    motor: "Motor",

    click: "Klik",
    step11: "Verbind de BESST Tool met uw computer.",
    step12: "Selecteer de juiste COM-poort. ",
    step13: "Het licht symbool wordt groen. ",
    step21: "Selecteer de onderdelen van de E-bike.",
    step22: "Verbind de BESST Tool met het display en de controller.",
    step23: 'Press HMI power button to switch HMI to "PC" mode',
    step232: 'Druk vier seconden op de aan / uit knop van het display om het systeem in te schakelen',
    case11: "Het display kan niet worden ingeschakeld:",
    case111: "Het display is niet ingeschakeld:",
    case12: "Controleer de kabelverbinding. Is het nog steeds niet mogelijk om het display in te schakelen dan heeft u een nieuwe display nodig om verder te gaan.",
    case121: "Controleer de kabelverbinding. Koppel het display los en sluit deze opnieuw aan.",
    case13: 'Het display geeft geen PC modus..',
    case17: "Druk",
    case18: 'op de BESST tool en schakel het display opnieuw in. Je kunt dit een paar keer herhalen.',
    case181: "'op de BESST tool. Is het nog steeds niet mogelijk om het display in te schakelen dan heeft u een nieuwe display nodig om verder te gaan. ",
    case20: 'Wanneer er nog steeds geen PC modus in het display verschijnt, ondersteunt het BESST niet en u heeft u een nieuw BESST compatible display nodig om verder te gaan.',
    case21: ' Opmerking: als u het display veranderd om verder te gaan, klik dan op "Lezen" om opnieuw te verbinden.',
    test11: " Hang de fiets op om letsel of schade te voorkomen.",
    test12: " Zorg ervoor dat het wiel vrij kan draaien voordat u op de knop drukt!",
    test13: "Selecteer de functies",
    test14: 'Selecteer "Alles" of "S", "Alles" voor de algehele system test, "S" om de componenten één voor één te testen; ',
    test15: "Start het test process.",
    createReport: "Rapport maken",

    start11: "Draai de gashendel, dan motor zou moeten werken, en ook moet je set groene licht zien.",
    start13: "Als motor werkt, maar u ziet geen groen licht, de gashendel is OK. Anders klikt u op",
    start14: " en vervangen het onderdeel.",
    start15: "Gelieve minstens twee maal de remhevel inknijpen, dan moet je groen licht zien.",
    start16: "Als de motor stopt maar je krijgt geen groen licht is de rem sensor ok. Anders, klikt u op ",
    start17: ' Houd de knop ingedrukt " ',
    start18: ' " op het display, dan zou het licht aan of uit moeten gaan, en ook zou je het groene licht moeten zien.',
    start19: " Als de verlichting niet werkt, maar u ziet nog steeds het groene lampje, vervang dan de verlichting. Als er geen verlichting brandt of als u geen groen licht ziet, klik dan ",
    start20: "en vervang dhet display of controller.",
    start21: ' Gelieve afwisselend te drukken op de "+" en "-", dan zou u het groene licht moeten zien.',
    start22: "Als er geen groen lampje brandt, klikt u op",
    start23: "en vervang het display.",
    start24: ' Houd de knop ingedrukt "',
    start25: ' " or "-", dan is de motor zou moeten werken bij lage snelheid, en ook moet je het groene licht zien.',
    start26: "Draai het wiel naar voren met de hand, dan zul je de snelheid hierboven zien en het groene licht krijgen.",
    start27: "Als er geen groen licht brandt, controleer of de magneet in de juiste positie is geplaatst. Als het nog steeds niet werkt, klikt u op",
    start28: "en vervang dan de sensor en de magneet.",
    start29: " Hang de fiets op om letsel of schade te voorkomen.",
    start30: "Zorg ervoor dat het wiel vrij is om te roteren!",
    start31: "Draai het pedaal vooruit met de hand, dan moet je het groene licht te zien.",
    start33: "Start Torque",
    start34: "Knijp alsjeblieft de remhendel in en druk met ongeveer 10kg op het pedaal, daarna zou je het groene licht moeten zien.",
    start35: "en vervang de torque sensor.",
    start36: "< Terug",
    start37: "Volgende >",

    toolTitle1: "Eerste versie BESST tool ",
    toolTitle2: "PC modus",
    tool11: " Ondersteunt alleen Uart-protocol met Mini-USB-poort aan de ene kant en twee USB Type B-poorten aan de andere kant. ",
    tool12: ' "PC" modus wordt gebruikt om gegevens van het scherm te lezen in vergelijking met de normale modus. Wanneer het display verbinding maakt met het BESST-programma en kan worden ingeschakeld, gaat het display naar de "PC" -modus.',
    tool13: "LCD Display",
    tool14: ' Het scherm toont "PC" of "Verbonden OK" terwijl deze inschakelt is.',
    tool15: "LED Display",
    tool16: " De LED van het hoogste ondersteuningsniveau brandt als u het display inschakelt.",

    updateTitle: 'Software Versie',
    updateHistory: 'Software History',
    update111: "Schakel tijdens het downloaden van Firmware naar de BESST-box het display uit.",
    update1111: "Schakel tijdens het downloaden van de Firmware naar BESST-box het display uit.",
    update112: "Koppel de controller kabel los tijdens het downloaden van firmware naar BESST-box.",
    update11: " Als u de firmware met succes hebt gedownload naar tool,",
    update12: 'schakel vervolgens het display in totdat u "BEEP" hoort en het display is uitgeschakeld.',
    update122: 'sluit vervolgens de HMI aan totdat u "BEEP" hoort en het display is uitgeschakeld.',
    update13: "vervolgens is de update voltooid.",
    update14: "Druk op de resetknop van de BESST-box.",
    update15: "Zet het display aan.",
    update16: ' verbind dan de controller totdat je de "pieptoon" hoort  .',
    update17: "over vervolgens de update uit en ontkoppel de controller kabel.",
    update19: "Schakel de controller opnieuw in.",
    update20: ' totdat u " pieptoon " hoort, wordt de update uitgevoerd.',

    error: "Error",
    errorDesc: "Geen gegevens verzameld. Het is mogelijk een verbindingsfout. ",
    errorNote: ' Klik op " opnieuw verbinden " om opnieuw verbinding te maken. ',
    errorHmi: ' SN is leeg. HMI is niet geregistreerd in BESST. Gelieve opnieuw te verbinden om te controleren of HMI in de "PC"-modus gaat.',
    errorController: "SN is leeg. Controller is niet geregistreerd in BESST. Gelieve opnieuw aansluiten of testen in de gereedschap sectie.",
    errorBattery: "SN is leeg. Batterij is niet geregistreerd in BESST. Gelieve opnieuw aansluiten of testen in de gereedschap sectie.",
    errorSensor: "Sn is empty. Sensor is not registered on BESST. Please reconnect or test on Tool section.",

    goTest: "Gaat succesvol in de testmodus.",

    resultTitle: "E-BIKE Diagnose Rapport",
    resultBike: "E-BIKE Model:",
    print: "Print",
    bikeId: "E-Bike ID",
    key: "Sleutel code",
    result: "Resultaat",
    saveReport: "Rapport opslaan",
    offLineTip: " In de offline modus kunt u de voertuig-SN typen en het rapport afdrukken. Klik op het pictogram afdrukken bovenaan deze pagina.",
    motorSNTip: " De invoer van het motor SN is anders dan de gegevens horen bij de fiets.",
    bikeSNTip1: "Type fiets ID.",
    bikeSNTip2: "Fiets SN is veranderd. Gelieve te controleren.",

    canStep1: "Wacht een moment aub...",
    canStep2: "Het commando om de controller uit te schakelen is succesvol verzonden.",
    canStep3: "Kon de opdracht voor het uitzetten van de controller niet verzenden. Probeer dit opnieuw te testen.",
    canStep4: "De controller is uitgeschakeld. Druk op het display de aan / uit-knop meer dan 3 s om de controller in te schakelen",
    canStep5: "De controller is ingeschakeld. Sluit dit venster om verder te gaan.",
    selectDevice1: "Selecteer een component. Het display en controller zijn noodzakelijk",
    selectDevice2: "Er is geen beschikbaar apparaat geselecteerd",

    failDesc: "Hoofdgegevens worden niet verzameld. Dit kan een verbindingsfout of softwareprobleem zijn.",
    failNote: 'Klik op "Opnieuw verbinden" om opnieuw verbinding te maken.',
    failNoteMotor: 'The motor information is empty because no bike traced.',

    analogLight: 'Communicatie-indicator',
    communicationOk: 'Communicatie is normaal',
    brake01: 'Controleer of de remmen van de E-bike zijn uitgerust met een uitschakelfunctie. Als de remmen deze functie niet hebben, klik dan op "Volgende." ',
    brake02: 'Houd de remhendel vast en laat hem los, herhaal dit meerdere keren, observeer het indicatielampje. Als deze groen en grijs wordt selecteer " Rem stop functie werkt normaal", anders "Rem stop functie is niet werkend." ',
    brake06: 'Als het indicatielampje niet groen wordt of het indicatielampje niet grijs wordt nadat het groen wordt, klikt u op "Mislukt" ',
    brake07: " Rem stop functie functioneert normaal ",
    brake08: " De rem stop functie functioneert niet ",
    light01: "De knop voor de verlichting is ",
    light02: 'Druk op de verlichtingsknop om deze in en uit te schakelen. Selecteer "De indicator functioneert normaal" en "De Indicator functioneert niet". ',
    light022: 'Controleer of de verlichting normaal in- en uitgeschakeld kan worden, selecteer "De verlichting functioneert normaal", selecteer anders "De verlichting functioneert niet goed" ',
    light03: 'Controleer of de E-bike een verlichting configuratie heeft, zo niet, druk dan op "Volgende"',
    light04: 'Communicatie indicator oordeel.',
    light05: 'Functionaliteit verlichting.',
    light06: 'Als het indicatielampje niet groen wordt of het indicatielampje niet grijs wordt nadat het groen wordt, klikt u op "Mislukt". ',
    light07: 'De indicator functioneert normaal',
    light08: 'De indicator functioneert niet',
    light09: 'De verlichting functioneert normaal',
    light10: 'De verlichting functioneert niet.',
    support02: 'Druk op de knop "+/-" om twee keer door het laagste niveau naar het hoogste niveau te gaan, kijk of dit venster en het display consistent zijn. Vervolgens kunt  u "Functioneert normaal" of " Functioneert abnormaal " selecteren. ',
    support03: ' Functioneert normaal',
    support04: ' Functioneert abnormaal',
    speed01: 'Houd het systeem ingeschakeld, observeer de positie van de snelheidssensor en draai het wiel volgens de richting van de E-bike. Let op de snelheid van bovenstaand venster en het display. Druk op "Snelheidssensor functioneert normaal", anders "Snelheidssensor functioneert niet." ',
    speed04: 'De snelheidssensor functioneert normaal',
    speed05: 'De snelheidssensor functioneert niet.',
    cadence01: 'Stel het niveau in boven het 1e niveau, draai het pedaal volgens de rijrichting, het observatievenster moet de snelheid weergeven, houd 20RPM langer dan 10 seconden op een constante snelheid, het zal automatisch beoordelen dat "cadanssensor normaal functioneert", druk anders op "Cadanssensor functioneert niet normaal." ',
    cadence03: 'Cadans sensor functioneert niet normaal',
    torque01: 'Stel het niveau in boven de 1e versnelling, draai snel het pedaal volgens de rijrichting, het observatievenster zal ten minste groter dan 0,75 V, minder dan 5 V weergeven en deze zal veranderen aan de hand van de uitgeoefende kracht, de waarde is hoger dan 0,75 V, het systeem zorgt automatisch voor een gekwalificeerde functie, anders drukt u op "Mislukt". ',
    torque02: 'Als u bevestigt dat het voertuig een torque sensor gebruikt, selecteer dan "T" ',
    walk01: 'Houdt de display knop vast',
    walk02: '  /"-" gedurende drie seconden of langer, als de motor draait en de snelheid lager is dan 6 km / h, zal het systeem automatisch beoordelen of deze goed functioneert, klik anders op "WALK communicatiefout." ',
    walk03: 'Druk',
    walk04: ' /"-" en kijk of het observatievenster "WALK" toont, anders drukt u op "WALK communicatiefout" ',
    walk05: 'WALK communicatiefout',
    walk06: 'If the bicycle has a flywheel on the rear wheel, place the chain on the smallest chain ring.',
    motor01: 'In de cadansmodus, als de motorstroom lager is dan 3A, zal BESST de motor markeren als OK.',
    motor02: ' "Loop" modus, als de motor stroom is minder dan 3A, zal BESST de Loop functie markeren als OK.',
    motor03: 'Als er nog steeds geen indicatie is dat de motor normaal functioneert, plaatst u het aandrijfwiel van het voertuig in een gedempte maar roteerbare staat, vervolgens start u de motor met het pedaal, let u op de normale waarde van de stroom, snelheid en cadans of torque spanning informatie, het systeem beoordeelt automatisch "motor werkt normaal" en anders "Mislukt". ',

    defectDes: "Problem description",
    defects: "Mechanical defect",
    image: "Image",
    video: "Video",
    videoMaxSize: "Video maximum size is 10M",
    defectLength: "Maximum 600 character including space",

    notice19_00: "No more reminders in 7 days",
    notice19_01: "Eerklaring voor wijzigingen na 2019.12.19",
    notice19_02: "De diagnosefunctie past mogelijk niet goed bij sommige CAN-producten die vóór oktober 2019 zijn geproduceerd.",
    notice19_03: "Vanwege het verschil tussen de hardware van de servicetool, is er enig bedieningsverschil bij gebruik van servicetool met hardwareversie van 'BESST.UC 3.0'",
    notice19_04: "BESST moet aangepast worden om het verschil tussen de hardware versies van de service tool te verwijderen.",
    notice19_05: "Note: if you continually diagnosis a CAN system, you must shutdown power of controller over 20 seconds every time.",

};


const diagnosisNormal = {
    com: "COM",
    select: "Selecteren",
    connect: "Verbinden",
    readAgain: "Opnieuw lezen",
    refresh: "vernieuwen",
    tip: "Tip",
    saveReport: "Rapport opslaan",
    sendReport: "Rapport versturen",
    selectError: "Selecteer error",
    tipContent01: " Controleer de volgende basisinformatie",
    tipContent02: " Controleer de correctheid van fiets ID.",
    title: {
        name: "Naam",
        sn: "Serie Nummer",
        diagnosisResult: "Resultaat van de diagnose",
        mechanicalFailure: "Mechanische error",
        serviceBtn: "Service",
    },
    bike: "E-Bike",
    sn: "Seriennummer",
    confirm: "Bevestiging",
    cancel: "Annuleren",
    solution: "Oplossing",
    hmiDefectOptions: {
        displayNotWork: "Display functioneerd niet",
        displayWaterInside: "Display met vocht problemen",
        displayBtnDamage: "Display knoppen beschadigd",
        displayCableDamage: "Display kabel beschadigd",
        displayScreenDamage: "Display scherm beschadigd",
        displayBracketDamage: "Display bracket beschadigd",
        displayScrewLoosen: "Display bouten los",
        displayBlackScreen: "Display scherm zwart",
        displayMessyCode: "Display onduidelijke code",
    },
    controllerDefectOptions: {
        noAssistance: "Geen ondersteuning",
        noLightFunc: "Geen verlichting",
        brakeCannotCutOffPower: "Rem stop functie werkt niet",
        tripDistanceShort: "Te weinig actieradius",
    },
    batteryDefectOptions: {
        batteryDamage: "Batterij beschadigd",
        chargerNotWork: "Oplader werkt niet",
        BatteryCannotCharge: "Batterij kan niet opgeladen worden",
    },
    motorDefectOptions: {
        motorNotWork: "De motor functioneerd niet maar het display werkt naar behoren",
        motorShake: "Motor Vibreert",
        motorNoisy: "Motor maakt geluid",
        motorLessPower: "Motor heeft geen kracht",
        motorRunIntermittently: "Motor ondersteunt af en toe",
        noFunction: "Geen 6KM/H functie",
    },
    defectOptions: {
        motorNotWork: "Motor ondersteunt niet ",
        motorNoisy: "Motor maakt geluid",
        motorDamage: "Motor beschadigd",
        hmiFailed: "Display werkt niet",
        hmiErrorCode: "Display heeft een error code",
        hmiDamage: "Display is beschadigd",
        disorderedIndication: "Onduidelijke indicatie",
        outOfControl: "Buiten werking",
        eBrakeFailed: "Rem stop werkt niet ",
        eBrakeDamage: "Rem stop beschadigd",
        lightFailed: "Verlichting werkt niet",
        walkAssistanceFailed: "Geen loop ondersteuning",
        throttleFailed: "Gashendel werkt niet",
        throttleDamage: "Gashendel beschadigd",
        shortRange: "Korte actieradius",
        cableDamage: "Kabel beschadigd",
        batteryProblem: "Batterij probleem",
        batteryDamage: "Batterij beschadigd"
    },
    noDefect: "Geen defect",
    read: "Lezen",
    test: "TEST",
    errorSolutionDialog: {
        title: "Error oplossing",
        name: "Naam",
        remark: "Opmerking",
        description: "Omschrijving",
        target: "Doel",
        solution: "Oplossing",
    },
    connectTipVideoDialog: {
        title: "Verbind tip video",
        tip: " Uw browser ondersteunt dit video bestand niet."
    },
    connectTipDialog: {
        title: "Verbind tip",
        tip: " Uw browser ondersteunt dit video bestand niet.",
        content: " Verbind aub BESST met het display en controllerkabel en schakel de display in, en controleer vervolgens of het display \"PC \" (LCD-type) of het LED-lampje Topmodus (LED-type) toont"
    },
    confirmSnDialog: {
        title: "Bevestig SN",
        content: " Controleer de sn op het voertuig om te bepalen of het goed is "
    },
    print: "Afdrukken",
    bikeInfo: "Fiets Info",
    diagnosis: {
        result: "Diagnosis Resultaat",
        hmi: " Display",
        battery: "Batterij",
        controller: "Controller",
        rpmSensor: "RPM Sensor",
        pedalSensor: "Pedal Sensor",
        motor: "Motor",
    },
    emptySNTip: "Fiets SN leeg",
    emptySNText: " Fiets SN is leeg en het sn verschijnt in het rapport, ga verder naar de testweergave?",
    back: "Terug",
    storeInfo: "Dealer informatie",

    preTipA: 'Schakel HMI in, BOX zal tweemaal "PIEPEN", klik dan op',
    continue: "Doorgaan",
    preTipB: "Als er geen 'piep' is, klik dan op ",
    here: "Meer"

};

const diagnosisTest = {
    back: "Terug",
    driveSystemTest: "Drive System Test",
    hmiMode: "Ondersteunings stand",
    currentMode: "Huidige ondersteunings stand",
    light: "Verlichting",
    title: {
        name: "Naam",
        sn: "Serie Nummer",
        diagnosisResult: "Diagnose Resultaat",
        mechanicalFailure: "Mechanisch probleem",
    },
    help: "Help",
    save: "Opslaan",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    hmiDefectOptions: {
        displayNotWork: "Display functioneerd niet",
        displayWaterInside: "Display met vocht problemen",
        displayBtnDamage: "Display knoppen beschadigd",
        displayCableDamage: "Display kabel beschadigd",
        displayScreenDamage: "Display scherm beschadigd",
        displayBracketDamage: "Display bracket beschadigd",
        displayScrewLoosen: "Display bouten los",
        displayBlackScreen: "Display scherm zwart",
        displayMessyCode: "Display onduidelijke code",
    },
    controllerDefectOptions: {
        noAssistance: "Geen ondersteuning",
        noLightFunc: "Geen verlichting",
        brakeCannotCutOffPower: "Motor stop werkt niet",
        tripDistanceShort: "Actieradius te kort",
    },
    batteryDefectOptions: {
        batteryDamage: "Batterij beschadigd",
        chargerNotWork: "Lader werkt niet",
        BatteryCannotCharge: "Batterij kan niet opgeladen worden",
    },
    motorDefectOptions: {
        motorNotWork: "De motor functioneerd niet maar het display werkt naar behoren",
        motorShake: "Motor Vibreert",
        motorNoisy: "Motor maakt geluid",
        motorLessPower: "Motor heeft geen kracht",
        motorRunIntermittently: "Motor ondersteunt af en toe ",
        noFunction: "Geen 6KM/H functie",
    },
    defectOptions: {
        motorNotWork: "Motor ondersteunt niet ",
        motorNoisy: "Motor maakt geluid",
        motorDamage: "Motor beschadigd",
        hmiFailed: "Display werkt niet",
        hmiErrorCode: "Display heeft een error code",
        hmiDamage: "Display is beschadigd",
        disorderedIndication: "Onduidelijke indicatie",
        outOfControl: "Buiten werking",
        eBrakeFailed: "Rem stop werkt niet ",
        eBrakeDamage: "Rem stop beschadigd",
        lightFailed: "Verlichting werkt niet",
        walkAssistanceFailed: "Geen loop ondersteuning",
        throttleFailed: "Gashendel werkt niet",
        throttleDamage: "Gashendel beschadigd",
        shortRange: "Korte actieradius",
        cableDamage: "Kabel beschadigd",
        batteryProblem: "Batterij probleem",
        batteryDamage: "Batterij beschadigd"
    },
    noDefect: "Geen defect",
    selectError: "Selecteer Defect",
    errorSolutionDialog: {
        title: "Error Oplossing",
        name: "Naam",
        remark: "Opmerking",
        description: "Omschrijving",
        target: "Doel",
        solution: "Oplossing",
    },
};


const reportList = {
    tableHead: {
        vehicleSn: "Fiets SN",
        modelName: "Model Naam",
        time: "Tijd",
        operation: "Onderneming"
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    search: {
        vehicleSn: 'Fiets SN',
        searchBtn: "Zoeken"
    },
    deleteDialog: {
        title: "Verwijder Rapport",
        content: "Weet u zeker dat u dit wilt verwijderen?"
    },
    delete: "Verwijderen",
    yes: "Ja",
    no: "Nee"
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
    back: "Terug",
    add: 'Toevoegen',
    reset: "Reset",
    delete: "Verwijderen",
    tableHead: {
        account: "Account",
        createTime: "Tijd",
        operation: "Bedrijf",
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    yes: "Ja",
    no: "Nee",
    addAccountDialog: {
        title: "Account toevoegen",
        email: "E-mail",
        password: "Wachtwoord"
    },
    deleteDialog: {
        title: "Account verwijderen",
        content: "Weet u zeker dat u het wilt verwijderen?"
    },
    resetPasswordDialog: {
        title: "Reset Wachtwoord",
        content: "Wilt u het wachtwoord voor dit account instellen op 123456?"
    }
};

const centerTicketList = {
    serviceCenter: "Service Center",
    category: "Categorie",
    status: "Status",
    look: "Kijken",
    noResult: "Geen resultaat！",
    total: "Total",
    tableHead: {
        serviceNo: "Service Nr.",
        title: "Titel",
        targetSn: "Doel serienummer",
        createTime: "Tijd",
        status: "Status",
        operation: "Operatie"
    },
    categoryList: {
        all: "Alle",
        vehicle: "Voertuig",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Batterij",
        light: "Licht",
        controller: "Controller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        pendingAssign: "Te distribueren",
        processing: "In bewerking",
        complete: "Voltooid"
    }
};

const myTicketList = {
    category: "Categorie",
    status: "Status",
    look: "Bekijken",
    tableHead: {
        serviceNo: "Service Nr.",
        category: "Categorie",
        title: "Titel",
        targetSn: "Kijken",
        createTime: "Tijd",
        status: "Status",
        operation: "BOperatie",
    },
    categoryList: {
        all: "Alle",
        vehicle: "Fiets",
        motor: "Motor",
        hmi: "Display",
        sensor: "Sensor",
        battery: "Batterij",
        light: "Licht",
        controller: "Controller",
        other: "Andere",
    },
    statusList: {
        all: "Alle",
        processing: "Verwerking",
        complete: "Voltooid",
        handleToAnother: "Overdragen aan een ander",
    },
    noResult: "Geen resultaat！",
    total: "Totaal",
    addTicketDialog: {
        dialogTitle: "Service-Ticket toevoegen",
        category: "Categorie",
        title: "Titel",
        description: "Omschrijving",
        bike: "E-Bike",
        sn: "Serienummer",
        serviceCenter: "Service Center",
        image: "Afbeelding",
        video: "Video",
        clickToUpload: "Klik om te uploaden"
    },
    confirm: "Bevestigen"

};

const serviceCenterList = {
    serviceCenter: "Service Centers",
    add: "Toevoegen",
    edit: "Bewerken",
    account: "Account",
    noResult: "Geen resultaat！",
    total: "Totaal",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    yes: "Ja",
    no: "Nee",
    tableHead: {
        logo: "Logo",
        name: "Naam",
        phone: "Telefoon",
        email: "E-mail",
        country: "Land",
        address: "Adres",
        operation: "Operatie",
    },
    deleteDialog: {
        title: "Service centrum verwijderen",
        content: "Weet u zeker dat u dit wilt verwijderen?"
    },
    createSC: "Service centrum maken",
    editSC: "Service Centrum bewerken",
    editDialog: {
        picture: "Afbeelding",
        tip: "Tip",
        content01: "U kunt beter een vierkante afbeelding uploaden met achtergrond",
        content02: "Ondersteunt jpg, PNG en 300 * 300/400 * 400, tot MB",
        name: "Naam",
        email: "E-mail",
        phone: "Telefoon",
        website: "Website",
        address: "Adres",
        postcode: "Postcode",
    },
    detailDialog: {
        title: "Detail",
        picture: "Afbeelding",
        name: "Naam",
        email: "E-mail",
        phone: "Telefoon",
        website: "Website",
        address: "Adres",
        postcode: "Postcode",
        ticketCount: "Ticket aantal",
        aveHandleTime: "Gemiddelde verwerkingstijd",
    },
    op: Operation,
};

const ticketDetail = {
    back: "Terug",
    handleUp: 'Handle Up',
    history: "Geschiedenis",
    ticketInfo: {
        infoTitle: "Details van de klacht",
        ticketNo: "Ticket no.",
        createTime: "Tijd",
        status: "Status",
        serviceCenter: "Service Center",
        title: "Titel",
        desc: "Omschrijving",
        image: 'Afbeelding',
        video: "Video"
    },
    user: {
        address: "Adres",
        website: "Website",
        email: "E-mail",
    },
    replay: {
        content: "Inhoud",
        image: "Afbeelding",
        sendMessage: "Bericht verzenden"
    },
    confirm: "Bevestigen",
    cancel: "Annuleren",
    handleUpTicketDialog: {
        title: "Handle Up Ticket",
        serviceCenter: "Service Center"
    },
    historyDialog: {
        title: "Ticket geschiedenis",
        name: "Naam",
        operation: "Bedrijf",
        time: "Tijd"
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
    vehicle: "Fiets"
};

const AssemblerOrderDetail = {
    back: "Terug",
    orderDetail: "Order Details",
    edit: "Bewerken",
    delete: "Verwijderen",
    orderInfo: "Bestelinformatie",
    orderNo: "Bestel Nr.",
    customerName: "Klant naam",
    note: "Opmerking",
    poNo: "Po No.",
    itemsList: "Item lijst",
    produce: "Produceren",
    vehicleList: "Lijst van voertuigen",
    deleteOrder: 'Order verwijderen',
    deleteTip: "Weet u zeker dat u het wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    tableHead: {
        modelName: "Model",
        partNo: "Artikelnummer",
        quantity: "Hoeveelheid",
        operation: "Bedrijf",
    },
    replace: "Vervangen",
    confirmDialog: {
        replace: "Vervangen",
        yes: 'Ja',
        no: 'Nee'
    },
    itemHead: {
        image: "Afbeelding",
        name: "Naam",
        partNo: "Artikelnummer"
    },
    search: "Zoeken",
    brandPlaceholder: "Selecteer merk",
    operation: "Bedrijf",
    modelName: "Model",
};

const AssemblerOrderList = {
    add: "Toevoegen",
    noResult: "Geen resultaat！",
    vehicleList: "Lijst van voertuigen",
    detail: "Detail",
    total: "Totaal",
    tableHead: {
        date: "Datum",
        orderNo: "Order Nr.",
        poNo: "Po No.",
        customerName: "Klantnaam",
        model: "Model",
        produced_qty: "Gefabriceerd/hoeveelheid",
        status: "Status",
        operation: "Bedrijf"
    },
    poNo: "Po No.",
    search: "Zoeken",
    startDate: "Selecteer de begindatum",
    endDate: "Selecteer de einddatum",
    model: 'Model',
};

const AssemblerOrderProduce = {
    back: "Terug",
    orderInfo: "Bestelinformatie",
    brand: "Merk",
    model: "Model",
    poNo: "Po No.",
    produced_qty: "Gefabriceerd / hoeveelheid",
    pleaseSelect: "Selecteer a.u.b.",
    reconnect: "Opnieuw verbinden",
    openConnectTip: "Open connect tip",
    partName: "Naam",
    sn: "Serienummer",
    hw: "HW",
    sw: "SW",
    connectTipVideo: "Verbinden met Tip video",
    connectTip: "Verbinden met de tip",
    connectTipText: " Graag BESST met het display en controller kabel vervinden en zet het display aan, en controleer of het display PC (LCD-Typ) of bij het LED display de bovenste LED toont.",
    confirm: "Bevestigen",
    itemFinishTip: "Dit model is klaar",
    selectTip: "Gelieve COM te selecteren.",
    tip: 'Tip',
    batteryBindTip: "Wilt u met de batterij verbinden?",
    yes: "Ja",
    no: "Nee",
    goDiagnosisTip: "Wilt u overschakelen naar de diagnostische weergave?",
    bindBattery: "Batterij aansluiten",
    vehicleSn: "Voertuig sn",
    componentSn: "Component sn",
    videoTip: "Uw browser ondersteunt geen video-tag.",
    error: "Error",
    historyDialog: {
        title: "Software geschiedenis",
        version: "Versie",
        updateTime: "Update-tijd",
        remark: "Opmerking",
        operation: "Bedrijf",
        noResult: "Geen resultaat！"
    },
    update: "Update",
    reset: "reset",

    isUsed: "is gebruikt.",
    canNotUsed: "kan niet worden gebruikt.",
    inputBikeSn: "Voer het serienummer van de fiets in",
    inputMotorSn: "Voer het serienummer van de motor in.",
    inputHmiSn: "Voer het serienummer van het display in.",
    inputCtlSn: "Voer het serienummer van de Controller in.",
    inputBatSn: "Please input battery sn.",
    inputSensorSn: "Voer het serienummer van de sensor in.",

    bikeSnExt: "Het serienummer van de fiets bestaat reeds.",
    saveSnText: ' Het volgende onderdeel SN (serienummer) bestaat niet in BESST. Controleer het SN. Als alles in orde is, kun je het SN nog steeds aan de fiets binden door op "Opslaan" te klikken. Neem contact op met het fietsmerk of de leverancier van componenten als deze situatie zich blijft voordoen.',

    cNam: AttributeName
};

const AssemblerOrderSave = {
    back: "Terug",
    createOrder: "Order maken",
    editOrder: "Order bewerken",
    orderInfo: "Order serienummer",
    poNo: "Po No.",
    customerInfo: "Klant info",
    customerName: "Klantnaam",
    note: "Opmerking",
    itemList: "Item lijst",
    addItem: "Item toevoegen",
    delete: "Verwijderen",
    modelName: "Modelnaam",
    addItems: "Item toevoegen",
    saveOrder: "De taak opslaan",
    tableHead: {
        name: "Naam",
        partNo: "Artikelnummer",
        quantity: "Hoeveelheid",
        operation: "Bedrijf"
    },
    itemHead: {
        image: "Afbeelding",
        name: "Naam",
        partNo: "Artikelnummer"
    },
    search: "Zoeken",
    brandPlaceholder: "Selecteer merk"
};

const AssemblerPartnerList = {
    back: "Terug",
    acceptInvitation: "Uitnodiging accepteren",
    currentBrand: "Huidige merk",
    noResult: "Geen resultaat！",
    total: "Totaal",
    brandInfo: "Merk info",
    logo: "Logo",
    name: "Naam",
    type: "Type",
    brand: "Merk",
    email: "E-mail",
    website: "Website",
    tel: "Telefoon",
    postcode: "Postcode",
    address: "Adres",
    invitationCode: "Uitnodigingscode",
    confirm: "Bevestigen",
    cancel: "Annuleren",
    hoverTip: "Ingesteld als huidig merk",
    tableHead: {
        name: "naam",
        phone: "Telefoon",
        website: "Website",
        country: "Land",
        operation: "Bedrijf"
    },
    codeEmptyTip: "Gelieve code in te voeren ",
    acceptSuccess: "Met succes geaccepteerd"
};

const AssemblerVehicleDetail = {
    vd: VehicleDetail
};

const AssemblerVehicleList = {
    brand: "Merk",
    poNo: "Po No.",
    modelName: "Model",
    bomSn: 'Onderdeel SN',
    search: "Zoeken",
    export: "Exporteren",
    delete: "Verwijderen",
    detail: "Detail",
    total: "Total",
    deleteVehicle: "Verwijder het voertuig",
    deleteTip: "Weet u zeker dat u dit wilt verwijderen?",
    yes: "Ja",
    no: "Nee",
    back: "Terug",
    tableHead: {
        vehicleSn: "Voertuig SN",
        brandLogo: "Merklogo",
        brandName: "Merknaam",
        modelName: "Model",
        poNo: "Po No.",
        produceDate: "Productie datum",
        saleDate: "Verkoop datum",
        operation: "Bedrijf",
        noSale: "Niet verkocht"
    },
    noResult: "Geen resultaat！",
    deleteSuccess: "Succesvol verwijderd",
    noSale: "Niet verkocht",
    exportDialog: {
        title: "Voertuig exPorten",
        startNum: "Beginnummer",
        endNum: "Laatste nummer",
        export: "Exporteren",
        cancel: "Annuleren"
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
// ./src/assets/lang/nl.js