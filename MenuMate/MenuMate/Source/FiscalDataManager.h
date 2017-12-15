//---------------------------------------------------------------------------

#ifndef FiscalDataManagerH
#define FiscalDataManagerH
#include "FiscalDataUtility.h"
//---------------------------------------------------------------------------
class TFiscalDataManager
{
    public:
        TFiscalDataManager();
        ~TFiscalDataManager();
        UnicodeString GetDataFromTransaction(TPaymentTransaction PaymentTransaction);
        bool PostFiscalData(AnsiString data);
        bool CheckFiscalDetails();
    private:
};
#endif
