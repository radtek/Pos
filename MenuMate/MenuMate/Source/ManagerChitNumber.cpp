 //---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerChitNumber.h"
#include <StrUtils.hpp>
#include <sstream>
#include <string>
#include <iostream>
//---------------------------------------------------------------------------

#pragma package(smart_init)

const chit_option_name_set_t TManagerChitNumber::no_options;
const chit_option_name_set_t TManagerChitNumber::no_option_additions;
const chit_option_name_set_t TManagerChitNumber::no_options_deleted;
const chit_option_rename_map_t TManagerChitNumber::no_options_renamed;

const bool
listed_chit_present(const std::map<int, TChitNumber> &chits)
{
    bool present = false;

    for (std::map<int, TChitNumber>::const_iterator i = chits.begin();
           !present && i != chits.end(); ++i)
        present |= i->second.Format == ectList;

    return present;
}

TManagerChitNumber::TManagerChitNumber()
{
    listed_chit_present_ = false;
}

void TManagerChitNumber::Initialise(Database::TDBTransaction &DBTransaction,int DeviceKey)
{
	CurrentKey = 0;
	LastAccesedChitKey = 0;
   TerminalKey = DeviceKey;
   LoadAll(DBTransaction);
   First(false);
}

void TManagerChitNumber::Add(Database::TDBTransaction &tr,
                             TChitNumber &ChitNumber,
                             const chit_option_name_set_t &options)
{

    const chit_key_t key = db_chit_t::add_chit(ChitNumber.DefaultMenu().c_str(),
                                               ChitNumber.Mask.c_str(),
                                               ChitNumber.Name.c_str(),
                                               options,
                                               ChitNumber.Format,
                                               ChitNumber.Type,
                                               ChitNumber.Default,
					                           ChitNumber.IsHoldAndSendDisabled,
                                               ChitNumber.IsNonChargableChit,
                                               ChitNumber.IsDisplayTablesEnabled,
                                               ChitNumber.IsCaptureCustomerNameAndPhone,
                                               ChitNumber.IsAutoSaveOnTab,
                                               ChitNumber.IsCaptureCustomerDetails,
                                               ChitNumber.PromptForPickUpDeliveryTime,
                                               ChitNumber.AddMinutes,
                                               ChitNumber.SaveCustomerName,
                                               ChitNumber.SaveMemberName,
                                               ChitNumber.SaveMemberAddress,
                                               ChitNumber.OnlinePickupOrder,
                                               ChitNumber.OnlineDeliveryOrder);

    ChitNumber.ChitNumberKey = key;
    ChitNumbers[key] = ChitNumber;
    UpdateRemovedTaxList(ChitNumber);
    UpdateDiscountLists(ChitNumber);
    listed_chit_present_ = ::listed_chit_present(ChitNumbers);

}

void TManagerChitNumber::Update(
  Database::TDBTransaction &,
  TChitNumber cn,
  const chit_option_name_set_t &options_to_be_added,
  const chit_option_name_set_t &options_to_be_deleted,
  const chit_option_rename_map_t &options_to_be_renamed)
{
    db_chit_t::update_chit(cn.ChitNumberKey,
                           cn.DefaultMenu().c_str(),
                           cn.Mask.c_str(),
                           cn.Name.c_str(),
                           options_to_be_added,
                           options_to_be_deleted,
                           options_to_be_renamed,
                           cn.Format,
                           cn.Type,
                           cn.Default,
			               cn.IsHoldAndSendDisabled,
                           cn.IsNonChargableChit,
                           cn.IsDisplayTablesEnabled,
                           cn.IsCaptureCustomerNameAndPhone,
                           cn.IsAutoSaveOnTab,
                           cn.IsCaptureCustomerDetails,
                           cn.PromptForPickUpDeliveryTime,
                           cn.AddMinutes,
                           cn.SaveCustomerName,
                           cn.SaveMemberName,
                           cn.SaveMemberAddress,
                           cn.OnlinePickupOrder,
                           cn.OnlineDeliveryOrder);

    ChitNumbers[cn.ChitNumberKey] = cn;

    UpdateRemovedTaxList(cn);
    UpdateDiscountLists(cn);
    listed_chit_present_ = ::listed_chit_present(ChitNumbers);
}

