//---------------------------------------------------------------------------


#pragma hdrstop

#include "VariableManager.h"
#include <Graphics.hpp>
#include <Memory>
#include "CPortCtl.hpp"
#include "CPort.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerVariable *VM;

//------------------------------------------------------------------------------

AnsiString TManagerVariable::GetStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Default)
{
	if( !fEnabled )return Default;
	AnsiString RetVal = Default;

	if(!GetDeviceStr(Transaction,VariablesKey,RetVal))
	{
		if(LocationProfileKey != 0)
		{
			if(!GetProfileStr(Transaction,LocationProfileKey,VariablesKey,RetVal))
			{
				GetVarStr(Transaction,VariablesKey,RetVal);
			}
		}
		else
		{
			GetVarStr(Transaction,VariablesKey,RetVal);
		}
	}
	return RetVal;
}

bool TManagerVariable::Delete(Database::TDBTransaction &Transaction,vmVariables VariablesKey)
{
	bool RetVal = false;
	if( !fEnabled )return RetVal;
	if(DeviceProfileKey != 0)
	{
		RetVal = DeleteProfileVariable(Transaction,DeviceProfileKey,VariablesKey);
	}
	return RetVal;
}

bool TManagerVariable::GetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString &Value)
{
	bool RetVal = false;
   if(GetProfileStrCache(DeviceProfileKey,VariablesKey,Value))
	{
      RetVal = true;
   }
   else
   {
		RetVal = GetProfileStrDB(Transaction,DeviceProfileKey,VariablesKey,Value);
   }
   return RetVal;
}


void TManagerVariable::SetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Value)
{
	if( !fEnabled || DeviceProfileKey == 0) return;
	try
	{
		SetProfileStr(Transaction,DeviceProfileKey,VariablesKey,Value);
	}
	catch(Exception &E)
	{
		throw;
	}
}


bool TManagerVariable::GetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey, AnsiString &Value)
{
	if( !fEnabled )return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"VARCHAR_VAL "
	"FROM "
		"VARIABLES "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{
		Value = IBInternalQuery->FieldByName("VARCHAR_VAL")->AsString;
		RetVal = true;
	}
	return RetVal;
}

void TManagerVariable::SetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,AnsiString Value)
{
   if( !fEnabled )return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VARIABLES "
		"SET "
			"VARIABLE_NAME = :VARIABLE_NAME, "
			"VARIABLE_DESCRIPTION = :VARIABLE_DESCRIPTION, "
			"VARIABLE_GROUP = :VARIABLE_GROUP, "
			"VARCHAR_VAL = :VARCHAR_VAL "
		"WHERE "
			"VARIABLES_KEY = :VARIABLES_KEY";
		IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
		IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
		IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
		IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
		IBInternalQuery->ParamByName("VARCHAR_VAL")->AsString = Value;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected == 0)
		{
			Create = true;
		}

		if(Create)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO VARIABLES ("
				"VARIABLES_KEY,"
				"VARIABLE_NAME,"
				"VARIABLE_DESCRIPTION,"
				"VARIABLE_GROUP,"
				"VARCHAR_VAL) "
			"VALUES ("
				":VARIABLES_KEY,"
				":VARIABLE_NAME,"
				":VARIABLE_DESCRIPTION,"
				":VARIABLE_GROUP,"
				":VARCHAR_VAL);";
			IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
			IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
			IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
			IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
			IBInternalQuery->ParamByName("VARCHAR_VAL")->AsString = Value;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
}

int TManagerVariable::GetInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey, int Default)
{
	if( !fEnabled )
    return Default;
	int RetVal = Default;

	if(!GetDeviceInt(Transaction,VariablesKey,RetVal))
    {
      if(LocationProfileKey != 0)
      {
        if(!GetProfileInt(Transaction,LocationProfileKey,VariablesKey,RetVal))
        {
            GetVarInt(Transaction,VariablesKey,RetVal);
        }
      }
      else
      {
        GetVarInt(Transaction,VariablesKey,RetVal);
      }
	}
	return RetVal;
}

bool TManagerVariable::GetDeviceInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,int &Value)
{
    bool RetVal = false;
    if(GetProfileIntCache(DeviceProfileKey,VariablesKey,Value))
    {
        RetVal = true;
    }
    else
    {
        RetVal = GetProfileIntDB(Transaction,DeviceProfileKey,VariablesKey,Value);
    }
    Value = RetVal;
    return RetVal;
}

void TManagerVariable::SetDeviceInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,int Value)
{
	if( !fEnabled || DeviceProfileKey == 0) return;
	try
	{
		SetProfileInt(Transaction,DeviceProfileKey,VariablesKey,Value);
	}
	catch(Exception &E)
	{
		throw;
	}
}


bool TManagerVariable::GetVarInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey, int &Value)
{
   if( !fEnabled )return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
		"INTEGER_VAL "
	"FROM "
		"VARSPROFILE "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;
		RetVal = true;
	}
   return RetVal;
}

void TManagerVariable::SetVarInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,int Value)
{
	if( !fEnabled )return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VARIABLES "
		"SET "
			"VARIABLE_NAME = :VARIABLE_NAME, "
			"VARIABLE_DESCRIPTION = :VARIABLE_DESCRIPTION, "
			"VARIABLE_GROUP = :VARIABLE_GROUP, "
			"INTEGER_VAL = :INTEGER_VAL "
		"WHERE "
			"VARIABLES_KEY = :VARIABLES_KEY";
		IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
		IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
		IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
		IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
		IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = Value;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected == 0)
		{
			Create = true;
		}

		if(Create)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO VARIABLES ("
				"VARIABLES_KEY,"
				"VARIABLE_NAME,"
				"VARIABLE_DESCRIPTION,"
				"VARIABLE_GROUP,"
				"INTEGER_VAL) "     
			"VALUES ("
				":VARIABLES_KEY,"
				":VARIABLE_NAME,"
				":VARIABLE_DESCRIPTION,"
				":VARIABLE_GROUP,"
				":INTEGER_VAL);";
			IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
			IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
			IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
			IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
			IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = Value;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
}

double TManagerVariable::GetNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,double Default)
{
	if( !fEnabled )return Default;
   double RetVal = Default;

	if(!GetDeviceNum(Transaction,VariablesKey,RetVal))
   {
      if(LocationProfileKey != 0)
      {
			if(!GetProfileNum(Transaction,LocationProfileKey,VariablesKey,RetVal))
         {
				GetVarNum(Transaction,VariablesKey,RetVal);
			}
		}
		else
		{
			GetVarNum(Transaction,VariablesKey,RetVal);
		}
	}
	return RetVal;
}

bool TManagerVariable::GetDeviceNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,double &Value)
{
   bool RetVal = false;
   if(GetProfileNumCache(DeviceProfileKey,VariablesKey,Value))
   {
      RetVal = true;
	}
   else
	{
		RetVal = GetProfileNumDB(Transaction,DeviceProfileKey,VariablesKey,Value);
	}
	return RetVal;
}

void TManagerVariable::SetDeviceNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,double Value)
{
	if( !fEnabled || DeviceProfileKey == 0) return;
	try
	{
		SetProfileNum(Transaction,DeviceProfileKey,VariablesKey,Value);
	}
	catch(Exception &E)
	{
		throw;
	}
}

bool TManagerVariable::GetVarNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey, double &Value)
{
	if( !fEnabled )return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
		
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"NUMERIC_VAL "
	"FROM "
		"VARIABLES "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("NUMERIC_VAL")->AsFloat;
		RetVal = true;
	}
   return RetVal;
}

void TManagerVariable::SetVarNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,double Value)
{
   if( !fEnabled )return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VARIABLES "
		"SET "
			"VARIABLE_NAME = :VARIABLE_NAME, "
			"VARIABLE_DESCRIPTION = :VARIABLE_DESCRIPTION, "
			"VARIABLE_GROUP = :VARIABLE_GROUP, "
			"NUMERIC_VAL = :NUMERIC_VAL "
		"WHERE "
			"VARIABLES_KEY = :VARIABLES_KEY";
		IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
		IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
		IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
		IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
		IBInternalQuery->ParamByName("NUMERIC_VAL")->AsFloat = Value;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected == 0)
		{
			Create = true;
		}

		if(Create)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO VARIABLES ("
				"VARIABLES_KEY,"
				"VARIABLE_NAME,"
				"VARIABLE_DESCRIPTION,"
				"VARIABLE_GROUP,"
				"NUMERIC_VAL) "
			"VALUES ("
				":VARIABLES_KEY,"
				":VARIABLE_NAME,"
				":VARIABLE_DESCRIPTION,"
				":VARIABLE_GROUP,"
				":NUMERIC_VAL);";
			IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
			IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
			IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
			IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
			IBInternalQuery->ParamByName("NUMERIC_VAL")->AsFloat = Value;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
}


