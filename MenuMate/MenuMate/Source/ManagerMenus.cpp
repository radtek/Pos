//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMenus.h"
#include "MMLogging.h"
#include "MMUtilFunc.h"
#include "MM_Menu.h"
#include "ItemSize.h"
#include "ListCourse.h"
#include "enumMenu.h"
#include "ItemRecipe.h"
#include "ItemSizeCategory.h"
#include "DeviceRealControl.h"
#include <stdexcept>
#include <cassert>

#ifdef MenuMate
#include "PhoenixHotelSystem.h"
#endif

#include <vector>

#include <i_item_definition.hh>
#include <i_item_definition_factory.hh>
#include <i_item_manager.hh>
#include <i_menu_manager.hh>
#include <i_size_definition.hh>
#include <i_size_definition_factory.hh>
#include <item_management_booter.hh>

using item_management::i_item_definition;
using item_management::i_item_definition_factory;
using item_management::i_item_manager;
using item_management::i_menu_manager;
using item_management::i_size_definition;
using item_management::i_size_definition_factory;

//---------------------------------------------------------------------------

#pragma package(smart_init)

std::pair<UnicodeString, bool>
TManagerMenus::MenuName(const int menu_key)
const
{
    std::pair<UnicodeString, bool> name_pair;
    Database::TDBTransaction tr(
      TDeviceRealControl::ActiveInstance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select menu_name "
                   "       from menu "
                   "       where menu_key = :key;";
    tr.StartTransaction();
        q->ParamByName("key")->AsInteger = menu_key;

        if ((name_pair.second = (q->ExecQuery(), q->RecordCount)))
            name_pair.first = q->FieldByName("menu_name")->AsString;
    tr.Commit();
    q->Close();

    return name_pair;
}

TItemSize *
TManagerMenus::FetchItemSizeByBarcode(
    const UnicodeString barcode)
{
        return Current->FetchItemSizeByBarcode(barcode);
}

std::auto_ptr<std::vector<TItem *> >
TManagerMenus::FetchItemsBySetMenuMask(
    const unsigned long set_menu_mask)
{
        return Current->FetchItemsBySetMenuMask(set_menu_mask);
}

std::auto_ptr<std::vector<TItem *> >
TManagerMenus::FetchItemsBySetMenuMaskByMenu(
    const unsigned long set_menu_mask,
    const UnicodeString menu_name)
{
        return Current->FetchItemsBySetMenuMaskByMenu(set_menu_mask,
                                                      menu_name);
}

void
TManagerMenus::GetMenuList(
  std::vector<std::pair<UnicodeString, int> > &destination,
  const TMenuType menu_type,
  const bool include_deleted,
  const bool only_palmable)
{
	Database::TDBTransaction tr(
	  TDeviceRealControl::ActiveInstance().DBControl);
	TIBSQL *qr = tr.Query(tr.AddQuery());

	qr->SQL->Text = "select menu_key, "
					"       menu_name "
					"       from menu "
					"       where menu_type = :menu_type ";
	if (!include_deleted)
		qr->SQL->Text = qr->SQL->Text
						+ "and deleted = 'F' ";
	if (only_palmable)
		qr->SQL->Text = qr->SQL->Text
						+ "and palmable = 'T' ";

	qr->SQL->Text = qr->SQL->Text
					+ "order by menu_name, "
					  "         menu_key ";
	tr.StartTransaction();
	qr->ParamByName("menu_type")->AsInteger = static_cast<int>(menu_type);

	for (qr->ExecQuery(); !qr->Eof; qr->Next())
		destination.push_back(
		  std::pair<UnicodeString, int>(
			qr->FieldByName("menu_name")->AsString,
			qr->FieldByName("menu_key")->AsInteger));

	tr.Commit();
}

void
TManagerMenus::GetMenuList(std::vector<UnicodeString> &destination)
const
{
   Database::TDBTransaction tr(
     TDeviceRealControl::ActiveInstance().DBControl);
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text = "select menu_name "
                   "       from menu "
                   "       where deleted = 'F';";

   for (tr.StartTransaction(), qr->ExecQuery(); !qr->Eof; qr->Next())
      destination.push_back(qr->FieldByName("menu_name")->AsString);
   tr.Commit();
}

void
TManagerMenus::GetMenuList(
  std::vector<std::pair<UnicodeString, int> > &destination)
{
   Database::TDBTransaction tr(
     TDeviceRealControl::ActiveInstance().DBControl);
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text = "select menu_key, "
                   "       menu_name "
                   "       from menu "
                   "       where deleted = 'F';";

   for (tr.StartTransaction(), qr->ExecQuery(); !qr->Eof; qr->Next())
      destination.push_back(
        std::pair<UnicodeString, int>(qr->FieldByName("menu_name")->AsString,
                                      qr->FieldByName("menu_key")->AsInteger));

   tr.Commit();
}

TItemSize *
TManagerMenus::FetchItemSizeByName(
  const UnicodeString &size_name,
  long inItemKey)
{
        TItem *item = FetchItemByKey(inItemKey);

        assert(size_name.Length() != 0);
        assert(item != NULL);

        return item->Sizes->SizeGet(size_name);
}

TItem *
TManagerMenus::FetchItemByID(long inItemID)
{
	TItem *item = NULL;

	for (int i = 0, j = Current->Count; !item && i < j; i++)
		item = Current->MenuGet(i)->FetchItemByID(inItemID);

	return item;
}

TItem *
TManagerMenus::FetchItemByKey(long inItemKey)
{
	TItem *item = NULL;

	for (int i = 0, j = Current->Count; !item && i < j; i++)
		item = Current->MenuGet(i)->FetchItemByKey(inItemKey);

	return item;
}

TManagerMenus::TManagerMenus() : Current(new TListMenuContainer), New(new TListMenuContainer)
{
}

TManagerMenus::Initialise(Database::TDBTransaction &DBTransaction)
{
   DefaultServingCourse = LoadDefaultServingCourse(DBTransaction);
}

// ---------------------------------------------------------------------------
int TManagerMenus::GetMenuServingCourse(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey)
{
   int MenuServingCourseKey = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT MENUSERVINGCOURSES_KEY " "FROM MENUSERVINGCOURSES " "WHERE " "MENU_KEY = :MENU_KEY AND "
		  "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 MenuServingCourseKey = IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return MenuServingCourseKey;
}

UnicodeString TManagerMenus::GetServingCourseKitchenName(Database::TDBTransaction &DBTransaction, int ServingCourseKey)
{

   UnicodeString ServingCourse = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "SELECT SERVINGCOURSE_KITCHEN_NAME " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 ServingCourse = UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString);
	  }

	  if (ServingCourse == "")
	  {
		 ServingCourse = GetServingCourseName(DBTransaction, ServingCourseKey);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourse;
}

