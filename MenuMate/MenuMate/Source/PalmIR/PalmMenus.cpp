//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PalmMenus.h"
#include "data.h"
#include "Unicode.hpp"
#include "ManagerStock.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#ifdef  PalmMate

char const soh = '[';
char const eoh = ']';
char const sep = '|';
char const coma = ',';

char const soo = '{';
char const eoo = '}';


TPalmMenus::TPalmMenus()
{
   MenuFood = NULL;
   MenuDrinks = NULL;
   New = new TListMenuContainer;
   Current = new TListMenuContainer;
   MenuUploadFood = new TMemoryStream;
   MenuUploadDrinks = new TMemoryStream;
}
//---------------------------------------------------------------------------
TPalmMenus::~TPalmMenus()
{
   delete New;
   delete Current;
   delete MenuUploadFood;
   delete MenuUploadDrinks;   
}
//---------------------------------------------------------------------------
void TPalmMenus::UpdateMenuAltered(TReqMenuAlter *Request)
{
	for ( int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i ++)
	{
		TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
      if(CurrentMenu->MenuKey ==  Request->MenuKey)
      {
         // Alter Items RAM.
         TItem *Item = CurrentMenu->FetchItemByKey(Request->Item_Key);
         Item->Enabled = Request->ItemEnabled;
         // Alter MenuEnableState.
         CurrentMenu->EnabledStateSync++;
      }
   }

   if(Request->Header.AlterDB)
   {
      frmData->IBAdjust->Close();
      frmData->IBAdjust->Params->Clear();
      frmData->IBAdjust->ParamCheck = true;
      frmData->IBAdjust->SQL->Text =
      " UPDATE \"ITEM\" i "
      " SET "
      " i.\"ENABLED\" = :THIS_ENABLED "
      " WHERE "
      "  i.\"ITEM_KEY\" = :LOC_ITEM_KEY ";
      frmData->IBAdjust->Params->ParamByName("THIS_ENABLED")->AsString = Request->ItemEnabled ? "T" : "F";
      frmData->IBAdjust->Params->ParamByName("LOC_ITEM_KEY")->AsInteger = Request->Item_Key;
      frmData->IBAdjust->ExecSQL();
      // TODO 5 -o Michael -c Improvement: Reenable the Course if it's disabled.
   }

   if (Request->BroadcastRequest)
   {
      Request->InstructionType = udpCommand;
      Request->Header.AlterDB = false;
      frmTCP->SendToAll(Request);
   }
}

//---------------------------------------------------------------------------

