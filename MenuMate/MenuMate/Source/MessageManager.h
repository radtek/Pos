//---------------------------------------------------------------------------

#ifndef MessageManagerH
#define MessageManagerH
//---------------------------------------------------------------------------
#include "Manager.h"
#include "Grids.hpp"
#include "MM_DBCore.h"

class TManagerMessage : public TManagerInterface
{
	public :
 	UnicodeString GetContent(Database::TDBTransaction &DBTransaction,int Key);
	UnicodeString GetTitle(Database::TDBTransaction &DBTransaction,int Key);
	void GetListTitle(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter);
   void GetListContent(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter);   
   int GetCount(Database::TDBTransaction &DBTransaction,int Filter);   
   int GetNew(Database::TDBTransaction &DBTransaction,int Filter);

   void SetContent(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Content);
   void SetTitle(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Title);
   void SetOrder(Database::TDBTransaction &DBTransaction,int Key,int AppearanceOrder);
   void SetFilter(Database::TDBTransaction &DBTransaction,int Key,int Filter);
   void Delete(Database::TDBTransaction &DBTransaction,int Key);   

	int Add(Database::TDBTransaction &DBTransaction,UnicodeString Title,UnicodeString Content,int Order,int Filter);
	void LoadMessages(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid, int Filter);
	TManagerMessage();
};

extern TManagerMessage *ManagerMessage;

#endif
