//---------------------------------------------------------------------------

#ifndef ManagerGeneralLedgerH
#define ManagerGeneralLedgerH
//---------------------------------------------------------------------------
#include "PaymentTransaction.h"
#include "MembershipGeneralLedgerTCP.h"
#include "rounding.h"

typedef std::map<int,Currency> TCategoryCurrency;

class TManagerGeneralLedger
{
	   std::auto_ptr<TMembershipGeneralLedgerTCP> MembershipGeneralLedgerTCP;
	   int GetRefNumber(Database::TDBTransaction &DBTransaction);
	   int UN_CODED;
	   int DefaultDiscountCode;
	   int PointsCode;
           int CompPointsCode;
           int DiningPointsCode;
	   int RoundingCode;
 public :
	   TManagerGeneralLedger();
	   int LastTransactionRef;
	   bool ProcessTransaction(TPaymentTransaction &PaymentTransaction);
       bool EnabledFor(TPaymentTransaction &PaymentTransaction);
       bool Enabled();
       void Initialise(Database::TDBTransaction &DBTransaction);
};
#endif
