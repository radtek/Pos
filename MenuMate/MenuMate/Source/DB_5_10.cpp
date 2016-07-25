//---------------------------------------------------------------------------


#pragma hdrstop

#include "DB_5_10.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)

void DB_5_10::Up_5_10(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.9";
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

				Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "ACTIVECHITNUMBER");
				if (itTable == TableNames.end())
				{
                    IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE ACTIVECHITNUMBER"
						"("
							"ACTIVECHITNUMBER_KEY  INTEGER,"
							"CHITNUMBER_KEY        INTEGER,"
							"CHIT_VALUE            VARCHAR(20),"
							"PRIMARY KEY (ACTIVECHITNUMBER_KEY),"
							"FOREIGN KEY (CHITNUMBER_KEY) REFERENCES CHITNUMBER(CHITNUMBER_KEY)"
						")";
                    IBQuery1->ExecQuery();
                }

                itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERS");
                if (itTable == TableNames.end())
                {
                    IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERS"
						"("
							"HELDORDER_KEY      	  	INTEGER NOT NULL,"
							"HELDORDER_MOVE_KEY   		INTEGER,"
							"TAB_KEY					INTEGER,"
							"TAB_TYPE					INTEGER,"
							"TAB_NAME            	  	VARCHAR(32),"
							"PARTY_NAME         	  	VARCHAR(32),"
							"TABLE_NUMBER         		INTEGER,"
							"TABLE_NAME         	  	VARCHAR(25),"
							"ORDER_TYPE           		INTEGER,"
							"DEVICE_KEY           		INTEGER NOT NULL,"
							"ORDER_TYPE_MESSAGE   		VARCHAR(200),"
							"CONTACTS_KEY         		INTEGER,"
							"ACTIVECHITNUMBER_KEY 		INTEGER,"
							"PRIMARY KEY (HELDORDER_KEY),"
							"FOREIGN KEY (DEVICE_KEY) 	REFERENCES DEVICES(DEVICE_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMS");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERITEMS"
						"("
							"HELDORDERITEM_KEY            		INTEGER,"
							"HELDORDER_KEY           			INTEGER NOT NULL,"
							"ITEM_NAME                    		VARCHAR(50),"
							"ITEM_ID                      		INTEGER,"
							"SIZE_NAME                    		VARCHAR(30),"
							"NOTE                         		VARCHAR(80),"
							"SEATNO               				INTEGER,"
							"TRANSNO                      		VARCHAR(50),"
							"MENU_NAME                    		VARCHAR(15),"
							"COURSE_NAME                  		VARCHAR(25),"
							"TIME_STAMP           				TIMESTAMP,"
							"MASTER_CONTAINER             		VARCHAR(30),"
							"SETMENU_MASK                 		INTEGER,"
							"SETMENU_GROUP                		INTEGER,"
							"ITEM_CATEGORY                		VARCHAR(40),"
							"SECURITY_REF                 		INTEGER,"
							"TIME_KEY             				INTEGER,"
							"QTY                          		NUMERIC(15,4),"
							"ITEM_KITCHEN_NAME            		VARCHAR(200),"
							"SIZE_KITCHEN_NAME            		VARCHAR(200),"
							"COURSE_KITCHEN_NAME         		VARCHAR(200),"
							"CATEGORY_KEY                 		INTEGER NOT NULL,"
							"SERVINGCOURSES_KEY           		INTEGER,"
							"CANCEL_STOCK_TYPE      		    INTEGER,"
							"ITEM_TYPE        		            INTEGER,"
							"MENU_ITEM_KEY                		INTEGER,"
							"PLU                          	  	INTEGER,"
							"PRIMARY KEY (HELDORDERITEM_KEY),"
							"FOREIGN KEY (HELDORDER_KEY)	  	REFERENCES HELDORDERS(HELDORDER_KEY),"
							"FOREIGN KEY (CATEGORY_KEY)       	REFERENCES ARCCATEGORIES(CATEGORY_KEY),"
							"FOREIGN KEY (SERVINGCOURSES_KEY) 	REFERENCES SERVINGCOURSES(SERVINGCOURSES_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERSIDEITEMS");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERSIDEITEMS"
						"("
							"HELDORDERSIDEITEM_KEY        		INTEGER,"
							"HELDORDERITEM_KEY            		INTEGER NOT NULL,"
							"ITEM_NAME                    		VARCHAR(50),"
							"ITEM_ID                      		INTEGER,"
							"SIZE_NAME                    		VARCHAR(30),"
							"COURSE_NAME                  		VARCHAR(25),"
							"ITEM_CATEGORY                		VARCHAR(40),"
							"SECURITY_REF                 		INTEGER,"
							"QTY                          		NUMERIC(15,4),"
							"ITEM_KITCHEN_NAME            		VARCHAR(200),"
							"SIZE_KITCHEN_NAME            		VARCHAR(200),"
							"COURSE_KITCHEN_NAME         		VARCHAR(200),"
							"CATEGORY_KEY                 		INTEGER NOT NULL,"
							"SERVINGCOURSES_KEY           		INTEGER,"
							"CANCEL_STOCK_TYPE            		INTEGER,"
							"PLU                          		INTEGER,"
							"PRIMARY KEY (HELDORDERSIDEITEM_KEY),"
							"FOREIGN KEY (HELDORDERITEM_KEY)  	REFERENCES HELDORDERITEMS(HELDORDERITEM_KEY),"
							"FOREIGN KEY (CATEGORY_KEY)       	REFERENCES ARCCATEGORIES(CATEGORY_KEY),"
							"FOREIGN KEY (SERVINGCOURSES_KEY) 	REFERENCES SERVINGCOURSES(SERVINGCOURSES_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMOPTION");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERITEMOPTION"
						"("
							"HELDORDEROPTION_KEY				INTEGER,"
							"HELDORDERITEM_KEY					INTEGER,"
							"OPTION_ID                    		INTEGER,"
							"OPTION_NAME                  		VARCHAR(28),"
							"REDIR_ID							INTEGER,"
							"PLUS                         		T_TRUEFALSE DEFAULT 'F',"
							"OPTION_KITCHEN_NAME				VARCHAR(200),"
							"PRIMARY KEY (HELDORDEROPTION_KEY),"
							"FOREIGN KEY (HELDORDERITEM_KEY)	REFERENCES HELDORDERITEMS(HELDORDERITEM_KEY)"
						 ")";
					IBQuery1->ExecQuery();
				}

				itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERPATRONCOUNT");
				if (itTable == TableNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text =
						"CREATE TABLE HELDORDERPATRONCOUNT"
						"("
							"HELDORDERPATRONCOUNT_KEY	INTEGER,"
							"HELDORDER_KEY				INTEGER,"
							"PATRON_COUNT				INTEGER,"
							"PATRON_TYPE				VARCHAR(40),"
							"PRIMARY KEY (HELDORDERPATRONCOUNT_KEY),"
							"FOREIGN KEY (HELDORDER_KEY) REFERENCES HELDORDERS(HELDORDER_KEY)"
						")";
					IBQuery1->ExecQuery();
				}

				Database::TGeneratorNames GeneratorNames;
				Util->GetGeneratorNames(GeneratorNames);

				Database::TGeneratorNames::iterator itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_ACTIVECHIT");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_ACTIVECHIT";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_ACTIVECHIT TO 0";
					IBQuery1->ExecQuery();
				}

				itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERS");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_HELDORDERS";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_HELDORDERS TO 0";
					IBQuery1->ExecQuery();
				}

				itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDEROPTION");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_HELDORDEROPTION";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_HELDORDEROPTION TO 0";
					IBQuery1->ExecQuery();
				}

				itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERPATRONCOUNT");
				if (itGen == GeneratorNames.end())
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "CREATE GENERATOR GEN_HELDORDERPATRONCOUNT";
					IBQuery1->ExecQuery();
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SET GENERATOR GEN_HELDORDERPATRONCOUNT TO 0";
					IBQuery1->ExecQuery();
				}

				Database::TViewNames ViewNames;
				Util->GetViewNames(ViewNames);

				Database::TViewNames::iterator itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSVIEW");
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

