//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectLocationSupplier.h"
 
#include "Suppliers.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectLocationSupplier *frmSelectLocationSupplier;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TfrmSelectLocationSupplier::TfrmSelectLocationSupplier(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::FormShow(TObject *Sender)
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
void __fastcall TfrmSelectLocationSupplier::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}*/

            if (qrContactforStockReq->Active && !qrContactforStockReq->IsEmpty() && dbgResult->Visible)
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
void __fastcall TfrmSelectLocationSupplier::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSelectLocationSupplier::ResizeGrids()
{
	dbgResult->TitleFont->Size				= dbgResult->Font->Size;
	dbgResult->Columns->Items[0]->Width	= dbgResult->ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::btnGoClick(TObject *Sender)
{

    qrContactforStockReq->Close();
     
      qrContactforStockReq->ParamByName("COMPANY_NAME")->AsString = edSearch->Text.UpperCase();
        qrContactforStockReq->ParamByName("LOCATION")->AsString = Location;

        	qrContactforStockReq->Open();
	if (!qrContactforStockReq->IsEmpty())
	{
		qrContactforStockReq->MoveBy(2);
		if (qrContactforStockReq->RecordCount == 1)
		{
			CompanyName	= qrContactforStockReq->FieldByName("COMPANY_NAME")->AsString;
			ContactKey	= qrContactforStockReq->FieldByName("CONTACT_LK")->AsInteger;
			qrContactforStockReq->Close();
			ModalResult = mrOk;
		}
		else
		{
			qrContactforStockReq->First();
			dbgResult->Visible = true;
			edSearch->Text = "";
			dbgResult->SetFocus();
			ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
			Top = (Screen->Height - Height) / 2;
		}
	}
	else
	{
		qrContactforStockReq->Close();
		qrContactforStockReq->ParamByName("COMPANY_NAME")->AsString = "%" + edSearch->Text.UpperCase() + "%";
           qrContactforStockReq->ParamByName("LOCATION")->AsString = Location;
		qrContactforStockReq->Open();
		if (qrContactforStockReq->IsEmpty())
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
			qrContactforStockReq->Next();
			qrContactforStockReq->First();
			if (qrContactforStockReq->RecordCount > 1)
			{
				dbgResult->Visible = true;
				edSearch->Text = "";
				dbgResult->SetFocus();
				ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
				Top = (Screen->Height - Height) / 2;
			}
			else
			{
				CompanyName	= qrContactforStockReq->FieldByName("COMPANY_NAME")->AsString;
				ContactKey	= qrContactforStockReq->FieldByName("CONTACT_LK")->AsInteger;
				qrContactforStockReq->Close();
				ModalResult = mrOk;
			}
		}
	}
       
   
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::dbgResultDblClick(TObject *Sender)
{
  TDataSet *ds = dbgResult->DataSource->DataSet;
  int selectedRows = dbgResult->SelectedRows->Count ;
  TSelectSupplierLocationNodeData *SelectedSupplierDetail = NULL;
  
     
if(selectedRows>1)
{
   for (int i = 0;i<selectedRows;i++)
  {
      ds->Bookmark = dbgResult->SelectedRows->Items[i];
   	CompanyName	= qrContactforStockReq->FieldByName("COMPANY_NAME")->AsString;
		ContactKey	= qrContactforStockReq->FieldByName("CONTACT_LK")->AsInteger;
     // StockKey= qrContact->FieldByName("STOCK_KEY")->AsInteger;
      SelectedSupplierDetail = new TSelectSupplierLocationNodeData;
      SelectedSupplierDetail->CompanyName = CompanyName;
      SelectedSupplierDetail->ContactKey =ContactKey;
      SelectedSupplierList->Add(SelectedSupplierDetail);
	ModalResult = mrOk;
 }
}
 else
{

	if (qrContactforStockReq->IsEmpty())
	{
		Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		CompanyName	= qrContactforStockReq->FieldByName("COMPANY_NAME")->AsString;
		ContactKey	= qrContactforStockReq->FieldByName("CONTACT_LK")->AsInteger;
		ModalResult = mrOk;

      SelectedSupplierDetail = new TSelectSupplierLocationNodeData;
      SelectedSupplierDetail->CompanyName = CompanyName;
      SelectedSupplierDetail->ContactKey =ContactKey;
      SelectedSupplierList->Add(SelectedSupplierDetail);

	}
    } 
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::dbgResultKeyPress(TObject *Sender,
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
void __fastcall TfrmSelectLocationSupplier::dbgResultKeyUp(TObject *Sender,
		WORD &Key, TShiftState Shift)
{ 


	if (Key == VK_UP)
	{   if (qrContactforStockReq->Bof)
		{
			Key = NULL;
			edSearch->SetFocus();
		}
      
      
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::edSearchKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnGoClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::btnFindClick(TObject *Sender)
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
void __fastcall TfrmSelectLocationSupplier::CMDialogChar(TWMKey &Msg)
{
	if (!(HIWORD(Msg.KeyData) & KF_ALTDOWN))
	{
		SendMessage(ActiveControl->Handle, WM_CHAR, Msg.CharCode, 0);
		Msg.Result = 1;
	}
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocationSupplier::edSearchChange(TObject *Sender)
{   	if (qrContactforStockReq->Active)
	{
   	qrContactforStockReq->Locate("COMPANY_NAME", edSearch->Text, TLocateOptions() << loPartialKey << loCaseInsensitive);
      }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
