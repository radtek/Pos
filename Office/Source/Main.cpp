//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"
#include "Main.h"
#include "Splash.h"
#include "StockData.h"
#include "MMData.h"
#include "ChefMateData.h"
#include "Reports.h"
#include "Backup.h"
#include "MenuEdit.h"
#include "StockMain.h"
#include "Login.h"
#include "Setup.h"
#include "PayrollExport.h"
#include "CSV.h"
#include "Administration.h"
#include "MenuEditor.h"
#include "SelectMenu.h"
#include "DebtorsMain.h"
#include "AuthenticationService.h"

#include "DebtorInvoices.h"
#include "SelectDebtor.h"
#include "Registration.h"
#include "BetaAgreement.h"

#include "StockReportData.h"
#include "MMReportData.h"
#include "ExportForm.h"
#include "CashupForm.h"
#include <dbxdsnint.h>          

#include <Registry.hpp>
#include "MMRegistry.h"
#include "Consts.h"
#include "RemoteHostManager.h"
#include "XeroIntegration.h"

#include <list>
#pragma link "TouchBtn"
#pragma link "pngimage"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "TouchControls"

#include <cassert>

USERES("Office.res");

//USEFORM("Splash.cpp", frmSplash);
//USEFORM("Setup.cpp", frmSetup);
USEFORM("Reports.cpp", frmReports);
USEFORM("Backup.cpp", frmBackup);
USEFORM("Utilities.cpp", dmUtilities); /* TDataModule: File Type */
//USEFORM("MMData.cpp", dmMMData); /* TDataModule: DesignClass */
//USEFORM("Dialog.cpp", frmDialog);
//USEFORM("MenuEdit.cpp", frmMenuEdit);
USEFORM("Main.cpp", frmMain);
USEUNIT("MMRegistry.cpp");
//USEFORM("Preview.cpp", frmPreview);
USEFORM("Users.cpp", frmUsers);
//USEFORM("AddUser.cpp", frmAddUser);
USEFORM("Login.cpp", frmLogin);
//USEFORM("SelectMenu.cpp", frmSelectMenu);
//USEFORM("PayrollExport.cpp", frmPayrollExport);
USEFORM("GetBarcode.cpp", frmGetBarcode);
USEFORM("Administration.cpp", frmAdministration);
//USEFORM("SelectCategory.cpp", frmSelectCategory);
//USEFORM("SelectMenuItem.cpp", frmSelectMenuItem);

//USEFORM("Stock\ViewTrans.cpp", frmViewTrans);
//USEFORM("Stock\AddStock.cpp", frmAddStock);
//USEFORM("Stock\PurchaseOrder.cpp", frmPurchaseOrder);
//USEFORM("Stock\CreateOrders.cpp", frmCreateOrders);
//USEFORM("Stock\StockData.cpp", dmStockData); /* TDataModule: DesignClass */
//USEFORM("Stock\Stocktake.cpp", frmStocktake);
//USEFORM("Stock\StocktakeProfiles.cpp", frmStockGroupVisible);
USEFORM("Stock\SelectDate.cpp", frmSelectDate);
USEFORM("Stock\StockMain.cpp", frmStockMain);
USEFORM("Stock\SelectStockItem.cpp", frmSelectStockItem);
USEFORM("Stock\SelectPurchaseOrder.cpp", frmSelectPurchaseOrder);
//USEFORM("Stock\ReceiveOrder.cpp", frmReceiveOrder);
//USEFORM("Stock\Invoices.cpp", frmInvoices);
//USEFORM("Stock\ReceiveStockItem.cpp", frmReceiveStockItem);
USEFORM("Stock\SelectSupplier.cpp", frmSelectSupplier);
USEFORM("Stock\Stock.cpp", frmStock);
USEFORM("Stock\Contacts.cpp", frmSuppliers);
//USEFORM("Stock\PurchaseOrders.cpp", frmPurchaseOrders);
//USEFORM("Stock\AddSupplierUnit.cpp", frmAddSupplierUnit);
//USEFORM("Stock\SelectLocation.cpp", frmSelectLocation);
//USEFORM("Stock\CreateProfile.cpp", frmCreateProfile);
//USEFORM("Stock\TransferMulti.cpp", frmTransferMulti);
//USEFORM("Stock\AddContact.cpp", frmAddContact);
//USEFORM("Stock\AddContact.cpp", frmAddContact);
//USEFORM("Stock\ReceiveInvoice.cpp", frmReceiveInvoice);
//USEFORM("Stock\MYOBPurchasesExport.cpp", frmMYOBPurchasesExport);
USEFORM("Stock\SetupPurchaseOrders.cpp", frmSetupPurchaseOrders);
//USEFORM("Stock\AddStockCategoryGroup.cpp", frmAddStockCategoryGroup);
//USEFORM("Stock\SelectSupplierStockItem.cpp", frmSelectSupplierStockItem);

