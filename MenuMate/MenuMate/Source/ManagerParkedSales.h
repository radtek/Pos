//---------------------------------------------------------------------------
#ifndef ManagerParkedSalesH
#define ManagerParkedSalesH

#include "MM_DBCore.h"
#include "ParkedSale.h"

//---------------------------------------------------------------------------
class TManagerParkedSales
{
	public:
	   TManagerParkedSales();
      int GetCount(Database::TDBTransaction &DBTransaction);
      void Delete(Database::TDBTransaction &DBTransaction,int TabKey);
      void SetParkedSale(Database::TDBTransaction &DBTransaction,TParkedSale *Sale);
      void DeleteAll(Database::TDBTransaction &DBTransaction);
      TParkedSale * GetParkedSale(Database::TDBTransaction &DBTransaction,int TabKey);
      TParkedSale * GetFirstParkedSale(Database::TDBTransaction &DBTransaction);
      void GetParkedSales(Database::TDBTransaction &DBTransaction,TStringList *ParkedSales);
      int SettempParkedSale(Database::TDBTransaction &DBTransaction,TParkedSale *Sale);

    protected:
      unsigned __int32 getMaxSeatCount();
};

#endif