TDateTime TManagerVariable::GetTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime Default)
{
	if( !fEnabled )return Default;
   TDateTime RetVal = Default;

	if(!GetDeviceTime(Transaction,VariablesKey,RetVal))
   {
      if(LocationProfileKey != 0)
      {
			if(!GetProfileTime(Transaction,LocationProfileKey,VariablesKey,RetVal))
         {
				GetVarTime(Transaction,VariablesKey,RetVal);
         }
		}
		else
		{
			GetVarTime(Transaction,VariablesKey,RetVal);
		}
	}
   return RetVal;
}

bool TManagerVariable::GetDeviceTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime &Value)
{
   bool RetVal = false;
   if(GetProfileTimeCache(DeviceProfileKey,VariablesKey,Value))
   {
		RetVal = true;
   }
   else
   {
      RetVal = GetProfileTimeDB(Transaction,DeviceProfileKey,VariablesKey,Value);
   }
	return RetVal;
}


void TManagerVariable::SetDeviceTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime Value)
{
	if( !fEnabled || DeviceProfileKey == 0) return;
	try
	{
		SetProfileTime(Transaction,DeviceProfileKey,VariablesKey,Value);
	}
	catch(Exception &E)
	{
		throw;
	}
}


bool TManagerVariable::GetVarTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey, TDateTime &Value)
{
	if( !fEnabled )return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"TIMESTAMP_VAL "
	"FROM "
		"VARIABLES "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("TIMESTAMP_VAL")->AsDateTime;
		RetVal = true;
	}
   return RetVal;
}

void TManagerVariable::SetVarTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,TDateTime Value)
{
   if( !fEnabled )return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VARIABLES "
		"SET "
			"VARIABLE_NAME = :VARIABLE_NAME, "
			"VARIABLE_DESCRIPTION = :VARIABLE_DESCRIPTION, "
			"VARIABLE_GROUP = :VARIABLE_GROUP, "
			"TIMESTAMP_VAL = :TIMESTAMP_VAL "
		"WHERE "
			"VARIABLES_KEY = :VARIABLES_KEY";
		IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
		IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
		IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
		IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
		IBInternalQuery->ParamByName("TIMESTAMP_VAL")->AsDateTime = Value;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected == 0)
		{
			Create = true;
		}

		if(Create)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO VARIABLES ("
				"VARIABLES_KEY,"
				"VARIABLE_NAME,"
				"VARIABLE_DESCRIPTION,"
				"VARIABLE_GROUP,"
				"TIMESTAMP_VAL) "
			"VALUES ("
				":VARIABLES_KEY,"
				":VARIABLE_NAME,"
				":VARIABLE_DESCRIPTION,"
				":VARIABLE_GROUP,"
				":TIMESTAMP_VAL);";
			IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
			IBInternalQuery->ParamByName("VARIABLE_NAME")->AsString = Name;
			IBInternalQuery->ParamByName("VARIABLE_DESCRIPTION")->AsString = Discription;
			IBInternalQuery->ParamByName("VARIABLE_GROUP")->AsInteger = Group;
			IBInternalQuery->ParamByName("TIMESTAMP_VAL")->AsDateTime = Value;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		throw;
	}
}

//---------------------------------------------------------------------------
bool TManagerVariable::GetBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey, bool Default)
{
	return (bool)GetInt(Transaction,VariablesKey,(int)Default);
}

void TManagerVariable::SetDeviceBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool Variable)
{
	try
	{
		SetDeviceInt(Transaction,VariablesKey,(int)Variable);
	}
	catch(Exception &E)
	{
		throw;
	}
}

