//---------------------------------------------------------------------------


#pragma hdrstop

#include "SplitMoney.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TSplitMoney::Clear()
{
   TotalCost = 0;
   CreditAvailable = 0;
   ShareOfCredit = 0;
   LeftOwing = 0;
   RemianingCredit = 0;
   PaymentAmount = 0;
   NumberOfPayments = 0;
   CreditDivisionsLeft = 0;
   DivisionsLeft = 0;
   TotalAdjustment = 0;
   GSTContent = 0;
   TotalDivisions = 0;
}
