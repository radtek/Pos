//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_6.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)
void DB_5_6::Up_5_6(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.5";
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
				Util->GetFieldNames("CONTACTTIME", FieldNames);

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "TOTALHOURS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTTIME ADD TOTALHOURS NUMERIC(15,2) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "BREAKS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTTIME ADD BREAKS TIME ";
					IBQuery1->ExecQuery();
				}

				FieldNames.clear();
				Util->GetFieldNames("BLINDBALANCE", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "DEPOSITBAG_ID");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE BLINDBALANCE ADD DEPOSITBAG_ID VARCHAR(20)";
					IBQuery1->ExecQuery();
				}

            FieldNames.clear();
				Util->GetFieldNames("CONTACTS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "HOURLY_RATE");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTS ADD HOURLY_RATE NUMERIC(15,2)";
					IBQuery1->ExecQuery();
				}

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "COMMISSION");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE COMMISSION (  COMMISSION_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "STAFF_KEY INTEGER, "
						 "STAFF_NAME VARCHAR(50), "
						 "ON_DATE TIMESTAMP, "
						 "AVG_COMMISSION NUMERIC(15,4), "
						 "DATEFROM TIMESTAMP, "
						 "DATETO TIMESTAMP, "
						 "TOTALCOMMISSION NUMERIC(15,4), "
						 "Z_KEY INTEGER);";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "PRINTERREADINGS");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE PRINTERREADINGS (  PRINTERREADINGS_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "PRINTER_NAME VARCHAR(20), "
						 "START_NUMBER INTEGER, "
						 "END_NUMBER INTEGER, "
						 "COPIES INTEGER, "
						 "Z_KEY INTEGER);";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "PAXCOUNT");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE PAXCOUNT ( PAXCOUNT_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "PAX_NUMBER INTEGER, "
						 "DATE_FROM DATE, "
						 "DATE_TO DATE, "
						 "Z_KEY INTEGER);";
					IBQuery1->ExecQuery();
				}

				Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_COMMISSION");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_COMMISSION;";
					IBQuery1->ExecQuery();
				}

				itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PRINTERREADINGS");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PRINTERREADINGS;";
					IBQuery1->ExecQuery();
				}

				itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PAXCOUNT");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PAXCOUNT;";
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

void DB_5_6::Down_5_6(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.6";
	AnsiString UpdateVersion = "5.5";

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
		Util->GetFieldNames("CONTACTTIME", FieldNames);
		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);
		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "TOTALHOURS");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME DROP COLUMN TOTALHOURS";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "BREAK");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTTIME DROP COLUMN BREAK";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("BLINDBALANCE", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "DEPOSITBAG_ID");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE BLINDBALANCE DROP COLUMN DEPOSITBAG_ID";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("CONTACTS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "HOURLY_RATE");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS DROP COLUMN HOURLY_RATE";
			IBQuery1->ExecQuery();
		}

		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "COMMISSION");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE COMMISSION ";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "PRINTERREADINGS");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE PRINTERREADINGS ";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "PAXCOUNT");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE PAXCOUNT ";
			IBQuery1->ExecQuery();
		}

		Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_COMMISSION");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_COMMISSION;";
			IBQuery1->ExecQuery();
		}

		itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PRINTERREADINGS");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_PRINTERREADINGS;";
			IBQuery1->ExecQuery();
		}

		itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PAXCOUNT");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_PAXCOUNT;";
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

