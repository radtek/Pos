//---------------------------------------------------------------------------


#pragma hdrstop

//---------------------------------------------------------------------------

#include "DBHeldOrder.h"
#include "DBActiveChit.h"
#include "DBOrder.h"
#include "ItemCompleteSub.h"
#include "MMLogging.h"
#include "OrderUtils.h"
#include "DBTab.h"
#include "DBTables.h"
#include "DBSecurity.h"
#include "DBRooms.h"
#include "DBThirdPartyCodes.h"
#include "DBSecurity.h"
#include "MMUtilFunc.h"
#include "DBWebUtil.h"

#include "ItemSizeCategory.h"
#include "ItemRecipe.h"
#include "enumMenu.h"

#include "SendForm.h"

#pragma package(smart_init)

TDBHeldOrder::TDBHeldOrder()
{
}

TDBHeldOrder::~TDBHeldOrder()
{
}

//---------------------------------------------------------------------------
// Store Order Methods
//---------------------------------------------------------------------------

void TDBHeldOrder::HoldOrders(Database::TDBTransaction &DBTransaction, TList *OrderObjects, std::vector<TPatronType> Patrons, int ActiveChitNumberKey)
{
	try
	{
		int HeldOrderKey = -1;
		std::set<long> SeatCounter;
		for(int i = 0 ; i < OrderObjects->Count ; i++)
		{
			TItemComplete *Order = (TItemComplete *)OrderObjects->Items[i];
			if (ProcessOrderToHold(DBTransaction,Order))
			{
				// Use the first order for the general order information

				if (HeldOrderKey == -1)
				{
						HeldOrderKey = SetHeldOrder(DBTransaction, Order, ActiveChitNumberKey);
				}
				SetHeldOrderItem(DBTransaction,Order, HeldOrderKey, ActiveChitNumberKey);

                if (Order->SeatNo > 0)
				{
					SeatCounter.insert(Order->SeatNo);
				}
			}
		}
		if (HeldOrderKey != -1)
		{
			SetHeldOrderPatronCount(DBTransaction, Patrons, HeldOrderKey, SeatCounter.size());
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBHeldOrder::ProcessOrderToHold(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	bool Process = Order->OrderType == NormalOrder && Order->TabType != TabNone;
	try
	{
		if (Process)
		{
			switch (Order->TabType)
			{
				case TabNormal:
				case TabStaff:
				case TabMember:
					//Get tab number
					break;
				case TabTableSeat:
                	if(TDBTables::Valid(Order->TableNo,Order->SeatNo))
					{
						int SeatKey = TDBTables::GetOrCreateSeat(DBTransaction,Order->TableNo,Order->SeatNo);
						Order->TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));

						TDBTables::SetSeatTab(DBTransaction,SeatKey,Order->TabKey);

						Order->TabName = TDBTab::GetTabName(DBTransaction,Order->TabKey);
						Order->TabContainerName = TDBTables::GetTableName(DBTransaction,Order->TableNo);

						if(Order->TabName == "")
						{
							Order->TabName = TDBTables::GetNameMinor(DBTransaction,Order->TableNo,Order->SeatNo);
							TDBTab::SetTabName(DBTransaction,Order->TabKey,Order->TabName);
						}

						TDBTab::SetTabType(DBTransaction,Order->TabKey, TabTableSeat);
						Order->PartyName = TDBTables::GetPartyName(DBTransaction,Order->TableNo);
					}
					else
					{
						throw Exception("ProcessOrder Invalid Zone Options");
					}
					break;
				case TabRoom:
					Order->TabKey 			= TDBRooms::FindOrCreateRoomTab(DBTransaction,Order->RoomNo);
					Order->PartyName 		= TDBRooms::GetPartyName(DBTransaction,Order->RoomNo);
					Order->TabContainerName = TDBRooms::GetRoomName(DBTransaction,Order->RoomNo);
					Order->TabName 			= Order->PartyName;

					TDBRooms::SetRoomStatus(DBTransaction,Order->RoomNo,eOccupied);
					break;
				case TabWeb:
					Order->WebKey = TDBWebUtil::FindOrCreateWebOrder(DBTransaction,Order->WebKey);

					TDBWebUtil::SetOrderGUID(DBTransaction,Order->WebKey,Order->WebID);

					Order->TabKey = TDBWebUtil::GetTabKeyByWebOrderKey(DBTransaction,Order->WebKey);
					if(Order->TabKey == 0)
					{
						Order->TabKey = TDBTab::GetOrCreateTab(DBTransaction,Order->TabKey);
						TDBTab::SetTabType(DBTransaction,Order->TabKey, TabWeb);
						TDBTab::SetTabName(DBTransaction,Order->TabKey, Order->TabName);
						TDBWebUtil::SetTabKey(DBTransaction,Order->WebKey,Order->TabKey);
						TDBTab::SetTabCredit(DBTransaction,Order->TabKey,TDBWebUtil::GetWebOrderPrePaid(DBTransaction,Order->WebKey));
					}
					Order->TabContainerName = TDBWebUtil::GetOrderGUID(DBTransaction,Order->WebKey);
					break;
				default:
					break;
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Tab Data Type :" + IntToStr(Order->TabType) +
					 " Tab Key " + IntToStr(Order->TabKey));
		throw;
	}
    return Process;
}

int TDBHeldOrder::SetHeldOrder(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int ActiveChitNumberKey)
{
	try
	{
		int MasterOrderKey = -1;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Check if the order is already stored in the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"HO.HELDORDER_KEY "
			"FROM "
				"HELDORDERS HO "
				"INNER JOIN HELDORDERITEMS HOI "
				"ON HO.HELDORDER_KEY = HOI.HELDORDER_KEY "
				"INNER JOIN DEVICES D "
				"ON HO.DEVICE_KEY = D.DEVICE_KEY "
			"WHERE "
				"HOI.ITEM_ID = :ITEM_ID "
				"AND HOI.SIZE_NAME = :SIZE_NAME "
				"AND D.DEVICE_NAME = :TERMINAL_NAME "
				"AND HOI.TRANSNO = :TRANSNO";
		IBInternalQuery->ParamByName("ITEM_ID"		)->AsInteger = Order->Item_ID;
		IBInternalQuery->ParamByName("SIZE_NAME"	)->AsString  = Order->Size;
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString  = Order->Terminal;
		IBInternalQuery->ParamByName("TRANSNO"		)->AsString  = Order->TransNo;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount == 0)
		{
			// Generate key for order

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDERS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			MasterOrderKey = IBInternalQuery->Fields[0]->AsInteger;

			// Get the device key for the terminal ordered from

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"SELECT "
					"D.DEVICE_KEY "
				"FROM "
					"DEVICES D INNER JOIN LOCATIONS L "
					"ON D.LOCATION_KEY = L.LOCATION_KEY "
				"WHERE "
					"D.DEVICE_NAME = :TERMINAL_NAME AND "
					"L.NAME = :LOCATION_NAME";
			IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = Order->Terminal;
			IBInternalQuery->ParamByName("LOCATION_NAME")->AsString = Order->OrderedLocation;
			IBInternalQuery->ExecQuery();
			int DeviceKey = IBInternalQuery->Fields[0]->AsInteger;

			// Add the order to the database

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO HELDORDERS "
				"("
					"HELDORDER_KEY,"
					"PARTY_NAME,"
					"TABLE_NUMBER,"
					"TABLE_NAME,"
					"ORDER_TYPE,"
					"DEVICE_KEY,"
					"ORDER_TYPE_MESSAGE,"
					"CONTACTS_KEY,"
					"ACTIVECHITNUMBER_KEY"
				")"
				" VALUES "
				"("
					":HELDORDER_KEY,"
					":PARTY_NAME,"
					":TABLE_NUMBER,"
					":TABLE_NAME,"
					":ORDER_TYPE,"
					":DEVICE_KEY,"
					":ORDER_TYPE_MESSAGE,"
					":CONTACTS_KEY,"
					":ACTIVECHITNUMBER_KEY"
				")";
			IBInternalQuery->ParamByName("HELDORDER_KEY"	 		)->AsInteger  = MasterOrderKey;
			IBInternalQuery->ParamByName("PARTY_NAME"		 		)->AsString   = Order->PartyName;
			switch(Order->TabType)
			{
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				case TabNormal:
				case TabStaff:
				case TabMember:
				case TabTableSeat:
					IBInternalQuery->ParamByName("TABLE_NUMBER"		)->AsInteger = Order->TableNo;
					IBInternalQuery->ParamByName("TABLE_NAME"  		)->AsString  = Order->TabContainerName.SubString(1,25);
					break;
				case TabRoom:
					IBInternalQuery->ParamByName("TABLE_NUMBER"		)->AsInteger = Order->RoomNo;
					IBInternalQuery->ParamByName("TABLE_NAME"  		)->AsString  = Order->TabContainerName;
					break;
			}
			IBInternalQuery->ParamByName("ORDER_TYPE"		 		)->AsInteger  = Order->OrderType;
			IBInternalQuery->ParamByName("DEVICE_KEY"		 		)->AsInteger  = DeviceKey;

			if (Order->OrderTypeMessage == "")
			{
				IBInternalQuery->ParamByName("ORDER_TYPE_MESSAGE"	)->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("ORDER_TYPE_MESSAGE"	)->AsString   = Order->OrderTypeMessage;
            }

			if (Order->ContactsKey == -1)
			{
				IBInternalQuery->ParamByName("CONTACTS_KEY"			)->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("CONTACTS_KEY"			)->AsInteger  = Order->ContactsKey;
            }

			if(ActiveChitNumberKey == -1)
			{
				IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY"	)->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY"	)->AsInteger  = ActiveChitNumberKey;
            }

			IBInternalQuery->ExecQuery();

			/* 	this is just a workaround to set the generated active chit number key into the model
				this property is only useful in hold and send function. ActiveChitKey is automatically generated each time
				an order is putting on hold and later we need this active chit key to compare with previously held orders to find out their
				relavance.
			*/
			Order->SetActiveChitNumberKey(ActiveChitNumberKey);
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "WARNING");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "HeldOrder Already Exists");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "HeldOrder already added with the Unique Key.");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Device :" + Order->Terminal);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Item :" + Order->Item);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Size :" + Order->Size);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Trans No :" + Order->TransNo);
			Order->DuplicateDetected = true;
		}
		return MasterOrderKey;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetHeldOrder Excep Serving Course Key " + IntToStr(Order->ServingCourse.ServingCourseKey));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetHeldOrder Excep Serving Course Name " + Order->ServingCourse.Name);

		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::SetHeldOrderPatronCount(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> Patrons, int HeldOrderKey, int SeatCount)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Add the patron counts to the database
		std::vector<TPatronType>::iterator it = Patrons.begin();
		if (it == Patrons.end())
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDERPATRONCOUNT, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int PatronCountKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO HELDORDERPATRONCOUNT "
				"("
					"HELDORDERPATRONCOUNT_KEY,"
					"HELDORDER_KEY,"
					"PATRON_COUNT,"
					"PATRON_TYPE"
				")"
				" VALUES "
				"("
					":HELDORDERPATRONCOUNT_KEY,"
					":HELDORDER_KEY,"
					":PATRON_COUNT,"
					":PATRON_TYPE"
				")";
			IBInternalQuery->ParamByName("HELDORDERPATRONCOUNT_KEY"	)->AsInteger = PatronCountKey;
			IBInternalQuery->ParamByName("HELDORDER_KEY"			)->AsInteger = HeldOrderKey;
			IBInternalQuery->ParamByName("PATRON_COUNT"				)->AsInteger = SeatCount;
			IBInternalQuery->ParamByName("PATRON_TYPE"				)->AsString  = "Guest";
			IBInternalQuery->ExecQuery();
        }
		for (; it != Patrons.end(); it++)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDERPATRONCOUNT, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int PatronCountKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO HELDORDERPATRONCOUNT "
				"("
					"HELDORDERPATRONCOUNT_KEY,"
					"HELDORDER_KEY,"
					"PATRON_COUNT,"
					"PATRON_TYPE"
				")"
				" VALUES "
				"("
					":HELDORDERPATRONCOUNT_KEY,"
					":HELDORDER_KEY,"
					":PATRON_COUNT,"
					":PATRON_TYPE"
				")";
			IBInternalQuery->ParamByName("HELDORDERPATRONCOUNT_KEY"	)->AsInteger = PatronCountKey;
			IBInternalQuery->ParamByName("HELDORDER_KEY"			)->AsInteger = HeldOrderKey;
			IBInternalQuery->ParamByName("PATRON_COUNT"				)->AsInteger = it->Count;
			IBInternalQuery->ParamByName("PATRON_TYPE"				)->AsString  = it->Name;
			IBInternalQuery->ExecQuery();
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::SetHeldOrderItem(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int HeldOrderKey, int ActiveChitNumberKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Get the category key from the item

		int PrimaryArcCatkey = TDBOrder::GetArchiveCategory(DBTransaction,Order->Categories->FinancialCategory);

		// Check if the item has a key and generate one if it doesn't

		if (Order->OrderKey == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			Order->OrderKey = IBInternalQuery->Fields[0]->AsInteger;
		}

		// Add the item to the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"INSERT INTO HELDORDERITEMS "
			"("
				"HELDORDERITEM_KEY,"
				"HELDORDER_KEY,"
				"ITEM_NAME,"
				"ITEM_ID,"
				"SIZE_NAME,"
				"NOTE,"
				"SEATNO,"
				"TRANSNO,"
				"MENU_NAME,"
				"COURSE_NAME,"
				"TIME_STAMP,"
				"MASTER_CONTAINER,"
				"SETMENU_MASK,"
				"SETMENU_GROUP,"
				//"ITEM_CATEGORY,"
				"SECURITY_REF,"
				"TIME_KEY,"
				"QTY,"
				"ITEM_KITCHEN_NAME,"
				"SIZE_KITCHEN_NAME,"
				"COURSE_KITCHEN_NAME,"
				"CATEGORY_KEY,"
				"SERVINGCOURSES_KEY,"
				//"CANCEL_STOCK_TYPE,"
				"ITEM_TYPE,"
				"MENU_ITEM_KEY,"
				"TAB_KEY,"
				"TAB_TYPE,"
				"TAB_NAME,"//,"
                //MM2038
                //this is for storing Cutomer's Phone Number
                "ID_NUMBER"
				//"PLU"
			")"
			" VALUES "
			"("
				":HELDORDERITEM_KEY,"
				":HELDORDER_KEY,"
				":ITEM_NAME,"
				":ITEM_ID,"
				":SIZE_NAME,"
				":NOTE,"
				":SEATNO,"
				":TRANSNO,"
				":MENU_NAME,"
				":COURSE_NAME,"
				":TIME_STAMP,"
				":MASTER_CONTAINER,"
				":SETMENU_MASK,"
				":SETMENU_GROUP,"
				//":ITEM_CATEGORY,"
				":SECURITY_REF,"
				":TIME_KEY,"
				":QTY,"
				":ITEM_KITCHEN_NAME,"
				":SIZE_KITCHEN_NAME,"
				":COURSE_KITCHEN_NAME,"
				":CATEGORY_KEY,"
				":SERVINGCOURSES_KEY,"
				//":CANCEL_STOCK_TYPE,"
				":ITEM_TYPE,"
				":MENU_ITEM_KEY,"
				":TAB_KEY,"
				":TAB_TYPE,"
				":TAB_NAME,"//,"
                //MM2038
                //this is for storing Cutomer's Phone Number
                ":ID_NUMBER"
				//":PLU"
			")";

		IBInternalQuery->ParamByName("HELDORDERITEM_KEY"	)->AsInteger  = Order->OrderKey;
		IBInternalQuery->ParamByName("HELDORDER_KEY"	   	)->AsInteger  = HeldOrderKey;
		IBInternalQuery->ParamByName("ITEM_NAME"			)->AsString   = Order->Item;
		IBInternalQuery->ParamByName("ITEM_ID"				)->AsInteger  = Order->Item_ID;
		IBInternalQuery->ParamByName("SIZE_NAME"			)->AsString   = Order->Size;
		IBInternalQuery->ParamByName("NOTE"					)->AsString   = Order->Note.SubString(1,78);
		IBInternalQuery->ParamByName("SEATNO"				)->AsInteger  = Order->SeatNo;
		IBInternalQuery->ParamByName("TRANSNO"				)->AsString   = Order->TransNo;
		IBInternalQuery->ParamByName("MENU_NAME"			)->AsString   = Order->MenuName;
		IBInternalQuery->ParamByName("COURSE_NAME"			)->AsString   = Order->Course;
		IBInternalQuery->ParamByName("TIME_STAMP"			)->AsDateTime = Order->TimeStamp;
		IBInternalQuery->ParamByName("MASTER_CONTAINER"		)->AsString   = Order->MasterContainer;
		IBInternalQuery->ParamByName("SETMENU_MASK"			)->AsInteger  = Order->SetMenuMask;
		IBInternalQuery->ParamByName("SETMENU_GROUP"		)->AsInteger  = Order->SetMenuGroup;
		//IBInternalQuery->ParamByName("ITEM_CATEGORY"		)->AsString   = "";
		IBInternalQuery->ParamByName("SECURITY_REF"			)->AsInteger  = Order->Security->GetSecurityRefNumber();
		IBInternalQuery->ParamByName("TIME_KEY"				)->AsInteger  = Order->TimeKey;
		IBInternalQuery->ParamByName("QTY"					)->AsCurrency = Order->GetQty();
		IBInternalQuery->ParamByName("ITEM_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(Order->ItemKitchenName);
		IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(Order->SizeKitchenName);
		IBInternalQuery->ParamByName("COURSE_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(Order->CourseKitchenName);
		IBInternalQuery->ParamByName("CATEGORY_KEY"			)->AsInteger  = PrimaryArcCatkey;
		IBInternalQuery->ParamByName("ITEM_TYPE"			)->AsInteger  = Order->ItemType;
		IBInternalQuery->ParamByName("SERVINGCOURSES_KEY"	)->AsInteger  = Order->ServingCourse.ServingCourseKey;

        if(Order->ItemOrderedFrom != NULL)
		{
			IBInternalQuery->ParamByName("MENU_ITEM_KEY"	)->AsInteger  = Order->ItemOrderedFrom->ItemKey;
		}
		else
		{
			IBInternalQuery->ParamByName("MENU_ITEM_KEY"	)->Clear();
		}
		switch(Order->TabType)
		{
			case TabHandheldCashAccount:
			case TabCashAccount:
			case TabNone:
				IBInternalQuery->ParamByName("TAB_KEY"		)->AsInteger  = 0;
				IBInternalQuery->ParamByName("TAB_TYPE"		)->AsInteger  = ActiveChitNumberKey == -1 ? TabHeld : TabNone;
				break;
			default:
				IBInternalQuery->ParamByName("TAB_KEY"		)->AsInteger  = Order->TabKey;
				IBInternalQuery->ParamByName("TAB_TYPE"		)->AsInteger  = Order->TabType;
				break;
		}
		IBInternalQuery->ParamByName("TAB_NAME"				)->AsString   = Order->TabName;
        //MM2038
	    IBInternalQuery->ParamByName("ID_NUMBER"	        )->AsString   = Order->IdNumber;
		//IBInternalQuery->ParamByName("PLU"				)->AsInteger = Order->PLU;
        IBInternalQuery->ExecQuery();

		// Add the item's Options to the database
		SetHeldOption(DBTransaction, Order);

		// Add the item's Sides to the database
		SetHeldOrderSideItems(DBTransaction, Order);

		// Add the item's Categories to the database
		SetHeldOrderItemCategory(DBTransaction, Order);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetOrder Excep Serving Course Key " + IntToStr(Order->ServingCourse.ServingCourseKey));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetOrder Excep Serving Course Name " + Order->ServingCourse.Name);

		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::SetHeldOrderSideItems(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	for (int i = 0; i < Order->SubOrders->Count; i++)
	{
		// Get the current SubOrder and catergory key to add to the database

		TItemCompleteSub *SubOrder = (TItemCompleteSub *)Order->SubOrders->Items[i];
		int ArcCatKey = TDBOrder::GetArchiveCategory(DBTransaction, SubOrder->Categories->FinancialCategory);

		// Check if the SubOrder has a key and give it one if it doesn't

		if (SubOrder->OrderKey == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			SubOrder->OrderKey = IBInternalQuery->Fields[0]->AsInteger;
		}

		// Set SubOrders key to the key of the order it is a side of

		SubOrder->MasterOrderKey = Order->OrderKey;

		// Add the SubOrder to the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"INSERT INTO HELDORDERSIDEITEMS"
			"("
				"HELDORDERSIDEITEM_KEY,"
				"HELDORDERITEM_KEY,"
				"ITEM_NAME,"
				"ITEM_ID,"
				"SIZE_NAME,"
				"COURSE_NAME,"
				//"ITEM_CATEGORY,"
				"SECURITY_REF,"
				"QTY,"
				"ITEM_KITCHEN_NAME,"
				"SIZE_KITCHEN_NAME,"
				"COURSE_KITCHEN_NAME,"
				"CATEGORY_KEY,"
				"SERVINGCOURSES_KEY"//,"
				//"CANCEL_STOCK_TYPE,"
				//"PLU"
			")"
			" VALUES "
			"("
				":HELDORDERSIDEITEM_KEY,"
				":HELDORDERITEM_KEY,"
				":ITEM_NAME,"
				":ITEM_ID,"
				":SIZE_NAME,"
				":COURSE_NAME,"
				":SECURITY_REF,"
				":QTY,"
				":ITEM_KITCHEN_NAME,"
				":SIZE_KITCHEN_NAME,"
				":COURSE_KITCHEN_NAME,"
				":CATEGORY_KEY,"
				":SERVINGCOURSES_KEY"//,"
				//":CANCEL_STOCK_TYPE,"
				//":PLU"
			")";
		IBInternalQuery->ParamByName("HELDORDERSIDEITEM_KEY")->AsInteger  = SubOrder->OrderKey;
		IBInternalQuery->ParamByName("HELDORDERITEM_KEY"	)->AsInteger  = SubOrder->MasterOrderKey;
		IBInternalQuery->ParamByName("ITEM_NAME"			)->AsString   = SubOrder->Item;
		IBInternalQuery->ParamByName("ITEM_ID"				)->AsInteger  = SubOrder->Item_ID;
		IBInternalQuery->ParamByName("SIZE_NAME"			)->AsString   = SubOrder->Size;
		IBInternalQuery->ParamByName("COURSE_NAME"			)->AsString   = SubOrder->Course;
		//IBInternalQuery->ParamByName("ITEM_CATEGORY"		)->AsString   = "";
		IBInternalQuery->ParamByName("SECURITY_REF"			)->AsInteger  = SubOrder->Security->GetSecurityRefNumber();
		IBInternalQuery->ParamByName("QTY"					)->AsCurrency = SubOrder->GetQty();
		IBInternalQuery->ParamByName("ITEM_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(SubOrder->ItemKitchenName);
		IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(SubOrder->SizeKitchenName);
		IBInternalQuery->ParamByName("COURSE_KITCHEN_NAME"	)->AsString   = UnicodeToUTF8AnsiString(SubOrder->CourseKitchenName);
		IBInternalQuery->ParamByName("CATEGORY_KEY"			)->AsInteger  = ArcCatKey;
		IBInternalQuery->ParamByName("SERVINGCOURSES_KEY"	)->AsInteger  = SubOrder->ServingCourse.ServingCourseKey;
		//IBInternalQuery->ParamByName("PLU"				)->AsInteger = SubOrder->PLU;
		IBInternalQuery->ExecQuery();

		// Add the side item's category to the database
		SetHeldOrderSideItemCategory(DBTransaction, SubOrder);
	}
}

void TDBHeldOrder::SetHeldOption(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	for (int i = 0; i < Order->OptionsSelected->Count; i++)
	{
		// Create a key for the option

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDEROPTION, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int OptionKey = IBInternalQuery->Fields[0]->AsInteger;

		// Get the current Option to add to the database

		TItemOption *CurrentOption = Order->OptionsSelected->OptionGet(i);

		// Add the Option to the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"INSERT INTO HELDORDERITEMOPTION"
			"("
				"HELDORDEROPTION_KEY,"
				"HELDORDERITEM_KEY,"
				"OPTION_ID,"
				"OPTION_NAME,"
				"PLUS,"
				"OPTION_KITCHEN_NAME"
			")"
			" VALUES "
				"("
				":HELDORDEROPTION_KEY,"
				":HELDORDERITEM_KEY,"
				":OPTION_ID,"
				":OPTION_NAME,"
				":PLUS,"
				":OPTION_KITCHEN_NAME"
			")";
		IBInternalQuery->ParamByName("HELDORDEROPTION_KEY"	)->AsInteger = OptionKey;
		IBInternalQuery->ParamByName("HELDORDERITEM_KEY"	)->AsInteger = Order->OrderKey;
		IBInternalQuery->ParamByName("OPTION_ID"			)->AsInteger = CurrentOption->OptionID;
		IBInternalQuery->ParamByName("OPTION_NAME"			)->AsString  = CurrentOption->KitchenName != "" ? CurrentOption->KitchenName : CurrentOption->Name;
		IBInternalQuery->ParamByName("PLUS"					)->AsString  = CurrentOption->IsPlus ? "T" : "F";
		IBInternalQuery->ParamByName("OPTION_KITCHEN_NAME"	)->AsString  = CurrentOption->KitchenName;
        IBInternalQuery->ExecQuery();
    }
}

void TDBHeldOrder::SetHeldOrderItemCategory(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	for (int i = 0; i < Order->Categories->Count; i++)
	{
		// Generate key for the item category

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDERITEMCAT, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int ItemCategoryKey = IBInternalQuery->Fields[0]->AsInteger;

		// Get order category to store

		TItemSizeCategory *CurrentCat = Order->Categories->CategoryGet(i);
		CurrentCat->CategoryKey = TDBOrder::GetArchiveCategory(DBTransaction, CurrentCat->Category);

		// Store link to category

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"INSERT INTO HELDORDERITEMCATEGORY "
			"("
				"HELDORDERITEMCATEGORY_KEY,"
				"HELDITEM_KEY,"
				"CATEGORY_KEY"
			")"
			" VALUES "
			"("
				":HELDORDERITEMCATEGORY_KEY,"
				":HELDITEM_KEY,"
				":CATEGORY_KEY"
			")";
		IBInternalQuery->ParamByName("HELDORDERITEMCATEGORY_KEY")->AsInteger = ItemCategoryKey;
		IBInternalQuery->ParamByName("HELDITEM_KEY"	   			)->AsInteger = Order->OrderKey;
		IBInternalQuery->ParamByName("CATEGORY_KEY"				)->AsInteger = CurrentCat->CategoryKey;
		IBInternalQuery->ExecQuery();
	}
}

void TDBHeldOrder::SetHeldOrderSideItemCategory(Database::TDBTransaction &DBTransaction, TItemCompleteSub *SubOrder)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	for (int i = 0; i < SubOrder->Categories->Count; i++)
	{
		// Generate key for the side item category

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HELDORDERSIDEITEMCAT, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int SideItemCategoryKey = IBInternalQuery->Fields[0]->AsInteger;

		// Get order category to store

		TItemSizeCategory *CurrentCat = SubOrder->Categories->CategoryGet(i);
		CurrentCat->CategoryKey = TDBOrder::GetArchiveCategory(DBTransaction, CurrentCat->Category);

		// Store link to category

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"INSERT INTO HELDORDERSIDEITEMCATEGORY "
			"("
				"HELDORDERSIDEITEMCATEGORY_KEY,"
				"HELDITEM_KEY,"
				"CATEGORY_KEY"
			")"
			" VALUES "
			"("
				":HELDORDERSIDEITEMCATEGORY_KEY,"
				":HELDITEM_KEY,"
				":CATEGORY_KEY"
			")";
		IBInternalQuery->ParamByName("HELDORDERSIDEITEMCATEGORY_KEY")->AsInteger = SideItemCategoryKey;
		IBInternalQuery->ParamByName("HELDITEM_KEY"					)->AsInteger = SubOrder->OrderKey;
		IBInternalQuery->ParamByName("CATEGORY_KEY"					)->AsInteger = CurrentCat->CategoryKey;
		IBInternalQuery->ExecQuery();
	}
}

//---------------------------------------------------------------------------
// Retrieve Order Methods
//---------------------------------------------------------------------------

void TDBHeldOrder::GetHeldOrdersFromChit(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int ActiveChitNumberKey)
{
	GetHeldOrdersFromIndex(DBTransaction, Orders, ChitNumber, Patrons, "ACTIVECHITNUMBER_KEY", ActiveChitNumberKey);
}

void TDBHeldOrder::GetHeldOrdersFromTable(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int TableNumber)
{
	GetHeldOrdersFromIndex(DBTransaction, Orders, ChitNumber, Patrons, "TABLE_NUMBER", TableNumber);
}

void TDBHeldOrder::GetHeldOrdersFromTab(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int TabKey)
{
	GetHeldOrdersFromIndex(DBTransaction, Orders, ChitNumber, Patrons, "TAB_KEY", TabKey);
}

void TDBHeldOrder::GetHeldOrdersFromKey(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, int HeldOrderKey)
{
	GetHeldOrdersFromIndex(DBTransaction, Orders, ChitNumber, Patrons, "HELDORDER_KEY", HeldOrderKey);
}

void TDBHeldOrder::GetHeldOrdersFromIndex(Database::TDBTransaction &DBTransaction, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons, UnicodeString IndexName, int IndexValue)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		//Select Stored Order with the Key Provided

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"HELDORDER_KEY,"
				"HELDORDERITEM_KEY,"
				"TAB_KEY,"
				"TAB_TYPE,"
				"TAB_NAME,"
                //MM2038
                "ID_NUMBER,"
				"PARTY_NAME,"
				"TABLE_NUMBER,"
				"TABLE_NAME,"
				"ORDER_TYPE,"
				"TERMINAL_NAME,"
				"ORDER_TYPE_MESSAGE,"
				"CONTACTS_KEY,"
				"ACTIVECHITNUMBER_KEY,"
				"ITEM_NAME,"
				"ITEM_ID,"
				"SIZE_NAME,"
				"NOTE,"
				"SEATNO,"
				"TRANSNO,"
				"MENU_NAME,"
				"COURSE_NAME,"
				"TIME_STAMP,"
				"MASTER_CONTAINER,"
				"SETMENU_MASK,"
				"SETMENU_GROUP,"
				"ITEM_CATEGORY,"
				"SECURITY_REF,"
				"TIME_KEY,"
				"QTY,"
				"ITEM_KITCHEN_NAME,"
				"SIZE_KITCHEN_NAME,"
				"COURSE_KITCHEN_NAME,"
				"CATEGORY_KEY,"
				"SERVINGCOURSES_KEY,"
				"CANCEL_STOCK_TYPE,"
				"ITEM_TYPE,"
				"MENU_ITEM_KEY,"
				"PLU,"
				"MENU_KEY "
			"FROM "
				"HELDORDERSVIEW HO "
			"WHERE "
				+ IndexName + " = :" + IndexName;
		IBInternalQuery->ParamByName(IndexName)->AsInteger = IndexValue;
		IBInternalQuery->ExecQuery();

		// Store Held Order key for loading patron count

		int HeldOrderKey = IBInternalQuery->FieldByName("HELDORDER_KEY")->AsInteger;

		// Load the Held Order Using the Information Retrieved

		LoadHeldOrders(DBTransaction, IBInternalQuery, Orders, ChitNumber, Patrons);
		LoadHeldOrderPatronCount(DBTransaction, Patrons, HeldOrderKey);

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::LoadHeldOrders(Database::TDBTransaction &DBTransaction, TIBSQL *OrderTable, TList *Orders, TChitNumber &ChitNumber, std::vector<TPatronType> &Patrons)
{
	for (; !OrderTable->Eof; OrderTable->Next())
	{
		// Create new Order item and add it to the orders list

		TItemComplete* Order = new TItemComplete;
		Orders->Add(Order);

		// Set values of the new order

		Order->OrderKey 		 = OrderTable->FieldByName("HELDORDERITEM_KEY"	)->AsInteger;
		Order->TabKey			 = OrderTable->FieldByName("TAB_KEY"			)->AsInteger;
		Order->TabType			 = OrderTable->FieldByName("TAB_TYPE"			)->AsInteger;
		Order->TabName			 = OrderTable->FieldByName("TAB_NAME"			)->AsString;
        //MM2038
        //the variable here is following the name in database itself (get permission to use this one not creating the new one)
	    Order->IdNumber			 = OrderTable->FieldByName("ID_NUMBER"			)->AsString;
		Order->PartyName		 = OrderTable->FieldByName("PARTY_NAME"		    )->AsString;
		Order->TableNo			 = OrderTable->FieldByName("TABLE_NUMBER"		)->AsInteger;
		Order->TabContainerName	 = OrderTable->FieldByName("TABLE_NAME"			)->AsString;
		Order->Terminal			 = OrderTable->FieldByName("TERMINAL_NAME"		)->AsString;
		Order->Item				 = OrderTable->FieldByName("ITEM_NAME"			)->AsString;
		Order->Item_ID			 = OrderTable->FieldByName("ITEM_ID"			)->AsInteger;
		Order->Size				 = OrderTable->FieldByName("SIZE_NAME"			)->AsString;
		Order->Note				 = OrderTable->FieldByName("NOTE"				)->AsString;
		Order->SeatNo			 = OrderTable->FieldByName("SEATNO"				)->AsInteger;
		Order->TransNo  		 = OrderTable->FieldByName("TRANSNO"			)->AsString;
		Order->MenuName			 = OrderTable->FieldByName("MENU_NAME"			)->AsString;
		Order->Course			 = OrderTable->FieldByName("COURSE_NAME"		)->AsString;
		Order->TimeStamp		 = OrderTable->FieldByName("TIME_STAMP"			)->AsDateTime;
		Order->MasterContainer	 = OrderTable->FieldByName("MASTER_CONTAINER"	)->AsString;
		Order->SetMenuMask		 = OrderTable->FieldByName("SETMENU_MASK"		)->AsInteger;
		Order->SetMenuGroup		 = OrderTable->FieldByName("SETMENU_GROUP"		)->AsInteger;
		Order->TimeKey			 = OrderTable->FieldByName("TIME_KEY"			)->AsInteger;
		Order->OriginalItemKey	 = OrderTable->FieldByName("MENU_ITEM_KEY"		)->AsInteger;
		Order->ItemKey	 		 = OrderTable->FieldByName("MENU_ITEM_KEY"		)->AsInteger;
		Order->PLU				 = OrderTable->FieldByName("PLU"				)->AsInteger;
		Order->MenuKey			 = OrderTable->FieldByName("MENU_KEY"			)->AsInteger;

		// Set Kitchen Names

		Order->ItemKitchenName	 = UTF8ToUnicodeString((AnsiString)OrderTable->FieldByName("ITEM_KITCHEN_NAME"	)->AsString);
		Order->SizeKitchenName 	 = UTF8ToUnicodeString((AnsiString)OrderTable->FieldByName("SIZE_KITCHEN_NAME"	)->AsString);
		Order->CourseKitchenName = UTF8ToUnicodeString((AnsiString)OrderTable->FieldByName("COURSE_KITCHEN_NAME")->AsString);

		// Set Item Type

		Order->ItemType	= (TItemType)OrderTable->FieldByName("ITEM_TYPE")->AsInteger;

		// Set Financial Category Key

		Order->Categories->FinancialCategoryKey = OrderTable->FieldByName("CATEGORY_KEY")->AsInteger;

		// Set Security Reference Number of the new order

		Order->Security->SetSecurityRefNumber(OrderTable->FieldByName("SECURITY_REF")->AsInteger);

		// Set Serving Course Key of the new order

		Order->ServingCourse.ServingCourseKey = OrderTable->FieldByName("SERVINGCOURSES_KEY")->AsInteger;

		// Set Quantity of the new order

		if (OrderTable->FieldByName("QTY")->IsNull)
		{
			Order->SetQty(1);
		}
		else
		{
			Order->SetQty(OrderTable->FieldByName("QTY")->AsFloat);
		}

		// Set Set Menu Master

		Order->SetMenuMaster = TST_PROMO_MASTER(Order->SetMenuMask);

		// Check if there is a active chit number

		bool IsActiveChitNumber = !OrderTable->FieldByName("ACTIVECHITNUMBER_KEY")->IsNull;

		// Set the Tab Name to the terminal ordered from if it is unindexed (i.e No Table Number, Chit Number or Tab Key)

		if (Order->TabType == TabHeld)
		{
			Order->TabName = Order->Terminal;
		}

		// Set the Customer Name and Order Type to default values if no information is stored on them

		if (OrderTable->FieldByName("ORDER_TYPE_MESSAGE")->IsNull)
		{
			Order->OrderTypeMessage = "";
		}
		else
		{
			Order->OrderTypeMessage = OrderTable->FieldByName("ORDER_TYPE_MESSAGE")->AsString;
		}

		if (OrderTable->FieldByName("CONTACTS_KEY")->IsNull)
		{
            Order->ContactsKey = -1;
		}
		else
		{
			Order->ContactsKey = OrderTable->FieldByName("CONTACTS_KEY")->AsInteger;
		}

		// Get the Chit Number of the Current Order if there is one

		if (IsActiveChitNumber)
		{
			int ChitKey = OrderTable->FieldByName("ACTIVECHITNUMBER_KEY")->AsInteger;
			ChitNumber = TDBActiveChit::LoadChitNumberFromKey(DBTransaction, ChitKey);
        }

		// Get Held Options and Sides of the Current Order

		LoadHeldOrderOptions(DBTransaction, Order);
		LoadHeldOrderSides(DBTransaction, Order);
		LoadHeldOrderServingCourse(DBTransaction, Order->ServingCourse);
		LoadHeldOrderPrinting(DBTransaction, Order);
		LoadHeldOrderSecurity(DBTransaction, Order->Security);
		LoadHeldOrderItemCategory(DBTransaction, Order->OrderKey, Order);

		// Get Held Order's Item Ordered From


		if (Order->OriginalItemKey != NULL)
		{
			Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);
		}

	}
}

void TDBHeldOrder::LoadHeldOrderOptions(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *OptionsTable = DBTransaction.Query(DBTransaction.AddQuery());

	// Get all options for the current order from the database

	OptionsTable->Close();
	OptionsTable->SQL->Clear();
	OptionsTable->SQL->Text =
		"SELECT "
			"HELDORDEROPTION_KEY,"
			"OPTION_ID,"
			"OPTION_NAME,"
			"REDIR_ID,"
			"PLUS,"
			"OPTION_KITCHEN_NAME "
		"FROM "
			"HELDORDERITEMOPTION "
		"WHERE "
			"HELDORDERITEM_KEY = :HELDORDERITEM_KEY";
	OptionsTable->ParamByName("HELDORDERITEM_KEY")->AsInteger = Order->OrderKey;
	OptionsTable->ExecQuery();

	// Recreate the option object from the information in the database

	for(; !OptionsTable->Eof; OptionsTable->Next())
	{
		// Create new option object and add it to the order's option list

		TItemOption *Option = new TItemOption;
		Option->Owner = Order->OptionsSelected;
		Order->OptionsSelected->OptionAdd(Option);

		// Set the values of the new option using information from the database

		Option->OptionKey 	= OptionsTable->FieldByName("HELDORDEROPTION_KEY")->AsInteger;
		Option->Name		= OptionsTable->FieldByName("OPTION_NAME")->AsString;
		Option->IsPlus		= OptionsTable->FieldByName("PLUS")->AsString.UpperCase() == "T";

		// Set Option ID

		Option->OptionID	= (unsigned short)OptionsTable->FieldByName("OPTION_ID")->AsInteger;

		//Set Kitchen Name

		Option->KitchenName = UTF8ToUnicodeString((AnsiString)OptionsTable->FieldByName("OPTION_KITCHEN_NAME")->AsString);
		if (Option->KitchenName == UnicodeString(""))
		{
			Option->KitchenName = Option->Name;
		}

		// Set other values to the default settings

		Option->Enabled 	= true;
		Option->Flags		= 0;
		Option->GroupNumber	= 0;
		Option->PlusOption	= 0;
		Option->MinusOption = 0;
		Option->ForcedMask	= 0;
		Option->Palm_ID		= 0;
	}
}

void TDBHeldOrder::LoadHeldOrderSides(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *OrderSidesTable = DBTransaction.Query(DBTransaction.AddQuery());

	// Get all order sides for current order from table

	OrderSidesTable->Close();
	OrderSidesTable->SQL->Clear();
	OrderSidesTable->SQL->Text =
		"SELECT "
			"HELDORDERSIDEITEM_KEY,"
			"HELDORDERITEM_KEY,"
			"ITEM_NAME,"
			"ITEM_ID,"
			"SIZE_NAME,"
			"COURSE_NAME,"
			"ITEM_CATEGORY,"
			"SECURITY_REF,"
			"QTY,"
			"ITEM_KITCHEN_NAME,"
			"SIZE_KITCHEN_NAME,"
			"COURSE_KITCHEN_NAME,"
			"CATEGORY_KEY,"
			"SERVINGCOURSES_KEY,"
			"CANCEL_STOCK_TYPE,"
			"PLU "
		"FROM "
			"HELDORDERSIDEITEMS "
		"WHERE "
			"HELDORDERITEM_KEY = :HELDORDERITEM_KEY";
	OrderSidesTable->ParamByName("HELDORDERITEM_KEY")->AsInteger = Order->OrderKey;
	OrderSidesTable->ExecQuery();

	for (; !OrderSidesTable->Eof; OrderSidesTable->Next())
	{
		// Create new suborder for current order and add it the sides list

		TItemCompleteSub *SubOrder = new TItemCompleteSub;
		Order->SubOrders->SubOrderAdd(SubOrder);

		// Set the values of the new suborder

		SubOrder->OrderKey 		 	= OrderSidesTable->FieldByName("HELDORDERSIDEITEM_KEY"	)->AsInteger;
		SubOrder->MasterOrderKey	= OrderSidesTable->FieldByName("HELDORDERITEM_KEY"  	)->AsInteger;
		SubOrder->Item				= OrderSidesTable->FieldByName("ITEM_NAME"				)->AsString;
		SubOrder->Item_ID			= OrderSidesTable->FieldByName("ITEM_ID"				)->AsInteger;
		SubOrder->Size				= OrderSidesTable->FieldByName("SIZE_NAME"				)->AsString;
		SubOrder->Course			= OrderSidesTable->FieldByName("COURSE_NAME"			)->AsString;
		SubOrder->PLU				= OrderSidesTable->FieldByName("PLU"					)->AsInteger;

		// Set Kitchen Names

		SubOrder->ItemKitchenName  	= UTF8ToUnicodeString((AnsiString)OrderSidesTable->FieldByName("ITEM_KITCHEN_NAME"  )->AsString);
		SubOrder->SizeKitchenName 	= UTF8ToUnicodeString((AnsiString)OrderSidesTable->FieldByName("SIZE_KITCHEN_NAME"  )->AsString);
		SubOrder->CourseKitchenName = UTF8ToUnicodeString((AnsiString)OrderSidesTable->FieldByName("COURSE_KITCHEN_NAME")->AsString);

		// Set Financial Category Key

		SubOrder->Categories->FinancialCategoryKey = OrderSidesTable->FieldByName("CATEGORY_KEY")->AsInteger;

		// Set Security Reference Number of the new suborder

		SubOrder->Security->SetSecurityRefNumber(OrderSidesTable->FieldByName("SECURITY_REF")->AsInteger);

		// Set Quantity of new suborder

		if (OrderSidesTable->FieldByName("QTY")->IsNull)
		{
			SubOrder->SetQty(1);
		}
		else
		{
			SubOrder->SetQty(OrderSidesTable->FieldByName("QTY")->AsFloat);
		}

		// Copy relvent values from the parent order

		SubOrder->ContactsKey		= Order->ContactsKey;
		SubOrder->OrderType		 	= Order->OrderType;
		SubOrder->OrderTypeMessage  = Order->OrderTypeMessage;
		SubOrder->TransNo  		 	= Order->TransNo;
		SubOrder->MenuName			= Order->MenuName;
		SubOrder->TimeStamp		 	= Order->TimeStamp;
		SubOrder->SetMenuMask		= Order->SetMenuMask;
		SubOrder->SetMenuGroup		= Order->SetMenuGroup;

		LoadHeldOrderSideItemCategory(DBTransaction, SubOrder->OrderKey, SubOrder);
	}
}

void TDBHeldOrder::LoadHeldOrderServingCourse(Database::TDBTransaction &DBTransaction, TServingCourse &ServingCourse)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	//Get serving course information from the database

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"SERVINGCOURSE_NAME,"
			"SERVINGCOURSE_KITCHEN_NAME,"
			"COLOUR,"
			"DISPLAY_ORDER "
		"FROM "
			"SERVINGCOURSES "
		"WHERE "
			"SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourse.ServingCourseKey;
	IBInternalQuery->ExecQuery();

	// Set values of the serving course

	ServingCourse.Name 				= IBInternalQuery->FieldByName("SERVINGCOURSE_NAME"	)->AsString;
	ServingCourse.SCOO 				= IBInternalQuery->FieldByName("DISPLAY_ORDER"		)->AsInteger;

	// Set the kitchen name of the serving course

	ServingCourse.KitchenName =  UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString);
	if(ServingCourse.KitchenName == UnicodeString(""))
	{
		ServingCourse.KitchenName = ServingCourse.Name;
	}

	// Set the colour of the serving course

	ServingCourse.Colour = (TColor)IBInternalQuery->FieldByName("COLOUR")->AsInteger;
}

