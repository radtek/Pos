//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerVariable.h"
#include <Graphics.hpp>
#include <Memory>
#include "CPortCtl.hpp"
#include "CPort.hpp"
#include "AutoZReporting.h"

#include "XeroIntegration.h"
#include "MYOBIntegration.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//------------------------------------------------------------------------------

UnicodeString TManagerVariable::GetStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Default)
{
	if( !fEnabled )return Default;
	UnicodeString RetVal = Default;

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

bool TManagerVariable::GetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString &Value)
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


void TManagerVariable::SetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Value)
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


bool TManagerVariable::GetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey, UnicodeString &Value)
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

// This method updates only varchar_val in variables table
void TManagerVariable::SetStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Value)
{
	if( !fEnabled )return;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"VARIABLES "
		"SET "
		"VARCHAR_VAL = :VARCHAR_VAL "
		"WHERE "
		"VARIABLES_KEY = :VARIABLES_KEY";
		IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
		IBInternalQuery->ParamByName("VARCHAR_VAL")->AsString = Value;
		IBInternalQuery->ExecQuery();
    
	}
	catch(Exception &E)
	{
		throw;
	}
}

void TManagerVariable::SetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Name,UnicodeString Discription,vmGroup Group,UnicodeString Value)
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
	if( !fEnabled )return Default;
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
	"VARIABLES "
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

void TManagerVariable::SetVarInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Name,UnicodeString Discription,vmGroup Group,int Value)
{
	if( !fEnabled )return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	try
	{
      if(VariablesKey!=5014)
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

void TManagerVariable::SetVarNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Name,UnicodeString Discription,vmGroup Group,double Value)
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

void TManagerVariable::SetVarTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Name,UnicodeString Discription,vmGroup Group,TDateTime Value)
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

