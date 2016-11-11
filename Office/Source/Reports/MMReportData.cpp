

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MMReportData.h"
#include "Login.h"
#include "MMData.h"
#include <vcl\Clipbrd.hpp>
#include <memory>

#include "SqlViewer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RpCon"
#pragma link "RpConDS"
#pragma link "RpDefine"
#pragma resource "*.dfm"
TdmMMReportData *dmMMReportData;
//---------------------------------------------------------------------------
__fastcall TdmMMReportData::TdmMMReportData(TComponent* Owner)
	: TDataModule(Owner)
{
    _taxJoins =  "left join  "
            "(SELECT  cast(1 as int) keyvalue , "                                                                                                                                                                                                          
            "TAXPROFILE.ORDER_KEY,  "                                                                                                                                                                                                              
                    "sum(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT ,            "                                                                                                                                                
                    "sum(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,   "                                                                                                                                                
                    "sum(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS ServiceChargeTax,"
                    " sum(CASE WHEN TAXPROFILE.TYPE = 5 THEN TAXPROFILE.TAX_RATE END) AS ProfitTax       "
                    "FROM (SELECT   "                                                                                                                                                                                                                      
                    "TFO.ORDER_KEY,  TAXPROFILES.TYPE,   "                                                                                                                                                                                                
                    "Cast(( sum(COALESCE(TAXPROFILES.RATE,0)) ) as Numeric(17,4)) TAX_RATE  "                                                                                                                                                                    
                    "FROM TAXPROFILES_ORDERS TFO "                                                                                                                                                                                                         
                    "left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY       "                                                                                                                                                              
                    "group by TFO.ORDER_KEY,TAXPROFILES.TYPE,TAXPROFILES.RATE )  TAXPROFILE "                                                                                                                                                              
                    "GROUP BY TAXPROFILE.ORDER_KEY                                          "                                                                                                                                                              
            " ) TaxDetails on TaxDetails.ORDER_KEY=ORDERS.ORDER_KEY                         "                                                                                                                                                               
            " left join (SELECT VARS.keyvalue  ,MIN(CASE WHEN setting.INTEGER_VAL = 1 THEN VARS.ServiceChargeTax else cast(0 as int)   END) AS ServiceChargeTax "                                                                                            
                       "  FROM (  SELECT      cast(1 as int) keyvalue   , "                                                                                                                                                                                                        
                       "             MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END "                                                                                                                                                                       
                       "                     ) AS ServiceChargeTax      FROM VARSPROFILE   )    VARS                 "                                                                                                                                                                               
                       "     left join (SELECT   cast(1 as int) keyvalue  ,a.INTEGER_VAL FROM VARSPROFILE a where a.VARIABLES_KEY=8005) setting on  setting.keyvalue=    VARS.keyvalue "                                                                                              
                       " group by 1   ) STAX on  STAX.keyvalue=TaxDetails.keyvalue "                                                                                                                                                                                             
           "left join ( SELECT                                                     "                                                                                                                                                                      
                "cast(1 as int) keyvalue   ,                                       "                                                                                                                                                                      
                " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8000 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludetax  ,                       "                                                                                                       
                " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8001 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludeServiceCharge  ,             "                                                                                                       
                " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8002 THEN VARSPROFILE.INTEGER_VAL END ) As isRecalculateTaxAfterDiscount  ,           "                                                                                                       
                " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8003 THEN VARSPROFILE.INTEGER_VAL END ) As  isRecalculateServiceChargeAfterDiscount , "                                                                                                       
                " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8005 THEN VARSPROFILE.INTEGER_VAL END ) As  isApplyTaxToServiceCharge                 "                                                                                                       
                " from VARSPROFILE) taxsetting on  taxsetting.keyvalue=TaxDetails.keyvalue                                                        ";

    _selectSalesIncl = "   Cast( "
"        (  "
"          Sum(  "
"               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +   "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) "
"                      )    +  "
"     cast((  Sum(   "
"                  case when taxsetting.isPriceIncludetax =1 then   "
"                               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))  "
"                    else  "
"                              case when taxsetting.isRecalculateTaxAfterDiscount =1  then  "
"                                   cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                           cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                             + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                                              ) )as numeric(17,4)) )as  numeric(17,4))  "
"                               else    "
"                                      cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4))  "
"                              end    "
"                 end    "
"            )  * cast ((COALESCE(TaxDetails.VAT,0))as numeric (17,4))/100 )as numeric(17,4))  +   "
"      sum( cast((  cast(( coalesce(ORDERS.PRICE,0)-  coalesce(ORDERS.MAXRETAILPRICE,0))as numeric(17,4)) *  cast((coalesce(TaxDetails.ProfitTax,0))as numeric(17,4))/100 )as numeric(17,4)) ) +  "
"        Sum(     "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then   "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                   cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                     + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then     "
"                                  cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                           else   "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))   "
"                        end  "
"                end  "
"             )  +  "
"    cast((   Sum(  "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then  "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then  "
"                                  cast((  (cast((  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"                              "
"                        else "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                        end  "
"                end      "
"             ) *  cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 )as numeric(17,4))  "
"      ) as numeric(17,4))  SalesIncl   ";

_groupByClause = 
                "TaxDetails.VAT,                          "
                "TaxDetails.ServiceCharge,                "                                                                                                                                                                                               
                "TaxDetails.ServiceChargeTax,             "                                                                                                                                                                                               
                "STAX.ServiceChargeTax ,                  "                                                                                                                                                                                           
                "taxsetting.isRecalculateTaxAfterDiscount " ;

AnsiString tip = " || ' TIP' ";
_dayArcBillSubQuery =  	" Select "
                                                "		DayArcBill.ArcBill_Key,               "
                                                "		DayArcBill.Time_Stamp,                "
                                                "		DayArcBill.Invoice_Number,            "
                                                "		DayArcBill.Total,                     "
                                                "		DayArcBill.Patron_Count,              "
                                                "		Security.Terminal_Name,               "
                                                "		Security.From_Val Staff_Name,         "
                                                "		DayArcBillPay.Note,                   "
                                                "        		ab.TABLE_NAME TABLE_NUMBER,   "
                                                "        		ab.price,                     "
                                                 "DayArcBillPAY.PAY_TYPE " + tip + " PAY_TYPE, "
                                                "DayArcBillPAY.TIP_AMOUNT price "
                                    "From "
                                        "DayArcBill Left Join Security On                      "
                                        "	DayArcBill.Security_Ref = Security.Security_Ref    "
                                        "Left Join DayArcBillPay On                            "
                                        "	DayArcBill.ArcBill_Key = DayArcBillPay.ArcBill_Key "
                                        "      left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME, cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM DayArchive a "
                                        "     group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=DayArcBill.ARCBILL_KEY "
                                     "Where "
                                        "Security.Security_Event = 'Billed By' And coalesce(DayArcBillPAY.TIP_AMOUNT,0) > 0 ";



_groupingForDayArcbill = "  GROUP BY DayArcBill.ARCBILL_KEY, DayArcBill.Time_Stamp,  "
                                        "DayArcBill.Invoice_Number,                               "
                                        "DayArcBill.Total,                                        "
                                        "DayArcBill.Patron_Count,                                 "
                                        "Security.Terminal_Name,                                  "
                                        "Security.From_Val ,                                      "
                                        "DayArcBillPay.Note,                                      "
                                      " ab.TABLE_NAME, ab.price, DayArcBillPAY.PAY_TYPE, DayArcBillPAY.TIP_AMOUNT ";

_arcBillSubQuery =      " Select   "
                                                "		ArcBill.ArcBill_Key,              "
                                                "		ArcBill.Time_Stamp,               "
                                                "		ArcBill.Invoice_Number,           "
                                                "		ArcBill.Total,                    "
                                                "		ArcBill.Patron_Count,             "
                                                "		Security.Terminal_Name,           "
                                                "		Security.From_Val Staff_Name,     "
                                                "		ArcBillPay.Note,                  "
                                                "        ab.TABLE_NAME TABLE_NUMBER,      "
                                                "        ab.price ,                        "
                                                 "ArcBillPAY.PAY_TYPE " + tip + " PAY_TYPE , "
                                                "ArcBillPAY.TIP_AMOUNT price "
                                    "From "
                                                "ArcBill Left Join Security On                      "
                                                "	ArcBill.Security_Ref = Security.Security_Ref    "
                                                "Left Join ArcBillPay On                            "
                                                "	ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
                                                "      left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME, cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM Archive a "
                                                "     group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=ArcBill.ARCBILL_KEY "
                                      "Where "
                                                "Security.Security_Event = 'Billed By'  And coalesce(ArcBillPAY.TIP_AMOUNT,0) > 0 ";

_groupingForArcbill =
                                              "  GROUP BY ArcBill.ARCBILL_KEY, ArcBill.Time_Stamp,  "
                                                "ArcBill.Invoice_Number,                               "
                                                "ArcBill.Total,                                        "
                                                "ArcBill.Patron_Count,                                 "
                                                "Security.Terminal_Name,                                  "
                                                "Security.From_Val ,                                      "
                                                "ArcBillPay.Note,                                      "
                                              " ab.TABLE_NAME, ab.price, ArcBillPAY.PAY_TYPE, ArcBillPAY.TIP_AMOUNT "; 

}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::DataModuleDestroy(TObject *Sender)
{
	if (cdsHalfHourlySummary->Active)
	{
		cdsHalfHourlySummary->EmptyDataSet();
		cdsHalfHourlySummary->Close();
	}
	if (cdsMenu->Active)
	{
		cdsMenu->EmptyDataSet();
		cdsMenu->Close();
	}

}

//---------------------------------------------------------------------------
AnsiString TdmMMReportData::ParamString(int Count, AnsiString Field, AnsiString ParamName, AnsiString Operator)
{
	AnsiString SQL = "(";
	for (int i=0; i<Count-1; i++)
	{
		SQL = SQL + Field + "  = :" + ParamName + IntToStr(i) + " " + Operator + " ";
	}
	SQL = SQL + Field + "  = :" + ParamName + IntToStr(Count-1) + ") ";
	return SQL;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupMenuRecipes(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST)
{
//	if (IncGST)
  //	{
		qrMenuStock->Close();
		qrMenuRecipe->Close();
		qrMenuRecipe->SQL->Text =
			"Select "
				"Menu.Menu_Name,"
				"Course.Course_Name,"
				"Item.Item_Name,"
				"ItemSize.Size_Name,"
				"ItemSize.Price,"
				"Recipe.Stock_Code Code,"
				"Recipe.Qty,"
				"Recipe.Stock_Location Location "
			"From "
				"Menu Left Join Course On "
					"Menu.Menu_Key = Course.Menu_Key "
				"Left Join Item On "
					"Course.Course_Key = Item.Course_Key "
				"Left Join ItemSize On "
					"Item.Item_Key = ItemSize.Item_Key "
				"Left Join Recipe On "
					"ItemSize.ItemSize_Key = Recipe.ItemSize_Key "
			"Where "
				"Recipe.Stock_Code is not null ";
		if (Menus->Count > 0)
		{
			qrMenuRecipe->SQL->Text	=	qrMenuRecipe->SQL->Text + "And (" +
												ParamString(Menus->Count, "Menu.Menu_Name", "MenuParam") + ")";
		}
		qrMenuRecipe->SQL->Text		=	qrMenuRecipe->SQL->Text +
			"Order By "
				"Menu.Menu_Name,"
				"Course.CAO,"
				"Item.IAO,"
				"ItemSize.ISAO";

		for (int i=0; i<Menus->Count; i++)
		{
			qrMenuRecipe->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
		}
		qrMenuStock->SQL->Text =
			"Select "
				"Stock.Code,"
				"Stock.Description,"
				"Stock.Stocktake_Unit,"
				//"Cast(Latest_Cost * (GST_Percent + 100) / 100 as Numeric(17,4)) Cost "
				"StockLocation.Average_Cost Cost,"
				"StockLocation.Assessed_Value "
			"From "
				"Stock Left Join StockLocation On "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Where "
				"Stock.Code = :Code And "
				"StockLocation.Location = :Location";

}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrMenuRecipeAfterScroll(TDataSet *DataSet)
{
	qrMenuStock->Close();
	qrMenuStock->ParamByName("Code")->AsString		= qrMenuRecipe->FieldByName("Code")->AsString;
	qrMenuStock->ParamByName("Location")->AsString	= qrMenuRecipe->FieldByName("Location")->AsString;
	qrMenuStock->Open();
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupMenuProfit(TDateTime StartTime, TDateTime EndTime, TStrings *Menus/*, bool IncGST*/)
{
  if (cdsMenu->Active)
	{  // already Used
		cdsMenu->EmptyDataSet();
	}
	else
	{
		try
		{
			cdsMenu->FieldDefs->Clear();
			cdsMenu->FieldDefs->Add("MENU_NAME", ftString, 15);
			cdsMenu->FieldDefs->Add("COURSE_NAME", ftString, 25);
			cdsMenu->FieldDefs->Add("ITEM_NAME", ftString, 50);
			cdsMenu->FieldDefs->Add("SIZE_NAME", ftString, 50);
			cdsMenu->FieldDefs->Add("PRICE", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("PRICEEXC", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("MENU_COST", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("SPECIAL_PRICE", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("SPECIAL_PRICEEXC", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("DIFFERENCE", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("POINTS_PERCENT", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("GST_PERCENT", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("CODE", ftString, 25);
			cdsMenu->FieldDefs->Add("QTY", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("LOCATION", ftString, 25);
        cdsMenu->FieldDefs->Add("BasePrice", ftCurrency, 0);
			//cdsMenu->FieldDefs->Add("COST", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("COST_TIMES_QTY", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("PROFIT", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("GP", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("GP_POINTS", ftCurrency, 0);
			cdsMenu->FieldDefs->Add("GP_SPECIAL_PRICE", ftCurrency, 0);
			cdsMenu->CreateDataSet();
		 }
		 catch (Exception &E)
		 {
			Application->MessageBox(("Create cdsMenu table failed, procedure SetupMenuProfit.\r " +  E.Message ).c_str(), "Error", MB_ICONERROR + MB_OK);
			return;
		 }
	 }
	 cdsMenu->LogChanges = false;

	/* report changed 27 May 2005 to exclude GST Options and have them all on one page
	 it also uses the average cost to keep it the same as the menu editor
	 NB the default is to use the Stock Cost but where Stock is not used, then use
	 the Cost from the Menu Editor
	*/
	 qrMenuStock->Close();
	 qrMenuStock->SQL->Text =
	 	"Select "
	 		"Stock.Code,"
	 		"Stock.Description,"
			"Stock.Stocktake_Unit,"
	 		"StockLocation.Average_Cost Cost "
	 		//"StockLocation.Latest_Cost  Cost "
	 		//"Cast(Latest_Cost * (GST_Percent + 100) / 100 as Numeric(17,4)) Cost "
	 	"From "
	 		"Stock Left Join StockLocation On "
	 		"Stock.Stock_Key = StockLocation.Stock_Key "
	 	"Where "
	 		"Stock.Code = :Code And "
	 		"StockLocation.Location = :Location";

	 qrMenu->Close();
	 qrMenu->SQL->Text =
	 	 "Select  "
	 		"Menu.Menu_Name, "
	 		"Course.Course_Name, "
			"Item.Item_Name, "
	 		"ItemSize.ItemSize_Key, "
			"ItemSize.Size_Name, "
			"ItemSize.Price, "
			"cast((ItemSize.Price * 100) / (ItemSize.GST_Percent + 100) as Numeric(17,4)) PriceExc, "
		 //	"ItemSize.Cost Menu_Cost, "
"cast((ItemSize.Cost * 100) / (ItemSize.COST_GST_PERCENT + 100) as Numeric(17,4)) Menu_Cost, "
			"ItemSize.Special_Price, "
			"cast((ItemSize.Special_Price * 100) / (ItemSize.GST_Percent + 100) as Numeric(17,4)) Special_PriceExc, "
			"ItemSize.Price - ItemSize.Special_Price Difference, "
			"ItemSize.Points_Percent, "
			"ItemSize.GST_Percent, "
			"Recipe.Stock_Code Code, "
			"Recipe.Qty, "
			"Recipe.Stock_Location Location  ,   "
            "cast((CASE WHEN VP.IsTax = 1 AND VP.IsSeviceCharge=1 THEN  (ItemSize.Price*100/(100+ coalesce(TAXP.ServiceCharge,0)+TAXP.Tax)) "
                    " WHEN VP.IsTax = 1 AND VP.IsSeviceCharge=0 THEN  (ItemSize.Price*100/(100  +TAXP.Tax)) "
                    "  WHEN VP.IsTax = 0 AND VP.IsSeviceCharge=1 THEN  (ItemSize.Price*100/(100 + coalesce(TAXP.ServiceCharge,0)+TAXP.Tax) ) "
                    " else ItemSize.Price   "
            "END) as numeric(17, 4)) AS BasePrice "
	 	 "From  "
	 		" Menu "
	 		 "Left Join(SELECT  COURSE_KEY,  MENU_KEY,  COURSE_NAME,  COURSE_ID,   CAO,cast(1 as int) keyvalue "
                "FROM COURSE ) Course On  "
	 			" Menu.Menu_Key = Course.Menu_Key  "
			 "Left Join Item On  "
	 			" Course.Course_Key = Item.Course_Key  "
	 		 "Left Join ItemSize On  "
				 "Item.Item_Key = ItemSize.Item_Key "
			 "Left Join Recipe On  "
				 "ItemSize.ItemSize_Key = Recipe.ItemSize_Key  "
				 " LEFT JOIN     "
			   "	  (      "
          "  SELECT a.ITEMSIZE_KEY,   "
              "   Sum(TAXPROFILES.RATE) RATE,  "
                " cast(sum(CASE WHEN TAXPROFILES.TYPE = 2 THEN TAXPROFILES.RATE END) as numeric(17, 4)) AS ServiceCharge, "
               "  cast(sum(CASE WHEN TAXPROFILES.TYPE = 0 THEN  TAXPROFILES.RATE END) as numeric(17, 4)) AS Tax   "

            " FROM ITEMSIZE a  "
            " left join  TAXPROFILES_ITEMSIZE on TAXPROFILES_ITEMSIZE.ITEMSIZE_KEY=a.ITEMSIZE_KEY    "
            " left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TAXPROFILES_ITEMSIZE.PROFILE_KEY   "
            " group by    "
            " a.ITEMSIZE_KEY   "
				"  ) TAXP ON TAXP.ITEMSIZE_KEY= ItemSize.ItemSize_Key  "
				  
	 "left join (SELECT      "
 "MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8000 THEN VARSPROFILE.INTEGER_VAL END) AS IsTax,     "
 " MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8001 THEN VARSPROFILE.INTEGER_VAL END) AS IsSeviceCharge,  "

 "cast(1 as int) keyvalue     "
" FROM VARSPROFILE      "
 ") VP on VP.keyvalue=Course.keyvalue   ";
	 if (Menus->Count > 0)
	 {
		qrMenu->SQL->Text	=	qrMenu->SQL->Text + "Where (" +
									ParamString(Menus->Count, "Menu.Menu_Name", "MenuParam") + ")";
	 }
	 qrMenu->SQL->Text		=	qrMenu->SQL->Text +
	 "Order By ItemSize.ItemSize_Key, "
			" Menu.Menu_Name, "
			" Course.CAO, "
			" Item.IAO, "
			" ItemSize.ISAO ; " ;

	  for (int i=0; i<Menus->Count; i++)
	  {
		qrMenu->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	  }
	  qrMenu->Open();
	  qrMenu->First();
	  int ItemSizeKey = qrMenu->FieldByName("ITEMSIZE_KEY")->AsInteger;
	  Currency CostTimesQty = 0.00,PriceExc = 0.00,BasePrice=0.00,SpecialPriceExc = 0.00,GSTPercent = 0.00,PointsPercent = 0.00;
	  while ( !qrMenu->Eof )
	  {
		  // Multiple same Products from MenuMate looking up the recipes costs from Stock
		  if ( ItemSizeKey == qrMenu->FieldByName("ITEMSIZE_KEY")->AsInteger )
		  {
				// Get Stock Cost
				qrMenuStock->Close();
				qrMenuStock->ParamByName("Code")->AsString		= qrMenu->FieldByName("Code")->AsString;
				qrMenuStock->ParamByName("Location")->AsString	= qrMenu->FieldByName("Location")->AsString;
				qrMenuStock->Open();
				if (qrMenu->FieldByName("Code")->AsString == "")
				{
					CostTimesQty =CostTimesQty +  qrMenu->FieldByName("MENU_COST")->AsCurrency;
				}
				else
				{   CostTimesQty = CostTimesQty + qrMenu->FieldByName("QTY")->AsCurrency * qrMenuStock->FieldByName("Cost")->AsCurrency;
					//CostTimesQty =  qrMenu->FieldByName("QTY")->AsCurrency * qrMenuStock->FieldByName("Cost")->AsCurrency;
				}
				if (cdsMenu->State != dsInsert)
				{
					// only write one record as the details are the same
					cdsMenu->Append();
					cdsMenu->FieldByName("MENU_NAME")->AsString         = qrMenu->FieldByName("MENU_NAME")->AsString;
					cdsMenu->FieldByName("COURSE_NAME")->AsString       = qrMenu->FieldByName("COURSE_NAME")->AsString;
					cdsMenu->FieldByName("ITEM_NAME")->AsString         = qrMenu->FieldByName("ITEM_NAME")->AsString;
					cdsMenu->FieldByName("SIZE_NAME")->AsString         = qrMenu->FieldByName("SIZE_NAME")->AsString;
					cdsMenu->FieldByName("PRICE")->AsCurrency           = qrMenu->FieldByName("PRICE")->AsCurrency;
					cdsMenu->FieldByName("PRICEEXC")->AsCurrency        = qrMenu->FieldByName("PRICEEXC")->AsCurrency;
					cdsMenu->FieldByName("MENU_COST")->AsCurrency       = qrMenu->FieldByName("MENU_COST")->AsCurrency;
					cdsMenu->FieldByName("SPECIAL_PRICE")->AsCurrency   = qrMenu->FieldByName("SPECIAL_PRICE")->AsCurrency;
					cdsMenu->FieldByName("SPECIAL_PRICEEXC")->AsCurrency = qrMenu->FieldByName("SPECIAL_PRICEEXC")->AsCurrency;
					cdsMenu->FieldByName("DIFFERENCE")->AsCurrency      = qrMenu->FieldByName("DIFFERENCE")->AsCurrency;
					cdsMenu->FieldByName("POINTS_PERCENT")->AsCurrency  = qrMenu->FieldByName("POINTS_PERCENT")->AsCurrency;
					cdsMenu->FieldByName("GST_PERCENT")->AsCurrency     = qrMenu->FieldByName("GST_PERCENT")->AsCurrency;
					cdsMenu->FieldByName("CODE")->AsString              = qrMenu->FieldByName("CODE")->AsString;
					cdsMenu->FieldByName("QTY")->AsCurrency             = qrMenu->FieldByName("QTY")->AsCurrency;
					cdsMenu->FieldByName("LOCATION")->AsString          = qrMenu->FieldByName("LOCATION")->AsString;
                    cdsMenu->FieldByName("BasePrice")->AsCurrency          = qrMenu->FieldByName("BasePrice")->AsCurrency;

					cdsMenu->FieldByName("PROFIT")->AsCurrency          = 0.00;
					cdsMenu->FieldByName("GP")->AsCurrency              = 0.00;
					cdsMenu->FieldByName("GP_POINTS")->AsCurrency       = 0.00;
					cdsMenu->FieldByName("GP_SPECIAL_PRICE")->AsCurrency = 0.00;
					// Keep for working out Profit, GP etc
					PriceExc   = qrMenu->FieldByName("PRICEEXC")->AsCurrency;
					GSTPercent = qrMenu->FieldByName("GST_PERCENT")->AsCurrency;
					PointsPercent = qrMenu->FieldByName("POINTS_PERCENT")->AsCurrency;
					SpecialPriceExc = qrMenu->FieldByName("SPECIAL_PRICEEXC")->AsCurrency;
                    BasePrice=  qrMenu->FieldByName("BasePrice")->AsCurrency;
				}
				qrMenu->Next();

		  }
		  else
		  {

				cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency = CostTimesQty;
				// Profit
				if (PriceExc != 0.00)
					cdsMenu->FieldByName("PROFIT")->AsCurrency = BasePrice - cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency;

				// GP
				if (PriceExc != 0.00)
                {
                    if(BasePrice > 0)
                    {
					   cdsMenu->FieldByName("GP")->AsCurrency = (cdsMenu->FieldByName("PROFIT")->AsCurrency / BasePrice) * 100; // when base price > 0...
                    }
                    else
                    {
                       cdsMenu->FieldByName("GP")->AsCurrency = (cdsMenu->FieldByName("PROFIT")->AsCurrency) * 100; // when base price is 0...
                    }
                 }
				// Special Price GP
				if (SpecialPriceExc != 0.00)
					cdsMenu->FieldByName("GP_SPECIAL_PRICE")->AsCurrency = (1 - (cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency / SpecialPriceExc)) * 100;
				// Points GP
				Currency PriceLessPoints = 0.00;
				if (PriceExc != 0.00)
					PriceLessPoints = (BasePrice * 100.0) / (100.0 + PointsPercent);
				//if (PriceLessPoints != 0.00 && cdsMenu->FieldByName("COST_TIMES_QTY").AsCurrency != 0.00)
				if (PriceLessPoints != 0.00)
                {
                    if(PointsPercent != 100)
                    {
                      cdsMenu->FieldByName("GP_POINTS")->AsCurrency =   ((cdsMenu->FieldByName("PROFIT")->AsCurrency )/(BasePrice-(BasePrice*PointsPercent/100)) )*100    ;
                    }
                    else
                    {
                       if(BasePrice > 0)
                       {
                           cdsMenu->FieldByName("GP_POINTS")->AsCurrency =   ((cdsMenu->FieldByName("PROFIT")->AsCurrency )/(BasePrice -(BasePrice/100) ))*100    ;
                       }
                       else
                       {
                          cdsMenu->FieldByName("GP_POINTS")->AsCurrency =   (cdsMenu->FieldByName("PROFIT")->AsCurrency )*100 ;
                       }

                    }
                }



				cdsMenu->Post();
				CostTimesQty = 0.00;
				ItemSizeKey = qrMenu->FieldByName("ITEMSIZE_KEY")->AsInteger;
		  }
	  } // while
	  // write the last record
	  if (cdsMenu->State == dsInsert)
	  {
		 cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency = CostTimesQty;
/*		 if (CostTimesQty == 0.00)
		 {
			cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency = qrMenu->FieldByName("MENU_COST")->AsCurrency;
		 }
		 else
		 {
			cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency = CostTimesQty;
		 }*/
		 // Profit
		 if (BasePrice != 0.00)
			cdsMenu->FieldByName("PROFIT")->AsCurrency = BasePrice - cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency;
		 // GP
		 if (BasePrice != 0.00)
			cdsMenu->FieldByName("GP")->AsCurrency =  (cdsMenu->FieldByName("PROFIT")->AsCurrency / BasePrice) * 100  ;
		 // Special Price GP
		 if (SpecialPriceExc != 0.00)
			cdsMenu->FieldByName("GP_SPECIAL_PRICE")->AsCurrency = (1 - (cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency / SpecialPriceExc)) * 100;
		 // Points GP
		 Currency PriceLessPoints = 0.00;
		 if (BasePrice != 0.00)
			PriceLessPoints = (BasePrice * 100.0) / (100.0 + PointsPercent);
		 //if (PriceLessPoints != 0.00 && cdsMenu->FieldByName("COST_TIMES_QTY").AsCurrency != 0.00)
		 if (PriceLessPoints != 0.00)
         	cdsMenu->FieldByName("GP_POINTS")->AsCurrency =   ((cdsMenu->FieldByName("PROFIT")->AsCurrency )/(BasePrice-(BasePrice*PointsPercent/100)) )* 100    ;

		  //	cdsMenu->FieldByName("GP_POINTS")->AsCurrency = (1 - (cdsMenu->FieldByName("COST_TIMES_QTY")->AsCurrency / PriceLessPoints)) * 100;
		 cdsMenu->Post();
	  }
	// TestData(qrMenu, true);
}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrMenuAfterScroll(TDataSet *DataSet)
{

		qrMenuStock->Close();
		qrMenuStock->ParamByName("Code")->AsString		= qrMenu->FieldByName("Code")->AsString;
		qrMenuStock->ParamByName("Location")->AsString	= qrMenu->FieldByName("Location")->AsString;
		qrMenuStock->Open();
  //	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupStockReductionItems(TStrings *Locations, TStrings *Groups)
{
	qrReductionItems->Close();
	qrReductionStock->Close();

	qrReductionStock->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,"
			"Stock.Description,"
			"Stock.Stocktake_Unit "
		"From "
			"Stock Inner Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Groups && Groups->Count > 0)
	{
		qrReductionStock->SQL->Text	=	qrReductionStock->SQL->Text + "And (" +
													ParamString(Groups->Count, "StockGroup.Stock_Group", "GroupParam") + ")";
	}
	qrReductionStock->SQL->Text		=	qrReductionStock->SQL->Text +
		"Order By "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order,"
			"Stock.Description";
	if (Groups)
	{
		for (int i=0; i<Groups->Count; i++)
		{
			qrReductionStock->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
		}
	}
	qrReductionItems->SQL->Text =
		"Select "
			"Menu.Menu_Name,"
			"Course.Course_Name,"
			"Item.Item_Name,"
			"ItemSize.Size_Name,"
			"ItemSize.Price,"
			"Recipe.Stock_Code Code,"
			"Recipe.Qty,"
			"Recipe.Stock_Location Location "
		"From "
			"Menu Left Join Course On "
				"Menu.Menu_Key = Course.Menu_Key "
			"Left Join Item On "
				"Course.Course_Key = Item.Course_Key "
			"Left Join ItemSize On "
				"Item.Item_Key = ItemSize.Item_Key "
			"Left Join Recipe On "
				"ItemSize.ItemSize_Key = Recipe.ItemSize_Key "
		"Where "
			"Recipe.Stock_Code = :Code ";
	if (Locations && Locations->Count > 0)
	{
		qrReductionItems->SQL->Text	=	qrReductionItems->SQL->Text + "And (" +
													ParamString(Locations->Count, "Recipe.Stock_Location", "LocParam") + ")";
	}
	qrReductionItems->SQL->Text		=	qrReductionItems->SQL->Text +
		"Order By "
			"Menu.Menu_Name,"
			"Course.CAO,"
			"Item.IAO,"
			"ItemSize.ISAO,"
			"Recipe.Stock_Location";
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrReductionItems->ParamByName("LocParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrReductionStockAfterScroll(
		TDataSet *DataSet)
{
	qrReductionItems->Close();
	qrReductionItems->ParamByName("Code")->AsString = qrReductionStock->FieldByName("Code")->AsString;
	qrReductionItems->Open();
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupMenu3rdPartyCodes(TStrings *Menus)
{
	 qrMenu3rdParty->Close();
	 qrMenu3rdParty->SQL->Text =
	 	"Select "
	 		"Menu.Menu_Name,"
	 		"Course.Course_Name,"
			"Item.Item_Name,"
			"ItemSize.ItemSize_Key,"
			"ItemSize.Size_Name,"
			"ItemSize.Price,"
			"ItemSize.Special_Price,"
			"ItemSize.Barcode,"
			"ThirdPartyCodes.Code "
	 	"From "
	 		"Menu Left Join Course On "
	 			"Menu.Menu_Key = Course.Menu_Key "
			"Left Join Item On "
	 			"Course.Course_Key = Item.Course_Key "
	 		"Left Join ItemSize On "
				"Item.Item_Key = ItemSize.Item_Key "
			"Left Join ThirdPartyCodes On "
				"ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key ";
	 if (Menus->Count > 0)
	 {
		qrMenu3rdParty->SQL->Text	=	qrMenu3rdParty->SQL->Text + "Where (" +
												ParamString(Menus->Count, "Menu.Menu_Name", "MenuParam") + ")";
	 }
	 qrMenu3rdParty->SQL->Text		=	qrMenu3rdParty->SQL->Text +
		"Order By "
			"Menu.Menu_Name,"
			"Course.CAO,"
			"Item.IAO,"
			"ItemSize.ISAO";

	for (int i=0; i<Menus->Count; i++)
	{
		qrMenu3rdParty->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCashup(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals)
{
	qrCashupFloat->Close();
	qrCashupFloat->SQL->Text =
		"Select "
			"Terminal_Name,"
			"Sum(Initial_Float) Initial_Float,"
			"Sum(Skims_Total) Skims_Total "
		"From "
			"Zeds "
		"Where "
			"Time_Stamp >= :StartTime and "
			"Time_Stamp < :EndTime ";
	if (Terminals->Count > 0)
	{
		qrCashupFloat->SQL->Text	=	qrCashupFloat->SQL->Text + "and (" +
												ParamString(Terminals->Count, "Terminal_Name", "TerminalParam") + ")";
	}
	qrCashupFloat->SQL->Text		=	qrCashupFloat->SQL->Text +
		"Group By "
			"Terminal_Name "
		"Order By "
			"Terminal_Name";
	for (int i=0; i<Terminals->Count; i++)
	{
		qrCashupFloat->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrCashupFloat->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCashupFloat->ParamByName("EndTime")->AsDateTime		= EndTime;

	qrCashup->Close();
	qrCashup->SQL->Text =
		"Select "
			"Security.Terminal_Name,"
			"UPPER(ArcBillPay.Pay_Type) Pay_Type,"
			"ArcBillPay.Group_number,"
		 //	"ArcBillPay.Properties,"
			"Sum (ArcBillPay.SubTotal) SubTotal,"

         "cast(Count (distinct ArcBillPay.ArcBill_Key) as int) Trans_Count "
	"From ArcBill "
  "inner join Security on Security.SECURITY_REF=ARCBILL.SECURITY_REF "
  "inner join ARCBILLPAY on ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
  "Where "
   "ArcBill.Time_Stamp >= :StartTime and "
   "ArcBill.Time_Stamp < :EndTime and "
         "ArcBillPay.Properties != 131072 and "
			"Security.Security_Event = 'Billed By' and "
			"ArcBillPay.SubTotal <> 0  ";


	if (Terminals->Count > 0)
	{
		qrCashup->SQL->Text	=	qrCashup->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrCashup->SQL->Text		=	qrCashup->SQL->Text +
		"Group By "
			"Security.Terminal_Name,"
			"ArcBillPay.Tax_Free,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type) "
		  //	"ArcBillPay.Properties "
		"Having "
			"Count (ArcBillPay.ArcBillPay_Key) > 0 "
		"Order By "
			"Security.Terminal_Name Asc, "
			"ArcBillPay.Tax_Free Desc, "
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type) Asc";
	for (int i=0; i<Terminals->Count; i++)
	{
		qrCashup->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrCashup->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCashup->ParamByName("EndTime")->AsDateTime		= EndTime;

	qrCashupTotal->Close();
	qrCashupTotal->SQL->Text =
		"Select "
			"UPPER(ArcBillPay.Pay_Type) Pay_Type,"
			"ArcBillPay.Group_number,"
		  //	"ArcBillPay.Properties,"
			"Sum (ArcBillPay.SubTotal) SubTotal,"
            
         "cast(Count (distinct ArcBillPay.ArcBill_Key) as int) Trans_Count "

        "From ArcBill "
  "inner join Security on Security.SECURITY_REF=ARCBILL.SECURITY_REF "
  "inner join ARCBILLPAY on ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
  "Where "
   "ArcBill.Time_Stamp >= :StartTime and "
   "ArcBill.Time_Stamp < :EndTime and "
         "ArcBillPay.Properties != 131072 and "
			"Security.Security_Event = 'Billed By'    and "
			"ArcBillPay.SubTotal <> 0  ";
	if (Terminals->Count > 0)
	{
		qrCashupTotal->SQL->Text	=	qrCashupTotal->SQL->Text + "and (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrCashupTotal->SQL->Text		=	qrCashupTotal->SQL->Text +
		"Group By "
			"ArcBillPay.Tax_Free,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type)  "
		   //	"ArcBillPay.Properties "
		"Having "
			"Count (ArcBillPay.ArcBillPay_Key) > 0 "
		"Order By "
			"ArcBillPay.Tax_Free Desc,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type) Asc";

	for (int i=0; i<Terminals->Count; i++)
	{
		qrCashupTotal->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrCashupTotal->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCashupTotal->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCashupReconciliation(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals)
{
	qrCashupRecon->Close();
	qrCashupRecon->SQL->Text =
		"Select "
			"Zeds.Z_Key,"
			"Zeds.Time_Stamp,"
			"Zeds.Terminal_Name,"
			"Zeds.Adjustments as Total_Variance,"
			"Security.From_Val as Cashier,"
			"BlindBalance.Payment as Payment_Type,"
			"BlindBalance.Payment_Group,"
			"BlindBalance.Payment_Trans_Qty as Transaction_Qty,"
            "cast(BlindBalance.Blind_Balance as numeric(15, 2)) Blind_Balance, "
			"cast(BlindBalance.System_Balance as numeric(15, 2)) System_Balance,"
			"cast(BlindBalance.Office_Balance as numeric(15, 2)) Office_Balance,"
			"cast((BlindBalance.System_Balance "
            "      - BlindBalance.Office_Balance) as numeric(15, 2)) as Variance "
		"From "
			"Zeds Inner Join Security on "
				"Zeds.Security_Ref = Security.Security_Ref "
			"Left Join BlindBalance on "
				"Zeds.Z_Key = BlindBalance.Z_Key "
		"Where "
			"Zeds.Time_Stamp > :StartDateTime And "
			"Zeds.Time_Stamp <= :EndDateTime ";

	if (Terminals && Terminals->Count > 0)
	{
		qrCashupRecon->SQL->Text = qrCashupRecon->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Zeds.Terminal_Name", "TerminalParam") + ")";
	}
	qrCashupRecon->SQL->Text =	qrCashupRecon->SQL->Text +

		"Order By "
			"Zeds.Time_Stamp,"
			"BlindBalance.Z_Key,"
			"BlindBalance.Payment_Group,"
			"BlindBalance.Payment";

	if (Terminals) for (int i=0; i<Terminals->Count; i++)
	{
		qrCashupTotal->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}

	qrCashupRecon->ParamByName("StartDateTime")->AsDateTime	= StartTime;
	qrCashupRecon->ParamByName("EndDateTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCategoryAnalysis(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, bool GroupByLocation)
{
	qrCategoryAnalysis->Close();
	qrCategoryAnalysis->SQL->Text =
		"Select ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Archive.Order_Location Location,";
	}
	else
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(Archive.Qty) Item_Count,"

	    	"Cast(Sum((Archive.Qty * Archive.BASE_PRICE  ) )+Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"      //sales excl
			"CAST(Sum((Archive.Qty * Archive.Price  ))+Sum(Archive.Discount) AS NUMERIC(17,4)) PriceInc,"
			"CAST(Sum(abs(Archive.Qty) * Archive.Cost) AS NUMERIC(17,4)) Cost,"

            "CAST(Sum(((cast(Archive.Qty * Archive.Price AS NUMERIC(17,4))) + Archive.Discount)) AS NUMERIC(17,4)) - Sum(Archive.Cost) Profit   "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
         " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
           " ((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		     " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable'     "
		     ") ) and "
		 " ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryAnalysis->SQL->Text	=	qrCategoryAnalysis->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrCategoryAnalysis->SQL->Text		=	qrCategoryAnalysis->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Archive.Order_Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "

		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"DayArchive.Order_Location Location,";
	}
	else
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(DayArchive.Qty) Item_Count,"
           	"Cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE  )  ) +Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		//sales excl
			"CAST(Sum((DayArchive.Qty * DayArchive.Price)  )+Sum(DayArchive.Discount) AS NUMERIC(17,4)) PriceInc,"
			"CAST(Sum(abs(DayArchive.Qty) * DayArchive.Cost) AS NUMERIC(17,4)) Cost,"

            "CAST(Sum(((CAST(DayArchive.Qty * DayArchive.Price as Numeric(17,4))) + DayArchive.Discount) ) AS NUMERIC(17,4)) - Sum(DayArchive.Cost) Profit "

		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Left Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		          "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
           " ((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		     " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable'     "
		     "  )  ) and "
		 "DAYARCHIVE.PRICE<>0 and "
			"DAYARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
			"DAYARCHIVE.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryAnalysis->SQL->Text	=	qrCategoryAnalysis->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrCategoryAnalysis->SQL->Text		=	qrCategoryAnalysis->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"DayArchive.Order_Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "

		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Orders.Order_Location Location,";
	}
	else
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(Orders.Qty) Item_Count,"

       " Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) +Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc ,    "
			"CAST(Sum((Orders.Qty * Orders.Price)  )+Sum(Orders.Discount) AS NUMERIC(17,4)) PriceInc,"
			"CAST(Sum(abs(Orders.Qty) * Orders.Cost) AS NUMERIC(17,4)) Cost,"

                "CAST (CAST(Sum(caST(((CAST(Orders.Qty * Orders.Price AS NUMERIC(17,4))) + Orders.Discount) AS NUMERIC(17,4))) AS NUMERIC(17,4)) - Sum(Orders.Cost) AS NUMERIC(17,4) ) Profit 	 "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Left Join ArcCategories on "
				"Orders.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"(Orders.Order_Type = 3 or "
			"Orders.Order_Type = 0) and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryAnalysis->SQL->Text	=	qrCategoryAnalysis->SQL->Text + "and (" +
													ParamString(Locations->Count, "Orders.Order_Location", "LocationParam") + ")";
	}
	qrCategoryAnalysis->SQL->Text		=	qrCategoryAnalysis->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"Orders.Order_Location,";
	}
	qrCategoryAnalysis->SQL->Text = qrCategoryAnalysis->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "

		"Order By "
			"1, 2, 3";

	qrCatLocTotal->Close();
	qrCatLocTotal->SQL->Text =
		"Select ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"Archive.Order_Location Location,";
	}
	else
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"CAST(Sum((Archive.Qty * Archive.Price) + Archive.Discount) AS NUMERIC(17,4)) PriceInc "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
		      "left join ARCORDERDISCOUNTS on ARCORDERDISCOUNTS.ARCHIVE_KEY = Archive.ARCHIVE_KEY "
		"Where "
		    " ((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		    " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable'    "
		    "  )  ) and  "
		 " ARCHIVE.PRICE<>0 and "
			"ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
			"ARCHIVE.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Group By "
			"Archive.Order_Location ";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"DayArchive.Order_Location Location,";
	}
	else
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"CAST(Sum((DayArchive.Qty * DayArchive.Price) + DayArchive.Discount) AS NUMERIC(17,4)) PriceInc "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    " ((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		    " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME ,0)<> 'Non-Chargeable'    "
		    "  )  ) and  "
			 "DAYARCHIVE.PRICE<>0 and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Group By "
			"DayArchive.Order_Location ";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"Orders.Order_Location Location,";
	}
	else
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
			"CAST(Sum((Orders.Qty * Orders.Price) + Orders.Discount) AS NUMERIC(17,4)) PriceInc "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"(Orders.Order_Type = 3 or "
			"Orders.Order_Type = 0) and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (GroupByLocation)
	{
		qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Group By "
			"Orders.Order_Location ";
	}
	qrCatLocTotal->SQL->Text = qrCatLocTotal->SQL->Text +
		"Order By "
			"1 ";

	qrCategoryAnalysis->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCategoryAnalysis->ParamByName("EndTime")->AsDateTime	= EndTime;
	qrCatLocTotal->ParamByName("StartTime")->AsDateTime		= StartTime;
	qrCatLocTotal->ParamByName("EndTime")->AsDateTime			= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrCategoryAnalysis->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}


}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCategoryBreakdown(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, bool GroupByLocation)
{
qrCategoryBreakdown->Close();
	qrCategoryBreakdown->SQL->Text =
		"Select ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"Archive.Order_Location Location,";
	}
	else
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(Archive.Qty) Item_Count,"

              "Sum(CAST(Archive.BASE_PRICE As Numeric(17,2)) * CAST(Archive.Qty As Numeric(17,4)) ) PriceExc ,    "
			" Sum(CAST(Archive.Price As Numeric(17,4)) * CAST( Archive.Qty As Numeric(17,4))+ Archive.Discount ) PriceInc,   "
			" Sum(CAST(Archive.Cost As Numeric(17,4)) * Archive.Qty ) Cost   "

		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Left Join ArcCategory on "
				"Archive.Archive_Key = ArcCategory.Archive_Key "
			"Left Join ArcCategories on "
				"ArcCategory.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"left join ARCORDERDISCOUNTS on ARCORDERDISCOUNTS.ARCHIVE_KEY = Archive.ARCHIVE_KEY	"
		   "Where "
		     "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		     "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'NonChargable' and  "
		     "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or Archive.DISCOUNT = 0) and "
			"(Archive.Order_Type = 3 or "
			"Archive.Order_Type = 0) and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"(Security.Security_Event = 'Ordered By') ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryBreakdown->SQL->Text	=	qrCategoryBreakdown->SQL->Text + "and (" +
										ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrCategoryBreakdown->SQL->Text		=	qrCategoryBreakdown->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text	= qrCategoryBreakdown->SQL->Text +
			"Archive.Order_Location,";
	}
	qrCategoryBreakdown->SQL->Text		= qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "
		"Having "
			"Count(Archive.Archive_Key) > 0 And "
			"ArcCategories.Category Is Not Null "

		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"DayArchive.Order_Location Location,";
	}
	else
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(DayArchive.Qty) Item_Count,"
		 "Sum(CAST(DayArchive.BASE_PRICE As Numeric(17,2)) * CAST(DayArchive.Qty As Numeric(17,4)) ) PriceExc ,    "
			" Sum(CAST(DayArchive.Price As Numeric(17,4)) * CAST( DayArchive.Qty As Numeric(17,4))+ DayArchive.Discount ) PriceInc,   "
			" Sum(CAST(DayArchive.Cost As Numeric(17,4)) * DayArchive.Qty ) Cost   "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Left Join DayArcCategory on "
				"DayArchive.Archive_Key = DayArcCategory.Archive_Key "
			"Left Join ArcCategories on "
				"DayArcCategory.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"left join DAYARCORDERDISCOUNTS on DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DayArchive.ARCHIVE_KEY	"
		   "Where "
		     "((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
		     "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'NonChargable' and  "
		     "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_KEY,0) > 0) or DayArchive.DISCOUNT = 0) and "
			"(DayArchive.Order_Type = 3 or "
			"DayArchive.Order_Type = 0) and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"(Security.Security_Event = 'Ordered By') ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryBreakdown->SQL->Text	=	qrCategoryBreakdown->SQL->Text + "and (" +
														ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrCategoryBreakdown->SQL->Text		=	qrCategoryBreakdown->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text	= qrCategoryBreakdown->SQL->Text +
			"DayArchive.Order_Location,";
	}
	qrCategoryBreakdown->SQL->Text		= qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 And "
			"ArcCategories.Category Is Not Null "

		"Union All "

		"Select ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"Orders.Order_Location Location,";
	}
	else
	{
		qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrCategoryBreakdown->SQL->Text = qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"ArcCategories.Category,"
			"Sum(Orders.Qty) Item_Count,"

              "Sum(CAST(Orders.BASE_PRICE As Numeric(17,2)) * CAST(Orders.Qty As Numeric(17,4)) ) PriceExc ,    "
			" Sum(CAST(Orders.Price As Numeric(17,4)) * CAST( Orders.Qty As Numeric(17,4))+ Orders.Discount ) PriceInc,   "
			" Sum(CAST(Orders.Cost As Numeric(17,4)) * Orders.Qty ) Cost   "

		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Left Join OrderCategory on "
				"Orders.Order_Key = OrderCategory.Order_Key "
			"Left Join ArcCategories on "
				"OrderCategory.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"(Security.Security_Event = 'Ordered By') ";
	if (Locations && Locations->Count > 0)
	{
		qrCategoryBreakdown->SQL->Text	=	qrCategoryBreakdown->SQL->Text + "and (" +
														ParamString(Locations->Count, "Orders.Order_Location", "LocationParam") + ")";
	}
	qrCategoryBreakdown->SQL->Text		=	qrCategoryBreakdown->SQL->Text +
		"Group By ";
	if (GroupByLocation)
	{
		qrCategoryBreakdown->SQL->Text	= qrCategoryBreakdown->SQL->Text +
			"Orders.Order_Location,";
	}
	qrCategoryBreakdown->SQL->Text		= qrCategoryBreakdown->SQL->Text +
			"CategoryGroups.Name,"
			"ArcCategories.Category "
		"Having "
			"Count(Orders.Order_Key) > 0 And "
			"ArcCategories.Category Is Not Null "

		"Order By "
			"1, 2,3";
	qrCategoryBreakdown->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCategoryBreakdown->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrCategoryBreakdown->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupHalfHourlyDaily(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals)
{
	qrHalfHoulrySummary->Close();
	qrHalfHoulrySummary->SQL->Text =
		"Select "
			"cast ( 'D' as Char(1)) Report_Type,"
			"ArcBill.Billed_Location,"
			"Security.Time_Stamp,"
			"Extract (Minute From ArcBill.Time_Stamp) / 30 Bill_Half_Hour,"
			"Extract (Minute From ArcBill.Time_Stamp) Bill_Minute,"
			"Extract (Hour From ArcBill.Time_Stamp) Bill_Hour,"
			"Extract (Day From ArcBill.Time_Stamp) Bill_Day,"
			"Extract (Month From ArcBill.Time_Stamp) Bill_Month,"
			"Extract (Year From ArcBill.Time_Stamp) Bill_Year,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From ArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From ArcBill.Time_Stamp) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From ArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From ArcBill.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
			"cast( ArcBill.Total as Numeric (17,4) ) Bill_Total,"
			"ArcBill.Patron_Count, "
		    "Cast(sum(archive.qty) as numeric(17,4)) SalesQty, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join ARCBILL on  "
         "Security.Security_Ref = ARCBILL.Security_Ref "
			"left join ARCHIVE on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
			"left join ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable') ) and "
			"Security.Security_Ref = ArcBill.Security_Ref and "
			"ArcBill.ArcBill_Key Is Not Null and "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' "
         " group by 1,2,3,4,5,6,7,8,9,10,11,12,13 ";
	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "ArcBill.Terminal_Name", "TerminalParam") + ")";
	}
	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +
		"Union All "

		"Select "
			"cast ( 'D' as Char(1)) Report_Type,"
			"DayArcBill.Billed_Location,"
			"DayArcBill.Time_Stamp,"
			"Extract (Minute From DayArcBill.Time_Stamp) / 30 Bill_Half_Hour,"
			"Extract (Minute From DayArcBill.Time_Stamp) Bill_Minute,"
			"Extract (Hour From DayArcBill.Time_Stamp) Bill_Hour,"
			"Extract (Day From DayArcBill.Time_Stamp) Bill_Day,"
			"Extract (Month From DayArcBill.Time_Stamp) Bill_Month,"
			"Extract (Year From DayArcBill.Time_Stamp) Bill_Year,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DayArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArcBill.Time_Stamp) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DayArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArcBill.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
			"cast( DayArcBill.Total as Numeric(17,4) ) Bill_Total,"
			"DayArcBill.Patron_Count, "
		    "Cast(sum(dayarchive.qty) as numeric(17,4)) SalesQty, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DAYARCBILL on  "
         "Security.Security_Ref = DAYARCBILL.Security_Ref "
			"left join DAYARCHIVE on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
			"left join DAYARCORDERDISCOUNTS on DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    "((COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
          "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' ) ) and "
           "DAYARCHIVE.PRICE<>0 and "
			"Security.Security_Ref = DayArcBill.Security_Ref and "
			"DayArcBill.ArcBill_Key Is Not Null and "
			"DayArcBill.Time_Stamp >= :StartTime and "
			"DayArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' "
         " group by 1,2,3,4,5,6,7,8,9,10,11,12,13 " ;
	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "DayArcBill.Terminal_Name", "TerminalParam") + ")";
	}
	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +

		"Order By "
			"2,3";

	for (int i=0; i<Terminals->Count; i++)
	{
		qrHalfHoulrySummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}

	qrHalfHoulrySummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrHalfHoulrySummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	AddInZeroHalfHours();
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupHalfHourlyConsolidated(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals)
{
	// View is for Grouping by Bill_Hour and Bill_Half_Hour
	// NB dont use ParamByName for Views -...
	sqlView->Close();
	sqlView->SQL->Text =
		"CREATE VIEW HalfHour(ArcBill_Key,Bill_Hour,Bill_Half_Hour) AS "
		"SELECT ArcBill.ArcBill_Key,"
			"Extract (Hour From ArcBill.Time_Stamp) Bill_Hour,"
			"Extract (Minute From ArcBill.Time_Stamp) / 30 Bill_Half_Hour "
		"FROM ArcBill,Security "
		"WHERE Security.Security_Ref = ArcBill.Security_Ref "
		"AND ArcBill.Time_Stamp between '" +
		 FormatDateTime("mm/dd/yy hh:mm",StartTime) + "' and '" +  FormatDateTime("mm/dd/yy hh:mm",EndTime) +
		"' and Security.Security_Event = 'Billed By' and "
		"ArcBill.ArcBill_Key Is Not Null "
		"Union All "
		"SELECT DayArcBill.ArcBill_Key,"
			"Extract (Hour From DayArcBill.Time_Stamp) Bill_Hour,"
			"Extract (Minute From DayArcBill.Time_Stamp) / 30 Bill_Half_Hour "
		"FROM DayArcBill,Security "
		"WHERE Security.Security_Ref = DayArcBill.Security_Ref "
		"AND DayArcBill.Time_Stamp between '" +
		FormatDateTime("mm/dd/yy hh:mm",StartTime) + "' and '" +  FormatDateTime("mm/dd/yy hh:mm",EndTime) +
		"' and Security.Security_Event = 'Billed By' and "
		"DayArcBill.ArcBill_Key Is Not Null";
	sqlView->ExecQuery();
	sqlView->Close();

	if (MMTrans->InTransaction == true )
	{
		MMTrans->Commit();
		MMTrans->StartTransaction();
	}

	qrHalfHoulrySummary->Close();
	qrHalfHoulrySummary->SQL->Text =
		"Select "
			"cast ( 'C' as Char(1)) Report_Type,"
			"ArcBill.Billed_Location,"
			"cast ('" +	 FormatDateTime("mm/dd/yy",StartTime) + "' AS TimeStamp) Time_Stamp," +
			"HALFHOUR.Bill_Half_Hour,"
			"cast(1 as integer) Bill_Minute,"
			"HALFHOUR.Bill_Hour,"
			"cast(1 as integer) Bill_Day,"
			"cast(1 as integer) Bill_Month,"
			"cast(2005 as integer) Bill_Year,"
			"min(cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From ArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From ArcBill.Time_Stamp) * 60 * 60) As Double Precision) / 86400 as Time)) Start_Time,"
			"min(cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From ArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From ArcBill.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400 as Time)) End_Time,"
			"cast (Sum(ArcBill.Total) as Numeric(17,4)) Bill_Total,"
			"cast (Sum(ArcBill.Patron_Count) as Integer) Patron_Count, "
			"cast(count(HALFHOUR.ArcBill_Key)as numeric(17,4))  SalesQty, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
		"From "
			"Security, ArcBill, HALFHOUR "
		"Where "
			"Security.Security_Ref = ArcBill.Security_Ref and "
			"ArcBill.ArcBill_Key = HALFHOUR.ArcBill_Key and "
			"ArcBill.ArcBill_Key Is Not Null and "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "ArcBill.Terminal_Name", "TerminalParam") + ")";
	}
	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +
		"Group By "
			"ArcBill.Billed_Location,"
			"HALFHOUR.Bill_Hour ,"
			"HALFHOUR.Bill_Half_Hour "
		"Union All "

		"Select "
			"cast ( 'C' as Char(1)) Report_Type,"
			"DayArcBill.Billed_Location,"
			"cast ('" +	 FormatDateTime("mm/dd/yy",StartTime) + "' AS TimeStamp) Time_Stamp," +
			"HALFHOUR.Bill_Half_Hour,"
			"cast(1 as integer) Bill_Minute,"
			"HALFHOUR.Bill_Hour,"
			"cast(1 as integer) Bill_Day,"
			"cast(1 as integer) Bill_Month,"
			"cast(2005 as integer) Bill_Year,"
			"min(cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DayArcBill.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArcBill.Time_Stamp) * 60 * 60) As Double Precision) / 86400 as Time)) Start_Time,"
			"min(cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Security.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArcBill.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400 as Time)) End_Time,"
			"cast (sum(DayArcBill.Total) as Numeric(17,4)) Bill_Total,"
			"cast (Sum(DayArcBill.Patron_Count) as Integer) Patron_Count, "
			"cast(count(HALFHOUR.ArcBill_Key)as numeric(17,4)) SalesQty, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "

		"From "
			"Security, DayArcBill, HALFHOUR "
		"Where "
			"Security.Security_Ref = DayArcBill.Security_Ref and "
			"DayArcBill.ArcBill_Key = HALFHOUR.ArcBill_Key and "
			"DayArcBill.ArcBill_Key Is Not Null and "
			"DayArcBill.Time_Stamp >= :StartTime and "
			"DayArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "DayArcBill.Terminal_Name", "TerminalParam") + ")";
	}
	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +
		"Group By "
			"DayArcBill.Billed_Location,"
			"HALFHOUR.Bill_Hour ,"
			"HALFHOUR.Bill_Half_Hour "
		"Order By "
			"2,6,4";

	for (int i=0; i<Terminals->Count; i++)
	{
		qrHalfHoulrySummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}

	qrHalfHoulrySummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrHalfHoulrySummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	AddInZeroHalfHours();
	//TestData(qrHalfHoulrySummary, true);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupTipsAndVouchers(TDateTime StartTime, TDateTime EndTime, TStrings *PayTypes)
{
	qrTipsVouchers->Close();
	qrTipsVouchers->SQL->Text =
		"Select "
			"ArcBill.Time_Stamp,"
			"Contacts.Name,"
			"UPPER(ArcSurcharge.Pay_Type) Pay_Type ,"
			"ArcSurcharge.Pay_Type_Details,"
			"ArcSurcharge.SubTotal,"
			"ArcBill.Invoice_Number "
		"From "
			"ArcSurcharge Left Join ArcBill On "
			"ArcSurcharge.Arcbill_Key = ArcBill.Arcbill_Key "
			"Left Join Security On "
			"ArcBill.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
			"Security.User_Key = Contacts.Contacts_Key "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (PayTypes->Count > 0)
	{
		qrTipsVouchers->SQL->Text	=	qrTipsVouchers->SQL->Text + "and (" +
												ParamString(PayTypes->Count, "ArcSurcharge.Pay_Type", "PayTypeParam") + ")";
	}
	qrTipsVouchers->SQL->Text		=			qrTipsVouchers->SQL->Text +
		"Order By "
			"UPPER(ArcSurcharge.Pay_Type),"
			"ArcBill.Time_Stamp";

	for (int i=0; i<PayTypes->Count; i++)
	{
		qrTipsVouchers->ParamByName("PayTypeParam" + IntToStr(i))->AsString = PayTypes->Strings[i];
	}

	qrTipsVouchers->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrTipsVouchers->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupWagesByDepatment(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrWages->Close();
	qrWages->SQL->Text =
        "select "
           "Contact_Type, "
           "Name, "
           "Payroll_ID, "
           "cast(Login_DateTime as timestamp) Login_DateTime, "
           "cast(Logout_DateTime as timestamp) Logout_DateTime, "
           "Breaks, "
           "cast((tt - bd) * 24 as float) Hours_Worked, "
           "cast((tt - bd) as float) Days_Worked, "
           "Department, "
           "Zone, "
           "Modified "
           "from (select (ct.logout_datetime - ct.login_datetime) tt, "
                        "(cast(ct.breaks as timestamp) "
                        " - cast((current_date || ', 00:00:00.000') as timestamp)) bd, "
                        "C.Contact_Type, "
                        "C.Name, "
                        "C.Payroll_ID, "
                        "cast(ct.Login_DateTime as timestamp) Login_DateTime, "
                        "cast(ct.Logout_DateTime as timestamp) Logout_DateTime, "
                        "ct.Breaks, "
                        "TCL.Name Department, "
                        "TCL.Code Zone, "
                        "ct.Modified "
                        "from contacttime ct "
                            "Inner Join Contacts C On "
                                "C.Contacts_Key = ct.Contacts_Key "
                            "Left Join TimeClockLocations TCL On "
                                "ct.TimeClockLocations_Key = TCL.TimeClockLocations_Key "
                        "where ct.breaks is not null "
        			          "and ct.Logout_DateTime >= :StartTime "
                              "and ct.Logout_DateTime < :EndTime "
                              "and ct.logout_datetime is not null "
                              "and (C.Contact_Type = 0 "
                              "or C.Contact_Type = 1) ";

    if (Names && Names->Count > 0)
	{
		qrWages->SQL->Text	=	qrWages->SQL->Text + "and (" +
										ParamString(Names->Count, "C.Name", "NamesParam") + ")";
	}
	qrWages->SQL->Text		=	qrWages->SQL->Text +

                            " ) "

    "Union All "

        "select "
           "Contact_Type, "
           "Name, "
           "Payroll_ID, "
           "cast(Login_DateTime as timestamp) Login_DateTime, "
           "cast(Logout_DateTime as timestamp) Logout_DateTime, "
           "Breaks, "
           "(tt) * 24 Hours_Worked, "
           "(tt) Days_Worked, "
           "Department, "
           "Zone, "
           "Modified "
           "from (select (ct.logout_datetime - ct.login_datetime) tt, "
                        "C.Contact_Type, "
                        "C.Name, "
                        "C.Payroll_ID, "
                        "cast(ct.Login_DateTime as timestamp) Login_DateTime, "
                        "cast(ct.Logout_DateTime as timestamp) Logout_DateTime, "
                        "ct.Breaks, "
                        "TCL.Name Department, "
                        "TCL.Code Zone, "
                        "ct.Modified "
                        "from contacttime ct "
                            "Inner Join Contacts C On "
                                "C.Contacts_Key = ct.Contacts_Key "
                            "Left Join TimeClockLocations TCL On "
                                "ct.TimeClockLocations_Key = TCL.TimeClockLocations_Key "
                        "where ct.breaks is null "
        			          "and ct.Logout_DateTime >= :StartTime "
                              "and ct.Logout_DateTime < :EndTime "
                              "and ct.logout_datetime is not null "
                              "and (C.Contact_Type = 0 "
                              "or C.Contact_Type = 1) ";

    if (Names && Names->Count > 0)
	{
		qrWages->SQL->Text	=	qrWages->SQL->Text + "and (" +
										ParamString(Names->Count, "C.Name", "NamesParam") + ")";
	}
	qrWages->SQL->Text		=	qrWages->SQL->Text +

                            " )"

                            "Order by "
                                "9,2,4";

	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrWages->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}

	qrWages->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrWages->ParamByName("EndTime")->AsDateTime		= EndTime;

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupWagesByStaff(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrWages->Close();
	qrWages->SQL->Text =
        "select "
           "Contact_Type, "
           "Name, "
           "Payroll_ID, "
           "cast(Login_DateTime as timestamp) Login_DateTime, "
           "cast(Logout_DateTime as timestamp) Logout_DateTime, "
           "Breaks, "
           "cast((tt - bd) * 24 as float) Hours_Worked, "
           "cast((tt - bd) as float) Days_Worked, "
           "Department, "
           "Zone, "
           "Modified "
           "from (select (ct.logout_datetime - ct.login_datetime) tt, "
                        "(cast(ct.breaks as timestamp) "
                        " - cast((current_date || ', 00:00:00.000') as timestamp)) bd, "
                        "C.Contact_Type, "
                        "C.Name, "
                        "C.Payroll_ID, "
                        "cast(ct.Login_DateTime as timestamp) Login_DateTime, "
                        "cast(ct.Logout_DateTime as timestamp) Logout_DateTime, "
                        "ct.Breaks, "
                        "TCL.Name Department, "
                        "TCL.Code Zone, "
                        "ct.Modified "
                        "from contacttime ct "
                            "Inner Join Contacts C On "
                                "C.Contacts_Key = ct.Contacts_Key "
                            "Left Join TimeClockLocations TCL On "
                                "ct.TimeClockLocations_Key = TCL.TimeClockLocations_Key "
                        "where ct.breaks is not null "
        			          "and ct.Logout_DateTime >= :StartTime "
                              "and ct.Logout_DateTime < :EndTime "
                              "and ct.logout_datetime is not null "
                              "and (C.Contact_Type = 0 "
                              "or C.Contact_Type = 1) ";

    if (Names && Names->Count > 0)
	{
		qrWages->SQL->Text	=	qrWages->SQL->Text + "and (" +
										ParamString(Names->Count, "C.Name", "NamesParam") + ")";
	}
	qrWages->SQL->Text		=	qrWages->SQL->Text +

                            " ) "

    "Union All "

        "select "
           "Contact_Type, "
           "Name, "
           "Payroll_ID, "
           "cast(Login_DateTime as timestamp) Login_DateTime, "
           "cast(Logout_DateTime as timestamp) Logout_DateTime, "
           "Breaks, "
           "(tt) * 24 Hours_Worked, "
           "(tt) Days_Worked, "
           "Department, "
           "Zone, "
           "Modified "
           "from (select (ct.logout_datetime - ct.login_datetime) tt, "
                        "C.Contact_Type, "
                        "C.Name, "
                        "C.Payroll_ID, "
                        "cast(ct.Login_DateTime as timestamp) Login_DateTime, "
                        "cast(ct.Logout_DateTime as timestamp) Logout_DateTime, "
                        "ct.Breaks, "
                        "TCL.Name Department, "
                        "TCL.Code Zone, "
                        "ct.Modified "
                        "from contacttime ct "
                            "Inner Join Contacts C On "
                                "C.Contacts_Key = ct.Contacts_Key "
                            "Left Join TimeClockLocations TCL On "
                                "ct.TimeClockLocations_Key = TCL.TimeClockLocations_Key "
                        "where ct.breaks is null "
        			          "and ct.Logout_DateTime >= :StartTime "
                              "and ct.Logout_DateTime < :EndTime "
                              "and ct.logout_datetime is not null "
                              "and (C.Contact_Type = 0 "
                              "or C.Contact_Type = 1) ";

    if (Names && Names->Count > 0)
	{
		qrWages->SQL->Text	=	qrWages->SQL->Text + "and (" +
										ParamString(Names->Count, "C.Name", "NamesParam") + ")";
	}
	qrWages->SQL->Text		=	qrWages->SQL->Text +

                            " )"

                            "Order by "
                                "2,9,4";

	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrWages->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}

	qrWages->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrWages->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupSavedSales(TStrings *TabTypes)
{
	qrSavedSales->Close();
	qrSavedSales->SQL->Text =
		"Select "
			"Tab.Tab_Type,"
			"Tab.Tab_Name,"
			"Contacts.Name Member_Name,"
			"Tables.Table_Name,"
			"Tables.Table_Number,"
			"cast(Sum(Orders.Qty * Orders.Price + Orders.Discount) as numeric(17, 4)) Total "
		"From "
			"Orders Left Join Tab On "
			"Orders.Tab_Key = Tab.Tab_Key "
			"Left join Seat On Tab.Tab_Key = Seat.Tab_Key "
			"Left Join Tables on Seat.Table_Key = Tables.Table_Key "
			"left join Contacts on Contacts.Contacts_Key = Tab.Contacts_Key ";
	if (TabTypes->Count > 0)
	{
		qrSavedSales->SQL->Text	=	qrSavedSales->SQL->Text + "Where (" +
											ParamString(TabTypes->Count, "Tab.Tab_Type", "TabTypeParam") + ")";
	}
	qrSavedSales->SQL->Text		=	qrSavedSales->SQL->Text +
		"Group By "
			"Tab.Tab_Type,"
			"Tab.Tab_Key,"
			"Tables.Table_Number,"
			"Tables.Table_Name,"
			"Tab.Tab_Name,"
			"Contacts.Name "
		"Order By "
			"Tab.Tab_Type,"
			"Tables.Table_Number,"
			"Tab.Tab_Name,"
			"Contacts.Name";
	for (int i=0; i<TabTypes->Count; i++)
	{
		qrSavedSales->ParamByName("TabTypeParam" + IntToStr(i))->AsString = TabTypes->Strings[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrPatronCountFilterRecord(
		TDataSet *DataSet, bool &Accept)
{
/*	if (qrAveSummary->Active)
	{
		Accept = qrPatronCount->FieldByName("Sales_Type")->AsInteger == qrAveSummary->FieldByName("Sales_Type")->AsInteger;
	}
	else
	{
		Accept = true;
	}*/
}
 
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrAveSummaryAfterScroll(TDataSet *DataSet)
{
//	qrPatronCount->First();
}
//---------------------------------------------------------------------------
 
void TdmMMReportData::SetupDayConsumption(TDateTime StartTime, TDateTime EndTime)
{
	qrDayConsumption->Close();
	qrDayConsumption->SQL->Text =
		"Select "
            "Archive.Time_Stamp, "
            "Extract (Minute From Archive.Time_Stamp) Order_Minute, "
			"Extract (Hour From Archive.Time_Stamp) Order_Hour, "
			"Extract (Day From Archive.Time_Stamp) Order_Day, "
			"Extract (Month From Archive.Time_Stamp) Order_Month, "
			"Extract (Year From Archive.Time_Stamp) Order_Year, "
			"Cast(Sum(Archive.Qty * Archive.Price ) +   Sum(Archive.Discount) as Numeric(17,4)) Price,"
			"Sum((Archive.Qty * abs(Archive.BASE_PRICE)) ) + Sum(Archive.DISCOUNT_WITHOUT_TAX) As PriceExc,"						//sales excl
			"Cast(Sum(abs(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
         "Left Join ArcCategories on "
         	"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
         " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                "FROM ARCORDERDISCOUNTS a "
                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
                
        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
        " (( "
           " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
           "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

			"Archive.Time_Stamp >= :StartTime and "
			"Archive.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Group By "
            "Archive.Time_Stamp "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

        "Union All "
        "Select "
           " writeoff.Time_Stamp, "
            "Extract (Minute From writeoff.Time_Stamp) Order_Minute,  "
			"Extract (Hour From writeoff.Time_Stamp) Order_Hour, "
		   "	Extract (Day From writeoff.Time_Stamp) Order_Day,  "
			"Extract (Month From writeoff.Time_Stamp) Order_Month, "
			"Extract (Year From writeoff.Time_Stamp) Order_Year,   "
			"Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price,  "
           " cast(0 as int) PriceExc, "
  			"Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
		"From   "
			"WRITEOFF Left Join  ArcCategories on   "
         	"WRITEOFF.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on  "
			" ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
            "where "
            "WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime "
        "Group By   "    

			"WRITEOFF.TIME_STAMP "

		"Union All "

		"select "
            "ORDERS.TIME_STAMP, "
            "Extract (Minute From ORDERS.Time_Stamp) Order_Minute, "
			"Extract (Hour From ORDERS.Time_Stamp) Order_Hour, "
			"Extract (Day From ORDERS.Time_Stamp) Order_Day, "
			"Extract (Month From ORDERS.Time_Stamp) Order_Month,  "
			"Extract (Year From ORDERS.Time_Stamp) Order_Year,  "
			"Cast(Sum((ORDERS.QTY * ORDERS.PRICE) ) +   Sum(ORDERS.DISCOUNT) as Numeric(17,4)) Price, "
			"Sum((ORDERS.Qty * ORDERS.BASE_PRICE ) ) +   Sum(ORDERS.DISCOUNT_WITHOUT_TAX) as PriceExc, "
			"Cast(Sum(abs(ORDERS.Cost) * ORDERS.Qty) as Numeric(17,4)) Cost, "
			+  _selectSalesIncl + //For Selecting salesIncl column

        "From  "
            "Security Left Join ORDERS on  "
                "Security.Security_Ref = ORDERS.Security_Ref "
            "Left Join ArcCategories on "
                "ORDERS.Category_Key = ArcCategories.Category_Key  "
            "Left Join CategoryGroups on  "
                "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
            + _taxJoins + ///For selecting tax
        "Where "
            "security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
            "ORDERS.Time_Stamp >= :StartTime and "
            "ORDERS.Time_Stamp < :EndTime and "
            "Security.Security_Event = 'Ordered By' "
        "Group By "
            "ORDERS.Time_Stamp, "
            +  _groupByClause + ///group by taxes

        "Union All "

		"Select "
            "DayArchive.Time_Stamp, "
            "Extract (Minute From DayArchive.Time_Stamp) Order_Minute, "
			"Extract (Hour From DayArchive.Time_Stamp) Order_Hour, "
			"Extract (Day From DayArchive.Time_Stamp) Order_Day, "
			"Extract (Month From DayArchive.Time_Stamp) Order_Month, "
			"Extract (Year From DayArchive.Time_Stamp) Order_Year, "

			"Cast(Sum((DayArchive.Qty * DAYARCHIVE.PRICE) ) +   Sum(DAYArchive.DISCOUNT) as Numeric(17,4)) Price,"				//sales excl
			"Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE) ) ) +   Sum(DAYArchive.DISCOUNT_WITHOUT_TAX) as PriceExc,"
			"Cast(Sum(abs(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
         "inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
         "Left Join ArcCategories on "
         	"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
           "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
         "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                        "FROM DAYARCORDERDISCOUNTS a "
                        "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
        " (( "
          "  COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

			"DayArchive.Time_Stamp >= :StartTime and "
			"DayArchive.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' "
		"Group By "
            "DayArchive.Time_Stamp "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "
		"Order By "
			"1,2,3,4";

	qrDayConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDayConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;

}

void TdmMMReportData::SetupCategoryConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Categories)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
            "SUM (Archive.QTY) Item_Count, "
			"Cast(Sum(Archive.Qty * Archive.Price )+ Sum(Archive.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE) ) ) +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"     //sales excl
			"Cast(Sum(abs(Archive.Cost)* Archive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
         "inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
         "Left Join ArcCategories on "
         	"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
         "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "

      " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

     " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where security.SECURITY_EVENT='CancelY' ) and  "

        "(( "
          "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"Archive.Item_Name,"
			"Archive.Size_Name   "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"SUM (DayArchive.QTY) Item_Count,"
			"Cast(Sum(DayArchive.Qty * DayArchive.Price ) + Sum(DayArchive.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE) ) ) +   Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"   //salex excl
			"Cast(Sum(abs(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost, "
             "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
         "inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
         "Left Join ArcCategories on "
         	"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "

		    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

		  " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "

        "(( "
          "  COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and  "

			"DayArchive.TIME_STAMP >= :StartTime and "
			"DayArchive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name  "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

         "Union All "

         "Select "
			"CategoryGroups.Name Group_Name,  "
			"ArcCategories.Category Course_Name, "
			"Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name,   "
			"WRITEOFF.SIZE_NAME,   "
			"cast(0 as int) Item_Count ,   "
			"Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price,  "
            "cast(0 as int) PriceExc, "
  			"Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost,   "
            "Cast(0.00 as numeric(17,4)) SalesIncl "
		"From    "
			"WRITEOFF Left Join  ArcCategories on   "
         	"WRITEOFF.Category_Key = ArcCategories.Category_Key   "
			"Left Join CategoryGroups on      "
			 "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
             "where "
             	"WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +

			 "Group By       "
			"CategoryGroups.Name,   "
			"ArcCategories.Category,  "
			"WRITEOFF.ITEM,   "
			"WRITEOFF.SIZE_NAME  "

		"Union All "

		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.Price ) + Sum(Orders.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) +  Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"           //sales excl
			"Cast(Sum(Orders.Cost * Orders.Qty) as Numeric(17,4)) Cost, "
           +  _selectSalesIncl +
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
            "left join ARCCATEGORIES on "    
         	"Orders.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
            + _taxJoins +
		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY')and  "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"Orders.Item_Name,"
			"Orders.Size_Name, "
            +  _groupByClause +
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4";

	for (int i=0; i<Categories->Count; i++)
	{
		qrConsumption->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCategoryConsumptionExcSurcharge(TDateTime StartTime, TDateTime EndTime, TStrings *Categories)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
	"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
		  	"Sum(Archive.Qty) Item_Count,"
  			"Cast(Sum(Archive.Qty * Archive.PRICE_LEVEL0 ) +  Sum(coalesce(Discounts_.DISCOUNT_WITHOUT_TAX,0))  as Numeric(17,4)) Price,"
			"Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE) ) ) +  Sum(coalesce(Discounts_.DISCOUNT_WITHOUT_TAX,0)) + COALESCE(Discounts_.TAX_ON_DISCOUNT,0) as Numeric(17,4)) PriceExc,"     //sales excl
			"Cast(Sum(abs(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Discounts_.DISCOUNT_WITHOUT_TAX,0) + COALESCE(Discounts_.TAX_ON_DISCOUNT,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
         "inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
         "Left Join ArcCategories on "
         	"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
           "Left Join "
			"(              "
            "    select     "
            "    Cast(sum( 	"	
            "            case when ArcOrderDiscounts.Discounted_Value > 0            "
            "            then 0                                                   "
            "            else ArcOrderDiscounts.Discounted_Value                     "
            "            end) as Numeric(15,4)) DISCOUNT_WITHOUT_TAX,              "
                "ARCHIVE.TAX_ON_DISCOUNT*-1 TAX_ON_DISCOUNT, "
            "        ArcOrderDiscounts.Archive_key                             "
            "    from ArcOrderDiscounts                                        "
            "    Inner Join Archive on                                         "
            "        ArcOrderDiscounts.Archive_Key = Archive.Archive_key    "
            "    group by Archive_Key, ARCHIVE.TAX_ON_DISCOUNT                                             "
			") As Discounts_ on Archive.Archive_Key = Discounts_.Archive_Key   "
          "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
            "FROM ARCORDERDISCOUNTS a "
            "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
            "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

		
        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
        "(( "
          "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') )  AND "	   //Exclude Surcharge

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
			"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"Archive.Item_Name,"
			"Archive.Size_Name, "
            "Discounts_.DISCOUNT_WITHOUT_TAX ,Discounts_.TAX_ON_DISCOUNT "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

			"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
		  	"Sum(DayArchive.Qty) Item_Count,"
   			"Cast(Sum(DayArchive.Qty * DayArchive.PRICE_LEVEL0 ) + Sum(coalesce(Discounts_.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) Price,"
			"Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE)  ) ) + Sum(coalesce(Discounts_.DISCOUNT_WITHOUT_TAX,0))+ COALESCE(Discounts_.TAX_ON_DISCOUNT,0)  as Numeric(17,4)) PriceExc,"   //salex excl
			"Cast(Sum(abs(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(Discounts_.DISCOUNT_WITHOUT_TAX,0) + COALESCE(Discounts_.TAX_ON_DISCOUNT,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
         "Left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
         "Left Join ArcCategories on "
         	"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
             "Left Join  "
			"(              "
            "    select     "
            "    Cast(sum( 	"	
            "            case when DayArcOrderDiscounts.Discounted_Value > 0            "
            "            then 0                                                   "
            "            else DayArcOrderDiscounts.Discounted_Value                     "
            "            end) as Numeric(15,4)) DISCOUNT_WITHOUT_TAX,              "
            "       DAYARCHIVE.TAX_ON_DISCOUNT*-1 TAX_ON_DISCOUNT, "
            "        DayArcOrderDiscounts.Archive_key                             "
            "    from DayArcOrderDiscounts                                        "
            "    Inner Join DayArchive on                                         "
            "        DayArcOrderDiscounts.Archive_Key = DayArchive.Archive_key    "
            "    group by Archive_Key, DAYARCHIVE.TAX_ON_DISCOUNT                                             "
			") As Discounts_ on DayArchive.Archive_Key = Discounts_.Archive_Key   "    
        "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
         "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                        "FROM DAYARCORDERDISCOUNTS a "
                        "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		    "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
        "(( "
          "  COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') )   and "	//Exclude surcharge

			"DayArchive.TIME_STAMP >= :StartTime and "
			"DayArchive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name, "
            "Discounts_.DISCOUNT_WITHOUT_TAX ,Discounts_.TAX_ON_DISCOUNT "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

             "Union All "

         "Select "
			"CategoryGroups.Name Group_Name,  "
			"ArcCategories.Category Course_Name, "
			"Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name,   "
			"WRITEOFF.SIZE_NAME,   "
			"cast(0 as int) Item_Count ,   "
			"Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price,  "
            "cast(0 as int) PriceExc, "
  			"Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost,   "
			" Cast(0.00 as numeric(17,4)) SalesIncl "
		"From    "
			"WRITEOFF Left Join  ArcCategories on   "
         	"WRITEOFF.Category_Key = ArcCategories.Category_Key   "
			"Left Join CategoryGroups on      "
			 "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
             "where "
             	"WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +

			 "Group By       "
			"CategoryGroups.Name,   "
			"ArcCategories.Category,  "
			"WRITEOFF.ITEM,   "
			"WRITEOFF.SIZE_NAME  "


		"Union All "


		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.PRICE_LEVEL0 ) + Sum(coalesce(Discounts_.discountedvalue,0)) as Numeric(17,4)) Price, "
		"	Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) +  Sum(coalesce(Discounts_.discountedvalue,0)) as Numeric(17,4)) PriceExc, "          //sales excl
		"	Cast(Sum(Orders.Cost * Orders.Qty) as Numeric(17,4)) Cost, "
        "      Cast(                                                   "
        "(                                                             "
        "  Sum(                                                        "
        "       cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) + " 
        "               cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                             "
        "                 + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
        "                                                  ) )as numeric(17,4)) )as  numeric(17,4))  "
        "              )    +                                                                        "
    " cast((  Sum(                                                                                   "
    "              case when taxsetting.isPriceIncludetax =1 then                                    "
    "                           cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) + " 
    "                                   cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4)) " 
    "                                     + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "  
    "                                                      ) )as numeric(17,4)) )as  numeric(17,4)) " 
    "                else                                                                           "
    "                          case when taxsetting.isRecalculateTaxAfterDiscount =1  then          "
    "                               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
    "                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                              "
    "                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) " 
    "                                                                          ) )as numeric(17,4)) )as  numeric(17,4)) " 
    "                           else                                                                                    "
    "                                  cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) " 
    "                          end                                                                                                                  "
    "             end                                                                                                                               "
    "        )  * cast ((COALESCE(TaxDetails.VAT,0))as numeric (17,4))/100 )as numeric(17,4))  +                                                    "
    "  sum( cast((  cast(( coalesce(ORDERS.PRICE,0)-  coalesce(ORDERS.MAXRETAILPRICE,0))as numeric(17,4)) *  cast((coalesce(TaxDetails.ProfitTax,0))as numeric(17,4))/100 )as numeric(17,4)) ) + " 
    "    Sum(                                                                                                                                                                                    "
    "         case when taxsetting.isPriceIncludeServiceCharge =1 then                                                                                                                           "
    "                       cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) + " 
    "                               cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                                      "
    "                                 + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))    "
    "                                                      ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))                                                                                                                                                                                          "
    "          else                                                                                                                                                                                                                                                                                                                                                                  "
    "              case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then                                                                                                                                                                                                                                                                                           "
    "                              cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +                                        "
    "                                   cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                                                                                "
    "                                     + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))"  
    "                                                                      ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))                               "
    "                       else                                                                                                                                                                                                          "
    "                       cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "  
    "                    end " 
    "            end         "
    "         )  +           "
    "cast((   Sum(           "
    "        case when taxsetting.isPriceIncludeServiceCharge =1 then " 
    "                      cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
    "                                  cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                                   "
    "                                    + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) " 
    "                                                     ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))                                                                                                                                                                                           "
    "         else                                                                                                                                                                                                                                                                                                                                                                   "
    "             case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then                                                                                                                                                                                                                                                                                            "
    "                             cast((  (cast((  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(Discounts_.discountedvalue,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +                                 "
    "                                  cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))                                                                                                                                                                                                 "
    "                                    + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "  
    "                                                     ) )as numeric(17,4)) )as  numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) " 
    "                                                                                                                                                                                                          "
    "                   else                                                                                                                                                                                   "
    "                      cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
    "                   end   "
    "           end           "
    "        ) *  cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 )as numeric(17,4)) " 
    " ) as numeric(17,4))  SalesIncl                                                               "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
         "Left Join ArcCategories on "
         	"Orders.Category_Key = ArcCategories.Category_Key "
		"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
        "Left Join " 
			"( "
            "    select                                                   "
            "    Cast(sum(                                                "
            "            case when OrderDiscounts.Discounted_Value > 0    "
            "            then 0                                           "
            "            else OrderDiscounts.DISCOUNTED_VALUE             "
            "            end) as Numeric(15,4)) discountedvalue,          "
            "        Orders.Order_key                                     "
            "    from OrderDiscounts                                      "
            "    Inner Join Orders on                                     "
            "        OrderDiscounts.Order_Key = Orders.Order_key          "
            "    group by Order_Key                                       "
			") As Discounts_ on Orders.Order_Key = Discounts_.Order_Key	  "
                    
         + _taxJoins + ///For selecting tax
                
		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY')and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"Orders.Item_Name,"
			"Orders.Size_Name, "
             +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4";

	for (int i=0; i<Categories->Count; i++)
	{
		qrConsumption->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupSalesTypeConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Categories, TStrings *SalesTypes)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Sum(Archive.Qty) Item_Count,"
			"Cast(Sum(Archive.Qty * Archive.Price  ) +  Sum(Archive.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Archive.Qty * Archive.BASE_PRICE  ) ) +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc," //sales excl
			"Cast(Sum(Archive.Cost * Archive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Inner Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Inner Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Inner Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
        " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                "FROM ARCORDERDISCOUNTS a "
                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

		"Where "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and "
               " ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	if (SalesTypes->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(SalesTypes->Count, "ArcBill.Sales_Type", "SalesTypeParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"Archive.Item_Name,"
			"Archive.Size_Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"Sum(DayArchive.Qty) Item_Count,"
			"Cast(Sum((DayArchive.Qty * DayArchive.PRICE ) )  + Sum(DayArchive.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE   ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"
			"Cast(Sum(DayArchive.Qty * DayArchive.Cost) as Numeric(17,4)) Cost, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Inner Join DayArcBill on "
				"Security.Security_Ref = DayArcBill.Security_Ref "
			"Inner Join DayArchive on "
				"DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key "
			"Inner Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Inner Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
		 "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                    "FROM DAYARCORDERDISCOUNTS a "
                    "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		           " ((  "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  ) and "
		 "DAYARCHIVE.PRICE<>0 and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	if (SalesTypes->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(SalesTypes->Count, "DayArcBill.Sales_Type", "SalesTypeParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"CategoryGroups.Name,"
			"ArcCategories.Category,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,2,3,4";

	for (int i=0; i<Categories->Count; i++)
	{
		qrConsumption->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}
	for (int i=0; i<SalesTypes->Count; i++)
	{
		qrConsumption->ParamByName("SalesTypeParam" + IntToStr(i))->AsString = SalesTypes->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;

    //ShowSql(qrConsumption->SQL->Text);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupMenuConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"Archive.Menu_Name Group_Name,"
			"Archive.Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name, "
           " SUM (Archive.QTY) Item_Count, "
			"Cast(Sum(Archive.Qty * Archive.Price  ) +  Sum(Archive.Discount) as Numeric(17,4)) Price,"
            "Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE) ) ) +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc, "       //sales excl   //chng
			"Cast(Sum(Archive.Qty * abs(Archive.Cost)) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
            " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                                "FROM ARCORDERDISCOUNTS a "
                                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                            "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	
        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
               " ((  "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  ) and "   //chng

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Archive.Menu_Name ", "MenuParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Archive.Menu_Name,"
			"Archive.Course_Name,"
			"Archive.Item_Name,"
			"Archive.Size_Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"DayArchive.Menu_Name Group_Name,"
			"DayArchive.Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name, "
            "SUM (DayArchive.QTY) Item_Count, "
			"Cast(Sum(DayArchive.Qty * DayArchive.Price ) + Sum(DayArchive.Discount) as Numeric(17,4)) Price,"
            "Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE)  ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"     //sales excl chng
			"Cast(Sum(DayArchive.Qty * abs(DayArchive.Cost)) as Numeric(17,4)) Cost, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "

		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
             "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
            "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                        "FROM DAYARCORDERDISCOUNTS a "
                        "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
            "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
            " (( "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  ) and "

			"DayArchive.TIME_STAMP >= :StartTime and "
			"DayArchive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "DayArchive.Menu_Name ", "MenuParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"DayArchive.Menu_Name,"
			"DayArchive.Course_Name,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

        "Union All "

          "Select  "
           " writeoff.MENU_NAME Group_Name, "
           " WRITEOFF.COURSE_NAME Course_Name, "
           " Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name,  "
            "WRITEOFF.SIZE_NAME,  "
           " cast(0 as int) Item_Count  ,   "
           " Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price, "
           " cast(0 as int) PriceExc, "
           " Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost,  "
           " Cast(0.00 as numeric(17,4)) SalesIncl "
    "From  "
			"WRITEOFF  "
            "where "
            "writeoff.Time_Stamp >= :StartTime and "
			"writeoff.Time_Stamp < :EndTime " ;
            if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "writeoff.Menu_Name", "MenuParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
	   " Group By    "
       " WRITEOFF.MENU_NAME,  "
       " WRITEOFF.COURSE_NAME, "
        "WRITEOFF.ITEM,   "
       " WRITEOFF.SIZE_NAME "

		"Union All "

		"Select "
			"Orders.Menu_Name Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.Price ) + Sum(Orders.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) + Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"   //Sales excl Report
			"Cast(Sum(Orders.Qty * Orders.Cost) as Numeric(17,4)) Cost, "
           +  _selectSalesIncl + //For Selecting salesIncl column
		"From "
			"Security Left Join Orders on "
			"Security.Security_Ref = Orders.Security_Ref "
             + _taxJoins + ///For selecting tax
                     
		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Orders.Menu_Name", "MenuParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Orders.Menu_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name, "
             +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4";

	for (int i=0; i<Menus->Count; i++)
	{
		qrConsumption->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLocationConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Menus, bool IncGST)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"Archive.Order_Location Group_Name,"
			"Archive.Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"SUM (Archive.QTY) Item_Count, "
			"Cast(Sum(Archive.Qty * Archive.Price  ) +  Sum(Archive.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE)) ) +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		//sales excl
	        "Cast(Sum(Archive.Qty * abs(Archive.Cost)) as Numeric(17,4)) Cost, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
            " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                            "FROM ARCORDERDISCOUNTS a "
                            "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		    "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " ((  "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Archive.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Course_Name,"
			"Archive.Item_Name,"
			"Archive.Size_Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"DayArchive.Order_Location Group_Name,"
			"DayArchive.Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"SUM (DayArchive.QTY) Item_Count, "
			"Cast(Sum(DayArchive.Qty * DayArchive.Price ) + Sum(DayArchive.Discount)as Numeric(17,4)) Price,"
			"Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE) ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"	   //sales excl
			"Cast(Sum(abs(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
			 "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                            "FROM DAYARCORDERDISCOUNTS a "
                            "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                         "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
                " ((   "
                " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
                " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
                "(DayArchive.Order_Type = 3 or "
                "DayArchive.Order_Type = 0) and "
                "DayArchive.TIME_STAMP >= :StartTime and "
                "DayArchive.TIME_STAMP < :EndTime and "
                "Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "DayArchive.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Course_Name,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

         "Union All "

        "Select  "
           " WRITEOFF.ORDER_LOCATION Group_Name,  "
           " WRITEOFF.COURSE_NAME Course_Name,  "
            "Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name, "
            "WRITEOFF.SIZE_NAME,  "
            "cast(0 as int) Item_Count  , "
            "Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price, "
            "cast(0 as int) PriceExc,  "
            "Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost,   "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
    "From  "
			"WRITEOFF   "
         "where "
         "WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime  " ;

	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "WRITEOFF.ORDER_LOCATION", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
  	    "Group By  "
        "WRITEOFF.ORDER_LOCATION ,  "
        "WRITEOFF.COURSE_NAME,   "
        "WRITEOFF.ITEM,  "
        "WRITEOFF.SIZE_NAME  "


		"Union All "

		"Select "
			"Orders.Order_Location Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.Price ) + Sum(Orders.Discount) as Numeric(17,4)) Price,"
			"Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) + Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		   //sales excl
			"Cast(Sum(Orders.Cost * Orders.Qty) as Numeric(17,4)) Cost, "
             +  _selectSalesIncl + //For Selecting salesIncl column
		"From "
			"Security Left Join Orders on "
			"Security.Security_Ref = Orders.Security_Ref "
             + _taxJoins + ///For selecting tax
		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Orders.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "Orders.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Orders.Order_Location,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name, "
            +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4";
	for (int i=0; i<Menus->Count; i++)
	{
		qrConsumption->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrConsumption->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupTabConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Tabs, bool IncGST)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"Tab.Tab_Name Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.BASE_PRICE ) + Sum(Orders.Discount) as Numeric(17,4)) Price,"
      	    "Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) + Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"				 //sales excl
			"Cast(Sum(Orders.Qty * Orders.Cost) as Numeric(17,4)) Cost, "
            +  _selectSalesIncl + //For Selecting salesIncl column

		"From "
			"Orders Left Join Security on "
				"Security.Security_Ref = Orders.Security_Ref "
		 "Left Join Tab on "
				"Orders.Tab_Key = Tab.Tab_Key "
         + _taxJoins + ///For selecting tax

		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Tabs->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Tabs->Count, "Tab.Tab_Name", "TabParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Tab.Tab_Name, "
			"Orders.Course_Name, "
			"Orders.Item_Name, "
			"Orders.Size_Name, "
            +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "
		"Order By "
			"Tab.Tab_Name ASC,"
			"Orders.Course_Name ASC,"
			"Orders.Item_Name ASC,"
			"Orders.Size_Name ASC";
	for (int i=0; i<Tabs->Count; i++)
	{
		qrConsumption->ParamByName("TabParam" + IntToStr(i))->AsString = Tabs->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::Setup3rdPartyConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Menus)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"Archive.Order_Location Group_Name,"
			"Archive.Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"SUM (Archive.QTY) Item_Count, "
			"Cast(Sum(Archive.Qty * Archive.Price  ) +  Sum(Archive.Discount) as Numeric(17,4)) Price,"
				"Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE)  ) ) +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		   //sales excl
			"Cast(Sum(Archive.Qty * abs(Archive.Cost)) as Numeric(17,4)) Cost,"
			"ThirdPartyCodes.Code, "
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join ThirdPartyCodes On "
				"Archive.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
             "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
         " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                        "FROM ARCORDERDISCOUNTS a "
                        "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                   "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or  ARCHIVE.PRICE!=0) and "

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Archive.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Course_Name,"
			"Archive.Item_Name,"
			"Archive.Size_Name,"
			"ThirdPartyCodes.Code, ThirdPartyCodes.ThirdPartyCodes_Key "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
			"And ThirdPartyCodes.ThirdPartyCodes_Key is not null "

		"Union All "

		"Select "
			"DayArchive.Order_Location Group_Name,"
			"DayArchive.Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"SUM (DayArchive.QTY) Item_Count, "
			"Cast(Sum(DayArchive.Qty * DayArchive.Price ) + Sum(DayArchive.Discount) as Numeric(17,4)) Price,"
				"Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE)  ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"			//sales excl
			"Cast(Sum(DayArchive.Qty * abs(DayArchive.Cost)) as Numeric(17,4)) Cost,"
			"ThirdPartyCodes.Code, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "

		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join ThirdPartyCodes On "
				"DayArchive.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "

		   "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                            "FROM DAYARCORDERDISCOUNTS a "
                            "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                            "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		
        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or DayArchive.PRICE!=0) and "

			"DayArchive.TIME_STAMP >= :StartTime and "
			"DayArchive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "DayArchive.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Course_Name,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name,"
			"ThirdPartyCodes.Code, "
			"ThirdPartyCodes.ThirdPartyCodes_Key "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "
			"And ThirdPartyCodes.ThirdPartyCodes_Key is not null "

		"Union All "

        "Select  "
           " WRITEOFF.ORDER_LOCATION Group_Name,  "
           " WRITEOFF.COURSE_NAME Course_Name,  "
            "Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name, "
            "WRITEOFF.SIZE_NAME,  "
            "cast(0 as int)  Item_Count  , "
            "Cast(sum(WRITEOFF.AMOUNT) as Numeric(17,4)) Price, "
            "cast(0 as int) PriceExc,  "
            "Cast(Sum(WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost ,  "
           " WRITEOFF.THIRD_PARTY_CODE CODE,  "
           " Cast(0.00 as numeric(17,4)) SalesIncl "
    "From  "
			"WRITEOFF   "
         "where "
         "WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime  " ;
         if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "WRITEOFF.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "WRITEOFF.ORDER_LOCATION", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
  	    "Group By  "
        "WRITEOFF.ORDER_LOCATION ,  "
        "WRITEOFF.COURSE_NAME,   "
        "WRITEOFF.ITEM,  "
        "WRITEOFF.SIZE_NAME , "
       " WRITEOFF.THIRD_PARTY_CODE  "

       "Union All "

		"Select "
			"Orders.Order_Location Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Qty * Orders.Price ) + Sum(Orders.Discount) as Numeric(17,4)) Price,"
            "Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) )  + Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		 //sales excl
			"Cast(Sum(Orders.Qty * Orders.Cost) as Numeric(17,4)) Cost,"
			"ThirdPartyCodes.Code, "
			 +  _selectSalesIncl + //For Selecting salesIncl column
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Inner Join ThirdPartyCodes On "
				"Orders.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
             + _taxJoins + ///For selecting tax

		"Where  security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Menus->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Menus->Count, "Orders.Menu_Name", "MenuParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Locations->Count, "Orders.Order_Location", "LocationParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
		"Group By "
			"Orders.Order_Location,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"ThirdPartyCodes.Code, ThirdPartyCodes.ThirdPartyCodes_Key, "
            +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "
			"And ThirdPartyCodes.ThirdPartyCodes_Key is not null "

		"Order By "
			"1,2,3,4";
	for (int i=0; i<Menus->Count; i++)
	{
		qrConsumption->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrConsumption->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupUserSales(TDateTime StartTime, TDateTime EndTime, TStrings *Names, TStrings *Groups)
{
	qrUserSales->Close();
	qrUserSales->SQL->Text =
		"Select "
			"Contacts.Name, "
			"Archive.Menu_Name Group_Name, "
			"Archive.Course_Name, "
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
			"Archive.Size_Name, "
			"cast(Sum(Archive.Qty) as numeric(17, 4)) Item_Count, "
		    "Cast(Sum((Archive.Qty * Archive.BASE_PRICE )) +  Sum(Archive.DISCOUNT_WITHOUT_TAX ) as Numeric(17,4)) Price,"		  //sales excl
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl, "
			"cast(Sum(Archive.Qty * Archive.Cost) as numeric(17, 4)) Cost, "
            "cast(0.00 as numeric(17,4)) PriceExcl, "
            "cast(0.00 as numeric(17,4)) Profit "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
         " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		                    "FROM ARCORDERDISCOUNTS a "
		                "GROUP by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "      ;

	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text = qrUserSales->SQL->Text + "Inner Join ArcCategories on Archive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
	qrUserSales->SQL->Text = qrUserSales->SQL->Text +
		"Where "
		    " ((  "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
		 " ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
		"Group By "
			"Contacts.Name, "
			"Archive.Menu_Name, "
			"Archive.Course_Name, "
			"Archive.Item_Name, "
			"Archive.Size_Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Name, "
			"DayArchive.Menu_Name Group_Name, "
			"DayArchive.Course_Name, "
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, "
			"DayArchive.Size_Name, "
			"cast(Sum(DayArchive.Qty) as numeric(17, 4)) Item_Count, "
			"Cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE ) ) +  Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) Price,"				 //sales excl
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl, "
			"cast(Sum(DayArchive.Qty * DayArchive.Cost) as numeric(17, 4)) Cost, "
             "cast(0.00 as numeric(17,4)) PriceExcl, "
            "cast(0.00 as numeric(17,4)) Profit "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, " 
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
		 "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		                "FROM DAYARCORDERDISCOUNTS a "
		                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY ";
	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text = qrUserSales->SQL->Text + "Inner Join ArcCategories on DayArchive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
	qrUserSales->SQL->Text = qrUserSales->SQL->Text +
		"Where "
		    " ((  "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
		 "DAYARCHIVE.PRICE<>0 and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
		"Group By "
			"Contacts.Name, "
			"DayArchive.Menu_Name, "
			"DayArchive.Course_Name, "
			"DayArchive.Item_Name, "
			"DayArchive.Size_Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Name, "
			"Orders.Menu_Name Group_Name, "
			"Orders.Course_Name, "
			"Orders.Item_Name, "
			"Orders.Size_Name, "
			"cast(Sum(Orders.Qty) as numeric(17, 4)) Item_Count, "
			"Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) ) +  Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) Price,"				//sales excl
            +  _selectSalesIncl +   //For Selecting salesIncl column
			", cast(Sum(Orders.Qty * Orders.Cost)  as numeric(17, 4)) Cost, "
            "cast(0.00 as numeric(17,4)) PriceExcl, "
            "cast(0.00 as numeric(17,4)) Profit "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
              + _taxJoins + ///For selecting tax
			"Left Join Contacts on "
				"Security.User_Key = Contacts.Contacts_Key ";
	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text = qrUserSales->SQL->Text + "Inner Join ArcCategories on Orders.Category_Key = "
												"ArcCategories.Category_Key ";
	}
	qrUserSales->SQL->Text = qrUserSales->SQL->Text +
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
		"Group By "
			"Contacts.Name, "
			"Orders.Menu_Name, "
			"Orders.Course_Name, "
			"Orders.Item_Name, "
			"Orders.Size_Name, "
             +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4,5 ";
	qrUserSales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrUserSales->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrUserSales->ParamByName("NameParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrUserSales->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
	//TestData(qrUserSales);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupUserSalesByCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrUserSales->Close();
	qrUserSales->SQL->Text =
        "Select "
         "NAME,"
         "GROUP_NAME,"
         "COURSE_NAME,"
         "cast(sum(ITEM_COUNT) as Numeric(15,2)) ITEM_COUNT,"
         "cast(sum(Price) as Numeric(15,2)) PRICE,"
         "cast(sum(COST) as Numeric(15,2)) COST, "
         "cast(sum(SalesIncl) as Numeric(15,2)) SalesIncl "
        "From "
        "("

   	"Select "
         "Contacts.Name,"
         "CategoryGroups.Name Group_Name,"
         "ArcCategories.Category Course_Name,"
         "Sum(Archive.Qty) Item_Count,"
         "Cast(Sum((Archive.Qty * Archive.BASE_PRICE  ) )   +  Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) Price,"			//sales excl
		"Sum(Archive.Cost * Archive.Qty) Cost, "
        "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "

      "From "
         "Security Left Join Archive On "
            "Security.Security_Ref = Archive.Security_Ref "
         "Left Join ArcCategories On "
            "Archive.Category_Key = ArcCategories.Category_Key "
         "Left Join CategoryGroups On "
            "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
         "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
      " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
      "Where "
		    " (( "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')) and "
         " ARCHIVE.PRICE<>0 and "
         "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
         "ARCHIVE.TIME_STAMP_BILLED < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name,"
         "CategoryGroups.Name,"
         "ArcCategories.Category "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

   	"Select "
         "Contacts.Name,"
         "CategoryGroups.Name Group_Name,"
         "ArcCategories.Category Course_Name,"
         "Sum(DayArchive.Qty) Item_Count,"
         "Cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE  ) )  +  Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"
         "Sum(DayArchive.Cost * DayArchive.Qty) Cost, "
         "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
      "From "
         "Security Left Join DayArchive On "
            "Security.Security_Ref = DayArchive.Security_Ref "
         "Left Join ArcCategories On "
            "DayArchive.Category_Key = ArcCategories.Category_Key "
         "Left Join CategoryGroups On "
            "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
         "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, " 
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
			"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "

	  "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
      "Where "
		    " ((  "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')) and "
        "DAYARCHIVE.PRICE<>0 and "
         "DayArchive.TIME_STAMP_BILLED >= :StartTime and "
         "DayArchive.TIME_STAMP_BILLED < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name,"
         "CategoryGroups.Name,"
         "ArcCategories.Category "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Union All "

   	"Select "
         "Contacts.Name,"
         "CategoryGroups.Name Group_Name,"
         "ArcCategories.Category Course_Name,"
         "Sum(Orders.Qty) Item_Count,"
        "Cast(Sum((Orders.Qty * Orders.BASE_PRICE ) )  +  Sum(Orders.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,"		 //sales excl
         "Sum(Orders.Cost * Orders.Qty) Cost, "
        +  _selectSalesIncl + //For Selecting salesIncl column
      "From "
         "Security Left Join Orders On "
            "Security.Security_Ref = Orders.Security_Ref "
         + _taxJoins + ///For selecting tax
         "Left Join ArcCategories On "
            "orders.Category_Key = ArcCategories.Category_Key "
         "Left Join CategoryGroups On "
            "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
         "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
      "Where "
         "Orders.Time_Stamp >= :StartTime and "
         "Orders.Time_Stamp < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name,"
         "CategoryGroups.Name,"
         "ArcCategories.Category, "
          +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "
        ")"
        "Group By "
        "NAME,"
        "GROUP_NAME,"
        "COURSE_NAME  "
  		"Order By "
    		"1,2,3 ";
	qrUserSales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrUserSales->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrUserSales->ParamByName("NameParam" + IntToStr(i))->AsString = Names->Strings[i];
	}

}
//---------------------------------------------------------------------------
// Dump sql to memo control on modal form
//---------------------------------------------------------------------------
void TdmMMReportData::ShowSql(AnsiString &s) {
    TSqlView *v = new TSqlView(NULL);
    v->SetSqlText(s);
    v->ShowModal();
    v->Close();
    delete v;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupUserSalesSummary(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrUserSales->Close();
	qrUserSales->SQL->Text =
   	"Select "
         "Contacts.Name,"
         "cast(Sum(Archive.Qty) as Numeric(15,2)) Item_Count,"
        "cast(Sum((Archive.Price * Archive.Qty + Archive.Discount)) as Numeric(15,2)) Price,"
        "Cast(Sum((Archive.Qty * Archive.BASE_PRICE  ) ) + Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExcl,"	   //sales excl
        "cast(Sum(Archive.Cost * Archive.Qty) as Numeric(15,2)) Cost,"
		"cast(Sum((Archive.Qty * Archive.BASE_PRICE + Archive.Discount) )  - Sum(Archive.Cost) as Numeric(15,2)) Profit, "
        "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
     "From "
         "Security Left Join Archive On "
            "Security.Security_Ref = Archive.Security_Ref "
     "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		                "FROM ARCORDERDISCOUNTS a "
		                "GROUP by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		            "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
         "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
      "Where "
		    " (( "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
        "ARCHIVE.PRICE<>0 and "
         "Archive.TIME_STAMP_BILLED >= :StartTime and "
         "Archive.TIME_STAMP_BILLED < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

   	"Select "
         "Contacts.Name,"
         "cast(Sum(DayArchive.Qty) as Numeric(15,2)) Item_Count,"
        "cast(Sum((DayArchive.Price + DayArchive.Discount) * DayArchive.Qty) as Numeric(15,2)) Price,"
         "Cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE  ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX)  as Numeric(17,4)) PriceExcl,"		  //sales excl
         "cast(Sum(DayArchive.Cost * DayArchive.Qty) as Numeric(15,2)) Cost,"
		 "cast(Sum((DayArchive.Qty * DAYARCHIVE.BASE_PRICE  + DayArchive.Discount) ) - Sum(DayArchive.Cost) as Numeric(15,2)) Profit, "
          "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "

      "From "
         "Security Left Join DayArchive On "
            "Security.Security_Ref = DayArchive.Security_Ref "
          "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, " 
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
		    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		                    "FROM DAYARCORDERDISCOUNTS a "
		                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		            "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
         "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
      "Where "
		    " (( "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
     "DAYARCHIVE.PRICE<>0 and "
         "DAYARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
         "DAYARCHIVE.TIME_STAMP_BILLED < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Union All "

   	"Select "
         "Contacts.Name,"
         "cast(Sum(Orders.Qty) as Numeric(15,2)) Item_Count,"
         "cast(Sum((Orders.Price ) * Orders.Qty) + Sum(Orders.Discount)  as Numeric(15,2)) Price,"
         "Cast(Sum((Orders.Qty * Orders.BASE_PRICE + Orders.DISCOUNT_WITHOUT_TAX) ) as Numeric(17,4)) PriceExcl,"
         "cast(Sum(Orders.Qty * Orders.Cost) as Numeric(15,2)) Cost,"
		 "cast(Sum((Orders.Qty * Orders.BASE_PRICE+ Orders.Discount) ) - Sum(Orders.Cost) as Numeric(15,2)) Profit, "
          +  _selectSalesIncl + //For Selecting salesIncl column
      "From "
         "Security Left Join Orders On "
            "Security.Security_Ref = Orders.Security_Ref "
         "Left Join Contacts On "
            "Security.User_Key = Contacts.Contacts_Key "
         + _taxJoins + ///For selecting tax
      "Where "
         "Orders.Time_Stamp >= :StartTime and "
         "Orders.Time_Stamp < :EndTime and "
         "Security.Security_Event = 'Ordered By' ";
	if (Names->Count > 0)
	{
		qrUserSales->SQL->Text	=	qrUserSales->SQL->Text + "and (" +
											ParamString(Names->Count, "Contacts.Name", "NameParam") + ")";
	}
	qrUserSales->SQL->Text		=	qrUserSales->SQL->Text +
      "Group By "
         "Contacts.Name, "
           +  _groupByClause + ///group by taxes
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3 ";
	qrUserSales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrUserSales->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrUserSales->ParamByName("NameParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCategoryConsumptionByHalfHour(TDateTime StartTime, TDateTime EndTime, TStrings *Categories)
{
	qrConsumption->Close();
	qrConsumption->SQL->Text =
		"Select "
			"CategoryGroups.Name Group_Name, "
			"ArcCategories.Category Course_Name, "
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
			"Archive.Size_Name, "
			"Cast(Extract (Minute From Archive.TIME_STAMP) / 30 as Integer) Order_Half_Hour, "
			"Extract (Minute From Archive.TIME_STAMP) Order_Minute, "
			"Extract (Hour From Archive.TIME_STAMP) Order_Hour, "
			"Extract (Day From Archive.TIME_STAMP) Order_Day, "
			"Extract (Month From Archive.TIME_STAMP) Order_Month, "
			"Extract (Year From Archive.TIME_STAMP) Order_Year, "
			"Cast(Archive.TIME_STAMP As Date) Order_Date, "
			"Cast(Cast('12/30/1899' AS TIMESTAMP) +  "
			"Cast(((Extract (Minute From Archive.TIME_STAMP) / 30) * 30 * 60) + "
			"(Extract (Hour From Archive.TIME_STAMP) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time, "
			"Cast(Cast('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Archive.TIME_STAMP) / 30) * 30 * 60) + "
			"(Extract (Hour From Archive.TIME_STAMP) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time ,"
		 	"Archive.Qty Item_Count,"
			"Cast((Archive.Qty * Archive.Price )+ (Archive.Discount) as Numeric(17,4)) Price,"
            "Cast(((Archive.Qty * abs(Archive.BASE_PRICE) ) ) + (Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) PriceExc,  "	 //sales excl
			"Cast((abs(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost ,"
            "Cast(Null As VarChar(50)) Code, "
            "Cast((Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
            " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                "FROM ARCORDERDISCOUNTS a "
                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		
        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or Archive.PRICE!=0) and "

			"Archive.TIME_STAMP >= :StartTime and "
			"Archive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' and "
            "Archive.Qty != 0  "	;
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +

		"Union All "

		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"Cast(Extract (Minute From DayArchive.TIME_STAMP) / 30 as Integer) Order_Half_Hour,"
			"Extract (Minute From DayArchive.TIME_STAMP) Order_Minute,"
			"Extract (Hour From DayArchive.TIME_STAMP) Order_Hour,"
			"Extract (Day From DayArchive.TIME_STAMP) Order_Day,"
			"Extract (Month From DayArchive.TIME_STAMP) Order_Month,"
			"Extract (Year From DayArchive.TIME_STAMP) Order_Year,"
			"Cast(DayArchive.TIME_STAMP As Date) Order_Date,"
			"Cast(Cast('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DayArchive.TIME_STAMP) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArchive.TIME_STAMP) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"Cast(Cast('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DayArchive.TIME_STAMP) / 30) * 30 * 60) + "
			"(Extract (Hour From DayArchive.TIME_STAMP) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
		  	"DayArchive.Qty Item_Count,"
			"Cast((DayArchive.Qty * DayArchive.Price ) + (DayArchive.Discount)   as Numeric(17,4)) Price,"
            "Cast(((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE)   ) ) + (DayArchive.DISCOUNT_WITHOUT_TAX)  as Numeric(17,4)) PriceExc,  "		//sales excl
			"Cast((abs(DayArchive.Qty) * DayArchive.Cost) as Numeric(17,4)) Cost , "
            "Cast(Null As VarChar(50)) Code, "
            "Cast((DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
             "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
		    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                "FROM DAYARCORDERDISCOUNTS a "
                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
			"Left Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "

        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or DAYArchive.PRICE!=0) and "

			"DayArchive.TIME_STAMP >= :StartTime and "
			"DayArchive.TIME_STAMP < :EndTime and "
			"Security.Security_Event = 'Ordered By' and "
            "DayArchive.Qty != 0  "	;
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +

        "Union All "

        "Select "
            "CategoryGroups.Name Group_Name,  "
            "ArcCategories.Category Course_Name, "
            "Cast(WRITEOFF.ITEM as VarChar(50)) Item_Name, "
            "WRITEOFF.SIZE_NAME, "
            "Cast(Extract (Minute From writeoff.Time_Stamp) / 30 as Integer) Order_Half_Hour, "
            "Extract (Minute From writeoff.Time_Stamp) Order_Minute, "
            "Extract (Hour From writeoff.Time_Stamp) Order_Hour, "
            "Extract (Day From writeoff.Time_Stamp) Order_Day, "
            "Extract (Month From writeoff.Time_Stamp) Order_Month, "
            "Extract (Year From writeoff.Time_Stamp) Order_Year, "
            "Cast(writeoff.Time_Stamp As Date) Order_Date, "
            "Cast(Cast('12/30/1899' AS TIMESTAMP) + "
            "Cast(((Extract (Minute From writeoff.Time_Stamp) / 30) * 30 * 60) + "
            "(Extract (Hour From writeoff.Time_Stamp) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time, "
            "Cast(Cast('12/30/1899' AS TIMESTAMP) + "
            "Cast(((Extract (Minute From writeoff.Time_Stamp) / 30) * 30 * 60) + "
            "(Extract (Hour From writeoff.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time, "
            "cast(0 as int) Item_Count ,   "
            "Cast((WRITEOFF.AMOUNT) as Numeric(17,4)) Price, "
            "cast(0 as int) PriceExc, "
            "Cast((WRITEOFF.QTY*WRITEOFF.cost) as Numeric(17,4)) Cost , "
            "Cast(Null As VarChar(50)) Code, "
            " Cast(0.00 as numeric(17,4)) SalesIncl "
    "From "
			"WRITEOFF Left Join  ArcCategories on  "
         	"WRITEOFF.Category_Key = ArcCategories.Category_Key   "
			"Left Join CategoryGroups on  "
			" ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
             "where "
             	"WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime  ";

            	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +

		"Union All "

		"Select "
			"CategoryGroups.Name Group_Name,"
			"ArcCategories.Category Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Cast(Extract (Minute From Orders.Time_Stamp) / 30 as Integer) Order_Half_Hour,"
			"Extract (Minute From Orders.Time_Stamp) Order_Minute,"
			"Extract (Hour From Orders.Time_Stamp) Order_Hour,"
			"Extract (Day From Orders.Time_Stamp) Order_Day,"
			"Extract (Month From Orders.Time_Stamp) Order_Month,"
			"Extract (Year From Orders.Time_Stamp) Order_Year,"
			"Cast(Orders.Time_Stamp As Date) Order_Date,"
			"Cast(Cast('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Orders.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From Orders.Time_Stamp) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"Cast(Cast('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Orders.Time_Stamp) / 30) * 30 * 60) + "
			"(Extract (Hour From Orders.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
			"Orders.Qty Item_Count,"
			"Cast((Orders.Qty * Orders.Price ) + (Orders.Discount)  as Numeric(17,4)) Price,"
            "Cast(((Orders.Qty * Orders.BASE_PRICE) ) + (Orders.DISCOUNT_WITHOUT_TAX)  as Numeric(17,4)) PriceExc,  "
	        "Cast((Orders.Qty * Orders.Cost) as Numeric(17,4)) Cost , "
          	"Cast(Null As VarChar(50)) Code, "
            +  _selectSalesIncl + //For Selecting salesIncl column

		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
			"Left Join ArcCategories on "
				"Orders.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
             + _taxJoins + ///For selecting tax
		"Where security.SECURITY_REF not in(select security.SECURITY_REF from SECURITY where SECURITY.SECURITY_EVENT='CancelY') and "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' and "
			"Orders.Qty != 0 ";
	if (Categories->Count > 0)
	{
		qrConsumption->SQL->Text	=	qrConsumption->SQL->Text + "and (" +
												ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrConsumption->SQL->Text		=	qrConsumption->SQL->Text +
    "group by    "                                                                                                                                                                                                                      
              "CategoryGroups.Name ,   "
              "ArcCategories.Category, "                                                                                                                                                                                                         
              "Orders.Size_Name,       "                                                                                                                                                                                                                
              "Orders.Item_Name ,      "                                                                                                                                                                                                                
              "Orders.Qty,             "                                                                                                                                                                                                                
              "Orders.Price ,          "
              "Orders.Discount ,       "
              "Orders.PRICE_INCL,      "                                                                                                                                                                                                   
              "Orders.Price_Adjust,    "                                                                                                                                                                                                                
              "Orders.Time_Stamp ,     "                                                                                                                                                                                                       
              "Orders.BASE_PRICE,      "
              "Orders.Cost,            "                                                                                                                                                                                                         
              "Orders.DISCOUNT_WITHOUT_TAX, "
              "Orders.order_key ,          "
              +  _groupByClause + ///group by taxes
		"Order By "
			"10,9,8,7,5,1,2,3,4,11";

	for (int i=0; i<Categories->Count; i++)
	{
		qrConsumption->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}
	qrConsumption->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrConsumption->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupHalfHourlyDailyByConsumption(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals)
{
	qrHalfHoulrySummary->Close();
	qrHalfHoulrySummary->SQL->Text =
		"Select "
            "ARCHIVE.ARCBILL_KEY OrderKey,"
		  	"cast ( 'D' as Char(1)) Report_Type,"
			"Archive.Order_Location Billed_Location,"
			"Archive.TIME_STAMP_BILLED,"
			"Extract (Minute From Archive.TIME_STAMP_BILLED) / 30 Bill_Half_Hour,"
			"Extract (Minute From Archive.TIME_STAMP_BILLED) Bill_Minute,"
			"Extract (Hour From Archive.TIME_STAMP_BILLED) Bill_Hour,"
			"Extract (Day From Archive.TIME_STAMP_BILLED) Bill_Day,"
			"Extract (Month From Archive.TIME_STAMP_BILLED) Bill_Month,"
			"Extract (Year From Archive.TIME_STAMP_BILLED) Bill_Year,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Archive.TIME_STAMP_BILLED) / 30) * 30 * 60) + "
			"(Extract (Hour From Archive.TIME_STAMP_BILLED) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From Archive.TIME_STAMP_BILLED) / 30) * 30 * 60) + "
			"(Extract (Hour From Archive.TIME_STAMP_BILLED) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
            "Cast(Sum((Archive.Qty * abs(Archive.BASE_PRICE)  ) ) + Sum(Archive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) Bill_Total,"		//sales excl
			"max(Patron_Count) Patron_Count,"
			"cast(SUM (Archive.QTY) as numeric(17,4))  SalesQty, "		   //sales Item count
            "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "

		"From "                                                   
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "
            "LEFT JOIN ( "
                 "SELECT "
                        "ARCORDERTAXES.ARCHIVE_KEY, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,               "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a                               "
                        "group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                        "order by 1 )  ARCORDERTAXES                        "
                        "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                        "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
        " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                "FROM ARCORDERDISCOUNTS a "
                "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                "ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
         " left join PATRONCOUNT on PATRONCOUNT.ARCBILL_KEY=ARCHIVE.ARCBILL_KEY "

        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or Archive.PRICE!=0) and "

			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By'  and "
            "ARCHIVE.ARCBILL_KEY > 0 ";

	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "Archive.Terminal_Name", "TerminalParam") + ")";
	}

    qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +
    " Group by "
                                            "OrderKey,Report_Type,Billed_Location,Archive.TIME_STAMP_BILLED,Archive.Qty " ;  


 	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +

        "Union All "

        "Select   cast(0 as int) order_key, "
        "cast ( 'D' as Char(1)) Report_Type, "
        "WRITEOFF.Order_Location Billed_Location,  "
        "writeoff.Time_Stamp,   "
        "Extract (Minute From  writeoff.Time_Stamp) / 30 Bill_Half_Hour,   "
        "Extract (Minute From  writeoff.Time_Stamp) Bill_Minute,    "
        "Extract (Hour From  writeoff.Time_Stamp) Bill_Hour,   "
        "Extract (Day From  writeoff.Time_Stamp) Bill_Day,  "
        "Extract (Month From  writeoff.Time_Stamp) Bill_Month,  "
        "Extract (Year From  writeoff.Time_Stamp) Bill_Year,   "
        "cast(CAST('12/30/1899' AS TIMESTAMP) +  "
        "Cast(((Extract (Minute From  writeoff.Time_Stamp) / 30) * 30 * 60) +  "
        "(Extract (Hour From  writeoff.Time_Stamp) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,  "
        "cast(CAST('12/30/1899' AS TIMESTAMP) +   "
        "Cast(((Extract (Minute From  writeoff.Time_Stamp) / 30) * 30 * 60) +  "
        "(Extract (Hour From  writeoff.Time_Stamp) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,  "
       " cast(0 as int) Bill_Total, "
       " cast(0 as int) Patron_Count, "
        "Cast(0.00 as numeric(17,4)) SalesQty, "
        " Cast(0.00 as numeric(17,4)) SalesIncl "

    "From   "
       " WRITEOFF  "
       "where "
       "WRITEOFF.Time_Stamp >= :StartTime and "
			"WRITEOFF.Time_Stamp < :EndTime " ;
        if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "WRITEOFF.TerminalName", "TerminalParam") + ")";
	}

    qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +

  	    "Group By "
        " Report_Type, "
        "WRITEOFF.Time_Stamp, "
        "WRITEOFF.ORDER_LOCATION  " ;

       qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +

		"Union All "

		"Select "
            "DAYARCHIVE.ARCBILL_KEY OrderKey,"
	   		"cast ( 'D' as Char(1)) Report_Type,"
			"DayArchive.Order_Location Billed_Location,"
			"DAYARCHIVE.TIME_STAMP_BILLED,"
			"Extract (Minute From DAYARCHIVE.TIME_STAMP_BILLED) / 30 Bill_Half_Hour,"
			"Extract (Minute From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Minute,"
			"Extract (Hour From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Hour,"
			"Extract (Day From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Day,"
			"Extract (Month From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Month,"
			"Extract (Year From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Year,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DAYARCHIVE.TIME_STAMP_BILLED) / 30) * 30 * 60) + "
			"(Extract (Hour From DAYARCHIVE.TIME_STAMP_BILLED) * 60 * 60) As Double Precision) / 86400  as Time) Start_Time,"
			"cast(CAST('12/30/1899' AS TIMESTAMP) + "
			"Cast(((Extract (Minute From DAYARCHIVE.TIME_STAMP_BILLED) / 30) * 30 * 60) + "
			"(Extract (Hour From DAYARCHIVE.TIME_STAMP_BILLED) * 60 * 60) + 1800 As Double Precision) / 86400  as Time) End_Time,"
           "Cast(Sum((DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE) ) ) + Sum(DayArchive.DISCOUNT_WITHOUT_TAX) as Numeric(17,4)) Bill_Total,"	  //sales excl
			"max(Patron_Count) Patron_Count, "
			"cast(SUM (DayArchive.QTY) as numeric(17,4)) SalesQty, "
            "Cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE  + COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) SalesIncl "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
            "LEFT JOIN ( "
				 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,               "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,     "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a                               "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
						"order by 1 )  DAYARCORDERTAXES                        "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )               "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY       "
		    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                    "FROM DAYARCORDERDISCOUNTS a "
                    "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
                    "DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
            " left join DAYPATRONCOUNT on DAYPATRONCOUNT.ARCBILL_KEY=DAYARCHIVE.ARCBILL_KEY  "

        " Where DAYARCHIVE.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY') and  "
		    " (( "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') or DAYArchive.PRICE!=0) and "

			"DAYArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DAYArchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Ordered By'  and "
            "DAYARCHIVE.ARCBILL_KEY > 0 ";   

	if (Terminals->Count > 0)
	{
		qrHalfHoulrySummary->SQL->Text	=	qrHalfHoulrySummary->SQL->Text + "and (" +
												ParamString(Terminals->Count, "DayArchive.Terminal_Name", "TerminalParam") + ")";
	}

    qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +
    " group by "
                                        "OrderKey,Report_Type,Billed_Location,DAYARCHIVE.TIME_STAMP_BILLED,DayArchive.Qty ";


 	qrHalfHoulrySummary->SQL->Text = qrHalfHoulrySummary->SQL->Text +

		"Order By "
			"2,3,4 ";

	for (int i=0; i<Terminals->Count; i++)
	{
		qrHalfHoulrySummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}

	qrHalfHoulrySummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrHalfHoulrySummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	AddInZeroHalfHours(true);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupPatronAverages(TDateTime StartTime, TDateTime EndTime)
{
	qrPatronAverages->Close();
	qrPatronAverages->SQL->Text =
		"Select "
			"Security.Time_Stamp,"
			"Extract (Day From Security.Time_Stamp) Bill_Day,"
			"Extract (Month From Security.Time_Stamp) Bill_Month,"
			"Extract (Year From Security.Time_Stamp) Bill_Year,"
			"ArcBill.Sales_Type,"
           "  Cast(Sum(Archive.QTY * Archive.BASE_PRICE + COALESCE(Archive.Discount,0)) as Numeric(17,4)) Total ,	"
			"ArcBill.Patron_Count "
		"From "
			"Security Left  Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
                  	"Left Join Archive on "
			    	"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime And "
			"ArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"ArcBill.Patron_Count is not null "
             	 "group by Security.Time_Stamp, ArcBill.Sales_Type,ArcBill.Patron_Count   "
		 "Union All "
		 "Select "
			"Security.Time_Stamp,"
			"Extract (Day From Security.Time_Stamp) Bill_Day,"
			"Extract (Month From Security.Time_Stamp) Bill_Month,"
			"Extract (Year From Security.Time_Stamp) Bill_Year,"
			"DayArcBill.Sales_Type,"
            "Cast(Sum(DAYArchive.QTY * DAYArchive.BASE_PRICE + COALESCE(DAYArchive.Discount,0)) as Numeric(17,4)) Total ,	"
			"DayArcBill.Patron_Count "
		"From "
			"Security Left  Join DayArcBill On "
				"Security.Security_Ref = DayArcBill.Security_Ref "
                  "Left Join DAYArchive on "
			   	"DayArcBill.ArcBill_Key = DAYArchive.ArcBill_Key "
		"Where "
			"DayArcBill.Time_Stamp >= :StartTime And "
			"DayArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"DayArcBill.Patron_Count is not null "
               "group by Security.Time_Stamp, DayArcBill.Sales_Type,DayArcBill.Patron_Count   "
		"Order By "
			"5,1";

	qrPatronAverages->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrPatronAverages->ParamByName("EndTime")->AsDateTime		= EndTime;

    //ShowSql(qrPatronAverages->SQL->Text);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupPatronTypes(TDateTime StartTime, TDateTime EndTime,TStrings *Types)
{
	qrPatronTypes->Close();
	qrPatronTypes->SQL->Text =
		"Select "
			"cast(ArcBill.Time_Stamp as Date) Time_Stamp,"
			"PatronCount.Patron_Type,"
			"PatronCount.Patron_Count,"
			"ArcBill.Total "
		"From "
			"Security Left  Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Right Join PatronCount On "
				"ArcBill.ArcBill_key = PatronCount.ArcBill_key "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime And "
			"ArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"ArcBill.Patron_Count <> 0 ";
	if (Types->Count)
	{
		qrPatronTypes->SQL->Text =	qrPatronTypes->SQL->Text + "And (" +
											ParamString(Types->Count, "PatronCount.Patron_Type", "TypeParam") + ")";
	}
	qrPatronTypes->SQL->Text =	qrPatronTypes->SQL->Text +
		 "Union All "

		 "Select "
			"cast(DayArcBill.Time_Stamp as Date) Time_Stamp,"
			"DayPatronCount.Patron_Type,"
			"DayPatronCount.Patron_Count,"
			"DayArcBill.Total "
		"From "
			"Security Left Join DayArcBill On "
				"Security.Security_Ref = DayArcBill.Security_Ref "
			"Right Join DayPatronCount On "
				"DayArcBill.ArcBill_key = DayPatronCount.ArcBill_key "
		"Where "
			"DayArcBill.Time_Stamp >= :StartTime And "
			"DayArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"DayArcBill.Patron_Count <> 0 ";
	if (Types->Count)
	{
		qrPatronTypes->SQL->Text =	qrPatronTypes->SQL->Text + "And (" +
											ParamString(Types->Count, "DayPatronCount.Patron_Type", "TypeParam") + ")";
	}
	qrPatronTypes->SQL->Text =	qrPatronTypes->SQL->Text +
		"Order By "
			"1,2";

	for (int i=0; i<Types->Count; i++)
	{
		qrPatronTypes->ParamByName("TypeParam" + IntToStr(i))->AsString = Types->Strings[i];
	}
	qrPatronTypes->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrPatronTypes->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------

void TdmMMReportData::SetupTurnAround(TDateTime StartTime, TDateTime EndTime)
{
	qrTurnAround->Close();
	qrTurnAround->SQL->Text =
		"Select "
			"Extract (Minute From Order_Sale_Start_Time) Sale_Minute,"
			"Extract (Hour From Order_Sale_Start_Time) Sale_Hour,"
			"Extract (Day From Order_Sale_Start_Time) Sale_Day,"
			"Extract (Month From Order_Sale_Start_Time) Sale_Month,"
			"Extract (Year From Order_Sale_Start_Time) Sale_Year,"

			"CAST('12/30/1899' AS TIMESTAMP) + "
			"(Cast((Extract(Hour From Order_Sale_Start_Time)) As Double Precision) / 24 + .0000000001) Start_Time,"

			"CAST('12/30/1899' AS TIMESTAMP) + "
			"(Cast((Extract(Hour From Order_Sale_Start_Time) + 1) As Double Precision) / 24 + .0000000001) End_Time,"

			"Order_Sale_Start_Time Sale_Start_Time,"
			"Order_Sale_Finish_Time Sale_End_Time,"
			"Arrival_Time Make_Start_Time,"
			"Bump_Time Make_End_Time,"

			"CAST('12/30/1899' AS TIMESTAMP) + (Order_Sale_Finish_Time - Order_Sale_Start_Time) Sale_Time,"
			"CAST('12/30/1899' AS TIMESTAMP) + (Bump_Time - Arrival_Time) Make_Time,"
			"CAST('12/30/1899' AS TIMESTAMP) + (Bump_Time - Arrival_Time) + (Order_Sale_Finish_Time - Order_Sale_Start_Time) Process_Time "
		"From "
			"Orders Inner Join OrderTimesView "
            "On Orders.Order_Key = OrderTimesView.Order_Key "
		"Where "
			"Order_Sale_Start_Time >= :StartTime and "
			"Order_Sale_Start_Time < :EndTime and "
			"Order_Sale_Start_Time Is Not Null And "
			"Order_Sale_Finish_Time Is Not Null And "
			"Arrival_Time Is Not Null And "
			"Bump_Time Is Not Null "
		"Order By "
			"Order_Sale_Start_Time";
	qrTurnAround->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrTurnAround->ParamByName("EndTime")->AsDateTime	= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupChronological(TDateTime StartTime, TDateTime EndTime, TStrings *Tables, TStrings *Tabs, TStrings *Terminals)
{
	qrChronological->Close();
	qrChronological->SQL->Text =
		"Select "
			"ARCHIVE.Time_Stamp, "
			"Extract(Day From Security.Time_Stamp) Item_Day, "
			"Extract(Second From Security.Time_Stamp) Item_Second, "
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
			"Archive.Qty, "
			"Archive.Size_Name, " 
		" cast(cast(Archive.Qty * Archive.BASE_PRICE as numeric(17, 4)) +COALESCE(ARCHIVE.DISCOUNT_WITHOUT_TAX,0) + COALESCE(AOT.VAT,0) + COALESCE(AOT.ServiceCharge,0) + COALESCE(AOT.OtherServiceCharge,0) as numeric(17, 4)) as Price, "

			"Archive.Table_Number, "
			"Cast(Archive.Tab_Name as VarChar(32)) Tab_Name, "
			"Security.From_Val User_Name, "
			"Security.Terminal_Name, "
			"Archive.Archive_Key Table_Key "
		"From "
			"Security Inner Join Archive on "
			"Security.Security_Ref = Archive.Security_Ref "
      " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

"LEFT JOIN ( "						   //Join With Tax
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "
		    " (( COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "

		 "ARCHIVE.PRICE<>0 and "
			"ARCHIVE.Time_Stamp >= :StartTime and "
			"ARCHIVE.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Tables->Count)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tables->Count, "Archive.Table_Number", "TableParam") + ")";
	}
	if (Tabs->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tabs->Count, "Archive.Tab_Name", "TabParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrChronological->SQL->Text =	qrChronological->SQL->Text +
		"Union All "

		"Select "
			"DAYARCHIVE.Time_Stamp,"
			"Extract(Day From Security.Time_Stamp) Item_Day,"
			"Extract(Second From Security.Time_Stamp) Item_Second,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Qty,"
			"DayArchive.Size_Name,"
			"cast(cast(DayArchive.Qty * DAYARCHIVE.BASE_PRICE  as numeric(17, 4)) +coalesce(DayArchive.DISCOUNT_WITHOUT_TAX,0)+ coalesce(AOT.VAT,0) + coalesce(AOT.ServiceCharge,0) + coalesce(AOT.OtherServiceCharge,0) as numeric(17, 4)) as Price,  "		 //sales Incl

			"DayArchive.Table_Number,"
			"Cast(DayArchive.Tab_Name as VarChar(32)) Tab_Name,"
			"Security.From_Val User_Name,"
			"Security.Terminal_Name,"
			"DayArchive.Archive_Key Table_Key "
		"From "
			"Security Inner Join DayArchive on "
			"Security.Security_Ref = DayArchive.Security_Ref "
		 "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
"LEFT JOIN ( "		//Join with Tax
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "
		    " (( "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "

		 "DAYARCHIVE.PRICE<>0 and "
			"DAYARCHIVE.Time_Stamp >= :StartTime and "
			"DAYARCHIVE.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Tables->Count)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tables->Count, "DayArchive.Table_Number", "TableParam") + ")";
	}
	if (Tabs->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tabs->Count, "DayArchive.Tab_Name", "TabParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrChronological->SQL->Text =	qrChronological->SQL->Text +

		"Union All "

		"Select "
			"Orders.Time_Stamp, "
			"Extract(Day From Security.Time_Stamp) Item_Day, "
			"Extract(Second From Security.Time_Stamp) Item_Second, "
			"Cast(Orders.Item_Name as VarChar(50)) Item_Name, "
			"Orders.Qty, "
			"Orders.Size_Name, "
             "Cast((( CAST(Orders.QTY *  Orders.BASE_PRICE as Numeric(17,4)) ) +(  (  cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.VAT,0)/100 as         numeric(17, 4))  ) )+( cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.ServiceCharge,0)/100 as numeric(17, 4)) ) + "
            "(cast((((Orders.Qty *        Orders.BASE_PRICE+ COALESCE(Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE(AOT.ServiceCharge,0)/100)*COALESCE(STAX.ServiceChargeTax,0)/100 as numeric(17, 4)))+  "
            "  ( COALESCE(Orders.DISCOUNT_WITHOUT_TAX,0)) "
            "  ) as Numeric(17,4)) Price   ,           "
			"Orders.Table_Number, "
			"Cast(Orders.Tab_Name as VarChar(32)) Tab_Name, "
			"Security.From_Val User_Name, "
			"Security.Terminal_Name, "
			"Orders.Order_Key Table_Key "
		"From "
			"Security Inner Join Orders on "
			"Security.Security_Ref = Orders.Security_Ref "
            "left join                                                                                                     "
"				(SELECT                                                                                        "
"                cast(1 as int) keyvalue   ,                                                                   "
"						  TAXPROFILE.ORDER_KEY,                                                                "
"					  	MIN(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT,                "
"					  	MIN(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,      "
"					  	MIN(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS OtherServiceCharge  "
"				 FROM (SELECT                                                                                  "
"                            TFO.ORDER_KEY,  TAXPROFILES.TYPE,                                                 "
"                            Cast(Sum(COALESCE(TAXPROFILES.RATE,0) ) as Numeric(17,4)) TAX_RATE                "
"                            FROM TAXPROFILES_ORDERS TFO                                                       "
"                            left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY                  "
"                            group by TFO.ORDER_KEY,TAXPROFILES.TYPE                                           "
"                            )  TAXPROFILE                                                                     "
"					  	GROUP BY TAXPROFILE.ORDER_KEY                                                          "
"				) AOT on AOT.ORDER_KEY=Orders.ORDER_KEY                                                        "
"                 left join (SELECT      cast(1 as int) keyvalue   ,                                           "
" MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax      FROM VARSPROFILE    ) STAX on STAX.keyvalue=AOT.keyvalue "

		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Tables->Count)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tables->Count, "Orders.Table_Number", "TableParam") + ")";
	}
	if (Tabs->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Tabs->Count, "Orders.Tab_Name", "TabParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrChronological->SQL->Text =	qrChronological->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrChronological->SQL->Text =	qrChronological->SQL->Text +

		"Order By "
			"1,4,5";
	for (int i=0; i<Tables->Count; i++)
	{
		qrChronological->ParamByName("TableParam" + IntToStr(i))->AsString = Tables->Strings[i];
	}
	for (int i=0; i<Tabs->Count; i++)
	{
		qrChronological->ParamByName("TabParam" + IntToStr(i))->AsString = Tabs->Strings[i];
	}
	for (int i=0; i<Terminals->Count; i++)
	{
		qrChronological->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrChronological->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrChronological->ParamByName("EndTime")->AsDateTime	= EndTime;
}

//---------------------------------------------------------------------------

void TdmMMReportData::SetupInvoice( TDateTime StartTime, TDateTime EndTime, TStrings *Members, TStrings *Groups)
{
	qrInvoice->Close();

    qrInvoice->SQL->Text =
        "select "
            "groups.name Name, contacts.name ContactName, invoices.TOTAL_INC, "
            "INVOICES.INVOICE_NUMBER, DAYARCBILL.TIME_STAMP, DAYARCBILL.STAFF_NAME, "
            "contacts.member_number "
        "from "
            "CONTACTGROUPS "
            "left join groups on CONTACTGROUPS.GROUPS_KEY = groups.GROUPS_KEY "
            "left join contacts on contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join invoices on invoices.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join DAYARCBILL on DAYARCBILL.INVOICE_KEY = invoices.INVOICE_KEY "
        "where "
            "dayarcbill.time_stamp >= :StartTime and "
            "dayarcbill.time_stamp < :EndTime ";


	if (Members->Count)
	{
		qrInvoice->SQL->Text =	qrInvoice->SQL->Text + "And (" +
											ParamString(Members->Count, "Contacts.Name", "MembersParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrInvoice->SQL->Text =	qrInvoice->SQL->Text + "And (" +
											ParamString(Groups->Count, "Groups.Name", "GroupsParam") + ")";
	}
	qrInvoice->SQL->Text =		qrInvoice->SQL->Text +

		"Union All "
        "select "
            "groups.name Name, contacts.name ContactName, invoices.TOTAL_INC, "
            "INVOICES.INVOICE_NUMBER, ARCBILL.TIME_STAMP, ARCBILL.STAFF_NAME, "
            "contacts.member_number "
        "from "
            "CONTACTGROUPS "
            "left join groups on CONTACTGROUPS.GROUPS_KEY = groups.GROUPS_KEY "
            "left join contacts on contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join invoices on invoices.CONTACTS_KEY = contacts.CONTACTS_KEY "
            "left join ARCBILL on ARCBILL.INVOICE_KEY = invoices.INVOICE_KEY "
        "where "
            "arcbill.time_stamp >= :StartTime and "
            "arcbill.time_stamp < :EndTime ";

	if (Members->Count)
	{
		qrInvoice->SQL->Text =	qrInvoice->SQL->Text + "And (" +
											ParamString(Members->Count, "Contacts.Name", "MembersParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrInvoice->SQL->Text =	qrInvoice->SQL->Text + "And (" +
											ParamString(Groups->Count, "Groups.Name", "GroupsParam") + ")";
	}
	qrInvoice->SQL->Text =		qrInvoice->SQL->Text +

		"Order By "
			"1,2,4";
	for (int i=0; i<Members->Count; i++)
	{
		qrInvoice->ParamByName("MembersParam" + IntToStr(i))->AsString = Members->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrInvoice->ParamByName("GroupsParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
	qrInvoice->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrInvoice->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupInvoiceDetailed( TDateTime StartTime, TDateTime EndTime, TStrings *Members)
{
	qrInvoiceDetailed->Close();
    qrInvoiceDetailed->SQL->Text =
        "select "
            "arcbill.Staff_Name, "
            "arcbill.time_stamp, "
            "INVOICES.TOTAL_INC, "
            "ARCHIVE.ITEM_NAME, "
            "cast(ARCHIVE.QTY * ARCHIVE.PRICE_LEVEL0  as numeric(17, 4)) PRICE, "
            "ARCHIVE.QTY, "
            "INVOICES.INVOICE_NUMBER, "
            "INVOICES.CONTACTS_KEY, "
            "contacts.member_number, "
            "CONTACTS.name, "
            "INVOICES.CLOSED "
        "from ARCBILL "
            "left join ARCHIVE on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
            "left join INVOICES on ARCBILL.INVOICE_KEY = INVOICES.INVOICE_KEY "
            "left join CONTACTS on INVOICES.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        " Left join ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    "(COALESCE( ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
             " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' ) and "
            "arcbill.time_stamp >= :StartTime and "
            "arcbill.time_stamp < :EndTime   " ;

	if (Members->Count)
	{
		qrInvoiceDetailed->SQL->Text =	qrInvoiceDetailed->SQL->Text + "And (" +
											ParamString(Members->Count, "Contacts.Name", "MembersParam") + ")";
	}
	qrInvoiceDetailed->SQL->Text =		qrInvoiceDetailed->SQL->Text +



            "union all "
        "select "
            "dayarcbill.Staff_Name, "
            "dayarcbill.time_stamp, "
            "INVOICES.TOTAL_INC, "
            "dayARCHIVE.ITEM_NAME, "
            "cast(dayARCHIVE.QTY * dayARCHIVE.PRICE_LEVEL0   as  numeric(17, 4)) PRICE, "
            "dayARCHIVE.QTY, "
            "INVOICES.INVOICE_NUMBER, "
            "INVOICES.CONTACTS_KEY, "
            "CONTACTS.name, "
            "contacts.member_number, "
            "INVOICES.CLOSED "
        "from dayARCBILL "
            "left join dayARCHIVE on dayARCBILL.ARCBILL_KEY = dayARCHIVE.ARCBILL_KEY "
            "left join INVOICES on dayARCBILL.INVOICE_KEY = INVOICES.INVOICE_KEY "
            "left join CONTACTS on INVOICES.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			" Left join DAYARCORDERDISCOUNTS on DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    "(COALESCE( DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
             " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' ) and "
            "dayarcbill.time_stamp >= :StartTime and "
            "dayarcbill.time_stamp < :EndTime   ";

	if (Members->Count)
	{
		qrInvoiceDetailed->SQL->Text =	qrInvoiceDetailed->SQL->Text + "And (" +
											ParamString(Members->Count, "Contacts.Name", "MembersParam") + ")";
	}
	qrInvoiceDetailed->SQL->Text =		qrInvoiceDetailed->SQL->Text +


            "union all "
        "select "
            "Orders.Server_Name, "
            "orders.TIME_STAMP, "
            "INVOICES.TOTAL_INC, "
            "orders.ITEM_NAME, "
            "cast(ORDERS.QTY * ORDERS.PRICE_LEVEL0   as numeric(17,4)) PRICE, "
            "ORDERS.QTY, "
            "INVOICES.INVOICE_NUMBER, "
            "INVOICES.CONTACTS_KEY, "
            "CONTACTS.NAME, "
            "contacts.member_number, "
            "INVOICES.CLOSED "
        "from INVOICES "
            "left join TAB on INVOICES.INVOICE_KEY = TAB.INVOICE_KEY "
            "left join ORDERS on TAB.TAB_KEY = ORDERS.TAB_KEY "
            "left join CONTACTS on INVOICES.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        "where "
            "orders.time_stamp >= :StartTime and "
            "orders.time_stamp < :EndTime and "
            "INVOICES.CLOSED = 'F' ";

	if (Members->Count)
	{
		qrInvoiceDetailed->SQL->Text =	qrInvoiceDetailed->SQL->Text + "And (" +
											ParamString(Members->Count, "Contacts.Name", "MembersParam") + ")";
	}

	qrInvoiceDetailed->SQL->Text =		qrInvoiceDetailed->SQL->Text +

		"Order By "
			"8, 6";

	for (int i=0; i<Members->Count; i++)
	{
		qrInvoiceDetailed->ParamByName("MembersParam" + IntToStr(i))->AsString = Members->Strings[i];
	}

	qrInvoiceDetailed->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrInvoiceDetailed->ParamByName("EndTime")->AsDateTime		= EndTime;



}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupBillPayments(AnsiString InvoiceNumber)
{
	qrBillPayments->Close();
	qrBillPayments->AfterScroll = NULL;
	qrBillPayments->SQL->Text =

"Select   "
    "		ArcBill.ArcBill_Key,              "
    "		ArcBill.Time_Stamp,               "
    "		ArcBill.Invoice_Number,           "
    "		ArcBill.Total,                    "
    "		ArcBill.Patron_Count,             "
    "		Security.Terminal_Name,           "
    "		Security.From_Val Staff_Name,     "
    "		ArcBillPay.Note,                  "
    "        ab.TABLE_NAME TABLE_NUMBER,      "
    "        ab.price ,                        "
    "        paymentPercent.PAY_TYPE,         "
    "        case when ArcBillPay.NOTE <> 'Total Change.' then ((cast (ab.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100) "
    "                else 0 end price                                                                                                  "
	"	From                                                                                                                           "
	"		ArcBill Left Join Security On                                                                                              "
	"			ArcBill.Security_Ref = Security.Security_Ref                                                                           "
	"		Left Join ArcBillPay On                                                                                                    "
	"			ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key                                                                           "
    "              left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME, cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM ARCHIVE a     "
    "             group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=ArcBill.ARCBILL_KEY                                        "
    "   inner join    (SELECT                                                                                                          "                        
    "    ArcBillPay.ARCBILL_KEY,                                                                                                       "
    "    ArcBillPay.PAY_TYPE,                                                                                                          "          
    "    cast((100* COALESCE(sum(ArcBillPay.SUBTOTAL),0))/                                                                             "
    "    Sum(ArcBill.TOTAL)                                                                                                            "
    "     as numeric(17, 4)) AS PayTypeTotal                                                                                           "                 
    "    FROM ArcBillPay                                                                                                               "                   
    "    left join ArcBill on ArcBill.ARCBILL_KEY=ArcBillPay.ARCBILL_KEY                                                               "             
    "    where  ArcBillPay.SUBTOTAL > 0 and ArcBillPay.CASH_OUT<>'T'                                                                   "
    "    group by ArcBillPay.PAY_TYPE ,ArcBillPay.ARCBILL_KEY)paymentPercent on paymentPercent.arcbill_key = ab.arcbill_key            "
	"	Where                                                                                                                          "
	"		Security.Security_Event = 'Billed By' And                                                                                  "
    "       ArcBillPay.SubTotal>0   and "
	"		ArcBill.Invoice_Number = :Invoice_Number                                                                                               "
    "    group by ArcBill.ArcBill_Key,                                                                                                 "
	"		ArcBill.Time_Stamp,                                                                                                        "
	"		ArcBill.Invoice_Number,                                                                                                    "
	"		ArcBill.Total,                                                                                                             "
	"		ArcBill.Patron_Count,                                                                                                      "
	"		Security.Terminal_Name,                                                                                                    "
	"		Security.From_Val ,                                                                                                        "
	"		ArcBillPay.Note,                                                                                                           "
    "        ab.TABLE_NAME,                                                                                                            "
    "        ab.price, paymentPercent.PayTypeTotal ,paymentPercent.PAY_TYPE                                                            "

    " UNION ALL "

    ///Query for adding Tip_Amount
    + _arcBillSubQuery +
    " AND ArcBill.Invoice_Number = :Invoice_Number "
    + _groupingForArcbill +

		"Union All "

"Select "
    "		DayArcBill.ArcBill_Key,               "
    "		DayArcBill.Time_Stamp,                "
    "		DayArcBill.Invoice_Number,            "
    "		DayArcBill.Total,                     "
    "		DayArcBill.Patron_Count,              "
    "		Security.Terminal_Name,               "
    "		Security.From_Val Staff_Name,         "
    "		DayArcBillPay.Note,                   "
    "        		ab.TABLE_NAME TABLE_NUMBER,   "
    "        		ab.price,                     "

    "        paymentPercent.PAY_TYPE,             "
    "        case when DAYARCBILLPAY.NOTE <> 'Total Change.' then ((cast (ab.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100) "
    "                else 0 end price                                                                                                     "
	"	From                                                                                                                              "
	"		DayArcBill Left Join Security On                                                                                              "
	"			DayArcBill.Security_Ref = Security.Security_Ref                                                                           "
	"		Left Join DayArcBillPay On                                                                                                    "
	"			DayArcBill.ArcBill_Key = DayArcBillPay.ArcBill_Key                                                                        "
    "              left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME, cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM DayARCHIVE a     "
    "             group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=DayArcBill.ARCBILL_KEY                                        "
    "   inner join    (SELECT                                                                                                             "
    "    DAYARCBILLPAY.ARCBILL_KEY,                                                                                                       "
    "    DAYARCBILLPAY.PAY_TYPE,                                                                                                          "
    "    cast((100* COALESCE(sum(DAYARCBILLPAY.SUBTOTAL),0))/                                                                             "
    "    Sum(DAYARCBILL.TOTAL)                                                                                                            "
    "     as numeric(17, 4)) AS PayTypeTotal                                                                                              "
    "    FROM DAYARCBILLPAY                                                                                                               "
    "    left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY=DAYARCBILLPAY.ARCBILL_KEY                                                         "
    "    where  DAYARCBILLPAY.SUBTOTAL > 0 and DAYARCBILLPAY.CASH_OUT<>'T'                                                                "
    "    group by DAYARCBILLPAY.PAY_TYPE ,DAYARCBILLPAY.ARCBILL_KEY)paymentPercent on paymentPercent.arcbill_key = ab.arcbill_key         "
	"	Where                                                                             "
	"		Security.Security_Event = 'Billed By' And                                     "
    "       DayArcBillPay.SubTotal>0   and "
	"		DayArcBill.Invoice_Number = :Invoice_Number                                  "
    "    group by DayArcBill.ArcBill_Key,                                                 "
	"		DayArcBill.Time_Stamp,                                                        "
	"		DayArcBill.Invoice_Number,                                                    "
	"		DayArcBill.Total,                                                             "
	"		DayArcBill.Patron_Count,                                                      "
	"		Security.Terminal_Name,                                                       "
	"		Security.From_Val ,                                                           "
	"		DayArcBillPay.Note,                                                           "
    "        ab.TABLE_NAME,                                                               "
    "        ab.price, paymentPercent.PayTypeTotal ,paymentPercent.PAY_TYPE               "
    "UNION ALL "

     ///Query for adding Tip_Amount
    + _dayArcBillSubQuery +
    " AND DayArcBill.Invoice_Number = :Invoice_Number "
    + _groupingForDayArcbill +
    "ORDER BY 1 asc, 11,12 DESC ";

	qrBillPayments->ParamByName("Invoice_Number")->AsString = InvoiceNumber;
}

//---------------------------------------------------------------------------
void TdmMMReportData::SetupBillTenders(TDateTime StartTime, TDateTime EndTime,
                                       TStrings *Tenders, TStrings *Terminals)
{
    AnsiString selected_tenders;
    AnsiString selected_terminals;
    int i = 0;
    int j;

    if (Tenders->Count) {
        selected_tenders = " and abp.pay_type in ('";
        for (j = Tenders->Count - 1; i < j; i++)
            selected_tenders =
              selected_tenders + Tenders->Strings[i] + "', '";
        selected_tenders = selected_tenders + Tenders->Strings[i] + "') ";
    }

    if (Terminals->Count) {
        selected_terminals = " and s.terminal_name in ('";
        for (i = 0, j = Terminals->Count - 1; i < j; i++)
            selected_terminals =
              selected_terminals + Terminals->Strings[i] + "', '";
        selected_terminals =
          selected_terminals + Terminals->Strings[i] + "') ";
    }

    qrBillTenders->Close();
    qrBillTenders->SQL->Text =
      " select                                        "
"            qpa.arcbill_key,                   "
"            paymentPercent.pay_type,           "
"            qpa.terminal_name,                 "
"            qpa.billed_by,                     "
"            qpa.billed_at,                     "
"            qpa.receipt_no  receipt_no,        "
"            CASE WHEN paymentPercent.pay_type = 'Voucher' THEN qpa.voucher_number WHEN(paymentPercent.pay_type = 'Gift Card') then qpa.GiftCard_number else '' END voucher_number, "
 "     CASE WHEN (paymentPercent.pay_type = 'Cash') THEN ABS(coalesce(aChange.CHANGE,0)+coalesce(aChange.CASHOUT,0)  )   when  (paymentPercent.pay_type = 'Eftpos' )THEN ABS(coalesce(aChange.CASHOUT,0))  "
            "  else 0 END change_recv,   "
"            qpa.note ,                         "
"            qpa.TABLE_NAME TABLE_NUMBER,       "
"            CASE WHEN (paymentPercent.pay_type = 'Cash' and chkeftpos.IsCash>1) THEN ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)+coalesce(aChange.Change,0)+coalesce(aChange.CASHOUT,0)-coalesce(paymentPercent.tip,0)  as numeric(17,4)))  "
          "   else ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)-coalesce(paymentPercent.tip,0) as numeric(17,4))) END price, "
"            CASE WHEN (paymentPercent.pay_type = 'Cash' and chkeftpos.IsCash>1) THEN ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)+coalesce(aChange.Change,0)+coalesce(aChange.CASHOUT,0)  as numeric(17,4)))  "
          "   else ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100) as numeric(17,4))) END paid_Excl_Change, "
          "cast(paymentPercent.tip as numeric(17,4)) tip "

" from (	"
"select   "
	"ab.arcbill_key, "
	"s.terminal_name, "
	"s.from_val billed_by,   "
	"ab.time_stamp billed_at, "
	"ab.invoice_number receipt_no, "
	"billpay.SUBTOTAL total_paid,  "
	"MIN(CASE WHEN VoucherQuery.PAY_TYPE = 'Voucher' THEN VoucherQuery.voucher_number END) AS voucher_number, "
	"MIN(CASE WHEN VoucherQuery.PAY_TYPE = 'Gift Card' THEN VoucherQuery.voucher_number END) AS GiftCard_number, "
	"abp.note note ,       "
	"tn.TABLE_NAME,    "
	"ab.total price,      "
	"abp.CASH_OUT    "
	"from ARCBILL ab "
	"inner join (SELECT  a.ARCBILL_KEY, sum(coalesce(a.SUBTOTAL,0)) SUBTOTAL FROM ARCBILLPAY a group by 1) billpay on "
	"billpay.ARCBILL_KEY =ab.ARCBILL_KEY  "
	"inner join ARCBILLpay abp on    "
	"abp.arcbill_key = ab.arcbill_key   "
	"inner join security s on        "
	"s.security_ref = ab.security_ref   "
	"left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME,cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM ARCHIVE a  "
	"group by a.ARCBILL_KEY,a.TABLE_NAME )tn on tn.ARCBILL_KEY=ab.ARCBILL_KEY       "
	"left join (select ARCBILLPAY.ARCBILL_KEY, ARCBILLPAY.VOUCHER_NUMBER,ARCBILLPAY.PAY_TYPE from ARCBILLPAY where ARCBILLPAY.VOUCHER_NUMBER <> '' "
	"group by ARCBILLPAY.ARCBILL_KEY,ARCBILLPAY.VOUCHER_NUMBER,ARCBILLPAY.PAY_TYPE)VoucherQuery on VoucherQuery.ARCBILL_KEY = ab.arcbill_key "
	"where                                "
	"abp.note <> 'Total Change.'         "
	"and s.security_event = 'Billed By' "
"                                and ab.time_stamp between :start_date and :end_date                                          "
	+ selected_tenders
      + selected_terminals
      + "        group by "
"ab.arcbill_key,      "
"s.terminal_name,     "
"s.from_val ,         "
"ab.time_stamp ,      "
"ab.invoice_number ,  "
"billpay.SUBTOTAL ,   "
"abp.note  ,          "
"tn.TABLE_NAME,       "
"tn.price,            "
"abp.CASH_OUT ,ab.total  ) qpa   "
"inner join (SELECT  a.ARCBILL_KEY,  "
"sum(CASE WHEN (a.pay_type = 'Eftpos') THEN 1 else 0 END )Iseftpos,sum(CASE WHEN (a.pay_type = 'Cash') THEN 1 else 0 END )IsCash   "
"FROM ARCBILLPAY a group by 1  ) chkeftpos on  chkeftpos.ARCBILL_KEY = qpa.arcbill_key "
"inner join (SELECT A.ARCBILL_KEY, MIN(CASE WHEN a.CASH_OUT = 'T' THEN COALESCE(a.SUBTOTAL,0) END) AS Cashout, "
"MIN(CASE WHEN ( a.NOTE='Total Change.') THEN coalesce(a.SUBTOTAL,0) END) AS CHANGE  FROM ARCBILLPAY a  group BY 1)aChange on  aChange.ARCBILL_KEY = qpa.arcbill_key "
"           inner join    (SELECT                                                                                             "
"        abp.ARCBILL_KEY,                                                                                              "
"        abp.PAY_TYPE,                                                                                                 "
"       cast((((100* COALESCE(sum(abp.SUBTOTAL),0))/Sum(ARCBILL.TOTAL))) as numeric(17, 4)) AS PayTypeTotal,   "
"       cast(coalesce(abp.TIP_AMOUNT,0) as numeric(17,4)) tip "
"        FROM ARCBILLPAY abp                                                                                                                         "
"        left join ARCBILL on ARCBILL.ARCBILL_KEY=abp.ARCBILL_KEY                                                                          "
"        where  abp.SUBTOTAL > 0 and abp.CASH_OUT<>'T'      "
 + selected_tenders +
"                                                                                                                                                 "
"        group by abp.PAY_TYPE ,abp.ARCBILL_KEY,abp.TIP_AMOUNT)paymentPercent on paymentPercent.arcbill_key = qpa.arcbill_key                      "
"                                                                                                                                                 "
"inner join  (select count(*) TabCount,a.ARCBILL_key                                                                                              "
"                             from                                                                                                                "
"                             (select a.TABLE_NAME, a.ARCBILL_KEY from                                                                            "
"                            ARCHIVE a)a                                                                                                          "
"                            group by a.ARCBILL_KEY)noOfTab on noOfTab.ARCBILL_KEY = qpa.arcbill_key                                              "
"                                                                                                                                                 "
"                                                                                                                                                 "
"                  inner join (select abp.arcbill_key,                                                                                            "
"                                     abp.voucher_number,                                                                                         "
"                                     abp.subtotal   change_recv                                                                                  "
"                                     from ARCBILLPAY abp                                                                                         "
"                                     where abp.note = 'Total Change.') qpb on                                                                    "
"                       qpb.arcbill_key = qpa.arcbill_key                                                                                         "
"            group by paymentPercent.pay_type, qpa.arcbill_key,                                                   "
"                      qpa.terminal_name,                                                                          "
"                      qpa.billed_by,                                                                              "
"                      qpa.billed_at,                                                                              "
"                      qpa.receipt_no,                                                                             "
"                      qpa.voucher_number,                                                                         "
"                      qpa.note , qpb.change_recv ,  qpa.TABLE_NAME, qpa.price , paymentPercent.PayTypeTotal ,qpa.GiftCard_number ,aChange.Change,chkeftpos.IsCash,aChange.CASHOUT,aChange.CHANGE,paymentPercent.tip  "

"UNION  all "

      " select                                        "
"            qpa.arcbill_key,                   "
"            paymentPercent.pay_type,           "
"            qpa.terminal_name,                 "
"            qpa.billed_by,                     "
"            qpa.billed_at,                     "
"            qpa.receipt_no  receipt_no,        "
"            CASE WHEN paymentPercent.pay_type = 'Voucher' THEN qpa.voucher_number WHEN(paymentPercent.pay_type = 'Gift Card') then qpa.GiftCard_number else '' END voucher_number, "
 "     CASE WHEN (paymentPercent.pay_type = 'Cash') THEN ABS(coalesce(aChange.CHANGE,0)+coalesce(aChange.CASHOUT,0)  )   when  (paymentPercent.pay_type = 'Eftpos' )THEN abs(coalesce(aChange.CASHOUT,0))  "
            "  else 0 END change_recv,   "
"            qpa.note ,                         "
"            qpa.TABLE_NAME TABLE_NUMBER,       "
"            CASE WHEN (paymentPercent.pay_type = 'Cash' and chkeftpos.IsCash>1) THEN ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)+coalesce(aChange.Change,0)+coalesce(aChange.CASHOUT,0)-coalesce(paymentPercent.tip,0)  as numeric(17,4)))  "
          "   else ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)-coalesce(paymentPercent.tip,0) as numeric(17,4))) END price, "
          "            CASE WHEN (paymentPercent.pay_type = 'Cash' and chkeftpos.IsCash>1) THEN ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)+coalesce(aChange.Change,0)+coalesce(aChange.CASHOUT,0)  as numeric(17,4)))  "
          "   else ( cast(((cast (qpa.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100) as numeric(17,4))) END paid_Excl_Change, "
          " cast(paymentPercent.tip as numeric(17,4)) tip "

" from ( "
	"select   "
	"ab.arcbill_key, "
	"s.terminal_name, "
	"s.from_val billed_by,   "
	"ab.time_stamp billed_at, "
	"ab.invoice_number receipt_no, "
	"billpay.SUBTOTAL total_paid,  "
	"MIN(CASE WHEN VoucherQuery.PAY_TYPE = 'Voucher' THEN VoucherQuery.voucher_number END) AS voucher_number, "
	"MIN(CASE WHEN VoucherQuery.PAY_TYPE = 'Gift Card' THEN VoucherQuery.voucher_number END) AS GiftCard_number, "
	"abp.note note ,       "
	"tn.TABLE_NAME,    "
	"ab.total price,      "
	"abp.CASH_OUT    "
	"from dayarcbill ab "
	"inner join (SELECT  a.ARCBILL_KEY, sum(a.SUBTOTAL) SUBTOTAL FROM dayarcbillPAY a group by 1) billpay on "
	"billpay.ARCBILL_KEY =ab.ARCBILL_KEY  "
	"inner join dayarcbillpay abp on    "
	"abp.arcbill_key = ab.arcbill_key   "
	"inner join security s on        "
	"s.security_ref = ab.security_ref   "
	"left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME,cast (sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM dayARCHIVE a  "
	"group by a.ARCBILL_KEY,a.TABLE_NAME )tn on tn.ARCBILL_KEY=ab.ARCBILL_KEY       "
	"left join (select dayarcbillPAY.ARCBILL_KEY, dayarcbillPAY.VOUCHER_NUMBER,dayarcbillPAY.PAY_TYPE from dayarcbillPAY where dayarcbillPAY.VOUCHER_NUMBER <> '' "
	"group by dayarcbillPAY.ARCBILL_KEY,dayarcbillPAY.VOUCHER_NUMBER,dayarcbillPAY.PAY_TYPE)VoucherQuery on VoucherQuery.ARCBILL_KEY = ab.arcbill_key "
	"where                                "
	"abp.note <> 'Total Change.'         "
	"and s.security_event = 'Billed By' "
"and ab.time_stamp between :start_date and :end_date                                             "
 + selected_tenders
      + selected_terminals
      +
	  "        group by "
"ab.arcbill_key,      "
"s.terminal_name,     "
"s.from_val ,         "
"ab.time_stamp ,      "
"ab.invoice_number ,  "
"billpay.SUBTOTAL ,   "
"abp.note  ,          "
"tn.TABLE_NAME,       "
"tn.price,            "
"abp.CASH_OUT  ,ab.total       ) qpa "
"inner join (SELECT  a.ARCBILL_KEY,  "
"sum(CASE WHEN (a.pay_type = 'Eftpos') THEN 1 else 0 END )Iseftpos,sum(CASE WHEN (a.pay_type = 'Cash') THEN 1 else 0 END )IsCash   "
"FROM DAYARCBILLPAY a group by 1  ) chkeftpos on  chkeftpos.ARCBILL_KEY = qpa.arcbill_key "
"inner join (SELECT A.ARCBILL_KEY, MIN(CASE WHEN a.CASH_OUT = 'T' THEN COALESCE(a.SUBTOTAL,0) END) AS Cashout, "
"MIN(CASE WHEN ( a.NOTE='Total Change.') THEN coalesce(a.SUBTOTAL,0) END) AS CHANGE  FROM DAYARCBILLPAY a  group BY 1)aChange on  aChange.ARCBILL_KEY = qpa.arcbill_key "
"           inner join    (SELECT                                                                                                "
"        abp.ARCBILL_KEY,                                                                                              "
"        abp.PAY_TYPE,                                                                                                 "
"     cast((((100* COALESCE(sum(abp.SUBTOTAL),0))/Sum(DAYARCBILL.TOTAL))) as numeric(17, 4)) AS PayTypeTotal,        "
"       cast(coalesce(abp.TIP_AMOUNT,0) as numeric(17,4)) tip "
"        FROM DAYARCBILLPAY abp                                                                                                                               "
"        left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY=abp.ARCBILL_KEY                                                                          "
"        where  abp.SUBTOTAL > 0 and abp.CASH_OUT<>'T'   "
+ selected_tenders +
"        group by abp.PAY_TYPE ,abp.ARCBILL_KEY,abp.TIP_AMOUNT)paymentPercent on paymentPercent.arcbill_key = qpa.arcbill_key                         "
"                  inner join (select abp.arcbill_key,                                                             "
"                                     abp.voucher_number,                                                          "
"                                     abp.subtotal   change_recv                                                   "
"                                     from dayarcbillpay abp                                                       "
"                                     where abp.note = 'Total Change.') qpb on                                     "
"                       qpb.arcbill_key = qpa.arcbill_key                                                          "
"             group by paymentPercent.pay_type, qpa.arcbill_key,                                                   "
"                      qpa.terminal_name,                                                                          "
"                      qpa.billed_by,                                                                              "
"                      qpa.billed_at,                                                                              "
"                      qpa.receipt_no,                                                                             "
"                      qpa.voucher_number,                                                                         "
"                      qpa.note , qpb.change_recv ,  qpa.TABLE_NAME, qpa.price , paymentPercent.PayTypeTotal ,qpa.GiftCard_number ,aChange.Change,chkeftpos.IsCash,aChange.CASHOUT,aChange.CHANGE,paymentPercent.tip  "

"      order by 2,1, 5, 3, 4, 6, 7, 8 ,9  ;";

    qrBillTenders->ParamByName("start_date")->AsDateTime = StartTime;
    qrBillTenders->ParamByName("end_date")->AsDateTime = EndTime;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TdmMMReportData::SetupSkimming( TDateTime StartTime, TDateTime EndTime)
{
	qrSkimming->Close();
    qrSkimming->SQL->Text =
            "Select "
                "Refloat_Skim.Amount, "
                "Refloat_Skim.Staff, "
                "Refloat_Skim.Terminal_Name, "
                "Refloat_Skim.Time_Stamp, "
                "Refloat_Skim.Transaction_type, "
                "Refloat_Skim.Reasons, "
                "zeds.initial_float "
            "From "
                "Refloat_Skim left join zeds on refloat_skim.Z_KEY = zeds.z_key "
            "Where "
                "Refloat_Skim.Time_Stamp >= :StartTime And "
                "Refloat_Skim.Time_Stamp < :EndTime "
            "Order by 3, 4;";


	qrSkimming->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrSkimming->ParamByName("EndTime")->AsDateTime		= EndTime;






/*

	 while ( !qrSkimming->Eof )
	 {

    if(qrSkimming->FieldByName("Transaction_Type")->AsString == "Skim")
        qrSkimming->FieldByName("Transaction_Type")->AsString = "Withdrawn";
    else if(qrSkimming->FieldByName("Transaction_Type")->AsString == "Refloat")
        qrSkimming->FieldByName("Transaction_Type")->AsString = "Deposit";
     }
  */
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupRefloat( TDateTime StartTime, TDateTime EndTime)
{
	qrSkimming->Close();
    qrSkimming->SQL->Text =
        "Select "
            "Amount, "
            "Staff, "
            "Terminal_Name, "
            "Time_Stamp, "
            "Reasons "
        "From "
            "Refloat_Skim "
        "Where "
            "Time_Stamp >= :StartTime And "
            "Time_Stamp < :EndTime And "
            "( Transaction_Type = 'Refloat');";


	qrRefloat->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrRefloat->ParamByName("EndTime")->AsDateTime		= EndTime;



}


//---------------------------------------------------------------------------
void TdmMMReportData::SetupBillPayments(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, TStrings *Terminals)
{
		/*
	  proposal for adding payment types

	  1 HEADER - get distinct keys Master
	  ArcBill
	  ArcBillPay
	  Contacts
	  Security

	  2 Item Details  Detail (1)
	  Archive
	  ArcSurcharge

	  3 Tenders Detail (2)
	  ArcBillPay

	*/

	qrBillPayments->Close();
	qrBillPayments->AfterScroll = NULL;
	qrBillPayments->SQL->Text =

    " Select                                         "
	"		ArcBill.ArcBill_Key,                    "
	"		ArcBill.Time_Stamp,                     "
	"		ArcBill.Invoice_Number,                 "
	"		ArcBill.Total,                          "
	"		ArcBill.Patron_Count,                   "
	"		Security.Terminal_Name,                 "
	"		Security.From_Val Staff_Name,           "
	"		ARCBILLPAY.Note,                        "
    "        		ab.TABLE_NAME TABLE_NUMBER,     "
    "        		ab.price,                       "
    "        paymentPercent.PAY_TYPE,               "
    "        case when ARCBILLPAY.NOTE <> 'Total Change.' then ((cast (ab.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100)    "
    "                else 0 end price                                                                                                     "
	"	From                                                                                                                              "
	"		ArcBill Inner Join Security On                                                                                                "
	"			ArcBill.Security_Ref = Security.Security_Ref                                                                              "
	"		Inner Join ARCBILLPAY On                                                                                                      "
	"			ArcBill.ArcBill_Key = ARCBILLPAY.ArcBill_Key                                                                              "
    "             left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME ,cast(sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM ARCHIVE a       "
    "             group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=ArcBill.ARCBILL_KEY                                           "
	"	    inner join    (SELECT                                                                                                         "
    "    ARCBILLPAY.ARCBILL_KEY,                                                                                                          "
    "    ARCBILLPAY.PAY_TYPE,                                                                                                             "
    "    cast((100* COALESCE(sum(ARCBILLPAY.SUBTOTAL),0))/                                                                                "
    "    Sum(ArcBill.TOTAL)                                                                                                               "
    "     as numeric(17, 4)) AS PayTypeTotal                                                                                              "
    "    FROM ARCBILLPAY                                                                                                                  "
    "    left join ArcBill on ArcBill.ARCBILL_KEY=ARCBILLPAY.ARCBILL_KEY                                                                  "
    "    where  ARCBILLPAY.SUBTOTAL > 0 and ARCBILLPAY.CASH_OUT<>'T'                                                                      "
    "    group by ARCBILLPAY.PAY_TYPE ,ARCBILLPAY.ARCBILL_KEY)paymentPercent on paymentPercent.arcbill_key = ab.arcbill_key               "
	"	Where                                                                                                                             "
    "     ARCBILLPAY.SubTotal>0   and                                                                                                     "
	"	ArcBill.Time_Stamp >= :StartTime And                                                                                              "
	"	ArcBill.Time_Stamp < :EndTime And                                                                                                 "
	"		Security.Security_Event = 'Billed By'   ";
    if (Invoices->Count)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoiceParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrBillPayments->SQL->Text =		qrBillPayments->SQL->Text +
    " group by ArcBill.ArcBill_Key,     "
	"		ArcBill.Time_Stamp,                                                                                                           "
	"		ArcBill.Invoice_Number,                                                                                                       "
	"		ArcBill.Total,                                                                                                                "
	"		ArcBill.Patron_Count,                                                                                                         "
	"		Security.Terminal_Name,                                                                                                       "
	"		Security.From_Val ,                                                                                                           "
	"		ARCBILLPAY.Note,                                                                                                              "
    "        ab.TABLE_NAME,                                                                                                               "
    "        ab.price, paymentPercent.PayTypeTotal ,paymentPercent.PAY_TYPE       ";

	qrBillPayments->SQL->Text =		qrBillPayments->SQL->Text +

     ///Query for adding Tip_Amount
     " Union All " +
     _arcBillSubQuery +
     "and ArcBill.Time_Stamp >= :StartTime And "
	"	ArcBill.Time_Stamp < :EndTime    ";
     if (Invoices->Count)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoiceParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrBillPayments->SQL->Text =		qrBillPayments->SQL->Text +
        _groupingForArcbill +

	" Union All "
    " Select                                       "
	"		DayArcBill.ArcBill_Key,               "
	"		DayArcBill.Time_Stamp,                "
	"		DayArcBill.Invoice_Number,            "
	"		DayArcBill.Total,                     "
	"		DayArcBill.Patron_Count,              "
	"		Security.Terminal_Name,               "
	"		Security.From_Val Staff_Name,         "
	"		DayArcBillPay.Note,                   "
    "        		ab.TABLE_NAME TABLE_NUMBER,   "
    "        		ab.price,                     "
    "        paymentPercent.PAY_TYPE,             "
    "        case when DAYARCBILLPAY.NOTE <> 'Total Change.' then ((cast (ab.price* paymentPercent.PayTypeTotal as numeric(17,4))) / 100) "
    "                else 0 end price  "
	"	From                                                        "
    "                                                               "
	"		DayArcBill Inner Join Security On                       "
	"			DayArcBill.Security_Ref = Security.Security_Ref     "
	"		Inner Join DayArcBillPay On                             "
	"			DayArcBill.ArcBill_Key = DayArcBillPay.ArcBill_Key  "
    "             left join(SELECT  a.ARCBILL_KEY,a.TABLE_NAME ,cast(sum(a.QTY * a.PRICE) as numeric(17,4)) price FROM DayARCHIVE a  "
    "             group by a.ARCBILL_KEY,a.TABLE_NAME )ab on ab.ARCBILL_KEY=DayArcBill.ARCBILL_KEY                                   "
	"	    inner join    (SELECT                                                                                                    "
    "    DAYARCBILLPAY.ARCBILL_KEY,                                                                                                  "              
    "    DAYARCBILLPAY.PAY_TYPE,                                                                                                     "
    "    cast((100* COALESCE(sum(DAYARCBILLPAY.SUBTOTAL),0))/                                                                        "     
    "    Sum(DAYARCBILL.TOTAL)                                                                                                       "     
    "     as numeric(17, 4)) AS PayTypeTotal                                                                                         "
    "    FROM DAYARCBILLPAY                                                                                                          "                       
    "    left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY=DAYARCBILLPAY.ARCBILL_KEY                                                    "
    "    where  DAYARCBILLPAY.SUBTOTAL > 0 and DAYARCBILLPAY.CASH_OUT<>'T'                                                           "     
    "                                                                                                                                "
    "    group by DAYARCBILLPAY.PAY_TYPE ,DAYARCBILLPAY.ARCBILL_KEY)paymentPercent on paymentPercent.arcbill_key = ab.arcbill_key    "  
	"	Where                                                                                                                        "
    "     DayArcBillPay.SubTotal>0   and                                                                                             "
	"	DayArcBill.Time_Stamp >= :StartTime And                                                                                        "
	"	DayArcBill.Time_Stamp < :EndTime And                                                                                           "
	"		Security.Security_Event = 'Billed By'   ";
    if (Invoices->Count)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Invoices->Count, "DayArcBill.Invoice_Number", "InvoiceParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrBillPayments->SQL->Text =		qrBillPayments->SQL->Text +

    " group by DayArcBill.ArcBill_Key,                                         "
	"		DayArcBill.Time_Stamp,                                             "
	"		DayArcBill.Invoice_Number,                                         "
	"		DayArcBill.Total,                                                  "
	"		DayArcBill.Patron_Count,                                           "
	"		Security.Terminal_Name,                                            "
	"		Security.From_Val ,                                                "
	"		DayArcBillPay.Note,                                                "
    "        ab.TABLE_NAME,                                                    "
    "        ab.price, paymentPercent.PayTypeTotal ,paymentPercent.PAY_TYPE    "

      ///Query for adding Tip_Amount
      " Union All "
    + _dayArcBillSubQuery  +
    " and	DayArcBill.Time_Stamp >= :StartTime And  "
	"	DayArcBill.Time_Stamp < :EndTime    " ;
     if (Invoices->Count)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Invoices->Count, "DayArcBill.Invoice_Number", "InvoiceParam") + ")";
	}
	if (Terminals->Count > 0)
	{
		qrBillPayments->SQL->Text =	qrBillPayments->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrBillPayments->SQL->Text =		qrBillPayments->SQL->Text +
      _groupingForDayArcbill +

    " ORDER BY 1 asc, 11,12 DESC ";

	for (int i=0; i<Invoices->Count; i++)
	{
		qrBillPayments->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}
	for (int i=0; i<Terminals->Count; i++)
	{
		qrBillPayments->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrBillPayments->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrBillPayments->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrBillPaymentsAfterScroll(
		TDataSet *DataSet)
{
	qrBillDetails->Close();//                   i'm broken
	qrBillDetails->ParamByName("ArcBill_Key")->AsInteger = qrBillPayments->FieldByName("ArcBill_Key")->AsInteger;
//	qrBillDetails->ParamByName("DayArcBill_Key")->AsInteger = qrBillPayments->FieldByName("DayArcBill_Key")->AsInteger;
	qrBillDetails->Open();
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupBillDetails(AnsiString InvoiceNumber)
{
	qrBillPayments->Close();
	qrBillPayments->AfterScroll = qrBillPaymentsAfterScroll;
	qrBillPayments->SQL->Text =
		"Select "
			"ArcBill.ArcBill_Key,"
			"Security.Time_Stamp,"
			"ArcBill.Invoice_Number,"
			"cast(ArcBill.Total as numeric(17, 4)) Total,"
			"ArcBill.Patron_Count,"
			"Security.Terminal_Name,"
			"Security.From_Val Staff_Name,"
			"cast(Sum(Archive.Discount ) as numeric(17, 4)) Discount,"
			"Cast(Null As VarChar(50)) Pay_Type,"
			"Cast(0 As Numeric(17, 4)) SubTotal, "
            "c.billed_to  billed_to "
		"From "
			"ArcBill Inner Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "
			"inner Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "

            "left join (Select "
                    "POINTSTRANSACTIONS.INVOICE_NUMBER, "
                    "case when (c.name is null) then 'Non-member transaction'else (c.name ||' '|| c.LAST_NAME) end billed_to "
                    "From POINTSTRANSACTIONS "
                    "left join contacts c on c.contacts_key = POINTSTRANSACTIONS.contacts_key "
                    "Group By 1,2 "
                    "union all "
                    "Select ab.INVOICE_NUMBER, "
                    "case when (c.name is null) then 'Non-member transaction'else (c.name ||' '|| c.LAST_NAME) end billed_to "
                    "From ARCHIVE "
                    "left join contacts c on c.contacts_key = ARCHIVE.LOYALTY_KEY "
                    "left join ARCBILL ab on ab.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY   "
                    "Group By "
                    "1,2) c on c.INVOICE_NUMBER = ArcBill.INVOICE_NUMBER "

           " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

		"Where "
        
		    "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and  "
		    "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and  "
			"ArcBill.Invoice_Number = :in and "
			"Security.Security_Event = 'Billed By' "
		"Group By "
			"ArcBill.ArcBill_Key,"
			"Security.Time_Stamp,"
			"ArcBill.Invoice_Number,"
			"ArcBill.Total,"
			"ArcBill.Patron_Count,"
			"Security.Terminal_Name,"
			"Security.From_Val, "
            "c.billed_to "

		"Order By "
			"4 Asc;";
	qrBillPayments->ParamByName("in")->AsString = InvoiceNumber;

	qrBillDetails->Close();
	qrBillDetails->SQL->Text =
		"Select "
			"Cast(Archive.Size_Name As VarChar(30)) Size_Name,"
			"Cast(Archive.Item_Name As VarChar(50)) Item_Name,"
			"cast(Archive.PRICE * Archive.Qty + Archive.Discount as numeric(17,4)) Price, "	   //Item price + Discount
			"Security.Time_Stamp,"
			"Security.Security_Ref,"
			"Security.Security_Event,"
			"Contacts.Name, "
			"cast(Archive.Discount as numeric(17, 4)) Discount,"
			"cast(Archive.Price * Archive.Qty as numeric(17, 4)) DiscPrice "
		"From "
			"Archive Left Join Security On "
				"Archive.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	"LEFT JOIN ( "					 //join with tax
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

		 "Where "

            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and "
			"Archive.ArcBill_Key = :ArcBill_Key And "
			"Security.Security_Event = 'Ordered By' "
		"Union All "

		"Select "
			"Cast('' As VarChar(30)) Size_Name,"
			"Cast(ArcSurcharge.Pay_Type As VarChar(50)) Item_Name,"
			"Cast(ArcSurcharge.SubTotal As Numeric(17, 4)) Price,"
			"Security.Time_Stamp,"
			"Security.Security_Ref,"
			"Cast('Ordered By' As VarChar(50)) Security_Event,"
			"Contacts.Name,"
			"cast(0.00 as Numeric(17,4)) Discount,"
			"Cast(ArcSurcharge.SubTotal As Numeric(17, 4)) DiscPrice "
		"From "
			"ArcSurcharge Left Join ArcBill On "
				"ArcSurcharge.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
            "left join archive on "
            "     archive.arcbill_key = arcsurcharge.arcbill_key "
  " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "

            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'and "
			"ArcSurcharge.ArcBill_Key = :ArcBill_Key And "
			"Security.Security_Event = 'Billed By' "

		"Order By "
			"4,"
			"2,"
			"1,"
			"5,"
			"6 Desc";
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupBillDetails(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, TStrings *Terminals)
{
	qrBillPayments->Close();
	qrBillPayments->AfterScroll = qrBillPaymentsAfterScroll;
	qrBillPayments->SQL->Text =
		"Select "
			"ArcBill.ArcBill_Key,"
			"0 as DayArcBill_Key,"
			"Security.Time_Stamp,"
			"ArcBill.Invoice_Number,"
			"cast(ArcBill.Total as numeric(17,4)) Total,"
			"ArcBill.Patron_Count,"
			"Security.Terminal_Name,"
			"Security.From_Val Staff_Name,"
			"cast(Sum(Archive.Discount) as numeric(17,4)) Discount,"
			"Cast(Null As VarChar(50)) Pay_Type,"
			"Cast(0 As Numeric(17, 4)) SubTotal, "
            "c.billed_to  billed_to "
		"From "
			"ArcBill Left Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "
			"left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "

            "left join (Select "
                    "POINTSTRANSACTIONS.INVOICE_NUMBER, "
                    "case when (c.name is null) then 'Non-member transaction'else (c.name ||' '|| c.LAST_NAME) end billed_to "
                    "From POINTSTRANSACTIONS "
                    "left join contacts c on c.contacts_key = POINTSTRANSACTIONS.contacts_key "
                    "Group By 1,2 "
                    "union all "
                    "Select ab.INVOICE_NUMBER, "
                    "case when (c.name is null) then 'Non-member transaction'else (c.name ||' '|| c.LAST_NAME) end billed_to "
                    "From ARCHIVE "
                    "left join contacts c on c.contacts_key = ARCHIVE.LOYALTY_KEY "
                    "left join ARCBILL ab on ab.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY   "
                    "Group By "
                    "1,2) c on c.INVOICE_NUMBER = ArcBill.INVOICE_NUMBER "



       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "  
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
			"Security.Time_Stamp >= :StartTime And "
			"Security.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' ";
	if (Invoices->Count)
	{
		qrBillPayments->SQL->Text	=	qrBillPayments->SQL->Text + "And (" +
												ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoiceParam") + ")";
	}
	if (Terminals->Count)
	{
		qrBillPayments->SQL->Text	=	qrBillPayments->SQL->Text + "And (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrBillPayments->SQL->Text		=	qrBillPayments->SQL->Text +

    		"Group By "
			"ArcBill.ArcBill_Key, "
			"Security.Time_Stamp, "
			"ArcBill.Invoice_Number, "
			"ArcBill.Total, "
			"ArcBill.Patron_Count, "
			"Security.Terminal_Name, "
			"Security.From_Val, "
            "c.billed_to "

            "Order By "
			"4 Asc;";


 			/*"union all "
            
            "Select "
			"ArcBill.ArcBill_Key, "
            "0 as DayArcBill_Key,"
			"Security.Time_Stamp, "
			"ArcBill.Invoice_Number, "
			"cast(ArcBill.Total as numeric(17, 4)) Total, "
			"ArcBill.Patron_Count, "
			"Security.Terminal_Name, "
			"Security.From_Val Staff_Name, "
			"Cast(0 As Numeric(17, 4))  Discount, "
			"Cast(Null As VarChar(50)) Pay_Type, "
			"Cast(0 As Numeric(17, 4)) SubTotal, "
            "case when (c.name is null) then 'Non-member transaction' "
                                       "else (c.name ||' '|| c.LAST_NAME) "
                 "end billed_to "
		"From "
		"POINTSTRANSACTIONS "
		"inner join ArcBill on "
			    "POINTSTRANSACTIONS.INVOICE_NUMBER = ArcBill.INVOICE_NUMBER "
			 " left Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "

            "left join contacts c on c.contacts_key = POINTSTRANSACTIONS.CONTACTS_KEY "

	"Where "
			"Security.Time_Stamp >= :StartTime And "
			"Security.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' ";
        if (Invoices->Count)
        {
            qrBillPayments->SQL->Text	=	qrBillPayments->SQL->Text + "And (" +
                                                    ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoiceParam") + ")";
        }
        if (Terminals->Count)
        {
            qrBillPayments->SQL->Text	=	qrBillPayments->SQL->Text + "And (" +
                                                    ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
        }
        qrBillPayments->SQL->Text		=	qrBillPayments->SQL->Text +
		"Group By "
			"ArcBill.ArcBill_Key, "
			"Security.Time_Stamp, "
			"ArcBill.Invoice_Number, "
			"ArcBill.Total, "
			"ArcBill.Patron_Count, "
			"Security.Terminal_Name, "
			"Security.From_Val, "
            "billed_to " */



	for (int i=0; i<Invoices->Count; i++)
	{
		qrBillPayments->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}
	for (int i=0; i<Terminals->Count; i++)
	{
		qrBillPayments->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrBillPayments->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrBillPayments->ParamByName("EndTime")->AsDateTime		= EndTime;

	qrBillDetails->Close();
	qrBillDetails->SQL->Text =
		"Select "
			"Cast(Archive.Size_Name As VarChar(30)) Size_Name,"
			"Cast(Archive.Item_Name As VarChar(50)) Item_Name,"
			"cast(sum(Archive.PRICE * Archive.Qty + (-1)*Archive.Discount) as numeric(17,4)) Price,"	 //price + discount
			"Security.Time_Stamp,"
			"Security.Security_Ref,"
			"Security.Security_Event,"
			"Contacts.Name,"
			"cast(sum(Archive.Discount) as numeric(17,4)) Discount,"
			"cast(sum(Archive.Qty * Archive.Price)  as numeric(17, 4)) DiscPrice "
		"From "
			"Archive "
            "Left Join Security On "
				"Archive.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
    " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	
		"Where "

            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'and "
	   		"Archive.ArcBill_Key = :ArcBill_Key And "
			"Security.Security_Event = 'Ordered By' "
         "group by  1,2 ,4,5,6,7 "
		"Union All "

		"Select "
			"Cast(DayArchive.Size_Name As VarChar(30)) Size_Name,"
			"Cast(DayArchive.Item_Name As VarChar(50)) Item_Name,"
			"cast(sum(DayArchive.PRICE + (-1)*DayArchive.Discount) as numeric(17, 4)) Price,"  // price + discount
			"Security.Time_Stamp,"
			"Security.Security_Ref,"
			"Security.Security_Event,"
			"Contacts.Name, "
			"cast(sum(DayArchive.Discount) as numeric(17, 4)) Discount,"
			"cast(sum(DayArchive.Qty * DayArchive.Price + DayArchive.Discount) as numeric(17, 4)) DiscPrice "
		"From "
			"DayArchive "
            "Left Join Security On "
				"DayArchive.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
	"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
	    	"Where "

            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and "
			"DayArchive.ArcBill_Key = :DayArcBill_Key And "
			"Security.Security_Event = 'Ordered By' "
          "group by  1,2 ,4,5,6,7 "
		"Union All "

		"Select "
			"Cast('' As VarChar(30)) Size_Name,"
			"Cast(ArcSurcharge.Pay_Type As VarChar(50)) Item_Name,"
			"Cast(sum(ArcSurcharge.SubTotal) As Numeric(17, 4)) Price,"
			"Security.Time_Stamp,"
			"Security.Security_Ref,"
			"Cast('Ordered By' As VarChar(50)) Security_Event,"
			"Contacts.Name,"
			"cast(0.00 as Numeric(17,4)) Discount,"
			"Cast(ArcSurcharge.SubTotal As Numeric(17, 4)) DiscPrice "
		"From "
			"ArcSurcharge Left Join ArcBill On "
				"ArcSurcharge.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
		"Where "
		  	"ArcSurcharge.ArcBill_Key = :ArcBill_Key And "
			"Security.Security_Event = 'Billed By' "
             "  group by  1,2 ,4,5,6,7 ,8,9 "
		"Order By "
			"4,"
			"2,"
			"1,"
			"5,"
			"6 Desc";
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupTabSummary(TStrings *TabTypes, TStrings *Tabs)
{

	qrTabSummary->Close();
	qrTabSummary->SQL->Text =
	"Select   "
	"Tab.Tab_Key,  "
	"Tab.Tab_Name,  "
	"Tab.Credit Credit,  "
	"Extract(Day From Orders.Time_Stamp) Trans_Day,  "
	"Extract(Month From Orders.Time_Stamp) Trans_Month, " 
	"Extract(Year From Orders.Time_Stamp) Trans_Year,  "
	"Orders.Time_Stamp,  "
	"Sum(Orders.Qty) Qty, "  
	"Orders.Course_Name, "  
	"Orders.Item_Name,  "
	"Orders.Size_Name, "
	"cast(Sum(Orders.Qty * Orders.BASE_PRICE + Orders.DISCOUNT_WITHOUT_TAX) as numeric(17, 4)) Total_Price ,  "
	"cast((Sum(Orders.Qty * Orders.BASE_PRICE+ Orders.DISCOUNT_WITHOUT_TAX))*Tax.VAT/100 as numeric(17, 4)) VAT,  "
	"cast((Sum(Orders.Qty * Orders.BASE_PRICE+ Orders.DISCOUNT_WITHOUT_TAX))*Tax.ServiceCharge/100 as numeric(17, 4)) ServiceCharge ,  "
	"cast(((Sum(Orders.Qty * Orders.BASE_PRICE+ Orders.DISCOUNT_WITHOUT_TAX))*Tax.ServiceCharge/100)*STAX.ServiceChargeTax/100 as numeric(17, 4))  ServiceChargeTax "
	"From  "
	"Tab Left Join Orders On  "
	"Tab.Tab_Key = Orders.Tab_Key  "

	"left join  "
	"(SELECT  cast(1 as int) keyvalue   ,  "
	"TAXPROFILE.ORDER_KEY,   "
	"MIN(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT, "
	"MIN(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,   "
	"MIN(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS OtherServiceCharge    "
	"FROM (SELECT "
	"TFO.ORDER_KEY,  TAXPROFILES.TYPE,  "
	"Cast(Sum(COALESCE(TAXPROFILES.RATE,0) ) as Numeric(17,4)) TAX_RATE  "
	"FROM TAXPROFILES_ORDERS TFO       "
	"left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY  "
	"group by TFO.ORDER_KEY,TAXPROFILES.TYPE     "
	")  TAXPROFILE   "
	"GROUP BY TAXPROFILE.ORDER_KEY  "
	") Tax on Tax.ORDER_KEY=Orders.ORDER_KEY "
	"left join (SELECT      cast(1 as int) keyvalue   , "
	"MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax      FROM VARSPROFILE    ) STAX on  STAX.keyvalue=Tax.keyvalue "
	"Where  "
		"(Orders.Order_Type = 3 Or  "
		"Orders.Order_Type = 0 Or  "
		"Orders.Order_Type Is Null) And  "
       "Tab.TAB_KEY not in (select tab.TAB_KEY from TAB  "
              " Inner join CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY  "
                           " Where CLIPP_TAB.IS_TAB_CLOSED = 'T' and tab.TAB_TYPE = 13) "  ;
	if (TabTypes->Count == 0)
	{
		qrTabSummary->SQL->Text			=	qrTabSummary->SQL->Text +
			" and Tab.Tab_Type in (0,1,2,13) ";
	}
	else
	{
		qrTabSummary->SQL->Text 	=	qrTabSummary->SQL->Text + "And (" +
												ParamString(TabTypes->Count, "Tab.Tab_Type", "TabTypeParam") + ")";
	}
	if (Tabs->Count)
	{
		qrTabSummary->SQL->Text 	=	qrTabSummary->SQL->Text + "And (" +
												ParamString(Tabs->Count, "Tab.Tab_Name", "TabParam") + ")";
	}
	qrTabSummary->SQL->Text			=	qrTabSummary->SQL->Text +
		"Group By  "
		"Tab.Tab_Key,  "
		"Tab.Tab_Name, " 
		"Tab.Credit,  "
		"Orders.Time_Stamp,  "
		"Orders.Course_Name, " 
		"Orders.Item_Name,  "
		"Orders.Size_Name ,Tax.VAT ,Tax.ServiceCharge ,Tax.OtherServiceCharge ,STAX.ServiceChargeTax "
		"Having  "
		"Sum(Orders.Qty) <> 0 " 
		"Order By  "
		"Tab.Tab_Name,  "
		"Tab.Tab_Key,  "
		"Orders.Time_Stamp,  "
		"Orders.Course_Name, " 
		"Orders.Item_Name,  "
		"Orders.Size_Name ; "  ;
	for (int i=0; i<Tabs->Count; i++)
	{
		qrTabSummary->ParamByName("TabParam" + IntToStr(i))->AsString = Tabs->Strings[i];
	}
	for (int i=0; i<TabTypes->Count; i++)
	{
		qrTabSummary->ParamByName("TabTypeParam" + IntToStr(i))->AsString = TabTypes->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupTabDetails(TStrings *TabTypes,TStrings *Tabs)
{
	qrTabDetails->Close();
	qrTabDetails->SQL->Text =
		"Select "
			"Tab_Name, "
			"ID_Type, "
			"ID_Number, "
			"ID_Expire, "
			"Tab_Type, "
			"Tab_Permanent "
		"From "
			"Tab "
        "Where "
            "Tab.TAB_KEY not in (select tab.TAB_KEY from TAB  "
                            "Inner join CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY "
                           " Where CLIPP_TAB.IS_TAB_CLOSED = 'T' and tab.TAB_TYPE = 13)  ";
	if (TabTypes->Count == 0)
	{
		qrTabDetails->SQL->Text			=	qrTabDetails->SQL->Text +
			" And Tab.Tab_Type in (0,1,2,13) ";
	}
	else
	{
		qrTabDetails->SQL->Text 	=	qrTabDetails->SQL->Text + " And (" +
												ParamString(TabTypes->Count, "Tab.Tab_Type", "TabTypeParam") + ")";
	}
	if (Tabs->Count)
	{
		qrTabDetails->SQL->Text 	=	qrTabDetails->SQL->Text + "And (" +
												ParamString(Tabs->Count, "Tab.Tab_Name", "TabParam") + ")";
	}
	qrTabDetails->SQL->Text			=	qrTabDetails->SQL->Text +
		"Order By "
			"Tab.Tab_Name";
	for (int i=0; i<Tabs->Count; i++)
	{
		qrTabDetails->ParamByName("TabParam" + IntToStr(i))->AsString = Tabs->Strings[i];
	}
	for (int i=0; i<TabTypes->Count; i++)
	{
		qrTabDetails->ParamByName("TabTypeParam" + IntToStr(i))->AsString = TabTypes->Strings[i];
	}

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TdmMMReportData::SetupSalesJournal(bool includeSCTaxInTax, TDateTime StartTime, TDateTime EndTime, TStrings *Terminals)
{
   qrSalesJournal1->Close();
   qrSalesJournal1->SQL->Text =
      "SELECT "
         "DAB.time_stamp datetime, "
         "DAB.invoice_number transaction_number, "
         "cast(Sum( COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
         "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, ";
   if( includeSCTaxInTax )
   {
      qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "cast(Sum(coalesce( T.ServiceCharge,0) )as numeric(17, 4)) Scharge, "
         "cast(Sum( coalesce(T.Tax,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) Tax, ";
   }
   else
   {
      qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "cast(Sum( coalesce(T.ServiceCharge,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) SCharge, "
         "cast(Sum( coalesce(T.Tax,0) )as numeric(17, 4)) Tax, ";
   }
   qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "Cast(Sum( DA.Qty * abs(DA.BASE_PRICE) ) as Numeric(17,4)) Price,  "
         "Cast(Sum( DA.Qty * abs(DA.BASE_PRICE) ) + Sum( COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SubTotal,  "
         "Cast(Sum( cast(DA.Qty * abs(DA .BASE_PRICE) as Numeric(17,4))) +Sum(COALESCE((T.ServiceCharge),0)) +Sum(COALESCE((T.Tax),0)) +Sum(COALESCE((T.ServiceChargeTax),0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) total  "
      "FROM DAYARCBILL DAB "
         "INNER JOIN DAYARCHIVE DA "
            "ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
         "LEFT JOIN "
            "("
               "SELECT "
                  "a.ARCHIVE_KEY, "
                  "sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE, "
                  "a.DISCOUNT_GROUPNAME "
               "FROM DAYARCORDERDISCOUNTS a "
               "GROUP BY a.ARCHIVE_KEY, a.DISCOUNT_GROUPNAME"
            ") DAOD ON DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
         "LEFT JOIN  "
            "( "
               "SELECT "
                  "DAYARCORDERTAXES.ARCHIVE_KEY, "
                  "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS Tax, "
                  "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
                  "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax "
               "FROM "
                  "("
                     "SELECT "
                        "a.ARCHIVE_KEY, "
                        "a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                     "FROM DAYARCORDERTAXES a "
                     "GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY  "
                     "ORDER BY 1"
                  ") DAYARCORDERTAXES "
               "GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY "
            ") T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "
      "WHERE "
         "("
            "("
               "COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' "
               "AND "
               "COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'"
            ")"
         ") "
         "AND "
         "DAB.Time_Stamp >= :StartTime and "
         "DAB.Time_Stamp < :EndTime ";
	if (Terminals->Count > 0)
	{
		qrSalesJournal1->SQL->Text	= qrSalesJournal1->SQL->Text + "and (" +
										         ParamString(Terminals->Count, "DA.Terminal_Name", "TerminalParam") + ")";
	}
   qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
      "GROUP BY DAB.invoice_number, DAB.time_stamp "

      " UNION ALL "

      "SELECT "
         "DAB.time_stamp datetime, "
         "DAB.invoice_number transaction_number, "
         "cast(Sum( COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
         "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, ";
   if( includeSCTaxInTax )
   {
      qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "cast(Sum( coalesce(T.ServiceCharge,0) )as numeric(17, 4)) SCharge, "
         "cast(Sum( coalesce(T.Tax,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) Tax, ";
   }
   else
   {
      qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "cast(Sum( coalesce(T.ServiceCharge,0) + COALESCE(T.ServiceChargeTax,0) )as numeric(17, 4)) SCharge, "
         "cast(Sum( coalesce(T.Tax,0) )as numeric(17, 4)) Tax, ";
   }
      qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
         "Cast(Sum( DA.Qty *  abs(DA.BASE_PRICE) ) as Numeric(17,4)) Price,  "
         "Cast(Sum( DA.Qty *   abs(DA.BASE_PRICE)  ) + Sum( COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SubTotal,  "
         "Cast(Sum( cast(DA.Qty * abs(DA .BASE_PRICE) as Numeric(17,4))) +Sum(COALESCE((T.ServiceCharge),0)) +Sum(COALESCE((T.Tax),0)) +Sum(COALESCE((T.ServiceChargeTax),0))+ Sum(COALESCE((DA.DISCOUNT_WITHOUT_TAX),0)) as Numeric(17,4)) total  "
      "FROM ARCBILL DAB "
         "INNER JOIN ARCHIVE DA "
            "ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
         "LEFT JOIN "
            "("
               "SELECT "
                  "a.ARCHIVE_KEY, "
                  "sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE, "
                  "a.DISCOUNT_GROUPNAME "
               "FROM ARCORDERDISCOUNTS a "
               "GROUP BY a.ARCHIVE_KEY, a.DISCOUNT_GROUPNAME"
            ") DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
         "LEFT JOIN "
            "( "
               "SELECT "
                  "ARCORDERTAXES.ARCHIVE_KEY, "
                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS Tax, "
                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
                  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax "
               "FROM "
                  "("
                     "SELECT "
                        "a.ARCHIVE_KEY, "
                        "a.TAX_TYPE, "
                        "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                     "FROM ARCORDERTAXES a "
                     "GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY "
                     "ORDER BY 1"
                  ") ARCORDERTAXES "
               "GROUP BY ARCORDERTAXES.ARCHIVE_KEY "
            ") T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "
      "WHERE "
         "("
            "("
               "COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' "
               "AND "
               "COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'"
            ")"
         ") "
         "AND "
         "DAB.Time_Stamp >= :StartTime and "
         "DAB.Time_Stamp < :EndTime ";
	if (Terminals->Count > 0)
	{
		qrSalesJournal1->SQL->Text	= qrSalesJournal1->SQL->Text + "and (" +
										         ParamString(Terminals->Count, "DA.Terminal_Name", "TerminalParam") + ")";
	}
   qrSalesJournal1->SQL->Text = qrSalesJournal1->SQL->Text +
      "GROUP BY  DAB.invoice_number, DAB.time_stamp   "
      "ORDER BY 1";


	for (int i=0; i<Terminals->Count; i++)
	{
		qrSalesJournal1->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
   qrSalesJournal1->ParamByName("StartTime")->AsDateTime	= StartTime;
   qrSalesJournal1->ParamByName("EndTime")->AsDateTime	= EndTime;
}

//---------------------------------------------------------------------------
void TdmMMReportData::SetupDiscounts(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Locations)
{
	qrDiscounts->Close();
	qrDiscounts->SQL->Text =
	 	"Select "
			"Archive.TIME_STAMP_BILLED Time_Stamp, "
			"Security.Terminal_Name, "
			"Extract(Day From Archive.TIME_STAMP_BILLED) Bill_Day, "
            "Extract(Month From ARCHIVE.TIME_STAMP_BILLED) Bill_Month, "
			"Extract(Year From ARCHIVE.TIME_STAMP_BILLED) Bill_Year, "
			"Security.Note, "
			"Cast(ARCORDERDISCOUNTS.NAME As Varchar(50)) Discount_Name, "
			"Contacts.Name, "
			"ArcBill.Invoice_Number, "

             "Cast(((sum(round(Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) +  "
             "COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0),2)))) as Numeric(17,4)) Total,    "
		    "cast((sum(round(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(Archive.DISCOUNT_WITHOUT_TAX/Archive.DISCOUNT),2)))as numeric(17, 2))  Discount,  "
            " cast(sum(round(ARCORDERDISCOUNTS.DISCOUNTED_VALUE-(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(Archive.DISCOUNT_WITHOUT_TAX/Archive.DISCOUNT)),2))as numeric(17, 2))  AS DiscountTax ,  "
			"cast((sum(round(ARCORDERDISCOUNTS.DISCOUNTED_VALUE,2))) as numeric(17,4))DiscountAmount, "
			"ARCBILL.ArcBill_Key,  "
			"Cast(Archive.Size_Name As VarChar(30)) Size_Name, "
			"Cast(Archive.ITEM_NAME As VarChar(50)) Item_Name,  "
             "Cast((sum(round(coalesce (Archive.QTY * Archive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price,  "

			"Menu.Menu_Type,"
			"cast(Archive.Order_Location as Varchar(25)) Order_Location, "
            "coalesce(cast((CASE WHEN MENU.MENU_TYPE = 0 THEN round(sum(Archive.DISCOUNT/AOD.key),2) END) as numeric(17, 2)),0) AS Food_Menu_Total,  "
            " coalesce(cast((CASE WHEN MENU.MENU_TYPE = 1 THEN  round(sum(Archive.DISCOUNT/AOD.key),2) END) as numeric(17, 2)),0) AS Beverages_Menu_Total, "

              "cast ('' as varchar(25) )CategoryGroup, "
              "cast ('' as varchar(25) ) Category, "
              "cast (0 as numeric(15,4)) Quantity, "
              "cast (0 as numeric(15,4)) Cost "
		"From "
			"Security Left Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
			"Left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
   " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME, a.NAME  "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME, a.NAME ) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
			"Left Join Menu On "
				"Archive.Menu_Name = Menu.Menu_Name "
"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
            "LEFT JOIN ( "
                            "SELECT  a.ARCHIVE_KEY, count(a.ARCHIVE_KEY) KEY  "
                                    "FROM ARCORDERDISCOUNTS a "
                                    "GROUP BY a.ARCHIVE_KEY  )AOD ON Archive.ARCHIVE_KEY = AOD.ARCHIVE_KEY "

		"Where "

          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
               "Archive.DISCOUNT_WITHOUT_TAX<>0 and "

			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Discounted By' And "
			"ArcBill.Invoice_Number Is Not Null and "
			"Archive.Archive_Key Is Not Null ";
	if (Locations->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	if (Discounts->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Discounts->Count, "Security.From_Val", "DiscParam") + ")";
	}
	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +

      " group by "
            "Archive.TIME_STAMP_BILLED , "
			"Security.Terminal_Name, "
			"Security.Note,  "
			"ARCORDERDISCOUNTS.NAME ,  "
			"Contacts.Name,  "
			"ARCBILL.Invoice_Number,  "
			"ARCBILL.ArcBill_Key,  "
		    "ARCHIVE.Size_Name , "
			"ARCHIVE.ITEM_NAME ,  "
	    	"MENU.MENU_TYPE, "
			"Archive.Order_Location  , "
            "Archive.TAX_ON_DISCOUNT   "

		"Union All "

		"Select "
			"Dayarchive.TIME_STAMP_BILLED Time_Stamp, "
			"Security.Terminal_Name, "
			"Extract(Day From Dayarchive.TIME_STAMP_BILLED) Bill_Day, "
            "Extract(Month From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Month, "
			"Extract(Year From DAYARCHIVE.TIME_STAMP_BILLED) Bill_Year, "
			"Security.Note, "
			"Cast(DAYARCORDERDISCOUNTS.NAME As Varchar(50)) Discount_Name, "
			"Contacts.Name, "
			"DAYARCBILL.Invoice_Number, "

            "Cast(((sum(DAYARCHIVE.QTY * DAYARCHIVE.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0) "
            "  ))) as Numeric(17,4)) Total, "
		    "cast((sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE*(DAYARCHIVE.DISCOUNT_WITHOUT_TAX/DAYARCHIVE.DISCOUNT)))as numeric(17, 2))  Discount,  "
            "cast((sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE-(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE*(DAYARCHIVE.DISCOUNT_WITHOUT_TAX/DAYARCHIVE.DISCOUNT))))as numeric(17, 2))  AS DiscountTax ,  "
			"cast((sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE)) as numeric(17,4))DiscountAmount, "
			"DAYARCBILL.ArcBill_Key,  "
			"Cast(DAYARCHIVE.Size_Name As VarChar(30)) Size_Name, "
			"Cast(DAYARCHIVE.ITEM_NAME As VarChar(50)) Item_Name,  "
            "Cast((sum(round(coalesce (DAYARCHIVE.QTY * DAYARCHIVE.PRICE_INCL,0),2) )) as Numeric(17,4)) Price, "
			"MENU.MENU_TYPE,  "
			"cast(DayArchive.Order_Location as Varchar(25)) Order_Location,  "
            " coalesce(cast((CASE WHEN MENU.MENU_TYPE = 0 THEN round(sum(DayArchive.DISCOUNT/DAOD.key),2) END) as numeric(17, 2)),0) AS Food_Menu_Total , "
            "coalesce(cast((CASE WHEN MENU.MENU_TYPE = 1 THEN  round(sum(DayArchive.DISCOUNT/DAOD.key),2) END) as numeric(17, 2)),0) AS Beverages_Menu_Total,  "

            "cast ('' as varchar(25) )CategoryGroup, "
            "cast ('' as varchar(25) ) Category, "
            "cast (0 as numeric(15,4)) Quantity, "
            "cast (0 as numeric(15,4)) Cost "

		"From "
			"Security Left Join DAYARCBILL On "
				"Security.Security_Ref = DAYARCBILL.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
           " left join DAYarchive on DAYarchive.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY   "
      		"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME, a.NAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME, a.NAME ) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
        "Left Join Menu On  "
				"DAYARCHIVE.Menu_Name = Menu.Menu_Name "
                "LEFT JOIN (  "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
						"FROM DAYARCORDERTAXES a   "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  DAYARCORDERTAXES     "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )  "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
                 "LEFT JOIN ( "
                            "SELECT  a.ARCHIVE_KEY, count(a.ARCHIVE_KEY) KEY "
                                    "FROM DAYARCORDERDISCOUNTS a "
                                    "GROUP BY a.ARCHIVE_KEY  )DAOD ON DayArchive.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "

			"Where "

         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
             "Dayarchive.DISCOUNT_WITHOUT_TAX<>0 and "
			"Dayarchive.TIME_STAMP_BILLED >= :StartTime and "
			"Dayarchive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Discounted By' and "
			"DAYARCBILL.Invoice_Number Is Not Null And "
            "DayArchive.Archive_Key Is Not Null ";
	// No Location here on purpose
	if (Discounts->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Discounts->Count, "Security.From_Val", "DiscParam") + ")";
	}
	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +

       " group by "
            "Dayarchive.TIME_STAMP_BILLED, "
			"Security.Terminal_Name, "
			"Security.Note,  "
			"DAYARCORDERDISCOUNTS.NAME ,  "
			"Contacts.Name,  "
			"DAYARCBILL.Invoice_Number,  "
			"DAYARCBILL.ArcBill_Key,  "
		    "DAYARCHIVE.Size_Name , "
			"DAYARCHIVE.ITEM_NAME ,  "
	    	"MENU.MENU_TYPE, "
			"DayArchive.Order_Location , DayArchive.TAX_ON_DISCOUNT, DayArchive.PRICE_INCL  "

		"Order By "
			"19, 1,14";

	for (int i=0; i<Discounts->Count; i++)
	{
		qrDiscounts->ParamByName("DiscParam" + IntToStr(i))->AsString = Discounts->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrDiscounts->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}

	qrDiscounts->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscounts->ParamByName("EndTime")->AsDateTime		= EndTime;


    qrDiscountGrandTotal->Close();
	qrDiscountGrandTotal->SQL->Text =

          "Select   "
          "Cast(sum(round((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)),2)) as Numeric(17,4)) Total, "
          "cast(sum(round(Archive.DISCOUNT_WITHOUT_TAX,2)) as numeric(17, 2)) Discount,  "
          "cast(sum(round(Archive.TAX_ON_DISCOUNT,2)) as numeric(17, 2)) DiscountTax,  "
          "	cast((sum(round(Archive.DISCOUNT,2))) as numeric(17,4))DiscountAmount,  "
         // "Cast(sum(round((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0) - coalesce(Archive.TAX_ON_DISCOUNT,0) ),2)) as Numeric(17,4)) Price "
          "Cast((sum(round(coalesce (Archive.QTY * Archive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price "
   " From "
        "ARCBILL  "
        "Left Join Archive On   "
			   "	ArcBill.ArcBill_Key = Archive.ArcBill_Key  "
               " 	left join (SELECT a.SECURITY_REF, a.FROM_VAL  "
              "FROM SECURITY a where a.SECURITY_EVENT='Discounted By' )SECURITY  on SECURITY.SECURITY_REF=Archive.SECURITY_REF  "
        "LEFT JOIN (  "
				   "	 SELECT    "
					 "	ARCORDERTAXES.ARCHIVE_KEY,  "
					 "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
					 "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
					  "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				 " FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
				   "		Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
					"	FROM ARCORDERTAXES a     "
					"	group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
					"	order by 1 )  ARCORDERTAXES   "
					"	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )  "
					"	AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY  "

      " Where ARCBILL.ARCBILL_KEY  in (Select     ARCBILL.ARCBILL_KEY from ARCBILL left join SECURITY on  SECURITY.SECURITY_REF=ArcBill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and  "
           "   ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where "
            "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
           "   COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') and  ARCHIVE.DISCOUNT_WITHOUT_TAX<>0 AND "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime  ";
	if (Locations->Count)
	{
		qrDiscountGrandTotal->SQL->Text 	= 	qrDiscountGrandTotal->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}

	if (Discounts->Count)
	{
        qrDiscountGrandTotal->SQL->Text 	=	qrDiscountGrandTotal->SQL->Text + "And (" +
											ParamString(Discounts->Count, "Security.From_Val", "DiscParam") + ")";
	}

    qrDiscountGrandTotal->SQL->Text        = qrDiscountGrandTotal->SQL->Text +
		"Union All "

          	"Select "
           " Cast(((sum(round((DAYARCHIVE.QTY * DAYARCHIVE.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ "
            " COALESCE(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0)),2)))) as Numeric(17,4)) Total,  "
            " cast((sum(round(DayArchive.DISCOUNT_WITHOUT_TAX,2)))as numeric(17, 2))  Discount,  "
           " cast((sum(round(DayArchive.TAX_ON_DISCOUNT,2)))as numeric(17, 2))  AS DiscountTax ,  "
		   "	cast((sum(round(DayArchive.DISCOUNT,2))) as numeric(17,4))DiscountAmount,  "
		 //  "Cast(((sum(round((DayArchive.QTY * DayArchive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)- coalesce(DayArchive.TAX_ON_DISCOUNT,0)),2)) )) as Numeric(17,4)) Price "
            "Cast((sum(round(coalesce (DayArchive.QTY * DayArchive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price "
		"From "
        "DAYARCBILL "

       " Left Join DayArchive On  "
				"DayArcbill.ArcBill_Key = DayArchive.ARCBILL_KEY  "
                "LEFT JOIN ( "
					 "SELECT  "
					   "	DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
					   "	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
					   "	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				 " FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

       "Where DayArcbill.ARCBILL_KEY  in (Select     DayArcbill.ARCBILL_KEY from DayArcbill left join SECURITY on  SECURITY.SECURITY_REF=DayArcbill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and "
              "DayArchive.ARCHIVE_KEY not in (Select DAYARCORDERDISCOUNTS.ARCHIVE_KEY from DAYARCORDERDISCOUNTS  where    "
              "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
             " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary')  AND  DAYARCHIVE.DISCOUNT_WITHOUT_TAX<>0 AND "
		"	Dayarchive.TIME_STAMP_BILLED >= :StartTime and "
		"	Dayarchive.TIME_STAMP_BILLED < :EndTime  ";

    qrDiscountGrandTotal->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscountGrandTotal->ParamByName("EndTime")->AsDateTime		= EndTime;



}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDiscountsAndSurchargesTotals(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Locations)
{
	qrDiscountsSurchargesTotals->Close();
	qrDiscountsSurchargesTotals->SQL->Text =
      "Select "
      "  Distinct ARCOD.NAME as DISCOUNT_NAME, "
      "  cast(abs(sum(ARCOD.DISCOUNTED_VALUE)) as numeric(17,4)) as TOTAL "
		"From "
      "      Security SEC "
      "      Join ArcBill ARCB On SEC.Security_Ref = ARCB.Security_Ref "
		"	Join Archive ARC On ARCB.ArcBill_Key = ARC.ArcBill_Key "
" LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,a.NAME,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY,a.NAME, a.DISCOUNT_GROUPNAME) "
		"ARCOD on ARCOD.ARCHIVE_KEY = ARC.ARCHIVE_KEY "

	 	"Where "

      " COALESCE(ARCOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
      " COALESCE(ARCOD.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and "
		"	ARC.Time_Stamp >= :StartTime and "
		"	ARC.Time_Stamp < :EndTime and "
		"	SEC.Security_Event = 'Discounted By' and "
        " ARCOD.DISCOUNTED_VALUE <>0 and "
		"	ARCB.Invoice_Number Is Not Null and "
		"	ARC.Archive_Key Is Not Null ";

	if (Locations->Count)
	{
		qrDiscountsSurchargesTotals->SQL->Text 	=	qrDiscountsSurchargesTotals->SQL->Text + "And (" +
											ParamString(Locations->Count, "ARC.Order_Location", "LocationParam") + ")";
	}
	if (Discounts->Count)
	{
		qrDiscountsSurchargesTotals->SQL->Text 	=	qrDiscountsSurchargesTotals->SQL->Text + "And (" +
											ParamString(Discounts->Count, "ARCOD.NAME", "DiscParam") + ")";
	}

	qrDiscountsSurchargesTotals->SQL->Text        = qrDiscountsSurchargesTotals->SQL->Text +
      "Group by "
         "ARCOD.NAME ";
		"Order By "
			"ARCOD.NAME asc ";

   // setting parameters
   for (int i=0; i<Discounts->Count; i++)
	{
		qrDiscountsSurchargesTotals->ParamByName("DiscParam" + IntToStr(i))->AsString = Discounts->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrDiscountsSurchargesTotals->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}

	qrDiscountsSurchargesTotals->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscountsSurchargesTotals->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDiscountedItemsDetails(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
	qrDiscounts->Close();
	qrDiscounts->SQL->Text =
	 	"Select "
			"Archive.Time_Stamp, "
			"Security.Terminal_Name, "
			"Extract(Day From Archive.Time_Stamp) Bill_Day, "
            "Extract(Month From ARCHIVE.Time_Stamp) Bill_Month, "
			"Extract(Year From ARCHIVE.Time_Stamp) Bill_Year, "
			"Security.Note, "
			"Cast(ARCORDERDISCOUNTS.NAME As Varchar(50)) Discount_Name, "
			"Contacts.Name, "
			"ArcBill.Invoice_Number, "

             "Cast( ((sum(Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)  "
            "  ))) as Numeric(17,4)) Total, "
		    "cast((sum(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(Archive.DISCOUNT_WITHOUT_TAX/Archive.DISCOUNT)))as numeric(17, 4))  Discount,  "
            "cast((sum(ARCORDERDISCOUNTS.DISCOUNTED_VALUE-(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(Archive.DISCOUNT_WITHOUT_TAX/Archive.DISCOUNT))))as numeric(17, 4))  AS DiscountTax ,  "
            //" Archive.TAX_ON_DISCOUNT DiscountTax, "
			"cast(round(sum(ARCORDERDISCOUNTS.DISCOUNTED_VALUE),2) as numeric(17,4))DiscountAmount, "
			"ARCBILL.ArcBill_Key,  "
			"Cast(Archive.Size_Name As VarChar(30)) Size_Name, "
			"Cast(Archive.ITEM_NAME As VarChar(50)) Item_Name,  "
		 // "Cast(((sum(Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0) - coalesce(Archive.TAX_ON_DISCOUNT,0)) )) as Numeric(17,4)) Price, "
         "Cast((sum(round(coalesce (Archive.QTY * Archive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price,  "

			"Menu.Menu_Type,"
			"cast(Archive.Order_Location as Varchar(25)) Order_Location, "
            "coalesce(cast((CASE WHEN MENU.MENU_TYPE = 0 THEN round(sum(Archive.DISCOUNT),2) END) as numeric(17, 4)),0) AS Food_Menu_Total ,  "
            " coalesce(cast((CASE WHEN MENU.MENU_TYPE = 1 THEN  round(sum(Archive.DISCOUNT),2) END) as numeric(17, 4)),0) AS Beverages_Menu_Total "
        //    "Cast(0 as Numeric(17,4)) PriceTotalByLocation,  "
         //   "Cast( 0 as Numeric(17,4)) TotalByLocation "

		"From "
			"Security Left Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
			"Left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
   " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME, a.NAME  "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME, a.NAME ) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
			"Left Join Menu On "
				"Archive.Menu_Name = Menu.Menu_Name "
"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

		"Where "

          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
               "Archive.DISCOUNT_WITHOUT_TAX<>0 and "

			"Archive.Time_Stamp >= :StartTime and "
			"Archive.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Discounted By' And "
			"ArcBill.Invoice_Number Is Not Null and "
			"Archive.Archive_Key Is Not Null ";
if (Locations->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}

	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +


      " group by "
            "Archive.Time_Stamp, "
			"Security.Terminal_Name, "
			"Security.Note,  "
			"ARCORDERDISCOUNTS.NAME ,  "
			"Contacts.Name,  "
			"ARCBILL.Invoice_Number,  "
			"ARCBILL.ArcBill_Key,  "
		    "ARCHIVE.Size_Name , "
			"ARCHIVE.ITEM_NAME ,  "
	    	"MENU.MENU_TYPE, "
			"Archive.Order_Location , Archive.TAX_ON_DISCOUNT "

		"Union All "

		"Select "
			"Dayarchive.Time_Stamp, "
			"Security.Terminal_Name, "
			"Extract(Day From Dayarchive.Time_Stamp) Bill_Day, "
            "Extract(Month From DAYARCHIVE.Time_Stamp) Bill_Month, "
			"Extract(Year From DAYARCHIVE.Time_Stamp) Bill_Year, "
			"Security.Note, "
			"Cast(DAYARCORDERDISCOUNTS.NAME As Varchar(50)) Discount_Name, "
			"Contacts.Name, "
			"DAYARCBILL.Invoice_Number, "

            "Cast(((sum(DAYARCHIVE.QTY * DAYARCHIVE.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0) "
            "  ))) as Numeric(17,4)) Total, "
        //     "DAYARCHIVE.Archive_Key, "
		    "cast((sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE*(DAYARCHIVE.DISCOUNT_WITHOUT_TAX/DAYARCHIVE.DISCOUNT)))as numeric(17, 4))  Discount,  "
            "cast((sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE-(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE*(DAYARCHIVE.DISCOUNT_WITHOUT_TAX/DAYARCHIVE.DISCOUNT))))as numeric(17, 4))  AS DiscountTax ,  "
           //  "DayArchive.TAX_ON_DISCOUNT DiscountTax, "
			"cast(round(sum(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE)) as numeric(17,4))DiscountAmount, "
			"DAYARCBILL.ArcBill_Key,  "
			"Cast(DAYARCHIVE.Size_Name As VarChar(30)) Size_Name, "
			"Cast(DAYARCHIVE.ITEM_NAME As VarChar(50)) Item_Name,  "
		 // "Cast(((sum(DayArchive.QTY * DayArchive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0) - coalesce(DayArchive.TAX_ON_DISCOUNT,0) ) )) as Numeric(17,4)) Price, "
            "Cast((sum(round(coalesce (DayArchive.QTY * DayArchive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price,  "

			"MENU.MENU_TYPE,  "
			"cast(DayArchive.Order_Location as Varchar(25)) Order_Location,  "
            " coalesce(cast((CASE WHEN MENU.MENU_TYPE = 0 THEN round(sum(DayArchive.DISCOUNT),2) END) as numeric(17, 4)),0) AS Food_Menu_Total , "
            "coalesce(cast((CASE WHEN MENU.MENU_TYPE = 1 THEN  round(sum(DayArchive.DISCOUNT),2) END) as numeric(17, 4)),0) AS Beverages_Menu_Total  "
           // "Cast(0 as Numeric(17,4)) PriceTotalByLocation,  "
          //  "Cast( 0 as Numeric(17,4)) TotalByLocation "
		"From "
			"Security Left Join DAYARCBILL On "
				"Security.Security_Ref = DAYARCBILL.Security_Ref "
			"Left Join Contacts On "
				"Security.User_Key = Contacts.Contacts_Key "
           " left join DAYarchive on DAYarchive.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY   "
      		"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME, a.NAME  "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME, a.NAME ) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
        "Left Join Menu On  "
				"DAYARCHIVE.Menu_Name = Menu.Menu_Name "
                "LEFT JOIN (  "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
						"FROM DAYARCORDERTAXES a   "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  DAYARCORDERTAXES     "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )  "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
                "  LEFT JOIN (  "
					 "SELECT DAOD.ARCORDERDISCOUNTS_KEY,DAOD.ARCHIVE_KEY, cast(1 as int) keyvalue  "
                            "FROM DAYARCORDERDISCOUNTS DAOD "
                           " GROUP by DAOD.ARCORDERDISCOUNTS_KEY,DAOD.ARCHIVE_KEY) DAOD on DayArchive.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "

			"Where "

         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
         " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
             "Dayarchive.DISCOUNT_WITHOUT_TAX<>0 and "
			"Dayarchive.Time_Stamp >= :StartTime and "
			"Dayarchive.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Discounted By' and "
			"DAYARCBILL.Invoice_Number Is Not Null And "
            "DayArchive.Archive_Key Is Not Null ";
if (Locations->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}

	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +

       " group by "
            "Dayarchive.Time_Stamp, "
			"Security.Terminal_Name, "
			"Security.Note,  "
			"DAYARCORDERDISCOUNTS.NAME ,  "
			"Contacts.Name,  "
			"DAYARCBILL.Invoice_Number,  "
			"DAYARCBILL.ArcBill_Key,  "
		    "DAYARCHIVE.Size_Name , "
			"DAYARCHIVE.ITEM_NAME ,  "
	    	"MENU.MENU_TYPE, "
			"DayArchive.Order_Location,DayArchive.TAX_ON_DISCOUNT "
         //   "DayArchive.DISCOUNT "

		"Order By "
			"19, 1,14";
//			"Security.Time_Stamp, "
//			"DAYARCBILL.ArcBill_Key ";

for (int i=0; i<Locations->Count; i++)
	{
		qrDiscounts->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}

	qrDiscounts->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscounts->ParamByName("EndTime")->AsDateTime		= EndTime;


    qrDiscountGrandTotal->Close();
	qrDiscountGrandTotal->SQL->Text =

         "Select   "
          "Cast(sum((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))) as Numeric(17,4)) Total, "
          "cast(sum(Archive.DISCOUNT_WITHOUT_TAX) as numeric(17, 2)) Discount,  "
          "cast(sum(Archive.TAX_ON_DISCOUNT) as numeric(17, 2)) DiscountTax,  "
          "	cast((sum(Archive.DISCOUNT)) as numeric(17,4))DiscountAmount,  "
       //  "Cast(sum((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0) - coalesce(Archive.TAX_ON_DISCOUNT,0) )) as Numeric(17,4)) Price "
       "Cast((sum(round(coalesce (Archive.QTY * Archive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price  "

   " From "
        "ARCBILL  "
        "Left Join Archive On   "
			   "	ArcBill.ArcBill_Key = Archive.ArcBill_Key  "
               " 	left join (SELECT a.SECURITY_REF, a.FROM_VAL  "
              "FROM SECURITY a where a.SECURITY_EVENT='Discounted By' )SECURITY  on SECURITY.SECURITY_REF=Archive.SECURITY_REF  "
        "LEFT JOIN (  "
				   "	 SELECT    "
					 "	ARCORDERTAXES.ARCHIVE_KEY,  "
					 "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
					 "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
					  "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				 " FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
				   "		Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
					"	FROM ARCORDERTAXES a     "
					"	group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
					"	order by 1 )  ARCORDERTAXES   "
					"	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )  "
					"	AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY  "

      " Where ARCBILL.ARCBILL_KEY  in (Select     ARCBILL.ARCBILL_KEY from ARCBILL left join SECURITY on  SECURITY.SECURITY_REF=ArcBill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and  "
           "   ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where "
            "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
           "   COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') and  ARCHIVE.DISCOUNT_WITHOUT_TAX<>0 AND "
			"Archive.Time_Stamp >= :StartTime and "
			"Archive.Time_Stamp < :EndTime  " ;
	if (Locations->Count)
	{
		qrDiscountGrandTotal->SQL->Text 	= 	qrDiscountGrandTotal->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}



    qrDiscountGrandTotal->SQL->Text        = qrDiscountGrandTotal->SQL->Text +
		"Union All "

          		"Select "
           " Cast(((sum(DAYARCHIVE.QTY * DAYARCHIVE.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ "
            " COALESCE(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0)))) as Numeric(17,4)) Total, "
            " cast((sum(DayArchive.DISCOUNT_WITHOUT_TAX))as numeric(17, 2))  Discount,  "
           " cast((sum(DayArchive.TAX_ON_DISCOUNT))as numeric(17, 2))  AS DiscountTax ,  "
		   "	cast((sum(DayArchive.DISCOUNT)) as numeric(17,4))DiscountAmount,  "
	  	  // "Cast(((sum(DayArchive.QTY * DayArchive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)- coalesce(DayArchive.TAX_ON_DISCOUNT,0)) )) as Numeric(17,4)) Price "
            "Cast((sum(round(coalesce (DayArchive.QTY * DayArchive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price  "
		"From "
        "DAYARCBILL "

       " Left Join DayArchive On  "
				"DayArcbill.ArcBill_Key = DayArchive.ARCBILL_KEY  "
                "LEFT JOIN ( "
					 "SELECT  "
					   "	DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
					   "	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
					   "	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				 " FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,  "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

       "Where DayArcbill.ARCBILL_KEY  in (Select     DayArcbill.ARCBILL_KEY from DayArcbill left join SECURITY on  SECURITY.SECURITY_REF=DayArcbill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and "
              "DayArchive.ARCHIVE_KEY not in (Select DAYARCORDERDISCOUNTS.ARCHIVE_KEY from DAYARCORDERDISCOUNTS  where    "
              "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
             " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary')  AND  DAYARCHIVE.DISCOUNT_WITHOUT_TAX<>0 AND "
		"	Dayarchive.Time_Stamp >= :StartTime and "
		"	Dayarchive.Time_Stamp < :EndTime  ";

    qrDiscountGrandTotal->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscountGrandTotal->ParamByName("EndTime")->AsDateTime		= EndTime;




}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDiscountedItemsSummary(TDateTime StartTime, TDateTime EndTime, TStrings *Discounts, TStrings *Locations)
{
	qrDiscounts->Close();
	qrDiscounts->SQL->Text =
		"Select "
			"Cast(Security.From_Val As Varchar(50)) Discount_Name, "
			"cast(round(Archive.DISCOUNT_WITHOUT_TAX,2) as numeric(17, 2)) Discount, "
            "cast(round(Archive.TAX_ON_DISCOUNT,2) as numeric(17, 2)) DiscountTax, "
			"Cast(Archive.Size_Name As VarChar(30)) Size_Name,"
			"Cast(Archive.Item_Name As VarChar(50)) Item_Name,"
	 	  //   "Cast(((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)- coalesce(Archive.TAX_ON_DISCOUNT,0) )) as Numeric(17,4)) Price,  "
              "Cast(((round(coalesce (Archive.QTY * Archive.PRICE_INCL,0),2) )) as Numeric(17,4)) Price,  "

            "Cast(((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))) as Numeric(17,4)) Total, "

			"cast(Archive.Order_Location as Varchar(25)) Order_Location,"
			"cast((Archive.Cost * Archive.Qty) as numeric(17, 2)) Cost,"
			"Archive.Qty Quantity,"
			"ArcCategories.Category,"
			"CategoryGroups.Name CategoryGroup "
		"From "
			" ArcBill  "
				"Left Join (	SELECT  a.SECURITY_REF, a.SECURITY_EVENT, a.FROM_VAL FROM SECURITY a where a.SECURITY_EVENT='Discounted By') SECURITY "
                " on SECURITY.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "


"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "

          "Arcbill.ARCBILL_KEY  in (Select     Arcbill.ARCBILL_KEY from ARCBILL left join SECURITY on  SECURITY.SECURITY_REF=Arcbill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and "
           "Archive.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where "
              "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or  "
              "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') and    "
			"Archive.Time_Stamp >= :StartTime and "
			"Archive.Time_Stamp < :EndTime and "
	   //		"Security.Security_Event = 'Discounted By' And "
			"ArcBill.Invoice_Number Is Not Null and "
			"Archive.Archive_Key Is Not Null and "
			"Archive.Discount != 0 and "
			"Archive.Qty != 0 ";
	if (Locations->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	if (Discounts->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Discounts->Count, "Security.From_Val", "DiscParam") + ")";
	}
	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +
		"Union All "

		"Select "
			"Cast(Security.From_Val As Varchar(50)) Discount_Name, "
			"cast(round(DAYARCHIVE.DISCOUNT_WITHOUT_TAX ,2) as numeric(17, 2)) Discount,"
            "cast(round(DAYARCHIVE.TAX_ON_DISCOUNT ,2) as numeric(17, 2)) DiscountTax,"
			"Cast(DayArchive.Size_Name As VarChar(30)) Size_Name,"
			"Cast(DayArchive.Item_Name As VarChar(50)) Item_Name,"
       //     "Cast(((DayArchive.QTY * DayArchive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0) - coalesce(DayArchive.TAX_ON_DISCOUNT,0))) as Numeric(17,4)) Price, "

            "Cast(((round(coalesce (DayArchive.QTY * DAYARCHIVE.PRICE_INCL,0),2) )) as Numeric(17,4)) Price,  "
             " Cast(((DayArchive.QTY * DAYARCHIVE.BASE_PRICE  +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))) as Numeric(17,4)) Total, "


			"cast(DayArchive.Order_Location as Varchar(25)) Order_Location,"
			"cast((DayArchive.Cost * DayArchive.Qty) as numeric(17, 2)) Cost,"
			"DayArchive.Qty Quantity,"
			"ArcCategories.Category,"
			"CategoryGroups.Name CategoryGroup "
		"From "
			" DayArcBill  "
				"Left Join (	SELECT  a.SECURITY_REF, a.SECURITY_EVENT, a.FROM_VAL FROM SECURITY a where a.SECURITY_EVENT='Discounted By') SECURITY "
                " on SECURITY.Security_Ref = DayArcBill.Security_Ref "
					"Left Join DayArchive On "
				"DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key "
			"Left Join ArcCategories on "
				"DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			 

"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "


         "DayArcbill.ARCBILL_KEY  in (Select     DayArcbill.ARCBILL_KEY from DAYARCBILL left join SECURITY on  SECURITY.SECURITY_REF= DayArcbill.Security_Ref where  security.SECURITY_EVENT='Discounted By') and "
              "DayArchive.ARCHIVE_KEY not in (Select DAYARCORDERDISCOUNTS.ARCHIVE_KEY from DAYARCORDERDISCOUNTS  where  "
              "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or  "
              "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') and  "
			"DayArchive.Time_Stamp >= :StartTime and "
			"DayArchive.Time_Stamp < :EndTime and "
		  //	"Security.Security_Event = 'Discounted By' And "
			"DayArcBill.Invoice_Number Is Not Null and "
			"DayArchive.Archive_Key Is Not Null and "
			"DayArchive.Discount != 0 and "
			"DayArchive.Qty != 0 ";
	if (Locations->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	if (Discounts->Count)
	{
		qrDiscounts->SQL->Text 	=	qrDiscounts->SQL->Text + "And (" +
											ParamString(Discounts->Count, "Security.From_Val", "DiscParam") + ")";
	}
	qrDiscounts->SQL->Text        = qrDiscounts->SQL->Text +
		"Order By "
			"7, 11, 10, 4, 3";
	for (int i=0; i<Discounts->Count; i++)
	{
		qrDiscounts->ParamByName("DiscParam" + IntToStr(i))->AsString = Discounts->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrDiscounts->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}

	qrDiscounts->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscounts->ParamByName("EndTime")->AsDateTime		= EndTime;
}

void TdmMMReportData::SetupCancelsBill(TDateTime StartTime, TDateTime EndTime)
{
	qrCancelsBill->Close();
	qrCancelsBill->SQL->Text =
		"Select Distinct "
			"ArcBill.ArcBill_Key,"
			"ArcBill.Invoice_Number,"
			"SecBill.Terminal_Name,"
			"SecBill.Time_Stamp,"
			"UsersBill.Name "
		"From "
			"Security SecurityArc Left Join Archive On "
				"SecurityArc.Security_Ref = Archive.Security_Ref "
			"Left Join ArcBill On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join Security SecBill On "
				"SecBill.Security_Ref = ArcBill.Security_Ref "
			"Left Join Contacts UsersBill On "
				"SecBill.User_Key = UsersBill.Contacts_Key "
      " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
         " (( "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"SecBill.Time_Stamp >= :StartTime and "
			"SecBill.Time_Stamp < :EndTime and "
			"(SecurityArc.Security_Event = 'Cancel' or SecurityArc.Security_Event = 'CancelY') And "
			"SecBill.Security_Event = 'Billed By' "
		"Order By "
			"SecBill.Time_Stamp,"
			"ArcBill.Invoice_Number ";
	qrCancelsBill->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCancelsBill->ParamByName("EndTime")->AsDateTime	= EndTime;

	qrCancelsDet->Close();
	qrCancelsDet->SQL->Text =
		"Select "
			"Archive.Archive_Key,"
			"Archive.ArcBill_Key,"
			"Archive.Table_Number,"
			"Archive.Seat_Number,"
			"Extract(Year From Archive.Time_Stamp) Bill_Year,"
			"Extract(Month From Archive.Time_Stamp) Bill_Month,"
			"Extract(Day From Archive.Time_Stamp) Bill_Day,"
			"Archive.Time_Stamp Order_Time_Stamp,"
			"Archive.Tab_Name,"
			"Archive.Size_Name,"
			"Archive.Item_Name,"
			"cast(Archive.Price * Archive.Qty as numeric(17, 4)) Price,"
			"Security.Security_Event,"
			"Security.Time_Stamp Time_Stamp1,"
			"Security.Time_Stamp Time_Stamp2,"
			"Contacts.Name "
		"From "
			"Archive "
            "Left Join Security On "
				"Archive.Security_Ref = Security.Security_Ref "
			"Left Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
        " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    " (( "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"Archive.ArcBill_Key = :ArcBill_Key "
		"Order By "
			"Archive.Tab_Name,"
			"Archive.Table_Number,"
			"Archive.Seat_Number,"
			"Archive.Time_Stamp,"
			"Archive.Item_Name,"
			"Archive.Size_Name,"
			"Archive.Archive_Key ";
}
//---------------------------------------------------------------------------
void __fastcall TdmMMReportData::qrCancelsBillAfterScroll(TDataSet *DataSet)
{
	qrCancelsDet->Close();
	qrCancelsDet->ParamByName("ArcBill_Key")->AsInteger = qrCancelsBill->FieldByName("ArcBill_Key")->AsInteger;
	qrCancelsDet->Open();
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCancels(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrCancels->Close();
	qrCancels->SQL->Text =
		"Select "
			"CanSec.Terminal_Name,"
			"CanSec.Note,"
			"Archive.Size_Name,"
			"Cast(Archive.Item_Name As Varchar(50)) Item_Name,"
			"cast(Archive.PRICE_LEVEL0 * Archive.Qty as numeric(17, 4)) Price,"
         "cast(COALESCE(AOT.VAT,0)+ COALESCE(AOT.ServiceCharge,0)+COALESCE(AOT.OtherServiceCharge,0) as numeric(17, 4)) EXCLUSIVETAX, "	 //tax
			"Extract(Day From CanSec.Time_Stamp) Credit_Day,"
			"Extract(Month From CanSec.Time_Stamp) Credit_Month,"
			"Extract(Year From CanSec.Time_Stamp) Credit_Year,"
			"CanSec.Time_Stamp Cancel_Time,"
			"CanSec.Time_Stamp Cancel_Date,"
			"CanContact.Name Cancel_Name,"
			"OrdContact.Name Order_Name,"
			"OrdSec.Time_Stamp Ordered_Date,"
			"ArcBill.Invoice_Number, "
			"Archive.Tab_Name,"
			"Archive.Table_Number,"
			"Archive.Seat_Number "
		"From "
			"Security CanSec Inner Join Archive On "
				"CanSec.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts CanContact On "
				"CanContact.Contacts_Key = CanSec.User_Key "
			"Left Join ArcBill On "
				"Archive.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts OrdContact On "
				"OrdContact.Contacts_Key = OrdSec.User_Key "
"LEFT JOIN ( "		 //join with tax
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
    " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
		    " (("
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"CanSec.Time_Stamp >= :StartTime and "
			"CanSec.Time_Stamp < :EndTime and "
			"(CanSec.Security_Event = 'Cancel' or CanSec.Security_Event = 'CancelY') and "
			"OrdSec.Security_Event = 'Ordered By' and "
			"Archive.Archive_Key Is Not Null ";
	if (Names->Count > 0)
	{
		qrCancels->SQL->Text	=	qrCancels->SQL->Text + "and (" +
										ParamString(Names->Count, "CanContact.Name", "NamesParam") + ")";
	}
	qrCancels->SQL->Text		=	qrCancels->SQL->Text +

		"Union All "

		"Select "
			"CanSec.Terminal_Name,"
			"CanSec.Note,"
			"DayArchive.Size_Name,"
			"Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,"
			"cast(DayArchive.PRICE_LEVEL0 * DayArchive.Qty as numeric(17, 4)) Price,"
         "cast(COALESCE(AOT.VAT,0)+ COALESCE(AOT.ServiceCharge,0)+COALESCE(AOT.OtherServiceCharge,0) as numeric(17, 4)) EXCLUSIVETAX, "		//tax
			"Extract(Day From CanSec.Time_Stamp) Credit_Day,"
			"Extract(Month From CanSec.Time_Stamp) Credit_Month,"
			"Extract(Year From CanSec.Time_Stamp) Credit_Year,"
			"CanSec.Time_Stamp Cancel_Time,"
			"CanSec.Time_Stamp Cancel_Date,"
			"CanContact.Name Cancel_Name,"
			"OrdContact.Name Order_Name,"
			"OrdSec.Time_Stamp Ordered_Date,"
			"DayArcBill.Invoice_Number, "
			"DayArchive.Tab_Name,"
			"DayArchive.Table_Number,"
			"DayArchive.Seat_Number "
		"From "
			"Security CanSec Inner Join DayArchive On "
				"CanSec.Security_Ref = DayArchive.Security_Ref "
			"Inner Join Contacts CanContact On "
				"CanContact.Contacts_Key = CanSec.User_Key "
			"Left Join DayArcBill On "
				"DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts OrdContact On "
				"OrdContact.Contacts_Key = OrdSec.User_Key "
	"LEFT JOIN ( "			//join with tax
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
		    " (("
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"CanSec.Time_Stamp >= :StartTime and "
			"CanSec.Time_Stamp < :EndTime and "
			"(CanSec.Security_Event = 'Cancel' or CanSec.Security_Event = 'CancelY') and "
			"OrdSec.Security_Event = 'Ordered By' and "
			"DayArchive.Archive_Key Is Not Null ";
	if (Names->Count > 0)
	{
		qrCancels->SQL->Text	=	qrCancels->SQL->Text + "and (" +
										ParamString(Names->Count, "CanContact.Name", "NamesParam") + ")";
	}
	qrCancels->SQL->Text		=	qrCancels->SQL->Text +

		"Union All "

		"Select "
			"CanSec.Terminal_Name,"
			"CanSec.Note,"
			"Orders.Size_Name,"
			"Cast(Orders.Item_Name As Varchar(50)) Item_Name,"
			"cast(Orders.PRICE_LEVEL0 * Orders.Qty as numeric(17, 4)) Price,"
            "cast(Orders.CANCEL_FINAL_PRICE * Orders.Qty - Orders.BASE_PRICE * Orders.Qty as numeric(17, 4)) as EXCLUSIVETAX, "
			"Extract(Day From CanSec.Time_Stamp) Credit_Day,"
			"Extract(Month From CanSec.Time_Stamp) Credit_Month,"
			"Extract(Year From CanSec.Time_Stamp) Credit_Year,"
			"CanSec.Time_Stamp Cancel_Time,"
			"CanSec.Time_Stamp Cancel_Date,"
			"CanContact.Name Cancel_Name,"
			"OrdContact.Name Order_Name,"
			"OrdSec.Time_Stamp Ordered_Date,"
			"Cast('[None]' As VarChar(50)) Invoice_Number, "
			"Orders.Tab_Name,"
			"Orders.Table_Number,"
			"Orders.SeatNo "
		"From "
			"Security CanSec Left Join Orders On "
				"CanSec.Security_Ref = Orders.Security_Ref "
			"Left Join Contacts CanContact On "
				"CanContact.Contacts_Key = CanSec.User_Key "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = Orders.Security_Ref "
			"Left Join Contacts OrdContact On "
				"OrdContact.Contacts_Key = OrdSec.User_Key "
		"Where "
			"CanSec.Time_Stamp >= :StartTime and "
			"CanSec.Time_Stamp < :EndTime and "
			"(CanSec.Security_Event = 'Cancel' or CanSec.Security_Event = 'CancelY') And "
			"OrdSec.Security_Event = 'Ordered By' and "
			"Orders.Order_Key Is Not Null ";
	if (Names->Count > 0)
	{
		qrCancels->SQL->Text	=	qrCancels->SQL->Text + "and (" +
										ParamString(Names->Count, "CanContact.Name", "NamesParam") + ")";
	}
	qrCancels->SQL->Text		=	qrCancels->SQL->Text +
		"Order By "
			"9, 4";
	qrCancels->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCancels->ParamByName("EndTime")->AsDateTime	= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrCancels->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCredits(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
	qrCredits->Close();
	qrCredits->SQL->Text =
		"Select "
			"Security.Terminal_Name,"
			"Security.Note,"
			"Archive.Size_Name,"
			"Cast(Archive.Item_Name As Varchar(50)) Item_Name,"
		 //	"cast(Archive.Qty * Archive.Price + Archive.Discount as numeric(17, 4)) Price,"
		 //	"cast(Archive.Price * Archive.Qty + Archive.Discount as numeric(17, 4)) Total_Price,"
 " Cast((Archive.QTY * Archive.BASE_PRICE +COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0) + COALESCE(abs(AOT.ProfitTax),0) + COALESCE(abs(AOT.LocalTax),0)  - COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) Total_Price, "
    " Cast((Archive.QTY * Archive.BASE_PRICE +COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0) + COALESCE(abs(AOT.ProfitTax),0) + COALESCE(abs(AOT.LocalTax),0)  - COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) Price, "

			"Extract(Day From Archive.TIME_STAMP_BILLED) Credit_Day,"
			"Extract(Month From Archive.TIME_STAMP_BILLED) Credit_Month,"
			"Extract(Year From Archive.TIME_STAMP_BILLED) Credit_Year,"
            "Extract(Hour From Archive.TIME_STAMP_BILLED) Credit_Hour,"
			"Archive.TIME_STAMP_BILLED,"
			"Contacts.Name User_Name "
		"From "
			"Security Inner Join Archive On "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "

" LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 5 THEN ARCORDERTAXES.TAX_VALUE END) AS ProfitTax, "
                        "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LocalTax "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"(Security.Security_Event = 'Credit' or Security.Security_Event = 'WriteOff') ";
		if (Locations->Count > 0)
		{
			qrCredits->SQL->Text	=	qrCredits->SQL->Text + "and (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationsParam") + ")";
		}
		qrCredits->SQL->Text		=	qrCredits->SQL->Text +
		"Union All "

		"Select "
			"Security.Terminal_Name,"
			"Security.Note,"
			"DayArchive.Size_Name,"
			"Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,"
		 //	"cast(DayArchive.Price * DayArchive.Qty + DayArchive.Discount as numeric(17, 4)) Price,"
		 //	"cast(DayArchive.Price * DayArchive.Qty + DayArchive.Discount as numeric(17, 4)) Total_Price,"
    " Cast((DayArchive.QTY * DAYARCHIVE.BASE_PRICE  + COALESCE(abs(AOT.VAT),0)+COALESCE( abs(AOT.ServiceCharge),0) + COALESCE( abs(AOT.OtherServiceCharge),0) + COALESCE(abs(AOT.ProfitTax),0) + COALESCE(abs(AOT.LocalTax),0)   - COALESCE((DayArchive.DISCOUNT_WITHOUT_TAX),0)) as Numeric(17,4)) Price, "
      " Cast((DayArchive.QTY * DAYARCHIVE.BASE_PRICE  + COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE( abs(AOT.OtherServiceCharge),0) + COALESCE(abs(AOT.ProfitTax),0) + COALESCE(abs(AOT.LocalTax),0) - COALESCE((DayArchive.DISCOUNT_WITHOUT_TAX),0)) as Numeric(17,4)) Total_Price, "

			"Extract(Day From DayArchive.TIME_STAMP_BILLED) Credit_Day,"
			"Extract(Month From DayArchive.TIME_STAMP_BILLED) Credit_Month,"
			"Extract(Year From DayArchive.TIME_STAMP_BILLED) Credit_Year,"
            "Extract(Hour From DayArchive.TIME_STAMP_BILLED) Credit_Hour,"
			"DayArchive.TIME_STAMP_BILLED,"
			"Contacts.Name User_Name "
		"From "
			"Security Inner Join DayArchive On "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge, "
                        "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 5 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ProfitTax, "
                        "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 4 THEN DAYARCORDERTAXES.TAX_VALUE END) AS LocalTax "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
			"(Security.Security_Event = 'Credit' or Security.Security_Event = 'WriteOff') ";
		if (Locations->Count > 0)
		{
			qrCredits->SQL->Text	=	qrCredits->SQL->Text + "and (" +
											ParamString(Locations->Count, "DayArchive.Order_Location", "LocationsParam") + ")";
		}
		qrCredits->SQL->Text		=	qrCredits->SQL->Text +

		"Order By "
			"11, 4";

	for (int i=0; i<Locations->Count; i++)
	{
		qrCredits->ParamByName("LocationsParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	qrCredits->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCredits->ParamByName("EndTime")->AsDateTime	= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupPriceAdjustments(TDateTime StartTime, TDateTime EndTime,TStrings *Locations, TStrings *Names, TStrings *Invoices)
{
	qrPriceAdjust->Close();
	qrPriceAdjust->SQL->Text =
		"Select "
			"ArcBill.Invoice_Number,"
			"SecOrder.Time_Stamp Date_Ordered,"
			"SecOrder.Time_Stamp Time_Ordered,"
			"ConOrder.Name Ordered_By,"
			"Archive.Size_Name,"
			"Cast(Archive.Item_Name as Varchar(50)) Item_Name,"
			"Archive.Order_Type,"

			"cast(Archive.BASE_PRICE * Archive.Qty as numeric(17, 4)) Price,"
			"cast( Archive.BASE_PRICE * Archive.Qty- Archive.Price_Level0 * Archive.Qty as numeric(17, 4)) as Total,"
            "cast(Archive.Price_Level0 * Archive.Qty as  numeric(17, 4)) as Price_Level,"
			"SecAdjust.Time_Stamp Date_Adjusted,"
			"SecAdjust.Time_Stamp Time_Adjusted,"
			"Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,"
			"Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,"
			"Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,"
			"SecAdjust.Terminal_Name,"
			"ConAdjust.Name Adjusted_By, "
			"Archive.Order_Location "
		"From "
			"Security SecOrder Left Join Archive On SecOrder.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join ArcBill On Archive.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On Archive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
      "Left JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	  "Where "
		    "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and  "

 "ARCHIVE.PRICE<>0 and "
	"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp < :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"Archive.Price <> Archive.Price_Level0 and "
			"Archive.Order_Type = 0 and Archive.HAPPY_HOUR = 'F' ";
	if (Locations->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Locations->Count, "Archive.Order_Location", "LocationsParam") + ")";
	}

	if (Names->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Names->Count, "ConAdjust.Name", "NamesParam") + ")";
	}
	if (Invoices->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoicesParam") + ")";
	}
	qrPriceAdjust->SQL->Text		=	qrPriceAdjust->SQL->Text +

        " Union All "


		"Select "
			"ArcBill.Invoice_Number,"
			"SecOrder.Time_Stamp Date_Ordered,"
			"SecOrder.Time_Stamp Time_Ordered,"
			"ConOrder.Name Ordered_By,"
			"Archive.Size_Name,"
			"Cast(Archive.Item_Name as Varchar(50)) Item_Name,"
			"Archive.Order_Type,"
			"cast(Archive.BASE_PRICE * Archive.Qty as numeric(17, 4)) Price,"
			"cast(  Archive.BASE_PRICE * Archive.Qty - Archive.Price_Level0 * Archive.Qty  as numeric(17, 4)) as Total,"
            "cast(Archive.Price_Level0 * Archive.Qty as  numeric(17, 4)) as Price_Level,"
			"SecAdjust.Time_Stamp Date_Adjusted,"
			"SecAdjust.Time_Stamp Time_Adjusted,"
			"Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,"
			"Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,"
			"Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,"
			"SecAdjust.Terminal_Name,"
			"ConAdjust.Name Adjusted_By, "
			"Archive.Order_Location "
		"From "
			"Security SecOrder Left Join Archive On SecOrder.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join ArcBill On Archive.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On Archive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
    "Left  JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
		     "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  "

 "ARCHIVE.PRICE<>0 and "
	"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp < :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"Archive.Price <> Archive.Price_Level1 and "
			"Archive.Order_Type = 0  and Archive.HAPPY_HOUR = 'T' ";
	if (Locations->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Locations->Count, "Archive.Order_Location", "LocationsParam") + ")";
	}

	if (Names->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Names->Count, "ConAdjust.Name", "NamesParam") + ")";
	}
	if (Invoices->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Invoices->Count, "ArcBill.Invoice_Number", "InvoicesParam") + ")";
	}
	qrPriceAdjust->SQL->Text		=	qrPriceAdjust->SQL->Text +

		" Union All "

		"Select "
			"DayArcBill.Invoice_Number,"
			"SecOrder.Time_Stamp Date_Ordered,"
			"SecOrder.Time_Stamp Time_Ordered,"
			"ConOrder.Name Ordered_By,"
			"DayArchive.Size_Name,"
			"Cast(DayArchive.Item_Name as Varchar(50)) Item_Name,"
			"DayArchive.Order_Type,"

            	"cast(DayArchive.BASE_PRICE * DayArchive.Qty as numeric(17, 4)) Price,"
			"cast( DayArchive.BASE_PRICE * DayArchive.Qty -DayArchive.Price_Level0 * DayArchive.Qty as numeric(17, 4)) as Total,"
            "cast(DayArchive.Price_Level0 * DayArchive.Qty as  numeric(17, 4)) as Price_Level,"
			"SecAdjust.Time_Stamp Date_Adjusted,"
			"SecAdjust.Time_Stamp Time_Adjusted,"
			"Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,"
			"Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,"
			"Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,"
			"SecAdjust.Terminal_Name,"
			"ConAdjust.Name Adjusted_By, "
			"DayArchive.Order_Location "
		"From "
			"Security SecOrder Left Join DayArchive On SecOrder.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join DayArcBill On DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On DayArchive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
	"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
		            "  COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and  "

 "DAYARCHIVE.PRICE<>0 and "


			"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp < :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"DayArchive.Price <> DayArchive.Price_Level0 and "
			"DayArchive.Order_Type = 0 and DayArchive.HAPPY_HOUR = 'F' ";
	if (Locations->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Locations->Count, "DayArchive.Order_Location", "LocationsParam") + ")";
	}
	if (Names->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Names->Count, "ConAdjust.Name", "NamesParam") + ")";
	}
	if (Invoices->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Invoices->Count, "DayArcBill.Invoice_Number", "InvoicesParam") + ")";
	}

        " Union All "

		"Select "
			"DayArcBill.Invoice_Number,"
			"SecOrder.Time_Stamp Date_Ordered,"
			"SecOrder.Time_Stamp Time_Ordered,"
			"ConOrder.Name Ordered_By,"
			"DayArchive.Size_Name,"
			"Cast(DayArchive.Item_Name as Varchar(50)) Item_Name,"
			"DayArchive.Order_Type,"
	"cast(DayArchive.BASE_PRICE * DayArchive.Qty as numeric(17, 4)) Price,"
			"cast( DayArchive.BASE_PRICE * DayArchive.Qty - DayArchive.Price_Level0 * DayArchive.Qty as numeric(17, 4)) as Total,"
            "cast(DayArchive.Price_Level0 * DayArchive.Qty as  numeric(17, 4)) as Price_Level,"

			"SecAdjust.Time_Stamp Date_Adjusted,"
			"SecAdjust.Time_Stamp Time_Adjusted,"
			"Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,"
			"Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,"
			"Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,"
			"SecAdjust.Terminal_Name,"
			"ConAdjust.Name Adjusted_By, "
			"DayArchive.Order_Location "
		"From "
			"Security SecOrder Left Join DayArchive On SecOrder.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key "
			"Left Join DayArcBill On DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key "
			"Left Join Security SecAdjust On DayArchive.Security_Ref = SecAdjust.Security_Ref "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key "
		"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
		         "  COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
           " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and  "

 "DAYARCHIVE.PRICE<>0 and "
	"SecOrder.Time_Stamp >= :StartTime and "
			"SecOrder.Time_Stamp < :EndTime and "
			"SecOrder.Security_Event = 'Ordered By' and "
			"SecAdjust.Security_Event = 'Price Adjust' and "
			"DayArchive.Price <> DayArchive.Price_Level1 and "
			"DayArchive.Order_Type = 0 and DayArchive.HAPPY_HOUR = 'T' ";
	if (Locations->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Locations->Count, "DayArchive.Order_Location", "LocationsParam") + ")";
	}
	if (Names->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Names->Count, "ConAdjust.Name", "NamesParam") + ")";
	}
	if (Invoices->Count > 0)
	{
		qrPriceAdjust->SQL->Text	=	qrPriceAdjust->SQL->Text + "and (" +
												ParamString(Invoices->Count, "DayArcBill.Invoice_Number", "InvoicesParam") + ")";
	}

	qrPriceAdjust->SQL->Text		=	qrPriceAdjust->SQL->Text +
		" Order By "
			"18,11,17,5";

	qrPriceAdjust->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrPriceAdjust->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Locations->Count; i++)
	{
		qrPriceAdjust->ParamByName("LocationsParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Names->Count; i++)
	{
		qrPriceAdjust->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Invoices->Count; i++)
	{
		qrPriceAdjust->ParamByName("InvoicesParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}

    //ShowSql(qrPriceAdjust->SQL->Text);

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupAdjustments(TDateTime StartTime, TDateTime EndTime, AnsiString AdjustType)
{
	qrAdjustments->Close();
	qrAdjustments->SQL->Text =
		"Select "
			"Security.Security_Event,"
			"Security.Terminal_Name,"
			"Security.Note,"
			"Archive.Size_Name,"
			"Cast(Archive.Item_Name As Varchar(50)) Item_Name,"
			"cast(Archive.Price * Archive.Qty as numeric(17, 4)) Price,"
			 "Cast((CAST(Archive.Qty * Archive.BASE_PRICE  as Numeric(17,4))  ) as Numeric(17,4)) ProductPrice,   "
			 "cast(( COALESCE(Archive.DISCOUNT,0))as numeric(17, 4)) AS Discount, "
             "cast(( COALESCE(Archive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT,     "
             "Cast(((CAST( Archive.QTY *  Archive.BASE_PRICE   as Numeric(17,4))      ) +(  COALESCE(AOT.VAT,0) )+( COALESCE( AOT.ServiceCharge,0)) +( COALESCE( AOT.OtherServiceCharge,0))+   "
             " ( COALESCE(Archive.DISCOUNT,0))   "
         "  ) as Numeric(17,4)) Total_Price   ,   "

			"Extract(Day From Security.Time_Stamp) Adjust_Day,"
			"Extract(Month From Security.Time_Stamp) Adjust_Month,"
			"Extract(Year From Security.Time_Stamp) Adjust_Year,"
            "Extract(Hour From Security.Time_Stamp) Adjust_Hour,"
            "Extract(Minute From Security.Time_Stamp) Adjust_Minute, "
            "Extract(Second From Security.Time_Stamp) Adjust_Second, "
			"Security.Time_Stamp Time_Stamp1,"
			"Security.Time_Stamp Time_Stamp2,"
             "Cast((  COALESCE(AOT.VAT,0) +COALESCE( AOT.ServiceCharge,0)+COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) Tax, "

			"Contacts.Name User_Name, "
            "Security.FROM_VAL, "
            "Security.TO_VAL "
		"From "
			"Security Inner Join Archive On "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
        " Left join ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
        	"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "
		    " (( "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = :AdjustType "

		"Union All "

		"Select "
			"Security.Security_Event,"
			"Security.Terminal_Name,"
			"Security.Note,"
			"DayArchive.Size_Name,"
			"Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,"
			"cast(DayArchive.Price * DayArchive.Qty as numeric(17, 4)) Price,"

             "Cast((CAST (DayArchive.Qty * DayArchive.BASE_PRICE  as Numeric(17,4))) as Numeric(17,4)) ProductPrice,   "
			 "cast(( COALESCE(DayArchive.DISCOUNT,0))as numeric(17, 4)) AS Discount, "
             "cast(( COALESCE(DayArchive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT,     "
             "Cast((( CAST( DayArchive.QTY *  DayArchive.BASE_PRICE   as Numeric(17,4))  )  +(  COALESCE(AOT.VAT,0) )+( COALESCE( AOT.ServiceCharge,0)) +( COALESCE( AOT.OtherServiceCharge,0))+   "
             "( COALESCE(DayArchive.DISCOUNT,0))   "
         " ) as Numeric(17,4)) Total_Price   , "

 

			"Extract(Day From Security.Time_Stamp) Adjust_Day,"
			"Extract(Month From Security.Time_Stamp) Adjust_Month,"
			"Extract(Year From Security.Time_Stamp) Adjust_Year,"
            "Extract(Hour From Security.Time_Stamp) Adjust_Hour,"
            "Extract(Minute From Security.Time_Stamp) Adjust_Minute, "
            "Extract(Second From Security.Time_Stamp) Adjust_Second, "
			"Security.Time_Stamp Time_Stamp1,"
			"Security.Time_Stamp Time_Stamp2,"
            "Cast((  COALESCE(AOT.VAT,0) +COALESCE( AOT.ServiceCharge,0)+COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) Tax, "

			"Contacts.Name User_Name, "
            "Security.FROM_VAL, "
            "Security.TO_VAL "
		"From "
			"Security Inner Join DayArchive On "
				"Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
		" Left join DAYARCORDERDISCOUNTS on DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
        	"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DAYARCHIVE.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Where "
		    " (( "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
            " COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = :AdjustType "

		"Union All "

		"Select "
			"Security.Security_Event,"
			"Security.Terminal_Name,"
			"Security.Note,"
			"Orders.Size_Name,"
			"Cast(Orders.Item_Name As Varchar(50)) Item_Name,"
			"cast(Orders.Price * Orders.Qty as numeric(17, 4)) Price,"

               "Cast((CAST(Orders.Qty * Orders.BASE_PRICE  as Numeric(17,4)) ) as Numeric(17,4)) ProductPrice,   "
			 "cast(( COALESCE(Orders.DISCOUNT,0))as numeric(17, 4)) AS Discount, "
             "cast(( COALESCE(Orders.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT,     "
             " Cast((( CAST(Orders.QTY *  Orders.BASE_PRICE as Numeric(17,4)) ) +(  (  cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.VAT,0)/100 as numeric(17, 4))  ) )+( cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.ServiceCharge,0)/100 as numeric(17, 4)) ) +(cast((((Orders.Qty * Orders.BASE_PRICE+ COALESCE(Orders.DISCOUNT,0)))*COALESCE(AOT.ServiceCharge,0)/100)*COALESCE(STAX.ServiceChargeTax,0)/100 as numeric(17, 4)))+  "
             " ( COALESCE(Orders.DISCOUNT,0)) "
          " ) as Numeric(17,4)) Total_Price   , "

			"Extract(Day From Security.Time_Stamp) Adjust_Day,"
			"Extract(Month From Security.Time_Stamp) Adjust_Month,"
			"Extract(Year From Security.Time_Stamp) Adjust_Year,"
            "Extract(Hour From Security.Time_Stamp) Adjust_Hour,"
            "Extract(Minute From Security.Time_Stamp) Adjust_Minute, "
            "Extract(Second From Security.Time_Stamp) Adjust_Second, "
			"Security.Time_Stamp Time_Stamp1,"
			"Security.Time_Stamp Time_Stamp2,"
          "     cast ((  cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.VAT,0)/100 as numeric(17, 4))  )+ (cast(( (Orders.Qty * Orders.BASE_PRICE+ COALESCE( Orders.DISCOUNT_WITHOUT_TAX,0)))*COALESCE( AOT.ServiceCharge,0)/100 as numeric(17, 4)) ) +(cast((((Orders.Qty * Orders.BASE_PRICE+ COALESCE(Orders.DISCOUNT,0)))*COALESCE(AOT.ServiceCharge,0)/100)*COALESCE(STAX.ServiceChargeTax,0)/100 as numeric(17, 4))) as numeric(17, 4)) Tax, "
            	"Contacts.Name User_Name, "
            "Security.FROM_VAL, "
            "Security.TO_VAL "                
		"From "
			"Security Inner Join Orders On "
				"Security.Security_Ref = Orders.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
				"left join "
				"(SELECT "
                "cast(1 as int) keyvalue   , "
						 " TAXPROFILE.ORDER_KEY,  "
					  "	MIN(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT,"
					  "	MIN(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,  "
					  "	MIN(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS OtherServiceCharge   "
				" FROM (SELECT  "
                           " TFO.ORDER_KEY,  TAXPROFILES.TYPE, "
                            "Cast(Sum(COALESCE(TAXPROFILES.RATE,0) ) as Numeric(17,4)) TAX_RATE "
                            "FROM TAXPROFILES_ORDERS TFO      "
                           " left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY "
                           " group by TFO.ORDER_KEY,TAXPROFILES.TYPE   "
                           " )  TAXPROFILE  "
					  "	GROUP BY TAXPROFILE.ORDER_KEY "
				") AOT on AOT.ORDER_KEY=Orders.ORDER_KEY "
                " left join (SELECT      cast(1 as int) keyvalue   ,   "
" MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax      FROM VARSPROFILE    ) STAX on STAX.keyvalue=AOT.keyvalue  "

		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = :AdjustType " 

		"Order By "
		 	"	17 desc ";

	qrAdjustments->ParamByName("AdjustType")->AsString		= AdjustType;
	qrAdjustments->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrAdjustments->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupManualCashDrawer(TDateTime StartTime, TDateTime EndTime, TStrings *Users)
{
	qrManualCashDrawer->Close();
	qrManualCashDrawer->SQL->Text =
		"Select "
			"Security.Time_Stamp Time_Stamp1,"
			"Security.Time_Stamp Time_Stamp2,"
			"Extract (Day From Security.Time_Stamp) Drawer_Day,"
			"Extract (Month From Security.Time_Stamp) Drawer_Month,"
			"Extract (Year From Security.Time_Stamp) Drawer_Year,"
			"Contacts.Name User_Name,"
			"Security.Terminal_Name, "
            "Security.NOTE "
		"From "
			"Security Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "
		"Where "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Manually Opened Cash Drawer' ";
	if (Users->Count > 0)
	{
		qrManualCashDrawer->SQL->Text =	qrManualCashDrawer->SQL->Text + "And (" +
													ParamString(Users->Count, "Contacts.Name", "Param") + ")";
	}
	qrManualCashDrawer->SQL->Text =		qrManualCashDrawer->SQL->Text +
		"Order By "
			"1, 2, 3";
	qrManualCashDrawer->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrManualCashDrawer->ParamByName("EndTime")->AsDateTime	= EndTime;
	for (int i=0; i<Users->Count; i++)
	{
		qrManualCashDrawer->ParamByName("Param" + IntToStr(i))->AsString = Users->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyPoints(TStrings *Customers)
{
	qrLoyalty->Close();
	qrLoyalty->SQL->Text =
		"Select "
			"Name,"
			"Member_Number,"
			"Total_Spent Points_Earnt,"
            "Earnt_Points,"
            "Loaded_Points,"
			"Cast(0 As Numeric(17,4)) Points_Redeemed,"
			"Total_Spent Points_Remaining,"
			"DateOfBirth \"Birthday\" "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4) ";
	if (Customers->Count > 0)
	{
		qrLoyalty->SQL->Text =	qrLoyalty->SQL->Text + "And (" +
										ParamString(Customers->Count, "Name", "Param") + ")";
	}
	qrLoyalty->SQL->Text =		qrLoyalty->SQL->Text +
		"Order By "
			"Name";
	for (int i=0; i<Customers->Count; i++)
	{
		qrLoyalty->ParamByName("Param" + IntToStr(i))->AsString = Customers->Strings[i];
	}
}

//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesSummaryCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups) //cww
{
	// Summary by Category
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =

		"select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"Archive.Item_Name Item_Name,"
			"Archive.Size_Name,"
			"cast(sum(Archive.Points_Earned) as numeric(17, 4)) as Earned,"
			"cast(sum(Archive.Redeemed) as numeric(17, 4)) as Redeemded,"
			"cast(Sum(Archive.Qty) as numeric(17, 4)) Item_Count,"
			"cast(Sum(Archive.Price * Archive.Qty + Archive.Discount) as numeric(17, 4)) Price,"
			"cast(Sum(cast((Archive.Price + Archive.Qty * Archive.Discount) as Numeric(17,4)) ) as numeric(17, 4)) PriceExcl,"
			"cast(Sum(Archive.Cost * Archive.Qty) as numeric(17, 4)) Cost,"
			"cast(Sum(cast((Archive.Price * Archive.Qty + Archive.Discount) as Numeric(17,4)) ) - Sum(archive.qty * Archive.Cost) as numeric(17, 4)) Profit "
		"From "
			"Archive Left Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key ";
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text + "Inner Join ArcCategories on Archive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
		  //	"(Archive.Points_Earned <> 0.00 or"
		  //	"Archive.Redeemed <> 0.00) and"
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime   and "
		  //	"Security.Security_Event = 'Ordered By' and "
			"Archive.Loyalty_Key <> 0 ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		"Group By "
			"Contacts.Name,"
			"Contacts.Member_Number,"
			//"Archive.Time_Stamp,"
			//"Archive.Menu_Name,"
			//"Archive.Course_Name,"
			"Archive.Item_Name,"
			"Archive.Size_Name "
			//"Arcbill.Invoice_Number "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			//"DayArchive.Time_Stamp,"
			//"DayArchive.Menu_Name Group_Name,"
			//"DayArchive.Course_Name,"
			"DayArchive.Item_Name Item_Name,"
			"DayArchive.Size_Name,"
			//"DayArcbill.Invoice_Number,"
			"cast(sum(DayArchive.Points_Earned) as numeric(17, 4)) as Earned,"
			"cast(sum(DayArchive.Redeemed) as numeric(17, 4)) as Redeemed,"
			"cast(Sum(DayArchive.Qty) as numeric(17, 4)) Item_Count,"
			"cast(Sum(DayArchive.Price * DayArchive.Qty + DayArchive.Discount) as numeric(17, 4)) Price,"
			"cast(Sum(cast((DayArchive.Price * DayArchive.Qty + DayArchive.Discount) as Numeric(17,4))) as numeric(17, 4)) PriceExcl,"
			"cast(Sum(DayArchive.Cost * DayArchive.Qty) as numeric(17, 4)) Cost,"
			"cast(Sum(cast((DayArchive.Price * DayArchive.Qty + DayArchive.Discount) as Numeric(17,4)) ) - Sum(dayarchive.qty * DayArchive.Cost) as numeric(17, 4)) Profit "
		"From "
			//"Security Left Join DayArchive on "
			//	"Security.Security_Ref = DayArchive.Security_Ref "
			"DayArchive Left Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key ";
			//"Left Join DayArcbill on "
			//	"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key " ;
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +  "Inner Join ArcCategories on DayArchive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
			//"(DayArchive.Points_Earned <> 0.00 or"
			//"DayArchive.Redeemed <> 0.00) and"
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			//"Security.Security_Event = 'Ordered By' and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		"Group By "
			"Contacts.Name,"
			"Contacts.Member_Number,"
			//"DayArchive.Time_Stamp,"
			//"DayArchive.Menu_Name,"
			//"DayArchive.Course_Name,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name "
		  //	"DayArcbill.Invoice_Number "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,2,6";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltySales->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrLoyaltySales->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesDetailedCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups) //cww
{
	// Detailed by Category
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =
		"select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"Archive.Time_Stamp,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Arcbill.Invoice_Number,"
			"Archive.Points_Earned as Earned,"
			"Archive.Redeemed as Redeemded,"
			"Archive.Qty as Item_Count,"
			"cast(Archive.Price * Archive.Qty + Archive.Discount as numeric(17, 4)) Price,"
			"casT(Archive.Cost * Archive.Qty as numeric(17,4)) Cost "
		"From "
			"Archive Left Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join Arcbill on "
				"Archive.Arcbill_Key = Arcbill.Arcbill_Key " ;
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text + "Inner Join ArcCategories on Archive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime   and "
			"Archive.Loyalty_Key <> 0 ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"DayArchive.Time_Stamp,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArcbill.Invoice_Number,"
			"DayArchive.Points_Earned as Earned,"
			"DayArchive.Redeemed as Redeemed,"
			"DayArchive.Qty Item_Count,"
			"cast(DayArchive.Price * DayArchive.Qty + DayArchive.Discount as numeric(17,4)) Price,"
			"cast(DayArchive.Cost * dayarchive.qty as numeric(17, 4)) Cost "
		"From "
			"DayArchive Left Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join DayArcbill on "
				"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key " ;
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +  "Inner Join ArcCategories on DayArchive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,2,6,4";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltySales->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrLoyaltySales->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesDetailedCategoryExternalMember(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups) //cww
{
	// Detailed by Category
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =
		"select "
			"Contacts.CONTACTS_3RDPARTY_KEY,"
			"Cast(Contacts.CONTACTS_3RDPARTY_KEY as VarChar(10)) Member_Number,"
			"Contacts.Name,"
			"Archive.Time_Stamp_Billed,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Arcbill.Invoice_Number,"
			"Archive.Points_Earned as Earned,"
			"Archive.Redeemed as Redeemded,"
			"Archive.Qty as Item_Count,"
			"cast(Archive.Price * archive.qty + Archive.Discount as numeric(17,4)) Price,"
			"cast(Archive.Cost * archive.qty as numeric(17, 4)) Cost "
		"From "
			"Archive Left Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join Arcbill on "
				"Archive.Arcbill_Key = Arcbill.Arcbill_Key " ;
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text + "Inner Join ArcCategories on Archive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime   and "
			"Archive.Loyalty_Key <> 0 ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.CONTACTS_3RDPARTY_KEY,"
			"Cast(Contacts.CONTACTS_3RDPARTY_KEY as VarChar(10)) Member_Number,"
			"Contacts.Name,"
			"DayArchive.Time_Stamp_Billed,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArcbill.Invoice_Number,"
			"DayArchive.Points_Earned as Earned,"
			"DayArchive.Redeemed as Redeemed,"
			"DayArchive.Qty Item_Count,"
			"cast(DayArchive.Price * dayarchive.qty + DayArchive.Discount as numeric(17,4)) Price,"
			"cast(DayArchive.Cost * dayarchive.qty as numeric(17, 4)) Cost "
		"From "
			"DayArchive Left Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join DayArcbill on "
				"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key " ;
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +  "Inner Join ArcCategories on DayArchive.Category_Key = "
												"ArcCategories.Category_Key ";
	}
		qrLoyaltySales->SQL->Text = qrLoyaltySales->SQL->Text +
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(Groups->Count, "ArcCategories.Category", "GroupParam") + ")";
	}

	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,3,7,5";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltySales->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrLoyaltySales->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesSummaryItem(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList )
{
	// Summary by Item and Size
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	std::auto_ptr<TStringList> ItemNamesList(new TStringList);
	std::auto_ptr<TStringList> SizeNamesList(new TStringList);
	if (ItemList && ItemList->Count > 0)
	{
		//Items are concatenated together for Selection eg DB - 7oz
		SplitItemsAndSizes(ItemList,ItemNamesList.get(),SizeNamesList.get());
	}
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =
		"select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"Archive.Item_Name Item_Name,"
			"Archive.Size_Name,"
			"cast(sum(Archive.Points_Earned) as numeric(17, 4)) as Earned,"
			"cast(sum(Archive.Redeemed) as numeric(17, 4)) as Redeemded,"
			"cast(Sum(Archive.Qty) as numeric(17, 4)) Item_Count,"
			//"Sum(Archive.Price) Price,"
			//"Sum(Archive.Cost) Cost "

			"cast(Sum(Archive.Price * archive.qty ) as numeric(17, 4)) Price,"
			// "cast(Sum((Archive.Price * archive.qty  + Archive.Discount) * 100 / (Archive.GST_Percent + 100)) as numeric(17, 4)) PriceExcl,"
            "Cast(Sum(Archive.Qty * Archive.BASE_PRICE ) as Numeric(17,4)) PriceExcl, "	 //price excl
			"cast(Sum(Archive.Cost * archive.qty) as numeric(17, 4)) Cost,"

 "Cast(Sum(Archive.Qty * Archive.BASE_PRICE ) - Sum(archive.qty * Archive.Cost) as Numeric(17,4)) Profit "	 //price excl

		 //	"cast(Sum((Archive.Price * archive.qty ) ) - Sum(archive.qty * Archive.Cost) as numeric(17, 4)) Profit "

		"From "
			"Archive Inner Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "Archive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "Archive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		"Group By "
			"Contacts.Name,"
			"Contacts.Member_Number,"
			"Archive.Item_Name,"
			"Archive.Size_Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"DayArchive.Item_Name Item_Name,"
			"DayArchive.Size_Name,"
			"cast(sum(DayArchive.Points_Earned) as numeric(17, 4)) as Earned,"
			"cast(sum(DayArchive.Redeemed) as numeric(17, 4)) as Redeemed,"
			"cast(Sum(DayArchive.Qty) as numeric(17, 4)) Item_Count,"
			"cast(Sum(DayArchive.Qty * DayArchive.Price  ) as numeric(17, 4)) Price,"
			//"cast(Sum((DayArchive.Qty * DayArchive.Price + DayArchive.Discount) * 100 / (DayArchive.GST_Percent + 100)) as numeric(17, 4)) PriceExcl,"
              "Cast(Sum(DayArchive.Qty * DayArchive.BASE_PRICE ) as Numeric(17,4)) PriceExcl, "	 //price excl
			"cast(Sum(DayArchive.Qty * DayArchive.Cost) as numeric(17, 4)) Cost,"
"Cast(Sum(DayArchive.Qty * DayArchive.BASE_PRICE ) - Sum(DayArchive.Qty * DayArchive.Cost)  as Numeric(17,4)) Profit "	 //price excl

		  //	"cast(Sum((DayArchive.Qty * DayArchive.Price  )  ) - Sum(DayArchive.Qty * DayArchive.Cost) as numeric(17, 4)) Profit "
		"From "
			"DayArchive Inner Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			//"Left Join DayArcbill on "
			//	"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "DayArchive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "DayArchive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		"Group By "
			"Contacts.Name,"
			"Contacts.Member_Number,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,2,3,4";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

    if (Names) {
    	for (int i=0; Names && i<Names->Count; i++) {
		    qrLoyaltySales->ParamByName("NamesParam"+ IntToStr(i))->AsString =
               Names->Strings[i];
    	}
    }

	// Item and Size Param
	if (ItemList && ItemList->Count > 0)
	{
		for (int i=0; i<ItemNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("ItemNameParam" + IntToStr(i))->AsString = ItemNamesList->Strings[i];
		}
		for (int i=0; i<SizeNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("SizeNameParam" + IntToStr(i))->AsString = SizeNamesList->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesDetailedItem(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList )
{
	// Detailed by Item and Size
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	std::auto_ptr<TStringList> ItemNamesList(new TStringList);
	std::auto_ptr<TStringList> SizeNamesList(new TStringList);
	if (ItemList && ItemList->Count > 0)
	{
		//Items are concatenated together for Selection eg DB - 7oz
		SplitItemsAndSizes(ItemList,ItemNamesList.get(),SizeNamesList.get());
	}
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =
		"select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"Archive.Time_Stamp,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Arcbill.Invoice_Number,"
			"Archive.Points_Earned as Earned,"
			"Archive.Redeemed as Redeemded,"
			"Archive.Qty as Item_Count,"
			"cast(Archive.Qty * Archive.Price as numeric(17, 4)) Price,"
			"cast(Archive.Qty * Archive.Cost as numeric(17,4)) as Cost "
		"From "
			"Archive Left Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join Arcbill on "
				"Archive.Arcbill_Key = Arcbill.Arcbill_Key "
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime   and "
			"Archive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "Archive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "Archive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.Member_Number,"
			"Contacts.Name,"
			"DayArchive.Time_Stamp,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArcbill.Invoice_Number,"
			"DayArchive.Points_Earned as Earned,"
			"DayArchive.Redeemed as Redeemed,"
			"DayArchive.Qty Item_Count,"
			"cast(DayArchive.Qty * DayArchive.Price as numeric(17,4)) Price,"
			"cast(DayArchive.Qty * DayArchive.Cost as numeric(17, 4)) Cost "
		"From "
			"DayArchive Left Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join DayArcbill on "
				"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "DayArchive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "DayArchive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		 //	"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,2,6,4";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltySales->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	// Item and Size Param
	if (ItemList && ItemList->Count > 0)
	{
		for (int i=0; i<ItemNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("ItemNameParam" + IntToStr(i))->AsString = ItemNamesList->Strings[i];
		}
		for (int i=0; i<SizeNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("SizeNameParam" + IntToStr(i))->AsString = SizeNamesList->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltySalesDetailedItemExternalMember(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList )
{
	// Detailed by Item and Size
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	std::auto_ptr<TStringList> ItemNamesList(new TStringList);
	std::auto_ptr<TStringList> SizeNamesList(new TStringList);
	if (ItemList && ItemList->Count > 0)
	{
		//Items are concatenated together for Selection eg DB - 7oz
		SplitItemsAndSizes(ItemList,ItemNamesList.get(),SizeNamesList.get());
	}
	qrLoyaltySales->Close();
	qrLoyaltySales->SQL->Text =
		"select "
			"Contacts.CONTACTS_3RDPARTY_KEY,"
			"Cast(Contacts.CONTACTS_3RDPARTY_KEY as VarChar(10)) Member_Number,"
			"Contacts.Name,"
			"Archive.Time_Stamp,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Arcbill.Invoice_Number,"
			"Archive.Points_Earned as Earned,"
			"Archive.Redeemed as Redeemded,"
			"Archive.Qty as Item_Count,"
			"cast(Archive.Price * archive.qty as numeric(17, 4)) Price,"
			"cast(Archive.Cost * archive.qty as numeric(17, 4)) as Cost "
		"From "
			"Archive Left Join Contacts on "
				"Archive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join Arcbill on "
				"Archive.Arcbill_Key = Arcbill.Arcbill_Key "
		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Time_Stamp_Billed >= :StartTime and "
			"Archive.Time_Stamp_Billed <  :EndTime   and "
			"Archive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "Archive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "Archive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		//	"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Contacts.CONTACTS_3RDPARTY_KEY,"
			"Cast(Contacts.CONTACTS_3RDPARTY_KEY as VarChar(10)) Member_Number,"
			"Contacts.Name,"
			"DayArchive.Time_Stamp,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArcbill.Invoice_Number,"
			"DayArchive.Points_Earned as Earned,"
			"DayArchive.Redeemed as Redeemed,"
			"DayArchive.Qty Item_Count,"
			"cast(DayArchive.Price * dayarchive.qty as numeric(17, 4)) Price,"
			"cast(DayArchive.Cost * dayarchive.qty as numeric(17, 4)) Cost "
		"From "
			"DayArchive Left Join Contacts on "
				"DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			"Left Join DayArcbill on "
				"DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Time_Stamp_Billed >= :StartTime and "
			"DayArchive.Time_Stamp_Billed <  :EndTime and "
			"DayArchive.Loyalty_Key <> 0 ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	// Item and Size
	if (ItemList && ItemList->Count > 0)
	{
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(ItemNamesList->Count, "DayArchive.Item_Name", "ItemNameParam") + ")";
		qrLoyaltySales->SQL->Text	=	qrLoyaltySales->SQL->Text + "And (" +
													ParamString(SizeNamesList->Count, "DayArchive.Size_Name", "SizeNameParam") + ")";
	}
	qrLoyaltySales->SQL->Text		=	qrLoyaltySales->SQL->Text +
		//"Having "
		 //	"Count(DayArchive.Archive_Key) > 0 "

		"Order By "
			"1,3,7,5";

	qrLoyaltySales->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltySales->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltySales->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	// Item and Size Param
	if (ItemList && ItemList->Count > 0)
	{
		for (int i=0; i<ItemNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("ItemNameParam" + IntToStr(i))->AsString = ItemNamesList->Strings[i];
		}
		for (int i=0; i<SizeNamesList->Count; i++)
		{
			qrLoyaltySales->ParamByName("SizeNameParam" + IntToStr(i))->AsString = SizeNamesList->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyDiscountedProducts(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts, TStrings *Names) //cww
{
	/*
	  NB done off "Archive.Time_Stamp"
	  Sales time as oposed to billing time
	*/
	qrLoyaltyDiscProducts->Close();
	qrLoyaltyDiscProducts->SQL->Text =
		"Select "
			"Security.Security_Event,"
			"Contacts.Name,"
			"Archive.Time_Stamp,"
			"Archive.Menu_Name Group_Name,"
			"Archive.Course_Name,"
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Archive.Qty as Item_Count,"
			"Security.From_Val as Old_Price,"
			"Security.To_Val as New_Price,"
			"Security.Note as Discount "
		"From "
			"Security Left Join Archive on Security.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts on Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"(Archive.Order_Type = 3 or Archive.Order_Type = 0) and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			//"Security.Security_Event in ('Member Discount','Location Discount') and  "
			"Archive.Loyalty_Key <> 0 ";
	if (Discounts && Discounts->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
														ParamString(Discounts->Count, "Security.Security_Event", "DiscountsParam") + ")";
	}
	if (Names && Names->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
														ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	qrLoyaltyDiscProducts->SQL->Text		=	qrLoyaltyDiscProducts->SQL->Text +

		"Union All "

	"Select "
			"Security.Security_Event,"
			"Contacts.Name,"
			"DayArchive.Time_Stamp,"
			"DayArchive.Menu_Name Group_Name,"
			"DayArchive.Course_Name,"
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArchive.Qty as Item_Count,"
			"Security.From_Val as Old_Price,"
			"Security.To_Val as New_Price,"
			"Security.Note as Discount "
		"From "
			"Security Left Join DayArchive on Security.Security_Ref = DayArchive.Security_Ref "
			"Inner Join Contacts on DayArchive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"(DayArchive.Order_Type = 3 or DayArchive.Order_Type = 0) and "
			"DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
		  //	"Security.Security_Event in ('Member Discount','Location Discount') and  "
			"DayArchive.Loyalty_Key <> 0 ";

	if (Discounts && Discounts->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
														ParamString(Discounts->Count, "Security.Security_Event", "DiscountsParam") + ")";
	}
	if (Names->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
														ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	qrLoyaltyDiscProducts->SQL->Text		=	qrLoyaltyDiscProducts->SQL->Text +

		"Union All "

	"Select "
			"Security.Security_Event,"
			"Contacts.Name,"
			"Orders.Time_Stamp,"
			"Orders.Menu_Name Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Orders.Qty as Item_Count,"
			"Security.From_Val as Old_Price,"
			"Security.To_Val as New_Price,"
			"Security.Note as Discount "
		"From "
			"Security Left Join Orders on Security.Security_Ref = Orders.Security_Ref "
			"Inner Join Contacts on Orders.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			//"Security.Security_Event in ('Member Discount','Location Discount') and "
			"Orders.Loyalty_Key <> 0 ";
	if (Discounts && Discounts->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
														ParamString(Discounts->Count, "Security.Security_Event", "DiscountsParam") + ")";
	}
	if (Names && Names->Count > 0)
	{
		qrLoyaltyDiscProducts->SQL->Text	=	qrLoyaltyDiscProducts->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	qrLoyaltyDiscProducts->SQL->Text		=	qrLoyaltyDiscProducts->SQL->Text +

		"Order By "
			"1,2,3,4,5,6";

	qrLoyaltyDiscProducts->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyDiscProducts->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltyDiscProducts->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Discounts->Count; i++)
	{
		qrLoyaltyDiscProducts->ParamByName("DiscountsParam" + IntToStr(i))->AsString = Discounts->Strings[i];
	}
  //TestData(qrLoyaltyDiscProducts,true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyHistoryVIPCredit(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations)
{
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	// Detailed Report with VIP Credit
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"Select "
			"Archive.Order_Location,"
			"Contacts.Name,"
			"cast('' as varchar(10)) ItemName,"
			"Contacts.Total_Spent Total_Points,"
			"Sum(Archive.Price * archive.qty) Total_Spent,"
			"Sum(Archive.Points_Earned) Total_Points_Earned,"
			"Sum(Archive.Redeemed) Total_Points_Redeemed "
		"From "
			"Archive Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime And ";
			"Archive.Item_Name <> '" + CurrentConnection.VIPCreditName + "' ";
	if (Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"Archive.Order_Location,"
			"Contacts.Name,"
			"cast('VIP Credit' as varchar(10)) ItemName,"
			"Contacts.Total_Spent Total_Points,"
			"Sum(Archive.Qty * Archive.Price) Total_Spent,"
			"Sum(Archive.Points_Earned) Total_Points_Earned,"
			"Sum(Archive.Redeemed) Total_Points_Redeemed "
		"From "
			"Archive Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime And ";
			"Archive.Item_Name = '" + CurrentConnection.VIPCreditName + "' ";
	if (Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"DayArchive.Order_Location,"
			"Contacts.Name,"
			"cast('' as varchar(10)) ItemName,"
			"Contacts.Total_Spent Total_Points,"
			"Sum(DayArchive.Qty * DayArchive.Price) Total_Spent,"
			"Sum(DayArchive.Points_Earned) Total_Points_Earned,"
			"Sum(DayArchive.Redeemed) Total_Points_Redeemed "
		"From "
			"DayArchive Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime And ";
			"DayArchive.Item_Name <> '" + CurrentConnection.VIPCreditName + "' ";
	if (Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"DayArchive.Order_Location,"
			"Contacts.Name,"
			"cast('VIP Credit' as varchar(10)) ItemName,"
			"Contacts.Total_Spent Total_Points,"
			"Sum(DayArchive.Qty * DayArchive.Price) Total_Spent,"
			"Sum(DayArchive.Points_Earned) Total_Points_Earned,"
			"Sum(DayArchive.Redeemed) Total_Points_Redeemed "
		"From "
			"DayArchive Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime And "
			"DayArchive.Item_Name = '" + CurrentConnection.VIPCreditName + "' ";
	if (Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Order By "
			"1,2";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
  //TestData(qrLoyaltyHistory,true);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupStockLostRevenue(TDateTime StartTime, TDateTime EndTime)
{
    qrLostRevenue->Close();
    qrLostRevenue->SQL->Text =
        "select Writeoff.item, "
            "WRITEOFF.TIME_STAMP, "
            "WRITEOFF.REASONS, "
            "WRITEOFF.AMOUNT, "
            "WRITEOFF.SIZE_NAME, "
            "ARCCATEGORIES.CATEGORY, "
            "CATEGORYGROUPS.NAME, "
            "MENU.MENU_NAME, "
            "COURSE.COURSE_NAME "
        "from WRITEOFF "
            "left join ARCCATEGORIES on writeoff.CATEGORY_KEY = ARCCATEGORIES.CATEGORY_KEY "
            "left join CATEGORYGROUPS on WRITEOFF.GROUP_KEY = CATEGORYGROUPS.CATEGORYGROUPS_KEY "
            "left join MENU on WRITEOFF.MENU_KEY = MENU.MENU_KEY "
            "left join COURSE on WRITEOFF.COURSE_KEY = COURSE.COURSE_KEY "
        "where "
			"WRITEOFF.TIME_STAMP >= :StartTime and "
			"WRITEOFF.TIME_STAMP < :EndTime "
        "order by course_name, menu_name;";

 	qrLostRevenue->ParamByName("StartTime")->AsDateTime	= StartTime;
 	qrLostRevenue->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyHistoryLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations)
{
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	// Detailed Report ( with NO VIP Credit )
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"Select "
			"cast(Archive.Order_Location as varchar(25)) Order_Location,"
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(Archive.Price * Archive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(Archive.Points_Earned) Total_Points_Earned,"
			"Sum(Archive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"Archive Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime  ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"cast(DayArchive.Order_Location as varchar(25)) Order_Location,"
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(DayArchive.Price * DayArchive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(DayArchive.Points_Earned) Total_Points_Earned,"
			"Sum(DayArchive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"DayArchive Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime  ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"cast(ArcBill.Billed_Location as varchar(25)) Order_Location,"
			"cast('Unknown' as varchar(50)) Name,"
			"cast(0 as numeric(17, 4)) Total_Points,"
			"cast(0 as numeric(17, 4)) Total_Spent,"
			"cast(0 as numeric(17, 4))  Total_Points_Earned,"
			"cast(0 as numeric(17, 4))  Total_Points_Redeemed,"
			"Sum(ArcSurcharge.SubTotal) Total_Points_Loaded "
		"From "
			"ArcSurcharge Left Join ArcBill On "
				"ArcSurcharge.Arcbill_Key = ArcBill.Arcbill_Key "
			"Left Join Security On "
				"ArcBill.Security_Ref = Security.Security_Ref "
		"Where "
			"ArcBill.Time_Stamp > :StartTime And "
			"ArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"ArcSurcharge.Properties = 65536 ";
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"ArcBill.Billed_Location " 

		"Union All "

		"Select "
			"cast(DayArcBill.Billed_Location as varchar(25)) Order_Location,"
			"cast('Unknown' as varchar(50)) Name,"
			"cast(0 as numeric(17, 4)) Total_Points,"
			"cast(0 as numeric(17, 4)) Total_Spent,"
			"cast(0 as numeric(17, 4))  Total_Points_Earned,"
			"cast(0 as numeric(17, 4))  Total_Points_Redeemed,"
			"Sum(DayArcSurcharge.SubTotal) Total_Points_Loaded "
		"From "
			"DayArcSurcharge Left Join DayArcBill On "
				"DayArcSurcharge.Arcbill_Key = DayArcBill.Arcbill_Key "
			"Left Join Security On "
				"DayArcBill.Security_Ref = Security.Security_Ref "
		"Where "
			"DayArcBill.Time_Stamp > :StartTime And "
			"DayArcBill.Time_Stamp < :EndTime And "
			"Security.Security_Event = 'Billed By' And "
			"DayArcSurcharge.Properties = 65536 ";
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArcBill.Billed_Location "

		"Order By "
			"1,2";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrLoyaltyHistory->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}


  //TestData(qrLoyaltyHistory,true);
}

void TdmMMReportData::SetupLoyaltyHistoryDates(TDateTime StartTime, TDateTime EndTime,TStrings *Locations)
{
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	// Detailed Report ( with NO VIP Credit )
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"Select "
			"cast(Archive.Order_Location as varchar(25)) Order_Location,"
            "cast(Archive.Time_Stamp as timestamp) TIME_STAMP, "
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(Archive.Price * Archive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(Archive.Points_Earned) Total_Points_Earned,"
			"Sum(Archive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"Archive Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime ";
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
            "Archive.Time_Stamp, "
			"Archive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"cast(DayArchive.Order_Location as varchar(25)) Order_Location,"
            "DayArchive.Time_Stamp TIME_STAMP, "
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(DayArchive.Price * DayArchive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(DayArchive.Points_Earned) Total_Points_Earned,"
			"Sum(DayArchive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"DayArchive Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime ";

	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
            "DayArchive.Time_Stamp, "
			"DayArchive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Order By "
			"1,2";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;

	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}


}

void TdmMMReportData::SetupLoyaltyHistoryCustomer(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations)
{
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	// Detailed Report ( with NO VIP Credit )
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"Select "
			"cast(Archive.Order_Location as varchar(25)) Order_Location,"
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(Archive.Price * Archive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(Archive.Points_Earned) Total_Points_Earned,"
			"Sum(Archive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"Archive "
         "Inner Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
        " Left join ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
			//"Inner Join Security On Archive.Security_Ref = Security.Security_Ref "
		"Where "
		    " ((ARCORDERDISCOUNTS.DISCOUNT_KEY>0 and "
          " ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME<> 'Non-Chargeable' and "
          " ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME<> 'Complimentary') or Archive.DISCOUNT =0) and "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Union All "

		"Select "
			"cast(DayArchive.Order_Location as varchar(25)) Order_Location,"
			"Contacts.Name,"
			"Contacts.Total_Spent Total_Points,"
			"cast(Sum(DayArchive.Price * DayArchive.Qty) as numeric(17,4)) Total_Spent,"
			"Sum(DayArchive.Points_Earned) Total_Points_Earned,"
			"Sum(DayArchive.Redeemed) Total_Points_Redeemed, "
			"cast(0 as numeric(17, 4)) Total_Points_Loaded "
		"From "
			"DayArchive "
         "Inner Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
	   	" Left join DAYARCORDERDISCOUNTS on DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
			//"Inner Join Security On DayArchive.Security_Ref = Security.Security_Ref "
		"Where "
		    " ((DAYARCORDERDISCOUNTS.DISCOUNT_KEY>0 and "
            " DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME<> 'Non-Chargeable' and "
            " DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME<> 'Complimentary') or DAYArchive.DISCOUNT =0) and "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime  " ;
	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Loyalty_Key,"
			"Contacts.Name,"
			"Contacts.Total_Spent "

		"Order By "
			"1,2";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrLoyaltyHistory->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}


  //TestData(qrLoyaltyHistory,true);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyHistoryCustomer(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations, bool t)
{
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"SELECT "
			 "ARCBILL.BILLED_LOCATION AS LOCATION, "
			 "CONTACTS.CONTACTS_KEY, "
			 "CONTACTS.MEMBER_NUMBER, "
			 "CONTACTS.NAME, "
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS, "
             "CONTACTS.INITIAL_EARNT_POINTS, "
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE, "
			 "POINTSTRANSACTIONS.INVOICE_NUMBER, "
			 "ARCBILL.TIME_STAMP, "
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS "
		"FROM CONTACTS "
             "LEFT JOIN POINTSTRANSACTIONS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN ARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
        "WHERE "
             "ADJUSTMENT_TYPE != 0 and BILLED_LOCATION is not null  "
			 "ArcBill.Time_Stamp > :StartTime And "
			 "ArcBill.Time_Stamp < :EndTime ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Arcbill.Billed_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		" GROUP BY "
			 "ARCBILL.BILLED_LOCATION, "
			 "CONTACTS.CONTACTS_KEY, "
			 "CONTACTS.MEMBER_NUMBER, "
			 "CONTACTS.NAME, "
			 "CONTACTS.TOTAL_SPENT, "
             "CONTACTS.INITIAL_EARNT_POINTS, "
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE, "
			 "POINTSTRANSACTIONS.INVOICE_NUMBER, "
			 "ARCBILL.TIME_STAMP "

		"Union All "

		"SELECT "
			 "DAYARCBILL.BILLED_LOCATION AS LOCATION, "
			 "CONTACTS.CONTACTS_KEY, "
			 "CONTACTS.MEMBER_NUMBER, "
			 "CONTACTS.NAME, "
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS, "
             "CONTACTS.INITIAL_EARNT_POINTS, "
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE, "
			 "POINTSTRANSACTIONS.INVOICE_NUMBER, "
			 "DAYARCBILL.TIME_STAMP, "
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS "
		"FROM CONTACTS "
             "LEFT JOIN POINTSTRANSACTIONS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN DAYARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
        "WHERE "
             "ADJUSTMENT_TYPE != 0 and BILLED_LOCATION is not null  "
			 "DayArcBill.Time_Stamp > :StartTime And "
			 "DayArcBill.Time_Stamp < :EndTime ";
	if (Names && Names->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArcbill.Billed_Location", "LocationParam") + ")";
	}
 	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"GROUP BY "
			 "DAYARCBILL.BILLED_LOCATION, "
			 "CONTACTS.CONTACTS_KEY, "
			 "CONTACTS.MEMBER_NUMBER, "
			 "CONTACTS.NAME, "
			 "CONTACTS.TOTAL_SPENT, "
             "CONTACTS.INITIAL_EARNT_POINTS, "
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE, "
			 "POINTSTRANSACTIONS.INVOICE_NUMBER, "
			 "DAYARCBILL.TIME_STAMP "

		"Order By "
			"2,7";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrLoyaltyHistory->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyAuditSummary(TDateTime StartTime, TDateTime EndTime, TStrings *Names)
{
	qrLoyaltyAuditSummary->Close();
	qrLoyaltyAuditSummary->SQL->Text =
		"SELECT "
			 "ARCBILL.BILLED_LOCATION AS LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "ARCBILL.TIME_STAMP,"
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,"
			 "ARCBILL.TOTAL TOTAL_SPENT "
		"FROM "
			 "CONTACTS LEFT JOIN POINTSTRANSACTIONS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN ARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
		"WHERE "
			 "(CONTACTS.CONTACT_TYPE = 2 OR CONTACTS.CONTACT_TYPE = 4) AND "
			 "ARCBILL.TIME_STAMP >= :StartTime AND "
			 "ARCBILL.TIME_STAMP < :EndTime ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Names->Count, "CONTACTS.NAME", "NamesParam") + ")";
	}

	qrLoyaltyAuditSummary->SQL->Text		=	qrLoyaltyAuditSummary->SQL->Text +

		"GROUP BY "
			 "ARCBILL.BILLED_LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "ARCBILL.TIME_STAMP,"
			 "ARCBILL.TOTAL "

		"UNION ALL "

		"SELECT "
			 "DAYARCBILL.BILLED_LOCATION AS LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "DAYARCBILL.TIME_STAMP,"
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,"
			 "DAYARCBILL.TOTAL TOTAL_SPENT "
		"FROM "
			 "CONTACTS LEFT JOIN POINTSTRANSACTIONS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN DAYARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
		"WHERE "
			 "(CONTACTS.CONTACT_TYPE = 2 OR CONTACTS.CONTACT_TYPE = 4) AND "
			 "DAYARCBILL.TIME_STAMP >= :StartTime AND "
			 "DAYARCBILL.TIME_STAMP < :EndTime ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Names->Count, "CONTACTS.NAME", "NamesParam") + ")";
	}

	qrLoyaltyAuditSummary->SQL->Text		=	qrLoyaltyAuditSummary->SQL->Text +

		"GROUP BY "
			 "DAYARCBILL.BILLED_LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "DAYARCBILL.TIME_STAMP,"
			 "DAYARCBILL.TOTAL "

		"ORDER BY "
			 "4,"
			 "7";

	qrLoyaltyAuditSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyAuditSummary->ParamByName("EndTime")->AsDateTime	= EndTime;

	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrLoyaltyAuditSummary->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyAudit(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations)
{
	qrLoyaltyAuditSummary->Close();
	qrLoyaltyAuditSummary->SQL->Text =
		"SELECT "
			 "ARCBILL.BILLED_LOCATION AS LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "ARCBILL.TIME_STAMP,"
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,"
			 "ARCBILL.TOTAL TOTAL_SPENT "
		"FROM "
			 "POINTSTRANSACTIONS LEFT JOIN CONTACTS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN ARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
		"WHERE "
			 "(CONTACTS.CONTACT_TYPE = 2 OR CONTACTS.CONTACT_TYPE = 4) AND "
			 "ARCBILL.TIME_STAMP > :StartTime AND "
			 "ARCBILL.TIME_STAMP < :EndTime ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Names->Count, "CONTACTS.NAME", "NamesParam") + ")";
	}

	if (Locations->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
	}

	qrLoyaltyAuditSummary->SQL->Text		=	qrLoyaltyAuditSummary->SQL->Text +
		"GROUP BY "
			 "ARCBILL.BILLED_LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "ARCBILL.TIME_STAMP,"
			 "ARCBILL.TOTAL "

		"UNION ALL "

		"SELECT "
			 "DAYARCBILL.BILLED_LOCATION AS LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT TOTAL_POINTS,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "DAYARCBILL.TIME_STAMP,"
			 "SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,"
			 "DAYARCBILL.TOTAL TOTAL_SPENT "
		"FROM "
			 "POINTSTRANSACTIONS LEFT JOIN CONTACTS ON "
				  "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
			 "LEFT JOIN DAYARCBILL ON "
				  "POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER ";

	if (Names && Names->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Names->Count, "CONTACTS.NAME", "NamesParam") + ")";
	}

	if (Locations->Count > 0)
	{
		qrLoyaltyAuditSummary->SQL->Text	=	qrLoyaltyAuditSummary->SQL->Text + "AND (" +
														ParamString(Locations->Count, "DAYARCBILL.BILLED_LOCATION", "LocationParam") + ")";
	}

	qrLoyaltyAuditSummary->SQL->Text		=	qrLoyaltyAuditSummary->SQL->Text +
		"WHERE "
			 "(CONTACTS.CONTACT_TYPE = 2 OR CONTACTS.CONTACT_TYPE = 4) AND "
			 "DAYARCBILL.TIME_STAMP > :StartTime AND "
			 "DAYARCBILL.TIME_STAMP < :EndTime "
		"GROUP BY "
			 "DAYARCBILL.BILLED_LOCATION,"
			 "CONTACTS.CONTACTS_KEY,"
			 "CONTACTS.MEMBER_NUMBER,"
			 "CONTACTS.NAME,"
			 "CONTACTS.TOTAL_SPENT,"
             "CONTACTS.INITIAL_EARNT_POINTS,"
			 "POINTSTRANSACTIONS.ADJUSTMENT_TYPE,"
			 "POINTSTRANSACTIONS.INVOICE_NUMBER,"
			 "DAYARCBILL.TIME_STAMP,"
			 "DAYARCBILL.TOTAL "

		"ORDER BY "
			 "4,"
			 "6,"
			 "8";

	qrLoyaltyAuditSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyAuditSummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrLoyaltyAuditSummary->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}

	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyAuditSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyHistoryVIPCredit(TDateTime StartTime, TDateTime EndTime,TStrings *Locations)
{
	// now looks at time_Stamp_Billed as this is the decider for when you get points
	// Summary Report with VIP Credit
	qrLoyaltyHistory->Close();
	qrLoyaltyHistory->SQL->Text =
		"Select "
			"Archive.Order_Location,"
			"'' as  Name, "
			"cast('Sales' as varchar(10)) ItemName, "
			"Archive.Time_Stamp_Billed Time_Stamp, "
			"cast(sum(Contacts.Total_Spent) as numeric(17, 4)) Total_Points,"
			"cast(Sum(Archive.Qty * Archive.Price + Archive.Discount) as numeric(17, 4)) Total_Spent,"
			"cast(Sum(Archive.Points_Earned) as numeric(17, 4)) Total_Points_Earned,"
			"cast(Sum(Archive.Redeemed) as numeric(17, 4)) Total_Points_Redeemed "
		"From "
			"Archive Left Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
			//"Inner Join Security On Archive.Security_Ref = Security.Security_Ref "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime And "
			//"Security.Time_Stamp > :StartTime And "
			//"Security.Time_Stamp < :EndTime And "
			//"Security.Security_Event = 'Ordered By' and "
			"Archive.Item_Name <> '" + CurrentConnection.VIPCreditName + "' "; //'VIP CREDIT' ";
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,Archive.TIME_STAMP_Billed "

		"Union All "

		"Select "
			"Archive.Order_Location,"
			"'' as  Name, "
			"cast('VIP Credit' as varchar(10)) ItemName,"
			"Archive.Time_Stamp_Billed Time_Stamp, "
			"cast(sum(Contacts.Total_Spent) as numeric(17, 4)) Total_Points,"
			"cast(Sum(Archive.Qty * Archive.Price + Archive.Discount)  as numeric(17, 4)) Total_Spent,"
			"cast(Sum(Archive.Points_Earned) as numeric(17, 4)) Total_Points_Earned,"
			"cast(Sum(Archive.Redeemed) as numeric(17, 4)) Total_Points_Redeemed "
		"From "
			"Archive Left Join Contacts On Archive.Loyalty_Key = Contacts.Contacts_Key "
			//"Inner Join Security On Archive.Security_Ref = Security.Security_Ref "
		"Where "
			"Archive.Time_Stamp_Billed > :StartTime And "
			"Archive.Time_Stamp_Billed < :EndTime And "
			//"Security.Time_Stamp > :StartTime And "
			//"Security.Time_Stamp < :EndTime And "
			//"Security.Security_Event = 'Ordered By' and "
			"Archive.Item_Name = '" + CurrentConnection.VIPCreditName + "' ";
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"Archive.Order_Location,Archive.Time_Stamp_Billed "

		"Union All "

		"Select "
			"DayArchive.Order_Location,"
			"'' as  Name, "
			"cast('Sales' as varchar(10)) ItemName,"
			"DayArchive.Time_Stamp_Billed Time_Stamp,"
			"cast(sum(Contacts.Total_Spent) as numeric(17, 4)) Total_Points,"
			"cast(Sum(DayArchive.Qty * DayArchive.Price + DayArchive.Discount) as numeric(17, 4)) Total_Spent,"
			"cast(Sum(DayArchive.Points_Earned) as numeric(17, 4)) Total_Points_Earned,"
			"cast(Sum(DayArchive.Redeemed) as numeric(17, 4)) Total_Points_Redeemed "
		"From "
			"DayArchive Left Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			//"Inner Join Security On DayArchive.Security_Ref = Security.Security_Ref "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime And "
			//"Security.Time_Stamp > :StartTime And "
			//"Security.Time_Stamp < :EndTime And "
			//"Security.Security_Event = 'Ordered By' and "
			"DayArchive.Item_Name <> '" + CurrentConnection.VIPCreditName + "' ";
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,DayArchive.Time_Stamp_Billed "

		"Union All "

		"Select "
			"DayArchive.Order_Location,"
			"'' as  Name, "
			"cast('VIP Credit' as varchar(10)) ItemName,"
			"DayArchive.Time_Stamp_Billed Time_Stamp,"
			"cast(sum(Contacts.Total_Spent) as numeric(17, 4)) Total_Points,"
			"cast(Sum(DayArchive.Qty * DayArchive.Price + DayArchive.Discount) as numeric(17, 4)) Total_Spent,"
			"cast(Sum(DayArchive.Points_Earned) as numeric(17, 4)) Total_Points_Earned,"
			"cast(Sum(DayArchive.Redeemed) as numeric(17, 4)) Total_Points_Redeemed "
		"From "
			"DayArchive Left Join Contacts On DayArchive.Loyalty_Key = Contacts.Contacts_Key "
			//"DayArchive Inner Join Security On DayArchive.Security_Ref = Security.Security_Ref "
		"Where "
			"DayArchive.Time_Stamp_Billed > :StartTime And "
			"DayArchive.Time_Stamp_Billed < :EndTime And "
			//"Security.Time_Stamp > :StartTime And "
			//"Security.Time_Stamp < :EndTime And "
			//"Security.Security_Event = 'Ordered By' and "
			"DayArchive.Item_Name = '" + CurrentConnection.VIPCreditName + "' ";
	if (Locations->Count > 0)
	{
		qrLoyaltyHistory->SQL->Text	=	qrLoyaltyHistory->SQL->Text + "and (" +
													ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ")";
	}
	qrLoyaltyHistory->SQL->Text		=	qrLoyaltyHistory->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,DayArchive.Time_Stamp_Billed "

		"Order By "
			"3,1,4";
	qrLoyaltyHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyaltyHistory->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyHistory->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyBirthdays(TStrings *Months)
{
	qrLoyalty->Close();
	qrLoyalty->SQL->Text =
		"Select "
			"Name,"
			"cast(Total_Spent as numeric(17, 4)) Points_Earnt,"
			"Cast(0 As Numeric(17,4)) Points_Redeemed,"
			"cast(Total_Spent as numeric(17, 4)) Points_Remaining,"
			"DateOfBirth \"Birthday\","
			"DateOfBirth DOB_Month,"
			"Extract(Month From DateOfBirth) Birthday_Month,"
			"Extract(Day From DateOfBirth) Birthday_Day,"

			"Extract(Year From Current_TimeStamp) - Extract(Year From DateOfBirth) Year_Diff,"
			"Extract(Month From Current_TimeStamp) - Extract(Month From DateOfBirth) Month_Diff,"
			"Extract(Day From Current_TimeStamp) - Extract(Day From DateOfBirth) Day_Diff,"

			"Mailing_Address \"Home Address\", "
			"Phone \"Home Phone\","
			"Mobile \"Mobile Phone\","
			"Fax \"Home Fax\","
			"Email \"E-mail\" "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4) And "
			"DateOfBirth Is Not Null And "
			"DateOfBirth <> '12/30/1899' ";
	if (Months->Count > 0)
	{
		qrLoyalty->SQL->Text =	qrLoyalty->SQL->Text + "And (" +
										ParamString(Months->Count, "Extract(Month From DateOfBirth)", "Param") + ")";
	}
	qrLoyalty->SQL->Text =		qrLoyalty->SQL->Text +
//			ParamString(Strings->Count, "Extract(Month From DateOfBirth)") +
		"Order By "
			"7,8,"
			"Name";
/*	for (int i=0; i<Strings->Count; i++)
	{
		qrLoyalty->ParamByName("Param" + IntToStr(i))->AsString = Strings->Strings[i];
	}*/
	for (int i=0; i<Months->Count; i++)
	{
		qrLoyalty->ParamByName("Param" + IntToStr(i))->AsString = Months->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyDetails(TStrings *Customers)
{
	qrLoyalty->Close();
	qrLoyalty->SQL->Text =
		"Select "
			"Name,"
			"Title,"
			"Sex,"
			"DateOfBirth \"Birthday\","
			"Mailing_Address \"Home Address\","
			"Phone \"Home Phone\","
			"Mobile \"Mobile Phone\","
			"Fax \"Home Fax\","
			"Email \"E-mail\","
			"Member_Number "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4) ";
	if (Customers->Count > 0)
	{
		qrLoyalty->SQL->Text =	qrLoyalty->SQL->Text + "And (" +
										ParamString(Customers->Count, "Name", "Param") + ")";
	}
	qrLoyalty->SQL->Text =		qrLoyalty->SQL->Text +
		"Order By "
			"Name";
	for (int i=0; i<Customers->Count; i++)
	{
		qrLoyalty->ParamByName("Param" + IntToStr(i))->AsString = Customers->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupNewLoyaltyDetails(TDateTime StartTime, TDateTime EndTime)
{
	qrLoyalty->Close();
	qrLoyalty->SQL->Text =
		"Select "
			"Name,"
			"Title,"
			"Sex,"
			"DateOfBirth \"Birthday\","
			"Mailing_Address \"Home Address\","
			"Phone \"Home Phone\","
			"Mobile \"Mobile Phone\","
			"Fax \"Home Fax\","
			"Email \"E-mail\","
			"Member_Number "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4) "
			"And Last_Modified >= :StartTime "
			"And Last_Modified < :EndTime "
		"Order By "
			"Name";
	qrLoyalty->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyalty->ParamByName("EndTime")->AsDateTime	= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupAlteredLoyaltyDetails(TDateTime StartTime, TDateTime EndTime)
{
	qrLoyalty->Close();
	qrLoyalty->SQL->Text =
		"Select "
			"Name,"
			"Title,"
			"Sex,"
			"DateOfBirth \"Birthday\","
			"Mailing_Address \"Home Address\","
			"Phone \"Home Phone\","
			"Mobile \"Mobile Phone\","
			"Fax \"Home Fax\","
			"Email \"E-mail\","
			"Member_Number "
		"From "
			"Contacts "
		"Where "
			"Contact_Type in (2,4) "
			"And Last_Modified >= :StartTime "
			"And Last_Modified < :EndTime "
		"Order By "
			"Name";
	qrLoyalty->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrLoyalty->ParamByName("EndTime")->AsDateTime	= EndTime;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDebtorInvoice(AnsiString InvoiceNumber)
{
	qrDebtorInvoice->Close();

            qrDebtorInvoice->SQL->Text =
           " Select "
"    Contacts.Name,Contacts.Mailing_Address,Invoices.Invoice_Number,Invoices.Creation_Date, "
"    Orders.PRICE_INCL Total_Inc,"
"    Invoices.Total_Excl, "
"    Orders.Size_Name, "
"    Cast(Orders.Item_Name as VarChar(50)) Item_Name,Sum(Orders.Qty) qty, "
"    Cast(Sum(Orders.Qty * Orders.PRICE + Orders.Discount ) as numeric(17,4)) Price1,"
"    Cast((Sum(Orders.Qty * Orders.Price + Orders.Discount)) as numeric(17, 4)) PriceExc,Orders.Discount,"
"    Cast((Orders.Price_Adjust) as numeric(17,4)) Price_Adjust, "
"    Orders.Time_Stamp Ordered_Time,HotelCustomer.Customer_Name,HotelCustomer.Room_Number  ,"
"   Cast( "
"        (  "
"          Sum(  "
"               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +   "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) "
"                      )    +  "
"     cast((  Sum(   "
"                  case when taxsetting.isPriceIncludetax =1 then   "
"                               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))  "
"                    else  "
"                              case when taxsetting.isRecalculateTaxAfterDiscount =1  then  "
"                                   cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                           cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                             + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                                              ) )as numeric(17,4)) )as  numeric(17,4))  "
"                               else    "
"                                      cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4))  "
"                              end    "
"                 end    "
"            )  * cast ((COALESCE(TaxDetails.VAT,0))as numeric (17,4))/100 )as numeric(17,4))  +   "
"      sum( cast((  cast(( coalesce(ORDERS.PRICE,0)-  coalesce(ORDERS.MAXRETAILPRICE,0))as numeric(17,4)) *  cast((coalesce(TaxDetails.ProfitTax,0))as numeric(17,4))/100 )as numeric(17,4)) ) +  "
"        Sum(     "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then   "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                   cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                     + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then     "
"                                  cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                           else   "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))   "
"                        end  "
"                end  "
"             )  +  "
"    cast((   Sum(  "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then  "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then  "
"                                  cast((  (cast((  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"                              "
"                        else "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                        end  "
"                end      "
"             ) *  cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 )as numeric(17,4))  "
"      ) as numeric(17,4))  price  , "
"	(CASE WHEN taxsetting.isRecalculateTaxAfterDiscount = 1 THEN     "
"     cast(  (Sum( cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))  "
"                  )    "
"              )* cast ((COALESCE(TaxDetails.VAT,0))as numeric(17,4))/100 as numeric(17, 4)   "
"           )        "
"         else cast((Sum(   cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4))   ))*    cast((COALESCE(TaxDetails.VAT,0))as numeric(17,4))/100 as numeric(17, 4))  "
"   END ) As VAT  ,   "
"      cast((Sum(  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))   ))* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 as numeric(17, 4)) ServiceCharge,  "
"                                    "
"                            "
"       cast(  cast((Sum(  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))   ))* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 as numeric(17, 4))  "
"         * cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 as numeric(17, 4))  ServiceChargeTax   "
  " From   "
"            Contacts Inner Join Invoices on                                                                                                                                                                                                              "
"                Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                                                                            "
"			Inner Join Tab on                                                                                                                                                                                                                             "
"                Invoices.Invoice_Key = Tab.Invoice_Key                                                                                                                                                                                                   "
"			Left Join Orders on                                                                                                                                                                                                                           "
"                Tab.Tab_Key = Orders.Tab_Key                                                                                                                                                                                                             "
"            Left Join HotelCustomer on                                                                                                                                                                                                                   "
"                Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                                                                         "
"            left join                                                                                                                                                                                                                                    "
"            (SELECT  cast(1 as int) keyvalue ,                                                                                                                                                                                                           "
"                    TAXPROFILE.ORDER_KEY,                                                                                                                                                                                                                "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT ,                                                                                                                                                             "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,                                                                                                                                                    "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS ServiceChargeTax ,                                                                                                                                                 "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 5 THEN TAXPROFILE.TAX_RATE END) AS ProfitTax   "
"                    FROM (SELECT                                                                                                                                                                                                                         "
"                    TFO.ORDER_KEY,  TAXPROFILES.TYPE,                                                                                                                                                                                                   "
"                    Cast(( sum(COALESCE(TAXPROFILES.RATE,0)) ) as Numeric(17,4)) TAX_RATE                                                                                                                                                                      "
"                    FROM TAXPROFILES_ORDERS TFO                                                                                                                                                                                                          "
"                    left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY                                                                                                                                                                     "
"                    group by TFO.ORDER_KEY,TAXPROFILES.TYPE,TAXPROFILES.RATE )  TAXPROFILE                                                                                                                                                               "
"                    GROUP BY TAXPROFILE.ORDER_KEY                                                                                                                                                                                                        "
"            ) TaxDetails on TaxDetails.ORDER_KEY=ORDERS.ORDER_KEY                                                                                                                                                                                        "
"          left join (SELECT VARS.keyvalue  ,MIN(CASE WHEN setting.INTEGER_VAL = 1 THEN VARS.ServiceChargeTax else cast(0 as int)   END) AS ServiceChargeTax                                                                                              "
" FROM (  SELECT      cast(1 as int) keyvalue   ,                                                                                                                                                                                                         "
" MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END                                                                                                                                                                         "
" ) AS ServiceChargeTax      FROM VARSPROFILE   )    VARS                                                                                                                                                                                                 "
" left join (SELECT   cast(1 as int) keyvalue  ,a.INTEGER_VAL FROM VARSPROFILE a where a.VARIABLES_KEY=8005) setting on  setting.keyvalue=    VARS.keyvalue                                                                                               "
" group by 1   ) STAX on  STAX.keyvalue=TaxDetails.keyvalue                                                                                                                                                                                               "
"           left join ( SELECT                                                                                                                                                                                                                            "
"                cast(1 as int) keyvalue   ,                                                                                                                                                                                                              "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8000 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludetax  ,                                                                                                                               "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8001 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludeServiceCharge  ,                                                                                                                     "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8002 THEN VARSPROFILE.INTEGER_VAL END ) As isRecalculateTaxAfterDiscount  ,                                                                                                                   "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8003 THEN VARSPROFILE.INTEGER_VAL END ) As  isRecalculateServiceChargeAfterDiscount ,                                                                                                         "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8005 THEN VARSPROFILE.INTEGER_VAL END ) As  isApplyTaxToServiceCharge                                                                                                                         "
"                 from VARSPROFILE) taxsetting on  taxsetting.keyvalue=TaxDetails.keyvalue                                                                                                                                                                "
"        Where                                                                                                                                                                                                                                            "
"			Invoices.Invoice_Number =  :Invoice_Number                                                                                                                                                                                                                "
"            group by                                                                                                                                                                                                                                     "
"                Contacts.Name,                                                                                                                                                                                                                           "
"                Contacts.Mailing_Address,                                                                                                                                                                                                                "
"                Invoices.Invoice_Number,                                                                                                                                                                                                                 "
"                Invoices.Creation_Date,                                                                                                                                                                                                                  "
"                Orders.PRICE_INCL,                                                                                                                                                                                                                       "
"                Invoices.Total_Excl,                                                                                                                                                                                                                     "
"                Orders.Size_Name,                                                                                                                                                                                                                        "
"                Orders.Item_Name ,                                                                                                                                                                                                                       "
"                Orders.Qty,                                                                                                                                                                                                                              "
"                Orders.Price , Orders.Discount ,                                                                                                                                                                                                         "
"                Orders.Price_Adjust,                                                                                                                                                                                                                     "
"                Orders.Time_Stamp ,                                                                                                                                                                                                                      "
"                HotelCustomer.Customer_Name,                                                                                                                                                                                                             "
"                HotelCustomer.Room_Number,                                                                                                                                                                                                               "
"                Orders.BASE_PRICE,                                                                                                                                                                                                                       "
"                Orders.DISCOUNT_WITHOUT_TAX,                                                                                                                                                                                                             "
"                TaxDetails.VAT,                                                                                                                                                                                                                          "
"                TaxDetails.ServiceCharge,                                                                                                                                                                                                                "
"                TaxDetails.ServiceChargeTax,                                                                                                                                                                                                             "
"                STAX.ServiceChargeTax ,                                                                                                                                                                                                                  "
"                Orders.order_key ,                                                                                                                                                                                                                       "
"                taxsetting.isRecalculateTaxAfterDiscount                                                                                                                                                                                                 "
"               union all                                                                                                                                                                            "
"     select                                                                                                                                                                                          "
"            Contacts.Name,                                                                                                                                                                           "
"			Contacts.Mailing_Address,                                                                                                                                                                "
"			Invoices.Invoice_Number,                                                                                                                                                                 "
"		    Invoices.Creation_Date,                                                                                                                                                                  "
"		    DayArchive.PRICE_INCL Total_Inc,                                                                                                                                                         "
"			Invoices.Total_Excl,                                                                                                                                                                     "
"		    DayArchive.Size_Name,                                                                                                                                                                    "
"		    Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,                                                                                                                                     "
"		    Sum(DayArchive.Qty) qty,                                                                                                                                                                 "
"		    Cast(Sum(DayArchive.Qty * DayArchive.Price + coalesce(DayArchive.Discount,0)) as numeric(17,4)) Price1,                                                                                  "
"			Cast(Sum(cast((DayArchive.Qty * DayArchive.Price + DayArchive.Discount) as Numeric(17,4))  ) as numeric(17,4)) PriceExc,                                                                 "
"		  	DayArchive.Discount,                                                                                                                                                                     "
"            Cast((DayArchive.Price_Adjust) as numeric(17,4)) Price_Adjust,                                                                                                                           "
"		    DayArchive.Time_Stamp Ordered_Time,                                                                                                                                                      "
"            HotelCustomer.Customer_Name,                                                                                                                                                             "
"            HotelCustomer.Room_Number,                                                                                                                                                               "
"            Cast(Sum(DayArchive.Qty * DayArchive.BASE_PRICE + (case when STAX.isRecalculateTaxAfterDiscount =1 then  coalesce(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0) else coalesce(DAYARCHIVE.DISCOUNT,0) end )) + sum(COALESCE(AOT.VAT,0)) + sum(COALESCE(AOT.ServiceCharge,0))                                       "
"	       + sum(COALESCE(AOT.ServiceChargeTax ,0))                                                                                                                                                  "
"           as Numeric(17,4)) Price ,                                                                                                                                                                 "
"          Cast(sum(COALESCE(AOT.VAT,0)) as Numeric(17,4)) VAT,                                    "
 "          Cast(sum(COALESCE(AOT.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,                "
 "                    Cast(sum(COALESCE(AOT.ServiceChargeTax,0)) as Numeric(17,4))ServiceChargeTax   "
"           From                                                                                                                                                                                      "
"			Contacts Inner Join Invoices on                                                                                                                                                          "
"            Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                            "
"			Inner Join DayArcBill on                                                                                                                                                                 "
"            Invoices.Invoice_Key = DayArcBill.Invoice_Key                                                                                                                                            "
"			Left Join DayArchive on                                                                                                                                                                  "
"            DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key                                                                                                                                          "
"           Left Join HotelCustomer on                                                                                                                                                                "
"        Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                             "
"         LEFT JOIN (                                                                                                                                                                                 "
"    SELECT                                                                                                                                                                                           "
"                DAYARCORDERTAXES.ARCHIVE_KEY, cast(1 as int) keyvalue ,                                                                                                                              "            
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT  ,                                                                                           "
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge  ,                                                                                 "
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax                                                                                 "
"				  FROM  DAYARCORDERTAXES                                                                                                                                                             "
"                GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )                                                                                                                                              "
"                AOT ON DAYARCHIVE.ARCHIVE_KEY = AOT.ARCHIVE_KEY   left join (SELECT      cast(1 as int) keyvalue   ,                                                                                 "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax ,                                                                             "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8002 THEN VARSPROFILE.INTEGER_VAL END ) As isRecalculateTaxAfterDiscount  ,                                                             "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8003 THEN VARSPROFILE.INTEGER_VAL END ) As  isRecalculateServiceChargeAfterDiscount                                                     "
"                FROM VARSPROFILE    ) STAX on  STAX.keyvalue=AOT.keyvalue                                                                                                                            "
"                Where                                                                                                                                                                                "
"			    invoices.Invoice_Number = :Invoice_Number                                                                                                                                            "
"						 group by                                                                                                                                                                    "
"		        Contacts.Name,                                                                                                                                                                       "
"                Contacts.Mailing_Address,                                                                                                                                                            "
"                Invoices.Invoice_Number,                                                                                                                                                             "
"                Invoices.Creation_Date,                                                                                                                                                              "
"                DayArchive.PRICE_INCL,                                                                                                                                                               "
"                Invoices.Total_Excl,                                                                                                                                                                 "
"                DayArchive.Size_Name,                                                                                                                                                                "
"                DayArchive.Item_Name ,                                                                                                                                                               "
"                DayArchive.Qty,                                                                                                                                                                      "
"                DayArchive.Price ,                                                                                                                                                                   "
"                 DayArchive.Discount ,                                                                                                                                                               "
"                DayArchive.Price_Adjust,                                                                                                                                                             "
"                DayArchive.Time_Stamp ,                                                                                                                                                              "
"                HotelCustomer.Customer_Name,                                                                                                                                                         "
"               HotelCustomer.Room_Number,                                                                                                                                                            "
"                DayArchive.BASE_PRICE,                                                                                                                                                               "
"                AOT.VAT,                                                                                                                                                                             "
"                 AOT.ServiceCharge,                                                                                                                                                                  "
"                 STAX.ServiceChargeTax                                                                                                                                                               "
"                 union all                                                                                                                                                                           "
" select                                                                                                                                                                                              "
"            Contacts.Name,                                                                                                                                                                           "
"			Contacts.Mailing_Address,                                                                                                                                                                "
"			Invoices.Invoice_Number,                                                                                                                                                                 "
"		    Invoices.Creation_Date,                                                                                                                                                                  "
"		    Archive.PRICE_INCL Total_Inc,                                                                                                                                                            "
"			Invoices.Total_Excl,                                                                                                                                                                     "
"		    Archive.Size_Name,                                                                                                                                                                       "
"		    Cast(Archive.Item_Name as VarChar(50)) Item_Name,                                                                                                                                        "
"		    Sum(Archive.Qty) qty,                                                                                                                                                                    "
"		    Cast(Sum(Archive.Qty * Archive.Price + coalesce(Archive.Discount,0)) as numeric(17,4)) Price1,                                                                                           "
"			Cast(Sum(cast((Archive.Qty * Archive.Price + Archive.Discount) as Numeric(17,4))  ) as numeric(17,4)) PriceExc,                                                                          "
"		  	Archive.Discount,                                                                                                                                                                        "
"           Cast((Archive.Price_Adjust) as numeric(17,4)) Price_Adjust,                                                                                                                               "
"		   Archive.Time_Stamp Ordered_Time,                                                                                                                                                          "
"           HotelCustomer.Customer_Name,                                                                                                                                                              "
"           HotelCustomer.Room_Number,                                                                                                                                                                "
"          Cast(Sum(Archive.Qty * Archive.BASE_PRICE + COALESCE(Archive.DISCOUNT,0)) + sum(COALESCE(AOT.VAT,0)) + sum(COALESCE(AOT.ServiceCharge,0))                                                  "
"	      + sum(COALESCE(AOT.ServiceChargeTax ,0))                                                                                                                                                   "
"           as Numeric(17,4)) Price ,                                                                                                                                                                 "
"          Cast(sum(COALESCE(AOT.VAT,0)) as Numeric(17,4)) VAT,                                    "
 "          Cast(sum(COALESCE(AOT.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,                "
"                    Cast(sum(COALESCE(AOT.ServiceChargeTax,0)) as Numeric(17,4))ServiceChargeTax   "
"         From                                                                                                                                                                                        "
"			Contacts Inner Join Invoices on                                                                                                                                                          "
"            Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                            "
"			Inner Join ArcBill on                                                                                                                                                                    "
"            Invoices.Invoice_Key = ArcBill.Invoice_Key                                                                                                                                               "
"			Left Join Archive on                                                                                                                                                                     "
"            ArcBill.ArcBill_Key = Archive.ArcBill_Key                                                                                                                                                "
"            Left Join HotelCustomer on                                                                                                                                                               "
"            Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                         "
"            LEFT JOIN (                                                                                                                                                                              "
" SELECT                                                                                                                                                                                              "
"					   	ARCORDERTAXES.ARCHIVE_KEY, cast(1 as int) keyvalue ,                                                                                                                         "
"					   	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT  ,                                                                                         "
"					   	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge  ,                                                                               "
"                        MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax                                                                               "
"				  FROM  ARCORDERTAXES                                                                                                                                                                "
"					   	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )                                                                                                                                         "
"						AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY   left join (SELECT      cast(1 as int) keyvalue   ,                                                                            "
"	       MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax      FROM VARSPROFILE    ) STAX on  STAX.keyvalue=AOT.keyvalue                       "
"						 Where                                                                                                                                                                       "
"			invoices.Invoice_Number = :Invoice_Number                                                                                                                                                "
"						 group by                                                                                                                                                                    "
"		        Contacts.Name,                                                                                                                                                                       "
"                Contacts.Mailing_Address,                                                                                                                                                            "
"                Invoices.Invoice_Number,                                                                                                                                                             "
"                Invoices.Creation_Date,                                                                                                                                                              "
"                Archive.PRICE_INCL,                                                                                                                                                                  "
"                Invoices.Total_Excl,                                                                                                                                                                 "
"                Archive.Size_Name,                                                                                                                                                                   "
"                Archive.Item_Name ,                                                                                                                                                                  "
"                Archive.Qty,                                                                                                                                                                         "
"                Archive.Price ,                                                                                                                                                                      "
"                 Archive.Discount ,                                                                                                                                                                  "
"                Archive.Price_Adjust,                                                                                                                                                                "
"                Archive.Time_Stamp ,                                                                                                                                                                 "
"                HotelCustomer.Customer_Name,                                                                                                                                                         "
"               HotelCustomer.Room_Number,                                                                                                                                                            "
"                Archive.BASE_PRICE,                                                                                                                                                                  "
"                AOT.VAT,                                                                                                                                                                             "
"                 AOT.ServiceCharge,                                                                                                                                                                  "
"                 STAX.ServiceChargeTax                                                                                                                                                               "
"                 Order By                                                                                                                                                                            "
"			      8,7 ";



	qrDebtorInvoice->ParamByName("Invoice_Number")->AsString = InvoiceNumber;
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDebtorInvoiceStatement(int DebtorKey, TDateTime PrintDate)
{
   /*
      Although we have the Invoice total (Total_Inc) in the invoice table,
      it is not possible to use it because sometimes the pos stores the discounted total in there ( when applied via selectDish )
      and if we apply the discount later ( via BillGroup ) this column does not have an affect.

      Hence it is required to look through the orders table for price*qty to find out the exact amounts that you have left to pay,
      and get the total paid from the arcbill for this particular invoice to calculate the entire invoice amount.

      Once we have that invoice amount we then reduce the discounts applied/saved in the orders table to produce the final invoice amount,
      which is reflected in the Price column in below query.

      Within the report, it will use the Price and Credit columns to produce the actual balance left to pay, and this should match
      with the values in POS.
   */

	qrDebtorInvoiceStatement->Close();
	qrDebtorInvoiceStatement->SQL->Text =
      "select "
      "    con.NAME, "
      "    con.MAILING_ADDRESS, "
      "    inv.CREATION_DATE, "
      "    inv.INVOICE_NUMBER, "
      "    -coalesce(prices_arc.TotalPaid_Arc,0) as Credit, "
      "    cast((ord_tab_details.SavedOrderPrice + coalesce(prices_arc.TotalPaid_Arc,0) + ord_tab_details.SavedDiscounts) as numeric(17,4))  as Price, "   // we should include the discounts saved on to orders and the discounts that has been previously paid
      "    hotel.CUSTOMER_NAME, "
      "    hotel.ROOM_NUMBER "
      "from "
      "    Contacts con "
      "    inner join INVOICES inv on con.CONTACTS_KEY = inv.CONTACTS_KEY and inv.CLOSED = 'F' "
      "    inner join TAB tab on inv.INVOICE_KEY = tab.INVOICE_KEY "
      "    left join HOTELCUSTOMER hotel on hotel.INVOICE_KEY = inv.INVOICE_KEY "
      "     inner join "
      "        ( "
      "           select "
      "                 ord.TAB_KEY, "
      "                 sum(ord.PRICE * ord.QTY ) as SavedOrderPrice, "
      "                 sum(ord.DISCOUNT) as SavedDiscounts "
      "           from "
      "                 ORDERS ord "
      "           group by ord.TAB_KEY "
      "        ) ord_tab_details on ord_tab_details.TAB_KEY = tab.TAB_KEY "
      "    left join "
      "        ( "
      "            select "
      "                arc.INVOICE_KEY, "
      "                sum( arc.DISCOUNT ) as Discount_Arc, "
      "                sum( arc.TOTAL ) as TotalPaid_Arc "
      "            from "
      "                ARCBILL arc "
      "            group by arc.INVOICE_KEY "
      "        ) prices_arc on prices_arc.INVOICE_KEY = inv.INVOICE_KEY "
      "where "
      "    inv.CREATION_DATE <= :PrintDate "
      "";

   qrDebtorInvoiceStatement->ParamByName("PrintDate")->AsDateTime = PrintDate;

	if (DebtorKey)
	{
		qrDebtorInvoiceStatement->SQL->Text = qrDebtorInvoiceStatement->SQL->Text + " and "
			"con.Contacts_Key = :Contacts_Key ";

      qrDebtorInvoiceStatement->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
	}

   qrDebtorInvoiceStatement->SQL->Text = qrDebtorInvoiceStatement->SQL->Text +
      "order by "
      "    con.NAME, inv.INVOICE_NUMBER "
      "";

/*
	qrDebtorInvoiceStatement->Close();
	qrDebtorInvoiceStatement->SQL->Text =
		"Select "
			"Contacts.Name,"
			"Contacts.Mailing_Address,"
			"Invoices.Creation_Date,"
			"Invoices.Invoice_Number,"
			"-(Tab.Credit) Credit,"
			"Sum(Orders.Price + Orders.Discount) Price, "
         "HotelCustomer.Customer_Name, "
         "HotelCustomer.Room_Number "
		"From "
			"Contacts Inner Join Invoices on "
				"Contacts.Contacts_Key = Invoices.Contacts_Key "
			"Inner Join Tab on "
				"Invoices.Invoice_Key = Tab.Invoice_Key "
			"Left Join Orders on "
				"Tab.Tab_Key = Orders.Tab_Key "
			"Left Join HotelCustomer on "
				"Invoices.Invoice_Key = HotelCustomer.Invoice_Key "
		"Where "
			"Invoices.Closed <> 'T' And "
			"Invoices.Creation_Date <= :PrintDate ";
	if (DebtorKey)
	{
		qrDebtorInvoiceStatement->SQL->Text = qrDebtorInvoiceStatement->SQL->Text + "and "
			"Contacts.Contacts_Key = :Contacts_Key ";
	}
	qrDebtorInvoiceStatement->SQL->Text = qrDebtorInvoiceStatement->SQL->Text +
		"Group By "
			"Contacts.Name,"
			"Contacts.Mailing_Address,"
			"Invoices.Invoice_Number,"
			"Invoices.Creation_Date,"
			"Tab.Credit,"
         "HotelCustomer.Customer_Name,"
         "HotelCustomer.Room_Number "
		"Order By "
			"1, 2, 3";
	if (DebtorKey)
	{
		qrDebtorInvoiceStatement->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
	}
	qrDebtorInvoiceStatement->ParamByName("PrintDate")->AsDateTime = PrintDate;
	//TestData(qrDebtorInvoiceStatement, true);

*/

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupDebtorSummary()
{
   /*
      refer to the comment on SetupDebtorInvoiceStatement for explanation on how the query works
      as this query is very similar to the other one, except for the group by clause at the end
   */

	qrDebtorSummary->Close();
	qrDebtorSummary->SQL->Text =
      "Select "
      "   con.Name, "
      "   -sum(coalesce(prices_arc.TotalPaid_Arc,0)) as Credit, "
      "    sum( cast((ord_tab_details.SavedOrderPrice + coalesce(prices_arc.TotalPaid_Arc,0) + ord_tab_details.SavedDiscounts) as numeric(17,4)) )  as Total "   // we should include the discounts saved on to orders and the discounts that has been previously paid
      "From "
      "    Contacts con "
      "    inner join INVOICES inv on con.CONTACTS_KEY = inv.CONTACTS_KEY and inv.CLOSED = 'F' "
      "    inner join TAB tab on inv.INVOICE_KEY = tab.INVOICE_KEY "
      "    left join HOTELCUSTOMER hotel on hotel.INVOICE_KEY = inv.INVOICE_KEY "
      "     inner join "
      "        ( "
      "           select "
      "                 ord.TAB_KEY, "
      "                 sum(ord.PRICE * ord.QTY ) as SavedOrderPrice, "
      "                 sum(ord.DISCOUNT) as SavedDiscounts "
      "           from "
      "                 ORDERS ord "
      "           group by ord.TAB_KEY "
      "        ) ord_tab_details on ord_tab_details.TAB_KEY = tab.TAB_KEY "
      "      left join "
      "      ( "
      "          select "
      "              arc.INVOICE_KEY, "
      "              sum( arc.DISCOUNT ) as Discount_Arc, "
      "              sum( arc.TOTAL ) as TotalPaid_Arc "
      "          from "
      "              ARCBILL arc "
      "          group by arc.INVOICE_KEY "
      "      ) prices_arc on prices_arc.INVOICE_KEY = inv.INVOICE_KEY "
      "Group By "
      "   con.Name "
      "Order By "
      "   1 "
      "";

/*
		"Select "
			"Contacts.Name,"
			"-Tab.Credit Credit,"
			"Sum(Orders.Price + Orders.Discount) Total "
		"From "
			"Contacts Inner Join Invoices on "
				"Contacts.Contacts_Key = Invoices.Contacts_Key "
			"Inner Join Tab on "
				"Invoices.Invoice_Key = Tab.Invoice_Key "
			"Left Join Orders on "
				"Tab.Tab_Key = Orders.Tab_Key "
		"Where "
			"Invoices.Closed <> 'T' "
		"Group By "
			"Contacts.Name,"
			"Tab.Tab_Key,"
			"Tab.Credit "
		"Order By "
			"1";
*/
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupHappyHour(TDateTime StartTime, TDateTime EndTime, TStrings *Locations,int CurrentSubReport)
{
	// CurrentSubReport 0 = all , 1 = Members , 2 = Non Members
	qrHappyHour->Close();
	qrHappyHour->SQL->Text =
		"Select "
			"Archive.Order_Location Group_Name,"
			"Archive.Course_Name,"
			"cast(Archive.Item_Name as Varchar(50)) Item_Name,"
			"Archive.Size_Name,"
			"Archive.GST_Percent,"
			"Sum(Archive.Qty) Item_Count,"
		    "Cast(Sum(Archive.Price * Archive.Qty ) as Numeric(17,4)) Price, "
			"Cast(sum(((ARCHIVE.PRICE*ARCHIVE.QTY - ARCHIVE.DISCOUNT )*ARCHIVE.PRICE_LEVEL0)/ARCHIVE.PRICE_LEVEL1)as numeric(17,4)) as PriceLevel0, "
			"Cast(   "
		   "	(sum(((ARCHIVE.PRICE*ARCHIVE.QTY - ARCHIVE.DISCOUNT )* ARCHIVE.PRICE_LEVEL0)/ARCHIVE.PRICE_LEVEL1)) -  "
		   "	(Sum(Archive.Price * Archive.Qty )-Sum(Archive.DISCOUNT * Archive.Qty )) as Numeric(17,4)   "
		   "	) as Difference,  "
			"Cast(Sum(Archive.Cost * Archive.Qty) as Numeric(17,4)) Cost,"

			"Cast(Sum(Archive.Price_Level1 * Archive.Qty) as Numeric(17,4)) PriceLevel1 "
		"From "
			"Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref "

		"Where "
			"Archive.Order_Type in (0,3) and "
			"Archive.Happy_Hour = 'T' and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations->Count > 0)
	{
		qrHappyHour->SQL->Text	=	qrHappyHour->SQL->Text + "and (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ") ";
	}
	if (CurrentSubReport == 1)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text +  "and Archive.Loyalty_Key > 0 ";
	}
	else if (CurrentSubReport == 2)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text +  "and (Archive.Loyalty_Key is NULL or Archive.Loyalty_Key = 0) ";
	}
	qrHappyHour->SQL->Text		=	qrHappyHour->SQL->Text +
		"Group By "
			"Archive.Order_Location,"
			"Archive.Course_Name,"
			"Archive.Item_Name,"
			"Archive.Size_Name, "
			"Archive.GST_Percent "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"DayArchive.Order_Location Group_Name,"
			"DayArchive.Course_Name,"
			"cast(DayArchive.Item_Name as Varchar(50)) Item_Name,"
			"DayArchive.Size_Name,"
			"DayArchive.GST_Percent,"
			"Sum(DayArchive.Qty) Item_Count,"
		"Cast(Sum(DayArchive.Price * DayArchive.Qty ) as Numeric(17,4)) Price, "
			"Cast(sum(((DayArchive.PRICE*DayArchive.QTY - DayArchive.DISCOUNT )*DayArchive.PRICE_LEVEL0)/DayArchive.PRICE_LEVEL1)as numeric(17,4)) as PriceLevel0, "
			"Cast(   "
		   "	(sum(((DayArchive.PRICE*DayArchive.QTY - DayArchive.DISCOUNT )* DayArchive.PRICE_LEVEL0)/DayArchive.PRICE_LEVEL1)) -  "
		   "	(Sum(DayArchive.Price * DayArchive.Qty )-Sum(DayArchive.DISCOUNT * DayArchive.Qty )) as Numeric(17,4)   "
		   "	) as Difference,  "
			"Cast(Sum(DayArchive.Cost * DayArchive.Qty) as Numeric(17,4)) Cost,"

			"Cast(Sum(DayArchive.Price_Level1) as Numeric(17,4)) PriceLevel1 "
		"From "
			"Security Left Join DayArchive on "
				"Security.Security_Ref = DayArchive.Security_Ref "
		"Where "
			"DayArchive.Order_Type in (0,3) and "
			"DayArchive.Happy_Hour = 'T' and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations->Count > 0)
	{
		qrHappyHour->SQL->Text	=	qrHappyHour->SQL->Text + "and (" +
											ParamString(Locations->Count, "DayArchive.Order_Location", "LocationParam") + ") ";
	}
	if (CurrentSubReport == 1)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text + "and DayArchive.Loyalty_Key > 0 ";
	}
	else if (CurrentSubReport == 2)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text +  "and (DayArchive.Loyalty_Key is NULL or DayArchive.Loyalty_Key = 0) ";
	}

	qrHappyHour->SQL->Text		=	qrHappyHour->SQL->Text +
		"Group By "
			"DayArchive.Order_Location,"
			"DayArchive.Course_Name,"
			"DayArchive.Item_Name,"
			"DayArchive.Size_Name,"
			"DayArchive.GST_Percent "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 "

		"Union All "

		"Select "
			"Orders.Order_Location Group_Name,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Orders.GST_Percent,"
			"Sum(Orders.Qty) Item_Count,"
			"Cast(Sum(Orders.Price * Orders.Qty ) as Numeric(17,4)) Price, "
			"Cast(sum(((Orders.PRICE*Orders.QTY - Orders.DISCOUNT )*Orders.PRICE_LEVEL0)/Orders.PRICE_LEVEL1)as numeric(17,4)) as PriceLevel0, "
			"Cast(   "
		   "	(sum(((Orders.PRICE*Orders.QTY - Orders.DISCOUNT )* Orders.PRICE_LEVEL0)/Orders.PRICE_LEVEL1)) -  "
		   "	(Sum(Orders.Price * Orders.Qty )-Sum(Orders.DISCOUNT * Orders.Qty )) as Numeric(17,4)   "
		   "	) as Difference,  "
			"Cast(Sum(Orders.Cost * Orders.Qty) as Numeric(17,4)) Cost,"

			"Cast(Sum(Orders.Price_Level1 * Orders.Qty) as Numeric(17,4)) PriceLevel1 "
		"From "
			"Security Left Join Orders on "
				"Security.Security_Ref = Orders.Security_Ref "
		"Where "
			"Orders.HappyHour = 'T' and "
			"Security.Time_Stamp >= :StartTime and "
			"Security.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Ordered By' ";
	if (Locations->Count > 0)
	{
		qrHappyHour->SQL->Text	=	qrHappyHour->SQL->Text + "and (" +
											ParamString(Locations->Count, "Orders.Order_Location", "LocationParam") + ") ";
	}
	if (CurrentSubReport == 1)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text +  "and Orders.Loyalty_Key > 0 ";
	}
	else if (CurrentSubReport == 2)
	{
		qrHappyHour->SQL->Text = qrHappyHour->SQL->Text +  "and (Orders.Loyalty_Key is NULL or Orders.Loyalty_Key = 0) ";
	}

	qrHappyHour->SQL->Text		=	qrHappyHour->SQL->Text +
		"Group By "
			"Orders.Order_Location,"
			"Orders.Course_Name,"
			"Orders.Item_Name,"
			"Orders.Size_Name,"
			"Orders.GST_Percent "
		"Having "
			"Count(Orders.Order_Key) > 0 "

		"Order By "
			"1,2,3,4,5";
	for (int i=0; i<Locations->Count; i++)
	{
		qrHappyHour->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	qrHappyHour->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrHappyHour->ParamByName("EndTime")->AsDateTime		= EndTime;
	//TestData(qrHappyHour,true);
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyPurchaseCountByContact(TStrings *Names)
{
	// Contact - Loyalty Purchase Counts
	qrLoyaltyPurchaseCount->Close();
	qrLoyaltyPurchaseCount->SQL->Text =
		"Select "
			"cast('Contact' as Varchar(8)) Report_Type,"
			"ContactFreebie.Item_Name,"
			"ContactFreebie.Size_Name,"
			"Cast(ContactFreebie.Item_Count as numeric(15,2)) Item_Count,"
			"Contacts.Name,"
			"cast(sum(ItemSize.Loc_Sale_Count) as numeric(17, 4)) Loc_Sale_Count,"
			"cast(sum(ItemSize.Loc_Discount_Percent) as numeric(17, 4)) Loc_Discount_Percent,"
			"cast(sum(ItemSize.Mem_Sale_Count) as numeric(17, 4)) Mem_Sale_Count,"
			"cast(sum(ItemSize.Mem_Discount_Percent) as numeric(17, 4)) Mem_Discount_Percent "
		"From "
			"ItemSize Left Join Item on "
				"ItemSize.Item_Key = Item.Item_Key "
			"Right join ContactFreebie on "
				"ItemSize.Size_Name = ContactFreebie.Size_Name and Item.Item_Name = ContactFreebie.Item_Name "
			"Left Join Contacts on "
				"ContactFreebie.Contacts_Key = Contacts.Contacts_Key ";
	if (Names->Count > 0)
	{
		qrLoyaltyPurchaseCount->SQL->Text	=	qrLoyaltyPurchaseCount->SQL->Text + "Where (" +
												ParamString(Names->Count, "Contacts.Name", "NamesParam") + ")";
	}
	qrLoyaltyPurchaseCount->SQL->Text = qrLoyaltyPurchaseCount->SQL->Text +
		"Group by "
			"Contacts.Name,"
			"ContactFreebie.Item_Name,"
			"ContactFreebie.Size_Name,"
			"ContactFreebie.Item_Count "
		"Order by "
			"Contacts.Name,ContactFreebie.Item_Name,ContactFreebie.Size_Name";

	for (int i=0; i<Names->Count; i++)
	{
		qrLoyaltyPurchaseCount->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
	}

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupLoyaltyPurchaseCountByLocation(TStrings *Locations)
{
	// Location - Loyalty Purchase Counts
	qrLoyaltyPurchaseCount->Close();
	qrLoyaltyPurchaseCount->SQL->Text =
		"Select "
			"cast('Location' as Varchar(8)) Report_Type,"
			"LocationsFreebie.Item_Name,"
			"LocationsFreebie.Size_Name,"
			"cast( LocationsFreebie.Item_Count as numeric(15,2)) Item_Count,"
			"Locations.Name,"
			"cast(sum(ItemSize.Loc_Sale_Count) as numeric(17, 4)) Loc_Sale_Count,"
			"cast(sum(ItemSize.Loc_Discount_Percent) as numeric(17, 4)) Loc_Discount_Percent,"
			"cast(sum(ItemSize.Mem_Sale_Count) as numeric(17, 4)) Mem_Sale_Count,"
			"cast(sum(ItemSize.Mem_Discount_Percent) as numeric(17, 4)) Mem_Discount_Percent "
		"From "
			"ItemSize Left Join Item on "
				"ItemSize.Item_Key = Item.Item_Key "
			"Right join LocationsFreebie on "
				"ItemSize.Size_Name = LocationsFreebie.Size_Name and Item.Item_Name = LocationsFreebie.Item_Name "
			"Left Join Locations on "
				"LocationsFreebie.Location_Key = Locations.Location_Key ";
	if (Locations->Count > 0)
	{
		qrLoyaltyPurchaseCount->SQL->Text	=	qrLoyaltyPurchaseCount->SQL->Text + "Where (" +
												ParamString(Locations->Count, "Locations.Name", "LocationParam") + ")";
	}
	qrLoyaltyPurchaseCount->SQL->Text = qrLoyaltyPurchaseCount->SQL->Text +
		"Group by "
			"Locations.Name,"
			"LocationsFreebie.Item_Name,"
			"LocationsFreebie.Size_Name,"
			"LocationsFreebie.Item_Count "
		"Order by "
			"Locations.Name,LocationsFreebie.Item_Name,LocationsFreebie.Size_Name";
	for (int i=0; i<Locations->Count; i++)
	{
		qrLoyaltyPurchaseCount->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SplitItemsAndSizes(TStrings *ItemList,TStringList *ItemNamesList,TStringList *SizeNamesList)
{
	// Made up of Item_Name + " - " + Size_Name
	AnsiString s;
	int j = 0;
	for (int i=0; i<ItemList->Count; i++)
	{
		s = ItemList->Strings[i];
		j = s.AnsiPos(" - ");
		ItemNamesList->Add( s.SubString(1,j-1));
		s.Delete(1,j + 2);
		SizeNamesList->Add( s );
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::AddInZeroHalfHours(bool isConsumptionByHalfHour)
{
	if (cdsHalfHourlySummary->Active)
	{  // already Used
		cdsHalfHourlySummary->EmptyDataSet();
	}
	else
	{
		try
		{
			cdsHalfHourlySummary->FieldDefs->Clear();
			cdsHalfHourlySummary->FieldDefs->Add("REPORT_TYPE", ftString, 1);
			cdsHalfHourlySummary->FieldDefs->Add("BILLED_LOCATION", ftString, 25);
			cdsHalfHourlySummary->FieldDefs->Add("TIME_STAMP", ftDateTime, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_HALF_HOUR", ftLargeint, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_MINUTE", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_HOUR", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_DAY", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_MONTH", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_YEAR", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("START_TIME", ftTime, 0);
			cdsHalfHourlySummary->FieldDefs->Add("END_TIME", ftTime, 0);
			cdsHalfHourlySummary->FieldDefs->Add("BILL_TOTAL", ftCurrency, 0);
			cdsHalfHourlySummary->FieldDefs->Add("PATRON_COUNT", ftInteger, 0);
			cdsHalfHourlySummary->FieldDefs->Add("SALESQTY", ftCurrency, 0);
            cdsHalfHourlySummary->FieldDefs->Add("SalesIncl", ftCurrency, 0);
			cdsHalfHourlySummary->CreateDataSet();
		 }
		 catch (Exception &E)
		 {
			  ShowMessage("Create cdsHalfHourlySummary table failed, procedure AddInZeroHalfHours");
		 }
	 }
	 cdsHalfHourlySummary->LogChanges = false;

	 Word CurrentHour, CurrentMin, CurrentSec, CurrentMSec;
	 Word PreviousHour, PreviousMin, PreviousSec,PreviousMSec;
	 Word CurrentYear, CurrentMonth, CurrentDay  ,StartMin, StartHour, ExpectedMin, ExpectedHour;
	 Word PreviousYear, PreviousMonth, PreviousDay;
	 AnsiString PreviousLocation,CurrentLocation;
	 TDateTime CurrentTimeStamp, PreviousTimeStamp;
	 bool AfterMidnight = false;

	 qrHalfHoulrySummary->Open();
	 qrHalfHoulrySummary->First();
	 // the times need to be rounded - qrHalfHoulrySummary times result in 12:59:59 and 11:29:59 etc
	 GetRoundedTime(qrHalfHoulrySummary->FieldByName("END_TIME")->AsDateTime, CurrentHour,CurrentMin,CurrentSec,CurrentMSec );
	 GetRoundedTime(qrHalfHoulrySummary->FieldByName("END_TIME")->AsDateTime, PreviousHour,PreviousMin,PreviousSec,PreviousMSec );
	 // For the First time thru
	 ExpectedHour = PreviousHour;
	 ExpectedMin  = PreviousMin;
	 PreviousLocation  = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
	 CurrentLocation   = PreviousLocation;
	 PreviousLocation  = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
     if(isConsumptionByHalfHour)
     {
        PreviousTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP_BILLED")->AsDateTime;
     }
     else
     {
        PreviousTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP")->AsDateTime;
     }
	 CurrentTimeStamp  = PreviousTimeStamp;
	 DecodeDate(PreviousTimeStamp, PreviousYear, PreviousMonth, PreviousDay);
	 DecodeDate(CurrentTimeStamp, CurrentYear, CurrentMonth, CurrentDay);
	 while ( !qrHalfHoulrySummary->Eof )
	 {
		// if the Expected End Time does not equal the Current Records Time
		// then create Half Hour records until they fit into the sequence
	  /*	if	( ExpectedHour < CurrentHour || ExpectedMin < CurrentMin  )
		{
			// Location and Date driven
			if ( PreviousLocation == CurrentLocation && PreviousDay == CurrentDay )
			{
				if (PreviousHour < 5 && CurrentHour > 5)
				{
					// dont print if they have traded after midnight and before opening time
					// ie 1am and the next record is 11am
				}
				else
				{
					bool Completed = false;
					while (!Completed )
					{
						// Calculate a Start Time from the End Time, decrese by a half hour
						StartHour = ExpectedHour;
						StartMin  = ExpectedMin;
						AdjustHalfHour( false,StartHour,StartMin,AfterMidnight );
						cdsHalfHourlySummary->Append();
						cdsHalfHourlySummary->FieldByName("REPORT_TYPE")->AsString       = qrHalfHoulrySummary->FieldByName("REPORT_TYPE")->AsString;
						cdsHalfHourlySummary->FieldByName("BILLED_LOCATION")->AsString   = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
						cdsHalfHourlySummary->FieldByName("TIME_STAMP")->AsDateTime      = qrHalfHoulrySummary->FieldByName("TIME_STAMP_BILLED")->AsDateTime;      //changes..
						cdsHalfHourlySummary->FieldByName("BILL_HALF_HOUR")->AsInteger   = StartMin / 30;
						if (qrHalfHoulrySummary->FieldByName("REPORT_TYPE")->AsString == "C")
						{  // consolidated
							cdsHalfHourlySummary->FieldByName("BILL_MINUTE")->AsInteger   = 1;
							cdsHalfHourlySummary->FieldByName("BILL_HOUR")->AsInteger     = StartHour;
							cdsHalfHourlySummary->FieldByName("BILL_DAY")->AsInteger      = 1;
							cdsHalfHourlySummary->FieldByName("BILL_MONTH")->AsInteger    = 1;
						}
						else
						{  // Detailed
							cdsHalfHourlySummary->FieldByName("BILL_MINUTE")->AsInteger   = StartMin;
							cdsHalfHourlySummary->FieldByName("BILL_HOUR")->AsInteger     = StartHour;
							cdsHalfHourlySummary->FieldByName("BILL_DAY")->AsInteger      = PreviousDay;
							cdsHalfHourlySummary->FieldByName("BILL_MONTH")->AsInteger    = PreviousMonth;
						}
						cdsHalfHourlySummary->FieldByName("BILL_YEAR")->AsInteger        = PreviousYear;
						cdsHalfHourlySummary->FieldByName("START_TIME")->AsDateTime      = EncodeTime(StartHour, StartMin, 0, 0);
						cdsHalfHourlySummary->FieldByName("END_TIME")->AsDateTime        = EncodeTime(ExpectedHour, ExpectedMin, 0, 0);
						cdsHalfHourlySummary->FieldByName("BILL_TOTAL")->AsFloat         = 0.00;
						cdsHalfHourlySummary->FieldByName("PATRON_COUNT")->AsInteger     = 0;
						cdsHalfHourlySummary->FieldByName("SALESQTY")->AsInteger         = 0;
						cdsHalfHourlySummary->Post();
						// increase Expected by half hour
						AdjustHalfHour( true,ExpectedHour,ExpectedMin,AfterMidnight );
						// date has increased when it has gone passed midnight
						//if (AfterMidnight == true)
						//{
							// PreviousTimeStamp = PreviousTimeStamp + 1;
							// DecodeDate(PreviousTimeStamp, PreviousYear, PreviousMonth, PreviousDay);
						  //	 AfterMidnight = false;
						//}
						if ( ExpectedHour == CurrentHour && ExpectedMin == CurrentMin )
						{
							Completed = true;
						}
					}
				}
			}
		}   */
		cdsHalfHourlySummary->Append();
		cdsHalfHourlySummary->FieldByName("REPORT_TYPE")->AsString       = qrHalfHoulrySummary->FieldByName("REPORT_TYPE")->AsString;
		cdsHalfHourlySummary->FieldByName("BILLED_LOCATION")->AsString   = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
        if(isConsumptionByHalfHour)
        {
            cdsHalfHourlySummary->FieldByName("TIME_STAMP")->AsDateTime      = qrHalfHoulrySummary->FieldByName("TIME_STAMP_BILLED")->AsDateTime;
        }
        else
        {
            cdsHalfHourlySummary->FieldByName("TIME_STAMP")->AsDateTime      = qrHalfHoulrySummary->FieldByName("TIME_STAMP")->AsDateTime;
        }
		cdsHalfHourlySummary->FieldByName("BILL_HALF_HOUR")->AsInteger   = qrHalfHoulrySummary->FieldByName("BILL_HALF_HOUR")->AsInteger;
		cdsHalfHourlySummary->FieldByName("BILL_MINUTE")->AsInteger      = qrHalfHoulrySummary->FieldByName("BILL_MINUTE")->AsInteger;
		cdsHalfHourlySummary->FieldByName("BILL_HOUR")->AsInteger        = qrHalfHoulrySummary->FieldByName("BILL_HOUR")->AsInteger;
		cdsHalfHourlySummary->FieldByName("BILL_DAY")->AsInteger         = qrHalfHoulrySummary->FieldByName("BILL_DAY")->AsInteger;
		cdsHalfHourlySummary->FieldByName("BILL_MONTH")->AsInteger       = qrHalfHoulrySummary->FieldByName("BILL_MONTH")->AsInteger;
		cdsHalfHourlySummary->FieldByName("BILL_YEAR")->AsInteger        = qrHalfHoulrySummary->FieldByName("BILL_YEAR")->AsInteger;
		cdsHalfHourlySummary->FieldByName("START_TIME")->AsDateTime      = FixHalfHour(qrHalfHoulrySummary->FieldByName("START_TIME")->AsDateTime);
		cdsHalfHourlySummary->FieldByName("END_TIME")->AsDateTime        = FixHalfHour(qrHalfHoulrySummary->FieldByName("END_TIME")->AsDateTime);
		cdsHalfHourlySummary->FieldByName("BILL_TOTAL")->AsCurrency      = qrHalfHoulrySummary->FieldByName("BILL_TOTAL")->AsCurrency;
		cdsHalfHourlySummary->FieldByName("PATRON_COUNT")->AsInteger     = qrHalfHoulrySummary->FieldByName("PATRON_COUNT")->AsInteger;
		cdsHalfHourlySummary->FieldByName("SALESQTY")->AsCurrency         = qrHalfHoulrySummary->FieldByName("SALESQTY")->AsCurrency;
        cdsHalfHourlySummary->FieldByName("SalesIncl")->AsCurrency         = qrHalfHoulrySummary->FieldByName("SalesIncl")->AsCurrency;
		cdsHalfHourlySummary->Post();
		// Keep Previous records End Time
		GetRoundedTime(qrHalfHoulrySummary->FieldByName("END_TIME")->AsDateTime, PreviousHour,PreviousMin,PreviousSec,PreviousMSec );
		// the Next expected half hour
		ExpectedHour = PreviousHour;
		ExpectedMin  = PreviousMin;
		PreviousLocation = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
        if(isConsumptionByHalfHour)
        {
            PreviousTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP_BILLED")->AsDateTime;
        }
        else
        {
            PreviousTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP")->AsDateTime;
        }
		DecodeDate(PreviousTimeStamp, PreviousYear, PreviousMonth, PreviousDay);
		// get the next record
		qrHalfHoulrySummary->Next();
		// Keep Current records End Time
		GetRoundedTime(qrHalfHoulrySummary->FieldByName("END_TIME")->AsDateTime, CurrentHour,CurrentMin,CurrentSec,CurrentMSec );
		// if the time changes betweeen Previous and Current records then make up the Expected Hour
		if ( PreviousHour != CurrentHour || PreviousMin != CurrentMin )
		{
			AdjustHalfHour( true,ExpectedHour,ExpectedMin,AfterMidnight );
		}
		CurrentLocation = qrHalfHoulrySummary->FieldByName("BILLED_LOCATION")->AsString;
        if(isConsumptionByHalfHour)
        {

            CurrentTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP_BILLED")->AsDateTime;
        }
        else
        {
            CurrentTimeStamp = qrHalfHoulrySummary->FieldByName("TIME_STAMP")->AsDateTime;
        }

		DecodeDate(CurrentTimeStamp, CurrentYear, CurrentMonth, CurrentDay);
	}
	qrHalfHoulrySummary->Close();
}
//---------------------------------------------------------------------------
TDateTime TdmMMReportData::FixHalfHour(TDateTime HalfHour)
{
	Word Hour, Min,Sec,MSec;
	DecodeTime(HalfHour, Hour,Min,Sec,MSec );
	if (Min == 59)
	{
		Min = 0;
		Hour++;
		// after midnight
		if (Hour == 24) Hour = 0;
	}
	if (Min == 29) Min = 30;
	return  EncodeTime(Hour,Min,0,0);
}
//---------------------------------------------------------------------------
void  TdmMMReportData::GetRoundedTime(TDateTime theTime, Word &theHour, Word &theMin, Word &theSec,Word &theMSec )
{
	 // keep the Manual Minutes rounded
	 DecodeTime(theTime, theHour,theMin,theSec,theMSec );
	 if (theMin == 59)
	 {
		theMin = 0;
		theHour++;
		// after midnight
		if (theHour == 24) theHour = 0;
	 }
	 if (theMin == 29) theMin = 30;
}
//---------------------------------------------------------------------------
void  TdmMMReportData::AdjustHalfHour( bool IncreaseHalfHour, Word &theHour, Word &theMin, bool &AfterMidnight )
{
	// routine both icreases and decreases by half hour intervals
	if (IncreaseHalfHour == true)
	{
		// increment in half hour intervals
		if (theMin == 0)
		{
			theMin = 30;
		}
		else //if (ManualMin == 30)
		{
			theMin = 0;

			theHour++;
			// after midnight
			if (theHour == 24)
			{
				theHour = 0;
				AfterMidnight = true;
			}
		}
	}
	else
	{
		 if (theMin == 0)
		 {
			theMin = 30;
			// prior to midnight
			if (theHour == 0)
				theHour = 23;
			else
				theHour--;
		 }
		 else
		 {
			theMin  = 0;
		 }
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::TestData(TIBQuery *theQuery, bool SaveToClipBrd)
{
	if (SaveToClipBrd)
	{
		std::auto_ptr<TClipboard> clipboard(new TClipboard);
		AnsiString s = theQuery->SQL->Text;
		AnsiString s1 = "";
		int j;
		for (int i=1; i<=s.Length(); i++)
		{
			j++;
			// search
			if (j >= 40)
			{

				if (s[i] == ' ')
				{
				  s1 += '\n';
				  j = 0;
				}
				else
				{
				s1 += s[i];
				}
			}
			else
			{
				s1 += s[i];
			}
		}
		clipboard->AsText = s1;
	}
	theQuery->Open();
	bool ShowForm = true;
	TfrmPreviewData *frmPreviewData = new TfrmPreviewData( NULL,theQuery,&ShowForm );
	if (ShowForm) frmPreviewData->ShowModal();
	delete frmPreviewData;

}

void TdmMMReportData::SetupDailySalesByCategories(TDateTime StartTime, TDateTime EndTime, TStrings *Categories)
{        qrDSR->Close();
	     qrDSR->SQL->Text =


 "Select "
			"CategoryGroups.Name Group_Name, "
			"ArcCategories.Category Course_Name, "
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
			"Archive.Size_Name, "
            "MIN(CASE WHEN Archive.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR,  "	//Include Happy Hour
			"Sum(Archive.Qty) Item_Count, "
            "Cast(Sum(Archive.Qty * abs(Archive.BASE_PRICE) ) as Numeric(17,4)) ProductPrice, "	 //price excl

            "Cast(Sum(ABS(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost , "
            "Archive.PRICE_LEVEL0 as Unit_Price, "
            "Cast(Sum(  COALESCE(AOT.VAT,0) ) as Numeric(17,4)) VAT, "
            "Cast(Sum( COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) ServiceCharge, "

            "cast(Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(Archive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "


              " Cast((cast(Sum(Archive.QTY * abs(Archive.BASE_PRICE))as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total  "




          	"From "
			"Security Left Join Archive on 	Security.Security_Ref = Archive.Security_Ref "
			"inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
			"Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

   " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		
        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "

			 "(COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
			 "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') and   "

            "Archive.TIME_STAMP_BILLED >= :StartTime and  "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
		  "	Security.Security_Event = 'Ordered By' "   ;

	if (Categories->Count > 0)
	{
		qrDSR->SQL->Text	=	qrDSR->SQL->Text + "and (" +
		ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
qrDSR->SQL->Text=	qrDSR->SQL->Text +
         "Group By "
			"CategoryGroups.Name, "
			"ArcCategories.Category, "
			"Archive.Item_Name, "
			"Archive.Size_Name , "
"Archive.HAPPY_HOUR ,Archive.PRICE_LEVEL0  "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

       "Union All "


 "Select "
			"CategoryGroups.Name Group_Name, "
			"ArcCategories.Category Course_Name, "
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, "
			"DayArchive.Size_Name, "
           "MIN(CASE WHEN DayArchive.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR,  "		   //Happy hour
			"Sum(DayArchive.Qty) Item_Count, "
            "Cast(Sum(DayArchive.Qty * abs(DAYARCHIVE.BASE_PRICE)  ) as Numeric(17,4)) ProductPrice, "        //price excl

            "Cast(Sum(ABS(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost , "
            "DayArchive.PRICE_LEVEL0 as Unit_Price, "
            "Cast(Sum( COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
            "Cast(Sum(  COALESCE( AOT.ServiceCharge,0)  + COALESCE( AOT.OtherServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "	//service charge

            "cast(Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(DayArchive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "


                        " Cast((cast(Sum(DayArchive.QTY * abs(DayArchive.BASE_PRICE))as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total  "
          	"From "
			"Security Left Join DayArchive on 	Security.Security_Ref = DayArchive.Security_Ref "
			"inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DayArchive.ARCBILL_KEY "
			"Left Join ArcCategories on DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

                        "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "



        " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY'or security.SECURITY_EVENT='Cancel') and  "

			  "(COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
			 "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  and "
        "DayArchive.TIME_STAMP_BILLED >= :StartTime and  "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
		  "	Security.Security_Event = 'Ordered By' "   ;

	if (Categories->Count > 0)
	{
		qrDSR->SQL->Text	=	qrDSR->SQL->Text + "and (" +
		ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
qrDSR->SQL->Text=	qrDSR->SQL->Text +
         "Group By "
			"CategoryGroups.Name, "
			"ArcCategories.Category, "
			"DayArchive.Item_Name, "
			"DayArchive.Size_Name , "
         "DayArchive.HAPPY_HOUR , DayArchive.PRICE_LEVEL0   "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 " ;



   for (int i=0; i<Categories->Count; i++)
	{
		qrDSR->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}
	qrDSR->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSR->ParamByName("EndTime")->AsDateTime		= EndTime;

}


void TdmMMReportData::SetupDailySalesByMenu(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST)
{

            qrDSR->Close();
	     qrDSR->SQL->Text =


 "Select "
			"CategoryGroups.Name Group_Name, "
			"ArcCategories.Category Course_Name, "
			"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
			"Archive.Size_Name, "
            "MIN(CASE WHEN Archive.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR,  "	//Include Happy Hour
			"Sum(Archive.Qty) Item_Count, "
            "Cast(Sum(Archive.Qty * ABS(Archive.BASE_PRICE)) as Numeric(17,4)) ProductPrice, "	 //price excl
            "Cast(Sum(ABS(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost , "
            "Archive.PRICE_LEVEL0 as Unit_Price, "
            "Cast(Sum(  COALESCE(AOT.VAT,0) ) as Numeric(17,4)) VAT, "
            "Cast(Sum( COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) ServiceCharge, "
            "cast(Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(Archive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
           " Cast((cast(Sum(Archive.QTY * ABS(Archive.BASE_PRICE))as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total  "


          	"From "
			"Security Left Join Archive on 	Security.Security_Ref = Archive.Security_Ref "
			"inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
			"Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

   " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	 " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "

			 "(COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
			 "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') and   "

        "Archive.TIME_STAMP_BILLED >= :StartTime and  "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
		  "	Security.Security_Event = 'Ordered By' "   ;

	if (Menus->Count > 0)
	{
		qrDSR->SQL->Text	=	qrDSR->SQL->Text + "and (" +
												ParamString(Menus->Count, "Archive.Menu_Name", "MenuParam") + ")";
	}
	qrDSR->SQL->Text		=	qrDSR->SQL->Text +
  "Group By "
			"CategoryGroups.Name, "
			"ArcCategories.Category, "
			"Archive.Item_Name, "
			"Archive.Size_Name , "
"Archive.HAPPY_HOUR ,Archive.PRICE_LEVEL0   "
		"Having "
			"Count(Archive.Archive_Key) > 0 "

   "Union All "

"Select "
			"CategoryGroups.Name Group_Name, "
			"ArcCategories.Category Course_Name, "
			"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, "
			"DayArchive.Size_Name, "
           "MIN(CASE WHEN DayArchive.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR,  "		   //Happy hour
			"Sum(DayArchive.Qty) Item_Count, "
            "Cast(Sum(DayArchive.Qty * ABS(DAYARCHIVE.BASE_PRICE)  ) as Numeric(17,4)) ProductPrice, "        //price excl
            "Cast(Sum(ABS(DayArchive.Cost) * DayArchive.Qty) as Numeric(17,4)) Cost , "
            "DayArchive.PRICE_LEVEL0 as Unit_Price, "
            "Cast(Sum( COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
            "Cast(Sum(  COALESCE( AOT.ServiceCharge,0)  + COALESCE( AOT.OtherServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "	//service charge
            "cast(Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(DayArchive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "                   
  

                        " Cast((cast(Sum(DayArchive.QTY * ABS(DayArchive.BASE_PRICE))as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total  "

     	"From "
			"Security Left Join DayArchive on 	Security.Security_Ref = DayArchive.Security_Ref "
			"inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DayArchive.ARCBILL_KEY "
			"Left Join ArcCategories on DayArchive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

                        "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "


	 " Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on  SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "

			  "(COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
			 "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  and "


        "DayArchive.TIME_STAMP_BILLED >= :StartTime and  "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
		  "	Security.Security_Event = 'Ordered By' "   ;


	if (Menus->Count > 0)
	{
		qrDSR->SQL->Text	=	qrDSR->SQL->Text + "and (" +
												ParamString(Menus->Count, "DayArchive.Menu_Name", "MenuParam") + ")";
	}

qrDSR->SQL->Text=	qrDSR->SQL->Text +
          "Group By "
			"CategoryGroups.Name, "
			"ArcCategories.Category, "
			"DayArchive.Item_Name, "
			"DayArchive.Size_Name , "
         "DayArchive.HAPPY_HOUR , DayArchive.PRICE_LEVEL0  "
		"Having "
			"Count(DayArchive.Archive_Key) > 0 " ;

	for (int i=0; i<Menus->Count; i++)
	{
		qrDSR->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}

	qrDSR->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSR->ParamByName("EndTime")->AsDateTime		= EndTime;
}

void TdmMMReportData::SetupDailySales(TDateTime StartTime, TDateTime EndTime)
{
qrDSRInvoice->Close();
	     qrDSRInvoice->SQL->Text =
	"SELECT "

				"DAB.time_stamp ,  "
            "Extract(year From  DAB.TIME_STAMP) Item_Year, "
				"Extract(month From  DAB.TIME_STAMP) Item_Month, "
				"Extract(Day From  DAB.TIME_STAMP) Item_Day, "
				"Extract(hour From  DAB.TIME_STAMP) Item_Hour, "
				"Extract(minute From  DAB.TIME_STAMP) Item_Minute, "
				"Extract(Second From  DAB.TIME_STAMP) Item_Second , "
				"DAB.invoice_number ,  "
				"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "

				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE )  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
				"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
				"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
		"FROM DAYARCBILL DAB "
			"INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY  "

                "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "


			"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
           " Where DA.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
		" ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) and  "


"DA.Time_Stamp_billed >= :StartTime and  "
			"DA.Time_Stamp_billed < :EndTime  "    ;
 qrDSRInvoice->SQL->Text		=	qrDSRInvoice->SQL->Text +
"GROUP BY  DAB.time_stamp,DAB.invoice_number,    da.ITEM_NAME,da.SIZE_NAME,da.QTY    "

" UNION ALL "



				"SELECT "
            	"DAB.time_stamp ,  "
                "Extract(year From  DAB.TIME_STAMP) Item_Year, "
				"Extract(month From  DAB.TIME_STAMP) Item_Month, "
				"Extract(Day From  DAB.TIME_STAMP) Item_Day, "
				"Extract(hour From  DAB.TIME_STAMP) Item_Hour, "
				"Extract(minute From  DAB.TIME_STAMP) Item_Minute, "
				"Extract(Second From  DAB.TIME_STAMP) Item_Second , "
				"DAB.invoice_number ,  "
				"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE )  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
				"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
				"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "

" FROM ARCBILL DAB "
"INNER JOIN ARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "


    "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
"LEFT JOIN ( "
						"SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
						"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
                 " Where DA.ARCHIVE_KEY not in (Select     ARCHIVE.ARCHIVE_KEY from ARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
	" ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) "
"and "

"DA.Time_Stamp_billed >= :StartTime and  "
			"DA.Time_Stamp_billed < :EndTime  "    ;
 qrDSRInvoice->SQL->Text		=	qrDSRInvoice->SQL->Text +
"GROUP BY  DAB.time_stamp,DAB.invoice_number,    da.ITEM_NAME,da.SIZE_NAME,da.QTY  ; " ;

	qrDSRInvoice->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRInvoice->ParamByName("EndTime")->AsDateTime		= EndTime;


}


void TdmMMReportData::SetupDailySalesByInvoice(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, bool IncGST)
{
  qrDSRInvoice->Close();
	     qrDSRInvoice->SQL->Text =
                 "SELECT "
"DAB.TOTAL FTOTAL , "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE) ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
			"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "

                "FROM DAYARCBILL DAB "
"INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
	"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
          " Where DA.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
	"  ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) "

 "and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrDSRInvoice->SQL->Text =	qrDSRInvoice->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}
	qrDSRInvoice->SQL->Text		=	qrDSRInvoice->SQL->Text +
" GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL , DAB.ARCBILL_KEY "

" UNION ALL "

 "SELECT "
"DAB.TOTAL FTOTAL , "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE)  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
			"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
" FROM ARCBILL DAB "
"INNER JOIN ARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "


  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
"LEFT JOIN ( "
						"SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
						"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
               " Where DA.ARCHIVE_KEY not in (Select     ARCHIVE.ARCHIVE_KEY from ARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
"  ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrDSRInvoice->SQL->Text =	qrDSRInvoice->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}

	qrDSRInvoice->SQL->Text		=	qrDSRInvoice->SQL->Text +
"GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL  , DAB.ARCBILL_KEY "
                     "Order BY 4 "        ;

for (int i=0; i<Invoices->Count; i++)
	{
		qrDSRInvoice->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}


	qrDSRInvoice->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRInvoice->ParamByName("EndTime")->AsDateTime		= EndTime;

}

   void TdmMMReportData::SetupDailySalesByInvoice( AnsiString InvoiceNumber)
     {
qrDSRInvoice->Close();
	     qrDSRInvoice->SQL->Text =
         "SELECT "
"DAB.TOTAL FTOTAL , "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE)  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
			"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
"FROM DAYARCBILL DAB "
"INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
	"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
            " Where DA.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
	" ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) "

 "and "

"DAB.Invoice_Number = :Invoice_Number "
" GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL  "

" UNION ALL "

 "SELECT "
"DAB.TOTAL FTOTAL , "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * ABS(DA.BASE_PRICE)   ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
				"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *ABS(DA.BASE_PRICE)  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
" FROM ARCBILL DAB "
"INNER JOIN ARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM  ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
"LEFT JOIN ( "
						"SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
						"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
" Where DA.ARCHIVE_KEY not in (Select     ARCHIVE.ARCHIVE_KEY from ARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
"  ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) and "



"DAB.Invoice_Number = :Invoice_Number "
"GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL "   ;
			 qrDSRInvoice->ParamByName("Invoice_Number")->AsString = InvoiceNumber;

}




void TdmMMReportData::SetupConsolidatedSales(TDateTime StartTime, TDateTime EndTime,TStrings *Locations)
{
try
	{
       	qrDSRTrans->Close();
    if(qrDSRTrans->Transaction->Active ==true)
  {
   qrDSRTrans -> Transaction -> Commit();
  }


  try{

	  /*	sqlAddLK->Close();
        sqlAddLK -> Transaction -> StartTransaction();
		sqlAddLK->SQL->Text = "select 1 from rdb$relations where rdb$relation_name = 'DSRREPORT' ";
		sqlAddLK->ExecQuery();
		int	NextCode = sqlAddLK->Fields[0]->AsInteger;
       NextCode=   sqlAddLK->FieldByName("CONSTANT")->AsInteger;
      sqlAddLK -> Transaction -> Commit();


        AnsiString DropStmt="DROP TABLE DSRREPORT";
		qrDSRTrans -> Transaction -> StartTransaction();
		qrDSRTrans -> SQL -> Clear();
		qrDSRTrans -> SQL -> Add(DropStmt);
		qrDSRTrans-> ExecSQL ();
		qrDSRTrans -> Transaction -> Commit();
         */
	   //	sqlAddLK->Close();
  }
  catch (...)
		 { qrDSRTrans -> Transaction -> Commit();

		 }

 if(qrDSRTrans->Transaction->Active ==false)
{
   AnsiString CreateTableStmt="execute procedure CREATE_DSR_PIVOT ( 'DSRREPORT', :StartTime ,:EndTime ) ";
   qrDSRTrans -> Close();
   qrDSRTrans -> SQL -> Clear();
qrDSRTrans    -> SQL -> Add(CreateTableStmt);
 qrDSRTrans->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRTrans->ParamByName("EndTime")->AsDateTime		= EndTime;
   qrDSRTrans -> Transaction -> StartTransaction();
   qrDSRTrans-> ExecSQL ();
   qrDSRTrans -> Transaction -> Commit();

   AnsiString populatedatastmt;
   populatedatastmt = "execute procedure POPULATE_DSR_PIVOT ( 'DSRREPORT', :StartTime ,:EndTime ) ";

   qrDSRTrans-> Close ();
   qrDSRTrans -> SQL -> Clear();
   qrDSRTrans -> SQL -> Add(populatedatastmt);

   qrDSRTrans->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRTrans->ParamByName("EndTime")->AsDateTime		= EndTime;

   qrDSRTrans -> Transaction -> StartTransaction();
   qrDSRTrans-> ExecSQL ();
   qrDSRTrans -> Transaction -> Commit();
   	qrDSRTrans->Close();
}

	     qrDSRConsolidated->SQL->Text =
"  SELECT * "
" from DSRREPORT  "
	" WHERE "
			"DSRREPORT.TIME_STAMP >= :StartTime and "
			"DSRREPORT.TIME_STAMP < :EndTime  ";
            if (Locations->Count > 0)
	{
		qrDSRConsolidated->SQL->Text	=	qrDSRConsolidated->SQL->Text + "and (" +
												ParamString(Locations->Count, "DSRREPORT.LOCATION", "LocationParam") + ")";
	}
      	for (int i=0; i<Locations->Count; i++)
	{
		qrDSRConsolidated->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	qrDSRConsolidated->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRConsolidated->ParamByName("EndTime")->AsDateTime		= EndTime;

}
	__finally
	{

	//qrDSRConsolidated -> Transaction -> Commit();
	}
}



//---------------------------------------------------------------------------


void TdmMMReportData::SetupComplimentary(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices)       //MM-3908
{
qrComplimentary->Close();
	     qrComplimentary->SQL->Text =
                 "SELECT "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
            "da.DISCOUNT_REASON, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * DA.BASE_PRICE ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
			"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *DA.BASE_PRICE  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "

                "FROM DAYARCBILL DAB "
"INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
	"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
	"WHERE ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) = 'Complimentary'  )) "

 "and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrComplimentary->SQL->Text =	qrComplimentary->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}
	qrComplimentary->SQL->Text		=	qrComplimentary->SQL->Text +
" GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME, da.DISCOUNT_REASON, da.SIZE_NAME,DAB.TOTAL , DAB.ARCBILL_KEY "

" UNION ALL "

 "SELECT "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
            "da.DISCOUNT_REASON, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * DA.BASE_PRICE  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
            "cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *DA.BASE_PRICE  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
" FROM ARCBILL DAB "
"INNER JOIN ARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "


  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
"LEFT JOIN ( "
						"SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
						"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
"WHERE ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) = 'Complimentary'  )) and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrComplimentary->SQL->Text =	qrComplimentary->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}

	qrComplimentary->SQL->Text		=	qrComplimentary->SQL->Text +
"GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME, da.DISCOUNT_REASON, da.SIZE_NAME,DAB.TOTAL  , DAB.ARCBILL_KEY "
                     "Order BY 3 "        ;

for (int i=0; i<Invoices->Count; i++)
	{
		qrComplimentary->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}


	qrComplimentary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrComplimentary->ParamByName("EndTime")->AsDateTime		= EndTime;


}







void TdmMMReportData::SetupNonChargeable(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices)
{
qrComplimentary->Close();
	     qrComplimentary->SQL->Text =
                 "SELECT "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
            "da.DISCOUNT_REASON, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * DA.BASE_PRICE ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
			"cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *DA.BASE_PRICE  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "

                "FROM DAYARCBILL DAB "
"INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
	"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
	"WHERE ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Complimentary' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable'  )) "

 "and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrComplimentary->SQL->Text =	qrComplimentary->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}
	qrComplimentary->SQL->Text		=	qrComplimentary->SQL->Text +
" GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME, da.DISCOUNT_REASON, da.SIZE_NAME,DAB.TOTAL , DAB.ARCBILL_KEY "

" UNION ALL "

 "SELECT "
          " DAB.time_stamp , "
          " DAB.invoice_number, "
"  DAB.ARCBILL_KEY, "
           	"Sum(da.Qty) Qty, "
				"da.ITEM_NAME, "
            "da.DISCOUNT_REASON, "
				"da.SIZE_NAME,  "
				"Cast(Sum(DA.Qty * DA.BASE_PRICE  ) as Numeric(17,4)) priceExcl, "
				"Cast(Sum(COALESCE( AOT.VAT ,0)) as Numeric(17,4)) VAT, "
				"Cast(Sum( COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
				"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
            "cast(Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
                 "cast(Sum( COALESCE(DA.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
					"Cast(cast(Sum( DA.QTY *DA.BASE_PRICE  )as Numeric(17,4))+Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) SalesIncl "
" FROM ARCBILL DAB "
"INNER JOIN ARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "


  "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY "
"LEFT JOIN ( "
						"SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
						"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY  "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
"WHERE ((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Complimentary' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable'  )) and "

"DAB.Time_Stamp >= :StartTime and  "
			"DAB.Time_Stamp < :EndTime  "       ;
		if (Invoices->Count)
	{
		qrComplimentary->SQL->Text =	qrComplimentary->SQL->Text + "And (" +

ParamString(Invoices->Count, "DAB.Invoice_Number", "InvoiceParam") + ")";
	}

	qrComplimentary->SQL->Text		=	qrComplimentary->SQL->Text +
"GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME, da.DISCOUNT_REASON, da.SIZE_NAME,DAB.TOTAL  , DAB.ARCBILL_KEY "
                     "Order BY 3 "        ;

for (int i=0; i<Invoices->Count; i++)
	{
		qrComplimentary->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}


	qrComplimentary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrComplimentary->ParamByName("EndTime")->AsDateTime		= EndTime;
}



void TdmMMReportData::SetupDailySalesByCategoriesDate(TDateTime StartTime, TDateTime EndTime, TStrings *Categories)

{
qrDSRMenuDay->Close();
	qrDSRMenuDay->SQL->Text =
            "Select "
            "ARCHIVE.MENU_NAME, "
            "CategoryGroups.Name Group_Name, "
            "ArcCategories.Category , "
            "Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
            "Archive.Size_Name, "
            "MIN(CASE WHEN Archive.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR, "
            "Extract (Day From ARCBILL.Time_Stamp) Order_Day, "
            "Extract (Month From ARCBILL.Time_Stamp) Order_Month, "
            "Extract (Year From ARCBILL.Time_Stamp) Order_Year, "
            "Sum((Archive.Qty)) Item_Count,  "
            "Cast(Sum(Archive.Qty * ABS(Archive.BASE_PRICE)) as Numeric(17,4)) ProductPrice, "
            "Cast(Sum(ABS(Archive.Cost) * Archive.Qty) as Numeric(17,4)) Cost , Archive.PRICE_LEVEL0 as Unit_Price, "
            "Cast(Sum(  COALESCE(AOT.VAT,0) ) as Numeric(17,4)) VAT, "
            "Cast(Sum(  COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
            "cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax, "
              "cast(Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(Archive.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
                " Cast((cast(Sum(Archive.QTY * ABS(Archive.BASE_PRICE) )as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total , "
            "Archive.Price, "
            "Archive.QTY  "
		"From  "
			"Security Left Join Archive on 	Security.Security_Ref = Archive.Security_Ref   "
			"inner join ARCBILL on ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY  "
			"Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
			"left join MENU on ARCHIVE.MENU_NAME=MENU.MENU_NAME "
			"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

 " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "


        " Where ARCHIVE.ARCHIVE_KEY not in (Select     archive.ARCHIVE_KEY from archive left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
			"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
			"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and   "

			"Security.Security_Event = 'Ordered By'   "
            "and "
 "Archive.PRICE<>0  "
"  and "
"ARCHIVE.TIME_STAMP_BILLED >= :StartTime and  "
			"ARCHIVE.TIME_STAMP_BILLED < :EndTime  " ;

if (Categories->Count > 0)
	{
		qrDSRMenuDay->SQL->Text	=	qrDSRMenuDay->SQL->Text + "and (" +
		ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
	qrDSRMenuDay->SQL->Text		=	qrDSRMenuDay->SQL->Text +
 "Group By "
			"CategoryGroups.Name, ARCHIVE.MENU_NAME,Order_Year,Order_Month,Order_Day , "
			"ArcCategories.Category, "
			"Archive.Item_Name, "
			"Archive.Size_Name , Archive.HAPPY_HOUR , "
            "Archive.PRICE_LEVEL0 ,Archive.Discount, "
            "Archive.Price,Archive.QTY  		Having "
			"Count(Archive.Archive_Key) > 0 "



      "Union All "


 	"Select "
		"DAYARCHIVE.MENU_NAME, "
		"CategoryGroups.Name Group_Name, "
		"ArcCategories.Category , "
		"Cast(DAYARCHIVE.Item_Name as VarChar(50)) Item_Name, "
		"DAYARCHIVE.Size_Name, "
		"MIN(CASE WHEN DAYARCHIVE.HAPPY_HOUR = 'T' THEN 'Yes' else '-' END) AS HAPPY_HOUR,  "
		"Extract (Day From DAYARCBILL.Time_Stamp) Order_Day, "
		"Extract (Month From DAYARCBILL.Time_Stamp) Order_Month, "
		"Extract (Year From DAYARCBILL.Time_Stamp) Order_Year, "
		"Sum((DAYARCHIVE.Qty)) Item_Count,  "
		"Cast(Sum(DAYARCHIVE.Qty * ABS(DAYARCHIVE.BASE_PRICE)  ) as Numeric(17,4)) ProductPrice, "
		"Cast(Sum(ABS(DAYARCHIVE.Cost) * DAYARCHIVE.Qty) as Numeric(17,4)) Cost , DAYARCHIVE.PRICE_LEVEL0 as Unit_Price, "
		"Cast(Sum(   COALESCE(AOT.VAT,0) ) as Numeric(17,4)) VAT, "
		"Cast(Sum(  COALESCE( AOT.ServiceCharge,0) ) as Numeric(17,4)) ServiceCharge, "
		"cast(Sum(COALESCE( AOT.OtherServiceCharge,0)) as numeric(17, 4)) as ServiceChargeTax,    "
	  "cast(Sum( COALESCE(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0))as numeric(17, 4)) AS Discount, "
             "cast(Sum( COALESCE(DAYARCHIVE.TAX_ON_DISCOUNT,0))as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
	                 
   " Cast((cast(Sum(DayArchive.QTY * ABS(DayArchive.BASE_PRICE))as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+   "
             " Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0))   "
         "  ) as Numeric(17,4)) Total , "
         "DayArchive.Price, "
            "DayArchive.QTY  "
 	"From  "
			"Security Left Join DAYARCHIVE on 	Security.Security_Ref = DAYARCHIVE.Security_Ref   "
			"inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY  "
			"Left Join ArcCategories on DAYARCHIVE.Category_Key = ArcCategories.Category_Key  "
			"Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
			"left join MENU on DAYARCHIVE.MENU_NAME=MENU.MENU_NAME "
			"LEFT JOIN ( "
					 "SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DAYARCHIVE.ARCHIVE_KEY = AOT.ARCHIVE_KEY "

     "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		" Where DAYARCHIVE.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and  "
			"COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Complimentary' and "
			"COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) <> 'Non-Chargeable' and "


 "DAYARCHIVE.PRICE<>0 and "
			"Security.Security_Event = 'Ordered By'  "
             "  and "
"DAYARCHIVE.TIME_STAMP_BILLED >= :StartTime and  "
			"DAYARCHIVE.TIME_STAMP_BILLED < :EndTime  " ;;

	if (Categories->Count > 0)
	{
		qrDSRMenuDay->SQL->Text	=	qrDSRMenuDay->SQL->Text + "and (" +
		ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
qrDSRMenuDay->SQL->Text=	qrDSRMenuDay->SQL->Text +
 "Group By "
       "CategoryGroups.Name, DAYARCHIVE.MENU_NAME,Order_Year,Order_Month,Order_Day , "
			"ArcCategories.Category, "
			"DAYARCHIVE.Item_Name, "
			"DAYARCHIVE.Size_Name , DAYARCHIVE.HAPPY_HOUR , "
            "DAYARCHIVE.PRICE_LEVEL0 ,DAYARCHIVE.Discount, "
            "DAYARCHIVE.Price,DAYARCHIVE.QTY   		Having "
			"Count(DAYARCHIVE.ARCHIVE_KEY) > 0  ; " ;


for (int i=0; i<Categories->Count; i++)
	{
		qrDSRMenuDay->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}

	qrDSRMenuDay->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRMenuDay->ParamByName("EndTime")->AsDateTime		= EndTime;
}


void TdmMMReportData::SetupTaxProfile(TStrings *Categories)
{

	qrTaxProfile->Close();
	qrTaxProfile->SQL->Text =
	"SELECT Item.ITEM_KEY,ITEMSIZE.SIZES_KEY, Item.COURSE_KEY, Item.ITEM_ID,CATEGORYGROUPS.NAME CATEGORYGROUPS ,ArcCategories.Category ,  Item.ITEM_NAME , ITEMSIZE.SIZE_NAME,  TAXPROFILES.NAME,TAXPROFILES.RATE "
	"FROM ITEM Item "
	"left join ITEMSIZE  on Item.ITEM_KEY=ITEMSIZE.ITEM_KEY "
	"left join TAXPROFILES_ITEMSIZE on ITEMSIZE.ITEMSIZE_KEY=TAXPROFILES_ITEMSIZE.ITEMSIZE_KEY "
	"left join TAXPROFILES on TAXPROFILES_ITEMSIZE.PROFILE_KEY=TAXPROFILES.PROFILE_KEY "
	"left join ARCCATEGORIES on ARCCATEGORIES.CATEGORY_KEY=ITEMSIZE.CATEGORY_KEY "
	"left join CATEGORYGROUPS on CATEGORYGROUPS.CATEGORYGROUPS_KEY=ARCCATEGORIES.CATEGORYGROUPS_KEY "

     "where Item.ITEM_KEY<>0"  ;
if (Categories->Count > 0)
	{
		qrTaxProfile->SQL->Text	=	qrTaxProfile->SQL->Text + "and (" +
		ParamString(Categories->Count, "ArcCategories.Category", "CategoryParam") + ")";
	}
qrTaxProfile->SQL->Text=	qrTaxProfile->SQL->Text +

 	"group by Item.ITEM_KEY, Item.COURSE_KEY, Item.ITEM_ID,CATEGORYGROUPS.NAME, ArcCategories.Category, Item.ITEM_NAME ,ITEMSIZE.SIZES_KEY, ITEMSIZE.SIZE_NAME,TAXPROFILES.NAME,TAXPROFILES.RATE " ;



 for (int i=0; i<Categories->Count; i++)
	{
		qrTaxProfile->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
	}


}

void TdmMMReportData::SetupSalesSummaryB(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
    qrSalesSummaryB->Close();
    qrSalesSummaryB->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text +
    // Total Discount
    "COALESCE((SELECT SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) AS DISCOUNT, "

    // Total VAT
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 0 AND ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime),0) + "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 4 AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS VAT, "

    // Total Service Charge
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (2,3) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS SERVICECHARGE, "

    // Total Local Tax
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 4 AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS LOCALTAX, "

    // Total Refund
    "CAST(COALESCE((SELECT SUM(CAST(ARCHIVE.PRICE * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) AS NUMERIC(17,4)) AS TOTALREFUNDS, "

    // Total Other Refund (non-cash)
    "MIN(COALESCE((SELECT SUM(CASHREF.PRICE * CASHREF.QTY) FROM "
    "(SELECT ARCHIVE.PRICE, ARCHIVE.QTY FROM ARCBILLPAY "
    "LEFT JOIN ARCHIVE ON ARCBILLPAY.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCBILLPAY.PAY_TYPE != 'Cash' AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCHIVE.ARCHIVE_KEY, SECURITY.SECURITY_REF, ARCHIVE.PRICE, ARCHIVE.QTY) AS CASHREF),0)) AS OTHERREFUNDS, "

    // Total Patron Count
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS TOTALPATRONCOUNT, "

    // Total Revenue  (Total + Service Charge + Local Tax + Refund)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(CAST(ARCHIVE.PRICE * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) AS TOTALREVENUE, "

    // Net Revenue  (Total + Service Charge + Local Tax)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS NETREVENUE, "

    // Total Void/Cancel and Discount (Refunds + Discounts)
    "COALESCE((SELECT SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime), 0) + "
    "COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE_LEVEL0 * -1) AS NUMERIC(17,4)) FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
    "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY WHERE SECURITY.SECURITY_EVENT IN ('Cancel','CancelY') AND "
    "ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) AS TREFUNDDISCOUNT, "

    // Total Gross (Revenue + Service Charge + Local Tax + Total Cancel + Discounts + Total Refund)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE_LEVEL0 * -1) AS NUMERIC(17,4)) FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
    "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY WHERE SECURITY.SECURITY_EVENT IN ('Cancel','CancelY') AND "
    "ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) - "
    "COALESCE((SELECT SUM(CAST(ARCHIVE.PRICE * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime), 0) AS GROSS, "

    // Total Net Sales (Revenue - Service Charge - Local Tax - Vat)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS NETSALES, "

    // Total Cash Refunds (Total Refund - Other Refund)
    "MIN(COALESCE((SELECT SUM(CASHREF.PRICE * CASHREF.QTY) FROM "
    "(SELECT ARCHIVE.PRICE, ARCHIVE.QTY FROM ARCBILLPAY "
    "LEFT JOIN ARCHIVE ON ARCBILLPAY.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCBILLPAY.PAY_TYPE = 'Cash' AND ARCBILLPAY.NOTE != 'Total Change.' AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCHIVE.ARCHIVE_KEY, SECURITY.SECURITY_REF, ARCHIVE.PRICE, ARCHIVE.QTY) AS CASHREF),0)) AS CASHREFUNDS, "

    // Total Cancel
    "COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE_LEVEL0 * -1) AS NUMERIC(17,4)) FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
    "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY WHERE SECURITY.SECURITY_EVENT IN ('Cancel','CancelY') AND "
    "ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) AS TOTALCANCEL, "

    // Total Tax Exempt
    "MIN(COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE*ARCHIVE.QTY) AS NUMERIC(16,4)) "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 0 AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime) - "
    "(SELECT SUM(A.TAX_VALUE) FROM "
    "(SELECT ARCORDERTAXES.ARCHIVE_KEY, ARCORDERTAXES.TAX_VALUE, ARCORDERTAXES.TAX_TYPE "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime) AS A LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = A.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_VALUE = 0 AND ARCORDERTAXES.TAX_TYPE = 0 AND A.TAX_TYPE IN (2,3)),0)) AS TAXEXEMPT, "

    // Total Vatable
    "COALESCE((SELECT CAST(SUM(ARCBILL.TOTAL) AS NUMERIC(17,4)) FROM ARCBILL WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) - "

    "MIN(COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE*ARCHIVE.QTY) AS NUMERIC(17,4)) "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 0 AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime) - "
    "(SELECT SUM(A.TAX_VALUE) FROM "
    "(SELECT ARCORDERTAXES.ARCHIVE_KEY, ARCORDERTAXES.TAX_VALUE, ARCORDERTAXES.TAX_TYPE "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime) AS A LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = A.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_VALUE = 0 AND ARCORDERTAXES.TAX_TYPE = 0 AND A.TAX_TYPE IN (2,3)),0)) - "

    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) AS VATABLE, "

    // Total Transaction (Ending Invoice Number - Beggining Invoice Number)
    "COALESCE((SELECT FIRST 1 ARCBILL.INVOICE_NUMBER FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime ORDER BY ARCBILL.ARCBILL_KEY ASC),0) FIRSTINVOICE, "
    "COALESCE((SELECT FIRST 1 ARCBILL.INVOICE_NUMBER FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime ORDER BY ARCBILL.ARCBILL_KEY DESC),0) AS LASTINVOICE, "

    "COALESCE((SELECT COUNT(ARCBILL.INVOICE_NUMBER) FROM ARCBILL WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) - "
    "COALESCE((SELECT COUNT(ARCBILL.INVOICE_NUMBER) FROM ARCBILL WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "AND ARCBILL.TOTAL = 0 AND ARCBILL.DISCOUNT != 0),0) AS TOTALTRANS, "

    // Total Average Per Guest (Total Revenue / Patron Count)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) / "
    "COALESCE(CASE WHEN (SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime) = 0 THEN 1 ELSE "
    "(SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime) END ,1) AS AVERAGEPERGUEST, "

    // Total Patron Count every Breakfast (5am to 11am)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 5 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 11 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS BREAKFASTCOUNT, "

    // Total Amount every Breakfast (5am to 11am)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 5 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 11 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS BREAKFASTAMOUNT, "

    // Total Patron Count every Lunch (11am to 2pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 11 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 14 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS LUNCHCOUNT, "

    // Total Amount every Lunch (11am to 2pm)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 11 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 14 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS LUNCHAMOUNT, "

    // Total Patron Count every Merienda(Snacks) (2pm to 7pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 14 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 19 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS MERIENDACOUNT, "

    // Total Amount every Merienda(Snacks) (2pm to 7pm)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 14 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 19 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS MERIENDAAMOUNT, "

    // Total Patron Count every Dinner (7pm to 5am)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 19 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS DINNERCOUNT, "

    // Total Amount every Dinner (7pm to 5am)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 19 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS DINNERAMOUNT, "

    // Total Amount for All Hours (Breakfast, Lunch, Merienda, Dinner)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS TOTALCHECKCOUNT, "

    // Other Discount that was not added in discount group
    "COALESCE((SELECT SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = '' AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) AS OTHERDISC "

    "FROM ARCHIVE ";

    if (Locations && Locations->Count > 0)
    {
        qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text + "WHERE (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text +
    "GROUP BY ";

    if (Locations)
    {
        qrSalesSummaryB->SQL->Text = qrSalesSummaryB->SQL->Text +
        "ARCHIVE.ORDER_LOCATION ";
    }
    qrSalesSummaryB->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesSummaryB->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesSummaryB->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrAccumulatedTotals->Close();
    qrAccumulatedTotals->SQL->Text = "Select ";

    qrAccumulatedTotals->SQL->Text = qrAccumulatedTotals->SQL->Text +
    "FIRST 1 "
    "ZEDS.TERMINAL_EARNINGS TODAYSEARNINGS, "
    "ZEDS.ZED_TOTAL ACCUMULATEDTOTAL, "
    "ZEDS.ZED_TOTAL - "
    "ZEDS.TERMINAL_EARNINGS AS OPENING_BALANCE "
    "FROM ZEDS "
    "WHERE ZEDS.TIME_STAMP >= :StartTime AND ZEDS.TIME_STAMP < :EndTime "

    "ORDER BY ZEDS.Z_KEY DESC";

    qrAccumulatedTotals->ParamByName("StartTime")->AsDateTime = StartTime;
    qrAccumulatedTotals->ParamByName("EndTime")->AsDateTime = EndTime;

    qrPaymentTotal->Close();
    qrPaymentTotal->SQL->Text = "Select ";

    if (Locations)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
    "ARCBILLPAY.PAY_GROUP PAYMENTGROUP_NAME, UPPER(ARCBILLPAY.PAY_TYPE) PAYMENT_NAME, SUM(ARCBILLPAY.SUBTOTAL) SUBTOTAL "
    "FROM ARCBILLPAY LEFT JOIN ARCBILL ON ARCBILLPAY.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
    "GROUP BY ARCBILLPAY.PAY_GROUP, UPPER(ARCBILLPAY.PAY_TYPE) ";

    if (Locations)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        ", ARCBILL.BILLED_LOCATION ";
    }
    qrPaymentTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrPaymentTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrPaymentTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrDiscountTotal->Close();
    qrDiscountTotal->SQL->Text = "Select ";

    if (Locations)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTALDISCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != '' AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
    "GROUP BY DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME ";

    if (Locations)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        ", ARCHIVE.ORDER_LOCATION ";
    }
    qrDiscountTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrDiscountTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrDiscountTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrDiscountTotalNoGroup->Close();
    qrDiscountTotalNoGroup->SQL->Text = "Select ";

    if (Locations)
    {
        qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTALDISCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = '' AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text +
    "GROUP BY ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME ";

    if (Locations)
    {
        qrDiscountTotalNoGroup->SQL->Text = qrDiscountTotalNoGroup->SQL->Text +
        ", ARCHIVE.ORDER_LOCATION ";
    }
    qrDiscountTotalNoGroup->ParamByName("StartTime")->AsDateTime = StartTime;
    qrDiscountTotalNoGroup->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrDiscountTotalNoGroup->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrCancelTotal->Close();
    qrCancelTotal->SQL->Text = "Select ";

    if (Locations && Locations->Count > 0)
    {
        qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text +
    "ARCHIVE.COURSE_NAME, SUM(ARCHIVE.QTY) TOTALQTY, SUM(ARCHIVE.PRICE_LEVEL0 * ARCHIVE.QTY) TOTALPRICE0, CONTACTS.NAME "

    "FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
    "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "

    "WHERE "
    "SECURITY.SECURITY_EVENT IN ('Cancel','CancelY') AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text +
    "GROUP BY ARCHIVE.COURSE_NAME, CONTACTS.NAME ";

    if (Locations && Locations->Count > 0)
    {
        qrCancelTotal->SQL->Text = qrCancelTotal->SQL->Text +
        ", ARCHIVE.ORDER_LOCATION ";
    }
    qrCancelTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrCancelTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrCancelTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }
}

void TdmMMReportData::SetupAccumulatedZeds(TDateTime StartTime, TDateTime EndTime)
{
    qrAccumulatedZeds->Close();
    qrAccumulatedZeds->SQL->Text = "Select ";

    qrAccumulatedZeds->SQL->Text = qrAccumulatedZeds->SQL->Text +
    "ZEDS.TIME_STAMP, "
    "ZEDS.ZED_TOTAL, "
    "ZEDS.TERMINAL_EARNINGS AS Z_REPORT, "
    "ZEDS.ZED_TOTAL - ZEDS.TERMINAL_EARNINGS AS OPENING_BALANCE "

    "FROM ZEDS "

    "WHERE "
    "ZEDS.TIME_STAMP >= :StartTime and "
    "ZEDS.TIME_STAMP < :EndTime";

    qrAccumulatedZeds->ParamByName("StartTime")->AsDateTime = StartTime;
    qrAccumulatedZeds->ParamByName("EndTime")->AsDateTime = EndTime;
}

void TdmMMReportData::SetupSalesSummaryC(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
    qrSalesRevenue->Close();
    qrSalesRevenue->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text +
    "ARCHIVE.MENU_NAME, ARCHIVE.COURSE_NAME, ARCHIVE.ITEM_NAME, ARCHIVE.SIZE_NAME, ARCHIVE.BASE_PRICE, ARCHIVE.COST, "
    "CASE WHEN ARCHIVE.HAPPY_HOUR = 'T' THEN 'Happy Hour' ELSE '' END AS HAPPY_HOUR, "
    "CASE WHEN SUM(ARCHIVE.QTY) !< 0 THEN SUM(ABS(ARCHIVE.QTY)) ELSE 0 END AS SQTY, "
    "(SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) RQTY, "
    "A.QTYPERCOURSE, A.RQTYPERCOURSE, A.GROSSPERCOURSE, A.DISCOUNTPERCOURSE, A.NETPERCOURSE,  "

    "CAST(SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) AS NUMERIC(17,4)) - SUM(ARCHIVE.DISCOUNT) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4)) AS GROSSSALES, "

    "CASE WHEN SUM(ARCHIVE.QTY) !<0 THEN SUM(ARCHIVE.DISCOUNT) ELSE 0 END AS DISCOUNTALL, "

    "CAST(CASE WHEN (SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) <0 THEN "
    "((SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) + "
    "SUM(ARCHIVE.PRICE * (CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4))) "
    "ELSE "
    "((SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4))) END AS NUMERIC(17,4)) AS NETSALES "

    "FROM ARCHIVE "

    "LEFT JOIN (SELECT ARCORDERTAXES.ARCHIVE_KEY, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS TAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGE, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGETAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LOCALTAX "
    "FROM (SELECT  A.ARCHIVE_KEY,A.TAX_TYPE, CAST(SUM(A.TAX_VALUE ) AS NUMERIC(17,4)) TAX_VALUE FROM ARCORDERTAXES A "
    "GROUP BY  A.TAX_TYPE, A.ARCHIVE_KEY ORDER BY 1 ) ARCORDERTAXES "
    "GROUP BY ARCORDERTAXES.ARCHIVE_KEY) AS T ON ARCHIVE.ARCHIVE_KEY = T.ARCHIVE_KEY "

    "LEFT JOIN "
    "(SELECT ARCHIVE.COURSE_NAME, "
    "CASE WHEN SUM(ARCHIVE.QTY) !< 0 THEN SUM(ABS(ARCHIVE.QTY)) ELSE 0 END AS QTYPERCOURSE, "

    "(SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) AS RQTYPERCOURSE, "

    "CAST(SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) AS NUMERIC(17,4)) - SUM(ARCHIVE.DISCOUNT) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4)) AS GROSSPERCOURSE, "

    "CASE WHEN SUM(ARCHIVE.QTY) !<0 THEN SUM(ARCHIVE.DISCOUNT) ELSE 0 END AS DISCOUNTPERCOURSE, "

    "CAST(CASE WHEN (SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) <0 THEN "
    "((SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) + "
    "SUM(ARCHIVE.PRICE * (CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4))) "
    "ELSE "
    "((SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4))) END AS NUMERIC(17,4)) AS NETPERCOURSE "

    "FROM ARCHIVE "
    "LEFT JOIN (SELECT ARCORDERTAXES.ARCHIVE_KEY, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS TAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGE, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGETAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LOCALTAX "
    "FROM (SELECT  A.ARCHIVE_KEY,A.TAX_TYPE, CAST(SUM(A.TAX_VALUE ) AS NUMERIC(17,4)) TAX_VALUE FROM ARCORDERTAXES A "
    "GROUP BY  A.TAX_TYPE, A.ARCHIVE_KEY ORDER BY 1 ) ARCORDERTAXES "
    "GROUP BY ARCORDERTAXES.ARCHIVE_KEY) AS T ON ARCHIVE.ARCHIVE_KEY = T.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP >= :StartTime and ARCHIVE.TIME_STAMP < :EndTime "
    "GROUP BY ARCHIVE.COURSE_NAME) AS A ON A.COURSE_NAME = ARCHIVE.COURSE_NAME "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text +
    "GROUP BY ARCHIVE.MENU_NAME, ARCHIVE.COURSE_NAME, ARCHIVE.ITEM_NAME, ARCHIVE.SIZE_NAME, ARCHIVE.BASE_PRICE, ARCHIVE.COST, ARCHIVE.HAPPY_HOUR, A.QTYPERCOURSE, A.RQTYPERCOURSE, A.GROSSPERCOURSE, A.DISCOUNTPERCOURSE, A.NETPERCOURSE ";

    if (Locations)
    {
        qrSalesRevenue->SQL->Text = qrSalesRevenue->SQL->Text +
        ", ARCHIVE.ORDER_LOCATION ";
    }
    qrSalesRevenue->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesRevenue->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesRevenue->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrSalesRevenueTotal->Close();
    qrSalesRevenueTotal->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text +
    "CASE WHEN SUM(ARCHIVE.QTY) !< 0 THEN SUM(ABS(ARCHIVE.QTY)) ELSE 0 END AS TOTALQTY, "
    "SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END) AS TOTALRQTY, "

    "(SELECT SUM(PRICE) FROM (SELECT CAST((ARCHIVE.PRICE * (SUM(ARCHIVE.QTY) - SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) AS NUMERIC(17,4)) AS PRICE "
    "FROM ARCHIVE WHERE ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime GROUP BY ARCHIVE.PRICE)) - SUM(ARCHIVE.DISCOUNT) - "
    "(SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCHIVE LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime) AS TOTALGROSS, "

    "(SELECT SUM(DISCOUNT) FROM (SELECT ARCHIVE.DISCOUNT FROM ARCHIVE WHERE ARCHIVE.QTY !< 0 AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime)) AS TOTALDISCOUNT, "

    "CAST((SELECT SUM(PRICE) FROM (SELECT CAST((ARCHIVE.PRICE * (SUM(ARCHIVE.QTY) - SUM(CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END))) AS NUMERIC(17,4)) - SUM(ARCHIVE.DISCOUNT) AS PRICE "
    "FROM ARCHIVE WHERE ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime GROUP BY ARCHIVE.PRICE)) + SUM(ARCHIVE.DISCOUNT) - "
    "(SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCHIVE LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime) + "
    "COALESCE((SELECT SUM(CAST(ARCHIVE.PRICE * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP BETWEEN :StartTime AND :EndTime), 0) AS NUMERIC(17, 4)) AS TOTALNET "

    "FROM ARCHIVE "

    "Left join ARCBILL on ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text +
    "GROUP BY ";

    if (Locations)
    {
        qrSalesRevenueTotal->SQL->Text = qrSalesRevenueTotal->SQL->Text +
        "ARCBILL.BILLED_LOCATION ";
    }
    qrSalesRevenueTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesRevenueTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesRevenueTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }


    qrSalesConsolidated->Close();
    qrSalesConsolidated->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text +
    // Total Gross (Revenue + Service Charge + Local Tax + Discounts + Total Refund)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) + "

    "COALESCE((SELECT SUM(TOTALDISCOUNT) FROM (SELECT ABS(CASE WHEN NAME = 'Directors' OR NAME = 'Food Wastage' OR NAME = 'Bar Wastage' OR NAME = 'Food Panda' THEN SUM(TOTALDISCOUNTPRICE) ELSE SUM(TOTALDISCOUNTCOST) END) TOTALDISCOUNT FROM "

    "(SELECT DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTALDISCOUNTPRICE, SUM(ARCHIVE.COST) TOTALDISCOUNTCOST, "
    "COUNT(DISCOUNTS.NAME) TOTALDISCOUNTCOUNT, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Complimentary' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime "
    "GROUP BY DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION) "
    "GROUP BY DISCOUNTGROUP_NAME, NAME)),0) + "

    "COALESCE((SELECT ABS(SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE)) TOTALDISCOUNT "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) + "

    "COALESCE((SELECT ABS(SUM(TOTALDISCOUNT)) FROM (SELECT ARCORDERDISCOUNTS.DISCOUNTED_VALUE TOTALDISCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Complimentary' AND ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and  ARCHIVE.TIME_STAMP_BILLED < :EndTime)),0) - "

    "COALESCE((SELECT SUM(CAST(ABS(ARCHIVE.PRICE) * ABS(ARCHIVE.QTY) AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime), 0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) AS TOTALGROSS, "

    // Total Revenue  ((Gross - Refund - Discount) + Tax + Service Charge + Local Tax )
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0)  AS TOTALREVENUE, "

    // Total Net Sales (Gross - Refund - Discount - Service Charge - Local Tax - Vat - Tips)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS NETSALES, "

    // Total Revenue - Service Charge
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS REVANDSC, "

    // Total Revenue - Service Charge - Local Tax
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (2,3,4) AND ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS REVENUESCANDLT, "

    // Total VAT + Local Tax
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS VAT, "

    // Total Service Charge
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (2,3) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS SERVICECHARGE, "

    // Total SC and TIP
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (2,3) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) + "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) AS SCANDTIP, "

    // Total Local Tax
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 4 AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS LOCALTAX, "

    // Total Refund
    "CAST(COALESCE((SELECT SUM(CAST(ARCHIVE.PRICE * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) AS NUMERIC(17,4)) AS TOTALREFUNDS, "

    // Total Refund Quantity
    "(COALESCE((SELECT SUM(CAST(ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
    "WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime), 0) * -1) AS TOTALREFUNDSQTY, "

    // Total FOC (Free of Charge) Discount
    "COALESCE((SELECT SUM(TOTALDISCOUNT) FROM (SELECT ABS(CASE WHEN NAME = 'Directors' OR NAME = 'Food Wastage' OR NAME = 'Bar Wastage' OR NAME = 'Food Panda' THEN SUM(TOTALDISCOUNTPRICE) ELSE SUM(TOTALDISCOUNTCOST) END) TOTALDISCOUNT FROM "

    "(SELECT DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTALDISCOUNTPRICE, SUM(ARCHIVE.COST) TOTALDISCOUNTCOST, "
    "COUNT(DISCOUNTS.NAME) TOTALDISCOUNTCOUNT, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Complimentary' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime "

    "GROUP BY DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION) "

    "GROUP BY DISCOUNTGROUP_NAME, NAME)),0) AS TOTALFOC, "


    // Total Corporate Discount
    "COALESCE((SELECT ABS(SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE)) TOTALDISCOUNT "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) AS TOTALCORP, "

    // Total Discount (Without FOC AND CORP)
    "COALESCE((SELECT ABS(SUM(TOTALDISCOUNT)) FROM (SELECT ARCORDERDISCOUNTS.DISCOUNTED_VALUE TOTALDISCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Complimentary' AND ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and  ARCHIVE.TIME_STAMP_BILLED < :EndTime)),0) AS TOTALNORMALDISCOUNT, "

    // Total Discount
    "COALESCE((SELECT SUM(TOTALDISCOUNT) FROM (SELECT ABS(CASE WHEN NAME = 'Directors' OR NAME = 'Food Wastage' OR NAME = 'Bar Wastage' OR NAME = 'Food Panda' THEN SUM(TOTALDISCOUNTPRICE) ELSE SUM(TOTALDISCOUNTCOST) END) TOTALDISCOUNT FROM "

    "(SELECT DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTALDISCOUNTPRICE, SUM(ARCHIVE.COST) TOTALDISCOUNTCOST, "
    "COUNT(DISCOUNTS.NAME) TOTALDISCOUNTCOUNT, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Complimentary' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime "
    "GROUP BY DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, ARCHIVE.ARCBILL_KEY, ARCHIVE.ORDER_LOCATION) "
    "GROUP BY DISCOUNTGROUP_NAME, NAME)),0) + "

    "COALESCE((SELECT ABS(SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE)) TOTALDISCOUNT "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE "
    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY AND DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime),0) + "

    "COALESCE((SELECT ABS(SUM(TOTALDISCOUNT)) FROM (SELECT ARCORDERDISCOUNTS.DISCOUNTED_VALUE TOTALDISCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Complimentary' AND ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and  ARCHIVE.TIME_STAMP_BILLED < :EndTime)),0) AS TOTALDISC, "

    // Total Tips
    "(SELECT SUM(coalesce(ARCSURCHARGE.SUBTOTAL,0) + coalesce(tip.TIP_AMOUNT,0) ) FROM ARCBILL  LEFT JOIN ARCSURCHARGE ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "left join( select sum(coalesce(ARCBILLPAY.TIP_AMOUNT,0)) tip_amount, ARCBILLPAY.ARCBILL_KEY from  ARCBILLPAY group by ARCBILLPAY.ARCBILL_KEY)tip on ARCBILL.ARCBILL_KEY = tip.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime) AS TOTALTIPS, "

    // Total Discount Count
    "(SELECT COUNT(ARCBILL_KEY) TOTALDISCOUNTCOUNT FROM "

    "(SELECT ARCHIVE.ARCBILL_KEY "
    "FROM DISCOUNT_GROUPS LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES ON DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTGROUPS_KEY "
    "LEFT JOIN DISCOUNTS ON DISCOUNTS.DISCOUNT_KEY = DISCOUNTGROUPS_DISCOUNTTYPES.DISCOUNTTYPE_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY "
    "LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime "

    "GROUP BY DISCOUNT_GROUPS.DISCOUNTGROUP_NAME, DISCOUNTS.NAME, ARCHIVE.ARCBILL_KEY,ARCHIVE.ORDER_LOCATION)) AS TOTALDISCOUNTCOUNT, "

    // Total Check Count (receipt count)
    "COALESCE((SELECT COUNT(ARCBILL.INVOICE_NUMBER) FROM ARCBILL WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) AS TOTALCHECK, "

    // Total Check Amount (receipt amount)
    "COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) AS TOTALCHECKAMOUNT, "

    // Total Saved Table Count
    "COALESCE((SELECT COUNT(TABLE_NAME) FROM (SELECT DISTINCT ORDERS.TABLE_NAME FROM ORDERS WHERE ORDERS.TIME_STAMP BETWEEN :StartTime AND :EndTime)),0) AS SAVETABLECOUNT, "

    // Total Saved Table Amount
    "COALESCE((SELECT SUM(ORDERS.PRICE) FROM ORDERS WHERE ORDERS.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) AS SAVEDTABLEAMOUNT, "

    // Total Manager Void Count
    "COALESCE((SELECT ABS(A.QTY) FROM "
    "(SELECT SUM(CAST(ARCHIVE.QTY AS NUMERIC(17,4))) AS QTY FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY LEFT JOIN CONTACTS ON ARCBILL.STAFF_NAME = CONTACTS.NAME "
    "LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF WHERE SECURITY.SECURITY_EVENT = 'Credit' AND CONTACTS.ACCESS_LEVEL IN ('-3671169','-125371536') "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime GROUP BY ARCBILL.TOTAL) AS A),0) AS MGRCOUNT, "

    // Total Manager Void Amount
    "COALESCE((SELECT SUM(TOTAL) FROM "
    "(SELECT ARCBILL.TOTAL FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY LEFT JOIN CONTACTS ON ARCBILL.STAFF_NAME = CONTACTS.NAME "
    "LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF WHERE SECURITY.SECURITY_EVENT = 'Credit' AND CONTACTS.ACCESS_LEVEL IN ('-3671169','-125371536') "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime GROUP BY ARCBILL.TOTAL)),0) AS MGRTOTAL, "

    // Total Tax Exempt
    "COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE*ARCHIVE.QTY) AS NUMERIC(17,4)) "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 0 AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) - "
    "COALESCE((SELECT SUM(A.TAX_VALUE) FROM "
    "(SELECT ARCORDERTAXES.ARCHIVE_KEY, ARCORDERTAXES.TAX_VALUE, ARCORDERTAXES.TAX_TYPE "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime) AS A LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = A.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_VALUE = 0 AND ARCORDERTAXES.TAX_TYPE = 0 AND A.TAX_TYPE IN (2,3)),0) AS TAXEXEMPT, "

    // Total Vatable
    "COALESCE((SELECT CAST(SUM(COALESCE(ARCBILL.TOTAL,0) - COALESCE(ARCSURCHARGE.SUBTOTAL,0)) AS NUMERIC(17,4)) FROM ARCBILL LEFT JOIN ARCSURCHARGE ON ARCBILL.ARCBILL_KEY = ARCSURCHARGE.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) - "

    "((COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE*ARCHIVE.QTY) AS NUMERIC(17,4)) "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE = 0 AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0) - "
    "COALESCE((SELECT SUM(A.TAX_VALUE) FROM "
    "(SELECT ARCORDERTAXES.ARCHIVE_KEY, ARCORDERTAXES.TAX_VALUE, ARCORDERTAXES.TAX_TYPE "
    "FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime) AS A LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = A.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_VALUE = 0 AND ARCORDERTAXES.TAX_TYPE = 0 AND A.TAX_TYPE IN (2,3)),0)) + "

    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime),0)) AS VATABLE, "

    // Total Cover Count (Patron Count) for Non Vat sales
    "(COALESCE((SELECT SUM(PATRON_COUNT) FROM "
    "(SELECT DISTINCT ARCBILL_KEY, PATRON_COUNT FROM (SELECT A.ARCBILL_KEY, SUM(ARCORDERTAXES.TAX_VALUE) AS TAX_VALUE, A.PATRON_COUNT FROM (SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT) AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ARCBILL_KEY, A.PATRON_COUNT) WHERE TAX_VALUE = 0)),0)) AS NONVATCOVERCOUNT, "

    // Total Cover Count (Patron Count) for Vatable sales
    "(COALESCE((SELECT SUM(PATRON_COUNT) FROM "
    "(SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE > 0 "
    "AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT)),0)) AS VATABLECOVERCOUNT, "

    // Total Cover Count
    "COALESCE((SELECT SUM(PATRON_COUNT) FROM "
    "(SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.PATRON_COUNT)),0) AS TOTALCOVERCOUNT, "

    // Total Check Count (Receipt Count) for Non Vat sales
    "(COALESCE((SELECT COUNT(INVOICE_NUMBER) FROM "
    "(SELECT DISTINCT ARCBILL_KEY, INVOICE_NUMBER FROM (SELECT A.ARCBILL_KEY, SUM(ARCORDERTAXES.TAX_VALUE) AS TAX_VALUE, A.INVOICE_NUMBER FROM (SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE = 0 "
    "AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER) AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ARCBILL_KEY, A.INVOICE_NUMBER) WHERE TAX_VALUE = 0)),0)) AS NONVATCHECKCOUNT, "

    // Total Check Count (Receipt Count) for Vatable sales
    "(COALESCE((SELECT COUNT(INVOICE_NUMBER) FROM "
    "(SELECT DISTINCT ARCBILL_KEY, INVOICE_NUMBER FROM (SELECT A.ARCBILL_KEY, SUM(ARCORDERTAXES.TAX_VALUE) AS TAX_VALUE, A.INVOICE_NUMBER FROM (SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE > 0 "
    "AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER) AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ARCBILL_KEY, A.INVOICE_NUMBER) WHERE TAX_VALUE > 0)),0)) AS VATABLECHECKCOUNT, "

    // Total Check Count
    "COALESCE((SELECT COUNT(INVOICE_NUMBER) FROM "
    "(SELECT DISTINCT ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER FROM ARCBILL LEFT JOIN ARCHIVE ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ARCBILL_KEY, ARCBILL.INVOICE_NUMBER)),0) AS TOTALCHECKCNT, "

    // Total Table Count For Vatable Sales
    "(COALESCE((SELECT COUNT(ORDER_IDENTIFICATION_NUMBER) FROM "
    "(SELECT DISTINCT ORDER_IDENTIFICATION_NUMBER FROM "

    "(SELECT A.ARCBILL_KEY, A.ORDER_IDENTIFICATION_NUMBER, SUM(A.TAX_VALUE) AS TAX_VALUE, A.TABLE_NUMBER, A.QTY FROM "

    "(SELECT DISTINCT ARCORDERTAXES.TAX_VALUE, ARCBILL.ARCBILL_KEY, ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER FROM TABLES LEFT JOIN ARCHIVE ON TABLES.TABLE_NUMBER = ARCHIVE.TABLE_NUMBER "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE > 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER, ARCBILL.ARCBILL_KEY, ARCORDERTAXES.TAX_VALUE) "

    "AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ORDER_IDENTIFICATION_NUMBER, A.TABLE_NUMBER, A.QTY, A.ARCBILL_KEY) "

    "WHERE TAX_VALUE > 0)),0)) AS VATABLETABLECOUNT, "


    // Total Table Count For Non Vat Sales
    "(COALESCE((SELECT COUNT(ORDER_IDENTIFICATION_NUMBER) FROM "
    "(SELECT DISTINCT ORDER_IDENTIFICATION_NUMBER FROM "

    "(SELECT A.ARCBILL_KEY, A.ORDER_IDENTIFICATION_NUMBER, SUM(A.TAX_VALUE) AS TAX_VALUE, A.TABLE_NUMBER, A.QTY FROM "

    "(SELECT DISTINCT ARCORDERTAXES.TAX_VALUE, ARCBILL.ARCBILL_KEY, ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER FROM TABLES LEFT JOIN ARCHIVE ON TABLES.TABLE_NUMBER = ARCHIVE.TABLE_NUMBER "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE = 0 AND ARCHIVE.DISCOUNT != 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER, ARCBILL.ARCBILL_KEY, ARCORDERTAXES.TAX_VALUE) "

    "AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ORDER_IDENTIFICATION_NUMBER, A.TABLE_NUMBER, A.QTY, A.ARCBILL_KEY) "

    "WHERE TAX_VALUE = 0)),0)) AS NONVATTABLECOUNT, "

    // Total Table Count
    "(COALESCE((SELECT COUNT(ORDER_IDENTIFICATION_NUMBER) FROM "
    "(SELECT DISTINCT ORDER_IDENTIFICATION_NUMBER FROM "

    "(SELECT A.ARCBILL_KEY, A.ORDER_IDENTIFICATION_NUMBER, SUM(A.TAX_VALUE) AS TAX_VALUE, A.TABLE_NUMBER, A.QTY FROM "

    "(SELECT DISTINCT ARCORDERTAXES.TAX_VALUE, ARCBILL.ARCBILL_KEY, ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER FROM TABLES LEFT JOIN ARCHIVE ON TABLES.TABLE_NUMBER = ARCHIVE.TABLE_NUMBER "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE = 0 AND ARCHIVE.DISCOUNT != 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER, ARCBILL.ARCBILL_KEY, ARCORDERTAXES.TAX_VALUE) "

    "AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ORDER_IDENTIFICATION_NUMBER, A.TABLE_NUMBER, A.QTY, A.ARCBILL_KEY) "

    "WHERE TAX_VALUE = 0)),0)) + "

    "(COALESCE((SELECT COUNT(ORDER_IDENTIFICATION_NUMBER) FROM "
    "(SELECT DISTINCT ORDER_IDENTIFICATION_NUMBER FROM "

    "(SELECT A.ARCBILL_KEY, A.ORDER_IDENTIFICATION_NUMBER, SUM(A.TAX_VALUE) AS TAX_VALUE, A.TABLE_NUMBER, A.QTY FROM "

    "(SELECT DISTINCT ARCORDERTAXES.TAX_VALUE, ARCBILL.ARCBILL_KEY, ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER FROM TABLES LEFT JOIN ARCHIVE ON TABLES.TABLE_NUMBER = ARCHIVE.TABLE_NUMBER "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "LEFT JOIN ARCORDERDISCOUNTS ON ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) AND ARCORDERTAXES.TAX_VALUE > 0 "
    "AND ARCHIVE.TIME_STAMP_BILLED BETWEEN :StartTime AND :EndTime "
    "GROUP BY ARCBILL.ORDER_IDENTIFICATION_NUMBER, ARCHIVE.QTY, TABLES.TABLE_NUMBER, ARCBILL.ARCBILL_KEY, ARCORDERTAXES.TAX_VALUE) "

    "AS A LEFT JOIN ARCHIVE ON ARCHIVE.ARCBILL_KEY = A.ARCBILL_KEY "
    "LEFT JOIN ARCORDERTAXES ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,4) "
    "GROUP BY A.ORDER_IDENTIFICATION_NUMBER, A.TABLE_NUMBER, A.QTY, A.ARCBILL_KEY) "

    "WHERE TAX_VALUE > 0)),0)) AS TOTALTABLECOUNT, "

    // Total Vatable Turn Time
    "COALESCE((SELECT SUM(DURATION) / 60 FROM (SELECT A_KEY, A_TKEY, TURNAROUNDTIMES.SALE_START_TIME, TURNAROUNDTIMES.SALE_END_TIME, "
    "CASE WHEN EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) > EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) THEN "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME)) ELSE "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME)) END AS DURATION FROM "

    "(SELECT ARC_KEY A_KEY, ARC_TKEY A_TKEY FROM (SELECT ARCHIVE.ARCHIVE_KEY ARC_KEY, ARCHIVE.TIME_KEY ARC_TKEY, ARCORDERTAXES.TAX_TYPE ARC_TYP, ARCORDERTAXES.TAX_VALUE ARC_TVAL FROM ARCHIVE "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY) WHERE ARC_TYP = 0) ABCD "
    "LEFT JOIN TURNAROUNDTIMES ON ABCD.A_TKEY = TURNAROUNDTIMES.TIME_KEY "
    "WHERE TURNAROUNDTIMES.SALE_START_TIME >= :StartTime AND TURNAROUNDTIMES.SALE_END_TIME < :EndTime)),0) - "

    "COALESCE((SELECT SUM(DURATION) / 60 FROM (SELECT A_KEY, A_TKEY, TURNAROUNDTIMES.SALE_START_TIME, TURNAROUNDTIMES.SALE_END_TIME, "
    "CASE WHEN EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) > EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) THEN "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME)) ELSE "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME)) END AS DURATION FROM "

    "(SELECT ARC_KEY A_KEY, ARC_TKEY A_TKEY FROM (SELECT ARCHIVE.ARCHIVE_KEY ARC_KEY, ARCHIVE.TIME_KEY ARC_TKEY, ARCORDERTAXES.TAX_TYPE ARC_TYP, ARCORDERTAXES.TAX_VALUE ARC_TVAL FROM ARCHIVE "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY) WHERE ARC_TYP = 0 AND ARC_TVAL = 0 ) ABCD "
    "LEFT JOIN TURNAROUNDTIMES ON ABCD.A_TKEY = TURNAROUNDTIMES.TIME_KEY "
    "WHERE TURNAROUNDTIMES.SALE_START_TIME >= :StartTime AND TURNAROUNDTIMES.SALE_END_TIME < :EndTime)),0) AS VATABLETURNTIME, "

    // Total Non Vat Turn Time
    "COALESCE((SELECT SUM(DURATION) / 60 FROM (SELECT A_KEY, A_TKEY, TURNAROUNDTIMES.SALE_START_TIME, TURNAROUNDTIMES.SALE_END_TIME, "
    "CASE WHEN EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) > EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) THEN "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME)) ELSE "
    "ABS(EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_START_TIME) - EXTRACT(SECOND FROM TURNAROUNDTIMES.SALE_END_TIME)) END AS DURATION FROM "

    "(SELECT ARC_KEY A_KEY, ARC_TKEY A_TKEY FROM (SELECT ARCHIVE.ARCHIVE_KEY ARC_KEY, ARCHIVE.TIME_KEY ARC_TKEY, ARCORDERTAXES.TAX_TYPE ARC_TYP, ARCORDERTAXES.TAX_VALUE ARC_TVAL FROM ARCHIVE "
    "LEFT JOIN ARCORDERTAXES ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY) WHERE ARC_TYP = 0 AND ARC_TVAL = 0 ) ABCD "
    "LEFT JOIN TURNAROUNDTIMES ON ABCD.A_TKEY = TURNAROUNDTIMES.TIME_KEY "
    "WHERE TURNAROUNDTIMES.SALE_START_TIME >= :StartTime AND TURNAROUNDTIMES.SALE_END_TIME < :EndTime)),0) AS NONVATTURNTIME "

    "FROM ARCHIVE "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text +
    "GROUP BY ";

    if (Locations)
    {
        qrSalesConsolidated->SQL->Text = qrSalesConsolidated->SQL->Text +
        "ARCHIVE.ORDER_LOCATION ";
    }
    qrSalesConsolidated->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesConsolidated->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesConsolidated->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrSalesCountByDayPart->Close();
    qrSalesCountByDayPart->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text +
    // Total Patron Count every Breakfast (5am to 11am) / As of 04/26/2016 it is now LUNCH (5am to 2:59pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 5 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 15 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS BREAKFASTCOUNT, "

    // Total Amount every Breakfast (5am to 11am) / As of 04/26/2016 it is now LUNCH (5am to 2:59pm)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 5 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 15 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 5 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) < 15 AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 5 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 15 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS BREAKFASTAMOUNT, "

    // Total Patron Count every Lunch (11am to 3pm) / As of 04/26/2016 it is now SNACK (3pm to 5:59pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 15 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 18 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS LUNCHCOUNT, "

    // Total Amount every Lunch (11am to 3pm) / As of 04/26/2016 it is now SNACK (3pm to 5:59pm)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 15 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 18 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 15 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) < 18 AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 15 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 18 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS LUNCHAMOUNT, "

    // Total Patron Count every Merienda(Snacks) (3pm to 6pm) / As of 04/26/2016 it is now DINNER (6pm to 8:59pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 18 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 21 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS MERIENDACOUNT, "

    // Total Amount every Merienda(Snacks) (3pm to 6pm) / As of 04/26/2016 it is now DINNER (6pm to 8:59pm)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 18 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 21 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 18 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) < 21 AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 18 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 21 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS MERIENDAAMOUNT, "

    // Total Patron Count every Dinner (6pm to 9pm) / As of 04/26/2016 it is now AFTER DINNER (9pm to 11:59pm)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 21 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) <= 23 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS DINNERCOUNT, "

    // Total Amount every Dinner (6pm to 9pm) / As of 04/26/2016 it is now AFTER DINNER (9pm to 11:59pm)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 21 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) <= 23 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 21 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) <= 23 AND "
    "ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 21 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 23 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS DINNERAMOUNT, "

    // Total Patron Count every After Dinner (9pm to 12am) / As of 04/26/2016 it is now LATE EVENING (12am to 12:59am)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 0 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 1 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS AFTERDINNERCOUNT, "

    // Total Amount every After Dinner (9pm to 12am) As of 04/26/2016 it is now LATE EVENING (12am to 12:59am)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 0 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 1 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 0 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) < 1 AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 0 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 1 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS AFTERDINNERAMOUNT, "

    // Total Patron Count every Late (12am onwards) / As of 04/26/2016 it is now AFTER HOURS (1am to 4:59am)
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 1 AND EXTRACT(HOUR FROM ARCBILL.TIME_STAMP) < 5 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS LATECOUNT, "

    // Total Amount every Late (12am onwards) / As of 04/26/2016 it is now AFTER HOURS (1am to 4:59am)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 1 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 5 "
    "AND ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) >= 1 AND EXTRACT(HOUR from ARCHIVE.TIME_STAMP) < 5 AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE EXTRACT(HOUR from ARCBILL.TIME_STAMP) >= 1 AND EXTRACT(HOUR from ARCBILL.TIME_STAMP) < 5 AND ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS LATEDINNERAMOUNT, "

    // Total Amount for All Hours (Breakfast, Lunch, Merienda, Dinner)
    "(COALESCE((SELECT SUM(ARCBILL.TOTAL) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) - "
    "COALESCE((SELECT SUM(ARCORDERTAXES.TAX_VALUE) FROM ARCORDERTAXES LEFT JOIN ARCHIVE ON ARCORDERTAXES.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "WHERE ARCORDERTAXES.TAX_TYPE IN (0,2,3,4) AND ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED < :EndTime),0)) - "
    "COALESCE((SELECT SUM(ARCSURCHARGE.SUBTOTAL) FROM ARCSURCHARGE LEFT JOIN ARCBILL ON ARCSURCHARGE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "WHERE ARCBILL.TIME_STAMP BETWEEN :StartTime AND :EndTime),0) "
    "AS TOTALCHECKCOUNT, "

    // Total Patron Count
    "COALESCE((SELECT SUM(ARCBILL.PATRON_COUNT) FROM ARCBILL WHERE ARCBILL.TIME_STAMP >= :StartTime AND ARCBILL.TIME_STAMP < :EndTime),0) AS TOTALPATRONCOUNT "

    "FROM ARCHIVE "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text +
    "GROUP BY ";

    if (Locations)
    {
        qrSalesCountByDayPart->SQL->Text = qrSalesCountByDayPart->SQL->Text +
        "ARCHIVE.ORDER_LOCATION ";
    }
    qrSalesCountByDayPart->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesCountByDayPart->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesCountByDayPart->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrPaymentTotal->Close();
    qrPaymentTotal->SQL->Text = "Select ";

    if (Locations)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
    "PAYMENTGROUPS.PAYMENTGROUP_NAME, PAYMENTTYPES.PAYMENT_NAME, SUM(ARCBILLPAY.SUBTOTAL) SUBTOTAL, COUNT(CASE WHEN ARCBILLPAY.NOTE != 'Total Change.' THEN PAYMENTTYPES.PAYMENT_NAME END) PAYMENTCOUNT "
    "FROM PAYMENTGROUPS LEFT JOIN PAYMENTGROUPS_PAYMENTTYPES ON PAYMENTGROUPS.PAYMENTGROUPS_KEY = PAYMENTGROUPS_PAYMENTTYPES.PAYMENTGROUPS_KEY "
    "LEFT JOIN PAYMENTTYPES ON PAYMENTTYPES.PAYMENT_KEY = PAYMENTGROUPS_PAYMENTTYPES.PAYMENT_KEY "
    "LEFT JOIN ARCBILLPAY ON PAYMENTTYPES.PAYMENT_NAME = ARCBILLPAY.PAY_TYPE "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCBILLPAY.ARCBILL_KEY "

    "WHERE "
    "PAYMENTGROUPS.PAYMENTGROUPS_KEY = PAYMENTGROUPS_PAYMENTTYPES.PAYMENTGROUPS_KEY AND "
    "ARCBILL.TIME_STAMP >= :StartTime and "
    "ARCBILL.TIME_STAMP < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
    "GROUP BY PAYMENTGROUPS.PAYMENTGROUP_NAME, PAYMENTTYPES.PAYMENT_NAME ";

    if (Locations)
    {
        qrPaymentTotal->SQL->Text = qrPaymentTotal->SQL->Text +
        ", ARCBILL.BILLED_LOCATION ";
    }
    qrPaymentTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrPaymentTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrPaymentTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrDiscountTotal->Close();
    qrDiscountTotal->SQL->Text = "Select ";

    if (Locations)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME, ABS(SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE)) TOTALDISCOUNT, COUNT(ARCHIVE.DISCOUNT_REASON) TOTALDISCOUNTCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Complimentary' AND ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME != 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
    "GROUP BY DISCOUNTGROUP_NAME, NAME	 ";

    if (Locations)
    {
        qrDiscountTotal->SQL->Text = qrDiscountTotal->SQL->Text +
        ", ARCBILL.BILLED_LOCATION ";
    }
    qrDiscountTotal->ParamByName("StartTime")->AsDateTime = StartTime;
    qrDiscountTotal->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrDiscountTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrDiscountTotalComplimentary->Close();
    qrDiscountTotalComplimentary->SQL->Text = "Select ";

    if (Locations)
    {
        qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME, "
    "ABS(CASE WHEN ARCORDERDISCOUNTS.NAME = 'Directors' OR ARCORDERDISCOUNTS.NAME = 'Food Wastage' OR ARCORDERDISCOUNTS.NAME = 'Bar Wastage' OR ARCORDERDISCOUNTS.NAME = 'Food Panda' THEN SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) ELSE SUM(ARCHIVE.COST) END) TOTALDISCOUNT, "
    "COUNT(ARCHIVE.DISCOUNT_REASON) TOTALDISCOUNTCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = 'Complimentary' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text +
    "GROUP BY DISCOUNTGROUP_NAME, NAME ";

    if (Locations)
    {
        qrDiscountTotalComplimentary->SQL->Text = qrDiscountTotalComplimentary->SQL->Text +
        ", ARCBILL.BILLED_LOCATION ";
    }
    qrDiscountTotalComplimentary->ParamByName("StartTime")->AsDateTime = StartTime;
    qrDiscountTotalComplimentary->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrDiscountTotalComplimentary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }


    qrDiscountTotalCorporate->Close();
    qrDiscountTotalCorporate->SQL->Text = "Select ";

    if (Locations)
    {
        qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME, ABS(SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE)) TOTALDISCOUNT, COUNT(ARCHIVE.DISCOUNT_REASON) TOTALDISCOUNTCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = 'Corporate Discount' AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text +
    "GROUP BY DISCOUNTGROUP_NAME, NAME ";

    if (Locations)
    {
        qrDiscountTotalCorporate->SQL->Text = qrDiscountTotalCorporate->SQL->Text +
        ", ARCBILL.BILLED_LOCATION ";
    }
    qrDiscountTotalCorporate->ParamByName("StartTime")->AsDateTime = StartTime;
    qrDiscountTotalCorporate->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrDiscountTotalCorporate->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrSalesConsolidatedCancel->Close();
    qrSalesConsolidatedCancel->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text +
    "SUM(ARCHIVE.QTY) TOTALQTY, SUM(ARCHIVE.PRICE_LEVEL0 * ARCHIVE.QTY) TOTALPRICE0 "

    "FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF "
    "LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
    "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "

    "WHERE "
    "SECURITY.SECURITY_EVENT IN ('Cancel','CancelY') AND "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text +
    "GROUP BY ";

    if (Locations)
    {
        qrSalesConsolidatedCancel->SQL->Text = qrSalesConsolidatedCancel->SQL->Text +
        "ARCBILL.BILLED_LOCATION ";
    }
    qrSalesConsolidatedCancel->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesConsolidatedCancel->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesConsolidatedCancel->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }


    qrSalesMenuSummary->Close();
    qrSalesMenuSummary->SQL->Text = "Select ";

    if (Locations && Locations->Count > 0)
    {
        qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text +
        "ARCHIVE.ORDER_LOCATION LOCATION,";
    }
    else
    {
        qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text +
    "ARCHIVE.MENU_NAME AS MENU_NAME, SUM(ARCHIVE.QTY) AS QTYPERMENU, "

    "CAST(SUM(ARCHIVE.PRICE * (ARCHIVE.QTY - CASE WHEN ARCHIVE.QTY < 0 THEN ARCHIVE.QTY ELSE 0 END)) AS NUMERIC(17,4)) + SUM(CASE WHEN ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = 'Complimentary' THEN "
    "ABS(CASE WHEN ARCORDERDISCOUNTS.NAME = 'Directors' OR ARCORDERDISCOUNTS.NAME = 'Food Wastage' OR ARCORDERDISCOUNTS.NAME = 'Bar Wastage' OR ARCORDERDISCOUNTS.NAME = 'Food Panda' THEN ARCHIVE.DISCOUNT ELSE ARCHIVE.COST END) "
    "ELSE ABS(ARCHIVE.DISCOUNT) END) - "
    "CAST(SUM(COALESCE(T.TAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGE,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.SERVICECHARGETAX,0)) AS NUMERIC(17,4)) - "
    "CAST(SUM(COALESCE(T.LOCALTAX,0)) AS NUMERIC(17,4)) AS AMOUNTPERMENU "

    "FROM ARCHIVE "

    "LEFT JOIN (SELECT ARCORDERTAXES.ARCHIVE_KEY, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS TAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGE, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS SERVICECHARGETAX, "
    "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS LOCALTAX "
    "FROM (SELECT  A.ARCHIVE_KEY,A.TAX_TYPE, CAST(SUM(A.TAX_VALUE ) AS NUMERIC(17,4)) TAX_VALUE FROM ARCORDERTAXES A "
    "GROUP BY  A.TAX_TYPE, A.ARCHIVE_KEY ORDER BY 1 ) ARCORDERTAXES "
    "GROUP BY ARCORDERTAXES.ARCHIVE_KEY) AS T ON ARCHIVE.ARCHIVE_KEY = T.ARCHIVE_KEY "

    "LEFT JOIN ARCORDERDISCOUNTS ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "

    "WHERE "
    "ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime ";

    if (Locations && Locations->Count > 0)
    {
        qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCHIVE.ORDER_LOCATION", "LocationParam") + ")";
    }
    qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text +
    "GROUP BY ARCHIVE.MENU_NAME ";

    if (Locations && Locations->Count > 0)
    {
        qrSalesMenuSummary->SQL->Text = qrSalesMenuSummary->SQL->Text +
        ", ARCHIVE.ORDER_LOCATION";
    }
    qrSalesMenuSummary->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesMenuSummary->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesMenuSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }

    qrSalesDiscountTotalLocation->Close();
    qrSalesDiscountTotalLocation->SQL->Text = "Select ";

    if (Locations)
    {
        qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text +
        "ARCBILL.BILLED_LOCATION LOCATION,";
    }
    else
    {
        qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text +
        "CAST('ALL LOCATION' As Varchar(25)) LOCATION,";
    }

    qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text +
    "ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME DISCOUNTGROUP_NAME, ARCORDERDISCOUNTS.NAME, "
    "ABS(CASE WHEN ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = 'Complimentary' THEN "
    "ABS(CASE WHEN ARCORDERDISCOUNTS.NAME = 'Directors' OR ARCORDERDISCOUNTS.NAME = 'Food Wastage' OR ARCORDERDISCOUNTS.NAME = 'Bar Wastage' OR ARCORDERDISCOUNTS.NAME = 'Food Panda' THEN SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) ELSE SUM(ARCHIVE.COST) END) ELSE "
    "SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) END) TOTALDISCOUNT, "
    "COUNT(ARCHIVE.DISCOUNT_REASON) TOTALDISCOUNTCOUNT "
    "FROM ARCORDERDISCOUNTS LEFT JOIN ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY "
    "LEFT JOIN ARCBILL ON ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "

    "WHERE ARCHIVE.TIME_STAMP_BILLED >= :StartTime and "
    "ARCHIVE.TIME_STAMP_BILLED < :EndTime";

    if (Locations && Locations->Count > 0)
    {
        qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text + "AND (" + ParamString(Locations->Count, "ARCBILL.BILLED_LOCATION", "LocationParam") + ")";
    }
    qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text +
    "GROUP BY DISCOUNTGROUP_NAME, NAME ";


    if (Locations)
    {
        qrSalesDiscountTotalLocation->SQL->Text = qrSalesDiscountTotalLocation->SQL->Text +
        ",ARCBILL.BILLED_LOCATION";
    }
    qrSalesDiscountTotalLocation->ParamByName("StartTime")->AsDateTime = StartTime;
    qrSalesDiscountTotalLocation->ParamByName("EndTime")->AsDateTime = EndTime;

    if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrSalesDiscountTotalLocation->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }
}
 //-------------------------------------------------------------------------------
void TdmMMReportData::SetupCashier(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals, TStrings *Locations)
{
   SalesSummaryLT(StartTime,  EndTime, Locations, Terminals);
   SetupCashupLT(StartTime,   EndTime,  Locations,  Terminals) ;

}
//---------------------------------------------------------------------------
//Existing Report salesSummarylocation  method
//-------------------------------------------------------------------------
void TdmMMReportData::SetupSalesSummaryByLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
	qrSalesSummary->Close();
	qrSalesSummary->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"Sum(Archive.Qty) Item_Count,"
           " Cast(Sum(Archive.QTY * Archive.BASE_PRICE+Archive.Qty ) as Numeric(17,4)) PriceExc ,  "
             "   Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) PriceInc , "
             "Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(abs(AOT.VAT),0)+COALESCE(abs(AOT.ServiceCharge),0) + COALESCE(abs(AOT.OtherServiceCharge),0)) as Numeric(17,4)) Sales_Inc, "

			"cast(Sum(Archive.Cost * Archive.Qty) as numeric(17, 4))  Cost "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
                "LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "

          "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  "

 "ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrSalesSummary->SQL->Text	=	qrSalesSummary->SQL->Text + "and (" +
												ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrSalesSummary->SQL->Text		=	qrSalesSummary->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
		"Order By "
			"1, 2";
	qrSalesSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrSalesSummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrSalesSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
	qrPatronCount->Close();
	qrPatronCount->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Sales_Type,"
			"Sum(ArcBill.Patron_Count) Patron_Count "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrPatronCount->SQL->Text	=	qrPatronCount->SQL->Text + "and (" +
												ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrPatronCount->SQL->Text		=	qrPatronCount->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Sales_Type "
		"Having "
			"Count(Patron_Count) > 0 "
		"Order By "
			"1,"
			"ArcBill.Sales_Type";
	qrPatronCount->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrPatronCount->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrPatronCount->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
	qrAveSummary->Close();
	qrAveSummary->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.ArcBill_Key,"
			"CategoryGroups.Name Category_Group,"
			"ArcBill.Sales_Type,"
			"cast(Max(ArcBill.Total) as numeric(17, 4)) Bill_Total,"
			"  Cast(Sum(Archive.QTY * Archive.BASE_PRICE + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) Orders_Total ,	"
			"Max(ArcBill.Patron_Count) Patron_Count "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
                "LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
          " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "

           "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
 "ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrAveSummary->SQL->Text	=	qrAveSummary->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrAveSummary->SQL->Text		=	qrAveSummary->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.ArcBill_Key,"
			"CategoryGroups.Name,"
			"ArcBill.Sales_Type "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
		"Order By "
			"1,"
			"CategoryGroups.Name,"
			"ArcBill.Sales_Type";
	qrAveSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrAveSummary->ParamByName("EndTime")->AsDateTime	= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrAveSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
	qrDiscountSummary->Close();
	qrDiscountSummary->SQL->Text =
   		"Select ";
	if (Locations)
	{
		qrDiscountSummary->SQL->Text = qrDiscountSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrDiscountSummary->SQL->Text = qrDiscountSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrDiscountSummary->SQL->Text = qrDiscountSummary->SQL->Text +
   			"Count(ArcBill.ArcBill_Key) Bill_Count,"
            "cast(Sum(Archive.DISCOUNT_WITHOUT_TAX) as numeric(17, 4)) DISCOUNT_WITHOUT_TAX ,  "
			 "  cast(Sum(Archive.TAX_ON_DISCOUNT) as numeric(17, 4)) TAX_ON_DISCOUNT  , "
			"cast(Sum(Archive.Discount) as numeric(17, 4)) Discount_Total "
		"From "
			"Security Left Join ArcBill On "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive On "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
            "inner JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "

		"Where "

         " COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and "     //change include and
         "ARCHIVE.PRICE<>0 and "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Discounted By' " ;


   	if (Locations && Locations->Count > 0)
	{
		qrDiscountSummary->SQL->Text	=	qrDiscountSummary->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}

	if (Locations)
	{ 	qrDiscountSummary->SQL->Text		=	qrDiscountSummary->SQL->Text +
		"Group By ";
		qrDiscountSummary->SQL->Text = qrDiscountSummary->SQL->Text +
			"ArcBill.Billed_Location ";
	}
	qrDiscountSummary->SQL->Text = qrDiscountSummary->SQL->Text +
				"Having "
			"Count(ArcBill.ArcBill_Key) > 0 ";

   qrDiscountSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscountSummary->ParamByName("EndTime")->AsDateTime		= EndTime;
   	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrDiscountSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}

	qrAdjustSummary->Close();
	qrAdjustSummary->SQL->Text =
   		"Select ";
	if (Locations)
	{
		qrAdjustSummary->SQL->Text = qrAdjustSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrAdjustSummary->SQL->Text = qrAdjustSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrAdjustSummary->SQL->Text = qrAdjustSummary->SQL->Text +
	  			"Archive.Order_Type,"
			"Sum(Archive.Qty) Item_Count,"
			"cast(Sum(Archive.Qty * Archive.BASE_PRICE - Archive.Qty * Archive.Price_Level0) as numeric(17, 4)) Total "
		"From "
			"Security Left Join Archive On "
				"Security.Security_Ref = Archive.Security_Ref "
         "Left Join ARCBILL On "
				 " Archive.ArcBill_Key=ArcBill.ArcBill_Key "
          " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Price Adjust' and "
			"Archive.Price <> Archive.Price_Level0 and "
            "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' " ;
      	if (Locations && Locations->Count > 0)
	{
		qrAdjustSummary->SQL->Text	=	qrAdjustSummary->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrAdjustSummary->SQL->Text		=	qrAdjustSummary->SQL->Text +
		"Group By " ;
      	if (Locations)
	{
		qrAdjustSummary->SQL->Text = qrAdjustSummary->SQL->Text +
			"ArcBill.Billed_Location, ";
	}
      qrAdjustSummary->SQL->Text = qrAdjustSummary->SQL->Text +
      "Archive.Order_Type ";
	qrAdjustSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrAdjustSummary->ParamByName("EndTime")->AsDateTime	= EndTime;
   	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrAdjustSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}

	qrWeightedGP->Close();
	qrWeightedGP->SQL->Text =

     "Select "
	    		" case when (arc.gp1)<>0 then  round(arc.gp1/ arc.gp2,2) else  round(arc.gp1,2) end as GP_PERCENT "
		       " from "
        "(select  "
        "cast(sum((((  cast( a.qty * a.price  as numeric(17, 4))) ) ) - a.qty * a.cost) as numeric(17, 4))  as gp1, "
        "cast(sum((( (cast(   a.qty * a.BASE_PRICE + COALESCE(a.DISCOUNT_WITHOUT_TAX,0)    as numeric(17, 4)))   ) ) ) as numeric(17, 4)) as gp2  "
        "from ARCHIVE a  "
         "Where  "
			"a.Time_Stamp_Billed >= :StartTime and "        //change Replace archive with a
			"a.Time_Stamp_Billed <  :EndTime "
         ")arc "  ;

   qrWeightedGP->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrWeightedGP->ParamByName("EndTime")->AsDateTime	= EndTime;


//-----
 qrCancelsAdj->Close();
	qrCancelsAdj->SQL->Text =

      "Select "
        "CanSec.Terminal_Name, "
        "cast(Archive.PRICE_LEVEL0 * Archive.Qty as numeric(17, 4)) Price, "
        "cast(1 as int) keyvalue "
		"From "
			"Security CanSec Inner Join Archive On "
				"CanSec.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts CanContact On "
				"CanContact.Contacts_Key = CanSec.User_Key "
			"Left Join ArcBill On "
				"Archive.ArcBill_Key = ArcBill.ArcBill_Key "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = Archive.Security_Ref "
			"Left Join Contacts OrdContact On "
				"OrdContact.Contacts_Key = OrdSec.User_Key "
        "LEFT JOIN ( "		

					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY, " 
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
     "LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "
             "Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
		     "((COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
             "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"CanSec.Security_Event = 'Cancel' and "
			"OrdSec.Security_Event = 'Ordered By' and "
			"Archive.Archive_Key Is Not Null "
 "union all "
		"Select "
			"CanSec.Terminal_Name, "			
			"cast(DayArchive.PRICE_LEVEL0 * DayArchive.Qty as numeric(17, 4)) Price, "
			"cast(1 as int) keyvalue "
		"From "
			"Security CanSec Inner Join DayArchive On "
				"CanSec.Security_Ref = DayArchive.Security_Ref "
			"Inner Join Contacts CanContact On "
				"CanContact.Contacts_Key = CanSec.User_Key " 
			"Left Join DayArcBill On "
				"DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = DayArchive.Security_Ref "
			"Left Join Contacts OrdContact On "
				"OrdContact.Contacts_Key = OrdSec.User_Key "
	"LEFT JOIN (  SELECT "
						"DAYARCORDERTAXES.ARCHIVE_KEY, " 
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge " 
				 " FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM DAYARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  DAYARCORDERTAXES "
						"GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
		"Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM DAYARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
	"Where "
           "DayArchive.TIME_STAMP_BILLED >= :StartTime and "
			"DayArchive.TIME_STAMP_BILLED < :EndTime and "
		    " (( COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
             "COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary') ) and "
			"CanSec.Security_Event = 'Cancel' and "
			"OrdSec.Security_Event = 'Ordered By' and "
			"DayArchive.Archive_Key Is Not Null "
	"union all "
         "Select "
			"CanSec.Terminal_Name, "
  			"cast(Orders.BASE_PRICE * Orders.Qty as numeric(17, 4)) Price, "
         "cast(1 as int) keyvalue  "
       "From "
			"Security CanSec Left Join Orders On "
			  "	CanSec.Security_Ref = Orders.Security_Ref "
			"Left Join Contacts CanContact On  "
				"CanContact.Contacts_Key = CanSec.User_Key  "
			"Left Join Security OrdSec On "
				"OrdSec.Security_Ref = Orders.Security_Ref "
			"Left Join Contacts OrdContact On  "
				"OrdContact.Contacts_Key = OrdSec.User_Key  "
		"Where "
			 "Orders.Time_Stamp >= :StartTime and "
			"Orders.Time_Stamp < :EndTime and "
			"CanSec.Security_Event = 'Cancel' and "
			"OrdSec.Security_Event = 'Ordered By' and "
			"Orders.Order_Key Is Not Null ";

      if (Locations && Locations->Count > 0)
	   {
		   qrCancelsAdj->SQL->Text	=	qrCancelsAdj->SQL->Text + "and (" +
											ParamString(Locations->Count, "orders.ORDER_LOCATION", "LocationParam") + ")";
	   }
   qrCancelsAdj->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCancelsAdj->ParamByName("EndTime")->AsDateTime	= EndTime;
   	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrCancelsAdj->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
   	

//-------for refunds
   qrRefunds->Close();
   qrRefunds->SQL->Text =
      		"Select ";
	if (Locations)
	{
		qrRefunds->SQL->Text = qrRefunds->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrRefunds->SQL->Text = qrRefunds->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrRefunds->SQL->Text = qrRefunds->SQL->Text +
  //		"Select "
			"Security.Terminal_Name,"

 " Cast((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.Discount,0)) as Numeric(17,4)) Total_Price, "
    " Cast((Archive.QTY * Archive.BASE_PRICE +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.OtherServiceCharge,0)+ COALESCE(Archive.Discount,0)) as Numeric(17,4)) Price, "

	  "cast(1 as int) keyvalue  "
		"From "
			"Security Inner Join Archive On "
				"Security.Security_Ref = Archive.Security_Ref "
			"Inner Join Contacts On "
				"Contacts.Contacts_Key = Security.User_Key "

" LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
                   "left join ARCBILL on ARCHIVE.ARCBILL_KEY=ARCBILL.ARCBILL_KEY "
		"Where "
			"Archive.TIME_STAMP_BILLED >= :StartTime and "
			"Archive.TIME_STAMP_BILLED < :EndTime and "
			"Security.Security_Event = 'Credit' ";
        	if (Locations && Locations->Count > 0)
	   {
		   qrRefunds->SQL->Text	=	qrRefunds->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	   }

	qrRefunds->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrRefunds->ParamByName("EndTime")->AsDateTime	= EndTime;
   	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrRefunds->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
}

//---------------------------------------------------------------------------
void TdmMMReportData::SetupFinanceDaily(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals, TStrings *Locations)
{

        SetupCashupLT(StartTime,  EndTime, Locations,Terminals) ;
        SalesSummaryDataLT(StartTime,  EndTime, Locations, Terminals);
        BillTendersByTerminal( StartTime,  EndTime, Terminals,Locations);
        qrDiscountsSurchargesTotals->Close();
        qrDiscountsSurchargesTotals->SQL->Text =

   //----------------------------------
      "Select "
                     "Cast(Security.From_Val As Varchar(50)) Discount_Name, "
                     "Cast((sum(Archive.DISCOUNT_WITHOUT_TAX)) as numeric(17,4)) as TOTAL "

            "From "
             "Security Left Join ArcBill On  "
             "Security.Security_Ref = ArcBill.Security_Ref  "
             "Left Join Archive On   "
             "ArcBill.ArcBill_Key = Archive.ArcBill_Key  "
             "Left Join ArcCategories on  "
              "Archive.Category_Key = ArcCategories.Category_Key   "
               "Left Join CategoryGroups on     "
            "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
            "LEFT JOIN (SELECT ARCORDERTAXES.ARCHIVE_KEY, "
               "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
               "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
               "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge  "
                  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,    "
               "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE   "
               "FROM ARCORDERTAXES a    "
                  "group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
                  "order by 1 )  ARCORDERTAXES   "
               "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )   "
               "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY  "
          "Where  "

      " ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where  "
         "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or  "
         "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary')  AND  "
         "Security.Security_Event = 'Discounted By' And  "
         "ArcBill.Invoice_Number Is Not Null and   "
      "Archive.Archive_Key Is Not Null and   "
      "Archive.Discount != 0 and   "
      "Archive.Qty != 0  and "
         "ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime  " ;
          if (Locations)
            {
                if (Locations->Count)
                {
                    qrDiscountsSurchargesTotals->SQL->Text 	=	qrDiscountsSurchargesTotals->SQL->Text + "And (" +
                    ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
                }

            }
             if (Terminals->Count > 0)
	         {
		            qrDiscountsSurchargesTotals->SQL->Text	=	qrDiscountsSurchargesTotals->SQL->Text + "and (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
            }
          qrDiscountsSurchargesTotals->SQL->Text        = qrDiscountsSurchargesTotals->SQL->Text +
			                   "group by 1    ";

   if (Locations)
	{
        if (Locations->Count)
        {
            for (int i=0; i<Locations->Count; i++)
            {
                 qrDiscountsSurchargesTotals->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
            }
        }
    }
   for (int i=0; i<Terminals->Count; i++)
	{
		qrDiscountsSurchargesTotals->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrDiscountsSurchargesTotals->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDiscountsSurchargesTotals->ParamByName("EndTime")->AsDateTime		= EndTime;

qrCatDiscount->Close();
	qrCatDiscount->SQL->Text =

"Select "
			"cast(Sum(Archive.DISCOUNT_WITHOUT_TAX) as numeric(17, 4)) Discount, "
			"cast(Sum(Archive.TAX_ON_DISCOUNT) as numeric(17, 4))  DiscountTax, "
			"Cast(Sum(Archive.QTY * Archive.BASE_PRICE  +COALESCE(AOT.VAT,0)+COALESCE( AOT.ServiceCharge,0) + COALESCE( AOT.ServiceChargeTax,0)) as Numeric(17,4)) Price, "
			"ArcCategories.Category, "
			"CategoryGroups.Name CategoryGroup, "
			"security.TERMINAL_NAME "
	"From "
			"Security Left Join ArcBill On "
			"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive On "
			"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
			"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
			"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "


		"LEFT JOIN ( "
			"SELECT "
			"ARCORDERTAXES.ARCHIVE_KEY,  "
			"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN  ARCORDERTAXES.TAX_VALUE END) AS VAT, "
			"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN  ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
			"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN  ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax "
			"FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
			"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
		"FROM ARCORDERTAXES a "
	"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
	"order by 1 )  ARCORDERTAXES "
	"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
	"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
	"Where "
   "ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where  "
   "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or  "
   "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary')  AND "
   "Security.Security_Event = 'Discounted By' And  "

"ArcBill.Time_Stamp >= :StartTime and "
 "ArcBill.Time_Stamp < :EndTime and "
"Security.Security_Event = 'Discounted By' And "
	"ArcBill.Invoice_Number Is Not Null and "
	"Archive.Archive_Key Is Not Null and "
	"Archive.Discount != 0 and "
	"Archive.Qty != 0 ";
	if (Locations)
	{if (Locations->Count)
	{
		qrCatDiscount->SQL->Text 	=	qrCatDiscount->SQL->Text + "And (" +
											ParamString(Locations->Count, "Archive.Order_Location", "LocationParam") + ")";
	}
	}
    if (Terminals->Count > 0)
	{
			qrCatDiscount->SQL->Text	=	qrCatDiscount->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}

	qrCatDiscount->SQL->Text        = qrCatDiscount->SQL->Text +

		"group by 4,5,6 "   ;

     if (Locations)
	{
        if (Locations->Count)
        {
            for (int i=0; i<Locations->Count; i++)
            {
                 qrCatDiscount->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
            }
        }
    }
   for (int i=0; i<Terminals->Count; i++)
	{
		qrCatDiscount->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}

	qrCatDiscount->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCatDiscount->ParamByName("EndTime")->AsDateTime		= EndTime;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TdmMMReportData::BillTendersByTerminal(TDateTime StartTime, TDateTime EndTime,
                                       TStrings *Terminals,  TStrings *Locations)
{

    AnsiString selected_terminals;
   AnsiString selected_locations;
    int i = 0;
    int j;
    if (Terminals->Count) {
        selected_terminals = " and s.terminal_name in ('";
        for (i = 0, j = Terminals->Count - 1; i < j; i++)
            selected_terminals =
              selected_terminals + Terminals->Strings[i] + "', '";
        selected_terminals =
          selected_terminals + Terminals->Strings[i] + "') ";
    }

   if (Locations->Count) {
        selected_locations = " and ab.BILLED_LOCATION in ('";
        for (i = 0, j = Locations->Count - 1; i < j; i++)
            selected_locations =
              selected_locations + Locations->Strings[i] + "', '";
        selected_locations =
          selected_locations + Locations->Strings[i] + "') ";
    }

    qrBillTenders->Close();
    qrBillTenders->SQL->Text =
      "select qpa.pay_type, "
      "       qpa.terminal_name, "
      "       cast(qpa.billed_by as VARCHAR(50)) billed_by, "
      "       cast(qpa.billed_at as TIMESTAMP) billed_at, "
      "       cast(qpa.receipt_no as VARCHAR(50)) receipt_no, "
      "       qpa.voucher_number,  "
      "       sum(qpb.change_recv) change_recv, "
      "       case when (qpa.pay_type <> 'Cash') "
      "                 then sum(qpa.total_paid + qpb.change_recv) "
      "                 else sum(qpa.total_paid) end total_paid, "
      "  cast((case when (qpa.pay_type <> 'Cash') "
                      "then sum(qpa.total_paid + qpb.change_recv) "
                     " else sum(qpa.total_paid) end ) - qpb.change_recv   as numeric(17, 4))  total, "
      "       qpa.note "
      "       from (select ab.arcbill_key, "
      "                    abp.pay_type, "
      "                    s.terminal_name, "
      "                    s.from_val billed_by, "
      "                    ab.time_stamp billed_at, "
      "                    ab.invoice_number receipt_no, "
      "                    abp.subtotal total_paid, "
      "                    abp.voucher_number voucher_number, "
      "                    abp.note note "
      "                    from arcbill ab "
      "                         inner join arcbillpay abp on "
      "                               abp.arcbill_key = ab.arcbill_key "
      "                         inner join security s on "
      "                               s.security_ref = ab.security_ref "
      "                    where abp.cash_out = 'F' "
      "                          and abp.note <> 'Total Change.' "
      "                          and s.security_event = 'Billed By' "
      "                          and ab.time_stamp between :start_date and :end_date "

      + selected_terminals + selected_locations
      + ") qpa "
      "            inner join (select abp.arcbill_key,  "
      "                               abp.voucher_number,  "
      "                               abp.subtotal * -1 change_recv  "
      "                               from arcbillpay abp  "
      "                               where abp.note = 'Total Change.') qpb on  "
      "                 qpb.arcbill_key = qpa.arcbill_key "
     
      "       group by qpa.pay_type, "
      "                qpa.terminal_name, "
      "                qpa.billed_by, "
      "                qpa.billed_at, "
      "                qpa.receipt_no, "
      "                qpa.voucher_number, "
      "                qpa.note , qpa.total_paid , qpb.change_recv "

      
      "order by 1, 2, 5, 3, 4, 6, 7, 8; ";

    qrBillTenders->ParamByName("start_date")->AsDateTime = StartTime;
    qrBillTenders->ParamByName("end_date")->AsDateTime = EndTime;
}
//---------------------------------------------------------------------------
//************************************************************//
 void TdmMMReportData::SalesSummaryDataLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations, TStrings *Terminals)
{

	qrSalesSummary->Close();
	qrSalesSummary->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"Sum(Archive.Qty) Item_Count,"
           " Cast(Sum(Archive.QTY * Archive.BASE_PRICE+Archive.Qty ) as Numeric(17,4)) PriceExc ,  "
             "   Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(AOT.VAT,0) + COALESCE(AOT.ServiceCharge,0) + COALESCE(AOT.OtherServiceCharge,0)) as Numeric(17,4)) PriceInc , "

			"cast(Sum(Archive.Cost * Archive.Qty) as numeric(17, 4))  Cost ,"
         "SECURITY.TERMINAL_NAME "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
                "LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
                  "Where Archive.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY "
                  "from ARCORDERDISCOUNTS where COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
                  "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') and "
                   "ARCHIVE.PRICE<>0 and "
			          "ArcBill.Time_Stamp >= :StartTime and "
			          "ArcBill.Time_Stamp < :EndTime and "
			          "Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrSalesSummary->SQL->Text	=	qrSalesSummary->SQL->Text + "and (" +
												ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
   if (Terminals->Count > 0)
	{
		qrSalesSummary->SQL->Text	=	qrSalesSummary->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrSalesSummary->SQL->Text		=	qrSalesSummary->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name ,"
         " SECURITY.TERMINAL_NAME "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
		"Order By "
			"1, 2,SECURITY.TERMINAL_NAME";
	qrSalesSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrSalesSummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrSalesSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
   for (int i=0; i<Terminals->Count; i++)
	{
		qrSalesSummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrPatronCount->Close();
	qrPatronCount->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Sales_Type,"
			"Sum(ArcBill.Patron_Count) Patron_Count, "
         "SECURITY.TERMINAL_NAME "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
		"Where "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"ArcBill.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrPatronCount->SQL->Text	=	qrPatronCount->SQL->Text + "and (" +
												ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
     if (Terminals->Count > 0)
	{
		qrPatronCount->SQL->Text	=	qrCashup->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrPatronCount->SQL->Text		=	qrPatronCount->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrPatronCount->SQL->Text = qrPatronCount->SQL->Text +
			"ArcBill.Sales_Type, "
         "SECURITY.TERMINAL_NAME "
		"Having "
			"Count(Patron_Count) > 0 "
		"Order By "
			"1,"
			"ArcBill.Sales_Type, "
         "SECURITY.TERMINAL_NAME ";
	qrPatronCount->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrPatronCount->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrPatronCount->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
   for (int i=0; i<Terminals->Count; i++)
	{
		qrPatronCount->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
	qrAveSummary->Close();
	qrAveSummary->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.ArcBill_Key,"
			"CategoryGroups.Name Category_Group,"
			"ArcBill.Sales_Type,"
			"cast(Max(ArcBill.Total) as numeric(17, 4)) Bill_Total,"
			"  Cast(Sum(Archive.QTY * Archive.BASE_PRICE + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(17,4)) Orders_Total ,	"
			"Max(ArcBill.Patron_Count) Patron_Count, "
         "SECURITY.TERMINAL_NAME "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
                "LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
          " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "

           "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and "
 "ARCHIVE.PRICE<>0 and "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrAveSummary->SQL->Text	=	qrAveSummary->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
   if (Terminals->Count > 0)
	{
		qrAveSummary->SQL->Text	=	qrCashup->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrAveSummary->SQL->Text		=	qrAveSummary->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrAveSummary->SQL->Text = qrAveSummary->SQL->Text +
			"ArcBill.ArcBill_Key,"
			"CategoryGroups.Name,"
			"ArcBill.Sales_Type, "
         "SECURITY.TERMINAL_NAME "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
		"Order By "
			"1,"
			"CategoryGroups.Name,"
			"ArcBill.Sales_Type, "
         "SECURITY.TERMINAL_NAME ";
	qrAveSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrAveSummary->ParamByName("EndTime")->AsDateTime	= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrAveSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
   for (int i=0; i<Terminals->Count; i++)
	{
		qrAveSummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCashupLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations, TStrings *Terminals)
{

	qrCashup->Close();
	qrCashup->SQL->Text =
		"Select "
			"Security.Terminal_Name,"
			"UPPER(ArcBillPay.Pay_Type) Pay_Type,"
			"ArcBillPay.Group_number,"
		 //	"ArcBillPay.Properties,"
			"Sum (ArcBillPay.SubTotal) SubTotal,"
          "ARCBILL.BILLED_LOCATION, "
         "cast(Count (distinct ArcBillPay.ArcBill_Key) as int) Trans_Count "

		"From "
			"(Security Left Outer Join ArcBill on "
			"Security.Security_Ref = ArcBill.Security_Ref) "
			"Left Outer Join ArcBillPay on "
			"ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
		"Where "
			"ARCBILL.Time_Stamp >= :StartTime and "
			"ARCBILL.Time_Stamp < :EndTime and "
         "ArcBillPay.Properties != 131072 and "
			"Security.Security_Event = 'Billed By'   and "
			"ArcBillPay.SubTotal <> 0  ";
	if (Terminals->Count > 0)
	{
		qrCashup->SQL->Text	=	qrCashup->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
   if (Locations && Locations->Count > 0)
	{
		qrCashup->SQL->Text	=	qrCashup->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrCashup->SQL->Text		=	qrCashup->SQL->Text +
		"Group By "
			"Security.Terminal_Name,"
			"ArcBillPay.Tax_Free,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type),"
		 //	"ArcBillPay.Properties, "
         "ARCBILL.BILLED_LOCATION "
		"Having "
			"Count (ArcBillPay.ArcBillPay_Key) > 0 "
		"Order By "
			"Security.Terminal_Name Asc, "
			"ArcBillPay.Tax_Free Desc, "
			"ArcBillPay.Group_number,"
         "ARCBILL.BILLED_LOCATION, "
			"UPPER(ArcBillPay.Pay_Type)  Asc";
	for (int i=0; i<Terminals->Count; i++)
	{
		qrCashup->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
   if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrCashup->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
   }
	qrCashup->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCashup->ParamByName("EndTime")->AsDateTime		= EndTime;


	qrCashupTotal->Close();
	qrCashupTotal->SQL->Text =
		"Select "
			"UPPER(ArcBillPay.Pay_Type) Pay_Type,"
			"ArcBillPay.Group_number,"
		   //	"ArcBillPay.Properties,"
			"Sum (ArcBillPay.SubTotal) SubTotal,"
			"cast(Count (distinct ArcBillPay.ArcBill_Key) as int) Trans_Count ,"
         "ARCBILL.BILLED_LOCATION "

		"From "
			"(Security Left Outer Join ArcBill on "
			"Security.Security_Ref = ArcBill.Security_Ref) "
			"Left Outer Join ArcBillPay on "
			"ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
		"Where "
			"ARCBILL.Time_Stamp >= :StartTime and "
			"ARCBILL.Time_Stamp < :EndTime and "
         "ArcBillPay.Properties != 131072 and "
			"Security.Security_Event = 'Billed By' and "
			"ArcBillPay.SubTotal <> 0  ";
	if (Terminals->Count > 0)
	{
		qrCashupTotal->SQL->Text	=	qrCashupTotal->SQL->Text + "and (" +
												ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
   if (Locations && Locations->Count > 0)
	{
		qrCashupTotal->SQL->Text	=	qrCashupTotal->SQL->Text + "and (" +
											ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
	qrCashupTotal->SQL->Text		=	qrCashupTotal->SQL->Text +
		"Group By "
			"ArcBillPay.Tax_Free,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type),"
		   //	"ArcBillPay.Properties, "
         "ARCBILL.BILLED_LOCATION "
		"Having "
			"Count (ArcBillPay.ArcBillPay_Key) > 0 "
		"Order By "
			"ArcBillPay.Tax_Free Desc,"
			"ArcBillPay.Group_number,"
			"UPPER(ArcBillPay.Pay_Type) Asc, "
         "ARCBILL.BILLED_LOCATION " ;

	for (int i=0; i<Terminals->Count; i++)
	{
		qrCashupTotal->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
   if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrCashupTotal->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
   }
	qrCashupTotal->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCashupTotal->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//----------------Following Method used in setupcashierand setup Finance methods  //This code is used in SalesSummaryDataLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations, TStrings *Terminals) function --so replace code by calling this method.
void TdmMMReportData::SalesSummaryLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations, TStrings *Terminals)
{

	qrSalesSummary->Close();
	qrSalesSummary->SQL->Text =
		"Select ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location Location,";
	}
	else
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"Cast('All Locations' As Varchar(25)) Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name Category_Group,"
			"Sum(Archive.Qty) Item_Count,"
           " Cast(Sum(Archive.QTY * Archive.BASE_PRICE+Archive.Qty ) as Numeric(17,4)) PriceExc ,  "
             "   Cast(Sum(Archive.QTY * Archive.BASE_PRICE  + COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)+ COALESCE(AOT.VAT,0) + COALESCE(AOT.ServiceCharge,0) + COALESCE(AOT.OtherServiceCharge,0)) as Numeric(17,4)) PriceInc , "

			"cast(Sum(Archive.Cost * Archive.Qty) as numeric(17, 4))  Cost ,"
         "SECURITY.TERMINAL_NAME "
		"From "
			"Security Left Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref "
			"Left Join Archive on "
				"ArcBill.ArcBill_Key = Archive.ArcBill_Key "
			"Left Join ArcCategories on "
				"Archive.Category_Key = ArcCategories.Category_Key "
			"Left Join CategoryGroups on "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
                "LEFT JOIN ( "
					 "SELECT "
						"ARCORDERTAXES.ARCHIVE_KEY,  "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
						"MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
				  "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE, "
						"Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
						"FROM ARCORDERTAXES a "
						"group by  a.TAX_TYPE  , a.ARCHIVE_KEY "
						"order by 1 )  ARCORDERTAXES "
						"GROUP BY ARCORDERTAXES.ARCHIVE_KEY ) "
						"AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY "
       " LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM ARCORDERDISCOUNTS a "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) "
		"ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
		"Where "

          "  COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
          "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  "

 "ARCHIVE.PRICE<>0 and "
			"ArcBill.Time_Stamp >= :StartTime and "
			"ArcBill.Time_Stamp < :EndTime and "
			"Security.Security_Event = 'Billed By' ";
	if (Locations && Locations->Count > 0)
	{
		qrSalesSummary->SQL->Text	=	qrSalesSummary->SQL->Text + "and (" +
												ParamString(Locations->Count, "ArcBill.Billed_Location", "LocationParam") + ")";
	}
   if (Terminals->Count > 0)
	{
		qrSalesSummary->SQL->Text	=	qrSalesSummary->SQL->Text + "and (" +
										ParamString(Terminals->Count, "Security.Terminal_Name", "TerminalParam") + ")";
	}
	qrSalesSummary->SQL->Text		=	qrSalesSummary->SQL->Text +
		"Group By ";
	if (Locations)
	{
		qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"ArcBill.Billed_Location,";
	}
	qrSalesSummary->SQL->Text = qrSalesSummary->SQL->Text +
			"CategoryGroups.Name ,"
         " SECURITY.TERMINAL_NAME "
		"Having "
			"Count(Archive.Archive_Key) > 0 "
		"Order By "
			"1, 2,7 ";
	qrSalesSummary->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrSalesSummary->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrSalesSummary->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
   for (int i=0; i<Terminals->Count; i++)
	{
		qrSalesSummary->ParamByName("TerminalParam" + IntToStr(i))->AsString = Terminals->Strings[i];
	}
}

//MM-2966
void TdmMMReportData::SetupConsolidatedSalesByItem(TDateTime StartTime, TDateTime EndTime)
{   try
	{
       	qrDSRTrans->Close();
    if(qrDSRTrans->Transaction->Active ==true)
  {
   qrDSRTrans -> Transaction -> Commit();
  }


  try{


	   //	sqlAddLK->Close();
  }
  catch (...)
		 { qrDSRTrans -> Transaction -> Commit();

		 }

 if(qrDSRTrans->Transaction->Active ==false)
{
   AnsiString CreateTableStmt="execute procedure CREATE_DSR_BY_ITEM ( 'DSRREPORTBYITEM', :StartTime ,:EndTime ) ";
   qrDSRTrans -> Close();
   qrDSRTrans -> SQL -> Clear();
qrDSRTrans    -> SQL -> Add(CreateTableStmt);
 qrDSRTrans->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRTrans->ParamByName("EndTime")->AsDateTime		= EndTime;
   qrDSRTrans -> Transaction -> StartTransaction();
   qrDSRTrans-> ExecSQL ();
   qrDSRTrans -> Transaction -> Commit();

   AnsiString populatedatastmt;
   populatedatastmt = "execute procedure POPULATE_DSR_BY_ITEM ( 'DSRREPORTBYITEM', :StartTime ,:EndTime ) ";

   qrDSRTrans-> Close ();
   qrDSRTrans -> SQL -> Clear();
   qrDSRTrans -> SQL -> Add(populatedatastmt);
   qrDSRTrans->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRTrans->ParamByName("EndTime")->AsDateTime		= EndTime;

   qrDSRTrans -> Transaction -> StartTransaction();
   qrDSRTrans-> ExecSQL ();
   qrDSRTrans -> Transaction -> Commit();
   	qrDSRTrans->Close();
}
        qrDSRConsolidatedByItem->Close();
	     qrDSRConsolidatedByItem->SQL->Text =
"  SELECT * "
" from DSRREPORTBYITEM a "
	" WHERE "
			"A.TIME_STAMP >= :StartTime and "
			"A.TIME_STAMP < :EndTime  ";
	qrDSRConsolidatedByItem->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrDSRConsolidatedByItem->ParamByName("EndTime")->AsDateTime		= EndTime;

}
	__finally
	{

	//qrDSRConsolidatedBYItem -> Transaction -> Commit();
	}
}

//MM-4327
//---------------------------------------------------------------------------
void TdmMMReportData::SetupCheckRemoval(TDateTime StartTime, TDateTime EndTime)   //MM-4327
{
	qrCheckRemoval->Close();
	qrCheckRemoval->SQL->Text =
        "SELECT "
        "b.TIME_STAMP, "
        "Extract(Day From b.Time_Stamp) Item_Day, "
        "Extract(Second From b.Time_Stamp) Item_Second, "
        "b.ORDER_LOCATION as \"LOCATION\", "
        "a.TERMINAL_NAME, "
        "d.NAME, "
        "b.PRICE, "
        "b.SIZE_NAME, "
        "b.ITEM_NAME,  "
        "b.QTY, "
        "b.REMOVE_USER  "
    "FROM "
        "SECURITY a "
    "INNER JOIN "
        "CANCELITEMS b "
    "ON "
        "a.SECURITY_REF = b.SECURITY_REF "
    "INNER JOIN "
        "CONTACTS d "
    "ON "
        "a.USER_KEY = d.CONTACTS_KEY "
    "WHERE "
        "a.SECURITY_EVENT = 'Check Removal' "
        "AND b.TIME_STAMP > :StartTime "
        "AND b.TIME_STAMP <= :EndTime "

        "Order By "
        "1 "    ;

	qrCheckRemoval->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrCheckRemoval->ParamByName("EndTime")->AsDateTime	= EndTime;


}
//MM-4101

 void TdmMMReportData::SetupDSRSummary(TDateTime StartTime, TDateTime EndTime,TStrings *Menus)
{
      	qrDSRCategory->Close();
		qrDSRCategory->SQL->Text =
        "Select ";
        if (Menus)
        {
            qrDSRCategory->SQL->Text = qrDSRCategory->SQL->Text +
                "Archive.MENU_NAME Category, ";
        }
        else
        {
            qrDSRCategory->SQL->Text = qrDSRCategory->SQL->Text +
                "ArcCategories.Category,";
        }
        qrDSRCategory->SQL->Text = qrDSRCategory->SQL->Text +

		" cast(sum(ABS(Archive.BASE_PRICE)*ARCHIVE.qty)as Numeric(17,4)) price "
		"From Archive "
		"Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  left join ARCBILL on ARCBILL.ARCBILL_KEY=ARCHIVE.ARCBILL_KEY left join security on ARCHIVE.SECURITY_REF= security.SECURITY_REF   "
		"Where  security.SECURITY_EVENT='Ordered By' and ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where "
       "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary') "
       "and ARCBILL.TIME_STAMP >= :StartTime and  ARCBILL.TIME_STAMP < :EndTime  "
       	"Group By ";
        if (Menus)
        {
            qrDSRCategory->SQL->Text = qrDSRCategory->SQL->Text +
                "Archive.MENU_NAME ";
        }
        else
        {
        qrDSRCategory->SQL->Text = qrDSRCategory->SQL->Text +
        "ArcCategories.Category " ;
        }

		qrDSRCategory->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRCategory->ParamByName("EndTime")->AsDateTime	= EndTime;

		qrDSRTax->Close();
		qrDSRTax->SQL->Text =
		"select  a.TAX_NAME, cast(sum(a.TAX_VALUE)as Numeric(17,4))  TAX_VALUE, cast ((CASE WHEN a.TAX_TYPE = 0  THEN  ('Sales Tax') "
        "when a.TAX_TYPE = 1  THEN  ('Purchase Taxes') "
        "when a.TAX_TYPE = 2  THEN  ('Service Charges') "
        "when a.TAX_TYPE = 3  THEN  ('Service Charge Taxes') "
        "when a.TAX_TYPE = 4  THEN  ('Local Taxes') "
        "when a.TAX_TYPE >4  THEN  ('Other Taxes') "
        "END) as varchar(50))as TAX_TYPE  "
		"FROM ARCORDERTAXES a inner join ARCHIVE AC on AC.ARCHIVE_KEY=a.ARCHIVE_KEY INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY=AC.ARCBILL_KEY   "
		"where  a.TAX_NAME<>''  and AC.TIME_STAMP_BILLED>=:StartTime and AC.TIME_STAMP_BILLED< :EndTime "
		"group BY   a.TAX_NAME, a.TAX_TYPE   "
        "order by a.TAX_TYPE  ";
		qrDSRTax->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRTax->ParamByName("EndTime")->AsDateTime	= EndTime;

		qrDSRDiscount->Close();
		qrDSRDiscount->SQL->Text = 
		"select DISCOUNT.NAME,cast(sum(DISCOUNT.Discount_Without_Tax)as Numeric(17,4)) Discount_Without_Tax,cast(sum(DISCOUNT.Tax_On_Discount)as Numeric(17,4)) Tax_On_Discount "
		"from (Select ARCORDERDISCOUNTS.NAME , MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE <0 THEN  "
		"coalesce(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT),0) END)  AS Discount_Without_Tax,MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE <0 THEN  "
		"coalesce(ARCORDERDISCOUNTS.DISCOUNTED_VALUE-(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT)),0) END)  AS Tax_On_Discount "
		"From ARCORDERDISCOUNTS  left join Archive on archive.ARCHIVE_KEY=ARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where   "
		"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and  "
		"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  ARCORDERDISCOUNTS.DISCOUNTED_VALUE <0 and "
		"Archive.TIME_STAMP_BILLED>=:StartTime and Archive.TIME_STAMP_BILLED< :EndTime "
		"Group By ARCORDERDISCOUNTS.NAME,ARChive.ARCHIVE_KEY  )DISCOUNT "
		" group by DISCOUNT.NAME  " ;
		qrDSRDiscount->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRDiscount->ParamByName("EndTime")->AsDateTime	= EndTime;

		qrDSRrefcan->Close();
		qrDSRrefcan->SQL->Text =
		"select  "
		"CAST(COALESCE((SELECT SUM(CAST(ABS(ARCHIVE.PRICE) * ARCHIVE.QTY AS NUMERIC(17,4))) FROM ARCHIVE LEFT JOIN SECURITY ON ARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
		"WHERE SECURITY.SECURITY_EVENT = 'Credit' AND ARCHIVE.TIME_STAMP >= :StartTime AND ARCHIVE.TIME_STAMP < :EndTime), 0) AS NUMERIC(17,4)) AS TOTALREFUNDS, "
		"COALESCE((SELECT CAST(SUM(ARCHIVE.PRICE_LEVEL0 * -1) AS NUMERIC(17,4)) FROM ARCHIVE LEFT JOIN SECURITY ON SECURITY.SECURITY_REF = ARCHIVE.SECURITY_REF   "
		"LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY = ARCHIVE.ARCBILL_KEY AND SECURITY.SECURITY_REF = ARCBILL.SECURITY_REF "
		"LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY WHERE (SECURITY.SECURITY_EVENT = 'Cancel' or SECURITY.SECURITY_EVENT = 'CancelY') AND  "
		"ARCHIVE.TIME_STAMP_BILLED >= :StartTime AND ARCHIVE.TIME_STAMP_BILLED<:EndTime),0) AS TOTALCANCEL "
		"FROM ARCHIVE  "
		"group by TOTALREFUNDS,TOTALCANCEL  ";
		qrDSRrefcan->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRrefcan->ParamByName("EndTime")->AsDateTime	= EndTime;

		qrDSRSurcharge->Close();
		qrDSRSurcharge->SQL->Text = 
		"select SURCHARGE.NAME,cast(sum(SURCHARGE.Surcharge_Without_Tax)as numeric(17,4)) Surcharge_Without_Tax,cast(sum(SURCHARGE.Tax_On_Surcharge)as numeric(17,4)) Tax_On_Surcharge "
		"from ( Select ARCORDERDISCOUNTS.NAME , MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE >0 THEN  "
		"coalesce(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT),0) END)  AS Surcharge_Without_Tax,MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE >0 THEN  "
		"coalesce(ARCORDERDISCOUNTS.DISCOUNTED_VALUE-(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT)),0) END)  AS Tax_On_Surcharge "
		"From ARCORDERDISCOUNTS  left join Archive on archive.ARCHIVE_KEY=ARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where   "
		"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and  "
		"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  ARCORDERDISCOUNTS.DISCOUNTED_VALUE >0 and "
		"Archive.TIME_STAMP_BILLED>=:StartTime and Archive.TIME_STAMP_BILLED< :EndTime "
		"Group By ARCORDERDISCOUNTS.NAME ,Archive.ARCHIVE_KEY  )SURCHARGE "
		" group by SURCHARGE.NAME  " ; 
		qrDSRSurcharge->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRSurcharge->ParamByName("EndTime")->AsDateTime	= EndTime;

		qrDSRPay->Close();
		qrDSRPay->SQL->Text =
		"select  "
		"UPPER(ARCBILLPAY.PAY_TYPE) PAYMENT_NAME, cast(SUM(ARCBILLPAY.SUBTOTAL)as numeric(17,4)) SUBTOTAL  "
		"FROM ARCBILLPAY inner JOIN ARCBILL ON ARCBILLPAY.ARCBILL_KEY = ARCBILL.ARCBILL_KEY inner join security on security.SECURITY_REF=ARCBILL.SECURITY_REF "
		"WHERE ArcBillPay.Properties != 131072 and Security.Security_Event = 'Billed By' and ARCBILL.TIME_STAMP >=:StartTime AND ARCBILL.TIME_STAMP <:EndTime "
		"GROUP BY UPPER(ARCBILLPAY.PAY_TYPE) ";
		qrDSRPay->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRPay->ParamByName("EndTime")->AsDateTime	= EndTime;

        SetupDSRSum(StartTime,EndTime);


}
void TdmMMReportData::SetupDSRSum(TDateTime StartTime, TDateTime EndTime)
{    	qrDSRSum->Close();
		qrDSRSum->SQL->Text =
 	"select a.categorysum,a.taxessum,a.discountsum,a.surchargesum,cast((a.categorysum+a.taxessum+a.discountsum+a.surchargesum)as numeric(17,4))GrandTotal, "
    "cast((a.categorysum+a.discountsum+a.surchargesum)as numeric(17,4))NetTotal,cast((a.PAIDAMOUNT-(a.categorysum+a.taxessum+a.discountsum+a.surchargesum))as numeric(17,4))RoundOff  "
	"from(select coalesce((select cast(sum(ABS(Archive.BASE_PRICE)*ARCHIVE.qty)as Numeric(17,4)) price From Archive "
	"Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  left join ARCBILL on ARCBILL.ARCBILL_KEY=ARCHIVE.ARCBILL_KEY left join security on ARCHIVE.SECURITY_REF= security.SECURITY_REF    "
	"Where security.SECURITY_EVENT='Ordered By' and ARCHIVE.ARCHIVE_KEY not in (Select ARCORDERDISCOUNTS.ARCHIVE_KEY from ARCORDERDISCOUNTS  where "
       "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Non-Chargeable' or "
       "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) = 'Complimentary')and ARCBILL.TIME_STAMP >= :StartTime and ARCBILL.TIME_STAMP < :EndTime),0)as categorysum,	"

	"coalesce((select  cast(sum(a.TAX_VALUE)as Numeric(17,4))  TAX_VALUE "
	"FROM ARCORDERTAXES a inner join ARCHIVE AC on AC.ARCHIVE_KEY=a.ARCHIVE_KEY INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY=AC.ARCBILL_KEY  "
	"where  a.TAX_NAME<>''and AC.TIME_STAMP_BILLED>=:StartTime and AC.TIME_STAMP_BILLED< :EndTime),0)as taxessum, "


	"coalesce((select cast(sum(DISCOUNT.Discount_Without_Tax)as Numeric(17,4)) Discount_Without_Tax "
	"from (Select ARCORDERDISCOUNTS.NAME,MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE <0 THEN " 
	"coalesce(ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT),0) END)  AS Discount_Without_Tax "
	"From ARCORDERDISCOUNTS  inner join Archive on archive.ARCHIVE_KEY=ARCORDERDISCOUNTS.ARCHIVE_KEY  "
	"where  " 
	"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and " 
	"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  ARCORDERDISCOUNTS.DISCOUNTED_VALUE <0 and "
	"archive.TIME_STAMP_BILLED>=:StartTime and archive.TIME_STAMP_BILLED< :EndTime "
	"Group By ARChive.ARCHIVE_KEY,ARCORDERDISCOUNTS.NAME "
	")DISCOUNT),0)as discountsum, "

	"coalesce((select cast(sum(SURCHARGE.Surcharge_Without_Tax)as numeric(17,4)) Surcharge_Without_Tax "
	"from ( Select  ARCORDERDISCOUNTS.NAME, MIN(CASE WHEN ARCORDERDISCOUNTS.DISCOUNTED_VALUE >0 THEN  "
	"coalesce((ARCORDERDISCOUNTS.DISCOUNTED_VALUE*(ARCHIVE.DISCOUNT_WITHOUT_TAX/ARCHIVE.DISCOUNT)),0) END)  AS Surcharge_Without_Tax "
	"From ARCORDERDISCOUNTS  left join Archive on archive.ARCHIVE_KEY=ARCORDERDISCOUNTS.ARCHIVE_KEY  "
	"where   "
	"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and  "
	"COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary'  and  ARCORDERDISCOUNTS.DISCOUNTED_VALUE >0  and "
	"ARChive.TIME_STAMP_BILLED>=:StartTime and ARChive.TIME_STAMP_BILLED< :EndTime "
	"Group By  ARCORDERDISCOUNTS.NAME,ARChive.ARCHIVE_KEY  )SURCHARGE),0)as surchargesum, "

	"coalesce((select "
	"cast(SUM(ARCBILLPAY.SUBTOTAL)as numeric(17,4)) SUBTOTAL "
	"FROM ARCBILLPAY inner JOIN ARCBILL ON ARCBILLPAY.ARCBILL_KEY = ARCBILL.ARCBILL_KEY inner join security on security.SECURITY_REF=ARCBILL.SECURITY_REF "
	 "WHERE ArcBillPay.Properties != 131072 and Security.Security_Event = 'Billed By' and ARCBILL.TIME_STAMP >=:StartTime AND ARCBILL.TIME_STAMP <:EndTime),0)as PAIDAMOUNT "
	"from archive )a "
	"group by a.categorysum,a.taxessum,a.discountsum,a.surchargesum,a.PAIDAMOUNT  " ;

		qrDSRSum->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrDSRSum->ParamByName("EndTime")->AsDateTime	= EndTime;


  }

//---------------------------------------------------------------------------
       void TdmMMReportData::SetupReprintReceipt(TDateTime StartTime, TDateTime EndTime)   //MM-4327
{
	qrReprintReceipt->Close();
	qrReprintReceipt->SQL->Text =

      	"SELECT a.SECURITY_KEY, "
	"a.SECURITY_REF, "
	"a.SECURITY_EVENT, "
	"a.FROM_VAL, a.TO_VAL, "
	"a.NOTE as Invoicenumber , "
    "a.TERMINAL_NAME, "
	"a.USER_KEY, "
	"a.TIME_STAMP "
 	"FROM SECURITY a "
	"where a.SECURITY_EVENT='Reprint Receipt' "
         "AND a.TIME_STAMP > :StartTime "
        "AND a.TIME_STAMP <= :EndTime " ; 

	qrReprintReceipt->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrReprintReceipt->ParamByName("EndTime")->AsDateTime	= EndTime;


}
void TdmMMReportData::ResetPoints(TDateTime StartTime, TDateTime EndTime, int i) //MM-4579
{
   qrResetPoints->Close();
   switch(i)
   {
      case 0:
      {
         qrResetPoints->SQL->Text =
            "SELECT contacts.NAME,"
            "a.TIMESTAMPS, a.CONTACTS_KEY, a.POINTS,a.ADJUSTMENT_TYPE "
            "FROM RESETPOINTS a "
            "left join contacts on contacts.CONTACTS_KEY=a.CONTACTS_KEY "
            "where a.TIMESTAMPS > :StartTime "
            "AND a.TIMESTAMPS <= :EndTime ";
         break;
      }
      case 1:
      {
         qrResetPoints->SQL->Text =
            "SELECT contacts.NAME,"
            "a.TIMESTAMPS, a.CONTACTS_KEY, a.POINTS,a.ADJUSTMENT_TYPE "
            "FROM RESETPOINTS a "
            "left join contacts on contacts.CONTACTS_KEY=a.CONTACTS_KEY "
            "where a.TIMESTAMPS > :StartTime "
            "AND a.TIMESTAMPS <= :EndTime and a.ADJUSTMENT_TYPE = 'Purchased'";
         break;
      }
      case 2:
      {
         qrResetPoints->SQL->Text =
            "SELECT contacts.NAME,"
            "a.TIMESTAMPS, a.CONTACTS_KEY, a.POINTS,a.ADJUSTMENT_TYPE "
            "FROM RESETPOINTS a "

            "left join contacts on contacts.CONTACTS_KEY=a.CONTACTS_KEY "
            "where a.TIMESTAMPS > :StartTime "
            "AND a.TIMESTAMPS <= :EndTime and a.ADJUSTMENT_TYPE = 'Earned'";
         break;
      }
      case 3:
      {
         qrResetPoints->SQL->Text =
            "SELECT contacts.NAME,"
            "a.TIMESTAMPS, a.CONTACTS_KEY, a.POINTS,a.ADJUSTMENT_TYPE "
            "FROM RESETPOINTS a "
            "left join contacts on contacts.CONTACTS_KEY=a.CONTACTS_KEY "
            "where a.TIMESTAMPS > :StartTime "
            "AND a.TIMESTAMPS <= :EndTime and a.ADJUSTMENT_TYPE = 'Redeemed'";
         break;
      }
   }
	qrResetPoints->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrResetPoints->ParamByName("EndTime")->AsDateTime	= EndTime;
}

 void TdmMMReportData::SetupReprintOrder(TDateTime StartTime, TDateTime EndTime)   //MM-4766
{
	qrReprintOrder->Close();
	qrReprintOrder->SQL->Text =


     "SELECT a.REPRINTORDER_KEY, a.DOCKETS_NUMBER, a.USERNAME, a.TERMINAL_NAME, a.TIME_STAMP "
     "FROM REPRINTORDER a "
     "where a.TIME_STAMP > :StartTime "
        "AND a.TIME_STAMP <= :EndTime " ;


	qrReprintOrder->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrReprintOrder->ParamByName("EndTime")->AsDateTime	= EndTime;


}

//---------------------------------------------------------------------------------------------------------------------------

void TdmMMReportData::SetupProfiltLoss(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{

    qrProfiltLoss->Close();
    qrProfiltLoss->SQL->Text =
    "SELECT    ";
    if (Locations)
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
            "   CombinedQuery.Location,                   ";
    }
    else
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
        "Cast('All Locations' As Varchar(25)) Location,";
    }
    qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +

    "   CombinedQuery.time_stamp ,                "
    "   CombinedQuery.invoice_number,             "
    "   CombinedQuery.ARCBILL_KEY,                "
    "   CombinedQuery.Qty,                        "
    "   CombinedQuery.ITEM_NAME,                  "
    "   CombinedQuery.Unit,                       "
    "   CombinedQuery.Unit_Price ,                "
    "   CombinedQuery.subtotal,                  "
    "   CombinedQuery.Tax,                        "
    "   CombinedQuery.DISCOUNT,                   "
    "   CombinedQuery.TAX_ON_DISCOUNT,            "
    "   Cast(CombinedQuery.NetAmount - CombinedQuery.Tax AS Numeric(17,4)) NetAmount, "
    "   CombinedQuery.Cost, "
    "   Cast(((CombinedQuery.NetAmount  - CombinedQuery.Tax)- CombinedQuery.Cost)  AS Numeric(17,4))Profit_Loss "
    "from "
    "(    "
                "SELECT "
                  " AB.BILLED_LOCATION Location, "

                  " AB.TOTAL FTOTAL ,     "
                  " AB.time_stamp ,       "
                  " AB.invoice_number,    "
                  " AB.ARCBILL_KEY,       "
                  " Sum(da.Qty) Qty,       "
                  " da.ITEM_NAME,          "
                  " da.SIZE_NAME Unit,     "
                  "cast((CASE WHEN coalesce(DA.PRICE_ADJUST,0) <> 0  THEN  coalesce(DA.PRICE_ADJUST,0) "
                              " WHEN DA.HAPPY_HOUR = 'F' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  coalesce(DA.PRICE_LEVEL0,0)  "
                              " WHEN DA.HAPPY_HOUR = 'T' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  coalesce(DA.PRICE_LEVEL1,0)  "
                              " WHEN coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE = 0 THEN  coalesce(DA.PRICE,0)  "
                        "END) as numeric(17, 4)) AS Unit_Price, "

                     "cast((CASE WHEN coalesce(DA.PRICE_ADJUST,0) <> 0  THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_ADJUST,0)) "
                                 "WHEN DA.HAPPY_HOUR = 'F' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_LEVEL0,0)) "
                                 "WHEN DA.HAPPY_HOUR = 'T' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_LEVEL1,0)) "
                                 "WHEN coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE = 0 THEN  SUM(coalesce(DA.Qty, 0) *coalesce(DA.PRICE,0)) "
                        "END) as numeric(17, 4)) AS SubTotal, "
                  " Cast(+ Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) Tax,    "
                  " cast(Sum(  COALESCE(DA.DISCOUNT,0)) +  COALESCE((ARCSURCHARGE.SUBTOTAL/ARC.noOfItem),0) as numeric(17, 4)) AS DISCOUNT, "
                  " cast(Sum(  COALESCE(DA.TAX_ON_DISCOUNT,0)) as numeric(17, 4)) AS TAX_ON_DISCOUNT, "
                  " Cast(Sum(abs(DA.Cost)* DA.Qty) as Numeric(17,4)) Cost,  "
                  " CAST(Sum( cast(DA.Qty * DA .BASE_PRICE as Numeric(17,4)))   + Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0)) + Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))+   COALESCE((ARCSURCHARGE.SUBTOTAL/ARC.noOfItem),0) AS Numeric(17,4)) NetAmount "

            "FROM ARCBILL AB "
                 "  INNER JOIN ARCHIVE DA ON AB.ARCBILL_KEY = DA.ARCBILL_KEY "
                 "  Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "

                 "                  FROM ARCORDERDISCOUNTS a                             "
                 "              group by                                                 "
                 "                  a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)                 "
                 "  DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY                            "
                 "  LEFT JOIN ARCSURCHARGE ON ARCSURCHARGE.ARCBILL_KEY = AB.ARCBILL_KEY  "
                "LEFT JOIN ( "
                            "	 SELECT "
                            "		ARCORDERTAXES.ARCHIVE_KEY,                                                                        "
                            "		MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,             "
                            "		MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,   "
                            "		MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge "
                            "  FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                   "
                            "		Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE   "
                            "		FROM ARCORDERTAXES a                              "
                            "		group by  a.TAX_TYPE  , a.ARCHIVE_KEY                "
                            "		order by 1 )  ARCORDERTAXES                       "
                            "		GROUP BY ARCORDERTAXES.ARCHIVE_KEY )              "
                            "		AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY              "
                 "  LEFT JOIN (                                                          "
                 "              SELECT count(*) noOfItem, DA.ARCBILL_KEY                 "
                 "              FROM (select                                             "
                 "                      a.ARCBILL_KEY, a.ITEM_NAME,a.SIZE_NAME                       "
                 "                      FROM ARCHIVE a                                   "
                 "                      GROUP BY                                         "
                 "                      1,2,3) DA                                          "
                 "                      GROUP BY                                         "
                 "                      DA.ARCBILL_KEY                                   "
                 "                      ORDER BY 2) ARC ON ARC.ARCBILL_KEY = AB.ARCBILL_KEY "
                 "     Where DA.ARCHIVE_KEY not in  "
                 "     (Select     ARCHIVE.ARCHIVE_KEY from ARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=ARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and "
                 "((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) "

            "AND "
            "AB.time_stamp >= :StartTime and  "
                "		AB.time_stamp < :EndTime "

                "GROUP BY "
                        "AB.BILLED_LOCATION, "
                        " AB.invoice_number, "
                        "AB.time_stamp, DAOD.DISCOUNTED_VALUE, "
                        " da.QTY, da.ITEM_NAME,da.SIZE_NAME,AB.TOTAL , "
                        "AB.ARCBILL_KEY ,DA.DISCOUNT, "
                        "ARC.noOfItem,ARCSURCHARGE.SUBTOTAL, DA.PRICE_ADJUST,DA.PRICE , "
                        " DA.PRICE_LEVEL0, DA.PRICE_LEVEL1,DA.HAPPY_HOUR  "
    ")CombinedQuery  "
    "WHERE "
    "CombinedQuery.time_stamp >= :StartTime and  "
    " CombinedQuery.time_stamp < :EndTime ";


    if (Locations && Locations->Count > 0)
    {
        qrProfiltLoss->SQL->Text	=	qrProfiltLoss->SQL->Text + "and (" +
                                                ParamString(Locations->Count, "CombinedQuery.Location ", "LocationParam") + ")";
    }
    qrProfiltLoss->SQL->Text		=	qrProfiltLoss->SQL->Text +
     "GROUP BY ";
    if (Locations)
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
            "CombinedQuery.LOCATION, ";
    }
    qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +

  " CombinedQuery.time_stamp ,      "
  " CombinedQuery.invoice_number,   "
  " CombinedQuery.ARCBILL_KEY,      "
  " CombinedQuery.Qty,              "
  " CombinedQuery.ITEM_NAME,        "
  " CombinedQuery.Unit,             "
  " CombinedQuery.Unit_Price ,      "
  " CombinedQuery.qty ,             "
  " CombinedQuery.Tax,              "
  " CombinedQuery.DISCOUNT,         "
  " CombinedQuery.SubTotal ,        "
  " CombinedQuery.Cost,             "
  " CombinedQuery.TAX_ON_DISCOUNT,   "
  "CombinedQuery.NetAmount  ";

    qrProfiltLoss->ParamByName("StartTime")->AsDateTime	= StartTime;
    qrProfiltLoss->ParamByName("EndTime")->AsDateTime		= EndTime;
     if (Locations)
    {
        for (int i=0; i<Locations->Count; i++)
        {
            qrProfiltLoss->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
        }
    }
   qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +

	 "UNION ALL "


     "SELECT ";
    if (Locations)
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
            "   CombinedQuery2.Location,                   ";
    }
    else
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
        "Cast('All Locations' As Varchar(25)) Location,";
    }
    qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
    "   CombinedQuery2.time_stamp ,                "
    "   CombinedQuery2.invoice_number,             "
    "   CombinedQuery2.ARCBILL_KEY,                "
    "   CombinedQuery2.Qty,                        "
    "   CombinedQuery2.ITEM_NAME,                  "
    "   CombinedQuery2.Unit,                       "
    "   CombinedQuery2.Unit_Price ,                "
    "   CombinedQuery2.subtotal ,  "
    "   CombinedQuery2.Tax,                        "
    "   CombinedQuery2.DISCOUNT,                   "
    "   CombinedQuery2.TAX_ON_DISCOUNT,            "
    "   Cast(CombinedQuery2.NetAmount  - CombinedQuery2.Tax AS Numeric(17,4)) NetAmount, "
    "   CombinedQuery2.Cost, "
    "   Cast(((CombinedQuery2.NetAmount - CombinedQuery2.Tax)- CombinedQuery2.Cost)  AS Numeric(17,4))Profit_Loss "
    "from "
    "(    "
                  "select "
                  " DAB.BILLED_LOCATION Location, "

                  " DAB.TOTAL FTOTAL ,     "
                  " DAB.time_stamp ,       "
                  " DAB.invoice_number,    "
                  " DAB.ARCBILL_KEY,       "
                  " Sum(da.Qty) Qty,       "
                  " da.ITEM_NAME,          "
                  " da.SIZE_NAME Unit,     "
                  "cast((CASE WHEN coalesce(DA.PRICE_ADJUST,0) <> 0  THEN  coalesce(DA.PRICE_ADJUST,0) "
                              " WHEN DA.HAPPY_HOUR = 'F' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  coalesce(DA.PRICE_LEVEL0, 0)  "
                              " WHEN DA.HAPPY_HOUR = 'T' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  coalesce(DA.PRICE_LEVEL1, 0)  "
                              " WHEN coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE = 0 THEN  coalesce(DA.PRICE,0)  "
                        "END) as numeric(17, 4)) AS Unit_Price, "

                     "cast((CASE WHEN coalesce(DA.PRICE_ADJUST,0) <> 0  THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_ADJUST,0)) "
                                 "WHEN DA.HAPPY_HOUR = 'F' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_LEVEL0, 0)) "
                                 "WHEN DA.HAPPY_HOUR = 'T' AND coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE <> 0 THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE_LEVEL1, 0)) "
                                 "WHEN coalesce(DA.PRICE_ADJUST,0) = 0 AND DA.PRICE = 0 THEN  SUM(coalesce(DA.Qty,0) *coalesce(DA.PRICE, 0)) "
                        "END) as numeric(17, 4)) AS SubTotal, "
                  " Cast(+ Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0)) as Numeric(17,4)) Tax,    "
                  " cast(Sum(  COALESCE(DA.DISCOUNT,0)) +  COALESCE((DAYARCSURCHARGE.SUBTOTAL/DAC.noOfItem),0) as numeric(17, 4)) AS DISCOUNT, "
                  " cast(Sum(  COALESCE(DA.TAX_ON_DISCOUNT,0)) as numeric(17, 4)) AS TAX_ON_DISCOUNT,  "
                  " Cast(Sum(abs(DA.Cost)* DA.Qty) as Numeric(17,4)) Cost,   "
                 " CAST(Sum( cast(DA.Qty * DA .BASE_PRICE as Numeric(17,4)))   + Sum(COALESCE(AOT.VAT,0))+ Sum(COALESCE( AOT.ServiceCharge,0)) + Sum(COALESCE( AOT.OtherServiceCharge,0)) + Sum(  COALESCE(DA.DISCOUNT_WITHOUT_TAX,0))+   COALESCE((DAYARCSURCHARGE.SUBTOTAL/DAC.noOfItem),0) AS Numeric(17,4)) NetAmount "

            "FROM DAYARCBILL DAB     "
                  " INNER JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
                  " Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME  "
                  "                 FROM DAYARCORDERDISCOUNTS a                                                       "
                  "             group by                                                                              "
                  "                 a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)                                              "
                  " DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY                                                         "
                  " LEFT JOIN DAYARCSURCHARGE ON DAYARCSURCHARGE.ARCBILL_KEY = DAB.ARCBILL_KEY                        "
                  "LEFT JOIN ( "
                            "	 SELECT "
                            "		DAYARCORDERTAXES.ARCHIVE_KEY,                                                                        "
                            "		MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,             "
                            "		MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,   "
                            "		MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge"
                            "  FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                   "
                            "		Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE   "
                            "		FROM DAYARCORDERTAXES a                              "
                            "		group by  a.TAX_TYPE  , a.ARCHIVE_KEY                "
                            "		order by 1 )  DAYARCORDERTAXES                       "
                            "		GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )              "
                            "		AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY              "
                  " LEFT JOIN (                                                                                       "
                  "             SELECT count(*) noOfItem, DA.ARCBILL_KEY                                              "
                  "             FROM (select                                                                          "
                  "                     a.ARCBILL_KEY, a.ITEM_NAME,a.SIZE_NAME                                                    "
                  "                     FROM DAYARCHIVE a                                                             "
                  "                     GROUP BY                                                                      "
                  "                     1,2,3) DA                                                                       "
                  "                     GROUP BY                                                                      "
                  "                     DA.ARCBILL_KEY                                                                "
                  "                     ORDER BY 2)DAC ON DAC.ARCBILL_KEY = DAB.ARCBILL_KEY                           "
                  "    Where DA.ARCHIVE_KEY not in                                                                    "
                  "    (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and "
                  "((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary'  )) "
            " and "
            "DAB.Time_Stamp >= :StartTime and "
            "			DAB.Time_Stamp < :EndTime "

            "GROUP BY "
                    "DAB.BILLED_LOCATION, "

                    " DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL , DAB.ARCBILL_KEY ,DA.BASE_PRICE, "
                    " DA.DISCOUNT,DAC.noOfItem,DAYARCSURCHARGE.SUBTOTAL, DA.PRICE_ADJUST,DA.PRICE , DA.PRICE_LEVEL0, DA.PRICE_LEVEL1,DA.HAPPY_HOUR    "
                    "ORDER BY "
                       "4 "
    ")CombinedQuery2  "
    "WHERE  CombinedQuery2.Time_Stamp >= :StartTime and "
    " CombinedQuery2.Time_Stamp < :EndTime ";


     if (Locations && Locations->Count > 0)
    {
        qrProfiltLoss->SQL->Text	=	qrProfiltLoss->SQL->Text + "and (" +
                                                ParamString(Locations->Count, "CombinedQuery2.Location", "LocationParam") + ")";
    }
    qrProfiltLoss->SQL->Text		=	qrProfiltLoss->SQL->Text +

    "GROUP BY ";
    if (Locations)
    {
        qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
            "CombinedQuery2.LOCATION, ";
    }
    qrProfiltLoss->SQL->Text = qrProfiltLoss->SQL->Text +
  " CombinedQuery2.time_stamp ,      "
  " CombinedQuery2.invoice_number,   "
  " CombinedQuery2.ARCBILL_KEY,      "
  " CombinedQuery2.Qty,              "
  " CombinedQuery2.ITEM_NAME,        "
  " CombinedQuery2.Unit,             "
  " CombinedQuery2.Unit_Price ,      "
  " CombinedQuery2.qty ,             "
  " CombinedQuery2.Tax,              "
  " CombinedQuery2.DISCOUNT,         "
  " CombinedQuery2.SubTotal ,        "
  " CombinedQuery2.Cost,             "
  " CombinedQuery2.TAX_ON_DISCOUNT,   "
  " CombinedQuery2.NetAmount "
  "ORDER BY "
   "4";

    qrProfiltLoss->ParamByName("StartTime")->AsDateTime	= StartTime;
    qrProfiltLoss->ParamByName("EndTime")->AsDateTime		= EndTime;

}
void TdmMMReportData::SetupPointSpend(TDateTime StartTime, TDateTime EndTime, TStrings *Categories, TStrings *payments)
{
	qrPointSpend->Close();
	qrPointSpend->SQL->Text =

 "select innerPart2.CATEGORY_name,                               "
"innerPart2.CATEGORY,                                           "
"ROUND(cast(sum(innerPart2.subtotal) as numeric(17,4)),2)subtotal,       "
"ROUND(cast(sum(innerPart2.point) as numeric(17,4)),2)point,             "
"ROUND(cast(sum(innerPart2.PayTypeTotal) as numeric(17,4)),2)cash        "
"from(                                                          "
"select innerPart1.CATEGORY_name,                               "
"innerPart1.CATEGORY,                                           "
"innerPart1.subtotal,                                           "
"innerPart1.point,                                              "

"innerPart1.time_stamp, "
"cast(sum(innerPart1.PayTypeTotal) as numeric(17,4))PayTypeTotal "
"from (                                                         "
"Select                                                         "
"	CategoryGroups.Name CATEGORY_name,                          "
"	ArcCategories.Category CATEGORY,                            "
"    ARCHIVE.TIME_STAMP,                                        "
"	Cast((cast(Sum(Archive.QTY * Archive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+   "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                 "
"	Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(AC.noOfItem,0))                                                   "
"	) as Numeric(17,4)) subtotal  ,                                                                   "
"	Cast((cast(Sum(Archive.QTY * Archive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+   "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                 "
"	Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(AC.noOfItem,0))                                                  "
"	)*ArcbillpayPoint.PointPersent/100 as Numeric(17,4)) point ,                                      "
"     OtherPayments.PAY_TYPE,                                                                         "
"     Cast((cast(Sum(Archive.QTY * Archive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+ "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                 "
"	Sum( COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(AC.noOfItem,0))                                                 "
"	)*COALESCE(OtherPayments.PayTypeTotal,0)/100 as Numeric(17,4))PayTypeTotal                                    "
"	                                                                                                  "
"	From                                                                                              "
"	Security Left Join Archive on  Security.Security_Ref = Archive.Security_Ref                       "
"	inner join ARCBILL on ARCBILL.ARCBILL_KEY = Archive.ARCBILL_KEY                                   "
"	Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key                      "
"	Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
"	INNER join (SELECT                                                                                "
"	ARCBILLPAY.ARCBILL_KEY,                                                                           "
"	cast((CASE WHEN ARCBILLPAY.PAY_TYPE = 'Points' or ARCBILLPAY.PAY_TYPE = 'Dining'  THEN ((100* COALESCE(sum(ARCBILLPAY.SUBTOTAL),0))/          "
"	Sum(ARCBILL.TOTAL))                                                                                                                           "
"    WHEN ARCBILLPAY.PAY_TYPE != 'Points'  THEN cast(1 as int)                                                                                    "
"     END) as numeric(17, 4)) AS PointPersent                                                                                                     "
"	FROM ARCBILLPAY                                                                                                                               "
"	left join ARCBILL on ARCBILL.ARCBILL_KEY=ARCBILLPAY.ARCBILL_KEY                                                                               "
"	where ARCBILLPAY.PAY_TYPE='Points' or ARCBILLPAY.PAY_TYPE = 'Dining'                                                                          "
"	group by ARCBILLPAY.PAY_TYPE ,ARCBILLPAY.ARCBILL_KEY) ArcbillpayPoint on ArcbillpayPoint.ARCBILL_KEY=Archive.ARCBILL_KEY                      "
"	LEFT JOIN (                                                                                                                                  "
"        SELECT                                                                                                                                   "
"        ARCBILLPAY.ARCBILL_KEY,                                                                                                                  "
"        ARCBILLPAY.PAY_TYPE,                                                                                                                     "
"        cast((CASE WHEN ARCBILLPAY.PAY_TYPE <> 'Points' and  ARCBILLPAY.PAY_TYPE <> 'Dining'  THEN ((100* COALESCE(sum(ARCBILLPAY.SUBTOTAL),0))/ "
"        Sum(ARCBILL.TOTAL))                                                                                                                      "
"        WHEN ARCBILLPAY.PAY_TYPE != 'Points'  THEN cast(1 as int)                                                                                "
"         END) as numeric(17, 4)) AS PayTypeTotal                                                                                                 "
"        FROM ARCBILLPAY                                                                                                                          "
"        left join ARCBILL on ARCBILL.ARCBILL_KEY=ARCBILLPAY.ARCBILL_KEY                                                                          "
"        where ARCBILLPAY.PAY_TYPE <> 'Points' and ARCBILLPAY.PAY_TYPE <> 'Dining' and  ARCBILLPAY.SUBTOTAL > 0 and ARCBILLPAY.CASH_OUT<>'T' ";
          if (payments->Count > 0)
	{
		qrPointSpend->SQL->Text	=	qrPointSpend->SQL->Text + "and (" +
		ParamString(payments->Count, "ARCBILLPAY.PAY_TYPE", "PaymentParam") + ")";
	}
qrPointSpend->SQL->Text=	qrPointSpend->SQL->Text +

"        group by ARCBILLPAY.PAY_TYPE ,ARCBILLPAY.ARCBILL_KEY ";
for (int i=0; i<payments->Count; i++)
	{
		qrPointSpend->ParamByName("PaymentParam" + IntToStr(i))->AsString = payments->Strings[i];
	}
 qrPointSpend->SQL->Text=	qrPointSpend->SQL->Text +

")OtherPayments on OtherPayments.ARCBILL_KEY=Archive.ARCBILL_KEY                     "
"	LEFT JOIN (                                                                                                                                   "
"	SELECT                                                                                                                                        "
"	ARCORDERTAXES.ARCHIVE_KEY,                                                                                                                    "
"	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,                                                            "
"	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,                                                  "
"	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge                                              "
"	FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                                                                                                       "
"	Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE                                                                                            "
"	FROM ARCORDERTAXES a  "
"	group by  a.TAX_TYPE  , a.ARCHIVE_KEY   "
"	order by 1 )  ARCORDERTAXES                                                                           "
"	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )                                                                  "
"	AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY                                                          "
"	Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME      "
"	FROM ARCORDERDISCOUNTS a                                                                              "
"	group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)                                                         "
"	ARCORDERDISCOUNTS on Archive.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY                              "
"   Left join (SELECT  a.ARCBILL_KEY,sum(a.SUBTOTAL) Surcharge_VALUE             "
	"FROM ARCSURCHARGE a                                                          "
	"where a.PAY_TYPE <> 'Points'                                                 "
	"group by a.ARCBILL_KEY)ARCSRCH ON  ARCSRCH.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
    "LEFT JOIN ( "
		"  SELECT count(*) noOfItem, DA.ARCBILL_KEY        "
		"  FROM (select                                    "
		"		   a.ARCBILL_KEY, a.ITEM_NAME,a.SIZE_NAME  "
		"		   FROM ARCHIVE a                       "
		"		   GROUP BY                                "
		"		   1,2,3) DA                               "
		"		   GROUP BY                                "
		"		   DA.ARCBILL_KEY                          "
		"		   ORDER BY 2)AC ON AC.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
"	Where Archive.ARCHIVE_KEY not in (Select     Archive.ARCHIVE_KEY from Archive left join SECURITY on   "
"	SECURITY.SECURITY_REF=Archive.SECURITY_REF where  security.SECURITY_EVENT='CancelY'or security.SECURITY_EVENT='Cancel') and  "
"	(COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and                                                     "
"	COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  and                                                     "
"	Security.Security_Event = 'Ordered By'                                                                                       "
"         Group By                                                                                                               "
"   CategoryGroups.Name,                                                                                                         "
"   ArcCategories.Category ,ArcbillpayPoint.PointPersent,ARCHIVE.TIME_STAMP,OtherPayments.PAY_TYPE,OtherPayments.PayTypeTotal "

"UNION ALL "
"Select                                "
"	CategoryGroups.Name CATEGORY_name, "
"	ArcCategories.Category CATEGORY,   "
"    DayArchive.TIME_STAMP,            "
"	Cast((cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+     "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                         "
"	Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(DAC.noOfItem,0))               "
"	) as Numeric(17,4)) subtotal  ,                                                                           "
"	Cast((cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+     "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                         "
"	Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(DAC.noOfItem,0))              "
"	)*ArcbillpayPoint.PointPersent/100 as Numeric(17,4)) point ,                                              "
"     OtherPayments.PAY_TYPE,                                                                                 "
"     Cast((cast(Sum(DayArchive.QTY * DayArchive.BASE_PRICE)as Numeric(17,4)) +Sum(  COALESCE(AOT.VAT,0) )+   "
"	Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))+                         "
"	Sum( COALESCE(DayArchive.DISCOUNT_WITHOUT_TAX,0)) +sum(coalesce(ARCSRCH.Surcharge_VALUE,0)/coalesce(DAC.noOfItem,0))              "
"	)*COALESCE(OtherPayments.PayTypeTotal,0)/100 as Numeric(17,4))PayTypeTotal                                            "
"	                                                                                                          "
"	From                                                                                                      "
"	Security Left Join DayArchive on  Security.Security_Ref = DayArchive.Security_Ref                         "
"	inner join DAYARCBILL on DAYARCBILL.ARCBILL_KEY = DayArchive.ARCBILL_KEY                                  "
"	Left Join ArcCategories on DayArchive.Category_Key = ArcCategories.Category_Key                           "
"	Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key          "
"	INNER join (SELECT                                                                                        "
"	DAYARCBILLPAY.ARCBILL_KEY,                                                                                 "
"	cast((CASE WHEN DAYARCBILLPAY.PAY_TYPE = 'Points' or DAYARCBILLPAY.PAY_TYPE = 'Dining'  THEN ((100* COALESCE(sum(DAYARCBILLPAY.SUBTOTAL),0))/          "
"	Sum(DAYARCBILL.TOTAL))                                                                                                                                 "
"    WHEN DAYARCBILLPAY.PAY_TYPE != 'Points'  THEN cast(1 as int)                                                                                          "
"     END) as numeric(17, 4)) AS PointPersent                                                                                                              "
"	FROM DAYARCBILLPAY                                                                                                                                     "
"	left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY=DAYARCBILLPAY.ARCBILL_KEY                                                                               "
"	where DAYARCBILLPAY.PAY_TYPE='Points' or DAYARCBILLPAY.PAY_TYPE = 'Dining'                                                                             "
"	group by DAYARCBILLPAY.PAY_TYPE ,DAYARCBILLPAY.ARCBILL_KEY) ArcbillpayPoint on ArcbillpayPoint.ARCBILL_KEY=DayArchive.ARCBILL_KEY                      "
"	LEFT JOIN (                                                                                                                                           "
"        SELECT                                                                                                                                            "
"        DAYARCBILLPAY.ARCBILL_KEY,                                                                                                                        "
"        DAYARCBILLPAY.PAY_TYPE,                                                                                                                           "
"        cast((CASE WHEN DAYARCBILLPAY.PAY_TYPE <> 'Points' and  DAYARCBILLPAY.PAY_TYPE <> 'Dining'  THEN ((100* COALESCE(sum(DAYARCBILLPAY.SUBTOTAL),0))/ "
"        Sum(DAYARCBILL.TOTAL))                                                                                                                            "
"        WHEN DAYARCBILLPAY.PAY_TYPE != 'Points'  THEN cast(1 as int)                                                                                      "
"         END) as numeric(17, 4)) AS PayTypeTotal                                                                                                          "
"        FROM DAYARCBILLPAY                                                                                                                                "
"        left join DAYARCBILL on DAYARCBILL.ARCBILL_KEY=DAYARCBILLPAY.ARCBILL_KEY                                                                          "
"        where DAYARCBILLPAY.PAY_TYPE <> 'Points' and DAYARCBILLPAY.PAY_TYPE <> 'Dining' and  DAYARCBILLPAY.SUBTOTAL > 0 and DAYARCBILLPAY.CASH_OUT<>'T' " ;


          if (payments->Count > 0)
	{
		qrPointSpend->SQL->Text	=	qrPointSpend->SQL->Text + "and (" +
		ParamString(payments->Count, "DAYARCBILLPAY.PAY_TYPE", "PaymentParam") + ")";
	}
qrPointSpend->SQL->Text=	qrPointSpend->SQL->Text +

"        group by DAYARCBILLPAY.PAY_TYPE ,DAYARCBILLPAY.ARCBILL_KEY ";
for (int i=0; i<payments->Count; i++)
	{
		qrPointSpend->ParamByName("PaymentParam" + IntToStr(i))->AsString = payments->Strings[i];
	}
 qrPointSpend->SQL->Text=	qrPointSpend->SQL->Text +

")OtherPayments on OtherPayments.ARCBILL_KEY=DayArchive.ARCBILL_KEY                                                                                        "
"	LEFT JOIN (                                                                                                                                            "
"	SELECT                                                                                                                                                 "
"	DAYARCORDERTAXES.ARCHIVE_KEY,                                                                                                                             "
"	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT,                                                                     "
"	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,                                                           "
"	MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge                                                       "
"	FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                                                                                                                "
"	Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE                                                                                                     "
"	FROM DAYARCORDERTAXES a                                                                                                                                   "
"	group by  a.TAX_TYPE  , a.ARCHIVE_KEY                                                                                                                  "
"	order by 1 )  DAYARCORDERTAXES                                                                                                                            "
"	GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )                                                                                                                  "
"	AOT ON DayArchive.ARCHIVE_KEY = AOT.ARCHIVE_KEY                                                                                                        "
"	Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME                                                       "
"	FROM DAYARCORDERDISCOUNTS a                                                                                                                            "
"	group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)                                                                                                          "
"	DAYARCORDERDISCOUNTS on DayArchive.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY                                                                      "
"   Left join (SELECT  a.ARCBILL_KEY,sum(a.SUBTOTAL) Surcharge_VALUE                                                                                       "
"	FROM DAYARCSURCHARGE a                                                                                                                                 "
"	where a.PAY_TYPE <> 'Points'                                                                                                                           "
"	group by a.ARCBILL_KEY)ARCSRCH ON  ARCSRCH.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY                                                                        "
"LEFT JOIN ( "
		"  SELECT count(*) noOfItem, DA.ARCBILL_KEY        "
		"  FROM (select                                    "
		"		   a.ARCBILL_KEY, a.ITEM_NAME,a.SIZE_NAME  "
		"		   FROM DAYARCHIVE a                       "
		"		   GROUP BY                                "
		"		   1,2,3) DA                               "
		"		   GROUP BY                                "
		"		   DA.ARCBILL_KEY                          "
		"		   ORDER BY 2)DAC ON DAC.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY "
"	Where DayArchive.ARCHIVE_KEY not in (Select     DayArchive.ARCHIVE_KEY from DayArchive left join SECURITY on                                           "
"	SECURITY.SECURITY_REF=DayArchive.SECURITY_REF where  security.SECURITY_EVENT='CancelY'or security.SECURITY_EVENT='Cancel') and                         "
"	(COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and                                                                            "
"	COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')  and                                                                            "
"	Security.Security_Event = 'Ordered By'                                                                                                                 "
"         Group By                                                                                                                                         "
"   CategoryGroups.Name,                                                                                                                                   "
"   ArcCategories.Category ,ArcbillpayPoint.PointPersent,DayArchive.TIME_STAMP,OtherPayments.PAY_TYPE,OtherPayments.PayTypeTotal                           "


")innerPart1 "
"   group by                  "
"   innerPart1.CATEGORY_name, "
"innerPart1.CATEGORY,         "
"innerPart1.subtotal,         "
" innerPart1.time_stamp, "
"innerPart1.point             "
")innerPart2                  "
"where "
"innerPart2.time_stamp >= :StartTime and "
"innerPart2.time_stamp < :EndTime ";


if (Categories->Count > 0)
{
    qrPointSpend->SQL->Text	=	qrPointSpend->SQL->Text + "and (" +
    ParamString(Categories->Count, "innerPart2.Category", "CategoryParam") + ")";
}
qrPointSpend->SQL->Text=	qrPointSpend->SQL->Text +

"group by                     "
"innerPart2.CATEGORY_name,    "
"innerPart2.CATEGORY          ";
 for (int i=0; i<Categories->Count; i++)
{
    qrPointSpend->ParamByName("CategoryParam" + IntToStr(i))->AsString = Categories->Strings[i];
}

qrPointSpend->ParamByName("StartTime")->AsDateTime	= StartTime;
qrPointSpend->ParamByName("EndTime")->AsDateTime	= EndTime;

}


void TdmMMReportData::SetupLoyaltyMembershipAuditItem1(TDateTime StartTime, TDateTime EndTime, TStrings *Names)

{

	qrMembershipAuditPointsBreakdown->Close();
	qrMembershipAuditPointsBreakdown->SQL->Text =
		"select "
				"CONTACTS.NAME as Contacts_name, "
				"Contacts.Mailing_address as Contacts_address, "
                "Contacts.Member_number as Contacts_member_number, "
				"case "
					"when (Contacts_balance is null) "
						"then 0 "
					"else Contacts_balance "
				"end as "
					"Contacts_balance, "
				"Companies.Company_name, "
				"Companies.Location_address as Company_address, "
				"Item_breakdown.Day_of_transaction, "
				"Item_breakdown.Terminal_name, "
				"Item_breakdown.Item_name, "
            "Item_breakdown.Number_purchased, "
				"Item_breakdown.Points_earned, "
				"Item_breakdown.Redeemed "
			"from "
				"( "
					"select "
					  "distinct "
						"Contacts_key, "
						"Day_of_transaction,"
						"Terminal_name, "
						"Item_name, "
						"Number_purchased, "
						"(Number_purchased * Redeemed) as Redeemed, "
						"(Number_purchased * Points_earned) as Points_earned "
					"from "
						"( "
							"select "
								"Pt.Contacts_key, "
								"cast(Pt.Time_stamp as date) "
                                    "as Day_of_transaction, "
								"ArcBill.Terminal_name, "
                                "case "
                                    "when (Adjustment_type = 1) "
                                        "then 'Points purchased' "
                                    "when (Adjustment_type = 3 and DISCOUNT_REASON = 'Loyalty Points Discount') "
                                        "then 'Loyalty Points Discount' "
                                    "else Item_name "
                                "end as "
                                    "Item_name, "
								"Pt.Adjustment_type, "
                                "case "
                                    "when (adjustment_type = 1) "
                                        "then adjustment "
                                    "else points_earned "
                                "end as "
                                    "points_earned, "
                                "case "
                                    "when (Adjustment_type = 1) "
                                        "then 0 "
                                    "when (Adjustment_type = 3 and DISCOUNT_REASON = 'Loyalty Points Discount') "
                                        "then abs(adjustment) "
                                    "else redeemed "
                                "end as "
                                    "Redeemed, "
								"count(*) as Number_purchased "
							"from "
								"( "
                                    "SELECT "
                                        "CONTACTS_KEY, "
                                        "TIME_STAMP, "
                                        "ADJUSTMENT_TYPE, "
                                        "SUM(ADJUSTMENT) AS ADJUSTMENT, "
                                        "EXPORTED_STATUS, "
                                        "TIME_STAMP_EXPORTED, "
                                        "INVOICE_NUMBER "
                                    "FROM "
                                        "POINTSTRANSACTIONS "
                                    "GROUP BY "
                                        "CONTACTS_KEY, "
                                        "TIME_STAMP, "
                                        "ADJUSTMENT_TYPE, "
                                        "EXPORTED_STATUS, "
                                        "TIME_STAMP_EXPORTED, "
                                        "INVOICE_NUMBER "
                                ") as Pt "
							"inner join "
								"ArcBill on "
									"ArcBill.Invoice_number = Pt.Invoice_number "
							"left join "
								"Archive on "
									"Archive.ArcBill_key = ArcBill.ArcBill_key "
					 	"where "

              "Pt.Time_stamp >=:StartTime  and "
                " Pt.Time_stamp <=:EndTime  "
                    "group by "

                    "Pt.Contacts_key, "
                    "Day_of_transaction, "
                    "ArcBill.Terminal_name, "
                    "Archive.Item_name, "
                    "Pt.Adjustment_type, "
                    "Points_earned, "
                    "Redeemed, "
                    "DISCOUNT_REASON"
                ") "
            ") as "
			    "Item_breakdown "
			"inner join "
				"Contacts on "
					"Contacts.Contacts_key = Item_breakdown.Contacts_key "
			"left join "
				"Companies on "
					"Companies.Company_key = Contacts.Company_key "
			"left join "
				"( "
                    "select "
                        "contacts_key, "
                        "sum(adjustment) as contacts_balance "
                    "from "
                        "( "
                            "select "
                            "   distinct "
                            "       contacts_key, "
                            "       time_stamp, "
                            "       sum(adjustment) as adjustment "
                            "from "
                            "   pointstransactions as pt "


                                  "where "
                                              "Pt.Time_stamp >=:StartTime and "
                                              " Pt.Time_stamp <=:EndTime "
                                                 "group by "
                                "contacts_key, "
                                "time_stamp "
                        ") "
                    "group by "
                        "contacts_key "
				") as "
					"Opening_balance on "
						"Opening_balance.Contacts_key = Item_breakdown.Contacts_key "
            "where "
                "(Points_earned <> 0 or "
                "Redeemed <> 0) ";

                 	if (Names && Names->Count > 0)
	{
		qrMembershipAuditPointsBreakdown->SQL->Text	=	qrMembershipAuditPointsBreakdown->SQL->Text + "AND (" +
														ParamString(Names->Count, "CONTACTS.NAME", "NamesParam") + ")";
	}



    	if (Names)
	{
		for (int i=0; i<Names->Count; i++)
		{
			qrMembershipAuditPointsBreakdown->ParamByName("NamesParam" + IntToStr(i))->AsString = Names->Strings[i];
		}
	}

        qrMembershipAuditPointsBreakdown->ParamByName("StartTime")->AsDateTime	= StartTime;
	  qrMembershipAuditPointsBreakdown->ParamByName("EndTime")->AsDateTime	= EndTime;

}
//---------------------------------------------------------------------------
void TdmMMReportData::SetupBreakdownCategory(TStrings *Menus)
{
	 qrBreakdownCategory->Close();
	 qrBreakdownCategory->SQL->Text =
	 	"select "
            "ARCCATEGORIES.CATEGORY, "

            "ITEM.ITEM_NAME, "
            "ITEMSIZE.SIZE_NAME,ITEMSIZE.PRICE "
            "from ARCCATEGORIES right join ITEMSIZECATEGORY on ARCCATEGORIES.CATEGORY_KEY=ITEMSIZECATEGORY.CATEGORY_KEY " 
            "left join ITEMSIZE on ITEMSIZECATEGORY.ITEMSIZE_KEY=ITEMSIZE.ITEMSIZE_KEY "
            "left join ITEM on ITEMSIZE.ITEM_KEY=ITEM.ITEM_KEY  "
            "left join SIZES on ITEMSIZE.SIZES_KEY=SIZES.SIZES_KEY "
            "left join MENU on SIZES.MENU_KEY=MENU.MENU_KEY " ;

         if (Menus->Count > 0)
	 {
		qrBreakdownCategory->SQL->Text	=	qrBreakdownCategory->SQL->Text + "Where (" +
												ParamString(Menus->Count, "Menu.Menu_Name", "MenuParam") + ")";
	 }
	 qrBreakdownCategory->SQL->Text		=	qrBreakdownCategory->SQL->Text +
     

		"group by "
            "ARCCATEGORIES.CATEGORY, "
            "MENU.MENU_NAME, "
            "ITEM.ITEM_NAME, "
            "ITEMSIZE.SIZE_NAME, "
            "ITEMSIZE.PRICE " 
        
        	"order by "
            "upper(ARCCATEGORIES.CATEGORY) asc , "
            "upper(ITEMSIZE.SIZE_NAME) asc " ;

             
           
             for (int i=0; i<Menus->Count; i++)
	{
		qrBreakdownCategory->ParamByName("MenuParam" + IntToStr(i))->AsString = Menus->Strings[i];
	}
}
//-----------------------------------------------------------------------------------------------
void TdmMMReportData::SetupSalesSummaryD(TDateTime StartTime, TDateTime EndTime)
{
    ////select report parameter
    AnsiString name = "'" + nameOfTaxPayer + "'";
    AnsiString address = "'" + addressOfTaxPayer + "'";
    AnsiString tin = frmLogin->CurrentUser.UserID ;
    qrSSDParemeter->Close();
	qrSSDParemeter->SQL->Text =
             "select  "
                     "cast ( 'Name Of Tax Payer' as varchar(30)) Header, "
                     "cast ( '" + nameOfTaxPayer + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
             "UNION ALL "
             "select  "
                      "cast ( 'Address Of Tax Payer' as varchar(30)) Header, "
                     "cast ( '" + addressOfTaxPayer + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
             "UNION ALL "
             "select  "
                      "cast ( 'Tin No.' as varchar(30)) Header, "
                     "cast ( '" + tinNumber + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
            "UNION ALL "
             "select  "
                      "cast ( 'Machine Name' as varchar(30)) Header, "
                     "cast ( '" + dmMMData->GetTerminalName() + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
            "UNION ALL "
             "select  "
                      "cast ( 'Software Name & Version No.' as varchar(30)) Header, "
                     "cast ( 'Menumate POS V6' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
              "UNION ALL "
             "select  "
                      "cast ( 'Serial Number' as varchar(30)) Header, "
                     "cast ( '" + serialNo + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
             "UNION ALL "
             "select  "
                      "cast ( 'User ID' as varchar(30)) Header, "
                     "cast ( '" + frmLogin->CurrentUser.UserID + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data "
             "UNION ALL "
             "select  "
                      "cast ( 'Date & Time Generated' as varchar(30)) Header, "
                     "cast ( '" + Now().FormatString("ddddd 'at' hh:nn") + "' as varchar(50)) Data "
                     "from Arcbill "
                     "GROUP BY Header, Data " ;

     qrSalesSummaryD->Close();
	 qrSalesSummaryD->SQL->Text =

        "Select "
            "ZEDS.Z_KEY Z_Counter, "
            "ZEDS.TIME_STAMP  ,     "
            "MIN(LPAD(BEGINV.INVOICE_NUMBER,11,0)) STARTING_INVOICE, "
            "MIN(LPAD(ENDINV.INVOICE_NUMBER,11,0)) END_INVOICE, "
            "CAST(ZEDS.ZED_TOTAL AS NUMERIC(17,4)) ZED_TOTAL, 								 "
            "CAST(ZEDS.TERMINAL_EARNINGS AS NUMERIC(17,4)) AS NET_BALANCE,                      "
            "CAST(ZEDS.ZED_TOTAL - ZEDS.TERMINAL_EARNINGS AS NUMERIC(17,4)) OPENING_BALANCE, "
            "CAST((COALESCE(DISCOUNT_QUERY.discount ,0)) AS NUMERIC(17,4)) OTHER_DISCOUNTS, "
            "CAST((COALESCE(DISCOUNT_QUERY.PWD ,0)) AS NUMERIC(17,4)) PWD_Discount, "
            "CAST((COALESCE(DISCOUNT_QUERY.SCD ,0)) AS NUMERIC(17,4)) SCD_Discount, "
            "CAST((COALESCE(DISCOUNT_QUERY.SCD ,0)) + (COALESCE(DISCOUNT_QUERY.PWD ,0)) + (COALESCE(DISCOUNT_QUERY.discount ,0)) AS NUMERIC(17,4)) total_discount, "
            "CAST((coalesce(TOTALREFUNDS.Refund_Amount,0)) AS NUMERIC(17,4)) RETURNS_AMOUNT,  "
            "CAST(sum(coalesce(CANCEL_AMOUNT.CANCEL_TOTAL,0)) AS NUMERIC(17,4)) VOID_AMOUNT,     "
            "CAST(ZEDS.TERMINAL_EARNINGS - (COALESCE(DISCOUNT_QUERY.discount ,0)) - (COALESCE(DISCOUNT_QUERY.SCD ,0)) - (COALESCE(DISCOUNT_QUERY.PWD ,0)) AS NUMERIC(17,4)) AS GROSS_AMOUNT, "
          //  "CAST((ZEDS.TERMINAL_EARNINGS  - SUM(COALESCE(ARCORDERDISCOUNTS.DISCOUNTED_VALUE ,0))) -sum(coalesce(TAX_EXEMPT.tax_exempt_SALE,0))  - sum(coalesce(AOT.ServiceCharge,0))  - sum(coalesce(AOT.OtherServiceCharge,0)) "
          //      " - ((sum(coalesce(AOT.VAT,0)) + sum(coalesce(AOT.localTax,0))+ sum(coalesce(AOT.PROFITTAX,0)))) AS NUMERIC(17,4)) VATABLE,  "
            "CAST((ZEDS.TERMINAL_EARNINGS - (ROUNDING.rounding_amount) - sum(coalesce(VAT_EXEMPT_SALE.price,0)) - SUM(coalesce(zero_rated.price,0))- sum(coalesce(AOT.VAT,0))) as NUMERIC(17,4)) VATABLE, "
            "CAST(sum(coalesce(AOT.VAT,0)) AS NUMERIC(17,4)) VAT, "
            "CAST(sum(coalesce(VAT_EXEMPT_SALE.price,0))as NUMERIC(17,4))  VAT_EXEMPT, "
            "CAST(SUM(coalesce(zero_rated.price,0)) AS NUMERIC(17,4)) ZERORATED,       "
            "CAST((ROUNDING.rounding_amount) AS NUMERIC(17,4)) SALES_OVERFLOW,   "
            "CAST('0000' AS VARCHAR(10)) RESET_COUNTER,		"
            "CAST('' AS VARCHAR(50)) REMARKS               "

        "FROM ZEDS "
        "INNER join ARCBILL on ARCBILL.Z_KEY = ZEDS.Z_KEY "
        "left join ARCHIVE on ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
        "LEFT JOIN ( "
                         "SELECT "
                                "ARCORDERTAXES.ARCHIVE_KEY, "
                                "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT                "

                                ///FOR FUTURE
                              /*  "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge, "
                                "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 4 THEN ARCORDERTAXES.TAX_VALUE END) AS localTax,            "
                                "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 5 THEN ARCORDERTAXES.TAX_VALUE END) AS PROFITTAX "   */

                          "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                   "
                                "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE  "
                                "FROM ARCORDERTAXES a                                "
                                "group by  a.TAX_TYPE  , a.ARCHIVE_KEY               "
                                "order by 1 )  ARCORDERTAXES                         "
                                "GROUP BY ARCORDERTAXES.ARCHIVE_KEY )                "
                                "AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY        "
       "LEFT JOIN  (select discountsection.Z_KEY,                         "
                            "    sum(COALESCE(discountsection.SCD,0)) SCD,         "
                            "    sum(COALESCE(discountsection.PWD,0))PWD,          "
                            "    sum(COALESCE(discountsection.discount,0)) DISCOUNT "
                            "from                    "
                            "(SELECT  ARCBILL.Z_KEY, "
                            "        cast(case when a.DISCOUNT_GROUPNAME = 'Senior Citizen' then sum(COALESCE(a.DISCOUNTED_VALUE,0))end as NUMERIC(17,4)) SCD,         "
                            "        cast(case when a.DISCOUNT_GROUPNAME = 'Person with Disability' then sum(COALESCE(a.DISCOUNTED_VALUE,0))end as NUMERIC(17,4)) PWD, "
                            "        cast(case when a.DISCOUNT_GROUPNAME <> 'Person with Disability' AND a.DISCOUNT_GROUPNAME <> 'Senior Citizen' then sum(COALESCE(a.DISCOUNTED_VALUE,0))end as NUMERIC(17,4)) discount "
                            "FROM ARCORDERDISCOUNTS a "
                            "left join ARCHIVE on a.ARCHIVE_KEy = archive.ARCHIVE_KEY        "
                            "left join ARCBILL on ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY  "
                            "GROUP BY ARCBILL.Z_KEY,a.DISCOUNT_GROUPNAME) discountsection GROUP BY discountsection.Z_KEY) DISCOUNT_QUERY on ZEDS.Z_KEY = DISCOUNT_QUERY.Z_KEY "
        "LEFT JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                    "FROM ARCORDERDISCOUNTS a "
                    "group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME) ARCORDERDISCOUNTS on ARCHIVE.ARCHIVE_KEY = ARCORDERDISCOUNTS.ARCHIVE_KEY "
        "LEFT JOIN (SELECT SUM(CAST(COALESCE(ARCBILL.TOTAL,0)  AS NUMERIC(17,4))) Refund_Amount, ARCBILL.Z_KEY FROM ARCBILL "
                    "WHERE ARCBILL.TOTAL < 0 GROUP BY ARCBILL.Z_KEY)TOTALREFUNDS ON TOTALREFUNDS.z_key = zeds.Z_KEY   " 
        "LEFT JOIN( select SUM(CAST(COALESCE(ARCHIVE.PRICE_LEVEL0,0) * ARCHIVE.QTY AS NUMERIC(17,4))) CANCEL_TOTAL, ARCHIVE.ARCBILL_KEY  "
                            "FROM ARCHIVE  "
                            "LEFT JOIN SECURITY ON SECURITY.SECURITY_REF =ARCHIVE.SECURITY_REF  "
                            "LEFT JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = SECURITY.USER_KEY  "
                         "WHERE   "
                                "(SECURITY.SECURITY_EVENT = 'Cancel'  "
                                "OR SECURITY.SECURITY_EVENT = 'CancelY' )   "
                        "GROUP BY ARCHIVE.ARCBILL_KEY)CANCEL_AMOUNT on CANCEL_AMOUNT.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
        "LEFT JOIN ( "
                    "SELECT DA.ARCHIVE_KEY, cast((coalesce(DA.BASE_PRICE,0) * DA.QTY)+ DA.DISCOUNT_WITHOUT_TAX + sum(coalesce(DAOT.TAX_VALUE,0)) as numeric(17,4))price "
                    "FROM ARCHIVE DA  "
                    "LEFT JOIN ARCORDERTAXES DAOT ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                    "WHERE DA.ARCHIVE_KEY  not IN (  "
                        "    SELECT ARCHIVE_KEY          "
                        "    FROM ARCORDERTAXES          "
                        "    WHERE (TAX_TYPE = 0 AND TAX_VALUE <> 0)       "
                        ")  AND (UPPER(DA.DISCOUNT_REASON) NOT LIKE '%DIPLOMAT%')  "
                        " group by da.ARCHIVE_KEY,da.DISCOUNT_WITHOUT_TAX,da.BASE_PRICE,da.QTY) VAT_EXEMPT_SALE ON ARCHIVE.ARCHIVE_KEY = VAT_EXEMPT_SALE.ARCHIVE_KEY "

        "left join (SELECT DA.ARCHIVE_KEY, cast(coalesce(DA.BASE_PRICE,0)*da.QTY  + DA.DISCOUNT_WITHOUT_TAX + coalesce(AOT.TAX_VALUE,0) as numeric(17,4)) price  "
                    "FROM ARCHIVE DA "
                    "LEFT JOIN (SELECT  a.ARCHIVE_KEY, "
                        "Cast(Sum(coalesce(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE "
                        "FROM ARCORDERTAXES a "
                        "group by  a.ARCHIVE_KEY "
                        "order by 1 ) "
                        "AOT ON  AOT.ARCHIVE_KEY = DA.ARCHIVE_KEY  "
                    "WHERE DA.DISCOUNT_REASON <> '' AND DA.DISCOUNT = 0 AND DA.ARCHIVE_KEY  IN ( "
                        "SELECT ARCHIVE_KEY "
                        "FROM ARCORDERTAXES "
                        "WHERE (TAX_TYPE = 0  and TAX_VALUE = 0 ) AND "
                        "ARCORDERTAXES.ARCHIVE_KEY NOT IN (SELECT a.ARCHIVE_KEY FROM ARCORDERDISCOUNTS a WHERE (A.DISCOUNT_GROUPNAME = 'Senior Citizen' AND A.DISCOUNTED_VALUE <> 0) OR (A.DISCOUNT_GROUPNAME = 'Person with Disability'))))zero_rated on ARCHIVE.ARCHIVE_KEY = zero_rated.archive_key "
        "left join(select cast(sum(coalesce(ARCBILLPAY.ROUNDING,0)) as numeric(17,4))rounding_amount, ARCBILL.Z_KEY from ARCBILLPAY inner join ARCBILL on ARCBILLPAY.ARCBILL_KEY = ARCBILL.ARCBILL_KEY "
                    "group by ARCBILL.Z_KEY)ROUNDING on ZEDS.Z_KEY = rounding.z_key "
        "LEFT JOIN( SELECT ARCBILL.INVOICE_NUMBER, ARCBILL.Z_KEY FROM ARCBILL where ARCBILL.TIME_STAMP IN(SELECT    MIN(ARCBILL.TIME_STAMP) min_ARCBILL_KEY  FROM ARCBILL GROUP BY ARCBILL.Z_KEY) ) BEGINV ON BEGINV.Z_KEY = ARCBILL.Z_KEY "
        "LEFT JOIN( SELECT ARCBILL.INVOICE_NUMBER, ARCBILL.Z_KEY FROM ARCBILL where ARCBILL.TIME_STAMP IN(SELECT    MAX(ARCBILL.TIME_STAMP) min_ARCBILL_KEY  FROM ARCBILL GROUP BY ARCBILL.Z_KEY) ) ENDINV ON ENDINV.Z_KEY = ARCBILL.Z_KEY "
        "Where "
                        "((   "
                     "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and "
                     "COALESCE(ARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary')) "
                     " and ZEDS.Time_Stamp >= :StartTime and ZEDS.Time_Stamp < :EndTime "
        "GROUP BY "
            "ZEDS.Z_KEY, ZEDS.TIME_STAMP, "
            "ZEDS.ZED_TOTAL, ZEDS.TERMINAL_EARNINGS , "
            "TOTALREFUNDS.Refund_Amount, "
            "ROUNDING.rounding_amount, "
            "DISCOUNT_QUERY.SCD, "
            "DISCOUNT_QUERY.PWD, "
            "DISCOUNT_QUERY.discount ";


    qrSalesSummaryD->ParamByName("StartTime")->AsDateTime	= StartTime;
    qrSalesSummaryD->ParamByName("EndTime")->AsDateTime	= EndTime;

}

//---------------------------------------------------------------------------
void TdmMMReportData::SetupEJournal(TDateTime StartTime, TDateTime EndTime)
{
   qrEJournal->Close();
   qrEJournal->SQL->Text =
      "SELECT "
         "DAB.ARCBILL_KEY, "
         "DAB.time_stamp datetime, "
         "DAB.RECEIPT receipt, "
         "DAB.INVOICE_NUMBER "
         "From "
			"DAYARCBILL DAB "
      "WHERE "
         "DAB.Time_Stamp >= :StartTime and "
         "DAB.Time_Stamp < :EndTime "

        " UNION ALL "

      "SELECT "
         "AB.ARCBILL_KEY, "
         "AB.time_stamp datetime, "
         "AB.RECEIPT receipt, "
         "AB.INVOICE_NUMBER " 
         "From "
			"ARCBILL AB "
      "WHERE "
         "AB.Time_Stamp >= :StartTime and "
         "AB.Time_Stamp < :EndTime "

         "order by 1 ";
   qrEJournal->ParamByName("StartTime")->AsDateTime	= StartTime;
   qrEJournal->ParamByName("EndTime")->AsDateTime	= EndTime;
}
















































