// ---------------------------------------------------------------------------
#pragma hdrstop
#include "FreebieManager.h"
#include "MMLogging.h"
#include "ManagerVariable.h"
#include "MMMessageBox.h"
#include <Memory>

#include "StringTableRes.h"
#include <StrUtils.hpp>

#include "DBSecurity.h"
#include "DBOrder.h"


#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerFreebie *ManagerFreebie;

bool TManagerFreebie::IsFreeContact(Database::TDBTransaction &DBTransaction, TItemMinorComplete * Item)
{
	bool RetVal = false;
	if (Item->MemberFreeSaleCount == 0)
	{
		RetVal = false;
	}
	else if (Item->MemberFreeSaleCount == -1 && Item->Loyalty_Key != 0)
	{
		RetVal = true;
	}
	else if(Item->Loyalty_Key != 0)
	{
		try
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT ITEM_COUNT FROM CONTACTFREEBIE " "WHERE CONTACTS_KEY = :CONTACTS_KEY AND " "UPPER(ITEM_NAME) = :ITEM_NAME AND " "UPPER(SIZE_NAME) = :SIZE_NAME";
			IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Item->Loyalty_Key;
			IBInternalQuery->ParamByName("ITEM_NAME")->AsString = UpperCase(Item->Item);
			IBInternalQuery->ParamByName("SIZE_NAME")->AsString = UpperCase(Item->Size);
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount)
			{
				int QtyAccumlated = IBInternalQuery->FieldByName("ITEM_COUNT")->AsInteger;
				if (QtyAccumlated >= Item->MemberFreeSaleCount
                    && Item->GetQty() >= 1) // we need to check the quantity of the item  in question, as we only allow an item to be free item, if it is a whole
				{
					RetVal = true;
				}
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			throw;
		}

		if (RetVal)
		{
			MessageBox("Congratulations you have earned a discounted " + Item->Item, " Membership Free Item!", MB_OK + MB_ICONINFORMATION);
		}
	}
	return RetVal;
}

bool TManagerFreebie::IsFreeLocation(Database::TDBTransaction &DBTransaction, int LocationKey, TItemMinorComplete * Item)
{
	bool RetVal = false;
	if (Item->LocationFreeSaleCount == 0)
	{
		RetVal = false;
	}
	else if (Item->LocationFreeSaleCount == -1 && LocationKey != 0)
	{
		RetVal = true;
	}
	else
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		try
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT ITEM_COUNT FROM LOCATIONSFREEBIE " "WHERE LOCATION_KEY = :LOCATION_KEY AND " "UPPER(ITEM_NAME) = :ITEM_NAME AND " "UPPER(SIZE_NAME) = :SIZE_NAME";
			IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
			IBInternalQuery->ParamByName("ITEM_NAME")->AsString = UpperCase(Item->Item);
			IBInternalQuery->ParamByName("SIZE_NAME")->AsString = UpperCase(Item->Size);
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount)
			{
				int QtyAccumlated = IBInternalQuery->FieldByName("ITEM_COUNT")->AsInteger;
				if (QtyAccumlated >= Item->LocationFreeSaleCount)
				{
					RetVal = true;
				}
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			throw;
		}

		if (RetVal)
		{
			MessageBox("Congratulations you have won a discounted " + Item->Item, "Location Winner!", MB_OK + MB_ICONINFORMATION);
		}
	}
	return RetVal;
}

