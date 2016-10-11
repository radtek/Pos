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
        static AnsiString ModifyInvoiceNumber(AnsiString inInvoiceNumber,int size);
        static UnicodeString LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen);
        static bool IsCancelTransaction(TPaymentTransaction PaymentTransaction);
};

#endif
