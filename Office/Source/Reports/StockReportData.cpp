//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StockReportData.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RpCon"
#pragma link "RpConDS"
#pragma link "RpDefine"
#pragma link "RpCon"
#pragma link "RpConDS"
#pragma link "RpDefine"
#pragma resource "*.dfm"
TdmStockReportData *dmStockReportData;
//---------------------------------------------------------------------------
__fastcall TdmStockReportData::TdmStockReportData(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString TdmStockReportData::ParamString(int Count, AnsiString Field, AnsiString ParamName, AnsiString Operator)
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
void TdmStockReportData::SetupStockValueByGroup(TStrings *Groups)
{
	qrStockValue->Close();
	qrStockValue->SQL->Text =
		"Select "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"cast(StockLocation.On_Hand * StockLocation.Average_Cost as float) Average_Cost, "
			"cast(StockLocation.On_Hand * StockLocation.Latest_Cost as float) Latest_Cost, "
			"StockLocation.Average_Cost Single_Average_Cost, "
			"StockLocation.Latest_Cost  Single_Latest_Cost "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Groups->Count > 0)
	{
		qrStockValue->SQL->Text	=	qrStockValue->SQL->Text + "And (" +
											ParamString(Groups->Count, "StockGroup.Stock_Group", "Param") + ")";
	}
	qrStockValue->SQL->Text 		=	qrStockValue->SQL->Text +
		"Order By "
			"StockCategory.Sort_Order, "
			"StockGroup.Sort_Order, "
			"StockLocation.Location";
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockValue->ParamByName("Param" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockValueByLocation(TStrings *Locations, TStrings *Groups)
{
	qrStockValue->Close();
	qrStockValue->SQL->Text =
		"Select "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"cast(StockLocation.On_Hand * StockLocation.Average_Cost as float) Average_Cost, "
			"cast(StockLocation.On_Hand * StockLocation.Latest_Cost as float)  Latest_Cost, "
			"StockLocation.Average_Cost Single_Average_Cost, "
			"StockLocation.Latest_Cost  Single_Latest_Cost,"
			"cast('StockValuebyLocation' as varchar(25)) Name,"
			"cast('' as varchar(25)) Committed_User_Name,"
			"cast('' as varchar(25)) Initialised_User_Name,"
			"cast(current_timestamp as Timestamp) Committed_Time,"
			"cast(current_timestamp as Timestamp) Initialised_Time "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Locations->Count > 0)
	{
		qrStockValue->SQL->Text	=	qrStockValue->SQL->Text + "And (" +
											ParamString(Locations->Count, "StockLocation.Location", "LocParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStockValue->SQL->Text	=	qrStockValue->SQL->Text + "And (" +
											ParamString(Groups->Count, "StockGroup.Stock_Group", "GroupParam") + ")";
	}
	qrStockValue->SQL->Text		=	qrStockValue->SQL->Text +
		"Order By "
			"12,16,"
			"StockLocation.Location, "
			"StockCategory.Sort_Order, "
			"StockGroup.Sort_Order ";
	for (int i=0; i<Locations->Count; i++)
	{
		qrStockValue->ParamByName("LocParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockValue->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}
//---------------------------------------------------------------------------
/*void TdmStockReportData::SetupStockDetailsByGroup(TStrings *Groups)
{
	qrStock->Close();
	qrStock->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order,"
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Inner Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join PurchaseStock On "
				"PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
		"Where "
			"Stock.Enabled = 'T' ";
	if (Groups->Count > 0)
	{
		qrStock->SQL->Text	=	qrStock->SQL->Text + "And (" +
										ParamString(Groups->Count, "StockGroup.Stock_Group", "Param") + ")";
	}
	qrStock->SQL->Text 		=	qrStock->SQL->Text +
		"Group By "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level "
		"Order By "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order, "
			"Stock.Description,"
			"StockLocation.Location";
	for (int i=0; i<Groups->Count; i++)
	{
		qrStock->ParamByName("Param" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockDetailsByLocation(TStrings *Locations, TStrings *Groups)
{
	qrStock->Close();
	qrStock->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order,"
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Inner Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join PurchaseStock On "
				"PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
		"Where "
			"Stock.Enabled = 'T' ";
	if (Locations->Count > 0)
	{
		qrStock->SQL->Text	=	qrStock->SQL->Text + "And (" +
										ParamString(Locations->Count, "StockLocation.Location", "LocParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStock->SQL->Text	=	qrStock->SQL->Text + "And (" +
										ParamString(Groups->Count, "StockGroup.Stock_Group", "GroupParam") + ")";
	}
	qrStock->SQL->Text 		=	qrStock->SQL->Text +
		"Group By "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level "
		"Order By "
			"StockLocation.Location, "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order, "
			"Stock.Description";
	for (int i=0; i<Locations->Count; i++)
	{
		qrStock->ParamByName("LocParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrStock->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}*/
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockBarcodes(TStrings *Groups)
{
	qrStockBarcodes->Close();
	qrStockBarcodes->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockCategory.Sort_Order,"
			"StockGroup.Stock_Group,"
			"StockGroup.Sort_Order,"
			"Stock.Code,"
			"Stock.Stocktake_Unit,"
			"Stock.Description,"
			"Barcode.Barcode,"
			"Cast (Null as VarChar(25)) Supplier_Unit,"
			"Cast (Null as VarChar(50)) Company_Name "
		"From "
			"Stock Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join Barcode on "
				"Stock.Stock_Key = Barcode.Stock_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Groups->Count > 0)
	{
		qrStockBarcodes->SQL->Text	=	qrStockBarcodes->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockGroup.Stock_Group", "Param") + ")";
	}
	qrStockBarcodes->SQL->Text 		=	qrStockBarcodes->SQL->Text +

		"Union All "

		"Select "
			"StockCategory.Stock_Category,"
			"StockCategory.Sort_Order,"
			"StockGroup.Stock_Group,"
			"StockGroup.Sort_Order,"
			"Stock.Code,"
			"Stock.Stocktake_Unit,"
			"Stock.Description,"
			"Barcode.Barcode,"
			"SupplierStock.Supplier_Unit,"
			"Contact.Company_Name "
		"From "
			"Stock Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join SupplierStock on "
				"Stock.Stock_Key = SupplierStock.Stock_Key "
			"Left Join Barcode on "
				"Barcode.Supplier_Stock_Key = SupplierStock.Supplier_Stock_Key "
			"Left Join Contact on "
				"Contact.Contact_LK = SupplierStock.Supplier_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"Barcode.Barcode Is Not Null ";
	if (Groups->Count > 0)
	{
		qrStockBarcodes->SQL->Text	=	qrStockBarcodes->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockGroup.Stock_Group", "Param") + ")";
	}
	qrStockBarcodes->SQL->Text 		=	qrStockBarcodes->SQL->Text +
		"Order By "
			"2,"
            "3,"
			"4,"
			"6";

	/*qrStockBarcodes->Close();
	qrStockBarcodes->SQL->Text =             
		"Select "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,"
			"Stock.Stocktake_Unit,"
			"Stock.Description,"
			"Barcode.Barcode,"
			"SupplierStock.Supplier_Unit,"
			"Contact.Company_Name "
		"From "
			"Stock Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join Barcode on "
				"Stock.Stock_Key = Barcode.Stock_Key "
			"Left Join SupplierStock on "
				"Barcode.Supplier_Stock_Key = SupplierStock.Supplier_Stock_Key "
			"Left Join Contact on "
				"Contact.Contact_LK = SupplierStock.Supplier_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Groups->Count > 0)
	{
		qrStockBarcodes->SQL->Text	=	qrStockBarcodes->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockGroup.Stock_Group", "Param") + ")";
	}
	qrStockBarcodes->SQL->Text 		=	qrStockBarcodes->SQL->Text +
		"Order By "
			"StockCategory.Sort_Order, "
			"StockGroup.Sort_Order, "
			"Stock.Description";*/
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockBarcodes->ParamByName("Param" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockDetails(TStrings *Locations, TStrings *Groups)
{
	qrStock->Close();
	qrStock->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order,"
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Inner Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Left Join PurchaseStock On "
				"PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) ";
	if (Locations && Locations->Count > 0)
	{
		qrStock->SQL->Text	=	qrStock->SQL->Text + "And (" +
										ParamString(Locations->Count, "StockLocation.Location", "LocParam") + ")";
	}
	if (Groups && Groups->Count > 0)
	{
		qrStock->SQL->Text	=	qrStock->SQL->Text + "And (" +
										ParamString(Groups->Count, "StockGroup.Stock_Group", "GroupParam") + ")";
	}
	qrStock->SQL->Text 		=	qrStock->SQL->Text +
		"Group By "
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"StockLocation.Average_Cost, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Min_Level, "
			"StockLocation.Max_Level, "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order "
		"Order By "
			"StockLocation.Location, "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order ";
	if (Locations)
	{
		for (int i=0; i<Locations->Count; i++)
		{
			qrStock->ParamByName("LocParam" + IntToStr(i))->AsString = Locations->Strings[i];
		}
	}
	if (Groups)
	{
		for (int i=0; i<Groups->Count; i++)
		{
			qrStock->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
		}
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockMovement(TDateTime StartTime, TDateTime EndTime, AnsiString Transaction,
	TStrings *Locations, TStrings *Groups)
{
	qrStockMovement->Close();
	qrStockMovement->SQL->Text =

        "Select "
            "Location, "
            "Stock_Category, "
            "Stock_Group, "
            "Description, "
            "Note, "
            "cast(Sum(Cost * Qty) as float) Total_Cost "
   		"From "
		"( "

		"Select "
			"StockTrans.Location Location,"
			"StockTrans.Stock_Category Stock_Category,"
			"StockTrans.Stock_Group Stock_Group,"
            "StockTrans.Description Description,"
            "StockTrans.Note Note, "
            "StockTrans.Unit_Cost Cost, "
            "case "
            "when(StockTrans.TRANSACTION_TYPE ='Manufacture') "
            "then  abs(StockTrans.Qty) "
            "else "
            "StockTrans.Qty end Qty "
		"From "
			"StockTrans "
		"Where "
			"StockTrans.Created >= :StartTime and "
			"StockTrans.Created < :EndTime ";
	if (Transaction != "")
	{
		qrStockMovement->SQL->Text =	qrStockMovement->SQL->Text + "And StockTrans.Transaction_Type = :Transaction ";
	}
	if (Locations->Count)
	{
		qrStockMovement->SQL->Text =	qrStockMovement->SQL->Text + "And (" +
												ParamString(Locations->Count, "StockTrans.Location", "LocationParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStockMovement->SQL->Text =	qrStockMovement->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockTrans.Stock_Group", "GroupParam") + ")";
	}
	qrStockMovement->SQL->Text =	qrStockMovement->SQL->Text +

    ") "
		"Group By "
			"Location,"
			"Stock_Category,"
			"Stock_Group,"
            "Description,"
            "Note "
		"Order By "
			"Location,"
			"Stock_Category,"
			"Stock_Group";
	for (int i=0; i<Locations->Count; i++)
	{
		qrStockMovement->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockMovement->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
	qrStockMovement->ParamByName("StartTime")->AsDateTime		= StartTime;
	qrStockMovement->ParamByName("EndTime")->AsDateTime		= EndTime;
	if (Transaction != "")
	{
		qrStockMovement->ParamByName("Transaction")->AsString	= Transaction;
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockTransfer(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups, int TransferNumber)
{
	qrStockTransferFrom->Close();
	qrStockTransferFrom->SQL->Text =
		"Select "
			"StockTrans.Location From_Location,"
			"StockTrans.Purchaser_Name To_Location,"
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
                        "TransactionBatch.REFERENCE,"
			"StockTrans.Unit,"
			"-cast(Sum(StockTrans.Qty) as numeric(15, 4)) Qty,"
			"-Sum(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost, "
            "StockTrans.Unit_Cost, "
			"STOCKTRANS.TRANSFER_ID, "
            "StockTrans.Created, "
            "STOCKREQUEST_STOCK.REQUEST_NUMBER "
		"From "
			"StockTrans  left join TransactionBatch on StockTrans.BATCH_KEY = TransactionBatch.BATCH_KEY  "
            "left join     STOCKREQUEST_STOCK on STOCKTRANS.STOCK_REQUEST_STOCK_KEY_VAL = STOCKREQUEST_STOCK.STOCK_REQUEST_STOCKKEY "
		"Where "
			"StockTrans.Created >= :StartTime and "
			"StockTrans.Created <= :EndTime and "
		 "StockTrans.Transaction_Type = 'Transfer'  and "
          "StockTrans.Qty < 0  ";
         if(TransferNumber > 0)
         {
            	qrStockTransferFrom->SQL->Text	=	qrStockTransferFrom->SQL->Text + " And STOCKTRANS.TRANSFER_ID = :TransferNumber ";
         }



    if( Locations != NULL)
    {
        if (Locations->Count)
        {
            qrStockTransferFrom->SQL->Text	=	qrStockTransferFrom->SQL->Text + "And (" +
                                                            ParamString(Locations->Count, "StockTrans.Location", "LocationParam") + ")";
        }
    }
    if(Groups != NULL)
    {
        if (Groups->Count > 0)
        {
            qrStockTransferFrom->SQL->Text	=	qrStockTransferFrom->SQL->Text + "And (" +
                                                            ParamString(Groups->Count, "StockTrans.Stock_Group", "GroupParam") + ")";
        }
    }
	qrStockTransferFrom->SQL->Text		=	qrStockTransferFrom->SQL->Text +
		"Group By "
			"StockTrans.Location,"
			"StockTrans.Purchaser_Name,"
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
            "TransactionBatch.REFERENCE,"
			"StockTrans.Unit, "
            "STOCKTRANS.TRANSFER_ID, "
            "StockTrans.Created, "
            "STOCKREQUEST_STOCK.REQUEST_NUMBER, "
             "StockTrans.Unit_Cost "
		"Order By "
            "STOCKREQUEST_STOCK.REQUEST_NUMBER, "
            "QTY desc, "
            "StockTrans.Location,"
			"StockTrans.Purchaser_Name,"
			"StockTrans.Stock_Category,"
     		"StockTrans.Stock_Group,"
			"StockTrans.Description";
	qrStockTransferFrom->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrStockTransferFrom->ParamByName("EndTime")->AsDateTime		= EndTime;
    
    if(TransferNumber > 0)
    {
        qrStockTransferFrom->ParamByName("TransferNumber")->AsInteger		= TransferNumber;
    }

    if(Locations != NULL)
    {
	for (int i=0; i<Locations->Count; i++)
	{
		qrStockTransferFrom->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
    }
    if(Groups != NULL)
    {
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockTransferFrom->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
    }
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockBatchTransfer(TDateTime StartTime, TDateTime EndTime)
{
	qrStockTransferFrom->Close();
	qrStockTransferFrom->SQL->Text =
		"Select "
			"TransactionBatch.Created,"
			"TransactionBatch.Reference,"
			"StockTrans.Location From_Location,"
			"StockTrans.Purchaser_Name To_Location,"
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
			"StockTrans.Unit,"
			"-StockTrans.Qty Qty,"
			"-(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost "
		"From "
			"TransactionBatch Left Join StockTrans On StockTrans.Batch_Key = TransactionBatch.Batch_Key "
		"Where "
			"TransactionBatch.Created >= :StartTime and "
			"TransactionBatch.Created < :EndTime and "
			"TransactionBatch.Transaction_Type = 'Transfer' and "
            "StockTrans.Qty <> 0  ";
		"Order By "
			"StockTrans.Location,"
			"TransactionBatch.Created,"
			"TransactionBatch.Reference, "
			"StockTrans.Purchaser_Name,"
			"StockTrans.Description";
	qrStockTransferFrom->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrStockTransferFrom->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockInquiry (TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups, TStrings *Transactions)
{
	qrStockInquiry->Close();
	qrStockInquiry->SQL->Text =
		"Select "
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
			"StockTrans.Created,"
			"StockTrans.Transaction_Type,"
			"StockTrans.Location,"
			"StockTrans.Unit,"
            "case "
            "when(StockTrans.Transaction_Type = 'Manufacture') "
            "then abs(StockTrans.Qty) "
            "else "
            "StockTrans.Qty end QTY, "
            "case "
            "when(StockTrans.Transaction_Type = 'Manufacture') "
            "then cast(StockTrans.Unit_Cost * abs(StockTrans.Qty) as float) "
            "else "
            "cast(StockTrans.Unit_Cost * StockTrans.Qty as float) end Total_Cost, "
			"StockTrans.User_Name,"
			"StockTrans.Supplier_Name,"
			"StockTrans.Reference,"
			"StockTrans.Note,"
			"Extract (Year From StockTrans.Created) Trans_Year,"
			"Extract (Month From StockTrans.Created) Trans_Month,"
			"Extract (Day From StockTrans.Created) Trans_Day "
		"From "
			"StockTrans "
		"Where "
			"Created >= :StartTime and "
			"Created < :EndTime ";
	if (Locations->Count)
	{
		qrStockInquiry->SQL->Text	=	qrStockInquiry->SQL->Text + "And (" +
												ParamString(Locations->Count, "StockTrans.Location", "LocationParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStockInquiry->SQL->Text	=	qrStockInquiry->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockTrans.Stock_Group", "GroupParam") + ")";
	}
	if (Transactions->Count)
	{
		qrStockInquiry->SQL->Text	=	qrStockInquiry->SQL->Text + "And (" +
												ParamString(Transactions->Count, "StockTrans.Transaction_Type", "TransactionParam") + ")";
	}
	qrStockInquiry->SQL->Text		=	qrStockInquiry->SQL->Text +
		"Order By "
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
			"StockTrans.Created,"
			"StockTrans.Transaction_Type";
	qrStockInquiry->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrStockInquiry->ParamByName("EndTime")->AsDateTime		= EndTime;
	for (int i=0; i<Locations->Count; i++)
	{
		qrStockInquiry->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockInquiry->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
	for (int i=0; i<Transactions->Count; i++)
	{
		qrStockInquiry->ParamByName("TransactionParam" + IntToStr(i))->AsString = Transactions->Strings[i];
	}

    int q = 0;

    qrStockInquiry->Open();
    for ( ; !qrStockInquiry->Eof; qrStockInquiry->Next(), q++)
        ;

    StartTime = EndTime;
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupWriteOffLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations)
{
    qrStockWriteOff->Close();
    qrStockWriteOff->SQL->Text =
        "Select "
			"StockTrans.Stock_Category,"
			"StockTrans.Stock_Group,"
			"StockTrans.Description,"
			"StockTrans.Created,"
			"StockTrans.Transaction_Type,"
			"StockTrans.Location,"
			"StockTrans.Unit,"
			"StockTrans.Qty,"
			"StockTrans.User_Name,"
			"StockTrans.Supplier_Name,"
			"StockTrans.Reference,"
			"StockTrans.Note,"
            "StockTrans.Total_Cost,"
			"Extract (Year From StockTrans.Created) Trans_Year,"
			"Extract (Month From StockTrans.Created) Trans_Month,"
			"Extract (Day From StockTrans.Created) Trans_Day "
		"From "
			"StockTrans "
 		"Where "
 			"StockTrans.Created >= :StartTime and "
 			"StockTrans.Created < :EndTime and "
            "(StockTrans.Transaction_Type = 'Writeoff'OR StockTrans.Transaction_Type = 'WriteOff') ";
	if (Locations->Count)
	{
		qrStockWriteOff->SQL->Text	=	qrStockWriteOff->SQL->Text + "And (" +
												ParamString(Locations->Count, "StockTrans.Location", "LocationParam") + ")";
	}
	qrStockWriteOff->SQL->Text		=	qrStockWriteOff->SQL->Text +
		"Order By "
			"StockTrans.Location,"
			"StockTrans.Created";
 	qrStockWriteOff->ParamByName("StartTime")->AsDateTime	= StartTime;
 	qrStockWriteOff->ParamByName("EndTime")->AsDateTime		= EndTime;
 	for (int i=0; i<Locations->Count; i++)
	{
		qrStockWriteOff->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupManufactureRecipe(TDateTime StartTime, TDateTime EndTime)
{
    qrStockManufacture->Close();
    qrStockManufacture->SQL->Text =
        "Select "
			"StockTrans.Description,"
			"StockTrans.Created,"
			"StockTrans.Location,"
			"abs(StockTrans.Qty) Qty, "
            "StockTrans.Total_Cost,"
            "StockTrans.Unit_Cost,"
            "StockTrans.Unit,"
            "StockTrans.Note "
		"From "
			"StockTrans "
 		"Where "
 			"StockTrans.Created >= :StartTime and "
 			"StockTrans.Created < :EndTime and "
            "StockTrans.Transaction_Type = 'Manufacture' "
        "Order By "
            "Note, "
            "StockTrans.Created,"
            "Qty DESC;";
 	qrStockManufacture->ParamByName("StartTime")->AsDateTime	= StartTime;
 	qrStockManufacture->ParamByName("EndTime")->AsDateTime		= EndTime;
}




//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockReorderItem(TStrings *Locations, TStrings *Groups)
{
	qrStockReorder->Close();
	qrStockReorder->SQL->Text =
		"Select "
			"StockLocation.Location, "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Description, "
			"Stock.Stocktake_Unit, "
			"StockLocation.Latest_Cost, "
			"StockLocation.Max_Level, "
			"StockLocation.Min_Level, "
			"StockLocation.On_Hand, "
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order "
		"From "
			"StockCategory Inner Join StockGroup On "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"Inner Join Stock On "
				"StockGroup.Stock_Group_Key = Stock.Stock_Group_Key "
			"Inner Join StockLocation On "
				"StockLocation.Stock_Key = Stock.Stock_Key "
			"Left Join PurchaseStock On "
				"(PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location) "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"(StockLocation.Max_Level <> 0 or StockLocation.Min_Level <> 0) ";//and "
//			"StockLocation.On_Order + StockLocation.On_Hand <= StockLocation.Min_Level ";
	if (Locations->Count)
	{
		qrStockReorder->SQL->Text	=	qrStockReorder->SQL->Text + "And (" +
												ParamString(Locations->Count, "StockLocation.Location", "LocationParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStockReorder->SQL->Text	=	qrStockReorder->SQL->Text + "And (" +
												ParamString(Groups->Count, "StockGroup.Stock_Group", "GroupParam") + ")";
	}
	qrStockReorder->SQL->Text 		=	qrStockReorder->SQL->Text +
		"Group By "
			"StockLocation.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,"
			"Stock.Description,"
			"Stock.Stocktake_Unit,"
			"StockLocation.Latest_Cost,"
			"StockLocation.Max_Level,"
			"StockLocation.Min_Level,"
			"StockLocation.On_Hand, "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order "

		"Having "
			"Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) + StockLocation.On_Hand <= StockLocation.Max_Level Or "
			"(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) Is Null And StockLocation.On_Hand <= StockLocation.Min_Level) "
		"Order By "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order,"
			"StockLocation.Location";
	for (int i=0; i<Locations->Count; i++)
	{
		qrStockReorder->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
	for (int i=0; i<Groups->Count; i++)
	{
		qrStockReorder->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
	}
}
//---------------------------------------------------------------------------
/*void TdmStockReportData::SetupStocktakeHistory(TDateTime StartTime, TDateTime EndTime, TStrings *StocktakeFilter)
{
	qrStocktakeHistory->Close();
	qrStocktakeHistory->SQL->Text =
		"Select "
			"Stocktake.Initialised_Time Created,"
			"Stocktake.Location,"
			"Stocktake.Initialised_User_Name User_Name,"
			"StocktakeHistory.Stock_Category,"
			"StocktakeHistory.Stock_Group,"
			"StocktakeHistory.Code,"
			"StocktakeHistory.Description,"
			"StocktakeHistory.Stocktake_Unit,"
			"StocktakeHistory.Average_Unit_Cost,"
			"StocktakeHistory.Latest_Unit_Cost,"
			"StocktakeHistory.Opening,"
			"StocktakeHistory.Inwards,"
			"StocktakeHistory.Writeoff,"
			"StocktakeHistory.Transfer,"
			"StocktakeHistory.Sales,"
			"StocktakeHistory.Closing - (StocktakeHistory.Sales + StocktakeHistory.Transfer + StocktakeHistory.Writeoff + StocktakeHistory.Inwards + StocktakeHistory.Opening) Stocktake,"
			"StocktakeHistory.Closing "
		"From "
			"Stocktake Left Join StocktakeHistory On Stocktake.Stocktake_Key = StocktakeHistory.Stocktake_Key "
		"Where ";
	if (StocktakeFilter->Count)
	{
		qrStocktakeHistory->SQL->Text	=	qrStocktakeHistory->SQL->Text + "(" +
													ParamString(StocktakeFilter->Count, "Stocktake.Stocktake_Key", "StocktakeKeyParam") + ")";
	}
	else
	{
		qrStocktakeHistory->SQL->Text	=	qrStocktakeHistory->SQL->Text +
			"Initialised_Time >= :StartTime and "
			"Initialised_Time < :EndTime ";
	}
	qrStocktakeHistory->SQL->Text		=	qrStocktakeHistory->SQL->Text +
		"Order By "
			"Stocktake.Initialised_Time,"
			"Stocktake.Location,"
			"Stocktake.Initialised_User_Name,"
			"StocktakeHistory.Stock_Category,"
			"StocktakeHistory.Stock_Group,"
			"StocktakeHistory.Description";

	if (StocktakeFilter->Count)
	{
		for (int i=0; i<StocktakeFilter->Count; i++)
		{
			qrStocktakeHistory->ParamByName("StocktakeKeyParam" + IntToStr(i))->AsInteger = StrToInt(StocktakeFilter->Strings[i]);
		}
	}
	else
	{
		qrStocktakeHistory->ParamByName("StartTime")->AsDateTime	= StartTime;
		qrStocktakeHistory->ParamByName("EndTime")->AsDateTime	= EndTime;
	}
} */
//---------------------------------------------------------------------------
void TdmStockReportData::SetupSupplierPurchases(TDateTime StartTime, TDateTime EndTime, TStrings *StockReceipts, TStrings *Invoices , int byval)
{
	qrStockReceiptByCat->Close();
	qrStockReceiptByCat->SQL->Text =
		"Select "
        "StockTrans.BATCH_KEY, "
//			"StockCategory.Stock_Category, "
//			"StockGroup.Stock_Group, "
			"StockTrans.Stock_Category, "
			"StockTrans.Stock_Group, "
			"StockTrans.Code, "
			"StockTrans.Description, "
			"StockTrans.Location, "
			"StockTrans.Created, "
			"StockTrans.Supplier_Name, "
			"StockTrans.Reference, "
			"StockTrans.ORDER_QTY as Qty, "
			"StockTrans.Order_Unit as Unit, "
			"StockTrans.Total_Cost, "
			//"StockTrans.Unit_Cost * StockTrans.Qty Total_Cost, "
			"StockTrans.Total_Cost + StockTrans.Total_GST as Total_Cost_Inc "
		  //	"Cast((Cast(StockTrans.Unit_Cost As Double Precision) * StockTrans.Qty) * (StockTrans.GST_Percent + 100) / 100 as Numeric(15, 4)) Total_Cost_Inc "
		"From "
			"StockTrans "
/*			"StockTrans Left Join Stock on "
				"StockTrans.Code = Stock.Code "
			"Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "*/
		"Where "
			"StockTrans.Created >= :StartTime and "
			"StockTrans.Created < :EndTime and "
			"StockTrans.Transaction_Type = 'Receipt' ";
	if (StockReceipts->Count)
	{
		qrStockReceiptByCat->SQL->Text	=	qrStockReceiptByCat->SQL->Text + "And (" +
														ParamString(StockReceipts->Count, "StockTrans.BATCH_KEY", "StockReceiptsParam") + ")";
	}
	if (Invoices->Count > 0)
	{
		qrStockReceiptByCat->SQL->Text	=	qrStockReceiptByCat->SQL->Text + "And (" +
														ParamString(Invoices->Count, "StockTrans.Reference", "InvoiceParam") + ")";
	}
	qrStockReceiptByCat->SQL->Text 		=	qrStockReceiptByCat->SQL->Text +
		"Order By "
            "StockTrans.Supplier_Name, "  ;

            	if (!byval)
	{

    qrStockReceiptByCat->SQL->Text = qrStockReceiptByCat->SQL->Text + "StockTrans.Reference , ";

	}
    else{
     qrStockReceiptByCat->SQL->Text = qrStockReceiptByCat->SQL->Text + "StockTrans.BATCH_KEY , ";

}
       	qrStockReceiptByCat->SQL->Text 		=	qrStockReceiptByCat->SQL->Text +


			"StockTrans.Created, "
			"StockTrans.Reference, "
			"StockTrans.Stock_Category, "
//			"StockCategory.Sort_Order, "
//			"StockGroup.Sort_Order, "
			"StockTrans.Description ";
	for (int i=0; i<StockReceipts->Count; i++)
	{
		qrStockReceiptByCat->ParamByName("StockReceiptsParam" + IntToStr(i))->AsString = StockReceipts->Strings[i];
	}
	for (int i=0; i<Invoices->Count; i++)
	{
		qrStockReceiptByCat->ParamByName("InvoiceParam" + IntToStr(i))->AsString = Invoices->Strings[i];
	}
	qrStockReceiptByCat->ParamByName("StartTime")->AsDateTime	= StartTime;
	qrStockReceiptByCat->ParamByName("EndTime")->AsDateTime		= EndTime;
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupSupplierDetails(TStrings *Suppliers)
{
	qrContactDetails->Close();
	qrContactDetails->SQL->Text =
		"Select "
			"Contact.Company_Name, "
			"Contact.Phone, "
			"Contact.Fax, "
			"Contact.Mobile, "
			"Contact.Email, "
			"Contact.Location_Address, "
			"Contact.Contact_Name, "
			"Contact.Contact_Phone, "
			"Contact.Bill_Name, "
			"Contact.Bill_Attn, "
			"Contact.Bill_Phone, "
			"Contact.Bill_Address "
		"From "
			"Contact ";
	if (Suppliers->Count)
	{
		qrContactDetails->SQL->Text	=	qrContactDetails->SQL->Text + "Where (" +
													ParamString(Suppliers->Count, "Contact.Company_Name", "SupplierParam") + ")";
	}
	qrContactDetails->SQL->Text		= qrContactDetails->SQL->Text +
		"Order By "
			"Contact.Company_Name ";
	for (int i=0; i<Suppliers->Count; i++)
	{
		qrContactDetails->ParamByName("SupplierParam" + IntToStr(i))->AsString = Suppliers->Strings[i];
	}
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockReorderBySupplier(TStrings *Suppliers)
{
	qrStockReorder->Close();
	qrStockReorder->SQL->Text =
		"Select "
			"Contact.Company_Name,"
			"SupplierStock.Latest_Cost,"
			"SupplierStock.Supplier_Code,"
			"StockLocation.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,"
			"Stock.Description,"
			"StockLocation.Max_Level,"
			"StockLocation.Min_Level,"
			"StockLocation.On_Hand,"
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order,"
			"Stock.Stocktake_Unit,"
			"SupplierStock.Supplier_Unit,"
			"SupplierStock.Qty, "
			"SupplierStock.Min_Order_Qty "
		"From "
			"SupplierStock Inner Join Stock On "
				"SupplierStock.Stock_Key = Stock.Stock_Key "
			"Inner Join StockGroup On "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory On "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Inner Join StockLocation On "
				"StockLocation.Stock_Key = Stock.Stock_Key "
			"Inner Join Contact On "
				"SupplierStock.Supplier_Key = Contact.Contact_LK "
			"Left Join PurchaseStock On "
				"(PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location) "
		"Where "
			"(StockLocation.Max_Level <> 0 or StockLocation.Min_Level <> 0) and "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"SupplierStock.Default_Order_Unit = 'T' and "
			"SupplierStock.Preferred_Supplier = 'T' ";
	if (Suppliers->Count)
	{
		qrStockReorder->SQL->Text	=	qrStockReorder->SQL->Text + "And (" +
												ParamString(Suppliers->Count, "Contact.Company_Name", "SupplierParam") + ")";
	}
	qrStockReorder->SQL->Text		= qrStockReorder->SQL->Text +
		"Group By "
			"Contact.Company_Name,"
			"SupplierStock.Latest_Cost,"
			"SupplierStock.Supplier_Code,"
			"StockLocation.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,"
			"Stock.Description,"
			"StockLocation.Max_Level,"
			"StockLocation.Min_Level,"
			"StockLocation.On_Hand,"
			"Stock.Stocktake_Unit,"
			"SupplierStock.Supplier_Unit,"
			"SupplierStock.Qty, "
			"SupplierStock.Min_Order_Qty "
		"Having "
			"Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) + StockLocation.On_Hand <= StockLocation.Max_Level Or "
			"(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) Is Null And StockLocation.On_Hand <= StockLocation.Min_Level) "
		"Order By "
	"Contact.Company_Name, "
	"SupplierStock.Latest_Cost, "
	"SupplierStock.Supplier_Code, "
	"StockLocation.Location, "
	"StockCategory.Stock_Category, "
	"StockGroup.Stock_Group, "
	"Stock.Code, "
	"Stock.Description, "
	"StockLocation.Max_Level, "
	"StockLocation.Min_Level, "
	"StockLocation.On_Hand, "
	"Stock.Stocktake_Unit, "
	"SupplierStock.Supplier_Unit, "
	"SupplierStock.Qty, "
	"SupplierStock.Min_Order_Qty ";

/*	qrStockReorder->Close();
	qrStockReorder->SQL->Text =
		"Select "
			"Contact.Company_Name, "
			"SupplierStock.Latest_Cost, "
			"SupplierStock.Supplier_Code, "
			"StockLocation.Location, "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Description, "
			"StockLocation.Max_Level, "
			"StockLocation.Min_Level, "
			"StockLocation.On_Hand, "
			"StockLocation.On_Order, "
			"StockLocation.On_Hand + StockLocation.On_Order Current_Level, "
			"StockLocation.Max_Level - "
			"(StockLocation.On_Hand + StockLocation.On_Order) "
				"Stock_Reorder, "
			"Stock.Stocktake_Unit, "
			"Cast((StockLocation.Max_Level - "
			"(StockLocation.On_Hand + StockLocation.On_Order)) / SupplierStock.Qty as Numeric(15,4)) "
				"Supplier_Reorder, "
			"SupplierStock.Supplier_Unit "
		"From "
			"SupplierStock Inner Join Stock On "
				"SupplierStock.Stock_Key = Stock.Stock_Key "
			"Inner Join StockGroup On "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Inner Join StockCategory On "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			"Inner Join StockLocation On "
				"StockLocation.Stock_Key = Stock.Stock_Key "
			"Inner Join Contact On "
				"SupplierStock.Supplier_Key = Contact.Contact_LK "
		"Where "
			"(StockLocation.Max_Level <> 0 or StockLocation.Min_Level <> 0) and "
			"StockLocation.On_Order + StockLocation.On_Hand < StockLocation.Min_Level and "
			"SupplierStock.Default_Order_Unit = 'T' and "
			"SupplierStock.Preferred_Supplier = 'T' ";
	if (Suppliers->Count)
	{
		qrStockReorder->SQL->Text	=	qrStockReorder->SQL->Text + "And (" +
												ParamString(Suppliers->Count, "Contact.Company_Name", "SupplierParam") + ")";
	}
	qrStockReorder->SQL->Text		= qrStockReorder->SQL->Text +
		"Order By "
			"Contact.Company_Name, "
			"StockCategory.Sort_Order, "
			"StockGroup.Sort_Order, "
			"Stock.Description, "
			"StockLocation.Location "; */
	for (int i=0; i<Suppliers->Count; i++)
	{
		qrStockReorder->ParamByName("SupplierParam" + IntToStr(i))->AsString = Suppliers->Strings[i];
	}
}

void TdmStockReportData::SetupInvoices(int BatchKey)
{
     qrCommitedInvoices->Close();
     qrCommitedInvoices->SQL->Text = "SELECT "
                                        "ST.TRANS_NUMBER, "
                                        "ST.REFERENCE, "
                                        "ST.BATCH_KEY, "
                                        "ST.DESCRIPTION, "
                                        "ST.CODE, "
                                        "ST.STOCK_CATEGORY, "
                                        "ST.ORDER_UNIT AS SUPPLIER_UNIT, "
                                        "ST.STOCK_GROUP, "
                                        "S.GST_PERCENT, "
                                        "S.STOCK_KEY, "
                                        "ST.GL_CODE, "
                                        "ST.LOCATION, "
                                        "ST.SUPPLIER_CODE, "
                                        "ST.SUPPLIER_KEY, "
                                        "ST.UNIT, "
                                        "(ST.TOTAL_COST / ST.ORDER_QTY) AS LATEST_COST, " //"SS.LATEST_COST, "
                                        "ST.ORDER_QTY, "
                                        "ST.QTY As SUPPLIER_UNIT_SIZE, "//"SS.QTY AS SUPPLIER_UNIT_SIZE, "
                                        "ST.TOTAL_COST  "//"Cast((SS.LATEST_COST * ST.ORDER_QTY) as numeric(15, 4)) AS TOTAL_COST "
                                    "FROM STOCKTRANS ST "
                                    "INNER JOIN STOCK S ON ST.CODE = S.CODE "
                                    //"INNER JOIN SUPPLIERSTOCK SS ON S.STOCK_KEY = SS.STOCK_KEY AND ST.SUPPLIER_KEY = SS.SUPPLIER_KEY "
                                    "WHERE ST.BATCH_KEY = :Batch_Key";
     qrCommitedInvoices->ParamByName("Batch_Key")->AsInteger = BatchKey;
}

void TdmStockReportData::SetupInvoices(AnsiString BatchKeys)
{
   qrExportedInvoices->Close();
	qrExportedInvoices->SQL->Text =
		"SELECT "
         "TB.REFERENCE AS Invoice_Number, "
         "1 AS TotalRows, "
         "CAST(TB.CREATED AS DATE) AS Created_Date, "
         "TB.TOTAL_COST AS Tax_Exclusive_Amount, "
         "TB.TOTAL_GST AS Tax, "
         "TB.SUPPLIER_NAME, "
         "Cast((TB.TOTAL_COST + TB.TOTAL_GST)AS NUMERIC(15,4)) AS Total_Cost "
      "FROM TRANSACTIONBATCH TB "
      "WHERE TB.BATCH_KEY IN (";

   qrExportedInvoices->SQL->Text = qrExportedInvoices->SQL->Text + BatchKeys + ") ";
   qrExportedInvoices->SQL->Text = qrExportedInvoices->SQL->Text + "ORDER BY TB.SUPPLIER_NAME ASC,TB.REFERENCE ASC, TB.CREATED ";

 }

//---------------------------------------------------------------------------
void TdmStockReportData::SetupSupplierBackOrders(TStrings *Suppliers)
{
	qrSupplierBackOrders->Close();
	qrSupplierBackOrders->SQL->Text =
		"Select "
			"Contact.Company_Name Supplier_Name,"
			"PurchaseOrder.Order_Number,"
			"PurchaseOrder.Created,"
			"PurchaseStock.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"PurchaseStock.Code,"
			"Stock.Description,"
			"PurchaseStock.Supplier_Unit,"
			"PurchaseStock.Supplier_Unit_Qty,"
//			"PurchaseStock.Supplier_Unit_Qty - PurchaseStock.Qty_Received - PurchaseStock.Qty_Not_Received To_Receive "
			"PurchaseStock.Qty_On_Order To_Receive "
		"From "
			"PurchaseStock Left Join PurchaseOrder On "
				"PurchaseStock.Order_FK = PurchaseOrder.Order_LK "
			"Left Join Contact On "
				"PurchaseOrder.Contact_FK = Contact.Contact_LK "
			"Left Join Stock On "
				"PurchaseStock.Code = Stock.Code "
			"Left Join StockGroup On "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory On "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"PurchaseStock.Qty_On_Order > 0 ";
//			"Qty_Received + Qty_Not_Received < Supplier_Unit_Qty ";
	if (Suppliers->Count)
	{
		qrSupplierBackOrders->SQL->Text	=	qrSupplierBackOrders->SQL->Text + "And (" +
														ParamString(Suppliers->Count, "Contact.Company_Name", "SupplierParam") + ")";
	}
	qrSupplierBackOrders->SQL->Text		= qrSupplierBackOrders->SQL->Text +
		"Order By "
			"Contact.Company_Name,"
			"PurchaseOrder.Order_Number,"
			"PurchaseStock.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"PurchaseStock.Code,"
			"Stock.Description";

	for (int i=0; i<Suppliers->Count; i++)
	{
		qrSupplierBackOrders->ParamByName("SupplierParam" + IntToStr(i))->AsString = Suppliers->Strings[i];
	}
}
//---------------------------------------------------------------------------
/*
Select
	PurchaseOrder.Order_Number,
	Contact.Company_Name Supplier_Name,
	PurchaseOrder.Supplier_Address,
	PurchaseOrder.Supplier_FAX,
	PurchaseOrder.Delivery_Address,
	PurchaseOrder.Instructions,
	PurchaseOrder.Required,
	PurchaseStock.Code,
	StockCategory.Stock_Category,
	StockGroup.Stock_Group,
	Stock.Description,
	PurchaseStock.Supplier_Unit,
	PurchaseStock.Supplier_Code,
	Stock.GST_Percent,
	Cast(Sum(PurchaseStock.Supplier_Unit_Qty) as Numeric(15,4)) Order_Qty,
	Cast(Sum(PurchaseStock.Supplier_Unit_Qty * PurchaseStock.Supplier_Unit_Cost) as Numeric(15,4)) Total_Cost,
	Cast(Sum((PurchaseStock.Supplier_Unit_Qty * PurchaseStock.Supplier_Unit_Cost) * (Stock.GST_Percent + 100) / 100) as Numeric(15, 4) ) Total_Cost_Inc
From
	PurchaseOrder Left Join PurchaseStock on
		PurchaseOrder.Order_LK = PurchaseStock.Order_FK
			Left Join Stock On
				PurchaseStock.Code = Stock.Code
			Left Join StockGroup On
				Stock.Stock_Group_Key = StockGroup.Stock_Group_Key
			Left Join StockCategory On
				StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key
	Left Join Contact On
		PurchaseOrder.Contact_FK = Contact.Contact_LK
Where
	PurchaseOrder.Order_LK = 1
Group By
	PurchaseOrder.Order_Number,
	Contact.Company_Name,
	PurchaseOrder.Supplier_Address,
	PurchaseOrder.Supplier_FAX,
	PurchaseOrder.Delivery_Address,
	PurchaseOrder.Instructions,
	PurchaseOrder.Required,
	PurchaseStock.Code,
	StockCategory.Stock_Category,
	StockGroup.Stock_Group,
	Stock.Description,
	PurchaseStock.Supplier_Unit,
	PurchaseStock.Supplier_Code,
	Stock.GST_Percent
Order By
	StockCategory.Stock_Category,
	StockGroup.Stock_Group,
	Stock.Description,
	PurchaseStock.Supplier_Unit
 */
void TdmStockReportData::SetupPurchaseOrder(int OrderKey)
{
/*  last mod 9/12/04 for Dunedin Casino
  Report calcs GST by Stock.GST( by stock line )
  Total GST is calculated on Total Cost Inc - Total Cost to make it always balance
*/
	qrPurchaseOrder->Close();
	qrPurchaseOrder->SQL->Text =
		"Select "
			"PurchaseOrder.Order_Number,"
			"Contact.Company_Name Supplier_Name,"
            "CONTACT.EMAIL, "
            "CONTACT.PHONE, "
			"PurchaseOrder.Supplier_Address,"
			"PurchaseOrder.Supplier_FAX,"
			"PurchaseOrder.Delivery_Address,"
			"PurchaseOrder.Instructions,"
			"PurchaseOrder.Required,"
			"PurchaseStock.Code,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Description,"
			"PurchaseStock.Supplier_Unit,"
			"PurchaseStock.Supplier_Code,"
   			"Stock.GST_Percent,"
			"Cast(Sum(PurchaseStock.Supplier_Unit_Qty) as Numeric(17,4)) Order_Qty,"
   			"Cast(Sum(cast(cast(PurchaseStock.Supplier_Unit_Qty as Numeric(17,4)) * cast(PurchaseStock.Supplier_Unit_Cost as Numeric(17,4)) as Numeric(17,4))) as Numeric(17,4)) Total_Cost,  "
   			"Cast(Sum(cast((cast(PurchaseStock.Supplier_Unit_Qty * PurchaseStock.Supplier_Unit_Cost as Numeric(17,4)) ) * (cast(Stock.GST_Percent + 100 as Numeric(17,4))) / 100 as Numeric(17,4))) as Numeric(17, 4) ) Total_Cost_Inc,   "
             "PurchaseStock.Supplier_Unit_Cost, "
   		    "PurchaseOrder.CREATED "
		"From "
			"PurchaseOrder Left Join PurchaseStock on "
				"PurchaseOrder.Order_LK = PurchaseStock.Order_FK "
					"Left Join Stock On "
						"PurchaseStock.Code = Stock.Code "
					"Left Join StockGroup On "
						"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
					"Left Join StockCategory On "
						"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
			        "Left Join Contact On "
				"PurchaseOrder.Contact_FK = Contact.Contact_LK "
		"Where "
			"PurchaseOrder.Order_LK = :Order_LK "
		"Group By "
        	"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order,"
			"PurchaseOrder.Order_Number,"
			"Contact.Company_Name,CONTACT.EMAIL,CONTACT.PHONE, "
			"PurchaseOrder.Supplier_Address,"
			"PurchaseOrder.Supplier_FAX,"
			"PurchaseOrder.Delivery_Address,"
			"PurchaseOrder.Instructions,"
			"PurchaseOrder.Required,"
			"PurchaseStock.Code,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Description,"
			"PurchaseStock.Supplier_Unit,"
			"PurchaseStock.Supplier_Code, "
			"Stock.GST_Percent, "
            "PurchaseStock.Supplier_Unit_Cost, "
   		    "PurchaseOrder.CREATED "
		"Order By "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order,"
			"Stock.Description,"
			"PurchaseStock.Supplier_Unit ";
	qrPurchaseOrder->ParamByName("Order_LK")->AsInteger = OrderKey;
}
//---------------------------------------------------------------------------
//void TdmStockReportData::SetupStockVariance(TDateTime StartTime, TDateTime EndTime, TStrings *Strings)
void TdmStockReportData::SetupStockVariance(int StocktakeKey,int RadioButtonValue,AnsiString VarianceAmount)
{
	// called from the Stocktake Menu - prior to Commit
	qrStockVariance->Close();
	qrStockVariance->SQL->Text =
		"Select "
			"Stock.Code,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Description,"
			"StockLocation.Location,"
			"StockLocation.Average_Cost,"
			"StockLocation.Latest_Cost,"
			"StockLocation.On_Hand, "
			"StockLocation.Stocktake,"
			"StockLocation.Variance ,"
			"Cast(StockLocation.Variance * StockLocation.Average_Cost As Numeric(15, 4)) Cost,"
			"StockLocation.Opening,"
            "StockLocation.Sales*(-1) Sales , "
			"StockLocation.Writeoff*(-1) Writeoff,"
			"StockLocation.Inwards,"
			"StockLocation.Transfer, "
            "coalesce(STK.QTY,0) Manufacture "
		"From "
			"StockCategory Left Outer Join StockGroup On "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"Left Outer Join Stock On "
				"StockGroup.Stock_Group_Key = Stock.Stock_Group_Key "
			"Left Join StockLocation On "
				"Stock.Stock_Key = StockLocation.Stock_Key "
               " left join(select STOCKTRANS.CODE, sum(abs(stocktrans.QTY)) QTY, STOCKTRANS.LOCATION from STOCKTRANS where stocktrans.TRANSACTION_TYPE='Manufacture'  and STOCKTRANS.CREATED>="
			"(select max(STOCKTAKEHISTORY.LAST_STOCKTAKE)from STOCKTAKEHISTORY ) and stocktrans.BATCH_KEY >= (select max(STOCKTAKEHISTORY.BATCH_KEY)from STOCKTAKEHISTORY ) "
             "group by "
			   " stocktrans.CODE, "
			   " stocktrans.LOCATION "

            " )STK on STOCK.CODE=STK.CODE and STK.LOCATION=StockLocation.LOCATION "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"StockLocation.Stocktake_Key = :Stocktake_Key ";
	switch (RadioButtonValue)
	{
		case 0 : qrStockVariance->SQL->Text = qrStockVariance->SQL->Text + "and StockLocation.Variance <> 0 "; // Variances
                 break;
		case 1 : break; // All
		case 2 : qrStockVariance->SQL->Text = qrStockVariance->SQL->Text +  // Variances > VarianceAmount
						"and (StockLocation.Variance > " + VarianceAmount +
						"or StockLocation.Variance  < -" + VarianceAmount + ") ";
					break;
	}

	qrStockVariance->SQL->Text = qrStockVariance->SQL->Text +

        "Group by "
        "Stock.Code,"
        "StockCategory.Stock_Category,"
        "StockGroup.Stock_Group,"
        "Stock.Description,"
        "StockLocation.Location,"
        "StockLocation.Average_Cost,"
        "StockLocation.Latest_Cost,"
        "StockLocation.On_Hand,"
        "StockLocation.Stocktake,"
        "StockLocation.Variance ,"
        "StockLocation.Opening, "
        "StockLocation.Sales, "
        "StockLocation.Writeoff, "
        "StockLocation.Inwards, "
        "StockLocation.Transfer, "
        "STK.QTY, "
        "StockLocation.SALES_PENDING "

        
		"Order By "
        //"StockGroup.Stock_Group, "
			"StockLocation.Location, "
            "upper(StockCategory.Stock_Category)asc, "
            "upper(StockGroup.Stock_Group)asc, "
			//"StockCategory.Sort_Order,"
			//"StockGroup.Sort_Order, "
			"upper(Stock.Description)asc ";
	qrStockVariance->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;

   	qrStockGroupVariance->SQL->Text =
	 "select "
    "B.Location,  "
    "B.Stock_Category, "
    "B.Stock_Group, "
    "coalesce(A.Opening,0) Opening, "
	"B.Inwards, "
    "B.Transfer, "
    "B.Closing, "
    "B.Usage, "
    "B.Assessed_Total,	"
    "B.Manufacture "
	"from	"
	"((Select "
	"		StockLocation.Location, "
	"		StockCategory.Stock_Category, "
	"		StockGroup.Stock_Group, "
	"		Sum(Cast(StockLocation.Average_Cost * StockLocation.Inwards As Numeric(15, 4))) Inwards, "
	"		Sum(Cast(StockLocation.Average_Cost * StockLocation.Transfer As Numeric(15, 4))) Transfer, "
	"		Sum(Cast(StockLocation.Average_Cost * StockLocation.Stocktake As Numeric(15, 4))) Closing, "
	"		Sum(Cast((StockLocation.Opening + StockLocation.Inwards + StockLocation.Transfer - StockLocation.Stocktake) "
	"			 * StockLocation.Average_Cost As Numeric(15, 4))) Usage, "
	"		Sum(Cast((StockLocation.Opening + StockLocation.Inwards + StockLocation.Transfer - StockLocation.Stocktake) "
	"			 * StockLocation.Assessed_Value As Numeric(15, 4))) Assessed_Total, "
    "        coalesce(STK.QTY,0) Manufacture "
	"	From  "
	"		StockCategory Inner Join StockGroup On "
	"			StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
	"		Inner Join Stock On "
	"			StockGroup.Stock_Group_Key = Stock.Stock_Group_Key "
	"		Inner Join StockLocation On "
	"			Stock.Stock_Key = StockLocation.Stock_Key "
    "        Left join (select sum(abs(stocktrans.QTY))QTY,STOCKTRANS.LOCATION,STOCKTRANS.STOCK_GROUP from STOCKTRANS where stocktrans.TRANSACTION_TYPE='Manufacture'  and STOCKTRANS.CREATED>=  "
	"        (select max(STOCKTAKEHISTORY.LAST_STOCKTAKE)from STOCKTAKEHISTORY ) and stocktrans.BATCH_KEY >= (select max(STOCKTAKEHISTORY.BATCH_KEY)from STOCKTAKEHISTORY ) "
    "        and STOCKTRANS.location =(select STOCKTAKEHISTORY.LOCATION from STOCKTAKEHISTORY where stocktakehistory.STOCKTAKE_KEY =:Stocktake_Key  group by STOCKTAKEHISTORY.LOCATION ) "
    "        group by STOCKTRANS.LOCATION,STOCKTRANS.STOCK_GROUP)STK on StockGroup.STOCK_GROUP=STK.STOCK_GROUP and STK.LOCATION=StockLocation.LOCATION "
	"	Where "
	"		Stock.Enabled = 'T' And "
	"		(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
	"		StockLocation.Stocktake Is Not Null And "
	"		StockLocation.Stocktake_Key = :Stocktake_Key "
	"	Group By "
     "    StockLocation.Location, "
      "   StockCategory.Stock_Category, "
    "     StockCategory.Sort_Order, "
     "    StockGroup.Stock_Group, "
    "     StockGroup.Sort_Order , "
    "     STK.QTY "
	"	Order By "
	"		StockLocation.Location, "
    "       upper(StockCategory.Stock_Category)asc, "
    "		upper(StockGroup.Stock_Group)asc)B	"
	//"		StockCategory.Sort_Order, "
	//"		StockGroup.Sort_Order)B	"
	"left join "

    "(   	Select "
			"StocktakeHistory.Location LocationA,  "
			"StocktakeHistory.Stock_Category Stock_CategoryA,  "
			"StocktakeHistory.Stock_Group Stock_GroupA,  "
			"Sum(Cast(COALESCE(StocktakeHistory.Average_Unit_Cost,0) * COALESCE(StocktakeHistory.Closing ,0) As Numeric(15, 4))) Opening "
		"From  "
			"StocktakeHistory Left Join Stock On  "
			"	StocktakeHistory.Code = Stock.Code  "
			"Left Join StockLocation On  "
			"	Stock.Stock_Key = StockLocation.Stock_Key and StocktakeHistory.Location = StockLocation.Location "
		   "Where "
			"StocktakeHistory.Stocktake_Key = :prevStocktakeKey "
			
		"Group By  "
			"LocationA, "
			"Stock_CategoryA, "
			"Stock_GroupA  "
		"Order By "
		"	LocationA, "
		"	upper(StocktakeHistory.Stock_Category)asc, "
		 " 	upper(StocktakeHistory.Stock_Group)asc  ) A "

 "on A.LocationA = B.Location and A.Stock_CategoryA = B.Stock_Category and A.Stock_GroupA= B.Stock_Group ) ";
	qrStockGroupVariance->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStockVariance(int StocktakeKey)
{         int prevStocktakeKey =StocktakeKey-1;
	// called from the Stocktake Menu ( after Commit ) & the Reports Menu
	qrStockVariance->Close();
	qrStockVariance->SQL->Text =
		"Select "
			"StocktakeHistory.Code,"
			"StocktakeHistory.Stock_Category,"
			"StocktakeHistory.Stock_Group,"
			"StocktakeHistory.Description,"
			"StocktakeHistory.Location,"
			"StocktakeHistory.Latest_Unit_Cost  Latest_Cost,"
			"StocktakeHistory.Average_Unit_Cost Average_Cost,"
            //"STKLOC.On_Hand On_Hand, "

			"StocktakeHistory.Sales + StocktakeHistory.Transfer + StocktakeHistory.Writeoff + StocktakeHistory.Inwards + StocktakeHistory.Opening  + coalesce(STK.QTY,0)  On_Hand,"
			"StocktakeHistory.Closing - (StocktakeHistory.Sales + StocktakeHistory.Transfer + StocktakeHistory.Writeoff + StocktakeHistory.Inwards + StocktakeHistory.Opening ) Variance,"
			"StocktakeHistory.Closing Stocktake,"
			"cast((StocktakeHistory.Closing - (StocktakeHistory.Sales + StocktakeHistory.Transfer + StocktakeHistory.Writeoff + StocktakeHistory.Inwards + StocktakeHistory.Opening)) * StocktakeHistory.Average_Unit_Cost As Numeric(15, 4)) Cost,"
			"StocktakeHistory.Opening,"
			"StocktakeHistory.Sales*(-1) Sales, "
			"StocktakeHistory.Writeoff*(-1) Writeoff, "
			"StocktakeHistory.Inwards,"
			"StocktakeHistory.Transfer,  "
            "coalesce(abs(STK.QTY),0) Manufacture "

		"From "
			"StocktakeHistory left join(select STOCKTRANS.CODE, sum(stocktrans.QTY) QTY ,STOCKTRANS.LOCATION from STOCKTRANS where stocktrans.TRANSACTION_TYPE='Manufacture'  and STOCKTRANS.CREATED>="
			"(select max(STOCKTAKEHISTORY.LAST_STOCKTAKE)from STOCKTAKEHISTORY) "
            "group by "
			   " stocktrans.CODE, "
			   " stocktrans.LOCATION "
            " )STK on STOCKTAKEHISTORY.CODE=STK.CODE and STK.LOCATION=STOCKTAKEHISTORY.LOCATION "

		"Where "
			"StocktakeHistory.Stocktake_Key = :Stocktake_Key "

		" Group by "
			" StocktakeHistory.Code, "
			" StocktakeHistory.Stock_Category, "
			" StocktakeHistory.Stock_Group, "
			" StocktakeHistory.Description, "
			" StocktakeHistory.Location, "
			" StocktakeHistory.Latest_Unit_Cost , "
			" StocktakeHistory.Average_Unit_Cost , "
			" StocktakeHistory.Closing, "
			" StocktakeHistory.Opening, "
			" StocktakeHistory.Sales, "
			" StocktakeHistory.Writeoff, "
			" StocktakeHistory.Inwards, "
			" StocktakeHistory.Transfer, "
			" STK.QTY "

		"Order By "
			"StocktakeHistory.Location, "
			"StocktakeHistory.Stock_Category, "
			"StocktakeHistory.Stock_Group, "
			"StocktakeHistory.Description ";
	qrStockVariance->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;

	qrStockGroupVariance->Close();
	qrStockGroupVariance->SQL->Text =
     "select R.Opening, "
            "R.Location, "
            "R.Stock_Category, "
            "R.Stock_Group, "
            "R.Inwards, "
            "R.Transfer, "
            "R.Closing, "
            "R.Usage,  "
           " R.Assessed_Total,  "
           "R.QTY  Manufacture "

          "from "


   "(Select  "
            "A.Opening, "
            "B.Location, "
            "B.Stock_Category, "
            "B.Stock_Group, "
            "B.Inwards, "
            "B.Transfer, "
            "B.Closing, "
            "B.Usage, "
            "B.Assessed_Total, "
            "B.QTY  "
            "from "
		  	"(	Select "
			"StocktakeHistory.Location, "
			"StocktakeHistory.Stock_Category, "
			"StocktakeHistory.Stock_Group, "
			"Sum(Cast(COALESCE(StocktakeHistory.Latest_Unit_Cost,0) * COALESCE(StocktakeHistory.Inwards,0)  As Numeric(15, 4))) Inwards, "
			"Sum(Cast((StocktakeHistory.Average_Unit_Cost * StocktakeHistory.Transfer )As Numeric(15, 4))) Transfer, "
			"Sum(Cast(StocktakeHistory.Average_Unit_Cost * StocktakeHistory.Closing  As Numeric(15, 4))) Closing, "
			"Sum(Cast((StocktakeHistory.Opening + StocktakeHistory.Inwards + StocktakeHistory.Transfer - StocktakeHistory.Closing) "
				 "* StocktakeHistory.Average_Unit_Cost As Numeric(15, 4))) Usage, "
			"Sum(Cast((StocktakeHistory.Opening + StocktakeHistory.Inwards + StocktakeHistory.Transfer - StocktakeHistory.Closing) "
			"	 * StockLocation.Assessed_Value As Numeric(15, 4))) Assessed_Total, "
            " coalesce(STK.QTY,0)  QTY "
		"From "
			"StocktakeHistory Left Join Stock On  "
			 "StocktakeHistory.Code = Stock.Code left join (select sum(abs(stocktrans.QTY))QTY,STOCKTRANS.LOCATION,STOCKTRANS.STOCK_GROUP from STOCKTRANS where stocktrans.TRANSACTION_TYPE='Manufacture'  and STOCKTRANS.CREATED>= "
			   "(select max(STOCKTAKEHISTORY.LAST_STOCKTAKE)from STOCKTAKEHISTORY where STOCKTAKEHISTORY.STOCKTAKE_KEY = :Stocktake_Key ) "
               "and STOCKTRANS.location =(select STOCKTAKEHISTORY.LOCATION from STOCKTAKEHISTORY where stocktakehistory.STOCKTAKE_KEY =:Stocktake_Key group by STOCKTAKEHISTORY.LOCATION ) "
               "group by STOCKTRANS.LOCATION,STOCKTRANS.STOCK_GROUP)STK on STOCKTAKEHISTORY.STOCK_GROUP=STK.STOCK_GROUP and STK.LOCATION=STOCKTAKEHISTORY.LOCATION "
			"Left Join StockLocation On  "
			"	Stock.Stock_Key = StockLocation.Stock_Key and StocktakeHistory.Location = StockLocation.Location "
		"Where  "
			"StocktakeHistory.Stocktake_Key = :Stocktake_Key "
		"Group By "
			"StocktakeHistory.Location, "
			"StocktakeHistory.Stock_Category, "
			"StocktakeHistory.Stock_Group , "
			"STK.QTY "
		"Order By "
			"StocktakeHistory.Location,  "
			"StocktakeHistory.Stock_Category, "
			"StocktakeHistory.Stock_Group) B "
			"		  	left Join "

"(   	Select "
			"StocktakeHistory.Location LocationA,  "
			"StocktakeHistory.Stock_Category Stock_CategoryA,  "
			"StocktakeHistory.Stock_Group Stock_GroupA,  "
			"Sum(Cast(COALESCE(StocktakeHistory.Average_Unit_Cost,0) * COALESCE(StocktakeHistory.Closing ,0) As Numeric(15, 4))) Opening "
		"From  "
			"StocktakeHistory Left Join Stock On  "
			"	StocktakeHistory.Code = Stock.Code  "
			"Left Join StockLocation On  "
			"	Stock.Stock_Key = StockLocation.Stock_Key and StocktakeHistory.Location = StockLocation.Location "
		   "Where "
			"StocktakeHistory.Stocktake_Key = :prevStocktakeKey "
			
		"Group By  "
			"LocationA, "
			"Stock_CategoryA, "
			"Stock_GroupA  "
		"Order By "
		"	LocationA, "
		"	Stock_CategoryA, "  
		 " 	Stock_GroupA  ) A "

 "on A.LocationA = B.Location and A.Stock_CategoryA = B.Stock_Category and A.Stock_GroupA= B.Stock_Group ) R "
 " left join (select stocktrans.STOCK_CATEGORY, "
 " stocktrans.STOCK_GROUP, "
     " sum(  "
       "coalesce(stocktrans.TOTAL_COST,0))Inwards from STOCKTRANS "
        "where STOCKTRANS.TRANSACTION_TYPE = 'Receipt' and StockTrans.Created >= "
		 " (select max(STOCKTAKEHISTORY.LAST_STOCKTAKE) from STOCKTAKEHISTORY where STOCKTAKEHISTORY.STOCKTAKE_KEY=:Stocktake_Key) and "
		 " StockTrans.Created <= (select cast('NOW' as timestamp) from rdb$database)  "
		  "group by STOCKTRANS.STOCK_CATEGORY, stocktrans.STOCK_GROUP)K on R.Stock_Category=K.Stock_Category and R.STOCK_GROUP=K.STOCK_GROUP " ;

    qrStockGroupVariance->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
    qrStockGroupVariance->ParamByName("prevStocktakeKey")->AsInteger = prevStocktakeKey;
  }

//---------------------------------------------------------------------------
void TdmStockReportData::SetupStocktake(int StocktakeKey, bool HideOnHand, bool HideBarcodes)
{
	qrStocktake->Close();
	qrStocktake->SQL->Text =
		"Select "
			"StockLocation.Location,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"Stock.Code,";
	if (HideBarcodes)
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Null Barcode,";
	}
	else
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Barcode.Barcode,";
	}
	qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Stock.Description,"
//			"StockLocation.On_Hand,"
			"Stock.Stocktake_Unit,";

	if (HideOnHand)
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Cast (32768 as Numeric(15,4)) On_Hand,";
	}
	else
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"StockLocation.On_Hand,";
	}
	qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As Numeric(15,4)) On_Order "
		"From "
			"StockCategory Inner Join StockGroup On "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"Inner Join Stock On "
				"StockGroup.Stock_Group_Key = Stock.Stock_Group_Key "
			"Inner Join StockLocation On "
				"StockLocation.Stock_Key = Stock.Stock_Key "
			"Left Join PurchaseStock On "
				"(PurchaseStock.Code = Stock.Code And PurchaseStock.Location = StockLocation.Location) ";

	if (false == HideBarcodes)
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Left Join Barcode on "
				"Stock.Stock_Key = Barcode.Stock_Key ";
	}

	qrStocktake->SQL->Text = qrStocktake->SQL->Text +
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"StockLocation.Stocktake_Key = :Stocktake_Key "
		"Group By "
			"StockLocation.Location,"
			"StockCategory.Sort_Order,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"
			"StockGroup.Sort_Order,"
			"Stock.Code,";
	if (false == HideBarcodes)
	{
		qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Barcode.Barcode,";
	} 
	qrStocktake->SQL->Text = qrStocktake->SQL->Text +
			"Stock.Description,"
			"Stock.Stocktake_Unit,"
			"StockLocation.On_Hand "
		"Order By "
			"StockLocation.Location, "
            "upper(StockCategory.Stock_Category)asc, "
            "upper(StockGroup.Stock_Group)asc, "
            "upper(Stock.Description)asc ";

	qrStocktake->ParamByName("Stocktake_Key")->AsString = StocktakeKey;
}
//---------------------------------------------------------------------------
void TdmStockReportData::SetupStocktakeStockValuation(TStrings *Stocktakes)
{
	qrStockValue->Close();
	qrStockValue->SQL->Text =
		"Select "
			"StocktakeHistory.Stock_Category,"
			"StocktakeHistory.Stock_Group,"
			"StocktakeHistory.Code,"
			"StocktakeHistory.Stocktake_Unit,"
			"StocktakeHistory.Description,"
			"StocktakeHistory.Location,"
			"StocktakeHistory.Closing On_Hand,"
			"cast(StocktakeHistory.Closing * StocktakeHistory.Average_Unit_Cost as float) Average_Cost,"
			"cast(StocktakeHistory.Closing * StocktakeHistory.Latest_Unit_Cost as float) Latest_Cost,"
			"StocktakeHistory.Latest_Unit_Cost  Single_Latest_Cost,"
			"StocktakeHistory.Average_Unit_Cost Single_Average_Cost,"
			"Stocktake.Name,"
			"Stocktake.Committed_User_Name,"
			"Stocktake.Initialised_User_Name,"
			"Stocktake.Committed_Time, "
			"Stocktake.Initialised_Time "
		"From "
			"StocktakeHistory Left Join Stocktake On "
				"StocktakeHistory.Batch_Key = Stocktake.Batch_key ";
	if (Stocktakes->Count)
	{
		qrStockValue->SQL->Text	=	qrStockValue->SQL->Text + "Where (" +
											ParamString(Stocktakes->Count, "StocktakeHistory.Stocktake_Key", "StocktakeParam") + ")";
	}
	qrStockValue->SQL->Text		= qrStockValue->SQL->Text +
		"Order By "
			"Stocktake.Name,"
			"Stocktake.Initialised_Time, "
			"StocktakeHistory.Location, "
			"StocktakeHistory.Stock_Category, "
			"StocktakeHistory.Stock_Group, "
			"StocktakeHistory.Description ";
	for (int i=0; i<Stocktakes->Count; i++)
	{
		qrStockValue->ParamByName("StocktakeParam" + IntToStr(i))->AsString = Stocktakes->Strings[i];
	}

	 /*	"Select "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"StockLocation.On_Hand * StockLocation.Average_Cost Average_Cost, "
			"StockLocation.On_Hand * StockLocation.Latest_Cost Latest_Cost "
		"From "
			"Stock Inner Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Left Join StockGroup on "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
//			"StockLocation.Location = :Location And "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) And "
			"StockLocation.Stocktake_Key = :Stocktake_Key "
		"Order By "
			"StockLoc	"StockCategory.Sort_Order, "
			"StockGroup.Sort_Order, "
			"Stock.Description "; */
	//qrStockValue->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
}
//---------------------------------------------------------------------------



 void TdmStockReportData::SetupStockRequestDetails(AnsiString Group,int StockReqKey,AnsiString Location)
{ qrStockRequest->Close();
	qrStockRequest->SQL->Text =

	"Select "
			"StockCategory.Stock_Category, "
			"StockGroup.Stock_Group, "
		   "STOCKREQUEST_STOCK.QTY_REQUIRED QTY, "
			"STOCKREQUEST.REQUEST_NUMBER, "
			"STOCKREQUEST.REQUEST_TYPE, "
         " cast(STOCKREQUEST.DATE_REQUIRED as timestamp ) DATE_REQUIRED , "
			"STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT, "
			"STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT_COST, "
            "Cast(Sum( STOCKREQUEST_STOCK.QTY_REQUIRED * STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT_COST) As Numeric(15,4)) STOCK_REQUEST_Total_COST, "
			"Stock.Code, "
			"Stock.Stocktake_Unit, "
			"Stock.Description, "
			"StockLocation.Location, "
			"StockLocation.On_Hand, "
			"cast(StockLocation.On_Hand * StockLocation.Average_Cost as float) Average_Cost, "
			"cast(StockLocation.On_Hand * StockLocation.Latest_Cost as float) Latest_Cost, "
			"StockLocation.Average_Cost Single_Average_Cost, "
			"StockLocation.Latest_Cost  Single_Latest_Cost "
		"From "
			"STOCKREQUEST_STOCK "
		  "	Left Join STOCKREQUEST on STOCKREQUEST.REQUEST_NUMBER  = STOCKREQUEST.REQUEST_NUMBER  "
			"Inner Join Stock on STOCKREQUEST_STOCK.STOCK_KEY  = Stock.Stock_Key "
			"Inner Join StockLocation on "
			"STOCKREQUEST_STOCK.Stock_Key = StockLocation.Stock_Key "
			"Left Join StockGroup on "
			"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory on "
			"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"Stock.Enabled = 'T' And "
			"(Stock.Deleted = 'F' Or Stock.Deleted Is Null) "
			"and STOCKREQUEST.REQUEST_NUMBER = :REQUEST_NUMBER "
         "and STOCKREQUEST_STOCK.REQUEST_NUMBER   = :REQUEST_NUMBER "
		" and STOCKREQUEST_STOCK.ISREMOVED='F' "
			"and "
			"StockCategory.Stock_Category = :Stock_Category  "
			"and "
			"StockLocation.Location = :Location "

             " group by "

            "STOCKREQUEST.DATE_REQUIRED  , "
            "STOCKREQUEST.REQUEST_NUMBER, "
              "StockCategory.Stock_Category, "
            "StockGroup.Stock_Group,   "
            "STOCKREQUEST_STOCK.QTY_REQUIRED , "
            "STOCKREQUEST.REQUEST_TYPE, "
            "STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT, "
            "STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT_COST, "
            "Stock.Code,  "
            "Stock.Stocktake_Unit,           "
            "Stock.Description,            "
            "StockLocation.Location,          "
            "StockLocation.On_Hand,          "
            "StockLocation.Average_Cost ,   "
            "StockLocation.Latest_Cost  ,  "
            "StockLocation.Average_Cost , "
            "StockLocation.Latest_Cost , "
            " StockCategory.Sort_Order, "
			"StockGroup.Sort_Order, "
			"Stock.Description,  "
		   "	StockLocation.Location    "
            " order by     "
           "StockCategory.Sort_Order,  "
			"StockGroup.Sort_Order,  "
			"Stock.Description,     "
			"StockLocation.Location  " ;

          qrStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger = StockReqKey;
          qrStockRequest->ParamByName("Stock_Category")->AsString = Group;
qrStockRequest->ParamByName("Location")->AsString = Location;


}

void TdmStockReportData::SetupSupplierInvoice(TDateTime StartTime, TDateTime EndTime)
{
    qrSupplierInvoice->Close();
    qrSupplierInvoice->SQL->Text =
    "SELECT TB.BATCH_KEY, ST.STOCK_CATEGORY, 1 as CategoryCount, IT.STOCK_GROUP, "
    "MIN(CASE WHEN IT.TRANSACTION_TYPE = 'Receipt' THEN IT.INVOICE_TOTAL END) AS INVOICE_TOTAL, "
    "MIN(CASE WHEN IT.TRANSACTION_TYPE = 'Packing Slip' THEN IT.INVOICE_TOTAL END) AS PACKING_SLIP_TOTAL, TB.CREATED "
    "FROM TRANSACTIONBATCH TB "
    "INNER JOIN STOCKTRANS ST ON TB.BATCH_KEY = ST.BATCH_KEY "
    "LEFT JOIN (SELECT STOCKTRANS.BATCH_KEY,STOCKTRANS.STOCK_GROUP,STOCKTRANS.TRANSACTION_TYPE, STOCKTRANS.TOTAL_COST AS INVOICE_TOTAL FROM STOCKTRANS ) IT ON TB.BATCH_KEY = IT.BATCH_KEY "
    "WHERE TB.CREATED >= :StartTime and TB.CREATED <= :EndTime "

    "and (TB.TRANSACTION_TYPE = 'Receipt' OR TB.TRANSACTION_TYPE = 'Packing Slip')"
    "GROUP BY ST.STOCK_CATEGORY, IT.STOCK_GROUP, TB.BATCH_KEY, IT.INVOICE_TOTAL, TB.CREATED ";


	qrSupplierInvoice->ParamByName("StartTime")->AsDateTime	= StartTime;
 	qrSupplierInvoice->ParamByName("EndTime")->AsDateTime   = EndTime;
}


void TdmStockReportData::TransferAudit(int Transfer_No)
{
     qrTransferAudit->Close();
	qrTransferAudit->SQL->Text =
	 "Select "

        "STOCKTRANS.TRANSFER_ID, "
           "STOCKTRANS.Location   From_Location, "
           "StockTrans.Purchaser_Name To_Location, "
           "STOCKTRANS.Created, "
           "STOCKTRANS.REFERENCE, "
           "STOCKTRANS.USER_ID,"
            "STOCKTRANS.Description, "
            "STOCKTRANS.UNIT, "
           "STOCKTRANS.UNIT_COST, "
            "STOCKTRANS.Stock_Category, "
           "STOCKTRANS.Stock_Group,"
          "Sum(StockTrans.Qty) Qty, "
         	"Sum(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost, "
             "STOCKTRANS.STOCK_REQUEST_NO, "
              "STOCKTRANS.REQUESTED_QTY_ORIGINAL_VAL "
	  "From "
			  "StockTrans  inner join     STOCKREQUEST_STOCK on STOCKTRANS.STOCK_REQUEST_STOCK_KEY_VAL = STOCKREQUEST_STOCK.STOCK_REQUEST_STOCKKEY "
	  "Where "
			"STOCKTRANS.TRANSFER_ID = :TRANSFER_ID  and Qty>0 ";


	qrTransferAudit->SQL->Text		=	qrTransferAudit->SQL->Text +
		" Group By "
		   "STOCKTRANS.Description, "
	      "STOCKTRANS.TRANSFER_ID, "
          "STOCKTRANS.Purchaser_Name , "
           "STOCKTRANS.Location, "
           "STOCKTRANS.Created, "
           "STOCKTRANS.REFERENCE, "
           "STOCKTRANS.USER_ID, "
           "STOCKTRANS.UNIT, "
           "STOCKTRANS.UNIT_COST, "
            "STOCKTRANS.Stock_Category, "
           "STOCKTRANS.Stock_Group, "
           "STOCKTRANS.STOCK_REQUEST_NO, "
           "STOCKTRANS.REQUESTED_QTY_ORIGINAL_VAL "
       "Order By "
			"STOCKTRANS.STOCK_REQUEST_NO,STOCKTRANS.Description ";

  	qrTransferAudit->ParamByName("TRANSFER_ID")->AsInteger	= Transfer_No;


}



//---------------------------------------------------------------------------
void TdmStockReportData::SetupSupplierPurchasesRuntime(AnsiString ReceiptNumber,  AnsiString Suppliers,  AnsiString Invoices)     //,AnsiString BatchKey
{
 qrStockReceiptByCat->Close();
 qrStockReceiptByCat->SQL->Text =
  "Select "
//   "StockCategory.Stock_Category, "
//   "StockGroup.Stock_Group, "
   "StockTrans.Stock_Category, "
   "StockTrans.Stock_Group, "
   "StockTrans.Code, "
   "StockTrans.Description, "
   "StockTrans.Location, "
   "StockTrans.Created, "
   "StockTrans.Supplier_Name, "
   "StockTrans.Reference, "
   "StockTrans.ORDER_QTY as Qty, "
   "StockTrans.Order_Unit as Unit, "
   "StockTrans.Total_Cost, "
   //"StockTrans.Unit_Cost * StockTrans.Qty Total_Cost, "
   "StockTrans.Total_Cost + StockTrans.Total_GST as Total_Cost_Inc, "
            "StockTrans.BATCH_KEY "
    // "Cast((Cast(StockTrans.Unit_Cost As Double Precision)  StockTrans.Qty)  (StockTrans.GST_Percent + 100) / 100 as Numeric(15, 4)) Total_Cost_Inc "
  "From "
   "StockTrans "
/*   "StockTrans Left Join Stock on "
    "StockTrans.Code = Stock.Code "
   "Left Join StockGroup on "
    "Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
   "Left Join StockCategory on "
    "StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "*/
  "Where "

   "StockTrans.BATCH_KEY =:ReceiptNumber  and "
   "StockTrans.Transaction_Type = 'Receipt' and "
            "StockTrans.Supplier_Name=:Supplier_Name  and "
            "StockTrans.Reference=:Reference "    ;

 qrStockReceiptByCat->SQL->Text   = qrStockReceiptByCat->SQL->Text +
  "Order By "
            "StockTrans.BATCH_KEY, "
   "StockTrans.Supplier_Name, "
   "StockTrans.Created, "
   "StockTrans.Reference, "
   "StockTrans.Stock_Category, "
//   "StockCategory.Sort_Order, "
//   "StockGroup.Sort_Order, "
   "StockTrans.Description ";


 qrStockReceiptByCat->ParamByName("ReceiptNumber")->AsString  = ReceiptNumber;
     qrStockReceiptByCat->ParamByName("Supplier_Name")->AsString = Suppliers;
 qrStockReceiptByCat->ParamByName("Reference")->AsString  = Invoices;

}
//---------------------------------------------------------------------------

void TdmStockReportData::SetupStockReconcialation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups)
{
    qrStockReconcialation->Close();

    qrStockReconcialation->SQL->Text =

    "select "
    "StockRe.STOCK_GROUP, "
    "StockRe.STOCK_SUB_GROUP, "
    "StockRe.LOCATION, "
    "cast(sum(StockRe.opening)as numeric(17,4)) as opening, "
    "cast(sum(StockRe.INWARDS)as numeric(17,4)) INWARDS, "
    "cast(sum(StockRe.Outgoing) as numeric(17,4)) Outgoing, "
    "cast(sum(StockRe.opening) + sum(StockRe.INWARDS) - sum(StockRe.Outgoing) as numeric(17,4)) closing  "
  "from(  "

    "select                                           "
    "    STOCKTRANS.STOCK_GROUP,                      "
    "     STOCK.STOCK_SUB_GROUP,                      "
    "    STOCKLOCATION.LOCATION,                      "
 //   "    STOCKLOCATION.STOCK_KEY,                     "
    "    cast(coalesce(ST.qty,0)as numeric(17,4)) as opening,               "
    "    cast(coalesce(incoming.qty,0)as numeric(17,4)) INWARDS,            "
    "    cast(coalesce(abs(salePlusWastage.qty),0) as numeric(17,4)) Outgoing,    "
    "    cast(coalesce(ST.qty,0) + coalesce(incoming.qty,0) - coalesce(abs(salePlusWastage.qty),0) as numeric(17,4)) closing "

    "    from STOCKLOCATION                                              "
    "    left join STOCK on STOCKLOCATION.STOCK_KEY=STOCK.STOCK_KEY      "
    "    left join (                                                    "
    "           select sum(coalesce(a.QTY,0)) qty , a.LOCATION, s.STOCK_SUB_GROUP   "
    "                    from STOCKTRANS a                               "
    "           inner join stock s on a.CODE = s.CODE "
    "                    where a.CREATED <= :StartTime   "
    "                    group by a.LOCATION, s.STOCK_SUB_GROUP)ST on ST.STOCK_SUB_GROUP = STOCK.STOCK_SUB_GROUP and ST.location = STOCKLOCATION.LOCATION "
    "     left join ( "
    "           select sum(coalesce(a.QTY,0)) qty , a.LOCATION, s.STOCK_SUB_GROUP "
    "                    from STOCKTRANS a "
    "           left join TRANSACTIONBATCH TB on TB.BATCH_KEY = a.BATCH_KEY "
    "           inner join stock s on a.CODE = s.CODE "
    "                    where (a.TRANSACTION_TYPE = 'Receipt' or a.TRANSACTION_TYPE = 'Stocktake' or (a.TRANSACTION_TYPE = 'Packing Slip' and TB.ISCOMMITTED_PACKINGSLIP = 'T')  or a.TRANSACTION_TYPE = 'Refund'  or (a.TRANSACTION_TYPE = 'Transfer' and a.QTY > 0)) and a.CREATED >= :StartTime and a.CREATED <= :EndTime "
    "                    group by a.LOCATION, s.STOCK_SUB_GROUP)incoming on incoming.STOCK_SUB_GROUP = STOCK.STOCK_SUB_GROUP and incoming.location = STOCKLOCATION.LOCATION  "
    "    left join ( "
    "           select sum(coalesce(a.QTY,0)) qty , a.LOCATION, s.STOCK_SUB_GROUP "
    "                    from STOCKTRANS a "
    "           inner join stock s on a.CODE = s.CODE "
    "                    where (a.TRANSACTION_TYPE = 'Sale' or ((a.TRANSACTION_TYPE = 'WriteOff' or a.TRANSACTION_TYPE = 'Writeoff') and a.QTY < 0) or (a.TRANSACTION_TYPE = 'Transfer' and a.QTY < 0)) and a.CREATED >= :StartTime and a.CREATED <= :EndTime "

    "                    group by a.LOCATION, s.STOCK_SUB_GROUP)salePlusWastage on salePlusWastage.STOCK_SUB_GROUP = STOCK.STOCK_SUB_GROUP and salePlusWastage.location = STOCKLOCATION.LOCATION   "

 //   "    left join STOCKGROUP on STOCK.STOCK_GROUP_KEY=STOCKGROUP.STOCK_GROUP_KEY  "

    " left  join STOCKTRANS on STOCK.CODE = STOCKTRANS.CODE  "
      "  WHERE STOCKTRANS.CREATED  >= :StartTime  and STOCKTRANS.CREATED <= :EndTime and STOCK.DELETED <> 'T' ";



    if (Locations->Count)
	{
		qrStockReconcialation->SQL->Text	=	qrStockReconcialation->SQL->Text + "and (" +
												ParamString(Locations->Count, "STOCKLOCATION.Location", "LocationParam") + ")";
	}
	if (Groups->Count > 0)
	{
		qrStockReconcialation->SQL->Text	=	qrStockReconcialation->SQL->Text + "And (" +
												ParamString(Groups->Count, "STOCKTRANS.Stock_Group", "GroupParam") + ")";
	}

	qrStockReconcialation->SQL->Text		=	qrStockReconcialation->SQL->Text +


     " Group by                       "
    "    STOCKLOCATION.LOCATION,     "
 //   "    STOCKLOCATION.STOCK_KEY,    "
    "    STOCKTRANS.STOCK_GROUP,     "
    "    STOCK.STOCK_SUB_GROUP ,     "
    "    ST.qty, incoming.qty ,      "
    "    salePlusWastage.qty         " 

     ")StockRe  "
       
       " Group by  "
       " StockRe.LOCATION,   "
       " StockRe.STOCK_GROUP, "
       " StockRe.STOCK_SUB_GROUP  "
  "order by "
       "StockRe.STOCK_GROUP, StockRe.STOCK_SUB_GROUP, StockRe.LOCATION ";

 	for (int i=0; i<Locations->Count; i++)
	{
		qrStockReconcialation->ParamByName("LocationParam" + IntToStr(i))->AsString = Locations->Strings[i];
	}
       for (int i=0; i<Groups->Count; i++)
    {
        qrStockReconcialation->ParamByName("GroupParam" + IntToStr(i))->AsString = Groups->Strings[i];
    }

    qrStockReconcialation->ParamByName("StartTime")->AsDateTime	= StartTime;
    qrStockReconcialation->ParamByName("EndTime")->AsDateTime		= EndTime;
}

