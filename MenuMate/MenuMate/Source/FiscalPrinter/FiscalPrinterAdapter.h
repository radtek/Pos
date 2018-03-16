//---------------------------------------------------------------------------

#ifndef FiscalPrinterAdapterH
#define FiscalPrinterAdapterH
#include <System.hpp>
#include<list>
#include <memory>
#include "PaymentTransaction.h"
#include "FiscalPrinting.h"
//---------------------------------------------------------------------------
class TFiscalPrinterAdapter
{
private:
    TFiscalBillDetails billDetails;
    void PrepareItemInfo(TPaymentTransaction paymentTransaction);
    void PrepartePaymnetInfo(TPaymentTransaction paymentTransaction);
    void PrepareDiscountDetails(std::vector<TFiscalDiscountDetails> &discountList,TItemMinorComplete *order);
    TFiscalPrinterResponse PrintFiscalReceipt(TFiscalBillDetails receiptData);
public:
       TFiscalPrinterAdapter();
       void ConvertInToFiscalData(TPaymentTransaction paymentTransaction);
       TFiscalPrinterResponse FiscalZReportSettlement();
};
#endif