void TManagerVariable::Initialise(Database::TDBTransaction &DBTransaction)
{
	try
	{
		SetVarStr(DBTransaction,vmAutoMateIP, "AutoMate IP Address",
					 "This should be set to the IP address of the machine running AutoMate.\r Normally in the form 192.168.0.XXX",
					 vmgNetwork, "");

	/*	SetVarStr(vmChefMateIP, "ChefMate IP Address",
					 "This should be set to the IP address of the machine running ChefMate.\r Normally in the form 192.168.0.XXX",
					 vmgNetwork, "");*/

		SetVarStr(DBTransaction,vmStockInterbaseIP, "Stock Interbase IP Address",
					 "This should be set to the IP address of the interbase server machine holding the Stock database.\r Normally in the form 192.168.0.XXX",
					 vmgNetwork, "");

		SetVarStr(DBTransaction,vmStockDatabasePath, "Stock Database Path",
					 "This should be set to the path of the Stock.fdb file as it resides on the server.\r"
					 "This is not a network mapped path. It is the Path as the server machine sees the Stock.fdb file.\r"
					 " Normally it is C:\\Program Files\\MenuMate\\Stock.fdb",
					 vmgNetwork, "C:\\Program Files\\MenuMate\\Stock.fdb");

		SetVarStr(DBTransaction,vmSntpIP, "Time Server IP Address",
					 "This should be set to the IP address or computer name of your Network Time server.\r"
					 "The till will use this setting to syncronise its clock with the computer at this IP address.\r",
					 vmgNetwork, "");

		SetVarStr(DBTransaction,vmGuestMasterPath, "Guest Master Path",
					 "This is the path where MenuMate expects to pick up and place it's GuestMaster Interface Files.",
					 vmg3rdPartyInterface, "");

		SetVarStr(DBTransaction,vmSeatLabel, "Seat Label",
					 "This is the label that appears on recetips and kithen docets that seperates seats or guests.\r"
					 "The default is 'Guest'\r",
					 vmgPrinting, "Guest");

		SetVarBool(DBTransaction,vmChitNumbers, "Use Chit Numbers",
					 "Does this till display chit numbers on receipts and kithen dockets.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarStr(DBTransaction,vmReceiptPrinter, "Receipt Printer",
					 "The name of your receipt printer.\r",
					 vmgPrinting, "None");

		SetVarStr(DBTransaction,vmChitLabel, "Auto Incrementing Chit Label",
					 "This is the text label that is placed before the Chit number when the system automatically generates a chit number for you.",
					 vmgPrinting, "Chit");

		SetVarStr(DBTransaction,vmCustomChitLabel, "User Defined Chit Label",
					 "This is the text label that is placed before the Chit number when a user selects a chit number manually.",
					 vmgPrinting, "Chit");

		SetVarBool(DBTransaction,vmDuplicateReceipts, "Duplicate All Receipts",
					 "Till will print two Receipts.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmDuplicateEftPosReceipt, "Duplicate EFTPOS Receipts",
					 "Till will print two EFTPOS Receipts.\r"
					 "Default is True",
					 vmgPrinting, true);

		SetVarBool(DBTransaction,vmPrintNoteWithDiscount, "Print Notes with Discounts",
					 "Print the discount note on the receipt.\r"
					 "Default is true",
					 vmgPrinting, true);

		SetVarBool(DBTransaction,vmDuplicateSignatureReceipts, "Duplicate EFTPOS Signature Receipts",
					 "Till will print two EFTPOS Receipts when signatures are required.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarInt(DBTransaction,vmPointsPaymentGroupNumber, "Points Group Number",
					 "The Transaction group that points spent will appear under on the Zeds.\r"
					 "Default is 0",
					 vmgPrinting, 0);

		SetVarBool(DBTransaction,vmTipOnRecipt, "Display Tip:__ On Receipt",
					 "Display Tip:___ On the receipt leaving space for the patron to enter a tip figure.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmOptionsOnRecipt, "Display Options On Receipt",
					 "Display the options ordered with the meal on the receipt.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmSignatureOnRecipt, "Display Signature Space On Receipt",
					 "Leave a gap for the user to Sign on each Receipt.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmSumPricingChangesOnRecipt, "Sum Pricing Changes on Receipt",
					 "Add any item surcharges or discounts into the item price.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmSumDiscountsOnRecipt, "Sum Discounts on Receipt",
					 "Add sum item discounts into a total at the bottom of the receipt.\r"
					 "this option will also exclued the discount from being summed into the .\r"
					 "individual item pricing.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmSummariseDiscountOnZed, "Summarise Discounts on Zed",
					 "This option will print a summary of the days discounts on the Zed.\r"
					 "rather than every discount instance.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmDisplayChitNumberOnReceipt, "Display Chit Number on Receipt",
					 "Add the Chit Number to the Top of the receipt.\r"
					 "Does not work for Saved Sales.\r"				 
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmDisplayBarCodesOnReceipt, "Display Barcodes on Receipt",
					 "Add the invoice number to the bottom of the receipt as a bar code.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmAlwaysPrintReceiptTenderedSales, "Always Print a Receipt (Tendering)",
					 "The till will always print a receipt when tendering a sale.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmAlwaysPrintReceiptCashSales, "Always Print a Receipt (Cash Sales)",
					 "The till will always print a receipt on Cash Sales.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmConsolidateReceipt, "Consolidate Receipt",
					 "The Receipt will no longer be devided into seat by seat sections.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmAlphaNumChitNumbers, "Enable Alpha Numeric Chit Numbers",
					 "This will allow you to type text as a chit number.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmShowPointsOnZed, "Display Members Points on Zed",
					 "All Members Points earned and redeemed will display on the Zed for that Zed period.\r"
					 "Default is True",
					 vmgPrinting, true);

		SetVarBool(DBTransaction,vmShowInvoiceInfoOnZed, "Display Invoice sections on Zed",
					 "All invoice charges and payments will display on Zeds for that Zed period.\r"
					 "Default is True",
					 vmgPrinting, true);

		SetVarBool(DBTransaction,vmUnRavelKitchenPrinting, "No Ravelling on Kitchen Dockets",
					 "This will display kitchen dockets without raveling and in\r"
					 "the order that they are ordered in on the POS.\r"
					 "Default is False",
					 vmgPrinting, false);

		SetVarStr(DBTransaction,vmKitchenPerSeatMultiplerString, "Kitchen Docket Multipler",
					 "This is the text that appears when more than one seat\r"
					 "has ordered the same item. i.e 2 'on' 3.\r"
					 "Default is ' on '",
					 vmgPrinting, " on ");

		SetVarStr(DBTransaction,vmKitchenBulletSide, "Kitchen Docket Side Bullet",
					 "This is the text that appears to the left of all sides.\r"
					 "Default is ' > '",
					 vmgPrinting, " > ");

		SetVarStr(DBTransaction,vmKitchenHeaderSide, "Kitchen Docket Side Header",
					 "This is the text that appears before all sides.\r"
					 "Default is '  Sides: '",
					 vmgPrinting, "  Sides: ");

		SetVarStr(DBTransaction,vmKitchenBulletOpt, "Kitchen Docket Option Bullet",
					 "This is the text that appears to the left of all Options.\r"
					 "Default is '    '",
					 vmgPrinting, "    ");

		SetVarStr(DBTransaction,vmKitchenBulletSetMenuItem, "Kitchen Docket Set Menu Item Bullet",
					 "This is the text that appears to the left of all Set Menu Items.\r"
					 "Default is '    '",
					 vmgPrinting, "    ");

		SetVarStr(DBTransaction,vmKitchenHeaderOpt, "Kitchen Docket Options Header",
					 "This is the text that appears before all Options.\r"
					 "Default is '  Options: '",
					 vmgPrinting, "  Options: ");

		SetVarStr(DBTransaction,vmKitchenBulletNote, "Kitchen Docket Note Bullet",
					 "This is the text that appears to the left of all Notes.\r"
					 "Default is ''",
					 vmgPrinting, "");

		SetVarStr(DBTransaction,vmKitchenHeaderNote, "Kitchen Docket Note Header",
					 "This is the text that appears before all Notes.\r"
					 "Default is 'Note: '",
					 vmgPrinting, " Note: ");

		SetVarStr(DBTransaction,vmKitchenBulletMix, "Kitchen Docket Mix Bullet",
					 "This is the text that appears to the left of all Mixs.\r"
					 "Default is ''",
					 vmgPrinting, "");

		SetVarStr(DBTransaction,vmKitchenHeaderMix, "Kitchen Docket Mix Header",
					 "This is the text that appears before all Mixs.\r"
					 "Default is '  Mix: '",
					 vmgPrinting, "  Mix: ");

		SetVarBool(DBTransaction,vmSaveAndPrintPrintsPartialOrder, "Save and Print Prints Partial Orders",
					 "All invoice charges and payments will display on Zeds for that Zed period.\r"
					 "Default is True",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmHideFreeItemsOnReceipt, "Hide Free Items On Receipt.",
					 "Item with a Natural Price of $0.00 will not show on Receipts.\r"
					 "Excludes Items Price Adjusted to $0.00.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmHideZeroDollarsOnReceipt, "Hide $0.00 On Receipt.",
					 "The Free Item will show.\r"
					 "The $0.00 will not appear in the Cash Coloum on Receipts.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmHideCancelledItemsOnReceipts, "Hide Cancelled Items On Receipt.",
					 "Cancelled Items will not show on Receipts.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmHidePrevPayemntsOnReceipts, "Hide Payment History On Receipt.",
					 "Previous Payment History will not show on Receipts.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmHideMembershipDetialsOnReceipts, "Hide Membership Detials On Receipt.",
					 "Membership Detials will not show on Receipts.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarBool(DBTransaction,vmRavellItemsWithPriceAdjusts, "Ravel up Price Adjusted items on Receipts.",
					 "Ravel up (sum qty) items on Receipts even though one or more have had there price changed.\r"
					 "Default is false",
					 vmgPrinting, false);

		SetVarInt(DBTransaction,vmPrinterModel, "Printer Model Setting",
					 "Determins the default settings loaded from the Printers.dat file.\r"
					 "[Axiohm A715 Thermal]  = 0\r"
					 "[Axiohm A714 Impact] 	 = 1\r"
					 "[Epson TM-T88 Thermal] = 2\r"
					 "[Epson U210B Impact] 	 = 3\r"
					 "[PosiFlex] 				 = 4\r"
					 "[Samsung Thermal] 		 = 5\r",
					 vmgPrinters, 0);

		SetVarInt(DBTransaction,vmPrinterEmulationMode, "Printer Emulation Setting",
					 "Determins the command set used by the printer.\r"
					 "Text Only = 0\r"
					 "Epson (TM 88) = 1\r"
					 "Star = 2\r"
					 "Script = 3\r",
					 vmgPrinters, 0);

		SetVarInt(DBTransaction,vmNormalCPL, "Characters Per Line",
					 "Number of characters per line this printer supports.\r"
					 "Default is 42.\r",
					 vmgPrinters, 42);

		SetVarInt(DBTransaction,vmNormalBoldCPL, "Bold Characters Per Line",
					 "Number of characters per line this printer supports in bold font.\r"
					 "Default is 42.\r",
					 vmgPrinters, 42);

		SetVarInt(DBTransaction,vmCutterGap, "Cutter Gap (Bottom Margen)",
					 "The size of the gap between the bottom of the docket and where the paper is cut.\r"
					 "Default is 0.\r",
					 vmgPrinters, 0);

		SetVarInt(DBTransaction,vmTopMargin, "Receipts Top Margin",
					 "The size of the gap between the top of the docket and the printing.\r"
					 "Default is 0.\r",
					 vmgPrinters, 0);

		SetVarInt(DBTransaction,vmKitchenTopMargin, "Kitchen Docket Top Margin",
					 "The size of the gap between the top of the docket and the printing.\r"
					 "Default is 0.\r",
					 vmgPrinters, 0);

		SetVarBool(DBTransaction,vmAlwaysUseFullCut, "Cut Command",
					 "Always send the full cut command to the printer.\r"
					 "Some printers do not support a TM 88 partial cut command.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmKanjiPrinter, "Kanji Printer Support",
					 "Does your printer support Kanji character sets.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintTabReceipts, "Auto Print Tab On Save",
					 "Whenever you save to a Tab it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintStaffReceipts, "Auto Print Staff Tab On Save",
					 "Whenever you save to a Staff Tab it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintMemberReceipts, "Auto Print Member Tab On Save",
					 "Whenever you save to a Member Tab it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintTableReceipts, "Auto Print Guest Tab On Save",
					 "Whenever you save to a Guest Tab it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintCashReceipts, "Auto Print Cash sales On Save",
					 "Whenever you save to a Cash sales it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintRoomReceipts, "Auto Print Room Tab On Save",
					 "Whenever you save to a Room it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarBool(DBTransaction,vmAutoPrintInvoiceReceipts, "Auto Print Invoice Tab On Save",
					 "Whenever you save to an Invoice it will automatically print a\r"
					 "Prelim Receipt.\r"
					 "Default is false.\r",
					 vmgPrinters, false);

		SetVarStr(DBTransaction,vmSingleLineChar, "Single Line Character",
					 "The character to print when printing a single solid line in text mode."
					 "Default is '-'.\r",
					 vmgPrinters, "-");

		SetVarStr(DBTransaction,vmDoubleLineChar, "Double Line Character",
					 "The character to print when printing a double solid line in text mode."
					 "Default is '='.\r",
					 vmgPrinters, "=");

		SetVarBool(DBTransaction,vmGraphicPrinter, "Printer is a Graphics Printer",
					 "Windows Graphics Printer using its Windows Driver."
					 "Default is false (POS Printer).\r",
					 vmgPrinters, false);

		SetVarStr(DBTransaction,vmGraphicFontName, "Graphics Printer Font Name",
					 "Font used on the printer when printing text."
					 "Default is 'Terminal'.\r",
					 vmgPrinters, "Terminal");

		SetVarInt(DBTransaction,vmGraphicFontSize, "Graphics Printer Font Size",
					 "Font Size used on the printer when printing text."
					 "Default is 12.\r",
					 vmgPrinters,12);

		SetVarInt(DBTransaction,vmGraphicHeaderX, "Graphics Header X Offset",
					 "X Offset used for the receipt header."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicHeaderY, "Graphics Header Y Offset",
					 "Y Offset used for the receipt header."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicHeaderWidth, "Graphics Header Width",
					 "Width used to scale the receipt header."
					 "Default is 800.\r",
					 vmgPrinters,800);

		SetVarInt(DBTransaction,vmGraphicHeaderHeight, "Graphics Header Height",
					 "Height used to scale the receipt header."
					 "Default is 600.\r",
					 vmgPrinters,600);

		SetVarStr(DBTransaction,vmGraphicHeaderFile, "Graphics Header File",
					 "File name for the  Bitmap graphics header for receipts."
					 "Default is GraphicHeader.bmp.\r",
					 vmgPrinters,"GraphicHeader.bmp");

		SetVarInt(DBTransaction,vmGraphicReceiptX, "Graphic Receipt X Offset",
					 "X Offset used for the receipt."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicReceiptY, "Graphics Receipt Y Offset",
					 "Y Offset used for the receipt."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicReceiptMaxLines, "Graphics Receipt Max Lines",
					 "Number of Lines of Receipt printed before a new page is sent to the printer."
					 "Default is 30.\r",
					 vmgPrinters,30);


		SetVarInt(DBTransaction,vmGraphicBackGroundX, "Graphics Background X Offset",
					 "X Offset used for the Receipt Background."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicBackGroundY, "Graphics Background Y Offset",
					 "Y Offset used for the Receipt Background."
					 "Default is 0.\r",
					 vmgPrinters,0);

		SetVarInt(DBTransaction,vmGraphicBackGroundWidth, "Graphics Background Width",
					 "Width used to scale the receipt Background."
					 "Default is 800.\r",
					 vmgPrinters,800);

		SetVarInt(DBTransaction,vmGraphicBackGroundHeight, "Graphics Background Height",
					 "Height used to scale the receipt Background."
					 "Default is 600.\r",
					 vmgPrinters,600);

		SetVarInt(DBTransaction,vmGraphicPageCountX, "Graphics Page Count X Offset",
					 "Width used to set the Page count horizontal position.\r"
					 "A setting of -1 will place it to the right most side of the page."
					 "Default is -1.\r",
					 vmgPrinters,-1);

		SetVarInt(DBTransaction,vmGraphicPageCountY, "Graphics Page Count Y Offset",
					 "Hieght used to set the Page count Vertical position.\r"
					 "A setting of -1 will place it at the bottom of the page."
					 "Default is -1.\r",
					 vmgPrinters,-1);

		SetVarStr(DBTransaction,vmGraphicBackGroundFile, "Graphics Background File",
					 "File name for the Bitmap graphics Receipts Background."
					 "Default is None.\r",
					 vmgPrinters,"");


		SetVarStr(DBTransaction,vmPoleDisplayTopLine, "Top line that displays on Pole Display",
					 "This is the text that will appear on the top line of your pole display.",
					 vmgMiscEquipment, "");

		SetVarStr(DBTransaction,vmPoleDisplayBottomLine, "Bottom line that displays on Pole Display",
					 "This is the text that will appear on the bottom line of your pole display.",
					 vmgMiscEquipment, "");

		SetVarInt(DBTransaction,vmPoleDisplayPort, "The Pole Display Serial Port",
					 "The serial port number for the pole display.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmPoleDisplayEmulationMode, "Pole Display Emulation",
					 "Does this pole display support the Birch Pole display commands.\r"
					 " 0 = No (Javlin Terminals) 1 = Yes (Birch Pole Displays).\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmSerialKickPort, "The Cashdraws Serail Port",
					 "The serial port number for the cash drawer.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmSerialKickPortLength, "Serial Kick Port Data Length",
					 "The Number of characters sent to kick the cash drawer.\r"
					 "Default is 30",
					 vmgMiscEquipment, 30);

		SetVarBool(DBTransaction,vmSerialKickPortHighChars, "Use High characters to Open Cash drawer",
					 "Send 1's instead of 0's to trigger the cash drawer kick (some kicks prefer 1's).\r"
					 "Default is False",
					 vmgMiscEquipment, false);

		SetVarInt(DBTransaction,vmProximityPort, "The Proximity Reader Serial Port",
					 "The serial port number for the Proximity Reader.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmBarCodeReaderPort, "The Barcode Reader Serial Port",
					 "The serial port number for the Barcode Reader.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmSerialPort, "A Serial Port",
					 "The serial port number for a Profile.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarStr(DBTransaction,vmSecuritySerialProfileName, "The Security Serial Port Profile",
					 "The Security Serial Port Profile",
					 vmgMiscEquipment, "SecuritySerialPort");

		SetVarInt(DBTransaction,vmSerialBaudRate, "Serial Port Baud Rate",
					 "The serial port Baud Rate.\r"
					 "Default is 19200, (ENUM of an internal List)",
					 vmgMiscEquipment, br19200);

		SetVarInt(DBTransaction,vmSerialDataBits, "Serial Port Databits",
					 "The serial port Databits.\r"
					 "Default is 8, (ENUM of an internal List)",
					 vmgMiscEquipment, dbEight);

		SetVarInt(DBTransaction,vmSerialStopBits, "Serial Port Stop Bits",
					 "The serial port Stop Bits.\r"
					 "Default is 1, (ENUM of an internal List)",
					 vmgMiscEquipment, sbOneStopBit);

		SetVarInt(DBTransaction,vmSerialParityBits, "Serial Port Parity Bits",
					 "Default is None",
					 vmgMiscEquipment, prNone);

		SetVarInt(DBTransaction,vmSerialRTSFlowControl, "Serial Port RTS Control",
					 "Default is Handshaking",
					 vmgMiscEquipment, rtsHandshake);

		SetVarBool(DBTransaction,vmSerialOutCTSFlow, "Serial Port CTS Control",
					 "Default is True",
					 vmgMiscEquipment, true);

		SetVarBool(DBTransaction,vmSerialXonXoffOut, "Serial Port Xon Xoff Out Control",
					 "Default is false",
					 vmgMiscEquipment, false);

		SetVarBool(DBTransaction,vmSerialAsync, "Serial Port runs Asynchronously",
					 "Default is true",
					 vmgMiscEquipment, true);

		SetVarBool(DBTransaction,vmSerialXonXoffIn, "Serial Port Xon Xoff In Control",
					 "Default is false",
					 vmgMiscEquipment, false);

		SetVarInt(DBTransaction,vmBluetoothPort, "The BlueTooth Host Serial Port",
					 "The serial port number for the virtual BlueTooh Host serial poer.\r"
					 " 0 = Disabled 1 = Serial Port 1 (Comm 1) etc.. .\r"
					 "Default is 0",
					 vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmBluetoothSerialTimeOut, "The BlueTooth Serial Timeout",
					 "The inter-byte serial timeout in milliseconds (ms).\r"
					 "Default is 250",
					 vmgMiscEquipment, 250);

		SetVarStr(DBTransaction,vmMembershipDatabaseIP, "Membership Interbase IP Address",
					 "This should be set to the IP address of the interbase server machine holding the Membership database.\r"
					 "This setting is only use for 3rd party membership products.\r"
					 "Normally in the form 192.168.0.XXX",
					 vmgNetwork, "");

		SetVarStr(DBTransaction,vmMembershipDatabasePath, "Membership Database Path",
					 "This should be set to the path of the Membership.fdb file as it resides on the server.\r"
					 "This setting is only use for 3rd party membership products.\r"
					 " Normally it is C:\\Program Files\\MenuMate\\Membership.fdb",
					 vmgNetwork, "C:\\Program Files\\MenuMate\\Membership.fdb");

		SetVarBool(DBTransaction,vmUseERSMemberShip, "Use ERS Membership Interface",
					 "Set this to true if you which to interfaces to ERS's membership package.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosSyncro, "Use Syncro EFTPOS",
					 "Till uses Syncro Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosANZ, "Use ANZ EFTPOS",
					 "Till uses ANZ Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosIngenico, "Use Ingenico EftPos",
					 "Till uses Ingenico Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosCadmus, "Use Cadmus EftPos",
					 "Till uses Cadmus Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosDPS, "Use DPS EftPos",
					 "Till uses DPS Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosIceLink, "Use ICELink EftPos",
					 "Till uses ICELink Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmSmartCardMembership, "Use Smart Cards",
					 "Enabled Membership to use smart cards.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);

		SetVarInt(DBTransaction,vmEftposSerialPort, "Eftpos Serial Port",
					 "Serial Port Number for the Cadmus & Icelink Eftpos.",
					 vmg3rdPartyInterface, -1);

		SetVarInt(DBTransaction,vmEftposMerchantNumber, "Eftpos Merchant Number",
					 "Merchant Number for the Cadmus Eftpos.\r"
					 "Default is 1",
					 vmg3rdPartyInterface, 1);

		SetVarBool(DBTransaction,vmUseStockMaster, "Use ERS Stock Master",
						"Does the system interface with stock master?.\r",
					 vmg3rdPartyInterface, 0);

		SetVarStr(DBTransaction,vmStockMasterExportPath, "Stock Master Export Path",
					 "Directory in which to place the stock master files.",
					 vmg3rdPartyInterface, "C:\\Program Files\\MenuMate\\");

		SetVarStr(DBTransaction,vmPhoenixIPAddress, "Phoenix IP Address",
					 "IP Address of the Phoenix Server (i.e 192.168.1.250)",
					 vmg3rdPartyInterface, "");

		SetVarInt(DBTransaction,vmPhoenixTCPPort, "Phoenix IP Port",
					 "Port Number for the Phoenix Server TCPIP port  (i.e 4444)",
					 vmg3rdPartyInterface, 4444);

		SetVarInt(DBTransaction,vmPhoenixPOSID, "Phoenix POS ID",
					 "The Unique POSID for this terminal on the Phoenix Server",
					 vmg3rdPartyInterface, 0);

		SetVarStr(DBTransaction,vmPhoenixPaymentCategory, "Phoenix Default Payment Category",
					 "The Category Payment types will deduct against in the Phoenix system/r"
					 "Default is 9999",
					 vmg3rdPartyInterface, "9999");

		SetVarStr(DBTransaction,vmPhoenixItemCategory, "Phoenix Default Item Category",
					 "The Category Menu Items will add against in the Phoenix system/r"
					 "Default is 9998",
					 vmg3rdPartyInterface, "9998");

		SetVarStr(DBTransaction,vmPhoenixPointsCategory, "Phoenix Points Category",
					 "The Category Points Payments will deduct against in the Phoenix system/r"
					 "Default is 9997",
					 vmg3rdPartyInterface, "9997");

		SetVarStr(DBTransaction,vmPhoenixCreditCategory, "Phoenix Credit Category",
					 "The Category Credit Payments will deduct against in the Phoenix system/r"
					 "Default is 9996",
					 vmg3rdPartyInterface, "9996");

		SetVarStr(DBTransaction,vmPhoenixDefaultAccount, "Phoenix Default Account",
					 "The Account number against which all non room charges are made./r"
					 "Default is 90000",
					 vmg3rdPartyInterface, "90000");

		SetVarStr(DBTransaction,vmPhoenixDefaultSurchargeAccount, "Phoenix Surcharge Account",
					 "The Account number against which all payment surcharges are made./r"
					 "This includes 'Cheque fee' type charges but not custom surchages like tips."
					 "Default is 9995",
					 vmg3rdPartyInterface, "9995");

		SetVarStr(DBTransaction,vmPhoenixDefaultRoundingAccount, "Phoenix Rounding Code",
					 "The Account number against which all payment roundings are sotred./r"
					 "This is mostly used for cash type charges."
					 "Default is 9994",
					 vmg3rdPartyInterface, "9994");

		SetVarBool(DBTransaction,vmTabsEnabled, "Enable Tabs Interface",
					 "If set customer and staff tabs will be enabled.\r"
					 "Default is True",
					 vmgMenuMateModules, true);

		SetVarBool(DBTransaction,vmTablesEnabled, "Enable Tables Interface",
					 "If set tables will be enabled.\r"
					 "Default is True",
					 vmgMenuMateModules, true);

		SetVarInt(DBTransaction,vmChefMateColour, "The ChefMate Till Colour",
					 "This is the colour this till appears under ChefMate.\r"
					 "Default is Red",
					 vmgMenuMateModules, (int)clRed);


		SetVarBool(DBTransaction,vmTrackSaleAndMakeTimes, "Sale & Make Time Tracking",
					 "Enable Sale and Make Time Tracking.\r"
					 "You have to have purchased and register the sale and make"
					 " turnaround time traking module.\r"
					 "Default is True",
					 vmgMenuMateModules, true);

		SetVarBool(DBTransaction,vmAlwaysForceLogin, "Force Logins Every Sale",
					 "If set tables will froce the user to login after every sale.\r"
					 "Default is False",
					 vmgSecurity, false);

		SetVarBool(DBTransaction,vmDisableManualLogin, "Disables Manual Login",
					 "You will be unable to log in via a User and PIN.\r"
					 "You can log in with Swipe Card and Prox only.\r"
					 "Default is False",
					 vmgSecurity, false);

		SetVarBool(DBTransaction,vmAutoLogoutPOS, "Auto-logout of POS",
					 "Automatically log the user out of the Point of Sale when the Auto-logout timer expires due to inactivity.\r"
					 "Default is False",
					 vmgSecurity, false);

		SetVarInt(DBTransaction,vmAutoLogoutSeconds, "Auto-logout Timer",
					 "The length of time in seconds of inactivity before logging the user off.\r"
					 "Default is 15",
					 vmgSecurity, 15);

		SetVarBool(DBTransaction,vmForcedReasons, "Forced Reasons",
					 "The user must enter a reason for Credits, Cancels etc...\r"
					 "Default is False",
					 vmgSecurity, false);

		SetVarBool(DBTransaction,vmRevertToFirstCourse, "Revert To First Course",
					 "Jump back to the top course after every sale.\r"
					 "Default is True",
					 vmgPOS, true);

		SetVarBool(DBTransaction,vmForcedSidesFirst, "Forced Sides First",
					 "Display forced sides before you display forced options.\r"
					 "Default is False",
					 vmgPOS, false);

		SetVarInt(DBTransaction,vmNonServicePeriod, "Non Service Period",
					 "The length of time in minutes that is takes a table to turn fully red.\r"
					 "Tables will turn blue in the table overview when they have just purchased an item.\r"
					 "Default is 15",
					 vmgPOS, 15);

		SetVarBool(DBTransaction,vmForceTender, "Force Tender Use",
					 "All sales must be tendered and the value of the sale entered.\r"
					 "This option disables the quick cash sale button.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmFTBypassElecTranTyp, "No Force Tenders On Eftpos",
					 "This option disables the requirement for the user to"
					 " enter that transaction amount when using electronic payment types"
					 " and Force Tenders.\r"
					 "Default is true",
					 vmgPOS, true);


		SetVarBool(DBTransaction,vmPLACE_HOLDER_FOR_OLD_MemberNumericPannelFirst, "Member Show Numeric Pannel",
					 "This option will display the Member numeric pannel fist by default.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarInt(DBTransaction,vmMemberQuickButtonCount, "Member Quick Buttons",
					 "This option sets the number of quick buttons on the Member Selection Page.\r"
					 "Default is Four (4)",
					 vmgPOS, 4);


		SetVarBool(DBTransaction,vmStaffNumericPannelFirst, "Staff Show Numeric Pannel",
					 "This option will display the Staff numeric pannel fist by default.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmMemberIncrementalSearch, "Enable Incremental Member Search",
					 "The Till will search for the closest matching member on every key stroke.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmDisplayServingCourseColours, "Display Serving Course Colours",
					 "Will Display the Serving Course buttons and there \"Above Item\"\r"
					 "lables in their Serving Course colour selected in office.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmHideServingCourseLabels, "Hide Serving Course Labels",
					 "Will Hide the Serving Course Labels that appear above items\r"
					 "in the right hand display.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmRememberLastServingCourse, "Remember Last Serving Course",
					 "This setting determins whether the Serving Course Prompt is displayed\r"
					 "when a course has no default Serving Course, or if the last Serving Course selected is assumed.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarBool(DBTransaction,vmPromptForPatronCount, "Prompt User For Patron Count",
					 "Prompts the user to enter the patron count for Tendered sales.\r"
					 "Default is false",
					 vmgPOS, false);

		SetVarInt(DBTransaction,vmMemberNameOnPoleDisplay, "Membership pole display Info",
					 "Selects the Membership information that is displayed on the pole display.\r"
					 "0 = None, 1 = Alias, 2 = Full Name\r"
					 "Default is None.",
					 vmgPOS, 0);

		SetVarInt(DBTransaction,vmMemberLoyaltyRewardsRules, "Membership Purchase Count Rules",
					 "Determines what type of orders count toward the Members reward system\r"
					 "0 = None, 1 = All Sales, 2 = Cash and Tender Sales, 3 = Saved Sales(Tables,Tabs, Etc)\r"
					 "Default is None.",
					 vmgPOS, 0);

		SetVarInt(DBTransaction,vmLocationLoyaltyRewardsRules, "Location Purchase Count Rules",
					 "Determines what type of orders count toward the Locations reward system.\r"
					 "0 = None, 1 = All Sales, 2 = Cash and Tender Sales, 3 = Saved Sales(Tables,Tabs, Etc)\r"
					 "Default is None.",
					 vmgPOS, 0);


		SetVarBool(DBTransaction,vmPOSDefaultsToBeverages, "POS Defaults to Beverage Menu",
					 "POS Displays the Beverage Menu when first entered.\r"
					 "Defaults to false.\r",
					 vmgPOS, false);

		SetVarInt(DBTransaction,vmClearLogsDays, "Clear Logs every X days",
					 "How many days of logs the till should accumulate.\r"
					 "A setting of Zero disables the clearing of logs.\r"
					 "Default is 30",
					 vmgLogging, 30);

		SetVarInt(DBTransaction,vmMaxLogsRecords, "Maximum number of records in the logs.",
					 "How many logs the system should accumulate.\r"
					 "A setting of Zero disables the maximum number of logs.\r"
					 "Default is 100,000",
					 vmgLogging, 100000);

		SetVarBool(DBTransaction,vmHappyHourExempt, "No Happy Hour",
					 "This Till ignores all happy hours.\r"
					 "Default is False",
					 vmgHappyHour, false);

		SetVarTime(DBTransaction,vmHappyHourStart, "Happy Hour Start",
					 "Time at which happy hour starts.\r",
					 vmgHappyHour, 0);

		SetVarTime(DBTransaction,vmHappyHourFinish, "Happy Hour Finish",
					 "Time at which happy hour finishs.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHSunday, "Happy Hour Applies on Sunday",
					 "Does happy hour kick in on Sundays?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHMonday, "Happy Hour Applies on Monday",
					 "Does happy hour kick in on Mondays?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHTuesday, "Happy Hour Applies on Tuesday",
						"Does happy hour kick in on Tuesdays?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHWednesday, "Happy Hour Applies on Wednesday",
						"Does happy hour kick in on Wednesdays?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHThursday, "Happy Hour Applies on Thursday",
						"Does happy hour kick in on Thursdays?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHFridays, "Happy Hour Applies on Fridays",
						"Does happy hour kick in on Fridayss?.\r",
					 vmgHappyHour, 0);

		SetVarBool(DBTransaction,vmHHSaturday, "Happy Hour Applies on Saturday",
						"Does happy hour kick in on Saturdays?.\r",
					 vmgHappyHour, 0);

		SetVarInt(DBTransaction,vmLuckyMemberPeriod, "Lucky Member Period.",
					"The sales period (in hours) over which a lucky member draw is selected.\r"
					 "Default is 24 hours.",
					 vmgHappyHour, 24);

		SetVarBool(DBTransaction,vmHappyHourMembersOnly, "Happy Hour Members Only.",
					"Happy Hour Prices will only be applied for Members.\r"
					"Default is false.",
					 vmgHappyHour, false);

		SetVarBool(DBTransaction,vmEarnsPointsWhileRedeeming, "Points are earned while Redeeming.",
					  "Account earns points even while redeeming points at sale time.\r",
					  vmgMisc, false);

		SetVarBool(DBTransaction,vmOnlyEarnsPointsWhileRedeeming, "Points are only earned while Redeeming.",
					  "Account only earns points while redeeming points at sale time.\r",
					  vmgMisc, false);

		SetVarInt(DBTransaction,vmMinMemberNumber, "Minimum Member Number",
					 "This value is the minimum member number when a new member is created.\r"
					 "Default is 1000",
					 vmgSystem, 1000);

		SetVarBool(DBTransaction,vmRecycleMemberNumber, "Recycle Member Numbers.",
					   "Member numbers are recycled when Members are deleted.\r"
                  "Default is false",
					   vmgMisc, false);

		SetVarInt(DBTransaction,vmInfoSync, "Palm Info Sync",
					 "This value is the current saved sync number for palm information.\r"
					 "Default is 0",
					 vmgSystem, 0);

		SetVarStr(DBTransaction,vmVersionNumber, "Terminal Version Number",
						 "This is the version number for the terminal\r"
						 "It is set by the terminal when it starts\r"
						 "Default is 0\r",
						 vmgSystem, "0");

		SetVarStr(DBTransaction,vmbtnDollar1, "Quick Tender Amount 1",
						 "Must be in the format $X.XX or $X or $.XX\r"
						 "Default is $1\r",
						 vmgSystem, "$1");

		SetVarStr(DBTransaction,vmbtnDollar2, "Quick Tender Amount 2",
						 "Must be in the format $X.XX or $X or $.XX\r"
						 "Default is $5\r",
						 vmgSystem, "$5");

		SetVarStr(DBTransaction,vmbtnDollar3, "Quick Tender Amount 3",
						 "Must be in the format $X.XX or $X or $.XX\r"
						 "Default is $10\r",
						 vmgSystem, "$10");

		SetVarStr(DBTransaction,vmbtnDollar4, "Quick Tender Amount 4",
						 "Must be in the format $X.XX or $X or $.XX\r"
						 "Default is $20\r",
						 vmgSystem, "$20");

		SetVarStr(DBTransaction,vmbtnDollar5, "Quick Tender Amount 5",
						 "Must be in the format $X.XX or $X or $.XX\r"
						 "Default is $50\r",
						 vmgSystem, "$50");


		SetVarStr(DBTransaction,vmSmartCardReader, "Smart Card Reader Name",
						 "The Name of the Smart Card Reader\r"
						 "Default is ''\r",
						 vmgSystem, "");


		SetVarStr(DBTransaction,vmSmartCardCode, "Smart Card Verification Code",
						 "Smart Card 3 byte Verification Code\r"
						 "Default is 'A,A,A'\r",
						 vmgSystem, "AAA");

		SetVarBool(DBTransaction,vmMidPointRoundsDown, "Rounding Midpoint Rounds Down",
					 "Determines if rounding midpoint rounds up or down.\r"
            	 "Default is down\r",
					 vmgSystem, true);

		SetVarInt(DBTransaction,vmSiteID, "The Site ID",
					 "A Number that uniquely identifies this site.\r"
					 "Default is -1.",
					 vmgSystem, -1);

      SetVarStr(DBTransaction,vmStaffMessageOfTheDay, "Staff Message",
                   "Staff Message Of The Day\r"
                   "Default is 'Change this in [Maintenance] [Misc Settings] [Staff Options]'\r",
                   vmgSystem, "Change this in [Maintenance] [Misc Settings] [Staff Options]");


      int GlobalProfileKey = VM->GetProfile(DBTransaction,eSystemProfiles,"Globals");
      if(GlobalProfileKey == 0)
      {
         GlobalProfileKey = VM->SetProfile(DBTransaction,eSystemProfiles,"Globals");
         SetProfileBool(DBTransaction,GlobalProfileKey,vmMidPointRoundsDown, true);
         SetProfileInt(DBTransaction,GlobalProfileKey,vmSiteID, -1);
   		SetProfileStr(DBTransaction,GlobalProfileKey,vmStaffMessageOfTheDay, "Change this in [Maintenance] [Misc Settings] [Staff Options]");         
      }
	}
	catch(Exception &E)
	{
	}
}

TManagerVariable::TManagerVariable()
{
	DeviceProfileKey = 0;
	LocationProfileKey = 0;
	Enabled = true;	
}



//------------------------------------------------------------------------------
//	Profiles.
//------------------------------------------------------------------------------
int TManagerVariable::GetProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName)
{
   int RetVal = 0;
   if(!GetProfileCache(Type,ProfileName,RetVal))
   {
		RetVal = GetProfileDB(Transaction,Type,ProfileName);
   }
   return RetVal;
}

