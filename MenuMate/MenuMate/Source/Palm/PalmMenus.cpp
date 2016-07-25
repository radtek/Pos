//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PalmMenus.h"
#include "MMData.h"

#include "Unicode.h"
#include "ManagerStock.h"
#include "ManagerNet.h"
#include "MMMessageBox.h"
#include "ItemSize.h"

#include "MM_Menu.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#ifdef  PalmMate

char const soh = '[';
char const eoh = ']';
char const sep = '|';
char const coma = ',';

char const soo = '{';
char const eoo = '}';


TManagerMenusPalm::TManagerMenusPalm()
{
	MenuFood = NULL;
	MenuDrinks = NULL;
	//	New = new TListMenuContainer;
	//   Current = new TListMenuContainer;
	MenuUploadFood = new TMemoryStream;
	MenuUploadDrinks = new TMemoryStream;
}
//---------------------------------------------------------------------------
TManagerMenusPalm::~TManagerMenusPalm()
{
	//   delete New;
	//   delete Current;
	delete MenuUploadFood;
	delete MenuUploadDrinks;
}
//---------------------------------------------------------------------------
void TManagerMenusPalm::UpdateMenuAltered(Database::TDBTransaction &DBTransaction,TNetMessageMenuAltered *Request)
{
	for ( int i = 0 ; i < TDeviceRealPalms::Instance().Menus->Current->Count; i ++)
	{
		TListMenu *CurrentMenu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
		if(CurrentMenu->MenuKey ==  Request->MenuKey)
		{
			// Alter Items RAM.
			TItem *Item = CurrentMenu->FetchItemByKey(Request->ItemKey);
			Item->Enabled = Request->ItemEnabled;
			// Alter MenuEnableState.
			CurrentMenu->EnabledStateSync++;
		}
	}

	if(Request->AlterDB)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" UPDATE \"ITEM\" i "
		" SET "
		" i.\"ENABLED\" = :THIS_ENABLED "
		" WHERE "
		"  i.\"ITEM_KEY\" = :LOC_ITEM_KEY ";
		IBInternalQuery->ParamByName("THIS_ENABLED")->AsString = Request->ItemEnabled ? "T" : "F";
		IBInternalQuery->ParamByName("LOC_ITEM_KEY")->AsInteger = Request->ItemKey;
		IBInternalQuery->ExecQuery();
		// TODO 5 -o Michael -c Improvement: Reenable the Course if it's disabled.
	}

	if (Request->Broadcast)
	{
		Request->AlterDB = false;
		TDeviceRealPalms::Instance().ManagerNet->SendToAll(Request);
	}
}