void TManagerVariable::InitialisePOSVars(Database::TDBTransaction &DBTransaction)
{
	try
	{
		SetVarStr(DBTransaction,vmAutoMateIP, "AutoMate IP Address",
		"This should be set to the IP address of the machine running AutoMate.\r Normally in the form 192.168.0.XXX",
		vmgNetwork, "");

		SetVarInt(DBTransaction,vmMMSubNet, "MenuMate SubNet",
		"Set to the number for this MenuMate Subnet.\r MenuMate will only respond to packets on the same SubNet",
		vmgNetwork, 0);

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

		SetVarStr(DBTransaction,vm_PLACEHOLDER_GuestMasterPath, "Guest Master Path",
		"This is the path where MenuMate expects to pick up and place it's GuestMaster Interface Files.",
		vmg3rdPartyInterface, "");

		SetVarStr(DBTransaction,vmSeatLabel, "Seat Label",
		"This is the label that appears on recetips and kithen docets that seperates seats or guests.\r"
		"The default is 'Guest'\r",
		vmgPrinting, "Guest");

		SetVarBool(DBTransaction,vm_PLACEHOLDER_UseChitNumbers, "Use Chit Numbers",
		"Does this till Use chit numbers on receipts and kithen dockets.\r"
		"Default is False",
		vmgPrinting, false);

		SetVarStr(DBTransaction,vmReceiptPrinter, "Receipt Printer",
		"The name of your receipt printer.\r",
		vmgPrinting, "None");

		SetVarStr(DBTransaction,vm_PLACEHOLDER_ChitLabel, "Auto Incrementing Chit Label",
		"This is the text label that is placed before the Chit number when the system automatically generates a chit number for you.",
		vmgPrinting, "Chit");

		SetVarStr(DBTransaction,vm_PLACEHOLDER_CustomChitLabel, "User Defined Chit Label",
		"This is the text label that is placed before the Chit number when a user selects a chit number manually.",
		vmgPrinting, "Chit");

		SetVarBool(DBTransaction,vmDuplicateReceipts, "Duplicate All Receipts",
		"Till will print two Receipts.\r"
		"Default is false",
		vmgPrinting, false);

		SetVarBool(DBTransaction,vmPrintZeroTaxes, "Print Taxes When Zero",
		"Print Taxes on Receipt When Zero.\r"
		"Default is false",
		vmgPrinting, false);

		SetVarBool(DBTransaction,vmPrintNoticeOnTransfer, "Print Notice On Transfer",
		"Print Notice On Transfer.\r"
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


		SetVarInt(DBTransaction,vmCreditPaymentGroupNumber, "Credit Group Number",
		"The Transaction group that credit spent will appear under on the Zeds.\r"
		"Default is 0",
		vmgPrinting, 0);

		SetVarInt(DBTransaction,vmMembersNameOnReceipt, "Members Name Receipt Format",
		"0 Known As / Full Name, 1 Full Name / Known As ,2 Full Name, 3 Known As, 4 First Name \r"
		"Default is 0",
		vmgPrinting, 0);


		/*		SetVarBool(DBTransaction,vmTipOnRecipt, "Display Tip:__ On Receipt",
					"Display Tip:___ On the receipt leaving space for the patron to enter a tip figure.\r"
					"Default is False",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmOptionsOnRecipt, "Display Options On Receipt",
					"Display the options ordered with the meal on the receipt.\r"
					"Default is False",
					vmgPrinting, false); */

		SetVarBool(DBTransaction,vmSignatureOnRecipt, "Display Signature Space On Receipt",
		"Leave a gap for the user to Sign on each Receipt.\r"
		"Default is False",
		vmgPrinting, false);

		SetVarBool(DBTransaction,vmSumPricingChangesOnRecipt, "Sum Pricing Changes on Receipt",
		"Add any item surcharges or discounts into the item price.\r"
		"Default is False",
		vmgPrinting, false);

		/*		SetVarBool(DBTransaction,vmSumDiscountsOnRecipt, "Sum Discounts on Receipt",
					"Add sum item discounts into a total at the bottom of the receipt.\r"
					"this option will also exclued the discount from being summed into the .\r"
					"individual item pricing.\r"
					"Default is False",
					vmgPrinting, false);*/

		SetVarBool(DBTransaction,vmSummariseDiscountOnZed, "Summarise Discounts on Zed",
		"This option will print a summary of the days discounts on the Zed.\r"
		"rather than every discount instance.\r"
		"Default is False",
		vmgPrinting, false);

		/*		SetVarBool(DBTransaction,vmDisplayChitNumberOnReceipt, "Display Chit Number on Receipt",
					"Add the Chit Number to the Top of the receipt.\r"
					"Does not work for Saved Sales.\r"
					"Default is False",
					vmgPrinting, false);*/

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

		/*		SetVarBool(DBTransaction,vmConsolidateReceipt, "Consolidate Receipt",
					"The Receipt will no longer be devided into seat by seat sections.\r"
					"Default is False",
					vmgPrinting, false);*/

		SetVarBool(DBTransaction,vm_PLACEHOLDER_AlphaNumChitNumbers, "Enable Alpha Numeric Chit Numbers",
		"This will allow you to type text as a chit number.\r"
		"Default is False",
		vmgPrinting, false);

		SetVarBool(DBTransaction,vmShowPointsOnZed, "Display Members Points on Zed",
		"All Members Points earned and redeemed will display on the Zed for that Zed period.\r"
		"Default is True",
		vmgPrinting, true);

		SetVarBool(DBTransaction, vmShowSessionDateInZed, "Display Session Date on Zed",
		"Session Date will be displayed on Zed.\r"
		"Default is False",
		vmgPrinting, false);

		// Warm If Open Table and Tabs frn
		SetVarBool(DBTransaction, vmWarnIfOpenTableAndTabs, "Enable warning when table still occupied",
		"Warning the user if there is open table and tabs before Zed report. ",
		vmgPOS, false);
		// Stop If Open Table and Tabs frn
		SetVarBool(DBTransaction, vmStopIfOpenTableAndTabs, "Enable do nothing when table still occupied",
		"Stop the process for Zed if there is open table and tabs. ",
		vmgPOS, false);

		// story MM1837
		SetVarBool(DBTransaction, vmShowTransactionSummaryGroups, "Show transaction summary",
		"Specifies whether or not to show the client details for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowBilledSalesTotals, "Show billed sales total",
		"Specifies whether or not to show the billed sales for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowComplimentarySalesTotals, "Show complimentary sales totals",
		"Specifies whether or not to show the complimentary sales for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowChargedSalesTotals, "Show charged sales totals",
		"Specifies whether or not to show the charged sales for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowTotals, "Show totals",
		"Specifies whether or not to show the totals for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowBreakdownCategories, "Show breakdown categories",
		"Specifies whether or not to show the breakdown categories for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowDiscountReport, "Show discount report",
		"Specifies whether or not to show the discount report for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowPointsReport, "Show points report",
		"Specifies whether or not to show the points report for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowPatronAverages, "Show patron averages",
		"Specifies whether or not to show the patron averages for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowProductionInfo, "Show production info",
		"Specifies whether or not to show the production info for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowAccountPurchases, "Show account purchases",
		"Specifies whether or not to show the account purchases for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowAccountBalancesTabs, "Show account balances tabs",
		"Specifies whether or not to show the account balances tabs for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowAccountBalances, "Show account balances",
		"Specifies whether or not to show the account balances for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmShowHourlySales, "Show hourly sales",
		"Specifies whether or not to show the hourly sales for the site "
		"when closing the til.",
		vmgPOS, false);
		SetVarBool(DBTransaction, vmRevenueFiguresAreTaxAndServiceChargeInclusive, "Revenue figures are tax and service charge inclusive",
		"Specifies whether or not to show the revenue figures are tax and service charge inclusive for the site "
		"when closing the til.",
		vmgPOS, false);

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

		/*		SetVarBool(DBTransaction,vmHideFreeItemsOnReceipt, "Hide Free Items On Receipt.",
					"Item with a Natural Price of $0.00 will not show on Receipts.\r"
					"Excludes Items Price Adjusted to $0.00.\r"
					"Default is false",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmHideZeroDollarsOnReceipt, "Hide $0.00 On Receipt.",
					"The Free Item will show.\r"
					"The $0.00 will not appear in the Cash Coloum on Receipts.\r"
					"Default is false",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmHideCancelledItemsOnReceipts, "Hide Cancelled Items On Receipt.",
					"Cancelled Items will not show on Receipts.\r"
					"Default is false",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmHidePrevPayemntsOnReceipts, "Hide Payment History On Receipt.",
					"Previous Payment History will not show on Receipts.\r"
					"Default is false",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmHideMembershipDetailsOnReceipts, "Hide Membership Details On Receipt.",
					"Membership Details will not show on Receipts.\r"
					"Default is false",
					vmgPrinting, false);*/

		/*		SetVarBool(DBTransaction,vmRavellItemsWithPriceAdjusts, "Ravel up Price Adjusted items on Receipts.",
					"Ravel up (sum qty) items on Receipts even though one or more have had there price changed.\r"
					"Default is false",
					vmgPrinting, false);*/

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

		SetVarInt(DBTransaction,vmPrinterCodePage, "Kitchen Printer Code Page",
		"The Code Page for the Printer for Multibyte Chatacter Sets.\r"
		"Default is 0.\r",
		vmgPrinters, 0);

		SetVarStr(DBTransaction,vmPrinterCodePageOn, "Printer Code Page On String",
		"The String to change Code Page for the Printer.\r"
		"Default is ''.\r",
		vmgPrinters, "");

		SetVarStr(DBTransaction,vmPrinterCodePageOff, "Printer Code Page Off String",
		"The String to change Code Page for the Printer.\r"
		"Default is ''.\r",
		vmgPrinters, "");

		SetVarStr(DBTransaction,vmPrinterInitCmd, "Printer Initilisation Command",
		"The String to Initilise the Printer, Often used to load a code page.\r"
		"Default is ''.\r",
		vmgPrinters, "");


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

		SetVarBool(DBTransaction,vmAutoPrintWebReceipts, "Auto Print Web Receipt On Save",
		"Whenever you save a Web Order it will automatically print a\r"
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

		SetVarInt(DBTransaction,vmProxWiegandFormat, "Prox Reader data format",
		"0) Normal , 1) Wiegand card so ignore the variable data at the start.\r"
		"Default is Normal",
		vmgMiscEquipment, 0);

		SetVarInt(DBTransaction,vmScalesProfile, "Scales Profile Key",
		"This is the profile key for this devices scales.\r"
		"Default is -1",
		vmgMiscEquipment, -1);

		SetVarInt(DBTransaction,vmProxProfile, "Prox Profile Key",
		"This is the profile key for this devices Prox reader.\r"
		"Default is -1",
		vmgMiscEquipment, -1);

		SetVarBool(DBTransaction,vmAutoConvertScalesWeights, "Convert scales weights automatically.",
		"If the scales are not measureing weights in kg then auto convert them.\r"
		"Default is False",
		vmgMiscEquipment, false);

		SetVarBool(DBTransaction, vmAllowEnterWeight, "Allow to enter weight.",
		"Allow to enter weight manually on Scales interface.\r"
		"Default is False", vmgMiscEquipment, false);

		SetVarBool(DBTransaction, vmAllowEnterPrice, "Allow to enter price.",
		"Allow to enter price manually on Scales interface.\r"
		"Default is False", vmgMiscEquipment, false);

		SetVarBool(DBTransaction, vmAllowEnterPrice, "Allow to enter price.",
		"Allow to enter price manually on Scales interface.\r"
		"Default is False", vmgMiscEquipment, false);

		SetVarBool(DBTransaction, vmShowCustomerDisplay, "Toggle second customer display.",
		"Toggle menumate second customer display system.\r"
		"Default is False", vmgMiscEquipment, false);

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

		SetVarInt(DBTransaction,vmMembershipDatabasePort, "Membership IP Port",
		"Port Number for the EBet Membership Server TCPIP port  (i.e 10061)",
		vmg3rdPartyInterface, 10061);

		SetVarInt(DBTransaction,vmMembershipTillID, "Membership Till ID",
		"EBet Membership Till ID  (i.e 1)",
		vmg3rdPartyInterface, 1);

		SetVarStr(DBTransaction,vmMembershipDatabasePath, "Membership Database Path",
		"This should be set to the path of the Membership.fdb file as it resides on the server.\r"
		"This setting is only use for 3rd party membership products.\r"
		" Normally it is C:\\Program Files\\MenuMate\\Membership.fdb",
		vmgNetwork, "C:\\Program Files\\MenuMate\\Membership.fdb");

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

		SetVarBool(DBTransaction,vmEnableEftPosCadmus, "Use Cadmus Keylink EftPos",
		"Till uses Cadmus Keylink one way Intergrated EFTPOS.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableEftPosCadmusCronos, "Use Cadmus Cronos EftPos",
		"Till uses Cadmus Cronos fully Intergrated EFTPOS.\r"
		"Default is False",
		vmg3rdPartyInterface, false);


		SetVarBool(DBTransaction,vmDisableInternalInvoicing, "Disable Internal Invoicing",
		"Disable Internal Invoicing so they only have the invoice payment type to export to MYOB.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableBlindBalances, "Enable Blind Balances",
		"Disables X Reports and enables blind balances on Zed reports.\r"
		"Default is False",
		vmgPOS, false);

		SetVarInt(DBTransaction,vmEndOfDay, "End Of Day",
		"Describes the end of day for POS.\r",
		vmgPOS, 5);

		SetVarBool(DBTransaction,vmExtendCurrencyDisplay, "Display fourteen characters on select dish.",
		"Display fourteen characters on select dish.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableTableDisplayMode, "Display Table View On Pos Start-up.",
		"Display Table View On Pos Start-up.\r"
		"Default is False",
		vmgPOS, false);
		SetVarBool(DBTransaction,vmEnableStaffHours, "Enable Staff Hours",
		"Enables staff hours on Zed reports.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableCommission, "Enable Commission",
		"Enables commission / tips on Zed reports.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnablePrinterCounts, "Enable Printer Counts",
		"Enables printer counts on Zed reports.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnablePaxCount, "Enable Pax Count",
		"Enables pax count on Zed reports.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmHideCredsCancels, "Enable Hide Credits and Cancels",
		"Hides the Credits and Cancels information on the Z report.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmCaptureCustomer, "Enable Capture The Customers Name",
		"Prompts the users to enter the customers name for every transaction.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableSeperateEarntPts, "Separate Earnt & Loaded Points",
		"Seperates the total points into Earnt and Loaded points.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmDisableConfirmationOnSave, "Disable Confirmation On Save",
		"Disable the confirmation dialog and receipt display that shows up on Save.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmDisableReceiptOnConfirmation, "Disable Receipt On Confirmation",
		"Disable the receipt on the confirmation dialog that shows up on Save.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,
		vmShowVoucher,
		"Show Voucher Details",
		"Shows extra information on redeemed vouchers.\r"
		"Default is False",
		vmgPOS,
		false);

		SetVarBool(DBTransaction,
		vmEnablePhoneOrders,
		"Enable Phone Orders",
		"Enables easy access to member creation.\r"
		"Default is False",
		vmgPOS,
		false);

		SetVarBool(DBTransaction,
		vmMenuPatronCount,
		"Enable Menu Patron Count",
		"Enables default patron count to be calculated from patron counts in items.\r"
		"Default is False",
		vmgPOS,
		false);

		SetVarBool(DBTransaction,vmEnableDisplayTax, "Display Tax",
		"Displays tax on POS.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableDisplayServiceCharge, "Display Service Charge",
		"Displays Service Charge on POS.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableNmiDisplay, "Enable Nmi Display",
		"Enable Nmi Display format on POS.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmInclServiceChargeTaxInTaxVal, "Include Service Charge Tax in Tax Value",
		"Include service charge tax in the tax value displayed on the POS instead of in the service charge value displayed.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableDisplayTaxRemoval, "Display Tax Removal Option",
		"Displays Tax Removal Option on POS.\r"
		"Default is False",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableEftPosDPS, "Use DPS EftPos",
		"Till uses DPS Intergrated EFTPOS.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction, vmEnableDPSTipping, "Enable DPS Tipping",
		"Can add tips to Visa Transactions on DPS EFTPOS. \r"
		"Default is False",
		vmg3rdPartyInterface,
		false);


		SetVarBool(DBTransaction,vmEnableEftPosIceLink, "Use ICELink EftPos",
		"Till uses ICELink Intergrated EFTPOS.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmSmartCardMembership, "Use Smart Cards",
		"Enabled Membership to use smart cards.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableAARewards, "Use AA Rewards",
		"Enabled AA Rwards System.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmEnableAARewardsReminder, "Use AA Rewards Reminder",
		"Enabled AA Rwards Reminder.\r"
		"Default is False",
		vmg3rdPartyInterface, false);

		SetVarInt(DBTransaction,vmEftposSerialPort, "Eftpos Serial Port",
		"Serial Port Number for the Cadmus & Icelink Eftpos.",
		vmg3rdPartyInterface, -1);

		SetVarInt(DBTransaction,vmEftposMerchantNumber, "Eftpos Merchant Number",
		"Merchant Number for the Cadmus Eftpos.\r"
		"Default is 1",
		vmg3rdPartyInterface, 1);

		SetVarBool(DBTransaction,vm_PLACEHOLDER_UseStockMaster, "Use ERS Stock Master",
		"Does the system interface with stock master?.\r",
		vmg3rdPartyInterface, 0);

		SetVarStr(DBTransaction,vmStockMasterExportPath, "Stock Master Export Path",
		"Directory in which to place the stock master files.",
		vmg3rdPartyInterface, "");

		SetVarStr(DBTransaction,vmPMSIPAddress, "PMS IP Address",
		"IP Address of the PMS Server (i.e 192.168.1.250)",
		vmg3rdPartyInterface, "");

		SetVarInt(DBTransaction,vmPMSTCPPort, "PMS IP Port",
		"Port Number for the PMS Server TCPIP port  (i.e 4444)",
		vmg3rdPartyInterface, 4444);

		SetVarInt(DBTransaction,vmPMSPOSID, "PMS POS ID",
		"The Unique POSID for this terminal on the PMS Server",
		vmg3rdPartyInterface, 0);

		SetVarStr(DBTransaction,vmPMSPaymentCategory, "PMS Default Payment Category",
		"The Category Payment types will deduct against in the PMS system/r"
		"Default is 9999",
		vmg3rdPartyInterface, "9999");

		SetVarStr(DBTransaction,vmPMSItemCategory, "PMS Default Item Category",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is 9998",
		vmg3rdPartyInterface, "9998");

		SetVarStr(DBTransaction,vmPMSPointsCategory, "PMS Interface Points Category",
		"The Category Points Payments will deduct against in the PMS system/r"
		"Default is 9997",
		vmg3rdPartyInterface, "9997");

		SetVarStr(DBTransaction,vmPMSCreditCategory, "PMS Credit Category",
		"The Category Credit Payments will deduct against in the PMS system/r"
		"Default is 9996",
		vmg3rdPartyInterface, "9996");

		SetVarStr(DBTransaction,vmPMSDefaultAccount, "PMS Default Account",
		"The Account number against which all non room charges are made./r"
		"Default is 90000",
		vmg3rdPartyInterface, "90000");

		SetVarStr(DBTransaction,vmPMSDefaultSurchargeAccount, "PMS Surcharge Account",
		"The Account number against which all payment surcharges are made./r"
		"This includes 'Cheque fee' type charges but not custom surchages like tips."
		"Default is 9995",
		vmg3rdPartyInterface, "9995");

		SetVarStr(DBTransaction,vmPMSDefaultRoundingAccount, "PMS Rounding Code",
		"The Account number against which all payment roundings are sotred./r"
		"This is mostly used for cash type charges."
		"Default is 9994",
		vmg3rdPartyInterface, "9994");

		SetVarBool(DBTransaction,vmTabsEnabled, "Enable Tabs Interface",
		"If set customer and staff tabs will be enabled.\r"
		"Default is True",
		vmgMenuMateModules, true);

		SetVarBool(
		DBTransaction,
		vmPrintStockWeborders,
		"Print Stock Purchase weborders",
		"Displays Stock Purchase weborder statistics on X and Z reports."
		"Default is False", vmgPOS, false);


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


		SetVarBool(DBTransaction,vm_PLACEHOLDER_MemberNumericPannelFirst, "Member Show Numeric Pannel",
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

		SetVarBool(DBTransaction,vm_PLACEHOLDER_DisplayServingCourseColours, "Display Serving Course Colours",
		"No Longer Used. Will Display the Serving Course buttons and there \"Above Item\"\r"
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

		SetVarBool(DBTransaction,vmHideFreeItemsAtBilling, "Hide Free Items at Billing",
		"Any free items are not displayed at Billing.\r"
		"Defaults to false.\r",
		vmgPOS, false);

		SetVarBool(DBTransaction,vmEnableWaiterStation,
		"Enable Waiter Station",
		"Current terminal will be act as a waiter station. \r "
		"Tender,CashSale,FastTender,Billing will be disabled on checked.Default is False",
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


		SetVarBool(DBTransaction,vmLuckyMemberDrawByLocationOnly, "Lucky Member Draw by Location",
		"Lucky Member Draw by Location.\r",
		vmgHappyHour, false);

		SetVarInt(DBTransaction,vmLuckyMemberPeriod, "Lucky Member Period.",
		"The sales period (in hours) over which a lucky member draw is selected.\r"
		"Default is 24 hours.",
		vmgHappyHour, 24);

		SetVarBool(DBTransaction,vmHappyHourMembersOnly, "Happy Hour Members Only.",
		"Happy Hour Prices will only be applied for Members.\r"
		"Default is false.",
		vmgHappyHour, false);

		SetVarBool(DBTransaction,vmNoPtsEarnedDuringHappyHour, "No Points Earned During Happy Hour.",
		"Members will not Earn Points During Happy Hour.\r"
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

		SetVarBool(DBTransaction,vmAllowSmartCardNotPresentMemberTransactions, "Allow Smart Card not present transactions.",
		"Allow a Member to use there membership account when no smart card is presented.\r"
		"Default is No\r",
		vmgSystem, false);


		SetVarStr(DBTransaction,vmSmartCardCode, "Smart Card Encryption Code",
		"Smart Card Encryption Code\r"
		"Default is ''\r",
		vmgSystem, "");

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
		"Default is ''\r",
		vmgSystem, "");

		SetVarInt(DBTransaction,vmRootAccount, "The Root Account",
		"This is the Key for the account that currently has Temp Admin Access.\r"
		"Default is 0.",
		vmgSystem, 0);

		SetVarTime(DBTransaction,vmRootAccountTimer, "The Root Account Expiry Timer",
		"This is the timer for when the Temp Admin Access expires.\r"
		"Default is 0.",
		vmgSystem, TDateTime(0));

		SetVarBool(DBTransaction,vmEnableBirthdayNotifications, "Enable Birthday Notifications",
		"Enable Birthday Notifications.\r"
		"Default is false\r",
		vmgSystem, false);

		SetVarInt(DBTransaction,vmScalesType, "The Type of Scales installed",
		"The Type of Scales installed. 0 Type A STD Stream, 1 Type C QUQA Stream\r"
		"Default is 0.",
		vmgSystem, 0);
		SetVarInt(DBTransaction,vmBarcodeFormat, "The Type of Barcode Format",
		"The Type of Barcode Format Used.\r"
		"Default is 0.",
		vmgSystem, 0);

		SetVarStr(DBTransaction,vmIntaMateIPAddress, "IntaMate IP Address",
		"IntaMate IP Address\r"
		"Default is ''\r",
		vmg3rdPartyInterface, "");


		SetVarInt(DBTransaction,vmIntaMatePort, "IntaMate TCPIP Port",
		"IntaMate TCPIP Port.\r"
		"Default is 0.",
		vmg3rdPartyInterface, 0);

		SetVarInt(DBTransaction,vmIntaMateIPTimeOut, "IntaMate TCPIP TimeOut",
		"IntaMate TCPIP Port Time Out.\r"
		"Default is 60000.",
		vmg3rdPartyInterface, 60000);

		SetVarInt(DBTransaction,vmIntaMateTerminalID, "IntaMate POS ID",
		"IntaMate POS ID.\r"
		"Default is 0.",
		vmg3rdPartyInterface, 0);

		SetVarStr(DBTransaction,vmPocketVoucherURL, "Pocket Voucher URL",
		"Pocket Voucher URL.\r"
		"Default is http://pos.host.pocketvouchers.com/menumate.aspx",
		vmg3rdPartyInterface, "http://pos.host.pocketvouchers.com/menumate.aspx");
		SetVarBool(DBTransaction,vmAcknowledgeFirstVisit, "First Visit Notifications",
		"Enable First Visit Notifications.\r"
		"Default is false\r",
		vmgSystem, false);

		SetVarBool(DBTransaction,vmEnableMemberSpendChits, "Enable Member Spend Chits",
		"Enable Member Spend Chits.\r"
		"Default is false\r",
		vmgSystem, false);

		SetVarStr(DBTransaction,vmMemberSpendChitsAmount, "Member Spend Chit Value",
		"Member Spend Chit Value\r"
		"Default is '5.00'\r",
		vmgSystem, "5.00");

		SetVarInt(DBTransaction,vmMembershipType, "MenuMate Membership Type",
		"MenuMate MembershipType.\r"
		"Default is Membership Type MenuMate.",
		vmg3rdPartyInterface, 0);

		SetVarInt(DBTransaction,vmWebMatePort, "WebMate Server Port",
		"Port number the Webmate server will listen on.\r"
		"Default is 30000.",
		vmg3rdPartyInterface, 65006);

		SetVarBool(DBTransaction,vmWebMateEnabled, "Enable WebMate Server",
		"Enable WebMate Server to accept web orders.\r"
		"Default is false\r",
		vmg3rdPartyInterface, false);

		SetVarBool(DBTransaction,vmAutoAcceptWebOrders, "Auto Accept Web Orders",
		"Auto Accept Web Orders\r"
		"Default is false\r",
		vmAutoAcceptWebOrders, false);

		SetVarBool(DBTransaction,vmAutoAddWebMembers, "Automaticaly add Members from Web Orders",
		"If a Web order comes in from a member the member will be automatically be\r"
		"added to the POS membership system.\r"
		"Default is false.\r",
		vmg3rdPartyInterface, false);

		SetVarInt(DBTransaction,vmKioskDisplayMode, "Display Mode for Kiosk",
		"0 is Points Mode, 1 Membership Mode.\r"
		"Default is 0.",
		vmg3rdPartyInterface, 0);

		SetVarInt(DBTransaction,vmKioskMembershipMode, "Membership Mode for Kiosk",
		"0 is ERS Club Membership, 1 External Membership Mode.\r"
		"Default is 0. ERS",
		vmg3rdPartyInterface, 0);

		SetVarInt(DBTransaction,vmPosGuiDimensionsColumns, "Display Items Grid No. Columns",
		"This Number of Menu Items Across the Screen.\r"
		"Default is 4.",
		vmgSystem, 4);

		SetVarInt(DBTransaction, vmRoundingTime, "Rounded to time",
		"Rounded to time.\r"
		"Default is 1.",
		vmgPOS, 1);

		SetVarInt(DBTransaction, vmPosGuiDimensionsRows,
		"Display Items Grid No. Rows",
		"This Number of Menu Items Down the Screen.\r"
		"Default is 8.",
		vmgSystem, 8);

		SetVarInt(DBTransaction, vmPosGridFontSize,
		"The font size used for Item Grid entries.",
		"Default is $8A (10pt). Top bit signals default.", vmgSystem,
		0x8a);

		SetVarBool(DBTransaction,
		vmBarExchangeServerGlobalEnabled,
		"Check BarExchange Global Server Setting",
		"This setting will enable or disable BarExchange Server if no any server Selected ",
		vmgSystem,
		false);

		SetVarInt(DBTransaction, vmAutoZProfileKey,
		"Auto Z-Reporting Profile Key",
		"The default is -1. This signifies that Auto Z-Reporting has "
		"never been configured.", vmgPOS, -1);

		SetVarInt(DBTransaction, vmAutoZAuthAs,
		"The User to run run Z-Reports as",
		"The default is 0. It signifies that Auto Z-Reporting "
		"has never been setup.", vmgPOS, 0);

		SetVarTime(DBTransaction, vmAutoZCheck1Time,
		"Auto Z-Reporting - Check 1 time",
		"Default is '30.12.1899, 00:00:00.000'. "
		"Keep in mind only the time is used.", vmgPOS, 0);
		SetVarInt(DBTransaction, vmAutoZCheck1Options,
		"Auto Z-Reporting - Check 1 options",
		"Default is 0, signifying this check is disabled.",
		vmgPOS, 0);

		SetVarTime(DBTransaction, vmAutoZCheck2Time,
		"Auto Z-Reporting - Check 2 time",
		"Default is '30.12.1899, 00:00:00.000'. "
		"Keep in mind only the time is used.", vmgPOS, 0);
		SetVarInt(DBTransaction, vmAutoZCheck2Options,
		"Auto Z-Reporting - Check 2 options",
		"Default is 0, signifying this check is disabled.",
		vmgPOS, 0);

		SetVarTime(DBTransaction, vmAutoZCheck3Time,
		"Auto Z-Reporting - Check 3 time",
		"Default is '30.12.1899, 00:00:00.000'. "
		"Keep in mind only the time is used.", vmgPOS, 0);
		SetVarInt(DBTransaction, vmAutoZCheck3Options,
		"Auto Z-Reporting - Check 3 options",
		"Default is 0, signifying this check is disabled.",
		vmgPOS, 0);

		SetVarTime(DBTransaction, vmAutoZCheck4Time,
		"Auto Z-Reporting - Check 4 time",
		"Default is '30.12.1899, 00:00:00.000'. "
		"Keep in mind only the time is used.", vmgPOS, 0);

		SetVarInt(DBTransaction, vmAutoZCheck4Options,
		"Auto Z-Reporting - Check 4 options",
		"Default is 0, signifying this check is disabled.",
		vmgPOS, 0);

		SetVarBool(DBTransaction,vmReservationsEnabled, "Enable Floor Plan and Reservations",
		"Enable Floor Plan and Reservations.\r"
		"Default is false.\r",
		vmg3rdPartyInterface, false);

		SetVarStr(DBTransaction,vmReservationsAddress, "Reservation / Floor Plan URL",
		"Reservation / Floor Plan URL.\r"
		"Default is http://localhost:8732/MenumateWinServices/service",
		vmg3rdPartyInterface, "http://localhost:8732/MenumateWinServices/service");

		SetVarBool(DBTransaction,vmFloorPlanTransparentTables, "Enable Floor Plan Transparent Tables",
		"Enable Floor Plan Transparent Tables.\r"
		"Default is false.\r",
		vmg3rdPartyInterface, false);

		SetVarInt(DBTransaction,vmFloorPlanTableColour, "Floor Plan Table Colour",
		"Floor Plan Table Colour.\r"
		"Default is Black.",
		vmg3rdPartyInterface, clBlack);

		SetVarInt(DBTransaction,vmFloorPlanTextColour, "Floor Plan Table Text Colour",
		"Floor Plan Table Text Colour.\r"
		"Default is White.",
		vmg3rdPartyInterface, clWhite);

		SetVarInt(DBTransaction,vmLastSelectedFloorPlanLocationID, "Last Select Floor Plan",
		"Last Selected Floor Plan ID.\r"
		"Default is -1 None Selected.",
		vmg3rdPartyInterface, -1);

		/* Quick Member Setup */
		SetVarBool(
		DBTransaction,
		vmQuickMemberSetupEnabled,
		"Enable Quick Member Setup",
		"Provides a faster way of creating members for smartcard users.",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmQuickMemberSetupPhoneIsMandatory,
		"QMS - Phone Number is mandatory.",
		"A phone number must be entered when creating a member via QMS.",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmQuickMemberSetupEmailIsMandatory,
		"QMS - Email Address is mandatory.",
		"An email address must be entered when creating a member via QMS.",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmQuickMemberSetupDateOfBirthIsMandatory,
		"QMS - Date of Birth is mandatory.",
		"The date of birth must be entered when creating a member via QMS.",
		vmgPOS,
		false);

		//story MM1837 remove
		/* Salesforce
		SetVarBool(
		DBTransaction,
		vmSalesforceUploadEnabled,
		"Upload to Salesforce",
		"Determines whether the P.O.S uploads transaction data to Salesforce.",
		vmgPOS,
		false);

		SetVarStr(
		DBTransaction,
		vmSalesforceUsername,
		"Salesforce Username",
		"The username component of the Salesforce login credential.",
		vmgPOS,
		"");

		SetVarStr(
		DBTransaction,
		vmSalesforcePassword,
		"Salesforce Password",
		"The password component of the Salesforce login credential.",
		vmgPOS,
		"");

		SetVarStr(
		DBTransaction,
		vmSalesforceSecurityToken,
		"Salesforce Security Token",
		"The security token component of the Salesforce login credential.",
		vmgPOS,
		"");

		SetVarBool(
		DBTransaction,
		vmSalesforceUploadStock,
		"Upload stock data to Salesforce",
		"Specifies whether stock data is uploaded to Salesforce when the till "
		" is closed.",
		vmgPOS,
			false);*/

		SetVarBool(
		DBTransaction,
		vmDepositBagNum,
		"Set this terminal for deposit bag number",
		"Allows only this terminal to be used to set the deposit bag number.",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmDontClearZedData,
		"Won't remove data during zed report",
		"If this till is Zed'd, the data will not be removed",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmHoldSend,
		"Wont send off kitchen docket until send is clicked",
		"If this box is ticked, the kitchen dockets wont b sent straight away.",
		vmgPOS,
		false);

		SetVarBool(
		DBTransaction,
		vmSpendPtsOnlyFullProduct,
		"Points can only be spend on full products",
		"If this box it ticked points will only be allowed to be used on the full price of a product.",
		vmgMisc,
		false);
		SetVarInt(
		DBTransaction,
		vmMinRedemptionPoint,
		"Minimum Points to redeem.",
		"Minimum Points to redeem, Default is 0.",
		vmgMisc,
		0);
		SetVarBool(
		DBTransaction,
		vmSpendPtsOnlySelectItems,
		"Points can only be spend on selected items",
		"If this box it ticked points will only be allowed to be used on items selected in the menu editor.",
		vmgMisc,
		false);
		//MM-3395
		SetVarBool(
		DBTransaction,
		vmAllowRefundPoints,
		"Points can not be refunded by staff",
		"If this box it ticked points will not be refunded by staff.",
		vmgMisc,
		false);

		SetVarBool(
		DBTransaction,
		vmEarntPointsOnWeight,
		"Points can earned based on weight for weighted items",
		"If this box is ticked points will be calculated on the weight purchased.",
		vmgMisc,
		false);


		SetVarBool(
		DBTransaction,
		vmPoleDisplayShowPoints,
		"Points can be displayed on the pole display",
		"If this box it ticked points will be displayed in the pole display.",
		vmgMisc,
		false);
		SetVarBool(
		DBTransaction,
		vmAllowPointPaymentByWeight,
		"Allow Point Payment By Weight",
		"If this box it ticked user can enter weight to redeem.",
		vmgMisc,
		false);
		SetVarBool(
		DBTransaction,
		vmAllowPointPaymentByValue,
		"Allow Point Payment By Value",
		"If this box it ticked user can enter points to redeem.",
		vmgMisc,
		true);

		SetVarBool(DBTransaction,
		vmNagUserToSelectChit,
		"Automatic chit prompting",
		"If this box is ticked the user will be prompted to select "
		"a new chit at the begining of each new order.",
		vmgPOS,
		false);

		SetVarBool(DBTransaction,
		vmItemPriceIncludeTax,
		"Price Include Tax Checkbox ticked",
		"This setting will set the Price to include Tax",
		vmgTax,
		true);

		SetVarBool(DBTransaction,
		vmItemPriceIncludeServiceCharge,
		"Price Include Service Charge Checkbox ticked",
		"This setting will set the Price to include Service Charge",
		vmgTax,
		false);

		SetVarBool(DBTransaction,
		vmCalculateTaxPostDiscount,
		"Calculate Tax After Discount",
		"Tax will be calculated after Discount is applied",
		vmgTax,
		true);

		SetVarBool(DBTransaction,
		vmCalculateServiceChargePostDiscount,
		"Calculate SVC After Discount",
		"Service Charge will be calculated after Discount is applied",
		vmgTax,
		true);

		SetVarBool(DBTransaction,
		vmUsingServiceCharge,
		"Service Charge is Being Used",
		"Will be true if neither of the settings to calculate service charge pre discount or post discount are set to true",
		vmgTax,
		true);

		SetVarBool(DBTransaction,
		vmApplyServiceChargeTax,
		"Apply Tax to Service Charge",
		"Will take tax applied at Tax Profile to be chosen and implemented",
		vmgTax,
		false);

		SetVarInt(DBTransaction,
		vmServiceChargeTaxProfileKey,
		"Service Charge Tax Tax Profile Key",
		"Getting the Tax Profile Key from Tax Profile Table",
		vmgTax,
		0);

		SetVarNum(DBTransaction,
		vmServiceChargeTaxRate,
		"Service Charge Tax Rate",
		"Getting the Service Charge Tax Rate",
		vmgTax,
		0);

		int GlobalProfileKey = GetProfile(DBTransaction,eSystemProfiles,"Globals");

		if(GlobalProfileKey == 0)
		{
			GlobalProfileKey = SetProfile(DBTransaction,eSystemProfiles,"Globals");

			SetProfileBool(DBTransaction, GlobalProfileKey, vmMidPointRoundsDown, true);
			SetProfileInt(DBTransaction, GlobalProfileKey, vmSiteID, -1);
			SetProfileStr(DBTransaction, GlobalProfileKey, vmStaffMessageOfTheDay, "");

			SetProfileInt(DBTransaction, GlobalProfileKey, vmPosGuiDimensionsRows, 8);
			SetProfileInt(DBTransaction, GlobalProfileKey, vmPosGuiDimensionsColumns, 4);
			SetProfileInt(DBTransaction, GlobalProfileKey, vmPosGridFontSize, 0x8a);

			SetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeTax,                true);
			SetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeServiceCharge,      false);
			SetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateTaxPostDiscount,           true);
			SetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateServiceChargePostDiscount, true);
			SetProfileBool(DBTransaction, GlobalProfileKey, vmUsingServiceCharge,                 false);
			SetProfileBool(DBTransaction, GlobalProfileKey, vmApplyServiceChargeTax,              false);
			SetProfileInt( DBTransaction, GlobalProfileKey, vmServiceChargeTaxProfileKey,         0);
			SetProfileNum( DBTransaction, GlobalProfileKey, vmServiceChargeTaxRate,               0);
		}

		SetVarStr( DBTransaction, vmXeroMachineName, "Xero Integration / Machine Name",
		"Xero Integration / Machine Name.\r"
		"Default is localhost",
		vmg3rdPartyInterface, "localhost" );

		SetVarStr( DBTransaction, vmXeroUserName, "Xero Integration / Username",
		"Xero Integration / Username.\r"
		"Default is empty string",
		vmg3rdPartyInterface, "" );

		SetVarStr( DBTransaction, vmXeroPassword, "Xero Integration / Password",
		"Xero Integration / Password.\r"
		"Default is empty string",
		vmg3rdPartyInterface, "" );

		SetVarStr(DBTransaction, vmXeroFolderPath, "Xero Integration / Folder URL",
		"Xero Integration / Folder URL.\r"
		"Default is C:\\Program Files\\Menumate\\" + XERO_INTEGRATION_FOLDER,
		vmg3rdPartyInterface, "C:\\Program Files\\Menumate\\" + XERO_INTEGRATION_FOLDER);

		SetVarInt(DBTransaction,vmExternalMembershipDiscountCode, "EM Discount Code",
		"External Membership Discount Code  (i.e 190)",
		vmg3rdPartyInterface, 190);
		SetVarInt(DBTransaction,vmExternalMembershipPointsCode, "EM Points Code",
		"External Membership Points Code  (i.e 104)",
		vmg3rdPartyInterface, 104);
		SetVarInt(DBTransaction,vmExternalMembershipSystemCode, "EM System Code",
		"External Membership Discount Code  (i.e 127)",
		vmg3rdPartyInterface, 127);

		//Loyalty mate
		SetVarBool(DBTransaction,
		vmLoyaltyMateEnabled,
		"Enable loyalty mate",
		"Enable or disables loyalty mate service access from pos "
		"Enable or disables loyalty mate service access from pos",
		vmgPOS,
		false);

		SetVarStr(DBTransaction,
		vmServiceURL,
		"LoyaltyMate WCF Service URL",
		"LoyaltyMate WCF Service URL.\r"
		"Default is http://localhost:8734/MenumateServices/LoyaltyMate",
		vmgPOS, "http://localhost:8734/MenumateServices/LoyaltyMate" );

		SetVarStr(DBTransaction,
		vmMemberActivationURL,
		"Member activation URL",
		"Member activation URL.\r"
		"Default is https://[SUBDOMAIN].loyalty-mate.com/accounts/sign_in",
		vmgPOS, "https://[SUBDOMAIN].loyalty-mate.com/accounts/sign_in" );

		SetVarBool(DBTransaction,
		vmEnableActivationTokenPrint,
		"Enable activation token printing",
		"This setting will enable or disable printing activation tokens on first member creation ",
		vmgPOS,
		false);

		SetVarStr(DBTransaction,
                    vmActivationTokenGreeting,
                    "Member activation token greeting",
			        "Activation token greeting appear on the receipt.\r"
			        "Default is Welcome to loyaltymate cloud services",
			        vmgPOS, "Welcome to loyaltymate cloud services");
        SetVarBool(DBTransaction,
                   vmUseTierLevels,
                   "Enable Tier Levels",
                   "Enable Tier Levels setting is used to update tier level after each transaction",
                   vmgPOS,
                   false);

        SetVarBool(DBTransaction,
                   vmAllowMemberDetailscreen,
                   "Enable  Allow Member Detail screen",
                   "Enable  Member Detail screen",
                   vmgPOS,
                   false);

         SetVarBool(DBTransaction,
                   vmMandatoryMembershipCard,
                   "Make payment Possible only By Membership Card",
                   "Make payment Possible only By Membership Card",
                   vmgPOS,
                   false);

       SetVarInt(DBTransaction,
                   vmBirthdayBuffer,
                   "Birthday Buffer",
                   "Duration in days in which user can claim birthday benefits.",
                   vmgPOS,
                   1);
       SetVarInt(DBTransaction,
                   vmFirstVisitPoint,
                   "First Visit Points",
                   "Points rewarded to member on his first visit.",
                   vmgPOS,
                   0);
        SetVarBool(DBTransaction,
                   vmShowCustomerDisplay,
                   "Show/Hide Customer Display",
                   "This setting will tell the POS system whether the Customner Display is to be shown or not ",
                   vmgMiscEquipment,
                   false);

		SetVarBool(DBTransaction,
		vmEnableParkedSales,
		"Enable/Disable parked sales",
		"This setting will tell the POS system whether the Parked Sales can be used or not ",
		vmgPOS,
		true);

		SetVarBool(DBTransaction,
		vmShowCustomerDisplay,
		"Show/Hide Customer Display",
		"This setting will tell the POS system whether the Customner Display is to be shown or not ",
		vmgMiscEquipment,
		false);
        SetVarBool(DBTransaction,
		vmIsMemberSalesOnlyEnabled,
		"Is Member Sales Only Enabled",
		"Is Member Sales Only Enabled ",
		vmgMiscEquipment,
		false);
		SetVarBool(DBTransaction,
		vmEnableParkedSales,
		"Enable/Disable parked sales",
		"This setting will tell the POS system whether the Parked Sales can be used or not ",
		vmgPOS,
		true);

		SetVarStr(DBTransaction,vmReceiptNumberLabel,"Receipt Number Label",
		"Label for receipt number in receipt print.",
		vmgPrinting, "Receipt No.");
		SetVarStr(DBTransaction,vmReprintReceiptLabel,"Reprint Receipt Label",
		"Label for reprint receipt print.",
		vmgPrinting, "");
		SetVarInt(DBTransaction,vmMallIndex, "Mall Index Key",
		"Getting the Mall Index Key from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmClassCode, "Mall Class Code",
		"Getting the Mall Class Code from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmOutletCode, "Mall Outlet Code",
		"Getting the Mall Outlet Code from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmTenantNo, "Mall Tenant Number",
		"Getting the Mall Tenant Number from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmMallPath, "Mall Export Location Path",
		"Getting the Mall Export Location Path from GUI",
		vmgPOS, 0);

        	SetVarStr(DBTransaction,vmDLFMallFileName, "DLF Mall File Name",
		"Getting the DLF Mall Export File Name from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmTradeCode, "Mall Trade Code",
		"Getting the Mall Trade Code from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmBranchCode, "Mall Branch Code",
		"Getting the Mall Branch Code from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmTerminalNo, "Mall POS Terminal Number",
		"Getting the Mall POS Terminal Number from GUI",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmSerialNo, "Mall POS Serial Number",
		"Getting the Mall POS Serial Number from GUI",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmBatchNo, "Robinson Batch No",
		"Getting the Batch number of every resports send to server",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmZCount, "SM ZCount flag",
		"Getting the ZCount value that indicates the Z-report has been performed",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmPPlantCount, "Counter for Power Plant Mall",
		"Getting the Power Plant Counter",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmPPlantPrevTime, "Check Previous Time for Power Plant Mall",
		"Getting the Power Plant Previous Time",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmPPlantHourly, "Hourly Flag for Power Plant Mall to know if there is transaction in an hour",
		"Getting the Power Plant Hourly Flag",
		vmgPOS, 0);
		SetVarInt(DBTransaction,vmPPlantFlag, "Flag for Power Plant Mall to know if the file already exists",
		"Getting the Power Plant Flag",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmFirstMallDate, "Checking Zed Dates",
		"Getting the FirstMallDate value that identify if the the first Day POS is set",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmLastTransactionDate, "Checking Last date the pos was used",
		"Getting the LastTransactionDate value that identify when is the POS last used",
		vmgPOS, 0);
		SetVarBool(DBTransaction,vmFirstMallSet, "Checking Zed Switch flag",
		"Getting the FirstMallSet value that identify switching of malls",
		vmgPOS, false);
		SetVarInt(DBTransaction,vmCMFileSerialNumber, "Serial Number for CapitaLand Mall file name count",
		"Serial Number Count",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmFTPServer, "FTP Server Name",
		"FTP Server Name used to send export file thru FTP",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmFTPPath, "FTP Path Name",
		"FTP Path Name used to locate where the file will be placed in FTP Server",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmFTPUserName, "FTP User Name",
		"FTP User Name used to identify the user name required by the FTP Server",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmFTPPassword, "FTP User Password",
		"FTP User Password is used to identify the accessibility of the user for FTP Server",
		vmgPOS, 0);
		SetVarStr(DBTransaction,vmConsolidateReportPaths, "Consolidated DB Paths",
		"Consilidated Database path to be use to consilidate sale from different database",
		vmgPOS, 0);
		SetVarBool(DBTransaction,vmEnableConsolidatedReport, "Checking the status of Consolidated Report",
		"Enabling or Disabling the Consolidated Report in Zed Report",
		vmgPOS, false);

		// Accumulated ZED
		SetVarBool(DBTransaction, vmShowAccumulatedZed, "Show Accumulated Zeds",
		"Specifies whether or not to show the accumulated total for the site "
		"when closing the til.",
		vmgPOS, false);
		// Tax Summary
		SetVarBool(DBTransaction, vmShowTaxSummary, "Show Tax Summary",
		"Specifies whether or not to show the tax summary for the site "
		"when closing the til.",
		vmgPOS, false);
		// Service Charge Summary
		SetVarBool(DBTransaction, vmShowServiceChargeSummary, "Show Service Charge Summary",
		"Specifies whether or not to show the service charge summary for the site "
		"when closing the til.",
		vmgPOS, false);

		//This will be picking the value from exisitng setting..
		//vmShowServiceChargeTax
		SetVarBool(DBTransaction, vmCombineServiceChargeTaxWithServiceCharge, "Combine Service Charge Tax with Service Charge",
		"Specifies whether or not to show the service charge tax with the service charge "
		"when closing the til.",
		vmgPOS, false);

		SetVarBool(DBTransaction, vmUpdateTableGUIOnOrderStatus, "Update Table GUI Based On Order Status",
		"Update Table GUI Based On Order Status "
		"Default is False.",
		vmgPOS, false);

		SetVarBool(DBTransaction, vmCombineServiceChargeTaxWithSalesTax, "Combine Service Charge Tax with Sales Tax",
		"Specifies whether or not to show the service charge tax combined with Sales Tax "
		"when closing the til.",
		vmgPOS, false);
		// Enable Client Details
		SetVarBool(DBTransaction, vmEnableClientDetails, "Enable Client Details",
		"Specifies whether or not to show the client details for the site "
		"when closing the til.",
		vmgPOS, false);


		SetVarBool(DBTransaction, vmShowCurrencySymbol, "Show Currency Symbol in POS",
		"Show Currency Symbol in POS "
		"Default is False.",
		vmgPOS, false);

		SetVarBool(DBTransaction, vmSkipConfirmationOnFastTender, "Skip confirmation on Fast Tender Shortcuts.",
		"Skip confirmation on Fast Tender Shortcuts"
		"Default is False.",
		vmgPOS, false);

		SetVarBool(DBTransaction, vmTransferTableOnPrintPrelim, "Transfer Table On Print Prelim To Tab.",
		"Transfer Table On Print Prelim To Tab"
		"Default is False.",
		vmgPOS, false);

		// Disable Sales Tax when the value is 0
		/*SetVarBool(DBTransaction, vmHideSalesTaxWhenValueIsZero, "Disable Sales tax when value is zero",
					"Specifies whether or not to show the sales tax even the value is zero the site ",
					vmgPOS, false);*/
		SetVarStr(DBTransaction,vmLastZedDate, "Checking Last date the zed was performed",
		"Getting the LastZedDate value that identify when is the Zed was last performed",
		vmgPOS, 0);

		SetVarStr(DBTransaction,vmLastZedTime, "Checking Last time the zed was performed",
		"Getting the LastZedTime value that identify when is the Zed was last performed",
		vmgPOS, 0);


		SetVarStr(DBTransaction, vmBarExchangePath,"Bar Exchange Path",
		"BarExchange Path Directory.\r"
		"Default is"+ ExtractFilePath( Application->ExeName ) + "Stock Exchange",
		vmgPOS, ExtractFilePath( Application->ExeName ) + "Stock Exchange" );

		SetVarStr(DBTransaction, vmBarExchangeSelect,"select None; Barstock; or Exchange ",
		"Getting that last selected setting that one of them in None, Barstock or Exchange \r"
		"Default is  None",
		vmgPOS, "None" );
		SetVarBool(DBTransaction,
		vmBarExchangeEnabled,
		"Enable Bar Exchange",
		"This setting will enable or disable Bar Exchange ",
		vmgPOS,
		false);
		SetVarBool(DBTransaction,
		vmBarExchangeServerEnabled,
		"Check BarExchange Server Setting",
		"This setting will enable or disable BarExchange Server if no any server Selected ",
		vmgPOS,
		false);
        //MM-4579
         SetVarBool(DBTransaction,
                   vmPointPurchased,
                   "Check Reset Member Point by Point Purchased Setting",
                   "This setting will enable or disable Member Reset Point by Point Purchased ",
                   vmgPOS,
                   false);
         //MM-4579
         SetVarBool(DBTransaction,
                   vmPointRedeemed,
                   "Check Reset Member Point by Point Redeem Setting",
                   "This setting will enable or disable Member Reset Point by Point Purchased ",
                   vmgPOS,
                   false);

         //MM-4579
         SetVarBool(DBTransaction,
                   vmPointEarned,
                   "Check Reset Member Point by Point Earned Setting",
                   "This setting will enable or disable Member Earned Point by Point Purchased ",
                   vmgPOS,
                   false);
        //MM-4579
        SetVarInt(DBTransaction,
                    vmResetDay, "How many days before point resets",
                     "This setting shows how many days a member points stored in ths system",
                     vmgPOS, 1);
		SetVarBool(DBTransaction,
                    vmIsDrinkCommandEnabled,
                    "Enable Drink Command",
                    "This setting will enable or disable Drink Command.",
                    vmgPOS,
                    false);
		SetVarStr(DBTransaction,
                    vmDrinkCommandServerPath,
                    "Drink Command Server Path",
                    "Default path is \r localhost",
                    vmgPOS,
                    "localhost");
		SetVarInt(DBTransaction,
                vmDrinkCommandServerPort,
                "Drink Command Server Port",
                "Default port is \r 15090",
                vmgPOS,
                15090);


		// MM4161
		SetVarStr(DBTransaction,vmMaximumChitNumber, "Setting the Maximum Chit Number",
		"This is used to save the settings of Maximum Chit Number",
		vmgPOS, 99);

		SetVarStr(DBTransaction,vmAmountValue, "Holding a amount value variable",
		"Getting the Amount value that stores any value for an amount",
		vmgPOS, 0);


           SetVarBool(DBTransaction, vmEmailZedReport, "Email Zed When Closing Till",
                 "Email Zed When Closing Till "
                 "Default is False.",
                 vmgPOS, false);


        SetVarStr(DBTransaction,vmSaveEmailId, "Save Email Id",
                                 "Saved Email id is used to send email id",
                                 vmgPOS, 0);


        SetVarStr(DBTransaction,vmEmailId, "Email Id",
                                 "Email id is used to send emails",
                                 vmgPOS, 0);


        SetVarStr(DBTransaction,vmEmailPassword, "Email Password",
                                 "Email Password is used to send email id",
                                 vmgPOS, 0);


        SetVarStr(DBTransaction,vmEmailHost, "Email Host",
                                 "Email host is used to send emails",
                                 vmgPOS, 0);

        SetVarInt(DBTransaction,vmEmailHostPort, "Email Host Port",
                 "Email host Port is used to send emails",
                 vmgPOS, 0);

		        SetVarNum(DBTransaction,vmRoundOnBillingAmount, "Rounding Amount On Billing",
                  "Rounding Amount On Billing",
                  vmgPOS,
                  0.01);
        SetVarBool(DBTransaction, vmRoundOnBilling, "Rounding On Billing",
                 "Rounding Amount On Billing."
                 "Default is False.",
                 vmgPOS, false);

        SetVarBool(DBTransaction, vmShowRemovalReport, "Show Removal Reports",
         "Show Cancel Removal Reports"
         "Default is true.",
         vmgPOS, true);

        SetVarBool(DBTransaction, vmEnableCancelCheckRemoval, "Setting for Enable Cancel",
         "Setting for Enable Cancel"
         "Default is False.",
         vmgPOS, false);

         SetVarBool(DBTransaction, vmRevenueFiguresAreDiscountInclusive,
         "Setting for deducing discount or surcharge",
         "If true then deduce the discount or surchrge from above three sections"
         "Default is True.",

         vmgPrinting, true);

        SetVarBool(DBTransaction,vmShowPriceAdjustment, "Show Price Adjustment on Zed",
					 "This Setting shows price adjusted by different users.\r"
					 "Default is True",
					 vmgPrinting, true);

		SetVarBool(DBTransaction,vmEnableApprovedLogin, "Enable Approved Login",
					 "Enable Approved Login to filter name.\r"
					 "Default is False",
					 vmgSecurity, false);

		SetVarBool(DBTransaction,vmClockInOutApprovedLogin, "Clock In Out Approved Login",
					 "Clock In Out Approved Login to filter name.\r"
					 "Default is False",
					 vmgSecurity, false);
        //MM-4775
       SetVarBool(DBTransaction,vmPointOnly, "Point only Transaction.",
					 "Point Only Transaction.\r"
					 "Default is False",
					 vmgPOS, false);
       SetVarBool(DBTransaction,vmPromptPatronCountOnTableSales, "Prompt User For Patron Count",
					 "Prompts the user to enter the patron count for Table sales only.\r"
					 "Default is false",
					 vmgPOS, false);
       SetVarBool(DBTransaction,vmPostZToAccountingSystem, "Post Z To Accounting System",
					 "Post Z To Accounting System.\r"
					 "Default is false",
					 vmgPOS, false);
        //MM 5610
        SetVarBool(DBTransaction,vmConfirmResetPoint, "Confirm on Reset Point",
					 "User need to confirm to reset the points.\r"
					 "Default is False",
					 vmgPOS, false);
		SetVarBool(DBTransaction,vmEnableEftPosSmartPay, "Use SmartPay EFTPOS",
					 "Till uses SmartPay Intergrated EFTPOS.\r"
					 "Default is False",
					 vmg3rdPartyInterface, false);
       SetVarStr(DBTransaction,vmEftPosSmartPayIp, "SmartPay EFTPOS Ip Address",
					 "SmartPay EFTPOS Ip Address.\r"
					 "Default is 127.0.0.1",
					 vmg3rdPartyInterface, "127.0.0.1");
        SetVarStr( DBTransaction, vmPointsPurchasedGLCode, "Points Purchased GLCode",
                    "Points Purchased GLCode."
                    "Default is 300",
                    vmg3rdPartyInterface, "300" );
        SetVarStr( DBTransaction, vmPointsSpentGLCode, "Points Spent GLCode",
                    "Points Spent GLCode."
                    "Default is 301",
                    vmg3rdPartyInterface, "301" );
        SetVarStr( DBTransaction, vmVoucherPurchasedGLCode, "Voucher Purchased GLCode",
                    "Voucher Purchased GLCode.\r"
                    "Default is 302",
                    vmg3rdPartyInterface, "302" );
        SetVarStr( DBTransaction, vmServiceChargeGLCode, "Service Charge GLCode",
                    "Service Charge GLCode.\r"
                    "Default is 400",
                    vmg3rdPartyInterface, "400" );
        SetVarStr( DBTransaction, vmServiceChargeTaxGLCode, "Service Charge Tax GLCode",
                    "Service Charge Tax GLCode.\r"
                    "Default is 401",
                    vmg3rdPartyInterface, "401" );
        SetVarStr( DBTransaction, vmRoundingGLCode, "Rounding GLCode",
                    "Rounding GLCode.\r"
                    "Default is 860",
                    vmg3rdPartyInterface, "860" );

       SetVarInt(DBTransaction, vmNewBook,"select Default; NewBook;   ",
			        "Getting that last selected setting that one of them in None, NewBook \r"
			        "Default is  Default",
			        vmgPOS, 0 );
       SetVarBool(DBTransaction, vmGiftCardOnly,"Gift Card Only.",
			        "If Setting is on then all new member will be giftcard \r"
			        "Default is  false",
			        vmgPOS, false );
       SetVarBool(DBTransaction,vmDeleteItemSizeAfterSale, "Delete ItemSize After Sale",
					 "Deleting Item Size after Sale.\r"
					 "Default is False",
					 vmgPOS, false);


      SetVarBool(DBTransaction,vmIsMemberSalesOnlyEnabled, "Enable MemberShipOnly",
					"Enable Member sales only for Run Rate Board.\r"
            	 	"Default is false\r",
					 vmgMiscEquipment, false);

      SetVarBool(DBTransaction,vmIsClippIntegrationEnabled, "Enable Clipping",
					"Enable Clipping for accessing Pos through app.\r"
            	 	"Default is false\r",
					 vmg3rdPartyInterface, false);
      SetVarInt(DBTransaction,vmClippTabStartIndex, "Tab Number",
					 "Tab number will start from.\r"
					 "Default is 800.",
					 vmg3rdPartyInterface, 800);
      SetVarInt(DBTransaction,vmClippTabEndIndex, "Tab Number",
					 "Tab number will end at.\r"
					 "Default is 990.",
					 vmg3rdPartyInterface, 990);
      SetVarBool(DBTransaction, vmShowItemSizesAsList, "Show Item Sizes As List in POS",
                     "Show Item Size As List in POS "
                     "Default is False.",
                      vmgPOS, false);
      SetVarBool(DBTransaction, vmShowItemPriceInTransferScreen, "Show Item Sizes In Transfer Screen",
                     "Show Item Price in Transfer Screen "
                     "Default is False.",
                      vmgPOS, false);
     SetVarInt(DBTransaction,vmNoOfPriceLevels, "No Of Price Levels.",
		"No Of Price Levels for Happy Hour.\r"
		"Default is 2.",
		vmgHappyHour, 2);
    SetVarBool(DBTransaction, vmTerminalExemptFromHappyHour, "Termainal is Exempt From Happy Hours",
                     "Happy Hour will not be applicable to this terminal "
                     "Default is False.",
                      vmgPOS, false);

//MM-6148
		SetVarBool(DBTransaction,
                    vmIsThorlinkEnabled,
                    "Enable Thorlink",
                    "This setting will enable or disable Thorlink.",
                    vmgPOS,
                    false);
		SetVarStr(DBTransaction,
                    vmThorlinkAppKey,
                    "Thorlink App Key",
                    "This Setting will accept Thorlink AppKey",
                    vmgPOS,
                    "AppKey");
		SetVarStr(DBTransaction,
                    vmThorlinkSiteNo,
                    "Thorlink Site Number",
                    "This Setting will accept Thorlink SiteNo",
                    vmgPOS,
                    "SiteNo");
		SetVarStr(DBTransaction,
                    vmThorlinkMerchantCode,
                    "Thorlink Merchant Code",
                    "This Setting will accept Thorlink MerchantCode",
                    vmgPOS,
                    "Merchant Code");
		SetVarStr(DBTransaction,
                    vmThorlinkDeviceCode,
                    "Thorlink Device Code",
                    "This Setting will accept Thorlink DeviceCode",
                    vmgPOS,
                    "Device Code");
      SetVarBool(DBTransaction, vmAwaitingPaymentInXero, "Account charges go to awaiting payment in Xero",
                     "Account charges go to awaiting payment in Xero not in draft "
                     "Default is False.",
                      vmg3rdPartyInterface, false);
      SetVarBool(DBTransaction,vmPostMoneyToGlAccounts, "Money posts to GL Accounts",
					 "Money posts to GL Accounts.\r"
					 "Default is false",
					 vmgPOS, false);
      SetVarBool(DBTransaction,vmPostMoneyAsPayment, "Money posts as payments against Daily Summary invoice",
					 "Money posts as payments against Daily Summary invoice.\r"
					 "Default is false",
					 vmgPOS, false);
      
      SetVarStr( DBTransaction, vmTabDepositCreditReceivedGLCode, "Tab deposit/credit received GL Code",
                    "Tab deposit/credit received GL Code.\r"
                    "Default is 200",
                    vmg3rdPartyInterface, "200" );
      SetVarStr( DBTransaction, vmTabDepositCreditRefundedGLCode, "Tab deposit/credit refunded GL Code",
                    "Tab deposit/credit refunded GL Code.\r"
                    "Default is 200",
                    vmg3rdPartyInterface, "200" );
     SetVarStr( DBTransaction, vmFloatGLCode, "Float GL Code",
                    "Float GL Code.\r"
                    "Default is blank",
                    vmg3rdPartyInterface, "" );
            SetVarBool(
            DBTransaction,
            vmSetTextFontSize,
            "Allow to Set Font Size 9.5mm",
            "If this is ticked font size of second display is 9.5mm.",
            vmgMisc,
            false);

		SetVarStr(DBTransaction,vmSetCustomerDisplayFontSize, "Display Customer Display Font Size",
		"This Number of Menu Items Across the Screen.\r"
		"Default is 0.",
		vmgMisc, "");
        SetVarNum(DBTransaction,vmWeightLimit, "Set Weight Limit",
                  "weight limit",
                  vmgPOS,
                  0.000);
       SetVarBool(DBTransaction, vmNotifyForManuallyEnteredWeight, "Manually entered weight",
                     "abcd"
                     "Default is False.",
                      vmgPOS, false);

      SetVarBool(DBTransaction, vmShowScreenToSelectItemForPoint, "Show another form To select item",
                     "Show another form To select item"
                     "Default is False.",
                      vmgPOS, false);
      SetVarBool(DBTransaction, vmNotifyLastWebOrder, "Show message when webmate interface is disable",
                     "Show message when webmate interface is disable "
                     "Default is False.",
                      vmgPOS, false);
      SetVarBool(DBTransaction, vmUpdateMenu, "Save variable when menu update",
                     "Save variable when menu update"
                     "Default is False.",
                      vmgPOS, false);
      SetVarBool(DBTransaction,vmPontsSpentCountedAsRevenue, "Ponts Spent Counted As Revenue",
                    "Ponts Spent Counted As Revenue.\r"
                    "Default is False",
                    vmgPOS, false);

      SetVarBool(DBTransaction,vmRunRateBoardEnabled, "Enable RunRate Board",
					"Enable Run Rate Board.\r"
            	 	"Default is false\r",
					 vmgMiscEquipment, false);
    SetVarStr(DBTransaction, vmRunRateBoardIP, "Run Rate Board IP",
                    "Run Rate url.\r"
                    "Default is localhost",
                        vmgMisc, "localhost" );

     SetVarBool(DBTransaction, vmShowLargeFonts, "Show Large Fonts",
                     "This Setting will increase font size when checked "
                     "Default is False.",
                      vmgPOS, false);

    SetVarBool(DBTransaction, vmItemSearch, "Item Search",
                     "Item Search Manually in POS"
                     "Default is False.",
                      vmgMiscEquipment, false);
    SetVarBool(DBTransaction, vmIsCloudSynced, "Is Cloud Synced For Discounts",
                     "If this setting id disabled all discouint will be deleted"
                     "Default is False.",
                      vmgPOS, false);


   SetVarBool(DBTransaction, vmShowDarkBackgroundInPOS, "Dark Background in POS",
                 "Show Dark background in POS"
                 "Default is False.",
                  vmgPOS, false);

	SetVarStr( DBTransaction, vmMYOBMachineName, "MYOB Integration / Machine Name",
                "Xero Integration / Machine Name.\r"
                "Default is localhost",
                vmg3rdPartyInterface, "localhost" );

	SetVarStr( DBTransaction, vmMYOBUserName, "MYOB Integration / Username",
                "Xero Integration / Username.\r"
                "Default is empty string",
                vmg3rdPartyInterface, "" );

	SetVarStr( DBTransaction, vmMYOBPassword, "MYOB Integration / Password",
                "MYOB Integration / Password.\r"
                "Default is empty string",
                vmg3rdPartyInterface, "" );

	SetVarStr(DBTransaction, vmMYOBFolderPath, "MYOB Integration / Folder URL",
                "MYOB Integration / Folder URL.\r"
                "Default is C:\\Program Files\\Menumate\\" + MYOB_INTEGRATION_FOLDER,
                vmg3rdPartyInterface, "C:\\Program Files\\Menumate\\" + MYOB_INTEGRATION_FOLDER);

		SetVarBool(DBTransaction,
                    vmIsXeroEnabled,
                    "Enable Xero",
                    "This setting will enable or disable Xero.",
                    vmgPOS,
                    false);

		SetVarBool(DBTransaction,
                    vmIsMYOBEnabled,
                    "Enable MYOB",
                    "This setting will enable or disable MYOB.",
                    vmgPOS,
                    false);
     SetVarBool(DBTransaction, vmOpenCashDrawer, "Open Cash Drawer",
                 "Open Cash Drawer after Zed"
                 "Default is False.",
                  vmgPOS, false);
       SetVarBool(DBTransaction, vmMergeSimilarItem, "Merge Similar item",
                 "Merge Similar item"
                 "Default is True.",
                  vmgPOS, true);
        SetVarBool(DBTransaction,vmCaptureRefundRefNo, "Capture Refund Ref. No",
                "Capture Refund Ref. No.\r"
                "Default is false",
                vmgPrinting, false);
		SetVarBool(DBTransaction,vmHideTaxInvoice, "Hide Tax Invoice",
                "Hide Tax Invoice.\r"
                "Default is false",
                vmgPrinting, false);
		SetVarStr(DBTransaction,vmReceiptDigits, "Digits for Receipts",
                "Digits for Receipts.\r"
                "Default is ",
                vmgPrinting, "0");
		SetVarBool(DBTransaction,vmExportReprintReceipt, "Export Reprint Receipt",
                "Export Reprint Receipt.\r"
                "Default is false",
                vmgPrinting, false);
		SetVarBool(DBTransaction,vmSetVoidFooter, "Set For Void Footer",
                "Set For Void Footer"
                "Default is false",
                vmgPrinting, false);
		SetVarBool(DBTransaction,vmSetSubHeader, "Set For Sub Header",
                "Set For Sub Header"
                "Default is false",
                vmgPrinting, false);
		SetVarBool(DBTransaction,vmShowVoidOrRefund, "Show Void or Refund ",
                "Show Void or Refund"
                "Default is false",
                vmgPrinting, false);
 		SetVarBool(DBTransaction,vmShowVoidNumber, "Show Void or Refund number",
                "Show Void or Refund number"
                "Default is false",
                vmgPrinting, false);
        SetVarStr( DBTransaction, vmEftPosTipGLCode, "EftPos Tip GLCode",
                "EftPos Tip GLCode."
                "Default is 860",
                vmg3rdPartyInterface, "860" );
		SetVarStr(DBTransaction,vmRefundReferenceLabel,"Refund Reference Label",
                "Label for Refund reference in receipt print.",
                vmgPrinting, "OR No.");
        SetVarBool(DBTransaction, vmHideRoundingOnReceipt, "Hide Rounding On Receipt",
                 "Rounding will be disable on receipt if setting is true"
                 "Default is False.",
                  vmgPOS, false);
 		SetVarBool(DBTransaction,vmExcludeReceipt, "Show Void or Refund number",
		"Show Void or Refund number"
		"Default is false",
		vmgPrinting, false);
 		SetVarBool(DBTransaction,vmExcludeXReport, "Show Void or Refund number",
		"Show Void or Refund number"
		"Default is false",
		vmgPrinting, false);
        SetVarBool(DBTransaction,vmUseBIRFormatInXZReport, "Use BIR Format in X and Z Report ",
             "Default is false",
              vmgPrinting, false);
		SetVarBool(DBTransaction,vmCashDenominationEntry, "Enable Cash Denomination",
		"Default is False",
		vmgPOS, false);
       SetVarInt(DBTransaction,
           vmGiftCardValidation,
           "Gift Card Validation",
           "Gift Card Validation Strategy.",
           vmgPOS,
           0);

        SetVarBool(DBTransaction, vmIsEnabledPeachTree, "Enable CSV Export",
                 "Enable/Disable CSV Export"
                 "Default is False.",
                  vmg3rdPartyInterface, false);

	    SetVarStr( DBTransaction, vmCSVPath, "CSV Export Path",
                "Path where CSV will be export.\r"
                "Default is RunTime Folder",
                vmg3rdPartyInterface, "" );
        SetVarStr(DBTransaction, vmCSVExportIP, "CSV Export IP",
                    "CSV Export url.\r"
                    "Default is localhost",
                        vmg3rdPartyInterface, "localhost" );
		SetVarBool(DBTransaction,vmMembershipPaid, "Enable Membership Payment",
		"Default is False",
		vmgPOS, false);
		SetVarBool(DBTransaction,vmUseMemberSubs, "Enable Membership Subscription",
		"Default is False",
		vmgPOS, false);
        SetVarBool(DBTransaction, vmFloatWithdrawFromCash, "Float Withdraw From Cash",
                 "If this setting is on float withdraw will happen from cash"
                 "Default is False.",
                  vmgPOS, false);
        SetVarStr( DBTransaction, vmCashWithdrawal, "Cash Withdrawal GLCode",
                "Cash Withdrawal GLCode."
                "Default is null",
                vmg3rdPartyInterface, "" );
        SetVarBool(DBTransaction,vmIsPanasonicIntegrationEnabled, "Enable Panasonic",
						"Enable Panasonic Integration for posting receipt on Server.\r"
						"Default is false\r",
						 vmg3rdPartyInterface, false);

		SetVarStr(DBTransaction,vmPanasonicServerIP, "Enter Server IP",
					"Enter IP Address for Server.\r"
            	 	"Default is localhost\r",
					 vmg3rdPartyInterface, "localhost");
        SetVarStr( DBTransaction, vmCashVariance, "Cash Variance GLCode",
                "Cash Variance GLCode."
                "Default is 6-3400",
                vmg3rdPartyInterface, "6-3400" );
        SetVarStr(DBTransaction, vmReportExportPath, "Report Export Path",
		            "Report Export Path Where Report Will be Exported",
		            vmgPOS, "");
		SetVarInt(DBTransaction,vmPMSType, "PMS Type",
		"PMS Type.\r"
		"Default is PMS Type none.",
		vmg3rdPartyInterface, 0);
		SetVarStr(DBTransaction,vmPMSTipAccount, "PMS Default Tip Account",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is 2",
		vmg3rdPartyInterface, "2");
		SetVarStr(DBTransaction,vmPMSExpensesAccount, "PMS Default Expenses Account",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is 32",
		vmg3rdPartyInterface, "32");
		SetVarStr(DBTransaction,vmPMSServiceChargeAccount, "PMS Default Service Charge Account",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is 32",
		vmg3rdPartyInterface, "32");
		SetVarStr(DBTransaction,vmPMSRoundingAccountSiHot, "PMS Default Rounding Account SiHot",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is ",
		vmg3rdPartyInterface, "");
		SetVarStr(DBTransaction,vmSiHotRounding, "PMS Default Rounding Account SiHot",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is ",
		vmg3rdPartyInterface, "");
		SetVarStr(DBTransaction,vmSiHotDefaultTransaction, "PMS Default Rounding Account SiHot",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is ",
		vmg3rdPartyInterface, "");
		SetVarStr(DBTransaction,vmSiHotDefaultTransactionName, "PMS Default Rounding Account Name SiHot",
		"The Category Menu Items will add against in the PMS system/r"
		"Default is ",
		vmg3rdPartyInterface, "");
		SetVarStr(DBTransaction,vmRevenueCentre, "PMS Revenue Centre",
		"PMS Revenue Centre./r"
		"Default is 0",
		vmg3rdPartyInterface, "0");
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


void __fastcall TManagerVariable::SetEnabled(bool value)
{
	if (fEnabled != value)
	{
		fEnabled = value;
	}
}

bool __fastcall TManagerVariable::GetEnabled()
{
	return fEnabled;
}

//------------------------------------------------------------------------------
//	Profiles.
//------------------------------------------------------------------------------
int TManagerVariable::GetProfile(Database::TDBTransaction &Transaction,ProfileType Type,UnicodeString ProfileName)
{
	int RetVal = 0;
	if(!GetProfileCache(Type,ProfileName,RetVal))
	{
		RetVal = GetProfileDB(Transaction,Type,ProfileName);
	}
	return RetVal;
}

int TManagerVariable::GetProfileDB(Database::TDBTransaction &Transaction,ProfileType Type,UnicodeString ProfileName)
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

int TManagerVariable::SetProfile(Database::TDBTransaction &Transaction,ProfileType Type,UnicodeString ProfileName)
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

bool TManagerVariable::SetProfile(Database::TDBTransaction &Transaction,int ProfileKey,ProfileType Type,UnicodeString ProfileName)
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


bool TManagerVariable::DelProfile(Database::TDBTransaction &Transaction,ProfileType Type,UnicodeString ProfileName)
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


bool TManagerVariable::GetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,UnicodeString &Value)
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

bool TManagerVariable::GetProfileStrDB(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,UnicodeString &Value)
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

void TManagerVariable::SetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey,UnicodeString Value)
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
		//		CacheGetProfileInt[ProfileKey][VariablesKey] = Value;
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
	//   CacheGetProfileInt[ProfileKey][VariablesKey] = Value;
}

