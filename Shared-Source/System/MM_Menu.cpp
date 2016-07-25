// ---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_Menu.h"
#include "DB_Utilities.h"
#include "DeviceRealTerminal.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
using namespace Menu;

//---------------------------------------------------------------------------
//	NameAndKey
//---------------------------------------------------------------------------
TNameAndKey::TNameAndKey( __int32 inKey, UnicodeString inName )
{
    Name = inName;
    Key  = inKey;
}

//---------------------------------------------------------------------------
TMenuLoadDB::TMenuLoadDB(TIBDatabase *MMDatabase, int MenuKey) : DBControl(MMDatabase), sqlMenu(new TIBSQL(NULL)),
sqlMenuCount(new TIBSQL(NULL)), sqlAvailableSizes(new TIBSQL(NULL)), sqlAvailableCategories(new TIBSQL(NULL)),
sqlAvailableLocations(new TIBSQL(NULL)), sqlItemSizeTaxProfiles(new TIBSQL(NULL)), sqlItemSides(new TIBSQL(NULL)),
sqlCourseOptions(new TIBSQL(NULL)), sqlSizeCategories(new TIBSQL(NULL)), sqlAvailableServingCourses(new TIBSQL(NULL)),sqlItemSizePriceLevels(new TIBSQL(NULL))
{
   DBControl.RegisterQuery(sqlMenu);
   DBControl.RegisterQuery(sqlMenuCount);
   DBControl.RegisterQuery(sqlAvailableSizes);
   DBControl.RegisterQuery(sqlAvailableCategories);
   DBControl.RegisterQuery(sqlAvailableLocations);
   DBControl.RegisterQuery(sqlItemSizeTaxProfiles);
   DBControl.RegisterQuery(sqlItemSides);
   DBControl.RegisterQuery(sqlCourseOptions);
   DBControl.RegisterQuery(sqlSizeCategories);
   DBControl.RegisterQuery(sqlAvailableServingCourses);
   DBControl.RegisterQuery(sqlItemSizePriceLevels);

   fMenuKey = MenuKey;

   sqlMenu->SQL->Text = MenuSQL;
   sqlMenuCount->SQL->Text = MenuCountSQL;
   sqlAvailableSizes->SQL->Text = AvailableSizesSQL;
   sqlAvailableCategories->SQL->Text = AvailableCategoriesSQL;
   sqlAvailableLocations->SQL->Text = AvailableLocationsSQL;
   sqlItemSizeTaxProfiles->SQL->Text = ItemSizeTaxProfilesSQL;
   sqlItemSides->SQL->Text = ItemSidesSQL;
   sqlCourseOptions->SQL->Text = CourseOptionsSQL;
   sqlSizeCategories->SQL->Text = SizeCategoriesSQL;
   sqlAvailableServingCourses->SQL->Text = AvailableServingCoursesSQL;
   sqlItemSizePriceLevels->SQL->Text = PriceLevelSQL;

   DBControl.StartTransaction();
}

// ---------------------------------------------------------------------------
TMenuLoadDB::~TMenuLoadDB()
{
   DBControl.CommitTransaction();
}

