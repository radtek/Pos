//---------------------------------------------------------------------------

#ifndef ManagerMenusH
#define ManagerMenusH
//---------------------------------------------------------------------------


#include "Manager.h"

#include "MM_DBCore.h"
#include "POS_XMLBase.h"
#include "ListMenu.h"

class TManagerMenus : public TManager
{
public:
	TManagerMenus();

	Initialise(Database::TDBTransaction &DBTransaction);
	//Backstop serving course to use if an item gets though the GUI selection with no serving course selected.
	TServingCourse DefaultServingCourse;
	TServingCourse LoadDefaultServingCourse(Database::TDBTransaction &DBTransaction);

//	void LoadServingCourses(Database::TDBTransaction &DBTransaction, TListMenu *Menu);

	int SetServingCourse(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse, UnicodeString ServingCourseKitchenName,
		int SCOO, bool Deleted, bool Selectable, TColor ServingCourseColour);

	int GetServingCourseByName(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse);
	int GetMenuServingCourse(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey);
	int SetServingCourseToMenu(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey);

	UnicodeString GetServingCourseKitchenName(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	UnicodeString GetServingCourseName(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	TServingCourse GetServingCourseFromDB(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	void GetCourseKitchenNameList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList);
	int GetServingCourseDisplayOrder(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	TColor GetServingCourseColour(Database::TDBTransaction &DBTransaction, int ServingCourseKey);

   void GetMenuList(std::vector<UnicodeString> &destination) const;
   void GetMenuList(std::vector<std::pair<UnicodeString, int> > &destination);
   void GetMenuList(std::vector<std::pair<UnicodeString, int> > &destination,
					const TMenuType menu_type,
					const bool include_deleted = true,
					const bool only_palmable = false);
    std::pair<UnicodeString, bool> MenuName(const int menu_key) const;

	void GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList);
	void GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList, TMenuType MenuType);
	bool GetMenusExist(Database::TDBTransaction &DBTransaction);
	void GetCourseList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList);
	bool IsAValidMenu(Database::TDBTransaction &DBTransaction, int MenuKey);
	bool IsAPendingMenu(int inMenu_Key);
	bool IsAPendingMenu(UnicodeString inMenu_Name);

	TListMenu *LoadMenuFromDB(Database::TDBControl &DBControl, const UnicodeString &MenuName, eMenuCommand Command);

	void SetMenuList(Database::TDBTransaction &DBTransaction, int DeviceKey);
	const std::auto_ptr <TListMenuContainer> Current; // Current Menus
	const std::auto_ptr <TListMenuContainer> New; // New Menus.

    std::vector<UnicodeString> GetCurrentMenus();
    bool MenuInCurrent(const UnicodeString &name);

	void BuildXMLMenu(Database::TDBControl &DBControl, TPOS_XMLBase &Data, int SiteID);
	void BuildXMLListCategories(Database::TDBTransaction &DBTransaction, TPOS_XMLBase &Data, int SiteID);
	void BuildXMLListGroup(Database::TDBTransaction &DBTransaction, TPOS_XMLBase &Data,int SiteID);

	TItem *FetchItemByKey(long inItemKey);
	TItem *FetchItemByID(long inItemID);

        TItemSize *FetchItemSizeByName(const UnicodeString &size_name,
                                       long inItemKey);

        TItemSize *FetchItemSizeByBarcode(const UnicodeString);

        std::auto_ptr<std::vector<TItem *> >
            FetchItemsBySetMenuMask(const unsigned long);

        std::auto_ptr<std::vector<TItem *> >
            FetchItemsBySetMenuMaskByMenu(const unsigned long, const UnicodeString);
};
#endif
