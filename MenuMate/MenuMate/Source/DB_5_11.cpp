//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_11.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)

void DB_5_11::Up_5_11(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.10";
	AnsiString UpdateVersion = "5.11";

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

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMCATEGORY");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERITEMCATEGORY"
						"("
							"HELDORDERITEMCATEGORY_KEY 	INTEGER NOT NULL,"
							"HELDITEM_KEY 				INTEGER NOT NULL,"
							"CATEGORY_KEY 				INTEGER NOT NULL,"
							"PRIMARY KEY (HELDORDERITEMCATEGORY_KEY),"
							"FOREIGN KEY (HELDITEM_KEY) 	REFERENCES HELDORDERITEMS(HELDORDERITEM_KEY),"
							"FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES(CATEGORY_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERSIDEITEMCATEGORY");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERSIDEITEMCATEGORY"
						"("
							"HELDORDERSIDEITEMCATEGORY_KEY 	INTEGER NOT NULL,"
							"HELDITEM_KEY 					INTEGER NOT NULL,"
							"CATEGORY_KEY 					INTEGER NOT NULL,"
							"PRIMARY KEY (HELDORDERSIDEITEMCATEGORY_KEY),"
							"FOREIGN KEY (HELDITEM_KEY) REFERENCES HELDORDERSIDEITEMS(HELDORDERSIDEITEM_KEY),"
							"FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES(CATEGORY_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TGeneratorNames::iterator itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERITEMCAT");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_HELDORDERITEMCAT";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_HELDORDERITEMCAT TO 0";
					IBQuery1->ExecQuery();
				}

				itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERSIDEITEMCAT");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_HELDORDERSIDEITEMCAT";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_HELDORDERSIDEITEMCAT TO 0";
					IBQuery1->ExecQuery();
				}

				Database::TFieldNames FieldNames;
				Util->GetFieldNames("HELDORDERITEMS", FieldNames);

				Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_KEY");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE HELDORDERITEMS ADD TAB_KEY INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("HELDORDERITEMS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_TYPE");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE HELDORDERITEMS ADD TAB_TYPE INTEGER ";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("HELDORDERITEMS", FieldNames);

				itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_NAME");
				if (itName == FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = " ALTER TABLE HELDORDERITEMS ADD TAB_NAME VARCHAR (32)";
					IBQuery1->ExecQuery();
				}

				Database::TViewNames ViewNames;
				Util->GetViewNames(ViewNames);

				Database::TViewNames::iterator itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSVIEW");
				if (itView != ViewNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "DROP VIEW HELDORDERSVIEW";
					IBQuery1->ExecQuery();
				}

				// Commit Current Changes to begin a Data Migration

				DBTransaction.Commit();
				DBTransaction.StartTransaction();

				Util->GetFieldNames("HELDORDERS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_KEY");
				if (itName != FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"SELECT "
							"HELDORDER_KEY,"
							"TAB_KEY "
						"FROM "
							"HELDORDERS ";
					IBQuery1->ExecQuery();

					for(; !IBQuery1->Eof; IBQuery1->Next())
					{
						IBQuery2->Close();
						IBQuery2->SQL->Text =
							"UPDATE "
								"HELDORDERITEMS "
							"SET "
								"TAB_KEY = :TAB_KEY "
							"WHERE "
								"HELDORDER_KEY = :HELDORDER_KEY";
						IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
						IBQuery2->ParamByName("TAB_KEY"		 )->AsInteger = IBQuery1->FieldByName("TAB_KEY"		 )->AsInteger;
						IBQuery2->ExecQuery();
					}

					IBQuery1->Close();
					IBQuery1->SQL->Text = "ALTER TABLE HELDORDERS DROP TAB_KEY";
					IBQuery1->ExecQuery();
				}

                Util->GetFieldNames("HELDORDERS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_TYPE");
				if (itName != FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"SELECT "
							"HELDORDER_KEY,"
							"TAB_TYPE "
						"FROM "
							"HELDORDERS ";
					IBQuery1->ExecQuery();

					for(; !IBQuery1->Eof; IBQuery1->Next())
					{
						IBQuery2->Close();
						IBQuery2->SQL->Text =
							"UPDATE "
								"HELDORDERITEMS "
							"SET "
								"TAB_TYPE = :TAB_TYPE "
							"WHERE "
								"HELDORDER_KEY = :HELDORDER_KEY";
						IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
						IBQuery2->ParamByName("TAB_TYPE"	 )->AsInteger = IBQuery1->FieldByName("TAB_TYPE"	 )->AsInteger;
						IBQuery2->ExecQuery();
					}

					IBQuery1->Close();
					IBQuery1->SQL->Text = "ALTER TABLE HELDORDERS DROP TAB_TYPE";
					IBQuery1->ExecQuery();
				}

				Util->GetFieldNames("HELDORDERS", FieldNames);
				itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_NAME");
				if (itName != FieldNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"SELECT "
							"HELDORDER_KEY,"
							"TAB_NAME "
						"FROM "
							"HELDORDERS ";
					IBQuery1->ExecQuery();

					for(; !IBQuery1->Eof; IBQuery1->Next())
					{
						IBQuery2->Close();
						IBQuery2->SQL->Text =
							"UPDATE "
								"HELDORDERITEMS "
							"SET "
								"TAB_NAME = :TAB_NAME "
							"WHERE "
								"HELDORDER_KEY = :HELDORDER_KEY";
						IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
						IBQuery2->ParamByName("TAB_NAME"	 )->AsString  = IBQuery1->FieldByName("TAB_NAME"	 )->AsString;
						IBQuery2->ExecQuery();
					}

					IBQuery1->Close();
					IBQuery1->SQL->Text = "ALTER TABLE HELDORDERS DROP TAB_NAME";
					IBQuery1->ExecQuery();
				}

				Util->GetViewNames(ViewNames);
				itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSVIEW");
				if (itView == ViewNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE VIEW HELDORDERSVIEW AS "
						"SELECT "
							"HOI.HELDORDERITEM_KEY,"
							"HO.HELDORDER_KEY,"
							"HOI.TAB_KEY,"
							"HOI.TAB_TYPE,"
							"HOI.TAB_NAME,"
							"HO.PARTY_NAME,"
							"HO.TABLE_NUMBER,"
							"HO.TABLE_NAME,"
							"HO.ORDER_TYPE,"
							"D.DEVICE_NAME AS TERMINAL_NAME,"
							"L.NAME AS ORDER_LOCATION,"
							"HO.ORDER_TYPE_MESSAGE,"
							"HO.CONTACTS_KEY,"
							"HO.ACTIVECHITNUMBER_KEY,"
							"HOI.ITEM_NAME,"
							"HOI.ITEM_ID,"
							"HOI.SIZE_NAME,"
							"HOI.NOTE,"
							"HOI.SEATNO,"
							"HOI.TRANSNO,"
							"HOI.MENU_NAME,"
							"HOI.COURSE_NAME,"
							"HOI.TIME_STAMP,"
							"HOI.MASTER_CONTAINER,"
							"HOI.SETMENU_MASK,"
							"HOI.SETMENU_GROUP,"
							"HOI.ITEM_CATEGORY,"
							"HOI.SECURITY_REF,"
							"HOI.TIME_KEY,"
							"HOI.QTY,"
							"HOI.ITEM_KITCHEN_NAME,"
							"HOI.SIZE_KITCHEN_NAME,"
							"HOI.COURSE_KITCHEN_NAME,"
							"HOI.CATEGORY_KEY,"
							"HOI.SERVINGCOURSES_KEY,"
							"HOI.CANCEL_STOCK_TYPE,"
							"HOI.ITEM_TYPE,"
							"HOI.MENU_ITEM_KEY,"
							"HOI.PLU,"
							"MENU.MENU_KEY "
						"FROM "
							"HELDORDERS HO "
							"INNER JOIN HELDORDERITEMS HOI "
							"ON HO.HELDORDER_KEY = HOI.HELDORDER_KEY "
							"INNER JOIN DEVICES D "
							"ON HO.DEVICE_KEY = D.DEVICE_KEY "
							"INNER JOIN LOCATIONS L "
							"ON D.LOCATION_KEY = L.LOCATION_KEY "
							"LEFT JOIN MENU "
							"ON HOI.MENU_NAME = MENU.MENU_NAME";
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

void DB_5_11::Down_5_11(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.11";
	AnsiString UpdateVersion = "5.10";

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

		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMCATEGORY");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERITEMCATEGORY";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERSIDEITEMCATEGORY");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERSIDEITEMCATEGORY";
			IBQuery1->ExecQuery();
		}

		Database::TGeneratorNames::iterator itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERITEMCAT");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_HELDORDERITEMCAT'";
			IBQuery1->ExecQuery();
		}

		itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERSIDEITEMCAT");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_HELDORDERSIDEITEMCAT'";
			IBQuery1->ExecQuery();
		}

		Database::TFieldNames FieldNames;
		Util->GetFieldNames("HELDORDERS", FieldNames);

		Database::TFieldNames::iterator itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_KEY");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE HELDORDERS ADD TAB_KEY INTEGER ";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("HELDORDERS", FieldNames);

		itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_TYPE");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE HELDORDERS ADD TAB_TYPE INTEGER ";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("HELDORDERS", FieldNames);

		itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_NAME");
		if (itName == FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = " ALTER TABLE HELDORDERS ADD TAB_NAME VARCHAR(32) ";
			IBQuery1->ExecQuery();
		}

		Database::TViewNames ViewNames;
		Util->GetViewNames(ViewNames);

		Database::TViewNames::iterator itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSVIEW");
		if (itView != ViewNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP VIEW HELDORDERSVIEW";
			IBQuery1->ExecQuery();
		}

        // Commit Current Changes to begin a Data Migration

		DBTransaction.Commit();
		DBTransaction.StartTransaction();

		Util->GetFieldNames("HELDORDERITEMS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_KEY");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"SELECT "
					"HELDORDER_KEY,"
					"TAB_KEY "
				"FROM "
					"HELDORDERITEMS ";
			IBQuery1->ExecQuery();

			for(; !IBQuery1->Eof; IBQuery1->Next())
			{
				IBQuery2->Close();
				IBQuery2->SQL->Text =
					"UPDATE "
						"HELDORDERS "
					"SET "
						"TAB_KEY = :TAB_KEY "
					"WHERE "
						"HELDORDER_KEY = :HELDORDER_KEY";
				IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
				IBQuery2->ParamByName("TAB_KEY"		 )->AsInteger = IBQuery1->FieldByName("TAB_KEY"		 )->AsInteger;
				IBQuery2->ExecQuery();
			}
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE HELDORDERITEMS DROP TAB_KEY";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("HELDORDERITEMS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_TYPE");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"SELECT "
					"HELDORDER_KEY,"
					"TAB_TYPE "
				"FROM "
					"HELDORDERITEMS ";
			IBQuery1->ExecQuery();

			for(; !IBQuery1->Eof; IBQuery1->Next())
			{
				IBQuery2->Close();
				IBQuery2->SQL->Text =
					"UPDATE "
						"HELDORDERS "
					"SET "
						"TAB_TYPE = :TAB_TYPE "
					"WHERE "
						"HELDORDER_KEY = :HELDORDER_KEY";
				IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
				IBQuery2->ParamByName("TAB_TYPE"	 )->AsInteger = IBQuery1->FieldByName("TAB_TYPE"	 )->AsInteger;
				IBQuery2->ExecQuery();
			}
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE HELDORDERITEMS DROP TAB_TYPE";
			IBQuery1->ExecQuery();
		}

		Util->GetFieldNames("HELDORDERITEMS", FieldNames);
		itName = std::find(FieldNames.begin(), FieldNames.end(), "TAB_NAME");
		if (itName != FieldNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"SELECT "
					"HELDORDER_KEY,"
					"TAB_NAME "
				"FROM "
					"HELDORDERITEMS ";
			IBQuery1->ExecQuery();

			for(; !IBQuery1->Eof; IBQuery1->Next())
			{
				IBQuery2->Close();
				IBQuery2->SQL->Text =
					"UPDATE "
						"HELDORDERS "
					"SET "
						"TAB_NAME = :TAB_NAME "
					"WHERE "
						"HELDORDER_KEY = :HELDORDER_KEY";
				IBQuery2->ParamByName("HELDORDER_KEY")->AsInteger = IBQuery1->FieldByName("HELDORDER_KEY")->AsInteger;
				IBQuery2->ParamByName("TAB_NAME"	 )->AsString  = IBQuery1->FieldByName("TAB_NAME"	 )->AsString;
				IBQuery2->ExecQuery();
			}
			IBQuery1->Close();
			IBQuery1->SQL->Text = "ALTER TABLE HELDORDERITEMS DROP TAB_NAME";
			IBQuery1->ExecQuery();
		}

		Util->GetViewNames(ViewNames);
        itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSVIEW");
		if (itView == ViewNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text =
				"CREATE VIEW HELDORDERSVIEW AS "
				"SELECT "
					"HOI.HELDORDERITEM_KEY,"
					"HO.HELDORDER_KEY,"
					"HO.TAB_KEY,"
					"HO.TAB_TYPE,"
					"HO.TAB_NAME,"
					"HO.PARTY_NAME,"
					"HO.TABLE_NUMBER,"
					"HO.TABLE_NAME,"
					"HO.ORDER_TYPE,"
					"D.DEVICE_NAME AS TERMINAL_NAME,"
					"L.NAME AS ORDER_LOCATION,"
					"HO.ORDER_TYPE_MESSAGE,"
					"HO.CONTACTS_KEY,"
					"HO.ACTIVECHITNUMBER_KEY,"
					"HOI.ITEM_NAME,"
					"HOI.ITEM_ID,"
					"HOI.SIZE_NAME,"
					"HOI.NOTE,"
					"HOI.SEATNO,"
					"HOI.TRANSNO,"
					"HOI.MENU_NAME,"
					"HOI.COURSE_NAME,"
					"HOI.TIME_STAMP,"
					"HOI.MASTER_CONTAINER,"
					"HOI.SETMENU_MASK,"
					"HOI.SETMENU_GROUP,"
					"HOI.ITEM_CATEGORY,"
					"HOI.SECURITY_REF,"
					"HOI.TIME_KEY,"
					"HOI.QTY,"
					"HOI.ITEM_KITCHEN_NAME,"
					"HOI.SIZE_KITCHEN_NAME,"
					"HOI.COURSE_KITCHEN_NAME,"
					"HOI.CATEGORY_KEY,"
					"HOI.SERVINGCOURSES_KEY,"
					"HOI.CANCEL_STOCK_TYPE,"
					"HOI.ITEM_TYPE,"
					"HOI.MENU_ITEM_KEY,"
					"HOI.PLU,"
					"MENU.MENU_KEY "
				"FROM "
					"HELDORDERS HO "
					"INNER JOIN HELDORDERITEMS HOI "
					"ON HO.HELDORDER_KEY = HOI.HELDORDER_KEY "
					"INNER JOIN DEVICES D "
					"ON HO.DEVICE_KEY = D.DEVICE_KEY "
					"INNER JOIN LOCATIONS L "
					"ON D.LOCATION_KEY = L.LOCATION_KEY "
					"LEFT JOIN MENU "
					"ON HOI.MENU_NAME = MENU.MENU_NAME";
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
