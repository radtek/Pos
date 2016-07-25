//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_2.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)


void DB_5_2::Up_5_2(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.1";
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
				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TTableNames::iterator itName = std::find(TableNames.begin(), TableNames.end(), "WEBORDERS");
				if (itName == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE WEBORDERS (  WEBORDER_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "GUID VARCHAR(50), "
						 "NAME VARCHAR(50), "
                   "STORE_NAME VARCHAR(50), "
                   "ORDER_DATE TIMESTAMP, "
						 "EXPECTED_DATE TIMESTAMP, "
                   "TAB_KEY INTEGER, "
                   "CONSTRAINT WEBORDERS_TAB_KEY FOREIGN KEY (TAB_KEY) REFERENCES TAB (TAB_KEY) ON UPDATE CASCADE ON DELETE SET NULL, "
                   "TIME_KEY INTEGER, "
                   "CONSTRAINT WEBORDERS_TIME_KEY FOREIGN KEY (TIME_KEY) REFERENCES TURNAROUNDTIMES (TIME_KEY) ON UPDATE CASCADE ON DELETE SET NULL, "
                   "STATUS INTEGER, "
                   "PREPAID NUMERIC(15,4), "
                   "CONTACTS_KEY INTEGER);";
					IBQuery1->ExecQuery();
				}

				itName = std::find(TableNames.begin(), TableNames.end(), "GROUPS");
				if (itName == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE GROUPS (  GROUPS_KEY INTEGER NOT NULL PRIMARY KEY, "
						 "NAME VARCHAR(50), "
                   "TYPE INTEGER, "
                   "STATUS INTEGER);";
					IBQuery1->ExecQuery();
				}

				itName = std::find(TableNames.begin(), TableNames.end(), "CONTACTGROUPS");
				if (itName == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						 "CREATE TABLE CONTACTGROUPS (  CONTACTGROUPS_KEY INTEGER NOT NULL PRIMARY KEY, "
                   "CONTACTS_KEY INTEGER, "
                   "CONSTRAINT CONTACTS_CONTACTS_KEY FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
                   "GROUPS_KEY INTEGER, "
                   "CONSTRAINT GROUPS_GROUPS_KEY FOREIGN KEY (GROUPS_KEY) REFERENCES GROUPS (GROUPS_KEY) ON UPDATE CASCADE ON DELETE CASCADE)";
					IBQuery1->ExecQuery();
				}

				IBQuery1->Close();
				IBQuery1->SQL->Text =
              "SELECT r.RDB$FIELD_NAME AS field_name, "
              "        f.RDB$FIELD_LENGTH AS field_length, "
              "        CASE f.RDB$FIELD_TYPE "
              "          WHEN 261 THEN 'BLOB' "
              "          WHEN 14 THEN 'CHAR' "
              "          WHEN 40 THEN 'CSTRING' "
              "          WHEN 11 THEN 'D_FLOAT' "
              "          WHEN 27 THEN 'DOUBLE' "
              "          WHEN 10 THEN 'FLOAT' "
              "          WHEN 16 THEN 'INT64' "
              "          WHEN 8 THEN 'INTEGER' "
              "          WHEN 9 THEN 'QUAD' "
              "          WHEN 7 THEN 'SMALLINT' "
              "          WHEN 12 THEN 'DATE' "
              "          WHEN 13 THEN 'TIME' "
              "          WHEN 35 THEN 'TIMESTAMP' "
              "          WHEN 37 THEN 'VARCHAR' "
              "          ELSE 'UNKNOWN' "
              "        END AS field_type "
              "   FROM RDB$RELATION_FIELDS r "
              "   LEFT JOIN RDB$FIELDS f ON r.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME "
              "   LEFT JOIN RDB$COLLATIONS coll ON f.RDB$COLLATION_ID = coll.RDB$COLLATION_ID "
              "   LEFT JOIN RDB$CHARACTER_SETS cset ON f.RDB$CHARACTER_SET_ID = cset.RDB$CHARACTER_SET_ID "
              "  WHERE r.RDB$RELATION_NAME = 'ORDERS' "
              "  AND r.RDB$FIELD_NAME = 'TRANSNO' "
              "ORDER BY r.RDB$FIELD_POSITION; ";
					IBQuery1->ExecQuery();
               if(IBQuery1->RecordCount == 1)
               {
                  if(IBQuery1->FieldByName("FIELD_TYPE")->AsString.Trim().UpperCase() == "INTEGER")
                  {  // Field Conversion Required
							IBQuery1->Close();
							IBQuery1->SQL->Text = "ALTER TABLE ORDERS ALTER TRANSNO TYPE VARCHAR(50) CHARACTER SET ISO8859_1";
							IBQuery1->ExecQuery();
                  }
               }

				Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WEBORDERS_KEY");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_WEBORDERS_KEY;";
					IBQuery1->ExecQuery();
				}

				itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_GROUPS_KEY");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_GROUPS_KEY;";
					IBQuery1->ExecQuery();
				}

				itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_CONTACTGROUPS_KEY");
				if (itGenName == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_CONTACTGROUPS_KEY;";
					IBQuery1->ExecQuery();
				}

				itName = std::find(TableNames.begin(), TableNames.end(), "DAYARCWEB");
				if (itName == TableNames.end())
            {
               IBQuery1->Close();
               IBQuery1->SQL->Text =
                   "CREATE TABLE DAYARCWEB ( "
                   "WEBORDER_KEY INTEGER, "
                   "CONSTRAINT DAYWEBARCHIVE_WEBKEY FOREIGN KEY (WEBORDER_KEY) REFERENCES WEBORDERS (WEBORDER_KEY), "
                   "ARCBILL_KEY INTEGER, "
                   "CONSTRAINT DAYWEBARCHIVE_BILLKEY FOREIGN KEY (ARCBILL_KEY) REFERENCES DAYARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE );";
               IBQuery1->ExecQuery();
            }

				itName = std::find(TableNames.begin(), TableNames.end(), "ARCWEB");
				if (itName == TableNames.end())
            {
               IBQuery1->Close();
               IBQuery1->SQL->Text =
                   "CREATE TABLE ARCWEB ( "
                   "WEBORDER_KEY INTEGER, "
                   "CONSTRAINT WEBARCHIVE_WEBKEY FOREIGN KEY (WEBORDER_KEY) REFERENCES WEBORDERS (WEBORDER_KEY), "
                   "ARCBILL_KEY INTEGER, "
                   "CONSTRAINT WEBARCHIVE_BILLKEY FOREIGN KEY (ARCBILL_KEY) REFERENCES ARCBILL (ARCBILL_KEY) ON UPDATE CASCADE ON DELETE CASCADE )";
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

void DB_5_2::Down_5_2(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.2";
	AnsiString UpdateVersion = "5.1";

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

		Database::TTableNames TableNames;
		Util->GetTableNames(TableNames);
		Database::TGeneratorNames GeneratorNames;
		Util->GetGeneratorNames(GeneratorNames);


		Database::TTableNames::iterator itName = std::find(TableNames.begin(), TableNames.end(), "DAYARCWEB");
		if (itName != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE DAYARCWEB ";
			IBQuery1->ExecQuery();
		}

		itName = std::find(TableNames.begin(), TableNames.end(), "ARCWEB");
		if (itName != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE ARCWEB ";
			IBQuery1->ExecQuery();
		}

  		itName = std::find(TableNames.begin(), TableNames.end(), "WEBORDERS");
		if (itName != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE WEBORDERS ";
			IBQuery1->ExecQuery();
		}

  		itName = std::find(TableNames.begin(), TableNames.end(), "CONTACTGROUPS");
		if (itName != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE CONTACTGROUPS ";
			IBQuery1->ExecQuery();
		}

  		itName = std::find(TableNames.begin(), TableNames.end(), "GROUPS");
		if (itName != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE GROUPS ";
			IBQuery1->ExecQuery();
		}

		Database::TGeneratorNames::iterator itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_WEBORDERS_KEY");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_WEBORDERS_KEY;";
			IBQuery1->ExecQuery();
		}

		itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_GROUPS_KEY");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_GROUPS_KEY;";
			IBQuery1->ExecQuery();
		}

		itGenName = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_CONTACTGROUPS_KEY");
		if (itGenName != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP GENERATOR GEN_CONTACTGROUPS_KEY;";
			IBQuery1->ExecQuery();
		}

		IBQuery1->Close();
      IBQuery1->SQL->Text =
        "SELECT r.RDB$FIELD_NAME AS field_name, "
        "        f.RDB$FIELD_LENGTH AS field_length, "
        "        CASE f.RDB$FIELD_TYPE "
        "          WHEN 261 THEN 'BLOB' "
        "          WHEN 14 THEN 'CHAR' "
        "          WHEN 40 THEN 'CSTRING' "
        "          WHEN 11 THEN 'D_FLOAT' "
        "          WHEN 27 THEN 'DOUBLE' "
        "          WHEN 10 THEN 'FLOAT' "
        "          WHEN 16 THEN 'INT64' "
        "          WHEN 8 THEN 'INTEGER' "
        "          WHEN 9 THEN 'QUAD' "
        "          WHEN 7 THEN 'SMALLINT' "
        "          WHEN 12 THEN 'DATE' "
        "          WHEN 13 THEN 'TIME' "
        "          WHEN 35 THEN 'TIMESTAMP' "
        "          WHEN 37 THEN 'VARCHAR' "
        "          ELSE 'UNKNOWN' "
        "        END AS field_type "
        "   FROM RDB$RELATION_FIELDS r "
        "   LEFT JOIN RDB$FIELDS f ON r.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME "
        "   LEFT JOIN RDB$COLLATIONS coll ON f.RDB$COLLATION_ID = coll.RDB$COLLATION_ID "
        "   LEFT JOIN RDB$CHARACTER_SETS cset ON f.RDB$CHARACTER_SET_ID = cset.RDB$CHARACTER_SET_ID "
        "  WHERE r.RDB$RELATION_NAME = 'ORDERS' "
        "  AND r.RDB$FIELD_NAME = 'TRANSNO' "
        "ORDER BY r.RDB$FIELD_POSITION; ";
         IBQuery1->ExecQuery();
         if(IBQuery1->RecordCount == 1)
         {
            if(IBQuery1->FieldByName("FIELD_TYPE")->AsString.Trim().UpperCase() != "INTEGER")
            {  // Field Conversion Required
               IBQuery1->Close();
               IBQuery1->SQL->Text = "ALTER TABLE ORDERS DROP TRANSNO";
               IBQuery1->ExecQuery();

               IBQuery1->Close();
               IBQuery1->SQL->Text = "ALTER TABLE ORDERS ADD TRANSNO INTEGER";
               IBQuery1->ExecQuery();

            }
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
