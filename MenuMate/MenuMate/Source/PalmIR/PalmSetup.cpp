//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PalmSetup.h"
#include "IBLogs.h"
#include "enum.h"
#include "Comms.h"
#include "Palm.h"
#include "Data.h"
#include "DataLink.h"
#include "MMRegistry.h"
#include "PrinterConfig.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma link "CPortCtl"
#pragma link "CPortCtl"
#pragma link "TouchCheckBox"
#pragma resource "*.dfm"
TfrmPalmSetup *frmPalmSetup;
//---------------------------------------------------------------------------
__fastcall TfrmPalmSetup::TfrmPalmSetup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnModScanClick(TObject *Sender)
{
	TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Commenced scanning for new modules");
	lbConnections->Clear();
	TComms::Instance().modReScan(lbConnections);
	TComms::Instance().ConnectLst->Pack();
	lbConnections->Clear();
	for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
	{
      if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
		{
			TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
			if(Printer->Offline)
			{
				lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + " OffLine " + IntToStr(Printer->ErrorCount));
			}
			else
         {
				if(Printer->HasPaper)
				{
               lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + " " + IntToStr(Printer->ErrorCount));
				}
				else
            {
					lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + "Paper Out" + " " + IntToStr(Printer->ErrorCount));
				}
			}
		}
		else
		{
			TConnectIR *IR = (TConnectIR *)TComms::Instance().ConnectLst->Items[i];
			lbConnections->Items->Add(IntToStr(IR->ID) + " " + IR->Name+ " " + IntToStr(IR->ErrorCount));
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnModClearClick(TObject *Sender)
{
	TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Connection List Cleared");
   bool StartTrans = false;
   try
   {
      if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }

      TDeviceRealTerminal::Instance().IBQuery->Close();
      TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
         "DELETE FROM CONNECTIONS "
         "WHERE "
            "DEVICE_KEY	= :DEVICE_KEY AND "
            "(CONNECT_TYPE	= :CONNECT_TYPE1 OR "
            "CONNECT_TYPE	= :CONNECT_TYPE2)";
      TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
      TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE1")->AsInteger = Windows_Printer;
      TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE2")->AsInteger = IR_TRX_Station;
      TDeviceRealTerminal::Instance().IBQuery->ExecSQL();
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Commit(); }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
   }

	lbConnections->Clear();
	TComms::Instance().modClear();
   cbReceiptPrinter->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnModAutoProgClick(TObject *Sender)
{
	TManagerLogs::Instance().Add(__FUNC__,SECURITY,"Commenced autoprogramming new modules");
	lbConnections->Clear();
	TComms::Instance().modAutoProgram(lbConnections);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnModResetClick(TObject *Sender)
{
	TManagerLogs::Instance().Add(__FUNC__,SECURITY,"Resetting all Modules to address zero");
	lbConnections->Clear();
	TComms::Instance().modReset();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnApplyTimeoutsClick(TObject *Sender)
{
	int RetryCount = StrToInt(edRetry->Text);
	int TimeOutms = StrToInt(edInitailResponse->Text);
	int TimeOutBetweenms = StrToInt(edInterval->Text);

	for(int i = 0; i < TComms::Instance().ConnectLst->Count ; i++)
	{
		TConnect *CurrentConnection = (TConnect *)TComms::Instance().ConnectLst->Items[i];
//      if(CurrentConnection->TRXType == IR_TRX_Station ||
//         CurrentConnection->TRXType == Printer_TRX_Station)
//      {
			CurrentConnection->DataLinkControl->RetryCount = RetryCount;
			CurrentConnection->DataLinkControl->TimeOutms = TimeOutms;
			CurrentConnection->DataLinkControl->TimeOutBetweenms = TimeOutBetweenms;
//      }
	}
	RegistryWrite(MMBaseKey, "SerialRetryCount", RetryCount);
	RegistryWrite(MMBaseKey, "SerialTimeOutms", TimeOutms);
	RegistryWrite(MMBaseKey, "SerialTimeOutBetweenms", TimeOutBetweenms);
	edRetry->Text = IntToStr(RetryCount);
	edInitailResponse->Text = IntToStr(TimeOutms);
	edInterval->Text = IntToStr(TimeOutBetweenms);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::FormShow(TObject *Sender)
{
	FormResize(NULL);
	cbReceiptPrinter->Clear();
	// Loop though Receipt printers
	if(TComms::Instance().ReceiptPrinter != NULL)
	{
		cbReceiptPrinter->Clear();
		TComms::Instance().ConnectLst->Pack();
		cbReceiptPrinter->Items->Add("None");
		int PrinterIndex = 0;
		for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
		{
			if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
			{
				TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
            cbReceiptPrinter->Items->Add(Printer->Name);
            if(TComms::Instance().ReceiptPrinter->Name == Printer->Name)
            {
               PrinterIndex = cbReceiptPrinter->Items->Count -1;
            }
			}
		}
		cbReceiptPrinter->ItemIndex = PrinterIndex;
	}
	else
	{
		cbReceiptPrinter->Clear();
		TComms::Instance().ConnectLst->Pack();
		cbReceiptPrinter->Items->Add("None");
		for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
		{
			if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
			{
				TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
				cbReceiptPrinter->Items->Add(Printer->Name);
			}
		}
		cbReceiptPrinter->ItemIndex = 0;
	}

	lbConnections->Items->Clear();
	TComms::Instance().ConnectLst->Pack();
	for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
	{
		if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
		{
			TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
			if(Printer->Offline)
			{
				lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + " OffLine " + IntToStr(Printer->ErrorCount));
			}
			else
			{
				if(Printer->HasPaper)
				{
					lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + " " + IntToStr(Printer->ErrorCount));
				}
				else
				{
					lbConnections->Items->Add(IntToStr(Printer->ID) + " " + Printer->Name + "Paper Out" + " " + IntToStr(Printer->ErrorCount));
				}
			}
		}
		else
		{
			TConnectIR *IR = (TConnectIR *)TComms::Instance().ConnectLst->Items[i];
			lbConnections->Items->Add(IntToStr(IR->ID) + " " + IR->Name+ " " + IntToStr(IR->ErrorCount));
		}
	}

	int RetryCount = 0;
	int TimeOutms = 0;
	int TimeOutBetweenms = 0;
	RegistryRead(MMBaseKey, "SerialRetryCount", RetryCount);
	RegistryRead(MMBaseKey, "SerialTimeOutms", TimeOutms);
	RegistryRead(MMBaseKey, "SerialTimeOutBetweenms", TimeOutBetweenms);
	edRetry->Text = IntToStr(RetryCount);
	edInitailResponse->Text = IntToStr(TimeOutms);
	edInterval->Text = IntToStr(TimeOutBetweenms);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbReceiptPrinterChange(TObject *Sender)
{
	if(cbReceiptPrinter->ItemIndex != -1)
	{
		if(cbReceiptPrinter->ItemIndex == 0)
		{
			TComms::Instance().ReceiptPrinter = NULL;
			VM->SetDeviceStr(vmReceiptPrinter,"None");
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__,SECURITY,"Receipt Printer Changed");
			TComboBox *pCB = (TComboBox *)Sender;
			if(Comms)
			{
				TComms::Instance().ReceiptPrinter = (TConnectPrinter *)TComms::Instance().FetchByName(pCB->Text,Windows_Printer);
				if(TComms::Instance().ReceiptPrinter)
				{
					VM->SetDeviceStr(vmReceiptPrinter,pCB->Text);
				}
         }
		}
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::cbReceiptPrinterDropDown(TObject *Sender)
{
	cbReceiptPrinter->Clear();
	TComms::Instance().ConnectLst->Pack();
	cbReceiptPrinter->Items->Add("None");
	int PrinterIndex = 0;
	for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
	{
		if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
		{
			TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
         cbReceiptPrinter->Items->Add(Printer->Name);
         if (TComms::Instance().ReceiptPrinter != NULL)
         {
            if(TComms::Instance().ReceiptPrinter->Name == Printer->Name)
            {
               PrinterIndex = cbReceiptPrinter->Items->Count -1;
            }
         }
		}
	}
	cbReceiptPrinter->ItemIndex = PrinterIndex;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::btnCloseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmSetup::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::TouchButton1Click(TObject *Sender)
{
   try
   {
      if(frmPrinterConfiguration == NULL)
      {
         Application->CreateForm(__classid(TfrmPrinterConfiguration), &frmPrinterConfiguration);
      }
		frmPrinterConfiguration->ShowModal();
   }
   __finally
   {
      delete frmPrinterConfiguration;
      frmPrinterConfiguration = NULL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmSetup::TouchButton2Click(TObject *Sender)
{
   if(Application->MessageBox("This action requires this PalmMate to be restarted. Are you sure you wish to continue?","Confirmation",
      MB_OKCANCEL + MB_DEFBUTTON2 + MB_ICONINFORMATION) == IDOK)
   {
		VM->Delete(vmBroadCastIP);
		VM->Delete(vmAutoMateIP);
      TDeviceRealTerminal::Instance().Reboot = true;
      Application->Terminate();
   }
}
//---------------------------------------------------------------------------
