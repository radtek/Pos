// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ParkedSales.h"
#include "ParkedSale.h"
#include "ManagerParkedSales.h"
#include "DeviceRealTerminal.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#define NUMBER_OF_SALES_IN_VIEW 5

TfrmParkedSales *frmParkedSales;

// ---------------------------------------------------------------------------
__fastcall TfrmParkedSales::TfrmParkedSales(TComponent* Owner) : TZForm(Owner), ParkedSales(new TManagerParkedSales())
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmParkedSales::FormShow(TObject *Sender)
{
	FormResize(Sender);
	while (sbSales->ControlCount != 0)
	{
		delete sbSales->Controls[0];
	}

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	std::auto_ptr <TStringList> Data(new TStringList);
	ParkedSales->GetParkedSales(DBTransaction, Data.get());

	int Y = 0;
	for (int i = 0; i < Data->Count; i++)
	{
		int LableHeight = sbSales->ClientHeight / NUMBER_OF_SALES_IN_VIEW;
		TTouchBtn *NewSale = new TTouchBtn(this);
		NewSale->OnMouseClick = TouchButtonSaleClick;
		NewSale->Top = Y + lbeTitle->Top;
		NewSale->Width = pnlParkedSales->ClientWidth - (lbeTitle->Top * 2);
		NewSale->Left = lbeTitle->Left;
		NewSale->Height = btnSalesUp->Height;
		NewSale->Caption = Data->Strings[i];
		NewSale->Tag = (int)Data->Objects[i];
		NewSale->Parent = sbSales;
		NewSale->ParentFont = true;
		Y += LableHeight;
		sbSales->VertScrollBar->Range = NewSale->Top + NewSale->Height;
	}

	DBTransaction.Commit();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmParkedSales::TouchButtonSaleClick(TObject *Sender)
{
	SelectedParkedSaleTabKey = ((TTouchBtn*)(Sender))->Tag;
	ModalResult = mrOk;
}

void __fastcall TfrmParkedSales::tbCloseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmParkedSales::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmParkedSales::pnlParkedSalesResize(TObject *Sender)
{
	sbSales->Align = alClient;
	sbSales->Align = alNone;
	sbSales->Width += 20;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmParkedSales::btnSeatsDownMouseClick(TObject *Sender)
{
	int StartScrollPos = sbSales->VertScrollBar->Position;
	int ButtonHeight = sbSales->ClientHeight / NUMBER_OF_SALES_IN_VIEW;
	int EndScrollPos = StartScrollPos + sbSales->ClientHeight - ButtonHeight;
	if (EndScrollPos > sbSales->VertScrollBar->Range)
		EndScrollPos = sbSales->VertScrollBar->Range;

	for (int i = StartScrollPos; i <= EndScrollPos; i += 8)
	{
		sbSales->VertScrollBar->Position = i;
		sbSales->Update();
	}
	sbSales->VertScrollBar->Position = EndScrollPos;

}
// ---------------------------------------------------------------------------

void __fastcall TfrmParkedSales::btnSalesUpMouseClick(TObject *Sender)
{
	int StartScrollPos = sbSales->VertScrollBar->Position;
	int ButtonHeight = sbSales->ClientHeight / NUMBER_OF_SALES_IN_VIEW;
	int EndScrollPos = StartScrollPos - sbSales->ClientHeight + ButtonHeight;
	if (EndScrollPos < 0)
		EndScrollPos = 0;

	for (int i = StartScrollPos; i >= EndScrollPos; i -= 8)
	{
		sbSales->VertScrollBar->Position = i;
		sbSales->Update();
	}
	sbSales->VertScrollBar->Position = EndScrollPos;
}
// ---------------------------------------------------------------------------
