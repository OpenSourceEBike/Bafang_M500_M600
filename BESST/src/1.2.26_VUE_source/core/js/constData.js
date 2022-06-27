function ConstData() {
    return {
        TRADE_TERMS: [
            {
                label: "Free Carrier",
                id: 0
            },
            {
                label: "Free Alongside",
                id: 1
            },
            {
                label: "Free on Board",
                id: 2
            }
        ],
        PAYMENT_TYPES: [
            {
                label: "Transfer_chequee",
                id: 0
            },
            {
                label: "Cash_cheque",
                id: 1
            },
            {
                label: "cash",
                id: 2
            },
            {
                label: "Bank_draft",
                id: 3
            },
            {
                label: "Acceptance_bill",
                id: 4
            },
            {
                label: "Online_Banking_Service",
                id: 5
            }
        ],
        PAYMENT_TERMS: [
            {
                label: "30% down payment/70% before delivery",
                id: 0
            },
            {
                label: "50% down payment/50% before delivery",
                id: 1
            },
            {
                label: "50% down payment/balance on delivery",
                id: 2
            },
            {
                label: "10% down payment/90% net 30 days",
                id: 3
            },
            {
                label: "Pay first and then pay the last payment",
                id: 4
            },
            {
                label: "100% PAID WITHIN 30 DAYS AFTER BLDATE",
                id: 5
            }
        ],
        CURRENCIES: [
            {
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
        PAYMENT_METHOD: [
            {
                id: 0,
                label: "Transfer check"
            },
            {
                id: 1,
                label: "Cash check"
            },
            {
                id: 2,
                label: "Cash"
            },
            {
                id: 3,
                label: "Bank draft"
            },
            {
                id: 4,
                label: "Acceptance draft"
            },
            {
                id: 5,
                label: "Online banking"
            },
            {
                id: 6,
                label: "Bank transfer"
            }
        ],
        SHIPMENT_TERMS: [
            {
                label: "By Train",
                id: 1
            },
            {
                label: "By Ship",
                id: 2
            },
            {
                label: "By Aircraf",
                id: 3
            }
        ]
    }
}

export default new ConstData();


//////////////////
// WEBPACK FOOTER
// ./core/js/constData.js
// module id = 718
// module chunks = 73