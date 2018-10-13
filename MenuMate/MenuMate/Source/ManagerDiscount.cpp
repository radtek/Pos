//---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerDiscount.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif


#include "MMLogging.h"

#include <map>
#include <memory>
#include "Money.h"
#include <math.h>
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "DBOrder.h"
#include <StrUtils.hpp>
#include "rounding.h"
#include "ComboDiscount.h"
#include "DiscountGroupsManager.h"
#include "DiscountGroup.h"
#include "IBillCalculator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerDiscount *ManagerDiscount;

TManagerDiscount::TManagerDiscount()
{
	Enabled = false;
}
//---------------------------------------------------------------------------
TManagerDiscount::~TManagerDiscount()
{
}
//---------------------------------------------------------------------------
void TManagerDiscount::Initialise()
{
	Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TManagerDiscount::SetEnabled(bool value)
{
	if (fEnabled != value)
	{
		fEnabled = value;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TManagerDiscount::GetEnabled()
{
	return fEnabled;
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountList(Database::TDBTransaction &tr,TStringList *destination_list,
                                        std::vector<eDiscountFilter> filters, bool ShowPointsAsDiscount)
{
   	if (!fEnabled || destination_list == NULL)
	return;
    AnsiString queryString = "select discount_key key,name from discounts ";

    if(!filters.empty())
    {
        int size = filters.size();
        queryString += " where ";
        for (std::vector<eDiscountFilter>::iterator ptrFilter = filters.begin(); ptrFilter != filters.end(); ptrFilter++)
        {
            eDiscountFilter filter = *ptrFilter;
            switch(filter)
            {
                case exclude_combos:
                queryString += " discount_mode <> 3 "; //DiscModeCombo
                break;
                case only_combos:
                queryString += " discount_mode = 3 ";    //DiscModeCombo
                break;
                case exclude_member_exempt:
                queryString += " members_exempt <> 'T' ";
                break;
            }
            size--;
            if(size > 0)
              queryString += " and ";
        }
    }

    queryString += " order by appearance_order asc";
    TIBSQL *IBInternalQuery;
    IBInternalQuery = tr.Query(tr.AddQuery());
    IBInternalQuery->SQL->Text = queryString;
    for (IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
		const int index = destination_list->Add(IBInternalQuery->FieldByName(L"name")->AsString);
		destination_list->Objects[index] = reinterpret_cast<TObject *>(IBInternalQuery->FieldByName(L"key")->AsInteger);
	}
	if(TGlobalSettings::Instance().EnableSeperateEarntPts && ShowPointsAsDiscount)
	{
		const int index = destination_list->Add("Loyalty Points");
		destination_list->Objects[index] = reinterpret_cast<TObject *>(dsMMMebersPointsKEY);
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountListByPriority(Database::TDBTransaction &DBTransaction,TStringList *List)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY,NAME,PRIORITY"
	" FROM"
	"  DISCOUNTS"
	" ORDER BY PRIORITY";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount && List != NULL)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int Index = List->Add(IBInternalQuery->FieldByName("NAME")->AsString);
			// Little bit of pointer abuse but we just want to store the int somewhere.
			List->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		}
	}
}
//---------------------------------------------------------------------------
bool TManagerDiscount::GetDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount)
{
	if( !fEnabled )return false;
	bool ReturnVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());


	if(DiscountKey == dsMMMebersPointsKEY)
	{
		Discount.DiscountKey = dsMMMebersPointsKEY;
		Discount.Name = "Loyalty Points";
		Discount.Description = "Loyalty Points Discount";
		Discount.DiscountCode = dsMMMebersPointsKEY;
		Discount.Priority = 999;
		Discount.AppearanceOrder = 0;
		Discount.PercentAmount = 0;
		Discount.Amount = 0;
		Discount.ComboAmount = 0;
		Discount.Rounding = 0.01;

		Discount.MembersOnly = true;
		Discount.MembersExempt = false;
		Discount.Type = dtFixed;
		Discount.Mode = DiscModeCurrency;
		Discount.MaximumValue = 0;
		Discount.Source = dsMMMebersPoints;
		Discount.Group = 0;
        Discount.OriginalAmount = 0;
		ReturnVal = true;
	}
	else if(DiscountKey == dsMMDealKey)
	{
		Discount.DiscountKey = dsMMDealKey;
		Discount.DiscountCode = dsMMDealKey;
		Discount.AppearanceOrder = 0;
		Discount.PercentAmount = 0;
		Discount.Rounding = 0.01;

		Discount.MembersOnly = false;
		Discount.MembersExempt = false;
		Discount.Type = dtFixed;
		Discount.Mode = DiscModeDeal; //DiscModeCombo;
		Discount.Source = dsMMSystem;
		Discount.Group = 0;
		ReturnVal = true;
	}
	else if(DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT *"
		" FROM"
		"  DISCOUNTS"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			Discount.DiscountKey = IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
			Discount.Name = IBInternalQuery->FieldByName("NAME")->AsString;
			Discount.Description = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
			Discount.DiscountCode = IBInternalQuery->FieldByName("DISCOUNT_ID")->AsString;
			Discount.Priority = IBInternalQuery->FieldByName("PRIORITY")->AsInteger;
			Discount.AppearanceOrder = IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;
			Discount.PercentAmount = IBInternalQuery->FieldByName("PERCENTAGE")->AsFloat;
			Discount.Amount = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
			Discount.ComboAmount = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
			Discount.Rounding = IBInternalQuery->FieldByName("ROUNDING")->AsCurrency;
			Discount.MembersOnly = IBInternalQuery->FieldByName("MEMBERS_ONLY")->AsString == "T";
			Discount.MembersExempt = IBInternalQuery->FieldByName("MEMBERS_EXEMPT")->AsString == "T";
			Discount.Type = static_cast<TDiscountType>(IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger);
			Discount.Mode = static_cast<TDiscountMode>(IBInternalQuery->FieldByName("DISCOUNT_MODE")->AsInteger);
			Discount.MaximumValue = IBInternalQuery->FieldByName("MAX_VALUE")->AsCurrency;
			Discount.Group = IBInternalQuery->FieldByName("DISCOUNT_GROUP")->AsInteger;
            Discount.MaxItemAffected  = IBInternalQuery->FieldByName("MAX_ITEM_AFFECTED")->AsInteger;
            Discount.MinItemRequired  = IBInternalQuery->FieldByName("MIN_ITEMS_REQUIRED")->AsInteger;
            Discount.DailyUsageAllowedPerMember  = IBInternalQuery->FieldByName("DAILY_USE_PER_MEMBER")->AsInteger;
            Discount.IsCloudDiscount = IBInternalQuery->FieldByName("IS_CLOUD_DISCOUNT")->AsString == "T";
            Discount.IsMembershipDiscount = IBInternalQuery->FieldByName("IS_MEMBERSHIP_DISCOUNT")->AsString == "T";
            if(Discount.Mode == DiscModePercent)
            {
               Discount.OriginalAmount = IBInternalQuery->FieldByName("PERCENTAGE")->AsFloat;
            }
            else
            {
               Discount.OriginalAmount = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            }
            Discount.DiscountAppliedTime = Now();
			GetDiscountCategories(DBTransaction,DiscountKey,Discount);
			ReturnVal = true;
		}
	}

	PopulateDiscountGroupPerType(DiscountKey, Discount);
	return ReturnVal;
}
//---------------------------------------------------------------------------
bool TManagerDiscount::GetNonChargableDiscount(Database::TDBTransaction &DBTransaction, TDiscount &Discount)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT c.DISCOUNT_KEY from DISCOUNTGROUPS_DISCOUNTTYPES a "
	"inner join DISCOUNT_GROUPS b on "
	"a.DISCOUNTGROUPS_KEY = b.DISCOUNTGROUPS_KEY "
	"inner join DISCOUNTS c on "
	"a.DISCOUNTTYPE_KEY = c.DISCOUNT_KEY "
	"where b.DISCOUNTGROUP_NAME = 'Non-Chargeable' " ;
	IBInternalQuery->ExecQuery();
	if(!IBInternalQuery->Eof)
	{
		int discountKey =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		GetDiscount(DBTransaction,discountKey,Discount) ;
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT CATEGORY_KEY"
		" FROM"
		"  DISCOUNTCATEGORIES"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;

		for(IBInternalQuery->ExecQuery();!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			Discount.CategoryFilterKeys.insert(IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger);
		}
	}

}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" DELETE "
		" FROM"
		"  DISCOUNTCATEGORIES"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO DISCOUNTCATEGORIES ("
		"DISCOUNTCATEGORIES_KEY,"
		"DISCOUNT_KEY,"
		"CATEGORY_KEY) "
		"VALUES ("
		"(SELECT GEN_ID(GEN_DISCOUNT_CATEGORIES, 1) FROM RDB$DATABASE),"
		":DISCOUNT_KEY,"
		":CATEGORY_KEY);";

		for (std::set<int>::iterator ptrCatKey = Discount.CategoryFilterKeys.begin();
		ptrCatKey != Discount.CategoryFilterKeys.end(); ptrCatKey++)
		{
			IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = *ptrCatKey;
			IBInternalQuery->ExecQuery();
		}
	}
}
//---------------------------------------------------------------------------
int TManagerDiscount::GetDiscountKeyByCode(Database::TDBTransaction &DBTransaction,AnsiString DiscountCode)
{
	if( !fEnabled )
      return false;
	int ReturnVal = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountCode != "")
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DISCOUNT_KEY"
		" FROM"
		"  DISCOUNTS"
		" WHERE"
		" DISCOUNT_ID = :DISCOUNT_ID";
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = DiscountCode;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			ReturnVal = IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		}
	}
	return ReturnVal;
}
//---------------------------------------------------------------------------
AnsiString TManagerDiscount::GetDiscountCodeByKey(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )
      return "";
	AnsiString ReturnVal = "";

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DISCOUNT_ID"
		" FROM"
		"  DISCOUNTS"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			ReturnVal = IBInternalQuery->FieldByName("DISCOUNT_ID")->AsString;
		}
	}
	return ReturnVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::DiscountCodeToKey(Database::TDBTransaction &DBTransaction,std::set<AnsiString> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts)
{
	for(std::set<AnsiString>::iterator itDiscountsID = AutoAppliedDiscountsID.begin(); itDiscountsID != AutoAppliedDiscountsID.end(); std::advance(itDiscountsID,1) )
	{
		int DiscountKey = GetDiscountKeyByCode(DBTransaction,*itDiscountsID);
		if(DiscountKey)
		{
			AutoAppliedDiscounts.insert(DiscountKey);
		}
	}
};
//---------------------------------------------------------------------------
void TManagerDiscount::DiscountKeyToCode(Database::TDBTransaction &DBTransaction,std::set<AnsiString> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts)
{
	for(std::set<int>::iterator itDiscountsKey = AutoAppliedDiscounts.begin(); itDiscountsKey != AutoAppliedDiscounts.end(); std::advance(itDiscountsKey,1) )
	{
		AnsiString DiscountCode = GetDiscountCodeByKey(DBTransaction,*itDiscountsKey);
		if(DiscountCode != "")
		{
			AutoAppliedDiscountsID.insert(DiscountCode);
		}
	}
};
//---------------------------------------------------------------------------
UnicodeString TManagerDiscount::GetDiscountDescription(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return "";

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	UnicodeString ReturnVal = "";
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DESCRIPTION"
	" FROM"
	"  DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		ReturnVal = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
	}

	return ReturnVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount Discount)
{
	if( !fEnabled )return ;

    if(Discount.IsCloudDiscount)
    {
       int discountKey = GetDiscountKeyByCode(DBTransaction,Discount.DiscountCode);
       if(discountKey > 0)
       {
         DiscountKey =  discountKey;
       }
    }
    else if(DiscountCodeExisits(DBTransaction,DiscountKey,Discount.DiscountCode))
	{
		throw Exception("A Discount with this Discount Code Already Exists.");
	}

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());


	if(DiscountKey == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DISCOUNTS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		DiscountKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO DISCOUNTS ("
		"DISCOUNT_KEY,"
		"NAME,"
		"DESCRIPTION,"
		"PERCENTAGE,"
		"AMOUNT,"
		"DISCOUNT_TYPE,"
		"DISCOUNT_MODE,"
		"ROUNDING,"
		"APPEARANCE_ORDER,"
		"PRIORITY,"
		"MEMBERS_ONLY,"
		"MEMBERS_EXEMPT,"
		"DISCOUNT_ID,"
		"MAX_VALUE,"
		"DISCOUNT_GROUP, "
        "DAILY_USE_PER_MEMBER,"
        "MAX_ITEM_AFFECTED,"
        "MIN_ITEMS_REQUIRED,"
        "IS_CLOUD_DISCOUNT, "
        "IS_MEMBERSHIP_DISCOUNT) "
		"VALUES ("
		":DISCOUNT_KEY,"
		":NAME,"
		":DESCRIPTION,"
		":PERCENTAGE,"
		":AMOUNT,"
		":DISCOUNT_TYPE,"
		":DISCOUNT_MODE,"
		":ROUNDING,"
		":APPEARANCE_ORDER,"
		":PRIORITY,"
		":MEMBERS_ONLY,"
		":MEMBERS_EXEMPT,"
		":DISCOUNT_ID,"
		":MAX_VALUE,"
		":DISCOUNT_GROUP, "
        ":DAILY_USE_PER_MEMBER,"
        ":MAX_ITEM_AFFECTED,"
        ":MIN_ITEMS_REQUIRED,"
        ":IS_CLOUD_DISCOUNT,"
        ":IS_MEMBERSHIP_DISCOUNT);";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("NAME")->AsString = Discount.Name;
		IBInternalQuery->ParamByName("DESCRIPTION")->AsString = Discount.Description;
		IBInternalQuery->ParamByName("PERCENTAGE")->AsFloat = Discount.PercentAmount;
		IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = Discount.Amount;
		IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = Discount.Rounding;
		IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = Discount.Type;
		IBInternalQuery->ParamByName("DISCOUNT_MODE")->AsInteger = Discount.Mode;
		IBInternalQuery->ParamByName("MEMBERS_ONLY")->AsString = (Discount.MembersOnly == true) ? "T" : "F";
		IBInternalQuery->ParamByName("MEMBERS_EXEMPT")->AsString = (Discount.MembersExempt  == true) ? "T" : "F";
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = Discount.AppearanceOrder;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = Discount.Priority;
		IBInternalQuery->ParamByName("DISCOUNT_GROUP")->AsInteger = Discount.Group;
		IBInternalQuery->ParamByName("DAILY_USE_PER_MEMBER")->AsInteger = Discount.DailyUsageAllowedPerMember;
		IBInternalQuery->ParamByName("MAX_ITEM_AFFECTED")->AsInteger = Discount.MaxItemAffected;
		IBInternalQuery->ParamByName("MIN_ITEMS_REQUIRED")->AsInteger = Discount.MinItemRequired;
        IBInternalQuery->ParamByName("IS_CLOUD_DISCOUNT")->AsString = Discount.IsCloudDiscount ? "T" : "F";
        IBInternalQuery->ParamByName("IS_MEMBERSHIP_DISCOUNT")->AsString = Discount.IsMembershipDiscount ? "T" : "F";
		if(Discount.DiscountCode == "")
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = Discount.DiscountCode;
		}
		IBInternalQuery->ParamByName("MAX_VALUE")->AsCurrency = Discount.MaximumValue;
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" NAME = :NAME,"
		" DESCRIPTION = :DESCRIPTION,"
		" PERCENTAGE = :PERCENTAGE,"
		" AMOUNT = :AMOUNT,"
		" DISCOUNT_MODE = :DISCOUNT_MODE,"
		" ROUNDING = :ROUNDING,"
		" DISCOUNT_TYPE = :DISCOUNT_TYPE,"
		" DISCOUNT_ID = :DISCOUNT_ID,"
		" APPEARANCE_ORDER = :APPEARANCE_ORDER,"
		" PRIORITY = :PRIORITY,"
		" MEMBERS_ONLY = :MEMBERS_ONLY,"
		" MEMBERS_EXEMPT = :MEMBERS_EXEMPT,"
		" MAX_VALUE = :MAX_VALUE,"
        " DISCOUNT_GROUP = :DISCOUNT_GROUP,"
        " DAILY_USE_PER_MEMBER=:DAILY_USE_PER_MEMBER,"
        " MAX_ITEM_AFFECTED=:MAX_ITEM_AFFECTED,"
        " MIN_ITEMS_REQUIRED=:MIN_ITEMS_REQUIRED,"
        " IS_CLOUD_DISCOUNT = :IS_CLOUD_DISCOUNT,"
        " IS_MEMBERSHIP_DISCOUNT = :IS_MEMBERSHIP_DISCOUNT"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";

		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("NAME")->AsString = Discount.Name;
		IBInternalQuery->ParamByName("DESCRIPTION")->AsString = Discount.Description;
		IBInternalQuery->ParamByName("PERCENTAGE")->AsFloat = Discount.PercentAmount;
		IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = Discount.Amount;
		IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = Discount.Rounding;
		IBInternalQuery->ParamByName("DISCOUNT_MODE")->AsInteger = Discount.Mode;
		IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = Discount.Type;
		IBInternalQuery->ParamByName("MEMBERS_ONLY")->AsString = (Discount.MembersOnly == true) ? "T" : "F";
		IBInternalQuery->ParamByName("MEMBERS_EXEMPT")->AsString = (Discount.MembersExempt  == true) ? "T" : "F";
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = Discount.AppearanceOrder;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = Discount.Priority;
		IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = Discount.Type;
		IBInternalQuery->ParamByName("MAX_VALUE")->AsCurrency = Discount.MaximumValue;
		IBInternalQuery->ParamByName("DISCOUNT_GROUP")->AsInteger = Discount.Group;
		IBInternalQuery->ParamByName("DAILY_USE_PER_MEMBER")->AsInteger = Discount.DailyUsageAllowedPerMember;
		IBInternalQuery->ParamByName("MAX_ITEM_AFFECTED")->AsInteger = Discount.MaxItemAffected;
		IBInternalQuery->ParamByName("MIN_ITEMS_REQUIRED")->AsInteger = Discount.MinItemRequired;
        IBInternalQuery->ParamByName("IS_CLOUD_DISCOUNT")->AsString = Discount.IsCloudDiscount ? "T" : "F";
        IBInternalQuery->ParamByName("IS_MEMBERSHIP_DISCOUNT")->AsString = Discount.IsMembershipDiscount ? "T" : "F";
		if(Discount.DiscountCode == "")
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = Discount.DiscountCode;
		}
		IBInternalQuery->ExecQuery();
	}
	SetDiscountCategories(DBTransaction,DiscountKey,Discount);
}
//---------------------------------------------------------------------------
bool TManagerDiscount::DiscountCodeExisits(Database::TDBTransaction &DBTransaction,long DiscountKey, AnsiString DiscountCode)
{
	bool RetVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountKey == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DISCOUNT_KEY"
		" FROM"
		"  DISCOUNTS"
		" WHERE DISCOUNT_ID = :DISCOUNT_ID";
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = DiscountCode;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			RetVal = true;
		}
	}
	else
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DISCOUNT_KEY"
		" FROM"
		"  DISCOUNTS"
		" WHERE DISCOUNT_ID = :DISCOUNT_ID AND "
		" DISCOUNT_KEY != :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = DiscountCode;
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			RetVal = true;
		}
	}
	return RetVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM "
	" DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountAmountDB(Database::TDBTransaction &DBTransaction,TList * DiscountedItems)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		for (int i = 0; i < DiscountedItems->Count ; i++)
		{
			TItemMinorComplete *Order = (TItemMinorComplete *) DiscountedItems->Items[i];
			//Clear all the Current Orders Discounts.
			TDBOrder::DeleteOrderDiscounts(DBTransaction,Order);
			// Reapply them to the DB.
			TDBOrder::SetOrderDiscounts(DBTransaction,Order);

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"ORDERS "
			"SET "
			"DISCOUNT = :DISCOUNT, "
			"DISCOUNT_REASON = :DISCOUNT_REASON, "
			"LOYALTY_KEY = :LOYALTY_KEY, "
			"ZED_PRICE = :ZED_PRICE, "
          "DISCOUNT_WITHOUT_TAX = :DISCOUNT_WITHOUT_TAX "
			"WHERE "
			"ORDER_KEY = :ORDER_KEY";
			IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
			IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Order->Loyalty_Key;
			IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Order->TotalAdjustment();
			IBInternalQuery->ParamByName("DISCOUNT_REASON")->AsString = Order->DiscountReasons().SubString(1,40);
			IBInternalQuery->ParamByName("ZED_PRICE")->AsCurrency = Order->PriceEach_BillCalc();
            IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = Order->BillCalcResult.DiscountWithoutTax;

			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::ClearDiscounts(TList * DiscountItems)
{
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		Order->DiscountsClear();
        for(int j = 0; j < Order->SubOrders->Count; j++)
         {
            TItemMinorComplete *SubOrder = (TItemMinorComplete *) Order->SubOrders->Items[j];
            SubOrder->DiscountsClear();
         }
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::ClearDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied)
{
	for (int i = 0; i < DiscountItems->Count && DiscountToBeApplied.Mode != DiscModeCombo; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		Order->DiscountRemove(DiscountToBeApplied);
        for(int j = 0; j < Order->SubOrders->Count; j++)
         {
            TItemMinorComplete *SubOrder = (TItemMinorComplete *) Order->SubOrders->Items[j];
            SubOrder->DiscountRemove(DiscountToBeApplied);
         }
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::ClearMemberDiscounts(TList * DiscountItems)
{
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		Order->DiscountByTypeRemove(dsMMMembership);
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::ClearMemberExemtDiscounts(TList * DiscountItems)
{
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
        for (std::vector<TDiscount>::iterator ptrDiscount = Order->Discounts.begin(); ptrDiscount != Order->Discounts.end(); )
        {
            TDiscount CurrentDiscount = *ptrDiscount;
            if (CurrentDiscount.MembersExempt)
            {
                Order->Discounts.erase(ptrDiscount);
                ptrDiscount = Order->Discounts.begin();
            }
            else
            {
                ptrDiscount++;
            }
        }

        for (int j = 0; j < Order->SubOrders->Count; j++)
        {
           TItemMinorComplete *SubOrder = (TItemMinorComplete*)Order->SubOrders->Items[j];
           for (std::vector<TDiscount>::iterator ptrDiscount = SubOrder->Discounts.begin(); ptrDiscount != SubOrder->Discounts.end(); )
           {
                TDiscount CurrentDiscount = *ptrDiscount;
                if (CurrentDiscount.MembersExempt)
                {
                    SubOrder->Discounts.erase(ptrDiscount);
                    ptrDiscount = SubOrder->Discounts.begin();
                }
                else
                {
                    ptrDiscount++;
                }
           }
        }
	}
}
//---------------------------------------------------------------------------
int __fastcall SortByFinalPriceAsc(void *Item1,void *Item2)
{
	TItemMinorComplete *Order1 = (TItemMinorComplete *)Item1;
	TItemMinorComplete *Order2 = (TItemMinorComplete *)Item2;
    double unitPrice1 = Order1->BillCalcResult.FinalPrice/Order1->GetQty();
    double unitPrice2 = Order2->BillCalcResult.FinalPrice/Order2->GetQty();

	if (unitPrice1 > unitPrice2)
	{
		return 1;
	}
	else if (unitPrice1 == unitPrice2)
	{
		return -0;
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
int __fastcall SortByFinalPriceDesc(void *Item1,void *Item2)
{
	TItemMinorComplete *Order1 = (TItemMinorComplete *)Item1;
	TItemMinorComplete *Order2 = (TItemMinorComplete *)Item2;
    double unitPrice1 = Order1->BillCalcResult.FinalPrice/Order1->GetQty();
    double unitPrice2 = Order2->BillCalcResult.FinalPrice/Order2->GetQty();

	if (unitPrice1 > unitPrice2)
	{
		return -1;
	}
	else if (unitPrice1 == unitPrice2)
	{
		return -0;
	}
	else
	{
		return 1;
	}
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    DiscountToBeApplied.AppliedMode = DiscountToBeApplied.Mode;
    TList *allItems = new TList();
    TItemMinorComplete *order;
    TItemMinorComplete *side;
    double orderQty = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(order->DiscountApplies(DiscountToBeApplied) && !order->DiscountApplied(DiscountToBeApplied.DiscountKey))
           allItems->Add(DiscountItems->Items[x]);

        if(order->SubOrders->Count > 0 && DiscountToBeApplied.Mode != DiscModeItem)
         {
            for(int subOrderIndex = 0 ; subOrderIndex < order->SubOrders->Count ; subOrderIndex++)
            {
              side = reinterpret_cast<TItemMinorComplete *>(order->SubOrders->Items[subOrderIndex]);
              if(side->DiscountApplies(DiscountToBeApplied) && !side->DiscountApplied(DiscountToBeApplied.DiscountKey))
                 allItems->Add(side);
            }
         }
     }

     if(DiscountToBeApplied.ProductPriority == ppCheapest)
     {
        allItems->Sort(&SortByFinalPriceAsc);
     }
     else if(DiscountToBeApplied.ProductPriority == ppHighest)
     {
         allItems->Sort(&SortByFinalPriceDesc);
     }

    switch(DiscountToBeApplied.Mode)
    {
       case DiscModeCurrency:
            AddCurrencyModeDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModeCombo:
            AddComboDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModeDeal:
            AddDealDiscount(DiscountItems,DiscountToBeApplied);
		    break;
       case DiscModeItem:
            AddItemModeDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModePercent:
            AddPercentageModeDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModeSetPrice:
            AddSetPriceDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModePoints:
            AddPointModeDiscount(allItems,DiscountToBeApplied);
		    break;
    }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddSetPriceDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;
    Currency discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    //Calculate order Total then calculate discount amount
    Currency order_total = GetOrderTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);
    Currency offset  = order_total - (maxDiscountQty * discount);
    //if total discount is greater than maximum value then change discount amount
    if(fabs(offset) > DiscountToBeApplied.MaximumValue && DiscountToBeApplied.MaximumValue > 0)
    {
        Currency amount = DiscountToBeApplied.MaximumValue;
        if(offset < 0)
           amount = -1 * amount;
        DiscountToBeApplied.AppliedMode = DiscModeCurrency;
        DiscountToBeApplied.Amount = amount;
        AddCurrencyModeDiscount(DiscountItems,DiscountToBeApplied);
        return;
    }

    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    Currency  itemDiscount = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        //if item quantity is equals to remaining quantity then apply discount on full quantity
        //else change amount as per quantity applicable
        if(maxDiscountQty >= order->GetQty())
         {
            scaled_discount.Amount = discount;
            order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
            Currency unitPrice = order->GrandTotal()/order->GetQty();
            itemDiscount = ((discount * maxDiscountQty) + ((order->GetQty() - maxDiscountQty) * unitPrice));
            scaled_discount.Amount = RoundToNearest(itemDiscount / order->GetQty(),0.01, TGlobalSettings::Instance().MidPointRoundsDown);;
            order->DiscountAdd(scaled_discount);
         }
         else
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
         maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddPercentageModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;

    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    Currency  discount = DiscountToBeApplied.PercentAmount;
    Currency order_total = GetOrderTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);
    //calculate discount amount
    Currency discountedAmount = RoundToNearest((order_total * fabs(discount))/100, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);
    bool isSurcharge =  discount < 0;
    //if discount amount is greater than max value then apply discount as curreny mode
    //with max value as discount amount
    if(discountedAmount >  scaled_discount.MaximumValue && scaled_discount.MaximumValue > 0)
    {
       scaled_discount.Amount =  isSurcharge ? -1 * scaled_discount.MaximumValue : scaled_discount.MaximumValue;
       scaled_discount.AppliedMode = DiscModeCurrency;
       AddCurrencyModeDiscount(DiscountItems,scaled_discount);
       return;
    }
    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        if(maxDiscountQty >= order->GetQty())
         {
           scaled_discount.PercentAmount = discount;
           order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
           scaled_discount.PercentAmount =  (discount * maxDiscountQty)/order->GetQty();
           order->DiscountAdd(scaled_discount);
         }
        else
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
        maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddCurrencyModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
	TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;
    Currency order_total = 0;
    Currency discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    order_total = GetOrderTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);

    if(order_total <= 0)
      order_total = 1;

    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    double  itemDiscount = 0;

    //if discount amount is greater than order total then change discount amount to order total
    if(order_total < discount)
       discount = order_total;

    bool isSurcharge =  discount < 0;
    if(scaled_discount.MaximumValue < fabs(discount) && scaled_discount.MaximumValue > 0)
       discount = isSurcharge ? -1 * scaled_discount.MaximumValue : scaled_discount.MaximumValue;

    Currency discountApplied = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty > order->GetQty())
         {
            Currency itemTotal = order->GrandTotal();// > 0 ? order->GrandTotal() : 1.00;
            if(x == DiscountItems->Count- 1)
            {
               itemDiscount  = (discount - discountApplied)/ order->GetQty();
            }
            else
            {
               itemDiscount = (itemTotal /(order_total * order->GetQty()))* discount ;
            }
            scaled_discount.Amount = itemDiscount;
            discountApplied +=  scaled_discount.Amount * order->GetQty();
            order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
            itemDiscount = (discount - discountApplied)/ order->GetQty();
            scaled_discount.Amount = itemDiscount;
            order->DiscountAdd(scaled_discount);
         }
         else
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
         maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddComboDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
	TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;

    Currency discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);
    Currency order_total  = GetOrderTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);
    discount = order_total - discount;
    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    Currency  itemDiscount = 0;

    Currency discountApplied = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty >= order->GetQty())
         {
            itemDiscount = (order->GrandTotal() * discount )/(order_total * order->GetQty());
            scaled_discount.Amount = itemDiscount;
            order->DiscountAdd(scaled_discount);
            discountApplied +=  itemDiscount * order->GetQty();
         }
         else if(maxDiscountQty > 0)
         {
            itemDiscount = (discount - discountApplied)/ order->GetQty();
            scaled_discount.Amount = itemDiscount;
            order->DiscountAdd(scaled_discount);
         }
         else
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
         maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddDealDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
        TItemMinorComplete *order;
        volatile unsigned int i = 0;
        unsigned int j = DiscountItems->Count;
		std::vector<Currency> item_totals;
		std::vector<TItemMinorComplete *> ordered_items;
		TItemCompleteSub *side;
		Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);
		Currency  order_total = 0;
		Currency  remaining_discount = 0;
		Currency  temp;
		TDiscount scaled_discount;

		item_totals.reserve(j);
		ordered_items.reserve(j);

		scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
		scaled_discount.Source = DiscountToBeApplied.Source;
		scaled_discount.Mode = DiscModeDeal;
        scaled_discount.AppliedMode = DiscModeDeal;
		scaled_discount.Priority = DiscountToBeApplied.Priority;
        scaled_discount.IsCloudDiscount = DiscountToBeApplied.IsCloudDiscount;
        scaled_discount.DiscountCode = DiscountToBeApplied.DiscountCode;
        scaled_discount.DailyUsageAllowedPerMember = DiscountToBeApplied.DailyUsageAllowedPerMember;
        scaled_discount.MembersOnly = DiscountToBeApplied.MembersOnly;
        scaled_discount.MembersExempt = DiscountToBeApplied.MembersExempt;
		Currency GrandTotal = 0;
		unsigned int k = 0;
		unsigned int l = 0;

		for (i=0; i < j; i++)
        {
			order =	reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
			//temp = RoundToNearest(order->GrandTotalExcCombo()/(order->GetQty()),0.01,false);
			//GrandTotal += RoundToNearest(order->GrandTotalExcCombo()/(order->GetQty())*(order->DealSelectedItems - order->PrevDealSelectedItems),0.01,false);;
            temp = RoundToNearest(order->Price()/(order->GetQty()),0.01,false);
            GrandTotal += RoundToNearest(order->Price()/(order->GetQty())*(order->DealSelectedItems - order->PrevDealSelectedItems),0.01,false);;
			order_total += temp;
			if (order->DiscountApplies(DiscountToBeApplied))
			{
				item_totals.push_back(temp);
				ordered_items.push_back(order);
			}
		}

		scaled_discount.Name 		= DiscountToBeApplied.Name;
		scaled_discount.Description = DiscountToBeApplied.Description;
		scaled_discount.ComboAmount = DiscountToBeApplied.ComboAmount;

       for (i = 0, j = ordered_items.size(); i < j; i++)
        {

           if(ordered_items.at(i)->GetQty() > 1)
             {
                Currency Amounts=0;
                Currency OtherDiscount = 0;
                int dealPriority = DiscountToBeApplied.Priority ;
                for (std::vector <TDiscount> ::iterator ptrDiscounts = ordered_items.at(i)->Discounts.begin();
                     ptrDiscounts != ordered_items.at(i)->Discounts.end(); std::advance(ptrDiscounts, 1))
                {
                   if(ptrDiscounts->Mode == DiscModeDeal)
                    {
                      Amounts = RoundToNearest(ptrDiscounts->ComboAmount, scaled_discount.Rounding, false);
                    }
                }

                BillCalculator::DISCOUNT_RESULT_LIST::iterator it_discount = ordered_items.at(i)->BillCalcResult.Discount.begin();
                for( ; it_discount != ordered_items.at(i)->BillCalcResult.Discount.end(); it_discount++ )
                {
                    if(it_discount->Priority < dealPriority)
                      OtherDiscount += (it_discount->Content / ordered_items.at(i)->GetQty());

                }

                if(Amounts < 0)
                {
                   Amounts = -1 * Amounts;
                }
                int dealExemptItems = ordered_items.at(i)->GetQty() - ordered_items.at(i)->DealSelectedItems;
                int currentDealItems = ordered_items.at(i)->DealSelectedItems - ordered_items.at(i)->PrevDealSelectedItems;
                //Currency currentdealTotal = RoundToNearest(((item_totals.at(i) / GrandTotal) * DiscountToBeApplied.Amount), 0.01, false);
                Currency currentdealTotal = RoundToNearest((item_totals.at(i) * DiscountToBeApplied.Amount) / GrandTotal, 0.01, false);
                Currency dealExemptTotal = (item_totals.at(i) - OtherDiscount)* dealExemptItems;
                Currency preiousDealTotal = Amounts * ordered_items.at(i)->PrevDealSelectedItems;
                Currency itemPriceAfterDisc = dealExemptTotal + preiousDealTotal + (currentdealTotal * currentDealItems);
                scaled_discount.Amount = RoundToNearest(itemPriceAfterDisc/ordered_items.at(i)->GetQty(), 0.01, false);
                scaled_discount.PercentAmount =item_totals.at(i) / order_total;
                scaled_discount.ComboAmount = ((currentdealTotal * currentDealItems) + preiousDealTotal)/ordered_items.at(i)->DealSelectedItems;
                ordered_items.at(i)->DiscountByTypeRemove(DiscModeDeal);
                ordered_items.at(i)->DiscountAdd(scaled_discount);
                remaining_discount += (currentdealTotal * currentDealItems);
            }
		 else
            {
                scaled_discount.Amount = RoundToNearest(((item_totals.at(i) / GrandTotal) * DiscountToBeApplied.Amount), scaled_discount.Rounding, false);
                scaled_discount.PercentAmount = item_totals.at(i) / order_total;
                ordered_items.at(i)->DiscountAdd(scaled_discount);
                remaining_discount += scaled_discount.Amount;
            }

			ordered_items.at(i)->PrevDealSelectedItems += (ordered_items.at(i)->DealSelectedItems- ordered_items.at(i)->PrevDealSelectedItems);
      }
     Currency dif = DiscountToBeApplied.ComboAmount - remaining_discount ;
     if(dif != 0)
     {
        TDiscount &dsc = ordered_items.at(i - 1)->Discount_Back();
        dsc.Amount += dif;

     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddItemModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;
    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    bool isSurcharge =  discount < 0;
    Currency maxDiscountValue = fabs(discount) * maxDiscountQty;

    if(maxDiscountValue > scaled_discount.MaximumValue && scaled_discount.MaximumValue > 0)
    {
       scaled_discount.Amount =  isSurcharge ? -1 * scaled_discount.MaximumValue : scaled_discount.MaximumValue;
       scaled_discount.AppliedMode = DiscModeCurrency;
       AddCurrencyModeDiscount(DiscountItems,scaled_discount);
       return;
    }

    Currency itemDiscount = 0;
    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        if(maxDiscountQty >= order->GetQty())
         {
           scaled_discount.Amount = discount;
           order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           itemDiscount = discount;
           if(discount * order->GetQty() >  order->GrandTotal())
              itemDiscount = order->GrandTotal()/order->GetQty();
           scaled_discount.Amount =  RoundToNearest((itemDiscount * maxDiscountQty)/order->GetQty(),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
           order->DiscountAdd(scaled_discount);
         }
        else
         {
           scaled_discount.AppliedMode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
        maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddPointModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = (DiscountToBeApplied.MaxItemAffected == 0 )|| (DiscountToBeApplied.MaxItemAffected > 0 && orderQty <  DiscountToBeApplied.MaxItemAffected)
                              ? orderQty : DiscountToBeApplied.MaxItemAffected;
    TDiscount scaled_discount;
    CopyDiscountDetails(scaled_discount,DiscountToBeApplied);
    scaled_discount.Amount = 0;
    Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    Currency pointsTotal = GetOrderPointsTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);
    Currency pointDiscount = pointsTotal - discount * maxDiscountQty;
    if(pointDiscount > scaled_discount.MaximumValue && scaled_discount.MaximumValue > 0)
    {
        discount = (pointsTotal - scaled_discount.MaximumValue) / maxDiscountQty;
    }

    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        order->ItemPriceForPoints =  discount;
        order->DiscountAdd(scaled_discount);
        maxDiscountQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void TManagerDiscount::CopyDiscountDetails(TDiscount& destination,TDiscount& source)
{
    destination.DiscountKey = source.DiscountKey;
    destination.Source = source.Source;
    destination.MaximumValue = source.MaximumValue;
    destination.Priority = source.Priority;
    destination.DiscountGroupList = source.DiscountGroupList;
    destination.IsThorBill = source.IsThorBill;
    destination.VoucherCode = source.VoucherCode;
    destination.Name 		= source.Name;
	destination.Description = source.Description;
    destination.Mode = source.Mode;
    destination.AppliedMode = source.AppliedMode;
    destination.MaxItemAffected = source.MaxItemAffected;
    destination.MinItemRequired = source.MinItemRequired;
    destination.IsCloudDiscount = source.IsCloudDiscount;
    destination.DiscountCode = source.DiscountCode;
    destination.DailyUsageAllowedPerMember = source.DailyUsageAllowedPerMember;
    destination.MembersOnly = source.MembersOnly;
    destination.MembersExempt = source.MembersExempt;
    destination.OriginalAmount = source.OriginalAmount;
    destination.DiscountAppliedTime = source.DiscountAppliedTime;
    destination.Rounding = source.Rounding;
    destination.CategoryFilterKeys = source.CategoryFilterKeys;
}
//---------------------------------------------------------------------------
Currency TManagerDiscount::GetOrderTotal(TList *DiscountItems,TDiscount DiscountToBeApplied, double maxDiscountQty)
{
    TItemMinorComplete *order;
    Currency  order_total = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty >= order->GetQty())
         {
            order_total += order->GrandTotal();
         }
         else
         {
            order_total += (order->GrandTotal()/order->GetQty())*maxDiscountQty;
         }
         maxDiscountQty -= order->GetQty();
         if(maxDiscountQty <= 0 && DiscountToBeApplied.MaxItemAffected > 0)
           break;

     }
     return order_total;
}
//---------------------------------------------------------------------------
Currency TManagerDiscount::GetOrderPointsTotal(TList *DiscountItems,TDiscount DiscountToBeApplied, double maxDiscountQty)
{
    TItemMinorComplete *order;
    Currency  order_total = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty >= order->GetQty())
         {
            order_total += order->ItemPriceForPointsOriginal;
         }
         else
         {
            order_total += (order->ItemPriceForPointsOriginal/order->GetQty())*maxDiscountQty;
         }
         maxDiscountQty -= order->GetQty();
         if(maxDiscountQty <= 0 && DiscountToBeApplied.MaxItemAffected > 0)
           break;

     }
     return order_total;
}
//---------------------------------------------------------------------------
double TManagerDiscount::GetOrderQty(TList *DiscountItems)
{
  double orderQty = 0;
  TItemMinorComplete *order;
  // Calculate Order item quantity to check that discount can be applied
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        orderQty += order->GetQty();
     }
  return orderQty;
}
//---------------------------------------------------------------------------
void TManagerDiscount::AddDiscountsByTime(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order)
{
	std::set<int> DiscountKeys;
	TDateTime DateTime = Order->TimeStamp;
	GetDiscountKeysByTime(DBTransaction,DateTime, DiscountKeys);
	for (	std::set<int>::iterator ptrDiscountKey = DiscountKeys.begin();
	ptrDiscountKey != DiscountKeys.end() ; std::advance(ptrDiscountKey,1))
	{
        TDiscount CurrentDiscount;
		GetDiscount(DBTransaction,*ptrDiscountKey, CurrentDiscount);
        std::auto_ptr<TList> itemList(new TList());
        itemList->Add(Order);
        AddDiscount(itemList.get(),CurrentDiscount);
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountKeysByTime(Database::TDBTransaction &DBTransaction,TDateTime &CurrentTime, std::set<int> &DiscountKeys)
{
	/* TODO : Fix up the recurrence on the discount times, Daily Weekly Monthly. */
	// Not sure how to handle the recurrence here.
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY"
	" FROM"
	"  DISCOUNTSTIME"
	" WHERE"
	" TIME_STAMP_START < :CURRENT_TIME AND"
	" TIME_STAMP_END > :CURRENT_TIME";
	IBInternalQuery->ParamByName("CURRENT_TIME")->AsDateTime = CurrentTime;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			DiscountKeys.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
		}
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::ClearDiscountDB(Database::TDBTransaction &DBTransaction,const std::set<long> &OrderKeys)
{
	try
	{
		if(!OrderKeys.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (	std::set<long>::const_iterator ptrItemKey = OrderKeys.begin();
			ptrItemKey != OrderKeys.end() ; ptrItemKey++,i++)
			{
				UnicodeString Key = IntToStr((int)*ptrItemKey);
				if(i%1000 == 0)
				{
					i = 1;
					Key = Key +  ") OR ORDER_KEY IN (" + Key;
				}
				KeysList->Add(Key);
			}

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"ORDERS "
			"SET "
			"DISCOUNT = 0 "
			"WHERE "
			"ORDER_KEY IN (" + KeysList->DelimitedText + ")";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::AddDiscountTime(Database::TDBTransaction &DBTransaction,TDiscountTimes &DiscountTime)
{
	if(DiscountTime.DiscountKey != 0)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DISCOUNTCARDS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		DiscountTime.DiscountTimeKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO DISCOUNTSTIME ("
		"DISCOUNTSTIME_KEY,"
		"DISCOUNT_KEY,"
		"TIME_STAMP_START,"
		"TIME_STAMP_END,"
		"PRIORITY_OVERRIDE,"
		"PROPERTIES) "
		"VALUES ("
		":DISCOUNTSTIME_KEY,"
		":DISCOUNT_KEY,"
		":TIME_STAMP_START,"
		":TIME_STAMP_END,"
		":PRIORITY_OVERRIDE,"
		":PROPERTIES);";
		IBInternalQuery->ParamByName("DISCOUNTSTIME_KEY")->AsInteger = DiscountTime.DiscountTimeKey;
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountTime.DiscountKey;
		IBInternalQuery->ParamByName("TIME_STAMP_START")->AsDateTime = DiscountTime.StartTime;
		IBInternalQuery->ParamByName("TIME_STAMP_END")->AsDateTime = DiscountTime.EndTime;
		IBInternalQuery->ParamByName("PRIORITY_OVERRIDE")->AsInteger = 0;
		IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = 0;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteDiscountTime(Database::TDBTransaction &DBTransaction,TDiscountTimes &DiscountTime)
{
	if( !fEnabled )return;

	if(DiscountTime.DiscountTimeKey != 0)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM "
		" DISCOUNTSTIME"
		" WHERE"
		" DISCOUNTSTIME_KEY = :DISCOUNTSTIME_KEY";
		IBInternalQuery->ParamByName("DISCOUNTSTIME_KEY")->AsInteger = DiscountTime.DiscountTimeKey;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountTime(Database::TDBTransaction &DBTransaction,int DiscountTimeKey,TDiscountTimes &DiscountTimes)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT * FROM DISCOUNTSTIME "
		" WHERE "
		" DISCOUNTSTIME_KEY = :DISCOUNTSTIME_KEY ";
		IBInternalQuery->ParamByName("DISCOUNTSTIME_KEY")->AsInteger = DiscountTimeKey;
		IBInternalQuery->ExecQuery();
		if (!IBInternalQuery->Eof)
		{
			DiscountTimes.DiscountTimeKey 	= 	DiscountTimeKey;
			DiscountTimes.DiscountKey	      =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
			DiscountTimes.StartTime 	      =  IBInternalQuery->FieldByName("TIME_STAMP_START")->AsDateTime;
			DiscountTimes.EndTime 	         =  IBInternalQuery->FieldByName("TIME_STAMP_END")->AsDateTime;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountTimes(Database::TDBTransaction &DBTransaction,int DiscountKey, std::vector<TDiscountTimes> &DiscountTimes)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT * FROM DISCOUNTSTIME "
		" WHERE "
		" DISCOUNT_KEY = :DISCOUNT_KEY "
		" ORDER BY TIME_STAMP_START,TIME_STAMP_END";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
		{
			TDiscountTimes DiscountTime;
			DiscountTime.DiscountKey	      =  DiscountKey;
			DiscountTime.DiscountTimeKey 		= 	IBInternalQuery->FieldByName("DISCOUNTSTIME_KEY")->AsInteger;
			DiscountTime.StartTime 	         =  IBInternalQuery->FieldByName("TIME_STAMP_START")->AsDateTime;
			DiscountTime.EndTime 	         =  IBInternalQuery->FieldByName("TIME_STAMP_END")->AsDateTime;
			DiscountTimes.push_back(DiscountTime);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteCard(Database::TDBTransaction &DBTransaction,UnicodeString Card)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM "
	" DISCOUNTCARDS"
	" WHERE"
	" CARD = :CARD";
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteCard(Database::TDBTransaction &DBTransaction,int DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM "
	" DISCOUNTCARDS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TManagerDiscount::SetCard(Database::TDBTransaction &DBTransaction,int DiscountKey, UnicodeString Card)
{
	if( !fEnabled )return ;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	int DiscountCardKey = GetDiscountCard(DBTransaction,Card);

	if(DiscountCardKey == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DISCOUNTCARDS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		DiscountCardKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO DISCOUNTCARDS ("
		"DISCOUNTCARDS_KEY,"
		"DISCOUNT_KEY,"
		"CARD) "
		"VALUES ("
		":DISCOUNTCARDS_KEY,"
		":DISCOUNT_KEY,"
		":CARD);";
		IBInternalQuery->ParamByName("DISCOUNTCARDS_KEY")->AsInteger = DiscountCardKey;
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("CARD")->AsString = Card;
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTCARDS"
		" SET"
		" DISCOUNT_KEY = :DISCOUNT_KEY,"
		" CARD = :CARD"
		" WHERE"
		" DISCOUNTCARDS_KEY = :DISCOUNTCARDS_KEY";

		IBInternalQuery->ParamByName("DISCOUNTCARDS_KEY")->AsInteger = DiscountCardKey;
		IBInternalQuery->ParamByName("CARD")->AsString = Card;
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
int TManagerDiscount::GetDiscount(Database::TDBTransaction &DBTransaction,UnicodeString Card)
{
	int RetVal = 0;
	if( !fEnabled )return RetVal;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY"
	" FROM"
	"  DISCOUNTCARDS"
	" WHERE "
	" CARD = :CARD";
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
int TManagerDiscount::GetDiscountCard(Database::TDBTransaction &DBTransaction,UnicodeString Card)
{
	int RetVal = 0;
	if( !fEnabled )return RetVal;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNTCARDS_KEY"
	" FROM"
	"  DISCOUNTCARDS"
	" WHERE "
	" CARD = :CARD";
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("DISCOUNTCARDS_KEY")->AsInteger;
	}

	return RetVal;
}
//---------------------------------------------------------------------------
Currency TManagerDiscount::GetDiscountTotal(TList * DiscountItems)
{
	Currency RetVal = 0;
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		RetVal +=  Order->TotalDiscount();
		for (int j = 0; j < Order->SubOrders->Count  ; j++)
		{
			TItemMinorComplete *SubOrder = (TItemMinorComplete *)Order->SubOrders->Items[j];
			RetVal +=  SubOrder->TotalDiscount();
		}
	}
	return RetVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetReportDiscountInfo(Database::TDBTransaction &DBTransaction,std::set<int> &DiscountsList,TStringList *Report)
{
	try
	{
		Report->Add("<br>");
		UnicodeString Temp = TABLE_START;
		Report->Add(Temp);

		Temp = TABLE_ROW4_HEADER;
		Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Discounts");
		Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", "&nbsp;");
		Report->Add(Temp);

		UnicodeString TempRow = "";
		TempRow = TABLE_ROW4;
		TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", 	"Name");
		TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", "");
		TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", 	"Percent");
		TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", "<b>Amount</b>");

		Report->Add(TempRow);

		for (std::set<int>::iterator ptrDiscountKey = DiscountsList.begin();
		ptrDiscountKey != DiscountsList.end();
		ptrDiscountKey++)
		{
			TDiscount CurrentDiscount;
			GetDiscount(DBTransaction,*ptrDiscountKey, CurrentDiscount);
			TempRow = TABLE_ROW4;
			TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", CurrentDiscount.Name);
			TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", UnicodeString("&nbsp;"));
			TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", FormatFloat("0.0 %",CurrentDiscount.PercentAmount) );
			TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", "<b>" + FormatFloat("$0.00",CurrentDiscount.Amount) + "</b>");

			Report->Add(TempRow);
		}

		Temp =TABLE_STOP;
		Report->Add(Temp);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetReportItemDiscounts(TItemMinorComplete *Item,TStringList *Report)
{
	bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown;
	bool DiscountMidPointRoundsDown = MidPointRoundsDown;
	Currency DiscountRounding = Currency(0);

	try
	{
		Report->Add("<br>");
		UnicodeString Temp = TABLE_START;
		Report->Add(Temp);

		Temp = TABLE_ROW4_HEADER;
		Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Item Discounts");
		Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", Item->Item + " " + Item->Size);
		Report->Add(Temp);

		UnicodeString TempRow = "";
		TempRow = TABLE_ROW4S;
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","40%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","left");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
		TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Item</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL2%","<b>Price Lv 0</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL3%","<b>Price Lv 1</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL4%","<b>Price</b>");
		Report->Add(TempRow);

		TempRow = TABLE_ROW4S;
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","40%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","left");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
		TempRow = AnsiReplaceStr(TempRow, "%COL1%", Item->Item);
		TempRow = AnsiReplaceStr(TempRow, "%COL2%", FormatFloat("$0.00",Item->PriceLevel0));
		TempRow = AnsiReplaceStr(TempRow, "%COL3%", FormatFloat("$0.00",Item->PriceLevel1));
		TempRow = AnsiReplaceStr(TempRow, "%COL4%", FormatFloat("$0.00",Item->Price()));
		Report->Add(TempRow);

		TempRow = TABLE_ROW4S;
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","40%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","20%");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","left");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
		TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Discount</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Adjustment</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>Value</b>");
		TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>New Price</b>");
		Report->Add(TempRow);

		if(Item->DiscountSize() != 0)
		{

			Currency ItemBalance = Item->Price();
			for (std::vector<TDiscount>::const_iterator ptrDiscounts = Item->Discounts.begin();ptrDiscounts != Item->Discounts.end(); std::advance(ptrDiscounts,1))
			{
				TempRow = TABLE_ROW4S;
				TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","40%");
				TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","20%");
				TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","20%");
				TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","20%");
				TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","left");
				TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","right");
				TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
				TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
				TempRow = AnsiReplaceStr(TempRow, "%COL1%", ptrDiscounts->GetDescription());
				if (ptrDiscounts->Mode == DiscModePercent)
				{
					TempRow = AnsiReplaceStr(TempRow, "%COL2%",FormatFloat("%0.00",ptrDiscounts->PercentAmount));
				}
				else if (ptrDiscounts->Mode == DiscModeSetPrice)
				{
					TempRow = AnsiReplaceStr(TempRow, "%COL2%",FormatFloat("$0.00",ptrDiscounts->Amount));
				}
				else if (ptrDiscounts->Mode == DiscModeItem)
				{
					TempRow = AnsiReplaceStr(TempRow, "%COL2%",FormatFloat("$0.00",ptrDiscounts->Amount));
				}
				else if (ptrDiscounts->Mode == DiscModeCurrency)
				{
					TempRow = AnsiReplaceStr(TempRow, "%COL2%",FormatFloat("$0.00",ptrDiscounts->Amount));
				}
				else if (ptrDiscounts->Mode == DiscModeCombo)
				{
					TempRow = AnsiReplaceStr(TempRow, "%COL2%",FormatFloat("$0.00",ptrDiscounts->Amount));
				}
				Currency ThisDiscountAmount = Item->DiscountValue_BillCalc(ptrDiscounts) + DiscountRounding;
				DiscountMidPointRoundsDown = MidPointRoundsDown != ThisDiscountAmount < 0;
				Currency RoundedDiscountAmount = RoundToNearest(ThisDiscountAmount, 0.01, DiscountMidPointRoundsDown);
				DiscountRounding = ThisDiscountAmount - RoundedDiscountAmount;
				TempRow = AnsiReplaceStr(TempRow, "%COL3%", FormatFloat("$0.00",RoundedDiscountAmount));
				ItemBalance += RoundedDiscountAmount;
				TempRow = AnsiReplaceStr(TempRow, "%COL4%", FormatFloat("$0.00",RoundToNearest(ItemBalance, 0.01, MidPointRoundsDown)));
				Report->Add(TempRow);
			}
			DiscountMidPointRoundsDown = MidPointRoundsDown != Item->TotalAdjustment() < 0;
			TempRow = TABLE_ROW4S;
			TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","40%");
			TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","20%");
			TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","20%");
			TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","20%");
			TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","left");
			TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","right");
			TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
			TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
			TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Total Discount</b>");
			TempRow = AnsiReplaceStr(TempRow, "%COL2%","&nbsp;");
			TempRow = AnsiReplaceStr(TempRow, "%COL3%",FormatFloat("<b>$0.00</b>",RoundToNearest(Item->TotalAdjustment(), 0.01, DiscountMidPointRoundsDown)));
			TempRow = AnsiReplaceStr(TempRow, "%COL4%",FormatFloat("<b>$0.00</b>",RoundToNearest(Item->TotalPriceAdjustment(), 0.01, MidPointRoundsDown)));
			Report->Add(TempRow);
		}
		Temp = TABLE_STOP;
		Report->Add(Temp);

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetReportItemDiscountsHeader(TStringList *Report)
{
	UnicodeString Temp = HTML_START;
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Discount Information");
	Report->Add(Temp);
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetReportItemDiscountsFooter(TStringList *Report)
{
	UnicodeString Temp = HTML_BODY_STOP;
	Report->Add(Temp);
}
//---------------------------------------------------------------------------
void TManagerDiscount::DecreasePriority(Database::TDBTransaction &DBTransaction, int DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Find the Item with the lower Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PRIORITY FROM DISCOUNTS "
	" WHERE DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	int CurrentDiscountPriorty =  IBInternalQuery->FieldByName("PRIORITY")->AsInteger;
	// Find the Item with the lower Priority (higher number).
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY,PRIORITY FROM DISCOUNTS WHERE "
	" PRIORITY > :CURRENT_PRIORITY AND DISCOUNT_KEY != :DISCOUNT_KEY"
	" ORDER BY PRIORITY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("CURRENT_PRIORITY")->AsInteger = CurrentDiscountPriorty;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{  // Now Swap them.
		int NextDiscountKey =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		int NextDiscountPriorty =  IBInternalQuery->FieldByName("PRIORITY")->AsInteger;

		if(CurrentDiscountPriorty == NextDiscountPriorty)
		{
			NextDiscountPriorty++;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" PRIORITY = :PRIORITY"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = NextDiscountPriorty;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" PRIORITY = :PRIORITY"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = NextDiscountKey;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = CurrentDiscountPriorty;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::IncreasePriority(Database::TDBTransaction &DBTransaction, int DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Find the Item with the Higher Priority. (lower Number)
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PRIORITY FROM DISCOUNTS "
	" WHERE DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	int CurrentDiscountPriorty =  IBInternalQuery->FieldByName("PRIORITY")->AsInteger;
	// Find the Item with the Higher Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY,PRIORITY FROM DISCOUNTS WHERE "
	" PRIORITY < :CURRENT_PRIORITY AND DISCOUNT_KEY != :DISCOUNT_KEY"
	" ORDER BY PRIORITY DESC";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("CURRENT_PRIORITY")->AsInteger = CurrentDiscountPriorty;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{  // Now Swap them.
		int PrevDiscountKey =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		int PrevDiscountPriorty =  IBInternalQuery->FieldByName("PRIORITY")->AsInteger;

		if(CurrentDiscountPriorty == PrevDiscountPriorty)
		{
			PrevDiscountPriorty--;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" PRIORITY = :PRIORITY"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = PrevDiscountPriorty;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" PRIORITY = :PRIORITY"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = PrevDiscountKey;
		IBInternalQuery->ParamByName("PRIORITY")->AsInteger = CurrentDiscountPriorty;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::DecreaseAppearanceOrder(Database::TDBTransaction &DBTransaction, int DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Find the Item with the lower Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT APPEARANCE_ORDER FROM DISCOUNTS "
	" WHERE DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	int CurrentDiscountAppOrder =  IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;
	// Find the Item with the lower Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY,APPEARANCE_ORDER FROM DISCOUNTS WHERE "
	" APPEARANCE_ORDER <= :APPEARANCE_ORDER AND DISCOUNT_KEY != :DISCOUNT_KEY"
	" ORDER BY APPEARANCE_ORDER DESC";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = CurrentDiscountAppOrder;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{  // Now Swap them.
		int PrevDiscountKey =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		int PrevDiscountAppOrder =  IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;

		if(CurrentDiscountAppOrder == PrevDiscountAppOrder)
		{
			PrevDiscountAppOrder--;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" APPEARANCE_ORDER = :APPEARANCE_ORDER"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = PrevDiscountAppOrder;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" APPEARANCE_ORDER = :APPEARANCE_ORDER"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = PrevDiscountKey;
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = CurrentDiscountAppOrder;
		IBInternalQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::IncreaseAppearanceOrder(Database::TDBTransaction &DBTransaction, int DiscountKey)
{
	if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Find the Item with the lower Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT APPEARANCE_ORDER FROM DISCOUNTS "
	" WHERE DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	int CurrentDiscountAppOrder =  IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;
	// Find the Item with the lower Priority.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_KEY,APPEARANCE_ORDER FROM DISCOUNTS WHERE "
	" APPEARANCE_ORDER >= :APPEARANCE_ORDER AND DISCOUNT_KEY != :DISCOUNT_KEY"
	" ORDER BY APPEARANCE_ORDER";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = CurrentDiscountAppOrder;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{  // Now Swap them.
		int NextDiscountKey =  IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		int NextDiscountAppOrder =  IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;

		if(CurrentDiscountAppOrder == NextDiscountAppOrder)
		{
			NextDiscountAppOrder++;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" APPEARANCE_ORDER = :APPEARANCE_ORDER"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = NextDiscountAppOrder;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" DISCOUNTS"
		" SET"
		" APPEARANCE_ORDER = :APPEARANCE_ORDER"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = NextDiscountKey;
		IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = CurrentDiscountAppOrder;
		IBInternalQuery->ExecQuery();
	}
}
void TManagerDiscount::CheckDiscount(TForm *inDisplayOwner, TDiscount inDiscount, TList **Orders)
{
	switch(inDiscount.Mode)
	{
	    case DiscModeCombo:	TComboDiscount ComboDiscount(inDisplayOwner, inDiscount, *Orders);
		*Orders = ComboDiscount.GetOrders();
		break;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::PopulateDiscountGroupPerType(long DiscountKey, TDiscount &Discount)
{
	TDiscountGroupsManager DGManager;
	std::auto_ptr<TList> DGGList(new TList());

	TDiscountGroup* group;
	DGManager.LoadAssignedGroupsByType(DiscountKey, DGGList.get());

	Discount.DiscountGroupList.clear();

	for(int i=0;i<DGGList->Count;i++) {
		group = (TDiscountGroup*)DGGList->Items[i];
		Discount.DiscountGroupList.push_back(*group);

		delete group;
	}
	DGGList->Clear();   // TList::Clear() will empty the list, however it will not remove the items from the memory
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteCloudDiscounts(Database::TDBTransaction &DBTransaction,std::vector<AnsiString> discountsCodes)
{
	if( !fEnabled )
      return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	AnsiString query =
	"DELETE FROM "
	"DISCOUNTS "
	"WHERE "
	"IS_CLOUD_DISCOUNT = 'T' ";
    int size = discountsCodes.size();
    if(size > 0)
    {
      query += " and discount_id not in (";
      for(std::vector<AnsiString>::iterator itDiscountCode = discountsCodes.begin() ; itDiscountCode != discountsCodes.end() ; ++itDiscountCode)
          {
               query += "'" + *itDiscountCode + "'";
               size--;
               if(size > 0)
                query += ",";
          }
      query += ");";;
    }
    IBInternalQuery->SQL->Text  = query;
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TManagerDiscount::DeleteDiscounts(Database::TDBTransaction &DBTransaction)
{
	if( !fEnabled )
      return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM DISCOUNTS";
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
bool TManagerDiscount::IsCloudDiscount(Database::TDBTransaction &DBTransaction,long discountKey)
{
	if( !fEnabled )
      return false;
    bool retVal = false;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT IS_CLOUD_DISCOUNT FROM "
	"DISCOUNTS "
	"WHERE "
	"DISCOUNT_KEY = :DISCOUNT_KEY";
    IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = discountKey;
	IBInternalQuery->ExecQuery();

    if(!IBInternalQuery->Eof)
    {
        if(IBInternalQuery->FieldByName("IS_CLOUD_DISCOUNT")->AsString == "T")
          retVal = true;
    }

    return retVal;
}
//-----------------------------------------------------------------------------------------------------------------------------
void TManagerDiscount::GetMembershipDiscounts(Database::TDBTransaction &DBTransaction,std::set<int> &discountKeys)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT DISCOUNT_KEY FROM "
	"DISCOUNTS "
	"WHERE "
	"IS_MEMBERSHIP_DISCOUNT = 'T'";
	IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
       discountKeys.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
	}
}