USEFORM("Reports\Reports.cpp", frmReports);
USEFORM("Reports\StockReportData.cpp", dmStockReportData); /* TDataModule: File Type */
USEFORM("Reports\MMReportData.cpp", dmMMReportData); /* TDataModule: File Type */

USEFORM("Debtors\DebtorsMain.cpp", frmDebtorsMain);
USEFORM("Debtors\SelectDebtor.cpp", frmSelectDebtor);
USEFORM("Debtors\Debtors.cpp", frmDebtors);
USEFORM("Debtors\DebtorInvoices.cpp", frmDebtorInvoices);

TfrmMain *frmMain;

const char ConnectionErrorStr[] = {"Office was unable to attach to the database.\rRestart Office to try again or check the settings in the Setup page."};
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
  : TForm(Owner)
{
	JustBooted = true;
	RegisterMidasLib(::DllGetDataSnapClassObject);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	if (JustBooted)
	{
		frmSetup		= new TfrmSetup(Application);
		frmSplash	= new TfrmSplash(Application);

		frmSplash->Show();
		frmSplash->Repaint();
		try
		{
		 	dmMMData		= new TdmMMData(Application);
			dmStockData	    = new TdmStockData(Application);
            dmChefMateData  = new TdmChefMateData(Application);
		}
		catch (Exception &E)
		{
			Application->ShowException(&E);
			Application->Terminate();
			return;
		}
		if (!frmSetup->InitialiseCompanies())
		{
			Application->Terminate();
			return;
		}
		if (dmStockData->dbStock->Connected)
		{
			if (!dmStockData->UpdateDB(frmSplash->lbeStatus))
			{
				Application->Terminate();
				return;
			}
		}
		try
		{
			lbeVersion->Caption = frmSplash->lbeVersion->Caption;

			AnsiString RegisteredName = "";
			bool Registered = false;
           dmMMData->Registered(&Registered, NULL, &RegisteredName);
           
		    if (IsDisplayOfficePath() && IsPosRegistered())
            {
               
               frmSplash->lbeRegistration->Caption = "Registered to " + DefaultCompany;
			}
			else
			{
				frmSplash->lbeRegistration->Caption = "Evaluation Version";
			}
			frmSplash->Repaint();

//			CreateForms();
			dmStockReportData = new TdmStockReportData(Application);
			dmMMReportData = new TdmMMReportData(Application);
//			Application->CreateForm(__classid(TdmStockReportData), &dmStockReportData);
//			Application->CreateForm(__classid(TdmMMReportData), &dmMMReportData);

			Application->CreateForm(__classid(TdmUtilities), &dmUtilities);
			Application->CreateForm(__classid(TfrmLogin), &frmLogin);
			Application->CreateForm(__classid(TfrmStockMain), &frmStockMain);
			Application->CreateForm(__classid(TfrmUsers), &frmUsers);
			Application->CreateForm(__classid(TfrmMenuEdit), &frmMenuEdit);

//			Application->CreateForm(__classid(TfrmAddUser), &frmAddUser);
//			Application->CreateForm(__classid(TfrmAddStock), &frmAddStock);
//			Application->CreateForm(__classid(TfrmAddContact), &frmAddContact);
//			Application->CreateForm(__classid(TfrmStockGroupVisible), &frmStockGroupVisible);
//			Application->CreateForm(__classid(TfrmStocktake), &frmStocktake);
//			Application->CreateForm(__classid(TfrmPurchaseOrder), &frmPurchaseOrder);
//			Application->CreateForm(__classid(TfrmCreateOrders), &frmCreateOrders);
//			Application->CreateForm(__classid(TfrmAddPurchaseStock), &frmAddPurchaseStock);
//			Application->CreateForm(__classid(TfrmReceiveInvoice), &frmReceiveInvoice);
//			Application->CreateForm(__classid(TfrmSelectMenu), &frmSelectMenu);
			Application->CreateForm(__classid(TfrmSelectStockItem), &frmSelectStockItem);
			Application->CreateForm(__classid(TfrmSelectPurchaseOrder), &frmSelectPurchaseOrder);
//			Application->CreateForm(__classid(TfrmReceiveOrder), &frmReceiveOrder);
//			Application->CreateForm(__classid(TfrmInvoices), &frmInvoices);
//			Application->CreateForm(__classid(TfrmReceiveStockItem), &frmReceiveStockItem);
			Application->CreateForm(__classid(TfrmSelectSupplier), &frmSelectSupplier);
			Application->CreateForm(__classid(TfrmStock), &frmStock);
			Application->CreateForm(__classid(TfrmSuppliers), &frmSuppliers);
//			Application->CreateForm(__classid(TfrmAddContact), &frmAddContact);
//			Application->CreateForm(__classid(TfrmPurchaseOrders), &frmPurchaseOrders);
//			Application->CreateForm(__classid(TfrmViewTrans), &frmViewTrans);
//			Application->CreateForm(__classid(TfrmTransfer), &frmTransfer);
//			Application->CreateForm(__classid(TfrmTransferMulti), &frmTransferMulti);
//			Application->CreateForm(__classid(TfrmAddSupplierUnit), &frmAddSupplierUnit);
//			Application->CreateForm(__classid(TfrmSelectLocation), &frmSelectLocation);
//			Application->CreateForm(__classid(TfrmSelectCategory), &frmSelectCategory);
//			Application->CreateForm(__classid(TfrmSelectSupplierStockItem), &frmSelectSupplierStockItem);
//			Application->CreateForm(__classid(TfrmCreateProfile), &frmCreateProfile);
//			Application->CreateForm(__classid(TfrmAdministration), &frmAdministration);
//			Application->CreateForm(__classid(TfrmMYOBPurchasesExport), &frmMYOBPurchasesExport);
			Application->CreateForm(__classid(TfrmSetupPurchaseOrders), &frmSetupPurchaseOrders);
//			Application->CreateForm(__classid(TfrmAddStockCategoryGroup), &frmAddStockCategoryGroup);
//			Application->CreateForm(__classid(TfrmSelectMenuItem), &frmSelectMenuItem);

//			Application->CreateForm(__classid(TfrmDialog), &frmDialog);
			Application->CreateForm(__classid(TfrmSelectDate), &frmSelectDate);

			Application->CreateForm(__classid(TfrmBackup), &frmBackup);
			Application->CreateForm(__classid(TfrmGetBarcode), &frmGetBarcode);
//			Application->CreateForm(__classid(TfrmPreview), &frmPreview);
			Application->CreateForm(__classid(TfrmReports), &frmReports);


//			Application->CreateForm(__classid(TfrmPayrollExport), &frmPayrollExport);

			Application->CreateForm(__classid(TfrmDebtorsMain), &frmDebtorsMain);
			Application->CreateForm(__classid(TfrmSelectDebtor), &frmSelectDebtor);
			Application->CreateForm(__classid(TfrmDebtors), &frmDebtors);
			Application->CreateForm(__classid(TfrmDebtorInvoices), &frmDebtorInvoices);
		}
		catch (Exception &E)
		{
			Application->ShowException(&E);
			Application->Terminate();
			return;
		}
		frmSplash->Close();

		JustBooted = false;
#ifdef ALPHA
		Application->MessageBox("This is an alpha version. NOT FOR COMMERCIAL USE!", "Alpha", MB_OK + MB_ICONINFORMATION);
#endif
#ifdef BETA
		TfrmBetaAgreement *frmBetaAgreement = new TfrmBetaAgreement(NULL);
		try
		{
			if (frmBetaAgreement->ShowModal() != mrOk)
			{
				Application->Terminate();
			}
		}
		__finally
		{
			delete frmBetaAgreement;
		}
#endif
	}
	// Set the Hint Pause.
	Application->HintPause = 1000;
	Application->HintHidePause = 9000;

	LongTimeFormat = ShortTimeFormat;

	bool Registered;
	unsigned int Modules;
	dmMMData->Registered(&Registered, &Modules);
	/*if (Registered)
	{
		//btnStock->Enabled = !(Modules & eMenuMateDirect);
		btnLoyalty->Enabled = (Modules & eRegMembers);
	}
	else
	{
		btnStock->Enabled = false;
		btnLoyalty->Enabled = false;
		btnReports->Enabled = false;
		btnBackup->Enabled = false;
		btnWages->Enabled = false;
	} */
    btnLoyalty->Enabled = true;
    btnStock->Enabled = true;
    btnLoyalty->Enabled = true;
    btnReports->Enabled = true;
    btnBackup->Enabled = true;
    btnWages->Enabled = true;
		//................................

		InitXeroIntegration();

		tiFailedXeroInvoices->Interval = FailedXeroInvoivesToSend() ? TXeroIntegration::Instance().FailedActiveInterval : TXeroIntegration::Instance().FailedPauseInterval;
		tiFailedXeroInvoices->Enabled  = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::WMDisplayChange(TWMDisplayChange& Message)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	dmMMData->CloseDB();
	dmStockData->CloseDB();
    dmChefMateData->CloseDB();
	dmMMData->Disconnect();
	dmStockData->Disconnect();
    dmChefMateData->Disconnect();

	if (frmSplash)
	{
		frmSplash->Close();
		//delete frmSplash;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormKeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::btnEditMenusMouseClick(TObject *Sender)
{
	bool Continue = false;
	if (dmStockData->dbStock->Connected)
	{
		frmLogin->LoginMode = lmLogin;
		if (frmLogin->ShowModal() == mrOk)
		{
            TTaxProfileManager taxProfileManager;
            TaxProfileMap taxProfileMap = taxProfileManager.LoadTaxProfiles();
            if( taxProfileMap.size()==0)
            {
               Application->MessageBox("Please Add Tax Profiles first.", "Error", MB_ICONERROR + MB_OK);
               return;
            }

			Continue = Security::AuthenticationService().HasPermission(
				frmLogin->CurrentUser.UserID,
				Security::MenuEditor);

			if (Continue == false)
				Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
		}
	}
	else
	{
		Continue = true;
	}
	if (Continue)
	{
		Visible = false;
		try
		{
			frmMenuEdit->Execute();
		}
		__finally
		{
			Visible = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnReportsMouseClick(TObject *Sender)
{
	if (dmStockData->dbStock->Connected)
	{
		frmLogin->LoginMode = lmLogin;
		if (frmLogin->ShowModal() == mrOk)
		{
			bool hasPermission = Security::AuthenticationService().HasPermission(
				frmLogin->CurrentUser.UserID,
				Security::Reports);

			if (hasPermission == false)
			{
				Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
				return;
			}

			Visible = false;
			try
			{
				frmReports->ShowModal();
			}
			__finally
			{
				Visible = true;
			}
		}
/*
		frmLogin->LoginMode = lmLogin;
		if (frmLogin->ShowModal() == mrOk)
		{
			Visible = false;
			try
			{
				frmReports->ShowModal();
			}
			__finally
			{
				Visible = true;
			}
		}*/
	}
	else if (dmMMData->dbMenuMate->Connected) // Need stock db to login so just show anyway. Not very secure!
	{
		Visible = false;
		try
		{
			frmReports->ShowModal();
		}
		__finally
		{
			Visible = true;
		}
	}
	else
	{
		Application->MessageBox(ConnectionErrorStr, "Error", MB_ICONERROR + MB_OK);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStockMouseClick(TObject *Sender)
{
	if (dmStockData->dbStock->Connected)
	{
		frmLogin->LoginMode = lmLogin;
		if (frmLogin->ShowModal() == mrOk)
		{
			bool hasPermission = Security::AuthenticationService().HasPermission(
				frmLogin->CurrentUser.UserID,
				Security::StockAdmin);

			if (hasPermission == false)
			{
				Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
				return;
			}

			Visible = false;
			try
			{
				frmStockMain->ShowModal();
			}
			__finally
			{
				Visible = true;
			}
		}
	}
	else
	{
		Application->MessageBox(ConnectionErrorStr, "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnLoyaltyMouseClick(TObject *Sender)
{
	if (dmStockData->dbStock->Connected)
	{
		frmLogin->LoginMode = lmLogin;
		if (frmLogin->ShowModal() == mrOk)
		{
			bool hasPermission = Security::AuthenticationService().HasPermission(
				frmLogin->CurrentUser.UserID,
				Security::Loyalty);

			if (hasPermission == false)
			{
				Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
				return;
			}

			frmDebtorsMain->ShowModal();
		}
	}
	else if (dmMMData->dbMenuMate->Connected) // Need stock db to login so just show anyway. Not very secure!
	{
		frmDebtorsMain->ShowModal();
	}
	else
	{
		Application->MessageBox(ConnectionErrorStr, "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnBackupMouseClick(TObject *Sender)
{
	if (!FileExists(ExtractFilePath(Application->ExeName) + "UnzDll.dll") ||
		 !FileExists(ExtractFilePath(Application->ExeName) + "ZipDll.dll"))
	{
		Application->MessageBox("The files UnzDll.dll and ZipDll.dll are required for backing up.", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{
		frmBackup->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnWagesMouseClick(TObject *Sender)
{
	frmLogin->LoginMode = lmLogin;
	if (frmLogin->ShowModal() == mrOk)
	{
		bool hasPermission = Security::AuthenticationService().HasPermission(
			frmLogin->CurrentUser.UserID,
			Security::AccountsExport);

		if (hasPermission == false)
		{
			Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
			return;
		}

		std::auto_ptr<TfrmExport> frm(new TfrmExport());
		frm->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSetupMouseClick(TObject *Sender)
{
    if (Security::AuthenticationService().GetRoles().size()) {
        if (frmLogin->LoginMode = lmLogin, frmLogin->ShowModal() != mrOk)
            return;
        else if (Security::AuthenticationService().HasPermission(
          frmLogin->CurrentUser.UserID, Security::DontAllowOfficeSetup)) {
            Application->MessageBox("You are not authorized for this area",
                                    "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

    frmSetup->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnExitMouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	frmSplash->lbeStatus->Caption = "Shutting Down...";
	frmSplash->lbeVersion->Caption = "";
	frmSplash->Show();
	frmSplash->Repaint();

	Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCashupMouseClick(TObject *Sender)
{
	frmLogin->LoginMode = lmLogin;
	if (frmLogin->ShowModal() == mrOk)
	{
		bool hasPermission = Security::AuthenticationService().HasPermission(
			frmLogin->CurrentUser.UserID,
			Security::CashUp);

		if (hasPermission == false)
		{
			Application->MessageBox("You are not authorized for this area", "Error", MB_ICONERROR + MB_OK);
			return;
		}

		std::auto_ptr<TfrmCashup> frm(new TfrmCashup());
		frm->ShowModal();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tiFailedXeroInvoicesTimer(TObject *Sender)
{
	tiFailedXeroInvoices->Enabled = false;

	//.................................

	if( FailedXeroInvoivesToSend() )
	{
		SendFailedXeroInvoices();
	}

	//.................................

	tiFailedXeroInvoices->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmMain::InitXeroIntegration()
{
	TXeroIntegration::Instance().XeroMachineName = LocalHost();
	TXeroIntegration::Instance().XeroFolderPath  = ReadXeroFolderPath();
	TXeroIntegration::Instance().XeroUserName    = "";
	TXeroIntegration::Instance().XeroPassword    = "";

	TXeroIntegration::Instance().Validate();
}
//---------------------------------------------------------------------------

AnsiString TfrmMain::LocalHost()
{
	return TRemoteHostManager::Instance().LocalHost();
}
//---------------------------------------------------------------------------

AnsiString TfrmMain::ReadXeroFolderPath()
{
    AnsiString result = "";

    //:::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
        AnsiString Key = OfficeKey + "\\" + ReadCompanyName() + "\\MYOB";
        RegistryRead( Key, "MYOBPath", result );
	}
	__finally
	{
 	}

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
AnsiString TfrmMain::ReadCompanyName()
{
    AnsiString result = "";

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    TStringList *companyNames = new TStringList;

	try
	{
		if( RegistryGetKeys( OfficeKey, companyNames ) && ( companyNames->Count > 0 ) )
		{
            result = companyNames->Strings[0];
		}
	}
	__finally
	{
		delete companyNames;
 	}

    //:::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

bool TfrmMain::FailedXeroInvoivesToSend()
{
	return TXeroIntegration::Instance().FailedInvoicesInQueue;
}
//---------------------------------------------------------------------------

void TfrmMain::SendFailedXeroInvoices()
{
	ResetFailedXeroInvoiceTimerInterval( TXeroIntegration::Instance().SendNextFailedXeroInvoice() );
}
//---------------------------------------------------------------------------

void TfrmMain::ResetFailedXeroInvoiceTimerInterval( unsigned inInvoiceCount )
{
	if( TXeroIntegration::Instance().XeroErrorMessage == XI_ERROR_NO_ERROR )
	{
		tiFailedXeroInvoices->Interval = inInvoiceCount > 0 ? TXeroIntegration::Instance().FailedActiveInterval : TXeroIntegration::Instance().FailedPauseInterval;
	}
	else
	{
		tiFailedXeroInvoices->Interval = TXeroIntegration::Instance().FailedFolderNotFoundInterval;
	}
}
//---------------------------------------------------------------------------
bool TfrmMain:: IsDisplayOfficePath()
{
  try
   {
     bool Isdatabasepathcorrect = false;
     AnsiString value;
	 RegistryRead(OfficeKey, "DefaultCompany", DefaultCompany);
     AnsiString Text;
     AnsiString Key = OfficeKey + "\\" + DefaultCompany;
     RegistryRead(Key,"IsOfficeConnected",value);
     if(value == "1")
        Isdatabasepathcorrect = true;
     return Isdatabasepathcorrect;
    }
    catch(Exception &E)
    {
       throw;
    }


}
//-------------------------------------------------------------------------------------------------------------
bool TfrmMain::IsPosRegistered()
{
    bool RetVal = false;
    try
    {
      qrComflag->Transaction->StartTransaction();
	  qrComflag->Close();
      qrComflag->SQL->Text = "SELECT VARIABLES_KEY FROM VARSPROFILE WHERE VARIABLES_KEY = :VARIABLES_KEY AND INTEGER_VAL = 1 ";
      qrComflag->ParamByName("VARIABLES_KEY")->AsInteger = 2302;
      qrComflag->ExecQuery();
      if(qrComflag->RecordCount)
       {
            RetVal = true;
       }
          qrComflag->Transaction->Commit();
    }
    catch(Exception &E)
    {
       throw;
    }
      return  RetVal;

}