void TDBHeldOrder::LoadHeldOrderPrinting(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Get the items printing infomation for the order from the database

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"PRINT_UNDERLINED,"
			"PRINT_BOLD,"
			"PRINT_DOUBLE_WIDTH,"
			"PRINT_DOUBLE_HEIGHT,"
			"PRINT_COLOUR,"
			"PRINT_FONT "
		"FROM "
			"ITEM "
		"WHERE "
			"ITEM_ID = :ITEM_ID";
	IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = Order->Item_ID;
	IBInternalQuery->ExecQuery();

	// Set the font info values for the order

	Order->FontInfo.Underlined	= (IBInternalQuery->FieldByName("PRINT_UNDERLINED"	 )->AsString  == "T");
	Order->FontInfo.Bold		= (IBInternalQuery->FieldByName("PRINT_BOLD"	 	 )->AsString  == "T");
	Order->FontInfo.Colour		= (IBInternalQuery->FieldByName("PRINT_COLOUR"		 )->AsInteger == 0	) ? fcBlack : fcRed;
	Order->FontInfo.Font		= (IBInternalQuery->FieldByName("PRINT_FONT"		 )->AsInteger == 0	) ? ftFontA : ftFontB;
	Order->FontInfo.Width		= (IBInternalQuery->FieldByName("PRINT_DOUBLE_WIDTH" )->AsString  == "T") ? fsDoubleSize : fsNormalSize;
	Order->FontInfo.Height      = (IBInternalQuery->FieldByName("PRINT_DOUBLE_HEIGHT")->AsString  == "T") ? fsDoubleSize : fsNormalSize;

    for (int i = 0; i < Order->OptionsSelected->Count; i++)
	{
		// Get the current option to set printing for

		TItemOption *CurrentOption = Order->OptionsSelected->OptionGet(i);

		// Get option printing information for the option from the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"PRINT_UNDERLINED,"
				"PRINT_BOLD,"
				"PRINT_DOUBLE_WIDTH,"
				"PRINT_DOUBLE_HEIGHT,"
				"PRINT_COLOUR,"
				"PRINT_FONT "
			"FROM "
				"OPTIONS "
			"WHERE "
				"OPTION_ID = :OPTION_ID";
		IBInternalQuery->ParamByName("OPTION_ID")->AsInteger = CurrentOption->OptionID;
		IBInternalQuery->ExecQuery();

		// Set the font info values for the current option

		CurrentOption->FontInfo.Underlined	= (IBInternalQuery->FieldByName("PRINT_UNDERLINED"	 )->AsString  == "T");
		CurrentOption->FontInfo.Bold		= (IBInternalQuery->FieldByName("PRINT_BOLD"	 	 )->AsString  == "T");
		CurrentOption->FontInfo.Colour		= (IBInternalQuery->FieldByName("PRINT_COLOUR"		 )->AsInteger == 0	) ? fcBlack : fcRed;
		CurrentOption->FontInfo.Font		= (IBInternalQuery->FieldByName("PRINT_FONT"		 )->AsInteger == 0	) ? ftFontA : ftFontB;
		CurrentOption->FontInfo.Width		= (IBInternalQuery->FieldByName("PRINT_DOUBLE_WIDTH" )->AsString  == "T") ? fsDoubleSize : fsNormalSize;
		CurrentOption->FontInfo.Height      = (IBInternalQuery->FieldByName("PRINT_DOUBLE_HEIGHT")->AsString  == "T") ? fsDoubleSize : fsNormalSize;
	}

	for (int i = 0; i < Order->SubOrders->Count; i++)
	{
		// Get the current SubOrder to set printing for

		TItemCompleteSub *SubOrder = (TItemCompleteSub *)Order->SubOrders->Items[i];

		// Get the items printing information for the SubOrder from the database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"PRINT_UNDERLINED,"
				"PRINT_BOLD,"
				"PRINT_DOUBLE_WIDTH,"
				"PRINT_DOUBLE_HEIGHT,"
				"PRINT_COLOUR,"
				"PRINT_FONT "
			"FROM "
				"ITEM "
			"WHERE "
				"ITEM_ID = :ITEM_ID";
		IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = SubOrder->Item_ID;
		IBInternalQuery->ExecQuery();

		// Set the font info values for the SubOrder

		SubOrder->FontInfo.Underlined	= (IBInternalQuery->FieldByName("PRINT_UNDERLINED"	 )->AsString  == "T");
		SubOrder->FontInfo.Bold			= (IBInternalQuery->FieldByName("PRINT_BOLD"	 	 )->AsString  == "T");
		SubOrder->FontInfo.Colour		= (IBInternalQuery->FieldByName("PRINT_COLOUR"		 )->AsInteger == 0	) ? fcBlack : fcRed;
		SubOrder->FontInfo.Font			= (IBInternalQuery->FieldByName("PRINT_FONT"		 )->AsInteger == 0	) ? ftFontA : ftFontB;
		SubOrder->FontInfo.Width		= (IBInternalQuery->FieldByName("PRINT_DOUBLE_WIDTH" )->AsString  == "T") ? fsDoubleSize : fsNormalSize;
		SubOrder->FontInfo.Height      	= (IBInternalQuery->FieldByName("PRINT_DOUBLE_HEIGHT")->AsString  == "T") ? fsDoubleSize : fsNormalSize;
	}
}