void TManagerChitNumber::ClearDefault(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" CHITNUMBER"
	" SET"
	" IS_DEFAULT = :IS_DEFAULT";
	IBInternalQuery->ParamByName("IS_DEFAULT")->AsString = 'F';
	IBInternalQuery->ExecQuery();

   for ( First(false) ; !Eof() ; Next(false))
   {
      ChitNumber().Default = false;
   }
}


void TManagerChitNumber::LoadAll(Database::TDBTransaction &DBTransaction)
{
    ChitNumbers.clear();
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * From CHITNUMBER chitnumber";
	IBInternalQuery->ExecQuery();

	while(!IBInternalQuery->Eof)
	{
            TChitNumber ChitNumber(
                           IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger,
                           IBInternalQuery->FieldByName("NAME")->AsString,
                           IBInternalQuery->FieldByName("MASK")->AsString,
                           (EChitPromptType)(IBInternalQuery->FieldByName("CHITTYPE")->AsInteger),
                           (EChitFormatType)(IBInternalQuery->FieldByName("FORMAT")->AsInteger),
                           IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger,
                           false, // Dafult chit no longer relivant for all chits.
                           (EChitTriggerType)(IBInternalQuery->FieldByName("SELECTION_TRIGGER")->AsInteger),
                           IBInternalQuery->FieldByName("SELECTION_NAME")->AsString,
			   IBInternalQuery->FieldByName("DEFAULT_MENU")->AsString,
			   IBInternalQuery->FieldByName("DISABLE_HOLD_AND_SEND")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("IS_NONCHARGABLECHIT")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_DISPLAY_TABLES")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_NAMEANDPHONENUMBER")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_AUTOSAVETAB")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_CUSTOMERDETAILS")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("PROMPT_FOR_PICKUP_DELIVERY_TIME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ADDMINUTES")->AsInteger,     //
                           IBInternalQuery->FieldByName("SAVE_CUSTOMER_NAME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("SAVE_MEMBER_NAME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("SAVE_MEMBER_ADDRESS")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ONLINE_PICKUP_ORDER")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ONLINE_DELIVERY_ORDER")->AsString == "T" ? true : false
                           );
      ChitNumbers[ChitNumber.ChitNumberKey] = ChitNumber;
      LoadRemovedTaxesList(DBTransaction, ChitNumber);
      LoadAppliedDiscountLists(DBTransaction, ChitNumber);
	  IBInternalQuery->Next();
	}

    listed_chit_present_ = ::listed_chit_present(ChitNumbers);
}

void TManagerChitNumber::Load(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    ChitNumbers.clear();
	IBInternalQuery->SQL->Text = "Select * From DEVICECHITS"
    							 " LEFT join CHITNUMBER on DEVICECHITS.CHITNUMBER_KEY = CHITNUMBER.CHITNUMBER_KEY"
                                 " WHERE DEVICECHITS.DEVICE_KEY = :DEVICE_KEY ";
    IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger      = TerminalKey;
	IBInternalQuery->ExecQuery();

	while(!IBInternalQuery->Eof)
	{
      TChitNumber ChitNumber(
                           IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger,
                           IBInternalQuery->FieldByName("NAME")->AsString,
                           IBInternalQuery->FieldByName("MASK")->AsString,
                           (EChitPromptType)(IBInternalQuery->FieldByName("CHITTYPE")->AsInteger),
                           (EChitFormatType)(IBInternalQuery->FieldByName("FORMAT")->AsInteger),
                           IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger,
                           IBInternalQuery->FieldByName("DEFAULT_CHIT")->AsInteger == 1,
                           (EChitTriggerType)(IBInternalQuery->FieldByName("SELECTION_TRIGGER")->AsInteger),
                           IBInternalQuery->FieldByName("SELECTION_NAME")->AsString,
			   IBInternalQuery->FieldByName("DEFAULT_MENU")->AsString,
			   IBInternalQuery->FieldByName("DISABLE_HOLD_AND_SEND")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("IS_NONCHARGABLECHIT")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_DISPLAY_TABLES")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_NAMEANDPHONENUMBER")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_AUTOSAVETAB")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ENABLE_CUSTOMERDETAILS")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("PROMPT_FOR_PICKUP_DELIVERY_TIME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ADDMINUTES")->AsInteger,     //
                           IBInternalQuery->FieldByName("SAVE_CUSTOMER_NAME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("SAVE_MEMBER_NAME")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("SAVE_MEMBER_ADDRESS")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ONLINE_PICKUP_ORDER")->AsString == "T" ? true : false,
                           IBInternalQuery->FieldByName("ONLINE_DELIVERY_ORDER")->AsString == "T" ? true : false
                           );
      ChitNumbers[ChitNumber.ChitNumberKey] = ChitNumber;
      LoadRemovedTaxesList(DBTransaction, ChitNumber);
      LoadAppliedDiscountLists(DBTransaction, ChitNumber);
	  IBInternalQuery->Next();
	}

    listed_chit_present_ = ::listed_chit_present(ChitNumbers);
}

