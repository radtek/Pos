//---------------------------------------------------------------------------


#pragma hdrstop

#include "DealUtilities.h"

//---------------------------------------------------------------------------
bool TDealUtilities::CompareDealsByPriority( IDeal* lhs, IDeal* rhs)
{
    return lhs->GetPriority() < rhs->GetPriority();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
