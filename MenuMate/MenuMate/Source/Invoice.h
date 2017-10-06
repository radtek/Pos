//---------------------------------------------------------------------------

#ifndef InvoiceH
#define InvoiceH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <set>
#include "MM_DBCore.h"

class TInvoice
{
	private:
		bool fEnabled;
      void __fastcall SetEnabled(bool value);
      bool __fastcall GetEnabled();

	public:
   	TInvoice();
   	~TInvoice();
	   __property bool Enabled  = { read=GetEnabled, write=SetEnabled };
      void Initialise();

	UnicodeString InvoiceNumber;

	int CreateInvoice(Database::TDBTransaction &DBTransaction,int ContactKey,Currency TotalCostExcl,Currency TotalCost,UnicodeString InvoiceNumber);
	UnicodeString GetInvoiceNumber(Database::TDBTransaction &DBTransaction,int InvoiceKey);
	int GetInvoiceTabs(Database::TDBTransaction &DBTransaction,int InvoiceKey,std::set<long> *Tabs);
	void SetInvoiceClosed(Database::TDBTransaction &DBTransaction,int inInvoiceKey);
	bool GetOutStanding(Database::TDBTransaction &DBTransaction,int ContactKey);
	int GetInvoiceExists(Database::TDBTransaction &DBTransaction,UnicodeString InvoiceNumber);
	int GetInvoiceOwner(Database::TDBTransaction &DBTransaction,int InvoiceKey);
   bool GetOwnerHasInvoice(Database::TDBTransaction &DBTransaction,int inContactKey);   
	UnicodeString GetNextInvoiceNumber(Database::TDBTransaction &DBTransaction);
    UnicodeString GetNextInvoiceNumber(Database::TDBTransaction &DBTransaction, int TypeOfSale);
	void SetOrdersToInvoiceFormat(Database::TDBTransaction &DBTransaction,int InvoiceKey,int InvoiceTabKey,UnicodeString InvoiceOwner);
    UnicodeString GetVoidInvoiceNumber(Database::TDBTransaction &DBTransaction);
    UnicodeString GetBeveragesInvoiceNumber(Database::TDBTransaction &DBTransaction);

};

extern TInvoice *Invoice;
//---------------------------------------------------------------------------
#endif