void TManagerFreebie::MakeFree(Database::TDBTransaction & DBTransaction, TItemMinorComplete * Item)
{
	if (Item->FreeSource == eFreeMember)
	{
		if (Item->Security->GetSecurityRefNumber() == 0)
		{
			Item->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
		}

		// Add Security.
		TSecurityReference *SecRef = new TSecurityReference;
		SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
		SecRef->Event = SecurityTypes[secMemberDiscount];
		SecRef->From = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, 2);

		/* We've got to make sure that any membership rewards currently placed
	    * on this item are removed before we go about adding another.
		 */
		Item->DiscountByTypeRemove(dsMMMembershipReward);

		TDiscount RewardDiscount;
		RewardDiscount.DiscountKey = -1;
		RewardDiscount.Priority = -1;
		RewardDiscount.Description = "Membership Reward";
		RewardDiscount.Name = "Member Reward";
		RewardDiscount.Source = dsMMMembershipReward;
		RewardDiscount.Mode = DiscModePercent;
        RewardDiscount.AppliedMode = DiscModePercent;
		RewardDiscount.PercentAmount = Item->MemberFreeSaleDiscount;
        RewardDiscount.OriginalAmount = Item->MemberFreeSaleDiscount;
		RewardDiscount.Rounding = MIN_CURRENCY_VALUE;
		Item->DiscountAdd(RewardDiscount);

		SecRef->To = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, 2);
		SecRef->Note = "Discount " + FloatToStr(Item->MemberFreeSaleDiscount) + "%";
		SecRef->TimeStamp = Now();
		Item->Security->SecurityAdd(SecRef);
	}
	else if (Item->FreeSource == eFreeLocation)
	{
		if (Item->Security->GetSecurityRefNumber() == 0)
		{
			Item->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
		}
		// Add Security.
		TSecurityReference *SecRef = new TSecurityReference;
		SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
		SecRef->Event = SecurityTypes[secLocationDiscount];
		SecRef->From = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, 2);

		TDiscount RewardDiscount;
		RewardDiscount.DiscountKey = -1;
		RewardDiscount.Priority = -1;
		RewardDiscount.Description = "Location Reward";
		RewardDiscount.Name = "Location Reward";
		RewardDiscount.Source = dsMMLocationReward;
		RewardDiscount.Mode = DiscModePercent;
        RewardDiscount.AppliedMode = DiscModePercent;
		RewardDiscount.PercentAmount = Item->LocationFreeSaleDiscount;
        RewardDiscount.OriginalAmount = Item->LocationFreeSaleDiscount;
		RewardDiscount.Rounding = MIN_CURRENCY_VALUE;
		Item->DiscountAdd(RewardDiscount);

		SecRef->To = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, 2);
		SecRef->Note = "Discount " + FloatToStr(Item->LocationFreeSaleDiscount) + "%";
		SecRef->TimeStamp = Now();
		Item->Security->SecurityAdd(SecRef);
	}
}

