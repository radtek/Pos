//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_4.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)
void DB_5_4::Up_5_4(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.3";
	AnsiString UpdateVersion = "5.4";

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
				Util->GetFieldNames("STOCKTRACK", FieldNames);

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "STAFF");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE STOCKTRACK ADD STAFF VARCHAR(30) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "REASON");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE STOCKTRACK ADD REASON VARCHAR(250) ";
					IBQuery1->ExecQuery();
				}

            FieldNames.clear();
				Util->GetFieldNames("ORDERS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "MENU_ITEM_KEY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ORDERS ADD MENU_ITEM_KEY INTEGER";
					IBQuery1->ExecQuery();
				}

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "WRITEOFF");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE WRITEOFF (  WRITEOFF_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "ITEM VARCHAR(100), "
						 "STAFF VARCHAR(50), "
						 "AMOUNT NUMERIC(15,4), "
						 "TIME_STAMP TIMESTAMP, "
						 "REASONS VARCHAR(200), "
						 "CATEGORY_KEY INTEGER, "
						 "GROUP_kEY INTEGER, "
						 "SUPPLIER_KEY INTEGER, "
						 "MENU_KEY INTEGER, "
						 "COURSE_KEY INTEGER, "
						 "ITEM_KEY INTEGER, "
						 "PARENT_KEY INTEGER, "
						 "SIZE_NAME VARCHAR(50));";

					IBQuery1->ExecQuery();
				}


				Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WRITEOFF");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_WRITEOFF;";
					IBQuery1->ExecQuery();
				}

            IBQuery->Close();
            IBQuery->SQL->Text = "SELECT * FROM CONTACTS where "
				"((ACCESS_LEVEL / 134217728 / 2) - (ACCESS_LEVEL / 134217728 / 2.0) <> 0)";
            IBQuery->ExecQuery();
            if (IBQuery->RecordCount == 0)
            {
                // update them with Tab access.
                IBQuery1->Close();
                IBQuery1->SQL->Text = "UPDATE CONTACTS SET ACCESS_LEVEL = ACCESS_LEVEL + 134217728"
                  " where "
                  "((ACCESS_LEVEL / 134217728 / 2) - (ACCESS_LEVEL / 134217728 / 2.0) = 0)";
                IBQuery1->ExecQuery();
				}


             IBQuery1->Close();
             IBQuery1->SQL->Text = "update ORDERDISCOUNTS o set o.MEMBERS_EXEMPT = 'F' where o.MEMBERS_EXEMPT is null;";
             IBQuery1->ExecQuery();


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




void DB_5_4::Down_5_4(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.4";
	AnsiString UpdateVersion = "5.3";

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
		Util->GetFieldNames("STOCKTRACK", FieldNames);
		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);
		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "STAFF");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE STOCKTRACK DROP COLUMN STAFF";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "REASON");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE STOCKTRACK DROP COLUMN REASON";
			IBQuery1->ExecQuery();

		}

		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "WRITEOFF");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE WRITEOFF ";
			IBQuery1->ExecQuery();
		}


		Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WRITEOFF");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_WRITEOFF;";
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