int TManagerVariable::GetProfileDB(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName)
{
	if( !fEnabled )return 0;
	int RetVal = 0;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
		"PROFILE_KEY "
	"FROM "
		"PROFILE "
	"WHERE "
		"NAME = :NAME AND "
      "PROFILE_TYPE = :PROFILE_TYPE";

	IBInternalQuery->ParamByName("NAME")->AsString = ProfileName;
	IBInternalQuery->ParamByName("PROFILE_TYPE")->AsInteger = Type;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		RetVal = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
		CacheGetProfile[Type][ProfileName] = RetVal;
	}
	return RetVal;
}

int TManagerVariable::SetProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName)
{
	if( !fEnabled )return 0;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	int RetVal = GetProfile(Transaction,Type,ProfileName);

   if(RetVal == 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PROFILE, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO PROFILE ("
         "PROFILE_KEY,"
         "PROFILE_TYPE,"
         "NAME) "
      "VALUES ("
         ":PROFILE_KEY,"
         ":PROFILE_TYPE,"
         ":NAME);";
      IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = RetVal;
      IBInternalQuery->ParamByName("PROFILE_TYPE")->AsInteger = Type;
	   IBInternalQuery->ParamByName("NAME")->AsString = ProfileName;
		IBInternalQuery->ExecQuery();
	}
   CacheGetProfile[Type][ProfileName] = RetVal;
	return RetVal;
}

bool TManagerVariable::SetProfile(Database::TDBTransaction &Transaction,int ProfileKey,ProfileType Type,AnsiString ProfileName)
{
   if( !fEnabled )return false;
	bool Found = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
	
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
      "PROFILE "
   "SET "
      "PROFILE_TYPE = :PROFILE_TYPE, "
      "NAME = :NAME "
   "WHERE "
      "PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ParamByName("PROFILE_TYPE")->AsInteger = Type;
   IBInternalQuery->ParamByName("NAME")->AsString = ProfileName;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RowsAffected != 0)
	{
		Found = true;
	}
   CacheGetProfile[Type][ProfileName] = ProfileKey;
	return Found;
}


