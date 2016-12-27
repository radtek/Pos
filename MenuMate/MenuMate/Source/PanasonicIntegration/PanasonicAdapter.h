//---------------------------------------------------------------------------

#ifndef PanasonicAdapterH
#define PanasonicAdapterH
//---------------------------------------------------------------------------
#endif

#include "PaymentTransaction.h"

class TPanasonicAdapter
{
private:
    long GetSiteId(TPaymentTransaction &paymentTransaction);
    void ConverTransactionInfoToPanasonicTransactionInfo(TPaymentTransaction &PaymentTransaction, long arcBillKey, UnicodeString receipt);
    void ConvertTransactionInfoToPanasonicItemList(TPaymentTransaction &PaymentTransaction, LongInt arcBill_Key);
    void ConvertTransactionInfoToPanasonicProduct(TPaymentTransaction &PaymentTransaction);
    void ConverTransactionInfoToTransactionDBServerInfo(TPaymentTransaction &PaymentTransaction);
    UnicodeString GetPOSVersionInfo(TPaymentTransaction &paymentTransaction);
public:
    void ConvertTransactionInfoToPanasonicInfo(TPaymentTransaction &PaymentTransaction, long ArcBillKey, UnicodeString receipt);
};

