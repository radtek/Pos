//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "ManagerStock.h"
#include "StringTableRes.h"
#include "MMLogging.h"
#include "ItemRecipe.h"
#include "ListCourse.h"
#include "ItemSize.h"
#include "GlobalSettings.h"
#include "WriteOff.h"

#include <Memory>
#include <Classes.hpp>
#include <StrUtils.hpp>

#pragma package(smart_init)
//---------------------------------------------------------------------------

TManagerStock::TManagerStock()
{
	StockModule = NULL;
}

TManagerStock::~TManagerStock()
{
	delete StockModule;
}

void TManagerStock::OpenDB()
{
	UnicodeString StockDB = TGlobalSettings::Instance().StockInterbaseIP + ":" + TGlobalSettings::Instance().StockDatabasePath;
	try
	{
		if(StockModule != NULL)
		{
			delete StockModule;
			StockModule = NULL;
		}

		if(TGlobalSettings::Instance().StockInterbaseIP != "" || TGlobalSettings::Instance().StockDatabasePath != "")
		{
			StockModule = new StockInterface::TStockInterface(StockDB);
			StockModule->Initialise();
			StockInterfaceEnabled = true;
		}
		else
		{
			StockInterfaceEnabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message + " StockModule Initialise Failed");
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message + " Full DB Path " + StockDB);
		StockInterfaceEnabled = false;
		throw;
	}
}

