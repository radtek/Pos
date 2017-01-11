//---------------------------------------------------------------------------

#ifndef PaySubsUtilityH
#define PaySubsUtilityH
#include "GlobalSettings.h"
#include "PaymentTransaction.h"
//---------------------------------------------------------------------------
class TPaySubsUtility
{
    private:


	public:
       static bool IsLocalLoyalty();
       static bool IsPaySubsEligible(TPaymentTransaction &PaymentTransaction,double &amount);
};
#endif
