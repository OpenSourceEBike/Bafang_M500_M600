function StatusData() {
    return {
        // 货币
        CURRENCIES: [{
            currency: 1,
            label: "€"
        },
        {
            currency: 2,
            label: "$"
        },
        {
            currency: 3,
            label: "DM."
        },
        {
            currency: 4,
            label: "￥"
        },
        {
            currency: 5,
            label: "￡"
        }
        ],
        // 用途
        PURPOSE: [{
            id: 0,
            label: "Repair",
        }, {
            id: 1,
            label: "Send",
        }],
        // 出库 类型（付费 免费）
        STOCKS_PAY_TYPE: [{
            id: 0,
            label: "Free",
            value: 0
        },
        {
            id: 1,
            label: "Pay",
            value: 1
        }
        ],
        TRANSPORT: ['B/L', 'tracking', 'ticket', 'HAWB'],
        // 贸易术语
        TRADE_TERM: [{
            label: 'DDP',
            value: 'DDP'
        }, {
            label: 'DDU',
            value: 'DDU'
        }, {
            label: 'DAP',
            value: 'DAP'
        }, {
            label: 'CIF',
            value: 'CIF'
        }, {
            label: 'CFR',
            value: 'CFR'
        }, {
            label: 'FOB',
            value: 'FOB'
        }, {
            label: 'Ex-Work',
            value: 'ExWork'
        }],
        // 运输方式
        TRANSPORT_METHOD: ['Sea', 'Air', 'Express', 'Land'],
        // 来源
        SOURCE_FROM: [{
            id: 0,
            label: "Material",
        }, {
            id: 1,
            label: "refurbished",
        }],
        // 包裹处理状态
        REPAIR_PACK_PROCESS: [{
            label: "Pending",
            id: 0
        },
        {
            label: "Repairing",
            id: 1
        },
        {
            label: "Replace",
            id: 2
        },
        {
            label: "Scrapped",
            id: 3
        }
        ],
        // 紧急等级
        LEVEL: [{
            label: "Normal",
            id: 0
        },
        {
            label: "Important",
            id: 1
        },
        {
            label: "Urgent",
            id: 2
        }
        ],
        // 维修后处理方式
        REPAIR_AFTER: ["Send To Customer", "Send To Netherlands", "Others"],
        // 状态
        REVIEW_STATUS: [{
            label: "Pending",
            id: 0
        },
        {
            label: "Reviewed",
            id: 1
        },
        ],
        PACK_REVIEW_STATUS: [{
            label: "Close",
            id: 0
        },
        {
            label: "Opend",
            id: 1
        },
        ],
        SORT: ['零件商', '品牌商', '经销商', '组装厂', '终端'],
        // 采购类型
        PURCHASE_TYPE: ['complaint', 'stock']
    }
}

export default new StatusData();



//////////////////
// WEBPACK FOOTER
// ./core/js/statusData.js
// module id = 719
// module chunks = 73