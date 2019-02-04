//---------------------------------------------------------------------------
#ifndef DBOrderH
#define DBOrderH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>

#include "MM_DBCore.h"
#include "PnMOrder.h"
#include "SeatOrders.h"
#include "enum.h"

class TDBOrder
{
	private :
    static bool IsSCDOrPWDDiscountConfigured(Database::TDBTransaction &DBTransaction, UnicodeString discountGroup);
    static void LoadOrderKeysWIthSCDOrPWDDiscount(Database::TDBTransaction &dBTransaction, int tabKey, std::set<__int64> &orderKeysWithSCDOrPWDDiscount,
                                                UnicodeString discountGroup);
    static void LoadOrderKeysWIthoutDiscount(Database::TDBTransaction &dBTransaction, int tabKey, std::set<__int64> &orderKeysWithSCDOrPWDDiscount);
public:
	TDBOrder();
	~TDBOrder();

	static void TransferOrders(Database::TDBTransaction &DBTransaction,TList *Orders, int TabDestKey, int UserKey, int TabSourceKey, bool isTransactionCommit=false);
	static bool CheckTransferCredit(Database::TDBTransaction &DBTransaction,TList *Orders, int TabDestKey);
	static __int64 GetNextTransNumber(Database::TDBTransaction &DBTransaction);
	static __int64 GetNextSetMenuGroupNumber(Database::TDBTransaction &DBTransaction);
	static void UpdateOrder(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order);
	//	__int64 SplitOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey, Currency ValueRequired);
	static __int64 SplitOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey, int WaysToSplit);
    static __int64 SplitOrderToQuantity(Database::TDBTransaction &DBTransaction,__int64 OrderKey, double quantityToSplit);
	//	TItemComplete *SplitOrder(Database::TDBTransaction &DBTransaction,TItemComplete *Order, Currency ValueRequired);

	static void DeleteOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order);
	static void SetOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order);

	static void DeleteOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order);
	static void SetOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order);

	static bool DeductOrder(
	TItemMinorComplete * const Order1,
	TItemMinorComplete * const Order2,
	const bool alter_patron_count = true);

	static void GetOrdersToValuesOfFromTabs(Database::TDBTransaction &DBTransaction,int TabKey, Currency AmountToPay, TList *Orders);
	static void CancelOrder(Database::TDBTransaction &DBTransaction,TItemComplete * Order);
	static void CancelOrder(Database::TDBTransaction &DBTransaction,TList *OrderObjects);
	static void SetOrder(Database::TDBTransaction &DBTransaction,TItemComplete * Order);

	static void SetSecurityRef(Database::TDBTransaction &DBTransaction,int OrderKey,int OldRef,int SecurityRef);
	static void SetOrdersLoyalty(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys, int inContactKey);
	static void ApplyPricingLevel0(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys,bool HappyHourFilter = false);
	static void ApplyPricingLevel1(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys,bool HappyHourFilter = false);

	static void GetPrevOrders(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName, int TableNo,int SeatNo,std::vector<TSeatOrders *> &SeatOrders);
	static void GetPrevOrder(Database::TDBTransaction &DBTransaction,int OrderKey,TItemMinorComplete *TItemMinorComplete);
	static int GetSetMenuGroupNumber(Database::TDBTransaction &DBTransaction);

	static void LoadPickNMixOrder(Database::TDBTransaction &DBTransaction,int OrderKey,TPnMOrder *Order);

	static void LoadPickNMixOrders(Database::TDBTransaction &DBTransaction,int TabKey, std::vector<TPnMOrder> &Orders);
	static double LoadPickNMixOrdersAndGetQuantity(Database::TDBTransaction &DBTransaction,int TabKey, std::map<__int64,TPnMOrder> &Orders, bool SelectingItems = false);
	static void LoadPickNMixOrders(Database::TDBTransaction &DBTransaction,std::set<__int64> &OrderKeys, std::map<__int64,TPnMOrder> &Orders);

	static void GetOrdersFromOrderKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> OrderKeys);
	static void GetOrdersIncludingSidesFromOrderKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> OrderKeys, bool isItemTransfer= false);
	static void GetMemberKeysFromOrderKeys(Database::TDBTransaction &DBTransaction,std::set<__int64> &MemberKeys, std::set<__int64> OrderKeys);
	static void GetTabKeysFromOrders(TList *Orders, std::set<__int64> &TabKeys);

	static void GetOrderFromOrderKey(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
	static void GetOrderIncludingSidesFromOrderKey(Database::TDBTransaction &DBTransaction,TItemComplete *Order, bool isItemTransfer = false);

	static void GetOrdersFromTabKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> SelectedTabs, bool isSide = false);
	static void GetOrdersIncludingSidesFromTabKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> SelectedTabs,  bool isItemTransfer = false);
	static bool GetOrderExists(Database::TDBTransaction &DBTransaction,int OrderKey);
	static void GetOrderKeys(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> &SelectedItems);
	static int SetArchiveCategory(Database::TDBTransaction &DBTransaction,UnicodeString Category);
	static int GetArchiveCategory(Database::TDBTransaction &DBTransaction,UnicodeString Category);
	static UnicodeString GetArchiveCategory(Database::TDBTransaction &DBTransaction,int CategoryKey);

	static void SetArchiveCategoryEnabled(Database::TDBTransaction &DBTransaction,int CategoryKey, bool Enabled);
	static void SetArchiveCategoryGroupEnabled(Database::TDBTransaction &DBTransaction,int CategoryGroupKey, bool Enabled);

	static int SetCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup);
	static int SetCategoryToGroup(Database::TDBTransaction &DBTransaction,int CategoryKey, int CategoryGroupKey);

	static int GetCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup);
	static UnicodeString GetCategoryGroup(Database::TDBTransaction &DBTransaction,int CategoryGroupKey);
	static int GetCategoriesCategoryGroup(Database::TDBTransaction &DBTransaction,int CategoryKey);
	static int GetOrCreateCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup);
	static void GetCategoryList(Database::TDBTransaction &DBTransaction,TStringList *CategoryList);
	static bool GetCategoryEnabled(Database::TDBTransaction &DBTransaction,int CategoryKey);
	static bool GetCategoryGroupEnabled(Database::TDBTransaction &DBTransaction,int CategoryGroupKey);
	static bool DeleteOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey);
	static void CloneOrders(Database::TDBTransaction &DBTransaction,TList *SourceOrders, TList *DestOrders);
	static void CloneOrdersPartialPaymentDifference(Database::TDBTransaction &DBTransaction,TList * const SourceOrders, TList * const DestOrders, const double &SplitPercentage,const Currency &RequestedTotal);
	static void CloneOrdersSplitPaymentDifference(Database::TDBTransaction &DBTransaction,TList * const SourceOrders, TList * const DestOrders, const double &SplitPercentage,const Currency &RequestedTotal);
	static void LoadOrder(Database::TDBTransaction &DBTransaction,TIBSQL *OrderTable,TItemComplete *Order);
	static void LoadOrderPrinting(TIBSQL *OrderTable,TItemComplete *Order);
	static void LoadOrderSecurity(Database::TDBTransaction &DBTransaction,TListSecurityRefContainer *Security);
	static void LoadOrderSide(Database::TDBTransaction &DBTransaction,TListSubOrderContainer *SubOrders,TIBSQL *SubOrdersTable);
	static void LoadOrderCategories(Database::TDBTransaction &DBTransaction, int OrderKey,TListCategoryContainer *Categories);
	static void LoadOrderOptions(TIBSQL *OptionsTable,TListOptionContainer *OptionsSelected);
	static void LoadOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order);
	static void LoadOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order);
	static void LoadOrderServingCourse(TIBSQL *ServeringCourseTable, TServingCourse &ServingCourse);
	static void LoadOrderReceipe(TIBSQL *RecipeTable,TListRecipeContainer *Recipes);
	static Currency LoadCreditFromOrders(Database::TDBTransaction &DBTransaction,TList *Orders);
	static void SetChitNumber(Database::TDBTransaction &DBTransaction,int NewChitNumber);
	static void AssignNewSecurityRef(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
	// Processes a new incomming Order.
	static void __fastcall ProcessOrders(Database::TDBTransaction &DBTransaction,TList *OrderObjects);
	static void __fastcall ProcessOrder(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
	static void GetOrdersFromOrderGUID(Database::TDBTransaction &DBTransaction,std::set<__int64> &OrderKeys, std::vector<UnicodeString> OrderGUID);
	static void GetArchiveCategories(Database::TDBTransaction &DBTransaction,std::map<int,UnicodeString > &CatList);

	//Get Customer Name and Order Type
	static void GetCNaOTFromTableNum(Database::TDBTransaction &DBTransaction, int TableNumber, std::pair<int, AnsiString> &CustNameAndOrderType);
	static bool GetCanBePaidWithPoints(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
	static bool MembershipApplied(Database::TDBTransaction &DBTransaction,__int64 MemberKey);
	static void UpdateOrdersForPartialPayment( Database::TDBTransaction &DBTransaction,    TList * SourceOrders, TList * PayableOrders,
	TItemComplete *SplittedItem,Currency RequestedTotal);
	static int GetTimeKey( Database::TDBTransaction &DBTransaction,int TableNumber);
	static void UpdateOrderForDelayedPayment(Database::TDBTransaction &DBTransaction,
	TItemComplete * Order,
	AnsiString sourceName,
	int TabKey,
	AnsiString TabName,
	AnsiString InvoiceNumber,
	int UserKey,
	TDateTime TransferTime);
	static int GetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,AnsiString tabName);
	static int GetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,int tabKey);
	static int GetOrderIdentificationNumberForTable(Database::TDBTransaction &DBTransaction,int tableNumber);
	static void SetOrderIdentificationNumberForTable(Database::TDBTransaction &DBTransaction,int tableNumber,int identificationNumber);
	static void SetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,AnsiString tabName,int identificationNumber);
	static void SetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,int tabKey,int identificationNumber);
	static void SetOrderIdentificationNumber(Database::TDBTransaction &DBTransaction,TList *Orders, int identificationNumber);
    static bool IsItemUsingPCD(Database::TDBTransaction &DBTransaction, int itemKey, AnsiString sizeName);
    static bool CheckItemHasSide(Database::TDBTransaction &DBTransaction, int Order_key);
    static UnicodeString GetContactName(Database::TDBTransaction &DBTransaction, int tab_key);
    static void TDBOrder::UpdateOrderTableForWebOrders(Database::TDBTransaction &DBTransaction, TItemComplete *Order);
    static Currency GetPriceForPoints(Database::TDBTransaction &DBTransaction,TItemComplete *Order);
    static void UpdateTabNameInOrder(Database::TDBTransaction &DBTransaction,long SourceKey, UnicodeString clipTabName,bool isTabSelected);
    static int CheckItemAvailability(Database::TDBTransaction &DBTransaction, int itemKey, AnsiString sizeName);
    static void SortOrders(TList * Orders);
    static void DeleteOrdersForreatructure(TList * Orders);
    static UnicodeString getOrderKeysList(TList *Orders);
    static void SetMemberEmailLoyaltyKeyForTable(Database::TDBTransaction &DBTransaction,int tableNumber,int loyaltyKey, UnicodeString email);
    static void SetMemberEmailLoyaltyKeyForTab(Database::TDBTransaction &DBTransaction,int tabKey,int loyaltyKey, UnicodeString email);
    static bool IsOrderSavedToTable(Database::TDBTransaction &DBTransaction, int tableNumber);
};
//---------------------------------------------------------------------------
#endif
