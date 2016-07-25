//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "pause.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPause *frmPause;
//---------------------------------------------------------------------------
__fastcall TfrmPause::TfrmPause(TComponent* Owner)
  : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPause::btnExitClick(TObject *Sender)
{
  Cancel = true;
}
//---------------------------------------------------------------------------
void TfrmPause::incbar()
{
  Progress->Position += 1;
  if(Progress->Position >= frmPause->Progress->Max)
  {
     Progress->Position = 0;
  }
}

void TfrmPause::Reset()
{
  Progress->Position = 0;
}
void __fastcall TfrmPause::FormShow(TObject *Sender)
{
  Cancel = false;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmPause::FormHide(TObject *Sender)
{
      frmPause->Progress->Max = 100;
		frmPause->Progress->Position = 0;
}
//---------------------------------------------------------------------------

