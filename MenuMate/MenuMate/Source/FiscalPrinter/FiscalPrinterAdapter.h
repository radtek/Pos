//---------------------------------------------------------------------------

#ifndef FiscalPrinterAdapterH
#define FiscalPrinterAdapterH
#include <System.hpp>
#include<list>
#include <memory>
#include "PaymentTransaction.h"
#include "FiscalDataObjects.h"
//---------------------------------------------------------------------------
class TFiscalPrinterAdapter
{
private:
    TFiscalBillDetails billDetails;
    void PrepareItemInfo(TPaymentTransaction paymentTransaction);
    void PrepartePaymnetInfo(TPaymentTransaction paymentTransaction);
    void PrepareDiscountDetails(std::vector<TFiscalDiscountDetails> &discountList,TItemMinorComplete *order);
    UnicodeString PrintFiscalReceipt(TFiscalBillDetails receiptData);
public:
       TFiscalPrinterAdapter();
       UnicodeString ConvertInToFiscalData(TPaymentTransaction paymentTransaction);
       UnicodeString FiscalZReportSettlement();
       UnicodeString GetFiscalPrinterStatus();
       UnicodeString OpenCashDrawerForFiscalPrinter();
};
#endif
