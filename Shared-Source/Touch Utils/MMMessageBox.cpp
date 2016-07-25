//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmMessageBox::TfrmMessageBox(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::FormShow(TObject *Sender)
{
	RetVal = 0;
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::tbtnAcceptedClick(TObject *Sender)
{
	RetVal = IDOK;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::tbtnDeclinedClick(TObject *Sender)
{
	RetVal = IDCANCEL;
	Close();
}

void __fastcall TfrmMessageBox::btnOkClick(TObject *Sender)
{
	RetVal = IDOK;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnCancelClick(TObject *Sender)
{
	RetVal = IDCANCEL;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnAbortClick(TObject *Sender)
{
	RetVal = IDABORT;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnRetryClick(TObject *Sender)
{
	RetVal = IDRETRY;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnIgnoreClick(TObject *Sender)
{
	RetVal = IDIGNORE;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnYesClick(TObject *Sender)
{
	RetVal = IDYES;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::btnNoClick(TObject *Sender)
{
	RetVal = IDNO;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageBox::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
	Left				= (Screen->Width - Width) / 2;
	Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
int MessageBox(UnicodeString Text, UnicodeString Caption, int Flags)
{
	int RetVal = 0;
   std::auto_ptr<TfrmMessageBox> (frmMessageBox)(TfrmMessageBox::Create<TfrmMessageBox>(Screen->ActiveForm));
   frmMessageBox->Flags = Flags;
   switch (Flags & 0x0F)
   {
      case MB_ACCEPTDECLINE:
         frmMessageBox->pnlAcceptDecline->Visible		= true;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_ABORTRETRYIGNORE:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= true;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_OK:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= true;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_OKCANCEL:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= true;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_RETRYCANCEL:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= true;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_YESNO:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= true;
         frmMessageBox->pnlYesNoCancel->Visible			= false;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      case MB_YESNOCANCEL:
         frmMessageBox->pnlAcceptDecline->Visible		= false;
         frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
         frmMessageBox->pnlOk->Visible						= false;
         frmMessageBox->pnlOkCancel->Visible				= false;
         frmMessageBox->pnlRetryCancel->Visible			= false;
         frmMessageBox->pnlYesNo->Visible					= false;
         frmMessageBox->pnlYesNoCancel->Visible			= true;
         frmMessageBox->pnlCustom->Visible				= false;
         break;
      default:
         return 0;
   }
   switch (Flags & 0xF0)
   {
      case MB_ICONEXCLAMATION:
         frmMessageBox->imgExclamation->Visible	= true;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= false;
         break;
      case MB_ICONINFORMATION:
         frmMessageBox->imgExclamation->Visible	= true;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= false;
         break;
      case MB_ICONQUESTION:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= true;
         frmMessageBox->imgHand->Visible			= false;
         break;
      case MB_ICONERROR:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= true;
         break;
      default:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= true;
         break;
   }
   int TextHeight;
   std::auto_ptr<TCanvas> MsgBoxCanvas(new TCanvas);
   frmMessageBox->lbeText->Canvas->Font->Name = frmMessageBox->lbeText->Font->Name;//"Arial";
   frmMessageBox->lbeText->Canvas->Font->Size = frmMessageBox->lbeText->Font->Size;//12;
   frmMessageBox->lbeText->Canvas->Font->Style = TFontStyles() << fsBold;
   TextHeight = DrawTextExW(frmMessageBox->lbeText->Canvas->Handle,
                            Text.c_str(),
                            Text.Length(),
                            &TRect(0, 0, frmMessageBox->lbeText->ClientWidth, 0),
                            DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX,
                            NULL);

   if (TextHeight < frmMessageBox->imgQuestion->Height + (frmMessageBox->imgQuestion->Top * 2))
   {
      TextHeight = frmMessageBox->imgQuestion->Height + (frmMessageBox->imgQuestion->Top * 2);
      frmMessageBox->lbeText->AutoSize = false;
      frmMessageBox->lbeText->Height = TextHeight + 4;
      frmMessageBox->lbeText->Width = frmMessageBox->Panel1->Width - frmMessageBox->lbeText->Left - 4;
   }
   else
   {
   frmMessageBox->lbeText->Caption = Text;

      frmMessageBox->lbeText->AutoSize = true;
      frmMessageBox->lbeText->Width = frmMessageBox->Panel1->Width - frmMessageBox->lbeText->Left - 4;
   }
   frmMessageBox->lbeText->Caption = Text;

   frmMessageBox->Bevel1->Top = frmMessageBox->lbeText->Top + frmMessageBox->lbeText->Height + 4;
   frmMessageBox->pnlOkCancel->Top = frmMessageBox->Bevel1->Top + frmMessageBox->Bevel1->Height + 4;
   frmMessageBox->pnlOk->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlRetryCancel->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlAbortRetryIgnore->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlYesNo->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlYesNoCancel->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->ClientHeight = frmMessageBox->pnlOkCancel->Top + frmMessageBox->pnlOkCancel->Height + frmMessageBox->BorderWidth + 8;

   frmMessageBox->Caption = Caption;
   if (frmMessageBox->ShowModal())
   {
      RetVal = frmMessageBox->RetVal;
   }
   else
   RetVal = 0;

   return RetVal;
}


//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
int CustomMessageBox(UnicodeString Text, UnicodeString Caption, int Flags,UnicodeString AffimCaption,UnicodeString CancelCaption)
{
	int RetVal = 0;
   std::auto_ptr<TfrmMessageBox> (frmMessageBox)(TfrmMessageBox::Create<TfrmMessageBox>(Screen->ActiveForm));
   frmMessageBox->Flags = Flags;

   frmMessageBox->pnlCustom->Visible				= true;
   frmMessageBox->tbtnAffim->Caption				= AffimCaption;
   frmMessageBox->tbtnCancel->Caption				= CancelCaption;
   frmMessageBox->pnlAbortRetryIgnore->Visible	= false;
   frmMessageBox->pnlOk->Visible						= false;
   frmMessageBox->pnlOkCancel->Visible				= false;
   frmMessageBox->pnlRetryCancel->Visible			= false;
   frmMessageBox->pnlYesNo->Visible					= false;
   frmMessageBox->pnlYesNoCancel->Visible			= false;
   frmMessageBox->pnlAcceptDecline->Visible		= false;
   switch (Flags & 0xF0)
   {
      case MB_ICONEXCLAMATION:
         frmMessageBox->imgExclamation->Visible	= true;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= false;
         break;
      case MB_ICONINFORMATION:
         frmMessageBox->imgExclamation->Visible	= true;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= false;
		 break;
      case MB_ICONQUESTION:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= true;
         frmMessageBox->imgHand->Visible			= false;
         break;
      case MB_ICONERROR:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= true;
         break;
      default:
         frmMessageBox->imgExclamation->Visible	= false;
         frmMessageBox->imgQuestion->Visible		= false;
         frmMessageBox->imgHand->Visible			= true;
         break;
   }
   int TextHeight;
   std::auto_ptr<TCanvas> MsgBoxCanvas(new TCanvas);
   frmMessageBox->lbeText->Canvas->Font->Name = frmMessageBox->lbeText->Font->Name;//"Arial";
   frmMessageBox->lbeText->Canvas->Font->Size = frmMessageBox->lbeText->Font->Size;//12;
   frmMessageBox->lbeText->Canvas->Font->Style = TFontStyles() << fsBold;
	TextHeight = DrawTextExW(frmMessageBox->lbeText->Canvas->Handle,
							Text.c_str(),
							Text.Length(),
                            &TRect(0, 0, frmMessageBox->lbeText->ClientWidth, 0),
							DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX,
							NULL);
   if (TextHeight < frmMessageBox->imgQuestion->Height + (frmMessageBox->imgQuestion->Top * 2))
   {
      TextHeight = frmMessageBox->imgQuestion->Height + (frmMessageBox->imgQuestion->Top * 2);
   }
   frmMessageBox->lbeText->Height = TextHeight + 4;
   frmMessageBox->Bevel1->Top = frmMessageBox->lbeText->Top + frmMessageBox->lbeText->Height + 4;
   frmMessageBox->pnlOkCancel->Top = frmMessageBox->Bevel1->Top + frmMessageBox->Bevel1->Height + 4;
   frmMessageBox->pnlOk->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlRetryCancel->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlAbortRetryIgnore->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlYesNo->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->pnlYesNoCancel->Top = frmMessageBox->pnlOkCancel->Top;
   frmMessageBox->ClientHeight = frmMessageBox->pnlOkCancel->Top + frmMessageBox->pnlOkCancel->Height + frmMessageBox->BorderWidth + 8;

   frmMessageBox->Caption = Caption;
   frmMessageBox->lbeText->Caption = Text;
   if (frmMessageBox->ShowModal())
   {
      RetVal = frmMessageBox->RetVal;
   }
   else
   RetVal = 0;
   return RetVal;
}
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmMessageBox::FormKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	switch (Flags & 0x0F)
	{
		case MB_ABORTRETRYIGNORE:
			switch(Key)
			{
				case VK_ESCAPE :
					btnAbortClick(NULL);
				break;
				case VK_SPACE	 :
				case '\r' :
					btnRetryClick(NULL);
				break;
			}
			break;
		case MB_OK:
			switch(Key)
			{
				case VK_ESCAPE :
				case VK_SPACE	 :
				case '\r' :
					btnOkClick(NULL);
				break;
			}
			break;
		case MB_OKCANCEL:
			switch(Key)
			{
				case VK_ESCAPE :
					btnCancelClick(NULL);
				break;
				case VK_SPACE	 :
				case '\r' :
					btnOkClick(NULL);
				break;
			}
			break;
		case MB_RETRYCANCEL:
			switch(Key)
			{
				case VK_ESCAPE :
					btnCancelClick(NULL);
				break;
				case VK_SPACE	 :
				case '\r' :
					btnRetryClick(NULL);
				break;
			}
			break;
		case MB_YESNO:
			switch(Key)
			{
				case VK_ESCAPE :
					btnNoClick(NULL);
				break;
				case VK_SPACE	 :
				case '\r' :
					btnYesClick(NULL);
				break;
			}
			break;
		case MB_YESNOCANCEL:
			switch(Key)
			{
				case VK_ESCAPE :
					btnCancelClick(NULL);
				break;
				case VK_SPACE	 :
				case '\r' :
					btnYesClick(NULL);
				break;
			}
			break;
	}
}
//---------------------------------------------------------------------------


