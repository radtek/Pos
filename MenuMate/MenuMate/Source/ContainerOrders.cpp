//---------------------------------------------------------------------------


#pragma hdrstop

#include "ContainerOrders.h"
#include "GlobalSettings.h"
#include "rounding.h"
#include "SelectDish.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

extern int OptionsCompare(TListOptionContainer * Options1, TListOptionContainer * Options2);
extern int __fastcall SubOrdersCompare(TListSubOrderContainer * List1, TListSubOrderContainer * List2);


int __fastcall OrderListSortFunc(void *Item1,void *Item2)
{
	TItemComplete *Order1 = (TItemComplete *)Item1;
	TItemComplete *Order2 = (TItemComplete *)Item2;
   // Items whos serving course is hidden are at the top.
	if (Order1->ServingCourse.Selectable > Order2->ServingCourse.Selectable)
	{
		return 1;
	}
	else if (Order1->ServingCourse.Selectable == Order2->ServingCourse.Selectable)
	{
		if (Order1->ServingCourse.SCOO > Order2->ServingCourse.SCOO)
		{
			return 1;
		}
		else if (Order1->ServingCourse.SCOO == Order2->ServingCourse.SCOO)
		{
			if (Order1->ServingCourse.ServingCourseKey > Order2->ServingCourse.ServingCourseKey)
			{
				return 1;
			}
			else if (Order1->ServingCourse.ServingCourseKey == Order2->ServingCourse.ServingCourseKey)
			{
				if (Order1->Item > Order2->Item)
				{
					return 1;
				}
				else if (Order1->Item == Order2->Item)
				{
					if (Order1->Size > Order2->Size)
					{
						return 1;
					}
					else if (Order1->Size == Order2->Size)
					{
						int RetVal = OptionsCompare(Order1->OptionsSelected,Order2->OptionsSelected); // Overloaded Function.
						if(RetVal != 0)
						{
							return RetVal;
						}
						else
						{
							RetVal = SubOrdersCompare(Order1->SubOrders,Order2->SubOrders); // Overloaded Function.
							if(RetVal != 0)
							{
								return RetVal;
							}
							else
							{
								if (Order1->TimeStamp < Order2->TimeStamp)
								{
									return 1;
								}
								else if (Order1->TimeStamp == Order2->TimeStamp)
								{
									if (Order1->SubOrders->Count < Order2->SubOrders->Count)
									{
										return 1;
									}
									else if ( Order1->SubOrders->Count == Order2->SubOrders->Count )
									{
										if (Order1->OrderType > Order2->OrderType)
										{
											return 1;
										}
										else if (Order1->OrderType == Order2->OrderType)
										{
											if (Order1->TotalPriceAdjustment() > Order2->TotalPriceAdjustment())

											{
												return 1;
											}
											else if (Order1->TotalPriceAdjustment() == Order2->TotalPriceAdjustment())
											{
												return 0;
											}
											else
											{
												return -1;
											}
										}
										else
										{
											return -1;
										}
									}
									else
									{
										return -1;
									}
								}
								else
								{
									return -1;
								}
							}
						}
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------

bool seperateNmiPriceBarcodedItem( TItemMinorComplete* CurrentItem )
{
	bool result = false;

	result = TGlobalSettings::Instance().EnableNmiDisplay
				&& CurrentItem->IsPriceBarcodedItem;

	return result;
}
//---------------------------------------------------------------------------

bool soloitem(TItemComplete *CurrentItem)
{
	if((CurrentItem->OrderType != NormalOrder)
		|| (CurrentItem->SetMenuMaster)
		|| (CurrentItem->SubOrders->Count != 0)
		|| (CurrentItem->WeightedPrice.WeightedItem == true)
		|| ( seperateNmiPriceBarcodedItem(CurrentItem)))	// checking to seperate nmi price barcode items
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool soloitem(TItemMinorComplete *CurrentItem)
{
	if((CurrentItem->SetMenuMaster)
		|| (CurrentItem->SubOrders->Count != 0)
		|| (CurrentItem->WeightedPrice.WeightedItem == true)
		|| ( seperateNmiPriceBarcodedItem(CurrentItem)))   // checking to seperate nmi price barcode items
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool match(TItemComplete *PrevItem,TItemComplete *CurrentItem )
{
   if((CurrentItem->Item != PrevItem->Item) ||
      (PrevItem->Note != CurrentItem->Note) ||
      (PrevItem->Size != CurrentItem->Size) ||
      (PrevItem->TotalPriceAdjustment() != CurrentItem->TotalPriceAdjustment()) ||
      (PrevItem->SetMenu != CurrentItem->SetMenu) ||
		(PrevItem->SetMenuGroup != CurrentItem->SetMenuGroup) ||
      (PrevItem->ServingCourse.ServingCourseKey != CurrentItem->ServingCourse.ServingCourseKey) ||
      (CurrentItem->Item_ID != PrevItem->Item_ID) ||
      (!CurrentItem->OptionsSelected->Compare(PrevItem->OptionsSelected)) ||
      (!CurrentItem->SubOrders->Compare(PrevItem->SubOrders))
      ) //
   {  // Match Failed
		return false;
	}
	return true;

}

//---------------------------------------------------------------------------
bool match(TItemMinorComplete *PrevItem,TItemMinorComplete *CurrentItem )
{
   if((CurrentItem->Item != PrevItem->Item) ||
      (PrevItem->Size != CurrentItem->Size) ||
      (PrevItem->TotalPriceAdjustment() != CurrentItem->TotalPriceAdjustment()) ||
      (!CurrentItem->SubOrders->Compare(PrevItem->SubOrders))
      ) //
   {  // Match Failed
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------


void __fastcall TContainerOrders::Compress()
{
	while(fCompressedItemsList->Count != 0)
	{
		delete (TItemsCompleteCompressed *) fCompressedItemsList->Items[0];
		fCompressedItemsList->Delete(0);
	}
	if(AppliedMembership.ContactKey != 0)
	{
		TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
		fCompressedItemsList->Add(CompressedItem);
		CompressedItem->Type = icMemberInfo;

		TItemRedirector *ItemParentRedirect = new TItemRedirector(CompressedItem);
		ItemParentRedirect->ParentRedirector = ItemParentRedirect;
		ItemParentRedirect->ItemType << itMembershipDisplay;
		ItemParentRedirect->ItemObject = NULL;
    if((TGlobalSettings::Instance().MembershipType==MembershipTypeThor) && (TGlobalSettings::Instance().IsThorlinkSelected))
    {
       CompressedItem->Display->AddObject(AppliedMembership.Name, ItemParentRedirect);
    }
    else
    {
       CompressedItem->Display->AddObject(AppliedMembership.Name +" "+ AppliedMembership.Surname +" (" + AppliedMembership.MembershipNumber + ")" , ItemParentRedirect);
    }

		if(TGlobalSettings::Instance().EnableSeperateEarntPts)
		{
			TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
			fCompressedItemsList->Add(CompressedItem);
			CompressedItem->Type = icMemberInfo;

			TItemRedirector *ItemParentRedirect = new TItemRedirector(CompressedItem);
			ItemParentRedirect->ParentRedirector = ItemParentRedirect;
			ItemParentRedirect->ItemType << itEarntPts;
			ItemParentRedirect->ItemObject = NULL;

			CompressedItem->Display->AddObject("Earned Points", ItemParentRedirect);
		}
		if(TGlobalSettings::Instance().EnableSeperateEarntPts)
		{
			TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
			fCompressedItemsList->Add(CompressedItem);
			CompressedItem->Type = icMemberInfo;

			TItemRedirector *ItemParentRedirect = new TItemRedirector(CompressedItem);
			ItemParentRedirect->ParentRedirector = ItemParentRedirect;
			ItemParentRedirect->ItemType << itLoadedPts;
			ItemParentRedirect->ItemObject = NULL;

			CompressedItem->Display->AddObject("Loaded Points", ItemParentRedirect);
		}

      if(AppliedMembership.Note != "")
      {
         TItemRedirector *ItemRedirect = new TItemRedirector(CompressedItem);
         ItemRedirect->ParentRedirector = ItemParentRedirect;
         ItemRedirect->ItemType << itMembershipDisplayNote;
         ItemRedirect->ItemObject = NULL;
         CompressedItem->Display->AddObject(AppliedMembership.Note, ItemRedirect);
      }


	}

	if(Count > 0)
	{
		TItemComplete *PrevItem = NULL;
		for (int i = 0; i < Count;)
		{
			TItemComplete *CurrentItem = Items[i];
			if(!HasServingCourseHeader(CurrentItem))
			{
				// Addin a ServingCourseHeader.
				TItemsCompleteCompressed *ServingHeader = new TItemsCompleteCompressed(this);
				fCompressedItemsList->Add(ServingHeader);
				ServingHeader->Type = icServingCourse;
				ServingHeader->ServingCourseKey = CurrentItem->ServingCourse.ServingCourseKey;
				ServingHeader->Visible = CurrentItem->ServingCourse.Selectable;
				ServingHeader->Add(CurrentItem);
				ServingHeader->BuildSelf();
			}

			TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
			fCompressedItemsList->Add(CompressedItem);
			CompressedItem->Type = icCompleteItems;
			PrevItem = CurrentItem;
			if(soloitem(CurrentItem) || UnRavelDisplay)
			{
				CompressedItem->Add(PrevItem);
				CompressedItem->BuildSelf();
				i++;
			}
			else
			{
				while(match(PrevItem,CurrentItem) && (!soloitem(CurrentItem)) && (i <  fItems->Count))
				{
					 CompressedItem->Add(CurrentItem);
					 i++;
					 if(i < fItems->Count)
					 {
						CurrentItem = Items[i];
					 }
				}
			}

			CompressedItem->BuildSelf();
			if(CurrentItem == fItems->Items[fItems->Count-1] && (!match(PrevItem,CurrentItem)))
			{
				if(!HasServingCourseHeader(CurrentItem))
				{
					// Addin a ServingCourseHeader.
					TItemsCompleteCompressed *ServingHeader = new TItemsCompleteCompressed(this);
					fCompressedItemsList->Add(ServingHeader);
					ServingHeader->Type = icServingCourse;
					ServingHeader->ServingCourseKey = CurrentItem->ServingCourse.ServingCourseKey;
   				ServingHeader->Visible = CurrentItem->ServingCourse.Selectable;
					ServingHeader->Add(CurrentItem);
					ServingHeader->BuildSelf();
				}

				CompressedItem = new TItemsCompleteCompressed(this);
				fCompressedItemsList->Add(CompressedItem);
				CompressedItem->Type = icCompleteItems;
				CompressedItem->Add(CurrentItem);
				CompressedItem->BuildSelf();
				i++;
			}
		}
	}

	if(PrevCount > 0)
	{
		TItemMinorComplete *PrevItem = NULL;
		for (int i = 0; i < PrevCount;)
		{
			TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
			fCompressedItemsList->Add(CompressedItem);
			CompressedItem->Type = icIncompleteItems;
			TItemMinorComplete *CurrentItem = PrevItems[i];
			PrevItem = CurrentItem;
			if(soloitem(CurrentItem))
			{
				CompressedItem->Add(PrevItem);
				CompressedItem->BuildSelf();
				i++;
			}
			else
			{
				while(match(PrevItem,CurrentItem) && (!soloitem(CurrentItem)) && (i <  fPrevOrders->Count))
				{
					 CompressedItem->Add(CurrentItem);
					 i++;
					 if(i < fPrevOrders->Count)
					 {
						CurrentItem = PrevItems[i];
					 }
				}
			}

			CompressedItem->BuildSelf();
			if(CurrentItem == fPrevOrders->Items[fPrevOrders->Count-1] && (!match(PrevItem,CurrentItem)))
			{
				CompressedItem = new TItemsCompleteCompressed(this);
				fCompressedItemsList->Add(CompressedItem);
				CompressedItem->Type = icIncompleteItems;
				CompressedItem->Add(CurrentItem);
//            CompressedItem->Count++;
				CompressedItem->BuildSelf();
				i++;
			}
		}
	}

	TotalDiscount = 0;
	for (int i=0; i< Count; i++)
	{
		TItemMinorComplete *Order = Items[i];
                Currency discountValue = Order->TotalAdjustmentSides();
                TotalDiscount += discountValue;

                //bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown != discountValue < 0;
		//TotalDiscount += RoundToNearest(discountValue, 0.01, MidPointRoundsDown);
	}


	if(TotalDiscount != 0)
	{
		TItemsCompleteCompressed *CompressedItem = new TItemsCompleteCompressed(this);
		fCompressedItemsList->Add(CompressedItem);
		CompressedItem->Type = icDiscount;

		TItemRedirector *ItemParentRedirect = new TItemRedirector(CompressedItem);
		ItemParentRedirect->ParentRedirector = ItemParentRedirect;
		ItemParentRedirect->ItemType << itDiscountDisplay;
		ItemParentRedirect->ItemObject = &TotalDiscount;

		// Recalculate the Discount Amount.

		UnicodeString DisplayName = TotalDiscount < 0 ? "Discount ": "Surcharge";
		CompressedItem->Display->AddObject("Total " + DisplayName, ItemParentRedirect);
	}
}

__fastcall TContainerOrders::TContainerOrders()
{
	fItems = new TList;
	fPrevOrders = new TList;
	fCompressedItemsList = new TList;
	ptrSortFunc = OrderListSortFunc;
	SelectedIndex = -1;
	UnRavelDisplay = false;
	HideServingCourseLabels = false;
}
//---------------------------------------------------------------------------
__fastcall TContainerOrders::~TContainerOrders()
{
   delete fItems;

   while(fCompressedItemsList->Count != 0)
   {
      delete (TItemsCompleteCompressed *) fCompressedItemsList->Items[0];
      fCompressedItemsList->Delete(0);
	}
   delete fCompressedItemsList;

   while(fPrevOrders->Count != 0)
   {
      delete (TItemMinorComplete *) fPrevOrders->Items[0];
      fPrevOrders->Delete(0);
   }
   delete fPrevOrders;
}
//---------------------------------------------------------------------------
int TContainerOrders::Add(TItemComplete *Item, TItem *Det)
{
	Item->ItemOrderedFrom = Det;
	LastItemSelected = Item;
   LastItemAdded = Item;
	return fItems->Add(Item);







/*	if(!soloitem(Item) )
	{
      TItemComplete *Match = NULL;
      for (int i = 0; i < Count; i++)
      {
         if( Match == NULL && match(Items[i],Item))
         {
            Match = Items[i];
         }
      }
      if(Match != NULL)
      {
			Match->Add(Item);
         return fItems->IndexOf(Match);
      }
      else
      {
         Item->ItemOrderedFrom = Det;
         LastItemSelected = Item;
         LastItemAdded = Item;
         return fItems->Add(Item);
      }
	}
   else
   {
      Item->ItemOrderedFrom = Det;
      LastItemSelected = Item;
      LastItemAdded = Item;
      return fItems->Add(Item);
	}*/
}
//---------------------------------------------------------------------------
int TContainerOrders::Remove(TItemMinorComplete *Item)
{
   // The sole purpose for this code is to ensure the next Items selected is either
   // an item in the same stack or (not yet implemeted) the stack above this one.
   int index = 0;
	TItemsCompleteCompressed *CompressedItem = NULL;
   bool found = false;
   for(int i = 0 ; i < CompressedCount && !found; i++)
   {
      CompressedItem = CompressedItems[i];
      if(CompressedItem->Contains(Item))
      {
         index = CompressedItem->Remove(Item);
         found = true;
      }
   }

   // remove the Item from the list.
	int retval = fItems->Remove(Item);
   // Get the Item before this one.
	if(index != 0 && CompressedItem != NULL)
	{
		if(index < Count && (index != -1))
      {
   		LastItemSelected = CompressedItem->ItemsList[index-1];
      }
		else if(Count > 0)
		{
			LastItemSelected = Items[0];
		}
		else
		{
			LastItemSelected = NULL;
		}
	}
   else if(index == 0 && CompressedItem != NULL) // removed the last item of this compressed container.
   {
      int CompressedIndex = fCompressedItemsList->IndexOf(CompressedItem);
      CompressedIndex--; //Set CompressedIndex to the Compressed container before this one.
 		LastItemSelected = NULL;
		// Loop untill we find the last item, All or all lists are found to be empty
      while(LastItemSelected == NULL && CompressedIndex >= 0)
      {
         CompressedItem = CompressedItems[CompressedIndex];
         if(CompressedItem->Count != 0) // Yay there are Items in this List
         {
            LastItemSelected = CompressedItem->ItemsList[CompressedItem->Count-1];
         }
         else // this list is empty try the next one down
         {
            CompressedIndex--;
         }
      }
   }
	else
	{
		LastItemSelected = NULL;
	}
   LastItemAdded = NULL;
	return retval;









/*   int retval = -1;
	if(Item->GetQty() > 1)
   {
		Item->SetQty(Item->GetQty()-1);
   }
   else
   {
	   // The sole purpose for this code is to ensure the next Items selected is either
   	// an item in the same stack or (not yet implemeted) the stack above this one.

      int index = 0;
		TItemsCompleteCompressed *CompressedItem = NULL;
      bool found = false;
      for(int i = 0 ; i < CompressedCount && !found; i++)
      {
         CompressedItem = CompressedItems[i];
			if(CompressedItem->Contains(Item))
         {
            index = CompressedItem->Remove(Item);
            found = true;
         }
      }

      // remove the Item from the list.
      retval = fItems->Remove(Item);
      // Get the Item before this one.
      if(index != 0 && CompressedItem != NULL)
		{
         if(index < Count && (index != -1))
         {
            LastItemSelected = CompressedItem->ItemsList[index-1];
         }
         else if(Count > 0)
         {
            LastItemSelected = Items[0];
         }
         else
         {
            LastItemSelected = NULL;
         }
      }
      else if(index == 0 && CompressedItem != NULL) // removed the last item of this compressed container.
      {
         int CompressedIndex = fCompressedItemsList->IndexOf(CompressedItem);
         CompressedIndex--; //Set CompressedIndex to the Compressed container before this one.
         LastItemSelected = NULL;
         // Loop untill we find the last item, All or all lists are found to be empty
         while(LastItemSelected == NULL && CompressedIndex >= 0)
         {
            CompressedItem = CompressedItems[CompressedIndex];
            if(CompressedItem->Count != 0) // Yay there are Items in this List
            {
               LastItemSelected = CompressedItem->ItemsList[CompressedItem->Count-1];
				}
            else // this list is empty try the next one down
            {
               CompressedIndex--;
            }
         }
		}
      else
      {
         LastItemSelected = NULL;
      }
      LastItemAdded = NULL;
   }
	return retval;     */
}
//---------------------------------------------------------------------------
int TContainerOrders::Delete(TItemComplete *Item)
{
	// The sole purpose for this code is to ensure the next Items selected is either
   // an item in the same stack or (not yet implemeted) the stack above this one.
	int retval = -1;
   int index = 0;
   TItemsCompleteCompressed *CompressedItem = NULL;
   bool found = false;
   for(int i = 0 ; i < CompressedCount && !found; i++)
   {
      CompressedItem = CompressedItems[i];
      if(CompressedItem->Contains(Item))
      {
         index = CompressedItem->Remove(Item);
         found = true;
      }
   }

   // remove the Item from the list.
   retval = fItems->Remove(Item);
   // Get the Item before this one.
   if(index != 0 && CompressedItem != NULL)
   {
      if(index < Count && (index != -1))
		{
         LastItemSelected = CompressedItem->ItemsList[index-1];
      }
      else if(Count > 0)
      {
         LastItemSelected = Items[0];
      }
      else
      {
         LastItemSelected = NULL;
      }
   }
   else if(index == 0 && CompressedItem != NULL) // removed the last item of this compressed container.
   {
      int CompressedIndex = fCompressedItemsList->IndexOf(CompressedItem);
      CompressedIndex--; //Set CompressedIndex to the Compressed container before this one.
      LastItemSelected = NULL;
      // Loop untill we find the last item, All or all lists are found to be empty
      while(LastItemSelected == NULL && CompressedIndex >= 0)
      {
         CompressedItem = CompressedItems[CompressedIndex];
         if(CompressedItem->Count != 0) // Yay there are Items in this List
         {
            LastItemSelected = CompressedItem->ItemsList[CompressedItem->Count-1];
         }
         else // this list is empty try the next one down
         {
            CompressedIndex--;
         }
      }
	}
   else
   {
      LastItemSelected = NULL;
   }
   LastItemAdded = NULL;
	return retval;
}
//---------------------------------------------------------------------------

void TContainerOrders::Delete(int i)
{
   bool LastItemAddedRemoved = false;
   if(LastItemSelected == Items[i] )
   {
      LastItemAddedRemoved = true;
   }
   fItems->Delete(i);
   if(LastItemAddedRemoved && Count > 0)
   {
      LastItemSelected = Items[0];
   }
   else if(Count == 0)
   {
      LastItemSelected = NULL;
   }
}
//---------------------------------------------------------------------------
void __fastcall TContainerOrders::Sort()
{
	if(!UnRavelDisplay)
	{
		fItems->Sort(ptrSortFunc);
	}
}

void TContainerOrders::RefreshDisplay()
{
	UnRavelDisplay = TGlobalSettings::Instance().UnRavelKitchenPrinting;

	Sort();
	Compress();
	//Set the Current Item.
	bool found = false;
	int Index = 0;
   // Find the Current selected Item Redirector.
   for(int i = 0 ; i < CompressedCount && !found; i++)
   {
      TItemsCompleteCompressed *CompressedItem = CompressedItems[i];
      for(int j = 0 ; j < CompressedItem->Display->Count && !found; j++)
      {
         TItemRedirector *Redirector = (TItemRedirector *)CompressedItem->Display->Objects[j];
         if(CompressedItem->Count > 1)
			{
				if(CompressedItem->Contains(LastItemSelected))
				{
					CurrentItemRedirector = Redirector;
					SelectedIndex = Index;
					found = true;
				}
			}
			else if(Redirector->ItemObject == LastItemSelected)
			{
				SelectedIndex = Index;
				CurrentItemRedirector = Redirector;
				found = true;
			}
			Index++;
		}
	}
}
//---------------------------------------------------------------------------
bool TContainerOrders::HasServingCourseHeader(TItemComplete *CurrentItem)
{
	bool RetVal = false;

	if(HideServingCourseLabels)
	{ // Pretend it has a serving course header already so it dosnt draw one.
		RetVal = true;
	}
	else
	{
      if(CurrentItem->SetMenuMaster || !CurrentItem->SetMenu || (CurrentItem->SetMenu && CurrentItem->SetMenuGroup == 0) )
      {
         for (int i = 0; i < fCompressedItemsList->Count ; i++)
         {
            TItemsCompleteCompressed *ServingHeader = (TItemsCompleteCompressed *)fCompressedItemsList->Items[i];
            if(ServingHeader->ServingCourseKey == CurrentItem->ServingCourse.ServingCourseKey)
            {
               RetVal = true;
            }
         }
      }
      else
      {
			 RetVal = true;
      }
   }
	return RetVal;
}

void TContainerOrders::PutCurrentItemRedirector(TItemRedirector *in)
{
	fCurrentItemRedirector = in;
	if (in->ItemType.Contains(itSubItem))
	{
		if (in->ItemType.Contains(itPrevItem))
		{
		}
		else
		{
			LastItemSelected = (TItemMinorComplete *)in->ItemObject;
		}
	}
	else if (in->ItemType.Contains(itSetMenuMasterItem))
	{
		if (in->ItemType.Contains(itPrevItem))
		{
		}
		else
		{
			LastItemSelected = (TItemMinorComplete *)in->CompressedContainer->ItemsList[0];
		}
	}
	else if (in->ItemType.Contains(itPrevItem))
	{
	}
	else if (in->ItemType.Contains(itMembershipDisplay))
	{
	}
	else if (in->ItemType.Contains(itMembershipDisplayNote))
	{
	}
	else if (in->ItemType.Contains(itDiscountDisplay))
	{
	}
	else if (in->ItemType.Contains(itEarntPts))
	{
	}
	else if (in->ItemType.Contains(itLoadedPts))
	{
	}
	else if (in->ItemType.Contains(itOption))
	{
		LastItemSelected = (TItemMinorComplete *)in->CompressedContainer->ItemsList[0];
	}
	else if (in->ItemType.Contains(itPromoItem))
	{
		LastItemSelected = (TItemMinorComplete *)in->ItemObject;
	}
	else
	{
		LastItemSelected = (TItemMinorComplete *)in->CompressedContainer->ItemsList[0];
	}

	if(fOnCurrentItemRedirectorChanged != NULL)
	{
		fOnCurrentItemRedirectorChanged(in);
	}
}

void TContainerOrders::Select(TItemMinorComplete *Item)
{
	LastItemSelected = Item;
	RefreshDisplay();
}