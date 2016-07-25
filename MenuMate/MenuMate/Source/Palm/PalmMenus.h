//---------------------------------------------------------------------------

#ifndef PalmMenusH
#define PalmMenusH

#ifdef PalmMate
#include "enum.h"

#include "Requests.h"
#include "NetMessageMenuAltered.h"
#include "NetMessageMenuChanged.h"
#include "MM_DBCore.h"
#include "ManagerMenus.h"

//---------------------------------------------------------------------------
class TReturnOption
{
   private:
   UnicodeString 			Name;
public:
   bool              IsPlus;
   long              Palm_ID;
};

typedef std::vector<TReturnOption> ReturnOptionList;
//---------------------------------------------------------------------------
class TReturnItemCompleteSub
{
	private :
	UnicodeString      MenuName;
	UnicodeString      Course;    // Course/Class
	UnicodeString      Item;
	UnicodeString Size;
	public :
	// Req Top Level Info.
   long Palm_ID;
   long Palm_Size_ID;
   Currency Price;
   Currency PriceAdjust; // Only used for Cancels / Crediting back an order.
   Currency Cost;
};
typedef std::vector<TReturnItemCompleteSub> ReturnSubOrdersList;
//---------------------------------------------------------------------------
class TReturnItemComplete
{
	private :
	UnicodeString      MenuName;
	UnicodeString      Course;    // Course/Class
	UnicodeString      Item;
	UnicodeString MasterContainer;
	UnicodeString Size;

	public :
	// Req Top Level Info.
	long Palm_ID;
	long OrderKey;
	long Palm_Size_ID;
	long ServingCourseID;
   long Palm_MasterContainer_ID;
   long ViewableLocations;   
   unsigned short SetMenuGroup;
	int  OrderType; // Normal , Credit Etc.. (Status)
	eTabType TabType;
	bool SetMenuItem;
	bool SetMenuMaster;
	unsigned short SetMenuMask;
	bool HappyHour;
	TMenuType       MenuType;
	float       Price;
	float       Cost;    // Only used if there is no recipe.
	float       PriceAdjust;
	float       Discount;   
   UnicodeString Note;
	//---------TCPIP------------
	ReturnOptionList OptionsSelected;
	ReturnSubOrdersList SubOrders;

//	void Load(TIBTable *OrderTable,TIBTable *OptionsTable,TIBTable *SubOrdersTable);
};
//---------------------------------------------------------------------------
class TManagerMenusPalm : public TManagerMenus
{
   public:
   // To Tidy this up we need a array of "slots" of "MenuTItem" classes where each MenuTItem has a
   // Current and a New TItem list.
   TListMenu   *MenuFood;
   TListMenu   *MenuDrinks;
//   TListMenuContainer  *Current; // Current Menus
//   TListMenuContainer  *New; // New Menus.
	void UpdateMenuAltered(Database::TDBTransaction &DBTransaction,TNetMessageMenuAltered *Request);
	void UpdateMenuChanged(Database::TDBControl &DBControl,TNetMessageMenuChanged *Request);
   TListMenu *IsACurrentMenu(int inMenu_Key);
   TListMenu *IsACurrentMenu(UnicodeString inMenu_Name);
   void SelectMenuOfType(TMenuType MenuType);
   TItem * FetchItemByID(long ItemID);
	bool FetchServingCourseByID(USHORT ServingCourseID, TServingCourse &ServingCourse);
	void ToggleMenus(Database::TDBTransaction &DBTransaction);
	void SwapInNewMenus();
   void MenuInserted(Database::TDBTransaction &DBTransaction);
	void MenuRemoved(Database::TDBTransaction &DBTransaction);
	bool RequiresRemove(Database::TDBTransaction &DBTransaction,UnicodeString inMenu_Name);
   bool RequiresUpdate(Database::TDBTransaction &DBTransaction,UnicodeString inMenu_Name);
	void MenuAddedRemoved(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request);
	void MenuChanged(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request);
	void GetCurrentMenus(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged *CurrentMenus);
   TMemoryStream *MenuUploadFood;
   TMemoryStream *MenuUploadDrinks;   

	TManagerMenusPalm();
	virtual ~TManagerMenusPalm();

private:
//	TListMenu	*LoadMenu(const UnicodeString &MenuName, eMenuCommand Command);
	void			BuildPalmMenu(TListMenu *Menu);
};

#endif

#endif
