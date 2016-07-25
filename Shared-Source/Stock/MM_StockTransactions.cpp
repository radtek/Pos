//---------------------------------------------------------------------------
#pragma hdrstop

#include <Forms.hpp>
#include <vector>
#include "MM_StockTransactions.h"
#include "CSV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Stock;
//---------------------------------------------------------------------------
TStockTransaction::TStockTransaction(TIBDatabase *IBDatabase) : TStockControl(IBDatabase),
			sqlCreateBatch(			new TIBSQL(NULL)),
			sqlUpdateBatch(			new TIBSQL(NULL)),
			sqlCreateTransaction(	new TIBSQL(NULL)),
			sqlStockDetails(			new TIBSQL(NULL)),
			sqlAdjustStock(			new TIBSQL(NULL))
{
	fDBTransaction.RegisterQuery(sqlCreateBatch);
	fDBTransaction.RegisterQuery(sqlUpdateBatch);
	fDBTransaction.RegisterQuery(sqlCreateTransaction);
	fDBTransaction.RegisterQuery(sqlStockDetails);
	fDBTransaction.RegisterQuery(sqlAdjustStock);

	sqlCreateBatch->SQL->Text =
		"Insert Into TransactionBatch ("
			"Batch_Key,"
			"Created,"
			"Transaction_Type,"
			"User_ID,"
			"User_Name,"
			"Reference,"
			"Supplier_Key,"
			"Supplier_Name,"
			"Supplier_ID,"
			"Export_1,"
			"Export_2,"
			"Export_3,"
			"Order_Number,"
			"Total_Cost,"
			"Total_GST,"
			"Closed,"
			"Exported )"
		"Values ("
			":Batch_Key,"
			":Created,"
			":Transaction_Type,"
			":User_ID,"
			":User_Name,"
			":Reference,"
			":Supplier_Key,"
			":Supplier_Name,"
			":Supplier_ID,"
			":Export_1,"
			":Export_2,"
			":Export_3,"
			":Order_Number,"
			":Total_Cost,"
			":Total_GST,"
			":Closed,"
			":Exported )";

	sqlUpdateBatch->SQL->Text =
		"Update "
			"TransactionBatch "
		"Set "
			"Total_Cost = :Total_Cost,"
			"Total_GST = :Total_GST "
		"Where "
			"Batch_Key = :Batch_Key";

	sqlCreateTransaction->SQL->Text =
		"Insert Into StockTrans ("
			"Trans_Number,"
			"Batch_Key,"
			"Transaction_Type,"
			"Code,"
			"Description,"
			"GST_Percent,"
			"Total_GST,"
			"Stock_Category,"
			"Stock_Group,"
			"GL_Code,"
			"Unit,"
			"Unit_Cost,"
			"Total_Cost,"
			"Location,"			// Transfer - Source
			"Purchaser_Name,"	// Transfer - Destination
			"User_ID,"
			"User_Name,"

			"Order_Unit,"
			"Order_Qty,"
			"Supplier_Code,"

			"Order_Number,"
			"Reference,"
			"Supplier_Key,"
			"Supplier_Name,"

			"Created,"
			"Qty,"
			"Note"
			")"
		"Values ("
			":Trans_Number,"
			":Batch_Key,"
			":Transaction_Type,"
			":Code,"
			":Description,"
			":GST_Percent,"
			":Total_GST,"
			":Stock_Category,"
			":Stock_Group,"
			":GL_Code,"
			":Unit,"
			":Unit_Cost,"
			":Total_Cost,"
			":Location,"
			":Purchaser_Name,"
			":User_ID,"
			":User_Name,"

			":Order_Unit,"
			":Order_Qty,"
			":Supplier_Code,"

			":Order_Number,"
			":Reference,"
			":Supplier_Key,"
			":Supplier_Name,"

			":Created,"
			":Qty,"
			":Note"
			")";

	sqlStockDetails->SQL->Text =
		"Select "
			"Stock.Code,"
			"Stock.Description,"
			"Stock.GST_Percent,"
			"Stock.Stocktake_Unit,"

			"StockCategory.GL_Code,"
			"StockCategory.Stock_Category,"
			"StockGroup.Stock_Group,"

			"StockLocation.Location,"
			"StockLocation.On_Hand,"
//			"StockLocation.On_Order,"
			"StockLocation.Latest_Cost,"
			"StockLocation.Average_Cost,"

			"StockLocation.Writeoffs_Pending,"
			"StockLocation.Sales_Pending,"

//			"StockLocation.Initialised_Time_Stamp,"
//			"StockLocation.Initialised_User_ID,"
//			"StockLocation.Initialised_User_Name,"

			"StockLocation.Last_Stocktake,"
			"StockLocation.Transfer,"
			"StockLocation.Inwards,"
			"StockLocation.Opening,"
			"StockLocation.Writeoff,"
			"StockLocation.Sales,"
			"StockLocation.Stocktake "

		"From "
			"Stock Left Join StockLocation On "
				"Stock.Stock_Key = StockLocation.Stock_Key "
			"Left Join StockGroup On "
				"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Left Join StockCategory On "
				"StockGroup.Stock_Category_Key = StockCategory.Stock_Category_Key "
		"Where "
			"Stock.Stock_Key = :Stock_Key And "
			"Upper(Location) = :UpperLocation";
}
//---------------------------------------------------------------------------
bool TStockTransaction::fGetStockDetails(int StockKey, AnsiString Location, TStockLocationDetails& StockLocationDetails)
{
	sqlStockDetails->Close();
	sqlStockDetails->ParamByName("Stock_Key")->AsInteger		= StockKey;
	sqlStockDetails->ParamByName("UpperLocation")->AsString	= Location.UpperCase();
	sqlStockDetails->ExecQuery();

	if (sqlStockDetails->Eof)
	{
		// Not found!
		sqlStockDetails->Close();
		// If there is no source location, this is an error. Bail out. (Deleted by another user?)
		return false;
	}
	else
	{
		fReadStockDetails(StockLocationDetails);
		// Do an exact match incase there are more that one. (Unlikely, unless doing a location rename.)
		for(; !sqlStockDetails->Eof; sqlStockDetails->Next())
		{
			if(sqlStockDetails->FieldByName("Location")->AsString == Location)
			{
				fReadStockDetails(StockLocationDetails);
			}
		}
		sqlStockDetails->Close();
		return true;
	}
/*//	// If there is no source location, this is an error. Bail out. (Deleted by another user?)
	if (!sqlStockDetails->Eof)
	{
		StockLocationDetails.StockCode			= sqlStockDetails->FieldByName("Code")->AsString;
		StockLocationDetails.Description			= sqlStockDetails->FieldByName("Description")->AsString;
		StockLocationDetails.GST_Percent			= sqlStockDetails->FieldByName("GST_Percent")->AsDouble;
		StockLocationDetails.Stock_Category		= sqlStockDetails->FieldByName("Stock_Category")->AsString;
		StockLocationDetails.Stock_Group			= sqlStockDetails->FieldByName("Stock_Group")->AsString;
		StockLocationDetails.GL_Code				= sqlStockDetails->FieldByName("GL_Code")->AsString;
		StockLocationDetails.Stocktake_Unit		= sqlStockDetails->FieldByName("Stocktake_Unit")->AsString;
		StockLocationDetails.Average_Cost		= sqlStockDetails->FieldByName("Average_Cost")->AsDouble;
		StockLocationDetails.Latest_Cost			= sqlStockDetails->FieldByName("Latest_Cost")->AsDouble;
		StockLocationDetails.On_Hand				= sqlStockDetails->FieldByName("On_Hand")->AsDouble;
//		StockLocationDetails.On_Order				= sqlStockDetails->FieldByName("On_Order")->AsDouble;

		StockLocationDetails.Writeoffs_Pending	= sqlStockDetails->FieldByName("Writeoffs_Pending")->AsDouble;
		StockLocationDetails.Sales_Pending		= sqlStockDetails->FieldByName("Sales_Pending")->AsDouble;

		StockLocationDetails.Last_Stocktake		= sqlStockDetails->FieldByName("Last_Stocktake")->AsDateTime;
		StockLocationDetails.Transfer				= sqlStockDetails->FieldByName("Transfer")->AsDouble;
		StockLocationDetails.Inwards				= sqlStockDetails->FieldByName("Inwards")->AsDouble;
		StockLocationDetails.Opening				= sqlStockDetails->FieldByName("Opening")->AsDouble;
		StockLocationDetails.Writeoff				= sqlStockDetails->FieldByName("Writeoff")->AsDouble;
		StockLocationDetails.Sales					= sqlStockDetails->FieldByName("Sales")->AsDouble;
		StockLocationDetails.Stocktake			= sqlStockDetails->FieldByName("Stocktake")->AsDouble;

		sqlStockDetails->Close();
		return true;
	}
	else
	{
		sqlStockDetails->Close();
		return false;
	} */
}
//---------------------------------------------------------------------------
void TStockTransaction::fReadStockDetails(TStockLocationDetails& StockLocationDetails)
{
	StockLocationDetails.StockCode			= sqlStockDetails->FieldByName("Code")->AsString;
	StockLocationDetails.Description			= sqlStockDetails->FieldByName("Description")->AsString;
	StockLocationDetails.Location				= sqlStockDetails->FieldByName("Location")->AsString;
	StockLocationDetails.GST_Percent			= sqlStockDetails->FieldByName("GST_Percent")->AsDouble;
	StockLocationDetails.Stock_Category		= sqlStockDetails->FieldByName("Stock_Category")->AsString;
	StockLocationDetails.Stock_Group			= sqlStockDetails->FieldByName("Stock_Group")->AsString;
	StockLocationDetails.GL_Code				= sqlStockDetails->FieldByName("GL_Code")->AsString;
	StockLocationDetails.Stocktake_Unit		= sqlStockDetails->FieldByName("Stocktake_Unit")->AsString;
	StockLocationDetails.Average_Cost		= sqlStockDetails->FieldByName("Average_Cost")->AsDouble;
	StockLocationDetails.Latest_Cost			= sqlStockDetails->FieldByName("Latest_Cost")->AsDouble;
	StockLocationDetails.On_Hand				= sqlStockDetails->FieldByName("On_Hand")->AsDouble;
//	StockLocationDetails.On_Order				= sqlStockDetails->FieldByName("On_Order")->AsDouble;

	StockLocationDetails.Writeoffs_Pending	= sqlStockDetails->FieldByName("Writeoffs_Pending")->AsDouble;
	StockLocationDetails.Sales_Pending		= sqlStockDetails->FieldByName("Sales_Pending")->AsDouble;

	StockLocationDetails.Last_Stocktake		= sqlStockDetails->FieldByName("Last_Stocktake")->AsDateTime;
	StockLocationDetails.Transfer				= sqlStockDetails->FieldByName("Transfer")->AsDouble;
	StockLocationDetails.Inwards				= sqlStockDetails->FieldByName("Inwards")->AsDouble;
	StockLocationDetails.Opening				= sqlStockDetails->FieldByName("Opening")->AsDouble;
	StockLocationDetails.Writeoff				= sqlStockDetails->FieldByName("Writeoff")->AsDouble;
	StockLocationDetails.Sales					= sqlStockDetails->FieldByName("Sales")->AsDouble;
	StockLocationDetails.Stocktake			= sqlStockDetails->FieldByName("Stocktake")->AsDouble;
   }
