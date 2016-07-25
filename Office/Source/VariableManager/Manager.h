//---------------------------------------------------------------------------

#ifndef ManagerH
#define ManagerH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>

#include "MM_DBCore.h"
//---------------------------------------------------------------------------

class TManager
{
	private :
	protected :
		bool fEnabled;
      void __fastcall SetEnabled(bool value);
      bool __fastcall GetEnabled();

	public:
		__property bool Enabled  = { read=GetEnabled, write=SetEnabled };
      TManager();
      virtual ~TManager();
};

class TManagerInterface : public TManager
{
	public :
   AnsiString virtual GetContent(Database::TDBTransaction &DBTransaction,int Key) = 0;
	AnsiString virtual GetTitle(Database::TDBTransaction &DBTransaction,int Key) = 0;
   void virtual GetListTitle(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter) = 0;
   void virtual GetListContent(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter) = 0;
	int virtual GetNew(Database::TDBTransaction &DBTransaction,int Filter) = 0;
   int virtual GetCount(Database::TDBTransaction &DBTransaction,int Filter) = 0;

	void virtual SetContent(Database::TDBTransaction &DBTransaction,int Key,AnsiString Content) = 0;
	void virtual SetTitle(Database::TDBTransaction &DBTransaction,int Key,AnsiString Title) = 0;
	void virtual SetOrder(Database::TDBTransaction &DBTransaction,int Key,int AppearanceOrder) = 0;
   void virtual SetFilter(Database::TDBTransaction &DBTransaction,int Key,int Filter) = 0;
   void virtual Delete(Database::TDBTransaction &DBTransaction,int Key) = 0;
};

#endif