void TPalmMenus::UpdateMenuChanged(TNetMessageMenuChanged *Request)
{
	bool StartedTrans = false;
   try
   {
      if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); StartedTrans = true; }

      TManagerLogs::Instance().Add(__FUNC__,"Menu Change","Menu Update Pending");

      frmData->IBMenu->Open();
      frmData->IBCourse->Open();
      frmData->IBItem->Open();
      frmData->IBItemSize->Open();
      frmData->IBItemSide->Open();
      frmData->IBRecipe->Open();
      frmData->IBOption->Open();
      frmData->IBSize->Open();

      for ( std::map<AnsiString,eMenuCommand>::const_iterator RequestMenus = Request->Menu_Names.begin();
            RequestMenus !=  Request->Menu_Names.end();
            RequestMenus++)
      {
         AnsiString RequestMenuName = RequestMenus->first;
         eMenuCommand RequestCommand = RequestMenus->second;
         if(RequestCommand == eMenuRemove )
         {
            TListMenu *Menu = new TListMenu;
            Menu->MenuKey = 0;
            Menu->MenuName = RequestMenuName;
            Menu->SwapInCommand = RequestCommand;
            New->MenuAdd(Menu);
         }
         else if(RequestCommand == eMenuAddReplace)
         {
            Variant locvalues = Variant(RequestMenuName);
				if (frmData->IBMenu->Locate("MENU_NAME", locvalues, TLocateOptions()))
            {
               TListMenu *Menu = new TListMenu;
               Menu->MenuName = frmData->IBMenu->FieldByName("MENU_NAME")->AsString;
               Menu->MenuKey = frmData->IBMenu->FieldByName("MENU_KEY")->AsInteger;
               Menu->Menu_Type = frmData->IBMenu->FieldByName("MENU_TYPE")->AsInteger;
               Menu->Description = frmData->IBMenu->FieldByName("DESCRIPTION")->AsString;
               Menu->SwapInCommand = RequestCommand;
               // Import New Printing Order.
               // TODO 5 -o Michael -c Improvement : Move the Menu name & Key from the Reg to the DB
               TManagerLogs::Instance().Add(__FUNC__,"Menu Change","Menu will change to " + (Menu->MenuName));

               unsigned short CourseDishNumber;
               unsigned short NonPalmCourseDishNumber;
               if(Menu->Menu_Type == eDrinksMenu)
               {
                  CourseDishNumber = 0x4001;
                  NonPalmCourseDishNumber = 0x4001;
               }
               else
               {
                  CourseDishNumber = 0x0001;
                  NonPalmCourseDishNumber = 0x0001;
					}

					// Find Highest Possable Key For Nonpalmable Courses and Items.
					TIBSQL *qrMaxPalmDishCourse			= NULL;
					try
					{
						qrMaxPalmDishCourse					= new TIBSQL(frmData);

						qrMaxPalmDishCourse->Database		= frmData->IBDatabase;
						qrMaxPalmDishCourse->Transaction	= frmData->IBTransaction;

						qrMaxPalmDishCourse->SQL->Text	=
							"SELECT "
								"COURSE.COURSE_NAME, "
								"COURSE.VIEW_LOCATION "
							"FROM "
								"COURSE INNER JOIN ITEM ON COURSE.COURSE_KEY = ITEM.COURSE_KEY "
							"WHERE "
								"COURSE.MENU_KEY = :MENU_KEY";
						qrMaxPalmDishCourse->ParamByName("MENU_KEY")->AsInteger = Menu->MenuKey;
						AnsiString LastCourseName = "";
						for (qrMaxPalmDishCourse->ExecQuery(); !qrMaxPalmDishCourse->Eof; qrMaxPalmDishCourse->Next())
						{
							if (qrMaxPalmDishCourse->FieldByName("VIEW_LOCATION")->AsInteger & LocPalm)
							{
								if (LastCourseName != qrMaxPalmDishCourse->FieldByName("COURSE_NAME")->AsString)
								{
									LastCourseName = qrMaxPalmDishCourse->FieldByName("COURSE_NAME")->AsString;
									NonPalmCourseDishNumber++;
								}
								NonPalmCourseDishNumber++;
							}
						}
					}
					__finally
					{
						delete qrMaxPalmDishCourse;
					}
					NonPalmCourseDishNumber += 10; //Go 10 Higher just in case. */

               unsigned char TempSizeID = 1;

               for(frmData->IBSize->First();!frmData->IBSize->Eof;frmData->IBSize->Next())
               {
                 TItemSize *Size = new TItemSize;
                 Size->Name = frmData->IBSize->FieldByName("SIZE_NAME")->AsString;
                 Size->Size_ID = (unsigned short)frmData->IBSize->FieldByName("SIZE_ID")->AsInteger;
                 Size->Palm_ID = TempSizeID;
                 Menu->Sizes->SizeAdd(Size);
                 TempSizeID++;
               }

               for (frmData->IBCourse->First(); !frmData->IBCourse->Eof; frmData->IBCourse->Next())
               {
                  TListCourse *Course = new TListCourse;
                  Course->Course_Name = frmData->IBCourse->FieldByName("COURSE_NAME")->AsString;
                  Course->CourseKitchenName  = UTF8ToWideString(frmData->IBCourse->FieldByName("COURSE_KITCHEN_NAME")->AsString);
                  Course->Course_Key = frmData->IBCourse->FieldByName("COURSE_KEY")->AsInteger;
                  Course->ViewableLocations = frmData->IBCourse->FieldByName("VIEW_LOCATION")->AsInteger;
                  //------MenuSync Block ----------
                  bool ApplyToUploads = false;
                  if (Course->ViewableLocations & LocPalm)
                  {
                     Course->Palm_ID = CourseDishNumber++;
	                  ApplyToUploads = true;                     
                  }
                  else
                  {
                     Course->Palm_ID = NonPalmCourseDishNumber++;
                  }
                  //-----------------------------

                  for (frmData->IBItem->First(); !frmData->IBItem->Eof; frmData->IBItem->Next())
                  {

                     TItem *Item = new TItem(Course);
                     Item->Item_Key = frmData->IBItem->FieldByName("ITEM_KEY")->AsInteger;
                     Item->Item_ID = frmData->IBItem->FieldByName("ITEM_ID")->AsInteger;
                     Item->Course_Key 	= Course->Course_Key;
                     // Menu_Key Only used for printing.
                     Item->MenuKey 		= Menu->MenuKey;
                     Item->Course     	= Course->Course_Name;
                     Item->CourseKitchenName = Course->CourseKitchenName;
                     Item->ViewableLocations = frmData->IBCourse->FieldByName("VIEW_LOCATION")->AsInteger;

                     Item->PrintingGroupOrder = frmData->IBCourse->FieldByName("COURSE_ID")->AsInteger;

                     Item->Item    				= frmData->IBItem->FieldByName("ITEM_NAME")->AsString;
                     Item->ItemKitchenName  	= UTF8ToWideString(frmData->IBItem->FieldByName("ITEM_KITCHEN_NAME")->AsString);

                     Item->SetColour   = (TColor)frmData->IBItem->FieldByName("BUTTON_COLOUR")->AsInteger;
                     Item->PrintChitNumber = frmData->IBItem->FieldByName("PRINT_CHIT")->AsBoolean;
                     Item->DisplaySizes    = frmData->IBItem->FieldByName("DISPLAY_SIZES")->AsBoolean;
                     Item->Enabled         = frmData->IBItem->FieldByName("ENABLED")->AsBoolean;
                     Item->ExclusivelyAsSide = frmData->IBItem->FieldByName("EXCLUSIVELY_AS_SIDE")->AsBoolean;
                     Item->PriceAdjust 		= 0;
                     Item->Note 				= "";
                     Item->MenuName			= frmData->IBMenu->FieldByName("MENU_NAME")->AsString;
                     Item->MenuType        =(TMenuType)frmData->IBMenu->FieldByName("MENU_TYPE")->AsInteger;


                     //------MenuSync Block ----------
                     if(ApplyToUploads)
                     {
                        Item->Palm_ID 		= CourseDishNumber++;
                     }
                     else
                     {
                        Item->Palm_ID = NonPalmCourseDishNumber++;
                     }


                     //-----------------------------

                     for (frmData->IBItemSize->First(); !frmData->IBItemSize->Eof; frmData->IBItemSize->Next())
                     {
                        TItemSize *Size = new TItemSize;
                        Size->Size_ID = (unsigned short)frmData->IBItemSize->FieldByName("SIZE_ID")->AsInteger;
                        Size->Name = frmData->IBItemSize->FieldByName("SIZE_NAME")->AsString;
	                     Size->SizeKitchenName  = UTF8ToWideString(frmData->IBItemSize->FieldByName("SIZE_KITCHEN_NAME")->AsString);
                        Size->Palm_ID = Menu->Sizes->SizeGet(Size->Name)->Palm_ID;
                        Size->Price = frmData->IBItemSize->FieldByName("PRICE")->AsCurrency;
                        Size->Cost  = frmData->IBItemSize->FieldByName("COST")->AsCurrency;
                        Size->CostGSTPercent = frmData->IBItemSize->FieldByName("COST_GST_PERCENT")->AsFloat;
                        Size->GSTPercent  = frmData->IBItemSize->FieldByName("GST_PERCENT")->AsFloat;
                        Size->HappyPrice = frmData->IBItemSize->FieldByName("SPECIAL_PRICE")->AsCurrency;
                        Size->Available_As_Standard = frmData->IBItemSize->FieldByName("AVAILABLE_AS_STANDARD")->AsBoolean;
                        Size->SetMenuMask = frmData->IBItemSize->FieldByName("SETMENU_MASK")->AsInteger;
                        Size->PointsPercent = frmData->IBItemSize->FieldByName("POINTS_PERCENT")->AsFloat;                        
								Size->ThirdPartyKey = frmData->IBItemSize->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;								                        
                        Size->SetMenuItem = false;
                        Size->SetMenuMaster = false;
                        if (TST_PROMO_MASTER(Size->SetMenuMask)) // Is a master.
                        {
                           Size->SetMenuMaster = true;
                        }
                        else if(Size->SetMenuMask != 0)
                        {
                           Size->SetMenuItem = true;
                        }

                        for (frmData->IBRecipe->First(); !frmData->IBRecipe->Eof; frmData->IBRecipe->Next())
                        {
                           TItemRecipe *RecipeItem = new TItemRecipe;

                           RecipeItem->StockCode       = frmData->IBRecipe->FieldByName("STOCK_CODE")->AsString;
                           RecipeItem->StockLocation       = frmData->IBRecipe->FieldByName("STOCK_LOCATION")->AsString;
                           RecipeItem->Qty           	= frmData->IBRecipe->FieldByName("Qty")->AsFloat;

                           Currency StockCost;
                           double StockGST;
									if (ManagerStock->GetStockDetails(RecipeItem->StockCode,RecipeItem->StockLocation,
                              RecipeItem->Description, RecipeItem->Unit, StockCost,StockGST))
                           {
                              RecipeItem->Cost = StockCost * RecipeItem->Qty;
                              RecipeItem->CostGSTPercent = StockGST;
                           }
                           Size->Recipes->RecipeAdd(RecipeItem);
                        }

                        // Size Catagories
                        frmData->IBQuery->Close();
                        frmData->IBQuery->SQL->Text = "SELECT CATEGORY_KEY,CATEGORY FROM CATEGORIES "
                                              "INNER JOIN ITEMSIZECATEGORY ON CATEGORIES.CATEGORY_KEY = ITEMSIZECATEGORY.CATEGORY_KEY "
                                              "WHERE ITEMSIZECATEGORY.ITEMSIZE_KEY = :ITEMSIZE_KEY";
                        frmData->IBQuery->ParamByName("ITEMSIZE_KEY")->AsString = frmData->IBItemSize->FieldByName("ITEMSIZE_KEY")->AsInteger;
                        frmData->IBQuery->Open();
                        for (frmData->IBQuery->First(); !frmData->IBQuery->Eof; frmData->IBQuery->Next())
                        {
                           TItemSizeCategory *SizeCat = new TItemSizeCategory;
                           SizeCat->CategoryKey = frmData->IBQuery->FieldByName("CATEGORY_KEY")->AsInteger;
                           SizeCat->Category = frmData->IBQuery->FieldByName("CATEGORY")->AsString;
                           Size->Categories->CategoryAdd(SizeCat);
                        }
                        Item->Sizes->SizeAdd(Size);
                     }

                     for (frmData->IBItemSide->First(); !frmData->IBItemSide->Eof; frmData->IBItemSide->Next())
                     {
                        Item->Sides->SideAdd(Item,frmData->IBItemSide->FieldByName("MASTER_ITEM_KEY")->AsInteger,
                                          frmData->IBItemSide->FieldByName("ITEM_KEY")->AsInteger,
                                          frmData->IBItemSide->FieldByName("IOO")->AsInteger,
                                          frmData->IBItemSide->FieldByName("GROUP_NO")->AsInteger,
                                          frmData->IBItemSide->FieldByName("MAX_SELECT")->AsInteger,
                                          frmData->IBItemSide->FieldByName("ALLOW_SKIP")->AsBoolean);
                     }

                     int OptionID = 1;
                     for (frmData->IBOption->First(); !frmData->IBOption->Eof; frmData->IBOption->Next())
                     {
                        Item->Options->OptionAdd( (unsigned short)frmData->IBOption->FieldByName("OPTION_ID")->AsInteger,
                                          frmData->IBOption->FieldByName("OPTION_NAME")->AsString,
                                          UTF8ToWideString(frmData->IBOption->FieldByName("OPTION_KITCHEN_NAME")->AsString),
                                          frmData->IBOption->FieldByName("ENABLED")->AsBoolean,
                                          frmData->IBOption->FieldByName("FLAGS")->AsInteger,
                                          frmData->IBOption->FieldByName("FORCED_MASK")->AsInteger,
                                          true,
					                           OptionID++);

                     }
                     Course->ItemAdd(Item);
                  }
                  Menu->CourseAdd(Course);
               }

               // Write out Forced Sides Data
               New->MenuAdd(Menu);

					// ----------------------------------------------------------------------
               // BUILD THE PALM MENUS
               //------MenuSync Block ----------
               TListSideContainer *ForcedSidesList = new TListSideContainer;
               TMemoryStream *MenuUpload;
               AnsiString FileName;
               try
               {
                  if (Menu->Menu_Type == eFoodMenu)
                  {
                      MenuUpload = TDeviceRealTerminal::Instance().Menus.MenuUploadFood;
                      FileName = "PalmFoodMenu.txt";
                  }
                  else
                  {
                      MenuUpload = TDeviceRealTerminal::Instance().Menus.MenuUploadDrinks;
                      FileName = "PalmDrinksMenu.txt";
                  }

                  char NewLineChar = 10;
                  MenuUpload->Clear();

                  MenuUpload->Write(&sep,sizeof(sep)); // Start of Header "|"
                  MenuUpload->Write(Menu->MenuName.c_str(),Menu->MenuName.Length());
                  MenuUpload->Write(&sep,sizeof(sep)); // Start of Header "|"
                  MenuUpload->Write(&NewLineChar,1); // newline character.

                  AnsiString SizeHeader = "[Sizes]";
                  MenuUpload->Write(SizeHeader.c_str(),SizeHeader.Length());
                  MenuUpload->Write(&NewLineChar,1); // newline character.
                  for (int i = 0; i < Menu->Sizes->Count ; i++)
                  {
                    MenuUpload->Write(Menu->Sizes->SizeGet(i)->Name.c_str(),
                                      Menu->Sizes->SizeGet(i)->Name.Length());
                    MenuUpload->Write(&NewLineChar,1); // newline character.
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
                        MenuUpload->Write(Course->Course_Name.c_str(),
                                          Course->Course_Name.Length());
                        MenuUpload->Write(&eoh,sizeof(eoh)); // Start of Header "]"
                        MenuUpload->Write(&NewLineChar,1); // newline character.

                        for (int j = 0; j < Course->Count ; j++)
                        {
                           TItem *Item = Course->ItemGet(j);
                           char TextBool = Item->DisplaySizes ? 'T':'F';
                           MenuUpload->Write(&TextBool,sizeof(TextBool));
                           TextBool = Item->ExclusivelyAsSide ? 'T':'F';
                           MenuUpload->Write(&TextBool,sizeof(TextBool));
                           MenuUpload->Write(AnsiString(Item->Item).c_str(),AnsiString(Item->Item).Length());
                           MenuUpload->Write(&sep,sizeof(sep)); //"|"
                           for (int k = 0; k < Item->Sizes->Count ; k++)
                           {
                              TItemSize *Size = Item->Sizes->SizeGet(k);
                              MenuUpload->Write(IntToStr(Size->Palm_ID).c_str(),IntToStr(Size->Palm_ID).Length());
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
                                 AnsiString strMask = IntToStr(Mask);
                                 MenuUpload->Write(strMask.c_str(),strMask.Length()); // AND off top btye
                              }
                              MenuUpload->Write(&sep,sizeof(sep)); //"|"
                           }

                        	for (int k = 1; k < 16; k++) // Max 16 Possible Sides Options Groups.
                           {
                              ForcedSidesList->SideClear();
                              Item->Sides->SidesRetriveMaskedList(k,ForcedSidesList);
                              if(ForcedSidesList->Count > 0)
                              {
                                 TItemSide *FirstSide = ForcedSidesList->SideGet(0);
                                 // Write out Forced Side info here.
                                 MenuUpload->Write(&coma,sizeof(coma)); //","
                                 MenuUpload->Write(IntToStr(FirstSide->GroupNo).c_str(),IntToStr(FirstSide->GroupNo).Length());
	                              MenuUpload->Write(&sep,sizeof(sep)); //"|"
                                 char DisplayNextButton = 'T';
                                 MenuUpload->Write(&DisplayNextButton,1);
                                 MenuUpload->Write(IntToStr(FirstSide->MaxSelect).c_str(),IntToStr(FirstSide->MaxSelect).Length());
	                              MenuUpload->Write(&sep,sizeof(sep)); //"|"
                                 for (int l = 0; l < ForcedSidesList->Count ; l ++)
                                 {
                                    TItem *ItemForcedSide = ForcedSidesList->SideGet(l)->Item;
												MenuUpload->Write(IntToStr(ItemForcedSide->Palm_ID).c_str(),IntToStr(ItemForcedSide->Palm_ID).Length());
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
                              MenuUpload->Write(IntToHex(Option->Flags,2).c_str(),IntToHex(Option->Flags,2).Length());
                              MenuUpload->Write(IntToHex(int(Option->ForcedMask),8).c_str(),IntToHex(int(Option->ForcedMask),8).Length());
                              MenuUpload->Write(Option->Name.c_str(),Option->Name.Length());
                              MenuUpload->Write(&sep,sizeof(sep));
                           }
                        }
                        MenuUpload->Write(&eoo,sizeof(eoo));  // "}"
                        MenuUpload->Write(&NewLineChar,1); // newline character.
                        MenuUpload->Write(&NewLineChar,1); // newline character.
                     }
                  }
               }
               __finally
               {
			         delete ForcedSidesList;
               }

               MenuUpload->SaveToFile(ExtractFilePath(Application->ExeName) + FileName);
            }
         }
      }
      if (frmData->IBTransaction->InTransaction && StartedTrans) { frmData->IBTransaction->Commit(); }
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,"Exception",Err.Message);
      if (frmData->IBTransaction->InTransaction && StartedTrans) { frmData->IBTransaction->Rollback(); }
   }
}

