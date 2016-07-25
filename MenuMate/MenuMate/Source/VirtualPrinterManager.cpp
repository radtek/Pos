//---------------------------------------------------------------------------
#pragma hdrstop

#include "VirtualPrinterManager.h"
#include "MMLogging.h"
#include "ManagerPhysicalPrinter.h"
#include "ItemSizeCategory.h"
#include <Memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerVirtualPrinter::TManagerVirtualPrinter()
{
}

TManagerVirtualPrinter::~TManagerVirtualPrinter()
{
}

void TManagerVirtualPrinter::InitialisePrinters(Database::TDBTransaction &DBTransaction, int inDeviceKey)
{
   // ReloadPrinters(DBTransaction);
   CreateWindowsConnection(DBTransaction,inDeviceKey);
}

/*void TManagerVirtualPrinter::ReloadPrinters(Database::TDBTransaction &DBTransaction)
{
	VirtualPrinters.clear();
	DBLoadPrinters(DBTransaction);
}*/

void TManagerVirtualPrinter::CreateWindowsConnection(Database::TDBTransaction &DBTransaction,int inDeviceKey)
{
	//Create any Windows printers as necessary.
	std::auto_ptr<TManagerPhysicalPrinter> ManagerPhysicalPrinter(new TManagerPhysicalPrinter);
	std::vector<TPrinterVirtual> VirtualPrinters;

	DBLoadPrinters(DBTransaction,VirtualPrinters);

	for (std::vector<TPrinterVirtual>::iterator ptrVirtualPrinters = VirtualPrinters.begin();
		  ptrVirtualPrinters != VirtualPrinters.end();
		  ptrVirtualPrinters++)
	{
		TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter->GetPhysicalPrinter(DBTransaction,ptrVirtualPrinters->PhysicalPrinterKey);

		if (PrinterPhysical.Type == ptWindows_Printer)
		{
			HANDLE hPrinter = NULL;
			bool PrinterExists = OpenPrinter(PrinterPhysical.UNCName().t_str(), &hPrinter, NULL);
			if (!PrinterExists)
			{
				PrinterExists = AddPrinterConnection(PrinterPhysical.UNCName().t_str());
			}

			if (hPrinter)
			{
				ClosePrinter(hPrinter);
			}
		}
	}
}

int TManagerVirtualPrinter::DBAddPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual inPrinter)
{

	int VPrinterKey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		VPrinterKey = DBCheckVirtualPrinter(DBTransaction,inPrinter);

		if(VPrinterKey == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VIRTUALPRINTER, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			VPrinterKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO VIRTUALPRINTER ("
					"VIRTUALPRINTER_KEY,"
					"PHYSICALPRINTER_KEY,"
					"NAME,"
					"PROFILE_KEY) "
				"VALUES ("
					":VIRTUALPRINTER_KEY,"
					":PHYSICALPRINTER_KEY,"
					":NAME,"
					":PROFILE_KEY);";
			IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = VPrinterKey;
			IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = inPrinter.PhysicalPrinterKey;
			IBInternalQuery->ParamByName("NAME")->AsString =  inPrinter.VirtualPrinterName;
			IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = 0;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return VPrinterKey;
}

void TManagerVirtualPrinter::DBRenamePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey,UnicodeString Name)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		//Check a Virtual Printer with that name dosnt exist.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VIRTUALPRINTER "
		"SET "
			"NAME = :NAME, "
			"PROFILE_KEY = :PROFILE_KEY "
		"WHERE "
			"VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterKey;
		IBInternalQuery->ParamByName("NAME")->AsString = Name;
		IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = 0;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerVirtualPrinter::DBReAssignPrinter(Database::TDBTransaction &DBTransaction,int PrinterKey,int PhysicalPrinterKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		//Check a Virtual Printer with that name dosnt exist.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"VIRTUALPRINTER "
		"SET "
			"PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY "
		"WHERE "
			"VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterKey;
		IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = PhysicalPrinterKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerVirtualPrinter::DBDeletePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey)
{
	
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"DELETE FROM VIRTUALPRINTER "
         "WHERE "
				"VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterKey;
		IBInternalQuery->ExecQuery();

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;		
	}
}


int TManagerVirtualPrinter::DBCheckVirtualPrinterByName(Database::TDBTransaction &DBTransaction,UnicodeString Name)
{
	int Retval = 0;
	
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER_KEY "
		" FROM "
		"  VIRTUALPRINTER "
		" WHERE "
		"  NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = Name;
		IBInternalQuery->ExecQuery();
		Retval = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
		
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;		
	}
	return Retval;
}

