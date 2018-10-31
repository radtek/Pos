//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMenusPOS.h"
#include "DeviceRealTerminal.h"
#include "ListCourse.h"
#include "MMLogging.h"
#include "DBOrder.h"
#include "MMMessageBox.h"
#include "ForcedSide.h"
#include "PhoenixHotelSystem.h"
#include "DBThirdPartyCodes.h"
#include "DBGroups.h"

#include "ImportMenu.h"
#include "SelectDish.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ------------------------------TManagerMenusPOS-----------------------------
// ---------------------------------------------------------------------------


TManagerMenusPOS::TManagerMenusPOS()
{
   FVisibleMenu = NULL;
}

// ---------------------------------------------------------------------------
TManagerMenusPOS::~TManagerMenusPOS()
{
}

// ---------------------------------------------------------------------------
void TManagerMenusPOS::SelectMenuOfType(TMenuType MenuType)
{
   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
	  if (Menu->Menu_Type == MenuType)
	  {
		 VisibleMenu = Menu;
		 break;
	  }
   }
}

// ---------------------------------------------------------------------------

TListMenu *TManagerMenusPOS::IsACurrentMenu(int inMenu_Key)
{
   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
	  if (Menu->MenuKey == inMenu_Key)
	  {
		 return Menu;
	  }
   }
   return NULL;
}

TListMenu *TManagerMenusPOS::IsACurrentMenu(AnsiString inMenu_Name)
{
   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
	  if (Menu->MenuName == inMenu_Name)
	  {
		 return Menu;
	  }
   }
   return NULL;
}

void TManagerMenusPOS::GetCurrentMenus(Database::TDBTransaction &DBTransaction, TNetMessageMenuChanged *CurrentMenus)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();

	  IBInternalQuery->SQL->Text =
		  " SELECT DEVICESMENUS.MENU_NAME " " FROM DEVICESMENUS " " WHERE " " DEVICESMENUS.DEVICE_KEY = :THIS_DEVICE";
	  IBInternalQuery->ParamByName("THIS_DEVICE")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 CurrentMenus->Menu_Names[IBInternalQuery->FieldByName("MENU_NAME")->AsString] = eMenuAddReplace;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