//---------------------------------------------------------------------------
bool TStockTransaction::fCreateBatch(TTransactionBatchInfo& BatchInfo)
{
	BatchInfo.BatchID = fDBTransaction.GetGeneratorVal("Gen_Stocktrans_Batch_Key");
	if (BatchInfo.BatchID)
	{
		sqlCreateBatch->Close();
		sqlCreateBatch->ParamByName("Batch_Key")->AsInteger = BatchInfo.BatchID;
		switch (BatchInfo.BatchType)
		{
			case ttTransfer:	sqlCreateBatch->ParamByName("Transaction_Type")->AsString = "Transfer";		break;
			case ttReceipt:	sqlCreateBatch->ParamByName("Transaction_Type")->AsString = "Receipt";		break;
			case ttSale:		sqlCreateBatch->ParamByName("Transaction_Type")->AsString = "Sale";			break;
			case ttStocktake:	sqlCreateBatch->ParamByName("Transaction_Type")->AsString = "Stocktake";	break;
		}
		sqlCreateBatch->ParamByName("User_ID")->AsString			= BatchInfo.User_ID;
		sqlCreateBatch->ParamByName("User_Name")->AsString			= BatchInfo.User_Name;
		sqlCreateBatch->ParamByName("Created")->AsDateTime			= BatchInfo.Created;
		sqlCreateBatch->ParamByName("Reference")->AsString			= BatchInfo.Reference;
		sqlCreateBatch->ParamByName("Supplier_Key")->AsString		= BatchInfo.Supplier_Key;
		sqlCreateBatch->ParamByName("Supplier_Name")->AsString	= BatchInfo.Supplier_Name;
		sqlCreateBatch->ParamByName("Supplier_ID")->AsString		= BatchInfo.Supplier_ID;
		sqlCreateBatch->ParamByName("Export_1")->AsString			= BatchInfo.Export_1;
		sqlCreateBatch->ParamByName("Export_2")->AsString			= BatchInfo.Export_2;
		sqlCreateBatch->ParamByName("Export_3")->AsString			= BatchInfo.Export_3;
		sqlCreateBatch->ParamByName("Order_Number")->AsString		= BatchInfo.Order_Number;
		sqlCreateBatch->ParamByName("Total_Cost")->AsDouble		= BatchInfo.Total_Cost;
		sqlCreateBatch->ParamByName("Total_GST")->AsDouble			= BatchInfo.Total_GST;
		sqlCreateBatch->ParamByName("Closed")->AsString				= BatchInfo.Closed?"T":"F";
		sqlCreateBatch->ParamByName("Exported")->AsString			= BatchInfo.Exported?"T":"F";
		sqlCreateBatch->ExecQuery();
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockTransaction::fUpdateBatch(TTransactionBatchInfo& BatchInfo)
{
	if (BatchInfo.BatchID)
	{
		sqlUpdateBatch->Close();
		sqlUpdateBatch->ParamByName("Batch_Key")->AsInteger 		= BatchInfo.BatchID;
		sqlUpdateBatch->ParamByName("Total_Cost")->AsDouble		= BatchInfo.Total_Cost;
		sqlUpdateBatch->ParamByName("Total_GST")->AsDouble			= BatchInfo.Total_GST;
		sqlUpdateBatch->ExecQuery();
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TStockTransaction::fAddBatchTransaction(TTransactionInfo const& TransactionInfo)
{
	sqlCreateTransaction->Close();
	sqlCreateTransaction->ParamByName("Trans_Number")->AsInteger		= fDBTransaction.GetGeneratorVal("Gen_Stocktrans");
	sqlCreateTransaction->ParamByName("Batch_Key")->AsInteger			= TransactionInfo.BatchInfo.BatchID;
	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString	= TransactionInfo.BatchInfo.BatchType;

	switch (TransactionInfo.TransactionType)//BatchInfo.BatchType)
	{
		case ttTransfer:	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "Transfer";		break;
		case ttReceipt:	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "Receipt";		break;
		case ttSale:		sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "Sale";			break;
		case ttWriteoff:	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "WriteOff";		break;
        case ttRefund:	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "Refund";		break;
		case ttStocktake:	sqlCreateTransaction->ParamByName("Transaction_Type")->AsString = "Stocktake";	break;
	}
	sqlCreateTransaction->ParamByName("Code")->AsString				= TransactionInfo.StockCode;
	sqlCreateTransaction->ParamByName("Description")->AsString		= TransactionInfo.Description;
	sqlCreateTransaction->ParamByName("GST_Percent")->AsDouble		= TransactionInfo.GST_Percent;
	sqlCreateTransaction->ParamByName("Total_GST")->AsDouble			= TransactionInfo.Total_GST;
	sqlCreateTransaction->ParamByName("Stock_Category")->AsString	= TransactionInfo.Stock_Category;
	sqlCreateTransaction->ParamByName("Stock_Group")->AsString		= TransactionInfo.Stock_Group;
	sqlCreateTransaction->ParamByName("GL_Code")->AsString			= TransactionInfo.GL_Code;
	sqlCreateTransaction->ParamByName("Qty")->AsDouble					= TransactionInfo.Qty;
	sqlCreateTransaction->ParamByName("Unit")->AsString				= TransactionInfo.Unit;
	sqlCreateTransaction->ParamByName("Unit_Cost")->AsCurrency		= TransactionInfo.Unit_Cost;
	sqlCreateTransaction->ParamByName("Total_Cost")->AsCurrency		= TransactionInfo.Total_Cost; //cww
	sqlCreateTransaction->ParamByName("Location")->AsString			= TransactionInfo.Location;
	sqlCreateTransaction->ParamByName("Purchaser_Name")->AsString	= TransactionInfo.Purchaser_Name;

	sqlCreateTransaction->ParamByName("Order_Unit")->AsString		= TransactionInfo.Order_Unit;
	sqlCreateTransaction->ParamByName("Order_Qty")->AsDouble			= TransactionInfo.Order_Qty;
	sqlCreateTransaction->ParamByName("Supplier_Code")->AsString	= TransactionInfo.Supplier_Code;

	sqlCreateTransaction->ParamByName("Supplier_Key")->AsInteger	= TransactionInfo.BatchInfo.Supplier_Key;
	sqlCreateTransaction->ParamByName("Supplier_Name")->AsString	= TransactionInfo.BatchInfo.Supplier_Name;
//	sqlCreateTransaction->ParamByName("Supplier_ID")->AsString		= TransactionInfo.BatchInfo.Supplier_ID;
//	sqlCreateTransaction->ParamByName("Export_1")->AsString			= TransactionInfo.BatchInfo.Export_1;
//	sqlCreateTransaction->ParamByName("Export_2")->AsString			= TransactionInfo.BatchInfo.Export_2;
//	sqlCreateTransaction->ParamByName("Export_3")->AsString			= TransactionInfo.BatchInfo.Export_3;
	sqlCreateTransaction->ParamByName("Order_Number")->AsString		= TransactionInfo.BatchInfo.Order_Number;
	sqlCreateTransaction->ParamByName("Reference")->AsString			= TransactionInfo.BatchInfo.Reference;

	sqlCreateTransaction->ParamByName("User_ID")->AsString			= TransactionInfo.BatchInfo.User_ID;

//	sqlCreateTransaction->ParamByName("Order_Number")->AsString		= TransactionInfo.BatchInfo.Order_Number;
//	sqlCreateTransaction->ParamByName("Supplier_Key")->AsInteger	= TransactionInfo.BatchInfo.Supplier_Key;
//	sqlCreateTransaction->ParamByName("Supplier_Name")->AsString	= TransactionInfo.BatchInfo.Supplier_Name;

	sqlCreateTransaction->ParamByName("Created")->AsDateTime			= TransactionInfo.BatchInfo.Created;
	sqlCreateTransaction->ParamByName("User_Name")->AsString			= TransactionInfo.Operator;
	sqlCreateTransaction->ParamByName("Note")->AsString			= TransactionInfo.Note;
	sqlCreateTransaction->ExecQuery();
}
//---------------------------------------------------------------------------
void TStockTransaction::fSetStockParams(int StockKey, AnsiString Location, TTransactionType TransactionType,
		 Currency AverageCost, Currency LatestCost,
		 double Qty)
{
	sqlAdjustStock->Close();
	sqlAdjustStock->SQL->Text =
		"Update "
			"StockLocation "
		"Set ";

	switch (TransactionType)
	{
		case ttTransfer:	sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Transfer = Transfer + :Quantity, ";	break;
		case ttReceipt:	sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Inwards = Inwards + :Quantity, ";	break;
		case ttSale:		sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Sales = Sales + :Quantity, ";			break;
		case ttWriteoff:   sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Writeoff = Writeoff + :Quantity, ";	break;

        case ttRefund:       sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Sales = Sales + :Quantity, ";			break;
        //sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text + "Refund = Refund + :Quantity, "; break;


	}
	sqlAdjustStock->SQL->Text = sqlAdjustStock->SQL->Text +
			"Average_Cost = :Average_Cost, "
			"Latest_Cost = :Latest_Cost, "
			"On_Hand = On_Hand + :Quantity "
		"Where "
			"Stock_Key = :Stock_Key And "
			"Location = :Location";
	sqlAdjustStock->Close();
	sqlAdjustStock->ParamByName("Stock_Key")->AsInteger	= StockKey;
	sqlAdjustStock->ParamByName("Location")->AsString		= Location;

	sqlAdjustStock->ParamByName("Average_Cost")->AsDouble	= AverageCost;
	sqlAdjustStock->ParamByName("Latest_Cost")->AsDouble	= LatestCost;

	sqlAdjustStock->ParamByName("Quantity")->AsDouble		= Qty;
  //  sqlAdjustStock->ParamByName("Refund")->AsDouble		= Qty;
	sqlAdjustStock->ExecQuery();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TReceiveInvoice::TReceiveInvoice(TIBDatabase *IBDatabase) : TStockTransaction(IBDatabase),
	sqlAdjustSupplierStock(	new TIBSQL(NULL)),
	sqlAdjustPurchaseStock(	new TIBSQL(NULL))
{
	fDBTransaction.RegisterQuery(sqlAdjustSupplierStock);
	fDBTransaction.RegisterQuery(sqlAdjustPurchaseStock);

	sqlAdjustPurchaseStock->SQL->Text =
		"Update "
			"PurchaseStock "
		"Set "
			"Qty_Received = Qty_Received + :QtyReceivedDelta,"
//			"Qty_On_Order = Qty_On_Order - :QtyBackOrderDelta "
			"Qty_On_Order = :Qty_On_Order "
		"Where "
			"Purchase_Stock_LK = :Purchase_Stock_Key";
}
//---------------------------------------------------------------------------
bool TReceiveInvoice::ReceiveInvoice(TTransactionBatchInfo& BatchInfo, TcInvoiceItemInfo const& InvoiceItems)
{
	fDBTransaction.StartTransaction();
	try
	{
		if (fCreateBatch(BatchInfo))
		{
			bool Success = true;
			TiInvoiceItemInfo iInvoiceItems;
			for (iInvoiceItems = InvoiceItems.begin(); iInvoiceItems != InvoiceItems.end(); iInvoiceItems++)
			{
				if (!fReceiveInvoiceItem(BatchInfo, iInvoiceItems))
				{
					Success = false;
					break;
				}
			}
			if (Success)
			{
				fDBTransaction.CommitTransaction();
			}
			else
			{
				fDBTransaction.UndoTransaction();
			}
			return Success;
		}
		else
		{
			fDBTransaction.UndoTransaction();
			return false;
		}
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		throw;
		//return false;
	}
}
//---------------------------------------------------------------------------
bool TReceiveInvoice::fReceiveInvoiceItem(TTransactionBatchInfo const& BatchInfo,
		TiInvoiceItemInfo const &InvoiceItemInfo)
{
	bool Success = false;
	if (InvoiceItemInfo->Supplier_Unit_Size != 0)
	{
		TStockLocationDetails StockDetails;
		if (fGetStockDetails(InvoiceItemInfo->Stock_Key, InvoiceItemInfo->Location, StockDetails))
		{
			// Create the generic transaction for the stocktrans table
			TTransactionInfo TransactionInfo(ttReceipt, BatchInfo);

			TransactionInfo.StockCode			= StockDetails.StockCode;
			TransactionInfo.Description		= StockDetails.Description;
			TransactionInfo.Stock_Category	= StockDetails.Stock_Category;
			TransactionInfo.Stock_Group		= StockDetails.Stock_Group;
			TransactionInfo.GL_Code				= StockDetails.GL_Code;
			TransactionInfo.Location			= InvoiceItemInfo->Location;
			TransactionInfo.Unit					= StockDetails.Stocktake_Unit;
			TransactionInfo.Unit_Cost			= InvoiceItemInfo->Supplier_Unit_Cost / InvoiceItemInfo->Supplier_Unit_Size;
			TransactionInfo.Total_Cost			= InvoiceItemInfo->Total_Cost;
			TransactionInfo.GST_Percent		= StockDetails.GST_Percent;
			TransactionInfo.Total_GST			= InvoiceItemInfo->Total_GST;
			TransactionInfo.Qty					= InvoiceItemInfo->Order_Qty * InvoiceItemInfo->Supplier_Unit_Size;
			TransactionInfo.Supplier_Code		= InvoiceItemInfo->Supplier_Code;
			TransactionInfo.Order_Unit			= InvoiceItemInfo->Order_Unit;
			TransactionInfo.Order_Qty			= InvoiceItemInfo->Order_Qty;

			if (InvoiceItemInfo->Order_Qty != 0)
			{
				fAddBatchTransaction(TransactionInfo);
			}

			// Calculate the new Latest and average costs for the stock item.
			// If receiving a -ve qty, average costing cannot be undone. Just leave as is.
			// Otherwise, if reciving an item that CURRENTLY has no stock on hand or $0.00 cost,
			// set to new items cost.
			// Otherwise calculate a weighted average based on total value of items in stock and
			// total value of stock received.
			double NewOnHand	= StockDetails.On_Hand + TransactionInfo.Qty;

			Currency NewAverage;
			Currency NewLatestCost;
			if (TransactionInfo.Qty <= 0)
			{
				NewLatestCost	= StockDetails.Latest_Cost;
				NewAverage		= StockDetails.Average_Cost;
			}
			else
			{
				NewLatestCost	= TransactionInfo.Unit_Cost;
				if (StockDetails.Average_Cost == 0 || StockDetails.On_Hand <= 0 || NewOnHand <= 0)
				{
					NewAverage	= TransactionInfo.Unit_Cost;
				}
				else
				{
					NewAverage	= ((StockDetails.Average_Cost * StockDetails.On_Hand) +
										(double(InvoiceItemInfo->Supplier_Unit_Cost) * double(TransactionInfo.Order_Qty))) / NewOnHand;
				}
			}
			// Update the Order item. Set it's backorder and received.
			if (InvoiceItemInfo->Purchase_Stock_Key)
			{
				sqlAdjustPurchaseStock->Close();
				sqlAdjustPurchaseStock->ParamByName("Purchase_Stock_Key")->AsInteger	= InvoiceItemInfo->Purchase_Stock_Key;
				sqlAdjustPurchaseStock->ParamByName("QtyReceivedDelta")->AsDouble		= TransactionInfo.Order_Qty;
				sqlAdjustPurchaseStock->ParamByName("Qty_On_Order")->AsDouble			= InvoiceItemInfo->BackOrder;
				sqlAdjustPurchaseStock->ExecQuery();
			}
			// ...and update the stock item, setting costs and on_hand.
			if (InvoiceItemInfo->Order_Qty != 0)
			{
				fSetStockParams(
						InvoiceItemInfo->Stock_Key,
						TransactionInfo.Location,
						ttReceipt,
						NewAverage,
						NewLatestCost,
						TransactionInfo.Qty);

				// Update the latest cost of the item supplied by the supplier if details are the same.
				if (BatchInfo.Supplier_Key)
				{
					fSetSupplierStockCost(
							InvoiceItemInfo->Stock_Key,
							BatchInfo.Supplier_Key,
							TransactionInfo.Order_Unit,
							InvoiceItemInfo->Supplier_Code,
							InvoiceItemInfo->Supplier_Unit_Size,
							InvoiceItemInfo->Supplier_Unit_Cost);
				}
				// Update the purchase order levels
			}
			Success = true;
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
void TReceiveInvoice::fSetSupplierStockCost(int StockKey, int SupplierKey, AnsiString SupplierUnit,
				AnsiString SupplierCode, double SupplierUnitSize, Currency SupplierUnitCost)
{
	sqlAdjustSupplierStock->Close();
	if (SupplierCode == "")
	{
		sqlAdjustSupplierStock->SQL->Text =
			"Update "
				"SupplierStock "
			"Set "
				"Latest_Cost = :Latest_Cost "
			"Where "
				"Stock_Key = :Stock_Key and "
				"Supplier_Key = :Supplier_Key and "
				"Supplier_Unit = :Supplier_Unit and "
				"(Supplier_Code = :Supplier_Code or Supplier_Code is NULL) and "
				"Qty = :Supplier_Unit_Size";
	}
	else
	{
		sqlAdjustSupplierStock->SQL->Text =
			"Update "
				"SupplierStock "
			"Set "
				"Latest_Cost = :Latest_Cost "
			"Where "
				"Stock_Key = :Stock_Key and "
				"Supplier_Key = :Supplier_Key and "
				"Supplier_Unit = :Supplier_Unit and "
				"Supplier_Code = :Supplier_Code and "
				"Qty = :Supplier_Unit_Size";
	}
	sqlAdjustSupplierStock->ParamByName("Latest_Cost")->AsDouble			= SupplierUnitCost;
	sqlAdjustSupplierStock->ParamByName("Stock_Key")->AsInteger				= StockKey;
	sqlAdjustSupplierStock->ParamByName("Supplier_Key")->AsInteger			= SupplierKey;
	sqlAdjustSupplierStock->ParamByName("Supplier_Unit")->AsString			= SupplierUnit;
	sqlAdjustSupplierStock->ParamByName("Supplier_Code")->AsString			= SupplierCode;
	sqlAdjustSupplierStock->ParamByName("Supplier_Unit_Size")->AsDouble	= SupplierUnitSize;
	sqlAdjustSupplierStock->ExecQuery();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TTransferStock::TTransferStock(TIBDatabase *IBDatabase) : TStockTransaction(IBDatabase)
{
}
//---------------------------------------------------------------------------
bool TTransferStock::TransferStock(TTransactionBatchInfo& BatchInfo, TcTransferInfo const& Transfers)
{
	fDBTransaction.StartTransaction();
	try
	{
		if (fCreateBatch(BatchInfo))
		{
			TiTransferInfo iTransfers;
			for (iTransfers = Transfers.begin(); iTransfers != Transfers.end(); iTransfers++)
			{
				fTransferStock(BatchInfo, iTransfers);
			}
			fDBTransaction.CommitTransaction();
		}
		else
		{
			fDBTransaction.UndoTransaction();
			return false;
		}
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void TTransferStock::fTransferStock(TTransactionBatchInfo const& BatchInfo, TiTransferInfo const &TransferInfo)
{
	AnsiString Source			= TransferInfo->Source;
	AnsiString Destination	= TransferInfo->Destination;
	double Quantity			= TransferInfo->Quantity;
	if (Quantity < 0)
	{
		// Turn the transfer around if transferring -ve stock. Makes sure averages work out.
		AnsiString Temp	= Destination;
		Destination			= Source;
		Source				= Temp;
		Quantity				= -Quantity;
	}
	TStockLocationDetails SourceDetails;
	if (fGetStockDetails(TransferInfo->StockKey, Source, SourceDetails))
	{
		double		DestOnHand;
		Currency		DestAverage;
		Currency		NewAverage;

		TStockLocationDetails DestDetails;
		if (!fGetStockDetails(TransferInfo->StockKey, Destination, DestDetails))
		{
			fCreateStockLocation(TransferInfo->StockKey, Destination);
			DestOnHand		= 0;
			DestAverage		= 0;
		}
		else
		{
			DestOnHand		= DestDetails.On_Hand;
			DestAverage		= DestDetails.Average_Cost;
		}
		// Destination doesn't have a usable average. Just make it the same as the source.
		if (DestAverage == 0 || DestOnHand <= 0 || (DestOnHand + Quantity) == 0)
		{
			NewAverage = SourceDetails.Average_Cost;
		}
		else
		{
			// Calculate new averages.
			NewAverage = ((SourceDetails.Average_Cost * Quantity) + (DestAverage * DestOnHand)) / (DestOnHand + Quantity);
		}
		TTransactionInfo TransactionInfo(ttTransfer, BatchInfo);

		TransactionInfo.StockCode			= SourceDetails.StockCode;
		TransactionInfo.Description		= SourceDetails.Description;
		TransactionInfo.Stock_Category	= SourceDetails.Stock_Category;
		TransactionInfo.Stock_Group		= SourceDetails.Stock_Group;
		TransactionInfo.GL_Code				= SourceDetails.GL_Code;
		TransactionInfo.Location			= Source;
		TransactionInfo.Unit					= SourceDetails.Stocktake_Unit;
		TransactionInfo.Unit_Cost			= SourceDetails.Average_Cost;
		TransactionInfo.GST_Percent		= SourceDetails.GST_Percent;
		TransactionInfo.Total_GST			= -Quantity * SourceDetails.GST_Percent * SourceDetails.Average_Cost / 100;
		TransactionInfo.Qty					= -Quantity;
		TransactionInfo.Total_Cost			= SourceDetails.Average_Cost * -Quantity;
		TransactionInfo.Purchaser_Name	= Destination;

		TransactionInfo.BatchInfo.Reference	= TransactionInfo.Purchaser_Name;

		fAddBatchTransaction(TransactionInfo);
		
		// Add the stock going the other way.
		TransactionInfo.Total_GST			= Quantity * SourceDetails.GST_Percent * SourceDetails.Average_Cost / 100;
		TransactionInfo.Qty					= Quantity;
		TransactionInfo.Total_Cost			= SourceDetails.Average_Cost * Quantity;
		TransactionInfo.Location			= Destination;
		TransactionInfo.Purchaser_Name	= Source;

		TransactionInfo.BatchInfo.Reference	= TransactionInfo.Purchaser_Name;

		fAddBatchTransaction(TransactionInfo);

		//Update Stock Levels
		fSetStockParams(	TransferInfo->StockKey, Source, ttTransfer,
								SourceDetails.Average_Cost, SourceDetails.Latest_Cost,
								-Quantity);//, 0);

		fSetStockParams(	TransferInfo->StockKey, Destination, ttTransfer,
								NewAverage, SourceDetails.Latest_Cost,
								Quantity);//, 0);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TStocktakeControl::TStocktakeControl(TIBDatabase *IBDatabase) : TStockTransaction(IBDatabase),
			sqlStocktakeHistory(		new TIBSQL(NULL)),
			sqlUpdateStockLocation(	new TIBSQL(NULL))
{
	fDBTransaction.RegisterQuery(sqlStocktakeHistory);
	fDBTransaction.RegisterQuery(sqlUpdateStockLocation);

	sqlStocktakeHistory->SQL->Text =
		"Insert Into StocktakeHistory ("
			"StocktakeHistory_Key,"
			"Batch_Key,"
			"Stocktake_Key,"
			"Code,"
			"Location,"
			"Average_Unit_Cost,"
			"Latest_Unit_Cost,"
			"Stock_Category,"
			"Stock_Group,"
			"Description,"
			"Last_Stocktake,"
			"Opening,"
			"Inwards,"
			"Writeoff,"
			"Transfer,"
			"Sales,"
			"Closing,"
			"Writeoffs_Pending,"
			"Sales_Pending,"
			"Stocktake_Unit  )"
		"Values ("
			":StocktakeHistory_Key,"
			":Batch_Key,"
			":Stocktake_Key,"
			":Code,"
			":Location,"
			":Average_Unit_Cost,"
			":Latest_Unit_Cost,"
			":Stock_Category,"
			":Stock_Group,"
			":Description,"
			":Last_Stocktake,"
			":Opening,"
			":Inwards,"
			":Writeoff,"
			":Transfer,"
			":Sales,"
			":Closing,"
			":Writeoffs_Pending,"
			":Sales_Pending,"
			":Stocktake_Unit ) ";

	sqlUpdateStockLocation->SQL->Text =
		"Update "
			"StockLocation "
		"Set "
		  "On_Hand           = :Counted + Sales_Pending + Writeoffs_Pending  ,"
		  "Initialised       = 'F',"
		  "Stocktake_Key     = Null,"
		  "Stocktake         = Null,"
		  "Variance          = Null,"
		  "Opening           = :Counted,"
		  "Last_Stocktake    = :Last_Stocktake,"
		  "Inwards           = 0,"
		  "Writeoff          = Writeoffs_Pending,"
		  "Transfer          = 0,"
		  "Sales             = Sales_Pending,"
		  "Writeoffs_Pending = 0,"
		  "Sales_Pending     = 0  "
		"Where "
		  "Stock_Key         = :StockKey And "
		  "Location          = :Location ";
}
//---------------------------------------------------------------------------
void TStocktakeControl::EnumStocktakes(TStocktakeList &StocktakeList, bool Uninitialised, bool Initialised, bool Committed)
{
	StocktakeList.clear();

	Database::TcpIBSQL sqlEnumStocktakes (new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlEnumStocktakes);
	fDBTransaction.StartTransaction();
	try
	{
		sqlEnumStocktakes->Close();
		std::vector<AnsiString> Filters;

		if (Initialised)
		{
			Filters.push_back(" (Initialised_Time Is Not Null And Committed_Time Is Null)");
		}
		if (Committed)
		{
			Filters.push_back(" Committed_Time Is Not Null");
		}
		if (Uninitialised)
		{
			Filters.push_back(" Initialised_Time Is Null");
		}
		sqlEnumStocktakes->SQL->Text = "Select * From Stocktake Where ";//Initialised_Time Is Null Order By Initialised_Time";
		for (unsigned i=0; i<Filters.size(); i++)
		{
			sqlEnumStocktakes->SQL->Text = sqlEnumStocktakes->SQL->Text + Filters[i];
			if (i!=Filters.size()-1)
			{
				sqlEnumStocktakes->SQL->Text = sqlEnumStocktakes->SQL->Text + " Or";
			}
		}
		sqlEnumStocktakes->SQL->Text = sqlEnumStocktakes->SQL->Text + " Order By Initialised_Time";

		for (sqlEnumStocktakes->ExecQuery(); !sqlEnumStocktakes->Eof; sqlEnumStocktakes->Next())
		{
			TStocktakeDetails StocktakeDetails;

			StocktakeDetails.StocktakeKey		= sqlEnumStocktakes->FieldByName("Stocktake_Key")->AsInteger;
			StocktakeDetails.BatchKey			= sqlEnumStocktakes->FieldByName("Batch_Key")->AsInteger;
			StocktakeDetails.Location			= sqlEnumStocktakes->FieldByName("Location")->AsString;
			StocktakeDetails.Name				= sqlEnumStocktakes->FieldByName("Name")->AsString;

			StocktakeDetails.Initialised		= !sqlEnumStocktakes->FieldByName("Initialised_Time")->IsNull;
			StocktakeDetails.InitialisedTime	= sqlEnumStocktakes->FieldByName("Initialised_Time")->AsDateTime;
			StocktakeDetails.InitialisedBy	= sqlEnumStocktakes->FieldByName("Initialised_User_Name")->AsString;

			StocktakeDetails.Committed			= !sqlEnumStocktakes->FieldByName("Committed_Time")->IsNull;
			StocktakeDetails.CommittedTime	= sqlEnumStocktakes->FieldByName("Committed_Time")->AsDateTime;
			StocktakeDetails.CommittedBy		= sqlEnumStocktakes->FieldByName("Committed_User_Name")->AsString;

			StocktakeList.push_back(StocktakeDetails);
		}
		fDBTransaction.CommitTransaction();
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
/*void TStocktakeControl::GetStocktake(int StocktakeKey, TStocktakeDetails &StocktakeInfo)
{
}*/
//---------------------------------------------------------------------------
int TStocktakeControl::CreateStocktake(const TStocktakeProfiles &StocktakeProfiles, const AnsiString &Name, const AnsiString &Location)
{
	Database::TcpIBSQL sqlAddStocktake(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlAddStocktake);
	sqlAddStocktake->SQL->Text =
		"Insert Into Stocktake ("
			"Stocktake_Key,"
			"Name,"
			"Location) "
		"Values ("
			":Stocktake_Key,"
			":Name,"
			":Location)";

	Database::TcpIBSQL sqlLocationStocktake(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlLocationStocktake);
	sqlLocationStocktake->SQL->Text =
		"Update "
			"StockLocation "
		"Set "
			"StockLocation.Stocktake_Key = :Stocktake_Key "
		"Where "
			"StockLocation.Location = :Location And "
			"StockLocation.Stocktake_Key Is Null And "
			"StockLocation.Stock_Key In "
			"( "
				"Select "
					"Stock.Stock_Key "
				"From "
					"Stock "
				"Where "
					"Stock.Stock_Group_Key = :Stock_Group_Key And "
					"Stock.Deleted = 'F' Or Stock.Deleted Is Null "
			")";

	fDBTransaction.StartTransaction();
	try
	{
		int StocktakeKey														= fDBTransaction.GetGeneratorVal("Gen_Stocktake_Key");

		sqlAddStocktake->ParamByName("Stocktake_Key")->AsInteger	= StocktakeKey;
		sqlAddStocktake->ParamByName("Name")->AsString				= Name;
		sqlAddStocktake->ParamByName("Location")->AsString			= Location;
		sqlAddStocktake->ExecQuery();

		for (unsigned int i=0; i<StocktakeProfiles.size(); i++)
		{
			for (unsigned int j=0; j<StocktakeProfiles[i].StockGroups.size(); j++)
			{
				sqlLocationStocktake->Close();
				sqlLocationStocktake->ParamByName("Stocktake_Key")->AsInteger		= StocktakeKey;
				sqlLocationStocktake->ParamByName("Location")->AsString				= Location;
				sqlLocationStocktake->ParamByName("Stock_Group_Key")->AsInteger	= StocktakeProfiles[i].StockGroups[j];
				sqlLocationStocktake->ExecQuery();
			}
		}
		fDBTransaction.CommitTransaction();
		return StocktakeKey;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return 0;
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::Initialise(int StocktakeKey, AnsiString UserID, AnsiString UserName, bool CopyOnHand)
{
	Database::TcpIBSQL sqlInitialise(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlInitialise);
	fDBTransaction.StartTransaction();
	try
	{
		sqlInitialise->Close();
		sqlInitialise->SQL->Text =
			"Update "
				"StockLocation "
			"Set ";
		if (CopyOnHand)
		{
			sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text + "StockLocation.Stocktake = StockLocation.On_Hand," +
																					"StockLocation.Variance  = 0,";
		}
		else
		{
			sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text + "StockLocation.Stocktake = 0," +
																					"StockLocation.Variance  = -StockLocation.On_Hand,";
		}
		sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text +
				"StockLocation.Initialised = 'T' "
			"Where "
				"StockLocation.Stocktake_Key = :Stocktake_Key And "
				"(StockLocation.Initialised = 'F' Or StockLocation.Initialised Is Null)";

		sqlInitialise->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
		sqlInitialise->ExecQuery();

		sqlInitialise->Close();
		sqlInitialise->SQL->Text =
			"Update "
				"Stocktake "
			"Set "
				"Initialised_Time = :Initialised_Time,"
				"Initialised_User_ID = :Initialised_User_ID,"
				"Initialised_User_Name = :Initialised_User_Name "
			"Where "
				"Stocktake_Key = :Stocktake_Key And "
				"Initialised_Time Is Null";
		sqlInitialise->ParamByName("Stocktake_Key")->AsInteger			= StocktakeKey;
		sqlInitialise->ParamByName("Initialised_Time")->AsDateTime		= Now();
		sqlInitialise->ParamByName("Initialised_User_ID")->AsString		= UserID;
		sqlInitialise->ParamByName("Initialised_User_Name")->AsString	= UserName;
		sqlInitialise->ExecQuery();

		bool Succes = sqlInitialise->RowsAffected > 0;

		fDBTransaction.CommitTransaction();
		return Succes;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
void TStocktakeControl::ResetInitialisation(int StocktakeKey, bool CopyOnHand)
{
	Database::TcpIBSQL sqlInitialise(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlInitialise);
	fDBTransaction.StartTransaction();
	try
	{
		sqlInitialise->Close();
		sqlInitialise->SQL->Text =
			"Update "
				"StockLocation "
			"Set ";
		if (CopyOnHand)
		{
			sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text + "StockLocation.Stocktake = StockLocation.On_Hand," +
																					"StockLocation.Variance  = 0 ";
		}
		else
		{
			sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text + "StockLocation.Stocktake = 0," +
																					"StockLocation.Variance  = -StockLocation.On_Hand ";
		}
		sqlInitialise->SQL->Text =	sqlInitialise->SQL->Text +
			"Where "
				"StockLocation.Stocktake_Key = :Stocktake_Key And "
				"StockLocation.Initialised = 'T'";

		sqlInitialise->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
		sqlInitialise->ExecQuery();

		fDBTransaction.CommitTransaction();
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::SetCount(int StockKey, AnsiString Location, Currency Count, Currency OnHand, Currency &Variance)
{
	Database::TcpIBSQL sqlUpdateStocktake(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlUpdateStocktake);
	sqlUpdateStocktake->SQL->Text =
		"Update "
			"StockLocation "
		"Set "
			"Stocktake = :Stocktake,"
			"Variance = :Variance "
		"Where "
			"Stock_Key = :Stock_Key And "
			"Location = :Location";
	fDBTransaction.StartTransaction();
	try
	{
		sqlUpdateStocktake->Close();
		sqlUpdateStocktake->ParamByName("Location")->AsString			= Location;
		sqlUpdateStocktake->ParamByName("Stock_Key")->AsInteger		= StockKey;
		sqlUpdateStocktake->ParamByName("Stocktake")->AsCurrency		= Count;
		sqlUpdateStocktake->ParamByName("Variance")->AsCurrency		= Count - OnHand;
		sqlUpdateStocktake->ExecQuery();

		fDBTransaction.CommitTransaction();

		Variance = Count - OnHand;

		return sqlUpdateStocktake->RowsAffected > 0;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::DeleteStocktake(int StocktakeKey)
{
	fDBTransaction.StartTransaction();
	try
	{
		fCancelInitialisation(StocktakeKey);

		Database::TcpIBSQL sqlDeleteStocktake(new TIBSQL(NULL));
		fDBTransaction.RegisterQuery(sqlDeleteStocktake);
		sqlDeleteStocktake->SQL->Text =
			"Delete From "
				"Stocktake "
			"Where "
				"Stocktake_Key = :Stocktake_Key And "
				"Committed_Time Is Null";
		sqlDeleteStocktake->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
		sqlDeleteStocktake->ExecQuery();

		if (sqlDeleteStocktake->RowsAffected > 0)
		{
			sqlDeleteStocktake->Close();
			sqlDeleteStocktake->SQL->Text =
				"Update "
					"StockLocation "
				"Set "
				  "Stocktake_Key = Null "
				"Where "
				  "Stocktake_Key = :Stocktake_Key";
			sqlDeleteStocktake->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
			sqlDeleteStocktake->ExecQuery();

			fDBTransaction.CommitTransaction();
			return true;
		}
		else
		{
			fDBTransaction.UndoTransaction();
			return false;
		}
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::CommitStocktake(TTransactionBatchInfo& BatchInfo, int StocktakeKey, TcStocktakeItemInfo const& StocktakeItems)
{
	fDBTransaction.StartTransaction();
	try
	{
		if (fCreateBatch(BatchInfo))
		{
			TiStocktakeItemInfo iStocktakeItems;
			for (iStocktakeItems = StocktakeItems.begin(); iStocktakeItems != StocktakeItems.end(); iStocktakeItems++)
			{
				fUpdateStocktakeItem(BatchInfo, StocktakeKey, iStocktakeItems);
			}
			Database::TcpIBSQL sqlUpdateStocktake(new TIBSQL(NULL));
			fDBTransaction.RegisterQuery(sqlUpdateStocktake);
			sqlUpdateStocktake->SQL->Text =
				"Update "
					"Stocktake "
				"Set "
					"Batch_Key = :Batch_Key,"
					"Committed_Time = :Committed_Time,"
					"Committed_User_ID = :Committed_User_ID,"
					"Committed_User_Name = :Committed_User_Name "
				"Where "
					"Stocktake_Key = :Stocktake_Key";
			sqlUpdateStocktake->ParamByName("Stocktake_Key")->AsInteger			= StocktakeKey;
			sqlUpdateStocktake->ParamByName("Batch_Key")->AsInteger				= BatchInfo.BatchID;
			sqlUpdateStocktake->ParamByName("Committed_Time")->AsDateTime		= Now();
			sqlUpdateStocktake->ParamByName("Committed_User_ID")->AsString		= BatchInfo.User_ID;
			sqlUpdateStocktake->ParamByName("Committed_User_Name")->AsString	= BatchInfo.User_Name;
			sqlUpdateStocktake->ExecQuery();

			fDBTransaction.CommitTransaction();
			return true;
		}
		else
		{
			fDBTransaction.UndoTransaction();
			return false;
		}
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::CancelInitialisation(int StocktakeKey)
{
	fDBTransaction.StartTransaction();
	try
	{
		fCancelInitialisation(StocktakeKey);
		fDBTransaction.CommitTransaction();
		return true;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStocktakeControl::CreateStockSCV(int StocktakeKey, AnsiString FileName)
{
	fDBTransaction.StartTransaction();
	try
	{
		TCsv ExportCSV;
		ExportCSV.Delimiter = ',';
		// Export of Barcodes now has 2 passes - Stock to Barcode and SupplierStock to Barcode
		// introduced since the introduction of Multiple Barcodes by Stock and Supplier
		Database::TcpIBSQL sqlBarcodes(new TIBSQL(NULL));
		fDBTransaction.RegisterQuery(sqlBarcodes);
		sqlBarcodes->SQL->Text =
			"Select "
				"Barcode.Barcode,"
				"Stock.Description "
			"From "
				"Stock Inner Join StockLocation On "
					"Stock.Stock_Key = StockLocation.Stock_Key "
				"Inner Join Barcode On "
					"Barcode.Stock_Key = Stock.Stock_Key "
			"Where "
				"Barcode.Barcode <> '' and "
				"Barcode.Barcode is not null and "
				"StockLocation.Stocktake_Key = :Stocktake_Key "

			"Union "

			"Select "
				"Barcode.Barcode,"
				"Stock.Description "
			"From "
				"Stock Inner Join StockLocation On "
					"Stock.Stock_Key = StockLocation.Stock_Key "
				"Left Join SupplierStock On "
					 "Stock.Stock_Key = SupplierStock.Stock_Key "
				"Inner Join Barcode On "
					 "BarCode.Supplier_Stock_Key =  SupplierStock.Supplier_Stock_Key "
			"Where "
				"Barcode.Barcode <> '' and "
				"Barcode.Barcode is not null and "
				"StockLocation.Stocktake_Key = :Stocktake_Key";
				
		sqlBarcodes->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
		for (sqlBarcodes->ExecQuery(); !sqlBarcodes->Eof; sqlBarcodes->Next())
		{
			int Row = ExportCSV.Add(",");
			ExportCSV.Cells[0][Row] = sqlBarcodes->FieldByName("Barcode")->AsString;
			ExportCSV.Cells[1][Row] = sqlBarcodes->FieldByName("Description")->AsString;
		}
		ExportCSV.SaveToFile(FileName);

		fDBTransaction.CommitTransaction();
		return true;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		Application->ShowException(&E);
		return false;
	}
}
//---------------------------------------------------------------------------
void TStocktakeControl::fCancelInitialisation(int StocktakeKey)
{
	Database::TcpIBSQL sqlUpdateStocktake(new TIBSQL(NULL));
	fDBTransaction.RegisterQuery(sqlUpdateStocktake);
	sqlUpdateStocktake->SQL->Text =
		"Update "
			"Stocktake "
		"Set "
			"Initialised_Time = Null,"
			"Initialised_User_ID = Null,"
			"Initialised_User_Name = Null "
		"Where "
			"Stocktake_Key = :Stocktake_Key and "
			"Initialised_Time is not Null and "
			"Committed_Time is Null";
	sqlUpdateStocktake->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
	sqlUpdateStocktake->ExecQuery();


	if (sqlUpdateStocktake->RowsAffected > 0)
	{
		Database::TcpIBSQL sqlCancelStockLocation(new TIBSQL(NULL));
		fDBTransaction.RegisterQuery(sqlCancelStockLocation);
		sqlCancelStockLocation->SQL->Text =
			"Update "
				"StockLocation "
			"Set "
			  "Initialised       = 'F',"
			  "Stocktake         = Null,"
			  "Variance          = Null,"
			  "On_Hand           = On_Hand + Sales_Pending + Writeoffs_Pending  ,"
			  "Sales_Pending     = 0,"
			  "Writeoffs_Pending = 0 "

			"Where "
			  "Stocktake_Key     = :Stocktake_Key";

		sqlCancelStockLocation->ParamByName("Stocktake_Key")->AsInteger = StocktakeKey;
		sqlCancelStockLocation->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TStocktakeControl::fUpdateStocktakeItem(TTransactionBatchInfo const& BatchInfo,
			int StocktakeKey, TiStocktakeItemInfo const &StocktakeItemInfo)
{
	TStockLocationDetails StockDetails;
	if (fGetStockDetails(StocktakeItemInfo->StockKey, StocktakeItemInfo->Location, StockDetails))
	{
		if (StocktakeItemInfo->Variance != 0)
		{
			TTransactionInfo TransactionInfo(ttStocktake, BatchInfo);

			TransactionInfo.StockCode			= StockDetails.StockCode;
			TransactionInfo.Description		= StockDetails.Description;
			TransactionInfo.Stock_Category	= StockDetails.Stock_Category;
			TransactionInfo.Stock_Group		= StockDetails.Stock_Group;
			TransactionInfo.GL_Code				= StockDetails.GL_Code;
			TransactionInfo.Location			= StocktakeItemInfo->Location;
			TransactionInfo.Unit					= StockDetails.Stocktake_Unit;
			TransactionInfo.Unit_Cost			= StockDetails.Average_Cost;
			TransactionInfo.GST_Percent		= StockDetails.GST_Percent;
			TransactionInfo.Total_GST			= StocktakeItemInfo->Variance * StockDetails.GST_Percent * StockDetails.Average_Cost / 100;
			TransactionInfo.Qty					= StocktakeItemInfo->Variance;
			TransactionInfo.Total_Cost			= StockDetails.Average_Cost * StocktakeItemInfo->Variance;

			fAddBatchTransaction(TransactionInfo);
		}
		sqlStocktakeHistory->Close();
		sqlStocktakeHistory->ParamByName("StocktakeHistory_Key")->AsInteger	= fDBTransaction.GetGeneratorVal("Gen_StocktakeHistory_Key");
		sqlStocktakeHistory->ParamByName("Batch_Key")->AsInteger					= BatchInfo.BatchID;
		sqlStocktakeHistory->ParamByName("Stocktake_Key")->AsInteger			= StocktakeKey;
		sqlStocktakeHistory->ParamByName("Code")->AsString							= StockDetails.StockCode;
		sqlStocktakeHistory->ParamByName("Location")->AsString					= StocktakeItemInfo->Location;
		sqlStocktakeHistory->ParamByName("Average_Unit_Cost")->AsDouble		= StockDetails.Average_Cost;
		sqlStocktakeHistory->ParamByName("Latest_Unit_Cost")->AsDouble			= StockDetails.Latest_Cost;
		sqlStocktakeHistory->ParamByName("Stock_Category")->AsString			= StockDetails.Stock_Category;
		sqlStocktakeHistory->ParamByName("Stock_Group")->AsString				= StockDetails.Stock_Group;
		sqlStocktakeHistory->ParamByName("Description")->AsString				= StockDetails.Description;
		sqlStocktakeHistory->ParamByName("Stocktake_Unit")->AsString			= StockDetails.Stocktake_Unit;
		sqlStocktakeHistory->ParamByName("Last_Stocktake")->AsDateTime			= StockDetails.Last_Stocktake;
		sqlStocktakeHistory->ParamByName("Opening")->AsDouble						= StockDetails.Opening;
		sqlStocktakeHistory->ParamByName("Inwards")->AsDouble						= StockDetails.Inwards;
		sqlStocktakeHistory->ParamByName("Writeoff")->AsDouble					= StockDetails.Writeoff;
		sqlStocktakeHistory->ParamByName("Transfer")->AsDouble					= StockDetails.Transfer;
		sqlStocktakeHistory->ParamByName("Sales")->AsDouble						= StockDetails.Sales;
		sqlStocktakeHistory->ParamByName("Sales_Pending")->AsDouble			  	= StockDetails.Sales_Pending;
		sqlStocktakeHistory->ParamByName("Writeoffs_Pending")->AsDouble	  	= StockDetails.Writeoffs_Pending;
		sqlStocktakeHistory->ParamByName("Closing")->AsDouble						= StocktakeItemInfo->Counted;
  		sqlStocktakeHistory->ExecQuery();

		//Update Stock Levels. Also moves pending sales and writeoffs.
		sqlUpdateStockLocation->Close();
		sqlUpdateStockLocation->ParamByName("StockKey")->AsInteger				= StocktakeItemInfo->StockKey;
		sqlUpdateStockLocation->ParamByName("Location")->AsString				= StocktakeItemInfo->Location;
		sqlUpdateStockLocation->ParamByName("Counted")->AsDouble					= StocktakeItemInfo->Counted;
		sqlUpdateStockLocation->ParamByName("Last_Stocktake")->AsDateTime		= BatchInfo.Created;
		sqlUpdateStockLocation->ExecQuery();
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TStockSale::TStockSale(TIBDatabase *IBDatabase) : TStockTransaction(IBDatabase),
			sqlSelectStock(			new TIBSQL(NULL)),
			sqlUpdateStockLocation(	new TIBSQL(NULL)),
			fBatchInfo(Stock::ttSale),
			fEnabled(false),
			fTotalCost(0),
			fTotalGST(0)
{
	fDBTransaction.RegisterQuery(sqlSelectStock);
	fDBTransaction.RegisterQuery(sqlUpdateStockLocation);

	sqlSelectStock->SQL->Text =
		"Select "
			"Stock.Stock_Key,"
			"StockLocation.Location,"
			"Upper(StockLocation.Location) Location_Upper,"
			"StockLocation.Initialised,"
			"StockLocation.Stocktake_Key,"
			"Stocktake.Initialised_Time "
		"From "
			"Stock Left Join StockLocation On Stock.Stock_Key = StockLocation.Stock_Key "
			"Left Join Stocktake On StockLocation.Stocktake_Key = Stocktake.Stocktake_Key "
		"Where "
			"Stock.Code = :Code";

	sqlUpdateStockLocation->SQL->Text =
		"Update "
			"StockLocation "
		"Set "
			"On_Hand = On_Hand + :On_Hand,"
			"Sales = Sales + :Sales,"
			"Writeoff = Writeoff + :Writeoff,"
			"Sales_Pending = Sales_Pending + :Sales_Pending,"
			"Writeoffs_Pending = Writeoffs_Pending + :Writeoffs_Pending "
		"Where "
			"Stock_Key = :Stock_Key and "
			"Location = :Location";

	fBatchInfo.Created	= Now();
	fBatchInfo.User_Name	= "MenuMate";

	fDBTransaction.StartTransaction();
	if (!fCreateBatch(fBatchInfo))
	{
		fDBTransaction.UndoTransaction();
		throw Exception("Unable to create batch data");
	}
	else
	{
		fEnabled = true;
	}
}
//---------------------------------------------------------------------------
bool TStockSale::AdjustStock(TSaleItemInfo &SaleItemInfo, AnsiString Operator, AnsiString Reason)
{
	if (fEnabled)
	{
		try
		{
			bool Success = true;
			sqlSelectStock->Close();
			sqlSelectStock->ParamByName("Code")->AsString = SaleItemInfo.StockCode;
			sqlSelectStock->ExecQuery();
			if (!sqlSelectStock->Eof)
			{
				// Find or add the locatio
				while (sqlSelectStock->FieldByName("Location_Upper")->AsString.Trim() != SaleItemInfo.Location.UpperCase().Trim() && !sqlSelectStock->Eof)
				{
					sqlSelectStock->Next();
				}
				if (sqlSelectStock->FieldByName("Location_Upper")->AsString.Trim() != SaleItemInfo.Location.UpperCase().Trim())
				{
					fCreateStockLocation(sqlSelectStock->FieldByName("Stock_Key")->AsInteger, SaleItemInfo.Location);
				}
				else
				{
					SaleItemInfo.Location = sqlSelectStock->FieldByName("Location")->AsString;
				}
				// Modify the stock item levels.
				sqlUpdateStockLocation->Close();
				sqlUpdateStockLocation->ParamByName("Stock_Key")->AsInteger	= sqlSelectStock->FieldByName("Stock_Key")->AsInteger;
				sqlUpdateStockLocation->ParamByName("Location")->AsString	= SaleItemInfo.Location;

				if (sqlSelectStock->FieldByName("Initialised")->AsString != "T")
				{
					// Not initialised for stocktake so update on_hand values. Leave Pending levels as they are. Modify
					sqlUpdateStockLocation->ParamByName("On_Hand")->AsFloat					= SaleItemInfo.Qty;
					sqlUpdateStockLocation->ParamByName("Sales_Pending")->AsFloat			= 0;
					sqlUpdateStockLocation->ParamByName("Writeoffs_Pending")->AsFloat		= 0;


					if (SaleItemInfo.AdjustmentType == ttSale)
					{
						sqlUpdateStockLocation->ParamByName("Sales")->AsFloat					= SaleItemInfo.Qty;
						sqlUpdateStockLocation->ParamByName("Writeoff")->AsFloat				= 0;

					}
					else if (SaleItemInfo.AdjustmentType == ttWriteoff)
					{

                         if(SaleItemInfo.Qty>0)
                        { sqlUpdateStockLocation->ParamByName("Sales")->AsFloat					= SaleItemInfo.Qty;
						sqlUpdateStockLocation->ParamByName("Writeoff")->AsFloat				= -SaleItemInfo.Qty;
                        sqlUpdateStockLocation->ParamByName("On_Hand")->AsFloat					= 0;
                        }
                        else
                        {   sqlUpdateStockLocation->ParamByName("Sales")->AsFloat					= 0;
                            sqlUpdateStockLocation->ParamByName("Writeoff")->AsFloat				= SaleItemInfo.Qty;
                      }
					}
                  else if (SaleItemInfo.AdjustmentType == ttRefund)
					{
					   	sqlUpdateStockLocation->ParamByName("Sales")->AsFloat					= SaleItemInfo.Qty;
					   sqlUpdateStockLocation->ParamByName("Writeoff")->AsFloat				= 0;
  					}
					else
					{
						Success = false;
					}
				}
				else
				{
					if (sqlSelectStock->FieldByName("Initialised_Time")->AsDateTime > fBatchInfo.Created)
					{
						// Force this item to appear 1 min after this stocktake. Will then be corerct in reports.
						fBatchInfo.Created = sqlSelectStock->FieldByName("Initialised_Time")->AsDateTime + EncodeTime(0, 1, 0, 0);
					}
					// In the middle of a stocktake so don't update on_hand values. Update Pending levels.
					sqlUpdateStockLocation->ParamByName("On_Hand")->AsFloat					= 0;
					sqlUpdateStockLocation->ParamByName("Sales")->AsFloat						= 0;
					sqlUpdateStockLocation->ParamByName("Writeoff")->AsFloat					= 0;
  					if (SaleItemInfo.AdjustmentType == ttSale)
					{
						sqlUpdateStockLocation->ParamByName("Sales_Pending")->AsFloat		= SaleItemInfo.Qty;
						sqlUpdateStockLocation->ParamByName("Writeoffs_Pending")->AsFloat	= 0;
   					}
					else  if (SaleItemInfo.AdjustmentType == ttWriteoff)
					{

                        if(SaleItemInfo.Qty>0)
                        { sqlUpdateStockLocation->ParamByName("Sales_Pending")->AsFloat		= SaleItemInfo.Qty;
						sqlUpdateStockLocation->ParamByName("Writeoffs_Pending")->AsFloat				= -SaleItemInfo.Qty;

                        sqlUpdateStockLocation->ParamByName("On_Hand")->AsFloat					= 0;
                        }
                        else
                        {   sqlUpdateStockLocation->ParamByName("Sales_Pending")->AsFloat		= 0;
                            sqlUpdateStockLocation->ParamByName("Writeoffs_Pending")->AsFloat				= SaleItemInfo.Qty;

                        }
					  	
					}
                    else  if (SaleItemInfo.AdjustmentType == ttRefund)
					{
						sqlUpdateStockLocation->ParamByName("Sales_Pending")->AsFloat	        = SaleItemInfo.Qty;
						sqlUpdateStockLocation->ParamByName("Writeoffs_Pending")->AsFloat	= 0;


					}

					else
					{
						Success = false;
					}
				}
				if (Success)
				{
					sqlUpdateStockLocation->ExecQuery();
					// Create the generic transaction for the stocktrans table
					TStockLocationDetails StockDetails;
					if (fGetStockDetails(sqlSelectStock->FieldByName("Stock_Key")->AsInteger, SaleItemInfo.Location, StockDetails))
					{
						TTransactionInfo TransactionInfo(SaleItemInfo.AdjustmentType, fBatchInfo);

						TransactionInfo.StockCode			= StockDetails.StockCode;
						TransactionInfo.Description		= StockDetails.Description;
						TransactionInfo.Stock_Category	= StockDetails.Stock_Category;
						TransactionInfo.Stock_Group		= StockDetails.Stock_Group;
						TransactionInfo.GL_Code				= StockDetails.GL_Code;
						TransactionInfo.Location			= SaleItemInfo.Location;
						TransactionInfo.Unit					= StockDetails.Stocktake_Unit;
						TransactionInfo.Unit_Cost			= StockDetails.Average_Cost;
						TransactionInfo.GST_Percent		= StockDetails.GST_Percent;
						TransactionInfo.Total_GST			= StockDetails.Average_Cost * StockDetails.GST_Percent * SaleItemInfo.Qty / 100;
						TransactionInfo.Qty					= SaleItemInfo.Qty;
						TransactionInfo.Total_Cost			= TransactionInfo.Unit_Cost * TransactionInfo.Qty;
						TransactionInfo.Operator			= Operator;
						TransactionInfo.Note				= Reason;

						fTotalCost	+= TransactionInfo.Total_Cost;//(TransactionInfo.Unit_Cost * TransactionInfo.Qty);
						fTotalGST	+= TransactionInfo.Total_GST;

						fAddBatchTransaction(TransactionInfo);
					}
				}
			}
			else
			{
				Success = false;
			}
			sqlSelectStock->Close();
			return Success;
		}
		catch (Exception &E)
		{
			fEnabled = false;
			fDBTransaction.UndoTransaction();
			throw;
		}
	}
	else
	{
		return true;	// Hopefully rolled back by MM anyway
	}
}
//---------------------------------------------------------------------------
bool TStockSale::Close(bool Commit)
{
	try
	{
		if (fEnabled && Commit)
		{
			fBatchInfo.Total_Cost	= fTotalCost;
			fBatchInfo.Total_GST		= fTotalGST;
			if (!fUpdateBatch(fBatchInfo))
			{
				fEnabled = false;
				fDBTransaction.UndoTransaction();
				return false;
			}
			fDBTransaction.CommitTransaction();
			return true;
		}
		else
		{
			fDBTransaction.UndoTransaction();
			return fEnabled;
		}
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
}
//---------------------------------------------------------------------------

