//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_7.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)
void DB_5_7::Up_5_7(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.6";
	AnsiString UpdateVersion = "5.7";

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

				Database::TTableNames TableNames;
				Util->GetTableNames(TableNames);

				Database::TFieldNames FieldNames;
				Util->GetFieldNames("CONTACTS", FieldNames);

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "EARNT_POINTS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTS ADD EARNT_POINTS NUMERIC(15,4) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "LOADED_POINTS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTS ADD LOADED_POINTS NUMERIC(15,4) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "INITIAL_EARNT_POINTS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTS ADD INITIAL_EARNT_POINTS NUMERIC(15,4) ";
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

void DB_5_7::Down_5_7(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.7";
	AnsiString UpdateVersion = "5.6";

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
		Util->GetFieldNames("CONTACTS", FieldNames);
		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);
		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "EARNT_POINTS");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS DROP COLUMN EARNT_POINTS";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "LOADED_POINTS");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME DROP COLUMN LOADED_POINTS";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("DISCOUNTS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "APPLYTOMEMBERS");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DISCOUNTS DROP COLUMN APPLYTOMEMBERS";
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

