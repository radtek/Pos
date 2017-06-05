//---------------------------------------------------------------------------

#ifndef ManagerMenusPOSH
#define ManagerMenusPOSH

#include "ManagerMenus.h"
#include "NetMessageMenuChanged.h"
#include "NetMessageMenuAltered.h"
#include "SystemEvents.h"
//---------------------------------------------------------------------------
class TManagerMenusPOS : public TManagerMenus
{
	AnsiString ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape = NULL);
	AnsiString EscapeString(AnsiString Line, char Qualifier, char Escape);
   TListMenu * IsACurrentMenu(int inMenu_Key);
	bool RequiresUpdate(Database::TDBTransaction &DBTransaction,AnsiString inMenu_Name);
	bool RequiresRemove(Database::TDBTransaction &DBTransaction,AnsiString inMenu_Name);
	//void UpdateMenuChanged(Database::TDBControl &DBControl,TNetMessageMenuChanged *Request,bool ShowProgress = true);
	int GetNextUniqueMenuID(Database::TDBTransaction &DBTransaction);
	int GetNextUniqueOptionID(Database::TDBTransaction &DBTransaction);

public:
	// To Tidy this up we need a array of "slots" of "MenuTItem" classes where each MenuTItem has a
	// Current and a New TItem list.
	TListMenu * IsACurrentMenu(AnsiString inMenu_Name);
	TListCourse       *VisibleCourse;
//	TListMenuContainer  *Current; // Current Menus
//	TListMenuContainer  *New; // New Menus.
	TSystemEvents OnMenuChanged;
	TSystemEvents OnTillsMenusAltered;
	TSystemEvents OnNewMenus;
	std::map<int, int> UpdateSync; // Menu_Key, Update_Sync.

    using TManagerMenus::GetCurrentMenus;

	void SwapInNewMenus();
	void ToggleMenus();
    void SetCurrentMenu(const UnicodeString &name);

	bool LoadMenu(TIBDatabase *inDBControl,TStringList *Menu, bool Force);
	void ForceMenu(TIBDatabase *IBDatabase,UnicodeString FileName);

    //.....................................................................

	// New XML menu format ... Used from version 5.18.
	bool LoadMenus( TIBDatabase *inDBControl, AnsiString inDirName  );
	bool LoadMenu(  TIBDatabase *inDBControl, AnsiString inFileName );

    //.....................................................................

	void SelectMenuOfType(TMenuType MenuType);
    void UpdateMenuChanged(Database::TDBControl &DBControl,TNetMessageMenuChanged *Request,bool ShowProgress = true);

	TManagerMenusPOS();
	virtual ~TManagerMenusPOS();
	void MenuChanged(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request,bool ShowProgress = true);
	void MenuAddedRemoved(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request);
	void GetCurrentMenus(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged *Request);
	void UpdateMenuAltered(Database::TDBTransaction &DBTransaction,TNetMessageMenuAltered *Request);
	__property TListMenu   * VisibleMenu  = { read=GetVisibleMenu, write=SetVisibleMenu };
	TServingCourse GetServingCourse(int ServingCourseKey);
private:
	TListMenu   * FVisibleMenu;
	void __fastcall SetVisibleMenu(TListMenu   * value);
	TListMenu   * __fastcall GetVisibleMenu();
};

#endif
