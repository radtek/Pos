//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SyndCodeGui.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmSyndCodeGui::TfrmSyndCodeGui(TComponent* Owner,TManagerSyndCode &inManagerSyndCode)
   : TZForm(Owner),
   ManagerSyndCode(inManagerSyndCode)
{

}

void __fastcall TfrmSyndCodeGui::tbtnNameMouseClick(TObject *Sender)
{
   // Run the Controller.
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = SyndCode.Name;//TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetSyndicateCode();
   frmTouchKeyboard->Caption = "Enter a Name for this Syndicate Code.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      SyndCode.Name = frmTouchKeyboard->KeyboardText;
   }
   tbtnName->Caption = SyndCode.Name;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::TouchBtn3MouseClick(TObject *Sender)
{
   // Run the Controller.
   std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
   frmTouchKeyboard->MaxLength = 50;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = "";//TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetSyndicateCode();
   frmTouchKeyboard->Caption = "Enter the Syndicate Code.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      if(frmTouchKeyboard->KeyboardText.Length() > 7  || frmTouchKeyboard->KeyboardText.Length() == 0)
      {
         SyndCode.OriginalSyndCode = SyndCode.DecryptedSyndCode = frmTouchKeyboard->KeyboardText;
         tbtnName->Caption = "Name : " + SyndCode.Name;         
      }
      else
      {
         MessageBox("A Syndicate Key must be longer than 10 charaters.", "Error",MB_OK + MB_ICONERROR);
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::tbtnEnabledMouseClick(TObject *Sender)
{
   SyndCode.Enabled = !SyndCode.Enabled;
   tbtnEnabled->Caption = (SyndCode.Enabled == true) ? "Enabled" : "Disabled";
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::btnCloseMouseClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::TouchBtn4MouseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::FormShow(TObject *Sender)
{
   FormResize(this);

   tbtnName->Caption = "Name : " + SyndCode.Name;
   tbtnEnabled->Caption = (SyndCode.Enabled == true) ? "Enabled" : "Disabled";
   tbtnEncryption->Caption = (SyndCode.DefaultEncryptCode == true) ? " Use For Encryption Code : True" : " Use For Encryption Code : False";
   btnUseForCom->Caption = (SyndCode.UseForCom == true) ? " MM Cloud Communication : True" : " MM Cloud Communication : False";
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::FormResize(TObject *Sender)
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
	Left				= (Screen->Width - Width) / 2;
   Top				= (Screen->Height - Height) / 2;
   pnlMajor->Width = pnlMinor->Left - this->BorderWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::tbtnEncryptionMouseClick(TObject *Sender)
{
   SyndCode.DefaultEncryptCode = !SyndCode.DefaultEncryptCode;
   tbtnEncryption->Caption = (SyndCode.DefaultEncryptCode == true) ? " Use For Encryption Code : True" : " Use For Encryption Code : False";
}
//---------------------------------------------------------------------------
void __fastcall TfrmSyndCodeGui::btnUseForComMouseClick(TObject *Sender)
{
  if(ManagerSyndCode.CanUseForCommunication(SyndCode.SyndCodeKey))
  {
    SyndCode.UseForCom = !SyndCode.UseForCom;
    btnUseForCom->Caption = (SyndCode.UseForCom == true) ? " MM Cloud Communication : True" : " Use For Communication : False";
  }
  else
  {
     MessageBox("You have setup a Syndicate Code for communication with Menumate Cloud already. This option can not be set this Syndicate Code.", "Warning", MB_OK);
  }
}
//---------------------------------------------------------------------------

