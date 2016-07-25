#include <vcl\vcl.h>
#pragma hdrstop

#include "Message.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMsgForm *MsgForm;
//---------------------------------------------------------------------------
__fastcall TMsgForm::TMsgForm(TComponent *Owner) : TForm(Owner)
{
  ProgressBar2 = new TProgressBar( StatusBar1 );	// Parent will delete it.

	ProgressBar2->Parent   = StatusBar1;
	ProgressBar2->Top      = 2;
	ProgressBar2->Left     = StatusBar1->Left + StatusBar1->Panels->Items[0]->Width +
									 StatusBar1->Panels->Items[1]->Width + 2;
	ProgressBar2->Height   = StatusBar1->Height - 2;
	ProgressBar2->Min      = 0;
	ProgressBar2->Max      = 100;
	ProgressBar2->Position = 0;
#  if __BORLANDC__ == 1344		// BCB4
	ProgressBar1->Smooth   = true;
	ProgressBar2->Smooth   = true;
#  endif
	FormInitialWidth = MsgForm->Width;
  SendMessage(RichEdit1->Handle, EM_EXLIMITTEXT, 0, 0x7FFFFFFD);
}
//---------------------------------------------------------------------------
void __fastcall TMsgForm::DismissButClick( TObject* /*Sender*/ )
{
	Hide();
}
//---------------------------------------------------------------------------
void __fastcall TMsgForm::CancelButClick( TObject* /*Sender*/ )
{
	if ( MainForm->ZipBuilder1->ZipBusy || MainForm->ZipBuilder1->UnzBusy )
		MainForm->ZipBuilder1->Cancel = true;
	else
		Hide(); // Nothing to cancel - assume user wants to close msg window.
}
//---------------------------------------------------------------------------
void __fastcall TMsgForm::FormResize( TObject* /*Sender*/ )
{
	ProgressBar2->Width = StatusBar1->Width - StatusBar1->Panels->Items[0]->Width -
								 StatusBar1->Panels->Items[1]->Width - 18;
	ProgressBar1->Width = 177 + (MsgForm->Width - FormInitialWidth);
}

