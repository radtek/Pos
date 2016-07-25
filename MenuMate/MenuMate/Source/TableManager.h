//---------------------------------------------------------------------------

#ifndef TableManagerH
#define TableManagerH
//---------------------------------------------------------------------------
#include "Manager.h"

#include "MM_DBCore.h"
#include "TableSeat.h"

//---------------------------------------------------------------------------
class TManagerTable : public TManager
{
	private :
		UnicodeString GetNameMinor(Database::TDBTransaction &DBTransaction,int TabKey);
	public:
	UnicodeString SeatLabel;
	void Initialise(Database::TDBTransaction &DBTransaction);
	int GetOrCreateTable(Database::TDBTransaction &DBTransaction,int inTableNo);
	int GetOrCreateSeat(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);

	int GetSeatKey(Database::TDBTransaction &DBTransaction,int TabKey);
	int GetSeatNo(Database::TDBTransaction &DBTransaction,int SeatKey);
	int GetSeatNoFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey);
	int GetTableKey(Database::TDBTransaction &DBTransaction,int SeatKey);
	int GetTableNo(Database::TDBTransaction &DBTransaction,int TableKey);
	int GetTabKey(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);
	int GetTabKey(Database::TDBTransaction &DBTransaction,int inSeatKey);
	void GetTabKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedTabs);
	void GetOrderKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedOrders);
	void GetTabKeysWithOrders(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> *SelectedTabs);
	void GetSeats(Database::TDBTransaction &DBTransaction,TStringList * TabList, int inTableNo);
	void SetTableName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString TableName);

	UnicodeString GetPartyNameShort(Database::TDBTransaction &DBTransaction,int inTableNo);
	UnicodeString GetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo);
	UnicodeString GetTableName(Database::TDBTransaction &DBTransaction,int inTableNo);
	UnicodeString GetNameMinor(Database::TDBTransaction &DBTransaction,int TableNumber , int SeatNumber);
	UnicodeString GetNameMinorShort(Database::TDBTransaction &DBTransaction,int TabKey);

	void SetSeatTab(Database::TDBTransaction &DBTransaction,int inSeatKey,int inTabKey);
	void SetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString PartyName);
	void SetSeatName(Database::TDBControl &IBDatabase,UnicodeString Name, int TableNo, int SeatNumber);
   void ClearSeatsName(Database::TDBTransaction &DBTransaction,int TableNo);   
	bool Valid(int inTableNo);
	bool Valid(int inTableNo, int inSeatNo);

	bool IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo, int inSeatNo);
	bool IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo);

	bool GetTableSeat(Database::TDBTransaction &DBTransaction,int TabKey,TTableSeat *Info);	
	bool GetSeatExists(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo);
	bool GetTableExists(Database::TDBTransaction &DBTransaction,int inTableNo);
	void SetSeatTabToNull(Database::TDBTransaction &DBTransaction,int inTabKey);

   Currency GetTableTotal(Database::TDBTransaction &DBTransaction,int TableNo);
   Currency GetTableBalance(Database::TDBTransaction &DBTransaction,int TableNo);      

};

#endif
