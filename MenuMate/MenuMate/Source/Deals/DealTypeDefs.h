//---------------------------------------------------------------------------

#ifndef DealTypeDefsH
#define DealTypeDefsH

#include <set>
#include <vector>
//---------------------------------------------------------------------------

class IDealLevel;
class IStoredDealLevel;
class IDeal;
class IStoredDeal;

struct DealLevelComparator;

typedef std::set<IDealLevel*, 		DealLevelComparator> DealLevelSet;
typedef std::set<IStoredDealLevel*, DealLevelComparator> StoredDealLevelSet;

typedef std::vector<IDeal*> 	  DealVector;
typedef std::vector<IStoredDeal*> StoredDealVector;

#endif
