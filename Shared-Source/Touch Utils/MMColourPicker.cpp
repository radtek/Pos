//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MMColourPicker.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrid"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

__fastcall TfrmColourPicker::TfrmColourPicker(TComponent* Owner)
	: TZForm(Owner)
{
	SelectedColour = clBlack;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmColourPicker::FormShow(TObject *Sender)
{
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmColourPicker::tbtnOtherColourMouseClick(TObject *Sender)
{
	ColorDialog->Execute();
   SelectedColour = ColorDialog->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmColourPicker::FormResize(TObject *Sender)
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
}

void __fastcall TfrmColourPicker::CColorGridChange(TObject *Sender)
{
   SelectedColour = CColorGrid->ForegroundColor;
}
//---------------------------------------------------------------------------

void __fastcall TfrmColourPicker::TouchBtn1MouseClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmColourPicker::TouchBtn3MouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmColourPicker::CColorGridClick(TObject *Sender)
{
   SelectedColour = CColorGrid->ForegroundColor;
}
//---------------------------------------------------------------------------

