//---------------------------------------------------------------------------

#ifndef StockManagerH
#define StockManagerH
//---------------------------------------------------------------------------
#include "ItemComplete.h"
#include "ItemCompleteSub.h"

#include "StockInterface.h"
#include "MM_DBCore.h"


class TManagerStock
{
	private:
		void UpdateStock(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
		void UpdateStock(Database::TDBTransaction &DBTransaction,TItemCompleteSub *Order);

		// Used for Stock on hand.
		StockInterface::TStockInterface *StockModule;
		bool StockInterfaceEnabled;

	public :
		void OpenDB();
		void UpdateStockCost(Database::TDBTransaction &DBTransaction,UnicodeString inStockCode, UnicodeString inLocation, UnicodeString inUnit, UnicodeString inDescription, Currency inCost,Currency inCostGSTPercent);
		void UpdateStock(Database::TDBTransaction &DBTransaction,TList *OrderObjects);
		bool GetStockDetails(Database::TDBTransaction &DBTransaction,UnicodeString StockCode,UnicodeString StockLocation, UnicodeString &StockDescription,
			UnicodeString &StockUnit, Currency &StockCost, double &StockGST);
	TManagerStock();
	virtual ~TManagerStock();
	void GetStockOnHand(Database::TDBTransaction &DBTransaction,TListRecipeContainer * Recipes,TStringList *Report);
	bool GetStockTrackQty(Database::TDBTransaction &DBTransaction,UnicodeString StockCode,UnicodeString StockLocation,double &StockTrackQty);
	void GetCourseStockReport(Database::TDBTransaction &DBTransaction,TListCourse *Course,TStringList *html);
	void UpdateItemCosts(Database::TDBTransaction &DBTransaction,UnicodeString inStockCode,Currency inCost,Currency inCostGSTPercent);
};

#endif
