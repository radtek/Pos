//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "Setup.h"
#include "Printing.h"
#include "MMLogging.h"
#include "Main.h"
#include "Pause.h"
#include "Comms.h"
#include <Printers.hpp>
#include "MMRegistry.h"
#include "DbMod.h"
#include "Processing.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include <FileCtrl.hpp>
#include "MMMessageBox.h"
#include "ManagerPhysicalPrinter.h"
#include "ReportDisplay.h"
#include "SerialConfig.h"
#include "ScaleControler.h"
#include "ProxReader.h"
#include "GUIScale.h"
#include <string>
#include "TaxProfile.h"
#include "MallExportUpdateAdaptor.h"

#include "MMCustomerDisplayManager.h"
#include "CashDrawerPortsList.h"

#include "MallExportResendReport.h"
#include "MallExportManager.h"
#include "MallExportRegenerateReport.h"
#include "RegenerateMallReport.h"
#include "PhoenixHotelSystem.h"
#include "MallExportSalesTypeAssignment.h"
#include "Mall.h"
#include "GUIDiscount.h"
#include "MallSalesTypeAssignment.h"
#include "EnableFloorPlan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmSetup::TfrmSetup(TComponent* Owner,Database::TDBControl &inIBDatabase)
	: TZForm(Owner), IBDatabase(inIBDatabase)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormCreate(TObject *Sender)
{
// Scaling Code......................
	// Load up the Header & footers for the printers...

     //	int Temp = ((TForm *)Sender)->Tag;
     //	((TForm *)Sender)->Tag = Screen->Width;
     //	((TForm *)Sender)->ScaleBy(Screen->Width, Temp);

	PageControl->ActivePage = tsPrinters;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::imgCloseClick(TObject *Sender)
{
    if(cbNewMallLoc->ItemIndex > 1)
    {
        UpdateMallInfo();
        Close();
    }
    else if(cbNewMallLoc->ItemIndex == 1)
    {
        if(edTaxRate->Text != "")
        {
            UpdateMallInfo();
            Close();
        }
        else
        {
            MessageBox("Please Enter Tax Rate", "Error",MB_OK + MB_ICONERROR);
        }
    }
    else
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormClose(TObject *Sender, TCloseAction &Action)
{
	try
	{
		bool AllowChange;
		PageControlChanging(NULL, AllowChange);
	}
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
      Tag = Screen->Width;
      if((double)Screen->Width / Screen->Height < 1.4)
      {
   			ScaleBy(Screen->Width, Temp);
      }

   }
   Width = Screen->Width;
   Height = Screen->Height;
   Top = 0;
   Left = 0;
   Panel6->Height = Panel6->Height - 10;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::FormShow(TObject *Sender)
{
    // Load up the Header & footers for the printers...
    FormResize(Sender);
    RestaurantLayoutsChanged = false;
    PageControl->ActivePage = tsPrinters;
    PageControlChange(Sender);
    loadCashDrawerPorts();
    UpdateLists();

    bool SecondarySwipe = false;

    // For Mall Export
    InitializeMallExport();

    edTopLine->Text = TGlobalSettings::Instance().PoleDisplayTopLine;
    edBottomLine->Text = TGlobalSettings::Instance().PoleDisplayBottomLine;

    rgMembershipType->ItemIndex = TGlobalSettings::Instance().MembershipType;
    cbBarcodeFormat->ItemIndex =  TGlobalSettings::Instance().BarcodeFormat;
    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();
    edSerialKickCharsCount->Text = IntToStr(TManagerVariable::Instance().GetInt(DBTransaction,vmSerialKickPortLength,30));
    cbUseHighChars->Checked =  TManagerVariable::Instance().GetBool(DBTransaction,vmSerialKickPortHighChars);

    DBTransaction.Commit();

    lbVersion->Caption = "Version : " + GetFileVersionString();

	switch(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber)
	{
		case -1:
		case 0 : cbPoleDisplay->ItemIndex = 0;
		break;
		default:
		cbPoleDisplay->ItemIndex = TDeviceRealTerminal::Instance().PoleDisplay->PortNumber;
		break;
	}

	switch(TDeviceRealTerminal::Instance().PoleDisplay->EmulationMode)
	{
		case -1:
		case ePoleDispNone : cbPoleDisplayMode->ItemIndex = 0;
		break;
		case ePoleDispCD5220 : cbPoleDisplayMode->ItemIndex = 1; break;
		case ePoleDispEscPOS : cbPoleDisplayMode->ItemIndex = 2; break;
		case ePoleDispDSPUTC : cbPoleDisplayMode->ItemIndex = 3; break;
		case ePoleDispDSPEMaxAdex : cbPoleDisplayMode->ItemIndex = 4; break;
		case ePoleDispToshiba : cbPoleDisplayMode->ItemIndex = 5; break;
		default:
			cbPoleDisplayMode->ItemIndex = ePoleDispNone;
		break;
	}

   switch(TDeviceRealTerminal::Instance().ProxReader->PortNumber)
	{
   	case -1:
  		case 0 : cbProxReader->ItemIndex = 0;
      break;
      default:
  		cbProxReader->ItemIndex = TDeviceRealTerminal::Instance().ProxReader->PortNumber;
      break;
   }

   switch(TDeviceRealTerminal::Instance().ProxReader->ProxFormat)
	{
  		case eproxStd : cbProxFormat->ItemIndex = 0;
      break;
  		case eproxWiegand : cbProxFormat->ItemIndex = 1;
      break;
      default:
  		cbProxFormat->ItemIndex = 0;
      break;
   }

	switch(TDeviceRealTerminal::Instance().BarcodeReader->PortNumber)
	{
		case -1:
		case 0 : cbBarCodePort->ItemIndex = 0;
		break;
		default:
		cbBarCodePort->ItemIndex = TDeviceRealTerminal::Instance().BarcodeReader->PortNumber;
		break;
	}

	switch(TDeviceRealTerminal::Instance().SecurityPort->PortNumber)
	{
		case -1:
		case 0 : cbSecuritySerialPort->ItemIndex = 0;
		break;
		default:
		cbSecuritySerialPort->ItemIndex = TDeviceRealTerminal::Instance().SecurityPort->PortNumber;
		break;
	}

	switch(TDeviceRealTerminal::Instance().Scales->PortNumber)
	{
		case -1:
		case 0 : cbWeighScales->ItemIndex = 0;
		break;
		default:
		cbWeighScales->ItemIndex = TDeviceRealTerminal::Instance().Scales->PortNumber;
		break;
	}

    switch(TDeviceRealTerminal::Instance().Scales->ScaleStreamType)
	{
		case TypeA_STDStream : cbScalesType->ItemIndex = 0;
		    break;
		case TypeC_QUQA : cbScalesType->ItemIndex = 1;
		    break;
        case Type_AveryFxStream: cbScalesType->ItemIndex = 2;
            break;
        case Type_KenkoKK300W:
            cbScalesType->ItemIndex = 3;
            break;
		default:
			cbScalesType->ItemIndex = 0;
		break;
	}
	tbtnIPSettingsRefreshMouseClick(Sender);

    cbShowCustomerDisplay->Checked = TGlobalSettings::Instance().ShowCustomerDisplay;
    if(TDeviceRealTerminal::Instance().BasePMS->Registered)
    {
     cbNewbookType->Enabled  =true;
    }
    else
    {
       cbNewbookType->Enabled  =false;

       	TGlobalSettings::Instance().NewBook = 0;
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmNewBook,TGlobalSettings::Instance().NewBook);
		DBTransaction.Commit();
    }
      cbNewbookType->ItemIndex =   TGlobalSettings::Instance().NewBook;
   //load new malls
   SetupNewMalls();
}

//---------------------------------------------------------------------------

void TfrmSetup::UpdateLists()
{
	// Draw Kick
    setCashDrawerPort();

	// Pole Dispaly
	if(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber == -1)
	{
      cbPoleDisplay->ItemIndex = 0;
   }
   else
   {
      cbPoleDisplay->ItemIndex = TDeviceRealTerminal::Instance().PoleDisplay->PortNumber;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::PageControlChanging(TObject *Sender,
		bool &AllowChange)
{
	if (PageControl->ActivePage == tsPrinters)
	{
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmStockMasterExportPath, edStockMasterExport->Text);
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMembershipType,TGlobalSettings::Instance().MembershipType);
		DBTransaction.Commit();
	}
	UpdateLists();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbCashDrawPortChange(TObject *Sender)
{
	if(cbCashDrawPort->ItemIndex != -1)
   {
      TComms::Instance().SerialKickPortNumber = cbCashDrawPort->ItemIndex;
      // Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmSerialKickPort,TComms::Instance().SerialKickPortNumber);
		DBTransaction.Commit();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnSelectStockMasterPathClick(TObject *Sender)
{
   UnicodeString Dir;
   SelectDirectory(_T("Select Export Dir"),_T(""),Dir);
   edStockMasterExport->Text = IncludeTrailingPathDelimiter(Dir);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::PageControlChange(TObject *Sender)
{
	if (PageControl->ActivePage == tsPrinters)
	{
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();

		AnsiString Path = TManagerVariable::Instance().GetStr(DBTransaction,vmStockMasterExportPath);
		edStockMasterExport->Text = Path;

		rgMembershipType->ItemIndex = TGlobalSettings::Instance().MembershipType;
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnDemoModeClick(TObject *Sender)
{
	// Enter Demo Mode.
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbPoleDisplayChange(TObject *Sender)
{
	if(cbPoleDisplay->ItemIndex != -1)
   {
      if(cbPoleDisplay->ItemIndex != 0)
      {
			 TDeviceRealTerminal::Instance().PoleDisplay->PortNumber = cbPoleDisplay->ItemIndex;
      }
      else
      {
			TDeviceRealTerminal::Instance().PoleDisplay->PortNumber = -1;
      }
      // Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmPoleDisplayPort,TDeviceRealTerminal::Instance().PoleDisplay->PortNumber);
		DBTransaction.Commit();
   }

	if(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber != -1)
   {
      TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("Pole Display","","Active On ","COM " + IntToStr(cbPoleDisplay->ItemIndex));
   }
}
//---------------------------------------------------------------------------



void __fastcall TfrmSetup::edTopLineMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().PoleDisplayTopLine;
	frmTouchKeyboard->Caption = "Enter Top Line for Pole Display";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edTopLine->Text = frmTouchKeyboard->KeyboardText;
		TGlobalSettings::Instance().PoleDisplayTopLine = frmTouchKeyboard->KeyboardText;

		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPoleDisplayTopLine,TGlobalSettings::Instance().PoleDisplayTopLine);
		DBTransaction.Commit();

      TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay(edTopLine->Text,"","","");
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::edBottomLineMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().PoleDisplayBottomLine;
	frmTouchKeyboard->Caption = "Enter Bottom Line for Pole Display";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edBottomLine->Text = frmTouchKeyboard->KeyboardText;
      TGlobalSettings::Instance().PoleDisplayBottomLine = frmTouchKeyboard->KeyboardText;
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPoleDisplayBottomLine,TGlobalSettings::Instance().PoleDisplayBottomLine);
		DBTransaction.Commit();
      TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("","",edBottomLine->Text,"");
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::cbUseHighCharsClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSerialKickPortHighChars,cbUseHighChars->Checked);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------



void __fastcall TfrmSetup::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
   int Result = StrToInt(edSerialKickCharsCount->Text);
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmSerialKickPortLength,Result);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edStockMasterExportMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edStockMasterExport->Text;
	frmTouchKeyboard->Caption = "Enter StockMaster Export Path";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        if(frmTouchKeyboard->KeyboardText != "")
        {
			edStockMasterExport->Text = IncludeTrailingPathDelimiter(frmTouchKeyboard->KeyboardText);
        }
        else
        {
			edStockMasterExport->Text = frmTouchKeyboard->KeyboardText;
        }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbPoleDisplayModeChange(TObject *Sender)
{
	if(cbPoleDisplayMode->ItemIndex != -1)
   {
      if(cbPoleDisplayMode->ItemIndex == 0)
      {
      	TDeviceRealTerminal::Instance().PoleDisplay->EmulationMode = ePoleDispNone;
      }
      else
      {
      	TDeviceRealTerminal::Instance().PoleDisplay->EmulationMode = (emPoleDisplyEmulation)cbPoleDisplayMode->ItemIndex;
      }
      // Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmPoleDisplayEmulationMode,TDeviceRealTerminal::Instance().PoleDisplay->EmulationMode);
		DBTransaction.Commit();
   }

	if(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber != -1)
   {
      TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("Pole Display Mode",IntToStr(TDeviceRealTerminal::Instance().PoleDisplay->EmulationMode)
																	,"Active On ","COM " + IntToStr(cbPoleDisplay->ItemIndex));
   }

}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::cbProxReaderChange(TObject *Sender)
{
	if(cbProxReader->ItemIndex != -1)
   {
      if(cbProxReader->ItemIndex != 0)
      {
         TDeviceRealTerminal::Instance().ProxReader->PortNumber = cbProxReader->ItemIndex;
      }
      else
      {
         TDeviceRealTerminal::Instance().ProxReader->PortNumber = -1;
      }
      // Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
      TDeviceRealTerminal::Instance().ProxReader->SaveSettings(DBTransaction);
		DBTransaction.Commit();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbBarCodePortChange(TObject *Sender)
{
	if(cbBarCodePort->ItemIndex != -1)
   {
		if(cbBarCodePort->ItemIndex != 0)
      {
			TDeviceRealTerminal::Instance().BarcodeReader->PortNumber = cbBarCodePort->ItemIndex;
      }
		else
		{
			TDeviceRealTerminal::Instance().BarcodeReader->PortNumber = -1;
		}
		// Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmBarCodeReaderPort,TDeviceRealTerminal::Instance().BarcodeReader->PortNumber);
		DBTransaction.Commit();
	}

	if(TDeviceRealTerminal::Instance().BarcodeReader->PortNumber != -1)
	{
		TDeviceRealTerminal::Instance().BarcodeReader->Open(TDeviceRealTerminal::Instance().BarcodeReader->PortNumber);
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbSecuritySerialPortChange(TObject *Sender)
{
	if(cbSecuritySerialPort->ItemIndex != -1)
   {
		if(cbSecuritySerialPort->ItemIndex != 0)
		{
			TDeviceRealTerminal::Instance().SecurityPort->PortNumber = cbSecuritySerialPort->ItemIndex;
		}
		else
		{
			TDeviceRealTerminal::Instance().SecurityPort->PortNumber = -1;
		}
		// Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
      TDeviceRealTerminal::Instance().SecurityPort->SaveSettings(DBTransaction);
		DBTransaction.Commit();
   }

	if(TDeviceRealTerminal::Instance().SecurityPort->PortNumber != -1)
   {
		TDeviceRealTerminal::Instance().SecurityPort->Open(TDeviceRealTerminal::Instance().SecurityPort->PortNumber);
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnIPSettingsRefreshMouseClick(TObject *Sender)
{
	lbeMMDBIP->Caption = "MenuMate DB IP Address " + TGlobalSettings::Instance().InterbaseIP;
	lbeMMDBPath->Caption = "MenuMate DB Path " + TGlobalSettings::Instance().DatabasePath;
	lbeMemDBIP->Caption = "Membership DB IP Address " + TGlobalSettings::Instance().MembershipDatabaseIP;

	switch (TGlobalSettings::Instance().MembershipType) {
		case MembershipTypeMenuMate :
		case MembershipTypeERS :
		lbeMemDBPath->Caption = "Membership DB Path " + TGlobalSettings::Instance().MembershipDatabasePath;
		break;
		case MembershipTypeEBet :
		case MembershipTypeExternal :
		lbeMemDBPath->Caption = "Membership DB Port " + IntToStr(TGlobalSettings::Instance().MembershipDatabasePort);
		break;
	}

	lbeStockDBIP->Caption = "Stock DB IP Address " + TGlobalSettings::Instance().StockInterbaseIP;
	lbeStockDBPath->Caption = "Stock DB Path " + TGlobalSettings::Instance().StockDatabasePath;
	lbeSNTPIP->Caption = "SNTP IP Address " + TGlobalSettings::Instance().SntpIP;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnReconfigMMDBMouseClick(TObject *Sender)
{
	AnsiString OldIP = TGlobalSettings::Instance().InterbaseIP;
	AnsiString OldDB = TGlobalSettings::Instance().DatabasePath;
	RegistryWrite(MMBaseKey "Database","InterbaseIP","");
	RegistryWrite(MMBaseKey "Database","DatabasePath","");
	if(!TDeviceRealTerminal::Instance().OpenMenuMateDB())
	{
		RegistryWrite(MMBaseKey "Database","InterbaseIP",OldIP);
		RegistryWrite(MMBaseKey "Database","DatabasePath",OldDB);
		bool Proceed = TDeviceRealTerminal::Instance().OpenMenuMateDB();
		if(!Proceed) Application->Terminate();
	}
	tbtnIPSettingsRefreshMouseClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnReconfigMemDBMouseClick(TObject *Sender)
{
   if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"])
   {
       AnsiString OldIP 	= TGlobalSettings::Instance().MembershipDatabaseIP;
       AnsiString OldDB 	= TGlobalSettings::Instance().MembershipDatabasePath;
       int OldPort			= TGlobalSettings::Instance().MembershipDatabasePort;
       TGlobalSettings::Instance().MembershipDatabaseIP = "";
       TGlobalSettings::Instance().MembershipDatabasePort = 0;
       TGlobalSettings::Instance().MembershipDatabasePath = "";
       if(!TDeviceRealTerminal::Instance().OpenMembershipDB(true))
       {
           TGlobalSettings::Instance().MembershipDatabaseIP = OldIP;
           TGlobalSettings::Instance().MembershipDatabasePath = OldDB;
           TGlobalSettings::Instance().MembershipDatabasePort = OldPort;
           bool Proceed = TDeviceRealTerminal::Instance().OpenMembershipDB(true);
           if(!Proceed) Application->Terminate();
       }
       tbtnIPSettingsRefreshMouseClick(Sender);
   }
   else
   {
		MessageBox("System Not Registered for membership", "Not Registered",MB_OK + MB_ICONERROR);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnReconfigStockDBMouseClick(TObject *Sender)
{
	AnsiString OldIP = TGlobalSettings::Instance().StockInterbaseIP;
	AnsiString OldDB = TGlobalSettings::Instance().StockDatabasePath;
	TGlobalSettings::Instance().StockInterbaseIP = "";
	TGlobalSettings::Instance().StockDatabasePath = "";
	if(!TDeviceRealTerminal::Instance().OpenStockDB(true))
	{
		TGlobalSettings::Instance().StockInterbaseIP = OldIP;
		TGlobalSettings::Instance().StockDatabasePath = OldDB;
		bool Proceed = TDeviceRealTerminal::Instance().OpenStockDB(true);
		if(!Proceed)Application->Terminate();
	}
	tbtnIPSettingsRefreshMouseClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnReconfigSNTPIPMouseClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().SntpIP;
	frmTouchKeyboard->Caption = "Enter SNTP IP Address";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		TGlobalSettings::Instance().SntpIP = frmTouchKeyboard->KeyboardText;
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSntpIP,TGlobalSettings::Instance().SntpIP);
		DBTransaction.Commit();
		TDeviceRealTerminal::Instance().UpdateClockSyncInfo();
	}
	tbtnIPSettingsRefreshMouseClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnStatusReportMouseClick(TObject *Sender)
{
//
	std::auto_ptr<TfrmReportDisplay> frmReportDisplay(TfrmReportDisplay::Create<TfrmReportDisplay>(this));
   std::auto_ptr<TStringList> Report(new TStringList);
	TDeviceRealTerminal::Instance().StatusReport(Report.get());
	frmReportDisplay->Navigate(Report.get());
	frmReportDisplay->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnDebuggerTestMouseClick(TObject *Sender)
{
	int *X = NULL;
	*X = 45;
}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::ConfigureClick(TObject *Sender)
{
   try
   {
      std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
      frmSerialConfig->LoadSettings(TDeviceRealTerminal::Instance().SecurityPort->GetTComPort(),TDeviceRealTerminal::Instance().SecurityPort->AsyncMode);
      if(frmSerialConfig->ShowModal())
      {
         TDeviceRealTerminal::Instance().SecurityPort->Close();
         frmSerialConfig->AssignSettings(TDeviceRealTerminal::Instance().SecurityPort->GetTComPort(),TDeviceRealTerminal::Instance().SecurityPort->AsyncMode);
         TDeviceRealTerminal::Instance().SecurityPort->Open(TDeviceRealTerminal::Instance().SecurityPort->PortNumber);
      	Database::TDBTransaction DBTransaction(IBDatabase);
      	DBTransaction.StartTransaction();
         TDeviceRealTerminal::Instance().SecurityPort->SaveSettings(DBTransaction);
   		DBTransaction.Commit();
      }
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::Button1Click(TObject *Sender)
{
   TDeviceRealTerminal::Instance().PoleDisplay->ClearPoleDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbProxFormatChange(TObject *Sender)
{
	if(cbProxFormat->ItemIndex != -1)
   {
      TDeviceRealTerminal::Instance().ProxReader->ProxFormat = (eProxFormat)cbProxFormat->ItemIndex;
      // Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmProxWiegandFormat,(int)TDeviceRealTerminal::Instance().ProxReader->ProxFormat);
		DBTransaction.Commit();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbWeighScalesChange(TObject *Sender)
{
   if(cbWeighScales->ItemIndex != -1)
   {
		if(cbWeighScales->ItemIndex != 0)
		{
			TDeviceRealTerminal::Instance().Scales->PortNumber = cbWeighScales->ItemIndex;
		}
		else
		{
			TDeviceRealTerminal::Instance().Scales->PortNumber = -1;
		}
		// Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
      	TDeviceRealTerminal::Instance().Scales->SaveSettings(DBTransaction);
		DBTransaction.Commit();
   }

	if(TDeviceRealTerminal::Instance().Scales->PortNumber != -1)
   {
		if(!TDeviceRealTerminal::Instance().Scales->Open(TDeviceRealTerminal::Instance().Scales->PortNumber))
		{
			MessageBox(TDeviceRealTerminal::Instance().Scales->LastError, "Error",MB_OK + MB_ICONERROR);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnWeighScalesConfigClick(TObject *Sender)
{
	try
   {
      std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
      frmSerialConfig->LoadSettings(TDeviceRealTerminal::Instance().Scales->GetTComPort(),TDeviceRealTerminal::Instance().Scales->AsyncMode);
      if(frmSerialConfig->ShowModal())
      {
         TDeviceRealTerminal::Instance().Scales->Close();
			frmSerialConfig->AssignSettings(TDeviceRealTerminal::Instance().Scales->GetTComPort(),TDeviceRealTerminal::Instance().Scales->AsyncMode);
			if(!TDeviceRealTerminal::Instance().Scales->Open(TDeviceRealTerminal::Instance().Scales->PortNumber))
			{
				MessageBox(TDeviceRealTerminal::Instance().Scales->LastError, "Error",MB_OK + MB_ICONERROR);
			}
			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();
         TDeviceRealTerminal::Instance().Scales->SaveSettings(DBTransaction);
   		DBTransaction.Commit();
      }
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnProxSerialConfigClick(TObject *Sender)
{
   try
   {
      std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
      frmSerialConfig->LoadSettings(TDeviceRealTerminal::Instance().ProxReader->GetTComPort(),false);
      if(frmSerialConfig->ShowModal())
      {
         TDeviceRealTerminal::Instance().ProxReader->Close();
         bool Async = false;
         frmSerialConfig->AssignSettings(TDeviceRealTerminal::Instance().ProxReader->GetTComPort(),Async);
      	Database::TDBTransaction DBTransaction(IBDatabase);
      	DBTransaction.StartTransaction();
         TDeviceRealTerminal::Instance().ProxReader->SaveSettings(DBTransaction);
   		DBTransaction.Commit();
      }
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnTestScalesClick(TObject *Sender)
{
	 TItemComplete *Order = new TItemComplete;
	 Order->ItemOrderedFrom			= NULL;
	 Order->TableNo 					= 0;
	 Order->SeatNo 					= 0;
	 Order->ItemKey					= -1;
	 Order->Item						= "Test Item";
	 Order->ItemKitchenName			= "Test Item";
	 Order->Item_ID					= -1;
	 Order->MenuName					= "Test Menu";
	 // TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
	 Order->MenuKey					= -1;
	 Order->ItemType					= eFoodItem;
	 Order->PrintChitNumber 		= false;
	 Order->OrderType					= NormalOrder;
	 Order->HappyHour					= false;
	 Order->TimeStamp 				= Now();
	 Order->TransNo 					= -1;

	 Order->Note 				 		= "";
	 Order->Course       		 	= "Test Course";
	 Order->CourseKitchenName 		= "Test Course";
	 TServingCourse CurrentServingCourse;
	 Order->ServingCourse		 	= CurrentServingCourse;
	 Order->ItemAppearanceOrder	= 0;
	 Order->Loyalty_Key   	 		= -1;
	 Order->Size 						= "Test Size";
	 Order->SizeKitchenName     	= "Test Size";
	 //Order->GSTPercent          	= 15;
     Order->TaxProfiles.push_back(TaxProfile("Test Tax", 15, SalesTax, 1/*,0*/));
	 Order->PointsPercent       	= 0;
	 Order->SetMenuMask		   	= 0;
	 Order->SetMenu			      = 0;
	 Order->SetMenuMaster	      = 0;
	 Order->ThirdPartyKey	      = 0;
	 Order->ThirdPartyCode	      = 0;
	 Order->PLU					      = -1;

	 Order->MemberFreeSaleCount 		= 0;
	 Order->MemberFreeSaleDiscount 	= 0;
	 Order->LocationFreeSaleCount 	= 0;
	 Order->LocationFreeSaleDiscount = 0;

	 Order->PriceLevel0		= 1;
	 Order->PriceLevel1		= 1;
	 Order->SetPriceLevel0();

	 Order->Cost = .5; // Get default cost if assigned.
	 Order->CostGSTPercent = 15;
	 TWeight TareWeight;
	 TareWeight.SetWeightIn_g(10);
	 Order->WeightedPrice.AssignTareWeight(TareWeight);
	 TWeight Weight;
	 std::auto_ptr<TScaleController> ScalesControler (new TScaleController(this,TDeviceRealTerminal::Instance().Scales.get(),TDeviceRealTerminal::Instance().PoleDisplay,Order,TGlobalSettings::Instance().AutoConvertScalesWeights));
	 if(ScalesControler->GetWeight(Weight) != mrOk) // Menu Change while selecting glass.

	 delete Order;
	 Order = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::ConfigPoleDisplayClick(TObject *Sender)
{
   try
   {
      std::auto_ptr<TfrmSerialConfig> frmSerialConfig(TfrmSerialConfig::Create<TfrmSerialConfig>(this));
      frmSerialConfig->LoadSettings(TDeviceRealTerminal::Instance().PoleDisplay->GetTComPort(),TDeviceRealTerminal::Instance().PoleDisplay->AsyncMode);
      if(frmSerialConfig->ShowModal())
      {
         TDeviceRealTerminal::Instance().PoleDisplay->Close();
         frmSerialConfig->AssignSettings(TDeviceRealTerminal::Instance().PoleDisplay->GetTComPort(),TDeviceRealTerminal::Instance().PoleDisplay->AsyncMode);
         TDeviceRealTerminal::Instance().PoleDisplay->Open(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber);
      	Database::TDBTransaction DBTransaction(IBDatabase);
      	DBTransaction.StartTransaction();
         TDeviceRealTerminal::Instance().PoleDisplay->SaveSettings(DBTransaction);
   		DBTransaction.Commit();

         if(TDeviceRealTerminal::Instance().PoleDisplay->PortNumber != -1)
         {
            TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("Pole Display","","Active On ","COM " + IntToStr(cbPoleDisplay->ItemIndex));
         }
      }
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::rgMembershipTypeClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().MembershipType != rgMembershipType->ItemIndex)
    {
      MessageBox("You will need to restart MenuMate for this to take effect.", "Restart Required", MB_OK + MB_ICONINFORMATION);
    }
	TGlobalSettings::Instance().MembershipType = rgMembershipType->ItemIndex;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
    if(!(rgMembershipType->ItemIndex == MembershipTypeMenuMate && !TGlobalSettings::Instance().LoyaltyMateEnabled))
    {
        if(TGlobalSettings::Instance().UseMemberSubs)
           MessageBox("Member Subscription will be turned off with this functionality.", "Information", MB_OK + MB_ICONINFORMATION);
        TGlobalSettings::Instance().UseMemberSubs = false;
        TManagerVariable &mv = TManagerVariable::Instance();
        int pk;
        if (!(pk = mv.GetProfile(DBTransaction, eSystemProfiles, "Globals")))
        pk = mv.SetProfile(DBTransaction, eSystemProfiles, "Globals");
        mv.SetProfileBool(DBTransaction, pk, vmUseMemberSubs, TGlobalSettings::Instance().UseMemberSubs);
    }
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMembershipType,TGlobalSettings::Instance().MembershipType);
	DBTransaction.Commit();
}

//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbScalesTypeChange(TObject *Sender)
{
   if(cbScalesType->ItemIndex != -1)
   {
        switch(cbScalesType->ItemIndex)
        {
            case 0:
                TDeviceRealTerminal::Instance().Scales->ScaleStreamType = TypeA_STDStream;
                break;
            case 2:
                TDeviceRealTerminal::Instance().Scales->ScaleStreamType = Type_AveryFxStream;
                break;
            case 3:
                TDeviceRealTerminal::Instance().Scales->ScaleStreamType = Type_KenkoKK300W;
                break;
            default:
                TDeviceRealTerminal::Instance().Scales->ScaleStreamType = TypeC_QUQA;
                break;
        }
		// Update Serial Draw Kick Value in Reg.
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
      	TDeviceRealTerminal::Instance().Scales->SaveSettings(DBTransaction);
		DBTransaction.Commit();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbMallLocChange(TObject *Sender)
{
    ChangeMallType();
    lbBranchCode->Caption = RenameBranchCode();
    lbSerialNumber->Caption = RenameSerialNumber();
    lbTenantNo->Caption = RenameTenantNumber();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edTenantNoMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupTenantNumber();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edMallPathMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupMallPath();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edClassCodeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupClassCode();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edTradeCodeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupTradeCode();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edOutletCodeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupOutletCode();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edBranchCodeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupBranchCode();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edTerminalNoMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupTerminalNumber();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edSerialNoMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupSerialNumber();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edFTPServerMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupFTPServer();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edFTPPathMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupFTPPath();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edFTPUserNameMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupFTPUserName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edFTPPasswordMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupFTPPassword();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edConsolidatedDBPathsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    SetupConsolidatedPaths();
}
//---------------------------------------------------------------------------

void TfrmSetup::ChangeMallType()
{
    TGlobalSettings::Instance().MallIndex = cbMallLoc->ItemIndex;
//    SaveCompValueinDBInt(vmMallIndex, TGlobalSettings::Instance().MallIndex);
    SaveCompValueinDBIntUnique(vmMallIndex, TGlobalSettings::Instance().MallIndex);
    SetSpecificMall(cbMallLoc->ItemIndex);
    TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
    SaveCompValueinDBStr(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupTenantNumber()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 50;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().TenantNo;
	frmTouchKeyboard->Caption = "Enter " + RenameTenantNumber();
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edTenantNo->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().TenantNo = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmTenantNo, TGlobalSettings::Instance().TenantNo); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupMallPath()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().MallPath;
	frmTouchKeyboard->Caption = "Enter File Location";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edMallPath->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().MallPath = frmTouchKeyboard->KeyboardText;
        CheckMallPath();
        SaveCompValueinDBStrUnique(vmMallPath, TGlobalSettings::Instance().MallPath); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupClassCode()
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->SetMaxLengthValue(10);
    frmTouchNumpad->Mode = pmSTR;
    frmTouchNumpad->Caption = "Enter Class Code";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = false;
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->NUMSTRInitial = TGlobalSettings::Instance().ClassCode;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        edClassCode->Text = frmTouchNumpad->NUMSTRResult;
        TGlobalSettings::Instance().ClassCode = frmTouchNumpad->NUMSTRResult;
        SaveCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode); // See Function Description
    }

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupTradeCode()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 10;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().TradeCode;
	frmTouchKeyboard->Caption = "Enter Trade Code";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edTradeCode->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().TradeCode = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupOutletCode()
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->SetMaxLengthValue(10);
    frmTouchNumpad->Mode = pmSTR;
    frmTouchNumpad->Caption = "Enter Outlet Number";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = false;
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->NUMSTRInitial = TGlobalSettings::Instance().OutletCode;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        edOutletCode->Text = frmTouchNumpad->NUMSTRResult;
        TGlobalSettings::Instance().OutletCode = frmTouchNumpad->NUMSTRResult;
        SaveCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode);// See Function Description
    }

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupBranchCode()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 50;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().BranchCode;
    frmTouchKeyboard->Caption = "Enter " + RenameBranchCode();

	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edBranchCode->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().BranchCode = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmBranchCode, TGlobalSettings::Instance().BranchCode); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupTerminalNumber()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().TerminalNo;
	frmTouchKeyboard->Caption = "Enter Terminal Number";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edTerminalNo->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().TerminalNo = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupSerialNumber()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 30;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().SerialNo;
	frmTouchKeyboard->Caption = "Enter Serial Number";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edSerialNo->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().SerialNo = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupFTPServer()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().FTPServer;
	frmTouchKeyboard->Caption = "Enter FTP Server Name (e.g ftppath.com)";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edFTPServer->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().FTPServer = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupFTPPath()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().FTPPath;
	frmTouchKeyboard->Caption = "Enter FTP Path (e.g folder1/folder2)";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edFTPPath->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().FTPPath = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupFTPUserName()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().FTPUserName;
	frmTouchKeyboard->Caption = "Enter FTP User Name (e.g username)";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edFTPUserName->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().FTPUserName = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------
void TfrmSetup::SetupFTPPassword()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().FTPPassword;
	frmTouchKeyboard->Caption = "Enter FTP Password (e.g password)";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edFTPPassword->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().FTPPassword = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void TfrmSetup::SetupConsolidatedPaths()
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 1000;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ConsolidateReportPaths;
	frmTouchKeyboard->Caption = "Enter Consolidated Database Paths \n (e.g 192.168.0.1:C:\\Program Files\\MenuMate,)";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edConsolidatedDBPaths->Text = frmTouchKeyboard->KeyboardText;
        TGlobalSettings::Instance().ConsolidateReportPaths = frmTouchKeyboard->KeyboardText;
        SaveCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths); // See Function Description
	}

    SaveMESettings();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbEnableConsolidatedRepClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableConsolidatedReport = cbEnableConsolidatedRep->Checked;
    SaveCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport);
    edConsolidatedDBPaths->Enabled = cbEnableConsolidatedRep->Checked;
    edConsolidatedDBPaths->Color = edConsolidatedDBPaths->Enabled ? clWindow : clInactiveCaptionText;
}
//---------------------------------------------------------------------------

// This Function is for saving each string component into the DB for every function call
void TfrmSetup::SaveCompValueinDBStr(vmVariables vmVar, UnicodeString CompName)
{
 		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    DBTransaction.StartTransaction();
	    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	    if(GlobalProfileKey == 0)
	    {
		    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	    }
        TManagerVariable::Instance().SetProfileStr(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
}
//---------------------------------------------------------------------------

// This Function is for saving each integer component into the DB for every function call
void TfrmSetup::SaveCompValueinDBInt(vmVariables vmVar, int CompName)
{ 		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    DBTransaction.StartTransaction();
	    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	    if(GlobalProfileKey == 0)
	    {
		    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	    }
        TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::SaveCompValueinDBBool(vmVariables vmVar, bool CompName){    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }    TManagerVariable::Instance().SetProfileBool(DBTransaction, GlobalProfileKey, vmVar, CompName);
    DBTransaction.Commit();}//---------------------------------------------------------------------------void TfrmSetup::SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName)
{    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileStr(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    DBTransaction.Commit();}//---------------------------------------------------------------------------

void TfrmSetup::ClearCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName, TEdit *CompfrmName)
{    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileStr(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    CompfrmName->Clear();
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::SaveCompValueinDBIntUnique(vmVariables vmVar, int CompName)
{
    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileInt(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::ClearCompValueinDBIntUnique(vmVariables vmVar, int CompName, TEdit *CompfrmName)
{
    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileInt(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    CompfrmName->Clear();
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::SaveCompValueinDBBoolUnique(vmVariables vmVar, bool CompName)
{
    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileBool(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::ClearCompValueinDBBoolUnique(vmVariables vmVar, bool CompName, TCheckBox *CompfrmName)
{
    Database::TDBTransaction DBTransaction(IBDatabase);
    DBTransaction.StartTransaction();

    CompfrmName->Checked = CompName;
    TManagerVariable::Instance().SetProfileBool(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmSetup::ClearAllComponentValue()
{
    ClearCompValueinDBStrUnique(vmTenantNo, TGlobalSettings::Instance().TenantNo = "", edTenantNo);
    ClearCompValueinDBStrUnique(vmMallPath, TGlobalSettings::Instance().MallPath = "", edMallPath);
    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmBranchCode, TGlobalSettings::Instance().BranchCode = "", edBranchCode);
    ClearCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo = "", edTerminalNo);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}
//---------------------------------------------------------------------------

// This Function is for Enabling and Disabling the Text field of the components
void TfrmSetup::EnableFieldComponent(bool isMallPathSet, bool isTerminalNoSet, bool isClassCodeSet, bool isTradeCodeSet,
                                     bool isOutletCodeSet, bool isSerialNoSet, bool isTenantNoSet, bool isBranchCodeSet,
                                     bool isFTPServerSet, bool isFTPPathSet, bool isFTPUserNameSet, bool isFTPPasswordSet,
                                     bool isConsolidatedRepSet, bool isEnableConsolidatedRepSet)
{
    edMallPath->Enabled = isMallPathSet;
    edMallPath->Color = edMallPath->Enabled ? clWindow : clInactiveCaptionText;
    edTerminalNo->Enabled = isTerminalNoSet;
    edTerminalNo->Color = edTerminalNo->Enabled ? clWindow : clInactiveCaptionText;

    edClassCode->Enabled = isClassCodeSet;
    edClassCode->Color = edClassCode->Enabled ? clWindow : clInactiveCaptionText;
    edTradeCode->Enabled = isTradeCodeSet;
    edTradeCode->Color = edTradeCode->Enabled ? clWindow : clInactiveCaptionText;
    edOutletCode->Enabled = isOutletCodeSet;
    edOutletCode->Color = edOutletCode->Enabled ? clWindow : clInactiveCaptionText;
    edSerialNo->Enabled = isSerialNoSet;
    edSerialNo->Color = edSerialNo->Enabled ? clWindow : clInactiveCaptionText;

    edTenantNo->Enabled = isTenantNoSet;
    edTenantNo->Color = edTenantNo->Enabled ? clWindow : clInactiveCaptionText;
    edBranchCode->Enabled = isBranchCodeSet;
    edBranchCode->Color = edBranchCode->Enabled ? clWindow : clInactiveCaptionText;

    edFTPServer->Enabled = isFTPServerSet;
    edFTPServer->Color = edFTPServer->Enabled ? clWindow : clInactiveCaptionText;
    edFTPPath->Enabled = isFTPPathSet;
    edFTPPath->Color = edFTPPath->Enabled ? clWindow : clInactiveCaptionText;
    edFTPUserName->Enabled = isFTPUserNameSet;
    edFTPUserName->Color = edFTPUserName->Enabled ? clWindow : clInactiveCaptionText;
    edFTPPassword->Enabled = isFTPPasswordSet;
    edFTPPassword->Color = edFTPPassword->Enabled ? clWindow : clInactiveCaptionText;

    edConsolidatedDBPaths->Enabled = isConsolidatedRepSet;
    edConsolidatedDBPaths->Color = edConsolidatedDBPaths->Enabled ? clWindow : clInactiveCaptionText;
    cbEnableConsolidatedRep->Enabled = isEnableConsolidatedRepSet;
    cbEnableConsolidatedRep->Color = cbEnableConsolidatedRep->Enabled ? clBtnFace : clInactiveCaptionText;
}
//---------------------------------------------------------------------------

// This Function is for enabling/disabling and clearing the DB for setting specific mall
void TfrmSetup::SetSpecificMall(int MallIdx)
{
    switch(MallIdx)
    {
        case SMMALL:
        EnableSMComponents();
        break;

        case ROBINSONMALL:
        EnableRobinsonComponents();
        break;

        case AYALAMALL:
        EnableAyalaComponents();
        break;

        case POWERPLANTMALL:
        EnablePowerPlantComponents();
        break;

        case CAPITALAND:
        EnableCapitalandComponents();
        break;

        case ALPHALANDMALL:
        EnableAlphalandComponents();
        break;

        case MEGAWORLDMALL:
        EnableMegaworldComponents();
        break;

        case SHANGRILAMALL:
        EnableShangrilaComponents();
        break;

        case DLFMALL:
        EnableDLFMallComponents();
        break;

        case FEDERALLANDMALL:
        EnableFederalLandComponents();
        break;

        default:
        NoSelectedMall();
        break;
    }
}
//---------------------------------------------------------------------------

void TfrmSetup::NoSelectedMall()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = false;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(false, false, false, false, false, false, false, false,
                         false, false, false, false, false, false);
    ClearAllComponentValue();
}
//---------------------------------------------------------------------------

void TfrmSetup::EnableSMComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, true, true, true, true, true, true, true,
                         false, false, false, false, false, false);
}
//---------------------------------------------------------------------------

void TfrmSetup::EnableRobinsonComponents()
{
    btnResendReport->Visible = true;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, true, false, false, false, false, true, false,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmBranchCode, TGlobalSettings::Instance().BranchCode = "", edBranchCode);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}
//---------------------------------------------------------------------------

void TfrmSetup::EnableAyalaComponents()
{
    btnResendReport->Visible = true;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;
    bool EnableConSolidated = TGlobalSettings::Instance().EnableConsolidatedReport;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, true, false, false, false, true, true, true,
                         false, false, false, false, EnableConSolidated, true);

    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
}
//---------------------------------------------------------------------------

void TfrmSetup::EnablePowerPlantComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, false, false, false, false, false, true, true,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo = "", edTerminalNo);
    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}

//---------------------------------------------------------------------------

void TfrmSetup::EnableCapitalandComponents()
{
    btnResendReport->Visible = true;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(false, false, false, false, false, false, false, true,
                         true, true, true, true, false, false);

    ClearCompValueinDBStrUnique(vmMallPath, TGlobalSettings::Instance().MallPath = "", edMallPath);
    ClearCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo = "", edTerminalNo);
    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmTenantNo, TGlobalSettings::Instance().TenantNo = "", edTenantNo);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}

//---------------------------------------------------------------------------

void TfrmSetup::EnableAlphalandComponents()
{
    btnResendReport->Visible = true;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, true, false, false, false, false, true, false,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmBranchCode, TGlobalSettings::Instance().BranchCode = "", edBranchCode);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}

//---------------------------------------------------------------------------

void TfrmSetup::EnableMegaworldComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = true;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, true, false, false, false, false, true, false,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmBranchCode, TGlobalSettings::Instance().BranchCode = "", edBranchCode);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}

//---------------------------------------------------------------------------

void TfrmSetup::EnableShangrilaComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, false, false, false, false, false, true, true,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo = "", edTerminalNo);
    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
    ClearCompValueinDBStrUnique(vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths = "", edConsolidatedDBPaths);
    ClearCompValueinDBBoolUnique(vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport = false, cbEnableConsolidatedRep);
}

//---------------------------------------------------------------------------

void TfrmSetup::EnableDLFMallComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword, ConsolidatedPath, EnableConsolidatedRep
    EnableFieldComponent(true, false, false, false, false, false, true, false,
                         true, true, true, true, false, false);

  /*  ClearCompValueinDBStrUnique(vmTerminalNo, TGlobalSettings::Instance().TerminalNo = "", edTerminalNo);
    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
        ClearCompValueinDBStrUnique(vmTenantNo, TGlobalSettings::Instance().TenantNo = "", edTenantNo);
  */  }

//---------------------------------------------------------------------------

void TfrmSetup::EnableFederalLandComponents()
{
    btnResendReport->Visible = false;
    btnRegenReport->Visible = true;
    btnAssignSalesType->Enabled = false;

    // MallPath, TerminalNo, ClassCode, TradeCode, OutletCode, SerialNo, TenantNo, BranchCode
    // FTPServer, FTPPath, FTPUserName, FTPPassword
    EnableFieldComponent(true, true, false, false, false, false, true, true,
                         false, false, false, false, false, false);

    ClearCompValueinDBStrUnique(vmClassCode, TGlobalSettings::Instance().ClassCode = "", edClassCode);
    ClearCompValueinDBStrUnique(vmTradeCode, TGlobalSettings::Instance().TradeCode = "", edTradeCode);
    ClearCompValueinDBStrUnique(vmOutletCode, TGlobalSettings::Instance().OutletCode = "", edOutletCode);
    ClearCompValueinDBStrUnique(vmSerialNo, TGlobalSettings::Instance().SerialNo = "", edSerialNo);
    ClearCompValueinDBStrUnique(vmFTPServer, TGlobalSettings::Instance().FTPServer = "", edFTPServer);
    ClearCompValueinDBStrUnique(vmFTPPath, TGlobalSettings::Instance().FTPPath = "", edFTPPath);
    ClearCompValueinDBStrUnique(vmFTPUserName, TGlobalSettings::Instance().FTPUserName = "", edFTPUserName);
    ClearCompValueinDBStrUnique(vmFTPPassword, TGlobalSettings::Instance().FTPPassword = "", edFTPPassword);
}

//---------------------------------------------------------------------------

void TfrmSetup::InitializeMallExport()
{
    // For Mall Export Temporarily HARDCODED
    cbMallLoc->AddItem("None",NULL);
    cbMallLoc->AddItem("SM Mall",NULL);
    cbMallLoc->AddItem("Robinsons Mall",NULL);
    cbMallLoc->AddItem("Ayala Mall",NULL);
    cbMallLoc->AddItem("Power Plant Mall",NULL);
    cbMallLoc->AddItem("Capitaland Mall",NULL);
    cbMallLoc->AddItem("Alphaland Mall",NULL);
    cbMallLoc->AddItem("Megaworld Mall",NULL);
    cbMallLoc->AddItem("Shangri-La Mall",NULL);
    cbMallLoc->AddItem("DLF Mall",NULL);
    cbMallLoc->AddItem("Federal Land Mall",NULL);

    lbBranchCode->Caption = RenameBranchCode();
    lbSerialNumber->Caption = RenameSerialNumber();
    lbTenantNo->Caption = RenameTenantNumber();

    edTenantNo->Text = TGlobalSettings::Instance().TenantNo;
    edMallPath->Text = TGlobalSettings::Instance().MallPath;
    edClassCode->Text = TGlobalSettings::Instance().ClassCode;
    edTradeCode->Text = TGlobalSettings::Instance().TradeCode;
    edOutletCode->Text = TGlobalSettings::Instance().OutletCode;
    edTerminalNo->Text = TGlobalSettings::Instance().TerminalNo;
    edBranchCode->Text = TGlobalSettings::Instance().BranchCode;
    edSerialNo->Text = TGlobalSettings::Instance().SerialNo;

    edFTPServer->Text = TGlobalSettings::Instance().FTPServer;
    edFTPPath->Text = TGlobalSettings::Instance().FTPPath;
    edFTPUserName->Text = TGlobalSettings::Instance().FTPUserName;
    edFTPPassword->Text = TGlobalSettings::Instance().FTPPassword;
    edConsolidatedDBPaths->Text = TGlobalSettings::Instance().ConsolidateReportPaths;
    cbEnableConsolidatedRep->Checked = TGlobalSettings::Instance().EnableConsolidatedReport;

    cbMallLoc->ItemIndex = TGlobalSettings::Instance().MallIndex;
    SetSpecificMall(TGlobalSettings::Instance().MallIndex);

//    SaveMESettings();
}
//---------------------------------------------------------------------------

// This function is to make sure that the path settings is an absolute path that end in a
// backslash
void TfrmSetup::CheckMallPath()
{
    UnicodeString MallPathVar = TGlobalSettings::Instance().MallPath;
    std::string StrArray = MallPathVar.t_str();
    UnicodeString EndOfStr = StrArray[MallPathVar.Length() - 1];

    if(EndOfStr != "\\")
    {
        TGlobalSettings::Instance().MallPath = TGlobalSettings::Instance().MallPath + "\\";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbBarcodeFormatChange(TObject *Sender)
{
   		TGlobalSettings::Instance().BarcodeFormat = cbBarcodeFormat->ItemIndex;
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmBarcodeFormat,TGlobalSettings::Instance().BarcodeFormat);
		DBTransaction.Commit();
}
//---------------------------------------------------------------------------

// This method renames the text header for the Branch Code depending on what mall is enabled
UnicodeString TfrmSetup::RenameBranchCode()
{
    UnicodeString Caption = "";

    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        Caption = "Tenant Name For DBF";
    }
    else if ((TGlobalSettings::Instance().MallIndex == POWERPLANTMALL) ||
             (TGlobalSettings::Instance().MallIndex == SHANGRILAMALL))
    {
        Caption = "Sales Department";
    }
    else if (TGlobalSettings::Instance().MallIndex == CAPITALAND)
    {
        Caption = "Machine ID";
    }
    else if (TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        Caption = "Merchant Name";
    }
    else
    {
        Caption = "Branch Code";
    }

    return Caption;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbShowCustomerDisplayClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowCustomerDisplay = cbShowCustomerDisplay->Checked;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowCustomerDisplay,TGlobalSettings::Instance().ShowCustomerDisplay);
	DBTransaction.Commit();

    refreshCustomerDisplay();
}
//---------------------------------------------------------------------------

void TfrmSetup::refreshCustomerDisplay()
{
    // Show or hide Customer Display, depends on the final settings
    if( TMMCustomerDisplayManager::Instance()->Open )
    {
        if( !cbShowCustomerDisplay->Checked )
        {
            // Hides the Customer Display
            TMMCustomerDisplayManager::Instance()->CloseCustomerDisplay();
            TMMCustomerDisplayManager::Done();
        }
    }
    else
    {
        if( cbShowCustomerDisplay->Checked )
        {
            // Shows the Customer Display
            TMMCustomerDisplayManager* customer_display_manager = TMMCustomerDisplayManager::Instance();
            customer_display_manager->OpenCustomerDisplay();
        }
    }
}
//---------------------------------------------------------------------------

void TfrmSetup::loadCashDrawerPorts()
{
    cbCashDrawPort->Clear();

    TCashDrawerPortsList cashDrawerPortsList;
    CashDrawerPortsMap portsMap = cashDrawerPortsList.GetCashDrawerPorts();

    CashDrawerPortsMap::iterator it = portsMap.begin();

    for( ; it != portsMap.end(); it++ )
    {
        cbCashDrawPort->AddItem( it->second, NULL );
    }
}
//---------------------------------------------------------------------------

void TfrmSetup::setCashDrawerPort()
{
	cbCashDrawPort->ItemIndex = TComms::Instance().SerialKickPortNumber;
}
//---------------------------------------------------------------------------



// This method is used for resending reports for Robinson and Ayala Mall
void __fastcall TfrmSetup::btnResendReportMouseClick(TObject *Sender)
{
    UnicodeString YearFolder = "";
    std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());

    if(MEM->IMallManager->CheckBackUpPath())
    {
        std::auto_ptr<TfrmMallExportResendReport> MallExportResendReport(TfrmMallExportResendReport::Create<TfrmMallExportResendReport>(this));
        TStringList* reportsList = new TStringList;
        TStringList* copyReports = new TStringList;
        TStringList* foldersList = new TStringList;

        MEM->IMallManager->GetReportsList(foldersList, YearFolder);
        MallExportResendReport->btnResendReport->Caption = "Select Folder";
        MallExportResendReport->btnSelectAll->Visible = false;
        MallExportResendReport->btnOk->Enabled = false;
        MallExportResendReport->ReportList = foldersList;

        if(MallExportResendReport->ShowModal() == mrOk)
        {
            YearFolder = MallExportResendReport->YearFolder;

            MEM->IMallManager->GetReportsList(reportsList, YearFolder);
            MallExportResendReport->btnResendReport->Caption = "Resend Report";
            MallExportResendReport->btnSelectAll->Visible = true;
            MallExportResendReport->btnOk->Enabled = true;
            MallExportResendReport->ReportList = reportsList;
            MallExportResendReport->allReports = copyReports;
            MallExportResendReport->ShowModal();
        }
    }
    else
    {
        MessageBox( "There are no files available for re-sending.", "", MB_OK );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::tbtnReconfigEmailHostMouseClick(TObject *Sender)
{
    UnicodeString host =  TGlobalSettings::Instance().EmailHost;
    host = DisplayInputForm(TGlobalSettings::Instance().EmailHost, "Please Enter Email Host");
    if(host !="")
    {
       TGlobalSettings::Instance().EmailHost = host;
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEmailHost,TGlobalSettings::Instance().EmailHost);
       DBTransaction.Commit();
    }

}

void __fastcall TfrmSetup::tbtnReconfigEmailHostPortMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->SetMaxLengthValue(5);
    frmTouchNumpad->Caption = "Please Enter Email Host Port No.";
	frmTouchNumpad->btnCancel->Visible = true;
	frmTouchNumpad->btnOk->Visible = true;
    frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = TGlobalSettings::Instance().EmailHostPort;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        TGlobalSettings::Instance().EmailHostPort = frmTouchNumpad->INTResult;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmEmailHostPort,TGlobalSettings::Instance().EmailHostPort);
        DBTransaction.Commit();
    }
}

void __fastcall TfrmSetup::tbtnReconfigEmailIdMouseClick(TObject *Sender)
{
    UnicodeString email_id;
    TMMContactInfo SmartCardContact;
    bool IsEmailValid = SmartCardContact.ValidEmail();
    email_id =  DisplayInputForm(TGlobalSettings::Instance().EmailId, "Please Enter Email Id");
    SmartCardContact.EMail = email_id;
    IsEmailValid  =  SmartCardContact.ValidEmail();
    if(!IsEmailValid)
    {
        while (!IsEmailValid && MessageBox("Entered email address is invalid.", "Invalid Email", MB_RETRYCANCEL) == IDRETRY)
        {
           if(!IsEmailValid)
           {
             IsEmailValid = true;
             email_id = DisplayInputForm(email_id, "Please Enter Email Id");
           }
           else
           {
              break;
           }
           SmartCardContact.EMail = email_id;
           IsEmailValid  =  SmartCardContact.ValidEmail();
        }
    }
    if(IsEmailValid)
    {
        TGlobalSettings::Instance().EmailId = email_id;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEmailId,TGlobalSettings::Instance().EmailId);
        DBTransaction.Commit();
    }
}

void __fastcall TfrmSetup::tbtnReconfigEmailPasswordMouseClick(TObject *Sender)
{

    TGlobalSettings::Instance().EmailPassword = DisplayInputForm(TGlobalSettings::Instance().EmailPassword, "Please Enter Email Password");

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmEmailPassword,TGlobalSettings::Instance().EmailPassword);
    DBTransaction.Commit();

}

UnicodeString TfrmSetup::DisplayInputForm(UnicodeString initialValue, AnsiString caption)
{
    UnicodeString finalvalue;
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));

    frmTouchKeyboard->MaxLength = 300;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = true;
    frmTouchKeyboard->MustHaveValue = true;
    if(caption == "Please Enter Email Password")
    {
      frmTouchKeyboard->KeyboardText = "";
      finalvalue = TGlobalSettings::Instance().EmailPassword;
    }
    else
    {
       frmTouchKeyboard->KeyboardText = initialValue;
    }
    frmTouchKeyboard->Caption = caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
       finalvalue = frmTouchKeyboard->KeyboardText;
    }
    return finalvalue;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnRegenerateReportMouseClick(TObject *Sender)
{
    UnicodeString YearFolder = "";
    std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());

    std::auto_ptr<TfrmMallExportRegenerateReport> MallExportRegenerateReport(TfrmMallExportRegenerateReport::Create<TfrmMallExportRegenerateReport>(this));
    MallExportRegenerateReport->ShowModal();
}
//---------------------------------------------------------------------------

void TfrmSetup::SaveMESettings()
{
    if((TGlobalSettings::Instance().MallIndex >= ROBINSONMALL) && (TGlobalSettings::Instance().MallIndex <= FEDERALLANDMALL))
    {
        TGlobalSettings::Instance().FirstMallSet = true;
        TGlobalSettings::Instance().FirstMallDate = Now().FormatString("mmddyy");
        TGlobalSettings::Instance().LastZedDate = "";
        TGlobalSettings::Instance().LastZedTime = "";

        if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
        {
            TGlobalSettings::Instance().FirstMallDate = Now().FormatString("mm/dd/yy");
            TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh:nn:ss");
        }
    }
    else
    {
        TGlobalSettings::Instance().FirstMallSet = false;
    }
    SaveCompValueinDBBool(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
    SaveCompValueinDBStr(vmFirstMallDate, TGlobalSettings::Instance().FirstMallDate);
    SaveCompValueinDBStr(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
    SaveCompValueinDBStr(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);
    frmMain->InitializeExportTimings(TGlobalSettings::Instance().FirstMallSet);
    TMallExportUpdateAdaptor().UpdateExportTableOnSettingsChange();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbNewbookTypeChange(TObject *Sender)
{
	TGlobalSettings::Instance().NewBook = cbNewbookType->ItemIndex;
		Database::TDBTransaction DBTransaction(IBDatabase);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmNewBook,TGlobalSettings::Instance().NewBook);
		DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnAssignSalesTypeMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmMallExportSalesTypeAssignment> MallExportSalesTypeAssignment(new TfrmMallExportSalesTypeAssignment(this));
    MallExportSalesTypeAssignment->ShowModal();
}
//---------------------------------------------------------------------------

// This method renames the text header for the Serial Number depending on what mall is enabled
UnicodeString TfrmSetup::RenameSerialNumber()
{
    UnicodeString Caption = "";

    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        Caption = "Contract Number";
    }
    else
    {
        Caption = "Serial Number";
    }

    return Caption;
}
//---------------------------------------------------------------------------

UnicodeString TfrmSetup::RenameTenantNumber()
{
    UnicodeString Caption = "";

    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        Caption = "Tenant Name For TEXT";
    }
    else if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        Caption = "Merchant Code";
    }
    else
    {
        Caption = "Tenant Number";
    }

    return Caption;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbNewMallLocChange(TObject *Sender)
{
    UpdateNoMallUI();
    if(cbNewMallLoc->ItemIndex )
    {
        TManagerMallSetup::InsertInToMallExport_Settings_Values(cbNewMallLoc->ItemIndex);
        LoadMallSettingInfo();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmSetup::edNewMallPathMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 100;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edNewMallPath->Text;
	frmTouchKeyboard->Caption = "Enter File Location";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        if((frmTouchKeyboard->KeyboardText).Trim() != "")
            edNewMallPath->Text = CheckAbsolutePath(frmTouchKeyboard->KeyboardText);
        else
           edNewMallPath->Text = "";

    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::edMallTenantNoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 50;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edMallTenantNo->Text;
	frmTouchKeyboard->Caption = "Enter Tenant Code";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edMallTenantNo->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edMallTerminalNoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edMallTerminalNo->Text;
	frmTouchKeyboard->Caption = "Enter Terminal Number";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edMallTerminalNo->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------
void  TfrmSetup::SetupNewMalls()
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    cbNewMallLoc->Clear();
    std::vector<UnicodeString> mallList;
    cbNewMallLoc->AddItem("None",NULL);

    mallList = TManagerMallSetup::LoadAllMalls(dbTransaction);
    for (int index = 0; index < mallList.size() ; index++)
    {
        cbNewMallLoc->AddItem(mallList[index],NULL);
    }
    int mallIndex = TManagerMallSetup::CheckActiveMallExist(dbTransaction);
    cbNewMallLoc->ItemIndex = mallIndex;
    if(cbNewMallLoc->ItemIndex)
        LoadMallSettingInfo();
    else
    {
        UpdateNoMallUI();
    }
    dbTransaction.Commit();
}
//----------------------------------------------------------------------------------------------
void TfrmSetup::LoadMallSettingInfo()
{
       //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //First We have to Inactive Activated mall Since mall Index has been changed..
        TManagerMallSetup::UpdateINActiveMall(dbTransaction);

        //Now Active selected mall
        TManagerMallSetup::UpdateActiveMall(dbTransaction, cbNewMallLoc->ItemIndex);

        //load all mall settings info
        mallInfo = TManagerMallSetup::LoadActiveMallSettings(dbTransaction);
        TGlobalSettings::Instance().mallInfo = mallInfo;
        std::list<TMallExportSettings>::iterator it;
        TControl *ChildControl;
        TEdit* editBox;

        for(it = mallInfo.MallSettings.begin(); it != mallInfo.MallSettings.end(); it++)
        {
            for (int i = 0; i < gbMallsNew->ControlCount; i++)
            {
                ChildControl = gbMallsNew->Controls[i];
                editBox = (TEdit*)ChildControl;
                if(it->ControlName == editBox->Name)
                {
                    ChildControl->Enabled = true;
                    editBox->Text = it->Value;
                    editBox->Color = clWindow;
                }
            }
        }
        dbTransaction.Commit();
}
//-------------------------------------------------------------------------------------
void TfrmSetup::UpdateMallInfo()
{
     //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();
        mallInfo.MallId = cbNewMallLoc->ItemIndex;
        mallInfo.MallName = cbNewMallLoc->Text;
        mallInfo.DeviceKey =  TDeviceRealTerminal::Instance().ID.ProfileKey;
        TMallExportSettings mallSetting;

        std::list<TMallExportSettings>::iterator it;
        TControl *ChildControl;
        TEdit* editBox;
        for(it = mallInfo.MallSettings.begin(); it != mallInfo.MallSettings.end(); it++)
        {
            for (int i = 0; i < gbMallsNew->ControlCount; i++)
            {
                ChildControl = gbMallsNew->Controls[i];
                editBox = (TEdit*)ChildControl;
                if(it->ControlName == editBox->Name)
                {
                    it->Value = editBox->Text;
                }
            }
        }
        TGlobalSettings::Instance().mallInfo =  mallInfo;
        TManagerMallSetup::UpdateMallExportSettingValues(dbTransaction, mallInfo);
        dbTransaction.Commit();
}
//--------------------------------------------------------------------------------------
void TfrmSetup::UpdateNoMallUI()
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    UnicodeString controlNameSubString = "";
    TMall mallDetails;
    mallDetails.MallId = 0;
    TGlobalSettings::Instance().mallInfo = mallDetails;
    TManagerMallSetup::UpdateINActiveMall(dbTransaction);
    TControl *ChildControl;
    TEdit* editBox;
    for (int i = 0; i < gbMallsNew->ControlCount; i++)
    {
        ChildControl = gbMallsNew->Controls[i];
        editBox = (TEdit*)ChildControl;
        controlNameSubString = editBox->Name.SubString(0,2);
        if(controlNameSubString != "lb" && controlNameSubString != "cb")
        {
            editBox->Text = "";
            editBox->Enabled = false;
            editBox->Color = clInactiveCaptionText;
        }
    }
    dbTransaction.Commit();
}
//-----------------------------------------------------------------------------------------
UnicodeString TfrmSetup::CheckAbsolutePath(UnicodeString path)
{
    UnicodeString MallPathVar = path;
    std::string StrArray = MallPathVar.t_str();
    UnicodeString EndOfStr = StrArray[MallPathVar.Length() - 1];

    if(EndOfStr != "\\")
    {
        path = path + "\\";
    }
    //Check if directory not exist than create it.
    if (!DirectoryExists(path))
    {
        CreateDir(path);
    }
    return path;
}
//----------------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::btnRegenMallReportMouseClick(TObject *Sender)
{
    std::auto_ptr<TfrmRegenerateMallReport> frmRegenerateMallReport(TfrmRegenerateMallReport::Create<TfrmRegenerateMallReport>(this));
    frmRegenerateMallReport->ShowModal();
}
//----------------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::edTaxRateClick(TObject *Sender)
{
    std::auto_ptr<TfrmDiscount> frmDiscount(TfrmDiscount::Create<TfrmDiscount>(this));
    frmDiscount->Mode = DiscModePercent;
    frmDiscount->tbToggleAmount->Visible = false;
    frmDiscount->pnlToggle->Visible = false;
    frmDiscount->PERCInitial = edTaxRate->Text != "" ? StrToCurr(edTaxRate->Text) : 0;
    frmDiscount->Caption = "Enter Tax Rate";
    if (frmDiscount->ShowModal() == mrOk)
    {
        edTaxRate->Text = frmDiscount->PERCResult;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::btnAssignMallSalesTypeMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmMallSalesTypeAssignment> mallSalesTypeAssignment(new TfrmMallSalesTypeAssignment(this));
    mallSalesTypeAssignment->ShowModal();
}
//---------------------------------------------------------------------------------------
void __fastcall TfrmSetup::btnMezzanineAreaMouseClick(TObject *Sender)
{
    ShowMezzanineArea();
}
//------------------------------------------------------------------------------------------
void TfrmSetup::ShowMezzanineArea()
{
    try
    {
        TFloorPlanReturnParams floorPlanReturnParams;
        std::auto_ptr<TEnableFloorPlan>floorPlan(new TEnableFloorPlan());
        floorPlan->Run( ( TForm* )this, true, floorPlanReturnParams, false );

        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();
        TGlobalSettings::Instance().MezzanineTablesMap.clear();
        TGlobalSettings::Instance().MezzanineTablesMap = TManagerMallSetup::LoadMezzanineAreaTablesByLocations(dbTransaction);
        dbTransaction.Commit();
    }
    catch(Exception & E)
    {
         MessageBox(E.Message,"",MB_OK);
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//----------------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::edMallServerIPClick(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);
    EditServerDetils(btn->Tag);
}
//------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::edMallServerPathClick(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);
    EditServerDetils(btn->Tag);
}
//------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::edMallUserNameClick(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);
    EditServerDetils(btn->Tag);
}
//------------------------------------------------------------------------------------------
void __fastcall TfrmSetup::edMallPasswordClick(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);
    EditServerDetils(btn->Tag);
}
//------------------------------------------------------------------------------------------
void TfrmSetup::EditServerDetils(int tagKey)
{
    if(tagKey > 4)
        return;

    AnsiString Caption = "";
    AnsiString KeyboardText = "";
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;

    switch(tagKey)
    {
        case 1:
            Caption = "Enter Server IP";
            frmTouchKeyboard->KeyboardText = edMallFTPServer->Text;
            frmTouchKeyboard->MaxLength = 50;
            break;
        case 2:
            Caption = "Enter Server Path";
            frmTouchKeyboard->KeyboardText = edMallFTPPath->Text;
            break;
        case 3:
            Caption = "Enter User Name";
            frmTouchKeyboard->KeyboardText = edMallFTPUserName->Text;
            break;
        case 4:
            Caption = "Enter password";
            frmTouchKeyboard->KeyboardText = edMallFTPPassword->Text;
            break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        DisplayServerInfo(tagKey, frmTouchKeyboard->KeyboardText);
	}
}
//-------------------------------------------------------------------------------------------
void TfrmSetup::DisplayServerInfo(int tagKey, AnsiString KeyboardText)
{
    switch(tagKey)
    {
        case 1:
            edMallFTPServer->Text = KeyboardText;
            break;
        case 2:
            edMallFTPPath->Text = KeyboardText;
            break;
        case 3:
            edMallFTPUserName->Text = KeyboardText;
            break;
        case 4:
            edMallFTPPassword->Text = KeyboardText;
            break;
        default:
            break;
    }
}

