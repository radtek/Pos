//---------------------------------------------------------------------------

#ifndef TransactionHelperH
#define TransactionHelperH
#include "PaymentTransaction.h"
#include "Printing.h"
class TTransactionHelper
{
    public:
        static bool CheckRoomPaytypeWhenFiscalSettingEnable(TPaymentTransaction PaymentTransaction);
        static bool IsAnyDiscountApplied(TPaymentTransaction &paymentTransaction);
        static bool IsNormalPrintRequiredWithFiscal(TReceipt *receipt, TPaymentTransaction paymentTransaction);
        static bool IsPaymentDoneWithParamPaymentType(TPaymentTransaction &paymentTransaction, ePaymentAttribute attributeIndex);
        static void PrintFiscalReceipt(TPaymentTransaction &paymentTransactionNew);
        static bool IsMewsConfigured();
        static bool IsOracleConfigured();
        static bool IsSiHotConfigured();
    private:
        static bool IsPaymentDoneForFiscal(TPaymentTransaction paymentTransaction);

};
//---------------------------------------------------------------------------
#endif