//---------------------------------------------------------------------------
void TManagerMenusPalm::UpdateMenuChanged(Database::TDBControl &DBControl,TNetMessageMenuChanged *Request)
{
	try
	{
		TManagerLogs::Instance().Add(__FUNC__,"Menu Change","Menu Update Pending");
		std::map<UnicodeString, eMenuCommand>::const_iterator iMenus;
        bool MenuChanged = false;
		for (iMenus=Request->Menu_Names.begin(); iMenus != Request->Menu_Names.end(); iMenus++)
		{
			UnicodeString RequestMenuName		= iMenus->first;
			eMenuCommand RequestCommand	= iMenus->second;

			if(RequestCommand == eMenuRemove)
			{
				TListMenu *Menu		= new TListMenu;
				Menu->MenuKey			= 0;
				Menu->MenuName			= RequestMenuName;
				Menu->SwapInCommand	= RequestCommand;
				New->MenuAdd(Menu);
                MenuChanged = true;
			}
			else if(RequestCommand == eMenuAddReplace)
			{
				// Does the Current Menu actually need updating.
				if(MenuFood != NULL && RequestMenuName == MenuFood->MenuName)
				{
					Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
					DBTransaction.StartTransaction();

					if(!IsAValidMenu(DBTransaction,MenuFood->MenuKey) && !IsAPendingMenu(MenuFood->MenuName))
					{
						TListMenu *Menu = LoadMenuFromDB(DBControl,RequestMenuName, RequestCommand);
						if (Menu)
						{
							BuildPalmMenu(Menu);
                            MenuChanged = true;
						}
					}
					DBTransaction.Commit();
				}
				else if (MenuDrinks != NULL && RequestMenuName == MenuDrinks->MenuName)
				{
					Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
					DBTransaction.StartTransaction();

					if(!IsAValidMenu(DBTransaction,MenuDrinks->MenuKey) && !IsAPendingMenu(MenuDrinks->MenuName))
					{
						TListMenu *Menu = LoadMenuFromDB(DBControl,RequestMenuName, RequestCommand);
						if (Menu)
						{
							BuildPalmMenu(Menu);
                            MenuChanged = true;
						}
					}
					DBTransaction.Commit();
				}
				else
				{
					TListMenu *Menu = LoadMenuFromDB(DBControl,RequestMenuName, RequestCommand);
					if (Menu)
					{
						BuildPalmMenu(Menu);
                        MenuChanged = true;
					}
				}
			}
		}

        if(MenuChanged)
        {
    		Database::TDBTransaction DBTransaction1(TDeviceRealPalms::Instance().DBControl);
           	DBTransaction1.StartTransaction();
            Initialise(DBTransaction1);
           	DBTransaction1.Commit();
        }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__,"Exception",Err.Message);
		MessageBox("The menu load failed.\r" + Err.Message, "Menu load Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
/*TListMenu *TManagerMenusPalm::LoadMenu(const UnicodeString &MenuName, eMenuCommand Command)
{
	Menu::TMenuLoadDB				MenuEnumerator(frmData->IBDatabase);

	Menu::TMenusInfo				MenusInfo;
	Menu::TMenusInfo::iterator	iMenus;
	MenuEnumerator.EnumMenus(MenusInfo);
	// Find the menu key based on name.
	for (iMenus=MenusInfo.begin(); iMenus!=MenusInfo.end(); iMenus++)
	{
		if (iMenus->Menu_Name == MenuName)
		{
			Menu::TMenuInfo				MenuInfo;
			Menu::TSizesInfo				SizesInfo;
			Menu::TCategoriesInfo		CategoriesInfo;
			Menu::TLocationsInfo			LocationsInfo;
			Menu::TServingCoursesInfo	ServingCoursesInfo;

			Menu::TMenuLoadDB				MenuLoader(frmData->IBDatabase, iMenus->Menu_Key);
			if (MenuLoader.GetMenu(&MenuInfo, &SizesInfo, &CategoriesInfo, &LocationsInfo, &ServingCoursesInfo))
			{
				TListMenu *Menu		= new TListMenu;

				Menu->MenuName			= MenuInfo.Menu_Name;
				Menu->MenuKey			= MenuInfo.Menu_Key;
				Menu->Menu_Type		= static_cast<TMenuType>(MenuInfo.Menu_Type);
				Menu->Description		= MenuInfo.Menu_Name;
				Menu->SwapInCommand	= Command;

				int PalmsID				= 1;
				// Import New Printing Order.
				// TODO 5 -o Michael -c Improvement : Move the Menu name & Key from the Reg to the DB
				TManagerLogs::Instance().Add(__FUNC__,"Menu Change","Menu will change to " + (Menu->MenuName));

				for (unsigned i=0; i<SizesInfo.Sizes.size(); i++)
				{
					TItemSize *Size	= new TItemSize;
					Size->Name			= SizesInfo.Sizes[i].Size_Name;
					Size->Size_ID		= SizesInfo.Sizes[i].Size_ID;
					Size->Palm_ID		= SizesInfo.Sizes[i].PalmID;
					Menu->Sizes->SizeAdd(Size);
				}
				for (unsigned i=0; i<ServingCoursesInfo.ServingCourses.size(); i++)
				{
					if (ServingCoursesInfo.ServingCourses[i].Enabled && !ServingCoursesInfo.ServingCourses[i].Deleted)
					{
						TServingCourse ServingCourse;
						ServingCourse.ServingCourseKey	= ServingCoursesInfo.ServingCourses[i].ServingCourses_Key;
						ServingCourse.Name					= ServingCoursesInfo.ServingCourses[i].ServingCourse_Name;
						ServingCourse.KitchenName			= ServingCoursesInfo.ServingCourses[i].ServingCourse_Kitchen_Name;
						ServingCourse.Colour					= ServingCoursesInfo.ServingCourses[i].Colour;
						ServingCourse.Selectable			= ServingCoursesInfo.ServingCourses[i].Selectable;
						ServingCourse.ServingCourseID		= ServingCourse.ServingCourseKey & 0xFFFF;

						Menu->ServingCourses.push_back(ServingCourse);
					}
				}
				Menu::TCourseInfo CourseInfo;
				while (MenuLoader.GetNextCourse(&CourseInfo))
				{
					TListCourse *Course					= new TListCourse;
					Course->Course_Name					= CourseInfo.Course_Name;
					Course->CourseKitchenName			= CourseInfo.Course_Kitchen_Name;
					if(Course->CourseKitchenName == WideString(""))
					{
						Course->CourseKitchenName		= Course->Course_Name;
					}
					Course->Course_Key					= CourseInfo.Course_Key;
					Course->ViewableLocations			= CourseInfo.View_Location;
					Course->DefaultServingCourseKey	= CourseInfo.ServingCourses_Key;
					PalmsID++;

					Menu::TItemInfo ItemInfo;
					while (MenuLoader.GetNextItem(&ItemInfo))
					{
						TItem *Item						= new TItem(Course);
						Item->ItemKey					= ItemInfo.Item_Key;
						Item->Item_ID					= ItemInfo.Item_ID;
						Item->Palm_ID					= (PalmsID++) | (Menu->Menu_Type==Menu::mtBeverageMenu?0x4000:0);//ItemInfo.Item_ID;
						Item->Item						= ItemInfo.Item_Name;
						Item->ItemKitchenName		= ItemInfo.Item_Kitchen_Name;
						if(Item->ItemKitchenName == WideString(""))
						{
							Item->ItemKitchenName	= Item->Item;
						}
						Item->SetColour				= ItemInfo.Button_Colour;
						Item->PrintChitNumber		= ItemInfo.Print_Chit;
						Item->DisplaySizes			= ItemInfo.Display_Sizes;
						Item->Enabled					= ItemInfo.Enabled;
						Item->ExclusivelyAsSide		= ItemInfo.Exclusively_As_Side;
						// Menu_Key Only used for printing.
						Item->MenuKey					= Menu->MenuKey;
						Item->Course_Key				= Course->Course_Key;
						Item->Course					= Course->Course_Name;
						Item->PrintingGroupOrder	= CourseInfo.Course_ID;
						Item->CourseKitchenName		= Course->CourseKitchenName;
						Item->ViewableLocations		= Course->ViewableLocations;
						Item->MenuName					= Menu->MenuName;
						Item->ItemType					= static_cast<TItemType>(Menu->Menu_Type);

						Item->PriceAdjust				= 0;
						Item->Note						= "";

						Menu::TItemSizeInfo ItemSizeInfo;
						while (MenuLoader.GetNextItemSize(&ItemSizeInfo))
						{
							TItemSize *Size							= new TItemSize;
							Size->Size_ID								= ItemSizeInfo.Size_ID;
							Size->Name									= ItemSizeInfo.Size_Name;
							Size->SizeKitchenName					= ItemSizeInfo.Size_Kitchen_Name;
							if(Size->SizeKitchenName == WideString(""))
							{
								Size->SizeKitchenName				= Size->Name;
							}

							Size->Palm_ID								= Menu->Sizes->SizeGet(Size->Name)->Palm_ID;
							Size->Price									= ItemSizeInfo.Price;
							Size->Cost									= ItemSizeInfo.Cost;
							Size->CostGSTPercent						= ItemSizeInfo.Cost_GST_Percent;
							Size->GSTPercent							= ItemSizeInfo.GST_Percent;
							Size->HappyPrice							= ItemSizeInfo.Special_Price;
							Size->Available_As_Standard			= ItemSizeInfo.Available_As_Standard;
							Size->SetMenuMask							= ItemSizeInfo.Setmenu_Mask;
							Size->PointsPercent						= ItemSizeInfo.Points_Percent;
							Size->ThirdPartyKey						= ItemSizeInfo.ThirdPartyCodes_Key;

							Size->Categories->FinancialCategory	= ItemSizeInfo.Category;
							if(Size->Categories->FinancialCategory == "")
							{
								throw Exception("Unable to Load, A Blank Financial Category is not allowed.");
							}
							Size->SetMenuItem							= false;
							Size->SetMenuMaster						= false;
							if (TST_PROMO_MASTER(Size->SetMenuMask)) // Is a master.
							{
								Size->SetMenuMaster					= true;
							}
							else if(Size->SetMenuMask != 0)
							{
								Size->SetMenuItem						= true;
							}
							// Check the Size Thridparty key with the Phoenix system.
							if (ManagerThirdParty)
							{
								UnicodeString Code = TDBThirdPartyCodes::GetThirdPartyCodeByKey(Size->ThirdPartyKey); // Added for POS
								if(!PhoenixHM->TestCode(Code))
								{
									throw Exception("Unable to Load Menu " + Menu->MenuName + ". The Third Party Code : " + Code + " is not found in the Phoenix System");
								}
							} // end Added for POS
							for (unsigned i=0; i<ItemSizeInfo.Recipes.size(); i++)
							{
								TItemRecipe *RecipeItem			= new TItemRecipe;

								RecipeItem->StockCode			= ItemSizeInfo.Recipes[i].Stock_Code;
								RecipeItem->StockLocation		= ItemSizeInfo.Recipes[i].Stock_Location;
								RecipeItem->Qty					= ItemSizeInfo.Recipes[i].Qty;
								RecipeItem->Cost					= ItemSizeInfo.Recipes[i].Stock_Unit_Cost * ItemSizeInfo.Recipes[i].Qty;
								RecipeItem->CostGSTPercent		= ItemSizeInfo.Recipes[i].Stock_GST_Percent;

								Currency	StockCost;
								double	StockGST;

								// Very slow! Do in MM_Menu query.
//								if (ManagerStock->GetStockDetails(RecipeItem->StockCode,RecipeItem->StockLocation,
//									RecipeItem->Description, RecipeItem->Unit, StockCost,StockGST))
//								{
//									RecipeItem->Cost				= StockCost * RecipeItem->Qty;
//									RecipeItem->CostGSTPercent	= StockGST;
//								}
								Size->Recipes->RecipeAdd(RecipeItem);
							}
							for (unsigned i=0; i<ItemSizeInfo.Categories.size(); i++)
							{
								TItemSizeCategory *SizeCat	= new TItemSizeCategory;

								SizeCat->CategoryKey			= ItemSizeInfo.Categories[i].Category_Key;
								SizeCat->Category				= ItemSizeInfo.Categories[i].Category;

								Size->Categories->CategoryAdd(SizeCat);
							}
							Item->Sizes->SizeAdd(Size);
						}
						// COuld be faster ways of doing this! (Refer to Office)
						for (unsigned i=0; i<ItemInfo.Sides.size(); i++)
						{
							Item->Sides->SideAdd(
									Item,
									ItemInfo.Sides[i].Master_Item_Key,
									ItemInfo.Sides[i].Item_Key,
									i,
									ItemInfo.Sides[i].Group_Number,
									ItemInfo.Sides[i].Max_Select,
									ItemInfo.Sides[i].Allow_Skip);
						}
						int PalmOptionID = 1;
						for (unsigned i=0; i<CourseInfo.Options.size(); i++)
						{
							TItemOption *ItemOption	= new TItemOption;

							ItemOption->Palm_ID		= PalmOptionID++;
							ItemOption->OptionID		= CourseInfo.Options[i].Option_ID;
							ItemOption->Name			= CourseInfo.Options[i].Option_Name;
							ItemOption->KitchenName	= CourseInfo.Options[i].Option_Kitchen_Name;
							if (ItemOption->KitchenName == WideString(""))
							{
								ItemOption->KitchenName = ItemOption->Name;
							}
							ItemOption->Enabled		= true;//CourseInfo.Options[i].Enabled, Not supported at the mo.;
							ItemOption->IsPlus		= true;
							ItemOption->ForcedMask	= CourseInfo.Options[i].Forced_Mask;
							ItemOption->Flags			= CourseInfo.Options[i].Flags;
							ItemOption->GroupNumber	= CourseInfo.Options[i].GroupNumber;
							ItemOption->PlusOption	= CourseInfo.Options[i].PlusOption;
							ItemOption->MinusOption	= CourseInfo.Options[i].MinusOption;
//							ItemOption->FontInfo		= CourseInfo.Options[i].;

							ItemOption->Owner			= Item->Options;
//							ItemOption->FontInfo		= CourseInfo.Options[i].

							Item->Options->OptionAdd(ItemOption);
						}
						Course->ItemAdd(Item);
					}
					Menu->CourseAdd(Course);
				}
//				unsigned short NonPalmCourseDishNumber = MenuLoader.GetMaxPalmID() + 10;
				New->MenuAdd(Menu);

				return Menu;
			}
		}
	}
	return NULL;
} */
//---------------------------------------------------------------------------
void TManagerMenusPalm::BuildPalmMenu(TListMenu *Menu)
{
	//------MenuSync Block ----------
	std::auto_ptr<TListSideContainer> ForcedSidesList(new TListSideContainer);
	TMemoryStream *MenuUpload;
	UnicodeString FileName;

	if (Menu->Menu_Type == eFoodMenu)
	{
		MenuUpload = TDeviceRealPalms::Instance().Menus->MenuUploadFood;
		FileName = "PalmFoodMenu.txt";
	}
	else
	{
		MenuUpload = TDeviceRealPalms::Instance().Menus->MenuUploadDrinks;
		FileName = "PalmDrinksMenu.txt";
	}

	char NewLineChar = 10;
	MenuUpload->Clear();

	MenuUpload->Write(&sep,sizeof(sep)); // Start of Header "|"
	MenuUpload->Write(Menu->MenuName.t_str(),Menu->MenuName.Length());
	MenuUpload->Write(&sep,sizeof(sep)); // Start of Header "|"
	MenuUpload->Write(&NewLineChar,1); // newline character.

	UnicodeString SizeHeader = "[Sizes]";
	MenuUpload->Write(SizeHeader.t_str(),SizeHeader.Length());
	MenuUpload->Write(&NewLineChar,1); // newline character.
	for (int i = 0; i < Menu->Sizes->Count ; i++)
	{
		MenuUpload->Write(Menu->Sizes->SizeGet(i)->Name.t_str(), Menu->Sizes->SizeGet(i)->Name.Length());
		MenuUpload->Write(&NewLineChar,1); // newline character.
	}
	MenuUpload->Write(&NewLineChar,1); // newline character.
	//-----------------------------
	UnicodeString ServingCourceHeader = "[ServingCourses]";
	MenuUpload->Write(ServingCourceHeader.t_str(), ServingCourceHeader.Length());
	MenuUpload->Write(&NewLineChar,1); // newline character.

	std::vector<TServingCourse>::const_iterator iServingCourse = Menu->ServingCourses.begin();
	while (iServingCourse != Menu->ServingCourses.end())
	{
		UnicodeString Selectable = "F";
		if (iServingCourse->Selectable) Selectable = "T";

		MenuUpload->Write(IntToStr(iServingCourse->ServingCourseID).t_str(), IntToStr(iServingCourse->ServingCourseID).Length());
		MenuUpload->Write(&sep,sizeof(sep)); //"|"
		MenuUpload->Write(UnicodeString(iServingCourse->Name).t_str(), UnicodeString(iServingCourse->Name).Length());
		MenuUpload->Write(&sep,sizeof(sep)); //"|"
		MenuUpload->Write(Selectable.t_str(), Selectable.Length());
		MenuUpload->Write(IntToStr(iServingCourse->Colour).t_str(), IntToStr(iServingCourse->Colour).Length());
		MenuUpload->Write(&NewLineChar,1); // newline character.

		iServingCourse++;
	}

	MenuUpload->Write(&NewLineChar,1); // newline character.
	//-----------------------------
	for (int i = 0; i < Menu->Count ; i++)
	{
		TListCourse *Course = Menu->CourseGet(i);
		if (Course->ViewableLocations & LocPalm)
		{
			// retrieve and create course header.
			MenuUpload->Write(&soh,sizeof(soh)); // Start of Header "["
			MenuUpload->Write(Course->Course_Name.t_str(),
			Course->Course_Name.Length());
			MenuUpload->Write(&eoh,sizeof(eoh)); // Start of Header "]"
			UnicodeString ServingCourseString(IntToStr(Course->DefaultServingCourseKey));
			MenuUpload->Write(ServingCourseString.t_str(), ServingCourseString.Length());
			MenuUpload->Write(&NewLineChar,1); // newline character.

			for (int j = 0; j < Course->Count ; j++)
			{
				TItem *Item = Course->ItemGet(j);
				char TextBool = Item->DisplaySizes ? 'T':'F';
				MenuUpload->Write(&TextBool,sizeof(TextBool));
				TextBool = Item->ExclusivelyAsSide ? 'T':'F';
				MenuUpload->Write(&TextBool,sizeof(TextBool));
				MenuUpload->Write(UnicodeString(Item->Item).t_str(),UnicodeString(Item->Item).Length());
				MenuUpload->Write(&sep,sizeof(sep)); //"|"
				for (int k = 0; k < Item->Sizes->Count ; k++)
				{
					TItemSize *Size = Item->Sizes->SizeGet(k);
					MenuUpload->Write(IntToStr(Size->Palm_ID).t_str(),IntToStr(Size->Palm_ID).Length());
					if(Size->SetMenuMaster||Size->SetMenuItem)
					{
						char SetMenuPrepend;
						if(Size->SetMenuMaster)
						{
							SetMenuPrepend = 'K';
						}
						else
						{
							SetMenuPrepend = Size->Available_As_Standard ? 'T':'F';
						}
						MenuUpload->Write(&SetMenuPrepend,sizeof(SetMenuPrepend));
						int Mask = Size->SetMenuMask;
						Mask = Mask & 0x00FFFFFF;
						UnicodeString strMask = IntToStr(Mask);
						MenuUpload->Write(strMask.t_str(),strMask.Length()); // AND off top btye
					}
					MenuUpload->Write(&sep,sizeof(sep)); //"|"
				}

				for (int k = 1; k < 16; k++) // Max 16 Possible Sides Options Groups.
				{
					ForcedSidesList->SideClear();
					Item->Sides->SidesRetriveMaskedList(k,ForcedSidesList.get());
					if(ForcedSidesList->Count > 0)
					{
						TItemSide *FirstSide = ForcedSidesList->SideGet(0);
						// Write out Forced Side info here.
						MenuUpload->Write(&coma,sizeof(coma)); //","
						MenuUpload->Write(IntToStr(FirstSide->GroupNo).t_str(),IntToStr(FirstSide->GroupNo).Length());
						MenuUpload->Write(&sep,sizeof(sep)); //"|"
						char DisplayNextButton = 'T';
						if(!FirstSide->AllowSkip)
						{
							DisplayNextButton = 'F';
						}
						MenuUpload->Write(&DisplayNextButton,1);
						MenuUpload->Write(IntToStr(FirstSide->MaxSelect).t_str(),IntToStr(FirstSide->MaxSelect).Length());
						MenuUpload->Write(&sep,sizeof(sep)); //"|"
						for (int l = 0; l < ForcedSidesList->Count ; l ++)
						{
							TItem *ItemForcedSide = ForcedSidesList->SideGet(l)->Item;
							MenuUpload->Write(IntToStr(ItemForcedSide->Palm_ID).t_str(),IntToStr(ItemForcedSide->Palm_ID).Length());
							MenuUpload->Write(&sep,sizeof(sep)); //"|"
						}
					}
				}
				MenuUpload->Write(&NewLineChar,1); // newline character.
			}
			// Write out option info here.
			MenuUpload->Write(&soo,sizeof(soo)); // "{"
			TItem *Item = Course->ItemGet(0);
			if(Item != NULL)
			{
				for (int j = 0; j < Item->Options->Count; j++)
				{
					TItemOption *Option = Item->Options->OptionGet(j);
					MenuUpload->Write(IntToHex(Option->Flags,2).t_str(),IntToHex(Option->Flags,2).Length());
					MenuUpload->Write(IntToHex(int(Option->ForcedMask),8).t_str(),IntToHex(int(Option->ForcedMask),8).Length());
					MenuUpload->Write(Option->Name.t_str(),Option->Name.Length());
					MenuUpload->Write(&sep,sizeof(sep));
				}
			}
			MenuUpload->Write(&eoo,sizeof(eoo));  // "}"
			MenuUpload->Write(&NewLineChar,1); // newline character.
			MenuUpload->Write(&NewLineChar,1); // newline character.
		}
	}
	MenuUpload->SaveToFile(ExtractFilePath(Application->ExeName) + FileName);
}
//---------------------------------------------------------------------------
TListMenu *TManagerMenusPalm::IsACurrentMenu(int inMenu_Key)
{
	if(MenuFood != NULL)
	{
		if(MenuFood->MenuKey == inMenu_Key)
		{
			return MenuFood;
		}
	}
	if(MenuDrinks != NULL)
	{
		if(MenuDrinks->MenuKey == inMenu_Key)
		{
			return MenuDrinks;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
void TManagerMenusPalm::GetCurrentMenus(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged *CurrentMenus)
{
	bool StartTrans = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DEVICESMENUS.MENU_NAME "
		" FROM DEVICESMENUS "
		" WHERE "
		" DEVICESMENUS.DEVICE_KEY = :THIS_DEVICE";
		IBInternalQuery->ParamByName("THIS_DEVICE")->AsInteger = TDeviceRealPalms::Instance().ID.DeviceKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof ;IBInternalQuery->Next())
		{
			CurrentMenus->Menu_Names[IBInternalQuery->FieldByName("MENU_NAME")->AsString] = eMenuAddReplace;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

TListMenu *TManagerMenusPalm::IsACurrentMenu(UnicodeString inMenu_Name)
{
	if(MenuFood != NULL)
	{
		if(MenuFood->MenuName == inMenu_Name)
		{
			return MenuFood;
		}
	}
	if(MenuDrinks != NULL)
	{
		if(MenuDrinks->MenuName == inMenu_Name)
		{
			return MenuDrinks;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
TItem * TManagerMenusPalm::FetchItemByID(long ItemID)
{
	for ( int i = 0 ; i < TDeviceRealPalms::Instance().Menus->Current->Count; i ++)
	{
		TListMenu *CurrentMenu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
		TItem * Item = CurrentMenu->FetchItemByID(ItemID);
		if(Item != NULL)
		{
			return Item;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
bool TManagerMenusPalm::FetchServingCourseByID(USHORT ServingCourseID, TServingCourse &ServingCourse)
{
	for (int i=0; i<TDeviceRealPalms::Instance().Menus->Current->Count; i++)
	{
		TListMenu *CurrentMenu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);

		if (CurrentMenu->GetServingCourseByID(ServingCourseID, ServingCourse))
		{
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
void TManagerMenusPalm::ToggleMenus(Database::TDBTransaction &DBTransaction)
{
	// Swap in new menus.
	SwapInNewMenus();
	SetMenuList(DBTransaction,TDeviceRealPalms::Instance().ID.DeviceKey);
	SelectMenuOfType(eDrinksMenu);
	SelectMenuOfType(eFoodMenu);
}
//---------------------------------------------------------------------------
void TManagerMenusPalm::SelectMenuOfType(TMenuType MenuType)
{
	TListMenu * Menu;
	for (int i = 0; i < TDeviceRealPalms::Instance().Menus->Current->Count; i++)
	{
		Menu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
		if (MenuType == eFoodMenu)
		{
			if(Menu->Menu_Type == MenuType)
			{
				MenuFood = Menu;
				break;
			}
		}
		else
		{
			if(Menu->Menu_Type == MenuType)
			{
				MenuDrinks = Menu;
				break;
			}
		}
	}
}


//---------------------------------------------------------------------------
bool TManagerMenusPalm::RequiresUpdate(Database::TDBTransaction &DBTransaction,UnicodeString inMenu_Name)
{
	// Menu that is Current, but Key does not match DB wil require updateding.
	TListMenu *Menu = IsACurrentMenu(inMenu_Name);
	if(Menu != NULL)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT MENU.MENU_KEY "
		" FROM MENU "
		" WHERE "
		"  MENU.MENU_NAME = :Loc_MENU_NAME";
		IBInternalQuery->ParamByName("Loc_MENU_NAME")->AsString = inMenu_Name;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			if(Menu->MenuKey != IBInternalQuery->FieldByName("MENU_KEY")->AsInteger)
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------

bool TManagerMenusPalm::RequiresRemove(Database::TDBTransaction &DBTransaction,UnicodeString inMenu_Name)
{
	TListMenu *Menu = IsACurrentMenu(inMenu_Name);
	if(Menu != NULL)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT m.\"MENU_NAME\" "
		" FROM \"MENU\" m"
		" WHERE "
		"  m.\"MENU_KEY\" = :Loc_MENU_KEY";
		IBInternalQuery->ParamByName("Loc_MENU_KEY")->AsInteger = Menu->MenuKey;
		IBInternalQuery->ExecQuery();
		if(!IBInternalQuery->RecordCount)
		{ // Ok Remove this menu uf it is not pending insertion.
			// This menu will be pending if a commit has occured and it required
			// updating.
			if(New->MenuGet(inMenu_Name) == NULL)
			{
				return true;
			}
		}
	}
	return false;
}

void TManagerMenusPalm::MenuInserted(Database::TDBTransaction &DBTransaction)
{
	TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
	try
	{
		bool UpdateRequired = false;
		for ( int i = 0 ; i < TDeviceRealPalms::Instance().Menus->Current->Count; i ++)
		{
			TListMenu *CurrentMenu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
			if(RequiresUpdate(DBTransaction,CurrentMenu->MenuName))
			{
				Request->Menu_Names[CurrentMenu->MenuName] = eMenuAddReplace;
				UpdateRequired = true;
			}
		}
		if(UpdateRequired)
		{
			TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl,Request);
			TDeviceRealPalms::Instance().Menus->ToggleMenus(DBTransaction);
		}
	}
	__finally
	{
		delete Request;
	}
}



void TManagerMenusPalm::MenuRemoved(Database::TDBTransaction &DBTransaction)
{
	TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
	try
	{
		bool UpdateRequired = false;
		for ( int i = 0 ; i < TDeviceRealPalms::Instance().Menus->Current->Count; i ++)
		{
			TListMenu *CurrentMenu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
			if(RequiresRemove(DBTransaction,CurrentMenu->MenuName))
			{
				Request->Menu_Names[CurrentMenu->MenuName] = eMenuRemove;
				UpdateRequired = true;
			}
		}
		if(UpdateRequired)
		{
			TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl,Request);
			TDeviceRealPalms::Instance().Menus->ToggleMenus(DBTransaction);
		}
	}
	__finally
	{
		delete Request;
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TManagerMenusPalm::MenuAddedRemoved(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request)
{
	//Simply rechecks all our current menus to see if they have changed.
	if (Request->Broadcast)
	{
		Request->CompareToDataBase = true;
		TDeviceRealPalms::Instance().ManagerNet->SendToAll(Request);
	}

	for (int i = 0 ; i < TDeviceRealPalms::Instance().Menus->Current->Count ; i++)
	{
		TListMenu *Menu = TDeviceRealPalms::Instance().Menus->Current->MenuGet(i);
		if(RequiresUpdate(DBTransaction,Menu->MenuName))
		{
			Request->Menu_Names[Menu->MenuName] = eMenuAddReplace;
		}
		else if(RequiresRemove(DBTransaction,Menu->MenuName))
		{
			Request->Menu_Names[Menu->MenuName] = eMenuRemove;
		}

	}
	if(Request->Menu_Names.size() > 0)
	{
		TDeviceRealPalms::Instance().Menus->UpdateMenuChanged(TDeviceRealPalms::Instance().DBControl,Request);
		TDeviceRealPalms::Instance().Menus->ToggleMenus(DBTransaction);
	}
}

//---------------------------------------------------------------------------

void TManagerMenusPalm::SwapInNewMenus()
{
	if(New->Count > 0)
	{
		for (int i = 0 ; i < TDeviceRealPalms::Instance().Menus->New->Count ; i++)
		{
			TListMenu *Menu = TDeviceRealPalms::Instance().Menus->New->MenuGet(i);

			if(Menu->SwapInCommand == eMenuRemove)
			{
				Current->MenuDelete(Current->MenuGet(Menu->MenuName));
			}
			else
			{
				Current->MenuDelete(Current->MenuGet(Menu->MenuName));
				Current->MenuAdd(Menu);
			}
		}
		MenuFood = NULL;
		MenuDrinks = NULL;
		New->MenuClear();
	}
}

//---------------------------------------------------------------------------
void TManagerMenusPalm::MenuChanged(Database::TDBTransaction &DBTransaction,TNetMessageMenuChanged * Request)
{
	// Palms have a Max of two menus and ignore MenuMates
	// 'Set your menus to this list' commands;
}


#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------







