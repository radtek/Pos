#include "DB_5_16.h"
#include "MM_DBCore.h"
#include "Predicates.h"
#include <algorithm>

using Database::TDBControl;
using Database::TDBUtilities;
using Database::TDBTransaction;

void DB_5_16::Apply(TDBControl &control)
{
    TDBTransaction t(control);
    TIBSQL *q = t.Query(t.AddQuery());

	q->SQL->Text = L"ALTER TABLE PAYMENTTYPES ADD CSV_READ_LOCATION VARCHAR (255);";
	t.StartTransaction();
	q->ExecQuery();
	q->Close();
	q->SQL->Text = L"ALTER TABLE PAYMENTTYPES ADD CSV_WRITE_LOCATION VARCHAR (255);";
	q->ExecQuery();
	t.Commit();
}

void DB_5_16::Up_5_16(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.15";
	AnsiString UpdateVersion = "5.16";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);
		Database::TcpIBSQL IBQuery2(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery2);

		IBQuery->Close();

		IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
		IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery->ExecQuery();
		if (IBQuery->RecordCount == 0)
		{
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Version Number " + UpdateFromVersion + " Required");
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Updating Database To Version Number " + UpdateVersion);

			IBQuery->Close();
			IBQuery->SQL->Text = "SELECT * FROM VERSIONHISTORY WHERE VERSION_NUMBER LIKE :VERSION_NUMBER;";
			IBQuery->ParamByName("VERSION_NUMBER")->AsString = UpdateVersion;
			IBQuery->ExecQuery();
			if (IBQuery->RecordCount == 0)
			{

				std::auto_ptr <Database::TDBUtilities> Util(new Database::TDBUtilities(DBControl));

				Database::TFieldNames FieldNames;
				Util->GetFieldNames("PAYMENTTYPES", FieldNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "CSV_READ_LOCATION");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD CSV_READ_LOCATION VARCHAR (255)";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "CSV_WRITE_LOCATION");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD CSV_WRITE_LOCATION VARCHAR (255)";
					IBQuery1->ExecQuery();
				}

				FieldNames.clear();
				Util->GetFieldNames("DISCOUNTS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "APPLYTOMEMBERS");
				if (itName != FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS DROP APPLYTOMEMBERS";
					IBQuery1->ExecQuery();
				}

				Sleep(1000);
				IBQuery1->Close();
				IBQuery1->ParamCheck = true;
				IBQuery1->SQL->Text =
					 "INSERT INTO VERSIONHISTORY ( VERSION_NUMBER, TIME_STAMP, COMMENT)" " VALUES ( :VersionInfo,:DateTime,'Upgrade');";
				IBQuery1->ParamByName("VersionInfo")->AsString = UpdateVersion;
				IBQuery1->ParamByName("DateTime")->AsDateTime = Now();
				IBQuery1->ExecQuery();

				DBTransaction.Commit();
				TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Update " + UpdateVersion + " Successfull");
				// Applying alterations here for speed.
			}

			else
			{
				TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Version Number " + UpdateVersion + " Updates Already Applied Successfully");
			}
		}
	}

	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		throw;
	}
}

void DB_5_16::Down_5_16(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.15";
	AnsiString UpdateVersion = "5.16";

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		Database::TcpIBSQL IBQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery);
		Database::TcpIBSQL IBQuery1(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery1);
		Database::TcpIBSQL IBQuery2(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBQuery2);

		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Tearing Down Database To Version Number " + UpdateVersion);

		std::auto_ptr <Database::TDBUtilities> Util(new Database::TDBUtilities(DBControl));

		Database::TFieldNames FieldNames;
		Util->GetFieldNames("PAYMENTTYPES", FieldNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "CSV_READ_LOCATION");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES DROP COLUMN CSV_READ_LOCATION";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "CSV_WRITE_LOCATION");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES DROP COLUMN CSV_WRITE_LOCATION";
			IBQuery1->ExecQuery();
		}

        FieldNames.clear();
        Util->GetFieldNames("DISCOUNTS", FieldNames);

        itName = std::find(FieldNames.begin(), FieldNames.end(), "APPLYTOMEMBERS");
        if (itName == FieldNames.end())
        {
            IBQuery1->Close();
            IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS ADD APPLYTOMEMBERS T_TRUEFALSE";
            IBQuery1->ExecQuery();
        }

		Sleep(1000);
		IBQuery1->Close();
		IBQuery1->ParamCheck = true;
		IBQuery1->SQL->Text = "DELETE FROM VERSIONHISTORY WHERE VERSION_NUMBER = :VERSION_NUMBER ";
		IBQuery1->ParamByName("VERSION_NUMBER")->AsString = UpdateFromVersion;
		IBQuery1->ExecQuery();

		DBTransaction.Commit();
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Downgrade From " + UpdateFromVersion + " Successfull");
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		throw;
	}

}