//---------------------------------------------------------------------------

TListMenu *TPalmMenus::IsACurrentMenu(int inMenu_Key)
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
TListMenu *TPalmMenus::IsACurrentMenu(AnsiString inMenu_Name)
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
TItem * TPalmMenus::FetchItemByID(long ItemID)
{
	for ( int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i ++)
	{
		TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
      TItem * Item = CurrentMenu->FetchItemByID(ItemID);
      if(Item != NULL)
      {
         return Item;
      }
   }
   return NULL;
}
//---------------------------------------------------------------------------
void TPalmMenus::ToggleMenus()
{
   // Swap in new menus.
   SwapInNewMenus();
   SelectMenuOfType(eDrinksMenu);
   SelectMenuOfType(eFoodMenu);
}
//---------------------------------------------------------------------------
void TPalmMenus::SelectMenuOfType(TMenuType MenuType)
{
   TListMenu * Menu;
   for (int i = 0; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i++)
   {
      Menu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
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
bool TPalmMenus::RequiresUpdate(AnsiString inMenu_Name)
{
   // Menu that is Current, but Key does not match DB wil require updateding.
   TListMenu *Menu = IsACurrentMenu(inMenu_Name);
   if(Menu != NULL)
   {
		if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
      frmData->IBQuery->Close();
      frmData->IBQuery->SQL->Text =
      " SELECT MENU.MENU_KEY "
      " FROM MENU "
      " WHERE "
      "  MENU.MENU_NAME = :Loc_MENU_NAME";
      frmData->IBQuery->Params->ParamByName("Loc_MENU_NAME")->AsString = inMenu_Name;
      frmData->IBQuery->Open();
      if(!frmData->IBQuery->IsEmpty())
      {
         if(Menu->MenuKey != frmData->IBQuery->FieldByName("MENU_KEY")->AsInteger)
         {
            return true;
         }
      }
		if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
   }
   return false;
}

//---------------------------------------------------------------------------

bool TPalmMenus::RequiresRemove(AnsiString inMenu_Name)
{
   TListMenu *Menu = IsACurrentMenu(inMenu_Name);
   if(Menu != NULL)
   {
		if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
      frmData->IBQuery->Close();
      frmData->IBQuery->SQL->Text =
      " SELECT m.\"MENU_NAME\" "
      " FROM \"MENU\" m"
      " WHERE "
      "  m.\"MENU_KEY\" = :Loc_MENU_KEY";
      frmData->IBQuery->Params->ParamByName("Loc_MENU_KEY")->AsInteger = Menu->MenuKey;
      frmData->IBQuery->Open();
      if(frmData->IBQuery->IsEmpty())
      { // Ok Remove this menu uf it is not pending insertion.
        // This menu will be pending if a commit has occured and it required
        // updating.
         if(New->MenuGet(inMenu_Name) == NULL)
         {
            return true;
         }
      }
		if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }      
   }
   return false;
}

void TPalmMenus::MenuInserted()
{
   TReqMenuChange *Request = new TReqMenuChange;
   try
   {
      bool UpdateRequired = false;
      for ( int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i ++)
      {
         TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
         if(RequiresUpdate(CurrentMenu->MenuName))
         {
            Request->Menu_Names[CurrentMenu->MenuName] = eMenuAddReplace;
            UpdateRequired = true;
         }
      }
      if(UpdateRequired)
      {
         TDeviceRealTerminal::Instance().Menus.UpdateMenuChanged(Request);
         TDeviceRealTerminal::Instance().Menus.SwapInNewMenus();
         TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
         TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);
      }
   }
   __finally
   {
      delete Request;
   }
}