void TManagerFreebie::IsPurchasing(Database::TDBTransaction & DBTransaction,TList *OrderList)
{
	/* Because of the way the payment form works where if you cancel out of it it rolls back the
     DB transaction you could Add product go into payments, swipe card to add membership, that
	 would call this code and then when you canceled out of the payment screen it would roll the
	 transaction back, that would roll back the deduction of the members count and thus they
	 would get the discount with no reduction in there count allowing them to continuly get the
	 discount, in order to prevent that this function need to happen in it's own transaction.

     However in the Billing Screen the any existing membership needs to be removed before it is added
     as these order are stored inthe DB removing the membership touches them
     (so any membership discount is also removed), with that as the case when they are touch here
     as part of ::UpdateOrder inside it's own transaction, you get a deadlock.

     The design fault is in the DB roll back as part of cancilling out of the payment screen
     if we got rid of that this function could have the transaction passed in ass normal and
     everything would be ok.

     So where to from here, in the interm to get a fix to the customer we going to pass the tranaction
     into this function and live with the exploit cause by backing out of the payment screen.
     */

	for (int i = 0; i < OrderList->Count; i++)
	{
		// Loop though all the Items.
		TItemMinorComplete * Item = (TItemMinorComplete *)OrderList->Items[i];
		if (!Item->IsFreebe)
		{ // If it's free already it dosn't count.
			if (IsFreeContact(DBTransaction, Item))
			{
				if (Item->MemberFreeSaleCount == -1)
				{
					Item->IsFreebe = true;
					Item->FreeSource = eFreeMember;
					MakeFree(DBTransaction,Item);
                    TDBOrder::UpdateOrder(DBTransaction,Item);
                }
				else if (Item->GetQty() > 1)
				{
					/* Ok they are entilted to a certain discount ( may be 100%) on one of these items
					 * So we have to duplicate one and add it to the Items List and remove one from the
					 * original list */

					TItemComplete *Order = new TItemComplete();
           			Item->Assign(Order);
                    Order->IsFreebe = true;
					Order->FreeSource = eFreeMember;
                    Order->SetQty(1);

                	Order->TransNo	= TDBOrder::GetNextTransNumber(DBTransaction);

                    Item->SetQty(Item->GetQty() - 1);
                    MakeFree(DBTransaction,Order);

                    /* If this Item came from the DB (saved sale) then update the DB*/
                    if(Item->OrderKey != 0)
                    {
                    	TDBOrder::SetOrder(DBTransaction,Order);
                    	TDBOrder::UpdateOrder(DBTransaction,Item);
                    }
                    OrderList->Add(Order);
				}
				else if (Item->GetQty() == 1)
				{ /* Apply discount to this Item */
					Item->IsFreebe = true;
					Item->FreeSource = eFreeMember;
					MakeFree(DBTransaction,Item);
                    TDBOrder::UpdateOrder(DBTransaction,Item);
				}

				if(Item->MemberFreeSaleCount != -1)
				{
					AdjustFreeCountContact(DBTransaction, Item->Loyalty_Key, Item->Item, Item->Size, -Item->MemberFreeSaleCount,Item->MemberFreeSaleCount);
				}
			}
			else if (IsFreeLocation(DBTransaction,TDeviceRealTerminal::Instance().ID.LocationKey, Item))
			{
				double ItemQty = Item->GetQty();
				//if (Item->GetQty() > 1)	This is returning true when a single item has a location loyalty discount attached causing an extra item with qty 0 to be made
				if (ItemQty > 1)
				{
					/* Ok they are entilted to a certain discount ( may be 100%) on one of these items
					 * So we have to duplicate one and add it to the Items List and remove one from the
					 * original list */

					TItemComplete *Order = new TItemComplete();
           			Item->Assign(Order);
                    Order->IsFreebe = true;
					Order->FreeSource = eFreeLocation;
                    Order->SetQty(1);
                    Order->TransNo	= TDBOrder::GetNextTransNumber(DBTransaction);

                    Item->SetQty(Item->GetQty() - 1);
                    MakeFree(DBTransaction,Order);

                    /* If this Item came from the DB (saved sale) then update the DB*/
                    if(Item->OrderKey != 0)
                    {
                    	TDBOrder::SetOrder(DBTransaction,Order);
                    	TDBOrder::UpdateOrder(DBTransaction,Item);
                    }

                    OrderList->Add(Order);

				}
				//else if (Item->GetQty() == 1)
				else if (ItemQty == 1)
				{ /* Apply discount to this Item */
					Item->IsFreebe = true;
					Item->FreeSource = eFreeLocation;
                    MakeFree(DBTransaction,Item);
                    TDBOrder::UpdateOrder(DBTransaction,Item);
				}

                if (Item->LocationFreeSaleCount != 0 && Item->LocationFreeSaleCount != -1)
				{
 					AdjustFreeCountLocation(DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey, Item->Item, Item->Size, -Item->LocationFreeSaleCount,Item->LocationFreeSaleCount);
				}
			}
		}
        TManagerFreebie::IsPurchasing(DBTransaction,Item->SubOrders);
	}
}

void TManagerFreebie::UndoFreeCount(Database::TDBTransaction & DBTransaction, TList *OrderList)
{
	/* If this Item was awarded as free then Put the Item Free Count back against ther persons count so the next one they buy is
	free instead. We put the Count back because we deduct the freeCount amount (aka 5 or so) from there Acculmlated total
    each time one is awarded for free,  so in this case there count would go 1,2,3,4,5 -5 = 0 1,2,3,... etc*/
	for (int i = 0; i < OrderList->Count; i++)
	{
		// Loop though all the Items.
		TItemMinorComplete * Item = (TItemMinorComplete *)OrderList->Items[i];
        if (Item->IsFreebe)
        {
            if (Item->FreeSource == eFreeMember)
            {
                if (Item->Loyalty_Key != 0)
                {
                    AdjustFreeCountContact(DBTransaction, Item->Loyalty_Key, Item->Item, Item->Size, Item->MemberFreeSaleCount,Item->MemberFreeSaleCount);
                }
                Item->IsFreebe = false;
				Item->DiscountByTypeRemove(dsMMMembershipReward);
                Item->ResetPrice();
            	if(Item->OrderKey != 0)
                {
                    TDBOrder::UpdateOrder(DBTransaction,Item);
                }
            }
            else
            {
                AdjustFreeCountLocation(DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey, Item->Item, Item->Size, Item->LocationFreeSaleCount,Item->LocationFreeSaleCount);
                Item->IsFreebe = false;
                Item->DiscountByTypeRemove(dsMMLocationReward);
                Item->ResetPrice();
            	if(Item->OrderKey != 0)
                {
                    TDBOrder::UpdateOrder(DBTransaction,Item);
                }
            }
            // Remove the Free Discount.
        }
        UndoFreeCount(DBTransaction,Item->SubOrders);
    }
}

