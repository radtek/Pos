//---------------------------------------------------------------------------

#ifndef PaySubsUtilityH
#define PaySubsUtilityH
#include "GlobalSettings.h"
#include "DevicerealControl.h"
#include "ManagerVariable.h"
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