bool TManagerVariable::DelProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName)
{
   if( !fEnabled )return false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	bool RetVal = false;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "DELETE FROM PROFILE WHERE NAME = :NAME AND PROFILE_TYPE = :PROFILE_TYPE";
   IBInternalQuery->ParamByName("NAME")->AsString = ProfileName;
   IBInternalQuery->ParamByName("PROFILE_TYPE")->AsInteger = Type;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RowsAffected > 0)
	{
		RetVal = true;
	}
   return RetVal;
}

bool TManagerVariable::DelProfile(Database::TDBTransaction &Transaction,int ProfileKey)
{
   if( !fEnabled || ProfileKey == 0) return false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

   bool RetVal = false;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "DELETE FROM PROFILE WHERE PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RowsAffected > 0)
	{
		RetVal = true;
	}
	return RetVal;
}

bool TManagerVariable::DeleteProfileVariable(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey)
{
   if( !fEnabled || ProfileKey == 0) return false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
			
   bool RetVal = false;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "DELETE FROM VARSPROFILE "
   							"WHERE VARIABLES_KEY = :VARIABLES_KEY "
                        "AND PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RowsAffected > 0)
	{
		RetVal = true;
	}
   return RetVal;
}


bool TManagerVariable::GetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,AnsiString &Value)
{
   bool RetVal = false;
   if(GetProfileStrCache(ProfileKey,VariablesKey,Value))
   {
      RetVal = true;
   }
   else
   {
      RetVal = GetProfileStrDB(Transaction,ProfileKey,VariablesKey,Value);
   }
   return RetVal;
}

