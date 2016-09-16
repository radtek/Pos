//---------------------------------------------------------------------------

#ifndef DBMenuH
#define DBMenuH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include "ServingCourse.h"
#include "ListMenu.h"

class TDBMenu
{
public:
	TDBMenu();

	static TServingCourse LoadDefaultServingCourse(Database::TDBTransaction &DBTransaction);

	static int SetServingCourse(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse, UnicodeString ServingCourseKitchenName,
		int SCOO, bool Deleted, bool Selectable, TColor ServingCourseColour);

	static int GetServingCourseByName(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse);
	static int GetMenuServingCourse(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey);
	static int SetServingCourseToMenu(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey);

	static UnicodeString GetServingCourseKitchenName(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	static UnicodeString GetServingCourseName(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	static TServingCourse GetServingCourseFromDB(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	static void GetCourseKitchenNameList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList);
	static int GetServingCourseDisplayOrder(Database::TDBTransaction &DBTransaction, int ServingCourseKey);
	static TColor GetServingCourseColour(Database::TDBTransaction &DBTransaction, int ServingCourseKey);

	static void GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList);
	static void GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList, TMenuType MenuType);
	static bool GetMenusExist(Database::TDBTransaction &DBTransaction);
	static void GetCourseList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList);
	static bool IsAValidMenu(Database::TDBTransaction &DBTransaction, int MenuKey);
	static bool IsAPendingMenu(int inMenu_Key);
	static bool IsAPendingMenu(UnicodeString inMenu_Name);

	static TListMenu *LoadMenuFromDB(Database::TDBControl &DBControl, const UnicodeString &MenuName, eMenuCommand Command);

	static void SetMenuList(Database::TDBTransaction &DBTransaction, int DeviceKey);
    static int GetOrCreateCloudCategoryGroup(Database::TDBTransaction &DBTransaction);
    static int GetOrCreateCloudCategory(Database::TDBTransaction &DBTransaction,AnsiString CategoryName);
};

#endif
