// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
#include "MM_DBCore.h"
#include "IOStream.h"
#include "Classes.hpp"
#include <memory>

// ---------------------------------------------------------------------------

#pragma argsused

int _tmain(int argc, _TCHAR* argv[])
{
	AnsiString FileName = "Results.csv";
	std::auto_ptr<TFileStream> File(new TFileStream(FileName,fmCreate));

	try
	{
		UnicodeString Host = "localhost";
		UnicodeString DB = "C:\\Program Files\\MenuMate\\Menumate.ib";
		if(argc > 1)
		{
			Host = argv[1];
			DB = argv[2];
		}
		cout << "Opening DB....." << endl;
		Database::TDBControl DBControl;
		DBControl.Init(Database::TDBSettings(Host,DB, false));
		DBControl.Connect();

		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		cout << "Querying DB....." << endl;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select " "archive.TIME_STAMP_BILLED, " "'XXXX' SummaID, " "ARCBILL.INVOICE_NUMBER, "
			 "archive.TERMINAL_NAME, " "archive.MENU_NAME, " "archive.COURSE_NAME, " "archive.ITEM_NAME, " "archive.SIZE_NAME, "
			 "archive.ORDER_LOCATION, " "archive.PRICE, " "archive.PRICE_ADJUST, " "archive.DISCOUNT, "
			 "(archive.PRICE + archive.PRICE_ADJUST + archive.DISCOUNT) Total, " "archive.COST, " "archive.GST_PERCENT, "
			 "archive.COST_GST_PERCENT, " "archive.REDEEMED Points_Redeemed, " "archive.POINTS_EARNED, " "archive.PRICE_LEVEL0 Noraml_Price, "
			 "archive.PRICE_LEVEL1 HappyHour_Price, " "contacts.name member_name, " "contacts.site_id, " "contacts.MEMBER_NUMBER, "
			 "arccategories.category, " "CATEGORYGROUPS.NAME category_group " "from archive "
			 "left join contacts on archive.loyalty_key = contacts.contacts_key "
			 "left join arccategories on archive.category_key = arccategories.category_key "
			 "left join CATEGORYGROUPS on arccategories.CATEGORYGROUPS_KEY = CATEGORYGROUPS.CATEGORYGROUPS_KEY "
			 "left join ARCBILL on (archive.ARCBILL_KEY = ARCBILL.ARCBILL_KEY) "
			 "order by ARCBILL.INVOICE_NUMBER,archive.TIME_STAMP_BILLED desc ";
		IBInternalQuery->ExecQuery();

		AnsiString Line = "";
		char CrLf[] = "\r\n";
		for (int i=0; i< IBInternalQuery->FieldCount; i++)
		{
			Line += IBInternalQuery->Fields[i]->Name;
			if(i != (IBInternalQuery->FieldCount-1))
			{
				Line += ",";
			}
		}
		File->Write(Line.c_str(),Line.Length());
		File->Write(&CrLf,2);

		cout << "Working....." << endl;

		int c = 0;
		int FieldCount = IBInternalQuery->FieldCount;
		int LastField = IBInternalQuery->FieldCount-1;
		for (int t = 0; !IBInternalQuery->Eof; IBInternalQuery->Next(), c++,t++)
		{
			Line = "";
			for (int i=0; i< FieldCount; i++)
			{
				Line += IBInternalQuery->Fields[i]->AsString;
				if(i != (LastField))
				{
					Line += ",";
				}
			}
			if(c > 10000)
			{
				wcout << "Still Working, I am up to Line "	<< IntToStr(t).c_str() << endl;
				c = 0;
			}
			File->Write(Line.c_str(),Line.Length());
			File->Write(CrLf,2);
		}
		DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		wcout << "Sorry I crashed :( Heres why... " << endl;
		wcout << E.Message.c_str() << endl;
		wcout << endl;
		wcout << "Usage SummaExport.exe ServerName DatabasePath " << endl;
		wcout << "The Default is SummaExport.exe localhost C:\\Program Files\\MenuMate\\MeuMate.ib " << endl;
	}
	wcout << "All done see 'results.csv' hit Return to exit. "	<< endl;
	char *input = new char[1024];
	cin.getline(input,1);
}
// ---------------------------------------------------------------------------
