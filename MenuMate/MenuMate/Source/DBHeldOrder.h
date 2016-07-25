//---------------------------------------------------------------------------

#ifndef DBHeldOrderH
#define DBHeldOrderH

#include <vector>

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>

#include "MM_DBCore.h"
#include "PnMOrder.h"
#include "SeatOrders.h"
#include "ChitNumber.h"
#include "SendForm.h"

class TDBHeldOrder
{
public:
	static void HoldOrders(Database::TDBTransaction &DBTransaction, TList *OrderObjects, std::vector<TPatronType> Patrons, int ActiveChitNumberKey = -1);

	static void GetHeldOrdersFromChit(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int ActiveChitNumberKey);
	static void GetHeldOrdersFromTable(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int TableNumber);
	static void GetHeldOrdersFromTab(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int TabKey);
	static void GetHeldOrdersFromKey(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int HeldOrderKey);

	static void DeleteHeldOrderByKey(Database::TDBTransaction &DBTransaction, int HeldOrderKey);

	static std::vector<TChitInfo> GetChitInfoOfHeldOrders(Database::TDBTransaction &DBTransaction);
	static std::vector<TTableInfo> GetTableInfoOfHeldOrders(Database::TDBTransaction &DBTransaction);
	static std::vector<TTabInfo> GetTabInfoOfHeldOrders(Database::TDBTransaction &DBTransaction);
	static std::vector<THeldOrderInfo> GetKeyInfoOfUnindexedHeldOrders(Database::TDBTransaction &DBTransaction);

	static std::vector<int> GetHeldOrderKeysFromChitKey(Database::TDBTransaction &DBTransaction, int ChitKey);
	static std::vector<int> GetHeldOrderKeysFromTableNumber(Database::TDBTransaction &DBTransaction, int TableNumber);
	static std::vector<int> GetHeldOrderKeysFromTabKey(Database::TDBTransaction &DBTransaction, int TabKey);

private:
	TDBHeldOrder();
	~TDBHeldOrder();

	static bool ProcessOrderToHold(Database::TDBTransaction &DBTransaction, TItemComplete *Order);

	static int  SetHeldOrder(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int ActiveChitNumberKey);
	static void SetHeldOrderPatronCount(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> Patrons, int HeldOrderKey, int SeatCount);
	static void SetHeldOrderItem(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int HeldOrderKey, int ActiveChitNumberKey);
	static void SetHeldOrderSideItems(Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void SetHeldOption(Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void SetHeldOrderItemCategory(Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void SetHeldOrderSideItemCategory(Database::TDBTransaction &DBTransaction, TItemCompleteSub *SubOrder);

	static void GetHeldOrdersFromIndex		  (Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, UnicodeString IndexName, int IndexValue);
	static void LoadHeldOrders				  (Database::TDBTransaction &DBTransaction, TIBSQL *OrderTable, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons);
	static void LoadHeldOrderOptions		  (Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void LoadHeldOrderSides			  (Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void LoadHeldOrderServingCourse	  (Database::TDBTransaction &DBTransaction, TServingCourse &ServingCourse);
	static void LoadHeldOrderPrinting		  (Database::TDBTransaction &DBTransaction, TItemComplete *Order);
	static void LoadHeldOrderSecurity		  (Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security);
	static void LoadHeldOrderItemCategory	  (Database::TDBTransaction &DBTransaction, int HeldItemKey, TItemComplete *Order);
	static void LoadHeldOrderSideItemCategory (Database::TDBTransaction &DBTransaction, int HeldItemKey, TItemCompleteSub *SubOrder);
	static void LoadHeldOrderCategory		  (Database::TDBTransaction &DBTransaction, UnicodeString TableName, int HeldItemKey, TItemMinorComplete *Item);
	static void LoadHeldOrderPatronCount	  (Database::TDBTransaction &DBTransaction, std::vector<TPatronType> &Patrons, int HeldOrderKey);

	static void DeleteHeldOrderPatronCount(Database::TDBTransaction &DBTransaction, int HeldOrderKey);
	static void DeleteHeldOrderItemCategory(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey);
	static void DeleteHeldOrderSideItemCategory(Database::TDBTransaction &DBTransaction, int HeldOrderSideItemKey);
	static void DeleteHeldOrderItemOption(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey);
	static void DeleteHeldOrderSideItem(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey);
	static void DeleteHeldOrderItem(Database::TDBTransaction &DBTransaction, int HeldOrderKey);
	static void DeleteHeldOrder(Database::TDBTransaction &DBTransaction, int HeldOrderKey);

	static std::vector<int> GetHeldOrderKeysFromIndex(Database::TDBTransaction &DBTransaction, UnicodeString IndexName, int IndexValue);
};
//---------------------------------------------------------------------------
#endif
