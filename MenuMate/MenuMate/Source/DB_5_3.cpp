// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DB_5_3.h"
#include <algorithm>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

void DB_5_3::Up_5_3(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.2";
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
				Util->GetFieldNames("CONTACTGROUPS", FieldNames);
				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "SUMMA_GROUP");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTGROUPS ADD SUMMA_GROUP INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("PAYMENTTYPES", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "INVOICE_EXPORT");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD INVOICE_EXPORT INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("CONTACTS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "ACCOUNT_PROFILE");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE CONTACTS ADD ACCOUNT_PROFILE INTEGER ";
					IBQuery1->ExecQuery();

				}

				Util->GetFieldNames("DISCOUNTS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_ONLY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS ADD MEMBERS_ONLY T_TRUEFALSE ";
					IBQuery1->ExecQuery();

				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_EXEMPT");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS ADD MEMBERS_EXEMPT T_TRUEFALSE ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("ORDERDISCOUNTS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_ONLY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ORDERDISCOUNTS ADD MEMBERS_ONLY T_TRUEFALSE ";
					IBQuery1->ExecQuery();

				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_EXEMPT");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ORDERDISCOUNTS ADD MEMBERS_EXEMPT T_TRUEFALSE ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("DAYARCORDERDISCOUNTS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_ONLY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DAYARCORDERDISCOUNTS ADD MEMBERS_ONLY T_TRUEFALSE ";
					IBQuery1->ExecQuery();

				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_EXEMPT");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DAYARCORDERDISCOUNTS ADD MEMBERS_EXEMPT T_TRUEFALSE ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("ARCORDERDISCOUNTS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_ONLY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ARCORDERDISCOUNTS ADD MEMBERS_ONLY T_TRUEFALSE ";
					IBQuery1->ExecQuery();

				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_EXEMPT");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ARCORDERDISCOUNTS ADD MEMBERS_EXEMPT T_TRUEFALSE ";
					IBQuery1->ExecQuery();
				}

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "REFLOAT_SKIM");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE REFLOAT_SKIM (  REFLOAT_SKIM_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "TRANSACTION_TYPE VARCHAR(20), "
						 "AMOUNT NUMERIC(15,4), "
				   "STAFF VARCHAR(50), "
				   "TERMINAL_NAME VARCHAR(22), "
						 "TIME_STAMP TIMESTAMP, "
				   "Z_KEY INTEGER, "
	   //			   "CONSTRAINT ZEDS_Z_KEY FOREIGN KEY (Z_KEY) REFERENCES ZEDS (Z_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
				   "REASONS VARCHAR(200));";
					IBQuery1->ExecQuery();
				}


				Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_REFLOAT_SKIM");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_REFLOAT_SKIM;";
					IBQuery1->ExecQuery();
				}
				DBTransaction.Commit();
				DBTransaction.StartTransaction();

                IBQuery1->Close();
                IBQuery1->SQL->Text = "update ORDERDISCOUNTS o set o.MEMBERS_EXEMPT = 'F' where o.MEMBERS_EXEMPT is null;";
                IBQuery1->ExecQuery();

                IBQuery1->Close();
                IBQuery1->SQL->Text = "update ORDERDISCOUNTS o set o.MEMBERS_ONLY = 'F' where o.MEMBERS_ONLY is null;";
                IBQuery1->ExecQuery();

                IBQuery1->Close();
                IBQuery1->SQL->Text = "update DAYARCORDERDISCOUNTS o set o.MEMBERS_EXEMPT = 'F' where o.MEMBERS_EXEMPT is null;";
                IBQuery1->ExecQuery();

                IBQuery1->Close();
                IBQuery1->SQL->Text = "update DAYARCORDERDISCOUNTS o set o.MEMBERS_ONLY = 'F' where o.MEMBERS_ONLY is null;";
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

void DB_5_3::Down_5_3(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.3";
	AnsiString UpdateVersion = "5.2";

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
		Util->GetFieldNames("CONTACTGROUPS", FieldNames);
		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);
		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "SUMMA_GROUP");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTGROUPS DROP COLUMN SUMMA_GROUP";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("PAYMENTTYPES", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "INVOICE_EXPORT");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE PAYMENTTYPES DROP COLUMN INVOICE_EXPORT";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("CONTACTS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "ACCOUNT_PROFILE");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE CONTACTS DROP COLUMN ACCOUNT_PROFILE";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("DISCOUNTS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_ONLY");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS DROP COLUMN MEMBERS_ONLY ";
			IBQuery1->ExecQuery();

		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "MEMBERS_EXEMPT");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE DISCOUNTS DROP COLUMN MEMBERS_EXEMPT";
			IBQuery1->ExecQuery();

		}

		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "REFLOAT_SKIM");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE REFLOAT_SKIM ";
			IBQuery1->ExecQuery();
		}


		Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_REFLOAT_SKIM");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_REFLOAT_SKIM;";
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
