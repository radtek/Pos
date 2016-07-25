//---------------------------------------------------------------------------

#ifndef RunManagerH
#define RunManagerH
//---------------------------------------------------------------------------

#include "Manager.h"
#include "Grids.hpp"
#include "MM_DBCore.h"

class TManagerRun : public TManagerInterface
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
   void Delete(Database::TDBTransaction &DBTransaction,int Key);   

	bool RunProgram(UnicodeString ProgramLine);
   int Add(Database::TDBTransaction &DBTransaction,UnicodeString Title,UnicodeString Content,int Order,int Filter);
	void LoadMessages(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid, int Filter);
	void SetFilter(Database::TDBTransaction &DBTransaction,int Key,int Filter) {}
   TManagerRun();
};

extern TManagerRun *ManagerRun;

#endif