int TManagerVirtualPrinter::GetVirtualPrintersDevice(Database::TDBTransaction &DBTransaction,int PrinterKey)
{
	int Retval = 0;
	
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" DEVICE_KEY "
		" FROM "
		"  VIRTUALPRINTER "
		" WHERE "
		"  VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY";
		IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = PrinterKey;
		IBInternalQuery->ExecQuery();
		Retval = IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Retval;
}


void TManagerVirtualPrinter::GetVirtualPrinterList(Database::TDBTransaction &DBTransaction,TStrings * List)
{
	
	List->Clear();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER_KEY,NAME "
		" FROM "
		"  VIRTUALPRINTER "
		" ORDER BY "
		"  NAME";
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			List->AddObject(IBInternalQuery->FieldByName("NAME")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger);

		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TManagerVirtualPrinter::DBCheckVirtualPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual inPrinterVirtual)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER_KEY "
		" FROM "
		"  VIRTUALPRINTER "
		"  WHERE "
		"  NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = inPrinterVirtual.VirtualPrinterName;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;		
	}
	return RetVal;
}

void TManagerVirtualPrinter::DBLoadPrinters(Database::TDBTransaction &DBTransaction,std::vector<TPrinterVirtual> &VirtualPrinters)
{
	
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER.VIRTUALPRINTER_KEY,VIRTUALPRINTER.NAME,VIRTUALPRINTER.PHYSICALPRINTER_KEY "
		" FROM VIRTUALPRINTER "
		" ORDER BY "
		"  VIRTUALPRINTER.NAME";
		IBInternalQuery->ExecQuery();
		
		for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			TPrinterVirtual PrinterVirtual;
			PrinterVirtual.VirtualPrinterKey = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
			PrinterVirtual.VirtualPrinterName = IBInternalQuery->FieldByName("NAME")->AsString;
			PrinterVirtual.PhysicalPrinterKey = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
			VirtualPrinters.push_back(PrinterVirtual);
		}
		
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;		
	}
}

TPrinterVirtual * TManagerVirtualPrinter::GetVirtualPrinterByKey(Database::TDBTransaction &DBTransaction,int VirtualPrinterKey)
{

    TPrinterVirtual *RetVal = NULL;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER.VIRTUALPRINTER_KEY,VIRTUALPRINTER.NAME,VIRTUALPRINTER.PHYSICALPRINTER_KEY "
		" FROM VIRTUALPRINTER "
        " WHERE VIRTUALPRINTER.VIRTUALPRINTER_KEY = :VIRTUALPRINTER_KEY "
		" ORDER BY "
		"  VIRTUALPRINTER.NAME";
		IBInternalQuery->ParamByName("VIRTUALPRINTER_KEY")->AsInteger = VirtualPrinterKey;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
            RetVal = new TPrinterVirtual();
			RetVal->VirtualPrinterKey = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
			RetVal->VirtualPrinterName = IBInternalQuery->FieldByName("NAME")->AsString;
			RetVal->PhysicalPrinterKey = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return RetVal;
}

TPrinterVirtual * TManagerVirtualPrinter::GetVirtualPrinterByName(Database::TDBTransaction &DBTransaction,UnicodeString VirtualPrinterName)
{
    TPrinterVirtual *RetVal = NULL;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" VIRTUALPRINTER.VIRTUALPRINTER_KEY,VIRTUALPRINTER.NAME,VIRTUALPRINTER.PHYSICALPRINTER_KEY "
		" FROM VIRTUALPRINTER "
        " WHERE VIRTUALPRINTER.NAME = :NAME "
		" ORDER BY "
		"  VIRTUALPRINTER.NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = VirtualPrinterName;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
            RetVal = new TPrinterVirtual();
			RetVal->VirtualPrinterKey = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
			RetVal->VirtualPrinterName = IBInternalQuery->FieldByName("NAME")->AsString;
			RetVal->PhysicalPrinterKey = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return RetVal;
}

