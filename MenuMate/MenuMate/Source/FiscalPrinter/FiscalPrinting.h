//---------------------------------------------------------------------------

#ifndef FiscalPrintingH
#define FiscalPrintingH
#include "FiscalPrinterWSDL.h"
#include "FiscalDataObjects.h"
//---------------------------------------------------------------------------
class TFiscalPrinting
{
private:
       _di_IWCFServiceFiscalPrinter fiscalClient; // WCF Client
       void initFIClient();
       FiscalService* GetItemList(std::vector<TFiscalItemDetails>::iterator it);
       FiscalPayment* GetPayment(std::vector<TFiscalPaymentDetails>::iterator it);
       DiscountDetails* GetDiscount(std::vector<TFiscalDiscountDetails>::iterator it);
public:
       TFiscalPrinting();
       TFiscalPrinterResponse PrintFiscalReceipt(TFiscalBillDetails receiptData);
};
#endif
