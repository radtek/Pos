// ---------------------------------------------------------------------------

#ifndef FreebieManagerH
#define FreebieManagerH
// ---------------------------------------------------------------------------
#include "Manager.h"

#include "ContactStaff.h"

#include "ItemMinorComplete.h"

enum eFreeSalesRules
{
	eFSRDisabled, eFSREnabledAll, eFSRCash, eFSRSaved
};

class TManagerFreebie : public TManager
{
    static void AdjustFreeCountContact(Database::TDBTransaction &DBTransaction, int ContactKey, AnsiString ItemName, AnsiString SizeName, Currency Qty,int CountRequired);
	static void AdjustFreeCountLocation(Database::TDBTransaction &DBTransaction, int LocationKey, AnsiString ItemName, AnsiString SizeName, int Qty,int CountRequired);
    static bool IsFreeLocation(Database::TDBTransaction &DBTransaction, int LocationKey, TItemMinorComplete * Item);
	static bool IsFreeContact(Database::TDBTransaction &DBTransaction, TItemMinorComplete * Item);
	static void MakeFree(Database::TDBTransaction & DBTransaction, TItemMinorComplete * Item);
public:
	// Contact
	void HasPurchased(Database::TDBTransaction &DBTransaction, TMMContactInfo *Member, AnsiString ItemName, AnsiString SizeName, Currency Qty, int CountRequired);
	// Location
	void HasPurchased(Database::TDBTransaction &DBTransaction, int LocationKey, AnsiString ItemName, AnsiString SizeName, int Qty, int CountRequired);

    static void IsPurchasing(Database::TDBTransaction & DBTransaction,TList *OrderList);
	static void UndoFreeCount(Database::TDBTransaction & DBTransaction, TList *OrderList);


	eFreeSalesRules MemberLoyaltyRewardsRules;
	eFreeSalesRules LocationLoyaltyRewardsRules;

	TManagerFreebie();
	void Initialise(Database::TDBTransaction &DBTransaction);
	void GetReportMemberInfo(Database::TDBTransaction &DBTransaction, int ContactKey, TStringList *Report);


};

extern TManagerFreebie *ManagerFreebie;
#endif