void TManagerVirtualPrinter::DBLoadPrinters(Database::TDBTransaction &DBTransaction,int DeviceKey,std::vector<TPrinterVirtual> &DevicePrinters,bool isChefmatePrinter)
{
	try
	{
		DevicePrinters.clear();

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();

        if (isChefmatePrinter)
        {
        IBInternalQuery->SQL->Text =
        " SELECT "
		" VIRTUALPRINTER.VIRTUALPRINTER_KEY,VIRTUALPRINTER.NAME,VIRTUALPRINTER.PHYSICALPRINTER_KEY"
		" FROM PHYSICALPRINTER"
        " inner JOIN VIRTUALPRINTER ON VIRTUALPRINTER.PHYSICALPRINTER_KEY = PHYSICALPRINTER.PHYSICALPRINTER_KEY"
	    " WHERE PHYSICALPRINTER.PRINTER_TYPE = :PRINTER_TYPE"
        " ORDER BY "
	    " VIRTUALPRINTER.NAME " ;
          IBInternalQuery->ParamByName("PRINTER_TYPE")->AsInteger = 2;
          IBInternalQuery->ExecQuery();

      }else
         {
            	IBInternalQuery->SQL->Text =
        " SELECT "
		" VIRTUALPRINTER.VIRTUALPRINTER_KEY,VIRTUALPRINTER.NAME,VIRTUALPRINTER.PHYSICALPRINTER_KEY"
		" FROM DEVICEVIRTUALPRINTER"
		" LEFT JOIN VIRTUALPRINTER ON VIRTUALPRINTER.VIRTUALPRINTER_KEY = DEVICEVIRTUALPRINTER.VIRTUALPRINTER_KEY"
		" WHERE DEVICEVIRTUALPRINTER.DEVICE_KEY = :DEVICE_KEY"
		" ORDER BY "
		"  VIRTUALPRINTER.NAME";
        IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = DeviceKey;
        IBInternalQuery->ExecQuery();
     }


		for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			TPrinterVirtual PrinterVirtual;
			PrinterVirtual.VirtualPrinterKey = IBInternalQuery->FieldByName("VIRTUALPRINTER_KEY")->AsInteger;
			PrinterVirtual.VirtualPrinterName = IBInternalQuery->FieldByName("NAME")->AsString;
			PrinterVirtual.PhysicalPrinterKey = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
			DevicePrinters.push_back(PrinterVirtual);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;		
	}
}

/*void TManagerVirtualPrinter::AddPrinter(TPrinterVirtual PrinterVirtual)
{
	VirtualPrinters.push_back(PrinterVirtual);
} */

void TManagerVirtualPrinter::RenamePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey, UnicodeString Name)
{
	DBRenamePrinter(DBTransaction,PrinterKey,Name);
}

void TManagerVirtualPrinter::DeletePrinter(Database::TDBTransaction &DBTransaction,int inPrinterKey)
{
	DBDeletePrinter(DBTransaction,inPrinterKey);
}

TStringList* TManagerVirtualPrinter::LoadChefmatePrinter(Database::TDBTransaction &DBTransaction,TItemComplete* inOrderItem)
{
   TStringList* printers = new TStringList();
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT a.COMPUTER_NAME FROM PHYSICALPRINTER a "
        "left join VIRTUALPRINTER b on a.PHYSICALPRINTER_KEY = b.PHYSICALPRINTER_KEY "
        "left join PRNORDER c on b.VIRTUALPRINTER_KEY = c.VIRTUALPRINTER_KEY "
        "where a.PRINTER_TYPE = 2 and c.COURSE_NAME = :COURSE_NAME Group by a.COMPUTER_NAME "
        " UNION "
        "SELECT a.COMPUTER_NAME FROM PHYSICALPRINTER a "
        "left join VIRTUALPRINTER b on a.PHYSICALPRINTER_KEY = b.PHYSICALPRINTER_KEY "
        "left join PRNCAT c on b.VIRTUALPRINTER_KEY = c.VIRTUALPRINTER_KEY "
        "where a.PRINTER_TYPE = 2 and c.CATEGORY_KEY in ( ";
        IBInternalQuery->SQL->Text	=  IBInternalQuery->SQL->Text	+ IntToStr(inOrderItem->Categories->FinancialCategoryKey) ;

        for (int i=0; i < inOrderItem->Categories->Count; i++)
		{
			IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "," + IntToStr(inOrderItem->Categories->CategoryGet(i)->CategoryKey);

		}
        IBInternalQuery->SQL->Text	=	IBInternalQuery->SQL->Text + ") Group by a.COMPUTER_NAME ";
		IBInternalQuery->ParamByName("COURSE_NAME")->AsString = inOrderItem->CourseKitchenName;
  		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
		{

			printers->Add(IBInternalQuery->FieldByName("COMPUTER_NAME")->AsString);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
   return printers;
}