TChitNumber TManagerChitNumber::LoadFromKey(Database::TDBTransaction &DBTransaction, int ChitKey)
{
    TChitNumber Chit;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * From CHITNUMBER"
                                 " WHERE CHITNUMBER.CHITNUMBER_KEY = :CHITNUMBER_KEY";
    IBInternalQuery->ParamByName("CHITNUMBER_KEY")->AsInteger      = ChitKey;
	IBInternalQuery->ExecQuery();

	if(!IBInternalQuery->Eof)
	{
          TChitNumber ChitNumber(
                               IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger,
                               IBInternalQuery->FieldByName("NAME")->AsString,
                               IBInternalQuery->FieldByName("MASK")->AsString,
                               (EChitPromptType)(IBInternalQuery->FieldByName("CHITTYPE")->AsInteger),
                               (EChitFormatType)(IBInternalQuery->FieldByName("FORMAT")->AsInteger),
                               IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger,
                               IBInternalQuery->FieldByName("IS_DEFAULT")->AsString == "T",
                               (EChitTriggerType)(IBInternalQuery->FieldByName("SELECTION_TRIGGER")->AsInteger),
                               IBInternalQuery->FieldByName("SELECTION_NAME")->AsString,
                               IBInternalQuery->FieldByName("DEFAULT_MENU")->AsString,
                               IBInternalQuery->FieldByName("DISABLE_HOLD_AND_SEND")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("IS_NONCHARGABLECHIT")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ENABLE_DISPLAY_TABLES")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ENABLE_NAMEANDPHONENUMBER")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ENABLE_AUTOSAVETAB")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ENABLE_CUSTOMERDETAILS")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("PROMPT_FOR_PICKUP_DELIVERY_TIME")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ADDMINUTES")->AsInteger,     //
                               IBInternalQuery->FieldByName("SAVE_CUSTOMER_NAME")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("SAVE_MEMBER_NAME")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("SAVE_MEMBER_ADDRESS")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ONLINE_PICKUP_ORDER")->AsString == "T" ? true : false,
                               IBInternalQuery->FieldByName("ONLINE_DELIVERY_ORDER")->AsString == "T" ? true : false
                               );
          Chit = ChitNumber;
	}

    return Chit;
}

void TManagerChitNumber::Remove(Database::TDBTransaction &, int ItemKey)
{
    db_chit_t::remove_chit(ItemKey);

    ChitNumbers.erase(ItemKey);

    listed_chit_present_ = ::listed_chit_present(ChitNumbers);

}

void TManagerChitNumber::First(bool EnabledCodesOnly)
{
    ptrChitNumbers = ChitNumbers.begin();

    if (EnabledCodesOnly && ptrChitNumbers->second.Type == ectOff) {
        Next();
        return;
    }

   CurrentKey = ptrChitNumbers->first;
}

