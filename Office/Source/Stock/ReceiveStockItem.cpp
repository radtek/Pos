//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Connections.h"
#include "ReceiveStockItem.h"
#include "StockData.h"
#include "Login.h"
#include "SelectSupplier.h"
#include "SelectDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "NumericEdit"
#pragma resource "*.dfm"
//TfrmReceiveStockItem *frmReceiveStockItem;
//---------------------------------------------------------------------------
__fastcall TfrmReceiveStockItem::TfrmReceiveStockItem(TComponent* Owner)
: TForm(Owner),
frmAddStock(new TfrmAddStock(NULL))
{

}
//---------------------------------------------------------------------------
TModalResult TfrmReceiveStockItem::Execute()
{
        isPrefferedSupplierSelected  = true;
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	dbluLocation->Enabled = true;
	qrStock->Close();
	qrStock->ParamByName("Code")->AsString = StockCode;
    if(CheckLocation=="")
    {qrStock->ParamByName("LOCATION")->AsString = "%";}
    else
    {
     qrStock->ParamByName("LOCATION")->AsString = CheckLocation;
       CheckLocation="";
     SupplierKey=  StockRequestSupplierKey ;
       }
	qrStock->Open();
	if (qrStock->IsEmpty())
	{
		Application->MessageBox("Could not find the stock item.", "Error", MB_ICONERROR);
		return mrCancel;
	}
	sgLocations->Cells[0][0]	= "Location";
	sgLocations->Cells[1][0]	= "On Hand";
	sgLocations->Cells[2][0]	= "On Order";
	sgLocations->Cells[3][0]	= "Min";
	sgLocations->Cells[4][0]	= "Max";
	sgLocations->Cells[0][1]	= "";
	sgLocations->Cells[1][1]	= "";
	sgLocations->Cells[2][1]	= "";
	sgLocations->Cells[3][1]	= "";
	sgLocations->Cells[4][1]	= "";
	sgLocations->RowCount = 2;
	int Row = 1;
	for (qrStock->First(); !qrStock->Eof; qrStock->Next())
	{
		if (Row > 1)
		{
			sgLocations->RowCount++;
		}
		sgLocations->Cells[0][Row] = qrStock->FieldByName("Location")->AsString;
		sgLocations->Cells[1][Row] = qrStock->FieldByName("On_Hand")->AsString;
		sgLocations->Cells[2][Row] = qrStock->FieldByName("On_Order")->AsString;
		sgLocations->Cells[3][Row] = qrStock->FieldByName("Min_Level")->AsString;
		sgLocations->Cells[4][Row] = qrStock->FieldByName("Max_Level")->AsString;
		Row++;
	}
	qrStock->First();
	lbeStockCode->Caption = StockCode;
	lbeStocktakeUnit->Caption = qrStock->FieldByName("Stocktake_Unit")->AsString;

	if (!qrStock->Locate("Default_Location", "T", TLocateOptions()))
	{
		if (CurrentConnection.SingleLocation)
		{
			if (!qrStock->Locate("Location", CurrentConnection.DefaultLocation, TLocateOptions()))
			{
				Application->MessageBox("Could not find the stock's default location.", "Error", MB_ICONERROR);
			}
		}
		else
		{
			// try to locate the DefaultLocation  - there is no Stock Default Location
			if (!qrStock->Locate("Location", CurrentConnection.DefaultLocation, TLocateOptions()))
			{
				// if not get the first non blank location
				while (qrStock->FieldByName("Location")->AsString == "" && !qrStock->Eof)
				{
					qrStock->Next();
				}
			}
		}
	}
	qrSupplier->Close();
	qrSupplier->ParamByName("Stock_Key")->AsInteger = qrStock->FieldByName("Stock_Key")->AsInteger;
	qrSupplier->Open();
	qrSupplier->FetchAll();

	// qrSupplierStock is automatically opened.
	if (SupplierKey)
	{
		if (!qrSupplier->Locate("Contact_LK", SupplierKey, TLocateOptions()) || qrSupplierStock->Eof)
		{
			frmAddStock->StockKey			= qrStock->FieldByName("Stock_Key")->AsInteger;
			frmAddStock->StockGroupKey		= qrStock->FieldByName("Stock_Group_Key")->AsInteger;
			frmAddStock->NewSupplierKey	= SupplierKey;

			frmAddStock->Mode					= amAddSupplierUnit;
			if (frmAddStock->ShowModal() != mrOk)
			{
				return mrCancel;
			}
			qrSupplier->Close();
			qrSupplier->ParamByName("Stock_Key")->AsInteger = qrStock->FieldByName("Stock_Key")->AsInteger;
			qrSupplier->Open();
			qrSupplier->FetchAll();

			// qrSupplierStock is automatically updated.
		}

		if (!qrSupplier->Locate("Contact_LK", SupplierKey, TLocateOptions()))
		{
			Application->MessageBox("Could not find the supplier for this product!", "Error", MB_ICONERROR);
			return mrCancel;
		}

		dbluSupplier->Enabled = false;
		btnAddSupplier->Enabled = false;
	}
	else
	{
		dbluSupplier->Enabled = true;
		btnAddSupplier->Enabled = true;
		qrSupplier->First();
		isPrefferedSupplierSelected = qrSupplier->Locate("Preferred_Supplier", "T", TLocateOptions());

        if(!isPrefferedSupplierSelected)
        {
            qrCheapestSupplier->Close();
            qrCheapestSupplier->ParamByName("Stock_Key")->AsInteger = qrStock->FieldByName("Stock_Key")->AsInteger;
            qrCheapestSupplier->Open();
	    qrCheapestSupplier->FetchAll();
            int supplierKey = qrCheapestSupplier->FieldByName("SUPPLIER_KEY")->AsInteger;
            qrCheapestSupplier->Locate("SUPPLIER_KEY",supplierKey,TLocateOptions()) ;
            qrSupplier->Locate("SUPPLIER_KEY", supplierKey, TLocateOptions());

        }
	}
	dbluSupplier->KeyValue = qrSupplier->FieldByName("Contact_LK")->AsInteger;

	neQty->Value = 0;
	lbeTitle->Caption = qrStock->FieldByName("Description")->AsString + " (" + qrSupplierStock->FieldByName("Company_Name")->AsString + ")";
	ShowModal();
	return ModalResult;
}
//---------------------------------------------------------------------------
int __fastcall TfrmReceiveStockItem::ShowModal()
{
	return TForm::ShowModal();
}
//---------------------------------------------------------------------------
void TfrmReceiveStockItem::ResizeGrids()
{
	const ColCount = 5;
	TStringGrid *Grid = sgLocations;
	const PercArray[ColCount] = {36, 16, 16, 16, 16};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->ColCount; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray[i] / PercLeft;
		if (PercArray[i] != 0)
		{
			Grid->ColWidths[i] = ColWidth - 1;
			WidthSoFar += ColWidth;
			PercLeft -= PercArray[i];
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::FormShow(TObject *Sender)
{
	ResizeGrids();
	dbluSupplierUnit->SetFocus();
	if (dbluSupplierUnit->Enabled && qrSupplierStock->FieldByName("Supplier_Unit")->AsString == "")
	{
		dbluSupplierUnit->SetFocus();
	}
	//else if (CurrentConnection.SingleLocation && qrStock->FieldByName("Location")->AsString == CurrentConnection.DefaultLocation)
	//{
	//	neQty->SetFocus();
	//}
	//else if (dbluLocation->Enabled)
	//{
	//	dbluLocation->SetFocus();
	//}
	else
	{
		neQty->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::FormClose(TObject *Sender,
TCloseAction &Action)
{
	qrSupplierStock->Close();
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::btnCancelClick(TObject *Sender)
{
	if (Transaction->InTransaction) Transaction->Rollback();
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::btnOkClick(TObject *Sender)
{

	if (qrSupplierStock->FieldByName("Supplier_Unit")->AsString == "")
	{
		Application->MessageBox("Please select or add a supplier unit.", "Error", MB_ICONERROR + MB_OK);
		btnAddSupplierUnit->SetFocus();
		return;
	}
	if (qrStock->FieldByName("Location")->AsString == "")
	{
		Application->MessageBox("Please select a location.", "Error", MB_ICONERROR + MB_OK);
		dbluLocation->SetFocus();
		return;
	}
	if (qrStock->FieldByName("Initialised")->AsString == "T")
	{
		Application->MessageBox("This item is currently initialised for a stocktake.", "Warning", MB_ICONWARNING + MB_OK);
		//		dbluLocation->SetFocus();
		//		return;
	}
	StockKey	 = qrStock->FieldByName("Stock_Key")->AsInteger;
	Description    	= qrStock->FieldByName("Description")->AsString;
	StockCategory 	= qrStock->FieldByName("Stock_Category")->AsString;
	StockGroup	= qrStock->FieldByName("Stock_Group")->AsString;
	GLCode		= qrStock->FieldByName("GL_Code")->AsString;
	GSTPercent	= qrStock->FieldByName("GST_Percent")->AsFloat;
	Location	= qrStock->FieldByName("Location")->AsString;
	StocktakeUnit	= qrStock->FieldByName("Stocktake_Unit")->AsString;
	Initialised	= (qrStock->FieldByName("Initialised")->AsString == "T");
	InitialisedTime	= qrStock->FieldByName("Initialised_Time")->AsDateTime;
   if(CurrentConnection.SettingDecimalPlaces==4)
        {
    LatestCost = StrToFloat(FormatFloat("0.0000",qrStock->FieldByName("Latest_Cost")->AsFloat));
 //  LatestCost = StrToFloat(FloatToStrF(qrStock->FieldByName("Latest_Cost")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
   }
   else
   {
    LatestCost =  StrToFloat(FormatFloat("0.00",qrStock->FieldByName("Latest_Cost")->AsFloat));
   }
   OnHandQty = StrToFloat(FloatToStrF(qrStock->FieldByName("On_Hand")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
   SupplierUnitCost = FloatToStrF(neCost1->Value,ffFixed,19, 4);
   StocktakeUnitQty  =  StrToFloat(FloatToStrF(SupplierUnitQty * qrSupplierStock->FieldByName("Qty")->AsFloat,ffFixed,19, 4));




	SupplierCode	= qrSupplierStock->FieldByName("Supplier_Code")->AsString;
	SupplierUnit	= qrSupplierStock->FieldByName("Supplier_Unit")->AsString;

	SupplierUnitQty	 =  neQty->Value;
	SupplierUnitSize  = qrSupplierStock->FieldByName("Qty")->AsFloat;


    // qrSupplierSelection
    qrSupplierSelection->Close();
	qrSupplierSelection->ParamByName("Stock_Key")->AsInteger = qrStock->FieldByName("Stock_Key")->AsInteger;
    qrSupplierSelection->ParamByName("SUPPLIER_KEY")->AsInteger = qrSupplier->FieldByName("Contact_LK")->AsInteger;
	qrSupplierSelection->Open();

    Supplier = qrSupplierSelection->FieldByName("Company_Name")->AsString;
           SupplierKey	= qrSupplierSelection->FieldByName("Contact_LK")->AsInteger;
	        SupplierWeborderTargetUri = qrSupplierSelection->FieldByName("weborder_target_uri")->AsString;
      /*
   if(!isPrefferedSupplierSelected)
     {
           Supplier = qrCheapestSupplier->FieldByName("Company_Name")->AsString;
           SupplierKey	= qrCheapestSupplier->FieldByName("Contact_LK")->AsInteger;
	        SupplierWeborderTargetUri = qrCheapestSupplier->FieldByName("weborder_target_uri")->AsString;
     }
   else
     {
           Supplier       	= qrSupplier->FieldByName("Company_Name")->AsString;
           SupplierKey	= qrSupplier->FieldByName("Contact_LK")->AsInteger;
           SupplierWeborderTargetUri = qrSupplier->FieldByName("weborder_target_uri")->AsString;
     }*/
	ModalResult			= mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::Panel1Enter(TObject *Sender)
{
	// This allows pressing up arrow on OK to move to edit.
	neCost4->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::qrStockAfterOpen(TDataSet *DataSet)
{
	((TIBQuery *)DataSet)->FetchAll();
	dbluLocation->KeyValue = DataSet->FieldByName("Location")->AsString;
	((TFloatField *)DataSet->FieldByName("Average_Cost"))->currency = true;
	((TFloatField *)DataSet->FieldByName("Latest_Cost"))->currency = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::dtStockTransAfterOpen(
TDataSet *DataSet)
{
	((TFloatField *)DataSet->FieldByName("Unit_Cost"))->currency = true;
	DataSet->FieldByName("Order_Qty")->Alignment = taLeftJustify;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::cbSupplierUnitChange(TObject *Sender)
{
	//	UnitCost = qrSupplierStock->FieldByName("Latest_Cost")->AsFloat;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::qrSupplierStockAfterScroll(
TDataSet *DataSet)
{
	neCost1->Value = qrSupplierStock->FieldByName("Latest_Cost")->AsFloat;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::dbluSupplierUnitCloseUp(
TObject *Sender)
{
	dbluSupplierUnit->KeyValue = qrSupplierStock->FieldByName("Supplier_Unit")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::dbluLocationCloseUp(TObject *Sender)
{
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::btnAddSupplierUnitClick(
TObject *Sender)
{
	frmAddStock->StockKey			= qrStock->FieldByName("Stock_Key")->AsInteger;
	frmAddStock->StockGroupKey		= qrStock->FieldByName("Stock_Group_Key")->AsInteger;
	frmAddStock->NewSupplierKey	= qrSupplier->FieldByName("Contact_LK")->AsInteger;//SupplierKey;

	frmAddStock->Mode					= amAddSupplierUnit;
	if (frmAddStock->ShowModal()	== mrOk)
	{
		if (dbluSupplier->Enabled)
		{
			qrSupplier->Close();
			qrSupplier->ParamByName("Stock_Key")->AsInteger			= qrStock->FieldByName("Stock_Key")->AsInteger;
			qrSupplier->Open();
			qrSupplier->FetchAll();
			qrSupplier->Locate("Contact_LK", frmAddStock->NewSupplierKey, TLocateOptions());
			dbluSupplier->KeyValue = qrSupplier->FieldByName("Contact_LK")->AsInteger;
		}

		qrSupplierStock->Close();
		qrSupplierStock->ParamByName("Stock_Key")->AsInteger		= qrStock->FieldByName("Stock_Key")->AsInteger;
		qrSupplierStock->ParamByName("Supplier_Key")->AsInteger	= qrSupplier->FieldByName("Contact_LK")->AsInteger;//SupplierKey;
		qrSupplierStock->Open();
		qrSupplierStock->FetchAll();
		if (!qrSupplierStock->Locate("Supplier_Unit", frmAddStock->SupplierUnit, TLocateOptions()))
		{
			qrSupplierStock->First();
			qrSupplierStock->Locate("Default_Order_Unit", "T", TLocateOptions());
		}
		dbluSupplierUnit->KeyValue = qrSupplierStock->FieldByName("Supplier_Unit")->AsString;
		neQty->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::btnAddLocationClick(TObject *Sender)
{
	frmAddStock->StockKey			= qrStock->FieldByName("Stock_Key")->AsInteger;
	frmAddStock->StockGroupKey		= qrStock->FieldByName("Stock_Group_Key")->AsInteger;

	frmAddStock->Mode					= amAddLocation;
	if (frmAddStock->ShowModal()	== mrOk)
	{
		qrStock->Close();
		qrStock->ParamByName("Code")->AsString = StockCode;
		qrStock->Open();
		qrStock->FetchAll();
		//		dtStockTrans->FieldByName("Location")->AsString = frmAddStock->Location;
		dbluLocation->KeyValue = frmAddStock->Location;
		dbluLocation->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::neQtyChange(TObject *Sender)
{
	neCostChange(neCost1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::neCostChange(TObject *Sender)
{
	try
	{
		neCost1->OnChange = NULL;
		neCost2->OnChange = NULL;
		neCost3->OnChange = NULL;
		neCost4->OnChange = NULL;
        
		double Cost1, Cost2, Cost3, Cost4;

		// Default costs, incase they can't be calculated.
		Cost1 = qrSupplierStock->FieldByName("Latest_Cost")->AsFloat;
		Cost2 = Cost1 * (qrStock->FieldByName("GST_Percent")->AsFloat + 100) / 100;
		Cost3 = Cost1 * neQty->Value;
		Cost4 = Cost2 * neQty->Value;
       
		if (Sender == neCost1)
		{
			Cost1 = neCost1->Value;
			Cost2 = Cost1 * (qrStock->FieldByName("GST_Percent")->AsFloat + 100) / 100;
			Cost3 = Cost1 * neQty->Value;
			Cost4 = Cost2 * neQty->Value;
		}
		else if (Sender == neCost2)
		{
			if (qrStock->FieldByName("GST_Percent")->AsFloat + 100 != 0)
			{
              
				Cost1 = neCost2->Value * 100 / (qrStock->FieldByName("GST_Percent")->AsFloat + 100);
				Cost2 = neCost2->Value;
				Cost3 = Cost1 * neQty->Value;
				Cost4 = Cost2 * neQty->Value;
			}
		}
		else if (Sender == neCost3)
		{
			if (neQty->Value != 0)
			{
				Cost1 = neCost3->Value / neQty->Value;
				Cost2 = Cost1 * (qrStock->FieldByName("GST_Percent")->AsFloat + 100) / 100;
				Cost3 = neCost3->Value;
				Cost4 = Cost2 * neQty->Value;
			}
		}
		else if (Sender == neCost4)
		{
			if (qrStock->FieldByName("GST_Percent")->AsFloat + 100 != 0 && neQty->Value != 0)
			{
				Cost1 = neCost4->Value / neQty->Value * 100 / (qrStock->FieldByName("GST_Percent")->AsFloat + 100);
				Cost2 = neCost4->Value / neQty->Value;
				Cost3 = Cost1 * neQty->Value;
				Cost4 = neCost4->Value;
			}                             
		}
   
		neCost1->Value		= Cost1;
        neCost1->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
		neCost2->Value		= Cost2;
        neCost2->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
		neCost3->Enabled	= (neQty->Value != 0);
		neCost4->Enabled	= (neQty->Value != 0);
		if (neQty->Value != 0)
		{
			neCost3->Value	= Cost3;
            neCost3->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
			neCost4->Value	= Cost4;
            neCost4->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
		}
       
		else
		{
			neCost3->Value	= 0;
			neCost4->Value	= 0;
		}
	}
	__finally
	{
		neCost1->OnChange	= neCostChange;
		neCost2->OnChange	= neCostChange;
		neCost3->OnChange	= neCostChange;
		neCost4->OnChange	= neCostChange;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::sgLocationsDrawCell(TObject *Sender,
int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	/*	TStringGrid *Grid = (TStringGrid *)Sender;
	if (!State.Contains(gdFixed))
	{
		Grid->Canvas->Brush->Color = Grid->Color;
		Grid->Canvas->Font->Color = Grid->Font->Color;
		Grid->Canvas->FillRect(Rect);
	}
	Grid->Canvas->TextRect(Rect, Rect.Left+2, Rect.Top+2, Grid->Cells[ACol][ARow]);*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::sgLocationsClick(TObject *Sender)
{
	qrStock->Locate("Location", sgLocations->Cells[0][sgLocations->Row], TLocateOptions());
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::sgLocationsKeyPress(TObject *Sender,
char &Key)
{
	qrStock->Locate("Location", sgLocations->Cells[0][sgLocations->Row], TLocateOptions());
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::qrStockAfterScroll(TDataSet *DataSet)
{
	try
	{
		qrStock->AfterScroll = NULL;
		sgLocations->OnClick = NULL;
		for(int i=1; i<sgLocations->RowCount; i++)
		{
			if (sgLocations->Cells[0][i] == qrStock->FieldByName("Location")->AsString)
			{
				sgLocations->Row = i;
			}
		}
	}
	__finally
	{
		qrStock->AfterScroll = qrStockAfterScroll;
		sgLocations->OnClick = sgLocationsClick;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::dbluSupplierCloseUp(TObject *Sender)
{
	dbluSupplier->KeyValue = qrSupplier->FieldByName("Contact_LK")->AsInteger;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::qrSupplierAfterScroll(
TDataSet *DataSet)
{
	qrSupplierStock->Close();
	qrSupplierStock->ParamByName("Stock_Key")->AsInteger		= qrStock->FieldByName("Stock_Key")->AsInteger;
	qrSupplierStock->ParamByName("Supplier_Key")->AsInteger	= qrSupplier->FieldByName("Contact_LK")->AsInteger;
	qrSupplierStock->Open();
	qrSupplierStock->FetchAll();
	if (!qrSupplierStock->Locate("Default_Order_Unit", "T", TLocateOptions()))
	{
		qrSupplierStock->First();
	}
	dbluSupplierUnit->KeyValue = qrSupplierStock->FieldByName("Supplier_Unit")->AsString;
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;

	lbeTitle->Caption = qrStock->FieldByName("Description")->AsString + " (" + qrSupplierStock->FieldByName("Company_Name")->AsString + ")";
}
//---------------------------------------------------------------------------
void __fastcall TfrmReceiveStockItem::neCost1KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveStockItem::neCost2KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveStockItem::neCost3KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   } 
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveStockItem::neCost4KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmReceiveStockItem::neQtyKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == '-' && !AllowNegativeQuantity)
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

