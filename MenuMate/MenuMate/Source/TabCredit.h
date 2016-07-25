//---------------------------------------------------------------------------

#ifndef TabCreditH
#define TabCreditH
//---------------------------------------------------------------------------
#include <System.hpp>

class TTabCredit
{
	public:
	TTabCredit() : CurrentCredit(0) , CreditRedeemed(0),PaymentGroupNumber(0) {}
	Currency CurrentCredit;
	Currency CreditRedeemed;
   int PaymentGroupNumber;
};
#endif