void TManagerChitNumber::Next(bool EnabledCodesOnly)
{
   advance(ptrChitNumbers,1);
   if(ptrChitNumbers != ChitNumbers.end())
   {
      if(EnabledCodesOnly)
      {
         do
         {
            if(ptrChitNumbers->second.Type == ectOff)
            {
               advance(ptrChitNumbers,1);
            }
         }
         while(ptrChitNumbers != ChitNumbers.end() && ptrChitNumbers->second.Type == ectOff);
         if(ptrChitNumbers != ChitNumbers.end())
         {
            CurrentKey = ptrChitNumbers->first;
         }
      }
      else
      {
         CurrentKey = ptrChitNumbers->first;
      }
   }
}

bool TManagerChitNumber::Eof()
{
	return  (ptrChitNumbers == ChitNumbers.end());
}

bool TManagerChitNumber::Bof()
{
	return  ptrChitNumbers == ChitNumbers.begin();
}

TChitNumber &TManagerChitNumber::ChitNumber()
{
   return ChitNumbers[CurrentKey];
}

TChitNumber &TManagerChitNumber::ChitNumberByKey(int ChitNumberKey)
{
	return ChitNumbers[ChitNumberKey];
}

bool TManagerChitNumber::Empty()
{
   return ChitNumbers.empty();
}

int TManagerChitNumber::Size()
{
   return ChitNumbers.size();
}

TChitNumber TManagerChitNumber::GetDefault()
{
   TChitNumber RetVal;
   for (First(true);!Eof();Next(true))
   {
      TChitNumber CurrentChitNumber = ChitNumber();
      if(CurrentChitNumber.Default)
      {
         RetVal = ChitNumber();
      }
   }
   return RetVal;
}

bool TManagerChitNumber::GetDefaultEnabled()
{
   bool RetVal = false;
   for (First(false);!Eof();Next(false))
   {
      TChitNumber CurrentChitNumber = ChitNumber();
      if(CurrentChitNumber.Default)
      {
         RetVal = CurrentChitNumber.Type != ectOff;
      }
   }
   return RetVal;
}

void TManagerChitNumber::GetNextChitNumber(Database::TDBTransaction &DBTransaction,TChitNumber &Chit)
{
   int key = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CHITNUMBER, 1) FROM RDB$DATABASE";
   IBInternalQuery->ExecQuery();
   key = IBInternalQuery->Fields[0]->AsInteger;
   int maxChitNumber =  GetMaxChitNumber();
   if(key > (maxChitNumber - 1))
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =   "SET GENERATOR \"GEN_CHITNUMBER\" TO 0;";
      IBInternalQuery->ExecQuery();
   }
   Chit.ChitNumber = IntToStr(key);
}

void TManagerChitNumber::ResetChitNumber(Database::TDBTransaction &DBTransaction)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =   "SET GENERATOR \"GEN_CHITNUMBER\" TO 0 ;";
   IBInternalQuery->ExecQuery();
}

const bool
TManagerChitNumber::ListedChitsArePresent()
const
{
    return ::listed_chit_present(ChitNumbers);
}

void TManagerChitNumber::ReLoadChitStatistics(Database::TDBTransaction &DBTransaction)
{
	DBTransaction.StartTransaction();
    ChitStatistics.clear();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "select ac.CHITNUMBER_KEY,ac.CHIT_VALUE,count(ac.ACTIVECHITNUMBER_KEY) HeldOrderCount, max(hov.TIME_STAMP) LastOrderTime "
								 "from ACTIVECHITNUMBER ac "
								 "inner join HELDORDERSVIEW hov on hov.ACTIVECHITNUMBER_KEY = ac.ACTIVECHITNUMBER_KEY "
								 "where TERMINAL_NAME = :terminal_name "
								 "group by ac.CHITNUMBER_KEY,ac.CHIT_VALUE "
								 "order by LastOrderTime desc";
	IBInternalQuery->ParamByName("terminal_name")->AsString  = TDeviceRealTerminal::Instance().ID.Name;
	IBInternalQuery->ExecQuery();
	UnicodeString key = "";
	UnicodeString firstKey = "";
	while(!IBInternalQuery->Eof)
	{
		TChitStats stat;
		stat.HeldOrderCount = IBInternalQuery->FieldByName("HeldOrderCount")->AsInteger;
		key =  IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsString + IBInternalQuery->FieldByName("CHIT_VALUE")->AsString;
		ChitStatistics[key] = stat;

		if(firstKey == "")
			firstKey = key;

		IBInternalQuery->Next();
	}

	DBTransaction.Commit();

	// set the color to green on the first item.. which would be the latest used one
	if(firstKey != "")
	{
		ChitStatistics[firstKey].IsLatest = true;
	}
}