TServingCourse TManagerMenus::GetServingCourseFromDB(Database::TDBTransaction &DBTransaction, int ServingCourseKey)
{

   TServingCourse ServingCourse;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "SELECT SERVINGCOURSES_KEY,SERVINGCOURSE_NAME,SERVINGCOURSE_KITCHEN_NAME, " "DELETED,SELECTABLE,DISPLAY_ORDER,COLOUR " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 ServingCourse.ServingCourseKey = IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger;

		 ServingCourse.Name = IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString;
		 ServingCourse.KitchenName = UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString);

		 /* ServingCourse.Name = IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString;
		 ServingCourse.KitchenName = IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString; */

		 if (ServingCourse.KitchenName == UnicodeString(""))
		 {
			ServingCourse.KitchenName = ServingCourse.Name;
		 }

		 ServingCourse.Deleted = IBInternalQuery->FieldByName("DELETED")->AsString.UpperCase() == "F" ? false : true;
		 ServingCourse.Selectable = IBInternalQuery->FieldByName("SELECTABLE")->AsString.UpperCase() == "F" ? false : true;
		 ServingCourse.SCOO = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
		 ServingCourse.Colour = (TColor)IBInternalQuery->FieldByName("COLOUR")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourse;
}

int TManagerMenus::GetServingCourseDisplayOrder(Database::TDBTransaction &DBTransaction, int ServingCourseKey)
{

   int DisplayOrder = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT DISPLAY_ORDER " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return DisplayOrder;
}

TColor TManagerMenus::GetServingCourseColour(Database::TDBTransaction &DBTransaction, int ServingCourseKey)
{

   TColor Colour = clMaroon;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT COLOUR " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 Colour = (TColor)IBInternalQuery->FieldByName("COLOUR")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Colour;
}

