// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WriteOff.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "Message.h"
#include "Comms.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TWriteOff::TWriteOff(TForm * inOwner) : Owner(inOwner), Parent_Key(NULL)
{
}

void TWriteOff::InsertToDatabase(Database::TDBTransaction &DBTransaction, TList *OrderObjects, AnsiString inStaff,
	std::vector <int> &inKeys)
{
	try
	{



		if (OrderObjects->Count > 0)
		{
			for (int i = 0; i < OrderObjects->Count; i++)
			{
				TItemComplete *Order = (TItemComplete*)OrderObjects->Items[i];
				InsertToDatabase(DBTransaction, Order, inStaff, inKeys);
				for (int j = 0; j < Order->SubOrders->Count; j++)
				{
					TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
					InsertToDatabase(DBTransaction, SubOrder, inStaff, inKeys);
				}

			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TWriteOff::InsertToDatabase(Database::TDBTransaction &DBTransaction, TItemComplete *inOrder, AnsiString inStaff,
	std::vector <int> &inKeys)
{
	try
	{
        if(inOrder->Cost < -1000000)
            inOrder->Cost = 0;

		int Key, Course_Key, Category_Key, Group_Key;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_WRITEOFF, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		Key = IBInternalQuery->Fields[0]->AsInteger;
		Parent_Key = Key;

		IBInternalQuery1->Close();
		IBInternalQuery1->SQL->Text = "SELECT COURSE_KEY FROM ITEM " "WHERE ITEM_ID = :ITEM_ID;";
		IBInternalQuery1->ParamByName("ITEM_ID")->AsInteger = inOrder->Item_ID;
		IBInternalQuery1->ExecQuery();
		if (IBInternalQuery1->RecordCount)
		{
			Course_Key = IBInternalQuery1->FieldByName("COURSE_KEY")->AsInteger;
		}

		if (inOrder->Size != "")
		{

			IBInternalQuery2->Close();
			IBInternalQuery2->SQL->Text =
				 "SELECT ITEMSIZE.CATEGORY_KEY, ARCCATEGORIES.CATEGORYGROUPS_KEY FROM ITEMSIZE " "LEFT JOIN ARCCATEGORIES ON ITEMSIZE.CATEGORY_KEY = ARCCATEGORIES.CATEGORY_KEY " "WHERE ITEM_KEY = :ITEM_KEY AND SIZE_NAME = :SIZE_NAME ;";
			IBInternalQuery2->ParamByName("ITEM_KEY")->AsInteger = inOrder->ItemKey;
			IBInternalQuery2->ParamByName("SIZE_NAME")->AsString = inOrder->Size;
			IBInternalQuery2->ExecQuery();
			if (IBInternalQuery2->RecordCount)
			{
				Category_Key = IBInternalQuery2->FieldByName("CATEGORY_KEY")->AsInteger;
				Group_Key = IBInternalQuery2->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
			}

		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO WRITEOFF (WRITEOFF_KEY, ITEM, STAFF, AMOUNT, TIME_STAMP, REASONS, CATEGORY_KEY, GROUP_KEY, SUPPLIER_KEY, MENU_KEY, COURSE_KEY, ITEM_KEY, PARENT_KEY, SIZE_NAME, TERMINALNAME, COST, QTY, THIRD_PARTY_CODE, ORDER_LOCATION, COURSE_NAME, MENU_NAME) " "VALUES (:WRITEOFF_KEY, :ITEM, :STAFF, :AMOUNT, :TIME_STAMP, :REASONS, :CATEGORY_KEY, :GROUP_KEY, :SUPPLIER_KEY, :MENU_KEY, :COURSE_KEY, :ITEM_KEY, :PARENT_KEY, :SIZE_NAME, :TERMINALNAME, :COST, :QTY, :THIRD_PARTY_CODE, :ORDER_LOCATION, :COURSE_NAME, :MENU_NAME);";
		IBInternalQuery->ParamByName("WRITEOFF_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("ITEM")->AsString = inOrder->Item; // Order->Item;
		IBInternalQuery->ParamByName("STAFF")->AsString = inStaff;
		IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = inOrder->GetPriceRRP();
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("REASONS")->AsString = Reason;
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = Category_Key;
		IBInternalQuery->ParamByName("GROUP_KEY")->AsInteger = Group_Key;
		IBInternalQuery->ParamByName("SUPPLIER_KEY")->AsInteger = NULL;
		IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = inOrder->MenuKey;
		IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = Course_Key;
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = inOrder->ItemKey;
		IBInternalQuery->ParamByName("PARENT_KEY")->AsInteger = NULL;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = inOrder->Size;
		IBInternalQuery->ParamByName("TERMINALNAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
        IBInternalQuery->ParamByName("COST")->AsCurrency = inOrder->Cost;
        IBInternalQuery->ParamByName("QTY")->AsInteger = inOrder->GetQty();
        IBInternalQuery->ParamByName("THIRD_PARTY_CODE")->AsString = inOrder->ThirdPartyCode;
        IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
        IBInternalQuery->ParamByName("COURSE_NAME")->AsString = inOrder->Course;
        IBInternalQuery->ParamByName("MENU_NAME")->AsString = inOrder->MenuName;

		IBInternalQuery->ExecQuery();

	}

	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TWriteOff::InsertToDatabase(Database::TDBTransaction &DBTransaction, TItemCompleteSub *inOrder, AnsiString inStaff,
	std::vector <int> &inKeys)
{
	try
	{
        if(inOrder->Cost < -1000000)
            inOrder->Cost = 0;

		int Key, Course_Key, Menu_Key, Category_Key, Group_Key;
		long Item_Key;


		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_WRITEOFF, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		Key = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery1->Close();
		IBInternalQuery1->SQL->Text =
			 "SELECT COURSE.MENU_KEY, COURSE.COURSE_KEY, ITEM.ITEM_KEY FROM ITEM " "LEFT JOIN COURSE ON ITEM.COURSE_KEY = COURSE.COURSE_KEY " "WHERE ITEM_ID = :ITEM_ID;";
		IBInternalQuery1->ParamByName("ITEM_ID")->AsInteger = inOrder->Item_ID;
		IBInternalQuery1->ExecQuery();
		if (IBInternalQuery1->RecordCount)
		{
			Course_Key = IBInternalQuery1->FieldByName("COURSE_KEY")->AsInteger;
			Menu_Key = IBInternalQuery1->FieldByName("MENU_KEY")->AsInteger;
			Item_Key = IBInternalQuery1->FieldByName("ITEM_KEY")->AsInteger;
		}

		if (inOrder->Size != "")
		{

			IBInternalQuery2->Close();
			IBInternalQuery2->SQL->Text =
				 "SELECT ITEMSIZE.CATEGORY_KEY, ARCCATEGORIES.CATEGORYGROUPS_KEY FROM ITEMSIZE " "LEFT JOIN ARCCATEGORIES ON ITEMSIZE.CATEGORY_KEY = ARCCATEGORIES.CATEGORY_KEY " "WHERE ITEM_ID = :ITEM_ID AND SIZE_NAME = :SIZE_NAME ;";
			IBInternalQuery2->ParamByName("ITEM_ID")->AsInteger = inOrder->Item_ID;
			IBInternalQuery2->ParamByName("SIZE_NAME")->AsString = inOrder->Size;
			IBInternalQuery2->ExecQuery();
			if (IBInternalQuery2->RecordCount)
			{
				Category_Key = IBInternalQuery2->FieldByName("CATEGORY_KEY")->AsInteger;
				Group_Key = IBInternalQuery2->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
			}

		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO WRITEOFF (WRITEOFF_KEY, ITEM, STAFF, AMOUNT, TIME_STAMP, REASONS, CATEGORY_KEY, GROUP_KEY, SUPPLIER_KEY, MENU_KEY, COURSE_KEY, ITEM_KEY, PARENT_KEY, SIZE_NAME, TERMINALNAME, COST, QTY, THIRD_PARTY_CODE, ORDER_LOCATION, COURSE_NAME, MENU_NAME) " "VALUES (:WRITEOFF_KEY, :ITEM, :STAFF, :AMOUNT, :TIME_STAMP, :REASONS, :CATEGORY_KEY, :GROUP_KEY, :SUPPLIER_KEY, :MENU_KEY, :COURSE_KEY, :ITEM_KEY, :PARENT_KEY, :SIZE_NAME, :TERMINALNAME, :COST, :QTY, :THIRD_PARTY_CODE, :ORDER_LOCATION, :COURSE_NAME, :MENU_NAME );";
		IBInternalQuery->ParamByName("WRITEOFF_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("ITEM")->AsString = inOrder->Item; // Order->Item;
		IBInternalQuery->ParamByName("STAFF")->AsString = inStaff;
		IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = inOrder->GetPriceRRP();
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("REASONS")->AsString = Reason;
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = Category_Key;
		IBInternalQuery->ParamByName("GROUP_KEY")->AsInteger = Group_Key;
		IBInternalQuery->ParamByName("SUPPLIER_KEY")->AsInteger = NULL;
		IBInternalQuery->ParamByName("MENU_KEY")->AsInteger = Menu_Key;
		IBInternalQuery->ParamByName("COURSE_KEY")->AsInteger = Course_Key;
		IBInternalQuery->ParamByName("ITEM_KEY")->AsLong = Item_Key;
		IBInternalQuery->ParamByName("PARENT_KEY")->AsInteger = Parent_Key;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = inOrder->Size;
        IBInternalQuery->ParamByName("TERMINALNAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
        IBInternalQuery->ParamByName("COST")->AsCurrency = inOrder->Cost;
        IBInternalQuery->ParamByName("QTY")->AsInteger = inOrder->GetQty();
        IBInternalQuery->ParamByName("THIRD_PARTY_CODE")->AsString = inOrder->ThirdPartyCode;
        IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
        IBInternalQuery->ParamByName("COURSE_NAME")->AsString = inOrder->Course;
        IBInternalQuery->ParamByName("MENU_NAME")->AsString = inOrder->MenuName;
		IBInternalQuery->ExecQuery();

	}

	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TWriteOff::runReason(void)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (Owner, TDeviceRealTerminal::Instance().DBControl));
	frmMessage->MessageType = eWriteOff;
	frmMessage->ShowModal();
	if (TGlobalSettings::Instance().ForcedReasons && frmMessage->TextResult == "")
	{
		do
		{
			MessageBox("You must enter a WriteOff reason", "Please Comply", MB_OK + MB_ICONINFORMATION);
			frmMessage->ShowModal();
		}
		while (frmMessage->TextResult == "");
	}
	Reason = frmMessage->TextResult;
}
