#ifndef enumTabH
#define enumTabH

enum eTabType { TabNormal, TabStaff, TabMember, TabTableSeat, TabHandheldCashAccount, TabRoom,
                TabInvoice, TabWeb, TabParkedSale, TabHeld, TabNone, TabCashAccount,TabDelayedPayment, TabClipp };
typedef int TMMTabType;

enum eClippPaymentType
{
    ClippProcessingFees,
    ClippTip,
    ClippPayment
};

typedef int TMMClippPaymentType;
#endif