void TManagerStock::UpdateStock(Database::TDBTransaction &DBTransaction,TList *OrderObjects, AnsiString Operator)
{
	try
	{
	  if(OrderObjects->Count > 0)
      {
			for(int i = 0 ; i < OrderObjects->Count ; i++)
			{
            TItemComplete *Order = (TItemComplete *)OrderObjects->Items[i];
				UpdateStock(DBTransaction,Order, Operator);
			for (int j = 0; j < Order->SubOrders->Count; j ++)
			{
					TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
					UpdateStock(DBTransaction,SubOrder, Operator);
			}

         }
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

//---------------------------------------------------------------------------

void TManagerStock::UpdateStock(Database::TDBTransaction &DBTransaction,TItemComplete *Order, AnsiString Operator)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		AnsiString temp = TDeviceRealTerminal::Instance().ID.Name;
		for(int i = 0;  i < Order->SalesRecipesToApply->Count ; i++)
		{
			TItemRecipe *Recipe = Order->SalesRecipesToApply->RecipeGet(i);
			if(Recipe->Enabled)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_STOCKTRACK, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
					"INSERT INTO STOCKTRACK ("
						"STOCK_KEY, "
						"STOCK_CODE, "
						"STOCK_LOCATION, "
						"QTY, "
						"PRODUCT, "
						"ADJUSTMENT_TYPE,"
						"DEVICE_NAME,"
						"STAFF) "
					"VALUES ("
						":LOCALKEY, "
						":LOC_CODE, "
						":LOC_LOCATION, "
						":ITEM_QTY, "
						":PRODUCT, "
						":ADJUSTMENT_TYPE,"
						":DEVICE_NAME,"
						":STAFF);";


				IBInternalQuery->ParamByName("ITEM_QTY")->AsCurrency			= Recipe->Qty * Order->GetQty();
				IBInternalQuery->ParamByName("LOCALKEY")->AsInteger		= LocalKey;
				IBInternalQuery->ParamByName("LOC_CODE")->AsString			= Recipe->StockCode;
				IBInternalQuery->ParamByName("LOC_LOCATION")->AsString	= Recipe->StockLocation;
				IBInternalQuery->ParamByName("PRODUCT")->AsString			= TDeviceRealTerminal::Instance().ID.Product;
				IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = Order->SalesRecipesToApply->AdjustmentType;
				IBInternalQuery->ParamByName("DEVICE_NAME")->AsString		= TDeviceRealTerminal::Instance().ID.Name;
				IBInternalQuery->ParamByName("STAFF")->AsString		= Operator;
				IBInternalQuery->ExecQuery();
			}
		}
		for(int i = 0;  i < Order->WastageRecipesToApply->Count ; i++)
		{
			TItemRecipe *Recipe = Order->WastageRecipesToApply->RecipeGet(i);
			Recipe->Reason = Order->Reason;
		 if(Recipe->Enabled)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_STOCKTRACK, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
					"INSERT INTO STOCKTRACK ("
						"STOCK_KEY, "
						"STOCK_CODE, "
						"STOCK_LOCATION, "
						"QTY, "
						"PRODUCT, "
						"ADJUSTMENT_TYPE,"
						"DEVICE_NAME, "
						"STAFF, "
						"REASON) "
					"VALUES ("
						":LOCALKEY, "
						":LOC_CODE, "
						":LOC_LOCATION, "
						":ITEM_QTY, "
						":PRODUCT, "
						":ADJUSTMENT_TYPE,"
						":DEVICE_NAME,"
						":STAFF,"
						":REASON);";

				IBInternalQuery->ParamByName("ITEM_QTY")->AsCurrency			= Recipe->Qty * Order->GetQty();
				IBInternalQuery->ParamByName("LOCALKEY")->AsInteger		= LocalKey;
				IBInternalQuery->ParamByName("LOC_CODE")->AsString			= Recipe->StockCode;
				IBInternalQuery->ParamByName("LOC_LOCATION")->AsString	= Recipe->StockLocation;
				IBInternalQuery->ParamByName("PRODUCT")->AsString			= TDeviceRealTerminal::Instance().ID.Product;
				IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = Order->WastageRecipesToApply->AdjustmentType;
				IBInternalQuery->ParamByName("DEVICE_NAME")->AsString		= TDeviceRealTerminal::Instance().ID.Name;
				IBInternalQuery->ParamByName("STAFF")->AsString   = Operator;
				IBInternalQuery->ParamByName("REASON")->AsString   = Recipe->Reason;
				IBInternalQuery->ExecQuery();
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

//------------------------------------------------------------------------------
void TManagerStock::UpdateStock(Database::TDBTransaction &DBTransaction,TItemCompleteSub *Order, AnsiString Operator)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		double StockQty;
		Currency Value, Cost;
		for(int i = 0;  i < Order->SalesRecipesToApply->Count ; i++)
		{
			TItemRecipe *Recipe = Order->SalesRecipesToApply->RecipeGet(i);
			if(Recipe->Enabled)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_STOCKTRACK, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->ParamCheck = true;
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
					"INSERT INTO STOCKTRACK ("
						"STOCK_KEY, "
						"STOCK_CODE, "
						"STOCK_LOCATION, "
						"QTY, "
						"PRODUCT, "
						"ADJUSTMENT_TYPE,"
						"DEVICE_NAME,"
						"STAFF) "
					"VALUES ("
						":STOCK_KEY, "
						":LOC_CODE, "
						":LOC_LOCATION, "
						":ITEM_QTY, "
						":PRODUCT, "
						":ADJUSTMENT_TYPE,"
						":DEVICE_NAME,"
						":STAFF);";

				IBInternalQuery->ParamByName("ITEM_QTY")->AsCurrency			= Recipe->Qty * Order->GetQty();
				IBInternalQuery->ParamByName("STOCK_KEY")->AsInteger		= LocalKey;
				IBInternalQuery->ParamByName("LOC_CODE")->AsString			= Recipe->StockCode;
				IBInternalQuery->ParamByName("LOC_LOCATION")->AsString	= Recipe->StockLocation;
				IBInternalQuery->ParamByName("PRODUCT")->AsString			= TDeviceRealTerminal::Instance().ID.Product;
				IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = Order->SalesRecipesToApply->AdjustmentType;
				IBInternalQuery->ParamByName("DEVICE_NAME")->AsString		= TDeviceRealTerminal::Instance().ID.Name;
				IBInternalQuery->ParamByName("STAFF")->AsString   = Operator;
				IBInternalQuery->ExecQuery();
			}
		}

		for(int i = 0;  i < Order->WastageRecipesToApply->Count ; i++)
		{
			TItemRecipe *Recipe = Order->WastageRecipesToApply->RecipeGet(i);
			Recipe->Reason = Order->Reason;
			if(Recipe->Enabled)
		 {
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_STOCKTRACK, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->ParamCheck = true;
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
					"INSERT INTO STOCKTRACK ("
						"STOCK_KEY, "
						"STOCK_CODE, "
						"STOCK_LOCATION, "
						"QTY, "
						"PRODUCT, "
						"ADJUSTMENT_TYPE,"
						"DEVICE_NAME,"
						"STAFF,"
						"REASON) "
					"VALUES ("
						":STOCK_KEY, "
						":LOC_CODE, "
						":LOC_LOCATION, "
						":ITEM_QTY, "
						":PRODUCT, "
						":ADJUSTMENT_TYPE,"
						":DEVICE_NAME,"
						":STAFF,"
						":REASON);";

				IBInternalQuery->ParamByName("ITEM_QTY")->AsCurrency			= Recipe->Qty * Order->GetQty();
				IBInternalQuery->ParamByName("STOCK_KEY")->AsInteger		= LocalKey;
				IBInternalQuery->ParamByName("LOC_CODE")->AsString			= Recipe->StockCode;
				IBInternalQuery->ParamByName("LOC_LOCATION")->AsString	= Recipe->StockLocation;
				IBInternalQuery->ParamByName("PRODUCT")->AsString			= TDeviceRealTerminal::Instance().ID.Product;
				IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = Order->WastageRecipesToApply->AdjustmentType;
				IBInternalQuery->ParamByName("DEVICE_NAME")->AsString		= TDeviceRealTerminal::Instance().ID.Name;
				IBInternalQuery->ParamByName("STAFF")->AsString		= Operator;
				IBInternalQuery->ParamByName("REASON")->AsString    = Recipe->Reason;
				IBInternalQuery->ExecQuery();
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

//------------------------------------------------------------------------------

bool TManagerStock::GetStockDetails(Database::TDBTransaction &DBTransaction,UnicodeString StockCode,UnicodeString StockLocation, UnicodeString &StockDescription,
	UnicodeString &StockUnit, Currency &StockCost, double &StockGST)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT COST, STOCK_DESC, UNIT,COST_GST_PERCENT FROM STOCKCOSTS "
			"WHERE STOCK_CODE = :ITEM_CODE AND "
		 "UPPER(LOCATION) = :LOCATION";
	  IBInternalQuery->ParamByName("ITEM_CODE")->AsString = StockCode;
		IBInternalQuery->ParamByName("LOCATION")->AsString =  StockLocation.UpperCase();
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			StockDescription	= IBInternalQuery->FieldByName("STOCK_DESC")->AsString;
			StockUnit			= IBInternalQuery->FieldByName("UNIT")->AsString;
			StockCost			= IBInternalQuery->FieldByName("COST")->AsCurrency;
			StockGST				= IBInternalQuery->FieldByName("COST_GST_PERCENT")->AsFloat;
			IBInternalQuery->Close();
			RetVal = true;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	}
	return RetVal;
}
//------------------------------------------------------------------------------


void TManagerStock::UpdateStockCost(Database::TDBTransaction &DBTransaction,UnicodeString inStockCode, UnicodeString inLocation,
												UnicodeString inUnit, UnicodeString inDescription,
												Currency inCost,Currency inCostGSTPercent)
{
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        DBTransaction.RegisterQuery(IBInternalQuery);

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE STOCKCOSTS "
			"SET "
				"COST			= :COST,"
				"COST_GST_PERCENT	 = :COST_GST_PERCENT,"
				"UNIT			= :UNIT,"
				"STOCK_DESC	= :DESCRIPTION "
			"WHERE "
				"STOCK_CODE	= :CODE AND "
				"UPPER(LOCATION)	= :LOCATION";

		IBInternalQuery->ParamByName("CODE")->AsString			= inStockCode;
		IBInternalQuery->ParamByName("LOCATION")->AsString    = inLocation.UpperCase();
		IBInternalQuery->ParamByName("UNIT")->AsString			= inUnit;
		IBInternalQuery->ParamByName("DESCRIPTION")->AsString	= inDescription;
		IBInternalQuery->ParamByName("COST")->AsCurrency		= inCost;
		IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsCurrency = inCostGSTPercent;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RowsAffected < 1)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO STOCKCOSTS ("
					"STOCK_CODE,"
					"LOCATION,"
					"UNIT,"
					"STOCK_DESC,"
					"COST,"
					"COST_GST_PERCENT) "
				"VALUES ("
					":CODE,"
					":LOCATION,"
					":UNIT,"
					":DESCRIPTION,"
					":COST,"
					":COST_GST_PERCENT);";
			IBInternalQuery->ParamByName("CODE")->AsString			= inStockCode;
			IBInternalQuery->ParamByName("LOCATION")->AsString		= inLocation;
			IBInternalQuery->ParamByName("UNIT")->AsString			= inUnit;
			IBInternalQuery->ParamByName("DESCRIPTION")->AsString	= inDescription;
			IBInternalQuery->ParamByName("COST")->AsCurrency		= inCost;
			IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsCurrency = inCostGSTPercent;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TManagerStock::UpdateItemCosts(Database::TDBTransaction &DBTransaction,UnicodeString inStockCode,
												Currency inCost,Currency inCostGSTPercent)
{
   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);

	try
	{
		// Find all the Item Sizes with this Stock Code.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT ITEMSIZE.ITEMSIZE_KEY FROM ITEMSIZE "
			"INNER JOIN RECIPE ON RECIPE.ITEMSIZE_KEY = ITEMSIZE.ITEMSIZE_KEY "
			"WHERE "
				"RECIPE.STOCK_CODE	= :STOCK_CODE";
		IBInternalQuery->ParamByName("STOCK_CODE")->AsString	= inStockCode;
		IBInternalQuery->ExecQuery();
		for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			int ItemKey = IBInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
			// Load up that items Receipes.
			Database::TcpIBSQL IBReceipeQuery(new TIBSQL(NULL));
			DBTransaction.RegisterQuery(IBReceipeQuery);

			IBReceipeQuery->Close();
			IBReceipeQuery->SQL->Text =
				"SELECT QTY,COST,COST_GST_PERCENT FROM RECIPE "
				"LEFT JOIN STOCKCOSTS ON STOCKCOSTS.STOCK_CODE = RECIPE.STOCK_CODE "
            " AND STOCKCOSTS.LOCATION = RECIPE.STOCK_LOCATION "
				"WHERE "
					"RECIPE.ITEMSIZE_KEY	= :ITEMSIZE_KEY";
			IBReceipeQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = ItemKey;
			IBReceipeQuery->ExecQuery();
			double CostIncGst = 0;
			double OrderCost = 0;
			double UnitCost = 0;
			double UnitQty = 0;
			// Recalculate That items cost price ( if it has a receipe(s) ).
			for (;!IBReceipeQuery->Eof ; IBReceipeQuery->Next())
			{
				UnitCost  	= 		double(IBReceipeQuery->FieldByName("COST")->AsFloat);
				UnitQty  	= 		double(IBReceipeQuery->FieldByName("QTY")->AsFloat);
				OrderCost 	+= 	double(UnitCost * UnitQty);
				CostIncGst 	+= 	double(UnitCost * UnitQty) *
										double(IBReceipeQuery->FieldByName("COST_GST_PERCENT")->AsFloat + 100.0) / double(100.0);
			}

			if(OrderCost != 0)
			{
				double CostGSTPercent = double(((CostIncGst - OrderCost) * 100) / double(OrderCost));
				// Store the item cost back into the item.
				Database::TcpIBSQL IBItemSizeQuery(new TIBSQL(NULL));
				DBTransaction.RegisterQuery(IBItemSizeQuery);

				IBItemSizeQuery->Close();
				IBItemSizeQuery->SQL->Text =
					"UPDATE ITEMSIZE "
					"SET "
						"COST					 = :COST, "
						"COST_GST_PERCENT	 = :COST_GST_PERCENT "
					"WHERE "
						"ITEMSIZE_KEY	= :ITEMSIZE_KEY";

				IBItemSizeQuery->ParamByName("COST")->AsCurrency					= OrderCost;
				IBItemSizeQuery->ParamByName("COST_GST_PERCENT")->AsCurrency   = CostGSTPercent;
				IBItemSizeQuery->ParamByName("ITEMSIZE_KEY")->AsInteger			= ItemKey;
				IBItemSizeQuery->ExecQuery();
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
      throw;
	}
}


bool TManagerStock::GetStockTrackQty(Database::TDBTransaction &DBTransaction,UnicodeString StockCode,UnicodeString StockLocation,double &StockTrackQty)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT SUM(QTY)QTY FROM STOCKTRACK "
			"WHERE STOCK_CODE = :STOCK_CODE AND "
			"UPPER(STOCK_LOCATION) = :STOCK_LOCATION";
		IBInternalQuery->ParamByName("STOCK_CODE")->AsString = StockCode;
		IBInternalQuery->ParamByName("STOCK_LOCATION")->AsString =  StockLocation.UpperCase();
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			StockTrackQty  	= IBInternalQuery->FieldByName("QTY")->AsFloat;
			RetVal = true;
		}
		IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return RetVal;
}
//------------------------------------------------------------------------------



void TManagerStock::GetStockOnHand(Database::TDBTransaction &DBTransaction,TListRecipeContainer * Recipes,TStringList *Report)
{
	try
	{
		if(!StockInterfaceEnabled) return;

		Report->Clear();
		std::auto_ptr<TResourceStream> StockRepTop(new TResourceStream((THandle)HInstance,UnicodeString("STOCK_ONHAND_TOP"), UnicodeString("HTML").w_str()));
		std::auto_ptr<TResourceStream> StockRepBottom(new TResourceStream((THandle)HInstance,UnicodeString("STOCK_ONHAND_BOTTOM"), UnicodeString("HTML").w_str()));

		std::auto_ptr<TStringList> Temp(new TStringList);
		Report->LoadFromStream(StockRepTop.get());
		for (int i = 0; i < Recipes->Count ; i++)
		{
			TItemRecipe *Recipe = Recipes->RecipeGet(i);
			StockInterface::TStockDetails Details;
			if(StockModule->GetStockDetails(Recipe->StockCode,Recipe->StockLocation,Details))
			{
				double StockTrackQty = 0;
				GetStockTrackQty(DBTransaction,Recipe->StockCode,Recipe->StockLocation,StockTrackQty);
				Report->Add("<tr>");
				Report->Add("<td width=\"19%\">" + Details.StockCode + "</td>");
				Report->Add("<td width=\"23%\">" + Details.Location + "</td>");
				Report->Add("<td width=\"15%\">" + FormatFloat("0.000",Details.OnHand - StockTrackQty) + "</td>");
				Report->Add("<td width=\"16%\">" + (Details.Unit == "" ? AnsiString("&nbsp;") : Details.Unit) + "</td>");
				Report->Add("<td width=\"27%\">" + (Details.Description == "" ? AnsiString("&nbsp;") : Details.Description) + "</td>");
				Report->Add("</tr>");
			}
		}
		Temp->LoadFromStream(StockRepBottom.get());
		Report->AddStrings(Temp.get());
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TManagerStock::GetCourseStockReport(Database::TDBTransaction &DBTransaction,TListCourse *Course,TStringList *html)
{
	try
	{
		if(!StockInterfaceEnabled)
      {
         UnicodeString Temp = LoadStr(HTML_START);
         Temp = AnsiReplaceStr(Temp, "%TITLE%", "Stock System Disabled.");
         html->Add(Temp);
         html->Add("Stock System Disabled.");
         Temp = LoadStr(HTML_BODY_STOP);
         html->Add(Temp);
      }
      else
      {
         UnicodeString TempHeaderRow = LoadStr(CSREPORT_RECEIPE_HEADER_ROW);
         UnicodeString Temp = LoadStr(CSREPORT_TOP);
         html->Add(Temp);
         // Load and repalce Course Section.
         Temp = LoadStr(CSREPORT_COURSE_ROW);
         Temp = AnsiReplaceStr(Temp, "%COURSENAME%", Course->Course_Name);
         html->Add(Temp);
         html->Add(TempHeaderRow);

         for (int i = 0; i < Course->Count ; i++)
         {
            TItem *Item = Course->ItemGet(i);
            TListSizeContainer *Sizes = Item->Sizes;
            for (int j = 0; j < Sizes->Count ; j++)
            {
               TItemSize *Size = Sizes->SizeGet(j);
               // Load and repalce Item Section and Size.
               Temp = LoadStr(CSREPORT_ITEM_ROW);
               Temp = AnsiReplaceStr(Temp, "%ITEMNAME%", Item->Item);
               Temp = AnsiReplaceStr(Temp, "%SIZENAME%", Size->Name);
               html->Add(Temp);
               TListRecipeContainer *Recipes = Size->Recipes;
               if(Recipes->Count == 0)
               {
                  html->Add("<tr>");
                  html->Add("<td colspan=\"2\">No Recipe for this Item/Size</td>");
                  html->Add("<td>&nbsp;</td>");
                  html->Add("<td>&nbsp;</td>");
                  html->Add("<td>&nbsp;</td>");
                  html->Add("</tr>");
               }
               else
               {
                  for (int i = 0; i < Recipes->Count ; i++)
                  {
                     TItemRecipe *Recipe = Recipes->RecipeGet(i);
                     StockInterface::TStockDetails Details;
                     if(StockModule->GetStockDetails(Recipe->StockCode,Recipe->StockLocation,Details))
                     {
                        double StockTrackQty = 0;
                        GetStockTrackQty(DBTransaction,Recipe->StockCode,Recipe->StockLocation,StockTrackQty);
                        UnicodeString RowColour = "";
                        if((Details.OnHand - StockTrackQty) <= Details.MinLevel)
                        {
                           RowColour = "bgcolor=\"#FF0033\""; // Red
                        }
                        else if (Details.OnHand - StockTrackQty < (Details.MinLevel + ((Details.MaxLevel - Details.MinLevel) *0.20)) )
                        {
                           RowColour = "bgcolor=\"#FFFF99\""; // Yellow
                        }

                        html->Add("<tr "+ RowColour +">");
                        html->Add("<td>" + Details.StockCode + "</td>");
                        html->Add("<td>" + Details.Location + "</td>");
                        html->Add("<td>" + FormatFloat("0.000",Details.OnHand - StockTrackQty) + " (" + FormatFloat(" 0.00",Details.MinLevel) + "/" + FormatFloat("0.00 ",Details.MaxLevel) + ")" "</td>");
								html->Add("<td>" + (Details.Unit == "" ? AnsiString("&nbsp;") : Details.Unit) + "</td>");
								html->Add("<td>" + (Details.Description == "" ? AnsiString("&nbsp;") : Details.Description) + "</td>");
   /*							html->Add("<td width=\"11%\">" + Details.StockCode + "</td>");
                        html->Add("<td width=\"16%\">" + Details.Location + "</td>");
                        html->Add("<td width=\"11%\">" + FormatFloat("0.000",Details.OnHand - StockTrackQty) + "</td>");
                        html->Add("<td width=\"14%\">" + (Details.Unit == "" ? UnicodeString("&nbsp;") : Details.Unit) + "</td>");
                        html->Add("<td width=\"48%\">" + (Details.Description == "" ? UnicodeString("&nbsp;") : Details.Description) + "</td>");*/

                        html->Add("</tr>");
                     }
                  }
               }
            }
         }
         Temp = LoadStr(CSREPORT_BOTTOM);
         html->Add(Temp);
      }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

