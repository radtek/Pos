//---------------------------------------------------------------------------

#ifndef FiscalPrintingH
#define FiscalPrintingH
#include "FiscalDataObjects.h"

//---------------------------------------------------------------------------
class TFiscalPrinting
{
private:
       void initFIClient();
//       TFiscalService* GetItemList(std::vector<TFiscalItemDetails>::iterator it);
//       FiscalPayment* GetPayment(std::vector<TFiscalPaymentDetails>::iterator it);
//       DiscountDetails* GetDiscount(std::vector<TFiscalDiscountDetails>::iterator it);
         void makeLogFile(AnsiString str1);
public:
       TFiscalPrinting();
       TFiscalPrinterResponse PrintFiscalReceipt(TFiscalBillDetails receiptData);
//       TFiscalPrinterResponse PrintZReport();
};
#endif
