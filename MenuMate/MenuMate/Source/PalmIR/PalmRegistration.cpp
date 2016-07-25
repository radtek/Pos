//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MMMessageBox.h"
#include "PalmRegistration.h"
#include "enum.h"
#include "Palm.h"
#include "TouchKeyboard.h"
#include "Processing.h"
#include "Comms.h"
#include "Data.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmPalmRegistration *frmPalmRegistration;
//---------------------------------------------------------------------------
__fastcall TfrmPalmRegistration::TfrmPalmRegistration(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::btnAddPalmClick(TObject *Sender)
{
   int MaxPalmLicenses = 1;
   TRegInfo *RegInfo = NULL;
   MaxPalmLicenses = TDeviceRealTerminal::Instance().RegisteredPalmLicences();

	if(TDeviceRealTerminal::Instance().PalmList->Count >= MaxPalmLicenses)
	{
		MessageBox("You have reached the maximum licenses this system is registered for.\rPlease contact IQWorks for a new site license.","Can not Register",MB_OK & MB_ICONERROR);
		return;
	}


   PalmDisplayName = "";
	frmTouchKeyboard->MaxLength = 12;
   frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = true;
   frmTouchKeyboard->KeyboardText = "";
   frmTouchKeyboard->Caption = "Enter Unique Palm Name.";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      if (frmTouchKeyboard->KeyboardText != "")
      {
         TDeviceRealTerminal::Instance().NewPalmRegisterName = frmTouchKeyboard->KeyboardText;
         PalmDisplayName = frmTouchKeyboard->KeyboardText;
         frmProcessing->Message = "Scanning For Palm " + PalmDisplayName +
                                  " 1:00";
			frmProcessing->CanCancel = true;
			frmProcessing->Show();
         try
         {
            TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Palm Licence Reprogramming Activated");
            TDeviceRealTerminal::Instance().Registering = true;
            TDeviceRealTerminal::Instance().Reregistering = false;
            TDeviceRealTerminal::Instance().RegisteringTimer = 59;
         // TODO 5 -o Michael -c Misc: check .
            if(TComms::Instance().Sync == NULL )
            {
               TComms::Instance().Start();
            }
            tmRegisterPalm->Enabled = true;
            while(!frmProcessing->Cancelled && TDeviceRealTerminal::Instance().Registering)
            {
               Application->ProcessMessages();
            }

            if(frmProcessing->Cancelled && TDeviceRealTerminal::Instance().RegisteringTimer == 0)
				{
               MessageBox("Registration operation timed out.\rPlease try again.","Registration",MB_OK & MB_ICONINFORMATION);
            }
            else if(frmProcessing->Cancelled)
            {
               MessageBox("Registration operation cancelled by user.","Registration",MB_OK & MB_ICONINFORMATION);
            }
            else if(!TDeviceRealTerminal::Instance().Registering)
            {
               TReqInfoSync *Request = new TReqInfoSync;
               try
               {
                  Request->BroadcastRequest = false;
                  TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
               }
               __finally
               {
                  delete Request;
               }
               MessageBox("Registration operation completed.\rPalm " +
															PalmDisplayName + " registered successfully.","Registration",MB_OK & MB_ICONINFORMATION);
				}
            else
            {
               MessageBox("Registration operation Failed.\rPlease try again.","Registration",MB_OK & MB_ICONINFORMATION);
            }

         }
         __finally
         {
            tmRegisterPalm->Enabled = false;
            TDeviceRealTerminal::Instance().Registering = false;
            TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Palm Licence Reprogramming Complete");
            frmProcessing->Close();

            lbPalms->Clear();
            for(int i = 0 ; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
            {
               TPalm *Palm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
					lbPalms->Items->AddObject(Palm->ID.DisplayName,Palm);
				}
			}
		}
		else
		{
			MessageBox("You must enter a valid Palm name.","Registration",MB_OK & MB_ICONINFORMATION);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::btnRemovePalmClick(TObject *Sender)
{
	if(lbPalms->Items->Count == 0)
	{
		MessageBox("There are no Palms To Remove","Remove Error",MB_OK & MB_ICONERROR);
		return;
	}

	if(lbPalms->ItemIndex == -1)
	{
		MessageBox("Please Select a Palm first.","Remove Error",MB_OK & MB_ICONERROR);
		return;
	}

	AnsiString Query = "Are you sure you wish to delete this Palm.\n"
							 "All Printer and Course configurations for this device will be lost. \n\n"
							 "If this device is currently active, its printing configurations will \n"
							 "be removed and it will become unregistered. \n\n";
	if (Application->MessageBox(Query.c_str(),"Confirmation",
		MB_YESNO + MB_DEFBUTTON2 + MB_ICONWARNING) == IDYES)
	{
		TPalm *PalmToRemove = (TPalm *)lbPalms->Items->Objects[lbPalms->ItemIndex];
      PalmToRemove->Destroy();
		PalmToRemove->RemoveFromPalmList = true;
		lbPalms->Items->Delete(lbPalms->ItemIndex);

      TReqInfoSync *Request = new TReqInfoSync;
      try
      {
         Request->BroadcastRequest = false;
         TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
      }
      __finally
      {
         delete Request;
      }
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::FormShow(TObject *Sender)
{
	FormResize(NULL);
	lbPalms->Clear();
	for(int i = 0 ; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
	{
		TPalm *Palm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
		lbPalms->Items->AddObject(Palm->ID.DisplayName,Palm);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::tmRegisterPalmTimer(TObject *Sender)
{
	if(TDeviceRealTerminal::Instance().Registering)
	{
		frmProcessing->Message = "Scanning For Palm " + PalmDisplayName +
										 " 0:" + IntToStr(TDeviceRealTerminal::Instance().RegisteringTimer);
		TDeviceRealTerminal::Instance().RegisteringTimer--;
		if(TDeviceRealTerminal::Instance().RegisteringTimer <= 0)
		{
			frmProcessing->Cancelled = true;
		}
	}
	else
	{
		tmRegisterPalm->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::btnCloseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::btnUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (lbPalms->ItemIndex > 0) lbPalms->ItemIndex--;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::btnDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (lbPalms->Items->Count) lbPalms->ItemIndex++;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPalmRegistration::tbtnReregisterClick(TObject *Sender)
{
   PalmDisplayName = "";
	if(lbPalms->Items->Count == 0)
	{
		MessageBox("Please Select a palm to re-register","Re-register Error",MB_OK & MB_ICONERROR);
		return;
	}

	if(lbPalms->ItemIndex == -1)
	{
		MessageBox("Please Select a palm to re-register","Re-register Error",MB_OK & MB_ICONERROR);
		return;
	}


	TPalm *PalmToReregister = (TPalm *)lbPalms->Items->Objects[lbPalms->ItemIndex];
   PalmDisplayName = PalmToReregister->ID.DisplayName;
	AnsiString Query = "You are about to Re-register "+ PalmDisplayName + ".\n"
							 "All Printer and Course configurations for this device will be kept. \n\n"
							 "Please ensure that no two devices are registed with the same name. \n\n";
	if (Application->MessageBox(Query.c_str(),"Confirmation",
		MB_YESNO + MB_DEFBUTTON2 + MB_ICONWARNING) == IDYES)
	{
      TDeviceRealTerminal::Instance().NewPalmRegisterName = PalmToReregister->ID.Name;
      frmProcessing->Message = "Scanning For Palm " + PalmDisplayName +
                               " 1:00";
      frmProcessing->CanCancel = true;
      frmProcessing->Show();
      try
      {
         TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Palm Licence Reprogramming Activated");
         TDeviceRealTerminal::Instance().Registering = true;
         TDeviceRealTerminal::Instance().Reregistering = true;
         TDeviceRealTerminal::Instance().ReregisteringOldPalmID = PalmToReregister->ID.DeviceID;
         TDeviceRealTerminal::Instance().RegisteringTimer = 59;
      // TODO 5 -o Michael -c Misc: check .
         if(TComms::Instance().Sync == NULL )
         {
            TComms::Instance().Start();
         }
         tmRegisterPalm->Enabled = true;
         while(!frmProcessing->Cancelled && TDeviceRealTerminal::Instance().Registering)
         {
            Application->ProcessMessages();
         }

         if(frmProcessing->Cancelled && TDeviceRealTerminal::Instance().RegisteringTimer == 0)
         {
            MessageBox("Registration operation timed out.\rPlease try again.","Registration",MB_OK & MB_ICONINFORMATION);
         }
         else if(frmProcessing->Cancelled)
         {
            MessageBox("Registration operation cancelled by user.","Registration",MB_OK & MB_ICONINFORMATION);
         }
         else if(!TDeviceRealTerminal::Instance().Registering)
         {
            TReqInfoSync *Request = new TReqInfoSync;
            try
            {
               Request->BroadcastRequest = false;
               TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
            }
            __finally
            {
               delete Request;
            }
            MessageBox("Registration operation completed.\rPalm " +
                                          PalmDisplayName + " registered successfully.","Registration",MB_OK & MB_ICONINFORMATION);
         }
         else
         {
            MessageBox("Registration operation Failed.\rPlease try again.","Registration",MB_OK & MB_ICONINFORMATION);
         }
      }
      __finally
      {
         tmRegisterPalm->Enabled = false;
         TDeviceRealTerminal::Instance().Registering = false;
         TDeviceRealTerminal::Instance().Reregistering = false;
         TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Palm Licence Reprogramming Complete");
         frmProcessing->Close();
         lbPalms->Clear();
         for(int i = 0 ; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
         {
            TPalm *Palm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
            lbPalms->Items->AddObject(Palm->ID.DisplayName,Palm);
         }
      }
   }
}
//---------------------------------------------------------------------------

