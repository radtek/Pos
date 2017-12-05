//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <winuser.h>

#include "Main.h"
#include "StockMain.h"
#include "StockData.h"
#include "MMData.h"
#include "Reports.h"
#include "AddContact.h"
//#include "ContactSearch.h"
#include "CreateOrders.h"
#include "PurchaseOrder.h"
#include "ViewTrans.h"
#include "StockRequestTransfer.h"
#include "Stocktake.h"
#include "Users.h"
#include "Login.h"
#include "Administration.h"

#include "SelectSupplier.h"
#include "SelectStockItem.h"
#include "SelectPurchaseOrder.h"
#include "ReceiveOrder.h"
#include "Invoices.h"
#include "ReceiveStockItem.h"
#include "Suppliers.h"
#include "Stock.h"
#include "PurchaseOrders.h"
#include "Permission.h"
#include "AuthenticationService.h"
//#include "Transfer.h"
#include "TransferMulti.h"
#include "MYOBPurchasesExport.h"
#include "Registration.h"

#include "SelectTransferLocations.h"
#include "WriteOffStock.h"
#include "RecipeBox.h"
#include "UpdateSupplierCosts.h"
#include "SelectSupplierStockRequest.h"
#include "StockRequest.h"
#include "StockRequestHistory.h"
#include "StockSubstitute.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmStockMain *frmStockMain;
//---------------------------------------------------------------------------
__fastcall TfrmStockMain::TfrmStockMain(TComponent* Owner)
	: TForm(Owner)
{
	dmMMData->Registered(&Registered, &Modules);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::FormShow(TObject *Sender)
{
	frmMain->Visible = false;
	if (Modules & eAccounting)
	{
      // TODO implement bulk export of invoices to Xero
		btnExport->Enabled = (Modules & eAccounting);
		if (CurrentConnection.AccountSystem == "")
		{
			btnExport->Caption = "Accounts Export";
		}
		else
		{
			btnExport->Caption = "Export To " + CurrentConnection.AccountSystem;
		}
	}
	else
	{
		btnExport->Enabled = false;
		btnExport->Caption = "Export To Accounts";
	}
    CreateStockRequestTable();
}

//---------------------------------------------------------------------------

void TfrmStockMain::CreateStockRequestTable()
{
    if (!Transaction->InTransaction)
        Transaction->StartTransaction();

    qrCreateStockRequestStockTable->Close();
    qrCreateStockRequestTable->Close();
    qrCheckTableExists->Close();
    qrStockRequestForeignKey->Close();
    qrStockRequestStockForeignKey->Close();
    qrStockRequestSupplierForeignKey->Close();
      qrcheckfield->Close();
      qrCheckTableExists->Close();
      qrupdatecode->Close();
         qrCheckTableExists->ParamByName("Table_Name")->AsString = "STOCKREQUEST";
    qrCheckTableExists->ExecQuery();



    // If end of line means there is no entry for the tables
    if(qrCheckTableExists->Eof)
    {
        qrCreateStockRequestTable->ExecQuery();
        qrCreateStockRequestStockTable->ExecQuery();
        qrStockRequestForeignKey->ExecQuery();
        qrStockRequestStockForeignKey->ExecQuery();
        qrStockRequestSupplierForeignKey->ExecQuery();
    }
      

       qrcheckfield->ParamByName("tablename")->AsString = "STOCKREQUEST_STOCK";
     qrcheckfield->ParamByName("field")->AsString = "SUPPLIER_STOCK_KEY";
    qrcheckfield->ExecQuery();
          if(qrcheckfield->Eof)
    {
     qrStockrequestKey->ExecQuery();
     qrupdatecode->ExecQuery();

    }

    if (Transaction->InTransaction)
        Transaction->Commit();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockMain::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	frmMain->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnHistoryClick(TObject *Sender)
{
	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		std::auto_ptr<TfrmViewTrans>frmViewTrans(new TfrmViewTrans(NULL));
		frmViewTrans->StockCode = frmSelectStockItem->StockCode;
		frmViewTrans->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnStocktakeClick(TObject *Sender)
{
    bool hasPermission = Security::AuthenticationService().HasPermission(
        frmLogin->CurrentUser.UserID,
        Security::Stocktake);

    if (hasPermission == false)
    {
        Application->MessageBox("You are not authorized for this feature", "Error", MB_ICONERROR + MB_OK);
        return;
    }




	TStocktakeGUI StocktakeGUI;
	StocktakeGUI.Execute();
/*
	std::auto_ptr<TfrmStockGroupVisible>frmStockGroupVisible(new TfrmStockGroupVisible(NULL));
	if (frmStockGroupVisible->ShowModal() == mrOk)
	{
		if (frmStockGroupVisible->Location != "")
		{
			std::auto_ptr<TfrmStocktake>frmStocktake(new TfrmStocktake(NULL));
			frmStocktake->Location = frmStockGroupVisible->Location;
			frmStocktake->ShowModal();
		}
      else
		{
			Application->MessageBox("You did not select a location.", "Error", MB_ICONERROR + MB_OK);
		}
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnPrintClick(TObject *Sender)
{
	frmReports->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnUsersClick(TObject *Sender)
{
	frmUsers->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnReceiveOrderClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowRecvPOrder)) {
            Application->MessageBox(
              "You do not have permission to recieve purchase orders.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

    std::auto_ptr<TfrmReceiveOrder>frmReceiveOrder(new TfrmReceiveOrder(NULL));
    frmReceiveOrder->OrderNumber = "";//frmSelectPurchaseOrder->OrderNumber;
    frmReceiveOrder->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnReceiveInvoiceClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowRecvInv)) {
            Application->MessageBox(
              "You do not have permission to receive invoices.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

	frmSelectSupplier->Reset = true;
	if (frmSelectSupplier->ShowModal() == mrOk)
	{
		std::auto_ptr<TfrmInvoices>frmInvoices(new TfrmInvoices(NULL));
		frmInvoices->ContactKey = frmSelectSupplier->ContactKey;
		frmInvoices->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnEditStockClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowEAStock)) {
            Application->MessageBox(
              "You do not have permission to edit or add stock items.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

	frmStock->StockMode = smEdit;
	frmStock->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnCreateOrderClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowCreatSOrder)) {
            Application->MessageBox(
              "You do not have permission to create supplier orders.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

	frmSelectSupplier->Reset = true;
	if (frmSelectSupplier->ShowModal() == mrOk)
	{
		std::auto_ptr<TfrmPurchaseOrders>frmPurchaseOrders(new TfrmPurchaseOrders(NULL));
		frmPurchaseOrders->ContactKey = frmSelectSupplier->ContactKey;
		frmPurchaseOrders->Execute();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::BitBtn1Click(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowCreatPOrder)) {
            Application->MessageBox(
              "You do not have permission to create purchase orders.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }        ;


        std::auto_ptr<TfrmSelectSupplierStockRequest>frmSelectSupplierStockRequest(new TfrmSelectSupplierStockRequest(NULL));
        if(frmSelectSupplierStockRequest->ShowModal() == mrOk)
        {
          TList *StockRequestList =  frmSelectSupplierStockRequest->SelectedRequestList;
          AnsiString DateRequired =  frmSelectSupplierStockRequest->DateRequired;
	  std::auto_ptr<TfrmCreateOrders>frmCreateOrders(new TfrmCreateOrders(NULL));
          frmCreateOrders->SelectedRequestList = StockRequestList;
          frmCreateOrders->DateRequired = (TDateTime)DateRequired; 
	  frmCreateOrders->ShowModal();
        }
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnEditContactsClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowEASupplier)) {
            Application->MessageBox(
              "You do not have permission to edit or add suppliers.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

	frmSuppliers->ContactsMode = cmEdit;
	frmSuppliers->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnTransferMultiClick(TObject *Sender)
{
    bool hasPermission = Security::AuthenticationService().HasPermission(
        frmLogin->CurrentUser.UserID,
        Security::Transfer);

    if (hasPermission == false)
    {
        Application->MessageBox("You are not authorized for this feature", "Error", MB_ICONERROR + MB_OK);
        return;
    }



	if (dmMMData->dbMenuMate->Connected)
	{
		TSelectTransferLocations SelectTransferLocations;
		if (SelectTransferLocations.Execute())
		{
			TTransferLocations TransferLocations	= SelectTransferLocations.Locations();

            std::auto_ptr<TfrmStockRequestTransfer>frmStockRequestTransfer(new TfrmStockRequestTransfer(NULL));
            frmStockRequestTransfer->Source = TransferLocations.Source;
            frmStockRequestTransfer->Destination = TransferLocations.Destination;
            frmStockRequestTransfer->ShowModal();

            if(frmStockRequestTransfer->ModalResult == mrOk)
            {
                TStringList* selectedStockRequestKeys = frmStockRequestTransfer->GetSelectedRequestKeys();

                std::auto_ptr<TfrmTransferMulti>frmTransferMulti(new TfrmTransferMulti(NULL));
                frmTransferMulti->Source					= TransferLocations.Source;
                frmTransferMulti->Destination				= TransferLocations.Destination;
                frmTransferMulti->SelectedStockRequestKeys = selectedStockRequestKeys;
                frmTransferMulti->ShowModal();
            }
		}
	}
	else
	{
		Application->MessageBox("A connection to the MenuMate database is required for this.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::CMDialogKey(TCMDialogKey &Msg)
{
/*	if (Msg.CharCode == VK_LEFT)
	{
		if (ActiveControl == btnEditStock)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnEditContacts)
		{
			btnEditStock->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveInvoice)
		{
			btnEditContacts->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnStocktake)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnTransfer)
		{
			btnStocktake->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveOrder)
		{
			btnTransfer->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnManualSale)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnCreateOrder)
		{
			btnManualSale->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnHistory)
		{
			btnCreateOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnClose)
		{
			btnReceiveInvoice->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnPrint)
		{
			btnReceiveInvoice->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnUsers)
		{
			btnReceiveInvoice->SetFocus();
			Msg.Result = 1;
			return;
		}
	}
	else if (Msg.CharCode == VK_RIGHT)
	{
		if (ActiveControl == btnEditStock)
		{
			btnEditContacts->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnEditContacts)
		{
			btnReceiveInvoice->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveInvoice)
		{
			btnClose->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnStocktake)
		{
			btnTransfer->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnTransfer)
		{
			btnReceiveOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveOrder)
		{
			btnClose->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnManualSale)
		{
			btnCreateOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnCreateOrder)
		{
			btnHistory->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnHistory)
		{
			btnClose->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnClose)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnPrint)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnUsers)
		{
			Msg.Result = 1;
			return;
		}
	}
	else if (Msg.CharCode == VK_UP)
	{
		if (ActiveControl == btnEditStock)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnEditContacts)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveInvoice)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnStocktake)
		{
			btnEditStock->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnTransfer)
		{
			btnEditContacts->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveOrder)
		{
			btnReceiveInvoice->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnManualSale)
		{
			btnStocktake->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnCreateOrder)
		{
			btnTransfer->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnHistory)
		{
			btnReceiveOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnClose)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnPrint)
		{
			btnClose->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnUsers)
		{
			btnPrint->SetFocus();
			Msg.Result = 1;
			return;
		}
	}
	else if (Msg.CharCode == VK_DOWN)
	{
		if (ActiveControl == btnEditStock)
		{
			btnStocktake->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnEditContacts)
		{
			btnTransfer->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveInvoice)
		{
			btnReceiveOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnStocktake)
		{
			btnManualSale->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnTransfer)
		{
			btnCreateOrder->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnReceiveOrder)
		{
			btnHistory->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnManualSale)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnCreateOrder)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnHistory)
		{
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnClose)
		{
			btnPrint->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnPrint)
		{
			btnUsers->SetFocus();
			Msg.Result = 1;
			return;
		}
		else if (ActiveControl == btnUsers)
		{
//			btnHistory->SetFocus();
			Msg.Result = 1;
			return;
		}
	}*/
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::FormKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	if (Shift.Contains(ssAlt) && Shift.Contains(ssCtrl) && Key == 0x5A)//VK_Z)
	{
		frmLogin->LoginMode = lmLoginAdmin;
		if (frmLogin->ShowModal() == mrOk)
		{
			std::auto_ptr<TfrmAdministration>frmAdministration(new TfrmAdministration(NULL));
			frmAdministration->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockMain::btnExportClick(TObject *Sender)
{
	if (CurrentConnection.AccountSystem == "")
	{
		Application->MessageBox("You need to configure accounts export under setup-export/import.", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{
        bool hasPermission = Security::AuthenticationService().HasPermission(
            frmLogin->CurrentUser.UserID,
            Security::MYOB);

        if (hasPermission == false)
        {
            Application->MessageBox("You are not authorized for this feature", "Error", MB_ICONERROR + MB_OK);
            return;
        }

		std::auto_ptr<TfrmMYOBPurchasesExport>frmMYOBPurchasesExport(new TfrmMYOBPurchasesExport(NULL));
		frmMYOBPurchasesExport->ShowModal();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockMain::WriteOffBtnClick(TObject *Sender)
{

    bool hasPermission = Security::AuthenticationService().HasPermission(
    frmLogin->CurrentUser.UserID,
            Security::WriteOff);

    if (hasPermission == false)
    {
        Application->MessageBox("You are not authorized for this feature", "Error", MB_ICONERROR + MB_OK);
        return;
    }



	std::auto_ptr<TfrmWriteOffStock>frmWriteOffStock(new TfrmWriteOffStock(this));
	frmWriteOffStock->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockMain::btnRecipesMouseClick(TObject *Sender)
{
    bool hasPermission = Security::AuthenticationService().HasPermission(
        frmLogin->CurrentUser.UserID,
        Security::Manufacture);

    if (hasPermission == false)
    {
        Application->MessageBox("You are not authorized for this feature", "Error", MB_ICONERROR + MB_OK);
        return;
    }



	std::auto_ptr<TfrmRecipeBox>frmRecipeBox(new TfrmRecipeBox(this));
	frmRecipeBox->ShowModal();

}
//---------------------------------------------------------------------------


void __fastcall TfrmStockMain::btnUpdateSupllierCostsClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size())
       {
         if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowUpdateSupplierCost))
             {
                Application->MessageBox(
                "You do not have permission to edit or add stock items.",
                "Error", MB_ICONERROR | MB_OK);
               return;
             }
        }
        frmSelectSupplier->Reset = true;
     	if (frmSelectSupplier->ShowModal() == mrOk)
	     {
                std::auto_ptr<TfrmUpdateSupplierCosts>frmUpdateSupplierCosts(new TfrmUpdateSupplierCosts(this));
		frmUpdateSupplierCosts->ContactKey = frmSelectSupplier->ContactKey;
		frmUpdateSupplierCosts->ShowModal();
	    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockMain::btnStockRequestClick(TObject *Sender)
{
 if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowStockRequest)) {
            Application->MessageBox(
              "You do not have permission to create stock requests.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }        ;

  	std::auto_ptr<TfrmStockRequestHistory> frmStockRequestHistory(new TfrmStockRequestHistory(NULL));
    try
    {
        if (frmStockRequestHistory->ShowModal() == mrOk)
        { //Do Nothing
        }
    }
    catch(Exception &E)
    {
        Application->ShowException(&E);
        return;
    }
}

void __fastcall TfrmStockMain::btnSubstituteClick(TObject *Sender)
{
  if (Security::AuthenticationService().GetRoles().size()) {
        if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DisallowStockSubstitute)) {
            Application->MessageBox(
              "You do not have permission to substitute stock.",
              "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }        ;

  	std::auto_ptr<TfrmStockSubstitute> frmStockSubstitute(new TfrmStockSubstitute(NULL));
    try
    {
        frmStockSubstitute->ShowModal();
    }
    catch(Exception &E)
    {
        Application->ShowException(&E);
        return;
    }
}
//---------------------------------------------------------------------------
