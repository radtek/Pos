//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchKeyboard"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTouchKeyboard::TfrmTouchKeyboard(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::FormCreate(TObject *Sender)
{
   AllowCarriageReturn = true;
   CloseOnDoubleCarriageReturn = AllowCarriageReturn;
   StartWithShiftDown = false;
   MustHaveValue = false;
   KeyboardText = "";
   MaxLength = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::FormShow(TObject *Sender)
{
   FormResize(this);
   TouchKeyboard->Shift = StartWithShiftDown;
   TouchKeyboard->CapsLock = CapsDown;
   memText->Text = KeyboardText;
   memText->SelStart = memText->Text.Length();
   memText->SetFocus();
   memText->MaxLength = MaxLength;
   lbeCaption->Caption = Caption;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TfrmTouchKeyboard::FormResize(TObject *Sender)
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
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::btnDoneClick(TObject *Sender)
{
    if (AllowCarriageReturn == false)
    {
        memText->Text = StringReplace(memText->Text,UnicodeString("\r"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
	    memText->Text = StringReplace(memText->Text,UnicodeString("\n"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
    }
    KeyboardText = memText->Text;
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::memTextKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN && AllowCarriageReturn == false)
   {
		KeyboardText = memText->Text;
		ModalResult = mrOk;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   StartWithShiftDown = true;
   MustHaveValue = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::CancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchKeyboard::ClearClick(TObject *Sender)
{
	memText->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   if(MustHaveValue && memText->Text == "")
	{
		if(ModalResult == mrCancel)
		{
			CanClose = true;
			return;
		}
      MessageBox("A value must be suppiled", "Please enter a value", MB_OK + MB_ICONINFORMATION);
      CanClose = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::memTextKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (Key == VK_RETURN && AllowCarriageReturn == true)
   {
      if(CloseOnDoubleCarriageReturn)
      {
         int LastLine = memText->Lines->Count-1;
		 	int CurPos = memText->SelStart;

         // Find the Line the Cursor is on.
         int i = 0;
         for (; CurPos > 0 && i < memText->Lines->Count; i ++)
         {
           CurPos -= memText->Lines->Strings[i].Length();
         }
         int CursorLine = i-1; // Zero Indexed.

         if(LastLine == CursorLine &&
            memText->Lines->Strings[LastLine] == "")
         {
            memText->Lines->Delete(LastLine);
            KeyboardText = memText->Text;
            ModalResult = mrOk;
		 	}
      }
   }
   else if (Key == VK_RETURN && AllowCarriageReturn == false)
   {
      KeyboardText = memText->Text;
		KeyboardText = StringReplace(KeyboardText,UnicodeString("\r"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
		KeyboardText = StringReplace(KeyboardText,UnicodeString("\n"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
      ModalResult = mrOk;
   }
}

//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::TouchKeyboardClick(TObject *Sender, wchar_t Char)
{
	PostMessage(memText->Handle, WM_CHAR, Char, 0);
	memText->SetFocus();


/*	if (((TPanel *)Sender)->Tag == 251)
   {
		PostMessage(memText->Handle, WM_CHAR, '\t', 0);
      memText->SetFocus();
   }
	// Print the backspace
	else if (((TPanel *)Sender)->Tag == 252)
   {
      PostMessage(memText->Handle, WM_CHAR, '\b', 0);
      memText->SetFocus();
   }
	// Print the Return
	else if (((TPanel *)Sender)->Tag == 253)
   {
      if (!AllowCarriageReturn)
      {
			KeyboardText = memText->Text;
         ModalResult = mrOk;
      }
      else
      {
         PostMessage(memText->Handle, WM_CHAR, '\r', 0);
         memText->SetFocus();
      }
   }
   // Print the space
	else if (((TPanel *)Sender)->Tag == 254)
   {
      PostMessage(memText->Handle, WM_CHAR, ' ', 0);
      memText->SetFocus();
   }
   // Print the character
	else if (((TPanel *)Sender)->Tag == 255)
   {
      Word Key;
      Key = ((TPanel *)Sender)->Caption.c_str()[0];
      PostMessage(memText->Handle, WM_CHAR, Key, 0);
      memText->SetFocus();
   }
   // Print the number / others
   else if (((TComponent *)Sender)->Tag != 0 && ((TComponent *)Sender)->Tag != 255)
   {
		PostMessage(memText->Handle, WM_CHAR,
      		ButtonChar[((TComponent *)Sender)->Tag-1][ShiftDown?0:1], 0);
   }      */

}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::TouchBtn4MouseClick(TObject *Sender)
{
  AddStringToText("@gmail.com");
}
//---------------------------------------------------------------------------

void __fastcall TfrmTouchKeyboard::TouchBtn5MouseClick(TObject *Sender)
{
  AddStringToText("com.au");
}
//---------------------------------------------------------------------------
void TfrmTouchKeyboard::AddStringToText(AnsiString stringToAdd)
{
   memText->Text = memText->Text +stringToAdd;
  KeyboardText = memText->Text ;
  memText->SelStart = KeyboardText.Length();
}

//---------------------------------------------------------------------------

