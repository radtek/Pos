//---------------------------------------------------------------------------

#ifndef PalmMenusH
#define PalmMenusH

#ifdef PalmMate
#include "enum.h"

#include "Requests.h"
#include "NetMessageMenuAltered.h"
#include "NetMessageMenuChanged.h"

//---------------------------------------------------------------------------
class TReturnOption
{
   private:
   AnsiString 			Name;
public:
   bool              IsPlus;
   long              Palm_ID;
};

typedef std::vector<TReturnOption> ReturnOptionList;
//---------------------------------------------------------------------------
class TReturnItemCompleteSub
{
	private :
	AnsiString      MenuName;
	AnsiString      Course;    // Course/Class
	AnsiString      Item;
	AnsiString Size;
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
	AnsiString      MenuName;
	AnsiString      Course;    // Course/Class
	AnsiString      Item;
	AnsiString MasterContainer;
	AnsiString Size;

	public :
	// Req Top Level Info.
   long Palm_ID;
   long Palm_Size_ID;
   long Palm_MasterContainer_ID;
   long ViewableLocations;   
   unsigned char SetMenuGroup;
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
   AnsiString Note;
	//---------TCPIP------------
	ReturnOptionList OptionsSelected;
	ReturnSubOrdersList SubOrders;

//	void Load(TIBTable *OrderTable,TIBTable *OptionsTable,TIBTable *SubOrdersTable);
};

//---------------------------------------------------------------------------

class TPalmMenus
{
   public:
   // To Tidy this up we need a array of "slots" of "MenuTItem" classes where each MenuTItem has a
   // Current and a New TItem list.
   TListMenu   *MenuFood;
   TListMenu   *MenuDrinks;
   TListMenuContainer  *Current; // Current Menus
   TListMenuContainer  *New; // New Menus.
	void UpdateMenuAltered(TNetMessageMenuAltered *Request);
	void UpdateMenuChanged(TNetMessageMenuChanged *Request);
   TListMenu *IsACurrentMenu(int inMenu_Key);
   TListMenu *IsACurrentMenu(AnsiString inMenu_Name);
   void SelectMenuOfType(TMenuType MenuType);
   TItem * FetchItemByID(long ItemID);
   void ToggleMenus();
   void SwapInNewMenus();
   void MenuInserted();
   void MenuRemoved();
   bool RequiresRemove(AnsiString inMenu_Name);
   bool RequiresUpdate(AnsiString inMenu_Name);
	void MenuAddedRemoved(TNetMessageMenuChanged * Request);
	void MenuChanged(TNetMessageMenuChanged * Request);
	void GetCurrentMenus(TNetMessageMenuChanged *CurrentMenus);
   TMemoryStream *MenuUploadFood;
   TMemoryStream *MenuUploadDrinks;   
   TPalmMenus();
   virtual ~TPalmMenus();
};

#endif

#endif
