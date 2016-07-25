//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IntaMateConfiguration.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
TfrmIntaMateConfiguration *frmIntaMateConfiguration;
//---------------------------------------------------------------------------
__fastcall TfrmIntaMateConfiguration::TfrmIntaMateConfiguration(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmIntaMateConfiguration::tbPhoenixIPAddressClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
   frmTouchKeyboard->MaxLength = 255;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = TCPIPAddress;
   frmTouchKeyboard->Caption = "Enter the IP Address of the IntaMate System.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      TCPIPAddress = frmTouchKeyboard->KeyboardText;
      tbPAddress->Caption = "Server TCP IP Address\r" + TCPIPAddress;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmIntaMateConfiguration::tbPhoenixPortNumberClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
   frmTouchNumpad->Caption = "Enter the Port Number of the IntaMate System.";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = TCPPort;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      TCPPort = frmTouchNumpad->INTResult;
      tbPortNumber->Caption = "IntaMate Server Port Number\r" + IntToStr(TCPPort);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmIntaMateConfiguration::tbPhoenixIDClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
   frmTouchNumpad->Caption = "Enter the Unique ID for this POS.";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = POSID;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      POSID = frmTouchNumpad->INTResult;
      tbPOSID->Caption = "P.O.S ID\r" + IntToStr(POSID);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmIntaMateConfiguration::FormShow(TObject *Sender)
{
   UpdateGUI();
}
//---------------------------------------------------------------------------
void TfrmIntaMateConfiguration::UpdateGUI()
{
	tbPAddress->Caption = "Server TCP IP Address\r" + TCPIPAddress;
	tbPortNumber->Caption = "Server Port Number\r" + IntToStr(TCPPort);
	tbPOSID->Caption = "P.O.S ID\r" + IntToStr(POSID);
	tbTimeOuts->Caption = "TCP Time out (ms)\r" + IntToStr(TCPTimeOut_ms);
}

void __fastcall TfrmIntaMateConfiguration::btnOkClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmIntaMateConfiguration::tbTimeOutsMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the TCP Read Time Out (ms).";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = TCPTimeOut_ms;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
		TCPTimeOut_ms = frmTouchNumpad->INTResult;
		tbTimeOuts->Caption = "TCP Time out (ms)\r" + IntToStr(TCPTimeOut_ms);
	}
}
//---------------------------------------------------------------------------

