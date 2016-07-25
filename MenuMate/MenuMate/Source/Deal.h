//---------------------------------------------------------------------------

#ifndef DealH
#define DealH

#include "IStoredDeal.h"
#include "DealTypeDefs.h"

//---------------------------------------------------------------------------
class TDeal : public IStoredDeal
{
	public:
		TDeal();
		TDeal(UnicodeString inName, int inPriority, int inCategoryKey, int inDealID);
		~TDeal();

		UnicodeString GetName();
		int GetPriority();
		int GetCategoryKey(); //Link to breakdown catgory for this deal. Not sure if this is the best varible to use so subject to change.
		int GetDealID();
		//IDealLevel GetDealLevel(int inDealLevelIndex);
		StoredDealLevelSet GetStoredDealLevels();

		void SetName(UnicodeString inName);
		void SetPriority(int inPriority);
		void SetCategoryKey(int inCatKey);
		void SetDealID(int inDealID);
		void SetStoredDealLevels(StoredDealLevelSet inDealLevels);
		//void AddDealLevel(IDealLevel inDealLevel);
		//void RemoveDealLevel(IDealLevel inDealLevel);

	private:
		UnicodeString name;
		int priority;
		int categoryKey;
		int dealID;
		StoredDealLevelSet dealLevels;
};
#endif
