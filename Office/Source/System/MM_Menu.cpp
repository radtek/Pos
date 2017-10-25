//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_Menu.h"
#include "Utilities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
using namespace Menu;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	NameAndKey
//---------------------------------------------------------------------------
TNameAndKey::TNameAndKey( __int32 inKey, WideString inName )
{
    Name = inName;
    Key  = inKey;
}

//---------------------------------------------------------------------------
TMenuLoadDB::TMenuLoadDB(TIBDatabase *MMDatabase, int MenuKey) : dbTransaction(MMDatabase),
	sqlMenu(					new TIBSQL( NULL ) ),
	sqlMenuCount(				new TIBSQL( NULL ) ),
	sqlAvailableSizes(		    new TIBSQL( NULL ) ),
	sqlAvailableCategories(	    new TIBSQL( NULL ) ),
	sqlAvailableLocations(	    new TIBSQL( NULL ) ),
	sqlItemSides(				new TIBSQL( NULL ) ),
	sqlCourseOptions(			new TIBSQL( NULL ) ),
	sqlSizeCategories(		    new TIBSQL( NULL ) ),
	sqlAvailableServingCourses( new TIBSQL( NULL ) ),
    sqlOptions(                 new TIBSQL( NULL ) )
{
	this->dbTransaction.RegisterQuery( sqlMenu );
	this->dbTransaction.RegisterQuery( sqlMenuCount );
	this->dbTransaction.RegisterQuery( sqlAvailableSizes );
	this->dbTransaction.RegisterQuery( sqlAvailableCategories );
	this->dbTransaction.RegisterQuery( sqlAvailableLocations );
	this->dbTransaction.RegisterQuery( sqlItemSides );
	this->dbTransaction.RegisterQuery( sqlCourseOptions );
	this->dbTransaction.RegisterQuery( sqlSizeCategories );
	this->dbTransaction.RegisterQuery( sqlAvailableServingCourses );
    this->dbTransaction.RegisterQuery( sqlOptions );

	fMenuKey = MenuKey;

	sqlMenu->SQL->Text					  = MenuSQL;
	sqlMenuCount->SQL->Text				  = MenuCountSQL;
	sqlAvailableSizes->SQL->Text		  = AvailableSizesSQL;
	sqlAvailableCategories->SQL->Text	  = AvailableCategoriesSQL;
	sqlAvailableLocations->SQL->Text	  = AvailableLocationsSQL;
	sqlItemSides->SQL->Text				  = ItemSidesSQL;
	sqlCourseOptions->SQL->Text			  = CourseOptionsSQL;
	sqlSizeCategories->SQL->Text		  = SizeCategoriesSQL;
	sqlAvailableServingCourses->SQL->Text = AvailableServingCoursesSQL;
	sqlOptions->SQL->Text                 = ForcedItemOptionsSQL;

	dbTransaction.StartTransaction();
}
//---------------------------------------------------------------------------
TMenuLoadDB::~TMenuLoadDB()
{
	this->dbTransaction.CommitTransaction();
}
//---------------------------------------------------------------------------
void TMenuLoadDB::EnumMenus(TMenusInfo &MenusInfo)
{
	Database::TcpIBSQL sqlEnumMenu(new TIBSQL(NULL));

	this->dbTransaction.RegisterQuery(sqlEnumMenu);

	sqlEnumMenu->SQL->Text		= "Select Menu_Key, Menu_Name, Menu_Type, Palmable From Menu Order By Menu_Name";

	MenusInfo.clear();

	sqlEnumMenu->Close();
	for (sqlEnumMenu->ExecQuery(); !sqlEnumMenu->Eof; sqlEnumMenu->Next())
	{
		TMenuInfo MenuInfo;
		MenuInfo.Key		= sqlEnumMenu->FieldByName("Menu_Key")->AsInteger;
		MenuInfo.Menu_Name	= sqlEnumMenu->FieldByName("Menu_Name")->AsString;
		MenuInfo.Menu_Type	= static_cast<TMenuType>(sqlEnumMenu->FieldByName("Menu_Type")->AsInteger);
		MenuInfo.Palmable	= sqlEnumMenu->FieldByName("Palmable")->AsString == "T";
		MenusInfo.push_back(MenuInfo);
	}
}
//---------------------------------------------------------------------------
bool TMenuLoadDB::GetMenu(
		Menu::TMenuInfo	*MenuInfo,
		Menu::TSizesInfo	*SizesInfo,
		TCategoriesInfo	*CategoriesInfo,
		TLocationsInfo		*LocationsInfo,
		TServingCoursesInfo *ServingCoursesInfo)
{
	sqlMenu->Close();
	sqlMenu->ParamByName("Menu_Key")->AsInteger				= fMenuKey;
	sqlMenu->ExecQuery();

	sqlMenuCount->Close();
	sqlMenuCount->ParamByName("Menu_Key")->AsInteger		= fMenuKey;
	sqlMenuCount->ExecQuery();
	fCount = sqlMenuCount->Fields[0]->AsInteger;
	sqlMenuCount->Close();

	sqlCourseOptions->Close();
	sqlCourseOptions->ParamByName("Menu_Key")->AsInteger	= fMenuKey;
	sqlCourseOptions->ExecQuery();

	sqlItemSides->Close();
	sqlItemSides->ParamByName("Menu_Key")->AsInteger		= fMenuKey;
	sqlItemSides->ExecQuery();

	sqlSizeCategories->Close();
	sqlSizeCategories->ParamByName("Menu_Key")->AsInteger	= fMenuKey;
	sqlSizeCategories->ExecQuery();

	fLastCourse		= -1;
	fLastItem		= -1;
	fLastItemSize	= -1;

	fPosition		= 0;
	bool Success	= true;
	if (!sqlMenu->Eof)
	{
		MenuInfo->Key    	= sqlMenu->FieldByName("Menu_Key")->AsInteger;
		MenuInfo->Menu_Name	= sqlMenu->FieldByName("Menu_Name")->AsString;
		MenuInfo->Menu_Type	= static_cast<TMenuType>(sqlMenu->FieldByName("Menu_Type")->AsInteger);
		MenuInfo->Palmable	= sqlMenu->FieldByName("Palmable")->AsString=="T";

		if (MenuInfo->Menu_Type == mtBeverageMenu)
		{
			fPalmID				= 0x4001;
		}
		else
		{
			fPalmID				= 0x0001;
		}
		Success = true;
	}
	else
	{
		MenuInfo->Key	    = 0;
		MenuInfo->Menu_Name	= "";
		MenuInfo->Menu_Type	= TMenuType(0);
		MenuInfo->Palmable	= false;

		Success = false;
	}
	return Success &&
		GetAvailableSizes(SizesInfo) &&
		GetAvailableCategories(CategoriesInfo) &&
		GetAvailableLocations(LocationsInfo) &&
		GetAvailableServingCourses(ServingCoursesInfo);
}
//---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableSizes(TSizesInfo *SizesInfo)
{
	bool Success = false;

	if (SizesInfo)
	{
		unsigned char PalmSizeID = 1;

		sqlAvailableSizes->Close();
		sqlAvailableSizes->ParamByName("Menu_Key")->AsInteger	= fMenuKey;
		for (sqlAvailableSizes->ExecQuery(); !sqlAvailableSizes->Eof; sqlAvailableSizes->Next())
		{
			TSizeInfo SizeInfo;

            SizeInfo.Key		     	= sqlAvailableSizes->FieldByName("Sizes_Key")->AsInteger;

			SizeInfo.Size_Name			= sqlAvailableSizes->FieldByName("Size_Name")->AsString; 
			SizeInfo.Size_Kitchen_Name	= sqlAvailableSizes->FieldByName("Size_Kitchen_Name")->AsString;

            SizeInfo.Size_Handheld_Name = sqlAvailableSizes->FieldByName("Handheld_Name")->AsString;
            SizeInfo.Size_Receipt_Name = sqlAvailableSizes->FieldByName("Receipt_Name")->AsString;
			SizeInfo.Weighed		   = sqlAvailableSizes->FieldByName("Weighted_Size")->AsString=="T";
			SizeInfo.Size_ID		   = sqlAvailableSizes->FieldByName("Size_ID")->AsInteger;
			SizeInfo.PalmID			   = PalmSizeID++;

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
//---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo )
{
	// also called from loadfromfile in versions prior to 3.4
	bool Success = false;
	if (ServingCoursesInfo)
	{
		// 2 passes needed as you need to get the Serving Courses for the
		// Menu and then the rest of the ones from the Database where they are Deleted = F
		TVarRec v[] = { FirstFrom,FirstWhere };
		sqlAvailableServingCourses->SQL->Text = Format(AvailableServingCoursesSQL,v, ARRAYSIZE(v) - 1);
		sqlAvailableServingCourses->ParamByName("Menu_Key")->AsInteger	= fMenuKey;
		LoadServingCourses( ServingCoursesInfo,true );

      // this pass is for the new menus, where there are no serving courses in it, we load all of em from database
      if( ServingCoursesInfo->ServingCourses.size() == 0 )
      {
		   TVarRec v1[] = { SecondFrom,SecondWhere };
		   sqlAvailableServingCourses->SQL->Text = Format(AvailableServingCoursesSQL,v1, ARRAYSIZE(v1) - 1);
		   LoadServingCourses( ServingCoursesInfo,true );
      }

		Success = true;
	}
	return Success;
}
//---------------------------------------------------------------------------
void TMenuLoadDB::LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo, bool Enabled )
{
	bool Found = false;
	// 2nd pass and there are no Serving Courses in the Menu - new Menu so make them all Enabled
//	if ( sqlAvailableServingCourses->SQL->Text.Pos(SecondWhere) > 0 && ServingCoursesInfo->ServingCourses.size() == 0)
//	{
//		Enabled = true;
//	}
	for (sqlAvailableServingCourses->ExecQuery(); !sqlAvailableServingCourses->Eof; sqlAvailableServingCourses->Next())
	{
		// weed out the duplicate entries from 2 passes of different sql's
		Found = false;
		for (unsigned i=0; i<ServingCoursesInfo->ServingCourses.size(); i++)
		{
			if (sqlAvailableServingCourses->FieldByName("ServingCourse_Name")->AsString == ServingCoursesInfo->ServingCourses[i].ServingCourse_Name)
				Found = true;
		}
		if (!Found)
		{
			TServingCourseInfo ServingCourseInfo;
			ServingCourseInfo.Key				         = sqlAvailableServingCourses->FieldByName("ServingCourses_Key")->AsInteger;
			ServingCourseInfo.ServingCourse_Name		 = sqlAvailableServingCourses->FieldByName("ServingCourse_Name")->AsString;
			ServingCourseInfo.ServingCourse_Kitchen_Name = sqlAvailableServingCourses->FieldByName("ServingCourse_Kitchen_Name")->AsString;
			ServingCourseInfo.Colour					 = (TColor)sqlAvailableServingCourses->FieldByName("Colour")->AsInteger;
			ServingCourseInfo.Enabled					 = Enabled; //sqlAvailableServingCourses->FieldByName("Deleted")->AsString == "T";
			ServingCourseInfo.Deleted 					 = false;
			ServingCourseInfo.Selectable     			 = dmUtilities->StrToBoolean(sqlAvailableServingCourses->FieldByName("Selectable")->AsString);
            ServingCourseInfo.DisplayOrder          = sqlAvailableServingCourses->FieldByName("Display_Order")->AsInteger;
			ServingCoursesInfo->ServingCourses.push_back(ServingCourseInfo);
		}
	}
	sqlAvailableServingCourses->Close();
}
//---------------------------------------------------------------------------
bool TMenuLoadDB::GetAvailableCategories(TCategoriesInfo *CategoriesInfo)
{
	bool Success = false;

	if (CategoriesInfo)
	{
		sqlAvailableCategories->Close();
		for (sqlAvailableCategories->ExecQuery(); !sqlAvailableCategories->Eof; sqlAvailableCategories->Next())
		{
			bool FoundGroup = false;
			std::vector<TCategoryGroupInfo>::iterator iGroup;
			for (iGroup=CategoriesInfo->CategoryGroups.begin(); iGroup!=CategoriesInfo->CategoryGroups.end(); iGroup++)
			{
				if ((*iGroup).Category_Group_Name == UTF8Decode( sqlAvailableCategories->FieldByName("Category_Group_Name")->AsString ))
				{
					FoundGroup	= true;
					break;
				}
			}
			if (!FoundGroup)
			{
				TCategoryGroupInfo CategoryGroupInfo;

                CategoryGroupInfo.Key                 = sqlAvailableCategories->FieldByName("CategoryGroups_Key")->AsInteger;
                CategoryGroupInfo.Category_Group_Name = sqlAvailableCategories->FieldByName("Category_Group_Name")->AsString;

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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
bool TMenuLoadDB::GetNextCourse(Menu::TCourseInfo *CourseInfo)
{
	bool Success = false;
	if (sqlMenu->Open)
	{
		while (sqlMenu->FieldByName("Course_Key")->AsInteger == fLastCourse && !sqlMenu->Eof)
		{
			Next();
		}
		if (!sqlMenu->Eof && sqlMenu->FieldByName("Course_Key")->AsInteger != 0)
		{
			fLastCourse		= sqlMenu->FieldByName("Course_Key")->AsInteger;
			fLastItem		= -1;
			fLastItemSize	= -1;
			CourseInfo->Key				     = sqlMenu->FieldByName("Course_Key")->AsInteger;
			CourseInfo->Course_Name		     = sqlMenu->FieldByName("Course_Name")->AsString;
			CourseInfo->Course_Kitchen_Name	 = sqlMenu->FieldByName("Course_Kitchen_Name")->AsString;
			CourseInfo->Course_Handheld_Name = sqlMenu->FieldByName("Course_Handheld_Name")->AsString;
			CourseInfo->Course_Receipt_Name  = sqlMenu->FieldByName("Course_Receipt_Name")->AsString;
			CourseInfo->View_Location		 = static_cast<TLocation>(sqlMenu->FieldByName("View_Location")->AsInteger);
			CourseInfo->Course_ID			 = sqlMenu->FieldByName("Course_ID")->AsInteger;
            CourseInfo->ServingCourse_Key	 = sqlMenu->FieldByName("ServingCourses_Key")->AsInteger;
			CourseInfo->ServingCourse_Name	 = sqlMenu->FieldByName("ServingCourse_Name")->AsString;
			CourseInfo->No_Default_Serving_Course = dmUtilities->StrToBoolean(sqlMenu->FieldByName("No_Default_Serving_Course")->AsString);
			if (CourseInfo->View_Location & lcPalm)
			{
				CourseInfo->PalmID				= fPalmID++;
			}
			else
			{
				CourseInfo->PalmID				= 0;
			}

			CourseInfo->Options.clear();

			if (sqlCourseOptions->Open && !sqlCourseOptions->Eof)
			{
				while (sqlCourseOptions->FieldByName("Course_Key")->AsInteger == CourseInfo->Key && !sqlCourseOptions->Eof)
				{
               TCourseOptionInfo CourseOptionInfo;

               CourseOptionInfo.Key		         = sqlCourseOptions->FieldByName("Options_Key")->AsInteger;

               CourseOptionInfo.Option_Name		 = sqlCourseOptions->FieldByName("Option_Name")->AsString;
               CourseOptionInfo.Option_Kitchen_Name = sqlCourseOptions->FieldByName("Option_Kitchen_Name")->AsString;
               CourseOptionInfo.Option_ID			 = sqlCourseOptions->FieldByName("Option_ID")->AsInteger;
               CourseOptionInfo.Forced_Mask		 = sqlCourseOptions->FieldByName("Forced_Mask")->AsInteger;
               CourseOptionInfo.Flags				 = sqlCourseOptions->FieldByName("Flags")->AsInteger;
               CourseOptionInfo.GroupNumber		 = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x0F);
			   CourseOptionInfo.Max_Select			 = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x10);
               CourseOptionInfo.PlusOption			 = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x20);
               CourseOptionInfo.MinusOption		    = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x40);
               CourseOptionInfo.Allow_Skip			 = (sqlCourseOptions->FieldByName("Flags")->AsInteger & 0x80);

               CourseOptionInfo.Print_Underlined	 = (sqlCourseOptions->FieldByName("Print_Underlined")->AsString == "T");
               CourseOptionInfo.Print_Bold			 = (sqlCourseOptions->FieldByName("Print_Bold")->AsString == "T");
               CourseOptionInfo.Print_Colour 		 = TColor(sqlCourseOptions->FieldByName("Print_Colour")->AsInteger);
               CourseOptionInfo.Print_Font			 = sqlCourseOptions->FieldByName("Print_Font")->AsInteger;
               CourseOptionInfo.Print_Double_Width	 = (sqlCourseOptions->FieldByName("Print_Double_Width")->AsString == "T");
               CourseOptionInfo.Print_Double_Height = (sqlCourseOptions->FieldByName("Print_Double_Height")->AsString == "T");

               CourseOptionInfo.Option_Handheld_Name = sqlCourseOptions->FieldByName("Option_Handheld_Name")->AsString;
               CourseOptionInfo.Option_Receipt_Name = sqlCourseOptions->FieldByName("Option_Receipt_Name")->AsString;

               CourseInfo->Options.push_back(CourseOptionInfo);

               sqlCourseOptions->Next();
				}
			}
			Success = true;
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
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
			if (!sqlMenu->Eof && sqlMenu->FieldByName("Item_Key")->AsInteger != 0)
			{
				fLastItem		= sqlMenu->FieldByName("Item_Key")->AsInteger;
				fLastItemSize	= -1;

				ItemInfo->Key			     = sqlMenu->FieldByName("Item_Key")->AsInteger;
				ItemInfo->Item_Name		     = sqlMenu->FieldByName("Item_Name")->AsString;

				ItemInfo->Item_Kitchen_Name	 = sqlMenu->FieldByName("Item_Kitchen_Name")->AsString;

                ItemInfo->Item_Handheld_Name = sqlMenu->FieldByName("Item_Handheld_Name")->AsString;
                ItemInfo->Item_Receipt_Name  = sqlMenu->FieldByName("Item_Receipt_Name")->AsString;

				ItemInfo->Button_Colour			= TColor(sqlMenu->FieldByName("Button_Colour")->AsInteger);
				ItemInfo->Exclusively_As_Side	= (sqlMenu->FieldByName("Exclusively_As_Side")->AsString == "T");
				ItemInfo->Enabled				= (sqlMenu->FieldByName("Enabled")->AsString == "T");
				ItemInfo->Item_ID				= sqlMenu->FieldByName("Item_ID")->AsInteger;
				ItemInfo->Print_Chit			= (sqlMenu->FieldByName("Print_Chit")->AsString == "T");
				ItemInfo->Display_Sizes			= (sqlMenu->FieldByName("Display_Sizes")->AsString == "T");
            // cww
				ItemInfo->Print_Underlined		= (sqlMenu->FieldByName("Print_Underlined")->AsString == "T");
				ItemInfo->Print_Bold				= (sqlMenu->FieldByName("Print_Bold")->AsString == "T");
				ItemInfo->Print_Colour 			= TColor(sqlMenu->FieldByName("Print_Colour")->AsInteger);
				ItemInfo->Print_Font				= sqlMenu->FieldByName("Print_Font")->AsInteger;
				ItemInfo->Print_Double_Width	= (sqlMenu->FieldByName("Print_Double_Width")->AsString == "T");
				ItemInfo->Print_Double_Height	= (sqlMenu->FieldByName("Print_Double_Height")->AsString == "T");

				if (sqlMenu->FieldByName("View_Location")->AsInteger & lcPalm)
				{
					ItemInfo->PalmID				= fPalmID++;
				}
				else
				{
					ItemInfo->PalmID				= 0;
				}
				ItemInfo->Sides.clear();

				if (sqlItemSides->Open && !sqlItemSides->Eof)
				{
					while (sqlItemSides->FieldByName("Master_Item_Key")->AsInteger == ItemInfo->Key && !sqlItemSides->Eof)
					{
						TItemSideInfo ItemSideInfo;

                        ItemSideInfo.Key             = sqlItemSides->FieldByName("ItemSides_Key")->AsInteger;
						ItemSideInfo.Item_Key		 = sqlItemSides->FieldByName("Item_Key")->AsInteger;
						ItemSideInfo.Master_Item_Key = sqlItemSides->FieldByName("Master_Item_Key")->AsInteger;
						ItemSideInfo.Course_Name	 = sqlItemSides->FieldByName("Course_Name")->AsString;
						//ItemSideInfo.IOO			 = sqlItemSides->FieldByName("IOO")->AsInteger;
						ItemSideInfo.Item_Name		 = sqlItemSides->FieldByName("Item_Name")->AsString;
						ItemSideInfo.Group_Number	 = sqlItemSides->FieldByName("Group_Number")->AsInteger;
						ItemSideInfo.Max_Select		 = sqlItemSides->FieldByName("Max_Select")->AsInteger;
						ItemSideInfo.Allow_Skip		 = (sqlItemSides->FieldByName("Allow_Skip")->AsString=="T");

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
//---------------------------------------------------------------------------          fdsgdfhgdfghf
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
			if (!sqlMenu->Eof && sqlMenu->FieldByName("ItemSize_Key")->AsInteger != 0)
			{
				fLastItemSize = sqlMenu->FieldByName("ItemSize_Key")->AsInteger;

				ItemSizeInfo->Key			     = sqlMenu->FieldByName("ItemSize_Key")->AsInteger;
				ItemSizeInfo->Size_Key         = sqlMenu->FieldByName("ItemSize_SizeKey")->AsInteger;
				ItemSizeInfo->Size_ID			 = sqlMenu->FieldByName("Size_ID")->AsInteger;
				ItemSizeInfo->Size_Name			 = sqlMenu->FieldByName("Size_Name")->AsString; 
				ItemSizeInfo->Size_Kitchen_Name	 = sqlMenu->FieldByName("Size_Kitchen_Name")->AsString;

			   ItemSizeInfo->Size_Handheld_Name = sqlMenu->FieldByName("ItemSize_Handheld_Name")->AsString;

			   ItemSizeInfo->Size_Receipt_Name =  sqlMenu->FieldByName("ItemSize_Receipt_Name")->AsString;

				ItemSizeInfo->Free					= ( sqlMenu->FieldByName("Free")->AsString == "T" );
				ItemSizeInfo->Price				    = sqlMenu->FieldByName("Price")->AsCurrency;
				ItemSizeInfo->MaxRetailPrice		= sqlMenu->FieldByName("MAXRETAILPRICE")->AsCurrency;
				ItemSizeInfo->Special_Price			= sqlMenu->FieldByName("Special_Price")->AsCurrency;
				ItemSizeInfo->GST_Percent			= sqlMenu->FieldByName("GST_Percent")->AsDouble;
				ItemSizeInfo->Cost					= sqlMenu->FieldByName("Cost")->AsCurrency;
				ItemSizeInfo->Cost_GST_Percent		= sqlMenu->FieldByName("Cost_GST_Percent")->AsDouble;
				ItemSizeInfo->Tare_Weight			= sqlMenu->FieldByName("Tare_Weight")->AsCurrency;
				ItemSizeInfo->Points_Percent		= sqlMenu->FieldByName("Points_Percent")->AsDouble;
				ItemSizeInfo->Barcode				= sqlMenu->FieldByName("Barcode")->AsString;
				ItemSizeInfo->CategoryKey 			= sqlMenu->FieldByName("Category_Key")->AsInteger;
                ItemSizeInfo->Category 				= sqlMenu->FieldByName("Category")->AsString;
				ItemSizeInfo->Setmenu_Mask			= sqlMenu->FieldByName("Setmenu_Mask")->AsInteger;
				ItemSizeInfo->Available_As_Standard	= (sqlMenu->FieldByName("Available_As_Standard")->AsString=="T");

				ItemSizeInfo->Mem_Sale_Count		= sqlMenu->FieldByName("Mem_Sale_Count")->AsInteger;
				ItemSizeInfo->Mem_Discount_Percent	= sqlMenu->FieldByName("Mem_Discount_Percent")->AsDouble;
				ItemSizeInfo->Loc_Sale_Count		= sqlMenu->FieldByName("Loc_Sale_Count")->AsInteger;
				ItemSizeInfo->Loc_Discount_Percent	= sqlMenu->FieldByName("Loc_Discount_Percent")->AsDouble;

				ItemSizeInfo->Third_Party_Code		= sqlMenu->FieldByName("Third_Party_Code")->AsString;
				ItemSizeInfo->ThirdPartyCodes_Key	= sqlMenu->FieldByName("ThirdPartyCodes_Key")->AsInteger;

            ItemSizeInfo->CanBePaidForUsingPoints = sqlMenu->FieldByName("can_be_paid_for_using_points")->AsInteger;

            ItemSizeInfo->AvailableQuantity = sqlMenu->FieldByName("available_quantity")->AsFloat;
            ItemSizeInfo->DefaultQuantity = sqlMenu->FieldByName("default_quantity")->AsFloat;
            ItemSizeInfo->WarningQuantity = sqlMenu->FieldByName("warning_quantity")->AsFloat;
            ItemSizeInfo->DisableWhenCountReachesZero = sqlMenu->FieldByName("disable_when_count_reaches_zero")->AsInteger;

            ItemSizeInfo->DefaultPatronCount = sqlMenu->FieldByName("default_patron_count")->AsInteger;

            ItemSizeInfo->PLU = sqlMenu->FieldByName("PLU")->AsInteger;
            ItemSizeInfo->PriceForPoints = sqlMenu->FieldByName("PRICE_FOR_POINTS")->AsInteger;
            ItemSizeInfo->RevenueCode = sqlMenu->FieldByName("REVENUECODE")->AsInteger;
            ItemSizeInfo->RevenueCodeDescription = sqlMenu->FieldByName("REVENUECODE_DESCRIPTION")->AsString;

				ItemSizeInfo->Categories.clear();
				ItemSizeInfo->Recipes.clear();

				if (sqlSizeCategories->Open && !sqlSizeCategories->Eof)
				{
					while (sqlSizeCategories->FieldByName("ItemSize_Key")->AsInteger == ItemSizeInfo->Key && !sqlSizeCategories->Eof)
					{
						TSizeCategoryInfo SizeCategoryInfo;

						SizeCategoryInfo.Key      = sqlSizeCategories->FieldByName("Category_Key")->AsInteger;
						SizeCategoryInfo.Category = sqlSizeCategories->FieldByName("Category")->AsString;

						ItemSizeInfo->Categories.push_back(SizeCategoryInfo);

						sqlSizeCategories->Next();
					}
				}

            //::::::::::::::::::::::::::::::::::::::::::
            ItemSizeInfo->TaxProfileKeys.clear();
            getTaxProfileKeysForItemSize( sqlMenu->FieldByName("ItemSize_Key")->AsInteger, &ItemSizeInfo->TaxProfileKeys );

            //::::::::::::::::::::::::::::::::::::::::::
             ItemSizeInfo->ItemSizePriceLevels.clear();
             GeTItemSizePriceLevels(sqlMenu->FieldByName("ItemSize_Key")->AsInteger , &ItemSizeInfo->ItemSizePriceLevels );
            //:::::::::::::::::::::::::::::::::::::::::::::
             ItemSizeInfo->ItemSizeTaxPercent.clear();
             GeTItemSizeTaxPercentage(sqlMenu->FieldByName("ItemSize_Key")->AsInteger , ItemSizeInfo->ItemSizeTaxPercent);


				while(sqlMenu->FieldByName("ItemSize_Key")->AsInteger == fLastItemSize && !sqlMenu->Eof)
				{
					if (!sqlMenu->FieldByName("Stock_Code")->IsNull)
					{
						TSizeRecipeInfo SizeRecipeInfo;
						SizeRecipeInfo.Key   		      = sqlMenu->FieldByName("Recipe_Key")->AsInteger;
						SizeRecipeInfo.Stock_Code		   = sqlMenu->FieldByName("Stock_Code")->AsString;
						SizeRecipeInfo.Qty				   = sqlMenu->FieldByName("Qty")->AsDouble;
						SizeRecipeInfo.Stock_GST_Percent = sqlMenu->FieldByName("Cost_GST_Percent")->AsCurrency;
						SizeRecipeInfo.Stock_Unit_Cost	= sqlMenu->FieldByName("Stock_Unit_Cost")->AsCurrency;
						SizeRecipeInfo.Stock_Location	   = sqlMenu->FieldByName("Stock_Location")->AsString;
                        ItemSizeInfo->Recipes.push_back(SizeRecipeInfo);
					}
					Next();
				}

				Success = true;
			}
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
void TMenuLoadDB::getTaxProfileKeysForItemSize( __int32 inItemSizeKey, std::set<__int32>* const outKeys )
{
   outKeys->clear();

   Database::TcpIBSQL sqlTaxProfiles( new TIBSQL( NULL ) );

   this->dbTransaction.RegisterQuery( sqlTaxProfiles );

   sqlTaxProfiles->SQL->Text = TaxProfileKeysSQL;
   sqlTaxProfiles->ParamByName( "itemsize_key" )->AsInteger	= inItemSizeKey;
   sqlTaxProfiles->ExecQuery();

   while( !sqlTaxProfiles->Eof )
   {
      __int32 taxProfileKey = sqlTaxProfiles->FieldByName( "key" )->AsInteger;

      outKeys->insert( taxProfileKey );

      sqlTaxProfiles->Next();
   }
   sqlTaxProfiles->Close();
}
//---------------------------------------------------------------------------
int TMenuLoadDB::GetMaxPalmID()
{
	return fPalmID;
}
//---------------------------------------------------------------------------

bool TMenuLoadDB::GetForcedItemOptionKeysWithGroupNumber( unsigned int inGroupNumber, unsigned int inItemKey,
										                  std::vector<TItemOptionInfo> &inForcedOptions )
{
	bool Success = true;

    //::::::::::::::::::::::::::::::::::::

    inForcedOptions.clear();

	sqlOptions->ParamByName( "item_key" )->AsInteger = inItemKey;
    sqlOptions->ExecQuery();

    //sqlOptions->Next();
    while( !sqlOptions->Eof )
    {
		if( ( sqlOptions->FieldByName( "flags" )->AsInteger & 0x0F ) == inGroupNumber )
		{
            TItemOptionInfo foption( sqlOptions->FieldByName( "option_name" )->AsString,
                                     sqlOptions->FieldByName( "foption_key" )->AsInteger,
                                     inItemKey,
                                     sqlOptions->FieldByName( "option_key"  )->AsInteger );

			inForcedOptions.push_back( foption );
		}
        sqlOptions->Next();
    }
    sqlOptions->Close();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
// ---------------------------------------------------------------------------

bool TMenuLoadDB::GetForcedItemOptionKeysWithGroupNumber( unsigned int inGroupNumber, unsigned int inItemKey,
										                  std::vector<TNameAndKey> &inForcedItemOptionKeys )
{
	bool Success = true;

    //::::::::::::::::::::::::::::::::::::

    inForcedItemOptionKeys.clear();

	sqlOptions->SQL->Text = ForcedItemOptionsSQL;

	sqlOptions->ParamByName( "item_key" )->AsInteger = inItemKey;
    sqlOptions->ExecQuery();

    sqlOptions->Next();
    while( !sqlOptions->Eof )
    {
		if( ( sqlOptions->FieldByName( "flags" )->AsInteger & 0x0F ) == inGroupNumber )
		{
            TNameAndKey nameAndKey( sqlOptions->FieldByName( "option_key" )->AsInteger, sqlOptions->FieldByName( "option_name" )->AsString );
			inForcedItemOptionKeys.push_back( nameAndKey );
		}
        sqlOptions->Next();
    }
    sqlOptions->Close();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
// ---------------------------------------------------------------------------

bool TMenuLoadDB::GetForcedItemOptionKeys( unsigned int inItemKey, std::vector<TNameAndKey> &inForcedItemOptionKeys )
{
	bool Success = true;

    //::::::::::::::::::::::::::::::::::::

    inForcedItemOptionKeys.clear();

	this->dbTransaction.RegisterQuery( sqlOptions );

	sqlOptions->SQL->Text = AllForcedItemOptionsSQL;

    sqlOptions->ParamByName( "item_key" )->AsInteger = inItemKey;
    sqlOptions->ExecQuery();

    sqlOptions->Next();
    while( !sqlOptions->Eof )
    {
        TNameAndKey nameAndKey( sqlOptions->FieldByName( "option_key" )->AsInteger, sqlOptions->FieldByName( "option_name" )->AsString );
        inForcedItemOptionKeys.push_back( nameAndKey );
        sqlOptions->Next();
    }
    sqlOptions->Close();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
//----------------------------------------------------------------------------
void TMenuLoadDB::GetAllRevenueCodesFromDB(std::map<int,AnsiString> &revenueCodesMap)
{

	Database::TcpIBSQL tpcOptions( new TIBSQL( NULL ) );

	this->dbTransaction.RegisterQuery( tpcOptions );

	tpcOptions->SQL->Text = RevenueCodesSQL;
    this->dbTransaction.StartTransaction();

    tpcOptions->ExecQuery();

    while( !tpcOptions->Eof )
    {
        TRevenueCodesInfo tpcInfo;

        tpcInfo.code            = tpcOptions->FieldByName( "REVENUECODE"         )->AsInteger;
        tpcInfo.codeDescription = tpcOptions->FieldByName( "REVENUECODE_DESCRIPTION"        )->AsString;

        /*AnsiString revenueCodeDetails = tpcInfo.code;
        revenueCodeDetails += "(";
        revenueCodeDetails += tpcInfo.codeDescription;
        revenueCodeDetails += ")";
        revenueCodes->Add(revenueCodeDetails);
        revenueCodes->Add(""); */

        revenueCodesMap.insert(std::pair<int,AnsiString>(tpcInfo.code,tpcInfo.codeDescription));
        tpcOptions->Next();
    }
    tpcOptions->Close();
}
// ---------------------------------------------------------------------------
bool TMenuLoadDB::GetThirdPartyCodes( std::vector<TThirdPartyCodeInfo>& outCodes )
{
	bool Success = true;

    //::::::::::::::::::::::::::::::::::::

    outCodes.clear();

	Database::TcpIBSQL tpcOptions( new TIBSQL( NULL ) );

	this->dbTransaction.RegisterQuery( tpcOptions );

	tpcOptions->SQL->Text = ThirdPartyCodesSQL;
    this->dbTransaction.StartTransaction();

    tpcOptions->ExecQuery();
    tpcOptions->Next();

    while( !tpcOptions->Eof )
    {
        TThirdPartyCodeInfo tpcInfo;

        tpcInfo.Key         = tpcOptions->FieldByName( "KEY"         )->AsInteger;
        tpcInfo.Code        = tpcOptions->FieldByName( "CODE"        )->AsString;
        tpcInfo.Code_Type   = tpcOptions->FieldByName( "CODETYPE"    )->AsInteger;
        tpcInfo.Visible     = tpcOptions->FieldByName( "VISIBLE"     )->AsString == 'T';
        tpcInfo.Description = tpcOptions->FieldByName( "DESCRIPTION" )->AsString;

        outCodes.push_back( tpcInfo );

        tpcOptions->Next();
    }
    tpcOptions->Close();

	this->dbTransaction.Commit();

    //::::::::::::::::::::::::::::::::::::

	return Success;
}
// ---------------------------------------------------------------------------
void TMenuLoadDB::GeTItemSizePriceLevels(int inItemSizeKey, std::map<int,TItemSizePriceLevel>* ItemSizePriceLevels )
{
    Database::TcpIBSQL sqlitemSizePriceLevels( new TIBSQL( NULL ) );

    this->dbTransaction.RegisterQuery( sqlitemSizePriceLevels );
    sqlitemSizePriceLevels->Close();
    sqlitemSizePriceLevels->SQL->Text = ItemSizePriceLevel;
    sqlitemSizePriceLevels->ParamByName( "itemsize_key" )->AsInteger = inItemSizeKey;
    sqlitemSizePriceLevels->ParamByName( "isEnabled" )->AsString= "T";

    sqlitemSizePriceLevels->ExecQuery();

    while( !sqlitemSizePriceLevels->Eof )
    {
     TItemSizePriceLevel priceLevel;

      priceLevel.PriceLevelKey =sqlitemSizePriceLevels->FieldByName( "PRICELEVEL_KEY" )->AsInteger;
      priceLevel.Price= sqlitemSizePriceLevels->FieldByName( "PRICE" )->AsCurrency;
      priceLevel.PriceLevelName=sqlitemSizePriceLevels->FieldByName("PRICELEVEL_NAME")->AsString;

      ItemSizePriceLevels->insert ( std::pair<int,TItemSizePriceLevel>(priceLevel.PriceLevelKey,priceLevel ) );

      sqlitemSizePriceLevels->Next();
    }
}
// ---------------------------------------------------------------------------
void TMenuLoadDB::GeTItemSizeTaxPercentage(int inItemSizeKey, std::vector<TItemSizeTaxesPercentage> &itemSizeTaxPercentage)
{
    Database::TcpIBSQL sqlitemSizeTaxPercent( new TIBSQL( NULL ) );

    this->dbTransaction.RegisterQuery( sqlitemSizeTaxPercent );
    sqlitemSizeTaxPercent->Close();
    sqlitemSizeTaxPercent->SQL->Text = ItemSizeTaxPercent;
    sqlitemSizeTaxPercent->ParamByName( "itemsize_key" )->AsInteger = inItemSizeKey;

    sqlitemSizeTaxPercent->ExecQuery();

    if( !sqlitemSizeTaxPercent->Eof )
    {
        TItemSizeTaxesPercentage taxTypePecent;

        taxTypePecent.SalesTaxPercent = sqlitemSizeTaxPercent->FieldByName( "Tax" )->AsCurrency;
        taxTypePecent.ServiceChargePercent = sqlitemSizeTaxPercent->FieldByName( "ServiceCharge" )->AsCurrency;

        itemSizeTaxPercentage.push_back(taxTypePecent);  
    }
}
