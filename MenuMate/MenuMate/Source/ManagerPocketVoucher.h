//---------------------------------------------------------------------------
#ifndef ManagerPocketVoucherH
#define ManagerPocketVoucherH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include "PocketVoucherTransport.h"
#include "Money.h"
#include "Payment.h"

class TManagerPocketVoucher
{
	std::auto_ptr<TXML_Transport_Http>  XML_HTTP;
   public :
 	AnsiString URL;

   TManagerPocketVoucher();
	void Initialise(Database::TDBTransaction &DBTransaction);
   void ProcessVoucher(TPaymentTransaction &PaymentTransaction,TPayment *Payment);

};
#endif
