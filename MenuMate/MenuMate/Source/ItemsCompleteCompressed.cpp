
//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemsCompleteCompressed.h"
#include "ItemRedirector.h"
#include "ItemComplete.h"
#include "ContainerOrders.h"
#include "ItemCompleteSub.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TItemsCompleteCompressed::TItemsCompleteCompressed(TContainerOrders *inContainer)
{
   Container = inContainer;
	ServingCourseKey = 0;
	Type = icIncompleteItems;
	Display = new TStringList;
	fItemsList = new TList;
   Visible = true;
}
//---------------------------------------------------------------------------
__fastcall TItemsCompleteCompressed::~TItemsCompleteCompressed()
{
	while(Display->Count != 0)
	{
		delete (TItemRedirector *) Display->Objects[0];
		Display->Delete(0);
	}
	delete Display;
	delete fItemsList;
}

//---------------------------------------------------------------------------
bool TItemsCompleteCompressed::Contains(TItemMinorComplete *ItemToFind)
{
	if(fItemsList->IndexOf(ItemToFind) == -1)
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
int TItemsCompleteCompressed::Add(TItemMinorComplete * Item)
{
	return fItemsList->Add(Item);
}
//---------------------------------------------------------------------------
int TItemsCompleteCompressed::Remove(TItemMinorComplete * Item)
{
	return fItemsList->Remove(Item);
}

Currency TItemsCompleteCompressed::GetQty()
{
   Currency Qty = 0;

   for (int i = 0; i < fItemsList->Count ; i ++)
   {
//	   TItemMinorComplete *ItemMinorComplete = (TItemMinorComplete *)ItemsList[0];
	  Qty += ItemsList[i]->GetQty();
   }
   return Qty;
}

//---------------------------------------------------------------------------
void TItemsCompleteCompressed::BuildSelf()
{
	clearDisplayFormat();

	if (Type == icServingCourse)
	{
		formatServingCourse();
	}
	else if (Type == icCompleteItems)
	{
		// builds pos items and set menus
		formatCompleteItem();
	}
	else
	{
		/*
		 * this method does not seem to getting executed from the tests performed.
		 * however given the nature of the system, it might be called from somewhere sometime.
		 */

 		formatOtherItem();
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::clearDisplayFormat()
{
	/* Ok the ListOptions and ListItems are going to have to be very similar to
	TListItemType as in ShowSeatOrders() */
	while (Display->Count != 0)
	{
		delete(TItemRedirector*)Display->Objects[0];
		Display->Delete(0);
	}

	Display->Clear();
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatServingCourse()
{
	if (Visible)
	{
		TItemComplete *ItemComplete = (TItemComplete*)ItemsList[0];
		TItemRedirector *ItemRedirect = new TItemRedirector(this);
		ItemRedirect->ParentRedirector = ItemRedirect;
		ItemRedirect->ItemType << itServingCourseDisplay;
		ItemRedirect->ItemObject = ItemsList[0];
		Display->AddObject(ItemComplete->ServingCourse.Name, ItemRedirect);
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatCompleteItem()
{
	TItemComplete *ItemComplete = (TItemComplete*)ItemsList[0];
    if(ItemComplete->Terminal == "DC_Term")                                        // Checks if the item has arrived from Drink Command
    {
      dc_item = true ;

    }
    else
    {
      dc_item = false;
    }
	if (ItemComplete->SetMenuMaster)
	{
		formatSetMenuMaster( ItemComplete );
	}
	else if (!ItemComplete->SetMenu)
	{
		formatNormalItem( ItemComplete );
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatOtherItem()
{
	TItemMinorComplete *ItemComplete = ItemsList[0];
	if (ItemComplete->SetMenuMaster)
	{
		TItemRedirector *ItemRedirect = new TItemRedirector(this);
		ItemRedirect->ParentRedirector = ItemRedirect;
		ItemRedirect->ItemType << itSetMenuMasterItem;
		if (Container->IndexPrev(ItemComplete) != -1)
		{
			ItemRedirect->ItemType << itPrevItem;
		}
		ItemRedirect->ItemObject = ItemsList[0];

		UnicodeString ItemCount = Container->UnRavelDisplay ? UnicodeString("") : UnicodeString(FormatFloat("0.###", Qty) + "x ");
		UnicodeString SizeString = ItemComplete->Size;

		if (ItemComplete->Size.UpperCase() == "DEFAULT")
		{
			SizeString = "";
		}
		Display->AddObject(ItemCount + SizeString + " " + ItemComplete->Item, ItemRedirect);

		for (int j = 0; j < Container->Count; j++) // Lists all New Items Ordered.
		{
			// See if any items belongs to this key
			if (Container->Items[j]->SetMenu && (ItemComplete->SetMenuGroup == Container->Items[j]->SetMenuGroup))
			{
				TItemRedirector *SetMenuRedirector = new TItemRedirector(this);
				SetMenuRedirector->ItemType << itPromoItem;
				if (Container->IndexPrev(Container->Items[j]) != -1)
				{
					SetMenuRedirector->ItemType << itPrevItem;
				}
				else
				{
					SetMenuRedirector->ItemType << itNormalItem;
				}

				SetMenuRedirector->ParentRedirector = ItemRedirect;
				SetMenuRedirector->ItemObject = Container->Items[j];

				if (Container->Items[j]->ItemOrderedFrom->Sizes->Count > 1 && Container->Items[j]->Size.UpperCase() != "DEFAULT")
				{
					Display->AddObject("[" + Container->Items[j]->ServingCourse.Name + "] " + Container->Items[j]->Size + " " + Container->Items[j]->Item, SetMenuRedirector);
				}
				else
				{
					Display->AddObject("[" + Container->Items[j]->ServingCourse.Name + "] " + Container->Items[j]->Item, SetMenuRedirector);
				}

				// Show options
				if (Container->Items[j]->OptionsSelected->Count > 0)
				{
					for (UINT i = 0; i < Container->Items[j]->OptionsSelected->Count; i++)
					{
						TItemRedirector *OptionRedirect = new TItemRedirector(this);
						if (Container->IndexPrev(Container->Items[j]) != -1)
						{
							OptionRedirect->ItemType << itPrevItem;
						}
						OptionRedirect->ItemType << itOption;

						OptionRedirect->ParentRedirector = SetMenuRedirector;
						OptionRedirect->ItemObject = Container->Items[j]->OptionsSelected->OptionGet(i);
						OptionRedirect->OptList = Container->Items[j]->OptionsSelected;

						if (Container->Items[j]->OptionsSelected->OptionGet(i)->IsPlus)
						{
							Display->AddObject("      + " + Container->Items[j]->OptionsSelected->OptionGet(i)->Name, OptionRedirect);
						}
						else
						{
							Display->AddObject("      - " + Container->Items[j]->OptionsSelected->OptionGet(i)->Name, OptionRedirect);
						}
					}
				}

				for (UINT k = 0; k < Container->Items[j]->SubOrders->Count; k++)
				{
					TItemCompleteSub * SubOrder = ((TItemCompleteSub*)Container->Items[j]->SubOrders->Items[k]);
					TItemRedirector *SubItemRedirect = new TItemRedirector(this);
					SubItemRedirect->ItemType << itSubItem;
					if (Container->IndexPrev(Container->Items[j]) != -1)
					{
						SubItemRedirect->ItemType << itPrevItem;
					}
					SubItemRedirect->ParentRedirector = SetMenuRedirector;
					SubItemRedirect->ItemObject = Container->Items[j]->SubOrders->Items[k];
					SubItemRedirect->SubOrdersList = Container->Items[j]->SubOrders;

					UnicodeString ItemCount = UnicodeString(FormatFloat("0.##", SubOrder->GetQty()) + "x ");
					if (SubOrder->Size.UpperCase() != "DEFAULT")
					{

						Display->AddObject("  " + ItemCount + SubOrder->Size + " " + SubOrder->Item, SubItemRedirect);
					}
					else
					{
						Display->AddObject("  " + ItemCount + SubOrder->Item, SubItemRedirect);
					}
				}
			}
		}
		// Display all Prev Ordered Set Menu Items.
		for (int j = 0; j < Container->PrevCount; j++) // Lists all New Items Ordered.
		{
			// See if any items belongs to this key
			if (Container->PrevItems[j]->SetMenu && (ItemComplete->SetMenuGroup == Container->PrevItems[j]->SetMenuGroup))
			{
				TItemRedirector *SetMenuRedirector = new TItemRedirector(this);
				SetMenuRedirector->ItemType << itPromoItem;
				SetMenuRedirector->ItemType << itPrevItem;

				SetMenuRedirector->ParentRedirector = ItemRedirect;
				SetMenuRedirector->ItemObject = Container->PrevItems[j];

				if (Container->PrevItems[j]->Size.UpperCase() != "DEFAULT")
				{
					Display->AddObject("[" + Container->PrevItems[j]->ServingCourse.Name + "] " + Container->PrevItems[j]->Size + " " + Container->PrevItems[j]->Item, SetMenuRedirector);
				}
				else
				{
					Display->AddObject("[" + Container->PrevItems[j]->ServingCourse.Name + "] " + Container->PrevItems[j]->Item, SetMenuRedirector);
				}

				for (UINT k = 0; k < Container->PrevItems[j]->SubOrders->Count; k++)
				{
					TItemCompleteSub * SubOrder = ((TItemCompleteSub*)Container->PrevItems[j]->SubOrders->Items[k]);
					TItemRedirector *SubItemRedirect = new TItemRedirector(this);
					SubItemRedirect->ItemType << itSubItem;
					if (Container->IndexPrev(Container->PrevItems[j]) != -1)
					{
						SubItemRedirect->ItemType << itPrevItem;
					}
					SubItemRedirect->ParentRedirector = SetMenuRedirector;
					SubItemRedirect->ItemObject = Container->PrevItems[j]->SubOrders->Items[k];
					SubItemRedirect->SubOrdersList = Container->PrevItems[j]->SubOrders;

					UnicodeString ItemCount = UnicodeString(FormatFloat("0.##", SubOrder->GetQty()) + "x ");
					if (SubOrder->Size.UpperCase() != "DEFAULT")
					{

						Display->AddObject("  " + ItemCount + SubOrder->Size + " " + SubOrder->Item, SubItemRedirect);
					}
					else
					{
						Display->AddObject("  " + ItemCount + SubOrder->Item, SubItemRedirect);
					}
				}
			}
		}
	}
	else if (!ItemComplete->SetMenu || (ItemComplete->SetMenu && ItemComplete->SetMenuGroup == 0))
	{
		TItemRedirector *ItemRedirect = new TItemRedirector(this);
		ItemRedirect->ParentRedirector = ItemRedirect;
		if (Container->IndexPrev(ItemComplete) != -1 )
		{
			ItemRedirect->ItemType << itPrevItem;
		}
		else
		{
			ItemRedirect->ItemType << itNormalItem;
		}

		// All objects in this list are the same point to the last one.
		ItemRedirect->ItemObject = (TItemComplete*)ItemsList[Count - 1];

		UnicodeString ItemCount = Container->UnRavelDisplay ? UnicodeString("") : UnicodeString(FormatFloat("0.###", Qty) + "x ");
		UnicodeString SizeString = ItemComplete->Size;

		if (ItemComplete->Size.UpperCase() == "DEFAULT")
		{
			SizeString = "";
		}
		Display->AddObject(ItemCount + SizeString + " " + ItemComplete->Item, ItemRedirect);

		for (UINT j = 0; j < ItemComplete->SubOrders->Count; j++)
		{
			TItemCompleteSub *SubOrder = ((TItemCompleteSub*)ItemComplete->SubOrders->Items[j]);
			TItemRedirector *SubItemRedirect = new TItemRedirector(this);
			SubItemRedirect->ParentRedirector = ItemRedirect;
			if (Container->IndexPrev(ItemComplete) != -1)
			{
				SubItemRedirect->ItemType << itPrevItem;
			}

			SubItemRedirect->ItemType << itSubItem;
			SubItemRedirect->ItemObject = ItemComplete->SubOrders->Items[j];
			SubItemRedirect->SubOrdersList = ItemComplete->SubOrders;

			UnicodeString ItemCount = UnicodeString(FormatFloat("0.##", SubOrder->GetQty()) + "x ");
			if (SubOrder->Size.UpperCase() != "DEFAULT")
			{

				Display->AddObject("  " + ItemCount + SubOrder->Size + " " + SubOrder->Item, SubItemRedirect);
			}
			else
			{
				Display->AddObject("  " + ItemCount + SubOrder->Item, SubItemRedirect);
			}
		}
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatSetMenuMaster(TItemComplete* ItemComplete)
{
	TItemRedirector *ItemRedirect = new TItemRedirector(this);
	ItemRedirect->ParentRedirector = ItemRedirect;
	ItemRedirect->ItemType << itSetMenuMasterItem;
	if (Container->IndexPrev(ItemComplete) != -1)
	{
		ItemRedirect->ItemType << itPrevItem;
	}
	ItemRedirect->ItemObject = ItemsList[0];

	UnicodeString ItemCount = Container->UnRavelDisplay ? UnicodeString("") : UnicodeString(FormatFloat("0.###", Qty) + "x ");
	UnicodeString SizeString = ItemComplete->Size;

	if (ItemComplete->ItemOrderedFrom->Sizes->Count == 1 || ItemComplete->Size.UpperCase() == "DEFAULT")
	{
		SizeString = "";
	}
	Display->AddObject(ItemCount + SizeString + " " + ItemComplete->Item, ItemRedirect);

	for (int j = 0; j < Container->Count; j++)
	{
		// See if any items belongs to this key
		if (Container->Items[j]->SetMenu && (ItemComplete->SetMenuGroup == Container->Items[j]->SetMenuGroup))
		{
			TItemRedirector *SetMenuRedirector = new TItemRedirector(this);
			SetMenuRedirector->ItemType << itPromoItem;
			if (Container->IndexPrev(Container->Items[j]) != -1)
			{
				SetMenuRedirector->ItemType << itPrevItem;
			}
			else
			{
				SetMenuRedirector->ItemType << itNormalItem;
			}
			SetMenuRedirector->ParentRedirector = ItemRedirect;
			SetMenuRedirector->ItemObject = Container->Items[j];

			if (Container->Items[j]->ItemOrderedFrom->Sizes->Count > 1 && Container->Items[j]->Size.UpperCase() != "DEFAULT")
			{
				Display->AddObject("[" + Container->Items[j]->ServingCourse.Name + "] " + Container->Items[j]->Size + " " + Container->Items[j]->Item, SetMenuRedirector);
			}
			else
			{
				Display->AddObject("[" + Container->Items[j]->ServingCourse.Name + "] " + Container->Items[j]->Item, SetMenuRedirector);
			}

			// Show options
			if (Container->Items[j]->OptionsSelected->Count > 0)
			{
				for (UINT i = 0; i < Container->Items[j]->OptionsSelected->Count; i++)
				{
					TItemRedirector *OptionRedirect = new TItemRedirector(this);
					OptionRedirect->ItemType << itOption;
					OptionRedirect->ParentRedirector = SetMenuRedirector;
					OptionRedirect->ItemObject = Container->Items[j]->OptionsSelected->OptionGet(i);
					OptionRedirect->OptList = Container->Items[j]->OptionsSelected;
					if (Container->Items[j]->OptionsSelected->OptionGet(i)->IsPlus)
					{
						Display->AddObject("      + " + Container->Items[j]->OptionsSelected->OptionGet(i)->Name, OptionRedirect);
					}
					else
					{
						Display->AddObject("      - " + Container->Items[j]->OptionsSelected->OptionGet(i)->Name, OptionRedirect);
					}
				}
			}

			for (UINT k = 0; k < Container->Items[j]->SubOrders->Count; k++)
			{
				TItemRedirector *SubItemRedirect = new TItemRedirector(this);
				SubItemRedirect->ItemType << itSubItem;
				SubItemRedirect->ParentRedirector = SetMenuRedirector;
				SubItemRedirect->ItemObject = Container->Items[j]->SubOrders->Items[k];
				SubItemRedirect->SubOrdersList = Container->Items[j]->SubOrders;
				if (((TItemCompleteSub*)Container->Items[j]->SubOrders->Items[k])->Size.UpperCase() != "DEFAULT")
				{
					Display->AddObject("    " + ((TItemCompleteSub*)Container->Items[j]->SubOrders->Items[k])->Size + " " + ((TItemCompleteSub*)Container->Items[j]->SubOrders->Items[k])->Item,
						SubItemRedirect);
				}
				else
				{
					Display->AddObject("    " + ((TItemCompleteSub*)Container->Items[j]->SubOrders->Items[k])->Item, SubItemRedirect);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatNormalItem( TItemComplete* ItemComplete )
{
	TItemRedirector *ItemRedirect = new TItemRedirector(this);
	ItemRedirect->ParentRedirector = ItemRedirect;

	if (Container->IndexPrev(ItemComplete) != -1)
	{
		ItemRedirect->ItemType << itPrevItem;
	}
	else
	{
		if(dc_item)
        {
            ItemRedirect->ItemType << itPrevItem;
        }
        else
        {
            ItemRedirect->ItemType << itNormalItem;
        }
	}

	// All objects in this list are the same point to the last one.
	ItemRedirect->ItemObject = (TItemComplete*)ItemsList[Count - 1];

	if (TGlobalSettings::Instance().EnableNmiDisplay)
	{
		formatNormalItemNMI(ItemComplete, ItemRedirect);
	}
	else
	{
		if (ItemComplete->WeightedPrice.WeightedItem)
		{
			Display->AddObject(ItemComplete->Item + "\n" + UnicodeString(FormatFloat("0.000", ItemComplete->GetQty())) + " " + ItemComplete->WeightedPrice.Weight.GetUnitAsString() + " @ " + CurrToStrF
				(ItemComplete->PriceEach(), ffNumber, CurrencyDecimals), ItemRedirect);
		}
		else
		{
             UnicodeString ItemCount = ( !Container->UnRavelDisplay
                 && !ItemComplete->WeightedPrice.WeightedItem ) ? UnicodeString(FormatFloat("0.###", Qty) + "x ") : UnicodeString("");


			UnicodeString SizeString = ( ItemComplete->ItemOrderedFrom->Sizes->Count == 1
								|| ItemComplete->Size.UpperCase() == "DEFAULT" ) ? UnicodeString("") : ItemComplete->Size;

			Display->AddObject(ItemCount + SizeString + " " + ItemComplete->Item, ItemRedirect);
		}
	}

	// options
	formatOptionsForItem(ItemComplete, ItemRedirect);

	// sides
	formatSidesForItem(ItemComplete, ItemRedirect);
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatNormalItemNMI( TItemComplete* ItemComplete, TItemRedirector* ItemRedirect )
{
	UnicodeString formattedOutput = "";
	
	if( ItemComplete->WeightedPrice.WeightedItem )
	{
		formattedOutput = ItemComplete->Item
							+ "\n"
							+ UnicodeString(FormatFloat("0.000", ItemComplete->GetQty()))
							+ " "
							+ ItemComplete->WeightedPrice.Weight.GetUnitAsString();

		bool hasTareWeight = ItemComplete->WeightedPrice.TareWeight.Valid()
								&& ItemComplete->WeightedPrice.TareWeight.ToDouble() > 0.00;

		// if the weighted item has tare weight, format it accordingly
		formattedOutput += hasTareWeight ? UnicodeString(" N") : UnicodeString("");

		formattedOutput += " @ "
							+ getCurrencySymbol()
							+ CurrToStrF(ItemComplete->PriceEach(), ffNumber, CurrencyDecimals)
							+ "/"
                            + ItemComplete->WeightedPrice.Weight.GetUnitAsString();
	}
	else
	{
		if (ItemComplete->IsPriceBarcodedItem)
		{
			// price barcoded items are displayed same as weighted items
			formattedOutput = ItemComplete->Item 
								+ "\n" 
								+ FormatFloat("0.###", ItemComplete->GetQty())
								+ " kg @ "
								+ getCurrencySymbol() 
								+ CurrToStrF(ItemComplete->PriceEach(), ffNumber, CurrencyDecimals) 
								+ "/kg" ;
		}
		else
		{
			UnicodeString SizeString = ItemComplete->ItemOrderedFrom->Sizes->Count == 1
										|| ItemComplete->Size.UpperCase() == "DEFAULT" ? UnicodeString("") : ItemComplete->Size + " ";

			// items with qty are displayed in an special way
			formattedOutput = SizeString
								+ ItemComplete->Item 
								+ "\n" 
								+ "qty "
								+ FormatFloat("0.###", ItemComplete->GetQty())
								+ " @ "
								+ getCurrencySymbol() 
								+ ItemComplete->PriceEach() 
								+ " each";
		}
	}

	Display->AddObject( formattedOutput, ItemRedirect);
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatOptionsForItem(TItemComplete * ItemComplete, TItemRedirector* ItemRedirect)
{
	if (ItemComplete->OptionsSelected->Count > 0)
	{
		for (UINT j = 0; j < ItemComplete->OptionsSelected->Count; j++)
		{
			TItemRedirector *OptionRedirect = new TItemRedirector(this);
			OptionRedirect->ParentRedirector = ItemRedirect;
			OptionRedirect->ItemType << itOption;
			if (Container->IndexPrev(ItemComplete) != -1)
			{
				OptionRedirect->ItemType << itPrevItem;
			}
			OptionRedirect->ItemObject = ItemComplete->OptionsSelected->OptionGet(j);
			OptionRedirect->OptList = ItemComplete->OptionsSelected;
			if (ItemComplete->OptionsSelected->OptionGet(j)->IsPlus)
			{
				Display->AddObject("      + " + ItemComplete->OptionsSelected->OptionGet(j)->Name, OptionRedirect);
			}
			else
			{
				Display->AddObject("      - " + ItemComplete->OptionsSelected->OptionGet(j)->Name, OptionRedirect);
			}
		}
	}
}
//---------------------------------------------------------------------------

void TItemsCompleteCompressed::formatSidesForItem(TItemComplete* ItemComplete, TItemRedirector* ItemRedirect)
{
	for (UINT j = 0; j < ItemComplete->SubOrders->Count; j++)
	{
		TItemCompleteSub * SubOrder = (TItemCompleteSub*)ItemComplete->SubOrders->Items[j];
		TItemRedirector *SubItemRedirect = new TItemRedirector(this);
		SubItemRedirect->ParentRedirector = ItemRedirect;
		SubItemRedirect->ItemType << itSubItem;
		if (Container->IndexPrev(ItemComplete) != -1)
		{
			SubItemRedirect->ItemType << itPrevItem;
		}

		SubItemRedirect->ItemObject = ItemComplete->SubOrders->Items[j];
		SubItemRedirect->SubOrdersList = ItemComplete->SubOrders;
      if (SubOrder->WeightedPrice.WeightedItem)
		{
			Display->AddObject("  " + SubOrder->Item + "\n" + "  " + UnicodeString(FormatFloat("0.000", SubOrder->GetQty())) + " " + SubOrder->WeightedPrice.Weight.GetUnitAsString() + " @ " + CurrToStrF
				(SubOrder->PriceEach(), ffNumber, CurrencyDecimals), SubItemRedirect);
		}
		else
		{
			UnicodeString ItemCount = ( !Container->UnRavelDisplay
								&& !SubOrder->WeightedPrice.WeightedItem ) ? UnicodeString(FormatFloat("0.###", SubOrder->GetQty()) + "x ") : UnicodeString("");

			UnicodeString SizeString = (  SubOrder->Size.UpperCase() == "DEFAULT" ) ? UnicodeString("") : SubOrder->Size;

			Display->AddObject("  " + ItemCount + SizeString + " " + SubOrder->Item, SubItemRedirect);
		}
		/*UnicodeString ItemCount = UnicodeString(FormatFloat("0.##", SubOrder->GetQty()) + "x ");
		if (SubOrder->Size.UpperCase() != "DEFAULT")
		{

			Display->AddObject("  " + ItemCount + SubOrder->Size + " " + SubOrder->Item, SubItemRedirect);
		}
		else
		{
			Display->AddObject("  " + ItemCount + SubOrder->Item, SubItemRedirect);
		}*/
	}
}
//---------------------------------------------------------------------------

UnicodeString TItemsCompleteCompressed::getCurrencySymbol()
{
	return CurrencyString;
}
//---------------------------------------------------------------------------