void TPalmMenus::MenuRemoved()
{
   TReqMenuChange *Request = new TReqMenuChange;
   try
   {
		bool UpdateRequired = false;
      for ( int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i ++)
      {
         TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
         if(RequiresRemove(CurrentMenu->MenuName))
         {
            Request->Menu_Names[CurrentMenu->MenuName] = eMenuRemove;
		      UpdateRequired = true;
         }
      }
      if(UpdateRequired)
      {
         TDeviceRealTerminal::Instance().Menus.UpdateMenuChanged(Request);
         TDeviceRealTerminal::Instance().Menus.SwapInNewMenus();
         TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
         TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);
      }
   }
   __finally
   {
      delete Request;
   }
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TPalmMenus::MenuAddedRemoved(TReqMenuChange * Request)
{
   //Simply rechecks all our current menus to see if they have changed.

   Request->Header.Error = proA_Ok;
   Request->ErrorMsg = "";
   if (Request->BroadcastRequest)
   {
      Request->InstructionType = udpCommand;
      Request->Header.AlterDB = false;
      Request->CompareToDataBase = true;
      frmTCP->SendToAll(Request);
   }

   for (int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count ; i++)
   {
      TListMenu *Menu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
      if(RequiresUpdate(Menu->MenuName))
      {
         Request->Menu_Names[Menu->MenuName] = eMenuAddReplace;
      }
      else if(RequiresRemove(Menu->MenuName))
      {
         Request->Menu_Names[Menu->MenuName] = eMenuRemove;
      }

   }
   if(Request->Menu_Names.size() > 0)
   {
      TDeviceRealTerminal::Instance().Menus.UpdateMenuChanged(Request);
		TDeviceRealTerminal::Instance().Menus.ToggleMenus();
   }
}