void TDBHeldOrder::LoadHeldOrderSecurity(Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Check For Dupliacte Ref and Events
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT * FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF";
	IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
	IBInternalQuery->ExecQuery();

	for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
	{
		TSecurityReference   *SecRef = new TSecurityReference;

		SecRef->UserKey      = IBInternalQuery->FieldByName("USER_KEY"		)->AsInteger;
		SecRef->Event        = IBInternalQuery->FieldByName("SECURITY_EVENT")->AsString;
		SecRef->From         = IBInternalQuery->FieldByName("FROM_VAL"		)->AsString;
		SecRef->To           = IBInternalQuery->FieldByName("TO_VAL"		)->AsString;
		SecRef->Note         = IBInternalQuery->FieldByName("NOTE"			)->AsString;
		SecRef->TimeStamp    = IBInternalQuery->FieldByName("TIME_STAMP"	)->AsDateTime;

		Security->SecurityAdd(SecRef);
	}
}

void TDBHeldOrder::LoadHeldOrderItemCategory(Database::TDBTransaction &DBTransaction, int HeldItemKey, TItemComplete *Order)
{
	LoadHeldOrderCategory(DBTransaction, "HELDORDERITEMCATEGORY", HeldItemKey, Order);
}

void TDBHeldOrder::LoadHeldOrderSideItemCategory(Database::TDBTransaction &DBTransaction, int HeldItemKey, TItemCompleteSub *SubOrder)
{
	LoadHeldOrderCategory(DBTransaction, "HELDORDERSIDEITEMCATEGORY", HeldItemKey, SubOrder);
}

