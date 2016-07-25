//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectSupplier.h"
#include "Suppliers.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectSupplier *frmSelectSupplier;
//---------------------------------------------------------------------------
__fastcall TfrmSelectSupplier::TfrmSelectSupplier(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::FormShow(TObject *Sender)
{
	FormResize(NULL);
SelectedSupplierList =new TList;
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
void __fastcall TfrmSelectSupplier::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}*/
	if (qrContact->Active && !qrContact->IsEmpty() && dbgResult->Visible)
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
void __fastcall TfrmSelectSupplier::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSelectSupplier::ResizeGrids()
{
	dbgResult->TitleFont->Size				= dbgResult->Font->Size;
	dbgResult->Columns->Items[0]->Width	= dbgResult->ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::btnGoClick(TObject *Sender)
{
	qrContact->Close();
	qrContact->ParamByName("COMPANY_NAME")->AsString = edSearch->Text.UpperCase();
	qrContact->Open();
	if (!qrContact->IsEmpty())
	{
		qrContact->MoveBy(2);
		if (qrContact->RecordCount == 1)
		{
			CompanyName	= qrContact->FieldByName("COMPANY_NAME")->AsString;
			ContactKey	= qrContact->FieldByName("CONTACT_LK")->AsInteger;
			qrContact->Close();
			ModalResult = mrOk;
		}
		else
		{
			qrContact->First();
			dbgResult->Visible = true;
			edSearch->Text = "";
			dbgResult->SetFocus();
			ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
			Top = (Screen->Height - Height) / 2;
		}
	}
	else
	{
		qrContact->Close();
		qrContact->ParamByName("COMPANY_NAME")->AsString = "%" + edSearch->Text.UpperCase() + "%";
		qrContact->Open();
		if (qrContact->IsEmpty())
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
			qrContact->Next();
			qrContact->First();
			if (qrContact->RecordCount > 1)
			{
				dbgResult->Visible = true;
				edSearch->Text = "";
				dbgResult->SetFocus();
				ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
				Top = (Screen->Height - Height) / 2;
			}
			else
			{
				CompanyName	= qrContact->FieldByName("COMPANY_NAME")->AsString;
				ContactKey	= qrContact->FieldByName("CONTACT_LK")->AsInteger;
				qrContact->Close();
				ModalResult = mrOk;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::dbgResultDblClick(TObject *Sender)
{
  TDataSet *ds = dbgResult->DataSource->DataSet;
  int selectedRows = dbgResult->SelectedRows->Count ;
  TSelectSupplierNodeData *SelectedSupplierDetail = NULL;

if(selectedRows>1)
{
   for (int i = 0;i<selectedRows;i++)
  {
      ds->Bookmark = dbgResult->SelectedRows->Items[i];
   	CompanyName	= qrContact->FieldByName("COMPANY_NAME")->AsString;
		ContactKey	= qrContact->FieldByName("CONTACT_LK")->AsInteger;
     // StockKey= qrContact->FieldByName("STOCK_KEY")->AsInteger;
      SelectedSupplierDetail = new TSelectSupplierNodeData;
      SelectedSupplierDetail->CompanyName = CompanyName;
      SelectedSupplierDetail->ContactKey =ContactKey;
      SelectedSupplierList->Add(SelectedSupplierDetail);
	ModalResult = mrOk;
 	}
}
 else
{

	if (qrContact->IsEmpty())
	{
		Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		CompanyName	= qrContact->FieldByName("COMPANY_NAME")->AsString;
		ContactKey	= qrContact->FieldByName("CONTACT_LK")->AsInteger;
		ModalResult = mrOk;

      SelectedSupplierDetail = new TSelectSupplierNodeData;
      SelectedSupplierDetail->CompanyName = CompanyName;
      SelectedSupplierDetail->ContactKey =ContactKey;
      SelectedSupplierList->Add(SelectedSupplierDetail);

	}
}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::dbgResultKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		dbgResultDblClick(Sender);
	}
	else if (isprint(Key) || Key == VK_BACK)
	{
		PostMessage(edSearch->Handle, WM_CHAR, Key, 0);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::dbgResultKeyUp(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
	if (Key == VK_UP)
	{
		if (qrContact->Bof)
		{
			Key = NULL;
			edSearch->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::edSearchKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnGoClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::btnFindClick(TObject *Sender)
{
	frmSuppliers->ContactsMode = cmSelect;
	if (frmSuppliers->ShowModal() == mrOk)
	{
		CompanyName	= frmSuppliers->CompanyName;
		ContactKey	= frmSuppliers->ContactKey;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::CMDialogChar(TWMKey &Msg)
{
	if (!(HIWORD(Msg.KeyData) & KF_ALTDOWN))
	{
		SendMessage(ActiveControl->Handle, WM_CHAR, Msg.CharCode, 0);
		Msg.Result = 1;
	}
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplier::edSearchChange(TObject *Sender)
{
	if (qrContact->Active)
	{
		qrContact->Locate("COMPANY_NAME", edSearch->Text, TLocateOptions() << loPartialKey << loCaseInsensitive);
	}
}
//---------------------------------------------------------------------------