//---------------------------------------------------------------------------

void TPalmMenus::SwapInNewMenus()
{
   if(New->Count > 0)
   {
      try
      {
         for (int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.New->Count ; i++)
         {
            TListMenu *Menu = TDeviceRealTerminal::Instance().Menus.New->MenuGet(i);

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
      __finally
      {
   		TDeviceRealTerminal::Instance().SetMenuList();
      }
   }
}

//---------------------------------------------------------------------------
void TPalmMenus::MenuChanged(TNetMessageMenuChanged * Request)
{
	// Palms have a Max of two menus and ignore MenuMates
	// 'Set your menus to this list' commands;
}

void TPalmMenus::GetCurrentMenus(TNetMessageMenuChanged *CurrentMenus)
{
	bool StartTrans = false;
	try
	{
		if (!frmData->IBTransaction->InTransaction) {frmData->IBTransaction->StartTransaction(); StartTrans = true; }

      frmData->IBQuery->Close();
      frmData->IBQuery->Params->Clear();
      frmData->IBQuery->ParamCheck = true;
      frmData->IBQuery->SQL->Text =
      " SELECT DEVICESMENUS.MENU_NAME "
      " FROM DEVICESMENUS "
      " WHERE "
      " DEVICESMENUS.DEVICE_KEY = :THIS_DEVICE";
      frmData->IBQuery->Params->ParamByName("THIS_DEVICE")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
      frmData->IBQuery->Open();
      for (frmData->IBQuery->First(); !frmData->IBQuery->Eof ;frmData->IBQuery->Next())
      {
         CurrentMenus->Menu_Names[frmData->IBQuery->FieldByName("MENU_NAME")->AsString] = eMenuAddReplace;
      }

		if (frmData->IBTransaction->InTransaction && StartTrans) { frmData->IBTransaction->Commit(); }
	}
	catch(Exception &E)
	{
		if (frmData->IBTransaction->InTransaction && StartTrans) { frmData->IBTransaction->Rollback(); }
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}


#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------