// ---------------------------------------------------------------------------
void TMenuLoadDB::EnumMenus(TMenusInfo &MenusInfo)
{
   Database::TcpIBSQL sqlEnumMenu(new TIBSQL(NULL));

   DBControl.RegisterQuery(sqlEnumMenu);

   sqlEnumMenu->SQL->Text = "Select Menu_Key, Menu_Name, Menu_Type, Palmable From Menu Order By Menu_Name";

   MenusInfo.clear();

   sqlEnumMenu->Close();
   for (sqlEnumMenu->ExecQuery(); !sqlEnumMenu->Eof; sqlEnumMenu->Next())
   {
	  TMenuInfo MenuInfo;
	  MenuInfo.Key = sqlEnumMenu->FieldByName("Menu_Key")->AsInteger;
	  MenuInfo.Menu_Name = sqlEnumMenu->FieldByName("Menu_Name")->AsString;
	  MenuInfo.Menu_Type = static_cast <TMenuType> (sqlEnumMenu->FieldByName("Menu_Type")->AsInteger);
	  MenuInfo.Palmable = sqlEnumMenu->FieldByName("Palmable")->AsString == "T";
	  MenusInfo.push_back(MenuInfo);
   }
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetMenu(Menu::TMenuInfo *MenuInfo, Menu::TSizesInfo *SizesInfo, TCategoriesInfo *CategoriesInfo,
   TLocationsInfo *LocationsInfo, TServingCoursesInfo *ServingCoursesInfo)
{
   sqlMenu->Close();
   sqlMenu->ParamByName("Menu_Key")->AsInteger = fMenuKey;
   sqlMenu->ExecQuery();

   sqlMenuCount->Close();
   sqlMenuCount->ParamByName("Menu_Key")->AsInteger = fMenuKey;
   sqlMenuCount->ExecQuery();
   fCount = sqlMenuCount->Fields[0]->AsInteger;
   sqlMenuCount->Close();

   sqlCourseOptions->Close();
   sqlCourseOptions->ParamByName("Menu_Key")->AsInteger = fMenuKey;
   sqlCourseOptions->ExecQuery();

   sqlItemSides->Close();
   sqlItemSides->ParamByName("Menu_Key")->AsInteger = fMenuKey;
   sqlItemSides->ExecQuery();

   sqlSizeCategories->Close();
   sqlSizeCategories->ParamByName("Menu_Key")->AsInteger = fMenuKey;
   sqlSizeCategories->ExecQuery();

   fLastCourse = -1;
   fLastItem = -1;
   fLastItemSize = -1;

   fPosition = 0;
   bool Success = true;
   if (!sqlMenu->Eof)
   {
	  MenuInfo->Key = sqlMenu->FieldByName("Menu_Key")->AsInteger;
	  MenuInfo->Menu_Name = sqlMenu->FieldByName("Menu_Name")->AsString;
	  MenuInfo->Menu_Type = static_cast <TMenuType> (sqlMenu->FieldByName("Menu_Type")->AsInteger);
	  MenuInfo->Palmable = sqlMenu->FieldByName("Palmable")->AsString == "T";

	  if (MenuInfo->Menu_Type == mtBeverageMenu)
	  {
		 fPalmID = 0x4001;
	  }
	  else
	  {
		 fPalmID = 0x0001;
	  }
	  Success = true;
   }
   else
   {
	  MenuInfo->Key = 0;
	  MenuInfo->Menu_Name = "";
	  MenuInfo->Menu_Type = TMenuType(0);
	  MenuInfo->Palmable = false;

	  Success = false;
   }
   return Success && GetAvailableSizes(SizesInfo) && GetAvailableCategories(CategoriesInfo) && GetAvailableLocations(LocationsInfo)
	   && GetAvailableServingCourses(ServingCoursesInfo);
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableSizes(TSizesInfo *SizesInfo)
{
   bool Success = false;

   if (SizesInfo)
   {
	  unsigned char PalmSizeID = 1;

	  sqlAvailableSizes->Close();
	  sqlAvailableSizes->ParamByName("Menu_Key")->AsInteger = fMenuKey;
	  for (sqlAvailableSizes->ExecQuery(); !sqlAvailableSizes->Eof; sqlAvailableSizes->Next())
	  {
		 TSizeInfo SizeInfo;

		 SizeInfo.Size_Name = UTF8ToUnicodeString((AnsiString)sqlAvailableSizes->FieldByName("Size_Name")->AsString);
		 SizeInfo.Size_Kitchen_Name = UTF8ToUnicodeString((AnsiString)sqlAvailableSizes->FieldByName("Size_Kitchen_Name")->AsString);
		 SizeInfo.Size_ID = sqlAvailableSizes->FieldByName("Size_ID")->AsInteger;
		 SizeInfo.Weighted_Size = sqlAvailableSizes->FieldByName("Weighted_Size")->AsString == "T";
		 SizeInfo.PalmID = PalmSizeID++;

		 SizesInfo->Sizes.push_back(SizeInfo);
	  }
	  sqlAvailableSizes->Close();
	  Success = true;
   }
   else
   {
	  Success = true;
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo)
{
   // also called from loadfromfile in versions prior to 3.4
   bool Success = false;
   if (ServingCoursesInfo)
   {
	  // 2 passes needed as you need to get the Serving Courses for the
	  // Menu and then the rest of the ones from the Database where they are Deleted = F
	  TVarRec v[] =
	  {
		 FirstFrom, FirstWhere
	  };
	  sqlAvailableServingCourses->SQL->Text = Format(AvailableServingCoursesSQL, v, ARRAYSIZE(v) - 1);
	  sqlAvailableServingCourses->ParamByName("Menu_Key")->AsInteger = fMenuKey;
	  LoadServingCourses(ServingCoursesInfo, true);

	  TVarRec v1[] =
	  {
		 SecondFrom, SecondWhere
	  };
	  sqlAvailableServingCourses->SQL->Text = Format(AvailableServingCoursesSQL, v1, ARRAYSIZE(v1) - 1);
	  LoadServingCourses(ServingCoursesInfo, false);

	  Success = true;
   }
   return Success;
}

// ---------------------------------------------------------------------------
void TMenuLoadDB::LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo, bool Enabled)
{
   bool Found = false;
   // 2nd pass and there are no Serving Courses in the Menu - new Menu so make them all Enabled
   if (sqlAvailableServingCourses->SQL->Text.Pos(SecondWhere) > 0 && ServingCoursesInfo->ServingCourses.size() == 0)
   {
	  Enabled = true;
   }
   for (sqlAvailableServingCourses->ExecQuery(); !sqlAvailableServingCourses->Eof; sqlAvailableServingCourses->Next())
   {
	  // weed out the duplicate entries from 2 passes of different sql's
	  Found = false;
	  for (unsigned i = 0; i < ServingCoursesInfo->ServingCourses.size(); i++)
	  {
		 if (UTF8ToUnicodeString((AnsiString)sqlAvailableServingCourses->FieldByName("ServingCourse_Name")->AsString) == ServingCoursesInfo->ServingCourses[i].ServingCourse_Name)
			Found = true;
	  }
	  if (!Found)
	  {
		 TServingCourseInfo ServingCourseInfo;
		 ServingCourseInfo.Key = sqlAvailableServingCourses->FieldByName("ServingCourses_Key")->AsInteger;
		 ServingCourseInfo.ServingCourse_Name = UTF8ToUnicodeString((AnsiString)sqlAvailableServingCourses->FieldByName("ServingCourse_Name")->AsString);
		 ServingCourseInfo.ServingCourse_Kitchen_Name =  UTF8ToUnicodeString((AnsiString)sqlAvailableServingCourses->FieldByName("ServingCourse_Kitchen_Name")->AsString);
		 ServingCourseInfo.Colour = (TColor)sqlAvailableServingCourses->FieldByName("Colour")->AsInteger;
		 ServingCourseInfo.Enabled = Enabled; // sqlAvailableServingCourses->FieldByName("Deleted")->AsString == "T";
		 ServingCourseInfo.Deleted = false;
		 ServingCourseInfo.Selectable = DB_Utilities::StrToBoolean(sqlAvailableServingCourses->FieldByName("Selectable")->AsString);
		 ServingCoursesInfo->ServingCourses.push_back(ServingCourseInfo);
	  }
   }
   sqlAvailableServingCourses->Close();

}

// ---------------------------------------------------------------------------
void TMenuLoadDB::LoadItemSizeTaxProfiles(TItemSizeInfo *ItemSizeInfo)
{
    AnsiString  name;
    TaxType     type;
    Percentage  rate;
    Priority    priority;
    int         profileKey;

    sqlItemSizeTaxProfiles->Close();
    sqlItemSizeTaxProfiles->ParamByName("ItemSize_Key")->AsInteger = ItemSizeInfo->Key;
    sqlItemSizeTaxProfiles->ExecQuery();
    while (!sqlItemSizeTaxProfiles->Eof)
    {
        name       = sqlItemSizeTaxProfiles->FieldByName("Name")->AsString;
        rate       = sqlItemSizeTaxProfiles->FieldByName("Rate")->AsCurrency;
        priority   = sqlItemSizeTaxProfiles->FieldByName("Priority")->AsInteger;
        profileKey = sqlItemSizeTaxProfiles->FieldByName("Profile_key")->AsInteger;

        type = TaxProfile::Convert(sqlItemSizeTaxProfiles->FieldByName("Type")->AsInteger);

        TaxProfile taxProfile(profileKey);

        taxProfile.taxProfileName = name;
        taxProfile.taxPercentage  = rate;
        taxProfile.taxProfileType = type;
        taxProfile.taxPriority    = priority;

        ItemSizeInfo->TaxProfiles.push_back(taxProfile);

        sqlItemSizeTaxProfiles->Next();
    }
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableCategories(TCategoriesInfo *CategoriesInfo)
{
   bool Success = false;

   if (CategoriesInfo)
   {
      sqlAvailableCategories->Close();
	  for (sqlAvailableCategories->ExecQuery(); !sqlAvailableCategories->Eof; sqlAvailableCategories->Next())
	  {
		 bool FoundGroup = false;
		 std::vector <TCategoryGroupInfo> ::iterator iGroup;
		 for (iGroup = CategoriesInfo->CategoryGroups.begin(); iGroup != CategoriesInfo->CategoryGroups.end(); iGroup++)
		 {
			if ((*iGroup).Category_Group_Name == UTF8ToUnicodeString((AnsiString)sqlAvailableCategories->FieldByName("Category_Group_Name")->AsString))
			{
			   FoundGroup = true;
			   break;
			}
		 }
		 if (!FoundGroup)
		 {
			TCategoryGroupInfo CategoryGroupInfo;
			CategoryGroupInfo.Category_Group_Name = UTF8ToUnicodeString((AnsiString)sqlAvailableCategories->FieldByName("Category_Group_Name")->AsString);
            TCategoryInfo category;
            category.Key = sqlAvailableCategories->FieldByName("Category_Key")->AsInteger;
            category.Category_Name = sqlAvailableCategories->FieldByName("Category")->AsString;
            category.Category_GL_Code = sqlAvailableCategories->FieldByName("GL_Code")->AsString;
            CategoryGroupInfo.Categories.push_back(category);
				//..................................
			CategoriesInfo->CategoryGroups.push_back(CategoryGroupInfo);
		 }
		 else
		 {
			 TCategoryInfo category;
            category.Key = sqlAvailableCategories->FieldByName("Category_Key")->AsInteger;
            category.Category_Name = sqlAvailableCategories->FieldByName("Category")->AsString;
            category.Category_GL_Code = sqlAvailableCategories->FieldByName("GL_Code")->AsString;
            (*iGroup).Categories.push_back(category);
		 }
	  }
	  sqlAvailableCategories->Close();
	  Success = true;
   }
   else
   {
	  Success = true;
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableLocations(TLocationsInfo *LocationsInfo)
{
   bool Success = false;
   if (LocationsInfo)
   {
	  sqlAvailableLocations->Close();
	  for (sqlAvailableLocations->ExecQuery(); !sqlAvailableLocations->Eof; sqlAvailableLocations->Next())
	  {
		 LocationsInfo->Locations.push_back(sqlAvailableLocations->FieldByName("Name")->AsString);
	  }
	  sqlAvailableLocations->Close();
	  Success = true;
   }
   else
   {
	  Success = true;
   }
   return Success;
}

// ---------------------------------------------------------------------------
int TMenuLoadDB::Count()
{
   if (sqlMenu->Open)
   {
	  return fCount;
   }
   else
   {
	  return 0;
   }
}

// ---------------------------------------------------------------------------
int TMenuLoadDB::Position()
{
   if (sqlMenu->Open)
   {
	  return fPosition;
   }
   else
   {
	  return 0;
   }
}

// ---------------------------------------------------------------------------
void TMenuLoadDB::Next()
{
   if (sqlMenu->Open)
   {
	  sqlMenu->Next();
	  if (!sqlMenu->Eof)
	  {
		 fPosition++;
	  }
   }
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetNextCourse(Menu::TCourseInfo *CourseInfo)
{
   bool Success = false;
   if (sqlMenu->Open)
   {
	  while (sqlMenu->FieldByName("Course_Key")->AsInteger == fLastCourse && !sqlMenu->Eof)
	  {
		 Next();
	  }
	  if (!sqlMenu->Eof && !sqlMenu->FieldByName("Course_Key")->IsNull)
	  {
		 fLastCourse = sqlMenu->FieldByName("Course_Key")->AsInteger;
		 fLastItem = -1;
		 fLastItemSize = -1;

		 CourseInfo->Key = sqlMenu->FieldByName("Course_Key")->AsInteger;
		 CourseInfo->Course_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Course_Name")->AsString);
		 CourseInfo->Course_Kitchen_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Course_Kitchen_Name")->AsString);
         CourseInfo->Course_Handheld_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Course_Handheld_Name")->AsString);
         CourseInfo->Course_Receipt_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Course_Receipt_Name")->AsString);
		 CourseInfo->View_Location = static_cast <TLocation> (sqlMenu->FieldByName("View_Location")->AsInteger);
		 CourseInfo->Course_ID = sqlMenu->FieldByName("Course_ID")->AsInteger;
		 CourseInfo->ServingCourse_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("ServingCourse_Name")->AsString);
		 CourseInfo->ServingCourses_Key = sqlMenu->FieldByName("ServingCourses_Key")->AsInteger;

		 CourseInfo->No_Default_Serving_Course = DB_Utilities::StrToBoolean(sqlMenu->FieldByName("No_Default_Serving_Course")->AsString);

		 if (CourseInfo->View_Location & lcPalm)
		 {
			CourseInfo->PalmID = fPalmID++;
		 }
		 else
		 {
			CourseInfo->PalmID = 0;
		 }

		 CourseInfo->Options.clear();

		 if (sqlCourseOptions->Open && !sqlCourseOptions->Eof)
		 {
			while (sqlCourseOptions->FieldByName("Course_Key")->AsInteger == CourseInfo->Key && !sqlCourseOptions->Eof)
			{
			   TCourseOptionInfo CourseOptionInfo;

			   CourseOptionInfo.Key = sqlCourseOptions->FieldByName("Options_Key")->AsInteger;
			   CourseOptionInfo.Option_Name = UTF8ToUnicodeString((AnsiString)sqlCourseOptions->FieldByName("Option_Name")->AsString);
			   CourseOptionInfo.Option_Kitchen_Name = UTF8ToUnicodeString((AnsiString)sqlCourseOptions->FieldByName("Option_Kitchen_Name")->AsString);
               CourseOptionInfo.Option_Handheld_Name = UTF8ToUnicodeString((AnsiString)sqlCourseOptions->FieldByName("Option_Handheld_Name")->AsString);
               CourseOptionInfo.Option_Receipt_Name = UTF8ToUnicodeString((AnsiString)sqlCourseOptions->FieldByName("Option_Receipt_Name")->AsString);
			   CourseOptionInfo.Option_ID = sqlCourseOptions->FieldByName("Option_ID")->AsInteger;
			   CourseOptionInfo.Forced_Mask = sqlCourseOptions->FieldByName("Forced_Mask")->AsInteger;
			   CourseOptionInfo.Flags = sqlCourseOptions->FieldByName("Flags")->AsInteger;
			   CourseOptionInfo.GroupNumber = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x0F);
			   CourseOptionInfo.Max_Select = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x10); //0x10
			   CourseOptionInfo.PlusOption = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x20);          //0x20
			   CourseOptionInfo.MinusOption = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x40);         //0x40
			   CourseOptionInfo.Allow_Skip = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x80);          //0x80
			   CourseOptionInfo.Print_Underlined = (sqlCourseOptions->FieldByName("Print_Underlined")->AsString == "T");
			   CourseOptionInfo.Print_Bold = (sqlCourseOptions->FieldByName("Print_Bold")->AsString == "T");
			   CourseOptionInfo.Print_Colour = TColor(sqlCourseOptions->FieldByName("Print_Colour")->AsInteger);
			   CourseOptionInfo.Print_Font = sqlCourseOptions->FieldByName("Print_Font")->AsInteger;
			   CourseOptionInfo.Print_Double_Width = (sqlCourseOptions->FieldByName("Print_Double_Width")->AsString == "T");
			   CourseOptionInfo.Print_Double_Height = (sqlCourseOptions->FieldByName("Print_Double_Height")->AsString == "T");
               CourseOptionInfo.Option_order = sqlCourseOptions->FieldByName("OPTION_ORDER")->AsInteger;

			   CourseInfo->Options.push_back(CourseOptionInfo);

			   sqlCourseOptions->Next();
			}
		 }
		 Success = true;
	  }
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetNextItem(Menu::TItemInfo *ItemInfo)
{
   bool Success = false;
   if (sqlMenu->Open)
   {
      while (sqlMenu->FieldByName("Item_Key")->AsInteger == fLastItem && !sqlMenu->Eof)
      {
          Next();
      }

      if (sqlMenu->FieldByName("Course_Key")->AsInteger == fLastCourse && !sqlMenu->Eof)
	  {
		 if (!sqlMenu->Eof && !sqlMenu->FieldByName("Item_Key")->IsNull)
		 {
			fLastItem = sqlMenu->FieldByName("Item_Key")->AsInteger;
			fLastItemSize = -1;

			ItemInfo->Key = sqlMenu->FieldByName("Item_Key")->AsInteger;
            ItemInfo->IAO = sqlMenu->FieldByName("IAO")->AsInteger;
            ItemInfo->Item_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Item_Name")->AsString);
			ItemInfo->Item_Kitchen_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Item_Kitchen_Name")->AsString);
			ItemInfo->Button_Colour = TColor(sqlMenu->FieldByName("Button_Colour")->AsInteger);
			ItemInfo->Exclusively_As_Side = (sqlMenu->FieldByName("Exclusively_As_Side")->AsString == "T");
			ItemInfo->Enabled = (sqlMenu->FieldByName("Enabled")->AsString == "T");
			ItemInfo->Item_ID = sqlMenu->FieldByName("Item_ID")->AsInteger;
			ItemInfo->Print_Chit = (sqlMenu->FieldByName("Print_Chit")->AsString == "T");
			ItemInfo->Display_Sizes = (sqlMenu->FieldByName("Display_Sizes")->AsString == "T");
			// cww
			ItemInfo->Print_Underlined = (sqlMenu->FieldByName("Print_Underlined")->AsString == "T");
			ItemInfo->Print_Bold = (sqlMenu->FieldByName("Print_Bold")->AsString == "T");
			ItemInfo->Print_Colour = TColor(sqlMenu->FieldByName("Print_Colour")->AsInteger);
			ItemInfo->Print_Font = sqlMenu->FieldByName("Print_Font")->AsInteger;
			ItemInfo->Print_Double_Width = (sqlMenu->FieldByName("Print_Double_Width")->AsString == "T");
			ItemInfo->Print_Double_Height = (sqlMenu->FieldByName("Print_Double_Height")->AsString == "T");

			if (sqlMenu->FieldByName("View_Location")->AsInteger & lcPalm)
			{
			   ItemInfo->PalmID = fPalmID++;
			}
			else
			{
			   ItemInfo->PalmID = 0;
			}
			ItemInfo->Sides.clear();

			if (sqlItemSides->Open && !sqlItemSides->Eof)
			{
			   while (sqlItemSides->FieldByName("Master_Item_Key")->AsInteger == ItemInfo->Key && !sqlItemSides->Eof)
			   {
				  TItemSideInfo ItemSideInfo;

				  ItemSideInfo.Master_Item_Key = sqlItemSides->FieldByName("Master_Item_Key")->AsInteger;
				  ItemSideInfo.Course_Name = sqlItemSides->FieldByName("Course_Name")->AsString;
				  ItemSideInfo.Item_Key = sqlItemSides->FieldByName("Item_Key")->AsInteger;
				  // ItemSideInfo.IOO					= sqlItemSides->FieldByName("IOO")->AsInteger;
				  ItemSideInfo.Item_Name = UTF8ToUnicodeString((AnsiString)sqlItemSides->FieldByName("Item_Name")->AsString);
				  ItemSideInfo.Group_Number = sqlItemSides->FieldByName("Group_Number")->AsInteger;
				  ItemSideInfo.Max_Select = sqlItemSides->FieldByName("Max_Select")->AsInteger;
				  ItemSideInfo.Allow_Skip = (sqlItemSides->FieldByName("Allow_Skip")->AsString == "T");

				  ItemInfo->Sides.push_back(ItemSideInfo);

				  sqlItemSides->Next();
			   }
			}
			Success = true;
		 }
	  }
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetNextItemSize(Menu::TItemSizeInfo *ItemSizeInfo)
{
   bool Success = false;
   if (sqlMenu->Open)
   {
	  if (sqlMenu->FieldByName("Item_Key")->AsInteger == fLastItem)
	  {
		 while (sqlMenu->FieldByName("ItemSize_Key")->AsInteger == fLastItemSize && !sqlMenu->Eof)
		 {
			Next();
		 }
		 if (!sqlMenu->Eof && !sqlMenu->FieldByName("ItemSize_Key")->IsNull)
		 {
			fLastItemSize = sqlMenu->FieldByName("ItemSize_Key")->AsInteger;

			ItemSizeInfo->Key = sqlMenu->FieldByName("ItemSize_Key")->AsInteger;
			ItemSizeInfo->Size_ID = sqlMenu->FieldByName("Size_ID")->AsInteger;
			ItemSizeInfo->Size_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Size_Name")->AsString);
			ItemSizeInfo->Size_Kitchen_Name = UTF8ToUnicodeString((AnsiString)sqlMenu->FieldByName("Size_Kitchen_Name")->AsString);
			ItemSizeInfo->Free = (sqlMenu->FieldByName("Free")->AsString == "T");
			ItemSizeInfo->Price = sqlMenu->FieldByName("Price")->AsCurrency;
            ItemSizeInfo->MaxRetailPrice = sqlMenu->FieldByName("MaxRetailPrice")->AsCurrency;
			ItemSizeInfo->Special_Price = sqlMenu->FieldByName("Special_Price")->AsCurrency;
            ItemSizeInfo->GST_Percent = sqlMenu->FieldByName("GST_Percent")->AsDouble;
			ItemSizeInfo->Cost = sqlMenu->FieldByName("Cost")->AsCurrency;
			ItemSizeInfo->Cost_GST_Percent = sqlMenu->FieldByName("Cost_GST_Percent")->AsDouble;
			ItemSizeInfo->Points_Percent = sqlMenu->FieldByName("Points_Percent")->AsDouble;
			ItemSizeInfo->Barcode = sqlMenu->FieldByName("Barcode")->AsString;

			ItemSizeInfo->Category = sqlMenu->FieldByName("Category")->AsString;
			ItemSizeInfo->CategoryKey = sqlMenu->FieldByName("Category_Key")->AsInteger;
            ItemSizeInfo->GLCode = sqlMenu->FieldByName("GL_CODE")->AsString;
			//ItemSizeInfo->Category_Group_Name = sqlMenu->FieldByName("Category_Group_Name")->AsString;
			//ItemSizeInfo->Category_Group_Key = sqlMenu->FieldByName("CategoryGroups_Key")->AsInteger;

			ItemSizeInfo->Setmenu_Mask = sqlMenu->FieldByName("Setmenu_Mask")->AsInteger;
			ItemSizeInfo->Available_As_Standard = (sqlMenu->FieldByName("Available_As_Standard")->AsString == "T");

			ItemSizeInfo->Mem_Sale_Count = sqlMenu->FieldByName("Mem_Sale_Count")->AsInteger;
			ItemSizeInfo->Mem_Discount_Percent = sqlMenu->FieldByName("Mem_Discount_Percent")->AsDouble;
			ItemSizeInfo->Loc_Sale_Count = sqlMenu->FieldByName("Loc_Sale_Count")->AsInteger;
			ItemSizeInfo->Loc_Discount_Percent = sqlMenu->FieldByName("Loc_Discount_Percent")->AsDouble;

			ItemSizeInfo->ThirdPartyCode = sqlMenu->FieldByName("ThirdPartyCode")->AsString;
			ItemSizeInfo->ThirdPartyCodes_Key = sqlMenu->FieldByName("ThirdPartyCodes_Key")->AsInteger;
			ItemSizeInfo->Tare_Weight.SetWeightIn_Kg(sqlMenu->FieldByName("Tare_Weight")->AsCurrency);
			ItemSizeInfo->PLU = sqlMenu->FieldByName("PLU")->AsInteger;
            ItemSizeInfo->IsInvisible = sqlMenu->FieldByName("IsInvisible")->AsString == "T" ? true : false;
#define QFBN(q, f, t) q->FieldByName(f)->t
			ItemSizeInfo->enabled =
			  QFBN(sqlMenu, "enabled", AsString) == "T";
                        ItemSizeInfo->available_quantity =
                          QFBN(sqlMenu, "available_quantity", AsCurrency);
                        ItemSizeInfo->default_quantity =
                          QFBN(sqlMenu, "default_quantity", AsCurrency);
                        ItemSizeInfo->warning_quantity =
                          QFBN(sqlMenu, "warning_quantity", AsCurrency);
                        ItemSizeInfo->disable_when_count_reaches_zero =
                          QFBN(sqlMenu,
                               "disable_when_count_reaches_zero",
			       AsInteger)
                          == 1;

                        ItemSizeInfo->CanBePaidForUsingPoints =
                          QFBN(sqlMenu,
                               "can_be_paid_for_using_points",
                               AsInteger);

			ItemSizeInfo->DefaultPatronCount =
			  QFBN(sqlMenu,
			       "default_patron_count",
			       AsInteger);
			ItemSizeInfo->PriceForPoints = QFBN(sqlMenu, "PRICE_FOR_POINTS", AsCurrency);
#undef QFBN

			ItemSizeInfo->Categories.clear();
			ItemSizeInfo->Recipes.clear();
            ItemSizeInfo->TaxProfiles.clear();

			if (sqlSizeCategories->Open && !sqlSizeCategories->Eof)
			{
			   while (sqlSizeCategories->FieldByName("ItemSize_Key")->AsInteger == ItemSizeInfo->Key && !sqlSizeCategories->Eof)
			   {
				  TSizeCategoryInfo SizeCategoryInfo;

				  SizeCategoryInfo.Category = sqlSizeCategories->FieldByName("Category")->AsString;
				  SizeCategoryInfo.Key = sqlSizeCategories->FieldByName("Category_Key")->AsInteger;
                  SizeCategoryInfo.GLCode = sqlSizeCategories->FieldByName("GL_CODE")->AsString;
				  ItemSizeInfo->Categories.push_back(SizeCategoryInfo);

				  sqlSizeCategories->Next();
			   }
			}
			while (sqlMenu->FieldByName("ItemSize_Key")->AsInteger == fLastItemSize && !sqlMenu->Eof)
			{
			   if (!sqlMenu->FieldByName("Stock_Code")->IsNull)
			   {
				  TSizeRecipeInfo SizeRecipeInfo;

				  SizeRecipeInfo.Stock_Code = sqlMenu->FieldByName("Stock_Code")->AsString;
				  SizeRecipeInfo.Qty = sqlMenu->FieldByName("Qty")->AsDouble;
				  SizeRecipeInfo.Stock_GST_Percent = sqlMenu->FieldByName("Cost_GST_Percent")->AsCurrency;
				  SizeRecipeInfo.Stock_Unit_Cost = sqlMenu->FieldByName("Stock_Unit_Cost")->AsCurrency;
				  SizeRecipeInfo.Stock_Location = sqlMenu->FieldByName("Stock_Location")->AsString;

				  ItemSizeInfo->Recipes.push_back(SizeRecipeInfo);
			   }
			   Next();
			}
            LoadItemSizeTaxProfiles(ItemSizeInfo);
            LoadItemSizePriceLevels(ItemSizeInfo);
			Success = true;
		 }
	  }
   }
   return Success;
}

// ---------------------------------------------------------------------------
int TMenuLoadDB::GetMaxPalmID()
{
   return fPalmID;
}
// ---------------------------------------------------------------------------

bool TMenuLoadDB::GetForcedItemOptionKeysWithGroupNumber(
									 unsigned int inGroupNumber,
									 unsigned int inItemKey,
						std::vector<TNameAndKey>& inForcedItemOptionKeys )
{
	bool Success = true;

    //::::::::::::::::::::::::::::::::::::

    inForcedItemOptionKeys.clear();

    Database::TcpIBSQL sqlOptions( new TIBSQL( NULL ) );

    DBControl.RegisterQuery( sqlOptions );

	sqlOptions->SQL->Text = ForcedItemOptionsSQL;
    DBControl.StartTransaction();

	sqlOptions->ParamByName( "item_key" )->AsInteger = inItemKey;
    sqlOptions->ExecQuery();

    sqlOptions->Next();
    while( !sqlOptions->Eof )
    {
		if( ( sqlOptions->FieldByName( "flags" )->AsInteger & 0x0F ) == inGroupNumber )
		{
			TNameAndKey nameAndKey(
							sqlOptions->FieldByName( "option_key" )->AsInteger,
							sqlOptions->FieldByName( "option_name" )->AsString );

			inForcedItemOptionKeys.push_back( nameAndKey );
		}
        sqlOptions->Next();
    }
    sqlOptions->Close();

	DBControl.Commit();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetForcedItemOptionKeys( unsigned int inItemKey, std::vector<TNameAndKey> &inForcedItemOptionKeys )
{
	bool Success = true;

	//::::::::::::::::::::::::::::::::::::

    inForcedItemOptionKeys.clear();

	Database::TcpIBSQL sqlOptions( new TIBSQL( NULL ) );

	DBControl.RegisterQuery( sqlOptions );

	sqlOptions->SQL->Text = AllForcedItemOptionsSQL;
    DBControl.StartTransaction();

    sqlOptions->ParamByName( "item_key" )->AsInteger = inItemKey;
    sqlOptions->ExecQuery();

    sqlOptions->Next();
    while( !sqlOptions->Eof )
    {
		TNameAndKey nameAndKey(
						sqlOptions->FieldByName( "option_key" )->AsInteger,
						sqlOptions->FieldByName( "option_name" )->AsString );

        inForcedItemOptionKeys.push_back( nameAndKey );
        sqlOptions->Next();
    }
    sqlOptions->Close();

	DBControl.Commit();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
// ---------------------------------------------------------------------------

void TMenuLoadDB::LoadItemSizePriceLevels(TItemSizeInfo *ItemSizeInfo)
{

    int         pricelevelKey;
    Currency price;
    AnsiString name;

    sqlItemSizePriceLevels->Close();
    sqlItemSizePriceLevels->ParamByName("ITEMSIZE_KEY")->AsInteger = ItemSizeInfo->Key;
    sqlItemSizePriceLevels->ExecQuery();


    while (!sqlItemSizePriceLevels->Eof)
    {
        TItemSizePriceLevel  priceLevel;

        priceLevel.PriceLevelKey= sqlItemSizePriceLevels->FieldByName("PRICELEVEL_KEY")->AsInteger;
        priceLevel.Price=sqlItemSizePriceLevels->FieldByName("PRICE")->AsCurrency;
        priceLevel.Name= sqlItemSizePriceLevels->FieldByName("PRICELEVEL_NAME")->AsString;

        ItemSizeInfo->PriceLevels.insert(std::pair<int ,TItemSizePriceLevel> (priceLevel.PriceLevelKey,priceLevel) );
        sqlItemSizePriceLevels->Next();

    }
}