void TDBHeldOrder::LoadHeldOrderCategory(Database::TDBTransaction &DBTransaction, UnicodeString TableName, int HeldItemKey, TItemMinorComplete *Item)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Get category list to load into and financial category key

	TListCategoryContainer *Categories = Item->Categories;
	int CategoryKey = Categories->FinancialCategoryKey;

	// Get the financial category from it's key

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"CATEGORY,GL_CODE "
		"FROM "
			"ARCCATEGORIES "
		"WHERE "
			"CATEGORY_KEY = :CATEGORY_KEY";
	IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
	IBInternalQuery->ExecQuery();

    // Set the financial category then find and set it's category group

	if (IBInternalQuery->RecordCount > 0)
	{
        Categories->FinancialCategory = IBInternalQuery->FieldByName("CATEGORY")->AsString;
        Categories->FinancialCategoryGLCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
	}

	int CategoryGroupKey = TDBOrder::GetCategoriesCategoryGroup(DBTransaction, CategoryKey);
	Categories->FinancialCategoryGroup = TDBOrder::GetCategoryGroup(DBTransaction, CategoryGroupKey);

	// Get the categories on their keys from the given table
	// (The given table has been aliased to CT (CategoryTable) for easy use)

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"AC.CATEGORY_KEY,"
			"AC.CATEGORY, "
            "AC.GL_CODE "
		"FROM "
			+ TableName + " CT INNER JOIN ARCCATEGORIES AC "
			"ON CT.CATEGORY_KEY = AC.CATEGORY_KEY "
		"WHERE "
			"CT.HELDITEM_KEY = :HELDITEM_KEY";
	IBInternalQuery->ParamByName("HELDITEM_KEY")->AsInteger = HeldItemKey;
	IBInternalQuery->ExecQuery();

	// Construct a category for each record and add it to the list

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		TItemSizeCategory *CurrentCat = new TItemSizeCategory;
		CurrentCat->CategoryKey = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
		CurrentCat->Category	= IBInternalQuery->FieldByName("CATEGORY"	 )->AsString;
        CurrentCat->GLCode	= IBInternalQuery->FieldByName("GL_CODE"	 )->AsString;
		Categories->CategoryAdd(CurrentCat);
    }
}