bool TManagerVariable::GetProfileStrDB(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,AnsiString &Value)
{
   if( !fEnabled || ProfileKey == 0) return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"VARCHAR_VAL "
	"FROM "
		"VARSPROFILE "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY AND "
		"PROFILE_KEY = :PROFILE_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
	IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("VARCHAR_VAL")->AsString;
		CacheGetProfileStr[ProfileKey][VariablesKey] = Value;
		RetVal = true;
	}
   return RetVal;
}

void TManagerVariable::SetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,AnsiString Value)
{
	if( !fEnabled || ProfileKey == 0) return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
		
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
      "VARSPROFILE "
   "SET "
      "VARCHAR_VAL = :VARCHAR_VAL "
   "WHERE "
      "VARIABLES_KEY = :VARIABLES_KEY AND "
      "PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ParamByName("VARCHAR_VAL")->AsString = Value;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RowsAffected == 0)
   {
      Create = true;
   }

   if(Create)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VARSPROFILE, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
      int Key = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO VARSPROFILE ("
         "VARSPROFILE_KEY,"
         "VARIABLES_KEY,"
         "PROFILE_KEY,"
         "VARCHAR_VAL) "
      "VALUES ("
         ":VARSPROFILE_KEY,"
         ":VARIABLES_KEY,"
         ":PROFILE_KEY,"
         ":VARCHAR_VAL);";
      IBInternalQuery->ParamByName("VARSPROFILE_KEY")->AsInteger = Key;
      IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
	   IBInternalQuery->ParamByName("VARCHAR_VAL")->AsString = Value;
		IBInternalQuery->ExecQuery();
	}
   CacheGetProfileStr[ProfileKey][VariablesKey] = Value;
}

