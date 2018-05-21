using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartConnectIntegration.Enums
{
    public enum TransactionType
    {
        Logon  = 1,
        SettlementEnquiry,
        SettlementCutOver,
        CardPurchase,
        CardPurchasePlusCash,
        CardCashAdvance,
        CardRefund,
        CardAuthorise,
        CardFinalise,
        TransResult,
        ReprintReceipt,
        TerminalStatus,
        TerminalReadCard
    }
}
