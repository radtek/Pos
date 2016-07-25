//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PalmRegistration.h"
#include "enum.h"
#include "Palm.h"
#include "MMTouchKeyboard.h"
#include "Processing.h"
#include "Comms.h"
#include "MMMessageBox.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmPalmRegistration *frmPalmRegistration;
//---------------------------------------------------------------------------
__fastcall TfrmPalmRegistration::TfrmPalmRegistration(TComponent* Owner)
	: TForm(Owner)
{
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

	UnicodeString Query = "Are you sure you wish to delete this Palm.\n"
							 "All Printer and Course configurations for this device will be lost. \n\n"
							 "If this device is currently active, its printing configurations will \n"
							 "be removed and it will become unregistered. \n\n";
	if (MessageBox(Query.c_str(),_T("Confirmation"), MB_YESNO + MB_DEFBUTTON2 + MB_ICONWARNING) == IDYES)
	{
		TPalm *PalmToRemove = (TPalm *)TDeviceRealPalms::Instance().PalmList->Items[lbPalms->ItemIndex];
		Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
		DBTransaction.StartTransaction();
		PalmToRemove->Destroy(DBTransaction);
		DBTransaction.Commit();
		TDeviceRealPalms::Instance().PalmList->Remove(PalmToRemove);
		lbPalms->Items->Delete(lbPalms->ItemIndex);
		delete PalmToRemove;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPalmRegistration::FormShow(TObject *Sender)
{
	FormResize(NULL);
	LastRegSyncNumber = -1;
	UpdateDisplay();
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
	randomize();
	TDeviceRealPalms::Instance().RegistrationPIN = IntToStr(random(10)) + IntToStr(random(10)) + IntToStr(random(10)) + IntToStr(random(10));
	TDeviceRealPalms::Instance().RegExpire->Enabled = true;
	MessageBox("Your Current Registration PIN is " + TDeviceRealPalms::Instance().RegistrationPIN,"Registration PIN",MB_OK & MB_ICONINFORMATION);

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void TfrmPalmRegistration::UpdateDisplay()
{
	if(LastRegSyncNumber != TDeviceRealPalms::Instance().RegSyncNumber)
	{
		LastRegSyncNumber = TDeviceRealPalms::Instance().RegSyncNumber;
		lbPalms->Clear();
		for(int i = 0 ; i < TDeviceRealPalms::Instance().PalmList->Count; i++)
		{
			TPalm *Palm = (TPalm *)TDeviceRealPalms::Instance().PalmList->Items[i];
			lbPalms->Items->Add(Palm->ID.Name);
		}
	}
}