UnicodeString TManagerChitNumber::GetColorForChitOption(UnicodeString chitOption)
{
	UnicodeString key = UnicodeString(LastAccesedChitKey) + chitOption;
	if(ChitStatistics.find(key) != ChitStatistics.end())
	{
		if(ChitStatistics[key].HeldOrderCount > 0 && ChitStatistics[key].IsLatest)
			return "Green";
		else if(ChitStatistics[key].HeldOrderCount > 0)
			return "Red";
		else
			return "Gray";
	}

	return "Gray";
}


void TManagerChitNumber::UpdateRemovedTaxList(TChitNumber &ChitNumber)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    DeleteRemovedTaxes(DBTransaction, ChitNumber.ChitNumberKey);
    DBTransaction.Commit();

    DBTransaction.StartTransaction();
    for(int i = 0; i < ChitNumber.RemovedTaxList->Count; i++)
    {
       AddRemovedTaxes(DBTransaction, ChitNumber.ChitNumberKey,(int)ChitNumber.RemovedTaxList->Objects[i]);
    }
    for(int k = 0; k < ChitNumber.RemovedServiceChargeList->Count; k++)
    {
       AddRemovedTaxes(DBTransaction, ChitNumber.ChitNumberKey,(int)ChitNumber.RemovedServiceChargeList->Objects[k]);
    }
    DBTransaction.Commit();
}

 void TManagerChitNumber::AddRemovedTaxes(Database::TDBTransaction &DBTransaction, int chit_key, int tax_profilekey)
 {
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "INSERT INTO CHITTAXRELATION ("
                               "CHITNUMBER_KEY, "
                               "TAX_PROFILE_KEY)"
                               "Values ( "
                               ":CHITNUMBER_KEY, "
                               ":TAX_PROFILE_KEY )";

    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ParamByName("TAX_PROFILE_KEY")->AsInteger = tax_profilekey;
    IBInsertQuery->ExecQuery();
 }

void TManagerChitNumber::DeleteRemovedTaxes(Database::TDBTransaction &DBTransaction, int chit_key)
{
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "Delete from  CHITTAXRELATION "
                               "where CHITNUMBER_KEY = :CHITNUMBER_KEY ";
    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ExecQuery();
}


void  TManagerChitNumber::LoadRemovedTaxesList(Database::TDBTransaction &DBTransaction,TChitNumber &ChitNumber)
{
   GetRemovedTaxesList(DBTransaction, ChitNumber.ChitNumberKey, ChitNumber.RemovedTaxList, 0);
   GetRemovedTaxesList(DBTransaction, ChitNumber.ChitNumberKey, ChitNumber.RemovedTaxList, 4);
   GetRemovedTaxesList(DBTransaction, ChitNumber.ChitNumberKey, ChitNumber.RemovedServiceChargeList, 2);
}

void TManagerChitNumber::GetRemovedTaxesList(Database::TDBTransaction &DBTransaction, int chit_key, TStringList* removelist, int type)
{
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "SELECT a.CHITNUMBER_KEY, a.TAX_PROFILE_KEY, b.Name, b.Type FROM CHITTAXRELATION a inner join "
                               "TAXPROFILES b on a.TAX_PROFILE_KEY = b.PROFILE_KEY where a.CHITNUMBER_KEY = :CHITNUMBER_KEY and b.Type = :Type ";

    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ParamByName("Type")->AsInteger = type;
    IBInsertQuery->ExecQuery();

    for(; !IBInsertQuery->Eof; IBInsertQuery->Next())
    {
      UnicodeString taxname = IBInsertQuery->FieldByName("Name")->AsString;
      int key = IBInsertQuery->FieldByName("TAX_PROFILE_KEY")->AsInteger;
      removelist->AddObject(taxname, (TObject *)key);
    }
}
void TManagerChitNumber::UpdateDiscountLists(TChitNumber &ChitNumber)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    DeleteAppliedDiscounts(DBTransaction, ChitNumber.ChitNumberKey);
    DBTransaction.Commit();

    DBTransaction.StartTransaction();
    for(int i = 0; i < ChitNumber.ApplyDiscountsList->Count; i++)
    {
       AddAppliedDiscounts(DBTransaction, ChitNumber.ChitNumberKey,(int)ChitNumber.ApplyDiscountsList->Objects[i]);
    }
    DBTransaction.Commit();
}