void TDBHeldOrder::LoadHeldOrderPatronCount(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> &Patrons, int HeldOrderKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Load patron count and type from database

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"PATRON_COUNT,"
			"PATRON_TYPE "
		"FROM "
			"HELDORDERPATRONCOUNT "
		"WHERE "
			"HELDORDER_KEY = :HELDORDER_KEY";
	IBInternalQuery->ParamByName("HELDORDER_KEY")->AsInteger = HeldOrderKey;
	IBInternalQuery->ExecQuery();

	// Create new patron using the loaded values and add it to the vector

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		TPatronType Patron;
		Patron.Count = IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;
		Patron.Name  = IBInternalQuery->FieldByName("PATRON_TYPE" )->AsString;
		Patrons.push_back(Patron);
    }
}

//---------------------------------------------------------------------------
// Table Clearing Methods
//---------------------------------------------------------------------------

void TDBHeldOrder::DeleteHeldOrderByKey(Database::TDBTransaction &DBTransaction, int HeldOrderKey)
{
	DeleteHeldOrderPatronCount(DBTransaction, HeldOrderKey);
	DeleteHeldOrderItem(DBTransaction, HeldOrderKey);
	DeleteHeldOrder(DBTransaction, HeldOrderKey);
}

void TDBHeldOrder::DeleteHeldOrderPatronCount(Database::TDBTransaction &DBTransaction, int HeldOrderKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"DELETE FROM "
				"HELDORDERPATRONCOUNT "
			"WHERE "
				"HELDORDER_KEY = :HELDORDER_KEY";
		IBInternalQuery->ParamByName("HELDORDER_KEY")->AsInteger = HeldOrderKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::DeleteHeldOrderItem(Database::TDBTransaction &DBTransaction, int HeldOrderKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"HELDORDERITEM_KEY "
			"FROM "
				"HELDORDERITEMS "
			"WHERE "
				"HELDORDER_KEY = :HELDORDER_KEY";
		IBInternalQuery->ParamByName("HELDORDER_KEY")->AsInteger = HeldOrderKey;
		IBInternalQuery->ExecQuery();

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int HeldOrderItemKey = IBInternalQuery->FieldByName("HELDORDERITEM_KEY")->AsInteger;
			DeleteHeldOrderSideItem(DBTransaction, HeldOrderItemKey);
			DeleteHeldOrderItemOption(DBTransaction, HeldOrderItemKey);
			DeleteHeldOrderItemCategory(DBTransaction, HeldOrderItemKey);
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"DELETE FROM "
				"HELDORDERITEMS "
			"WHERE "
				"HELDORDER_KEY = :HELDORDER_KEY";
		IBInternalQuery->ParamByName("HELDORDER_KEY")->AsInteger = HeldOrderKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::DeleteHeldOrderSideItem(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"HELDORDERSIDEITEM_KEY "
			"FROM "
				"HELDORDERSIDEITEMS "
			"WHERE "
				"HELDORDERITEM_KEY = :HELDORDERITEM_KEY";
		IBInternalQuery->ParamByName("HELDORDERITEM_KEY")->AsInteger = HeldOrderItemKey;
		IBInternalQuery->ExecQuery();


        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int HeldOrderSideItemKey = IBInternalQuery->FieldByName("HELDORDERSIDEITEM_KEY")->AsInteger;
			DeleteHeldOrderSideItemCategory(DBTransaction, HeldOrderSideItemKey);
        }

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"DELETE FROM "
				"HELDORDERSIDEITEMS "
			"WHERE "
				"HELDORDERITEM_KEY = :HELDORDERITEM_KEY";
		IBInternalQuery->ParamByName("HELDORDERITEM_KEY")->AsInteger = HeldOrderItemKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::DeleteHeldOrderItemOption(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"DELETE FROM "
				"HELDORDERITEMOPTION "
			"WHERE "
				"HELDORDERITEM_KEY = :HELDORDERITEM_KEY";
		IBInternalQuery->ParamByName("HELDORDERITEM_KEY")->AsInteger = HeldOrderItemKey;
        IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBHeldOrder::DeleteHeldOrderSideItemCategory(Database::TDBTransaction &DBTransaction, int HeldOrderSideItemKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"DELETE FROM "
			"HELDORDERSIDEITEMCATEGORY "
		"WHERE "
			"HELDITEM_KEY = :HELDITEM_KEY";
	IBInternalQuery->ParamByName("HELDITEM_KEY")->AsInteger = HeldOrderSideItemKey;
	IBInternalQuery->ExecQuery();
}

void TDBHeldOrder::DeleteHeldOrderItemCategory(Database::TDBTransaction &DBTransaction, int HeldOrderItemKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"DELETE FROM "
			"HELDORDERITEMCATEGORY "
		"WHERE "
			"HELDITEM_KEY = :HELDITEM_KEY";
	IBInternalQuery->ParamByName("HELDITEM_KEY")->AsInteger = HeldOrderItemKey;
	IBInternalQuery->ExecQuery();
}

void TDBHeldOrder::DeleteHeldOrder(Database::TDBTransaction &t, int HeldOrderKey)
{
	try {
		TIBSQL *q = t.Query(t.AddQuery());
        q->Close();
        q->SQL->Text = "select activechitnumber_key key "
                       "       from heldorders "
                       "       where heldorder_key = :key;";

		q->ParamByName("key")->AsInteger = HeldOrderKey;
        if (q->ExecQuery(), !q->FieldByName("key")->IsNull)
            TDBActiveChit::DeleteActiveChit(t,
                                            q->FieldByName("key")->AsInteger);
		q->Close();
		q->SQL->Text = "delete from heldorders "
				       "       where heldorder_key = :key;";

		q->ParamByName("key")->AsInteger = HeldOrderKey;
		q->ExecQuery();
	} catch(Exception &E) {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

//---------------------------------------------------------------------------
// Retrieve Indexs Methods
//---------------------------------------------------------------------------

std::vector<TChitInfo> TDBHeldOrder::GetChitInfoOfHeldOrders(Database::TDBTransaction &DBTransaction)
{
	vector<TChitInfo> ChitNumbersInfo;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Select all active chit keys from the held orders

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"DISTINCT ACTIVECHITNUMBER_KEY "
			"FROM "
				"HELDORDERS "
			"WHERE "
				"ACTIVECHITNUMBER_KEY IS NOT NULL";
		IBInternalQuery->ExecQuery();

		// Populate the chit numbers using the active chit number stored in the database

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int ActiveChitKey = IBInternalQuery->FieldByName("ACTIVECHITNUMBER_KEY")->AsInteger;
			TChitInfo CurrentChit(DBTransaction, ActiveChitKey);
			ChitNumbersInfo.push_back(CurrentChit);
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return ChitNumbersInfo;
}

std::vector<TTableInfo> TDBHeldOrder::GetTableInfoOfHeldOrders(Database::TDBTransaction &DBTransaction)
{
	std::vector<TTableInfo> TableNumbersInfo;

    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Select all table numbers and latest time stamps from the held orders

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"TABLE_NUMBER,"
				"TABLE_NAME,"
				"MAX(TIME_STAMP) AS TIME_STAMP "
			"FROM "
				"HELDORDERSVIEW "
			"WHERE "
				"TABLE_NUMBER > 0 "
			"GROUP BY "
				"TABLE_NUMBER,"
				"TABLE_NAME";
		IBInternalQuery->ExecQuery();

		// Populate table numbers using the table number stored in the database and the latest time stamp from that table

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int TableNumber = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
			UnicodeString TableName = IBInternalQuery->FieldByName("TABLE_NAME")->AsString;
			TDateTime TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			TTableInfo CurrentTable(DBTransaction, TableNumber, TableName, TimeStamp);
			TableNumbersInfo.push_back(CurrentTable);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return TableNumbersInfo;
}

std::vector<TTabInfo> TDBHeldOrder::GetTabInfoOfHeldOrders(Database::TDBTransaction &DBTransaction)
{
	std::vector<TTabInfo> TabsInfo;

    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Select all table numbers and latest time stamps from the held orders

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT DISTINCT "
				"TAB_KEY,"
				"TAB_NAME,"
                //MM2038
	            "ID_NUMBER,"
				"TAB_TYPE "
			"FROM "
				"HELDORDERSVIEW "
			"WHERE "
				"TAB_KEY != 0 "
				"AND "
				"TAB_TYPE != :TAB_TABLE_SEAT";
		IBInternalQuery->ParamByName("TAB_TABLE_SEAT")->AsInteger = TabTableSeat;
		IBInternalQuery->ExecQuery();

		// Populate table numbers using the table number stored in the database and the latest time stamp from that table

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			UnicodeString TabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
			TTabInfo CurrentTab(DBTransaction, TabKey, TabName);
			TabsInfo.push_back(CurrentTab);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return TabsInfo;
}

std::vector<THeldOrderInfo> TDBHeldOrder::GetKeyInfoOfUnindexedHeldOrders(Database::TDBTransaction &DBTransaction)
{
	vector<THeldOrderInfo> HeldOrdersInfo;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Select held order keys, device name and latest time stamps from database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"HELDORDER_KEY,"
				"TERMINAL_NAME,"
				"MAX(TIME_STAMP) AS TIME_STAMP "
			"FROM "
				"HELDORDERSVIEW "
			"WHERE "
				"TABLE_NUMBER = 0 "
				"AND "
				"TAB_KEY = 0 "
				"AND "
				"ACTIVECHITNUMBER_KEY IS NULL "
			"GROUP BY "
				"HELDORDER_KEY, TERMINAL_NAME ";
		IBInternalQuery->ExecQuery();

		// Populate keys info with held order keys, device names and time stamps

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			int HeldOrderKey = IBInternalQuery->FieldByName("HELDORDER_KEY")->AsInteger;
			UnicodeString HeldOrderDevice = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
			TDateTime HeldOrderTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			THeldOrderInfo HeldOrderInfo(HeldOrderKey, HeldOrderDevice, HeldOrderTime);
			HeldOrdersInfo.push_back(HeldOrderInfo);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return HeldOrdersInfo;
}

std::vector<int> TDBHeldOrder::GetHeldOrderKeysFromChitKey(Database::TDBTransaction &DBTransaction, int ChitKey)
{
	return GetHeldOrderKeysFromIndex(DBTransaction, "ACTIVECHITNUMBER_KEY", ChitKey);
}

std::vector<int> TDBHeldOrder::GetHeldOrderKeysFromTableNumber(Database::TDBTransaction &DBTransaction, int TableNumber)
{
	return GetHeldOrderKeysFromIndex(DBTransaction, "TABLE_NUMBER", TableNumber);
}

std::vector<int> TDBHeldOrder::GetHeldOrderKeysFromTabKey(Database::TDBTransaction &DBTransaction, int TabKey)
{
	return GetHeldOrderKeysFromIndex(DBTransaction, "TAB_KEY", TabKey);
}

std::vector<int> TDBHeldOrder::GetHeldOrderKeysFromIndex(Database::TDBTransaction &DBTransaction, UnicodeString IndexName, int IndexValue)
{
	// Create vector to store the held order keys

	std::vector<int> HeldOrderKeys;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Select held order keys, device name and latest time stamps from database

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"DISTINCT HELDORDER_KEY "
			"FROM "
				"HELDORDERSVIEW "
			"WHERE "
				+ IndexName + " = :" + IndexName;
        IBInternalQuery->ParamByName(IndexName)->AsInteger = IndexValue;
		IBInternalQuery->ExecQuery();

		for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			HeldOrderKeys.push_back(IBInternalQuery->FieldByName("HELDORDER_KEY")->AsInteger);
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return HeldOrderKeys;
}