bool TManagerMenusPOS::RequiresUpdate(Database::TDBTransaction &DBTransaction, AnsiString inMenu_Name)
{
   // Menu that is Current, but Key does not match DB wil require updateding.

   bool Retval = false;
   try
   {
	  TListMenu *Menu = IsACurrentMenu(inMenu_Name);
	  if (Menu != NULL)
	  {
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT MENU.MENU_KEY " " FROM MENU " " WHERE " "  MENU.MENU_NAME = :Loc_MENU_NAME";
		 IBInternalQuery->ParamByName("Loc_MENU_NAME")->AsString = inMenu_Name;
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount)
		 {
			if (Menu->MenuKey != IBInternalQuery->FieldByName("MENU_KEY")->AsInteger)
			{
			   Retval = true;
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Retval;
}

bool TManagerMenusPOS::RequiresRemove(Database::TDBTransaction &DBTransaction, AnsiString inMenu_Name)
{

   bool Retval = false;
   try
   {
	  TListMenu *Menu = IsACurrentMenu(inMenu_Name);
	  if (Menu != NULL)
	  {
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT MENU_NAME " " FROM MENU " " WHERE " "  MENU_NAME = :MENU_NAME";
		 IBInternalQuery->ParamByName("MENU_NAME")->AsString = Menu->MenuName;
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount == 0)
		 { // Ok Remove this menu if it is not pending insertion.
			// This menu will be pending if a commit has occured and it required
			// updating.
			if (!IsAPendingMenu(inMenu_Name))
			{
			   Retval = true;
			}
		 }

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

AnsiString TManagerMenusPOS::EscapeString(AnsiString Line, char Qualifier, char Escape)
{
   AnsiString Temp;
   for (int i = 1; i <= Line.Length(); i++)
   {
	  if (Line[i] == Qualifier || Line[i] == Escape)
	  {
		 Temp += Escape;
	  }
	  Temp += Line[i];
   }
   return Temp;
}

// ---------------------------------------------------------------------------
AnsiString TManagerMenusPOS::ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
   AnsiString Temp = "";

   int StartPos = Offset + 1; // Not Zero based indexing.
   bool Escaped = false;
   bool InsideQualifier = false;
   bool Completed = false;

   for (int i = StartPos; i <= Line.Length() && !Completed; i++)
   {
	  if (Escape != NULL && Line[i] == Escape)
	  {
		 if (Escaped)
		 {
			Temp += Escape;
			Escaped = false;
		 }
		 else
		 {
			Escaped = true;
		 }
	  }
	  else if (Qualifier != NULL && Line[i] == Qualifier) // Check for "
	  {
		 if (Escaped)
		 {
			Temp += Qualifier;
			Escaped = false;
		 }
		 else
		 {
			InsideQualifier = !InsideQualifier;
		 }
	  }
	  else if (Line[i] == Delimiter) // Check for ,
	  {
		 if (Escaped)
		 {
			Temp += Delimiter;
			Escaped = false;
		 }
		 else if (InsideQualifier)
		 {
			Temp += Delimiter;
		 }
		 else
		 {
			Completed = true;
		 }
	  }
	  else
	  {
		 if (Escaped)
		 {
			Temp += Escape; // Shouldn't be escape. Assume the Escape char was part of the word.
			Escaped = false;
		 }
		 Temp += Line[i];
	  }
	  Offset++;
   }
   return Temp.Trim();
}

// ---------------------------------------------------------------------------
bool TManagerMenusPOS::LoadMenu(TIBDatabase *IBDatabase, TStringList *Menu, bool Force)
{
   bool Success = false;
   if (Menu->Strings[0] == MENU_IMPORT_VERSION)
   {
	  AnsiString LastSuccessfulItem, LastSuccessfulSize, LastSuccessfulCourse, NewMenuName;

	  Database::TDBTransaction DBTransaction(IBDatabase);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
	  frmProcessing->Message = "Importing Menus";
	  frmProcessing->Show();
	  try
	  {
		 long MenuKeyToDelete = 0;
		 long NewMenuKey = 0;
		 long NewSizeKey = 0;
		 long NewCourseKey = 0;
		 long NewItemKey = 0;
		 long NewItemID = 0;
		 long NewItemSizeKey = 0;
		 long NewRecipeKey = 0;
		 long NewOptionKey = 0;

		 std::vector <TForcedSide> ForcedSidesList;
		 AnsiString NextWord;
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		 TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
		 TIBSQL *IBInternalQuery3 = DBTransaction.Query(DBTransaction.AddQuery());

		 int IAO = 0;
		 int IOO = 0;
		 int CAO = 0;
		 int OAO = 0;
		 int DCAO = 0;
		 int SCOO = 0;

		 TDeviceRealTerminal::Instance().BasePMS->ClearCodesTestedOk();

		 enum TReading
		 {
			RMenu, RSizes, RCourse, RItem, RSide, RItemSize, RRecipe, ROption, RCategories, RCategory, RThirdPartyCodes, RServingCourse
		 }Reading;

		 for (int i = 1; i < Menu->Count; i++)
		 {
			if (Menu->Strings[i] == "[Menu]")
			{
			   Reading = RMenu;
			}
			else if (Menu->Strings[i] == "[Sizes]")
			{
			   Reading = RSizes;
			}
			else if (Menu->Strings[i] == "[Categories]")
			{
			   Reading = RCategories;
			}
			else if (Menu->Strings[i] == "[Course]")
			{
			   Reading = RCourse;
			}
			else if (Menu->Strings[i] == "[Item]")
			{
			   Reading = RItem;
			}
			else if (Menu->Strings[i] == "[Side]")
			{
			   Reading = RSide;
			}
			else if (Menu->Strings[i] == "[Size]")
			{
			   Reading = RItemSize;
			}
			else if (Menu->Strings[i] == "[Category]")
			{
			   Reading = RCategory;
			}
			else if (Menu->Strings[i] == "[Recipe]")
			{
			   Reading = RRecipe;
			}
			else if (Menu->Strings[i] == "[Options]")
			{
			   Reading = ROption;
			}
			else if (Menu->Strings[i] == "[3rd Party Groups]")
			{
			   Reading = RThirdPartyCodes;
			}
			else if (Menu->Strings[i] == "[Serving Course]")
			{
			   Reading = RServingCourse;
			}
			else
			{
			   int Offset = 0;
			   switch(Reading)
			   {
			   case RMenu:
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  NewMenuName = NextWord;

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT * FROM MENU WHERE MENU_NAME = :MENU_NAME";
				  IBInternalQuery->ParamByName("MENU_NAME")->AsString = NewMenuName;
				  IBInternalQuery->ExecQuery();

				  if (IBInternalQuery->RecordCount)
				  {
					 IBInternalQuery2->Close();
					 IBInternalQuery2->SQL->Text = "DELETE FROM MENU WHERE MENU_NAME = :COPY_MENU_NAME";
					 IBInternalQuery2->ParamByName("COPY_MENU_NAME")->AsString = AnsiString("Cpy " + NewMenuName).SubString(1, 15);
					 IBInternalQuery2->ExecQuery();

					 IBInternalQuery2->Close();
					 IBInternalQuery2->SQL->Text = "UPDATE MENU SET MENU_NAME = :NEW_MENU_NAME WHERE MENU_NAME = :MENU_NAME";
					 IBInternalQuery2->ParamByName("NEW_MENU_NAME")->AsString = AnsiString("Cpy " + NewMenuName).SubString(1, 15);
					 IBInternalQuery2->ParamByName("MENU_NAME")->AsString = NewMenuName;
					 IBInternalQuery2->ExecQuery();

					 MenuKeyToDelete = IBInternalQuery->FieldByName("MENU_KEY")->AsInteger;
				  }

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MENU, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  NewMenuKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();

				  IBInternalQuery->SQL->Clear();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO MENU (" "MENU_KEY, " "MENU_NAME, " "MENU_TYPE, " "DESCRIPTION, " "PALMABLE) " "VALUES ("
					  ":MENU_KEY, " ":MENU_NAME, " ":MENU_TYPE, " ":DESCRIPTION, " ":PALMABLE);";
				  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = NewMenuKey;
				  IBInternalQuery->ParamByName("MENU_NAME")->AsString = AnsiString(NextWord).SubString(1, 15);
				  IBInternalQuery->ParamByName("DESCRIPTION")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("MENU_TYPE")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PALMABLE")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  IBInternalQuery->ExecQuery();
				  break;
			   case RSizes:
				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SIZE, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  NewSizeKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();

				  IBInternalQuery->SQL->Clear();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO SIZES (" "SIZES_KEY, " "MENU_KEY, " "SIZE_NAME, " "SIZE_KITCHEN_NAME, "
					  "WEIGHTED_SIZE, " "SIZE_ID, HANDHELD_NAME,RECEIPT_NAME) " "VALUES (" ":SIZES_KEY, " ":MENU_KEY, " ":SIZE_NAME, "
					  ":SIZE_KITCHEN_NAME, " ":WEIGHTED_SIZE, " ":SIZE_ID,:HANDHELD_NAME,:RECEIPT_NAME);";
				  IBInternalQuery->ParamByName("SIZES_KEY")->AsInteger = NewSizeKey;
				  IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = NewMenuKey;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("SIZE_NAME")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("HANDHELD_NAME")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("RECEIPT_NAME")->AsString = NextWord;

				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("WEIGHTED_SIZE")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";

				  IBInternalQuery->ParamByName("SIZE_ID")->AsInteger = GetNextUniqueMenuID(DBTransaction);
				  IBInternalQuery->ExecQuery();
				  break;
			   case RCategories:
				  {
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString Category = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString CategoryGroup = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 bool RemoveCategory = UpperCase(NextWord) == "TRUE" ? true : false;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 bool RemoveCategoryGroup = UpperCase(NextWord) == "TRUE" ? true : false;

					 if (Category == "")
					 {
						throw Exception("Unable to Import A Blank Category is not allowed.");
					 }
					 if (CategoryGroup == "")
					 {
						throw Exception("Unable to Import A Blank Category Group is not allowed.");
					 }
					 TDBOrder::SetCategoryGroup(DBTransaction, CategoryGroup);

					 if (RemoveCategoryGroup)
					 {
						// Remove all the Categories in this Group.
						int CatGroupKey = TDBOrder::GetCategoryGroup(DBTransaction, CategoryGroup);
						if (CatGroupKey != 0)
						{
						   TDBOrder::SetArchiveCategoryGroupEnabled(DBTransaction, CatGroupKey, !RemoveCategoryGroup);
						}
					 }
					 else if (!RemoveCategory)
					 {
						int CatGroupKey = TDBOrder::GetOrCreateCategoryGroup(DBTransaction, CategoryGroup);
						TDBOrder::SetArchiveCategoryGroupEnabled(DBTransaction, CatGroupKey, !RemoveCategoryGroup);
						int CatKey = TDBOrder::GetArchiveCategory(DBTransaction, Category);
						if (CatKey == 0)
						{
						   CatKey = TDBOrder::SetArchiveCategory(DBTransaction, Category);
						   TDBOrder::SetCategoryToGroup(DBTransaction, CatKey, CatGroupKey);
						}
						else
						{
						   if (TDBOrder::GetCategoriesCategoryGroup(DBTransaction, CatKey) != CatGroupKey)
						   {
							  TDBOrder::SetCategoryToGroup(DBTransaction, CatKey, CatGroupKey);
						   }
						}
					 }

					 int CatKey = TDBOrder::GetArchiveCategory(DBTransaction, Category);
					 if (CatKey != 0)
					 {
						TDBOrder::SetArchiveCategoryEnabled(DBTransaction, CatKey, !RemoveCategory);
					 }
				  }break;
			   case RServingCourse:
				  {
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString ServingCourse = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString KitchenServingCourse = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 bool VisibleOnThisMenu = NextWord == "T" ? true : false;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 bool Deleted = NextWord == "T" ? true : false;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 bool Selectable = NextWord == "T" ? true : false;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 TColor ServingCourseColour = (TColor)StrToIntDef(NextWord, (int)clMaroon);
					 SCOO = SCOO++;
					 int ServingCourseKey = SetServingCourse(DBTransaction, ServingCourse, KitchenServingCourse, SCOO, Deleted, Selectable,
						ServingCourseColour);
					 if (VisibleOnThisMenu)
					 {
						SetServingCourseToMenu(DBTransaction, NewMenuKey, ServingCourseKey);
					 }
					 break;
				  }
			   case RThirdPartyCodes:
				  {
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString Code = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString Description = NextWord;
					 TDBThirdPartyCodes::SetThirdPartyCode(DBTransaction, Code, Description, tpItemSize);
                     if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Phoenix)
                     {
                         if (!TDeviceRealTerminal::Instance().BasePMS->TestCode(Code))
                         {
                            throw Exception("Unable to Import Menu " + NewMenuName + ". The Third Party Code : " + Code +
                               " is not found in the PMS System");
                         }
                     }
				  }break;
			   case RCourse:

				  {
					 IBInternalQuery->Close();
					 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_COURSE, 1) FROM RDB$DATABASE";
					 IBInternalQuery->ExecQuery();
					 NewCourseKey = IBInternalQuery->Fields[0]->AsInteger;

					 IBInternalQuery->Close();

					 IBInternalQuery->SQL->Clear();
					 IBInternalQuery->SQL->Text =
						 "INSERT INTO COURSE (" "COURSE_KEY, " "MENU_KEY, " "COURSE_ID, " "COURSE_NAME, "
						 "COURSE_KITCHEN_NAME, " "VIEW_LOCATION, " "ENABLED, " "SERVINGCOURSES_KEY, "
						 "NO_DEFAULT_SERVING_COURSE, " "CAO,HANDHELD_NAME,RECEIPT_NAME) " "VALUES ( " ":COURSE_KEY, " ":MENU_KEY, " ":COURSE_ID, " ":COURSE_NAME, "
						 ":COURSE_KITCHEN_NAME, " ":VIEW_LOCATION, " ":ENABLED, " ":SERVINGCOURSES_KEY, " ":NO_DEFAULT_SERVING_COURSE, "
						 ":CAO,:HANDHELD_NAME,:RECEIPT_NAME);";
					 IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = NewCourseKey;
					 IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = NewMenuKey;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 if (NextWord == "0")
					 {
						IBInternalQuery->ParamByName("COURSE_ID")->AsInteger = GetNextUniqueMenuID(DBTransaction);
					 }
					 else
					 {
						IBInternalQuery->ParamByName("COURSE_ID")->AsString = NextWord;
					 }
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("COURSE_NAME")->AsString = NextWord;
					 LastSuccessfulCourse = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("COURSE_KITCHEN_NAME")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                     IBInternalQuery->ParamByName("HANDHELD_NAME")->AsString = NextWord;
                     NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                     IBInternalQuery->ParamByName("RECEIPT_NAME")->AsString = NextWord;
                     NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');

					 IBInternalQuery->ParamByName("VIEW_LOCATION")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("ENABLED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 AnsiString ServingCourse = NextWord;
					 int ServingCourseKey = GetServingCourseByName(DBTransaction, ServingCourse);

					 if (ServingCourseKey == 0)
					 {
						IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->Clear();
					 }
					 else
					 {
						IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
					 }

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("NO_DEFAULT_SERVING_COURSE")->AsString = NextWord;
					 IBInternalQuery->ParamByName("CAO")->AsInteger = CAO++;
					 IBInternalQuery->ExecQuery();
				  }break;
			   case RItem:
				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ITEM, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  NewItemKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();

				  IBInternalQuery->SQL->Clear();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO ITEM (" "ITEM_KEY, " "COURSE_KEY, " "ITEM_ID, " "ITEM_NAME, " "ITEM_CATEGORY, "
					  "DISPLAY_SIZES, " "ENABLED, " "IAO, " "BUTTON_COLOUR, " "PRINT_CHIT, " "EXCLUSIVELY_AS_SIDE, "
					  "ITEM_KITCHEN_NAME, " "PRINT_UNDERLINED, " "PRINT_BOLD, " "PRINT_COLOUR, " "PRINT_FONT, " "PRINT_DOUBLE_WIDTH, "
					  "PRINT_DOUBLE_HEIGHT,HANDHELD_NAME,RECEIPT_NAME) " "VALUES ( " ":ITEM_KEY, " ":COURSE_KEY, " ":ITEM_ID, " ":ITEM_NAME, " ":ITEM_CATEGORY, "
					  ":DISPLAY_SIZES, " ":ENABLED, " ":IAO, " ":BUTTON_COLOUR, " ":PRINT_CHIT, " ":EXCLUSIVELY_AS_SIDE, "
					  ":ITEM_KITCHEN_NAME, " ":PRINT_UNDERLINED, " ":PRINT_BOLD, " ":PRINT_COLOUR, " ":PRINT_FONT, "
					  ":PRINT_DOUBLE_WIDTH, " ":PRINT_DOUBLE_HEIGHT,:HANDHELD_NAME,:RECEIPT_NAME); ";

				  IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = NewItemKey;
				  IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = NewCourseKey;

				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  if (NextWord == "0")
				  {
					 NewItemID = GetNextUniqueMenuID(DBTransaction);
				  }
				  else
				  {
					 NewItemID = StrToIntDef(NextWord, 0);
				  }
				  IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = NewItemID;

				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("ITEM_NAME")->AsString = NextWord;
				  LastSuccessfulItem = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("ITEM_KITCHEN_NAME")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                  IBInternalQuery->ParamByName("HANDHELD_NAME")->AsString = NextWord;
                  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                  IBInternalQuery->ParamByName("RECEIPT_NAME")->AsString = NextWord;
				  // NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"','\\');
				  IBInternalQuery->ParamByName("ITEM_CATEGORY")->AsString = "";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("BUTTON_COLOUR")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("DISPLAY_SIZES")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("ENABLED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("EXCLUSIVELY_AS_SIDE")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  IBInternalQuery->ParamByName("PRINT_CHIT")->AsString = "T";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_UNDERLINED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_BOLD")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_COLOUR")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_FONT")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_DOUBLE_WIDTH")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("PRINT_DOUBLE_HEIGHT")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
				  IBInternalQuery->ParamByName("IAO")->AsInteger = IAO++;
				  IOO = 0;
				  IBInternalQuery->ExecQuery();
				  break;
			   case RSide:
				  {
					 TForcedSide Side;
					 Side.MasterItemKey = NewItemKey;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 Side.CourseLookup = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 Side.ItemLookup = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 Side.GroupNo = StrToInt(NextWord);
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 Side.Max_Select = StrToInt(NextWord);
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 Side.AllowSkip = (UpperCase(NextWord) == "TRUE");
					 Side.MenuLookup = NewMenuName;
					 Side.IOO = IOO++;
					 ForcedSidesList.push_back(Side);
				  }break;
			   case RItemSize:
				  {
					 IBInternalQuery->Close();
					 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ITEMSIZE, 1) FROM RDB$DATABASE";
					 IBInternalQuery->ExecQuery();
					 NewItemSizeKey = IBInternalQuery->Fields[0]->AsInteger;

					 IBInternalQuery->Close();

					 IBInternalQuery->SQL->Clear();
					 IBInternalQuery->SQL->Text =
						 "INSERT INTO ITEMSIZE (" "ITEMSIZE_KEY, " "ITEM_KEY, " "SIZES_KEY, " "ITEM_ID, " "SIZE_ID, "
						 "PALM_ITEM_ID, " "PALM_SIZE_ID, " "SIZE_NAME, " "PRICE, " "SPECIAL_PRICE, " "COST, " "ISAO, " "FREE, "
						 "AVAILABLE_AS_STANDARD, " "NO_RECIPE, " "BARCODE, " "SETMENU_MASK, " "ENABLED, " "GST_PERCENT, "
						 "COST_GST_PERCENT, " "POINTS_PERCENT, " "SIZE_KITCHEN_NAME, " "THIRDPARTYCODES_KEY, " "MEM_DISCOUNT_PERCENT, "
						 "LOC_DISCOUNT_PERCENT, " "MEM_SALE_COUNT, " "LOC_SALE_COUNT, " "TARE_WEIGHT, " "CATEGORY_KEY, PLU,HANDHELD_NAME,RECEIPT_NAME, PRICE_FOR_POINTS) " "VALUES ( "
						 ":ITEMSIZE_KEY, " ":ITEM_KEY, " ":SIZES_KEY, " ":ITEM_ID, " ":SIZE_ID, " ":PALM_ITEM_ID, " ":PALM_SIZE_ID, "
						 ":SIZE_NAME, " ":PRICE, " ":SPECIAL_PRICE, " ":COST, " ":ISAO, " ":FREE, " ":AVAILABLE_AS_STANDARD, "
						 ":NO_RECIPE, " ":BARCODE, " ":SETMENU_MASK, " ":ENABLED, " ":GST_PERCENT, " ":COST_GST_PERCENT, "
						 ":POINTS_PERCENT, " ":SIZE_KITCHEN_NAME, " ":THIRDPARTYCODES_KEY, " ":MEM_DISCOUNT_PERCENT, "
						 ":LOC_DISCOUNT_PERCENT, " ":MEM_SALE_COUNT, " ":LOC_SALE_COUNT, " ":TARE_WEIGHT, " ":CATEGORY_KEY, :PLU,:HANDHELD_NAME,:RECEIPT_NAME, :PRICE_FOR_POINTS);";
					 IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = NewItemSizeKey;
					 IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = NewItemKey;

					 IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = NewItemID;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("SIZE_NAME")->AsString = NextWord;
					 LastSuccessfulSize = NextWord;

					 IBInternalQuery2->Close();
					 IBInternalQuery2->SQL->Text =
						 "SELECT SIZES_KEY,SIZE_ID,SIZE_KITCHEN_NAME,HANDHELD_NAME,RECEIPT_NAME FROM SIZES "
						 "WHERE " "SIZE_NAME	= :SIZE_NAME AND MENU_KEY = :MENU_KEY";
					 IBInternalQuery2->ParamByName("SIZE_NAME")->AsString = NextWord;
					 IBInternalQuery2->ParamByName("MENU_KEY")->AsString = NewMenuKey;
					 IBInternalQuery2->ExecQuery();
					 if (IBInternalQuery2->RecordCount == 0)
					 {
						throw Exception("Size Not Found in Size Table");
					 }

					 IBInternalQuery->ParamByName("SIZES_KEY")->AsInteger = IBInternalQuery2->FieldByName("SIZES_KEY")->AsInteger;
					 IBInternalQuery->ParamByName("SIZE_ID")->AsInteger = IBInternalQuery2->FieldByName("SIZE_ID")->AsInteger;
					 IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME")->AsString = IBInternalQuery2->FieldByName("SIZE_KITCHEN_NAME")->AsString;
					 IBInternalQuery->ParamByName("HANDHELD_NAME")->AsString = IBInternalQuery2->FieldByName("HANDHELD_NAME")->AsString;
					 IBInternalQuery->ParamByName("RECEIPT_NAME")->AsString = IBInternalQuery2->FieldByName("RECEIPT_NAME")->AsString;

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("FREE")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRICE")->AsString = NextWord;

                     //if pricelevels are more than two
                     IBInternalQuery2->Close();
					 IBInternalQuery2->SQL->Text =
                                    "SELECT "
                                        "INTEGER_VAL "
                                    "FROM "
                                        "VARIABLES "
                                    "WHERE "
                                        "VARIABLES_KEY = 5014; ";
                     IBInternalQuery2->ExecQuery();
                     int noOfPricelevels;
                     noOfPricelevels= IBInternalQuery2->Fields[0]->AsInteger;

                     if (noOfPricelevels ==0)noOfPricelevels=2;

                     for(int priceLevelKey = 1; priceLevelKey <=noOfPricelevels ; priceLevelKey++)
                     {
                        IBInternalQuery3->Close();
					    IBInternalQuery3->SQL->Text =
                          "INSERT INTO PRICELEVELITEMSIZE (" "PRICELEVELITEMSIZE_KEY, " "ITEMSIZE_KEY, " "PRICELEVEL_KEY, " "PRICE, " "ISENABLED) " "VALUES ( "
					            ":PRICELEVELITEMSIZE_KEY, " ":ITEMSIZE_KEY, " ":PRICELEVEL_KEY, " ":PRICE, " ":ISENABLED);";

                         IBInternalQuery->Close();
                         IBInternalQuery->SQL->Text =
                                "SELECT GEN_ID(GEN_PriceLevelItemSize_KEY, 1) FROM RDB$DATABASE; ";
                         IBInternalQuery->ExecQuery();

                         IBInternalQuery3->ParamByName("PRICELEVELITEMSIZE_KEY")->AsInteger = IBInternalQuery->Fields[0]->AsInteger;
                         IBInternalQuery3->ParamByName("ITEMSIZE_KEY")->AsInteger = NewItemSizeKey;
                         IBInternalQuery3->ParamByName("PRICELEVEL_KEY")->AsInteger = priceLevelKey;
                         IBInternalQuery3->ParamByName("PRICE")->AsCurrency = StrToCurr(NextWord);
                         IBInternalQuery3->ParamByName("ISENABLED")->AsString = "T";
                         IBInternalQuery3->ExecQuery();
                     }

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("SPECIAL_PRICE")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("GST_PERCENT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');

                     if(StrToCurr(NextWord) < -1000000 || StrToCurr(NextWord) > 900000000)
                        NextWord = "0";

					 IBInternalQuery->ParamByName("COST")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("POINTS_PERCENT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("MEM_SALE_COUNT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("MEM_DISCOUNT_PERCENT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("LOC_SALE_COUNT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("LOC_DISCOUNT_PERCENT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("NO_RECIPE")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("SETMENU_MASK")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("AVAILABLE_AS_STANDARD")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("BARCODE")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("ENABLED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 // Lookup the financial category
					 int CategoryKey = TDBOrder::GetArchiveCategory(DBTransaction, NextWord);
					 if (CategoryKey != 0)
					 {
						IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
					 }
					 else
					 {
						throw Exception("Unable to locate Category Key " + IntToStr(CategoryKey));
					 }

					 IBInternalQuery->ParamByName("ISAO")->AsInteger = DCAO++;

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = TDBThirdPartyCodes::SetThirdPartyCode(DBTransaction,
						NextWord, "", tpItemSize);
					 if (!TDeviceRealTerminal::Instance().BasePMS->TestCode(NextWord))
					 {
						throw Exception("Unable to Import Menu " + NewMenuName + " Category " + NextWord + " is not found in PMS System");
					 }
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("TARE_WEIGHT")->AsString = NextWord;

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');

                     //TODO -o Michael -c Improvement : This should be set by Office not generated by the POS.
                     if(NextWord == "")
                     {
					 	IBInternalQuery->ParamByName("PLU")->AsInteger = NewItemSizeKey;
                     }
                     else
                     {
					 	IBInternalQuery->ParamByName("PLU")->AsString = NextWord;
                     }
					 IBInternalQuery->ExecQuery();
				  }break;
			   case RRecipe:
				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_RECIPE, 1) FROM RDB$DATABASE";
				  IBInternalQuery->ExecQuery();
				  NewRecipeKey = IBInternalQuery->Fields[0]->AsInteger;

				  IBInternalQuery->Close();

				  IBInternalQuery->SQL->Clear();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO RECIPE (" "RECIPE_KEY, " "ITEMSIZE_KEY, " "STOCK_CODE, " "QTY, " "STOCK_LOCATION," "ORGINAL_STOCK_CODE) " "VALUES ( "
					  ":RECIPE_KEY, " ":ITEMSIZE_KEY, " ":STOCK_CODE, " ":QTY, " ":STOCK_LOCATION,"":ORGINAL_STOCK_CODE);";
				  IBInternalQuery->ParamByName("RECIPE_KEY")->AsInteger = NewRecipeKey;
				  IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = NewItemSizeKey;

				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("STOCK_CODE")->AsString = NextWord;
                                  IBInternalQuery->ParamByName("ORGINAL_STOCK_CODE")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  // IBInternalQuery->ParamByName("STOCK_DESC")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  // IBInternalQuery->ParamByName("UNIT")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("QTY")->AsString = NextWord;
				  NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
				  IBInternalQuery->ParamByName("STOCK_LOCATION")->AsString = NextWord;
				  IBInternalQuery->ExecQuery();
				  break;
			   case RCategory:
				  {
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 int CatKey = TDBOrder::GetArchiveCategory(DBTransaction, NextWord);
					 if (CatKey != 0)
					 {
						IBInternalQuery2->Close();
						IBInternalQuery2->SQL->Text =
							"INSERT INTO ITEMSIZECATEGORY (" "ITEMSIZE_KEY," "CATEGORY_KEY) " "VALUES (" ":ITEMSIZE_KEY,"
							":CATEGORY_KEY);";
						IBInternalQuery2->ParamByName("ITEMSIZE_KEY")->AsInteger = NewItemSizeKey;
						IBInternalQuery2->ParamByName("CATEGORY_KEY")->AsInteger = CatKey;
						IBInternalQuery2->ExecQuery();
					 }
					 else
					 {
						throw Exception("Unable to Import Menu the BreakDown Category : " + NextWord + " was not found");
					 }
				  }break;
			   case ROption:
				  {
					 IBInternalQuery->Close();
					 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_OPTION, 1) FROM RDB$DATABASE";
					 IBInternalQuery->ExecQuery();
					 NewOptionKey = IBInternalQuery->Fields[0]->AsInteger;

					 IBInternalQuery->Close();

					 IBInternalQuery->SQL->Clear();
					 IBInternalQuery->SQL->Text =
						 "INSERT INTO OPTIONS (" "OPTIONS_KEY, " "COURSE_KEY, " "OPTION_ID, " "OPTION_NAME, "
						 "OPTION_ORDER, " "REDIR_ID, " "ENABLED, " "FLAGS, " "FORCED_MASK, " "OPTION_KITCHEN_NAME, "
						 "PRINT_UNDERLINED, " "PRINT_BOLD, " "PRINT_COLOUR, " "PRINT_FONT, " "PRINT_DOUBLE_WIDTH, "
                         "PRINT_DOUBLE_HEIGHT,HANDHELD_NAME,RECEIPT_NAME) "
						 "VALUES ( " ":OPTIONS_KEY, " ":COURSE_KEY, " ":OPTION_ID, " ":OPTION_NAME, " ":OPTION_ORDER, " ":REDIR_ID, "
						 ":ENABLED, " ":FLAGS, " ":FORCED_MASK, " ":OPTION_KITCHEN_NAME, " ":PRINT_UNDERLINED, " ":PRINT_BOLD, "
						 ":PRINT_COLOUR, " ":PRINT_FONT, " ":PRINT_DOUBLE_WIDTH, " ":PRINT_DOUBLE_HEIGHT,:HANDHELD_NAME,:RECEIPT_NAME);";

					 IBInternalQuery->ParamByName("OPTIONS_KEY")->AsInteger = NewOptionKey;
					 IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = NewCourseKey;

					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 if (NextWord == "0")
					 {
						IBInternalQuery->ParamByName("OPTION_ID")->AsInteger = GetNextUniqueOptionID(DBTransaction);
					 }
					 else
					 {
						IBInternalQuery->ParamByName("OPTION_ID")->AsString = NextWord;
					 }
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("OPTION_NAME")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("OPTION_KITCHEN_NAME")->AsString = NextWord;
                     NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                     IBInternalQuery->ParamByName("HANDHELD_NAME")->AsString = NextWord;
                     NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
                     IBInternalQuery->ParamByName("RECEIPT_NAME")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\'); // CondimentsMask
					 IBInternalQuery->ParamByName("FORCED_MASK")->AsInteger = StrToInt(NextWord);
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\'); // CondimentsFlags
					 IBInternalQuery->ParamByName("FLAGS")->AsInteger = StrToInt(NextWord);
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("REDIR_ID")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("ENABLED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_UNDERLINED")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_BOLD")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_COLOUR")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_FONT")->AsString = NextWord;
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_DOUBLE_WIDTH")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 NextWord = ReadCSVWord(Menu->Strings[i], Offset, ',', '"', '\\');
					 IBInternalQuery->ParamByName("PRINT_DOUBLE_HEIGHT")->AsString = (UpperCase(NextWord) == "TRUE") ? "T" : "F";
					 IBInternalQuery->ParamByName("OPTION_ORDER")->AsInteger = OAO++;
					 IBInternalQuery->ExecQuery();
				  }break;
			   }
			}
		 }

		 Success = true;
		 for (int i = 0; i < ForcedSidesList.size(); i++)
		 {
			TForcedSide Side = ForcedSidesList[i];
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT " "ITEM_KEY " "FROM " "ITEM " " INNER JOIN COURSE ON ITEM.COURSE_KEY = COURSE.COURSE_KEY"
				" INNER JOIN MENU ON COURSE.MENU_KEY = MENU.MENU_KEY " "WHERE " "MENU.MENU_NAME = :MENU_NAME AND "
				"COURSE.COURSE_NAME = :COURSE_NAME AND " "ITEM.ITEM_NAME = :ITEM_NAME";
			IBInternalQuery->ParamByName("MENU_NAME")->AsString = Side.MenuLookup;
			IBInternalQuery->ParamByName("COURSE_NAME")->AsString = Side.CourseLookup;
			IBInternalQuery->ParamByName("ITEM_NAME")->AsString = Side.ItemLookup;
			IBInternalQuery->ExecQuery();

			if (IBInternalQuery->RecordCount)
			{
			   IBInternalQuery2->Close();
			   IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_ITEMSIDES, 1) FROM RDB$DATABASE";
			   IBInternalQuery2->ExecQuery();
			   int NewItemSidesKey = IBInternalQuery2->Fields[0]->AsInteger;

			   IBInternalQuery2->Close();
			   IBInternalQuery2->ParamCheck = true;
			   IBInternalQuery2->SQL->Clear();
			   IBInternalQuery2->SQL->Text =
				   "INSERT INTO ITEMSIDES (" "ITEMSIDES_KEY, " "MASTER_ITEM_KEY, " "ITEM_KEY, " "IOO, " "MAX_SELECT, " "GROUP_NO, "
				   "ALLOW_SKIP) " "VALUES ( " ":ITEMSIDES_KEY, " ":MASTER_ITEM_KEY, " ":ITEM_KEY, " ":IOO, " ":MAX_SELECT, " ":GROUP_NO, "
				   ":ALLOW_SKIP);";
			   IBInternalQuery2->ParamByName("ITEMSIDES_KEY")->AsInteger = NewItemSidesKey;
			   IBInternalQuery2->ParamByName("MASTER_ITEM_KEY")->AsInteger = Side.MasterItemKey;
			   IBInternalQuery2->ParamByName("ITEM_KEY")->AsInteger = IBInternalQuery->FieldByName("ITEM_KEY")->AsInteger;
			   IBInternalQuery2->ParamByName("IOO")->AsInteger = Side.IOO;
			   IBInternalQuery2->ParamByName("GROUP_NO")->AsInteger = Side.GroupNo;
			   IBInternalQuery2->ParamByName("MAX_SELECT")->AsInteger = Side.Max_Select;
			   IBInternalQuery2->ParamByName("ALLOW_SKIP")->AsString = Side.AllowSkip ? "T" : "F";
			   IBInternalQuery2->ExecQuery();
			}
		 }

		 if (MenuKeyToDelete != 0)
		 {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT " "COURSE_KEY, " "COURSE_NAME, " "MENU_KEY " "FROM " "COURSE " "WHERE "
				"MENU_KEY = :OLDMENU_KEY";
			IBInternalQuery->ParamByName("OLDMENU_KEY")->AsInteger = MenuKeyToDelete;
			IBInternalQuery->ExecQuery();
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
			   // Find the Equv Course in the New Menu by course name.
			   IBInternalQuery2->Close();
			   IBInternalQuery2->SQL->Text = "SELECT " "COURSE_KEY, " "COURSE_NAME, " "MENU_KEY " "FROM " "COURSE " "WHERE "
				   "MENU_KEY = :NEWMENU_KEY AND " "COURSE_NAME = :OLDCOURSE_NAME";
			   IBInternalQuery2->ParamByName("NEWMENU_KEY")->AsInteger = NewMenuKey;

			   IBInternalQuery2->ParamByName("OLDCOURSE_NAME")->AsString = IBInternalQuery->FieldByName("COURSE_NAME")->AsString;
			   /* IBInternalQuery2->ParamByName("OLDCOURSE_NAME")->AsString =
			   UTF8ToUnicodeString(IBInternalQuery->FieldByName("COURSE_NAME")->AsString); */

			   IBInternalQuery2->ExecQuery();
			   if (!IBInternalQuery2->Eof)
			   {
				  // Update the PRNORDER table.
				  IBInternalQuery3->Close();
				  IBInternalQuery3->ParamCheck = true;
				  IBInternalQuery3->SQL->Text = "UPDATE " "PRNORDER " "SET " "PRNORDER.MENU_KEY = :THIS_NEWMENU_KEY, "
					  "PRNORDER.COURSE_KEY = :THIS_NEWCOURSE_KEY " "WHERE " "PRNORDER.COURSE_KEY = :THIS_OLDCOURSE_KEY";
				  IBInternalQuery3->ParamByName("THIS_NEWMENU_KEY")->AsInteger = NewMenuKey;
				  IBInternalQuery3->ParamByName("THIS_NEWCOURSE_KEY")->AsInteger = IBInternalQuery2->FieldByName("COURSE_KEY")->AsInteger;
				  IBInternalQuery3->ParamByName("THIS_OLDCOURSE_KEY")->AsInteger = IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger;
				  IBInternalQuery3->ExecQuery();
			   }
			}

			// Remove Menu to be deleted.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "DELETE FROM MENU WHERE MENU_KEY = :LOC_MENU_KEY";
			IBInternalQuery->ParamByName("LOC_MENU_KEY")->AsInteger = MenuKeyToDelete;
			IBInternalQuery->ExecQuery();
		 }
		 frmProcessing->Close();

		 DBTransaction.Commit();

		 TDeviceRealTerminal::Instance().BasePMS->ClearCodesTestedOk();
	  }
	  catch(Exception & E)
	  {
		 MessageBox("The menu update failed." "\rMenu: " + NewMenuName + "\rLast successful course: " + LastSuccessfulCourse +
			"\rLast successful item: " + LastSuccessfulItem + "\rLast successful size: " + LastSuccessfulSize +
			"\rPlease report this error message to your system administrator. \r" + E.Message, "Error", MB_ICONERROR + MB_OK);
		 TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		 frmProcessing->Close();
	  }
   }
   else
   {
	  MessageBox("The file format is invalid.", "Error", MB_ICONERROR + MB_OK);
   }
   return Success;
}
// ---------------------------------------------------------------------------
void TManagerMenusPOS::ForceMenu(TIBDatabase *IBDatabase, UnicodeString FileName)
{
   TStringList *Menu;
   TFileStream *MenuStream;
   try
   {
	  bool MenuFailed = false;

	  std::auto_ptr <TStringList> Menu(new TStringList);
	  TFileStream *MenuStream = new TFileStream(FileName, fmOpenRead | fmShareExclusive);
	  try
	  {
		 Menu->LoadFromStream(MenuStream);
		 if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "Menu Import\\Old Menus"))
		 {
			CreateDir(ExtractFilePath(Application->ExeName) + "Menu Import\\Old Menus");
		 }

		 if (Menu->Strings[0] == MENU_IMPORT_VERSION)
		 {
			if (LoadMenu(IBDatabase, Menu.get(), true))
			{
			   Menu->SaveToFile(ExtractFilePath(Application->ExeName) + "Menu Import\\Old Menus\\" + Now().FormatString
				  ("yyyy-mm-dd - hh-nn-ss ") + ExtractFileName(FileName));
			}
			else
			{
			   MenuFailed = true;
			}
		 }
		 else
		 {
			MessageBox("The Menu file " + ExtractFileName(FileName) + " is the wrong version.\rIt is " + Menu->Strings[0]
			   + " where " + MENU_IMPORT_VERSION + " is required.\rPlease ensure MenuMate Office has been upgraded.\rThe Menu file " +
			   ExtractFileName(FileName) + " has been moved into the Failed folder.", "Error", MB_ICONERROR + MB_OK);
			MenuFailed = true;
		 }
	  }
	  __finally
	  {
		 delete MenuStream;
	  }

	  if (MenuFailed)
	  {
		 if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "Menu Import\\Failed"))
		 {
			ForceDirectories(ExtractFilePath(Application->ExeName) + "Menu Import\\Failed");
		 }
		 if (FileExists(ExtractFilePath(Application->ExeName) + "Menu Import\\Failed\\" + ExtractFileName(FileName)))
		 {
			DeleteFile(ExtractFilePath(Application->ExeName) + "Menu Import\\Failed\\" + ExtractFileName(FileName));
		 }
		 UnicodeString Dest = ExtractFilePath(Application->ExeName) + "Menu Import\\Failed\\" + ExtractFileName(FileName);
		 if(CopyFile(FileName.t_str(), Dest.t_str(), false) != 0)
         {
             if (FileExists(ExtractFilePath(Application->ExeName) + "Menu Import\\" + ExtractFileName(FileName)))
             {
                DeleteFile(ExtractFilePath(Application->ExeName) + "Menu Import\\" + ExtractFileName(FileName));
             }

         }
	  }
   }
   __finally
   {
	  DeleteFile(FileName);
   }
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// New XML menu format ... Used from version 5.18.
bool TManagerMenusPOS::LoadMenus( TIBDatabase *IBDatabase, AnsiString inDirName )
{
	// Returns  TRUE if at least one menu has been loaded.
	// Returns FALSE if no menu has been loaded.

	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
	frmProcessing->Message = "Importing Menus";
	frmProcessing->Show();

	try
	{
		std::auto_ptr<TImportMenu> importMenu( new TImportMenu( IBDatabase ) );

		importMenu->ResetErrorLog();
		result = importMenu->ImportFromXMLFilesInDir( inDirName );

		if( importMenu->FailedToLoadAMenu() ) // At least one menu failed to be loaded.
		{
			MessageBox(	importMenu->ErrorLog(), "Error", MB_ICONERROR + MB_OK );
		}

		frmProcessing->Close();

         //Sync Menu with web for online ordering..
         if(TGlobalSettings::Instance().EnableOnlineOrdering)
         {
            frmSelectDish->SyncSiteMenus();
         }

		TDeviceRealTerminal::Instance().BasePMS->ClearCodesTestedOk();

	}
	catch( Exception & E )  // Review this catch section.
	{
		MessageBox(	"The menu load process failed: " + E.Message, "Error", MB_ICONERROR + MB_OK);

		TManagerLogs::Instance().Add( __FUNC__, EXCEPTIONLOG, E.Message );
		frmProcessing->Close();
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
// ---------------------------------------------------------------------------

void TManagerMenusPOS::UpdateMenuChanged(Database::TDBControl &DBControl, TNetMessageMenuChanged *Request, bool ShowProgress)
{
   std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
   Processing->Message = "Loading Menus";
   if (ShowProgress)
   {
	  Processing->Show();
   }

   try
   {
	  bool MenuChanged = false;
	  for (std::map <UnicodeString, eMenuCommand> ::const_iterator RequestMenus = Request->Menu_Names.begin();
		 RequestMenus != Request->Menu_Names.end(); RequestMenus++)
	  {
		 UnicodeString RequestMenuName = RequestMenus->first;
		 eMenuCommand RequestCommand = RequestMenus->second;
		 if (RequestCommand == eMenuRemove)
		 {
			TListMenu *Menu = new TListMenu;
			Menu->MenuKey = 0;
			Menu->MenuName = RequestMenuName;
			Menu->SwapInCommand = RequestCommand;
			New->MenuDelete(New->MenuGet(RequestMenuName));
			New->MenuAdd(Menu);
			MenuChanged = true;
		 }
		 else if (RequestCommand == eMenuAddReplace)
		 {
			LoadMenuFromDB(DBControl, RequestMenuName, RequestCommand);
			MenuChanged = true;
		 }
		 OnTillsMenusAltered.Occured();
	  }
	  if (MenuChanged == true)
	  {
		 Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
		 DBTransaction1.StartTransaction();
		 Initialise(DBTransaction1);
		 DBTransaction1.Commit();
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  MessageBox("The menu load failed.\r" + Err.Message, "Menu load Error", MB_ICONERROR + MB_OK);
   }

   if (ShowProgress)
   {
	  Processing->Close();
   }
}

// ---------------------------------------------------------------------------
void
TManagerMenusPOS::SetCurrentMenu(const UnicodeString &name)
{
    VisibleMenu = Current->MenuGet(name);
}

void TManagerMenusPOS::ToggleMenus()
{
   if (VisibleMenu != NULL)
   {
	  if (Current->Count == 0)
	  {
		 SwapInNewMenus();
	  }

	  TListMenu *NextMenu = Current->MenuGetNext(VisibleMenu);
	  if (NextMenu != NULL)
	  {
		 VisibleMenu = NextMenu;
	  }
	  else
	  {
		 VisibleMenu = Current->MenuGet(0);
	  }
   }
   else
   {
	  TListMenu *NextMenu = Current->MenuGet(0);
	  if (NextMenu != NULL)
	  {
		 VisibleMenu = NextMenu;
	  }
	  else
	  {
		 SwapInNewMenus();
		 NextMenu = Current->MenuGet(0);
		 if (NextMenu != NULL)
		 {
			VisibleMenu = NextMenu;
		 }
		 else
		 {
			VisibleMenu = NULL;
		 }
	  }
   }
}

// ---------------------------------------------------------------------------

void TManagerMenusPOS::SwapInNewMenus()
{
   if (New->Count > 0)
   {
	  std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
	  frmProcessing->Message = "Updating Menus";
	  frmProcessing->Show();
	  try
	  {
		 bool InvalidateVisibleMenu = false;
		 bool AMenuNowExits = false;
		 TListMenu *NewVisibleMenu = NULL;
		 for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->New->Count; i++)
		 {
			TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->New->MenuGet(i);

			if (Menu->SwapInCommand == eMenuRemove)
			{
			   TListMenu *MenuToRemove = Current->MenuGet(Menu->MenuName);
			   if (VisibleMenu == MenuToRemove)
			   {
				  InvalidateVisibleMenu = true;
			   }

			   Current->MenuDelete(MenuToRemove);
			   UpdateSync.erase(Menu->MenuKey);
			}
			else
			{
			   TListMenu *MenuToReplace = Current->MenuGet(Menu->MenuName);
			   if (VisibleMenu == MenuToReplace)
			   {
				  InvalidateVisibleMenu = true;
				  NewVisibleMenu = Menu;
			   }
			   Current->MenuDelete(MenuToReplace);
			   if (VisibleMenu == NULL && Current->Count == 0)
		        { // This will trigger a Menu change event so System can freash itself.
				  AMenuNowExits = true;
		  	  }
			   Current->MenuAdd(Menu);
			   UpdateSync[Menu->MenuKey] = Menu->EnabledStateSync;
			}
		 }
		 New->MenuClear();
		 if (InvalidateVisibleMenu)
		 {
			VisibleMenu = NULL;
			if (NewVisibleMenu != NULL)
			{
			   VisibleMenu = NewVisibleMenu;
			}
		 }
		 if (AMenuNowExits)
		 {
			TDeviceRealTerminal::Instance().Menus->VisibleMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(0);
		 }
	  }
	  __finally
	  {
		 frmProcessing->Close();
	  }
   }
}

// ---------------------------------------------------------------------------
void TManagerMenusPOS::UpdateMenuAltered(Database::TDBTransaction &DBTransaction, TNetMessageMenuAltered *Request)
{

   try
   {
	  for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
	  {
		 TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
		 if (CurrentMenu->MenuKey == Request->MenuKey)
		 {
			// Alter RAM.
			TItem *ThisItem = CurrentMenu->FetchItemByKey(Request->ItemKey);
			ThisItem->Enabled = Request->ItemEnabled;
			// Alter MenuEnableState.
			CurrentMenu->EnabledStateSync++;
		 }
	  }

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  if (Request->AlterDB)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " UPDATE \"ITEM\" i " " SET " " i.\"ENABLED\" = :THIS_ENABLED " " WHERE "
			 "  i.\"ITEM_KEY\" = :LOC_ITEM_KEY ";
		 IBInternalQuery->ParamByName("THIS_ENABLED")->AsString = Request->ItemEnabled ? "T" : "F";
		 IBInternalQuery->ParamByName("LOC_ITEM_KEY")->AsInteger = Request->ItemKey;
		 IBInternalQuery->ExecQuery();

	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	  throw;
   }
}

void TManagerMenusPOS::MenuChanged(Database::TDBTransaction &DBTransaction, TNetMessageMenuChanged * Request, bool ShowProgress)
{
   // Sort out which menus have to be removed and or updated from this request..
   if (Request->Broadcast)
   {
	  TDeviceRealTerminal::Instance().ManagerNet->SendToAll(Request);
   }

   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);

	  // remove all current menus that are not in the request.
	  if (Request->Menu_Names.find(Menu->MenuName) == Request->Menu_Names.end())
	  {
		 Request->Menu_Names[Menu->MenuName] = eMenuRemove;
	  }
	  else
	  { // remove menus from request that are current (dont bother reimporting them).
		 if (IsAValidMenu(DBTransaction, Menu->MenuKey))
		 {
			Request->Menu_Names.erase(Menu->MenuName);
		 }
		 else if (IsAPendingMenu(Menu->MenuName))
		 {
			Request->Menu_Names.erase(Menu->MenuName);
		 }
	  }
   }

   if (Request->Menu_Names.size() > 0)
   {
	  UpdateMenuChanged(TDeviceRealTerminal::Instance().DBControl, Request, ShowProgress);
   }
}

// ---------------------------------------------------------------------------
void TManagerMenusPOS::MenuAddedRemoved(Database::TDBTransaction &DBTransaction, TNetMessageMenuChanged * Request)
{
   // Simply rechecks all our current menus to see if they have changed.

   if (Request->Broadcast)
   {
	  Request->CompareToDataBase = true;
	  TDeviceRealTerminal::Instance().ManagerNet->SendToAll(Request);
   }

   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
	  if (RequiresUpdate(DBTransaction, Menu->MenuName))
	  {
		 Request->Menu_Names[Menu->MenuName] = eMenuAddReplace;
	  }
	  else if (RequiresRemove(DBTransaction, Menu->MenuName))
	  {
		 Request->Menu_Names[Menu->MenuName] = eMenuRemove;
	  }

   }
   if (Request->Menu_Names.size() > 0)
   {
	  TDeviceRealTerminal::Instance().Menus->UpdateMenuChanged(TDeviceRealTerminal::Instance().DBControl, Request);
   }
   OnNewMenus.Occured();
}

int TManagerMenusPOS::GetNextUniqueMenuID(Database::TDBTransaction &DBTransaction)
{
   int key = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MENUID, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  key = IBInternalQuery->Fields[0]->AsInteger;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return key;
}

int TManagerMenusPOS::GetNextUniqueOptionID(Database::TDBTransaction &DBTransaction)
{
   int key = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_OPTION, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  key = IBInternalQuery->Fields[0]->AsInteger;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return key;
}

TServingCourse TManagerMenusPOS::GetServingCourse(int ServingCourseKey)
{
   TServingCourse RetVal;

   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
   {
	  TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
	  for (int i = 0; i < Menu->ServingCourses.size(); i++)
	  {
		 if (Menu->ServingCourses[i].ServingCourseKey == ServingCourseKey)
		 {
			RetVal = Menu->ServingCourses[i];
		 }
	  }
   }
   return RetVal;
}

void __fastcall TManagerMenusPOS::SetVisibleMenu(TListMenu * value)
{
   if (FVisibleMenu != value)
   {
	  FVisibleMenu = value;
	  OnMenuChanged.Occured();
   }
}

TListMenu * __fastcall TManagerMenusPOS::GetVisibleMenu()
{
   return FVisibleMenu;
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