void TManagerFreebie::HasPurchased(Database::TDBTransaction &DBTransaction, int LocationKey, AnsiString ItemName, AnsiString SizeName, int Qty, int CountRequired)
{
	if (CountRequired != 0 && CountRequired != -1)
	{
 		AdjustFreeCountLocation(DBTransaction, LocationKey, ItemName, SizeName, Qty, CountRequired);
	}
}

void TManagerFreebie::HasPurchased(Database::TDBTransaction &DBTransaction, TMMContactInfo *Member, AnsiString ItemName, AnsiString SizeName, Currency Qty, int CountRequired)
{
	if (Member != NULL && CountRequired != 0 && CountRequired != -1)
	{
    	AdjustFreeCountContact(DBTransaction, Member->ContactKey, ItemName, SizeName, Qty,CountRequired);
    }
}

void TManagerFreebie::AdjustFreeCountContact(Database::TDBTransaction &DBTransaction, int ContactKey, AnsiString ItemName, AnsiString SizeName, Currency Qty,int CountRequired)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Text =
            "UPDATE CONTACTFREEBIE "
            "SET "
            "ITEM_COUNT			= ITEM_COUNT + :QTY, "
            "MAX_COUNT			= :MAX_COUNT " "WHERE "
            "CONTACTS_KEY		= :CONTACTS_KEY AND "
            "UPPER(ITEM_NAME)		= :ITEM_NAME AND "
            "UPPER(SIZE_NAME)		= :SIZE_NAME";

        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
        IBInternalQuery->ParamByName("QTY")->AsCurrency = Qty;
        IBInternalQuery->ParamByName("MAX_COUNT")->AsInteger = CountRequired;
        IBInternalQuery->ParamByName("ITEM_NAME")->AsString = UpperCase(ItemName);
        IBInternalQuery->ParamByName("SIZE_NAME")->AsString = UpperCase(SizeName);
        IBInternalQuery->ExecQuery();

        if (IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTFREEBIE, 1) FROM RDB$DATABASE";
            IBInternalQuery->ExecQuery();
            int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

            IBInternalQuery->Close();
            IBInternalQuery->ParamCheck = true;
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text =
                "INSERT INTO CONTACTFREEBIE ("
                "CONTACTFREEBIE_KEY, "
                "CONTACTS_KEY, "
                "ITEM_NAME, "
                "SIZE_NAME, "
                "ITEM_COUNT, "
                "MAX_COUNT) "
                "VALUES ("
                ":CONTACTFREEBIE_KEY, "
                ":CONTACTS_KEY, "
                ":ITEM_NAME, "
                ":SIZE_NAME, "
                ":ITEM_COUNT, "
                ":MAX_COUNT);";

            IBInternalQuery->ParamByName("CONTACTFREEBIE_KEY")->AsInteger = LocalKey;
            IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
            IBInternalQuery->ParamByName("ITEM_NAME")->AsString = ItemName;
            IBInternalQuery->ParamByName("SIZE_NAME")->AsString = SizeName;
            IBInternalQuery->ParamByName("ITEM_COUNT")->AsCurrency = Qty;
            IBInternalQuery->ParamByName("MAX_COUNT")->AsInteger = CountRequired;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }

}

