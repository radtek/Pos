//---------------------------------------------------------------------------

#ifndef MallExportInterfaceH
#define MallExportInterfaceH
#include "PaymentTransaction.h"
//---------------------------------------------------------------------------
class TMallExportInterface
{
    public:
    virtual bool PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey) = 0;
    virtual bool Export() = 0;
    virtual void RegenerateMallReport(TDateTime sDate, TDateTime eDate) = 0;
};
#endif
