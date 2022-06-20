function Const() {
    return {
        ERROR: {
            ALREADY_EXISTS: 6,
            INVALID_USER_NAME: 5,
            ERROR_PASSWORD: 3,
            ERROR_NETWORK: -10000,
            ERROR_INVALID: -1,
            ERROR_PARAM_NOT_SET: 1,
            ERROR_TOKEN_INVALID: 2,
            ERROR_LOGIN_FAIL: 3,
            ERROR_WRONG_PARAM: 4,
            ERROR_NOT_EXIST: 5,
            ERROR_EXIST: 6,
            ERROR_ORG_NOT_EXIST: 7,
            ERROR_ORG_MEMBER_NOT_EXISTS: 8,
            ERROR_REGISTER: 9,
            ERROR_USER_NOT_EXISTS: 10,
            ERROR_PHONE_HAS_BEEN_TAKEN: 11,
            ERROR_BIND_USER_BIND_EXISTS: 12,
            ERROR_WRONG_TYPE: 13,
            ERROR_SAVE_ERROR: 14,
            ERROR_ACTION_NOT_ALLOWED: 15,
            ERROR_WRONG_VERIFICATION_CODE: 16,
            ERROR_SEND_PHONE_VCODE_TOO_OFTEN: 17
        },
        SYSTEM: {
            LOG_LEVEL_ERROR: 1,
            LOG_LEVEL_WARN: 2,
            LOG_LEVEL_INFO: 3,
            LOG_LEVEL_TRACE: 4,
            LOG_LEVEL_DEBUG: 5,

            VERSION: '1.2.26',


            VERSION_DATE: '2020-12-11'

        },

        NET: {

            // 线上
            END_POINT: 'http://test.api.besst.bafang-service.com/client/1',
            // END_POINT: 'http://yicksolutions.ddns.net:9898/client/1',
            // END_POINT: 'http://localhost:9898/client/1',
            // END_POINT: 'http://106.14.208.198:7109/client/1',
            // END_POINT: 'http://10.0.0.114:9898/client/1',
            // END_POINT: 'http://192.168.133.104:9898/client/1',
            // END_POINT: 'http://192.168.133.100:9898/client/1',
            // 外网
            // END_POINT: 'http://139.196.174.48:9898/client/1',
            // END_POINT: 'http://192.168.0.141:7109/client/1',
            // END_POINT: 'http://192.168.60.205:9898/client/1',
            // END_POINT: 'http://192.168.60.143:9898/client/1',
            // END_POINT: 'http://192.168.0.141:7109/client/1',  //本地test

            // END_POINT: 'http://192.168.0.184:7109/client/1',  //李恒
            // END_POINT: 'http://192.168.0.141:7109/client/1',
            // END_POINT: 'http://139.196.174.48:9898/client/1',
            // END_POINT: 'http://192.168.60.112:9898/client/1',

            VERSION: 1,
            CLIENT: 3,


            // IMG_UPLOAD_END_POINT: 'http://192.168.0.141:7109/file/img-upload',
            IMG_UPLOAD_END_POINT: 'http://test.api.besst.bafang-service.com/file/img-upload',
            // IMG_UPLOAD_END_POINT: 'http://106.14.208.198:7109/file/img-upload',
            // IMG_UPLOAD_END_POINT: 'http://139.196.174.48:9898/file/img-upload',
            // FILE_UPLOAD_END_POINT: 'http://192.168.0.141:7109/file/file-upload',
            FILE_UPLOAD_END_POINT: 'http://test.api.besst.bafang-service.com/file/file-upload',
            // FILE_UPLOAD_END_POINT: 'http://106.14.208.198:7109/file/file-upload',
            // FILE_UPLOAD_END_POINT:   'http://139.196.174.48:9898/file/file-upload',
            IMG_URL_PREFIX: 'http://static.besst.bafang-service.com/besst/img/',
            FILE_URL_PREFIX: 'http://static.besst.bafang-service.com/besst/file/',

            // UPLOAD_CONTACT_URL: 'http://192.168.0.237:7109/client/1/invite/upload-contact',
            UPLOAD_CONTACT_URL: 'http://test.api.besst.bafang-service.com/client/1/invite/upload-contact',
            UPLOAD_MATERIAL_URL: 'http://test.api.besst.bafang-service.com/client/1/material/upload-excel',
            // UPLOAD_MATERIAL_URL: 'http://58.211.213.54:9898/client/1/material/upload-excel',
        },

        DATA: {
            KEY_PREFIX: 'besst.data.',
            KEY_TOKEN: 'token',
            KEY_USER: 'user',
            KEY_ORG: 'org',
            KEY_SP: 'sp',
            KEY_LANG: 'lang',
            KEY_AUTH: 'auth',
            KEY_COMPONENT_ORDER_LIST_PAGE: 'component.order.list.page',
            KEY_COMPONENT_ORDER_LIST_CONDITION: 'component.order.list.condition',
            KEY_COMPONENT_COMPONENT_LIST_CONDITION: 'component.component.list.condition',
            KEY_COMPONENT_PROJECT_SELECT_SPEC: 'component.project.select.spec',
            KEY_ASSEMBLER_ORDER_LIST_PAGE: 'assembler.order.list.page',
            KEY_ASSEMBLER_VEHICLE_LIST_CONDITION: 'assembler.vehicle.list.condition',
            KEY_BRAND_VEHICLE: 'brand.vehicle',
            KEY_I18N: 'i18n',
            KEY_I18N_UPDATE: 'i18n.update',
            KEY_ADMIN_TYPE: 'admin.type',
            KEY_TEST_DATA: 'test',
            KEY_REPORT: 'report',
            KEY_OFFLINE: 'offline',
            KEY_PORT: 'port',

            KEY_READ_HMI_RESULT: 'read.hmi.result',
            KEY_READ_CONTROLLER_RESULT: 'read.controller.result',
            KEY_READ_BATTERY_RESULT: 'read.battery.result',

            KEY_OFFLINE_OBJECT_RECORD: 'offline.object.record',
            KEY_BATCH_RECORD: 'batch.save.record',
            KEY_DIAGNOSIS_TEST: 'diagnosis.test',
            KEY_DIAGNOSIS_CONNECTION: 'diagnosis.connection',

            KEY_MATERIAL_LINE: 'material.line',
            KEY_MATERIAL_STATUS: 'material.status',
            KEY_MATERIAL_SCAN_TEM: 'material.scan.tem',

            KEY_AFTER_SALES_PAGE: 'after.sales.page',
            KEY_UPDATE_COUNT: 'update.count',
            KEY_TRY_ACCOUNT: 'try.account',
            KEY_TEM_ACCOUNT: 'tem.account',
            KEY_DIG_REMIND: 'try.dig.remind',

        },

        COMPONENT_ROOT_CATEGORY: 2,
        PAGE_SIZE: 10,
        BAFANG_ORG_ID: 25,

        EVENT_HUB: {
            REFRESH_ITEM_LIST_BY_CATEGORY_ID: 'refresh_item_list_by_category_id',
            REFRESH_PARTNER_BRAND_ICON: 'refresh_partner_brand_icon',
            NO_AUTH: 'no_auth',
            OPEN_MENU: 'open_menu',
            DIAGNOSIS: 'event_diagnosis',
            DIAGNOSIS_TEST: 'event_diagnosis_test',
            ORG_CHANGE: 'org_change',
        },

        //公司type
        ORG_TYPE: {
            'BRAND': 1,
            'DEALER': 2,
            'OEM': 3,
            'COMPONENT': 4,
            'SERVICE_CENTER': 5,
            'SPECIAL_DEAlER': 6
        },

        ORG_CATEGORY_TYPE: {
            1: 'BRAND',
            2: 'DEALER',
            3: 'OEM',
            4: 'COMPONENT',
            5: 'SERVICE CENTER',
            6: 'SPECIAL DEAlER'
        },

        CATEGORY_ROOT_TYPE: {
            'BIKE': 1,
            'MOTOR': 3,
            'HMI': 4,
            'BATTERY': 5,
            'SENSOR': 6,
            'CONTROLLER': 7,
            'CONNECTOR': 8,
            'CHARGER': 9,
            'TOOL': 10
        },

        CATEGORY_ROOT: {
            3: 'Motor',
            4: 'HMI',
            5: 'Battery',
            6: 'Sensor',
            7: 'Controller',
            8: 'Connector',
            9: 'Charger',
            10: 'Tool'
        },

        OFFLINE_ORG: [
            65,
            356
        ],

        // 语言类型
        LANGUAGE_TYPE: {
            2: 'German',
            3: 'English',
            1: 'Chinese',
            4: 'Dutch'
        },

        // 货币符号
        CURRENCY_TYPE: {
            1: '€',
            2: '$',
            3: 'DM.',
            4: '￥',
            5: '￡'
        },

        //pack unit类型
        PACK_UNIT: {
            1: 'PCS/box',
            2: 'PCS/carton',
            3: 'PCS/bag',
        },

        // 订单状态
        ORDER_STATUS: {
            en: {
                '0': 'In Process',
                '1': 'Finished',
                '-1': 'All',
            },
            zh: {
                '0': '处理中',
                '1': '完成',
                '-1': '所有',
            },
            de: {
                '0': 'Im Gange',
                '1': 'Fertig',
                '-1': 'Alles',
            },
            nl: {
                '0': 'In behandeling',
                '1': 'Klaar',
                '-1': 'Allemaal',
            }
        },

        //测试反馈状态
        FILE_STATUS: {
            '0': 'Urgent',
            '1': 'Wait',
            '2': 'Finished',
        },

        //分组属性input_type:
        CATEGORY_ATTR_INPUT_TYPE: {
            'TEXT': 1, // 文本框
            'SELECT': 2, // 选择
            'RADIO': 3, // 单选
            'CHECKBOX': 4, // 多选
            'FILE': 5, // 单文件
            'FILES': 6, // 多文件
            'PIC': 7, // 单图片
            'PICS': 8, // 多图片
            'TEXTARE': 9 // 文本域
        },

        TICKET_TYPE: {
            en: {
                '0': 'INIT',
                '100': 'PENDING ASSIGN',
                '200': 'PROCESSING',
                '300': 'COMPLETE'
            },
            zh: {
                '0': '初始',
                '100': '待分配',
                '200': '处理中',
                '300': '已完成'
            },
            de: {
                '0': 'INITIALISIEREN',
                '100': 'ZU VERTEILEN',
                '200': 'IN BEARBEITUNG',
                '300': 'FERTIGSTELLEN '
            },
            nl: {
                '0': 'Initialiseren',
                '100': 'In behandeling',
                '200': 'Verwerken',
                '300': 'Klaar'
            }
        },

        MEMBER_STATUS: {
            'All': 0,
            'NOT ACTIVATE': 1,
            'ACTIVATE': 2,
        },

        wheelDiameters: [
            { value: '6000', label: '6', max: '880', min: '400' },
            { value: '7000', label: '7', max: '880', min: '520' },
            { value: '8000', label: '8', max: '880', min: '520' },
            { value: 'A000', label: '10', max: '880', min: '520' },
            { value: 'C000', label: '12', max: '1300', min: '910' },
            { value: 'E000', label: '14', max: '1300', min: '910' },
            { value: '0001', label: '16', max: '1600', min: '1208' },
            { value: '1001', label: '17', max: '1600', min: '1208' },
            { value: '2001', label: '18', max: '1600', min: '1208' },
            { value: '4001', label: '20', max: '1880', min: '1290' },
            { value: '6001', label: '22', max: '1880', min: '1290' },
            { value: '7001', label: '23', max: '1880', min: '1290' },
            { value: '8001', label: '24', max: '2200', min: '1290' },
            { value: '9001', label: '25', max: '2200', min: '1880' },
            { value: 'A001', label: '26', max: '2510', min: '1880' },
            { value: 'B001', label: '27', max: '2510', min: '1880' },
            { value: 'C001', label: '28', max: '2510', min: '1880' },
            { value: 'D001', label: '29', max: '2510', min: '1880' },
            { value: '0002', label: '32', max: '2652', min: '2200' },
            { value: 'B501', label: '27.5', max: '2510', min: '1880' },
            { value: '0019', label: '400', max: '1600', min: '1208' },
            { value: '102C', label: '450', max: '1600', min: '1208' },
            { value: '8025', label: '600', max: '2200', min: '1600' },
            { value: 'A028', label: '650', max: '2200', min: '1600' },
            { value: 'C02B', label: '700', max: '2510', min: '1880' },
            { value: 'FFFF', label: 'No limit', max: '1', min: '0' }
        ],

        MATERIAL_STATUS: {
            en: {
                '-1': 'In plan',
                '0': 'In preparation',
                '1': 'In assembly',
                '2': 'In storage',
                '3': 'In test',
                '4': 'Abnormal assembly',
                '5': 'Abnormal test',
            },
            zh: {
                '-1': '计划中',
                '0': '备料中',
                '1': '装配中',
                '2': '已入库',
                '3': '测试中',
                '4': '装配异常',
                '5': '测试异常',
            }
        },

        TICKET_CATEGORY_TYPE_LIST: {
            1: 'Vehicle',
            2: 'Motor',
            3: 'Hmi',
            4: 'Sensor',
            5: 'Battery',
            6: 'Front Light',
            7: 'Rear Light',
            8: 'Other',
        },

        OBJECT_CATEGORY_ROOT_TYPE: {
            'VEHICLE': 1,
            'COMPONENT': 2,
        },

        TEST_ERROR_CODE: {
            1: {
                code: 1,
                error: 'Success',
                desc: 'Your enter test mode successfully',
                solution: [],
            },

            4: {
                code: 4,
                name: 'Error 04',
                remark: 'The second LED flickers 4 times in rapid succession',
                desc: 'Throttle signal level is greater than the start level',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change extension cable to check if the cable is available.',
                    'Change a new throttle to check if the throttle is available.',
                    'Change a new controller to check if the controller is available.'
                ],
                target: ['Throttle', 'Controller'],
            },

            5: {
                code: 5,
                name: 'Error 05',
                remark: 'The second LED flickers 5 times in rapid succession',
                desc: 'Throttle signal abnormity',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change extension cable to check if the cable is available.',
                    'Change a new throttle to check if the throttle is available.',
                    'Change a new controller to check if the controller is available.'
                ],
                target: ['Throttle', 'Controller'],
            },

            7: {
                code: 7,
                name: 'Error 07',
                remark: 'The second LED flickers 7 times in rapid succession',
                desc: 'The battery voltage is higher than the defined over-voltage value of controller',
                solution: [
                    'Change a new battery to check if the battery is available.',
                    'Change a new controller to check if the controller is available.'
                ],
                target: ['Battery', 'Controller'],
            },

            8: {
                code: 8,
                name: 'Error 08',
                remark: 'The second LED flickers 8 times in rapid succession',
                desc: 'Motor hall signal abnormity',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change the motor extension cable to check if the cable is available.',
                    'Change the display to check the display is available.',
                    'Change the motor to check the motor is available.',
                    'Change the controller to check the motor is available.',
                ],
                target: ['HMI', 'Controller', 'Motor'],
            },

            9: {
                code: 9,
                name: 'Error 09',
                remark: 'The second LED flickers 9 times in rapid succession',
                desc: 'Motor phase wire failure',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change the motor extension cable to check if the cable is available.',
                    'Change the display to check the display is available.',
                    'Change the motor to check the motor is available.',
                    'Change the controller to check the motor is available.'
                ],
                target: ['HMI', 'Controller', 'Motor'],
            },

            10: {
                code: 10,
                name: 'Error 10',
                remark: 'The first LED flickers 1 time',
                desc: 'Motor temperature reaches the protection value',
                solution: [
                    "If the Error shows when you are riding, the system need cool down. Don't worry.",
                    "If the Error shows when the bike doesn't work too much. Change the motor to check the motor is available.",
                    "Change the controller to check the controller is available."
                ],
                target: ['Controller', 'Motor'],
            },

            11: {
                code: 11,
                name: 'Error 11',
                remark: 'The first LED flickers 1 time and alternate to the second LED flickers 1 time',
                desc: 'Motor temperature sensor is abnormal',
                solution: [
                    "If the Error shows when you are riding, the system need cool down. Don't worry.",
                    "If the Error shows when the bike doesn't work too much. Change the controller to check the controller is available.",
                    'Change the motor to check the motor is available.'
                ],
                target: ['Controller', 'Motor'],
            },

            12: {
                code: 12,
                name: 'Error 12',
                remark: 'The first LED flickers 1 time and alternate to the second LED flickers 2 times in rapid succession',
                desc: 'Current sensor breakdown',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Update the software of the controller to check the latest software.',
                    'Change the motor to check the motor is available.',
                    'Change the controller to check the controller is available.'
                ],
                target: ['Controller'],
            },

            14: {
                code: 14,
                name: 'Error 14',
                remark: 'The first LED flickers 1 time and alternate to the second LED flickers 4 times in rapid succession',
                desc: 'Controller temperature reaches the protection value',
                solution: [
                    "If the Error shows when you are riding, the system need cool down. Don't worry.",
                    "If the Error shows when the bike doesn't work too much, update the software of the controller to check the latest software.",
                    'Change the controller to check the controller is available.'
                ],
                target: ['Controller', 'Motor'],
            },

            15: {
                code: 15,
                name: 'Error 15',
                remark: 'The first LED flickers 1 time and alternate to the second LED flickers 5 times in rapid succession',
                desc: 'Controller temperature sensor failure',
                solution: [
                    'Change the controller to check the controller is available.'
                ],
                target: ['Controller', 'Motor'],
            },

            21: {
                code: 21,
                name: 'Error 21',
                remark: 'The first LED flickers 2 times in rapid succession and alternate to the second LED flickers 1 time',
                desc: 'The RPM sensor within the motor or the external speed-detecting sensor is abnormal',
                solution: [
                    'Please check the RPM sensor connector connect properly.',
                    'Please check the magnet is locked on the proper place.',
                    'Update the software of the controller to check the latest software.',
                    'Change the RPM sensor to check the RPM sensor is available.',
                    'Change the display to check the display is available.',
                    'Change the controller to check the controller is available.'
                ],
                target: ['HMI', 'Controller', 'RPM sensor'],
            },

            25: {
                code: 25,
                name: 'Error 25',
                remark: 'The first LED flickers 2 times in rapid succession and alternate to the second LED flickers 5 times in rapid succession',
                desc: 'Torque sensor torque signal failure',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change the torque sensor to check the torque sensor is available.',
                    'Change the display to check the display is available.',
                    'Update the software of the controller to check the latest software.',
                    'Change the controller to check the controller is available.'
                ],
                target: ['HMI', 'Controller', 'Torque sensor'],
            },

            26: {
                code: 26,
                name: 'Error 26',
                remark: 'The first LED flickers 2 times in rapid succession and alternate to the second LED flickers 6 times in rapid succession',
                desc: 'Torque sensor speed signal failure',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change the torque sensor to check the torque sensor is available.',
                    'Change the display to check the display is available.',
                    'Update the software of the controller to check the latest software.',
                    'Change the controller to check the controller is available.'
                ],
                target: ['HMI', 'Torque sensor'],
            },

            27: {
                code: 27,
                name: 'Error 27',
                remark: 'The first LED flickers 2 times in rapid succession and alternate to the second LED flickers 7 times in rapid succession',
                desc: 'Controller detected overcurrent',
                solution: [
                    'Using the BESST tool update the controller. If the problem still occurs, please change the motor.'
                ],
                target: ['Controller', 'Motor'],
            },

            30: {
                code: 30,
                name: 'Error 30',
                remark: 'The first LED flickers 3 times in rapid succession',
                desc: 'The display fails to receive data from the controller',
                solution: [
                    'Please check the cable connector, make sure that they connect properly.',
                    'Change the display to check the display is available.',
                    'Update the software of the controller to check the latest software.',
                    'Change the controller to check the controller is available.',
                ],
                target: ['HMI', 'Controller'],
            },

            33: {
                code: 33,
                name: 'Error 33',
                remark: 'The first LED flickers 3 times in rapid succession and alternate to the second LED flickers 3 times in rapid succession',
                desc: 'Brake signal has an error(If brake sensors are fitted)',
                solution: [
                    'Please check all connectors are correctly connected on the brakes.',
                    'Change the brakes to see if it resolves the problem',
                    'Please change the motor',
                ],
                target: ['Brake', 'Motor'],
            },

            35: {
                code: 35,
                name: 'Error 35',
                remark: 'The first LED flickers 3 times in rapid succession and alternate to the second LED flickers 5 times in rapid succession',
                desc: 'Detection circuit for 15V has an error',
                solution: [
                    'Using the BESST tool update the controller to see if this resolves the problem. If not, please change the motor.'
                ],
                target: ['Controller', 'Motor'],
            },

            36: {
                code: 36,
                name: 'Error 36',
                remark: 'The first LED flickers 3 times in rapid succession and alternate to the second LED flickers 6 times in rapid succession',
                desc: 'Detection circuit on the keypad has an error',
                solution: [
                    'Using the BESST tool update the controller to see if this resolves the problem. If not, please change the motor.'
                ],
                target: ['Controller', 'Motor'],
            },

            37: {
                code: 37,
                name: 'Error 37',
                remark: 'The first LED flickers 3 times in rapid succession and alternate to the second LED flickers 7 times in rapid succession',
                desc: 'WDT circuit is faulty',
                solution: [
                    'Using the BESST tool update the controller to see if this resolves the problem. If not, please change the motor.'
                ],
                target: ['Controller', 'Motor'],
            },

            41: {
                code: 41,
                name: 'Error 41',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 1 times in rapid succession',
                desc: 'Total voltage from the battery is too high',
                solution: [
                    'Please change the battery.'
                ],
                target: ['Battery'],
            },

            42: {
                code: 42,
                name: 'Error 42',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 2 times in rapid succession',
                desc: 'Total voltage from the battery is too low',
                solution: [
                    'Please Charge the battery. If the problem still occurs, please change the battery.'
                ],
                target: ['Battery'],
            },

            43: {
                code: 43,
                name: 'Error 43',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 3 times in rapid succession',
                desc: 'Total power from the battery cells is too high.',
                solution: [
                    'Please change the battery.'
                ],
                target: ['Battery'],
            },

            45: {
                code: 45,
                name: 'Error 45',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 5 times in rapid succession',
                desc: 'Temperature from the battery is too high',
                solution: [
                    'Please let the bike to cool down.If the problem still occurs, please change the battery.'
                ],
                target: ['Battery'],
            },

            46: {
                code: 46,
                name: 'Error 46',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 6 times in rapid succession',
                desc: 'Temperature from the battery is too low',
                solution: [
                    'Please bring the battery to room temperature.If the problem still occurs, please change the battery.'
                ],
                target: ['Battery'],
            },

            47: {
                code: 47,
                name: 'Error 47',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 7 times in rapid succession',
                desc: 'SOC of the battery is too high.',
                solution: [
                    'Please change the battery.'
                ],
                target: ['Battery'],
            },

            48: {
                code: 48,
                name: 'Error 48',
                remark: 'The first LED flickers 4 times in rapid succession and alternate to the second LED flickers 8 times in rapid succession',
                desc: 'SOC of the battery is too low.',
                solution: [
                    'Please change the battery.'
                ],
                target: ['Battery'],
            },

            71: {
                code: 71,
                name: 'Error 71',
                remark: 'The first LED flickers 7 times in rapid succession and alternate to the second LED flickers 1 times in rapid succession',
                desc: 'Electronic lock is jammed',
                solution: [
                    'Using the BESST tool update the Display',
                    'Change the display.',
                    'please change the electronic lock'
                ],
                target: ['Display', 'Electronic lock'],
            },

            81: {
                code: 81,
                name: 'Error 81',
                remark: 'The first LED flickers 8 times in rapid succession and alternate to the second LED flickers 1 times in rapid succession',
                desc: 'Bluetooth module has an error',
                solution: [
                    'Using the BESST tool update the display to see if this resolves the problem. If not, please change the display.'
                ],
                target: ['Display'],
            },
        },

        //video 的category type
        VIDEO_CATEGORY_TYPE: {
            1: 'System Error - LCD',
            2: 'System Error - LED',
            3: 'HMI',
            4: 'Motor',
            5: 'Controller',
            6: 'Sensor',
            7: 'Battery',
            8: 'Charger'
        },

        AUTH_LIST: {
            productMenu: "product.menu",
            productPart: "product.part",
            productTraining: "product.training",
            productEditCategory: "product.edit_category",
            productProductAdd: "product.product.add",
            productProductEdit: "product.product.edit",
            productProductDelete: "product.product.delete",
            productProductBomView: "product.product.bom.view",
            productProductBomEdit: "product.product.bom.edit",
            productProductBomDelete: "product.product.bom.delete",
            productProductTrainingEdit: "product.product.training.edit",
            productProductTrainingDelete: "product.product.training.delete",
            productSecondaryMotor: "product_secondary_motor",
            productSecondaryHmi: "product_secondary_hmi",
            productSecondaryBattery: "product_secondary_battery",
            productSecondarySensor: "product_secondary_sensor",
            productSecondaryController: "product_secondary_controller",
            productSecondaryConnector: "product_secondary_connector",
            productSecondaryCharger: "product_secondary_charger",
            productSecondaryTool: "product_secondary_tool",
            productionMenu: "production.menu",
            productionAdd: "production.add",
            productionEdit: "production.edit",
            productionDelete: "production.delete",
            productionProduce: "production.produce",
            productionReport: "production.report",
            orderMenu: "order.menu",
            orderAdd: "order.add",
            orderEdit: "order.edit",
            orderDelete: "order.delete",
            orderReplace: "order.replace",
            itemMenu: "item.menu",
            itemAdd: "item.add",
            itemEdit: "item.edit",
            itemDelete: "item.delete",
            itemCopy: "item.copy",
            itemBomEdit: "item.bom.edit",
            itemBomDelete: "item.bom.delete",
            accountMenu: "account.menu",
            accountDelete: "account.delete",
            ticketMenu: "ticket.menu",
            ticketDelete: "ticket.delete",
            ticketSC: "ticket.service_center",
            ticketSCAdd: "ticket.service_center.add",
            ticketSCEdit: "ticket.service_center.edit",
            ticketComplaint: "ticket.complaint",
            ticketTask: "ticket.task",
            brandMenu: "brand.menu",
            brandAdd: "brand.add",
            brandEdit: "brand.edit",
            brandDelete: "brand.delete",
            objectMenu: "object.menu",
            objectDelete: "object.delete",
            objectReplace: "object.replace",
            toolMenu: "tool.menu",
            diagnoseMenu: "diagnose.menu",
            errorShootingMenu: "error_shooting.menu",
            dealerMenu: "dealer.menu",
            dealerDelete: "dealer.delete",
            dealerInvitation: "dealer.invitation",
            oemMenu: "oem.menu",
            oemDelete: "oem.delete",
            oemInvitation: "oem.invitation",
            fileFeedback: "file.error",
            fileFeedbackDelete: "file.error.delete",
            fileFeedbackDetail: "file.error.detail",
            fileFeedbackAdd: "file.error.add",
            fileFeedbackEdit: "file.error.edit",
            material: "auth_material",
        },

        OptionList: [{
                type: 'bike',
                saveData: 'bikeList',
                model: '车辆',
                data: [{
                        name: 'bi01',
                        label: '无助力功能'
                    },
                    {
                        name: 'bi02',
                        label: '不能停止电机运转'
                    },
                    {
                        name: 'bi03',
                        label: '无法开机'
                    },
                    {
                        name: 'bi04',
                        label: '转把可电机运行，助力功能不正常'
                    },
                    {
                        name: 'bi05',
                        label: '助力不连续'
                    },
                    {
                        name: 'bi06',
                        label: '助力功能减弱'
                    },
                    {
                        name: 'bi07',
                        label: '电池和仪表电量显示不匹配'
                    },
                    {
                        name: 'bi08',
                        label: '整车共振异响或抖动'
                    },
                    {
                        name: 'bi09',
                        label: '前灯不亮，不能正常工作'
                    },
                    {
                        name: 'bi10',
                        label: '后灯不亮，不能正常工作'
                    },
                    {
                        name: 'bi11',
                        label: '骑行里程缩短'
                    },
                    {
                        name: 'bi12',
                        label: '无速度显示'
                    }
                ]
            },
            {
                type: 'motor',
                model: '电机',
                saveData: 'motorList',
                data: [{
                        name: "m01",
                        label: '运行时噪声大，不连续'
                    },
                    {
                        name: "m02",
                        label: '无电状态，传动噪声大'
                    },
                    {
                        name: "m03",
                        label: '接头和线损坏'
                    },
                    {
                        name: "m04",
                        label: '主轴松动'
                    },
                    {
                        name: "m05",
                        label: '外壳损坏'
                    },
                    {
                        name: "m06",
                        label: '运行颤抖'
                    },
                    {
                        name: "m07",
                        label: '电机壳体温度太高'
                    },
                    {
                        name: "m08",
                        label: '电机进水'
                    },
                    {
                        name: "m09",
                        label: '电机烧毁'
                    },
                    {
                        name: "m10",
                        label: '固定螺栓孔损坏'
                    }
                ]
            },
            {
                type: 'hmi',
                model: '仪表',
                saveData: 'hmiList',
                data: [{
                        name: "h01",
                        label: '外壳不良'
                    },
                    {
                        name: "h02",
                        label: '按键不工作'
                    },
                    {
                        name: "h03",
                        label: '屏幕雾气'
                    },
                    {
                        name: "h04",
                        label: '接头和线损坏'
                    },
                    {
                        name: "h05",
                        label: '仪表支架损坏'
                    },
                    {
                        name: "h06",
                        label: '显示异常'
                    },
                    {
                        name: "h07",
                        label: '显示屏损坏'
                    },
                    {
                        name: "h08",
                        label: '蓝牙不能联机'
                    },
                    {
                        name: "h09",
                        label: '震动功能不工作'
                    },
                ]
            },
            {
                type: 'controller',
                model: '控制器',
                saveData: 'controllerList',
                data: [{
                        name: "c01",
                        label: '控制器烧毁'
                    },
                    {
                        name: "c02",
                        label: '固定部分松脱'
                    },
                    {
                        name: "c03",
                        label: '控制器进水'
                    },
                    {
                        name: "c04",
                        label: '接头和线损坏'
                    }
                ]
            },
            {
                type: 'battery',
                model: '电池',
                saveData: 'batteryList',
                data: [{
                        name: "b01",
                        label: '外壳不良'
                    },
                    {
                        name: "b02",
                        label: '滑板损坏'
                    },
                    {
                        name: "b03",
                        label: '放电口损坏'
                    },
                    {
                        name: "b04",
                        label: '连接线损坏'
                    },
                    {
                        name: "b05",
                        label: '不能充电'
                    },
                    {
                        name: "b06",
                        label: '不能放电'
                    },
                    {
                        name: "b07",
                        label: '充电口橡胶盖帽损坏'
                    },
                    {
                        name: "b08",
                        label: '电池电量显示和按键损坏'
                    },
                    {
                        name: "b09",
                        label: '钥匙片损坏'
                    },
                    {
                        name: "b10",
                        label: '钥匙片丢失'
                    },
                    {
                        name: "b11",
                        label: '电池拆卸困难'
                    }
                ]
            },
            {
                type: 'sensor',
                model: '传感器',
                saveData: 'sensorList',
                data: [{
                        name: "s01",
                        label: '轴向松动'
                    },
                    {
                        name: "s02",
                        label: '锁紧螺母松脱'
                    },
                    {
                        name: "s03",
                        label: '传感器锈蚀'
                    },
                    {
                        name: "s04",
                        label: '接头和线损坏'
                    },
                    {
                        name: "s05",
                        label: '传感器轴承损坏'
                    },
                    {
                        name: "s06",
                        label: '左护腕损坏'
                    },
                    {
                        name: "s07",
                        label: '右护腕损坏'
                    },
                ]
            }
        ],

        // {
        //     type: 'charger',
        //     model: '充电器',
        //     saveData: 'chargerList',
        //     data: [
        //         {
        //             name: "ch01",
        //             label: '无'
        //         }
        //     ]
        // },
        // {
        //     type: 'other',
        //     model: '其他',
        //     saveData: 'otherList',
        //     data: [
        //         {
        //             name: "o01",
        //             label: '无'
        //         }
        //     ]
        // }


        OBJECT_RECORD_TYPE: {
            EDIT: 1,
            UPDATE: 2,
        },
    };
}

export default new Const();



//////////////////
// WEBPACK FOOTER
// ./core/js/const.js
// module id = 89
// module chunks = 73