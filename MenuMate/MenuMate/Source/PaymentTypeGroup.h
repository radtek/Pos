//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupH
#define PaymentTypeGroupH

#include <Classes.hpp>

#define CASH_PAYMENT_GROUP "Cash Sales"
#define GIFT_PAYMENT_GROUP "Gift Card"
#define DEBIT_PAYMENT_GROUP "Debit Card"
#define OTHER_CARD_PAYMENT_GROUP "Other Card"
#define MASTER_CARD_PAYMENT_GROUP "Master Card"
#define VISA_PAYMENT_GROUP "VISA"
#define AMEX_PAYMENT_GROUP "AmEx"
#define DINERS_PAYMENT_GROUP "Diners"
#define JCB_PAYMENT_GROUP "JCB"
#define CARD_PAYMENT_GROUP "Card Sales"
#define CHECK_PAYMENT_GROUP "Check Sales"
#define EPAY_PAYMENT_GROUP "EPay Sales"
#define OTHER_PAYMENT_GROUP "Other Sales"

//---------------------------------------------------------------------------
class TPaymentTypeGroup
{
    public:
        TPaymentTypeGroup(int dbkey);
        TPaymentTypeGroup(int dbKey, UnicodeString name, bool isDefault);
        int DBKey;
        UnicodeString Name;
        bool IsDefault;
};
#endif
