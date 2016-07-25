//---------------------------------------------------------------------------

#ifndef DealUtilitiesH
#define DealUtilitiesH
//---------------------------------------------------------------------------

#include "IStoredDeal.h"
#include "IStoredDealLevel.h"

//---------------------------------------------------------------------------

class TDealUtilities
{
    public:
		static bool CompareDealsByPriority(
                                    IDeal* lhs,
                                    IDeal* rhs);

        //static bool compareDealLevels(

};

#endif