bool TManagerVariable::GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,int &Value)
{
	bool RetVal = false;
   if(GetProfileIntCache(ProfileKey,VariablesKey,Value))
   {
      RetVal = true;
   }
   else
   {
		RetVal = GetProfileIntDB(Transaction,ProfileKey,VariablesKey,Value);
   }
   return RetVal;
}

bool TManagerVariable::GetProfileIntDB(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,int &Value)
{
	if( !fEnabled || ProfileKey == 0) return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"INTEGER_VAL "
	"FROM "
		"VARSPROFILE "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY AND "
		"PROFILE_KEY = :PROFILE_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;
		CacheGetProfileInt[ProfileKey][VariablesKey] = Value;
		RetVal = true;
	}
	return RetVal;
}

void TManagerVariable::SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,int Value)
{
   if( !fEnabled || ProfileKey == 0) return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
      "VARSPROFILE "
   "SET "
      "INTEGER_VAL = :INTEGER_VAL "
   "WHERE "
      "VARIABLES_KEY = :VARIABLES_KEY AND "
      "PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = Value;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RowsAffected == 0)
   {
      Create = true;
   }

   if(Create)
	{
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VARSPROFILE, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
		int Key = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO VARSPROFILE ("
         "VARSPROFILE_KEY,"
         "VARIABLES_KEY,"
         "PROFILE_KEY,"
         "INTEGER_VAL) "
      "VALUES ("
         ":VARSPROFILE_KEY,"
         ":VARIABLES_KEY,"
         ":PROFILE_KEY,"
         ":INTEGER_VAL);";
      IBInternalQuery->ParamByName("VARSPROFILE_KEY")->AsInteger = Key;
      IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
	   IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = Value;
		IBInternalQuery->ExecQuery();
	}
   CacheGetProfileInt[ProfileKey][VariablesKey] = Value;
}

bool TManagerVariable::GetProfileNum(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,double &Value)
{
   bool RetVal = false;
   if(GetProfileNumCache(ProfileKey,VariablesKey,Value))
   {
      RetVal = true;
   }
   else
   {
		RetVal = GetProfileNumDB(Transaction,ProfileKey,VariablesKey,Value);
   }
   return RetVal;
}

bool TManagerVariable::GetProfileNumDB(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,double &Value)
{
   if( !fEnabled || ProfileKey == 0) return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
		"NUMERIC_VAL "
	"FROM "
		"VARSPROFILE "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY AND "
		"PROFILE_KEY = :PROFILE_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("NUMERIC_VAL")->AsFloat;
		CacheGetProfileNum[ProfileKey][VariablesKey] = Value;
		RetVal = true;
	}
   return RetVal;
}