void TManagerFreebie::AdjustFreeCountLocation(Database::TDBTransaction &DBTransaction, int LocationKey, AnsiString ItemName, AnsiString SizeName, int Qty, int CountRequired)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Text =
            "UPDATE LOCATIONSFREEBIE "
            "SET "
            "ITEM_COUNT			= ITEM_COUNT + :QTY, "
            "MAX_COUNT			= :MAX_COUNT "
            "WHERE "
            "LOCATION_KEY		= :LOCATION_KEY AND "
            "UPPER(ITEM_NAME)		= :ITEM_NAME AND "
            "UPPER(SIZE_NAME)		= :SIZE_NAME";

        IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
        IBInternalQuery->ParamByName("QTY")->AsInteger = Qty;
        IBInternalQuery->ParamByName("MAX_COUNT")->AsInteger = CountRequired;
        IBInternalQuery->ParamByName("ITEM_NAME")->AsString = UpperCase(ItemName);
        IBInternalQuery->ParamByName("SIZE_NAME")->AsString = UpperCase(SizeName);
        IBInternalQuery->ExecQuery();

        if (IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOCATIONSFREEBIE, 1) FROM RDB$DATABASE";
            IBInternalQuery->ExecQuery();
            int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

            IBInternalQuery->Close();
            IBInternalQuery->ParamCheck = true;
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text =
                "INSERT INTO LOCATIONSFREEBIE ("
                "LOCATIONSFREEBIE_KEY, "
                "LOCATION_KEY, "
                "ITEM_NAME, "
                "SIZE_NAME, "
                "ITEM_COUNT, "
                "MAX_COUNT) "
                "VALUES ("
                ":LOCATIONSFREEBIE_KEY, "
                ":LOCATION_KEY, "
                ":ITEM_NAME, "
                ":SIZE_NAME, "
                ":ITEM_COUNT, "
                ":MAX_COUNT);";

            IBInternalQuery->ParamByName("LOCATIONSFREEBIE_KEY")->AsInteger = LocalKey;
            IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
            IBInternalQuery->ParamByName("ITEM_NAME")->AsString = ItemName;
            IBInternalQuery->ParamByName("SIZE_NAME")->AsString = SizeName;
            IBInternalQuery->ParamByName("ITEM_COUNT")->AsInteger = 1;
            IBInternalQuery->ParamByName("MAX_COUNT")->AsInteger = CountRequired;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}

void TManagerFreebie::Initialise(Database::TDBTransaction &DBTransaction)
{
	MemberLoyaltyRewardsRules = (eFreeSalesRules)TManagerVariable::Instance().GetInt(DBTransaction, vmMemberLoyaltyRewardsRules, 0);
	LocationLoyaltyRewardsRules = (eFreeSalesRules)TManagerVariable::Instance().GetInt(DBTransaction, vmLocationLoyaltyRewardsRules, 0);
}

TManagerFreebie::TManagerFreebie()
{
	MemberLoyaltyRewardsRules = (eFreeSalesRules)0;
	LocationLoyaltyRewardsRules = (eFreeSalesRules)0;
}

void TManagerFreebie::GetReportMemberInfo(Database::TDBTransaction &DBTransaction, int ContactKey, TStringList *Report)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                     "  CONTACTFREEBIE_KEY, "
                                     "  CONTACTS_KEY, "
                                     "  ITEM_NAME, "
                                     "  SIZE_NAME, "
                                     "  cast(ITEM_COUNT as numeric(15,2)) ITEM_COUNT, "
                                     "  MAX_COUNT FROM CONTACTFREEBIE "
                                     "WHERE "
                                     "  CONTACTS_KEY = :CONTACTS_KEY "
                                     "ORDER BY "
                                     "  ITEM_NAME,SIZE_NAME";

		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			// Add the Freebie Block.
			Report->Add("<br>");
			AnsiString Temp = TABLE_START;
			Report->Add(Temp);
			Temp = TABLE_ROW6_HEADER;
			Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Rewards Count");
			Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", "All Items Alphabetical");
			Report->Add(Temp);

			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				AnsiString TempRow = TABLE_ROW6;
				TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Item :");
				TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", IBInternalQuery->FieldByName("ITEM_NAME")->AsString);
				TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Size :");
				TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", IBInternalQuery->FieldByName("SIZE_NAME")->AsString);
				TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE3%", "Count :");
				TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT3%", IBInternalQuery->FieldByName("ITEM_COUNT")->AsString + "/" + IBInternalQuery->FieldByName("MAX_COUNT")->AsString);
				Report->Add(TempRow);
			}
			Temp = TABLE_STOP;
			Report->Add(Temp);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