//				itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSIDESVIEW");
//				if (itView == ViewNames.end())
//				{
//					IBQuery1->Close();
//					IBQuery1->SQL->Text =
//						"CREATE VIEW HELDORDERSIDESVIEW AS "
//						"SELECT "
//							"HOSI.HELDORDERSIDEITEM_KEY,"
//							"HOSI.HELDORDERITEM_KEY,"
//							"HOV.TAB_KEY,"
//							"HOV.TAB_TYPE,"
//							"HOV.TAB_NAME,"
//							"HOV.PARTY_NAME,"
//							"HOV.TABLE_NUMBER,"
//							"HOV.TABLE_NAME,"
//							"HOV.ORDER_TYPE,"
//							"HOV.TERMINAL_NAME,"
//							"HOV.ORDER_LOCATION,"
//							"HOV.ORDER_TYPE_MESSAGE,"
//							"HOV.CONTACTS_KEY,"
//							"HOV.ACTIVECHITNUMBER_KEY,"
//							"HOSI.ITEM_NAME,"
//							"HOSI.ITEM_ID,"
//							"HOSI.SIZE_NAME,"
//							"HOV.NOTE,"
//							"HOV.SEATNO,"
//							"HOV.TRANSNO,"
//							"HOV.MENU_NAME,"
//							"HOSI.COURSE_NAME,"
//							"HOV.TIME_STAMP,"
//							"HOV.MASTER_CONTAINER,"
//							"HOV.SETMENU_MASK,"
//							"HOV.SETMENU_GROUP,"
//							"HOSI.ITEM_CATEGORY,"
//							"HOSI.SECURITY_REF,"
//							"HOV.TIME_KEY,"
//							"HOSI.QTY,"
//							"HOSI.ITEM_KITCHEN_NAME,"
//							"HOSI.SIZE_KITCHEN_NAME,"
//							"HOSI.COURSE_KITCHEN_NAME,"
//							"HOSI.CATEGORY_KEY,"
//							"HOSI.SERVINGCOURSES_KEY,"
//							"HOSI.CANCEL_STOCK_TYPE,"
//							"HOV.ITEM_TYPE,"
//							"HOV.MENU_ITEM_KEY,"
//							"HOSI.PLU "
//						"FROM "
//							"HELDORDERSIDEITEMS HOSI "
//							"INNER JOIN HELDORDERSVIEW HOV "
//							"ON HOSI.HELDORDERITEM_KEY = HOV.HELDORDERITEM_KEY";
//					IBQuery1->ExecQuery();
//				}

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