void TManagerChitNumber::AddAppliedDiscounts(Database::TDBTransaction &DBTransaction, int chit_key, int discount_key)
{
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "INSERT INTO CHITDISCOUNTRELATION ("
                               "CHITNUMBER_KEY, "
                               "DISCOUNT_KEY)"
                               "Values ( "
                               ":CHITNUMBER_KEY, "
                               ":DISCOUNT_KEY )";

    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ParamByName("DISCOUNT_KEY")->AsInteger = discount_key;
    IBInsertQuery->ExecQuery();
 }

void TManagerChitNumber::DeleteAppliedDiscounts(Database::TDBTransaction &DBTransaction, int chit_key)
{
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "Delete from  CHITDISCOUNTRELATION "
                               "where CHITNUMBER_KEY = :CHITNUMBER_KEY ";
    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ExecQuery();
}

void  TManagerChitNumber::LoadAppliedDiscountLists(Database::TDBTransaction &DBTransaction,TChitNumber &ChitNumber)
{
   GetAppliedDiscountLists(DBTransaction, ChitNumber.ChitNumberKey, ChitNumber.ApplyDiscountsList);
}

void TManagerChitNumber::GetAppliedDiscountLists(Database::TDBTransaction &DBTransaction, int chit_key, TStringList* discountlist)
{
    TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInsertQuery->Close();
    IBInsertQuery->SQL->Text = "SELECT a.CHITNUMBER_KEY, a.DISCOUNT_KEY, b.Name FROM CHITDISCOUNTRELATION a inner join "
                               "DISCOUNTS b on a.DISCOUNT_KEY = b.DISCOUNT_KEY where a.CHITNUMBER_KEY = :CHITNUMBER_KEY ";
    IBInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chit_key;
    IBInsertQuery->ExecQuery();

    for(; !IBInsertQuery->Eof; IBInsertQuery->Next())
    {
      UnicodeString discount = IBInsertQuery->FieldByName("Name")->AsString;
      int key = IBInsertQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
      discountlist->AddObject(discount, (TObject *)key);
    }
}

bool TManagerChitNumber::CheckIsOnLinePickupOrDeliveryChecked(Database::TDBTransaction &DBTransaction, AnsiString caption, TStringList *getList)
{
    bool retVal = false;

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    if(caption == "ONLINE_PICKUP_ORDER")
    {
       IBInternalQuery->SQL->Text = "SELECT a.ONLINE_PICKUP_ORDER, a.CHITNUMBER_KEY, a.NAME   "
                               "FROM CHITNUMBER a where a.ONLINE_PICKUP_ORDER = 'T' ";
    }
    else
    {
       IBInternalQuery->SQL->Text = "SELECT a.ONLINE_DELIVERY_ORDER, a.CHITNUMBER_KEY, a.NAME  "
                               "FROM CHITNUMBER a where a.ONLINE_DELIVERY_ORDER = 'T' ";
    }
    IBInternalQuery->ExecQuery();

    if(IBInternalQuery->RecordCount)
    {
       UnicodeString chitname = IBInternalQuery->FieldByName("Name")->AsString;
       int key = IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger;
       getList->AddObject(chitname, (TObject *)key);
    }
    return retVal;
}
//-------------------------------------------------------------------------------------------------------------------------------
int TManagerChitNumber::GetMaxChitNumber()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT a.INTEGER_VAL FROM VARSPROFILE a where a.VARIABLES_KEY = :VARIABLES_KEY ";
    IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger  = 9048;
    IBInternalQuery->ExecQuery();
    int maxChitNumber =  IBInternalQuery->Fields[0]->AsInteger;
    DBTransaction.Commit();
    return maxChitNumber;
}
