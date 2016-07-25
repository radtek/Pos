//---------------------------------------------------------------------------

#ifndef RewardsTransactionH
#define RewardsTransactionH
//---------------------------------------------------------------------------
#include <System.hpp>

class TRewardsTransaction
{
   public:
      TRewardsTransaction() : CardNumber(""),EFTPOSRef(""),Amount(0){};
      UnicodeString CardNumber;
      UnicodeString EFTPOSRef;
      Currency Amount;
};
#endif