void DB_5_10::Down_5_10(Database::TDBControl &DBControl)
{
	AnsiString UpdateFromVersion = "5.10";
	AnsiString UpdateVersion = "5.9";

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

		Database::TTableNames::iterator itTable = std::find(TableNames.begin(), TableNames.end(), "ACTIVECHITNUMBER");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE ACTIVECHITNUMBER";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERS");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERS";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMS");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERITEMS";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERSIDEITEMS");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERSIDEITEMS";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERITEMOPTION");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERITEMOPTION";
			IBQuery1->ExecQuery();
		}

		itTable = std::find(TableNames.begin(), TableNames.end(), "HELDORDERPATRONCOUNT");
		if (itTable != TableNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DROP TABLE HELDORDERPATRONCOUNT";
			IBQuery1->ExecQuery();
		}

		Database::TGeneratorNames::iterator itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_ACTIVECHIT");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_ACTIVECHIT'";
			IBQuery1->ExecQuery();
		}

		itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERS");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_HELDORDERS'";
			IBQuery1->ExecQuery();
		}

		itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDEROPTION");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_HELDORDEROPTION'";
			IBQuery1->ExecQuery();
		}

		itGen = std::find(GeneratorNames.begin(), GeneratorNames.end(), "GEN_HELDORDERPATRONCOUNT");
		if (itGen != GeneratorNames.end())
		{
			IBQuery1->Close();
			IBQuery1->SQL->Text = "DELETE FROM RDB$GENERATORS "
								  "WHERE RDB$GENERATOR_NAME = 'GEN_HELDORDERPATRONCOUNT'";
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

//        itView = std::find(ViewNames.begin(), ViewNames.end(), "HELDORDERSIDESVIEW");
//		if (itView != ViewNames.end())
//		{
//			IBQuery1->Close();
//			IBQuery1->SQL->Text = "DROP VIEW HELDORDERSIDESVIEW";
//			IBQuery1->ExecQuery();
//		}

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