void TManagerVariable::SetProfileInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,int Value)
{
	if( !fEnabled || DeviceProfileKey == 0) return;
	bool Create = false;

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	Transaction.RegisterQuery(IBInternalQuery);

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"VARSPROFILE "
	"SET "
	"INTEGER_VAL = :INTEGER_VAL, "
	"PROFILE_KEY = :PROFILE_KEY "
	"WHERE "
	"VARIABLES_KEY = :VARIABLES_KEY";
	IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = VariablesKey;
	IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = DeviceProfileKey;
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
		IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = DeviceProfileKey;
		IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = Value;
		IBInternalQuery->ExecQuery();
	}
	CacheGetProfileInt[DeviceProfileKey][VariablesKey] = Value;
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
	if(GetProfileTimeCache(ProfileKey,VariablesKey,Value))
	{
		RetVal = true;
	}
	else
	{
		RetVal = GetProfileTimeDB(Transaction,ProfileKey,VariablesKey,Value);
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

void TManagerVariable::SetVarBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Name,UnicodeString Discription,vmGroup Group,bool Value)
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

bool TManagerVariable::GetProfileCache(ProfileType Type,UnicodeString ProfileName,int &Value)
{
	bool Retval = false;
	std::map< ProfileType ,std::map<UnicodeString,int> >::iterator ptrVariables = CacheGetProfile.find(Type);
	if(ptrVariables != CacheGetProfile.end())
	{
		std::map<UnicodeString,int>::iterator ptrVar = ptrVariables->second.find(ProfileName);
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

bool TManagerVariable::GetProfileStrCache(int ProfileKey, vmVariables VariablesKey, UnicodeString &Value)
{
	bool Retval = false;
	std::map< int , std::map<vmVariables,UnicodeString> >::iterator ptrVariables = CacheGetProfileStr.find(ProfileKey);
	if(ptrVariables != CacheGetProfileStr.end())
	{
		std::map<vmVariables,UnicodeString>::iterator ptrVar = ptrVariables->second.find(VariablesKey);
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


