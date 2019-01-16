
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "SmartCardConfigGUI.h"
#include "MMTouchKeyboard.h"
#include "SmartCardAPI.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"

#include "DeviceRealTerminal.h"
#include "MMLogging.h"
#include "SmartcardPreloader.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#include "SyndCodeController.h"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmSmartCardConfiguration::TfrmSmartCardConfiguration(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSmartCardConfiguration::tbSetVerifactionCode(
      TObject *Sender)
{
   // Create a SynCode Controler.
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   TSyndCodeController SyndCodeController(this,DBTransaction,TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager());
   SyndCodeController.Run();
   DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSmartCardConfiguration::TouchBtn2MouseClick(
      TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSmartCardConfiguration::FormShow(TObject *Sender)
{
	FormResize(NULL);
   tbtnFormatCard->Enabled = true;
//   static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"]);
   tbtnRestoreCard->Enabled =  true;
//   static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"]);
   tbtnExploreCard->Enabled =  true;
//   static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"]);
   tbtnReassignCard->Enabled =  true;
//   static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSmartCardConfiguration::FormResize(TObject *Sender)
{
	SetWindowPos(Handle, HWND_TOP, Left, Top, Width, Height, 0);
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
   ClientWidth    = Panel24->Width + Panel2->Width + BorderWidth;
}
//---------------------------------------------------------------------------


void __fastcall TfrmSmartCardConfiguration::tbtnFormatCardMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
   try
   {
      if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardCorrupt || Shift.Contains(ssCtrl))
      {
         if(Shift.Contains(ssCtrl) && Shift.Contains(ssAlt) && Shift.Contains(ssShift) )
         {
            // Leave the GUID intact, Reset the PSC (Programable Security Code)
            if (CustomMessageBox("This operation will remove all points and members information off the card it will also make the card unrestorable!.\rAre you sure you wish to completly format this card.\r" ,"Erase entire card & GUID.",MB_ICONQUESTION,"Format Card","Dont Format Card") == IDOK)
            {
               TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->FormatCardToFactory();
            }
         }
         else
         {
            // Leave the GUID intact, Reset the PSC (Programable Security Code) & Black User Data Area.
            if (CustomMessageBox("This operation will remove all points and members information off the card.\rAre you sure you wish to format this card.\r" ,"Erase entire card.",MB_ICONQUESTION,"Format Card","Dont Format Card") == IDOK)
            {
               TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->FormatCardToNewUser();
            }
         }
      }
      else
      {
         MessageBox("Card appears to be ok. There is no need to clear this card.", "Error, clear unnecessary", MB_OK + MB_ICONERROR);
      }
   }
	catch(Exception &E)
	{
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSmartCardConfiguration::tbtnRestoreCardMouseUp(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
   try
   {
	  if( TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardBlank ) // Smart Card is Blank
	  {
			TDeviceRealTerminal::Instance().ManagerMembership->RestoreCard();
	  }
	  else if( TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardCorrupt ) // Smart Card is Corrupted
	  {
			TDeviceRealTerminal::Instance().ManagerMembership->RestoreCardWithPoints();
			TManagerLogs::Instance().Add("TAMPER CHECK",EXCEPTIONLOG,"Card Restored with Points.");
	  }
	  else if( Shift.Contains(ssCtrl) && Shift.Contains(ssAlt) && Shift.Contains(ssShift) ) // Force to Restore. Only MenuMate Staff are allowed to do it
	  {
			TLoginSuccess Result = lsAccepted;
			AnsiString PIN;

			std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
			frmTouchNumpad->Caption = "Enter Access PIN";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->Mode = pmPIN;

			if (frmTouchNumpad->ShowModal() == mrOk)
			{
			   PIN = frmTouchNumpad->STRResult;
			   AnsiString PIN2 = Now().FormatString("nnddhh");
			   // TODO : Mke this this Min/Day/Hour hh = 24 hour format.
			   if(PIN == PIN2)
			   {
					TDeviceRealTerminal::Instance().ManagerMembership->RestoreCardWithPoints();
					TManagerLogs::Instance().Add("TAMPER CHECK",EXCEPTIONLOG,"Card Restored with Points.");
			   }
			}
	  }
	  else
	  {
		 MessageBox( "Card appears to be ok. There is no need to restore this card.", "Error, restore unnecessary", MB_OK + MB_ICONERROR );
	  }
    }
	catch(Exception &E)
	{
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSmartCardConfiguration::tbtnExploreCardMouseUp(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
	  int Y)
{
   try
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->ExploreCard();
   }
	catch(Exception &E)
	{
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSmartCardConfiguration::tbtnReassignCardMouseUp(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
	  int Y)
{
   try
   {
      if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardInserted)
      {
         TDeviceRealTerminal::Instance().ManagerMembership->ReassignCard();
      }
      else
      {
         MessageBox("Please Insert Card in Reader", "Error No Card", MB_OK + MB_ICONERROR);
      }
   }
	catch(Exception &E)
	{
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}

#pragma warn -par

void __fastcall
TfrmSmartCardConfiguration::preload_card_clicked(TObject *Sender)
{
	std::auto_ptr<TSmartcard_Preloader> preloader(new TSmartcard_Preloader);
	preloader->ShowModal();
}

#pragma warn .par

