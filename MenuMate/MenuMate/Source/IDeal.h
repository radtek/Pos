//---------------------------------------------------------------------------

#ifndef IDealH
#define IDealH

#include <set>
#include <System.hpp>
#include "IDealLevel.h";
#include "DealTypeDefs.h"

//---------------------------------------------------------------------------
class IDeal
{
	public:
		virtual ~IDeal() {}

		virtual UnicodeString GetName() = 0;
		virtual int GetPriority() = 0;
		virtual int GetCategoryKey() = 0; //Link to breakdown catgory for this deal. Not sure if this is the best varible to use so subject to change.
		//virtual IDealLevel GetDealLevel(int inDealLevelIndex) = 0;
		//virtual DealLevelSet GetDealLevels() = 0;

		virtual void SetName(UnicodeString inName) = 0;
		virtual void SetPriority(int inPriority) = 0;
		virtual void SetCategoryKey(int inCatKey) = 0;
		//virtual void SetDealLevels(std::set<IDealLevel*, DealLevelComparator> inDealLevels) = 0;
		//virtual void AddDealLevel(IDealLevel inDealLevel) = 0;
		//virtual void RemoveDealLevel(IDealLevel inDealLevel) = 0;
};
#endif
