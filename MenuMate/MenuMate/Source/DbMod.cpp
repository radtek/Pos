//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "DbMod.h"
#include <stdlib.h>
#include <Classes.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>

#ifdef PalmMate
#include "Palm.h"
#endif

#include "Version.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pngimage"
#pragma resource "*.dfm"

TfrmDBMod *frmDBMod;

//---------------------------------------------------------------------------
__fastcall TfrmDBMod::TfrmDBMod(TComponent* Owner)
	: TZForm(Owner)
{
//	lbeVersion->Caption = GetFileVersion();
    lbeVersion->Caption = GetFileVersionString();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDBMod::FormCreate(TObject *Sender)
{
//	ClientWidth = imgMenuMate->Width;
//	ClientHeight = imgMenuMate->Height;
	Left = (Screen->Width / 2) - (Width/2);
	Top = (Screen->Height / 2) - (Height/2);
}
//---------------------------------------------------------------------------

void TfrmDBMod::Init()
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmDBMod::imgMenuMateDblClick(TObject *Sender)
{
	CancelRebuild = CanCancel;
}
//---------------------------------------------------------------------------

void TfrmDBMod::SetCaption(UnicodeString Caption)
{
   Label1->Caption = Caption;
	Label1->Update();
}

void TfrmDBMod::SetRegCaption(UnicodeString Caption)
{
   lbRegInfo->Caption = Caption;
   lbRegInfo->Update();
/*   lbRegInfo->Left = ClientWidth - lbRegInfo->Width - 5;
   lbRegInfo->Top = imgMenuMate->Height - lbRegInfo->Height  - 5;*/
}

void __fastcall TfrmDBMod::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key == VK_ESCAPE || Key == VK_RETURN)
   {
   	CancelRebuild = CanCancel;
   }
}

