//---------------------------------------------------------------------------

#ifndef DBTablesH
#define DBTablesH
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "TableSeat.h"
#include "PatronType.h"
#include <vector>
#include <memory>
#include <set>
#include "MezzanineDetails.h"

enum TOrderBillingStatus {eNoneStatus, ePrelim, eCallAwayStatus,ePartialSplit};


//---------------------------------------------------------------------------
class TDBTables
{
	private :

	static UnicodeString GetNameMinor(Database::TDBTransaction &DBTransaction,int TabKey);

        //insert mizzanine table record.
    static void InsertMezzanineTablesRecord(Database::TDBTransaction &dbTransaction, int tableNumber, TMezzanineTable mezzanineDetails);

    //delete mizzanine table record.
    static void DeleteMezzanineTablesRecord(Database::TDBTransaction &dbTransaction, int tableNumber, TMezzanineTable mezzanineDetails);

	public:
	static int GetOrCreateTable(Database::TDBTransaction &DBTransaction, int inTableNo, bool IsTableSelected = true);
    static AnsiString UpdateTableStatus(Database::TDBTransaction &DBTransaction,int inTableNo, bool IsTableSelected = true);
	static int GetOrCreateSeat(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);

	static int GetSeatKey(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetSeatNo(Database::TDBTransaction &DBTransaction,int SeatKey);
	static int GetSeatNoFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetTableKey(Database::TDBTransaction &DBTransaction,int SeatKey);
	static int GetTableNo(Database::TDBTransaction &DBTransaction,int TableKey);
	static int GetTabKey(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);
	static int GetTabKey(Database::TDBTransaction &DBTransaction,int inSeatKey);
	static void GetTabKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedTabs);
	static void GetOrderKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedOrders);
	static void GetTabKeysWithOrders(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> *SelectedTabs);
	static void GetSeats(Database::TDBTransaction &DBTransaction,TStringList * TabList, int inTableNo);
	static void SetTableName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString TableName);

	static UnicodeString GetPartyNameShort(Database::TDBTransaction &DBTransaction,int inTableNo);
	static UnicodeString GetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo);
	static UnicodeString GetTableName(Database::TDBTransaction &DBTransaction,int inTableNo);
	static UnicodeString GetNameMinor(Database::TDBTransaction &DBTransaction,int TableNumber , int SeatNumber);
	static UnicodeString GetNameMinorShort(Database::TDBTransaction &DBTransaction,int TabKey);

    static std::vector<TPatronType> GetPatronCount(Database::TDBTransaction &DBTransaction, int tableNo);
	static void SetPatronCount(Database::TDBTransaction &DBTransaction, int tableNo, std::vector<TPatronType> patronTypes);
    static void ShiftPatronCountToNewTable(Database::TDBTransaction &DBTransaction, int sourceTableNo, int destinationTableNo);
    static void ClearPatronCount(Database::TDBTransaction &DBTransaction, int tableNo);
    static void ClearPatronCounts(Database::TDBTransaction &DBTransaction);

	static void SetSeatTab(Database::TDBTransaction &DBTransaction,int inSeatKey,int inTabKey);
	static void SetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString PartyName);
	static void SetSeatName(Database::TDBTransaction &DBTransaction, UnicodeString Name, int TableNo, int SeatNumber);
    static void ClearSeatsName(Database::TDBTransaction &DBTransaction,int TableNo);
	static bool Valid(int inTableNo);
	static bool Valid(int inTableNo, int inSeatNo);
    static bool IsTableLocked(Database::TDBTransaction &DBTransaction,int TableNumber);

	static bool IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo, int inSeatNo);
	static bool IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo);

	static bool GetTableSeat(Database::TDBTransaction &DBTransaction,int TabKey,TTableSeat *Info);
	static bool GetSeatExists(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);
	static bool GetTableExists(Database::TDBTransaction &DBTransaction,int inTableNo);
	static void SetSeatTabToNull(Database::TDBTransaction &DBTransaction,int inTabKey);

   	static Currency GetTableTotal(Database::TDBTransaction &DBTransaction,int TableNo);
   	static Currency GetTableBalance(Database::TDBTransaction &DBTransaction,int TableNo);
    static void GetPartyNames(Database::TDBTransaction &DBTransaction, std::map<int,UnicodeString> &PartyNames);
    static void GetTableStatus(Database::TDBTransaction &DBTransaction, std::map<int,UnicodeString> &Status);
    static void SetTableBillingStatus(Database::TDBTransaction &DBTransaction,int TableNo,TOrderBillingStatus status);
    static int GetTableBillingStatus(Database::TDBTransaction &DBTransaction,int TableNo);
    static TDateTime GetMaxTableBillingTimeStamp(Database::TDBTransaction &DBTransaction,int TableNo);
    static std::vector<TPatronType> TDBTables::GetBilledPatronCount(Database::TDBTransaction &DBTransaction, int tabKey);
    static int GetPatronNumbers( Database::TDBTransaction &DBTransaction,int TableNumber);
    static void SetPatronNumbers( Database::TDBTransaction &DBTransaction,int TableNumber,int PatronCount);
    static bool IsSeatPresent( Database::TDBTransaction &DBTransaction,int TableNumber,int SeatNumber);
    static int GetPatronNumbersForWebOrders( Database::TDBTransaction &DBTransaction,int TableNumber);
    static void UpdateTablePartyName( Database::TDBTransaction &dbTransaction, int TableNumber);

    //saveMezzanine Area Tables.
    static void SaveMezzanineAreaTables(std::map<int, std::vector<TMezzanineTable> > mezzanineTables);

    //load table which are in mezzanine area.
    static std::set<int> GetMezzanineAreaTables(TMezzanineTable mezzanineDetails);
    static bool HasOnlineOrders(int tableNumber);
    static UnicodeString GetMemberEmail(int tableNumber);
};

#endif
