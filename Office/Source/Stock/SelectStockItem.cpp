//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectStockItem.h"
#include "StockData.h"
#include "Stock.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectStockItem *frmSelectStockItem;
//---------------------------------------------------------------------------
__fastcall TfrmSelectStockItem::TfrmSelectStockItem(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (Reset)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		Reset = false;
		dbgResult->Visible = false;
		ClientHeight = btnCancel->Top + btnCancel->Height + Panel1->Top;
		Top = (Screen->Height - Height) / 2;
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		edSearch->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
	NewItemName = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}*/
	if (qrStock->Active && !qrStock->IsEmpty() && dbgResult->Visible)
	{
		ClientHeight					= btnOk->Top + btnOk->Height + Panel1->Top;
	}
	else
	{
		ClientHeight					= btnCancel->Top + btnCancel->Height + Panel1->Top;
	}
//	ClientWidth							= btnCancel->Left + btnCancel->Width + Panel1->Left;
   Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;
   ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::WMDisplayChange(TWMDisplayChange& Message)
{
//	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSelectStockItem::ResizeGrids()
{
	const ColCount = 2;
	TDBGrid *Grid = dbgResult;
	Grid->TitleFont->Size = Grid->Font->Size;
	const PercArray[ColCount] = {43, 57};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray[i] / PercLeft - 1;
		if (PercArray[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::btnGoClick(TObject *Sender)
{
        frmStock->Location = Location;

	if (edSearch->Text == "")
	{
		btnFindAddClick(NULL);
	}
	else
	{
		qrStock->Close();
		qrStock->SQL->Text =
			"SELECT "
				"STOCK.STOCK_KEY, "
				"CODE, "
				"DESCRIPTION, "
            "CONVERSION_FACTOR, "
            "REDUCTION_UNIT, "
            "STOCKTAKE_UNIT "
			"FROM "
				"STOCK "
                "left join StockLocation on Stock.Stock_Key = StockLocation.Stock_Key "
			"WHERE "
				"UPPER(DESCRIPTION) = :DESCRIPTION And "
				"Enabled = 'T' And "
				"Deleted = 'F'"
                "and StockLocation.LOCATION like :Location "
                 " group by   STOCK_KEY,Code,DESCRIPTION, STOCKTAKE_UNIT ,CONVERSION_FACTOR,REDUCTION_UNIT ";
		qrStock->ParamByName("DESCRIPTION")->AsString = edSearch->Text.UpperCase();
        if(Location=="")
        { qrStock->ParamByName("Location")->AsString = "%";
        }
        else
        	qrStock->ParamByName("Location")->AsString = Location;


		qrStock->Open();
		if (!qrStock->IsEmpty())
		{
			StockKey		= qrStock->FieldByName("STOCK_KEY")->AsInteger;
			StockCode	= qrStock->FieldByName("CODE")->AsString;
			Description	= qrStock->FieldByName("DESCRIPTION")->AsString;
			Unit			= qrStock->FieldByName("STOCKTAKE_UNIT")->AsString;
         ConversionFactor = qrStock->FieldByName("CONVERSION_FACTOR")->AsCurrency;
         ReductionUnit			= qrStock->FieldByName("REDUCTION_UNIT")->AsString;
			qrStock->Close();
			ModalResult = mrOk;
		}
		else
		{
			qrStock->Close();
			qrStock->SQL->Text =
				"SELECT "
					"STOCK.STOCK_KEY,"
					"STOCK.CODE,"
					"STOCK.DESCRIPTION,"
               "STOCK.CONVERSION_FACTOR,"
               "STOCK.REDUCTION_UNIT, "
					"STOCK.STOCKTAKE_UNIT,"
					"StockGroup.Stock_Group "
				"FROM "
					"STOCK INNER JOIN StockGroup ON STOCK.Stock_Group_KEY = StockGroup.Stock_Group_KEY "
                    "left join StockLocation on Stock.Stock_Key = StockLocation.Stock_Key "
				"WHERE "
					"UPPER(STOCK.DESCRIPTION) LIKE :DESCRIPTION And "
					"Stock.Enabled = 'T' And "
					"Stock.Deleted = 'F' "
                     "and StockLocation.LOCATION like :Location "
                 " group by    STOCK.STOCK_KEY,	Code,DESCRIPTION, STOCK.CONVERSION_FACTOR,"
               "STOCK.REDUCTION_UNIT, STOCKTAKE_UNIT,Stock_Group "
				 ;
			qrStock->ParamByName("DESCRIPTION")->AsString = "%" + edSearch->Text.UpperCase() + "%";
             if(Location=="")
        { qrStock->ParamByName("Location")->AsString = "%";
        }
        else
        	qrStock->ParamByName("Location")->AsString = Location;

			qrStock->Open();
			if (qrStock->IsEmpty())
			{
				Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
				edSearch->Text = "";
				edSearch->SetFocus();
				dbgResult->Visible = false;
				ClientHeight = btnCancel->Top + btnCancel->Height + Panel1->Top;
				Top = (Screen->Height - Height) / 2;
			}
			else
			{
				qrStock->Next();
				qrStock->First();
				if (qrStock->RecordCount > 1)
				{
					edSearch->Text = "";
					dbgResult->Visible = true;
					dbgResult->SetFocus();
					ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
					Top = (Screen->Height - Height) / 2;
				}
				else
				{
					StockKey 	= qrStock->FieldByName("STOCK_KEY")->AsInteger;
					StockCode 	= qrStock->FieldByName("CODE")->AsString;
					Description	= qrStock->FieldByName("DESCRIPTION")->AsString;
					Unit			= qrStock->FieldByName("STOCKTAKE_UNIT")->AsString;
               ConversionFactor = qrStock->FieldByName("CONVERSION_FACTOR")->AsCurrency;
               ReductionUnit			= qrStock->FieldByName("REDUCTION_UNIT")->AsString;
					qrStock->Close();
					ModalResult = mrOk;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::edSearchKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnGoClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::dbgResultDblClick(TObject *Sender)
{
	if (qrStock->IsEmpty())
	{
		Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		StockKey 	= qrStock->FieldByName("STOCK_KEY")->AsInteger;
		StockCode	= qrStock->FieldByName("CODE")->AsString;
		Description	= qrStock->FieldByName("DESCRIPTION")->AsString;
		Unit			= qrStock->FieldByName("STOCKTAKE_UNIT")->AsString;
      ConversionFactor = qrStock->FieldByName("CONVERSION_FACTOR")->AsCurrency;
      ReductionUnit			= qrStock->FieldByName("REDUCTION_UNIT")->AsString;
		ModalResult	= mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::dbgResultKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		dbgResultDblClick(Sender);
	}
	else if (isprint(Key) || Key == '\b')
	{
		PostMessage(edSearch->Handle, WM_CHAR, Key, 0);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::dbgResultKeyUp(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
	if (Key == VK_UP)
	{
		if (qrStock->Bof)
		{
			Key = NULL;
			edSearch->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::btnFindAddClick(TObject *Sender)
{
    frmStock->Location = Location;
	frmStock->StockMode = smSelect;
	frmStock->NewItemName = NewItemName;
	if (frmStock->ShowModal() == mrOk)
	{
		StockKey		= frmStock->StockKey;
		qrStock->SQL->Text =
            "SELECT  "
                "a.Code, "
                "a.DESCRIPTION,  "
                "a.CONVERSION_FACTOR, "
                "a.REDUCTION_UNIT, "
                "a.STOCKTAKE_UNIT, "
                "b.LOCATION "
			"FROM "
				"STOCK a inner join STOCKLOCATION b on a.STOCK_KEY = b.STOCK_KEY "
			"WHERE "
				"a.Stock_Key = :Stock_Key";
		qrStock->ParamByName("Stock_Key")->AsString = StockKey;
		qrStock->Open();
		if (!qrStock->IsEmpty())
		{
			Description = qrStock->FieldByName("DESCRIPTION")->AsString;
			Unit			= qrStock->FieldByName("STOCKTAKE_UNIT")->AsString;
			StockCode	= qrStock->FieldByName("Code")->AsString;
             ConversionFactor = qrStock->FieldByName("CONVERSION_FACTOR")->AsCurrency;
             ReductionUnit			= qrStock->FieldByName("REDUCTION_UNIT")->AsString;
             if(Location == "")
             {
                Location = qrStock->FieldByName("Location")->AsString;
             }
         
			ModalResult = mrOk;
		}
      else
      {
         ConversionFactor = 1;
         ReductionUnit = "";
			Description = "";
			Unit			= "";
			StockCode	= NULL;
      }
		qrStock->Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::CMDialogChar(TWMKey &Msg)
{
	if (!(HIWORD(Msg.KeyData) & KF_ALTDOWN))
	{
		SendMessage(ActiveControl->Handle, WM_CHAR, Msg.CharCode, 0);
		Msg.Result = 1;
	}
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStockItem::edSearchChange(TObject *Sender)
{
	if (qrStock->Active)
	{
		qrStock->Locate("DESCRIPTION", edSearch->Text, TLocateOptions() << loPartialKey << loCaseInsensitive);
	}
}
//---------------------------------------------------------------------------

void TfrmSelectStockItem::ClearLocation()
{
    frmStock->Location = NULL;
}


