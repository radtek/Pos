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

#include "StringTableRes.h"
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
TDiscountMode TManagerDiscount::GetDiscountMode(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if(GetDiscountPercent(DBTransaction,DiscountKey) != 0 )
	{
		return DiscModePercent;
	}
	else
	{
		return DiscModeCurrency;
	}
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetVoucherListThor(Database::TDBTransaction &tr,TStringList *destination_list,bool ShowPointsAsDiscount)
{

	TIBSQL *q;

	if (!fEnabled || destination_list == NULL)
	return;

 	q = tr.Query(tr.AddQuery());
    q->Close();

    q->SQL->Text =  "select thor_vouchers_key key, name from thorlink_vouchers "
                    "where VOUCHER_GROUP = 1 order by appearance_order asc";

	for (q->ExecQuery(); !q->Eof; q->Next())
    {
		const int index =
		destination_list->Add(q->FieldByName("name")->AsString);
		destination_list->Objects[index] =
		reinterpret_cast<TObject *>(q->FieldByName("key")->AsInteger);
	}

	if(TGlobalSettings::Instance().EnableSeperateEarntPts && ShowPointsAsDiscount)
	{
		const int index = destination_list->Add("Loyalty Points");
		destination_list->Objects[index] = reinterpret_cast<TObject *>(dsMMMebersPointsKEY);
	}

}
//---------------------------------------------------------------------------
void TManagerDiscount::GetDiscountList(Database::TDBTransaction &tr,TStringList *destination_list,const discount_list_combo_filter_t filter, bool ShowPointsAsDiscount)
{
	static const UnicodeString queries[1][__num_discount_list_combo_filters] = {
		/* menu not specified. */ {
			/* exclude_combos */ (
			L"select discount_key key, "
			"       name "
			"       from discounts "
			"       where discount_mode <> :combo_discount_mode "
			"       order by appearance_order asc;"
			), /* include_combos */ (
			L"select discount_key key, "
			"       name "
			"       from discounts "
			"       order by appearance_order asc;"
			), /* only_combos */ (
			L"select discount_key key, "
			"       name "
			"       from discounts "
			"       where discount_mode = :combo_discount_mode "
			"       order by appearance_order asc;"
			)
		}
	};
	TIBSQL *q;

	if (!fEnabled || destination_list == NULL)
	return;

	q = tr.Query(tr.AddQuery());
	q->SQL->Text = queries[0][filter];
	if (filter == exclude_combos || filter == only_combos)
	q->ParamByName(L"combo_discount_mode")->AsInteger = DiscModeCombo;

	for (q->ExecQuery(); !q->Eof; q->Next()) {
		const int index =
		destination_list->Add(q->FieldByName(L"name")->AsString);
		destination_list->Objects[index] =
		reinterpret_cast<TObject *>(q->FieldByName(L"key")->AsInteger);
	}

	/* TODO 1 -o Michael -c Kludge : This code is less than ideal
		we are fudging the creation of a Discount here and the  Manager Dicount will
		recoginse the Key and supply an appropriate system discount when queryied.
		In an ideal world the membership discount around points would be a system
		discount and these 'system discounts' would live in the DB and
		show where appropriate. */

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
bool TManagerDiscount::SetAmountDB(Database::TDBTransaction &DBTransaction,long DiscountKey, Currency Amount)
{
	if( !fEnabled )return false;
	bool ReturnVal = false;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());


	if(DiscountKey != 0)
	{
           IBInternalQuery->SQL->Clear();
           IBInternalQuery->SQL->Text =
              "UPDATE THORLINK_VOUCHERS SET "
                 "AMOUNT             = :AMOUNT "
                 " WHERE THOR_VOUCHERS_KEY = :THOR_VOUCHERS_KEY";
           IBInternalQuery->ParamByName("THOR_VOUCHERS_KEY")->AsInteger    = DiscountKey;
           IBInternalQuery->ParamByName("AMOUNT")->AsString	            = Amount;
           IBInternalQuery->ExecQuery();
	}

//	PopulateDiscountGroupPerType(DiscountKey, Discount);
	return ReturnVal;
}
//---------------------------------------------------------------------------
bool TManagerDiscount::GetThorlinkDiscount(Database::TDBTransaction &DBTransaction,TDiscount &Discount)
{
	if( !fEnabled )return false;
	bool ReturnVal = false;
//
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	if(Discount.DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT THORLINK_VOUCHERS.VOUCHERS_CODE"
		" FROM"
		" THORLINK_VOUCHERS"
        " LEFT JOIN DISCOUNTS ON THORLINK_VOUCHERS.PROPERTIES = DISCOUNTS.DISCOUNT_ID"
		" WHERE"
		" DISCOUNT_KEY = :DISCOUNT_KEY";
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = Discount.DiscountKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
            Discount.VoucherCode = IBInternalQuery->FieldByName("VOUCHERS_CODE")->AsString;
			ReturnVal = true;
		}
	}
	return ReturnVal;
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
		Discount.ID = dsMMMebersPointsKEY;
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
		ReturnVal = true;
	}
	else if(DiscountKey == dsMMDealKey)
	{
		Discount.DiscountKey = dsMMDealKey;
		Discount.ID = dsMMDealKey;
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
			Discount.ID = IBInternalQuery->FieldByName("DISCOUNT_ID")->AsInteger;
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
void TManagerDiscount::GetThorVoucherCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountKey != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT CATEGORY_KEY"
		" FROM"
		"  THORVOUCHERS_CATEGORIES"
		" WHERE "
		" THOR_VOUCHERS_KEY = :THOR_VOUCHERS_KEY "
        " AND Is_THORVOUCHERS = :Is_THORVOUCHER ";
		IBInternalQuery->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = DiscountKey;
        IBInternalQuery->ParamByName("Is_THORVOUCHER")->AsString = "T";
		for(IBInternalQuery->ExecQuery();!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			Discount.CategoryFilterKeys.insert(IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger);
		}
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
int TManagerDiscount::GetDiscountKeyByID(Database::TDBTransaction &DBTransaction,BYTE DiscountID)
{
	if( !fEnabled )return false;
	int ReturnVal = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(DiscountID != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT DISCOUNT_KEY"
		" FROM"
		"  DISCOUNTS"
		" WHERE"
		" DISCOUNT_ID = :DISCOUNT_ID";
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = DiscountID;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			ReturnVal = IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
		}
	}
	return ReturnVal;
}
//---------------------------------------------------------------------------
BYTE TManagerDiscount::GetDiscountIDByKey(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return false;
	BYTE ReturnVal = 0;

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
			ReturnVal = (BYTE)IBInternalQuery->FieldByName("DISCOUNT_ID")->AsInteger;
		}
	}
	return ReturnVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::DiscountIDToKey(Database::TDBTransaction &DBTransaction,std::set<byte> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts)
{
	for(std::set<byte>::iterator itDiscountsID = AutoAppliedDiscountsID.begin(); itDiscountsID != AutoAppliedDiscountsID.end(); std::advance(itDiscountsID,1) )
	{
		int DiscountKey = GetDiscountKeyByID(DBTransaction,*itDiscountsID);
		if(DiscountKey)
		{
			AutoAppliedDiscounts.insert(DiscountKey);
		}
	}
};
//---------------------------------------------------------------------------
void TManagerDiscount::DiscountKeyToID(Database::TDBTransaction &DBTransaction,std::set<byte> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts)
{
	for(std::set<int>::iterator itDiscountsKey = AutoAppliedDiscounts.begin(); itDiscountsKey != AutoAppliedDiscounts.end(); std::advance(itDiscountsKey,1) )
	{
		BYTE DiscountID = GetDiscountIDByKey(DBTransaction,*itDiscountsKey);
		if(DiscountID)
		{
			AutoAppliedDiscountsID.insert(DiscountID);
		}
	}
};
//---------------------------------------------------------------------------
TDiscountType TManagerDiscount::GetDiscountType(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return dtFixed;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	TDiscountType ReturnVal = dtFixed;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT DISCOUNT_TYPE"
	" FROM"
	"  DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		ReturnVal = static_cast<TDiscountType>(IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger);
	}

	return ReturnVal;
}
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
double TManagerDiscount::GetDiscountPercent(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	double ReturnVal = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PERCENTAGE"
	" FROM"
	"  DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		ReturnVal = IBInternalQuery->FieldByName("PERCENTAGE")->AsFloat;
	}

	return ReturnVal;

}
//---------------------------------------------------------------------------
Currency TManagerDiscount::GetDiscountAmount(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency ReturnVal = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT AMOUNT"
	" FROM"
	"  DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		ReturnVal = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
	}

	return ReturnVal;
}
//---------------------------------------------------------------------------
Currency TManagerDiscount::GetDiscountRounding(Database::TDBTransaction &DBTransaction,long DiscountKey)
{
	if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency ReturnVal = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT ROUNDING"
	" FROM"
	"  DISCOUNTS"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		ReturnVal = IBInternalQuery->FieldByName("ROUNDING")->AsCurrency;
	}

	return ReturnVal;
}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountName(Database::TDBTransaction &DBTransaction,long DiscountKey,UnicodeString inName)
{
	if( !fEnabled )return ;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" DISCOUNTS"
	" SET"
	" NAME = :NAME"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";
	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("NAME")->AsString = inName;
	IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountDescription(Database::TDBTransaction &DBTransaction,long DiscountKey,UnicodeString inDescription)
{
	if( !fEnabled )return ;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" DISCOUNTS"
	" SET"
	" DESCRIPTION = :DESCRIPTION"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";

	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("DESCRIPTION")->AsString = inDescription;
	IBInternalQuery->ExecQuery();


}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountPercent(Database::TDBTransaction &DBTransaction,long DiscountKey, double inPercent)
{
	if( !fEnabled )return ;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" DISCOUNTS"
	" SET"
	" PERCENTAGE = :PERCENTAGE"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";

	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("PERCENTAGE")->AsFloat = inPercent;
	IBInternalQuery->ExecQuery();


}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscountAmount(Database::TDBTransaction &DBTransaction,long DiscountKey, Currency inAmount)
{
	if( !fEnabled )return ;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" DISCOUNTS"
	" SET"
	" AMOUNT = :AMOUNT"
	" WHERE"
	" DISCOUNT_KEY = :DISCOUNT_KEY";

	IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = inAmount;
	IBInternalQuery->ExecQuery();

}
//---------------------------------------------------------------------------
void TManagerDiscount::SetDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount Discount)
{
	if( !fEnabled )return ;

	if(DiscountIDExisits(DBTransaction,DiscountKey,Discount.ID))
	{
		throw Exception("A Discount with this Discount ID Already Exists.");
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
		"DISCOUNT_GROUP) "
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
		":DISCOUNT_GROUP );";
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

		if(Discount.ID < 1)
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = Discount.ID;
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
		"DISCOUNT_GROUP = :DISCOUNT_GROUP"
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
		if(Discount.ID < 1)
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = Discount.ID;
		}
		IBInternalQuery->ExecQuery();
	}
	SetDiscountCategories(DBTransaction,DiscountKey,Discount);
}
//---------------------------------------------------------------------------
bool TManagerDiscount::DiscountIDExisits(Database::TDBTransaction &DBTransaction,long DiscountKey, int DiscountID)
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
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = DiscountID;
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
		IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = DiscountID;
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
void TManagerDiscount::ClearThorVouchersDiscounts(TList * DiscountItems)
{
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		Order->ThorVouchersDiscountsClear();
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
int __fastcall SortByFinalPrice(void *Item1,void *Item2)
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
void  TManagerDiscount::AddDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{

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
    allItems->Sort(&SortByFinalPrice);
    switch(DiscountToBeApplied.Mode)
    {
       case DiscModeCurrency:
            AddCurrencyModeDiscount(allItems,DiscountToBeApplied);
		    break;
       case DiscModeCombo:
            AddComboDiscount(DiscountItems,DiscountToBeApplied);
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

    double maxDiscountQty = DiscountToBeApplied.MaxItemAffected == 0 ? orderQty : DiscountToBeApplied.MaxItemAffected;
    Currency discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    TDiscount scaled_discount;
    scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
    scaled_discount.Source = DiscountToBeApplied.Source;
    scaled_discount.MaximumValue = DiscountToBeApplied.MaximumValue;
    scaled_discount.Priority = DiscountToBeApplied.Priority;
    scaled_discount.IsThorBill = DiscountToBeApplied.IsThorBill;
    scaled_discount.VoucherCode = DiscountToBeApplied.VoucherCode;
    scaled_discount.Name 		= DiscountToBeApplied.Name;
    scaled_discount.Description = DiscountToBeApplied.Description;
    scaled_discount.Mode = DiscountToBeApplied.Mode;
    scaled_discount.MaxItemAffected = DiscountToBeApplied.MaxItemAffected;
    scaled_discount.MinItemRequired = DiscountToBeApplied.MinItemRequired;

    Currency  itemDiscount = 0;
    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty >= order->GetQty() )
         {
            scaled_discount.Amount = discount;
            order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
            Currency unitPrice = order->GrandTotal()/order->GetQty();
            itemDiscount = ((discount * maxDiscountQty)+((order->GetQty() - maxDiscountQty) * unitPrice));
            scaled_discount.Amount = itemDiscount/order->GetQty();
            order->DiscountAdd(scaled_discount);
         }
         else
         {
           scaled_discount.Mode = DiscModeCurrency;
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

    double remainingQty = DiscountToBeApplied.MaxItemAffected == 0 ? orderQty : DiscountToBeApplied.MaxItemAffected;
    TDiscount scaled_discount;
    scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
    scaled_discount.Source = DiscountToBeApplied.Source;
    scaled_discount.MaximumValue = DiscountToBeApplied.MaximumValue;
    scaled_discount.Priority = DiscountToBeApplied.Priority;
    scaled_discount.DiscountGroupList = DiscountToBeApplied.DiscountGroupList;
    scaled_discount.IsThorBill = DiscountToBeApplied.IsThorBill;
    scaled_discount.VoucherCode = DiscountToBeApplied.VoucherCode;
    scaled_discount.Name 		= DiscountToBeApplied.Name;
	scaled_discount.Description = DiscountToBeApplied.Description;
    scaled_discount.Mode = DiscountToBeApplied.Mode;
    scaled_discount.MaxItemAffected = DiscountToBeApplied.MaxItemAffected;
    scaled_discount.MinItemRequired = DiscountToBeApplied.MinItemRequired;
    Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        if(remainingQty >= order->GetQty())
         {
           scaled_discount.PercentAmount = DiscountToBeApplied.PercentAmount;
           order->DiscountAdd(scaled_discount);
         }
         else if(remainingQty > 0)
         {
           scaled_discount.PercentAmount =  (DiscountToBeApplied.PercentAmount * remainingQty)/order->GetQty();
           scaled_discount.PercentAmount = scaled_discount.PercentAmount;
           order->DiscountAdd(scaled_discount);
         }
        else
         {
           scaled_discount.Mode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
        remainingQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddCurrencyModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
	TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double maxDiscountQty = DiscountToBeApplied.MaxItemAffected == 0 ? orderQty : DiscountToBeApplied.MaxItemAffected;
    Currency order_total = 0;
    Currency discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);
    order_total = GetOrderTotal(DiscountItems,DiscountToBeApplied,maxDiscountQty);

    if(order_total < 0)
      return;

    TDiscount scaled_discount;
    scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
    scaled_discount.Source = DiscountToBeApplied.Source;
    scaled_discount.MaximumValue = DiscountToBeApplied.MaximumValue;
    scaled_discount.Priority = DiscountToBeApplied.Priority;
    scaled_discount.IsThorBill = DiscountToBeApplied.IsThorBill;
    scaled_discount.VoucherCode = DiscountToBeApplied.VoucherCode;
    scaled_discount.Name 		= DiscountToBeApplied.Name;
    scaled_discount.Description = DiscountToBeApplied.Description;
    scaled_discount.MaxItemAffected = DiscountToBeApplied.MaxItemAffected;
    scaled_discount.MinItemRequired = DiscountToBeApplied.MinItemRequired;
    Currency  itemDiscount = 0;

    //if discount amount is greater than order total then change discount amount to order total
    if(order_total < discount)
       discount = order_total;


    for(int x = 0; x < DiscountItems->Count; x++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[x]);
        if(maxDiscountQty >= order->GetQty())
         {
            itemDiscount = (order->GrandTotal() * discount )/(order_total * order->GetQty());
            scaled_discount.Amount = itemDiscount;
            order->DiscountAdd(scaled_discount);
         }
         else if(maxDiscountQty > 0)
         {
            itemDiscount = (order->GrandTotal() * discount * maxDiscountQty)/(order_total * order->GetQty()* order->GetQty());
            scaled_discount.Amount = itemDiscount;
            order->DiscountAdd(scaled_discount);
         }
         else
         {
           scaled_discount.Mode = DiscModeCurrency;
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

        /*
        * We need to do this so that we can prevent applying the same discount
        * on the same item multiple times. See DiscountApplied(...).
        */
        scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
        scaled_discount.Source = DiscountToBeApplied.Source;
        scaled_discount.Mode = DiscModeCombo;
        scaled_discount.Priority = DiscountToBeApplied.Priority;
        /*
        * Flatten the tree and calculate the order total.
        * We could probably turn this into a set of functions but where to put
        * them?
        */
        Currency GrandTotal = 0;
		for (unsigned int k, l; i < j; i++) {
			order =
			reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
			temp = order->GrandTotalExcCombo()/(order->GetQty());

			GrandTotal += order->GrandTotalExcCombo()/(order->GetQty())*(order->SelectedItems - order->PrevSelectedItems);
			order_total += temp;
			if (order->DiscountApplies(DiscountToBeApplied))
			{
				item_totals.push_back(temp);
				ordered_items.push_back(order);
			}

			for (k = 0, l = order->SubOrders->Count; k < l; k++)
            {
				side = order->SubOrders->SubOrderGet(k);
				temp = side->GrandTotalExcCombo()/(side->GetQty());
				GrandTotal += side->GrandTotalExcCombo()/(order->GetQty())*(order->SelectedItems - order->PrevSelectedItems);
				order_total += temp;
				if (side->DiscountApplies(DiscountToBeApplied))
				{
					item_totals.push_back(temp);
					ordered_items.push_back(side);
				}
			}
		}

		scaled_discount.Name 		= DiscountToBeApplied.Name;
		scaled_discount.Description = DiscountToBeApplied.Description;
		scaled_discount.ComboAmount = DiscountToBeApplied.ComboAmount;

		Currency con = 0;
		Currency sum = 0;
		for (i = 0, j = ordered_items.size(); i < j; i++)
		{
			int rem = ordered_items.at(i)->GetQty() - ordered_items.at(i)->PrevSelectedItems;
			Currency Total = ordered_items.at(i)->GrandTotalExcCombo()/ordered_items.at(i)->GetQty()*rem;
			int remain = ordered_items.at(i)->GetQty() - ordered_items.at(i)->SelectedItems;

			if(ordered_items.at(i)->GetQty() > 1)
			{
				double F = item_totals.at(i)*(ordered_items.at(i)->SelectedItems - ordered_items.at(i)->PrevSelectedItems)/GrandTotal;
				scaled_discount.ComboAmount = F*DiscountToBeApplied.ComboAmount;
				con = (Total/rem*(rem - ordered_items.at(i)->SelectedItems + ordered_items.at(i)->PrevSelectedItems));
				scaled_discount.Amount = RoundToNearest(con + F*DiscountToBeApplied.ComboAmount + ordered_items.at(i)->DiscountGet(DiscModeCombo), scaled_discount.Rounding, false);
				if(ordered_items.at(i)->DiscountGet(DiscModeCombo) != 0)
				{
					scaled_discount.ComboAmount = RoundToNearest(ordered_items.at(i)->DiscountGet(DiscModeCombo) + F*DiscountToBeApplied.ComboAmount, scaled_discount.Rounding, false);
					sum += scaled_discount.ComboAmount;
				}
				Currency tmp = (ordered_items.at(i)->GrandTotal() - scaled_discount.Amount)/(ordered_items.at(i)->SelectedItems - ordered_items.at(i)->PrevSelectedItems);
				remaining_discount += (item_totals.at(i) - tmp)*(ordered_items.at(i)->SelectedItems - ordered_items.at(i)->PrevSelectedItems);
				ordered_items.at(i)->DiscountByTypeRemove(DiscModeCombo);
				ordered_items.at(i)->DiscountAdd(scaled_discount);

			}
			else
			{
				scaled_discount.Amount = RoundToNearest((item_totals.at(i) / GrandTotal * DiscountToBeApplied.Amount), scaled_discount.Rounding, false);
				scaled_discount.PercentAmount = item_totals.at(i) / order_total;
				ordered_items.at(i)->DiscountAdd(scaled_discount);
				remaining_discount += (scaled_discount.Amount);
			}
			ordered_items.at(i)->PrevSelectedItems = ordered_items.at(i)->SelectedItems;
		}
		Currency dif = DiscountToBeApplied.ComboAmount - remaining_discount ;
		if(dif != 0)
		{
			TDiscount &dsc = ordered_items.at(i - 1)->Discount_Back();
			dsc.Amount += dif;
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
		scaled_discount.Priority = DiscountToBeApplied.Priority;


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
                for (std::vector <TDiscount> ::iterator ptrDiscounts = ordered_items.at(i)->DiscountsBegin();
                     ptrDiscounts != ordered_items.at(i)->DiscountsEnd(); std::advance(ptrDiscounts, 1))
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

    double remainingQty = DiscountToBeApplied.MaxItemAffected == 0 ? orderQty : DiscountToBeApplied.MaxItemAffected;
    TDiscount scaled_discount;
    scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
    scaled_discount.Source = DiscountToBeApplied.Source;
    scaled_discount.MaximumValue = DiscountToBeApplied.MaximumValue;
    scaled_discount.Priority = DiscountToBeApplied.Priority;
    scaled_discount.DiscountGroupList = DiscountToBeApplied.DiscountGroupList;
    scaled_discount.IsThorBill = DiscountToBeApplied.IsThorBill;
    scaled_discount.VoucherCode = DiscountToBeApplied.VoucherCode;
    scaled_discount.Name 		= DiscountToBeApplied.Name;
	scaled_discount.Description = DiscountToBeApplied.Description;
    scaled_discount.Mode = DiscountToBeApplied.Mode;
    scaled_discount.MaxItemAffected = DiscountToBeApplied.MaxItemAffected;
    scaled_discount.MinItemRequired = DiscountToBeApplied.MinItemRequired;
    Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);
    Currency itemDiscount = 0;
    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        if(remainingQty >= order->GetQty())
         {
           scaled_discount.Amount = discount;
           order->DiscountAdd(scaled_discount);
         }
         else if(remainingQty > 0)
         {
           scaled_discount.Mode = DiscModeCurrency;
           itemDiscount = discount;
           if(discount * order->GetQty() >  order->GrandTotal())
              itemDiscount = order->GrandTotal()/order->GetQty();
           scaled_discount.Amount =  (itemDiscount * remainingQty)/order->GetQty();
           order->DiscountAdd(scaled_discount);
         }
        else
         {
           scaled_discount.Mode = DiscModeCurrency;
           scaled_discount.Amount =  0;
           order->DiscountAdd(scaled_discount);
         }
        remainingQty -= order->GetQty();
     }
}
//---------------------------------------------------------------------------
void  TManagerDiscount::AddPointModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied)
{
    TItemMinorComplete *order;
    double orderQty = GetOrderQty(DiscountItems);
    if(orderQty < DiscountToBeApplied.MinItemRequired)
       return;

    double remainingQty = DiscountToBeApplied.MaxItemAffected == 0 ? orderQty : DiscountToBeApplied.MaxItemAffected;
    TDiscount scaled_discount;
    scaled_discount.DiscountKey = DiscountToBeApplied.DiscountKey;
    scaled_discount.Source = DiscountToBeApplied.Source;
    scaled_discount.MaximumValue = DiscountToBeApplied.MaximumValue;
    scaled_discount.Priority = DiscountToBeApplied.Priority;
    scaled_discount.DiscountGroupList = DiscountToBeApplied.DiscountGroupList;
    scaled_discount.IsThorBill = DiscountToBeApplied.IsThorBill;
    scaled_discount.VoucherCode = DiscountToBeApplied.VoucherCode;
    scaled_discount.Name 		= DiscountToBeApplied.Name;
	scaled_discount.Description = DiscountToBeApplied.Description;
    scaled_discount.Mode = DiscountToBeApplied.Mode;
    scaled_discount.MaxItemAffected = DiscountToBeApplied.MaxItemAffected;
    scaled_discount.MinItemRequired = DiscountToBeApplied.MinItemRequired;
    scaled_discount.Amount = 0;
    Currency  discount = RoundToNearest(DiscountToBeApplied.Amount, DiscountToBeApplied.Rounding, TGlobalSettings::Instance().MidPointRoundsDown);

    for(int i = 0; i < DiscountItems->Count ; i++)
     {
        order = reinterpret_cast<TItemMinorComplete *>(DiscountItems->Items[i]);
        if(remainingQty >= order->GetQty())
         {
            order->ItemPriceForPoints =  discount;
            order->PayByPointsQuantity = order->GetQty();
         }
         else if(remainingQty > 0)
         {
            //Currency amount =  ((remainingQty * discount) + ((order->GetQty() - remainingQty) * order->ItemPriceForPoints)/order->GetQty());
            order->ItemPriceForPoints = discount;//RoundToNearest(amount,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
            order->PayByPointsQuantity = remainingQty;
         }
        order->ItemPriceForPoints =  discount;
        order->DiscountAdd(scaled_discount);
        remainingQty -= order->GetQty();
     }
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
		//Auto Discounts wont add a discount twice to an order to stop double ups.
 		if(!Order->DiscountApplied(*ptrDiscountKey) )
		{
			TDiscount CurrentDiscount;
			GetDiscount(DBTransaction,*ptrDiscountKey, CurrentDiscount);
            if(CurrentDiscount.Mode == DiscModeItem)
            {
              CurrentDiscount.Mode = DiscModeCurrency;
            }
            if(CurrentDiscount.MinItemRequired <= 1)
			   Order->DiscountAddIncSides(CurrentDiscount);
		}
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
Currency TManagerDiscount::GetDiscountAmount(TDiscount DiscountToApply, double Amount, TList * DiscountItems)
{
	Currency RetVal = 0;
	Currency OrdersTotal = 0;
	for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		if((!DiscountToApply.CategoryFilterKeys.empty() && DiscountToApply.ContainsCatKey(Order->Categories)) || DiscountToApply.CategoryFilterKeys.empty())
		{
			// Add up the Total Value.
			OrdersTotal += Order->Price();
		}

		for (int j = 0; j < Order->SubOrders->Count  ; j++)
		{
			TItemMinorComplete *SubOrder = (TItemMinorComplete *)Order->SubOrders->Items[j];
			if((!DiscountToApply.CategoryFilterKeys.empty() && DiscountToApply.ContainsCatKey(SubOrder->Categories)) || DiscountToApply.CategoryFilterKeys.empty())
			{
				OrdersTotal += SubOrder->Price();
			}
		}

	}

	if (DiscountToApply.Mode == DiscModePercent)
	{
		RetVal = RoundToNearest(Amount * double(OrdersTotal) / double(100.0),DiscountToApply.Rounding,TGlobalSettings::Instance().MidPointRoundsDown);
	}
	else if (DiscountToApply.Mode == DiscModeSetPrice)
	{
		RetVal = RoundToNearest(Amount,DiscountToApply.Rounding,TGlobalSettings::Instance().MidPointRoundsDown);
	}
	//************4278*********************/
	else if (DiscountToApply.Mode == DiscModeItem)
	{   /*
		for (int i = 0; i < DiscountItems->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *) DiscountItems->Items[i];
		Amount+=Amount;
		for (int j = 0; j < Order->SubOrders->Count  ; j++)
		{
			TItemMinorComplete *SubOrder = (TItemMinorComplete *)Order->SubOrders->Items[j];
		Amount+=Amount;
		}
	} */
		RetVal = RoundToNearest(Amount , DiscountToApply.Rounding,TGlobalSettings::Instance().MidPointRoundsDown);
	}
	//************4278*********************/
	else if (DiscountToApply.Mode == DiscModeCurrency)
	{
		if(Amount > OrdersTotal)
		{
			RetVal = OrdersTotal;
		}
		else
		{
			RetVal = Amount;
		}
	}
	return RetVal;
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
		UnicodeString Temp = LoadStr(TABLE_START);
		Report->Add(Temp);

		Temp = LoadStr(TABLE_ROW4_HEADER);
		Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Discounts");
		Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", "&nbsp;");
		Report->Add(Temp);

		UnicodeString TempRow = "";
		TempRow = LoadStr(TABLE_ROW4);
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
			TempRow = LoadStr(TABLE_ROW4);
			TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", CurrentDiscount.Name);
			TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", UnicodeString("&nbsp;"));
			TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", FormatFloat("0.0 %",CurrentDiscount.PercentAmount) );
			TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", "<b>" + FormatFloat("$0.00",CurrentDiscount.Amount) + "</b>");

			Report->Add(TempRow);
		}

		Temp = LoadStr(TABLE_STOP);
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
		UnicodeString Temp = LoadStr(TABLE_START);
		Report->Add(Temp);

		Temp = LoadStr(TABLE_ROW4_HEADER);
		Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Item Discounts");
		Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", Item->Item + " " + Item->Size);
		Report->Add(Temp);

		UnicodeString TempRow = "";
		TempRow = LoadStr(TABLE_ROW4S);
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

		TempRow = LoadStr(TABLE_ROW4S);
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

		TempRow = LoadStr(TABLE_ROW4S);
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
			for (std::vector<TDiscount>::const_iterator ptrDiscounts = Item->DiscountsBegin();ptrDiscounts != Item->DiscountsEnd(); std::advance(ptrDiscounts,1))
			{
				TempRow = LoadStr(TABLE_ROW4S);
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
			TempRow = LoadStr(TABLE_ROW4S);
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
		Temp = LoadStr(TABLE_STOP);
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
	UnicodeString Temp = LoadStr(HTML_START);
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Discount Information");
	Report->Add(Temp);
}
//---------------------------------------------------------------------------
void TManagerDiscount::GetReportItemDiscountsFooter(TStringList *Report)
{
	UnicodeString Temp = LoadStr(HTML_BODY_STOP);
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
//---------------------------------------------------------------------------
void TManagerDiscount::BuildXMLListDiscounts(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data)
{
	try
	{
		// Update the IntaMate ID with the Invoice Number.
		Data.Doc.Clear();
		TiXmlDeclaration * decl = new TiXmlDeclaration(  _T("1.0"), _T("UTF-8"), _T("") );
		Data.Doc.LinkEndChild( decl );

		// Insert DOCTYPE definiation here.
		TiXmlElement * List = new TiXmlElement( xmlEleListDiscounts );
		List->SetAttribute(xmlAttrID, AnsiString(Data.IntaMateID).c_str());
		List->SetAttribute(xmlAttrSiteID, TGlobalSettings::Instance().SiteID);

		std::auto_ptr<TStringList>DiscountList(new TStringList);
		ManagerDiscount->GetDiscountList(DBTransaction,DiscountList.get());

		for (int i = 0; i < DiscountList->Count; i++)
		{
			TDiscount Discount;
			GetDiscount(DBTransaction,(int)DiscountList->Objects[i], Discount);

			TiXmlElement *EleDiscount = new TiXmlElement( xmlEleDiscount );
			EleDiscount->SetAttribute(xmlAttrID,         Discount.DiscountKey);
			EleDiscount->SetAttribute(xmlAttrName,       Discount.Name.t_str() );
			EleDiscount->SetAttribute(xmlRequestType,    Discount.Type );
			EleDiscount->SetAttribute(xmlAttrMode,       Discount.Mode );
			EleDiscount->SetAttribute(xmlAttrDiscription,Discount.Description.t_str() );
			EleDiscount->SetAttribute(xmlAttrPercent,    Discount.PercentAmount );
			EleDiscount->SetAttribute(xmlAttrAmount,     Discount.Amount );
			EleDiscount->SetAttribute(xmlAttrRounding,   Discount.Rounding );
			EleDiscount->SetAttribute(xmlAttrInitials,   Discount.Source );

			List->LinkEndChild( EleDiscount );
		}
		Data.Doc.LinkEndChild( List );
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
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
int TManagerDiscount::GetDiscountKeyForVoucher(int id)
{
    int discountKey = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        " SELECT NAME"
        " FROM "
        "  THORLINK_VOUCHERS "
        " WHERE "
        " THOR_VOUCHERS_KEY = :THOR_VOUCHERS_KEY ";
        IBInternalQuery->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = id;
        IBInternalQuery->ExecQuery();
        AnsiString voucherName = IBInternalQuery->FieldByName("NAME")->AsString;
        int code = atoi((voucherName.SubString(0 , (voucherName.Pos(":") - 1))).c_str());

        TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery1->Close();
        IBInternalQuery1->SQL->Text =
        " SELECT DISCOUNT_KEY"
        " FROM "
        "  DISCOUNTS "
        " WHERE "
        " DISCOUNT_ID = :DISCOUNT_ID ";
        IBInternalQuery1->ParamByName("DISCOUNT_ID")->AsInteger = code;
        IBInternalQuery1->ExecQuery();
        if(IBInternalQuery1->RecordCount > 0)
        {
            discountKey = IBInternalQuery1->FieldByName("DISCOUNT_KEY")->AsInteger;
        }
        DBTransaction.Commit();
     }
     catch(Exception & E)
     {
        DBTransaction.Rollback();
 		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
     }
     return discountKey;
}
//------------------------------------------------------------------------------------------------------------------------------------
bool TManagerDiscount::IsVouchersAvailable()
{
    Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
    dBTransaction.StartTransaction();

    TIBSQL *ibInternalQuery = dBTransaction.Query(dBTransaction.AddQuery());
    bool retval = false;

	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "select *  "
                                        "FROM THORLINK_VOUCHERS " ;
		ibInternalQuery->ExecQuery();
        if(ibInternalQuery->RecordCount > 0)
        {
            retval = true;
        }
        dBTransaction.Commit();
        return retval;
	}
	catch(Exception &E)
	{
        dBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------
