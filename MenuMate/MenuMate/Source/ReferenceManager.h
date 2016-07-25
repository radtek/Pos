//---------------------------------------------------------------------------

#ifndef ReferenceManagerH
#define ReferenceManagerH

#include "MM_DBCore.h"
#include <system.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <memory>
#include <vector>
#include <Grids.hpp>

//---------------------------------------------------------------------------
/*
#define REFTYPE_EFTPOS 1
#define REFTYPE_PMS 2
#define REFTYPE_INVOICE 3
#define REFTYPE_POCKETVOUCHER 4
#define REFTYPE_USER 255*/

enum REFTYPE { REFTYPE_EFTPOS = 1,
					REFTYPE_PMS = 2,
               REFTYPE_INVOICE = 3,
					REFTYPE_POCKETVOUCHER_CODE = 4,
               REFTYPE_POCKETVOUCHER_PVTRANS = 5,
				REFTYPE_POCKETVOUCHER_MMTRANS = 6,
				REFTYPE_GENERAL_LEDGER = 7,
               REFTYPE_USER = 255};


class TReference
{
	public:
	TReference() : Key(0),Type(REFTYPE_USER),Name(""),Code(""){}
	int Key;
	REFTYPE Type;
	AnsiString Name;
	AnsiString Code;
};

typedef std::pair<AnsiString,TReference> RefRefType;

class TManagerReference
{
	private :
	public:
	TManagerReference();
	~TManagerReference();

	int GetCount(Database::TDBTransaction &DBTransaction);
	TReference GetReference(Database::TDBTransaction &DBTransaction, int Key);
	TReference GetReferenceByType(Database::TDBTransaction &DBTransaction, REFTYPE Type);
	void GetReferences(Database::TDBTransaction &DBTransaction,TStringList *List);
	void GetReferences(Database::TDBTransaction &DBTransaction,TStringList *List,REFTYPE TypeFilter);
	void GetReferences(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid);
	void SetReference(Database::TDBTransaction &DBTransaction,long Key,TReference Reference);
	void Delete(Database::TDBTransaction &DBTransaction,int Key);
   void Initialise(Database::TDBTransaction &DBTransaction);
   AnsiString BuildReference(int SiteID,AnsiString TerminalName,REFTYPE TYPE);
};

#endif
