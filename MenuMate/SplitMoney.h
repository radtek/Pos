//---------------------------------------------------------------------------

#ifndef SplitMoneyH
#define SplitMoneyH
//---------------------------------------------------------------------------
#include <System.hpp>

class TSplitMoney
{
public:
   Currency TotalCost;
   Currency CreditAvailable;
   Currency TotalAdjustment;
   Currency ShareOfCredit;
   Currency LeftOwing;
   Currency RemianingCredit;
   Currency PaymentAmount;
   Currency GSTContent;
   int NumberOfPayments;
   int CreditDivisionsLeft;
   int DivisionsLeft;
   int TotalDivisions;
   void Clear();
};

#endif
