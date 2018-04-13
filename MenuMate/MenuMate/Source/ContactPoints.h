//---------------------------------------------------------------------------

#ifndef ContactPointsH
#define ContactPointsH
//---------------------------------------------------------------------------
#include "Classes.hpp"
#include "MM_DBCore.h"
#include "PointsType.h"
#include "PointsRulesSet.h"
#include "PointsTransaction.h"
#include <map>

typedef std::map<TPointsType, Currency> TPointsStore;

class TContactPoints
{
	private:

    TPointsStore PointsStore;


    Currency GetPointsValue(TPointsType type);
    Currency GetPointsValue(TPointsTypePair type);
    Currency GetPointsValueWithoutSale(TPointsTypePair type);
    Currency GetPointsValue(TPointsAssignedSource inSource);
    Currency GetCurrentPointsValue(TPointsTypePair type);

    /* Works though the Orders List and recalculates the amount of earned points
    and updates the PointsStore. */
	void CalcEarned(TList *OrdersList , bool IsallowtoEarnWhileRedeem);
        Currency CalcLoadedPointsValue(TList *OrdersList);
    /*From the Points Store retrives the amount of points redeemed and spreads them
    across all items prioritised based on whether those items earn them points */
	void ReAssignRedeemed(TList *OrdersList);


	public :

    TPointsRulesSet PointsRules;
    TPointsRulesSubsSet PointsRulesSubs;
    /* The "Per transaction' ExportStatus is held in the Key Value (TPointsType)
    for the Map when the element to be sotred in the Map is created.
    The Key value for the Map is const by definition, so we cant even the
    elements in the object.

    However as all TContactPoints are expoerted at once we can hold a variable here
    to repersent this change in status.

    In the furture the Map could be adjusted so the Key is of type TPointsTypePair
    (change to a proper class) with theoperator overloading of TPointsType moved
    into the class as well, as it's only used for Map Key comparasions.*/

    TPointsExportStatus ExportStatus;
    TDateTime TimeStampExported;

    /* Returns the Points total filtered by where the points came from
    i.e to get the original balance when the contact was loaded use pasDatabase.
    */
    Currency getPointsBalance(TPointsAssignedSource Source);
    Currency getPointsEarned(TPointsAssignedSource Source);
    Currency getPointsRedeemed(TPointsAssignedSource Source);
    Currency getPointsPurchased(TPointsAssignedSource Source);
    Currency getPointsRefunded(TPointsAssignedSource Source);


    /* Use this to get the Points by Account type,
    	i.e Loyalty points, those normally earned on food etc.
        Account Points, the Extra account for Aussie loyalty.
        Held Points, Point in the "Held Account" that have not yet been applied.*/
    Currency getPointsBalance(TPointsTransactionAccountType Account);
    Currency getPointsEarned(TPointsTransactionAccountType Account);
    Currency getPointsRedeemed(TPointsTransactionAccountType Account);
    Currency getPointsPurchased(TPointsTransactionAccountType Account);
    Currency getPointsRefunded(TPointsTransactionAccountType Account);
    Currency getPointsBalanceFromDBWithLoyaltymate(TPointsTransactionAccountType Account);

	/* A very specialised Balance function that return the balance by Source
     and account type, Used for retriving the pasDatabase balance of the
     loyalty account in order to get the maxamum amount of points that can be spent
     while excluding the current points that will be earned on the current transaction

     Simply put you can only spend what you already have in the DB not what you have
     on top of what you are going to get.*/
    Currency getPointsBalance(TPointsAssignedSource Source, TPointsTransactionAccountType Account);

    /* Returns the Totals reguardless of Account or source */
    Currency getPointsBalance();
    Currency getPointsEarned();
    Currency getPointsRedeemed();
    Currency getPointsPurchased();
    Currency getBirthDayRewardPoints();
    Currency getFirstVisitPoints();
    Currency getPointsRefunded();

	/* Returns the current total amounts Earned Redeemed and Purcahsed in this
    Transaction. i.e this excludes Values loaded from the DB.*/
    Currency getCurrentPointsEarned();
    Currency getCurrentPointsRedeemed();
    Currency getCurrentPointsRedeemed(TPointsTransactionAccountType Account);
    Currency getCurrentPointsPurchased();
    Currency getCurrentPointsRefunded();

    /* We really shouldn't give a breakdown of held points by type
    given they are held by the system they are not in an 'accout' yet */
    Currency getPointsHeldPurchased();
	Currency getPointsHeldEarned();
	Currency getPointsHeldTotal();

    TContactPoints();
    void Assign(TContactPoints &inPoints);
    void Clear();
    void ClearPoints();
    void ClearBySource(TPointsAssignedSource Source);
    void ClearByAccountType(TPointsTypePair PointsToClear);
    /* Clears all sources except pasDatabase */
    void ResetPoints();

    /* Will reload or recalculate points as some have been used to apply a discount.
    	May not be needed at this stage there are several options.
        1) Have this class maintain a list of PointsTransactions.
        	Then have the system add a transaction when a "Points Discount is created."
        2) Have the discount system call Load, and add a TPointsType here when a dicount is created
        	or removed. Then when the discount is saved and the points changes applied
            a PointsTransactions can be created to balance the books.

        3) (this would be messy) Have the discount engine spend and refund points from the DB and then call
            reload on this structure.
    */
    void Recalc(TList *OrdersList,int memberType,bool isRefundTransaction,bool HasReedeempoints);

    void LoadFromStreamVer1(TMemoryStream *Stream);
    void LoadFromStream(int CardVersion,TMemoryStream *Stream);
    void Load(TPointsType type,Currency Points);
    void Add(std::vector <TPointsTransaction> &Transactions);
    //void Add(TContactPoints &Points);


    void SaveToStream(TMemoryStream *Stream);

    void toHTML(TStringList *Report);
    static TPointsTypePair GetPointsType(int AdjustmentType, int AdjustmentSubType);
	static TPointsExportStatus getPointsStatus(int inStatus);

    TPointsStore getExcludingSource(TPointsAssignedSource inSource);
	TPointsStore getPointsStore();
    bool HasTransactions();
    bool HasHeldTransactions();
    void setExportStatus(TPointsExportStatus ExportStatus);
    /* This fills out the store with the empty TPointsType
    It's used when generateing the HTML reports so you can see
    fields with a Balance of 0 (Which are not mornally held in the PointsStore
    As a tidy up you could make TPointsStore it's own class and put this method in it. */
    static void PadOutEmptyStoreTPointsType(TPointsStore &Store);
    Currency getBirthDayRewardRedeemedPoints();
    Currency getFirstVisitRedeemedPoints();
};

#endif

