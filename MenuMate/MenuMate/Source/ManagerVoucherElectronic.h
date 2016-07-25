//---------------------------------------------------------------------------

#ifndef ManagerVoucherElectronicH
#define ManagerVoucherElectronicH
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "Money.h"
#include "Payment.h"

class TManagerVoucherElectronic
{
   public :
 	AnsiString URL;

   TManagerVoucherElectronic();
   bool ProcessVoucher(TPaymentTransaction &PaymentTransaction,TPayment *Payment);

};
#endif

