//---------------------------------------------------------------------------

#ifndef ReceiptUtilityH
#define ReceiptUtilityH
#include "PrintingSections.h"
#include "ReqPrintJob.h"
#include "PrintFormat.h"
//---------------------------------------------------------------------------


class TReceiptUtility
{
	private:
    public:
        TReceiptUtility();
        ~TReceiptUtility();
        static void PrintTaxInvoice(TReqPrintJob *PrintJob, TPrintFormat *pPrinter);
        static void CustomizeReceiptNo(TReqPrintJob *PrintJob,TPrintFormat *pPrinter);
        static void ShowRefundReference(TReqPrintJob *PrintJob,TPrintFormat *pPrinter);
        static void CustomizeReceiptNoLabel(TReqPrintJob *PrintJob,TPrintFormat *pPrinter);
        static void PrintReceiptHeaderSecond(TReqPrintJob *PrintJob,TPrintFormat *pPrinter);
        static bool PrintReceiptFooterSecond(TReqPrintJob *PrintJob,TPrintFormat *pPrinter);
        static void ModifyInvoiceNumber(TReqPrintJob *PrintJob,TPrintFormat *pPrinter,int size);
        static UnicodeString LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen);
};

#endif