int TManagerMenus::SetServingCourse(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse,
   UnicodeString ServingCourseKitchenName, int SCOO, bool Deleted, bool Selectable, TColor ServingCourseColour)
{
   int ServingCourseKey = 0;
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  ServingCourseKey = GetServingCourseByName(DBTransaction, ServingCourse);

	  if (ServingCourseKey == 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 ServingCourseKey = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME," "SERVINGCOURSE_KITCHEN_NAME,"
			 "DELETED," "DISPLAY_ORDER," "SELECTABLE," "COLOUR) " "VALUES (" ":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME,"
			 ":SERVINGCOURSE_KITCHEN_NAME," ":DELETED," ":DISPLAY_ORDER," ":SELECTABLE," ":COLOUR);";
		 IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
		 IBInternalQuery->ParamByName("SERVINGCOURSE_NAME")->AsString = ServingCourse;
		 IBInternalQuery->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = ServingCourseKitchenName;
		 IBInternalQuery->ParamByName("DELETED")->AsString = Deleted ? "T" : "F";
		 IBInternalQuery->ParamByName("SELECTABLE")->AsString = Selectable ? "T" : "F";
		 IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = SCOO;
		 IBInternalQuery->ParamByName("COLOUR")->AsInteger = (int)ServingCourseColour;
		 IBInternalQuery->ExecQuery();
	  }
	  else
	  {
		 bool UpdateRequired = false;
		 TServingCourse CurrentServingCourse = GetServingCourseFromDB(DBTransaction, ServingCourseKey);

		 if (CurrentServingCourse.Deleted != Deleted)
			UpdateRequired = true;
		 if (CurrentServingCourse.Name != ServingCourse)
			UpdateRequired = true;
		 if (CurrentServingCourse.KitchenName != UTF8ToUnicodeString((AnsiString)ServingCourseKitchenName))
			UpdateRequired = true;
		 if (CurrentServingCourse.Selectable != Selectable)
			UpdateRequired = true;
		 if (CurrentServingCourse.SCOO != SCOO)
			UpdateRequired = true;
		 if (CurrentServingCourse.Colour != ServingCourseColour)
			UpdateRequired = true;

		 if (UpdateRequired)
		 {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "UPDATE " "SERVINGCOURSES " "SET " "SERVINGCOURSE_NAME = :SERVINGCOURSE_NAME, "
				"SERVINGCOURSE_KITCHEN_NAME = :SERVINGCOURSE_KITCHEN_NAME, " "DELETED = :DELETED, " "SELECTABLE = :SELECTABLE, "
				"DISPLAY_ORDER = :DISPLAY_ORDER, " "COLOUR = :COLOUR " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
			IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
			IBInternalQuery->ParamByName("SERVINGCOURSE_NAME")->AsString = ServingCourse;
			IBInternalQuery->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = ServingCourseKitchenName;
			IBInternalQuery->ParamByName("DELETED")->AsString = Deleted ? "T" : "F";
			IBInternalQuery->ParamByName("SELECTABLE")->AsString = Selectable ? "T" : "F";
			IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = SCOO;
			IBInternalQuery->ParamByName("COLOUR")->AsInteger = (int)ServingCourseColour;
			IBInternalQuery->ExecQuery();
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourseKey;
}

int TManagerMenus::SetServingCourseToMenu(Database::TDBTransaction &DBTransaction, int MenuKey, int ServingCourseKey)
{
   if (MenuKey == 0)
	  return 0;
   if (ServingCourseKey == 0)
	  return 0;
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  if (GetMenuServingCourse(DBTransaction, MenuKey, ServingCourseKey) == 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MENUSERVINGCOURSES, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 int MenuServingCourseKey = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO MENUSERVINGCOURSES (" "MENUSERVINGCOURSES_KEY," "MENU_KEY," "SERVINGCOURSES_KEY) "
			 "VALUES (" ":MENUSERVINGCOURSES_KEY," ":MENU_KEY," ":SERVINGCOURSES_KEY);";
		 IBInternalQuery->ParamByName("MENUSERVINGCOURSES_KEY")->AsInteger = MenuServingCourseKey;
		 IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
		 IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourseKey;
}

TServingCourse TManagerMenus::LoadDefaultServingCourse(Database::TDBTransaction &DBTransaction)
{
   TServingCourse ServingCourse;
   // Find the Emergency Backup Serving Course. It should be the 'No serving course' course.
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "Select * From SERVINGCOURSES Order By SERVINGCOURSES_KEY";
   IBInternalQuery->Close();
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->Eof)
   { // There is no serving course so add one and use that.

	  TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBQuery->Close();
	  IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
	  IBQuery->ExecQuery();
	  ServingCourse.ServingCourseKey = IBQuery->Fields[0]->AsInteger;

	  ServingCourse.Name = "No Serving Course";
	  ServingCourse.KitchenName = "No Serving Course";
	  ServingCourse.Colour = (int)clOlive;
	  ServingCourse.Deleted = false;
	  ServingCourse.Selectable = false;

	  IBQuery->Close();
	  IBQuery->ParamCheck = true;
	  IBQuery->SQL->Clear();
	  IBQuery->SQL->Text =
		  "INSERT INTO SERVINGCOURSES (" "SERVINGCOURSES_KEY," "SERVINGCOURSE_NAME, "
		  "SERVINGCOURSE_KITCHEN_NAME, " "DELETED, " "SELECTABLE, " "DISPLAY_ORDER, " "COLOUR) " "VALUES ("
		  ":SERVINGCOURSES_KEY," ":SERVINGCOURSE_NAME, " ":SERVINGCOURSE_KITCHEN_NAME, " ":DELETED, " ":SELECTABLE, " ":DISPLAY_ORDER, "
		  ":COLOUR);";
	  IBQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourse.ServingCourseKey;
	  IBQuery->ParamByName("SERVINGCOURSE_NAME")->AsString = ServingCourse.Name;
	  IBQuery->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = ServingCourse.KitchenName;
	  IBQuery->ParamByName("DELETED")->AsString = ServingCourse.Deleted ? "T" : "F";
	  IBQuery->ParamByName("SELECTABLE")->AsString = ServingCourse.Selectable ? "T" : "F";
	  IBQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 0;
	  IBQuery->ParamByName("COLOUR")->AsInteger = ServingCourse.Colour;
	  IBQuery->ExecQuery();
   }
   else
   {
	  ServingCourse.ServingCourseKey = IBInternalQuery->FieldByName("ServingCourses_Key")->AsInteger;
	  ServingCourse.Name = IBInternalQuery->FieldByName("ServingCourse_Name")->AsString;
	  ServingCourse.KitchenName = IBInternalQuery->FieldByName("ServingCourse_Kitchen_Name")->AsString;
	  ServingCourse.Colour = (TColor)IBInternalQuery->FieldByName("Colour")->AsInteger;
	  ServingCourse.Deleted = false;
	  ServingCourse.Selectable = false;
   }
   IBInternalQuery->Close();
   return ServingCourse;
}

/* void TManagerMenus::LoadServingCourse(Database::TDBTransaction &DBTransaction, TListMenu *Menu)
{
try
{


TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
IBInternalQuery->Close();
IBInternalQuery->SQL->Text = "SELECT SERVINGCOURSES_KEY FROM MENUSERVINGCOURSES WHERE MENU_KEY = :MENU_KEY";
IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = Menu->MenuKey;
IBInternalQuery->ExecQuery();
for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
{
TServingCourse ServingCourse = GetServingCourseFromDB(DBTransaction,
IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger);
Menu->ServingCourses.push_back(ServingCourse);
}
}
catch(Exception & E)
{
TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
throw;
}
} */

int TManagerMenus::GetServingCourseByName(Database::TDBTransaction &DBTransaction, UnicodeString ServingCourse)
{
   int ServingCourseKey = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT SERVINGCOURSES_KEY " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSE_NAME = :SERVINGCOURSE_NAME";
	  IBInternalQuery->ParamByName("SERVINGCOURSE_NAME")->AsString = ServingCourse;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 ServingCourseKey = IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourseKey;
}

UnicodeString TManagerMenus::GetServingCourseName(Database::TDBTransaction &DBTransaction, int ServingCourseKey)
{

   UnicodeString ServingCourse = "";

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT SERVINGCOURSE_NAME " "FROM SERVINGCOURSES " "WHERE " "SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 ServingCourse = IBInternalQuery->FieldByName("SERVINGCOURSE_NAME")->AsString;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ServingCourse;
}

void TManagerMenus::GetCourseList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT COURSE_KEY,COURSE_NAME " "FROM COURSE " "WHERE " "MENU_KEY = :MENU_KEY";
	  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 CourseList->AddObject(IBInternalQuery->FieldByName("COURSE_NAME")->AsString,
			(TObject*)(IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger));
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

void TManagerMenus::GetCourseKitchenNameList(Database::TDBTransaction &DBTransaction, int MenuKey, TStringList *CourseList)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =

		  "Select COURSE_KEY,cast(COURSE_KITCHEN_NAME as varchar(200)) COURSE_NAME From  COURSE Where"
		  " MENU_KEY = :MENU_KEY and COURSE_KITCHEN_NAME IS NOT NULL and COURSE_KITCHEN_NAME != '' ";

	  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 if (CourseList->IndexOf(UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("COURSE_NAME")->AsString)) == -1)
		 {
			CourseList->AddObject(UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("COURSE_NAME")->AsString),
			   (TObject*)(IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger));
		 }
	  }

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
          " Select COURSE_KEY,cast(COURSE_NAME as varchar(200)) COURSE_NAME From COURSE Where MENU_KEY = :MENU_KEY and "
		  " (COURSE_KITCHEN_NAME IS NULL OR COURSE_KITCHEN_NAME = '')";

	  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
	  IBInternalQuery->ExecQuery();


	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 if (CourseList->IndexOf(IBInternalQuery->FieldByName("COURSE_NAME")->AsString) == -1)
		 {
			CourseList->AddObject( IBInternalQuery->FieldByName("COURSE_NAME")->AsString.Unique(),(TObject*)(IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger));
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

void TManagerMenus::GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT MENU_KEY,MENU_NAME " "FROM MENU";
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 MenuList->AddObject(UnicodeString(IBInternalQuery->FieldByName("MENU_NAME")->AsString),
			(TObject*)(IBInternalQuery->FieldByName("MENU_KEY")->AsInteger));
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
void TManagerMenus::GetMenuList(Database::TDBTransaction &DBTransaction, TStringList *MenuList, TMenuType MenuType)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "MENU_KEY," "MENU_NAME " "FROM " "MENU " "Where " "Menu_Type = :Menu_Type " "Order By "
		  "Menu_Name";
	  IBInternalQuery->ParamByName("Menu_Type")->AsInteger = MenuType;
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 MenuList->AddObject(IBInternalQuery->FieldByName("MENU_NAME")->AsString,
			(TObject*)(IBInternalQuery->FieldByName("MENU_KEY")->AsInteger));
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
bool TManagerMenus::IsAValidMenu(Database::TDBTransaction &DBTransaction, int MenuKey)
{
   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();

	  IBInternalQuery->SQL->Text = " SELECT MENU_KEY " " FROM MENU " " WHERE " " MENU_KEY = :MENU_KEY";
	  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = MenuKey;
	  IBInternalQuery->ExecQuery();
	  RetVal = (IBInternalQuery->RecordCount != 0);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TManagerMenus::IsAPendingMenu(int inMenu_Key)
{
   for (int i = 0; i < New->Count; i++)
   {
	  TListMenu *Menu = New->MenuGet(i);
	  if (Menu->MenuKey == inMenu_Key)
	  {
		 return true;
	  }
   }
   return false;
}

bool TManagerMenus::IsAPendingMenu(UnicodeString inMenu_Name)
{
   for (int i = 0; i < New->Count; i++)
   {
	  TListMenu *Menu = New->MenuGet(i);
	  if (Menu->MenuName == inMenu_Name)
	  {
		 return true;
	  }
   }
   return false;
}

bool TManagerMenus::GetMenusExist(Database::TDBTransaction &DBTransaction)
{
   bool Retval = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT MENU_KEY,MENU_NAME " "FROM MENU";
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount > 0)
	  {
		 Retval = true;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Retval;
}

// ---------------------------------------------------------------------------

TListMenu *TManagerMenus::LoadMenuFromDB(Database::TDBControl &DBControl, const UnicodeString &MenuName, eMenuCommand Command)
{
   Menu::TMenuLoadDB MenuEnumerator(DBControl);

   i_item_manager &im_item_manager =
      item_management::kickstarter::get_item_manager();

   const i_item_definition_factory &im_item_definition_factory =
      im_item_manager.get_item_definition_factory();

   const i_size_definition_factory &im_size_definition_factory =
      im_item_manager.get_size_definition_factory();

   i_menu_manager &im_menu_manager =
      item_management::kickstarter::get_menu_manager();

   Menu::TMenusInfo MenusInfo;
   Menu::TMenusInfo::iterator iMenus;
   MenuEnumerator.EnumMenus(MenusInfo);
   // Find the menu key based on name.
#ifdef MenuMate
   TDeviceRealTerminal::Instance().BasePMS->ClearCodesTestedOk();
#endif

   for (iMenus = MenusInfo.begin(); iMenus != MenusInfo.end(); iMenus++)
   {
	  if (iMenus->Menu_Name == MenuName)
	  {
		 Menu::TMenuInfo MenuInfo;
		 Menu::TSizesInfo SizesInfo;
		 Menu::TCategoriesInfo CategoriesInfo;
		 Menu::TLocationsInfo LocationsInfo;
		 Menu::TServingCoursesInfo ServingCoursesInfo;

		 Menu::TMenuLoadDB MenuLoader(DBControl, iMenus->Key);
		 if (MenuLoader.GetMenu(&MenuInfo, &SizesInfo, &CategoriesInfo, &LocationsInfo, &ServingCoursesInfo))
		 {
			TListMenu *Menu = new TListMenu;

			Menu->MenuName = MenuInfo.Menu_Name;
			Menu->MenuKey = MenuInfo.Key;
			Menu->Menu_Type = static_cast <TMenuType> (MenuInfo.Menu_Type);
			Menu->Description = MenuInfo.Menu_Name;
			Menu->SwapInCommand = Command;

            if (im_menu_manager.is_menu_loaded(MenuInfo.Key))
                im_menu_manager.remove_menu(MenuInfo.Key);

            im_menu_manager.add_menu(MenuInfo.Key);

			int PalmsID = 1;
			// Import New Printing Order.

			TManagerLogs::Instance().Add(__FUNC__, "Menu Change", "Menu will change to " + (Menu->MenuName));

			for (unsigned i = 0; i < SizesInfo.Sizes.size(); i++)
			{
			   TItemSize *Size = new TItemSize;
			   Size->Name = SizesInfo.Sizes[i].Size_Name;
			   Size->Size_ID = SizesInfo.Sizes[i].Size_ID;
			   Size->Palm_ID = SizesInfo.Sizes[i].PalmID;
			   Size->Weighted = SizesInfo.Sizes[i].Weighted_Size;
			   Menu->Sizes->SizeAdd(Size);
			}

			for (unsigned i = 0; i < ServingCoursesInfo.ServingCourses.size(); i++)
			{
			   if (ServingCoursesInfo.ServingCourses[i].Enabled && !ServingCoursesInfo.ServingCourses[i].Deleted)
			   {
				  TServingCourse ServingCourse;
				  ServingCourse.ServingCourseKey = ServingCoursesInfo.ServingCourses[i].Key;
				  ServingCourse.Name = ServingCoursesInfo.ServingCourses[i].ServingCourse_Name;
				  ServingCourse.KitchenName = ServingCoursesInfo.ServingCourses[i].ServingCourse_Kitchen_Name;
				  if (ServingCourse.KitchenName == UnicodeString(""))
				  {
					 ServingCourse.KitchenName = ServingCourse.Name;
				  }

				  ServingCourse.Colour = ServingCoursesInfo.ServingCourses[i].Colour;
				  ServingCourse.Selectable = ServingCoursesInfo.ServingCourses[i].Selectable;
				  ServingCourse.ServingCourseID = ServingCourse.ServingCourseKey & 0xFFFF;
				  ServingCourse.SCOO = i;
				  Menu->ServingCourses.push_back(ServingCourse);
			   }
			}
			Menu::TCourseInfo CourseInfo;
			while (MenuLoader.GetNextCourse(&CourseInfo))
			{
			   TListCourse *Course = new TListCourse;
			   Course->Course_Name = CourseInfo.Course_Name;
			   Course->CourseKitchenName = CourseInfo.Course_Kitchen_Name;
			   if (Course->CourseKitchenName == UnicodeString(""))
			   {
				  Course->CourseKitchenName = Course->Course_Name;
			   }
			   Course->Course_Key = CourseInfo.Key;
			   Course->ViewableLocations = CourseInfo.View_Location;
			   Course->DefaultServingCourseKey = CourseInfo.ServingCourses_Key;
			   Course->No_Default_Serving_Course = CourseInfo.No_Default_Serving_Course;
			   PalmsID++;

				int ItemOptionGroup = 0; // Added for POS

				Menu::TItemInfo ItemInfo;
                std::vector<i_item_definition *> im_item_definitions;
               bool isItemPresent = false;
			   while (MenuLoader.GetNextItem(&ItemInfo))
			   {
				  TItem *Item = new TItem(Course);
				  Item->ItemKey = ItemInfo.Key;
				  Item->Item_ID = ItemInfo.Item_ID;
				  Item->Palm_ID = (PalmsID++) | (Menu->Menu_Type == Menu::mtBeverageMenu ? 0x4000 : 0); // ItemInfo.Item_ID;
				  Item->Item = ItemInfo.Item_Name;
				  Item->ItemKitchenName = ItemInfo.Item_Kitchen_Name;
				  if (Item->ItemKitchenName == UnicodeString(""))
				  {
					 Item->ItemKitchenName = Item->Item;
				  }
				  Item->SetColour = ItemInfo.Button_Colour;
				  Item->PrintChitNumber = ItemInfo.Print_Chit;
				  Item->DisplaySizes = ItemInfo.Display_Sizes;
				  Item->Enabled = ItemInfo.Enabled;
				  Item->ExclusivelyAsSide = ItemInfo.Exclusively_As_Side;
				  // Menu_Key Only used for printing.
				  Item->MenuKey = Menu->MenuKey;
				  Item->Course_Key = Course->Course_Key;
				  Item->Course = Course->Course_Name;
				  Item->PrintingGroupOrder = CourseInfo.Course_ID;
				  Item->CourseKitchenName = Course->CourseKitchenName;
				  Item->ViewableLocations = Course->ViewableLocations;
				  Item->MenuName = Menu->MenuName;
				  Item->ItemType = static_cast <TItemType> (Menu->Menu_Type);
				  Item->Note = "";
                  Item->ItemIdentifierKey = ItemInfo.ItemIdentifierKey;
				  // Added for POS
				  Item->OptionGroup = ItemOptionGroup++;
				  Item->ItemAppearanceOrder = ItemInfo.IAO;
				  Item->FontInfo.Underlined = ItemInfo.Print_Underlined;
				  Item->FontInfo.Bold = ItemInfo.Print_Bold;
				  Item->FontInfo.Width = ItemInfo.Print_Double_Width ? fsDoubleSize : fsNormalSize;
				  Item->FontInfo.Height = ItemInfo.Print_Double_Height ? fsDoubleSize : fsNormalSize;
				  Item->FontInfo.Colour = (ItemInfo.Print_Colour == 0) ? fcBlack : fcRed;
				  Item->FontInfo.Font = (ItemInfo.Print_Font == 0) ? ftFontA : ftFontB;
				  // end added for POS

                  std::vector<i_size_definition *> im_size_definitions;

				  Menu::TItemSizeInfo ItemSizeInfo;
                  bool isSizePresent = false;
				  while (MenuLoader.GetNextItemSize(&ItemSizeInfo))
				  {
                      im_size_definitions.push_back(
                      im_size_definition_factory.create(
                      ItemSizeInfo.available_quantity,
                      ItemSizeInfo.default_quantity,
                      ItemSizeInfo.enabled,
                      ItemSizeInfo.Key,
                      ItemSizeInfo.Size_Name,
                      ItemSizeInfo.Price,
                      ItemSizeInfo.MaxRetailPrice,
                      ItemSizeInfo.Special_Price,
                      ItemSizeInfo.warning_quantity));


                      if(!ItemSizeInfo.IsInvisible)
                      {
                             isSizePresent = true;
                             TItemSize *Size = new TItemSize(ItemInfo.Key);
                                                 Size->ItemSizeKey = ItemSizeInfo.Key;
                             Size->Size_ID = ItemSizeInfo.Size_ID;
                             Size->Name = ItemSizeInfo.Size_Name;
                             Size->SizeKitchenName = ItemSizeInfo.Size_Kitchen_Name;
                             if (Size->SizeKitchenName == UnicodeString(""))
                             {
                                Size->SizeKitchenName = Size->Name;
                             }

                             if(Menu->Sizes->SizeGet(Size->Name) != NULL)
                             {
                                 Size->Palm_ID = Menu->Sizes->SizeGet(Size->Name)->Palm_ID;
                             }


                             if(Menu->Sizes->SizeGet(Size->Name) != NULL)
                             {
                                Size->Weighted = Menu->Sizes->SizeGet(Size->Name)->Weighted;
                             }
                             Size->Price = ItemSizeInfo.Price;
                             Size->MaxRetailPrice = ItemSizeInfo.MaxRetailPrice;
                             Size->Cost = ItemSizeInfo.Cost;
                             Size->PLU = ItemSizeInfo.PLU;
                             Size->CostGSTPercent = ItemSizeInfo.Cost_GST_Percent;
                             Size->GSTPercent = ItemSizeInfo.GST_Percent;
                             Size->TaxProfiles = ItemSizeInfo.TaxProfiles;
                             Size->Barcode = ItemSizeInfo.Barcode; // Added for POS
                             Size->HappyPrice = ItemSizeInfo.Special_Price;
                             Size->Available_As_Standard = ItemSizeInfo.Available_As_Standard;
                             Size->SetMenuMask = ItemSizeInfo.Setmenu_Mask;
                             Size->PointsPercent = ItemSizeInfo.Points_Percent;
                             Size->ThirdPartyKey = ItemSizeInfo.ThirdPartyCodes_Key;
                             Size->ThirdPartyCode = ItemSizeInfo.ThirdPartyCode;

                             Size->MemberFreeSaleCount = ItemSizeInfo.Mem_Sale_Count;
                             Size->MemberFreeSaleDiscount = ItemSizeInfo.Mem_Discount_Percent;
                             Size->LocationFreeSaleCount = ItemSizeInfo.Loc_Sale_Count;
                             Size->LocationFreeSaleDiscount = ItemSizeInfo.Loc_Discount_Percent;
                             Size->TareWeight = ItemSizeInfo.Tare_Weight;
                             Size->PLU = ItemSizeInfo.PLU;
                             Size->CanBePaidForUsingPoints = ItemSizeInfo.CanBePaidForUsingPoints;
                             Size->DefaultPatronCount(ItemSizeInfo.DefaultPatronCount);
                             Size->Categories->FinancialCategoryKey = ItemSizeInfo.CategoryKey;
                             Size->Categories->FinancialCategory = ItemSizeInfo.Category;
                             Size->Categories->FinancialCategoryGLCode = ItemSizeInfo.GLCode;
                             Size->Categories->FinancialCategoryGroup = ItemSizeInfo.Category_Group_Name;

                             Size->PriceLevels = ItemSizeInfo.PriceLevels;
                             Size->RevenueCode = ItemSizeInfo.RevenueCode;
                             Size->ItemSizeIdentifierKey = ItemSizeInfo.ItemSizeIdentifierKey;



                             if (Size->Categories->FinancialCategory == "")
                             {
                                throw Exception("Unable to Load, A Blank Financial Category is not allowed.");
                             }
                             Size->SetMenuItem = false;
                             Size->SetMenuMaster = false;
                             if (TST_PROMO_MASTER(Size->SetMenuMask)) // Is a master.
                             {
                                Size->SetMenuMaster = true;
                             }
                             else if (Size->SetMenuMask != 0)
                             {
                                Size->SetMenuItem = true;
                             }
        #ifdef MenuMate
                             // Check the Size Thridparty key with the Phoenix system.
                               Database::TDBTransaction DBTransaction(DBControl);
                               DBTransaction.StartTransaction();
                               UnicodeString Code = Size->ThirdPartyCode;
                               DBTransaction.Commit();
                               if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Phoenix)
                               {
                                   if (!TDeviceRealTerminal::Instance().BasePMS->TestCode(Code))
                                   {
                                      throw Exception("Unable to Load Menu " + Menu->MenuName + ". The Third Party Code : " + Code +
                                        " is not found in the PMS System");
                                   } // end Added for POS
                               }
        #endif
                             for (unsigned i = 0; i < ItemSizeInfo.Recipes.size(); i++)
                             {
                                TItemRecipe *RecipeItem = new TItemRecipe;

                                RecipeItem->StockCode = ItemSizeInfo.Recipes[i].Stock_Code;
                                RecipeItem->StockLocation = ItemSizeInfo.Recipes[i].Stock_Location;
                                RecipeItem->Qty = ItemSizeInfo.Recipes[i].Qty;
                                RecipeItem->Cost = ItemSizeInfo.Recipes[i].Stock_Unit_Cost * ItemSizeInfo.Recipes[i].Qty;
                                RecipeItem->CostGSTPercent = ItemSizeInfo.Recipes[i].Stock_GST_Percent;

                                Size->Recipes->RecipeAdd(RecipeItem);
                             }
                             for (unsigned i = 0; i < ItemSizeInfo.Categories.size(); i++)
                             {
                                TItemSizeCategory *SizeCat = new TItemSizeCategory;

                                SizeCat->CategoryKey = ItemSizeInfo.Categories[i].Key;
                                SizeCat->Category = ItemSizeInfo.Categories[i].Category;

                                Size->Categories->CategoryAdd(SizeCat);
                             }
                             Size->CostForPoints = ItemSizeInfo.PriceForPoints; // add cost for points.
                             Item->Sizes->SizeAdd(Size);
                        }
				  }
				  // COuld be faster ways of doing this! (Refer to Office)
				  for (unsigned i = 0; i < ItemInfo.Sides.size(); i++)
				  {
					 Item->Sides->SideAdd(Item, ItemInfo.Sides[i].Master_Item_Key, ItemInfo.Sides[i].Item_Key, i,
						ItemInfo.Sides[i].Group_Number, ItemInfo.Sides[i].Max_Select, ItemInfo.Sides[i].Allow_Skip);
				  }
				  int PalmOptionID = 1;
				  for (unsigned i = 0; i < CourseInfo.Options.size(); i++)
				  {
					 TItemOption *ItemOption = new TItemOption;

					 ItemOption->OptionKey = CourseInfo.Options[i].Key;
					 ItemOption->Palm_ID = PalmOptionID++;
					 ItemOption->OptionID = CourseInfo.Options[i].Option_ID;
					 ItemOption->Name = CourseInfo.Options[i].Option_Name;
					 ItemOption->KitchenName = CourseInfo.Options[i].Option_Kitchen_Name;
					 if (ItemOption->KitchenName == UnicodeString(""))
					 {
						ItemOption->KitchenName = ItemOption->Name;
					 }
					 ItemOption->Enabled = true; // CourseInfo.Options[i].Enabled, Not supported at the mo.;
					 ItemOption->IsPlus = true;
					 ItemOption->ForcedMask = CourseInfo.Options[i].Forced_Mask;
					 ItemOption->Flags = CourseInfo.Options[i].Flags;
					 ItemOption->GroupNumber = CourseInfo.Options[i].GroupNumber;
					 ItemOption->PlusOption = CourseInfo.Options[i].PlusOption;
					 ItemOption->MinusOption = CourseInfo.Options[i].MinusOption;
					 ItemOption->AllowSkip = CourseInfo.Options[i].Allow_Skip; // Added for POS
					 ItemOption->DisallowMuliSelect = CourseInfo.Options[i].Max_Select; // Added for POS

					 ItemOption->FontInfo.Underlined = CourseInfo.Options[i].Print_Underlined;
					 ItemOption->FontInfo.Bold = CourseInfo.Options[i].Print_Bold;
					 ItemOption->FontInfo.Width = CourseInfo.Options[i].Print_Double_Width ? fsDoubleSize : fsNormalSize;
					 ItemOption->FontInfo.Height = CourseInfo.Options[i].Print_Double_Height ? fsDoubleSize : fsNormalSize;
					 ItemOption->FontInfo.Colour = (CourseInfo.Options[i].Print_Colour == 0) ? fcBlack : fcRed;
					 ItemOption->FontInfo.Font = (CourseInfo.Options[i].Print_Font == 0) ? ftFontA : ftFontB;
                     ItemOption->OptionOrder = CourseInfo.Options[i].Option_order;

					 ItemOption->Owner = Item->Options;
					 Item->Options->OptionAdd(ItemOption);
				  }

                  im_item_definitions.push_back( im_item_definition_factory.create(ItemInfo.Item_Name, im_size_definitions, ItemInfo.Key));
                  if(isSizePresent)
                  {
				    Course->ItemAdd(Item);
                    isItemPresent = true;
                  }
			   }

              im_menu_manager.add_course(CourseInfo.Course_ID, MenuInfo.Key);
              im_menu_manager.add_item_definitions(MenuInfo.Key, CourseInfo.Course_ID, im_item_definitions);
              im_item_definitions.clear();
              if(isItemPresent)
			   Menu->CourseAdd(Course);
			}
			New->MenuAdd(Menu);
			return Menu;
		 }
	  }
   }
   return NULL;
}
bool
TManagerMenus::MenuInCurrent(const UnicodeString &name)
{
    return Current->MenuGet(name) != NULL;
}

std::vector<UnicodeString>
TManagerMenus::GetCurrentMenus()
{
    TListMenuContainer &list = *Current;
    std::vector<UnicodeString> menus;

    for (int i = 0; i < list.Count; i++)
        menus.push_back(list.MenuGet(i)->MenuName);

    return menus;
}

void TManagerMenus::SetMenuList(Database::TDBTransaction &DBTransaction, int DeviceKey)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  if (DeviceKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " DELETE " " FROM " " DEVICESMENUS " " WHERE " " DEVICE_KEY = :DEVICE_KEY ";
		 IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
		 IBInternalQuery->ExecQuery();

		 for (int i = 0; i < Current->Count; i++)
		 {
			TListMenu *Menu = Current->MenuGet(i);
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Add("INSERT INTO DEVICESMENUS (DEVICE_KEY , MENU_NAME ) ");
			IBInternalQuery->SQL->Add("VALUES ( :DEVICE_KEY, :THIS_MENU );");
			IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
			IBInternalQuery->ParamByName("THIS_MENU")->AsString = Menu->MenuName;
			IBInternalQuery->ExecQuery();
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}
