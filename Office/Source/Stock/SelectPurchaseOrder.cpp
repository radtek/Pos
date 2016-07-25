//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectPurchaseOrder.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectPurchaseOrder *frmSelectPurchaseOrder;
//---------------------------------------------------------------------------
__fastcall TfrmSelectPurchaseOrder::TfrmSelectPurchaseOrder(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::FormShow(TObject *Sender)
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
void __fastcall TfrmSelectPurchaseOrder::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}*/
	if (qrOrder->Active && !qrOrder->IsEmpty() && dbgResult->Visible)
	{
		ClientHeight					= btnOk->Top + btnOk->Height + Panel1->Top;
	}
	else
	{
		ClientHeight					= btnCancel->Top + btnCancel->Height + Panel1->Top;
	}
	ClientWidth							= btnCancel->Left + btnCancel->Width + Panel1->Left;
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSelectPurchaseOrder::ResizeGrids()
{
	const ColCount = 2;
	TDBGrid *Grid = dbgResult;
	Grid->TitleFont->Size = Grid->Font->Size;
	const PercArray[ColCount] = {66, 34};
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
void __fastcall TfrmSelectPurchaseOrder::btnGoClick(TObject *Sender)
{
	qrOrder->Close();
	qrOrder->SQL->Text =
		"SELECT "
			"ORDER_NUMBER "
		"FROM "
			"PURCHASEORDER "
		"WHERE "
			"UPPER(ORDER_NUMBER) = :ORDER_NUMBER";
	qrOrder->ParamByName("ORDER_NUMBER")->AsString = edSearch->Text.UpperCase();
	qrOrder->Open();
	if (!qrOrder->IsEmpty())
	{
		OrderNumber = qrOrder->FieldByName("ORDER_NUMBER")->AsString;
		qrOrder->Close();
		ModalResult = mrOk;
	}
	else
	{
		qrOrder->Close();
		qrOrder->SQL->Text =
			"SELECT "
				"Company_Name SUPPLIER_NAME,"
				"ORDER_NUMBER "
			"FROM "
				"PURCHASEORDER Left Join Contact on "
					"PurchaseOrder.Contact_FK = Contact.Contact_LK "
			"WHERE "
				"UPPER(ORDER_NUMBER) LIKE :ORDER_NUMBER "
			"ORDER BY "
				"Company_Name ASC";
		qrOrder->ParamByName("ORDER_NUMBER")->AsString = "%" + edSearch->Text.UpperCase() + "%";
		qrOrder->Open();
		if (qrOrder->IsEmpty())
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
			qrOrder->Next();
			qrOrder->First();
			if (qrOrder->RecordCount > 1)
			{
				dbgResult->Visible = true;
				dbgResult->SetFocus();
				ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
				Top = (Screen->Height - Height) / 2;
			}
			else
			{
				OrderNumber = qrOrder->FieldByName("ORDER_NUMBER")->AsString;
				qrOrder->Close();
				ModalResult = mrOk;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::edSearchKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnGoClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::dbgResultDblClick(TObject *Sender)
{
	if (qrOrder->IsEmpty())
	{
		Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		OrderNumber = qrOrder->FieldByName("ORDER_NUMBER")->AsString;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::dbgResultKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		dbgResultDblClick(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::FormKeyUp(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
	if (Key == VK_UP)
	{
		if (qrOrder->Bof)
		{
			Key = NULL;
			edSearch->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPurchaseOrder::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