void TManagerVariable::SetProfileNum(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,double Value)
{
   if( !fEnabled || ProfileKey == 0) return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
		
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
      "VARSPROFILE "
   "SET "
      "NUMERIC_VAL = :NUMERIC_VAL "
   "WHERE "
      "VARIABLES_KEY = :VARIABLES_KEY AND "
      "PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ParamByName("NUMERIC_VAL")->AsFloat = Value;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RowsAffected == 0)
   {
      Create = true;
   }

   if(Create)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VARSPROFILE, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
      int Key = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO VARSPROFILE ("
         "VARSPROFILE_KEY,"
         "VARIABLES_KEY,"
         "PROFILE_KEY,"
         "NUMERIC_VAL) "
      "VALUES ("
         ":VARSPROFILE_KEY,"
         ":VARIABLES_KEY,"
         ":PROFILE_KEY,"
         ":NUMERIC_VAL);";
      IBInternalQuery->ParamByName("VARSPROFILE_KEY")->AsInteger = Key;
      IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
	   IBInternalQuery->ParamByName("NUMERIC_VAL")->AsFloat = Value;
		IBInternalQuery->ExecQuery();
	}
   CacheGetProfileNum[ProfileKey][VariablesKey] = Value;
}

bool TManagerVariable::GetProfileTime(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,TDateTime &Value)
{
   bool RetVal = false;
   if(GetProfileTimeCache(DeviceProfileKey,VariablesKey,Value))
   {
      RetVal = true;
   }
   else
   {
      RetVal = GetProfileTimeDB(Transaction,DeviceProfileKey,VariablesKey,Value);
   }
	return RetVal;
}

bool TManagerVariable::GetProfileTimeDB(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,TDateTime &Value)
{
	if( !fEnabled || ProfileKey == 0) return false;
	bool RetVal = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"TIMESTAMP_VAL "
	"FROM "
		"VARSPROFILE "
	"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY AND "
		"PROFILE_KEY = :PROFILE_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
   {
		Value = IBInternalQuery->FieldByName("TIMESTAMP_VAL")->AsDateTime;
		CacheGetProfileTime[ProfileKey][VariablesKey] = Value;
		RetVal = true;
	}
	return RetVal;
}

void TManagerVariable::SetProfileTime(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,TDateTime Value)
{
   if( !fEnabled || ProfileKey == 0) return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
		
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
		"VARSPROFILE "
   "SET "
      "TIMESTAMP_VAL = :TIMESTAMP_VAL "
   "WHERE "
      "VARIABLES_KEY = :VARIABLES_KEY AND "
      "PROFILE_KEY = :PROFILE_KEY";
   IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
   IBInternalQuery->ParamByName("TIMESTAMP_VAL")->AsDateTime = Value;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RowsAffected == 0)
   {
      Create = true;
   }

   if(Create)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VARSPROFILE, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int Key = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO VARSPROFILE ("
         "VARSPROFILE_KEY,"
         "VARIABLES_KEY,"
         "PROFILE_KEY,"
         "TIMESTAMP_VAL) "
      "VALUES ("
         ":VARSPROFILE_KEY,"
         ":VARIABLES_KEY,"
         ":PROFILE_KEY,"
         ":TIMESTAMP_VAL);";
      IBInternalQuery->ParamByName("VARSPROFILE_KEY")->AsInteger = Key;
      IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	   IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = ProfileKey;
		IBInternalQuery->ParamByName("TIMESTAMP_VAL")->AsDateTime = Value;
		IBInternalQuery->ExecQuery();
	}
	CacheGetProfileTime[ProfileKey][VariablesKey] = Value;
}

bool TManagerVariable::GetProfileBool(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, bool &Value)
{
   int Temp = (int)Value;
   bool Retval = false;
	Retval = GetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (bool)Temp;
   return Retval;
}

void TManagerVariable::SetProfileBool(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,bool Value)
{
   int Temp = (int)Value;
	SetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (bool)Temp;
}

bool TManagerVariable::GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, unsigned char &Value)
{
   int Temp = (int)Value;
   bool Retval = false;
	Retval = GetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (unsigned char)Temp;
   return Retval;
}

void TManagerVariable::SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, unsigned char Value)
{
   int Temp = (int)Value;
	SetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (unsigned char)Temp;
}

bool TManagerVariable::GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, char &Value)
{
	int Temp = (int)Value;
   bool Retval = false;
	Retval = GetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (char)Temp;
   return Retval;
}

void TManagerVariable::SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, char Value)
{
   int Temp = (int)Value;
	SetProfileInt(Transaction,ProfileKey,VariablesKey,Temp);
   Value = (char)Temp;
}

void TManagerVariable::SetVarBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,bool Value)
{
   int Temp = (int)Value;
	try
	{
		SetVarInt(Transaction,VariablesKey,Name,Discription,Group,Temp);
		Value = (bool)Temp;
	}
	catch(Exception &E)
	{
		throw;
	}
}


bool TManagerVariable::GetVarBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool &Value)
{
   int Temp = (int)Value;
   bool Retval = false;
	Retval = GetVarInt(Transaction,VariablesKey, Temp);
   Value = (bool)Temp;
   return Retval;
}

bool TManagerVariable::GetProfileCache(ProfileType Type,AnsiString ProfileName,int &Value)
{
   bool Retval = false;
   std::map< ProfileType ,std::map<AnsiString,int> >::iterator ptrVariables = CacheGetProfile.find(Type);
   if(ptrVariables != CacheGetProfile.end())
   {
      std::map<AnsiString,int>::iterator ptrVar = ptrVariables->second.find(ProfileName);
      if(ptrVar != ptrVariables->second.end())
      {
         Value = ptrVar->second;
         Retval = true;
      }
   }
   return Retval;
}

bool TManagerVariable::GetProfileIntCache(int ProfileKey, vmVariables VariablesKey, int &Value)
{
   bool Retval = false;
   std::map< int , std::map<vmVariables,int> >::iterator ptrVariables = CacheGetProfileInt.find(ProfileKey);
   if(ptrVariables != CacheGetProfileInt.end())
   {
      std::map<vmVariables,int>::iterator ptrVar = ptrVariables->second.find(VariablesKey);
      if(ptrVar != ptrVariables->second.end())
      {
         Value = ptrVar->second;
         Retval = true;
      }
   }
   return Retval;
}

bool TManagerVariable::GetProfileNumCache(int ProfileKey, vmVariables VariablesKey, double &Value)
{
   bool Retval = false;
   std::map< int , std::map<vmVariables,double> >::iterator ptrVariables = CacheGetProfileNum.find(ProfileKey);
   if(ptrVariables != CacheGetProfileNum.end())
   {
      std::map<vmVariables,double>::iterator ptrVar = ptrVariables->second.find(VariablesKey);
      if(ptrVar != ptrVariables->second.end())
      {
         Value = ptrVar->second;
         Retval = true;
      }
   }
   return Retval;
}

bool TManagerVariable::GetProfileStrCache(int ProfileKey, vmVariables VariablesKey, AnsiString &Value)
{
   bool Retval = false;
   std::map< int , std::map<vmVariables,AnsiString> >::iterator ptrVariables = CacheGetProfileStr.find(ProfileKey);
   if(ptrVariables != CacheGetProfileStr.end())
   {
      std::map<vmVariables,AnsiString>::iterator ptrVar = ptrVariables->second.find(VariablesKey);
      if(ptrVar != ptrVariables->second.end())
      {
         Value = ptrVar->second;
         Retval = true;
      }
   }
   return Retval;
}

bool TManagerVariable::GetProfileTimeCache(int ProfileKey, vmVariables VariablesKey, TDateTime &Value)
{
   bool Retval = false;
   std::map< int , std::map<vmVariables,TDateTime> >::iterator ptrVariables = CacheGetProfileTime.find(ProfileKey);
   if(ptrVariables != CacheGetProfileTime.end())
   {
      std::map<vmVariables,TDateTime>::iterator ptrVar = ptrVariables->second.find(VariablesKey);
      if(ptrVar != ptrVariables->second.end())
      {
         Value = ptrVar->second;
         Retval = true;
      }
   }
   return Retval;
}

