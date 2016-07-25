#pragma hdrstop

#include "DB_5_5.h"
#include <algorithm>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

void DB_5_5::Up_5_5(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.4";
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

				std::auto_ptr<Database::TDBUtilities>Util(new Database::TDBUtilities(DBControl));

				Database::TTableNames TableNames;
				Util->GetTableNames(TableNames);

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "PRNCAT");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE TABLE PRNCAT (  PRNCAT_KEY INTEGER NOT NULL PRIMARY KEY, " "VIRTUALPRINTER_KEY INTEGER, "
						"FOREIGN KEY (VIRTUALPRINTER_KEY) REFERENCES VIRTUALPRINTER (VIRTUALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " "CATEGORY_KEY INTEGER, "
						"FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE, " "PRNORD INTEGER);";
					IBQuery1->ExecQuery();
				}

				Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PRNCAT");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_PRNCAT;";
					IBQuery1->ExecQuery();
				}

				AnsiString base_alter_str;
				AnsiString field_strs[3] =
				{
					"ITEM", "SIZE", "COURSE"
				};

				AnsiString tables_to_update[6] =
				{
					"COURSE", "ITEM", "ITEMSIZE", "OPTIONS", "ORDEROPTION", "SIZES"
				};

				for (int i = 0; i < 6; i++) {

					Database::TFieldNames FieldNames;
					Database::TFieldNames FieldNames1;
					Util->GetFieldNames(tables_to_update[i], FieldNames);
					Util->GetFieldNames(tables_to_update[i], FieldNames1);

					Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "HANDHELD_NAME");
					Database::TFieldNames::iterator itName1 = std::find(FieldNames1.begin(), FieldNames1.end(), "RECEIPT_NAME");
					if (itName == FieldNames.end() && itName1 == FieldNames1.end())
					{
						base_alter_str = "alter table " + tables_to_update[i];
						IBQuery1->Close();
						IBQuery1->SQL->Text = base_alter_str + " add handheld_name varchar(50); ";
						IBQuery1->ExecQuery();
						IBQuery1->Close();
						IBQuery1->SQL->Text = base_alter_str + " add receipt_name varchar(50); ";
						IBQuery1->ExecQuery();
					}
				}

				for (int j = 0; j < 3; j++)
				{

					Database::TFieldNames FieldNames;
					Database::TFieldNames FieldNames1;
					Util->GetFieldNames("ORDERS", FieldNames);
					Util->GetFieldNames("ORDERS", FieldNames1);

					Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), field_strs[j] + "_HANDHELD_NAME");
					Database::TFieldNames::iterator itName1 = std::find(FieldNames.begin(), FieldNames.end(), field_strs[j] + "_RECEIPT_NAME");

					if (itName == FieldNames.end() && itName1 == FieldNames1.end())
					{

						IBQuery1->Close();
						IBQuery1->SQL->Text = "alter table Orders add " + field_strs[j] + "_handheld_name " + "varchar(50);";
						IBQuery1->ExecQuery();
						IBQuery1->Close();
						IBQuery1->SQL->Text = "alter table Orders add " + field_strs[j] + "_receipt_name " + "varchar(50);";
						IBQuery1->ExecQuery();
					}
				}

				Database::TFieldNames FieldNames;
				Util->GetFieldNames("ITEMSIZE", FieldNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE ADD PLU INTEGER NOT NULL";
					IBQuery1->ExecQuery();

					DBTransaction.Commit();
					DBTransaction.StartTransaction();

					IBQuery1->Close();
					IBQuery1->SQL->Text = "UPDATE ITEMSIZE I SET I.PLU = I.ITEMSIZE_KEY";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("ORDERS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ORDERS ADD PLU INTEGER ";
					IBQuery1->ExecQuery();
				}

				FieldNames.clear();
				Util->GetFieldNames("WRITEOFF", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "TERMINALNAME");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE WRITEOFF ADD TERMINALNAME VARCHAR(50)";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("DAYARCHIVE", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE DAYARCHIVE ADD PLU INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("ARCHIVE", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE ARCHIVE ADD PLU INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("PAYMENTTYPES", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_URL");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD VOUCHER_URL VARCHAR(80) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_USER");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD VOUCHER_USER VARCHAR(50) ";
					IBQuery1->ExecQuery();
				}

				itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_PASS");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES ADD VOUCHER_PASS VARCHAR(20) ";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "WEBDATA");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE TABLE WEBDATA (  WEBDATA_KEY INTEGER NOT NULL PRIMARY KEY, "
                    	"WEBORDER_KEY INTEGER,  "
						"FOREIGN KEY (WEBORDER_KEY) REFERENCES WEBORDERS (WEBORDER_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
                        "CATEGORY VARCHAR(60),ELEMENT VARCHAR(60),DATA VARCHAR(60));";
					IBQuery1->ExecQuery();
				}

				itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WEBDATA_KEY");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_WEBDATA_KEY;";
					IBQuery1->ExecQuery();
				}

				Sleep(1000);
				IBQuery1->Close();
				IBQuery1->ParamCheck = true;
				IBQuery1->SQL->Text = "INSERT INTO VERSIONHISTORY ( VERSION_NUMBER, TIME_STAMP, COMMENT)" " VALUES ( :VersionInfo,:DateTime,'Upgrade');";
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

void DB_5_5::Down_5_5(Database::TDBControl &DBControl)
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

		std::auto_ptr<Database::TDBUtilities>Util(new Database::TDBUtilities(DBControl));

		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);

		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);

		/*
		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "PRNCAT");
		if (itTable != TableNames.end())
		{
		IBQuery1->Close();
		IBQuery1->SQL->Text = "DROP TABLE WRITEOFF ";
		IBQuery1->ExecQuery();
		}
		 */

		Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_PRNCAT");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_PRNCAT;";
			IBQuery1->ExecQuery();
		}

		Database::TFieldNames FieldNames;
		Util->GetFieldNames("ITEMSIZE", FieldNames);
		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ITEMSIZE DROP COLUMN PLU";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("ORDERS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE ORDERS DROP COLUMN PLU";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("WRITEOFF", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "TERMINALNAME");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE WRITEOFF DROP COLUMN TERMINALNAME";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("DAYARCHIVE", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE DAYARCHIVE DROP COLUMN PLU";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("ARCHIVE", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "PLU");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE ARCHIVE DROP COLUMN PLU";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("PAYMENTTYPES", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_URL");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES DROP COLUMN VOUCHER_URL";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_USER");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES DROP COLUMN VOUCHER_USER";
			IBQuery1->ExecQuery();
		}

		itName = std::find(FieldNames.begin(), FieldNames.end(), "VOUCHER_PASS");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE PAYMENTTYPES DROP COLUMN VOUCHER_PASS";
			IBQuery1->ExecQuery();
		}

        Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "WEBDATA");
		if (itTable != TableNames.end())
		{
		IBQuery1->Close();
		IBQuery1->SQL->Text = "DROP TABLE WEBDATA ";
		IBQuery1->ExecQuery();
		}

		itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WEBDATA_KEY");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_WEBDATA_KEY;";
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
