//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ViewTrans.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmViewTrans *frmViewTrans;
//---------------------------------------------------------------------------
__fastcall TfrmViewTrans::TfrmViewTrans(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::FormCreate(TObject *Sender)
{
	pcFilter->ActivePage == tsReceived;
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::Button1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::FormShow(TObject *Sender)
{
	FormResize(NULL);

	Panel1->Visible = true;
	Panel2->Visible = false;

	if (!Transaction->InTransaction) Transaction->StartTransaction();

	qrStock->Close();
	qrStock->ParamByName("CODE")->AsString = StockCode;
	qrStock->Open();
	qrStock->FetchAll();
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;

	if (!rbAll->Checked && !rbThisMonth->Checked && !rbLastMonth->Checked)
	{
		rbAll->OnClick = NULL;
		rbAll->Checked = true;
		rbAll->OnClick = rbRangeClick;
	}
	for (int i=0; i<5; i++)
	{
		// Set all 4 queries to the same SQL depending on the date filter.
		// Only one will be activated. Others will be activated in OnTabChange.
		TIBQuery *Query;
		AnsiString TransactionType;
		switch (i)
		{
			case 0:
				Query = qrReceipt;
				TransactionType = "Receipt";
				break;
			case 1:
				Query = qrStocktake;
				TransactionType = "Stocktake";
				break;
			case 2:
				Query = qrManualSales;
				TransactionType = "Manual Sale";
				break;
			case 3:
				Query = qrSales;
				TransactionType = "Sale";
				break;
			case 4:
				Query = qrTransfer;
				TransactionType = "Transfer";
				break;
		}
		Query->Close();
		if (rbAll->Checked)
		{
			Query->SQL->Text =
				"SELECT "
					"CREATED,"
					"USER_NAME,"
					"SUPPLIER_NAME,"
					"REFERENCE,"
					"ORDER_QTY, "
					"ORDER_UNIT, "
					"QTY,"
					"UNIT, "
					"TRANS_NUMBER,"
					"UNIT_COST,"
					"PURCHASER_NAME,"
					"SALE_PRICE,"
					"SUPPLIER_CODE "
				"FROM "
					"STOCKTRANS "
				"WHERE "
					"CODE = :CODE AND "
					"LOCATION = :LOCATION AND "
					"TRANSACTION_TYPE = :TRANSACTION_TYPE "
				"ORDER BY CREATED DESC";
		}
		else
		if (rbLastMonth->Checked)
		{
			Query->SQL->Text =
				"SELECT "
					"CREATED,"
					"USER_NAME,"
					"SUPPLIER_NAME,"
					"REFERENCE,"
					"ORDER_QTY, "
					"ORDER_UNIT, "
					"QTY,"
					"UNIT, "
					"TRANS_NUMBER,"
					"UNIT_COST,"
					"PURCHASER_NAME,"
					"SALE_PRICE,"
					"SUPPLIER_CODE "
				"FROM "
					"STOCKTRANS "
				"WHERE "
					"CODE = :CODE AND "
					"LOCATION = :LOCATION AND "
					"TRANSACTION_TYPE = :TRANSACTION_TYPE AND "
					"EXTRACT (MONTH FROM CREATED) = :MONTH AND "
					"EXTRACT (YEAR FROM CREATED) = :YEAR "
				"ORDER BY CREATED DESC";
			Word Year, Month, Day;
			DecodeDate(IncMonth(Now(), -1), Year, Month, Day);
			Query->ParamByName("MONTH")->AsInteger = Month;
			Query->ParamByName("YEAR")->AsInteger = Year;
		}
		else
		if (rbThisMonth->Checked)
		{
			Query->SQL->Text =
				"SELECT "
					"CREATED,"
					"USER_NAME,"
					"SUPPLIER_NAME,"
					"REFERENCE,"
					"ORDER_QTY, "
					"ORDER_UNIT, "
					"QTY,"
					"UNIT, "
					"TRANS_NUMBER,"
					"UNIT_COST,"
					"PURCHASER_NAME,"
					"SALE_PRICE,"
					"SUPPLIER_CODE "
				"FROM "
					"STOCKTRANS "
				"WHERE "
					"CODE = :CODE AND "
					"LOCATION = :LOCATION AND "
					"TRANSACTION_TYPE = :TRANSACTION_TYPE AND "
					"EXTRACT (MONTH FROM CREATED) = :MONTH AND "
					"EXTRACT (YEAR FROM CREATED) = :YEAR "
				"ORDER BY CREATED DESC";
			Word Year, Month, Day;
			DecodeDate(Now(), Year, Month, Day);
			Query->ParamByName("MONTH")->AsInteger = Month;
			Query->ParamByName("YEAR")->AsInteger = Year;
		}
		Query->ParamByName("CODE")->AsString = StockCode;
		Query->ParamByName("TRANSACTION_TYPE")->AsString = TransactionType;

		if (i == pcFilter->TabIndex)
		{
			// This is the visible one.
			dsTransaction->DataSet = Query;
			Query->Open();
		}
	}
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}
	ClientHeight							= Button1->Top + Button1->Height + pnlStock->Top;
	ClientWidth								= pcFilter->Width + (pcFilter->Left * 2);
	Left										= (Screen->Width - Width) / 2;
	Top										= (Screen->Height - Height) / 2;
	dbgReceipt->TitleFont->Size		= dbgReceipt->Font->Size;
	dbgStocktake->TitleFont->Size		= dbgStocktake->Font->Size;
	dbgManualSales->TitleFont->Size	= dbgManualSales->Font->Size;
	dbgSales->TitleFont->Size			= dbgSales->Font->Size;
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmViewTrans::ResizeGrids()
{
	const ColCount1 = 5;
	TDBGrid *Grid = dbgReceipt;
	const PercArray1[ColCount1] = {15, 30, 20, 15, 20};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray1[i] / PercLeft - 1;
		if (PercArray1[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray1[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}

	const ColCount2 = 4;
	Grid = dbgStocktake;
	const PercArray2[ColCount2] = {25, 25, 25, 25};
	WidthSoFar = 0;
	PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray2[i] / PercLeft - 1;
		if (PercArray2[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray2[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}

	const ColCount3 = 6;
	Grid = dbgManualSales;
	const PercArray3[ColCount3] = {15, 20, 10, 15, 30, 10};
	WidthSoFar = 0;
	PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray3[i] / PercLeft - 1;
		if (PercArray3[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray3[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}

	const ColCount4 = 3;
	Grid = dbgSales;
	const PercArray4[ColCount4] = {34, 33, 33};
	WidthSoFar = 0;
	PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray4[i] / PercLeft - 1;
		if (PercArray4[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray4[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}

	const ColCount5 = 6;
	Grid = dbgTransfer;
	const PercArray5[ColCount5] = {15, 18, 11, 15, 11, 30};
	WidthSoFar = 0;
	PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray5[i] / PercLeft - 1;
		if (PercArray5[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray5[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::pcFilterChange(TObject *Sender)
{
	TIBQuery *Query;
	AnsiString TransactionType;

	if (pcFilter->ActivePage == tsReceived)
	{
		Query = qrReceipt;
	}
	else if (pcFilter->ActivePage == tsStocktake)
	{
		Query = qrStocktake;
	}
	else if (pcFilter->ActivePage == tsManualSales)
	{
		Query = qrManualSales;
	}
	else if (pcFilter->ActivePage == tsSales)
	{
		Query = qrSales;
	}
	else if (pcFilter->ActivePage == tsTransfer)
	{
		Query = qrTransfer;
	}
	Query->Open();
	dsTransaction->DataSet = Query;
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::rbRangeClick(TObject *Sender)
{
	FormShow(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::edFindKeyUp(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	qrReceipt->Open();
	if (qrReceipt->Locate("REFERENCE", edFind->Text, TLocateOptions() << loPartialKey << loCaseInsensitive))
	{
		dsTransaction->DataSet = qrReceipt;
		pcFilter->TabIndex = 0;
		return;
	}
	qrStocktake->Open();
	if (qrStocktake->Locate("REFERENCE", edFind->Text, TLocateOptions() << loPartialKey << loCaseInsensitive))
	{
		dsTransaction->DataSet = qrStocktake;
		pcFilter->TabIndex = 1;
		return;
	}
	qrManualSales->Open();
	if (qrManualSales->Locate("REFERENCE", edFind->Text, TLocateOptions() << loPartialKey << loCaseInsensitive))
	{
		dsTransaction->DataSet = qrManualSales;
		pcFilter->TabIndex = 2;
		return;
	}
	qrSales->Open();
	if (qrSales->Locate("REFERENCE", edFind->Text, TLocateOptions() << loPartialKey << loCaseInsensitive))
	{
		dsTransaction->DataSet = qrSales;
		pcFilter->TabIndex = 3;
		return;
	}
	qrTransfer->Open();
	if (qrTransfer->Locate("REFERENCE", edFind->Text, TLocateOptions() << loPartialKey << loCaseInsensitive))
	{
		dsTransaction->DataSet = qrTransfer;
		pcFilter->TabIndex = 4;
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::btnPrintAllClick(TObject *Sender)
{
/*	AnsiString Formula;
	if (ViewMode == vmStock)
	{
		Formula = "{stocktrans.Code} = \"" + dmStockData->dtStock->FieldByName("CODE")->AsString + "\"";
	}
	else if (ViewMode == vmContact)
	{
		Formula = "{stocktrans.Supplier} = \"" + dmStockData->dtContact->FieldByName("COMPANY_NAME")->AsString + "\"";
	}
	Panel1->Visible = false;
	Panel2->Visible = true;
	std::vector <AnsiString> DBAlias;
	DBAlias.push_back(dmStockData->DatabasePath);
	std::vector <AnsiString> DBName;
	std::vector <TFormula> Formulas;
	frmReports->PrintReport(	frmReports->crAnalysis,
										ExtractFilePath(Application->ExeName) + "Stock Reports\\StockTransactions.rpt",
										DBAlias,
										DBName,
										Formulas,
										Formula,
										NULL,
										NULL); */
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::btnPrintCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::qrAfterOpen(TDataSet *DataSet)
{
	DataSet->FieldByName("ORDER_QTY")->Alignment							= taLeftJustify;
	DataSet->FieldByName("QTY")->Alignment									= taLeftJustify;
	((TFloatField *)DataSet->FieldByName("SALE_PRICE"))->currency	= true;
	((TFloatField *)DataSet->FieldByName("UNIT_COST"))->currency	= true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewTrans::dbluLocationCloseUp(TObject *Sender)
{
	dbluLocation->KeyValue = qrStock->FieldByName("Location")->AsString;
}
//---------------------------------------------------------------------------

